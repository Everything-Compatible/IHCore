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
                {"IHCore::FrameUpdate",FuncInfo{IHVerify_FrameUpdate, FuncType::Action}},
                {"ManualTest", FuncInfo{IHVerify_ManualTest, FuncType::Action}},
                {"WatchPointer",FuncInfo{IHVerify_WatchPointer,FuncType::Action}},
                {"FindObjects",  FuncInfo{IHVerify_FindObjects,  FuncType::Action}},
                {"FindTechnoInfo",FuncInfo{IHVerify_FindTechnoInfo,FuncType::Action}},
                {"FindTypes",    FuncInfo{IHVerify_FindTypes,    FuncType::Action}},
                {"GetHouseInfo", FuncInfo{IHVerify_GetHouseInfo, FuncType::Action}},
                {"GetHouseTechTree", FuncInfo{IHVerify_GetHouseTechTree, FuncType::Action}},
                {"GetTechBuildingTypes", FuncInfo{IHVerify_GetTechBuildingTypes, FuncType::Action}},
                {"GetTechnoInfo", FuncInfo{IHVerify_GetTechnoInfo, FuncType::Action}},
                {"QueueMission",        FuncInfo{IHVerify_QueueMission,        FuncType::Action}},
                {"StartRepairBuilding", FuncInfo{IHVerify_StartRepairBuilding, FuncType::Action}},
                {"GetMapInfo",          FuncInfo{IHVerify_GetMapInfo,          FuncType::Action}},
                {"IsOnMap",             FuncInfo{IHVerify_IsOnMap,             FuncType::Condition}},
                {"GetObjectCoords",     FuncInfo{IHVerify_GetObjectCoords,     FuncType::Action}},
                {"GetCellInfo",         FuncInfo{IHVerify_GetCellInfo,         FuncType::Action}},
                {"FindObjectsOnCell",   FuncInfo{IHVerify_FindObjectsOnCell,   FuncType::Action}},
                {"FindObjectsInRange",  FuncInfo{IHVerify_FindObjectsInRange,  FuncType::Action}},
                {"FindAerialObjects",   FuncInfo{IHVerify_FindAerialObjects,   FuncType::Action}},
                {"CalcCellSpread",      FuncInfo{IHVerify_CalcCellSpread,      FuncType::Action}},
                {"IssueOrder",          FuncInfo{IHVerify_IssueOrder,          FuncType::Action}},
                {"SetDestination",      FuncInfo{IHVerify_SetDestination,      FuncType::Action}},
                {"GetMovementState",    FuncInfo{IHVerify_GetMovementState,    FuncType::Action}},
                {"CanEnterCell",        FuncInfo{IHVerify_CanEnterCell,        FuncType::Condition}},
                {"CellAddrToCoords",    FuncInfo{IHVerify_CellAddrToCoords,    FuncType::Action}},
                {"CellCoordsToAddr",    FuncInfo{IHVerify_CellCoordsToAddr,    FuncType::Condition}},
                {"GetCellShroud",       FuncInfo{IHVerify_GetCellShroud,       FuncType::Action}},
                {"UnshroudCell",        FuncInfo{IHVerify_UnshroudCell,        FuncType::Action}},
                {"AdjustCellShroud",    FuncInfo{IHVerify_AdjustCellShroud,    FuncType::Action}},
                {"GetCellTiberium",     FuncInfo{IHVerify_GetCellTiberium,     FuncType::Action}},
                {"AddCellTiberium",     FuncInfo{IHVerify_AddCellTiberium,     FuncType::Action}},
                {"RemoveCellTiberium",  FuncInfo{IHVerify_RemoveCellTiberium,  FuncType::Action}},
                {"SpreadCellTiberium",  FuncInfo{IHVerify_SpreadCellTiberium,  FuncType::Action}},
                {"GetCellRadiation",    FuncInfo{IHVerify_GetCellRadiation,    FuncType::Action}},
                {"IncreaseCellRadiation", FuncInfo{IHVerify_IncreaseCellRadiation, FuncType::Action}},
                {"DecreaseCellRadiation", FuncInfo{IHVerify_DecreaseCellRadiation, FuncType::Action}},
                {"GetFrameRateInfo", FuncInfo{IHVerify_GetFrameRateInfo, FuncType::Action}},
                {"SelectObject", FuncInfo{IHVerify_SelectObject, FuncType::Action}},
                {"DeployObject", FuncInfo{IHVerify_DeployObject, FuncType::Action}},
                {"MarkObject",   FuncInfo{IHVerify_MarkObject,   FuncType::Action}},
                {"UnmarkObject", FuncInfo{IHVerify_UnmarkObject, FuncType::Action}},
                {"ListMarks",    FuncInfo{IHVerify_ListMarks,    FuncType::Action}},
                {"ClearMarks",   FuncInfo{IHVerify_ClearMarks,   FuncType::Action}},
                {"CheckBuildability",FuncInfo{IHVerify_CheckBuildability,FuncType::Action}},
                {"GetFactoryInfo",FuncInfo{IHVerify_GetFactoryInfo,FuncType::Action}},
                {"FactoryProduce",FuncInfo{IHVerify_FactoryProduce,FuncType::Action}},
                {"FactoryPlace",FuncInfo{IHVerify_FactoryPlace,FuncType::Action}},
                {"FactorySuspend",FuncInfo{IHVerify_FactorySuspend,FuncType::Action}},
                {"FactoryAbandon",FuncInfo{IHVerify_FactoryAbandon,FuncType::Action}},
                {"FactoryCompleteProduction",FuncInfo{IHVerify_FactoryCompleteProduction,FuncType::Action}},
                {"FindFactory",FuncInfo{IHVerify_FindFactory,FuncType::Condition}},
                {"GetTechnoTypeInfo",FuncInfo{IHVerify_GetTechnoTypeInfo,FuncType::Action}},
                {"GetBuildingStatus",FuncInfo{IHVerify_GetBuildingStatus,FuncType::Action}},
                {"CanPlaceBuilding",FuncInfo{IHVerify_CanPlaceBuilding,FuncType::Condition}},
                {"IsObjectAlive",FuncInfo{IHVerify_IsObjectAlive,FuncType::Condition}},
                {"GetHouseProduction",FuncInfo{IHVerify_GetHouseProduction,FuncType::Action}},
                {"AIProduce",FuncInfo{IHVerify_AIProduce,FuncType::Action}},
                {"AIStopProduction",FuncInfo{IHVerify_AIStopProduction,FuncType::Action}},
                {"ListAllFactories",FuncInfo{IHVerify_ListAllFactories,FuncType::Action}},
                {"WatchBuildingReady",FuncInfo{IHVerify_WatchBuildingReady,FuncType::Action}},
                
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
