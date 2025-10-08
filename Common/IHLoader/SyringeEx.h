#pragma once
#include <Syringe.h>
#include <vector>
#include <unordered_map>
#include <string>
#include "ExtJson.h"

#if SYR_VER == 2

#pragma pack(push, 16)
#pragma warning(push)
#pragma warning( disable : 4324)
__declspec(align(32)) struct hookaltdecl {
	unsigned int hookAddr;
	unsigned int hookSize;
	const char* hookName;
	int Priority;
	const char* SubPriorityPtr;
};

static_assert(sizeof(hookaltdecl) == 32);
#pragma warning(pop)
#pragma pack(pop)

#pragma section(".hphks00", read, write)

namespace SyringeData {
	namespace HookAlt {

	};
};

#define declhookex(hook, funcname, size, priority, sub_priority) \
namespace SyringeData { \
	namespace HookAlt { __declspec(allocate(".hphks00")) hookaltdecl _hk__ ## hook ## funcname = { ## hook, ## size, #funcname, ## priority, ## sub_priority }; }; \
};

#endif

#ifndef declhookex
#define declhookex(hook, funcname, size, priority, sub_priority)
#endif // declhookex

//#ifdef DEFINE_HOOK 

#define DEFINE_HOOKEX(hook, funcname, size, priority, sub_priority) \
declhookex(hook, funcname, size, priority, sub_priority) \
EXPORT_FUNC(funcname)

#define DEFINE_HOOKEX_AGAIN(hook, funcname, size, priority, sub_priority) \
declhookex(hook, funcname, size, priority, sub_priority)

using HookType = DWORD(__cdecl*)(REGISTERS*);

template<typename T>
struct SyrPArray
{
	size_t N;
	const T* Data;

	SyrPArray() :N(0), Data(nullptr) {}
	SyrPArray(const std::vector<T>& p) :N(p.size()), Data(p.data()) {}

};


//��DllMain����Init::Initialize()�󼴿�ʹ��
namespace SyringeData
{
	struct ExeRemoteData
	{
		char SyringeVersionStr[256];
		BYTE VMajor;
		BYTE VMinor;
		BYTE VRelease;
		BYTE VBuild;

		char FileName[260];
		char AbsPath[260];
		DWORD BaseAddress;
		DWORD EntryPoint;
		
		int dwReserved[3];
	};

	struct LibRemoteData
	{
		const char* LibName;
		const char* AbsPath;
		DWORD LibID;
		const char* SettingText;

		int dwReserved[4];
	};

	struct AddrRemoteData
	{
		DWORD Addr;
		DWORD HookDataAddr;
		int OverriddenCount;
		int HookCount;
		int dwReserved[6];
		DWORD FirstHookIndex;//VLA Header

		inline DWORD HookIdx(int i)
		{
			return *((&FirstHookIndex) + i);
		}

		inline BYTE* HookData()
		{
			return (BYTE*)HookDataAddr;
		}

		size_t GetOpSize() const;
	};
	struct HookRemoteData
	{
		const char* ProcName;
		DWORD HookID;
		DWORD LibID;
		DWORD HookAddress;
		size_t OverrideLength;
		int dwReserved[3];
	};

	struct MemCopyData
	{
		char* Name;
		void* Addr;
	};

	struct DaemonData
	{
		BOOL EnableDaemon;
		BOOL OpenAsDaemon;
		DWORD ThreadID;
		const wchar_t* lpReportString;
		DWORD lpReportStringLen;
		const char* lpDebugPipeName;
		DWORD lpDebugPipeNameLen;
		int dwReserved[9];
	};

	struct SharedMemHeader
	{
		int TotalSize;
		int WritingComplete;
		int RecordCount;
		int RecordSize;

		DWORD DatabaseAddr;
		DWORD DllRecordAddr;
		int DllRecordCount;
		int ReservedPtr;
		int ReservedHandle;
		int Reserved[7];
	};

	struct HookContext
	{
		REGISTERS* pReg;
		SyrPArray<BYTE> Stack;

		HookContext() = delete;
		HookContext(const HookContext&) = delete;
		HookContext(HookContext&&);
		HookContext(REGISTERS* pReg);
		HookContext(size_t StackSize, size_t TopOffset);
		~HookContext();
		DWORD CallHook(HookType Proc) const;
		DWORD CallHook(DWORD Address, const std::string& Lib, const std::string& Proc) const;
	};
#define ThisHookContext HookContext{ R };

	extern std::unordered_map<std::string, LibRemoteData*> LibMap;
	extern std::unordered_map<DWORD, AddrRemoteData*> AddrMap;
	extern std::unordered_map<DWORD, HookRemoteData*> HookMap;
	extern std::unordered_map<std::string, MemCopyData*> MemMap;
	extern std::unordered_map<std::wstring, DWORD> LoadLib;

	bool HasSyringeIH();
	DWORD SyringeHash(const char* str);
	DWORD SyringeHashUpper(const char* str);
	DWORD GetSyringeProcID();
	const std::string& ExecutableDirectoryPath();
	const std::string& UniqueIDByPath();//ֻ���ǵ�ַ
	DWORD BaseAddress();
	void InitRemoteData();
	ExeRemoteData& GetExeData();
	DWORD GetDatabaseSize();
	LibRemoteData* GetLibData(const std::string& Name);
	LibRemoteData* GetLibData(const DWORD LibID);
	DWORD GetLibID(const std::string& Name);
	DWORD GetHookID(const std::string& Lib, const std::string& Proc);
	AddrRemoteData* GetAddrData(DWORD Addr);
	MemCopyData* GetCopyMemData(const std::string& Name);
	HookRemoteData* GetHookData(const std::string& Lib, const std::string& Proc);
	HookRemoteData* GetHookData(const DWORD HookID);
	DWORD GetLibBaseAddress(std::wstring Name);

	SyrPArray<BYTE> GetOpCode(DWORD Addr);
	int GetValidHookCount(DWORD Addr);
	const char* GetSettingText(const LibRemoteData& Lib);
	void ReplaceHookFunction(DWORD Address, const std::string& Lib, const std::string& Proc, DWORD pFunc);
	void RestoreHookFunction(DWORD Address, const std::string& Lib, const std::string& Proc);
	void DisableOpCode(DWORD Addr);
	void EnableOpCode(DWORD Addr);
	void EnableHook(DWORD Address, const std::string& Lib, const std::string& Proc);
	void DisableHook(DWORD Address, const std::string& Lib, const std::string& Proc);
	void EnableAllHooksAt(DWORD Address);
	void DisableAllHooksAt(DWORD Address);
	bool IsHookValid(DWORD Address, const std::string& Lib, const std::string& Proc);
	HookType GetHookProc(DWORD Address, const std::string& Lib, const std::string& Proc);

	bool IsDaemonSupported();
	bool SetDaemonThread(DWORD id);
	bool IsADaemonNow();
	bool DaemonConnect(int WaitBusyMillis = 3000);
	bool DaemonDisconnect();
	bool IsDaemonConnected();
	std::string SendRequestMessage(const std::string& Message);//����ʽ����
	const wchar_t* GetDaemonReport();
	bool ShouldCloseDaemonPipe();
}
/*

    GetOpCode��ȡ/�޸ı����ǵĻ�����
	DisableOpCodeȡ��ִ��ԭʼ������
	EnableOpCode�ָ�ִ��ԭʼ������
	GetActualOverriddenBytes��ȡʵ�ʸ��ǵ��ֽ���
    GetValidHookCount��ȡ��ַ����Ч�Ĺ�������
	EnableHook����ʱ����ָ������
	DisableHook����ʱ����ָ������
	EnableAllHooksAt����ĳ����ַ��ȫ������
	DisableAllHooksAt����ĳ����ַ��ȫ������
	ReplaceHookFunction����ĳ������ִ�еĺ���
	RestoreHookFunction�ָ�ĳ������ִ�еĺ���
	ReplaceFunctionBody�滻������
	RestoreFunctionBody�ָ�������
	GetSettingJSON��ȡ����
	GetSettingText��ȡ���ã������ı�
	GetLoadedConfigJSON��ȡȫ������
	GetLoadedConfigText��ȡȫ�����ã������ı�
	HookContext:���ù���

*/