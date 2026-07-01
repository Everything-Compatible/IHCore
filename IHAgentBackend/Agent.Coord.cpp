// ════════════════════════════════════════════════════════════════════════════
//  Agent.Coord.cpp — v2 查询层（阶段 C）
//
//  所有查询均接受 `-SessionID` 并通过 `RequireSession(Context)` 进行验证
//  （该函数检查 _GameStarted + SessionID 查找）。返回 Techno/Cell
//  信息的查询通过阶段 A 中的 `IsTechnoVisibleTo` / `IsCellVisibleTo`
//  应用 visibility filtering（可见性过滤）。Cheat 会话可看到所有内容。
//
//  重命名（v1 → v2）：
//    GetHouseTechTree    → GetTechTree
//    GetTechnoTypeInfo   → GetTypeInfo
//    FindObjectsOnCell   → GetObjectsOnCell
//    FindAerialObjects   → GetAerialObjects
//    GetHouseProduction + GetFactoryInfo + ListAllFactories → GetProductionState
//
//  合并：
//    FindObjects + FindTechnoInfo + FindObjectsInRange → GetTechnoInRange
//
//  增强：
//    GetTechnoInfo 现在吸收 GetMovementState + GetBuildingStatus
//
//  新增：
//    GetForceDistribution, GetCurrentTime, GetPlayerViewRoute, FindPath（stub 桩函数）
//
//  Cell 操作（Shroud 迷雾/Tiberium 泰伯利亚/Radiation 辐射/conversions 转换/FrameRateInfo 帧率信息）是
//  辅助性的，保持不变——属于阶段 D 的范围。
// ════════════════════════════════════════════════════════════════════════════

#include "AgentBackend.h"

#include <format>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <EC.h>
#include <Debug.h>
#include <YRPP.h>
#include <MapClass.h>
#include <CellClass.h>
#include <CellSpread.h>
#include <AircraftClass.h>
#include <FootClass.h>
#include <LocomotionClass.h>
#include <TiberiumClass.h>
#include <FPSCounter.h>
#include <GameOptionsClass.h>
#include <EventClass.h>
#include <HouseClass.h>
#include <TechnoClass.h>
#include <BuildingClass.h>
#include <FactoryClass.h>
#include <TechnoTypeClass.h>
#include <UnitTypeClass.h>
#include <InfantryTypeClass.h>
#include <BuildingTypeClass.h>
#include <AircraftTypeClass.h>
#include <RulesClass.h>
#include <TacticalClass.h>
#include <Unsorted.h>
#include <ExtJson.h>

// ════════════════════════════════════════════════════════════════════════════
//  静态辅助函数（名称查找）
// ════════════════════════════════════════════════════════════════════════════

static const char* MissionName(int m) {
    static const char* names[] = {
        "Sleep","Attack","Move","QMove","Retreat","Guard","Sticky",
        "Enter","Capture","Eaten","Harvest","Area_Guard","Return",
        "Stop","Ambush","Hunt","Unload","Sabotage","Construction",
        "Selling","Repair","Rescue","Missile","Harmless","Open",
        "Patrol","ParadropApproach","ParadropOverfly","Wait",
        "AttackMove","SpyplaneApproach","SpyplaneOverfly"
    };
    return (m >= 0 && m <= 31) ? names[m] : "None";
}

static const char* ZoneName(int z) {
    static const char* names[] = {
        "Normal","Crusher","Destroyer","AmphibiousDestroyer",
        "AmphibiousCrusher","Amphibious","Subterrannean","Infantry","Fly"
    };
    return (z >= 0 && z <= 8) ? names[z] : "Unknown";
}

static const char* SpeedName(int s) {
    static const char* names[] = {
        "Foot","Track","Wheel","Hover","Winged","Float","Amphibious","FloatBeach"
    };
    return (s >= 0 && s <= 7) ? names[s] : "Unknown";
}

static const char* MoveResultName(int m) {
    static const char* names[] = {"OK","Cloak","MovingBlock","ClosedGate",
        "FriendlyDestroyable","Destroyable","Temp","No"};
    if (m >= 0 && m <= 7) return names[m];
    return "Unknown";
}

static const char* CanBuildResultName(int cbr)
{
    switch (cbr) {
        case  1: return "Buildable";
        case  0: return "Unbuildable";
        case -1: return "TemporarilyUnbuildable";
        default: return "Unknown";
    }
}

static const char* BStateName(int st)
{
    switch (st) {
        case 0: return "Construction";
        case 1: return "Idle";
        case 2: return "Active";
        case 3: return "Full";
        case 4: return "Aux1";
        case 5: return "Aux2";
        default: return "Unknown";
    }
}

static const char* BuildCatName(int cat)
{
    switch (cat) {
        case 0: return "DontCare";
        case 1: return "Tech";
        case 2: return "Resource";
        case 3: return "Power";
        case 4: return "Infrastructure";
        case 5: return "Combat";
        default: return "Unknown";
    }
}

static const char* AbsTypeName(AbstractType at)
{
    if (at == AbstractType::UnitType)     return "UnitType";
    if (at == AbstractType::InfantryType) return "InfantryType";
    if (at == AbstractType::BuildingType) return "BuildingType";
    if (at == AbstractType::AircraftType) return "AircraftType";
    if (at == AbstractType::Unit)         return "Unit";
    if (at == AbstractType::Infantry)     return "Infantry";
    if (at == AbstractType::Building)     return "Building";
    if (at == AbstractType::Aircraft)     return "Aircraft";
    if (at == AbstractType::House)        return "House";
    if (at == AbstractType::Factory)      return "Factory";
    return "Unknown";
}

static const char* GameSpeedName(int gs)
{
    switch (gs) {
        case 0:  return "Fastest";
        case 1:  return "Fast";
        case 2:  return "Medium";
        case 3:  return "Slow";
        case 4:  return "VerySlow";
        case 5:  return "Slowest";
        default: return "Unknown";
    }
}

// 通过 ID 字符串查找 TechnoType（不区分大小写）。由 GetTypeInfo 使用。
// Agent.Actions.cpp 直接使用 TechnoTypeClass::Find；此本地辅助函数
// 保留用于 GetTypeInfo 的不区分大小写匹配。
static TechnoTypeClass* FindTechnoTypeByID(const char* id)
{
    for (auto pItem : *TechnoTypeClass::Array) {
        if (pItem && !_strcmpi(pItem->ID, id))
            return pItem;
    }
    return nullptr;
}

// 查找驻军给定步兵的建筑（由 GetTechnoInfo 使用）。
static BuildingClass* FindGarrisonBuilding(InfantryClass* pInfantry)
{
    if (!pInfantry) return nullptr;
    HouseClass* pHouse = pInfantry->Owner;
    if (!pHouse) return nullptr;
    for (auto pBuilding : pHouse->Buildings)
    {
        for (auto& occ : pBuilding->Occupants)
        {
            if (occ == pInfantry)
                return pBuilding;
        }
    }
    return nullptr;
}

// 将 TechnoType 条目的 JSON 文本附加到 `result`（用于 GetTechTree 数组）。
static void AppendTypeEntry(std::string& result, const char* whatAmI, TechnoTypeClass* pType)
{
    JsonFile F;
    auto Obj = F.GetObj();
    Obj.AddString("WhatAmI", whatAmI);
    Obj.AddInt("WhatAmIInt", (int)pType->WhatAmI());
    Obj.AddString("ID", pType->ID);
    Obj.AddString("Name", pType->Name ? pType->Name : "");
    if (pType->UIName)
    {
        auto u8s = UTF16ToUTF8(pType->UIName);
        Obj.AddString("UIName", ~u8s);
    }
    else
    {
        Obj.AddString("UIName", "");
    }

    Obj.AddInt("Cost", pType->Cost);
    Obj.AddInt("Soylent", pType->Soylent);
    Obj.AddInt("Strength", pType->Strength);
    Obj.AddInt("BuildLimit", pType->BuildLimit);
    Obj.AddInt("Sight", pType->Sight);
    Obj.AddInt("Speed", pType->Speed);
    Obj.AddInt("Armor", (int)pType->Armor);
    Obj.AddInt("Passengers", pType->Passengers);

    Obj.AddBool("Naval", pType->Naval);
    Obj.AddBool("ResourceGatherer", pType->ResourceGatherer);
    Obj.AddBool("Subterranean", pType->IsSubterranean);
    Obj.AddBool("Underwater", pType->Underwater);
    Obj.AddBool("JumpJet", pType->JumpJet);

    auto Mov = pType->MovementZone;
    bool IsAmphibious = (Mov == MovementZone::Amphibious || Mov == MovementZone::AmphibiousCrusher || Mov == MovementZone::AmphibiousDestroyer);
    Obj.AddBool("IsAmphibious", IsAmphibious);

    Obj.AddDouble("SizeLimit", pType->SizeLimit);

    result += Obj.GetText();
}

// ════════════════════════════════════════════════════════════════════════════
//  重命名 / 接入 SessionID 的查询
// ════════════════════════════════════════════════════════════════════════════

// ---------- GetMapInfo（受门控；无需真正的可见性过滤）----------
void __cdecl IHAgentBackend_GetMapInfo(JsonObject Context)
{
    if (!RequireSession(Context)) return;
    auto& Inst = *MapClass::Instance;
    auto& Bnd = Inst.MapCoordBounds;

    JsonFile F;
    auto Obj = F.GetObj();
    Obj.AddInt("Width",  Inst.MapRect.Width);
    Obj.AddInt("Height", Inst.MapRect.Height);
    Obj.AddInt("Left",   Bnd.Left);
    Obj.AddInt("Top",    Bnd.Top);
    Obj.AddInt("Right",  Bnd.Right);
    Obj.AddInt("Bottom", Bnd.Bottom);

    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ---------- GetCellInfo（可见性过滤）----------
void __cdecl IHAgentBackend_GetCellInfo(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    CellStruct cs { (short)oX.GetInt(), (short)oY.GetInt() };
    CellClass* pCell = MapClass::Instance->TryGetCellAt(cs);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    if (!IsCellVisibleTo(pCell, pS)) {
        JsonFile F;
        auto Obj = F.GetObj();
        Obj.AddBool("Visible", false);
        cJSON* arr = cJSON_CreateIntArray(nullptr, 0);
        cJSON_AddItemToArray(arr, cJSON_CreateNumber((int)cs.X));
        cJSON_AddItemToArray(arr, cJSON_CreateNumber((int)cs.Y));
        Obj.AddObjectItem("Cell", arr);
        std::string result = Obj.GetText();
        std::cout << result << std::endl;
        ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
        ECDebug::DoNotEcho();
        return;
    }

    JsonFile F;
    auto Obj = F.GetObj();
    Obj.AddBool("Visible", true);
    Obj.AddInt("LandType", (int)pCell->LandType);
    {
        char buf[16];
        sprintf_s(buf, "0x%02X", (int)pCell->OccupationFlags);
        Obj.AddString("OccupationFlags", buf);
    }
    {
        char buf[16];
        sprintf_s(buf, "0x%02X", (int)pCell->AltOccupationFlags);
        Obj.AddString("AltOccupationFlags", buf);
    }
    Obj.AddBool("ContainsBridge", pCell->ContainsBridge());
    Obj.AddInt("SlopeIndex", pCell->SlopeIndex);

    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ---------- GetHouseInfo（可见性过滤，仅对自己 House 做聚类）----------
void __cdecl IHAgentBackend_GetHouseInfo(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    // 默认 House = 会话的 House；-House 可能覆盖此值。
    int idx = pS->House;
    auto oH = Context.GetObjectItem("House");
    if (oH && oH.IsTypeNumber()) idx = oH.GetInt();

    if (idx < 0 || idx >= (*HouseClass::Array).Count) {
        ECDebug::ReturnStdError(ERROR_INVALID_INDEX); return;
    }
    auto pHouse = (*HouseClass::Array)[idx];
    if (!pHouse) { ECDebug::ReturnStdError(ERROR_INVALID_DATA); return; }

    auto pSelf = HouseClass::Array->Items[pS->House];
    bool isOwnHouse    = (idx == pS->House);
    bool alliedToSelf = pSelf ? pSelf->IsAlliedWith(pHouse) : false;
    bool fullInfo      = isOwnHouse || alliedToSelf || pS->Cheat;

    if (!fullInfo) {
        // 对非同盟 House 提供有限信息
        JsonFile F;
        auto Obj = F.GetObj();
        Obj.AddInt("Index", idx);
        Obj.AddString("Name", pHouse->PlainName ? pHouse->PlainName : "");
        Obj.AddString("Faction", pHouse->Type ? pHouse->Type->ID : "(unknown)");
        Obj.AddBool("IsPlayer", pHouse->IsCurrentPlayer());
        Obj.AddBool("FullInfo", false);
        std::string result = Obj.GetText();
        std::cout << result << std::endl;
        ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
        ECDebug::DoNotEcho();
        return;
    }

    JsonFile F;
    auto Obj = F.GetObj();
    Obj.AddInt("Index", idx);
    Obj.AddString("Name", pHouse->PlainName ? pHouse->PlainName : "");
    Obj.AddString("ParentCountry", pHouse->Type ? pHouse->Type->ID : "(unknown)");
    Obj.AddString("Country",
        (pHouse->Type && pHouse->Type->ParentCountry)
            ? (const char*)pHouse->Type->ParentCountry : "(unknown)");
    Obj.AddInt("SideIndex", pHouse->SideIndex);
    Obj.AddString("Color",
        std::format("{},{},{}",
            (int)pHouse->Color.R, (int)pHouse->Color.G, (int)pHouse->Color.B));
    Obj.AddString("LaserColor",
        std::format("{},{},{}",
            (int)pHouse->LaserColor.R, (int)pHouse->LaserColor.G, (int)pHouse->LaserColor.B));
    Obj.AddInt("Money", pHouse->Available_Money());
    Obj.AddInt("OwnedUnits",     pHouse->OwnedUnits);
    Obj.AddInt("OwnedBuildings", pHouse->OwnedBuildings);
    Obj.AddInt("OwnedInfantry",  pHouse->OwnedInfantry);
    Obj.AddInt("OwnedAircraft",  pHouse->OwnedAircraft);
    Obj.AddBool("IsPlayer", pHouse->IsCurrentPlayer());
    Obj.AddInt("Player", pHouse->ArrayIndex);
    Obj.AddBool("FullInfo", true);

    // 聚类检测——仅对会话自己的 House 进行
    if (isOwnHouse)
    {
        std::unordered_map<int, std::vector<TechnoClass*>> cellMap;
        for (auto pT : *TechnoClass::Array)
        {
            if (!pT || pT->InLimbo) continue;
            auto wa = (int)pT->WhatAmI();
            if (wa != (int)AbstractType::Unit && wa != (int)AbstractType::Infantry) continue;
            auto pH = pT->GetOwningHouse();
            if (!pH || pH->ArrayIndex != idx) continue;

            auto mc = pT->GetMapCoords();
            int cellKey = ((int)mc.Y) * 10000 + (int)mc.X;
            cellMap[cellKey].push_back(pT);
        }

        std::unordered_set<int> reportedCells;
        std::string clusters = "[";
        bool firstCluster = true;

        for (auto& [cellKey, vec] : cellMap)
        {
            if (reportedCells.count(cellKey)) continue;
            if (vec.empty()) continue;

            int cx = cellKey % 10000;
            int cy = cellKey / 10000;

            int cellTotal = 0;
            std::unordered_map<std::string, int> typeCount;

            for (int dy = -1; dy <= 1; ++dy)
            {
                for (int dx = -1; dx <= 1; ++dx)
                {
                    int nk = (cy + dy) * 10000 + (cx + dx);
                    auto it = cellMap.find(nk);
                    if (it == cellMap.end()) continue;
                    for (auto pT : it->second)
                    {
                        auto pTT = pT->GetTechnoType();
                        if (pTT && pTT->ID[0])
                            typeCount[pTT->ID]++;
                        cellTotal++;
                    }
                }
            }

            if (cellTotal >= 5)
            {
                for (int dy = -1; dy <= 1; ++dy)
                    for (int dx = -1; dx <= 1; ++dx)
                        reportedCells.insert((cy + dy) * 10000 + (cx + dx));

                if (!firstCluster) clusters += ",";
                firstCluster = false;

                JsonFile cf;
                auto cObj = cf.GetObj();
                cJSON* carr = cJSON_CreateIntArray(nullptr, 0);
                cJSON_AddItemToArray(carr, cJSON_CreateNumber(cx));
                cJSON_AddItemToArray(carr, cJSON_CreateNumber(cy));
                cObj.AddObjectItem("cell", carr);
                cObj.AddInt("count", cellTotal);

                JsonFile typesObj;
                auto tObj = typesObj.GetObj();
                for (auto& [tid, cnt] : typeCount)
                    tObj.AddInt(tid, cnt);
                cObj.AddObjectItem("Types", std::move(typesObj));

                clusters += cObj.GetText();
            }
        }

        clusters += "]";
        // 将 clusters 数组作为原始字符串附加——JsonObject 无法轻松
        // 合并预构建的数组字符串，因此我们手动拼接。
        std::string result = Obj.GetText();
        // 去除尾部的 '}' 并附加 Clusters，然后重新闭合。
        if (!result.empty() && result.back() == '}') result.pop_back();
        result += ",\"Clusters\":";
        result += clusters;
        result += "}";

        std::cout << result << std::endl;
        ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
        ECDebug::DoNotEcho();
        return;
    }

    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ---------- GetTechnoInfo（可见性过滤；吸收 movement + building 状态）----------
void __cdecl IHAgentBackend_GetTechnoInfo(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oAddr = Context.GetObjectItem("Address");
    DWORD addr;
    if (!oAddr) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    if (oAddr.IsTypeString()) {
        std::string s = oAddr.GetString();
        try {
            addr = (DWORD)std::stoul(s, nullptr, 0);
        } catch (...) {
            addr = (DWORD)oAddr.GetInt();
        }
    } else if (oAddr.IsTypeNumber()) {
        addr = (DWORD)oAddr.GetInt();
    } else {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }

    auto pAbs = (AbstractClass*)addr;
    if (!pAbs || !IsPointerAlive(addr)) {
        ECDebug::ReturnStdError(ERROR_INVALID_DATA); return;
    }
    auto wa = pAbs->WhatAmI();
    if (wa != AbstractType::Infantry && wa != AbstractType::Unit &&
        wa != AbstractType::Building && wa != AbstractType::Aircraft) {
        ECDebug::ReturnStdError(ERROR_INVALID_DATA); return;
    }

    auto pTechno = (TechnoClass*)pAbs;

    // 可见性检查——如果不可见，返回有限的响应
    if (!IsTechnoVisibleTo(pTechno, pS)) {
        JsonFile F;
        auto Obj = F.GetObj();
        Obj.AddBool("Visible", false);
        char addrBuf[16];
        sprintf_s(addrBuf, "0x%08X", addr);
        Obj.AddString("Address", addrBuf);
        Obj.AddNull("LastKnownCell");
        std::string result = Obj.GetText();
        std::cout << result << std::endl;
        ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
        ECDebug::DoNotEcho();
        return;
    }

    auto pType = pTechno->GetTechnoType();
    if (!pType) { ECDebug::ReturnStdError(ERROR_INVALID_DATA); return; }

    int WhatAmI = (int)pTechno->WhatAmI();
    const char* WhatAmIStr = pTechno->GetClassName();
    DWORD UniqueID = pTechno->UniqueID;
    CoordStruct Coords = pTechno->GetCoords();
    int CoordsInt[3] = { Coords.X, Coords.Y, Coords.Z };
    int HouseIndex = pTechno->GetOwningHouseIndex();
    const char* TypeId = pType->ID;
    const char* TypeName = pType->Name ? pType->Name : "";
    const wchar_t* TypeUIName = pType->UIName ? pType->UIName : L"";
    std::string TypeUINameU8 = ~UTF16ToUTF8(TypeUIName);
    int Health = pTechno->Health;
    int MaxHealth = pType->Strength;
    int Ammo = pTechno->Ammo;
    int MaxAmmo = pType->Ammo;
    bool InLimbo = pTechno->InLimbo;
    bool Controllable = pTechno->IsControllable();
    int Mission = (int)pTechno->GetCurrentMission();
    bool Selected = pTechno->IsSelected;
    bool isFoot = (WhatAmI != (int)AbstractType::Building);

    JsonFile F;
    auto Obj = F.GetObj();
    Obj.AddBool("Visible", true);
    Obj.AddInt("WhatAmI", WhatAmI);
    Obj.AddString("WhatAmIStr", WhatAmIStr);
    Obj.AddInt("UniqueID", UniqueID);
    Obj.AddObjectItem("Coords", cJSON_CreateIntArray(CoordsInt, 3));
    Obj.AddInt("HouseIndex", HouseIndex);
    Obj.AddString("TypeId", TypeId);
    Obj.AddString("TypeName", TypeName);
    Obj.AddString("TypeUIName", TypeUINameU8);
    Obj.AddInt("Health", Health);
    Obj.AddInt("MaxHealth", MaxHealth);
    Obj.AddInt("Ammo", Ammo);
    Obj.AddInt("MaxAmmo", MaxAmmo);
    Obj.AddInt("Mission", Mission);
    Obj.AddBool("InLimbo", InLimbo);
    Obj.AddBool("Controllable", Controllable);
    Obj.AddBool("Selected", Selected);

    // ── Movement 状态（从 GetMovementState 吸收）──
    int curMis = (int)pTechno->CurrentMission;
    int queMis = (int)pTechno->QueuedMission;

    bool isMoving = false;
    int headX = 0, headY = 0, headZ = 0;
    std::string destAddr = "0x00000000";
    bool destHasCell = false;
    int destCellX = 0, destCellY = 0;
    double speedPct = 1.0;
    const char* zoneStr = "Unknown";
    const char* speedStr = "Unknown";

    if (isFoot) {
        auto pFoot = (FootClass*)pTechno;
        speedPct = pFoot->SpeedPercentage;

        auto pTType = pTechno->GetTechnoType();
        if (pTType) {
            zoneStr  = ZoneName((int)pTType->MovementZone);
            speedStr = SpeedName((int)pTType->SpeedType);
        }

        if (pFoot->Locomotor) {
            isMoving = pFoot->Locomotor->Is_Moving();
            CoordStruct htc;
            pFoot->Locomotor->Head_To_Coord(&htc);
            headX = htc.X; headY = htc.Y; headZ = htc.Z;
        }

        if (pFoot->Destination) {
            destAddr = std::format("0x{:08X}", (DWORD)pFoot->Destination);
            auto dWhat = pFoot->Destination->WhatAmI();
            if (dWhat == AbstractType::Cell) {
                auto& mc = ((CellClass*)pFoot->Destination)->MapCoords;
                destHasCell = true;
                destCellX = (int)mc.X;
                destCellY = (int)mc.Y;
            } else if (dWhat == AbstractType::Unit || dWhat == AbstractType::Infantry ||
                       dWhat == AbstractType::Building || dWhat == AbstractType::Aircraft ||
                       dWhat == AbstractType::House) {
                auto cell = ((ObjectClass*)pFoot->Destination)->GetMapCoords();
                destHasCell = true;
                destCellX = (int)cell.X;
                destCellY = (int)cell.Y;
            }
        }
    }

    std::string targetStr = "0x00000000";
    if (pTechno->Target)
        targetStr = std::format("0x{:08X}", (DWORD)pTechno->Target);

    // CurrentMission 对象
    {
        JsonFile mf;
        auto mObj = mf.GetObj();
        mObj.AddInt("value", curMis);
        mObj.AddString("name", MissionName(curMis));
        Obj.AddObjectItem("CurrentMission", std::move(mf));
    }
    // QueuedMission 对象
    {
        JsonFile mf;
        auto mObj = mf.GetObj();
        mObj.AddInt("value", queMis);
        mObj.AddString("name", MissionName(queMis));
        Obj.AddObjectItem("QueuedMission", std::move(mf));
    }
    Obj.AddInt("MissionStatus", pTechno->MissionStatus);
    Obj.AddBool("IsMoving", isMoving);
    Obj.AddBool("IsInAir", pTechno->InAir ? true : false);

    // Destination 对象
    {
        JsonFile df;
        auto dObj = df.GetObj();
        dObj.AddString("Address", destAddr);
        if (destHasCell) {
            dObj.AddInt("CellX", destCellX);
            dObj.AddInt("CellY", destCellY);
        } else {
            dObj.AddNull("CellX");
            dObj.AddNull("CellY");
        }
        Obj.AddObjectItem("Destination", std::move(df));
    }

    Obj.AddString("Target", targetStr);

    // HeadToCoord 对象
    {
        JsonFile hf;
        auto hObj = hf.GetObj();
        hObj.AddInt("X", headX);
        hObj.AddInt("Y", headY);
        hObj.AddInt("Z", headZ);
        Obj.AddObjectItem("HeadToCoord", std::move(hf));
    }

    Obj.AddDouble("SpeedPercentage", speedPct);
    Obj.AddString("MovementZone", zoneStr);
    Obj.AddString("SpeedType", speedStr);

    // ── Building 状态（从 GetBuildingStatus 吸收；仅适用于 buildings）──
    if (WhatAmI == (int)AbstractType::Building)
    {
        auto pBld = (BuildingClass*)pTechno;
        bool factorySuspended = false;
        bool factoryIsManual  = false;
        DWORD factoryAddr = 0;
        if (pBld->Factory) {
            factorySuspended = pBld->Factory->IsSuspended;
            factoryIsManual  = pBld->Factory->IsManual;
            factoryAddr      = (DWORD)pBld->Factory;
        }

        auto mapCoords = pBld->GetMapCoords();

        Obj.AddBool("ActuallyPlacedOnMap", pBld->ActuallyPlacedOnMap);
        Obj.AddBool("IsReadyToCommence", pBld->IsReadyToCommence);
        Obj.AddBool("HasBuildUp", pBld->HasBuildUp);
        Obj.AddInt("BState", pBld->BState);
        Obj.AddString("BStateName", BStateName(pBld->BState));
        Obj.AddInt("QueueBState", pBld->QueueBState);

        char fbuf[16];
        sprintf_s(fbuf, "0x%08X", factoryAddr);
        Obj.AddString("Factory", fbuf);
        Obj.AddBool("FactorySuspended", factorySuspended);
        Obj.AddBool("FactoryManual", factoryIsManual);

        Obj.AddBool("HasPower", pBld->HasPower);
        Obj.AddInt("UpgradeLevel", pBld->UpgradeLevel);
        Obj.AddBool("BeingProduced", pBld->BeingProduced);
        Obj.AddBool("ShouldRebuild", pBld->ShouldRebuild);
        Obj.AddBool("HasBeenCaptured", pBld->HasBeenCaptured);
        Obj.AddBool("IsDamaged", pBld->IsDamaged);
    }

    // ── Transporter 运输器 / Passengers 乘客 / Occupants 驻军 / Garrison 驻扎建筑 ──
    if (pTechno->Transporter)
        Obj.AddString("Transporter", std::format("0x{:08X}", (DWORD)pTechno->Transporter));
    else
        Obj.AddNull("Transporter");

    if (pTechno->Passengers.NumPassengers && pTechno->Passengers.FirstPassenger)
    {
        std::vector<std::string> passengerAddrs;
        auto pPassenger = pTechno->Passengers.FirstPassenger;
        while (pPassenger)
        {
            passengerAddrs.push_back(std::format("0x{:08X}", (DWORD)pPassenger));
            pPassenger = abstract_cast<FootClass*>(pPassenger->NextObject);
        }
        Obj.AddArrayString("Passengers", passengerAddrs);
    }
    else
        Obj.AddNull("Passengers");

    if (auto pBld = abstract_cast<BuildingClass*>(pTechno); pBld)
    {
        auto pBldType = pBld->Type;
        if (pBldType && pBldType->BridgeRepairHut)
        {
            if (MapClass::Instance->IsLinkedBridgeDestroyed(pBld->GetMapCoords()))
                Obj.AddString("BridgeStatus", "Destroyed");
            else
                Obj.AddString("BridgeStatus", "Intact");
        }
        else
            Obj.AddNull("BridgeStatus");

        std::vector<std::string> occupantAddrs;
        for (auto pOcc : pBld->Occupants)
            occupantAddrs.push_back(std::format("0x{:08X}", (DWORD)pOcc));
        if (!occupantAddrs.empty())
            Obj.AddArrayString("Occupants", occupantAddrs);
        else
            Obj.AddNull("Occupants");
    }
    else
    {
        Obj.AddNull("Occupants");
        Obj.AddNull("BridgeStatus");
    }

    if (auto pInf = abstract_cast<InfantryClass*>(pTechno); pInf)
    {
        if (pInf->InLimbo && pInf->Type->Occupier)
        {
            auto pGarrison = FindGarrisonBuilding(pInf);
            if (pGarrison)
                Obj.AddString("GarrisonBuilding", std::format("0x{:08X}", (DWORD)pGarrison));
            else
                Obj.AddNull("GarrisonBuilding");
        }
        else
            Obj.AddNull("GarrisonBuilding");
    }
    else
        Obj.AddNull("GarrisonBuilding");

    auto HealthStatus = pTechno->GetHealthStatus();
    switch (HealthStatus)
    {
    case HealthState::Green:
        Obj.AddString("HealthStatus", "Green"); break;
    case HealthState::Yellow:
        Obj.AddString("HealthStatus", "Yellow"); break;
    case HealthState::Red:
        Obj.AddString("HealthStatus", "Red"); break;
    default:
        Obj.AddString("HealthStatus", "Unknown");
    }

    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ---------- GetTechTree（从 GetHouseTechTree 重命名）----------
void __cdecl IHAgentBackend_GetTechTree(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    int hidx = pS->House;
    auto oH = Context.GetObjectItem("House");
    if (oH && oH.IsTypeNumber()) hidx = oH.GetInt();

    if (hidx < 0 || hidx >= (*HouseClass::Array).Count) {
        ECDebug::ReturnStdError(ERROR_INVALID_INDEX); return;
    }
    auto pHouse = (*HouseClass::Array)[hidx];
    if (!pHouse) { ECDebug::ReturnStdError(ERROR_INVALID_DATA); return; }

    // 授权检查：查询非自己 House 需要 Cheat 或同盟关系
    if (hidx != pS->House && !pS->Cheat) {
        auto pSelf = HouseClass::Array->Items[pS->House];
        if (!pSelf || !pSelf->IsAlliedWith(pHouse)) {
            ECDebug::ReturnStdError(ERROR_ACCESS_DENIED); return;
        }
    }

    std::string result = "{\"House\":";
    result += std::to_string(hidx);
    result += ",\"Buildable\":[";
    result.reserve(65536);
    int total = 0;

    for (auto pType : *UnitTypeClass::Array) {
        if (!pType) continue;
        if ((int)pHouse->CanBuild(pType, false, false) >= 1) {
            if (total > 0) result += ",";
            AppendTypeEntry(result, "UnitType", pType);
            total++;
        }
    }
    for (auto pType : *InfantryTypeClass::Array) {
        if (!pType) continue;
        if ((int)pHouse->CanBuild(pType, false, false) >= 1) {
            if (total > 0) result += ",";
            AppendTypeEntry(result, "InfantryType", pType);
            total++;
        }
    }
    for (auto pType : *BuildingTypeClass::Array) {
        if (!pType) continue;
        if ((int)pHouse->CanBuild(pType, false, false) >= 1) {
            if (total > 0) result += ",";
            AppendTypeEntry(result, "BuildingType", pType);
            total++;
        }
    }
    for (auto pType : *AircraftTypeClass::Array) {
        if (!pType) continue;
        if ((int)pHouse->CanBuild(pType, false, false) >= 1) {
            if (total > 0) result += ",";
            AppendTypeEntry(result, "AircraftType", pType);
            total++;
        }
    }

    result += std::format("],\"TotalCount\":{}}}", total);
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ---------- GetTypeInfo（从 GetTechnoTypeInfo 重命名；静态信息，无可见性）----------
void __cdecl IHAgentBackend_GetTypeInfo(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oType = Context.GetObjectItem("TypeID");
    if (!oType || !oType.IsTypeString()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    auto typeStr = oType.GetString();
    TechnoTypeClass* pType = FindTechnoTypeByID(typeStr.c_str());
    if (!pType) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }

    JsonFile F;
    auto Obj = F.GetObj();
    Obj.AddString("TypeID", pType->ID);
    Obj.AddString("Name", pType->Name ? pType->Name : "");
    Obj.AddString("WhatAmI", AbsTypeName(pType->WhatAmI()));
    Obj.AddInt("WhatAmIInt", (int)pType->WhatAmI());
    Obj.AddInt("Cost", pType->Cost);
    Obj.AddInt("Soylent", pType->Soylent);
    Obj.AddInt("Strength", pType->Strength);
    Obj.AddInt("Armor", (int)pType->Armor);
    Obj.AddInt("TechLevel", pType->TechLevel);
    Obj.AddInt("BuildLimit", pType->BuildLimit);
    Obj.AddInt("Sight", pType->Sight);
    Obj.AddInt("Speed", pType->Speed);
    Obj.AddInt("ROT", pType->ROT);
    Obj.AddInt("Passengers", pType->Passengers);
    Obj.AddInt("Storage", pType->Storage);
    Obj.AddInt("Weight", pType->Weight);
    Obj.AddInt("PhysicalSize", pType->PhysicalSize);
    Obj.AddBool("Naval", pType->Naval);
    Obj.AddBool("Organic", pType->Organic);
    Obj.AddBool("Cloakable", pType->Cloakable);
    Obj.AddBool("Crusher", pType->Crusher);
    Obj.AddBool("OmniCrusher", pType->OmniCrusher);
    Obj.AddBool("Repairable", pType->Repairable);
    Obj.AddBool("Crewed", pType->Crewed);
    Obj.AddDouble("BuildTimeMultiplier", (double)pType->BuildTimeMultiplier);
    Obj.AddInt("ThreatPosed", pType->ThreatPosed);
    Obj.AddInt("Points", pType->Points);
    Obj.AddBool("IsTrain", pType->IsTrain);
    Obj.AddBool("ResourceGatherer", pType->ResourceGatherer);
    Obj.AddBool("CanBeHidden", pType->CanBeHidden);

    if (pType->WhatAmI() == AbstractType::BuildingType) {
        auto pBT = (BuildingTypeClass*)pType;
        Obj.AddString("BuildCat", BuildCatName((int)pBT->BuildCat));
        Obj.AddInt("BuildCatInt", (int)pBT->BuildCat);
    } else {
        Obj.AddString("BuildCat", "N/A");
        Obj.AddInt("BuildCatInt", -1);
    }

    Obj.AddInt("TurretCount", pType->TurretCount);
    Obj.AddInt("WeaponCount", pType->WeaponCount);
    Obj.AddBool("Subterranean", pType->IsSubterranean);
    Obj.AddBool("Underwater", pType->Underwater);
    Obj.AddBool("JumpJet", pType->JumpJet);
    Obj.AddInt("GuardRange", pType->GuardRange);
    Obj.AddInt("Category", (int)pType->Category);
    Obj.AddString("SpeedType", SpeedName((int)pType->SpeedType));
    Obj.AddString("MovementZone", ZoneName((int)pType->MovementZone));
    Obj.AddInt("MovementZoneInt", (int)pType->MovementZone);
    Obj.AddInt("SpeedTypeInt", (int)pType->SpeedType);
    Obj.AddInt("DeployTime", pType->DeployTime);
    Obj.AddInt("UndeployDelay", pType->UndeployDelay);

    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ---------- GetObjectsOnCell（从 FindObjectsOnCell 重命名；可见性过滤）----------
void __cdecl IHAgentBackend_GetObjectsOnCell(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    CellStruct cs { (short)oX.GetInt(), (short)oY.GetInt() };
    CellClass* pCell = MapClass::Instance->TryGetCellAt(cs);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    std::string result = "[";
    bool first = true;
    for (auto pObj = pCell->FirstObject; pObj; pObj = pObj->NextObject)
    {
        // 仅返回 Techno 对象；跳过非 Techno 条目
        auto wa = pObj->WhatAmI();
        if (wa != AbstractType::Unit && wa != AbstractType::Infantry &&
            wa != AbstractType::Building && wa != AbstractType::Aircraft)
            continue;

        auto pTechno = (TechnoClass*)pObj;
        if (!IsTechnoVisibleTo(pTechno, pS)) continue;

        if (!first) result += ",";
        first = false;
        result += std::format("\"0x{:08X}\"", (DWORD)pObj);
    }
    result += "]";
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ---------- GetAerialObjects（从 FindAerialObjects 重命名；可见性过滤）----------
void __cdecl IHAgentBackend_GetAerialObjects(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    short cx = (short)oX.GetInt(), cy = (short)oY.GetInt();
    auto oR = Context.GetObjectItem("Range");
    int range = 0;
    if (oR && oR.IsTypeNumber()) range = oR.GetInt();
    int rangeCells = range / 256 + 1;
    if (rangeCells > 50) rangeCells = 50;

    std::string result = "[";
    bool first = true;
    for (auto pAir : *AircraftClass::Array)
    {
        if (!pAir || pAir->InLimbo) continue;
        CellStruct airCell = pAir->GetMapCoords();
        if (!MapClass::Instance->IsWithinUsableArea(airCell, false)) continue;
        int dx = (int)airCell.X - (int)cx;
        int dy = (int)airCell.Y - (int)cy;
        if (abs(dx) <= rangeCells && abs(dy) <= rangeCells)
        {
            if (!IsTechnoVisibleTo((TechnoClass*)pAir, pS)) continue;
            if (!first) result += ",";
            first = false;
            result += std::format(R"({{"addr":"0x{:08X}","whatami":2}})", (DWORD)pAir);
        }
    }
    result += "]";
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  合并查询：GetTechnoInRange
//  替代 FindObjects + FindTechnoInfo + FindObjectsInRange。
//
//  参数：-SessionID -X -Y -Range [-WhatAmI Unit|Infantry|Building|Aircraft]
//          [-House <int>] [-Type <str>] [-Limit <int>] [-Offset <int>]
//  返回 JSON 数组：{addr,whatami,whatami_str,type,hp,maxhp,mission,
//          cell:[X,Y],house,selected,inlimbo}
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_GetTechnoInRange(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    short cx = (short)oX.GetInt(), cy = (short)oY.GetInt();

    auto oR = Context.GetObjectItem("Range");
    int range = 0;
    if (oR && oR.IsTypeNumber()) range = oR.GetInt();
    if (range < 0) range = 0;
    if (range > 50) range = 50;

    int filterAbs = -1;
    auto oWA = Context.GetObjectItem("WhatAmI");
    if (oWA && oWA.IsTypeString()) {
        std::string swa = oWA.GetString();
        if (swa == "Unit" || swa == "UnitType")         filterAbs = (int)AbstractType::Unit;
        else if (swa == "Infantry" || swa == "InfantryType") filterAbs = (int)AbstractType::Infantry;
        else if (swa == "Building" || swa == "BuildingType") filterAbs = (int)AbstractType::Building;
        else if (swa == "Aircraft" || swa == "AircraftType") filterAbs = (int)AbstractType::Aircraft;
    }

    int filterHouse = -1;
    auto oH = Context.GetObjectItem("House");
    if (oH && oH.IsTypeNumber()) filterHouse = oH.GetInt();

    std::string filterType;
    auto oT = Context.GetObjectItem("Type");
    if (oT && oT.IsTypeString()) filterType = oT.GetString();

    int limit = 50;
    auto oL = Context.GetObjectItem("Limit");
    if (oL && oL.IsTypeNumber()) limit = oL.GetInt();
    if (limit > 500) limit = 500;
    if (limit < 0) limit = 0;

    int offset = 0;
    auto oO = Context.GetObjectItem("Offset");
    if (oO && oO.IsTypeNumber()) offset = oO.GetInt();
    if (offset < 0) offset = 0;

    auto count = CellSpread::NumCells((unsigned)range);
    std::string result = "[";
    result.reserve(8192);
    int skipped = 0;
    int collected = 0;

    for (size_t i = 0; i < count && collected < limit; i++)
    {
        auto offset_c = CellSpread::GetCell(i);
        CellStruct cs { (short)(cx + offset_c.X), (short)(cy + offset_c.Y) };
        CellClass* pCell = MapClass::Instance->TryGetCellAt(cs);
        if (!pCell) continue;

        for (auto pObj = pCell->FirstObject; pObj; pObj = pObj->NextObject)
        {
            // 跳过非 Techno 的 FirstObject 条目
            auto wa = pObj->WhatAmI();
            if (wa != AbstractType::Unit && wa != AbstractType::Infantry &&
                wa != AbstractType::Building && wa != AbstractType::Aircraft)
                continue;

            auto pTechno = (TechnoClass*)pObj;
            if (pTechno->InLimbo) continue;

            // 可见性过滤
            if (!IsTechnoVisibleTo(pTechno, pS)) continue;

            int absType = (int)wa;
            if (filterAbs >= 0 && absType != filterAbs) continue;

            if (filterHouse >= 0) {
                auto pHouse = pTechno->GetOwningHouse();
                if (!pHouse || pHouse->ArrayIndex != filterHouse) continue;
            }

            auto pType = pTechno->GetTechnoType();
            if (!filterType.empty()) {
                if (!pType || filterType != std::string(pType->ID)) continue;
            }

            if (skipped < offset) { skipped++; continue; }

            auto cell = pTechno->GetMapCoords();
            int hp = pTechno->Health;
            int maxhp = pType ? pType->Strength : 0;
            int misInt = (int)pTechno->GetCurrentMission();
            const char* waStr = pTechno->GetClassName();
            int houseIdx = -1;
            auto pHouse = pTechno->GetOwningHouse();
            if (pHouse) houseIdx = pHouse->ArrayIndex;

            if (collected > 0) result += ",";
            result += std::format(
                R"({{"addr":"0x{:08X}","whatami":{},"whatami_str":"{}","type":"{}",)"
                R"("hp":{},"maxhp":{},"mission":{},)"
                R"("cell":[{},{}],"house":{},)"
                R"("selected":{},"inlimbo":{}}})",
                (DWORD)pTechno, absType, waStr,
                pType ? pType->ID : "",
                hp, maxhp, misInt,
                (int)cell.X, (int)cell.Y, houseIdx,
                pTechno->IsSelected ? "true" : "false",
                pTechno->InLimbo ? "true" : "false"
            );
            collected++;
            if (collected >= limit) break;
        }
    }

    result += "]";
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  GetProductionState
//  替代 GetHouseProduction + GetFactoryInfo + ListAllFactories。
//
//  枚举会话 House 中 Factory != nullptr 的建筑，
//  返回每个工厂的状态以及 House 的 ProducingXxxTypeIndex。
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_GetProductionState(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto pHouse = HouseClass::Array->Items[pS->House];
    if (!pHouse) { ECDebug::ReturnStdError(ERROR_INVALID_DATA); return; }

    auto resolveTypeID = [](int idx, AbstractType at) -> std::string {
        if (idx < 0) return "";
        auto pTT = TechnoTypeClass::GetByTypeAndIndex(at, idx);
        return pTT ? std::string(pTT->ID) : "";
    };

    JsonFile F;
    auto Obj = F.GetObj();
    Obj.AddInt("House", pS->House);

    // 解析 ProducingXxxTypeIndex → type ID
    auto addProducing = [&](const char* key, int idx, AbstractType at) {
        std::string id = resolveTypeID(idx, at);
        if (id.empty()) Obj.AddNull(key);
        else            Obj.AddString(key, id);
    };
    addProducing("ProducingBuildingTypeID", pHouse->ProducingBuildingTypeIndex, AbstractType::BuildingType);
    addProducing("ProducingUnitTypeID",     pHouse->ProducingUnitTypeIndex,     AbstractType::UnitType);
    addProducing("ProducingInfantryTypeID", pHouse->ProducingInfantryTypeIndex, AbstractType::InfantryType);
    addProducing("ProducingAircraftTypeID", pHouse->ProducingAircraftTypeIndex, AbstractType::AircraftType);

    Obj.AddInt("ProducingBuildingTypeIndex", pHouse->ProducingBuildingTypeIndex);
    Obj.AddInt("ProducingUnitTypeIndex",     pHouse->ProducingUnitTypeIndex);
    Obj.AddInt("ProducingInfantryTypeIndex", pHouse->ProducingInfantryTypeIndex);
    Obj.AddInt("ProducingAircraftTypeIndex", pHouse->ProducingAircraftTypeIndex);

    // Factories 数组——构建为拼接的对象文本字符串
    std::string factories = "[";
    bool first = true;
    for (auto pBld : pHouse->Buildings)
    {
        if (!pBld) continue;
        auto pFact = pBld->Factory;
        if (!pFact) continue;

        if (!first) factories += ",";
        first = false;

        JsonFile ff;
        auto fObj = ff.GetObj();
        char addrBuf[16];
        sprintf_s(addrBuf, "0x%08X", (DWORD)pBld);
        fObj.AddString("Address", addrBuf);
        fObj.AddString("TypeID", pBld->Type ? pBld->Type->ID : "");

        // 队列：factory 的 QueuedObjects（TechnoTypeClass* 列表）
        std::vector<std::string> queueIds;
        int qCount = pFact->QueuedObjects.Count;
        if (qCount > 0 && qCount < 100) {
            for (int i = 0; i < qCount; i++) {
                auto pQ = pFact->QueuedObjects.Items[i];
                if (pQ) queueIds.push_back(pQ->ID);
            }
        }
        fObj.AddArrayString("Queue", queueIds);

        fObj.AddInt("Progress", pFact->GetProgress());
        fObj.AddBool("Suspended", pFact->IsSuspended);
        // IsPrimary：匹配 House 的 Primary_ForXxx 指针之一
        bool isPrimary = (pFact == pHouse->Primary_ForBuildings ||
                         pFact == pHouse->Primary_ForVehicles  ||
                         pFact == pHouse->Primary_ForShips     ||
                         pFact == pHouse->Primary_ForInfantry  ||
                         pFact == pHouse->Primary_ForAircraft  ||
                         pFact == pHouse->Primary_ForDefenses);
        fObj.AddBool("IsPrimary", isPrimary);
        fObj.AddBool("IsManual", pFact->IsManual);
        fObj.AddBool("OnHold", pFact->OnHold);
        fObj.AddBool("IsDone", pFact->IsDone());
        fObj.AddInt("CostPerStep", pFact->GetCostPerStep());
        fObj.AddInt("Balance", pFact->Balance);
        fObj.AddInt("SpecialItem", pFact->SpecialItem);

        factories += fObj.GetText();
    }
    factories += "]";

    // 将 Factories 数组拼接到对象文本中
    std::string result = Obj.GetText();
    if (!result.empty() && result.back() == '}') result.pop_back();
    result += ",\"Factories\":";
    result += factories;
    result += "}";

    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  新增查询
// ════════════════════════════════════════════════════════════════════════════

// ---------- GetForceDistribution ----------
//  -SessionID [-House <int>]（默认使用会话的 House）
//  将 House 的 Technos 按类型 × 3×3 空间区域分桶统计。
void __cdecl IHAgentBackend_GetForceDistribution(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    int targetHouse = pS->House;
    auto oH = Context.GetObjectItem("House");
    if (oH && oH.IsTypeNumber()) targetHouse = oH.GetInt();
    if (targetHouse < 0 || targetHouse >= HouseClass::Array->Count) {
        ECDebug::ReturnStdError(ERROR_INVALID_INDEX); return;
    }

    auto& bnd = MapClass::Instance->MapCoordBounds;
    int minX = bnd.Left, maxX = bnd.Right;
    int minY = bnd.Top,  maxY = bnd.Bottom;
    int spanX = (maxX > minX) ? (maxX - minX) : 1;
    int spanY = (maxY > minY) ? (maxY - minY) : 1;

    auto regionOf = [&](int x, int y) -> std::pair<int,int> {
        int rx = (int)((long)(x - minX) * 3 / spanX);
        int ry = (int)((long)(y - minY) * 3 / spanY);
        if (rx < 0) rx = 0; if (rx > 2) rx = 2;
        if (ry < 0) ry = 0; if (ry > 2) ry = 2;
        return { rx, ry };
    };

    int total = 0;
    int byTypeInfantry = 0, byTypeUnit = 0, byTypeBuilding = 0, byTypeAircraft = 0;

    // 3×3 = 9 个区域；索引 = ry*3 + rx
    struct RegionBucket {
        int infantry=0, unit=0, building=0, aircraft=0, total=0;
    };
    RegionBucket regions[9];

    for (auto pT : *TechnoClass::Array)
    {
        if (!pT || pT->InLimbo) continue;
        if (pT->GetOwningHouseIndex() != targetHouse) continue;

        auto wa = pT->WhatAmI();
        auto mc = pT->GetMapCoords();
        auto [rx, ry] = regionOf((int)mc.X, (int)mc.Y);
        int rIdx = ry * 3 + rx;
        if (rIdx < 0 || rIdx >= 9) continue;

        total++;
        regions[rIdx].total++;

        if (wa == AbstractType::Infantry) {
            byTypeInfantry++; regions[rIdx].infantry++;
        } else if (wa == AbstractType::Unit) {
            byTypeUnit++; regions[rIdx].unit++;
        } else if (wa == AbstractType::Building) {
            byTypeBuilding++; regions[rIdx].building++;
        } else if (wa == AbstractType::Aircraft) {
            byTypeAircraft++; regions[rIdx].aircraft++;
        }
    }

    JsonFile F;
    auto Obj = F.GetObj();
    Obj.AddInt("House", targetHouse);
    Obj.AddInt("Total", total);

    {
        JsonFile bt;
        auto btObj = bt.GetObj();
        btObj.AddInt("Infantry", byTypeInfantry);
        btObj.AddInt("Unit",      byTypeUnit);
        btObj.AddInt("Building",  byTypeBuilding);
        btObj.AddInt("Aircraft",  byTypeAircraft);
        Obj.AddObjectItem("ByType", std::move(bt));
    }

    // ByRegion 数组——按顺序 (0,0)..(2,2) 共 9 个条目
    std::string byRegion = "[";
    for (int ry = 0; ry < 3; ++ry) {
        for (int rx = 0; rx < 3; ++rx) {
            int rIdx = ry * 3 + rx;
            if (rx != 0 || ry != 0) byRegion += ",";
            JsonFile rf;
            auto rObj = rf.GetObj();
            cJSON* rarr = cJSON_CreateIntArray(nullptr, 0);
            cJSON_AddItemToArray(rarr, cJSON_CreateNumber(rx));
            cJSON_AddItemToArray(rarr, cJSON_CreateNumber(ry));
            rObj.AddObjectItem("Region", rarr);
            rObj.AddInt("Infantry", regions[rIdx].infantry);
            rObj.AddInt("Unit",      regions[rIdx].unit);
            rObj.AddInt("Building",  regions[rIdx].building);
            rObj.AddInt("Aircraft",  regions[rIdx].aircraft);
            rObj.AddInt("Total",     regions[rIdx].total);
            byRegion += rObj.GetText();
        }
    }
    byRegion += "]";

    std::string result = Obj.GetText();
    if (!result.empty() && result.back() == '}') result.pop_back();
    result += ",\"ByRegion\":";
    result += byRegion;
    result += "}";

    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ---------- GetCurrentTime ----------
void __cdecl IHAgentBackend_GetCurrentTime(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    int frame = Unsorted::CurrentFrame;
    double gameSeconds = (double)frame / 15.0;

    JsonFile F;
    auto Obj = F.GetObj();
    Obj.AddInt("Frame", frame);
    Obj.AddDouble("GameSeconds", gameSeconds);

    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ---------- GetPlayerViewRoute ----------
//  -SessionID -FromTime <double> -ToTime <double>
//  过滤会话的 ViewHistory（Frame 在 [FromTime*15, ToTime*15] 范围内）。
void __cdecl IHAgentBackend_GetPlayerViewRoute(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oFrom = Context.GetObjectItem("FromTime");
    auto oTo   = Context.GetObjectItem("ToTime");
    if (!oFrom || !oFrom.IsTypeNumber() || !oTo || !oTo.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    double fromTime = oFrom.GetDouble();
    double toTime   = oTo.GetDouble();
    int64_t fromFrame = (int64_t)(fromTime * 15.0);
    int64_t toFrame   = (int64_t)(toTime   * 15.0);

    std::string result = "[";
    bool first = true;
    for (const auto& s : pS->ViewHistory) {
        if (s.Frame < fromFrame || s.Frame > toFrame) continue;
        if (!first) result += ",";
        first = false;
        double time = (double)s.Frame / 15.0;
        result += std::format(
            R"({{"Time":{:.4f},"Cell":[{},{}]}})",
            time, (int)s.Cell.X, (int)s.Cell.Y);
    }
    result += "]";

    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ---------- FindPath（STUB 桩函数）----------
//  -SessionID -FromX -FromY -ToX -ToY [-TechnoTypeID <str>]
void __cdecl IHAgentBackend_FindPath(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    // 验证参数（以便调用者即使在我们 stub 的情况下也能获得 BAD_ARGUMENTS 反馈）
    auto oFX = Context.GetObjectItem("FromX"), oFY = Context.GetObjectItem("FromY");
    auto oTX = Context.GetObjectItem("ToX"),   oTY = Context.GetObjectItem("ToY");
    if (!oFX || !oFX.IsTypeNumber() || !oFY || !oFY.IsTypeNumber() ||
        !oTX || !oTX.IsTypeNumber() || !oTY || !oTY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }

    std::string result =
        R"({"Success":false,"Reason":"NotImplemented","Suggestion":"Use engine A* via UnitClass instance or implement custom A* over CellClass"})";
    std::cout << result << std::endl;
    std::cout << "FindPath: stubbed" << std::endl;
    ECDebug::ReturnStdError(ERROR_NOT_SUPPORTED);
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  Cell 操作——辅助性（保持不变，属于阶段 D 的范围）
//  这些操作不需要 -SessionID。
// ════════════════════════════════════════════════════════════════════════════

static CellClass* ResolveCell(JsonObject Context)
{
    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) return nullptr;
    CellStruct cs { (short)oX.GetInt(), (short)oY.GetInt() };
    return MapClass::Instance->TryGetCellAt(cs);
}

static const char* TiberiumTypeName(int overlayIndex)
{
    if (overlayIndex < 0) return "None";
    auto pTib = TiberiumClass::Find(overlayIndex);
    return pTib ? pTib->ID : "Unknown";
}

// ---------- CellAddrToCoords ----------
void __cdecl IHAgentBackend_CellAddrToCoords(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    auto pCell = (CellClass*)(DWORD)oAddr.GetInt();
    if (!pCell || pCell->WhatAmI() != AbstractType::Cell) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }
    auto s = std::format(R"({{"X":{},"Y":{}}})", (int)pCell->MapCoords.X, (int)pCell->MapCoords.Y);
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// ---------- CellCoordsToAddr ----------
void __cdecl IHAgentBackend_CellCoordsToAddr(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    CellStruct cs { (short)oX.GetInt(), (short)oY.GetInt() };
    CellClass* pCell = MapClass::Instance->TryGetCellAt(cs);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }
    auto s = std::format("\"0x{:08X}\"", (DWORD)pCell);
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// ---------- Shroud 迷雾操作 ----------
void __cdecl IHAgentBackend_GetCellShroud(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    CellClass* pCell = ResolveCell(Context);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    auto s = std::format(R"({{"IsShrouded":{},"ShroudCounter":{}}})",
        pCell->IsShrouded() ? "true" : "false", pCell->ShroudCounter);
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_UnshroudCell(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    CellClass* pCell = ResolveCell(Context);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    pCell->Unshroud();
    std::cout << std::format("UnshroudCell ({},{}) ShroudCounter={}",
        (int)pCell->MapCoords.X, (int)pCell->MapCoords.Y, pCell->ShroudCounter) << std::endl;
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_AdjustCellShroud(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    CellClass* pCell = ResolveCell(Context);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    auto oAction = Context.GetObjectItem("Action");
    if (!oAction || !oAction.IsTypeString()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    auto action = oAction.GetString();
    if (action == "Increase")      pCell->IncreaseShroudCounter();
    else if (action == "Decrease") pCell->ReduceShroudCounter();
    else if (action == "Clear")    pCell->Unshroud();
    else { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    std::cout << std::format("AdjustCellShroud ({},{}) {} → ShroudCounter={}",
        (int)pCell->MapCoords.X, (int)pCell->MapCoords.Y, action, pCell->ShroudCounter) << std::endl;
    ECDebug::DoNotEcho();
}

// ---------- Tiberium 泰伯利亚操作 ----------
void __cdecl IHAgentBackend_GetCellTiberium(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    CellClass* pCell = ResolveCell(Context);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    int idx = pCell->GetContainedTiberiumIndex();
    int val = pCell->GetContainedTiberiumValue();
    auto s = std::format(R"({{"Index":{},"Value":{},"TypeName":"{}"}})",
        idx, val, TiberiumTypeName(idx));
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_AddCellTiberium(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    CellClass* pCell = ResolveCell(Context);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    auto oIdx = Context.GetObjectItem("Index"), oAmt = Context.GetObjectItem("Amount");
    if (!oIdx || !oIdx.IsTypeNumber() || !oAmt || !oAmt.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    bool ok = pCell->IncreaseTiberium(oIdx.GetInt(), oAmt.GetInt());
    std::cout << std::format("AddCellTiberium ({},{}) idx={} amt={} ok={}",
        (int)pCell->MapCoords.X, (int)pCell->MapCoords.Y,
        oIdx.GetInt(), oAmt.GetInt(), ok ? "true" : "false") << std::endl;
    if (ok)  ECDebug::ReturnString({ u8"true", 4 });
    else     ECDebug::ReturnString({ u8"false", 5 });
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_RemoveCellTiberium(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    CellClass* pCell = ResolveCell(Context);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    auto oAmt = Context.GetObjectItem("Amount");
    if (!oAmt || !oAmt.IsTypeNumber()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    pCell->ReduceTiberium(oAmt.GetInt());
    std::cout << std::format("RemoveCellTiberium ({},{}) amt={} remaining={}",
        (int)pCell->MapCoords.X, (int)pCell->MapCoords.Y,
        oAmt.GetInt(), pCell->GetContainedTiberiumValue()) << std::endl;
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_SpreadCellTiberium(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    CellClass* pCell = ResolveCell(Context);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    bool forced = false;
    auto oF = Context.GetObjectItem("Force");
    if (oF && oF.IsTypeBool()) forced = oF.GetBool();
    bool spread = pCell->SpreadTiberium(forced);
    auto s = std::format(R"({{"Spread":{},"Cell":[{},{}]}})",
        spread ? "true" : "false", (int)pCell->MapCoords.X, (int)pCell->MapCoords.Y);
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// ---------- Radiation 辐射操作 ----------
void __cdecl IHAgentBackend_GetCellRadiation(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    CellClass* pCell = ResolveCell(Context);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    auto s = std::format(R"({{"IsRadiated":{},"RadLevel":{},"RadSite":"0x{:08X}"}})",
        pCell->IsRadiated() ? "true" : "false",
        pCell->RadLevel, (DWORD)pCell->GetRadSite());
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_IncreaseCellRadiation(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    CellClass* pCell = ResolveCell(Context);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    auto oAmt = Context.GetObjectItem("Amount");
    if (!oAmt || !oAmt.IsTypeNumber()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    pCell->RadLevel_Increase(oAmt.GetDouble());
    std::cout << std::format("IncreaseCellRadiation ({},{}) +{} → RadLevel={}",
        (int)pCell->MapCoords.X, (int)pCell->MapCoords.Y,
        oAmt.GetDouble(), pCell->RadLevel) << std::endl;
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_DecreaseCellRadiation(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    CellClass* pCell = ResolveCell(Context);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    auto oAmt = Context.GetObjectItem("Amount");
    if (!oAmt || !oAmt.IsTypeNumber()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    pCell->RadLevel_Decrease(oAmt.GetDouble());
    std::cout << std::format("DecreaseCellRadiation ({},{}) -{} → RadLevel={}",
        (int)pCell->MapCoords.X, (int)pCell->MapCoords.Y,
        oAmt.GetDouble(), pCell->RadLevel) << std::endl;
    ECDebug::DoNotEcho();
}

// ---------- GetFrameRateInfo ----------
void __cdecl IHAgentBackend_GetFrameRateInfo(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    int  instantFPS = (int)FPSCounter::CurrentFrameRate;
    double avgFPS  = FPSCounter::GetAverageFrameRate();
    int  curFrame  = Unsorted::CurrentFrame;
    int  gameSpeed = GameOptionsClass::Instance->GameSpeed;
    auto s = std::format(
        R"({{"CurrentFrame":{},"InstantFPS":{},"AverageFPS":{:.2f},"GameSpeed":{},"SpeedName":"{}"}})",
        curFrame, instantFPS, avgFPS, gameSpeed, GameSpeedName(gameSpeed)
    );
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}
