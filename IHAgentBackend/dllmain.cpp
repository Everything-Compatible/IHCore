// dllmain.cpp : IHAgentBackend DLL 入口点。
#include "framework.h"
#include <EC.h>
#include <Version.h>
#include "AgentBackend.h"
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
                IHAgentBackend_Init();
                IH::SetTextDrawRouter("Status", StatusRouter);
            },
            []() {
                IHAgentBackend_OrderedInit();
            },
            {
                // ── 引擎生命周期 /调试工具 ──
                {"IHCore::Reset",FuncInfo{IHAgentBackend_OnGameReset,  FuncType::Action}},
                {"IHCore::FrameUpdate",FuncInfo{IHAgentBackend_FrameUpdate, FuncType::Action}},
                {"ManualTest", FuncInfo{IHAgentBackend_ManualTest, FuncType::Action}},

                // ── 查询 ──
                {"GetMapInfo",          FuncInfo{IHAgentBackend_GetMapInfo,          FuncType::Action}},
                {"GetCellInfo",         FuncInfo{IHAgentBackend_GetCellInfo,         FuncType::Action}},
                {"GetHouseInfo",        FuncInfo{IHAgentBackend_GetHouseInfo,       FuncType::Action}},
                {"GetTechnoInfo",       FuncInfo{IHAgentBackend_GetTechnoInfo,      FuncType::Action}},
                {"GetTechTree",         FuncInfo{IHAgentBackend_GetTechTree,        FuncType::Action}},
                {"GetTypeInfo",         FuncInfo{IHAgentBackend_GetTypeInfo,        FuncType::Action}},
                {"GetObjectsOnCell",    FuncInfo{IHAgentBackend_GetObjectsOnCell,   FuncType::Action}},
                {"GetAerialObjects",    FuncInfo{IHAgentBackend_GetAerialObjects,   FuncType::Action}},
                {"GetTechnoInRange",    FuncInfo{IHAgentBackend_GetTechnoInRange,  FuncType::Action}},
                {"GetProductionState",  FuncInfo{IHAgentBackend_GetProductionState, FuncType::Action}},
                {"GetForceDistribution",FuncInfo{IHAgentBackend_GetForceDistribution, FuncType::Action}},
                {"GetCurrentTime",     FuncInfo{IHAgentBackend_GetCurrentTime,     FuncType::Action}},
                {"GetPlayerViewRoute",  FuncInfo{IHAgentBackend_GetPlayerViewRoute, FuncType::Action}},
                {"FindPath",            FuncInfo{IHAgentBackend_FindPath,          FuncType::Action}},

                // ── 格子操作 ──
                {"CellAddrToCoords",    FuncInfo{IHAgentBackend_CellAddrToCoords,    FuncType::Action}},
                {"CellCoordsToAddr",    FuncInfo{IHAgentBackend_CellCoordsToAddr,    FuncType::Condition}},
                {"GetCellShroud",       FuncInfo{IHAgentBackend_GetCellShroud,       FuncType::Action}},
                {"UnshroudCell",        FuncInfo{IHAgentBackend_UnshroudCell,        FuncType::Action}},
                {"AdjustCellShroud",    FuncInfo{IHAgentBackend_AdjustCellShroud,    FuncType::Action}},
                {"GetCellTiberium",     FuncInfo{IHAgentBackend_GetCellTiberium,     FuncType::Action}},
                {"AddCellTiberium",     FuncInfo{IHAgentBackend_AddCellTiberium,     FuncType::Action}},
                {"RemoveCellTiberium",  FuncInfo{IHAgentBackend_RemoveCellTiberium,  FuncType::Action}},
                {"SpreadCellTiberium",  FuncInfo{IHAgentBackend_SpreadCellTiberium,  FuncType::Action}},
                {"GetCellRadiation",    FuncInfo{IHAgentBackend_GetCellRadiation,    FuncType::Action}},
                {"IncreaseCellRadiation", FuncInfo{IHAgentBackend_IncreaseCellRadiation, FuncType::Action}},
                {"DecreaseCellRadiation", FuncInfo{IHAgentBackend_DecreaseCellRadiation, FuncType::Action}},
                {"GetFrameRateInfo", FuncInfo{IHAgentBackend_GetFrameRateInfo, FuncType::Action}},

                // ── 标记系统 ──
                {"MarkObject",   FuncInfo{IHAgentBackend_MarkObject,   FuncType::Action}},
                {"UnmarkObject", FuncInfo{IHAgentBackend_UnmarkObject, FuncType::Action}},
                {"ListMarks",    FuncInfo{IHAgentBackend_ListMarks,    FuncType::Action}},
                {"ClearMarks",   FuncInfo{IHAgentBackend_ClearMarks,   FuncType::Action}},

                // ── 会话管理 ──
                {"BeginSession",       FuncInfo{IHAgentBackend_BeginSession,        FuncType::Action}},
                {"EndSession",         FuncInfo{IHAgentBackend_EndSession,          FuncType::Action}},
                {"SessionHeartbeat",   FuncInfo{IHAgentBackend_SessionHeartbeat,    FuncType::Action}},
                {"GetToolList",        FuncInfo{IHAgentBackend_GetToolList,         FuncType::Action}},

                // ── 操作 ──
                {"IssueOrder",     FuncInfo{IHAgentBackend_IssueOrder,     FuncType::Action}},
                {"Deploy",         FuncInfo{IHAgentBackend_Deploy,         FuncType::Action}},
                {"Detonate",       FuncInfo{IHAgentBackend_Detonate,       FuncType::Action}},
                {"Scatter",        FuncInfo{IHAgentBackend_Scatter,        FuncType::Action}},
                {"Produce",        FuncInfo{IHAgentBackend_Produce,        FuncType::Action}},
                {"Suspend",        FuncInfo{IHAgentBackend_Suspend,        FuncType::Action}},
                {"Abandon",        FuncInfo{IHAgentBackend_Abandon,        FuncType::Action}},
                {"AbandonAll",     FuncInfo{IHAgentBackend_AbandonAll,     FuncType::Action}},
                {"SetPrimary",     FuncInfo{IHAgentBackend_SetPrimary,     FuncType::Action}},
                {"Place",          FuncInfo{IHAgentBackend_Place,          FuncType::Action}},
                {"SpecialPlace",   FuncInfo{IHAgentBackend_SpecialPlace,   FuncType::Action}},
                {"Repair",         FuncInfo{IHAgentBackend_Repair,         FuncType::Action}},
                {"Sell",           FuncInfo{IHAgentBackend_Sell,           FuncType::Action}},
                {"SellCell",       FuncInfo{IHAgentBackend_SellCell,       FuncType::Action}},
                {"Ally",           FuncInfo{IHAgentBackend_Ally,           FuncType::Action}},
                {"Cheer",          FuncInfo{IHAgentBackend_Cheer,          FuncType::Action}},
                {"AIProduce",      FuncInfo{IHAgentBackend_AIProduce,      FuncType::Action}},
                {"AIStopProduction",FuncInfo{IHAgentBackend_AIStopProduction,FuncType::Action}},
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
