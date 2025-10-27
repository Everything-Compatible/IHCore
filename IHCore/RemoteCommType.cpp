#include "ECInterprocess.h"
#include "RemoteComponents.h"
#include "ECDbgConsole.h"
#include "Global.h"
#include "RemoteCommType.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

bool ServiceProcessManager::StartServiceProcess(std::u8string_view CommandLine, bool RedirectConsole, std::u8string& NewLocation)
{
	if (NewLocation.empty())return false;
	if (CommandLine.empty())return false;

	STARTUPINFOW startup_info = { 0 };
	startup_info.cb = sizeof(startup_info);

	SECURITY_ATTRIBUTES security_attrs = { 0 };
	security_attrs.nLength = sizeof(SECURITY_ATTRIBUTES);
	security_attrs.bInheritHandle = TRUE;  // 允许句柄继承

	HANDLE child_stdout_rd = NULL;
	HANDLE child_stdout_wr = NULL;
	HANDLE child_stdin_rd = NULL;
	HANDLE child_stdin_wr = NULL;

	{
		// 创建管道用于重定向子进程的stdout
		if (!CreatePipe(&child_stdout_rd, &child_stdout_wr, &security_attrs, 0)) {
			return false;
		}

		// 确保读句柄不被子进程继承
		if (!SetHandleInformation(child_stdout_rd, HANDLE_FLAG_INHERIT, 0)) {
			CloseHandle(child_stdout_rd);
			CloseHandle(child_stdout_wr);
			return false;
		}

		// 创建管道用于重定向子进程的stdin
		if (!CreatePipe(&child_stdin_rd, &child_stdin_wr, &security_attrs, 0)) {
			CloseHandle(child_stdout_rd);
			CloseHandle(child_stdout_wr);
			return false;
		}

		// 确保写句柄不被子进程继承
		if (!SetHandleInformation(child_stdin_wr, HANDLE_FLAG_INHERIT, 0)) {
			CloseHandle(child_stdout_rd);
			CloseHandle(child_stdout_wr);
			CloseHandle(child_stdin_rd);
			CloseHandle(child_stdin_wr);
			return false;
		}

		// 设置启动信息以使用重定向的句柄
		startup_info.dwFlags |= STARTF_USESTDHANDLES;
		startup_info.hStdInput = child_stdin_rd;
		startup_info.hStdOutput = child_stdout_wr;
		startup_info.hStdError = child_stdout_wr;  // 将stderr也重定向到stdout

		console_redirected = true;
	}

	auto CmdLineW = UTF8toUnicode(std::string(~CommandLine));

	// 创建进程
	BOOL success = CreateProcessW(
		nullptr,                    // 应用程序名（在命令行中指定）
		CmdLineW.data(),			// 命令行
		nullptr,                    // 进程安全属性
		nullptr,                    // 线程安全属性
		TRUE,						// 是否继承句柄
		RedirectConsole ? 0 : CREATE_NO_WINDOW, // 创建标志（不重定向时隐藏窗口）
		nullptr,                    // 环境变量
		nullptr,                    // 当前目录
		&startup_info,              // 启动信息
		&process_info // 进程信息
	);

	if (!success) {
		CloseHandle(child_stdout_rd);
		CloseHandle(child_stdout_wr);
		CloseHandle(child_stdin_rd);
		CloseHandle(child_stdin_wr);
		return false;
	}

	// 关闭子进程端的句柄（我们使用父进程端的句柄）
	CloseHandle(child_stdin_rd);
	CloseHandle(child_stdout_wr);

	// 等待位置重置
	if (NewLocation == u8"DYNAMIC")
	{
		WaitForLocationReset(child_stdout_rd, NewLocation, RedirectConsole);
	}
	else if(NewLocation.back() == u8'$')
	{
		NewLocation.pop_back();
		auto ProcID = GetProcessId(process_info.hProcess);
		char idbuf[32];
		sprintf_s(idbuf, "0x%08X", ProcID);
		NewLocation += conv idbuf;
	}

	if (RedirectConsole)
	{
		// 启动I/O重定向线程
		StartConsoleRedirection(
			child_stdout_rd,  // 从子进程读取stdout
			child_stdin_wr    // 向子进程写入stdin
		);
	}
	else
	{
		//结束连接
		CloseHandle(child_stdout_rd);
		CloseHandle(child_stdin_wr);
	}

	return true;
}

void ServiceProcessManager::WaitForLocationReset(HANDLE child_stdout_rd, std::u8string& NewLocation, bool RedirectConsole)
{
	// 1. 循环读取 child_stdout_rd 的内容，缓冲到字符串。
	// 2. 查找 "Location:" 前缀，若找到则提取引号内的新位置字符串。
	// 3. 设置 NewLocation 为新值并返回。
	// Target -  Location:"xxx"

	const DWORD buffer_size = 4096;
	std::string buffer_accum;
	CHAR buffer[buffer_size];
	DWORD bytes_read;

	while (true) {
		BOOL success = ReadFile(child_stdout_rd, buffer, buffer_size - 1, &bytes_read, nullptr);
		if (!success || bytes_read == 0) {
			break; // 读取失败或管道关闭
		}
		buffer[bytes_read] = '\0';
		buffer_accum.append(buffer, bytes_read);

		// 查找 Location:"xxx"
		size_t loc_pos = buffer_accum.find("Location:\"");
		if (loc_pos != std::string::npos) {
			size_t start = loc_pos + strlen("Location:\"");
			size_t end = buffer_accum.find('"', start);
			if (end != std::string::npos) {
				std::string location_str = buffer_accum.substr(start, end - start);
				NewLocation = std::u8string(location_str.begin(), location_str.end());
				if (RedirectConsole)
				{
					//输出所有接收到的，在这之后的内容
					size_t output_start = end + 1;
					if (output_start < buffer_accum.size()) {
						std::string remaining = buffer_accum.substr(output_start);
						if (!remaining.empty()) {
							DWORD bytes_written;
							HANDLE console_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
							WriteFile(console_stdout, remaining.data(), static_cast<DWORD>(remaining.size()), &bytes_written, nullptr);
						}
					}
				}
				break;
			}
		}

		// 防止缓冲区无限增长
		if (buffer_accum.size() > 8192) {
			buffer_accum.erase(0, buffer_accum.size() - 4096);
		}
	}
}

void ServiceProcessManager::StartConsoleRedirection(HANDLE child_stdout_rd, HANDLE child_stdin_wr)
{
	// 创建线程来处理子进程的输出（从子进程读取并显示在控制台）
	std::thread output_thread([this, child_stdout_rd]() {
		const DWORD buffer_size = 4096;
		CHAR buffer[buffer_size];
		DWORD bytes_read;

		IPC_Log("[EC] ServiceProcessManager: Started console redirection for %s\n",
			conv ServiceProcessComm::GetProcessNameByComm(this).c_str());

		while (true) {
			BOOL success = ReadFile(child_stdout_rd, buffer, buffer_size - 1, &bytes_read, nullptr);

			if (!success || bytes_read == 0) {
				// 读取失败或子进程关闭了输出
				break;
			}

			// 确保字符串以null结尾
			buffer[bytes_read] = '\0';

			auto U8 = ANSItoUTF8(buffer);

			LogCommFlow(u8" STDOUT of " + ServiceProcessComm::GetProcessNameByComm(this) + u8" :\n " + ~U8);

			// 输出到控制台
			DWORD bytes_written;
			HANDLE console_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
			WriteFile(console_stdout, U8.c_str(), U8.size(), &bytes_written, nullptr);
		}

		IPC_Log("[EC] ServiceProcessManager: Console redirection ended for %s\n",
			conv ServiceProcessComm::GetProcessNameByComm(this).c_str());

		CloseHandle(child_stdout_rd);
		});

	output_thread.detach();


	std::thread input_thread([this, child_stdin_wr]() {
		
		//read from this->InputBuffer and write to child_stdin_wr
		while (true) {

			if (this->InputBuffer.Empty())
			{
				// 等待一段时间再检查
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				continue;
			}

			std::u8string input_line = this->InputBuffer.Pop();
			if (input_line.empty()) {
				// 空行表示结束输入
				break;
			}
			// 写入子进程的stdin
			DWORD bytes_written;
			std::string input_str(input_line.begin(), input_line.end());
			input_str.push_back('\n'); // 添加换行符
			BOOL result = WriteFile(child_stdin_wr, input_str.data(), static_cast<DWORD>(input_str.size()), &bytes_written, nullptr);

			if (!result || bytes_written != input_str.size()) {
				// 写入失败
				break;
			}
		}

		CloseHandle(child_stdin_wr);
		
	});

	// 分离线程，让它们独立运行
	
	input_thread.detach();
}

void ServiceProcessManager::StopServiceProcess()
{
	if (process_info.hProcess) {

		TerminateProcess(process_info.hProcess, 0);

		WaitForSingleObject(process_info.hProcess, 5000);

		CloseHandle(process_info.hProcess);
		CloseHandle(process_info.hThread);

		process_info.hProcess = NULL;
		process_info.hThread = NULL;
	}
}

bool ServiceProcessManager::IsServiceProcessRunning()
{
	if (!process_info.hProcess) {
		return false;
	}

	DWORD exit_code;
	if (GetExitCodeProcess(process_info.hProcess, &exit_code)) {
		return exit_code == STILL_ACTIVE;
	}

	return false;
}

//----------------------------------------------

namespace ServiceProcessComm
{
	thread_local Target LocalCommTarget = Target::Host;
	thread_local std::u8string LocalCommTargetName;
	thread_local bool IsCurrentThreadOnConsole = false;

	struct GlobalCommTarget
	{
		Target CommTarget = Target::Host;
		std::u8string CommTargetName;
	};
	InfoStack< GlobalCommTarget> CommTargets;


	std::unordered_map<std::u8string, ServiceProcessInfo*> IOInfo;
	std::unordered_map< ServiceProcessInfo*, std::u8string> IOInfoRev;
	std::mutex IOInfoMutex;
	

	void RegisterProcessComm(const std::u8string& Name, ServiceProcessInfo& Info)
	{
		std::lock_guard <std::mutex> lock(IOInfoMutex);
		IOInfo[Name] = &Info;
		IOInfoRev[&Info] = Name;
	}

	void UnregisterProcessComm(const std::u8string& Name)
	{
		std::lock_guard <std::mutex> lock(IOInfoMutex);
		auto it = IOInfo.find(Name);
		if (it != IOInfo.end())
		{
			it->second->InputBuffer.Clear();
			IOInfoRev.erase(it->second);
			IOInfo.erase(it);
		}
	}

	void UnregisterProcessComm(ServiceProcessInfo& Info)
	{
		std::lock_guard <std::mutex> lock(IOInfoMutex);
		auto it = IOInfoRev.find(&Info);
		if (it != IOInfoRev.end())
		{
			Info.InputBuffer.Clear();
			IOInfo.erase(it->second);
			IOInfoRev.erase(it);
		}
	}

	std::u8string GetProcessNameByComm(ServiceProcessInfo* Info)
	{
		std::lock_guard <std::mutex> lock(IOInfoMutex);
		auto it = IOInfoRev.find(Info);
		return it != IOInfoRev.end() ? it->second : u8"";
	}

	ServiceProcessInfo* GetProcessCommByName(const std::u8string& Name)
	{
		std::lock_guard <std::mutex> lock(IOInfoMutex);
		auto it = IOInfo.find(Name);
		return it != IOInfo.end() ? it->second : nullptr;
	}

	void PushCommTarget(Target t, const std::u8string& Name)
	{
		CommTargets.Push({ t, Name });
	}

	void PopCommTarget()
	{
		if (!CommTargets.Empty())
		{
			CommTargets.Pop();
		}
	}

	void SetLocalCommTarget(Target t, const std::u8string& Name)
	{
		LocalCommTarget = t;
		LocalCommTargetName = Name;
	}

	void SetCommTargetAsLocal()
	{
		PushCommTarget(LocalCommTarget, LocalCommTargetName);
	}

	std::unique_ptr<std::jthread> CommSession;

	std::u8string PeekStdIn()
	{
		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
		{
			return u8"";
		}

		//非阻塞的ReadFile
		//PeekConsoleInput + ReadFile

		DWORD events = 0;
		if (!GetNumberOfConsoleInputEvents(hStdin, &events) || events == 0)
		{
			return u8"";
		}

		INPUT_RECORD input_record;
		DWORD records_read = 0;
		std::u8string result;

		if (PeekConsoleInputW(hStdin, &input_record, 1, &records_read) && records_read > 0)
		{
			//确认是否真的有内容
			bool HasContent = false;

			if (input_record.EventType == KEY_EVENT &&
				input_record.Event.KeyEvent.bKeyDown)
			{
				WCHAR ch = input_record.Event.KeyEvent.uChar.UnicodeChar;
				// 检查是否是可打印字符或控制字符
				if (ch != 0)
				{
					HasContent = true;
				}
			}

			if (!HasContent)
			{
				return u8"";
			}

			const size_t BUF_SIZE = 8192;
			auto buffer = std::make_unique<wchar_t[]>(BUF_SIZE);
			DWORD bytesRead = 0;

			auto rres = ReadConsoleW(hStdin, buffer.get(), BUF_SIZE - 1, &bytesRead, NULL);
			if (rres && bytesRead > 0)
			{
				buffer[bytesRead] = 0;
				std::wstring ws = buffer.get();
				return ~UnicodetoUTF8(ws);
			}

			return result;
		}
		else return u8"";
	}

	void CommSessionUpdate()
	{
		bool empty = CommTargets.Empty();
		GlobalCommTarget tg{ Target::Host, u8"" };
		if (!empty)
		{
			tg = CommTargets.Pop();
			CommTargets.Push(tg);
		}

		switch (tg.CommTarget)
		{
		case Target::Host:
			//什么也不做，交由主线程处理
			//不会向子进程发送数据
			break;
		case Target::Service:
		{
			auto it = IOInfo.find(tg.CommTargetName);
			if (it != IOInfo.end())
			{
				//Peek Stdin
				//if there is, read and send to the target
				auto Input = PeekStdIn();
				if (!Input.empty())
				{
					it->second->InputBuffer.Push(std::move(Input));
				}
			}
			break;
		}
		case Target::Broadcast:
		{
			auto Input = PeekStdIn();
			if (!Input.empty())
			{
				for (auto& [_, info] : IOInfo)
				{
					info->InputBuffer.Push(Input);
				}
			}
			break;
		}
		};
	}

	void BeginCommSession()
	{
		CommSession.reset(new std::jthread([](std::stop_token stoken) {
			while (!stoken.stop_requested())
			{
				CommSessionUpdate();
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
			}));
	}

	void EndCommSession()
	{
		if (CommSession)
		{
			CommSession->get_stop_source().request_stop();
			CommSession->join();
			CommSession.reset();
		}

		for (auto& [_, info] : IOInfo)
		{
			info->InputBuffer.Clear();
		}
	}
}

//----------------------------------------------

bool RemoteComm_NamedPipe::BaseConnect(std::u8string_view RegName, std::u8string_view CmdLine, std::u8string& Location)
{
	Connected = false;

	if (!Proc.StartServiceProcess(CmdLine, ECDebug::HasConsole(), Location))
		return false;

	ServiceProcessComm::RegisterProcessComm(std::u8string(RegName), Proc);

	auto PipeName = UTF8toUnicode(std::string(~Location));

	for (int retry = 0; retry < RemoteComponentMaxRetry; ++retry)
	{
		pipe_handle_ = CreateFileW(
			PipeName.c_str(),			// 管道名称
			GENERIC_READ | GENERIC_WRITE, // 读写权限
			0,                          // 不共享
			nullptr,                    // 默认安全属性
			OPEN_EXISTING,              // 打开已存在的管道
			FILE_ATTRIBUTE_NORMAL,      // 普通属性
			nullptr);                   // 无模板文件

		if (pipe_handle_ != INVALID_HANDLE_VALUE) {
			// 设置管道为消息模式
			DWORD mode = PIPE_READMODE_MESSAGE;
			if (SetNamedPipeHandleState(pipe_handle_, &mode, nullptr, nullptr)) {
				read_buffer_.resize(BUFFER_SIZE);
				Connected = true;
				return true;
			}
			else {
				// 设置模式失败，关闭句柄
				CloseHandle(pipe_handle_);
				pipe_handle_ = INVALID_HANDLE_VALUE;
			}
		}

		// 等待后重试
		std::this_thread::sleep_for(std::chrono::milliseconds(RemoteComponentRetryInterval));

	}

	return false;
}

void RemoteComm_NamedPipe::BaseDisconnect()
{
	//Causing Crashes on process exit
	//ServiceProcessComm::UnregisterProcessComm(Proc);

	if (pipe_handle_ != INVALID_HANDLE_VALUE) {
		CloseHandle(pipe_handle_);
		pipe_handle_ = INVALID_HANDLE_VALUE;
	}

	Connected = false;

	Proc.StopServiceProcess();
}

std::u8string RemoteComm_NamedPipe::ReadString()
{
	if (!Proc.IsServiceProcessRunning()) return u8"";

	if (!Connected || pipe_handle_ == INVALID_HANDLE_VALUE) return u8"";

	DWORD bytes_read = 0;
	BOOL success = ReadFile(
		pipe_handle_,                   // 管道句柄
		read_buffer_.data(),            // 缓冲区
		static_cast<DWORD>(read_buffer_.size()), // 缓冲区大小
		&bytes_read,                    // 实际读取的字节数
		nullptr);                       // 不重叠

	if (success && bytes_read > 0) {
		return std::u8string(reinterpret_cast<char8_t*>(read_buffer_.data()), bytes_read);
	}

	// 读取失败，检查是否连接断开
	if (GetLastError() == ERROR_BROKEN_PIPE) {
		Connected = false;
		return u8"";
	}

	return u8"";
}

void RemoteComm_NamedPipe::WriteString(std::u8string_view Str)
{
	if (!Proc.IsServiceProcessRunning()) return;

	if (!Connected || pipe_handle_ == INVALID_HANDLE_VALUE) return;

	DWORD bytes_written = 0;
	BOOL success = WriteFile(
		pipe_handle_,                   // 管道句柄
		Str.data(),                     // 数据
		static_cast<DWORD>(Str.length()), // 数据长度
		&bytes_written,                 // 实际写入的字节数
		nullptr);                       // 不重叠

	// 检查写入是否完整
	if (!success || bytes_written != Str.length()) {
		// 写入失败，检查是否连接断开
		if (GetLastError() == ERROR_BROKEN_PIPE) {
			Connected = false;
		}
	}
}

bool RemoteComm_NamedPipe::HasStringToRead()
{
	//Skip Service Running Check
	if (!Connected || pipe_handle_ == INVALID_HANDLE_VALUE) return false;

	DWORD bytes_available = 0;
	DWORD bytes_left = 0;

	if (PeekNamedPipe(
		pipe_handle_,                   // 管道句柄
		nullptr,                        // 不读取数据
		0,                              // 缓冲区大小为0
		nullptr,                        // 不返回读取的字节数
		&bytes_available,               // 可用的字节数
		&bytes_left)) {                 // 消息中剩余的字节数

		return bytes_available > 0;
	}

	// PeekNamedPipe 失败，可能连接已断开
	if (GetLastError() == ERROR_BROKEN_PIPE) {
		Connected = false;
	}

	return false;
}

//----------------------------------------------

bool WSAHasInit{ false };

bool WSAGlobalInit()
{
	if (WSAHasInit) return true;

	WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0) {
		return false;
	}

	WSAHasInit = true;

	return true;
}

void WSAGlobalUninit()
{
	if (WSAHasInit)
		WSACleanup();
}

bool RemoteComm_TCP::ParseAddress(std::u8string_view Location, std::string& host, int& port) {
	std::string location_str(conv Location.data(), Location.length());

	size_t colon_pos = location_str.find(':');
	if (colon_pos == std::string::npos) {
		return false;
	}

	host = location_str.substr(0, colon_pos);
	std::string port_str = location_str.substr(colon_pos + 1);

	try {
		port = std::stoi(port_str);
	}
	catch (...) {
		return false;
	}

	return true;
}

bool RemoteComm_TCP::ConnectToServer(const std::string& host, int port)
{
	socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_ == INVALID_SOCKET) {
		return false;
	}

	// 设置地址信息
	sockaddr_in server_addr = { 0 };
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(static_cast<u_short>(port));

	// 解析主机名
	if (host == "localhost" || host.empty()) {
		server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	}
	else {
		inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);
		if (server_addr.sin_addr.s_addr == INADDR_NONE) {
			// 尝试通过主机名解析
			/*
			auto host_entry = gethostbyname(host.c_str());
			if (host_entry) {
				server_addr.sin_addr.s_addr = *reinterpret_cast<unsigned long*>(host_entry->h_addr_list[0]);
			}
			else {
				closesocket(socket_);
				socket_ = INVALID_SOCKET;
				return false;
			}
			*/
			struct addrinfo hints = { 0 };
			struct addrinfo* result = nullptr;

			hints.ai_family = AF_UNSPEC;    // 支持IPv4和IPv6
			hints.ai_socktype = SOCK_STREAM;

			int error = getaddrinfo(host.c_str(), nullptr, &hints, &result);
			if (error == 0 && result != nullptr) {
				// 查找适合的地址（优先IPv4）
				struct addrinfo* ptr = result;
				while (ptr != nullptr) {
					if (ptr->ai_family == AF_INET) {
						struct sockaddr_in* addr_in = (struct sockaddr_in*)ptr->ai_addr;
						server_addr.sin_addr = addr_in->sin_addr;
						break;
					}
					ptr = ptr->ai_next;
				}
				freeaddrinfo(result);

				if (ptr == nullptr) {
					closesocket(socket_);
					socket_ = INVALID_SOCKET;
					return false;
				}
			}
			else {
				closesocket(socket_);
				socket_ = INVALID_SOCKET;
				return false;
			}
		}
	}

	// 尝试连接，重试几次
	for (int retry = 0; retry < RemoteComponentMaxRetry; ++retry) {
		int result = connect(socket_,
			reinterpret_cast<sockaddr*>(&server_addr),
			sizeof(server_addr));

		if (result == 0) {
			Connected = true;
			read_buffer_.resize(BUFFER_SIZE);

			// 设置socket为非阻塞模式以便HasStringToRead可以工作
			u_long mode = 1; // 1表示非阻塞
			ioctlsocket(socket_, FIONBIO, &mode);
			return true;
		}

		// 等待后重试
		std::this_thread::sleep_for(std::chrono::milliseconds(RemoteComponentRetryInterval));
	}

	// 连接失败，清理
	closesocket(socket_);
	socket_ = INVALID_SOCKET;
	return false;
}

bool RemoteComm_TCP::BaseConnect(std::u8string_view RegName, std::u8string_view CmdLine, std::u8string& Location)
{
	Connected = false;

	if (!Proc.StartServiceProcess(CmdLine, ECDebug::HasConsole(), Location))
		return false;

	ServiceProcessComm::RegisterProcessComm(std::u8string(RegName), Proc);

	WSAGlobalInit();
	

	// 解析地址和端口
	std::string host;
	int port;
	if (!ParseAddress(Location, host, port)) {
		return false;
	}

	// 等待服务进程启动并连接
	return ConnectToServer(host, port);
}

void RemoteComm_TCP::BaseDisconnect()
{
	ServiceProcessComm::UnregisterProcessComm(Proc);

	if (socket_ != INVALID_SOCKET) {
		closesocket(socket_);
		socket_ = INVALID_SOCKET;
	}

	Connected = false;

	Proc.StopServiceProcess();
}

std::u8string RemoteComm_TCP::ReadString()
{
	if (!Proc.IsServiceProcessRunning()) return u8"";

	if (!Connected || socket_ == INVALID_SOCKET) return u8"";

	// 临时设置为阻塞模式进行读取
	u_long mode = 0; // 0表示阻塞
	ioctlsocket(socket_, FIONBIO, &mode);

	int bytes_received = recv(socket_,
		reinterpret_cast<char*>(read_buffer_.data()),
		static_cast<int>(read_buffer_.size()), 0);

	// 恢复为非阻塞模式
	mode = 1;
	ioctlsocket(socket_, FIONBIO, &mode);

	if (bytes_received > 0) {
		return std::u8string(read_buffer_.data(), bytes_received);
	}
	else if (bytes_received == 0) {
		Connected = false;
	}
	else {
		// 错误处理
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK) {
			Connected = false;
		}
	}

	return u8"";
}

void RemoteComm_TCP::WriteString(std::u8string_view Str)
{
	if (!Proc.IsServiceProcessRunning()) return;

	if (!Connected || socket_ == INVALID_SOCKET) return;

	int bytes_sent = send(socket_,
		reinterpret_cast<const char*>(Str.data()),
		static_cast<int>(Str.length()), 0);

	if (bytes_sent == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK) {
			Connected = false;
		}
	}
}

bool RemoteComm_TCP::HasStringToRead()
{
	//Skip Service Running Check
	if (!Connected || socket_ == INVALID_SOCKET) return false;

	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(socket_, &read_set);

	timeval timeout = { 0, 0 }; // 立即返回

	int result = select(0, &read_set, nullptr, nullptr, &timeout);
	if (result > 0) {
		return FD_ISSET(socket_, &read_set);
	}
	else if (result == 0) {
		// 超时，没有数据
		return false;
	}
	else {
		// 错误
		Connected = false;
		return false;
	}
}
