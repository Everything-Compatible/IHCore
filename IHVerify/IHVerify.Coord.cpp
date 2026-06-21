#include "IHVerify.h"
#include <format>
#include <iostream>
#include <string>
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

// ---------- GetMapInfo ----------
void __cdecl IHVerify_GetMapInfo(JsonObject)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    auto& Inst = *MapClass::Instance;
    auto& Bnd = Inst.MapCoordBounds;
    auto s = std::format(
        R"({{"Width":{},"Height":{},"Left":{},"Top":{},"Right":{},"Bottom":{}}})",
        Inst.MapRect.Width, Inst.MapRect.Height,
        Bnd.Left, Bnd.Top, Bnd.Right, Bnd.Bottom
    );
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// ---------- IsOnMap ----------
void __cdecl IHVerify_IsOnMap(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    CellStruct cell { (short)oX.GetInt(), (short)oY.GetInt() };
    bool ok = MapClass::Instance->IsWithinUsableArea(cell, false);
    std::cout << (ok ? "true" : "false") << std::endl;
    if (ok) { ECDebug::ReturnString({ u8"true", 4 }); }
    else    { ECDebug::ReturnString({ u8"false", 5 }); }
    ECDebug::DoNotEcho();
}

// ---------- GetObjectCoords ----------
void __cdecl IHVerify_GetObjectCoords(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    auto pAbs = (AbstractClass*)(DWORD)oAddr.GetInt();
    if (!pAbs) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    auto pObj = (ObjectClass*)pAbs;
    auto coords = pObj->GetCoords();
    auto mapCoords = pObj->GetMapCoords();
    int height = pObj->GetHeight();
    bool inAir = pObj->IsInAir();
    bool onFloor = pObj->IsOnFloor();

    auto s = std::format(
        R"({{"X":{},"Y":{},"Z":{},"CellX":{},"CellY":{},"Height":{},"IsInAir":{},"IsOnFloor":{}}})",
        coords.X, coords.Y, coords.Z,
        (int)mapCoords.X, (int)mapCoords.Y, height,
        inAir ? "true" : "false", onFloor ? "true" : "false"
    );
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// ---------- GetCellInfo ----------
void __cdecl IHVerify_GetCellInfo(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    CellStruct cs { (short)oX.GetInt(), (short)oY.GetInt() };
    CellClass* pCell = MapClass::Instance->TryGetCellAt(cs);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    auto s = std::format(
        R"({{"LandType":{},"OccupationFlags":"0x{:02X}","AltOccupationFlags":"0x{:02X}","ContainsBridge":{},"SlopeIndex":{}}})",
        (int)pCell->LandType,
        (int)pCell->OccupationFlags, (int)pCell->AltOccupationFlags,
        pCell->ContainsBridge() ? "true" : "false",
        pCell->SlopeIndex
    );
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// ---------- FindObjectsOnCell ----------
void __cdecl IHVerify_FindObjectsOnCell(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    CellStruct cs { (short)oX.GetInt(), (short)oY.GetInt() };
    CellClass* pCell = MapClass::Instance->TryGetCellAt(cs);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    std::string s = "[";
    for (auto pObj = pCell->FirstObject; pObj; pObj = pObj->NextObject)
    {
        if (s.size() > 1) s += ",";
        s += std::format("\"0x{:08X}\"", (DWORD)pObj);
    }
    s += "]";
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// ---------- FindObjectsInRange ----------
void __cdecl IHVerify_FindObjectsInRange(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    short cx = (short)oX.GetInt(), cy = (short)oY.GetInt();

    auto oR = Context.GetObjectItem("Range");
    int range = 0;
    if (oR && oR.IsTypeNumber()) range = oR.GetInt();
    if (range < 0) range = 0; if (range > 50) range = 50;

    int filterAbs = -1;
    auto oWA = Context.GetObjectItem("WhatAmI");
    if (oWA && oWA.IsTypeString()) {
        auto swa = ~oWA.GetString();
        if (swa == u8"Unit" || swa == u8"UnitType")         filterAbs = (int)AbstractType::Unit;
        if (swa == u8"Infantry" || swa == u8"InfantryType") filterAbs = (int)AbstractType::Infantry;
        if (swa == u8"Building" || swa == u8"BuildingType") filterAbs = (int)AbstractType::Building;
        if (swa == u8"Aircraft" || swa == u8"AircraftType") filterAbs = (int)AbstractType::Aircraft;
    }
    int filterHouse = -1;
    auto oH = Context.GetObjectItem("House");
    if (oH && oH.IsTypeNumber()) filterHouse = oH.GetInt();

    auto count = CellSpread::NumCells((unsigned)range);
    std::string s = "[";
    int total = 0;

    for (size_t i = 0; i < count && total < 200; i++)
    {
        auto offset = CellSpread::GetCell(i);
        CellStruct cs { (short)(cx + offset.X), (short)(cy + offset.Y) };
        CellClass* pCell = MapClass::Instance->TryGetCellAt(cs);
        if (!pCell) continue;
        for (auto pObj = pCell->FirstObject; pObj; pObj = pObj->NextObject)
        {
            if (((ObjectClass*)pObj)->InLimbo) continue;
            auto absType = (int)pObj->WhatAmI();
            if (filterAbs >= 0 && absType != filterAbs) continue;
            if (filterHouse >= 0) {
                auto pHouse = pObj->GetOwningHouse();
                if (!pHouse || pHouse->ArrayIndex != filterHouse) continue;
            }
            if (total > 0) s += ",";
            s += std::format(R"({{"addr":"0x{:08X}","whatami":{},"cell":[{},{}]}})",
                (DWORD)pObj, absType, (int)cs.X, (int)cs.Y);
            total++;
        }
    }
    s += "]";
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// ---------- FindAerialObjects ----------
void __cdecl IHVerify_FindAerialObjects(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
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

    std::string s = "[";
    for (auto pAir : *AircraftClass::Array)
    {
        if (!pAir || pAir->InLimbo) continue;
        CellStruct airCell = pAir->GetMapCoords();
        if (!MapClass::Instance->IsWithinUsableArea(airCell, false)) continue;
        int dx = (int)airCell.X - (int)cx;
        int dy = (int)airCell.Y - (int)cy;
        if (abs(dx) <= rangeCells && abs(dy) <= rangeCells)
        {
            if (s.size() > 1) s += ",";
            s += std::format(R"({{"addr":"0x{:08X}","whatami":2}})", (DWORD)pAir);
        }
    }
    s += "]";
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// ---------- CalcCellSpread ----------
void __cdecl IHVerify_CalcCellSpread(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    auto oS = Context.GetObjectItem("Spread");
    if (!oS || !oS.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    unsigned int nSpread = (unsigned int)oS.GetInt();
    if (nSpread > 50) nSpread = 50;
    auto count = CellSpread::NumCells(nSpread);
    std::string s = "[";
    for (size_t i = 0; i < count; i++)
    {
        auto& off = CellSpread::GetCell(i);
        if (i > 0) s += ",";
        s += std::format("[{},{}]", (int)off.X, (int)off.Y);
    }
    s += "]";
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// ---------- Mission name lookup (int → string) ----------
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

// ---------- GetMovementState ----------
void __cdecl IHVerify_GetMovementState(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    DWORD addr = (DWORD)oAddr.GetInt();
    auto pAbs = (AbstractClass*)addr;
    if (!pAbs) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }
    auto absType = pAbs->WhatAmI();
    if (absType != AbstractType::Unit && absType != AbstractType::Infantry &&
        absType != AbstractType::Building && absType != AbstractType::Aircraft) {
        ECDebug::ReturnStdError(ERROR_INVALID_FUNCTION); return;
    }
    auto pTechno = (TechnoClass*)pAbs;
    bool isFoot = (absType != AbstractType::Building);

    // Current/Queued Mission
    int curMis = (int)pTechno->CurrentMission;
    int queMis = (int)pTechno->QueuedMission;

    // Locomotor state (FootClass only)
    bool isMoving = false;
    int headX = 0, headY = 0, headZ = 0;
    std::string destAddr = "\"0x00000000\"";
    std::string destCellX = "null", destCellY = "null";
    double speedPct = 1.0;
    std::string zoneStr = "\"Unknown\"", speedStr = "\"Unknown\"";

    if (isFoot) {
        auto pFoot = (FootClass*)pTechno;
        speedPct = pFoot->SpeedPercentage;

        auto pType = pTechno->GetTechnoType();
        if (pType) {
            zoneStr = std::format("\"{}\"", ZoneName((int)pType->MovementZone));
            speedStr = std::format("\"{}\"", SpeedName((int)pType->SpeedType));
        }

        if (pFoot->Locomotor) {
            isMoving = pFoot->Locomotor->Is_Moving();
            CoordStruct htc;
            pFoot->Locomotor->Head_To_Coord(&htc);
            headX = htc.X; headY = htc.Y; headZ = htc.Z;
        }

        if (pFoot->Destination) {
            auto dWhat = pFoot->Destination->WhatAmI();
            destAddr = std::format("\"0x{:08X}\"", (DWORD)pFoot->Destination);
            if (dWhat == AbstractType::Cell) {
                auto& mc = ((CellClass*)pFoot->Destination)->MapCoords;
                destCellX = std::to_string((int)mc.X);
                destCellY = std::to_string((int)mc.Y);
            } else if (dWhat == AbstractType::Unit || dWhat == AbstractType::Infantry ||
                       dWhat == AbstractType::Building || dWhat == AbstractType::Aircraft ||
                       dWhat == AbstractType::House) {
                auto cell = ((ObjectClass*)pFoot->Destination)->GetMapCoords();
                destCellX = std::to_string((int)cell.X);
                destCellY = std::to_string((int)cell.Y);
            }
        }
    }

    // Target
    std::string targetStr = "\"0x00000000\"";
    if (pTechno->Target)
        targetStr = std::format("\"0x{:08X}\"", (DWORD)pTechno->Target);

    auto s = std::format(
        R"({{"CurrentMission":{{"value":{},"name":"{}"}},"QueuedMission":{{"value":{},"name":"{}"}})"
        R"(,"MissionStatus":{},"IsMoving":{},"IsInAir":{})"
        R"(,"Destination":{{"Address":{},"CellX":{},"CellY":{}}})"
        R"(,"Target":{},"HeadToCoord":{{"X":{},"Y":{},"Z":{}}})"
        R"(,"SpeedPercentage":{},"MovementZone":{},"SpeedType":{}}})",
        curMis, MissionName(curMis), queMis, MissionName(queMis),
        pTechno->MissionStatus,
        isMoving ? "true" : "false",
        pTechno->InAir ? "true" : "false",
        destAddr, destCellX, destCellY,
        targetStr, headX, headY, headZ,
        speedPct, zoneStr, speedStr
    );
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// ---------- CanEnterCell ----------
void __cdecl IHVerify_CanEnterCell(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber()) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    DWORD addr = (DWORD)oAddr.GetInt();
    auto pAbs = (AbstractClass*)addr;
    if (!pAbs) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }
    auto absType = pAbs->WhatAmI();
    if (absType != AbstractType::Unit && absType != AbstractType::Infantry &&
        absType != AbstractType::Building && absType != AbstractType::Aircraft) {
        ECDebug::ReturnStdError(ERROR_INVALID_FUNCTION); return;
    }

    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    CellStruct cs { (short)oX.GetInt(), (short)oY.GetInt() };
    CellClass* pCell = MapClass::Instance->TryGetCellAt(cs);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    bool canEnter = false;
    const char* resultName = "No";
    if (absType != AbstractType::Building) {
        auto pFoot = (FootClass*)pAbs;
        if (pFoot->Locomotor) {
            Move result = pFoot->Locomotor->Can_Enter_Cell(cs);
            resultName = MoveResultName((int)result);
            canEnter = (result == Move::OK);
        }
    }

    auto s = std::format(
        R"({{"CanEnter":{},"Result":"{}","Cell":[{},{}],"Address":"0x{:08X}"}})",
        canEnter ? "true" : "false", resultName,
        (int)cs.X, (int)cs.Y, addr
    );
    std::cout << s << std::endl;
    ECDebug::ReturnString({ (const char8_t*)s.c_str(), s.size() });
    ECDebug::DoNotEcho();
}

// -- Cell operation helper --

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

//  CellAddrToCoords
void __cdecl IHVerify_CellAddrToCoords(JsonObject Context)
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

//  CellCoordsToAddr
void __cdecl IHVerify_CellCoordsToAddr(JsonObject Context)
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

//  Shroud

// ---------- GetCellShroud ----------
void __cdecl IHVerify_GetCellShroud(JsonObject Context)
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

// ---------- UnshroudCell ----------
void __cdecl IHVerify_UnshroudCell(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }
    CellClass* pCell = ResolveCell(Context);
    if (!pCell) { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    pCell->Unshroud();
    std::cout << std::format("UnshroudCell ({},{}) ShroudCounter={}",
        (int)pCell->MapCoords.X, (int)pCell->MapCoords.Y, pCell->ShroudCounter) << std::endl;
    ECDebug::DoNotEcho();
}

// ---------- AdjustCellShroud ----------
void __cdecl IHVerify_AdjustCellShroud(JsonObject Context)
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

//  Tiberium

// ---------- GetCellTiberium ----------
void __cdecl IHVerify_GetCellTiberium(JsonObject Context)
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

// ---------- AddCellTiberium ----------
void __cdecl IHVerify_AddCellTiberium(JsonObject Context)
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
    if (ok)
        ECDebug::ReturnString({ u8"true", 4 });
    else
        ECDebug::ReturnString({ u8"false", 5 });
    ECDebug::DoNotEcho();
}

// ---------- RemoveCellTiberium ----------
void __cdecl IHVerify_RemoveCellTiberium(JsonObject Context)
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

// ---------- SpreadCellTiberium ----------
void __cdecl IHVerify_SpreadCellTiberium(JsonObject Context)
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

//  Radiation

// ---------- GetCellRadiation ----------
void __cdecl IHVerify_GetCellRadiation(JsonObject Context)
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

// ---------- IncreaseCellRadiation ----------
void __cdecl IHVerify_IncreaseCellRadiation(JsonObject Context)
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

// ---------- DecreaseCellRadiation ----------
void __cdecl IHVerify_DecreaseCellRadiation(JsonObject Context)
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

void __cdecl IHVerify_GetFrameRateInfo(JsonObject Context)
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
