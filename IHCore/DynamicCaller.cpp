#include <Windows.h>
#include <LocalData.h> 
#include <set>
#include "ECInterprocess.h"





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

Final Function:

push    ebp          55
mov     ebp, esp     8B EC
push    offset Ctx   68 xx xx xx xx  (Fill in with Ctx)
push    [ebp+Obj]    FF 75 08
call    pFunc        E8 xx xx xx xx  (Call Offset)
add     esp, 8       83 C4 08
pop     ebp          5D
retn                 C3

*/
const BYTE DynamicFunc_Template[] = {
	0x55,
	0x8B, 0xEC,
	0x68, INIT, INIT, INIT, INIT,
	0xFF, 0x75, 0x08,
	0xE8, INIT, INIT, INIT, INIT,
	0x83, 0xC4, 0x08,
	0x5D,
	0xC3
};
//Align FuncSize by 16
const size_t DynamicFunc_FuncSize = ((sizeof(DynamicFunc_Template) + 0xF) >> 4) << 4;

void GenerateDynamicFunction(RemotePackedCaller_t pFunc, const CallerContext* Ctx, BYTE* Address)
{
	//Write to Address
	memset(Address, 0xCC, DynamicFunc_FuncSize);
	memcpy(Address, DynamicFunc_Template, sizeof(DynamicFunc_Template));

	//Fill in Ctx Address
	memcpy(Address + 0x4, &Ctx, 4);

	//Fill in Call Offset
	DWORD_PTR CallOffset = (DWORD_PTR)pFunc - ((DWORD_PTR)Address + 0x10);
	memcpy(Address + 0xC, &CallOffset, 4);
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

std::set<CallerContext> Callers;

BYTE* GetRemoteFunctionAddrHelper(const char* Target, const char* Method, int Version)
{
	auto [Iter, Success] = Callers.insert(CallerContext{ conv Target, conv Method, Version });
	if (!Success)
	{
		return (BYTE*)Iter->Function;
	}
	else
	{
		auto pFunc = AllocateDynamicFunctionSpace();
		if (!pFunc)
		{
			Callers.erase(Iter);
			return nullptr;
		}
		GenerateDynamicFunction(ExternalFunctionCaller, std::addressof(*Iter), pFunc);
		Iter->Function = (RemoteCaller_t)pFunc;
		return pFunc;
	}
}

RemoteCaller_t GetRemoteFunctionAddr(const char* Target, const char* Method, int Version)
{
	return reinterpret_cast<RemoteCaller_t>(GetRemoteFunctionAddrHelper(Target, Method, Version));
}