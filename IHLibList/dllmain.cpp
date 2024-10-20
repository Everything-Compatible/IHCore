// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

std::vector<FuncHandle> Handles;
std::unordered_map<std::string, std::vector<IHInitialLoadService>> ServiceRequests;




extern "C"
{
	EXPORT int __cdecl GetNLibs(void)
	{
		return Handles.size();
	}
	EXPORT int __cdecl GetMaxLibs(void)
	{
		return INT_MAX;
	}
	EXPORT bool __cdecl RegisterEntry(FuncHandle HD)
	{
		Handles.push_back(HD);
		return true;
	}
	EXPORT const FuncHandle* __cdecl GetEntries(void)
	{
		return Handles.data();
	}
	EXPORT void __cdecl ServiceRequest(IHInitialLoadService IService)
	{
		ServiceRequests[IService.Service].push_back(IService);
	}
	EXPORT void __cdecl QueryServiceRequest(const char* Service, UniPArray* Ret)
	{
		if (!Service || !Ret)return;
		auto& p = ServiceRequests[Service];
		Ret->N = p.size();
		Ret->Ptr = p.data();
	}
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

