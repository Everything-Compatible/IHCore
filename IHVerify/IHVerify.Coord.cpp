#include "IHVerify.h"
#include <format>
#include <iostream>
#include <string>
#include <YRPP.h>
#include <MapClass.h>
#include <CellClass.h>
#include <CellSpread.h>
#include <AircraftClass.h>

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
