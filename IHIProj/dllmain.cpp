// dllmain.cpp : 定义 DLL 应用程序的入口点。
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <EC.h>
#include <Version.h>
#include "IProjFile.h"

extern "C" __declspec(dllexport) void SyringeForceLoad()
{
    ; // This function is intentionally left empty to force the DLL to load.
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (ECInitLibrary(
            PRODUCT_LIBNAME_STR,
            PRODUCT_VERSION,
            PRODUCT_LOWEST_SUPPORTED_VERSION,
            PRODUCT_FULLNAME_U8STR,
            []() {
            },
            []() {
            },
            {}
            ))
        {
            RegisterIHFile(INIWeaverProjFileClass);
			RegisterIHFileFilter(INIWeaverProjFileClass, INIWeaverProjFileFilter);
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

