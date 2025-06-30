#include <Syringe.h>
#include <Helpers/Macro.h>
#include <Dbghelp.h>
#include <Unsorted.h>
#include "Debug.h"
#include <string>
#include <chrono>
#include "ExtFile.h"
#include "ExtJson.h"
#include "SyringeEx.h"
#include <map>
#include "ToolFunc.h"
#include "EC.Listener.h"

#if 0
LONG CALLBACK IHExceptionHandler(PEXCEPTION_POINTERS const pExs);

std::wstring IHFullDump(
	std::wstring destinationFolder,
	PMINIDUMP_EXCEPTION_INFORMATION const pException)
{
	std::wstring filename = std::move(destinationFolder);
	filename += L"\\extcrashdump.dmp";

	HANDLE dumpFile = CreateFileW(filename.c_str(), GENERIC_WRITE,
		0, nullptr, CREATE_ALWAYS, FILE_FLAG_RANDOM_ACCESS, nullptr);

	MINIDUMP_TYPE type = static_cast<MINIDUMP_TYPE>(MiniDumpWithFullMemory);

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dumpFile, type, pException, nullptr, nullptr);
	CloseHandle(dumpFile);

	return filename;
}

std::wstring IHFullDump(
	PMINIDUMP_EXCEPTION_INFORMATION const pException)
{
	return IHFullDump(PrepareSnapshotDirectory(), pException);
}

/*
*
*
LONG CALLBACK IHExceptionFilter(PEXCEPTION_POINTERS const pExs)
{
	switch (pExs->ExceptionRecord->ExceptionCode) {
	case EXCEPTION_BREAKPOINT:
	case MS_VC_EXCEPTION:
		return EXCEPTION_CONTINUE_SEARCH;
	}

	IHExceptionHandler(pExs);
}

*
*
void Debug::FreeMouse() {
//	static bool freed = false;
//	if(!freed) {
		Game::sub_53E6B0();

		MouseClass::Instance->UpdateCursor(MouseCursorType::Default, false);
		WWMouseClass::Instance->ReleaseMouse();

		ShowCursor(TRUE);

		auto const BlackSurface = [](DSurface* const pSurface) {
			if(pSurface) {
				pSurface->Fill(0);
			}
		};

		BlackSurface(DSurface::Alternate);
		BlackSurface(DSurface::Composite);
		BlackSurface(DSurface::Hidden);
		BlackSurface(DSurface::Hidden_2);
		BlackSurface(DSurface::Primary);
		BlackSurface(DSurface::Sidebar);
		BlackSurface(DSurface::Tile);

		ShowCursor(TRUE);

//		DirectDrawWrap::DisposeOfStuff();
//		DirectDrawWrap::lpDD->SetCooperativeLevel(Game::hWnd, eDDCoopLevel::DDSCL_NORMAL);
//		DirectDrawWrap::lpDD->SetDisplayMode(800, 600, 16);

//		freed = true;
//	}
}
*/


void Debug_FatalError(bool Dump) {
	wchar_t Message[0x400];
	wsprintfW(Message,
		L"An internal error has been encountered and the game is unable to continue normally. "
		L"Please notify the mod's creators about this issue, or visit our website at "
		L"http://ares.strategy-x.com for updates and support.\n\n"
		L"%hs",
		Ares::readBuffer);

	Debug::Log("\nFatal Error:\n");
	Debug::Log("%s\n", Ares::readBuffer);

	MessageBoxW(Game::hWnd, Message, L"Fatal Error - Yuri's Revenge", MB_OK | MB_ICONERROR);

	if (Dump) {
		Debug::FullDump();
	}
}


[[noreturn]] void IHExit(UINT ExitCode) {
	Debug::Log("Exiting...\n");
	ExitProcess(ExitCode);
}

std::wstring IHPrepareSnapshotDirectory() {
	wchar_t path[MAX_PATH];
	auto const len = GetCurrentDirectoryW(MAX_PATH, path);
	std::wstring buffer(path, path + len);

	buffer += L"\\debug";
	CreateDirectoryW(buffer.c_str(), nullptr);

	SYSTEMTIME time;
	GetLocalTime(&time);

	wchar_t subpath[64];
	swprintf(subpath, 64, L"\\snapshot-%04u%02u%02u-%02u%02u%02u", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	buffer += subpath;
	CreateDirectoryW(buffer.c_str(), nullptr);

	return buffer;
}

#pragma warning(push)
#pragma warning(disable: 4646) // this function does not return, though it isn't declared VOID

[[noreturn]] LONG CALLBACK IHExcetionHandler(PEXCEPTION_POINTERS const pExs)
{
	//Debug::FreeMouse();
	Debug::Log("IHCore : Exception handler fired!\n");
	Debug::Log("Exception %X at %p\n", pExs->ExceptionRecord->ExceptionCode, pExs->ExceptionRecord->ExceptionAddress);
	SetWindowTextW(Game::hWnd, L"Fatal Error - Yuri's Revenge");

	//	if (IsDebuggerAttached()) return EXCEPTION_CONTINUE_SEARCH;

	switch (pExs->ExceptionRecord->ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
	case EXCEPTION_BREAKPOINT:
	case EXCEPTION_DATATYPE_MISALIGNMENT:
	case EXCEPTION_FLT_DENORMAL_OPERAND:
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
	case EXCEPTION_FLT_INEXACT_RESULT:
	case EXCEPTION_FLT_INVALID_OPERATION:
	case EXCEPTION_FLT_OVERFLOW:
	case EXCEPTION_FLT_STACK_CHECK:
	case EXCEPTION_FLT_UNDERFLOW:
	case EXCEPTION_ILLEGAL_INSTRUCTION:
	case EXCEPTION_IN_PAGE_ERROR:
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
	case EXCEPTION_INT_OVERFLOW:
	case EXCEPTION_INVALID_DISPOSITION:
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
	case EXCEPTION_PRIV_INSTRUCTION:
	case EXCEPTION_SINGLE_STEP:
	case EXCEPTION_STACK_OVERFLOW:
	case 0xE06D7363: // exception thrown and not caught
	{
		std::wstring path = Exception::PrepareSnapshotDirectory();

		if (Debug::bLog) {
			std::wstring logCopy = path + L"\\debug.log";
			CopyFileW(Debug::LogFileTempName.c_str(), logCopy.c_str(), FALSE);
		}

		std::wstring except_file = path + L"\\except.txt";

		if (FILE* except = _wfsopen(except_file.c_str(), L"w", _SH_DENYNO)) {
			constexpr auto const pDelim = "---------------------\n";
			fprintf(except, "Internal Error encountered!\n");
			fprintf(except, pDelim);
			fprintf(except, VERSION_STRVER);
			fprintf(except, "\n");
			fprintf(except, pDelim);

			fprintf(except, "Exception code: %08X at %08p\n", pExs->ExceptionRecord->ExceptionCode, pExs->ExceptionRecord->ExceptionAddress);

			fprintf(except, "Registers:\n");
			PCONTEXT pCtxt = pExs->ContextRecord;
			fprintf(except, "EIP: %08X\tESP: %08X\tEBP: %08X\n", pCtxt->Eip, pCtxt->Esp, pCtxt->Ebp);
			fprintf(except, "EAX: %08X\tEBX: %08X\tECX: %08X\n", pCtxt->Eax, pCtxt->Ebx, pCtxt->Ecx);
			fprintf(except, "EDX: %08X\tESI: %08X\tEDI: %08X\n", pCtxt->Edx, pCtxt->Esi, pCtxt->Edi);

			fprintf(except, "\nStack dump:\n");
			DWORD* ptr = reinterpret_cast<DWORD*>(pCtxt->Esp);
			for (int i = 0; i < 0x100; ++i) {
				fprintf(except, "%08p: %08X\n", ptr, *ptr);
				++ptr;
			}

			fclose(except);
			Debug::Log("Exception data has been saved to file:\n%ls\n", except_file.c_str());
		}

		if (MessageBoxW(Game::hWnd, L"Yuri's Revenge has encountered a fatal error!\nWould you like to create a full crash report for the developers?", L"Fatal Error!", MB_YESNO | MB_ICONERROR) == IDYES) {
			HCURSOR loadCursor = LoadCursor(nullptr, IDC_WAIT);
			SetClassLong(Game::hWnd, GCL_HCURSOR, reinterpret_cast<LONG>(loadCursor));
			SetCursor(loadCursor);
			Debug::Log("Making a memory dump\n");

			MINIDUMP_EXCEPTION_INFORMATION expParam;
			expParam.ThreadId = GetCurrentThreadId();
			expParam.ExceptionPointers = pExs;
			expParam.ClientPointers = FALSE;

			Exception::FullDump(std::move(path), &expParam);

			loadCursor = LoadCursor(nullptr, IDC_ARROW);
			SetClassLong(Game::hWnd, GCL_HCURSOR, reinterpret_cast<LONG>(loadCursor));
			SetCursor(loadCursor);
			Debug_FatalError("The cause of this error could not be determined.\r\n"
				"%s"
				"A crash dump should have been created in your game's \\debug subfolder.\r\n"
				"You can submit that to the developers (along with debug.txt and syringe.log)."
				, Debug::bParserErrorDetected ? "(One or more parser errors have been detected that might be responsible. Check the debug logs.)\r\n" : ""
			);
		}
		break;
	}
	case ERROR_MOD_NOT_FOUND:
	case ERROR_PROC_NOT_FOUND:
		Debug::Log("Massive failure: Procedure or module not found!\n");
		break;
	default:
		Debug::Log("Massive failure: reason unknown, have fun figuring it out\n");
		Debug::DumpObj(reinterpret_cast<byte*>(pExs->ExceptionRecord), sizeof(*(pExs->ExceptionRecord)));
		//			return EXCEPTION_CONTINUE_SEARCH;
		break;
	}

	IHExit(pExs->ExceptionRecord->ExceptionCode);
};

#pragma warning(pop)
#endif

void CallBeginWritingExceptIH();

void TmpWriteLineV(ExtFileClass& File, char const* const pFormat, va_list ArgList) noexcept
{
	if (File.Available())
	{
		time_t raw;
		time(&raw);
		tm t;
		localtime_s(&t, &raw);
		auto now = std::chrono::system_clock::now();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		int imilli = (int)millis.count();
		fprintf(File.GetRaw(), "[%02d:%02d:%02d:%03d] ", t.tm_hour, t.tm_min, t.tm_sec, imilli);
		vfprintf(File.GetRaw(), pFormat, ArgList);
		fputs("\n", File.GetRaw());
		fflush(File.GetRaw());
	}
}
void TmpWriteLine(ExtFileClass& File, char const* const pFormat, ...) noexcept
{
	va_list args;
	va_start(args, pFormat);
	TmpWriteLineV(File, pFormat, args);
	va_end(args);
}



#define EXCEPTION_UNKNOWN_ERROR_1 0xE06D7363
#define STATUS_FAIL_FAST_EXCEPTION 0xC0000409
const std::unordered_map<int, std::string> ExcMap
{ {
EXCEPTION_ACCESS_VIOLATION,"������ͼԽȨ����ĳ����ַ��"}, {
EXCEPTION_ARRAY_BOUNDS_EXCEEDED,"�߽��鷢�����������Խ�硣"}, {
EXCEPTION_BREAKPOINT,"�����ϵ㡣"}, {
EXCEPTION_DATATYPE_MISALIGNMENT,"�����Զ�дδ���������������ݡ�"}, {
EXCEPTION_FLT_DENORMAL_OPERAND,"��������ʱ����ͼ�����޷���ʾΪ��׼����ֵ�Ĺ�С��������"}, {
EXCEPTION_FLT_DIVIDE_BY_ZERO,"���и���������ʱ��ͼ����0��"}, {
EXCEPTION_FLT_INEXACT_RESULT,"��������Ľ����Խ�˿�׼ȷ��ʾ�ķ�Χ��"}, {
EXCEPTION_FLT_INVALID_OPERATION,"δ֪�ĸ����������"}, {
EXCEPTION_FLT_OVERFLOW,"���븡���������ָ������"}, {
EXCEPTION_FLT_STACK_CHECK,"��������ʱ����ջ��������������硣"}, {
EXCEPTION_FLT_UNDERFLOW,"���븡���������ָ����С��"}, {
EXCEPTION_ILLEGAL_INSTRUCTION,"������ִ����Ч��ָ��򲻴��ڵ�ָ�"}, {
EXCEPTION_IN_PAGE_ERROR,"������ͼ����ϵͳ��ʱ�޷����ص��ڴ�ҳ�棬��ͨ���������г���ʱ�������ӶϿ��ȡ�"}, {
EXCEPTION_INT_DIVIDE_BY_ZERO,"������������ʱ��ͼ����0��"}, {
EXCEPTION_INT_OVERFLOW,"��������Ľ����������硣"}, {
EXCEPTION_INVALID_DISPOSITION,"�쳣���������쳣�Ĵ�����Ч��ʹ�ø߼����Եĳ���Ա��Ӧ�������쳣��"}, {
EXCEPTION_NONCONTINUABLE_EXCEPTION,"������ͼ�ڷ��������쳣��������С�"}, {
EXCEPTION_PRIV_INSTRUCTION,"������ִ������Ȩִ�е�ָ�"}, {
EXCEPTION_SINGLE_STEP,"���ڵ��������У���ִ��һ��ָ�"}, {
EXCEPTION_STACK_OVERFLOW,"ջ�ռ䷢�����硣"}, {
STATUS_FAIL_FAST_EXCEPTION ,"����ʧ�ܻ���Ҫ����������˳���"}, {
EXCEPTION_UNKNOWN_ERROR_1 ,"�׳���C++�쳣�������񣬿�������ȱ�ٶ�Ӧ��catch�飬��C++������ʱ���ô����쳣��"}
};

const std::unordered_map<int, std::string> TmpMap
{
	{0x10,"��ִ��"},
	{0x20,"��/ִ��"},
	{0x40,"��/д/ִ��"},
	{0x80,"д��ʱ����/ִ��"},
	{0x01,"���ɷ���"},
	{0x02,"ֻ��"},
	{0x04,"��/д"},
	{0x08,"д��ʱ����"},
	{0x00,"δ����/���ͷ�"},
};

std::string GetExcStr(int Exc)
{
	auto it = ExcMap.find(Exc);
	if (it == ExcMap.cend())
	{
		return "δ֪";
	}
	else
	{
		return it->second;
	}
}

bool IsExecutable(HANDLE hProc, LPCVOID Ptr)
{
	MEMORY_BASIC_INFORMATION BInfo;
	if (VirtualQueryEx(hProc, Ptr, &BInfo, sizeof(BInfo)))
	{
		if (BInfo.Protect == 0x10 || BInfo.Protect == 0x20 || BInfo.Protect == 0x40 || BInfo.Protect == 0x80)return true;
		else return false;
	}
	else return false;
}

std::string GetAccessStr(HANDLE hProc, LPCVOID Ptr)
{
	MEMORY_BASIC_INFORMATION BInfo;
	if (VirtualQueryEx(hProc, Ptr, &BInfo, sizeof(BInfo)))
	{
		auto it = TmpMap.find(BInfo.Protect);
		if (it == TmpMap.cend())
		{
			return "δ֪Ȩ�ޣ����������ڴ汣�����Գ�������ȷ����ֵ�ĺ��壩��" + std::to_string(BInfo.Protect);
		}
		else
		{
			return it->second;
		}
	}
	else return "��ȡʧ��";
}

bool LongStackDump = false;
bool OnlyShowStackFrame = false;
JsonFile SyringeSetting;
std::vector<std::pair<std::wstring, DWORD>> LibBaseAddr;
std::vector<SyringeData::AddrRemoteData*> AddrSortedByOp;

void QueryNecessarySetting()
{
	if (!SyringeData::HasSyringeIH())return;
	auto Str = GetStringFromFile<ExtFileClass>("Syringe.json");
	if (Str.empty())return;
	SyringeSetting.Parse(Str);
	if (!SyringeSetting.Available())return;
	auto Obj = SyringeSetting.GetObj();
	auto SObj = Obj.GetObjectItem("LongStackDump");
	if (SObj.Available() && SObj.IsTypeBool())
		LongStackDump = SObj.GetBool();
	SObj = Obj.GetObjectItem("OnlyShowStackFrame");
	if (SObj.Available() && SObj.IsTypeBool())
		OnlyShowStackFrame = SObj.GetBool();
	for(const auto& p:SyringeData::LoadLib)
		LibBaseAddr.push_back(p);
	std::sort(LibBaseAddr.begin(), LibBaseAddr.end(), [](const auto& l, const auto& r) {return l.second < r.second; });
	for (const auto& [k, v] : SyringeData::AddrMap)
		if (v)AddrSortedByOp.push_back(v);
	std::sort(AddrSortedByOp.begin(), AddrSortedByOp.end(), [](const auto& l, const auto& r) {return l->HookDataAddr < r->HookDataAddr; });
}

bool SyringeReadMem(void const* address, void* buffer, DWORD size)
{
	return (ReadProcessMemory(GetCurrentProcess(), address, buffer, size, nullptr) != FALSE);
}


SyringeData::AddrRemoteData* GetDataFromOpAddr(DWORD Addr)
{
	if (!SyringeData::HasSyringeIH())return nullptr;
	static SyringeData::AddrRemoteData tmp;
	tmp.HookDataAddr = Addr;
	auto it = std::upper_bound(AddrSortedByOp.begin(), AddrSortedByOp.end(), &tmp, [](const auto& l, const auto& r) {return l->HookDataAddr < r->HookDataAddr; });
	if (it == AddrSortedByOp.end())
	{
		if (Addr >= AddrSortedByOp.back()->HookDataAddr + AddrSortedByOp.back()->GetOpSize())
			return nullptr;
		else return AddrSortedByOp.back();
	}
	else if (it == AddrSortedByOp.begin())
	{
		return nullptr;
	}
	else
	{
		--it;
		return *it;
	}
}

std::pair<DWORD, std::string> AnalyzeAddr(DWORD Addr)
{
	if (SyringeData::HasSyringeIH())
	{
		auto pAddr = GetDataFromOpAddr(Addr);
		if (pAddr)
		{
			char buf[30]{};
			_itoa_s(pAddr->Addr, buf, 16);
			return std::make_pair(Addr - pAddr->HookDataAddr, std::string("����") + buf + "�Ĺ���");
		}
		for (size_t i = 0; i < LibBaseAddr.size() - 1; i++)
		{
			if (LibBaseAddr[i].second <= Addr && Addr < LibBaseAddr[i + 1].second)
				return std::make_pair(Addr - LibBaseAddr[i].second, std::move(UnicodetoANSI(LibBaseAddr[i].first)));
		}
		if (LibBaseAddr.back().second <= Addr)
			return std::make_pair(Addr - LibBaseAddr.back().second, std::move(UnicodetoANSI(LibBaseAddr.back().first)));
	}
	return std::make_pair(Addr, "UNKNOWN");
}

bool InHookRange(DWORD Addr)
{
	return GetDataFromOpAddr(Addr) != nullptr;
}

ExtFileClass* pExcept;

void IHAnalyzeError(PEXCEPTION_POINTERS const pExs)
{
	QueryNecessarySetting();
	
	auto const exceptCode = pExs->ExceptionRecord->ExceptionCode;
	auto const exceptAddr = pExs->ExceptionRecord->ExceptionAddress;
	auto const AccessAddr = pExs->ExceptionRecord->ExceptionInformation[1];
	ExtFileClass ExceptFile;
	pExcept = &ExceptFile;
	ExceptFile.Open("except_ih.txt", "w");
	TmpWriteLine(ExceptFile, "IHCore ���������ű�Q��");
	CallBeginWritingExceptIH();
	if (SyringeData::HasSyringeIH())
	{
		TmpWriteLine(ExceptFile, "IHCore �������� %s�������л�ȡ��Ҫ�ķ�����Ϣ��",SyringeData::GetExeData().SyringeVersionStr);
	}
	else
	{
		TmpWriteLine(ExceptFile, "IHCore ���Ҳ���SyringeIH������������Ϣ��������ȱʧ��");
	}
	if (!SyringeSetting.Available())
		TmpWriteLine(ExceptFile, "IHCore ��δ�ҵ�Syringe.json����ȡĬ�����á�");
	else
	{
		TmpWriteLine(ExceptFile, "IHCore ����Syringe.json��������һЩ���á�");
		TmpWriteLine(ExceptFile, "LongStackDump = \"%s\"", CStrBoolImpl(LongStackDump, StrBoolType::Str_true_false));
		TmpWriteLine(ExceptFile, "OnlyShowStackFrame = \"%s\"", CStrBoolImpl(OnlyShowStackFrame, StrBoolType::Str_true_false));
	}
	auto [Rel, Str] = AnalyzeAddr((DWORD)exceptAddr);
	TmpWriteLine(ExceptFile,
		__FUNCTION__ ": �����쳣������: 0x%08X ", exceptCode);
	TmpWriteLine(ExceptFile, 
		"(����ԭ��%s)", GetExcStr(exceptCode).c_str());
	TmpWriteLine(ExceptFile, 
		"��ַ�� 0x%08X��%s+%X��[����Ȩ�ޣ�%s]",
		exceptAddr, Str.c_str(), Rel, GetAccessStr(GetCurrentProcess(), exceptAddr).c_str());
	if (IsExecutable(GetCurrentProcess(), (LPCVOID)exceptAddr))TmpWriteLine(ExceptFile, "�����쳣�ĵ�ַΪ��ִ�еĴ��롣");
	else TmpWriteLine(ExceptFile, "�����쳣�ĵ�ַ���Ǵ��룬����Ϊ������ڴ档");


	//TmpWriteLine(ExceptFile, __FUNCTION__ ": ACCESS VIOLATION at 0x%08X!", exceptAddr);
	HANDLE currentThread = GetCurrentThread();

	char const* access = nullptr;
	switch (pExs->ExceptionRecord->ExceptionInformation[0])
	{
	case 0: access = "��ȡ"; break;
	case 1: access = "д��"; break;
	case 8: access = "ִ��"; break;
	}

	auto [Rel2, Str2] = AnalyzeAddr((DWORD)AccessAddr);
	TmpWriteLine(ExceptFile, "������ͼ%s 0x%08X��%s+%X��[����Ȩ�ޣ�%s]��",
		access ? access : ("<δ֪��Ϊ��" + std::to_string(pExs->ExceptionRecord->ExceptionInformation[0]) + ">").c_str(),
		AccessAddr, Str2.c_str(), Rel2,
		GetAccessStr(GetCurrentProcess(), (LPCVOID)AccessAddr).c_str());
	if (IsExecutable(GetCurrentProcess(), (LPCVOID)AccessAddr))TmpWriteLine(ExceptFile, "��ͼ���ʵĵ�ַΪ��ִ�еĴ��롣");
	else TmpWriteLine(ExceptFile, "��ͼ���ʵĵ�ַ���Ǵ��룬����Ϊ������ڴ档");


	CONTEXT context;
	context.ContextFlags = CONTEXT_FULL;
	GetThreadContext(currentThread, &context);

	ExceptFile.Ln();
	TmpWriteLine(ExceptFile, "�Ĵ�����");
	TmpWriteLine(ExceptFile, "\tEAX = 0x%08X\tECX = 0x%08X\tEDX = 0x%08X",
		context.Eax, context.Ecx, context.Edx);
	TmpWriteLine(ExceptFile, "\tEBX = 0x%08X\tESP = 0x%08X\tEBP = 0x%08X",
		context.Ebx, context.Esp, context.Ebp);
	TmpWriteLine(ExceptFile, "\tESI = 0x%08X\tEDI = 0x%08X\tEIP = 0x%08X",
		context.Esi, context.Edi, context.Eip);
	ExceptFile.Ln();



	TmpWriteLine(ExceptFile, "\t��ջת����Ϣ���������ܵ�ջ֡�ֶΣ�");
	auto const esp = reinterpret_cast<DWORD*>(context.Esp);
	auto const eend = LongStackDump ? (DWORD*)0xFFFFFFFF : esp + 0x100;
	for (auto p = esp; p < eend; ++p)
	{
		DWORD dw;
		if (SyringeReadMem(p, &dw, 4)) {
			if (dw)
			{

				auto [Rel1, Str1] = AnalyzeAddr(dw);
				if (IsExecutable(GetCurrentProcess(), (LPCVOID)dw))
				{
					if (!OnlyShowStackFrame)
						ExceptFile.Ln();
				}
				else if (OnlyShowStackFrame)
				{
					continue;
				}
				auto pData = GetDataFromOpAddr(dw);
				if (pData)
					TmpWriteLine(ExceptFile, "\t0x%08X:\t0x%08X ������ %X �Ĺ��ӣ�[����Ȩ�ޣ�%s]",
						p, dw, pData->Addr, GetAccessStr(GetCurrentProcess(), (LPCVOID)dw).c_str());
				else TmpWriteLine(ExceptFile, "\t0x%08X:\t0x%08X ��%s+%X��[����Ȩ�ޣ�%s]",
					p, dw, Str1.c_str(), Rel1,
					GetAccessStr(GetCurrentProcess(), (LPCVOID)dw).c_str());
			}
			else if (!OnlyShowStackFrame)
			{
				TmpWriteLine(ExceptFile, "\t0x%08X:\t0x%08X", p, dw);
			}
		}
		else {
			if (LongStackDump)
			{
				break;
			}
			TmpWriteLine(ExceptFile, "\t0x%08X:\t���޷���ȡ��", p);
		}
	}
}


DEFINE_HOOK(0x4C8FE0, ExceptionHandlerEx, 9)
{
	GET(LPEXCEPTION_POINTERS, pExs, EDX);
	Debug::Log("IHCore:Writing Error Report ...");
	IHAnalyzeError(pExs);
	return 0;
}




void CALLBACK Export_AnalyzeAddr(DWORD /*In*/ Addr, size_t /*In*/ BaseDescBufSize, DWORD& /*Out*/ RelAddr, char* /*Out GBK*/ BaseDesc)
{
	auto P = AnalyzeAddr(Addr);
	RelAddr = P.first;
	if (P.second.size() >= BaseDescBufSize)
	{
		strncpy_s(BaseDesc, BaseDescBufSize, P.second.c_str(), BaseDescBufSize - 1);
	}
	else
	{
		strcpy_s(BaseDesc, BaseDescBufSize, P.second.c_str());
	}
}
void CALLBACK Export_AccessStr(LPVOID /*In*/ Addr, size_t /*In*/ BaseDescBufSize, char* /*Out GBK*/ AccessStr)
{
	auto Access = GetAccessStr(GetCurrentProcess(), Addr);
	if (Access.size() >= BaseDescBufSize)
	{
		strncpy_s(AccessStr, BaseDescBufSize, Access.c_str(), BaseDescBufSize - 1);
	}
	else
	{
		strcpy_s(AccessStr, BaseDescBufSize, Access.c_str());
	}
}
bool CALLBACK Export_IsAddrExecutable(LPVOID /*In*/ Addr)
{
	return IsExecutable(GetCurrentProcess(), Addr);
}
void CALLBACK Export_WriteToExceptIH(const char* /*In GBK*/ Format, ...)
{
	va_list args;
	va_start(args, Format);
	TmpWriteLineV(*pExcept, Format, args);
	va_end(args);
}


void CallBeginWritingExceptIH()
{
	ExceptionAnalyzer A;
	A.AnalyzeAddr = Export_AnalyzeAddr;
	A.GetAccessStr = Export_AccessStr;
	A.IsAddrExecutable = Export_IsAddrExecutable;
	A.WriteToExceptIH = Export_WriteToExceptIH;
	for (auto hd : ECListener::GetAll("EC::BeginWritingExceptIH"))
	{
		AsType<void CALLBACK(const ExceptionAnalyzer& Anal)>(hd)(A);
	}
}