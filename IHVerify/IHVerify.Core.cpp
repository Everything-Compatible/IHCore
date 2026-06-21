#include "IHVerify.h"
#include "Pointers.h"
#include <format>
#include <unordered_map>
#include <string>
#include <atomic>
#include <CRT.h>
#include <AbstractTypeClass.h>
#include <YRPP.h>
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
        if (!pAbs || ((ObjectClass*)pAbs)->InLimbo) continue;

        auto absType = (int)pAbs->WhatAmI();
        if (filterAbs >= 0 && absType != filterAbs) continue;

        if (filterHouse >= 0)
        {
            auto pHouse = pAbs->GetOwningHouse();
            if (!pHouse || pHouse->ArrayIndex != filterHouse) continue;
        }

        if (!filterType.empty())
        {
            if (absType == (int)AbstractType::House) continue;
            auto pType = ((TechnoClass*)pAbs)->GetTechnoType();
            if (!pType || std::string(pType->ID) != filterType) continue;
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
        R"({{"Index":{},"Name":"{}","Faction":"{}","Country":"{}")"
        R"(,"Color":"{},{},{}","LaserColor":"{},{},{}")"
        R"(,"Money":{},"OwnedUnits":{},"OwnedBuildings":{},"OwnedInfantry":{},"OwnedAircraft":{})"
        R"(,"IsPlayer":"{}","Player":{}}})",
        idx,
        pHouse->PlainName,
        pHouse->Type ? pHouse->Type->ID : "(unknown)",
        (pHouse->Type && pHouse->Type->ParentCountry) ? (const char*)pHouse->Type->ParentCountry : "(unknown)",
        (int)pHouse->Color.R, (int)pHouse->Color.G, (int)pHouse->Color.B,
        (int)pHouse->LaserColor.R, (int)pHouse->LaserColor.G, (int)pHouse->LaserColor.B,
        pHouse->Available_Money(),
        pHouse->OwnedUnits, pHouse->OwnedBuildings,
        pHouse->OwnedInfantry, pHouse->OwnedAircraft,
        pHouse->IsCurrentPlayer() ? "yes" : "no",
        pHouse->ArrayIndex
    );

    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}
