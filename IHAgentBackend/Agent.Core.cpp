#include "AgentBackend.h"
#include "Pointers.h"
#include <format>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <atomic>
#include <CRT.h>
#include <AbstractTypeClass.h>
#include <YRPP.h>
#include <UnitTypeClass.h>
#include <InfantryTypeClass.h>
#include <BuildingTypeClass.h>
#include <AircraftTypeClass.h>
#include <ExtJson.h>
#include <iostream>

// Only usable after game start — set when IHCore::Reset broadcasts to all DLLs + remote components
std::atomic<bool> _GameStarted{false};
static std::atomic<bool> _HasInit{false};
void IHVerify_Init() { _GameStarted.store(false, std::memory_order_relaxed); }

// Called by ECInitLibrary's OrderedInit callback — check version + set up listeners
void IHVerify_OrderedInit()
{
    auto ihCore = Ext::GetLib("IHCore");
    if (ihCore.Version() < 514)
    {
        MessageBoxA(nullptr,
            "IHCore version < 514, IHVerify cannot run.\nUpdate IHCore to 0.5.14 or later.",
            "IHVerify — Fatal Error", MB_OK | MB_ICONERROR);
        CRT::exit(0);
    }
    ECListener::Listen_PointerExpired(PointerExpiredProcess);
    RegisterAddressCommentProvider("IHVerify", MarkerACP);
    _HasInit.store(true, std::memory_order_relaxed);
}

// Called via IHCore::Reset broadcast at game start — fires on ALL DLLs + remote components
void __cdecl IHVerify_OnGameReset(JsonObject)
{
    _GameStarted.store(true, std::memory_order_release);
}

// Status var cache (returned pointers must remain valid)
static std::unordered_map<std::string, std::string> _StatusVars;

static UTF8_CString SetStatusVar(const std::string& key, std::string&& val)
{
    auto& slot = _StatusVars[key];
    slot = std::move(val);
    return conv slot.c_str();
}

// ------------------------------------------------------------------
//  StatusRouter: <Status.Frame>, <Status.PlayerCount>,
//                <Status.House.0.Money>, etc.
// ------------------------------------------------------------------
UTF8_CString __cdecl StatusRouter(const char* Key)
{
    if (!_GameStarted.load(std::memory_order_acquire)) return nullptr;
    std::string_view k(Key);

    // Global state
    if (k == "Frame")
        return SetStatusVar("Frame", std::to_string(Unsorted::CurrentFrame));

    if (k == "PlayerCount")
        return SetStatusVar("PlayerCount", std::to_string(HouseClass::Array->Count));

    if (k == "CurrentPlayerID")
    {
        auto p = HouseClass::CurrentPlayer();
        return p ? SetStatusVar("CurrentPlayerID", std::to_string(p->ArrayIndex)) : nullptr;
    }

    if (k == "ObserverID")
    {
        auto p = HouseClass::CurrentPlayer();
        if (p && p->IsObserver()) return SetStatusVar("ObserverID", std::to_string(p->ArrayIndex));
        return nullptr;
    }

    auto& pSess = SessionClass::Instance;
    if (k == "Difficulty")
        return SetStatusVar("Difficulty", std::to_string((int)pSess->Config.AIDifficulty));

    // House.N.field
    if (k.starts_with("House."))
    {
        auto rest = std::string(k.substr(6));
        auto dot = rest.find('.');
        if (dot == std::string::npos) return nullptr;
        auto idxStr = rest.substr(0, dot);
        auto field  = rest.substr(dot + 1);
        int idx = 0;
        try { idx = std::stoi(idxStr); } catch (...) { return nullptr; }
        if (idx < 0 || idx >= (*HouseClass::Array).Count) return nullptr;
        auto pHouse = (*HouseClass::Array)[idx];
        if (!pHouse) return nullptr;

        auto sk = std::string(k);

        if (field == "Name")
            return SetStatusVar(sk, pHouse->PlainName);
        if (field == "Money")
            return SetStatusVar(sk, std::to_string(pHouse->Available_Money()));
        if (field == "OwnedUnits")
            return SetStatusVar(sk, std::to_string(pHouse->OwnedUnits));
        if (field == "OwnedBuildings")
            return SetStatusVar(sk, std::to_string(pHouse->OwnedBuildings));
        if (field == "OwnedInfantry")
            return SetStatusVar(sk, std::to_string(pHouse->OwnedInfantry));
        if (field == "OwnedAircraft")
            return SetStatusVar(sk, std::to_string(pHouse->OwnedAircraft));
        if (field == "Player")
            return SetStatusVar(sk, pHouse->IsCurrentPlayer() ? "Yes" : "No");
        if (field == "Faction")
            return SetStatusVar(sk, pHouse->Type ? pHouse->Type->ID : "(unknown)");
        if (field == "Country" && pHouse->Type)
        {
            auto pc = pHouse->Type->ParentCountry;
            return pc ? SetStatusVar(sk, pc.operator const char*()) : nullptr;
        }
        if (field == "Color")
            return SetStatusVar(sk, std::format("{},{},{}", (int)pHouse->Color.R, (int)pHouse->Color.G, (int)pHouse->Color.B));
        if (field == "LaserColor")
            return SetStatusVar(sk, std::format("{},{},{}", (int)pHouse->LaserColor.R, (int)pHouse->LaserColor.G, (int)pHouse->LaserColor.B));
        return nullptr;
    }

    return nullptr;
}

// ── WildcardMatch for FindObjects / FindTypes ──
static bool WildcardMatch(const char* pattern, const char* str)
{
    if (*pattern == '\0' && *str == '\0') return true;
    if (*pattern == '\0') return false;
    if (*pattern == '*') {
        while (*(pattern + 1) == '*') pattern++;
        if (WildcardMatch(pattern + 1, str)) return true;
        return (*str != '\0') && WildcardMatch(pattern, str + 1);
    }
    if (*pattern == '?')
        return (*str != '\0') && WildcardMatch(pattern + 1, str + 1);
    return (*pattern == *str) && WildcardMatch(pattern + 1, str + 1);
}

// ------------------------------------------------------------------
//  FindObjects: enumerate AbstractClass::Array with filters
// ------------------------------------------------------------------
void __cdecl IHVerify_FindObjects(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    int filterAbs = -1;
    auto oWA = Context.GetObjectItem("WhatAmI");
    if (oWA && oWA.IsTypeString()) {
        auto s = ~oWA.GetString();
        if (s == u8"Unit" || s == u8"UnitType")     filterAbs = (int)AbstractType::Unit;
        if (s == u8"Infantry" || s == u8"InfantryType") filterAbs = (int)AbstractType::Infantry;
        if (s == u8"Building" || s == u8"BuildingType") filterAbs = (int)AbstractType::Building;
        if (s == u8"Aircraft" || s == u8"AircraftType") filterAbs = (int)AbstractType::Aircraft;
        if (s == u8"House")    filterAbs = (int)AbstractType::House;
        if (s == u8"Factory")  filterAbs = (int)AbstractType::Factory;
    }

    int filterHouse = -1;
    auto oH = Context.GetObjectItem("House");
    if (oH && oH.IsTypeNumber()) filterHouse = oH.GetInt();

    std::string filterType;
    auto oT = Context.GetObjectItem("Type");
    if (oT && oT.IsTypeString()) filterType = oT.GetString();

    std::string filterMission;
    auto oM = Context.GetObjectItem("Mission");
    if (oM && oM.IsTypeString()) filterMission = oM.GetString();

    int limit = 50;
    auto oL = Context.GetObjectItem("Limit");
    if (oL && oL.IsTypeNumber()) limit = oL.GetInt();
    if (limit > 500) limit = 500;

    int offset = 0;
    auto oO = Context.GetObjectItem("Offset");
    if (oO && oO.IsTypeNumber()) offset = oO.GetInt();

    std::string result = "[";
    result.reserve(4096);
    int skipped = 0;
    int collected = 0;

    for (auto pAbs : *AbstractClass::Array)
    {
        if (!pAbs) continue;
        auto absType = (int)pAbs->WhatAmI();

        // InLimbo only applies to ObjectClass descendants — skip for Factory/House/etc
        if (absType == (int)AbstractType::Factory) {
            // FactoryClass doesn't inherit ObjectClass — no InLimbo, no GetOwningHouse
        } else if (absType == (int)AbstractType::House) {
            // HouseClass doesn't have InLimbo either
        } else {
            if (((ObjectClass*)pAbs)->InLimbo) continue;
        }

        if (filterAbs >= 0 && absType != filterAbs) continue;

        if (filterHouse >= 0)
        {
            int houseIdx = -1;
            if (absType == (int)AbstractType::Factory) {
                houseIdx = ((FactoryClass*)pAbs)->Owner ? ((FactoryClass*)pAbs)->Owner->ArrayIndex : -1;
            } else if (absType == (int)AbstractType::House) {
                houseIdx = ((HouseClass*)pAbs)->ArrayIndex;
            } else {
                auto pHouse = pAbs->GetOwningHouse();
                houseIdx = pHouse ? pHouse->ArrayIndex : -1;
            }
            if (houseIdx != filterHouse) continue;
        }

        if (!filterType.empty())
        {
            if (absType == (int)AbstractType::Factory) {
                // Factory: match by Object's TechnoType ID, or skip
                auto pFact = (FactoryClass*)pAbs;
                if (!pFact->Object) continue;
                auto pTT = pFact->Object->GetTechnoType();
                if (!pTT || std::string(pTT->ID) != filterType) continue;
            }
            else if (absType == (int)AbstractType::House) continue;
            else {
                auto pType = ((TechnoClass*)pAbs)->GetTechnoType();
                if (!pType || std::string(pType->ID) != filterType) continue;
            }
        }

        if (!filterMission.empty())
        {
            if (absType < (int)AbstractType::Infantry || absType > (int)AbstractType::Aircraft)
                continue;
            auto mis = (int)((ObjectClass*)pAbs)->GetCurrentMission();
            if (std::to_string(mis) != filterMission) continue;
        }

        if (skipped < offset) { skipped++; continue; }

        if (collected > 0) result += ",";
        char buf[20];
        sprintf_s(buf, "\"0x%08X\"", (DWORD)pAbs);
        result += buf;
        collected++;
        if (collected >= limit) break;
    }

    result += "]";
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ------------------------------------------------------------------
//  FindTechnoInfo: enumerate *TechnoClass::Array with brief info per entry
//    -WhatAmI Unit/Infantry/Building/Aircraft  (optional)
//    -House <int>  -Type <str>  -Mission <str>  -Limit <int>  -Offset <int>
//  Returns [{addr,whatami,whatami_str,type,hp,maxhp,mission,cell,house,selected,inlimbo},...]
//  Use this for battlefield awareness — iterates TechnoClass::Array only,
//  skipping all non-Techno objects (House/Factory/Bullet/Anim/etc.).
// ------------------------------------------------------------------
void __cdecl IHVerify_FindTechnoInfo(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    int filterAbs = -1;
    auto oWA = Context.GetObjectItem("WhatAmI");
    if (oWA && oWA.IsTypeString()) {
        auto s = ~oWA.GetString();
        if (s == u8"Unit")              filterAbs = (int)AbstractType::Unit;
        if (s == u8"Infantry")          filterAbs = (int)AbstractType::Infantry;
        if (s == u8"Building")          filterAbs = (int)AbstractType::Building;
        if (s == u8"Aircraft")          filterAbs = (int)AbstractType::Aircraft;
    }

    int filterHouse = -1;
    auto oH = Context.GetObjectItem("House");
    if (oH && oH.IsTypeNumber()) filterHouse = oH.GetInt();

    std::string filterType;
    auto oT = Context.GetObjectItem("Type");
    if (oT && oT.IsTypeString()) filterType = oT.GetString();

    std::string filterMission;
    auto oM = Context.GetObjectItem("Mission");
    if (oM && oM.IsTypeString()) filterMission = oM.GetString();

    int limit = 50;
    auto oL = Context.GetObjectItem("Limit");
    if (oL && oL.IsTypeNumber()) limit = oL.GetInt();
    if (limit > 500) limit = 500;

    int offset = 0;
    auto oO = Context.GetObjectItem("Offset");
    if (oO && oO.IsTypeNumber()) offset = oO.GetInt();

	bool selectedOnly = false;
    auto oSel = Context.GetObjectItem("SelectedOnly");
    if (oSel && oSel.IsTypeBool()) selectedOnly = oSel.GetBool();

    bool considerLimbo = false;
    auto oCon = Context.GetObjectItem("ConsiderLimbo");
    if (oCon && oCon.IsTypeBool()) considerLimbo = oCon.GetBool();

    bool limboOnly = false;
    auto oLim = Context.GetObjectItem("LimboOnly");
    if (oLim && oLim.IsTypeBool()) limboOnly = oLim.GetBool();

    bool techBuilding = false;
	auto oTB = Context.GetObjectItem("TechBuilding");
	if (oTB && oTB.IsTypeBool()) techBuilding = oTB.GetBool();

	std::unordered_set<TechnoTypeClass*> techBuildingTypes;
    if (techBuilding)
    {
        for (auto pType : RulesClass::Instance->NeutralTechBuildings)
            techBuildingTypes.insert(pType);
    }

    std::string result = "[";
    result.reserve(16384);
    int skipped = 0;
    int collected = 0;

    for (auto pTechno : *TechnoClass::Array)
    {
        if (!pTechno) continue;
        if (!considerLimbo && !limboOnly && pTechno->InLimbo) continue;
		if (limboOnly && !pTechno->InLimbo) continue;

        auto absType = (int)pTechno->WhatAmI();
        if (filterAbs >= 0 && absType != filterAbs) continue;

        if (filterHouse >= 0)
        {
            auto pHouse = pTechno->GetOwningHouse();
            if (!pHouse || pHouse->ArrayIndex != filterHouse) continue;
        }

        if (selectedOnly)
        {
			if (!pTechno->IsSelected) continue;
        }

        auto pType = pTechno->GetTechnoType();

        if (!filterType.empty())
        {
            if (!pType || filterType != std::string(pType->ID)) continue;
        }

        if (techBuilding)
        {
            if (!pType || techBuildingTypes.find(pType) == techBuildingTypes.end()) continue;
        }

        if (!filterMission.empty())
        {
            auto mis = (int)pTechno->GetCurrentMission();
            if (std::to_string(mis) != filterMission) continue;
        }

        if (skipped < offset) { skipped++; continue; }

        auto coords = pTechno->GetCoords();
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

    result += "]";
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ------------------------------------------------------------------
//  FindTypes: enumerate *AbstractTypeClass::Array for type definitions
// ------------------------------------------------------------------
void __cdecl IHVerify_FindTypes(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    std::string filterID;
    bool hasIDFilter = false;
    auto oID = Context.GetObjectItem("ID");
    if (oID && oID.IsTypeString()) {
        filterID = oID.GetString();
        hasIDFilter = true;
    }

    int filterAbs = -1;
    auto oWA = Context.GetObjectItem("WhatAmI");
    if (oWA && oWA.IsTypeString()) {
        auto s = ~oWA.GetString();
        if (s == u8"UnitType")     filterAbs = (int)AbstractType::UnitType;
        if (s == u8"InfantryType") filterAbs = (int)AbstractType::InfantryType;
        if (s == u8"BuildingType") filterAbs = (int)AbstractType::BuildingType;
        if (s == u8"AircraftType") filterAbs = (int)AbstractType::AircraftType;
        if (s == u8"AnimType")     filterAbs = (int)AbstractType::AnimType;
    }

    int limit = 50;
    auto oL = Context.GetObjectItem("Limit");
    if (oL && oL.IsTypeNumber()) limit = oL.GetInt();
    if (limit > 500) limit = 500;

    int offset = 0;
    auto oO = Context.GetObjectItem("Offset");
    if (oO && oO.IsTypeNumber()) offset = oO.GetInt();

    std::string result = "[";
    result.reserve(4096);
    int skipped = 0;
    int collected = 0;

    for (auto pType : *AbstractTypeClass::Array)
    {
        if (!pType) continue;

        auto absType = (int)pType->WhatAmI();
        if (filterAbs >= 0 && absType != filterAbs) continue;

        if (hasIDFilter && !WildcardMatch(filterID.c_str(), pType->ID))
            continue;

        if (skipped < offset) { skipped++; continue; }

        if (collected > 0) result += ",";
        result += std::format(R"({{"ID":"{}","Name":"{}","WhatAmI":{}}})",
            pType->ID,
            pType->Name ? pType->Name : "",
            absType);
        collected++;
        if (collected >= limit) break;
    }

    result += "]";
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ------------------------------------------------------------------
//  GetHouseInfo: structured JSON for a House
//    -House <index>  (optional, default 0)
//  Includes cluster detection: groups of ≥5 same-house Unit/Infantry
//  within a 3×3 cell radius.
// ------------------------------------------------------------------
void __cdecl IHVerify_GetHouseInfo(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    int idx = 0;
    auto oH = Context.GetObjectItem("House");
    if (oH && oH.IsTypeNumber()) idx = oH.GetInt();

    if (idx < 0 || idx >= (*HouseClass::Array).Count) {
        ECDebug::ReturnStdError(ERROR_INVALID_INDEX); return;
    }
    auto pHouse = (*HouseClass::Array)[idx];
    if (!pHouse) { ECDebug::ReturnStdError(ERROR_INVALID_DATA); return; }

    std::string result = std::format(
        R"({{"Index":{},"Name":"{}","ParentCountry":"{}","Country":"{}","SideIndex":{})"
        R"(,"Color":"{},{},{}","LaserColor":"{},{},{}")"
        R"(,"Money":{},"OwnedUnits":{},"OwnedBuildings":{},"OwnedInfantry":{},"OwnedAircraft":{})"
        R"(,"IsPlayer":"{}","Player":{})",
        idx,
        pHouse->PlainName,
        pHouse->Type ? pHouse->Type->ID : "(unknown)",
        (pHouse->Type && pHouse->Type->ParentCountry) ? (const char*)pHouse->Type->ParentCountry : "(unknown)",
        pHouse->SideIndex,
        (int)pHouse->Color.R, (int)pHouse->Color.G, (int)pHouse->Color.B,
        (int)pHouse->LaserColor.R, (int)pHouse->LaserColor.G, (int)pHouse->LaserColor.B,
        pHouse->Available_Money(),
        pHouse->OwnedUnits, pHouse->OwnedBuildings,
        pHouse->OwnedInfantry, pHouse->OwnedAircraft,
        pHouse->IsCurrentPlayer() ? "yes" : "no",
        pHouse->ArrayIndex
    );

    // ── Cluster Detection ──────────────────────────────────────────
    // Iterate Unit + Infantry of this house, map cell→pointers,
    // then detect cells where ≥5 same-house Technos are within 3×3.
    {
        // Step 1: collect Unit & Infantry positions by cell key
        // We use a map from cellKey (Y*10000+X) → vector of TechnoClass*
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

        // Step 2: for each populated cell, check 3×3 neighborhood
        // Store detected clusters; dedup by cellKey so each cluster only
        // appears once even if multiple cells in the same cluster trigger.
        std::unordered_set<int> reportedCells;
        std::string clusters = "[";

        for (auto& [cellKey, vec] : cellMap)
        {
            if (reportedCells.count(cellKey)) continue;
            if (vec.empty()) continue;

            int cx = cellKey % 10000;
            int cy = cellKey / 10000;

            // Count same-house Technos in 3×3 area
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
                // Mark all cells in this 3×3 region as reported
                for (int dy = -1; dy <= 1; ++dy)
                    for (int dx = -1; dx <= 1; ++dx)
                        reportedCells.insert((cy + dy) * 10000 + (cx + dx));

                if (clusters.size() > 1) clusters += ",";

                // Build Types dict: "HTNK":3,"E1":2
                std::string typesDict = "{";
                bool firstType = true;
                for (auto& [tid, cnt] : typeCount)
                {
                    if (!firstType) typesDict += ",";
                    firstType = false;
                    typesDict += std::format(R"("{}":{})", tid, cnt);
                }
                typesDict += "}";

                clusters += std::format(
                    R"({{"cell":[{},{}],"count":{},"Types":{}}})",
                    cx, cy, cellTotal, typesDict);
            }
        }

        clusters += "]";
        result += std::format(R"(,"Clusters":{})", clusters);
    }

    result += "}";  // close root JSON object
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ── Helper: format a single TechnoType entry for GetHouseTechTree ──
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
        Obj.AddString("UIName", "");

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

// ------------------------------------------------------------------
//  GetHouseTechTree: enumerate all TechnoTypes that a House can build
//    -House <index>  (optional, default 0)
//  Calls CanBuild(type, true, false) on every type in the four
//  TechnoType arrays (UnitType, InfantryType, BuildingType, AircraftType)
//  and returns only the buildable entries with brief info.
// ------------------------------------------------------------------
void __cdecl IHVerify_GetHouseTechTree(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    int hidx = 0;
    auto oH = Context.GetObjectItem("House");
    if (oH && oH.IsTypeNumber()) hidx = oH.GetInt();

    if (hidx < 0 || hidx >= (*HouseClass::Array).Count) {
        ECDebug::ReturnStdError(ERROR_INVALID_INDEX); return;
    }
    auto pHouse = (*HouseClass::Array)[hidx];
    if (!pHouse) { ECDebug::ReturnStdError(ERROR_INVALID_DATA); return; }

    std::string result = "{\"House\":";
    result += std::to_string(hidx);
    result += ",\"Buildable\":[";
    result.reserve(65536);
    int total = 0;

    // UnitType
    for (auto pType : *UnitTypeClass::Array) {
        if (!pType) continue;
        if ((int)pHouse->CanBuild(pType, false, false) >= 1) {
            if (total > 0) result += ",";
            AppendTypeEntry(result, "UnitType", pType);
            total++;
        }
    }
    // InfantryType
    for (auto pType : *InfantryTypeClass::Array) {
        if (!pType) continue;
        if ((int)pHouse->CanBuild(pType, false, false) >= 1) {
            if (total > 0) result += ",";
            AppendTypeEntry(result, "InfantryType", pType);
            total++;
        }
    }
    // BuildingType
    for (auto pType : *BuildingTypeClass::Array) {
        if (!pType) continue;
        if ((int)pHouse->CanBuild(pType, false, false) >= 1) {
            if (total > 0) result += ",";
            AppendTypeEntry(result, "BuildingType", pType);
            total++;
        }
    }
    // AircraftType
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

// ------------------------------------------------------------------
// GetTechBuildingTypes
//   (no args)
// ------------------------------------------------------------------
void __cdecl IHVerify_GetTechBuildingTypes(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    std::string result = "{\"Buildable\":[";
    result.reserve(65536);
    int total = 0;

    for (auto pType : RulesClass::Instance->NeutralTechBuildings) {
        if (!pType) continue;
        if (total > 0) result += ",";
        AppendTypeEntry(result, "BuildingType", pType);
        total++;
    }

    result += std::format("],\"TotalCount\":{}}}", total);
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

BuildingClass* FindGarrisonBuilding(InfantryClass* pInfantry)
{
    HouseClass* pHouse = pInfantry->Owner;
    if (!pHouse) return nullptr;

    for (auto pBuilding : pHouse->Buildings)
    {
        for(auto& occ : pBuilding->Occupants)
        {
            if (occ == pInfantry)
                return pBuilding;
		}
    }

    return nullptr;
}

// ------------------------------------------------------------------
//  GetTechnoInfo: structured JSON for a Techno
//    -Address <object address>  (required)
// ------------------------------------------------------------------
void __cdecl IHVerify_GetTechnoInfo(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }

    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber())
    {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    DWORD addr = (DWORD)oAddr.GetInt();

    {
        auto pAbs = (AbstractClass*)addr;
        if (!pAbs || !IsPointerAlive(addr)) {
            ECDebug::ReturnStdError(ERROR_INVALID_DATA);
            return;
        }
        auto wa = pAbs->WhatAmI();
        if (
            wa != AbstractType::Infantry && 
			wa != AbstractType::Unit &&
			wa != AbstractType::Building &&
			wa != AbstractType::Aircraft
            ) {
            ECDebug::ReturnStdError(ERROR_INVALID_DATA);
            return;
        }
    }

    auto pTechno = (TechnoClass*)addr;
    auto pType = pTechno->GetTechnoType();
    if (!pType) {
        ECDebug::ReturnStdError(ERROR_INVALID_DATA);
        return;
	}


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
    
    
    

    JsonFile F;
    auto Obj = F.GetObj();
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

    /*
    passenger
    Transporter
    occupant
    BunkerLinkedItem
    */
    if(pTechno->Transporter)
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
        if (pBldType->BridgeRepairHut)
        {
            if(MapClass::Instance->IsLinkedBridgeDestroyed(pBld->GetMapCoords()))
				Obj.AddString("BridgeStatus", "Destroyed");
			else
				Obj.AddString("BridgeStatus", "Intact");
        }
        else
			Obj.AddNull("BridgeStatus");


        std::vector<std::string> occupantAddrs;
        for(auto pOcc : pBld->Occupants)
        {
            occupantAddrs.push_back(std::format("0x{:08X}", (DWORD)pOcc));
		}
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

    if(auto pInf = abstract_cast<InfantryClass*>(pTechno); pInf)
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
        Obj.AddString("HealthStatus", "Green");
		break;
    case HealthState::Yellow:
		Obj.AddString("HealthStatus", "Yellow");
        break;
	case HealthState::Red:
		Obj.AddString("HealthStatus", "Red");
        break;
    default:
		Obj.AddString("HealthStatus", "Unknown");
    }




    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString(~result);
	ECDebug::DoNotEcho();
}

// ------------------------------------------------------------------
//  IsObjectAlive (Condition): check if an address is still in AbstractClass::Array
//    -Address <0x...>
//  Returns true if the pointer is alive (still in the global object array),
//  false if it has been destroyed or was never valid.
// ------------------------------------------------------------------

bool IsPointerAlive(DWORD addr)
{
    if (!addr) return false;
    for (auto pAbs : *AbstractClass::Array)
    {
        if ((DWORD)pAbs == addr)
            return true;
    }
    return false;
}

bool IsFactoryAlive(DWORD addr)
{
    if (!addr) return false;
    for (auto pFact : *FactoryClass::Array)
    {
        if ((DWORD)pFact == addr)
            return true;
    }
    return false;
}

void __cdecl IHVerify_IsObjectAlive(JsonObject Context)
{
    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber())
    {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    DWORD addr = (DWORD)oAddr.GetInt();
    bool alive = IsPointerAlive(addr);
    std::cout << (alive ? "true" : "false") << std::endl;
    if (alive)
        ECDebug::ReturnString({ u8"true", 4 });
    else
        ECDebug::ReturnString({ u8"false", 5 });
    ECDebug::DoNotEcho();
}