// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "MyLoader.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (Init::Initialize())
        {
            InitialLoad::CreateRequestAndSubmit<InitialLoadParam_CustomPath>("IHFile::CustomPathListFirst", UseRelativePath, "\\WTF\\");
            InitialLoad::CreateRequestAndSubmit<InitialLoadParam_RedirectFile>("IHFile::RedirectFileName", "rulesmd.ini", "rulesmo.ini");
            //InitialLoad::CreateRequestAndSubmit<InitialLoadParam_CustomFile>("IHFile::AddMixFile", "something.mix");
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

