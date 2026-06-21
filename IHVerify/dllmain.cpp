// dllmain.cpp : IHVerify DLL entry point.
#include "framework.h"
#include <EC.h>
#include <Version.h>
#include "IHVerify.h"
#include "Pointers.h"
#include <CRT.h>

extern "C" __declspec(dllexport) void SyringeForceLoad()
{
    ;
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
                IHVerify_Init();
                IH::SetTextDrawRouter("Status", StatusRouter);
            },
            []() {
                IHVerify_OrderedInit();
            },
            {
                {"IHCore::Reset",FuncInfo{IHVerify_OnGameReset,  FuncType::Action}},
                {"FindObjects",  FuncInfo{IHVerify_FindObjects,  FuncType::Action}},
                {"FindTypes",    FuncInfo{IHVerify_FindTypes,    FuncType::Action}},
                {"GetHouseInfo", FuncInfo{IHVerify_GetHouseInfo, FuncType::Action}},
                {"QueueMission",        FuncInfo{IHVerify_QueueMission,        FuncType::Action}},
                {"GetMapInfo",          FuncInfo{IHVerify_GetMapInfo,          FuncType::Action}},
                {"IsOnMap",             FuncInfo{IHVerify_IsOnMap,             FuncType::Condition}},
                {"GetObjectCoords",     FuncInfo{IHVerify_GetObjectCoords,     FuncType::Action}},
                {"GetCellInfo",         FuncInfo{IHVerify_GetCellInfo,         FuncType::Action}},
                {"FindObjectsOnCell",   FuncInfo{IHVerify_FindObjectsOnCell,   FuncType::Action}},
                {"FindObjectsInRange",  FuncInfo{IHVerify_FindObjectsInRange,  FuncType::Action}},
                {"FindAerialObjects",   FuncInfo{IHVerify_FindAerialObjects,   FuncType::Action}},
                {"CalcCellSpread",      FuncInfo{IHVerify_CalcCellSpread,      FuncType::Action}},
                {"SelectObject", FuncInfo{IHVerify_SelectObject, FuncType::Action}},
                {"DeployObject", FuncInfo{IHVerify_DeployObject, FuncType::Action}},
                {"MarkObject",   FuncInfo{IHVerify_MarkObject,   FuncType::Action}},
                {"UnmarkObject", FuncInfo{IHVerify_UnmarkObject, FuncType::Action}},
                {"ListMarks",    FuncInfo{IHVerify_ListMarks,    FuncType::Action}},
                {"ClearMarks",   FuncInfo{IHVerify_ClearMarks,   FuncType::Action}},
            },
            {}
            ))
        {
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
