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
    if (!pAbs || !IsPointerAlive(addr)) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

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
    if (!pAbs || !IsPointerAlive(addr)) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

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

void __cdecl IHVerify_StartRepairBuilding(JsonObject Context)
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
    if (!pAbs || !IsPointerAlive(addr)) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    auto absType = pAbs->WhatAmI();
    if (absType != AbstractType::Building)
    {
        ECDebug::ReturnStdError(ERROR_INVALID_FUNCTION);
        return;
    }

	auto pBld = (BuildingClass*)pAbs;

    if (!pBld->CanBeRepaired())
    {
		ECDebug::ReturnError(~std::format("Building 0x{:08X} cannot be repaired", addr), ERROR_INVALID_FUNCTION);
		return;
    }

    pBld->IsBeingRepaired = true;
    std::cout << std::format("Building 0x{:08X} is being repaired", addr) << std::endl;
    ECDebug::DoNotEcho();
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
    if (!pAbs || !IsPointerAlive(addr)) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

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
//  IssueOrder: high-level order combining SetDestination/SetTarget + QueueMission
//    -Address <0x...>
//    -Mission <Move|Attack|AttackMove|Guard|AreaGuard|Stop|Hunt|Retreat|Harvest|Enter|Capture|Patrol|Unload>
//    [-ToX <int> -ToY <int> | -To <0x...>]   — destination
//    [-Target <0x...>]                          — attack target
// ------------------------------------------------------------------
struct OrderSpec {
    const char* name;
    Mission mission;
    bool needsDest;
    bool destCellOk;
    bool destObjOk;
    bool targetOrDest;  // Attack: Target or Destination accepted
};

static const OrderSpec s_Orders[] = {
    {"Move",        Mission::Move,        true,  true,  true,  false},
    {"Attack",      Mission::Attack,      false, true,  true,  true },
    {"AttackMove",  Mission::AttackMove,  true,  true,  true,  false},
    {"Guard",       Mission::Guard,       false, false, false, false},
    {"AreaGuard",   Mission::Area_Guard,  true,  true,  true,  false},
    {"Stop",        Mission::Stop,        false, false, false, false},
    {"Hunt",        Mission::Hunt,        false, false, false, false},
    {"Retreat",     Mission::Retreat,     true,  true,  false, false},
    {"Harvest",     Mission::Harvest,     true,  false, true,  false},
    {"Enter",       Mission::Enter,       true,  false, true,  false},
    {"Capture",     Mission::Capture,     true,  false, true,  false},
    {"Patrol",      Mission::Patrol,      true,  true,  false, false},
    {"Unload",      Mission::Unload,      false, false, false, false},
};

void __cdecl IHVerify_IssueOrder(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    // Parse -Address
    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    DWORD addr = (DWORD)oAddr.GetInt();
    auto pAbs = (AbstractClass*)addr;
    if (!pAbs || !IsPointerAlive(addr)) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }
    auto absType = pAbs->WhatAmI();
    if (absType != AbstractType::Unit && absType != AbstractType::Infantry &&
        absType != AbstractType::Building && absType != AbstractType::Aircraft) {
        ECDebug::ReturnStdError(ERROR_INVALID_FUNCTION); return;
    }
    auto pTechno = (TechnoClass*)pAbs;

    // Parse -Mission
    auto oMission = Context.GetObjectItem("Mission");
    if (!oMission || !oMission.IsTypeString()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    std::string missionName = oMission.GetString();

    const OrderSpec* pSpec = nullptr;
    for (auto& spec : s_Orders) {
        if (missionName == spec.name) { pSpec = &spec; break; }
    }
    if (!pSpec) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }

    // Parse Destination
    AbstractClass* pDest = nullptr;
    bool hasDest = false;
    auto oToX = Context.GetObjectItem("ToX"), oToY = Context.GetObjectItem("ToY");
    auto oTo = Context.GetObjectItem("To");
    if (oToX && oToY && oToX.IsTypeNumber() && oToY.IsTypeNumber()) {
        if (!pSpec->destCellOk && !pSpec->targetOrDest) {
            ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
        }
        CellStruct cs { (short)oToX.GetInt(), (short)oToY.GetInt() };
        pDest = (AbstractClass*)MapClass::Instance->TryGetCellAt(cs);
        if (!pDest) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }
        hasDest = true;
    } else if (oTo && oTo.IsTypeNumber()) {
        if (!pSpec->destObjOk && !pSpec->targetOrDest) {
            ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
        }
        pDest = (AbstractClass*)(DWORD)oTo.GetInt();
        hasDest = true;
    }

    // Parse Target
    AbstractClass* pTarget = nullptr;
    bool hasTarget = false;
    auto oTarget = Context.GetObjectItem("Target");
    if (oTarget && oTarget.IsTypeNumber()) {
        pTarget = (AbstractClass*)(DWORD)oTarget.GetInt();
        hasTarget = true;
    }

    // Validate parameter requirements
    if (pSpec->targetOrDest) {
        // Attack: either Target or Destination required
        if (!hasTarget && !hasDest) {
            ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
        }
    } else {
        if (pSpec->needsDest && !hasDest) {
            ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
        }
    }

    // Execute
    if (pSpec->mission == Mission::Attack) {
        if (hasTarget) {
            pTechno->SetTarget(pTarget);
        } else {
            pTechno->SetDestination(pDest, true);
        }
    } else {
        if (hasDest) {
            pTechno->SetDestination(pDest, true);
        } else if (pSpec->mission == Mission::Stop) {
            pTechno->SetDestination(nullptr, true);
        }
        if (hasTarget) {
            pTechno->SetTarget(pTarget);
        }
    }

    pTechno->QueueMission(pSpec->mission, true);

    // Output
    std::string out = std::format("IssueOrder: {} addr=0x{:08X}", pSpec->name, addr);
    if (hasTarget) out += std::format(" target=0x{:08X}", (DWORD)pTarget);
    if (hasDest) {
        if (pDest->WhatAmI() == AbstractType::Cell)
            out += std::format(" toCell=({},{})", (int)((CellClass*)pDest)->MapCoords.X, (int)((CellClass*)pDest)->MapCoords.Y);
        else
            out += std::format(" to=0x{:08X}", (DWORD)pDest);
    }
    std::cout << out << std::endl;
    ECDebug::DoNotEcho();
}

// ------------------------------------------------------------------
//  SetDestination: set movement destination (without queuing a Mission)
//    -Address <0x...>  [-ToX <int> -ToY <int> | -To <0x...> | -Clear]
// ------------------------------------------------------------------
void __cdecl IHVerify_SetDestination(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    DWORD addr = (DWORD)oAddr.GetInt();
    auto pAbs = (AbstractClass*)addr;
    if (!pAbs || !IsPointerAlive(addr)) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }
    auto absType = pAbs->WhatAmI();
    if (absType != AbstractType::Unit && absType != AbstractType::Infantry &&
        absType != AbstractType::Building && absType != AbstractType::Aircraft) {
        ECDebug::ReturnStdError(ERROR_INVALID_FUNCTION); return;
    }
    auto pTechno = (TechnoClass*)pAbs;

    AbstractClass* pDest = nullptr;
    bool hasClear = false;
    auto oClear = Context.GetObjectItem("Clear");
    if (oClear && oClear.IsTypeBool()) hasClear = oClear.GetBool();

    auto oToX = Context.GetObjectItem("ToX"), oToY = Context.GetObjectItem("ToY");
    auto oTo = Context.GetObjectItem("To");

    if (hasClear) {
        pDest = nullptr;
    } else if (oToX && oToY && oToX.IsTypeNumber() && oToY.IsTypeNumber()) {
        CellStruct cs { (short)oToX.GetInt(), (short)oToY.GetInt() };
        pDest = (AbstractClass*)MapClass::Instance->TryGetCellAt(cs);
        if (!pDest) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }
    } else if (oTo && oTo.IsTypeNumber()) {
        pDest = (AbstractClass*)(DWORD)oTo.GetInt();
    } else {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }

    pTechno->SetDestination(pDest, true);

    if (pDest) {
        if (pDest->WhatAmI() == AbstractType::Cell)
            std::cout << std::format("SetDestination addr=0x{:08X} toCell=({},{})", addr,
                (int)((CellClass*)pDest)->MapCoords.X, (int)((CellClass*)pDest)->MapCoords.Y) << std::endl;
        else
            std::cout << std::format("SetDestination addr=0x{:08X} to=0x{:08X}", addr, (DWORD)pDest) << std::endl;
    } else {
        std::cout << std::format("SetDestination addr=0x{:08X} cleared", addr) << std::endl;
    }
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
    {"IssueOrder",    FuncInfo(IHVerify_IssueOrder,    FuncType::Action)},
    {"SetDestination",FuncInfo(IHVerify_SetDestination,FuncType::Action)},
};

FuncInfo* IHVerify_GetFunc(const char* Name, int Version)
{
    auto it = s_Funcs.find(Name);
    return (it != s_Funcs.end()) ? &it->second : nullptr;
}
