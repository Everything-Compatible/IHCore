// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "..\Common\IHLoader\IH.Config.h"
#include <cstdint>

std::vector<FuncHandle> Handles;
std::unordered_map<std::string, std::vector<IHInitialLoadService>> ServiceRequests;
std::unordered_map<std::string, std::unordered_map<std::string, LPCVOID>> NamedPointers;
std::unordered_map<std::string, std::unordered_map<LPCVOID, std::string>> NamedPointersInv;
ConfData Data;
std::mt19937 Randomizer;
int MaxID{ 1919810 };

extern "C"
{
	EXPORT LPCVOID  __cdecl GetNamedPointer(const char* Usage, const char* Name)
	{
		return NamedPointers[Usage][Name];
	}
	EXPORT const char*  __cdecl NamedPointer_GetName(const char* Usage, LPCVOID Pointer)
	{
		return NamedPointersInv[Usage][Pointer].c_str();
	}
	EXPORT void  __cdecl SetNamedPointer(const char* Usage, const char* Name, LPCVOID Pointer)
	{
		NamedPointers[Usage][Name] = Pointer;
		NamedPointersInv[Usage][Pointer] = Name;
	}
	EXPORT int __cdecl GenerateID()
	{
		return MaxID++;
	}
	EXPORT unsigned int __cdecl RandomUINT()
	{
		return Randomizer();
	}
	EXPORT ConfData* __cdecl GetConfData(void)
	{
		return &Data;
	}
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
	{
		std::random_device rdv;
		Randomizer.seed(rdv());
	}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

