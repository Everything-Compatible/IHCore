#include "IHVerify.h"
#include <format>
#include <string>
#include <iostream>
#include <EventClass.h>
#include <YRPP.h>

// ------------------------------------------------------------------
//  SelectObject: select or deselect a Techno
//    -Address <0x...>
//    [-Deselect]  bool, default false
// ------------------------------------------------------------------
void __cdecl IHVerify_SelectObject(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber())
    {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    DWORD addr = (DWORD)oAddr.GetInt();

    auto pAbs = (TechnoClass*)addr;
    if (!pAbs) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    auto absType = pAbs->WhatAmI();
    if (absType != AbstractType::Unit &&
        absType != AbstractType::Infantry &&
        absType != AbstractType::Building &&
        absType != AbstractType::Aircraft)
    {
        ECDebug::ReturnStdError(ERROR_INVALID_FUNCTION);
        return;
    }

    bool deselect = false;
    auto oD = Context.GetObjectItem("Deselect");
    if (oD && oD.IsTypeBool()) deselect = oD.GetBool();

    if (deselect)
    {
        pAbs->IsSelected = false;
    }
    else
    {
        auto pCur = HouseClass::CurrentPlayer();
        pAbs->Select();
    }

    std::cout << std::format("{} 0x{:08X}", deselect ? "Deselected" : "Selected", addr) << std::endl;
    ECDebug::DoNotEcho();
}

// ------------------------------------------------------------------
//  DeployObject: deploy a Techno
//    -Address <0x...>
//    [-Method clicked|mission]  default "clicked"
//      clicked  = simulate player clicking Deploy button
//      mission  = directly queue Mission::Unload
// ------------------------------------------------------------------
void __cdecl IHVerify_DeployObject(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber())
    {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    DWORD addr = (DWORD)oAddr.GetInt();

    auto pAbs = (AbstractClass*)addr;
    if (!pAbs) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    auto absType = pAbs->WhatAmI();
    if (absType != AbstractType::Unit &&
        absType != AbstractType::Infantry &&
        absType != AbstractType::Building &&
        absType != AbstractType::Aircraft)
    {
        ECDebug::ReturnStdError(ERROR_INVALID_FUNCTION);
        return;
    }

    auto pTechno = (TechnoClass*)pAbs;

    std::string method = "clicked";
    auto oM = Context.GetObjectItem("Method");
    if (oM && oM.IsTypeString()) method = oM.GetString();

    if (method == "mission")
    {
        pTechno->QueueMission(Mission::Unload, true);
        std::cout << std::format("Deploy (mission) queued for 0x{:08X}", addr) << std::endl;
    }
    else
    {
        pTechno->ClickedEvent(NetworkEvents::Deploy);
        std::cout << std::format("Deploy (clicked) triggered for 0x{:08X}", addr) << std::endl;
    }

    ECDebug::DoNotEcho();
}

// ------------------------------------------------------------------
//  QueueMission: queue a Mission on a Techno
//    -Address <0x...>  -Mission <name|int>
// ------------------------------------------------------------------
static int ParseMission(const std::string& s)
{
    if (s.empty()) return -2;
    bool is_int = true;
    for (char c : s) { if (!isdigit(c) && c != '-') { is_int = false; break; } }
    if (is_int) return std::stoi(s);
    if (s == "Sleep")          return (int)Mission::Sleep;
    if (s == "Attack")         return (int)Mission::Attack;
    if (s == "Move")           return (int)Mission::Move;
    if (s == "QMove")          return (int)Mission::QMove;
    if (s == "Retreat")        return (int)Mission::Retreat;
    if (s == "Guard")          return (int)Mission::Guard;
    if (s == "Sticky")         return (int)Mission::Sticky;
    if (s == "Enter")          return (int)Mission::Enter;
    if (s == "Capture")        return (int)Mission::Capture;
    if (s == "Eaten")          return (int)Mission::Eaten;
    if (s == "Harvest")        return (int)Mission::Harvest;
    if (s == "Area_Guard")     return (int)Mission::Area_Guard;
    if (s == "Return")         return (int)Mission::Return;
    if (s == "Stop")           return (int)Mission::Stop;
    if (s == "Ambush")         return (int)Mission::Ambush;
    if (s == "Hunt")           return (int)Mission::Hunt;
    if (s == "Unload")         return (int)Mission::Unload;
    if (s == "Sabotage")       return (int)Mission::Sabotage;
    if (s == "Construction")   return (int)Mission::Construction;
    if (s == "Selling")        return (int)Mission::Selling;
    if (s == "Repair")         return (int)Mission::Repair;
    if (s == "Rescue")         return (int)Mission::Rescue;
    if (s == "Missile")        return (int)Mission::Missile;
    if (s == "Harmless")       return (int)Mission::Harmless;
    if (s == "Open")           return (int)Mission::Open;
    if (s == "Patrol")         return (int)Mission::Patrol;
    if (s == "ParadropApproach") return (int)Mission::ParadropApproach;
    if (s == "ParadropOverfly")  return (int)Mission::ParadropOverfly;
    if (s == "Wait")           return (int)Mission::Wait;
    if (s == "AttackMove")     return (int)Mission::AttackMove;
    if (s == "SpyplaneApproach") return (int)Mission::SpyplaneApproach;
    if (s == "SpyplaneOverfly")  return (int)Mission::SpyplaneOverfly;
    return -2;
}

void __cdecl IHVerify_QueueMission(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber())
    {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    DWORD addr = (DWORD)oAddr.GetInt();

    auto oMission = Context.GetObjectItem("Mission");
    if (!oMission)
    {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }

    int mis;
    if (oMission.IsTypeString())
        mis = ParseMission(oMission.GetString());
    else if (oMission.IsTypeNumber())
        mis = oMission.GetInt();
    else
    {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    if (mis < -1 || mis > 31)
    {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }

    auto pAbs = (AbstractClass*)addr;
    if (!pAbs) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    auto absType = pAbs->WhatAmI();
    if (absType != AbstractType::Unit &&
        absType != AbstractType::Infantry &&
        absType != AbstractType::Building &&
        absType != AbstractType::Aircraft)
    {
        ECDebug::ReturnStdError(ERROR_INVALID_FUNCTION);
        return;
    }

    auto pTechno = (TechnoClass*)pAbs;
    pTechno->QueueMission((Mission)mis, true);
    std::cout << std::format("QueueMission addr=0x{:08X} mission={} start=true", addr, mis) << std::endl;
    ECDebug::DoNotEcho();
}

// ------------------------------------------------------------------
//  GetFunc -- also accessible via ECInitLibrary actions map
// ------------------------------------------------------------------
static std::unordered_map<std::string, FuncInfo> s_Funcs
{
    {"FindObjects",   FuncInfo(IHVerify_FindObjects,   FuncType::Action)},
    {"SelectObject",  FuncInfo(IHVerify_SelectObject,  FuncType::Action)},
    {"DeployObject",  FuncInfo(IHVerify_DeployObject,  FuncType::Action)},
    {"QueueMission",  FuncInfo(IHVerify_QueueMission,  FuncType::Action)},
};

FuncInfo* IHVerify_GetFunc(const char* Name, int Version)
{
    auto it = s_Funcs.find(Name);
    return (it != s_Funcs.end()) ? &it->second : nullptr;
}
