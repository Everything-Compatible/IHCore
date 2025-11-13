
#include "ECDbgConsole.h"
#include "ToolFunc.h"
#include "ECInterprocess.h"
#include "StringManagerExt.h"
#include <Windows.h>
#include <thread>
#include <iostream>
#include <sstream>	
#include <LocalData.h>
#include <optional>

extern JsonFile IHCoreJson;

std::unique_ptr<std::thread> ECDbgConsoleManagerThread;
std::unique_ptr<std::thread> SyringeDaemonMonitorThread;
std::atomic_bool ManagerTerminate{ false };
using u8stringstream = std::basic_stringstream<char8_t>;
std::atomic_bool SyringeDaemonMonitor_Connecting{ false };

void SyringeDaemonMonitorThreadFunc();
void ECDbgConsoleManagerThreadFunc();

bool NeedsDaemonMonitor()
{
	return SyringeData::IsADaemonNow() && !SyringeDaemonMonitorThread;
}

namespace ECExec
{
	HANDLE ConsoleMutex = NULL;

	char MutexName[200];

	void InitConsoleMutex()
	{
		sprintf_s(MutexName, "Global\\ECDbgConsoleMutex_%08X", GetCurrentProcessId());

		if (ConsoleMutex == NULL)
		{
			ConsoleMutex = CreateMutexA(NULL, FALSE, MutexName);
			if (ConsoleMutex == NULL)
			{
				MessageBoxW(NULL, (L"Failed to create ECDbgConsole mutex\nError Code : "+std::to_wstring(GetLastError())).c_str()
					, L"Error", MB_ICONERROR | MB_OK);
			}
		}
	}

	//locked = console control itself
	//unlocked = wait for external processor
	bool IsConsoleLocked()
	{
		//return true;//TMP


		//check if the mutex is locked by another process/thread
		if (ConsoleMutex == NULL)
		{
			InitConsoleMutex();
		}
		if (ConsoleMutex == NULL)return false;
		auto Condition = WaitForSingleObject(ConsoleMutex, 0);
		bool Locked = (Condition == WAIT_TIMEOUT);
		if(Condition == WAIT_OBJECT_0)
		{
			ReleaseMutex(ConsoleMutex);
		}

		return Locked;
	}

	bool LockConsole()
	{
		if (ConsoleMutex == NULL)
		{
			InitConsoleMutex();
		}
		if (ConsoleMutex == NULL)return false;
		auto Condition = WaitForSingleObject(ConsoleMutex, INFINITE);
		return Condition == WAIT_OBJECT_0;
	}

	bool UnlockConsole()
	{
		if (ConsoleMutex == NULL)return false;
		auto Condition = ReleaseMutex(ConsoleMutex);
		return Condition != 0;
	}

	bool TryLockConsole()
	{
		if (ConsoleMutex == NULL)
		{
			InitConsoleMutex();
		}
		if (ConsoleMutex == NULL)return false;
		auto Condition = WaitForSingleObject(ConsoleMutex, 0);
		return Condition == WAIT_OBJECT_0;
	}
}

namespace ECCommandRemoteBuf
{
	std::u8string Ret;
	std::u8string ErrorStr;
	int ErrorCode;
	std::atomic_bool UseValue = false;
	bool ReturnedValue;
	std::mutex RWMutex;//always 1 read 1 write

	void Enter()
	{
		RWMutex.lock();
		UseValue = true;
		ReturnedValue = false;
		Ret.clear();
		ErrorStr.clear();
		ErrorCode = 0;
	}

	void Exit()
	{
		UseValue = false;
		RWMutex.unlock();
	}
}

namespace ECCommand
{
	bool OutputReturnedValue = true;
	bool ReturnedValue = false;
	std::unordered_map<std::u8string, std::u8string> GlobalVariables;
	int ErrorCode;

	UTF8_CString __cdecl TextDrawRouter_GlobalVariables(const char* Key)
	{
		auto it = GlobalVariables.find(conv Key);
		if (it == GlobalVariables.end())return nullptr;
		else return it->second.c_str();
	}

	void SetGlobalVar(const std::u8string& Key, const std::u8string& Value)
	{
		if (Key.empty() || Value.empty())return;
		GlobalVariables[Key] = Value;
	}

	void DoNotEcho()
	{
		if (ECCommandRemoteBuf::UseValue) return;
		OutputReturnedValue = false;
	}

	void ReturnString(const std::u8string& Str)
	{
		if (ECCommandRemoteBuf::UseValue)
		{
			ECCommandRemoteBuf::ErrorStr = u8"";
			ECCommandRemoteBuf::Ret = Str;
			ECCommandRemoteBuf::ErrorCode = 0;
			ECCommandRemoteBuf::ReturnedValue = true;
		}
		else
		{
			ReturnedValue = true;
			if (Str.empty())return;
			GlobalVariables[u8"RET"] = Str;
			GlobalVariables[u8"ERROR_STRING"] = u8"";
			SetErrorCode(0);
		}
		
	}

	void ReturnError(const std::u8string& Str, int Code)
	{
		if (ECCommandRemoteBuf::UseValue)
		{
			ECCommandRemoteBuf::ErrorStr = Str;
			ECCommandRemoteBuf::Ret = u8"\033[31m[ERROR]\033[0m" + Str;
			ECCommandRemoteBuf::ErrorCode = Code;
			ECCommandRemoteBuf::ReturnedValue = true;
		}
		else
		{
			ReturnedValue = true;
			GlobalVariables[u8"RET"] = u8"\033[31m[ERROR]\033[0m" + Str;
			GlobalVariables[u8"ERROR_STRING"] = Str;
			SetErrorCode(Code);
		}
	}

	void ReturnStdError(long Code)
	{
		ReturnError(FormatMessageU8(Code), static_cast<int>(Code));
	}

	void SetErrorCode(int Code)
	{
		if (ECCommandRemoteBuf::UseValue)
		{
			ECCommandRemoteBuf::ErrorCode = Code;
		}
		else
		{
			ErrorCode = Code;
		}
	}

	int GetErrorCode()
	{
		return ErrorCode;
	}

	void SetResult(GenCallRetType Result)
	{
		if (Result == GenCallRetType::True)GlobalVariables[u8"RET"] = u8"True";
		else if (Result == GenCallRetType::False)GlobalVariables[u8"RET"] = u8"False";
		else if (Result == GenCallRetType::Void)GlobalVariables[u8"RET"] = u8"Void";
		else GlobalVariables[u8"RET"] = u8"Default";
		GlobalVariables[u8"ERROR_STRING"] = u8"";
	}

	void GetLastResult(std::u8string& Ret, std::u8string& ErrorStr, int& ErrorCode)
	{
		Ret = GetVar(u8"RET");
		ErrorStr = GetVar(u8"ERROR_STRING");
		ErrorCode = GetErrorCode();
	}

	const std::u8string& GetVar(const std::u8string& Key)
	{
		const static std::u8string EmptyString = u8"";
		auto it = GlobalVariables.find(Key);
		if (it == GlobalVariables.end())return EmptyString;
		else return it->second;
	}

	//Pick out pattern #Type@HEX of ArgumentStr
	std::optional<GeneratorParam> GetGeneratorParamFromStr(std::string_view Str)
	{
		if (Str.empty())return std::nullopt;
		if (Str[0] != '#')return std::nullopt;
		auto atpos = Str.find_first_of('@');
		if (atpos == std::string_view::npos || atpos == 1) return std::nullopt;
		auto type = Str.substr(1, atpos - 1);
		auto hexstr = Str.substr(atpos + 1);
		if (type.empty() || hexstr.empty()) return std::nullopt;
		uint64_t Addr = 0;
		try
		{
			Addr = std::stoull(std::string(hexstr), nullptr, 16);
		}
		catch (...)
		{
			return std::nullopt;
		}
		auto Ty = AbstractClass::GetClassType(std::string(type).c_str());
		return GeneratorParam{ FuncInfo::GClassVersion, reinterpret_cast<void*>(Addr), Ty, 0 };
	}

	//Pick out pattern {#Type@HEX} of ArgumentStr
	std::optional<GeneratorParam> GetGeneratorParamFromArgStr(const std::string& Str)
	{
		if (Str.empty())return std::nullopt;
		if (Str[0] != '{' || Str.back() != '}')
		{
			return std::nullopt;
		}
		return GetGeneratorParamFromStr(std::string_view(Str.data() + 1, Str.size() - 2));
	}

	DbgCommand ProcessCommand(const std::u8string& _Command)
	{
		std::scoped_lock lock(ECCommandRemoteBuf::RWMutex);

		//Command Format
		auto Command = _Command;
		for (auto& p : GlobalVariables)
		{
			auto pos = Command.find(u8"<" + p.first + u8">");
			if (pos != std::string::npos)
			{
				Command.replace(pos, p.first.size() + 2, ~EscapeString(~p.second));
			}
		}
		//puts(conv Command.c_str());

		//CommandName Json(Args)
		auto argbegin = Command.find_first_of(u8"{");
		std::u8string CommandName{ Command.begin(), 
			argbegin == std::string::npos ? Command.end() : Command.begin() + argbegin };

		auto CommandArg = SplitParamEx(CommandName);

		auto ArgumentStr = argbegin == std::string::npos ? std::u8string(u8"{}") : Trim(Command.substr(argbegin));

		if (!ArgumentStr.empty() && ArgumentStr.back() != '}')
		{
			return [] { return u8"\033[31m参数字符串括号未闭合！\033[0m"; };
		}

		if (CommandArg.empty())
		{
			return [] { return u8"\033[31m指令名称为空！\033[0m"; };
		}

		//Lib.Func@Ver
		std::u8string CommandLib;
		std::u8string CommandFn;
		int CommandVersion;
		{
			

			auto DotPos = CommandArg[0].find_first_of('.');
			if (DotPos != std::string::npos)
			{
				CommandLib = CommandArg[0].substr(0, DotPos);
				CommandFn = CommandArg[0].substr(DotPos + 1);
			}
			else
			{
				CommandLib = u8"";
				CommandFn = CommandArg[0];
			}

			auto VerPos = CommandFn.find_first_of('@');
			if (VerPos != std::string::npos)
			{
				try
				{
					CommandVersion = std::stoi(~CommandFn.substr(VerPos + 1));
				}
				catch (...)
				{
					return [] { return u8"\033[31m指令版本号格式错误！\033[0m"; };
				}
				CommandFn = CommandFn.substr(0, VerPos);
			}
			else
			{
				CommandVersion = DoNotCheckVersion;
			}
		}

		//std::cout << "CommandLib: " << ~CommandLib << ", CommandFn: " << ~CommandFn
		//  	<< ", CommandVersion: " << CommandVersion << std::endl;

		bool IsSyringeCmd = (CommandLib == u8"Syringe");

		//Debug::Log("[EC] Processing Command: Lib=\"%s\", Fn=\"%s\", Ver=%d, IsSyringe=%d\n",
		//	conv CommandLib.data(), conv CommandFn.data(), CommandVersion, IsSyringeCmd ? 1 : 0);

		FuncInfo* CommandInfo;
		
		if(IsSyringeCmd)
			CommandInfo = nullptr;
		else
		{
			auto FuncOption = Local::PickFunctionForCommand(conv CommandLib.c_str(), conv CommandFn.c_str(), CommandVersion);
			if (std::holds_alternative<std::nullopt_t>(FuncOption))
			{
				return [] { return u8"\033[31m指令未找到！\033[0m"; };
			}
			else if (std::holds_alternative<FuncInfo*>(FuncOption))
			{
				CommandInfo = std::get<FuncInfo*>(FuncOption);
			}
			else
			{
				auto& FuncMap = std::get<std::unordered_map<std::string, FuncInfo*>>(FuncOption);
				//unresolved and return
				std::u8string Result = u8"\033[31m可能的指令不唯一，请指定指令来源！\033[33m\n可能是：\n";
				for (auto& [LibName, pf] : FuncMap)
				{
					Result += u8"  " + ~LibName + u8"." + CommandFn + u8"\n";
				}
				Result += u8"\033[0m";
				return [R = std::move(Result)] {
					return R;
				};
			}
		}

		auto GenParam = GetGeneratorParamFromArgStr(~ArgumentStr);

		if (GenParam)
		{
			return [CommandInfo, IsSyringeCmd, CommandFn, Param = *GenParam]
				{
					using namespace std::string_literals;
					OutputReturnedValue = true;
					ReturnedValue = false;
					auto Result = IsSyringeCmd ? 
						ProcessSyringeRequestAlt(~CommandFn, Param) :
						Local::GeneralCallAlt(*CommandInfo, Param);
					if (!OutputReturnedValue)std::cout << std::endl;
					if (!ReturnedValue)
					{
						SetResult(Result);
						if (Result == GenCallRetType::True) return OutputReturnedValue ? u8"True"s : u8""s;
						else if (Result == GenCallRetType::False) return OutputReturnedValue ? u8"False"s : u8""s;
						else if (Result == GenCallRetType::Void) return u8""s;
						else return u8"\033[31m无法识别参数。\033[33m"s;
					}
					else return OutputReturnedValue ? GlobalVariables[u8"RET"] : u8""s;
				};
		}
		else
		{
			std::unordered_map<std::u8string, std::u8string> ExtArgs_Str;
			std::unordered_map<std::u8string, bool> ExtArgs_Bool;
			std::unordered_map<std::u8string, int> ExtArgs_int;
			std::unordered_map<std::u8string, double> ExtArgs_Double;
			if (CommandArg.size() > 1)
			{
				//as params 
				//-Key1 Value1 -Key2 Value2
				if (CommandArg.size() % 2 == 0)
				{
					return [] { return u8"\033[31m参数键与值不匹配！\033[0m"; };
				}
				for (size_t i = 1; i < CommandArg.size(); i += 2)
				{
					if (CommandArg[i].empty() || CommandArg[i + 1].empty())
					{
						return [] { return u8"\033[31m参数键或值不能为空！\033[0m"; };
					}
					if (CommandArg[i][0] != u8'-')
					{
						return [] { return u8"\033[31m参数键必须以'-'开头！\033[0m"; };
					}
					auto Key = CommandArg[i].substr(1); // Remove the leading '-'
					if (Key.empty())
					{
						return [] { return u8"\033[31m参数键不能为空！\033[0m"; };
					}

					auto& val = CommandArg[i + 1];

					if (val.empty())
					{
						return [] { return u8"\033[31m参数值不能为空！\033[0m"; };
					}

					if(val.front() == '\"' && val.back() == '\"' && val.size() >= 2)
						ExtArgs_Str[Key] = val.substr(1, val.size() - 2);
					else if (val.front() == '\'' && val.back() == '\'' && val.size() >= 2)
						ExtArgs_Str[Key] = val.substr(1, val.size() - 2);
					else if (!_strcmpi((~val).c_str(), "true"))
					{
						ExtArgs_Bool[Key] = true;
					}
					else if (!_strcmpi((~val).c_str(), "false"))
					{
						ExtArgs_Bool[Key] = false;
					}
					else if (val.find_first_not_of(u8"0123456789-+") == std::string::npos)
					{
						try
						{
							ExtArgs_int[Key] = std::stoi(~val);
						}
						catch (...)
						{
							return [] { return u8"\033[31m参数值不是整数！\033[0m"; };
						}
					}
					else if (val.find_first_not_of(u8"0123456789.-+eE") == std::string::npos)
					{
						try
						{
							ExtArgs_Double[Key] = std::stod(~val);
						}
						catch (...)
						{
							return [] { return u8"\033[31m参数值不是浮点数！\033[0m"; };
						}
					}
					else
					{
						ExtArgs_Str[Key] = val;
					}


				}
			}

			JsonFile ArgJson;
			auto ErrorStr = ArgJson.ParseTmpChecked(std::move(~ArgumentStr), conv u8"\033[31m【错误位置】\033[33m");

			if (!ErrorStr.empty())
			{
				return [ES = std::move(~ErrorStr)] {
					std::stringstream ss;
					ss << conv u8"参数语法错误：\033[33m\n" << ~ ES << conv u8"\033[0m";
					return ~ss.str();
					};
			}

			if (!ArgJson.Available())
			{
				return [] { return u8"\033[31m参数不可用！\033[0m";}; 			
			}

			for (auto& [p, v] : ExtArgs_Str)
			{
				ArgJson.GetObj().AddString(~p, ~v);
			}
			for (auto& [p, v] : ExtArgs_Bool)
			{
				ArgJson.GetObj().AddBool(~p, v);
			}
			for (auto& [p, v] : ExtArgs_int)
			{
				ArgJson.GetObj().AddInt(~p, v);
			}
			for (auto& [p, v] : ExtArgs_Double)
			{
				ArgJson.GetObj().AddDouble(~p, v);
			}

			return [CommandInfo, IsSyringeCmd, CommandFn,Arg = std::move(ArgJson)] () mutable
			{
				using namespace std::string_literals;
				OutputReturnedValue = true;
				ReturnedValue = false;
				
				if (CommandInfo->Type == FuncType::Remote)
				{
					ReturnInfoPtr pInfo;
					//Debug::Log("[EC] Console : CommandInfo->Func = 0x%08X ; Arg = 0x%08X ; &pInfo = 0x%08X\n", CommandInfo->Func, Arg.GetRaw(), std::addressof(pInfo));
					((RemoteCaller_t)CommandInfo->Func)(pInfo, Arg);
					//Debug::Log("[EC] Console : &Ret 0x%08X, Ret 0x%08X\n",&pInfo, pInfo.Ptr.get());

					auto& Result = pInfo;

					auto ResponseStr { ~Result.GetResponseData().GetText() };
					auto ResponseStrEx { ~Result.GetResponseData().GetTextEx() };

					GlobalVariables[u8"RET"] = ResponseStr;
					GlobalVariables[u8"ERROR_STRING"] = Result.Succeeded() ? u8""s : Result.GetErrorMessage();
					auto Ret = Result.Succeeded() ? u8""s : u8"\033[31m" + *Result.Ptr->ErrorMessage + u8"\033[0m";
					SetErrorCode(Result.Succeeded() ? 0 : 1);
					ReturnedValue = true;
					OutputReturnedValue = false;
					std::cout << ~ResponseStrEx << std::endl;

					Result.Release();
					return Ret;
				}
				else
				{
					auto Result = IsSyringeCmd ?
						ProcessSyringeRequest(~CommandFn, std::move(Arg)) :
						Local::GeneralCall(*CommandInfo, Arg);
					if (!OutputReturnedValue)std::cout << std::endl;
					if (!ReturnedValue)
					{
						SetResult(Result);
						if (Result == GenCallRetType::True) return OutputReturnedValue ? u8"True"s : u8""s;
						else if (Result == GenCallRetType::False) return OutputReturnedValue ? u8"False"s : u8""s;
						else if (Result == GenCallRetType::Void) return u8""s;
						else return u8"\033[31m无法识别参数。\033[33m"s;
					}
					else return OutputReturnedValue ? GlobalVariables[u8"RET"] : u8""s;
				}
			};
		}
	}
}


namespace ECDebug
{
	InfoStack<DbgCommand> CommandStack;
	InfoStack<std::u8string> CommandOutput;

	void FlushCommands()
	{
		for (auto&& Command : ECDebug::CommandStack.Release())
		{
			ECDebug::CommandOutput.Push(Command());
		}
	}

	bool ConsoleOpen{ false };
	bool ShouldHandleConsole{ false };

	void HandleConsole()
	{
		if (ShouldHandleConsole)
		{
			ECDbgConsoleManagerThread = std::make_unique<std::thread>(ECDbgConsoleManagerThreadFunc);
			ECDbgConsoleManagerThread->detach();
		}
	}

	bool IsConsoleOpen()
	{
		return ConsoleOpen;
	}

	bool HasConsole()
	{
		static bool Init = false;
		static bool _HasConsole = false;
		if (Init)return _HasConsole;
		else
		{
			Init = true;
			if (!IHCoreJson.Available())
			{
				return _HasConsole;
			}
			else
			{
				auto Obj = IHCoreJson.GetObj().GetObjectItem("OpenECDebugConsole");
				if (Obj.Available() && Obj.IsTypeBool())
					_HasConsole = Obj.GetBool();
				return _HasConsole;
			}
		}
	}

	bool OpenDebugConsole()
	{
		if (HasConsole())
		{
			if (AllocConsole())
			{
				FILE* fp;
				freopen_s(&fp, "CONOUT$", "w", stdout);
				freopen_s(&fp, "CONOUT$", "w", stderr);
				freopen_s(&fp, "CONIN$", "r", stdin);

				SetConsoleTitleW(L"万物互通 调试控制台");

				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				DWORD mode = 0;
				GetConsoleMode(hConsole, &mode);
				SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

				ConsoleOpen = true;	
				if (!ECDbgConsoleManagerThread)
				{
					ManagerTerminate = false;
					ShouldHandleConsole = true;
				}

				if (!SetConsoleOutputCP(CP_UTF8)) {
					DWORD error = GetLastError();
					std::cerr << "设置输出代码页失败! 错误代码: " << error << std::endl;
					return 1;
				}

				if (!SetConsoleCP(CP_UTF8)) {
					DWORD error = GetLastError();
					std::cerr << "设置输入代码页失败! 错误代码: " << error << std::endl;
					return 1;
				}

				return true;
			}
			else
			{
				ConsoleOpen = false;
				return false;
			}
		}
		else
		{
			ConsoleOpen = false;
			return false;
		}
	}

	bool CloseDebugConsole()
	{
		if (HasConsole() && ConsoleOpen)
		{
			if (FreeConsole())
			{
				fclose(stdout);
				fclose(stderr);
				fclose(stdin);
				ConsoleOpen = false;
				return true;
			}
			else
			{
				return false;
			}
		}

		ManagerTerminate = true;
		return true; 
	}

	std::string prompt = "EC >";
	std::string input;

	

	void ConsoleLoop()
	{
		input.clear();
		while (input.empty())
		{
			std::cout << prompt;
			std::getline(std::cin, input);
		}
		CommandStack.Push(std::move(ECCommand::ProcessCommand(~input)));
		int iWait = 0;
		int iOutput = 0;
		while (CommandOutput.Empty() && !ECExec::IsConsoleLocked() && !NeedsDaemonMonitor())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			if (iWait > 20 && iWait & 1)
			{
				putchar('*');//after 1s : risk of time out
				iOutput++;
			}
			iWait++;
		}

		if (iOutput)putchar('\n');

		//Locked == process itself
		if (NeedsDaemonMonitor())
		{
			SyringeDaemonMonitor_Connecting = true;
			SyringeDaemonMonitorThread = std::make_unique<std::thread>(SyringeDaemonMonitorThreadFunc);
			SyringeDaemonMonitorThread->detach();
			CommandStack.Clear();
			printf(conv u8"\033[33m进入调试模式……");
			while(SyringeDaemonMonitor_Connecting)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				putchar('.');
			}
			puts("\033[0m");
		}
		if (!CommandOutput.Empty())
		{
			for (auto&& Output : CommandOutput.Release())
			{
				if (!Output.empty())
				{
					std::cout << ~Output << std::endl;
				}
			}
		}
		if (ECExec::IsConsoleLocked())
		{
			for (auto&& Cmd : CommandStack.Release())
			{
				std::cout << ~Cmd() << std::endl;
			}
		}
	}
}

void ECDbgConsoleManagerThreadFunc()
{
	SyringeData::SetDaemonThread(GetCurrentThreadId());
	while (!ManagerTerminate)
	{
		ECDebug::ConsoleLoop();
	}
}

void SyringeDaemonMonitorThreadFunc()
{
	SyringeData::DaemonConnect();
	if (SyringeData::IsDaemonConnected())
	{
		printf(conv u8"\033[33m已连接 \033[1;34m%s \033[33m。\033[0m\n", SyringeData::GetExeData().SyringeVersionStr);
	}
	else
	{
		puts(conv u8"\033[1;31m无法连接到Syringe。\033[0m");
		puts(conv u8"\033[33m已退出调试模式。\033[0m");
		return;
	}
	SyringeDaemonMonitor_Connecting = false;

	Game::IsActive = false;

	while (1)
	{
		if (!SyringeData::IsDaemonConnected())return;
		if (SyringeData::ShouldCloseDaemonPipe())break;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	SyringeData::DaemonDisconnect();
}

UTF8_CString __cdecl TextDrawRouter_EnvVariable(const char* Key)
{
	static wchar_t Value[32768];
	static std::u8string Ret;
	Value[0] = L'\0';
	auto res = GetEnvironmentVariableW(UTF8toUnicode(Key).c_str(), Value, 32767);
	Ret = ~UnicodetoUTF8(Value);
	if (!res && GetLastError() == ERROR_ENVVAR_NOT_FOUND) return nullptr;
	else return Ret.c_str();
}

void DebugConsole_InitBeforeEverything()
{
	IH::SetTextDrawRouter("ConsoleVar", ECCommand::TextDrawRouter_GlobalVariables);
	IH::SetTextDrawRouter("EnvVar", TextDrawRouter_EnvVariable);
}