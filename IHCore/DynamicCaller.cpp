#include <Windows.h>
#include <LocalData.h> 
#include <set>
#include "ECInterprocess.h"
#include "Debug.h"





//--------------------------------------------------------------


bool CallerContext::operator< (const CallerContext& rhs) const
{
	int cmp = Target.compare(rhs.Target);
	if (cmp != 0)
		return cmp < 0;
	cmp = Method.compare(rhs.Method);
	if (cmp != 0)
		return cmp < 0;
	return Version < rhs.Version;
}

const BYTE INIT = 0xFF;
/*




extern "C" __declspec(dllexport) __declspec(noinline) void __cdecl Function2(int a1, int a2, int a3)
{
	printf("%d %d %d", a1, a2, a3);
}

extern "C" __declspec(dllexport) __declspec(noinline) void __cdecl Function1(int a1, int a2)
{
	Function2(a1, a2, 0x114514);
}



Final Function:

push    ebp          55
mov     ebp, esp     8B EC
push    offset Ctx   68 xx xx xx xx  (Fill in with Ctx)
push    [ebp+C]      FF 75 0C
push    [ebp+8]      FF 75 08
call    pFunc        E8 xx xx xx xx  (Call Offset)
add     esp, 0xC     83 C4 0C
pop     ebp          5D
retn                 C3

*/


const BYTE DynamicFunc_Template[] = {
	0x55,
	0x8B, 0xEC,
	0x68, INIT, INIT, INIT, INIT,
	0xFF, 0x75, 0x0C,
	0xFF, 0x75, 0x08,
	0xE8, INIT, INIT, INIT, INIT,
	0x83, 0xC4, 0x0C,
	0x5D,
	0xC3
};
//Align FuncSize by 16
const size_t DynamicFunc_FuncSize = ((sizeof(DynamicFunc_Template) + 0xF) >> 4) << 4;

void GenerateDynamicFunction(RemotePackedCaller_t pFunc, const CallerContext* Ctx, BYTE* Address)
{
	//Debug::Log("[EC] GDF : Generating Dynamic Function at 0x%08X for Ctx at 0x%08X\n", Address, Ctx);

	//Write to Address
	memset(Address, 0xCC, DynamicFunc_FuncSize);
	memcpy(Address, DynamicFunc_Template, sizeof(DynamicFunc_Template));

	//Fill in Ctx Address
	memcpy(Address + 0x4, &Ctx, 4);

	//Fill in Call Offset
	DWORD_PTR CallOffset = (DWORD_PTR)pFunc - ((DWORD_PTR)Address + 0x13);
	memcpy(Address + 0xF, &CallOffset, 4);
}


std::vector<LPBYTE> Allocated_Pages;
DWORD LastPageUsed = 0;
DWORD dwPageSize = 0;

BYTE* AllocateDynamicFunctionSpace()
{
	if (!dwPageSize)
	{
		SYSTEM_INFO SysInfo;
		GetSystemInfo(&SysInfo);
		dwPageSize = SysInfo.dwPageSize;
	}
	if (dwPageSize < DynamicFunc_FuncSize)
		return nullptr;

	if (
		Allocated_Pages.empty() ||
		LastPageUsed + DynamicFunc_FuncSize > dwPageSize
		)
	{
		BYTE* NewPage = (BYTE*)VirtualAlloc(NULL, dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!NewPage)
			return nullptr;
		LastPageUsed = DynamicFunc_FuncSize;
		Allocated_Pages.push_back(NewPage);
		return NewPage;
	}
	else
	{
		auto Addr = Allocated_Pages.back() + LastPageUsed;
		LastPageUsed += DynamicFunc_FuncSize;
		return Addr;
	}
	
}

std::set<std::unique_ptr<CallerContext>> Callers;

BYTE* GetRemoteFunctionAddrHelper(const char* Target, const char* Method, int Version)
{
	auto [Iter, Success] = Callers.insert(std::make_unique<CallerContext>(conv Target, conv Method, Version));
	if (!Success)
	{
		return (BYTE*)Iter->get()->Function;
	}
	else
	{
		auto pFunc = AllocateDynamicFunctionSpace();
		if (!pFunc)
		{
			Callers.erase(Iter);
			return nullptr;
		}
		GenerateDynamicFunction(ExternalFunctionCaller, Iter->get(), pFunc);
		Iter->get()->Function = (RemoteCaller_t)pFunc;
		return pFunc;
	}
}

RemoteCaller_t GetRemoteFunctionAddr(const char* Target, const char* Method, int Version)
{
	return reinterpret_cast<RemoteCaller_t>(GetRemoteFunctionAddrHelper(Target, Method, Version));
}