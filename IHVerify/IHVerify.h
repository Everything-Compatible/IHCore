#pragma once
#include <EC.h>
#include <Debug.h>
#include <unordered_map>

void IHVerify_Init();
void IHVerify_OrderedInit();  // checks IHCore version, sets up listeners + ACP
void __cdecl IHVerify_OnGameReset(JsonObject);  // called via IHCore::Reset at game start
FuncInfo* IHVerify_GetFunc(const char* Name, int Version);

// Helper: check if a pointer is still in AbstractClass::Array (alive)
bool IsPointerAlive(DWORD addr);
// Helper: check if a factory pointer is still in FactoryClass::Array
bool IsFactoryAlive(DWORD addr);

// Route: <Status.xxx> / <Status.House.N.xxx>
UTF8_CString __cdecl StatusRouter(const char* Key);

// Action: FindObjects -Type -House -WhatAmI -Mission -Limit -Offset
void __cdecl IHVerify_FindObjects(JsonObject Context);

// Action: FindTechnoInfo — enumerate TechnoClass::Array with brief info per entry
//   -WhatAmI Unit/Infantry/Building/Aircraft (optional; all four if omitted)
//   -House <int>  -Type <str>  -Mission <str>  -Limit <int>  -Offset <int>
//   Returns [{addr,whatami,whatami_str,type,hp,maxhp,mission,cell,house,selected,inlimbo},...]
//   Use this for battlefield awareness — it's faster than FindObjects for Techno queries.
void __cdecl IHVerify_FindTechnoInfo(JsonObject Context);

// Action: FindTypes -ID <pattern> -WhatAmI <UnitType|BuildingType|...> -Limit -Offset
void __cdecl IHVerify_FindTypes(JsonObject Context);

// Action: GetHouseInfo -House <index>  (default 0)
void __cdecl IHVerify_GetHouseInfo(JsonObject Context);

// Action: GetTechnoInfo -Address <0x...>  (Unit/Infantry/Building/Aircraft)
void __cdecl IHVerify_GetTechnoInfo(JsonObject Context);

// Condition: IsObjectAlive -Address <0x...> — check if pointer exists in AbstractClass::Array
void __cdecl IHVerify_IsObjectAlive(JsonObject Context);

// Action: QueueMission -Address <0x...> -Mission <name|int> (Guard/Harvest/...)
void __cdecl IHVerify_QueueMission(JsonObject Context);

void __cdecl IHVerify_StartRepairBuilding(JsonObject Context);

// Action: GetMapInfo — returns map dimensions
void __cdecl IHVerify_GetMapInfo(JsonObject Context);
// Action: IsOnMap -X -Y — check cell validity
void __cdecl IHVerify_IsOnMap(JsonObject Context);
// Action: GetObjectCoords -Address <0x...> — position/height/air status
void __cdecl IHVerify_GetObjectCoords(JsonObject Context);
// Action: GetCellInfo -X -Y — terrain properties
void __cdecl IHVerify_GetCellInfo(JsonObject Context);
// Action: FindObjectsOnCell -X -Y — things on a cell
void __cdecl IHVerify_FindObjectsOnCell(JsonObject Context);
// Action: FindObjectsInRange -X -Y -Range [-WhatAmI] [-House]
void __cdecl IHVerify_FindObjectsInRange(JsonObject Context);
// Action: FindAerialObjects -X -Y -Range — flying Technos
void __cdecl IHVerify_FindAerialObjects(JsonObject Context);
// Action: CalcCellSpread -Spread — cell list for spread value
void __cdecl IHVerify_CalcCellSpread(JsonObject Context);

// Action: SelectObject -Address <0x...> [-Deselect]
void __cdecl IHVerify_SelectObject(JsonObject Context);

// Action: DeployObject -Address <0x...> [-Method clicked|mission]
void __cdecl IHVerify_DeployObject(JsonObject Context);

// Action: IssueOrder -Address <0x...> -Mission <Move|Attack|AttackMove|Guard|AreaGuard|Stop|Hunt|Retreat|Harvest|Enter|Capture|Patrol|Unload> [-ToX -ToY | -To <0x...>] [-Target <0x...>]
void __cdecl IHVerify_IssueOrder(JsonObject Context);

// Action: SetDestination -Address <0x...> [-ToX -ToY | -To <0x...> | -Clear]
void __cdecl IHVerify_SetDestination(JsonObject Context);

// Action: GetMovementState -Address <0x...>
void __cdecl IHVerify_GetMovementState(JsonObject Context);

// Condition: CanEnterCell -Address <0x...> -X <int> -Y <int>
void __cdecl IHVerify_CanEnterCell(JsonObject Context);

// ═══════════════════════════════════════════════════════════
//  Cell operations — Shroud / Tiberium / Radiation
// ═══════════════════════════════════════════════════════════

// Conversion: CellAddrToCoords -Address <cell 0x...>  → {X,Y}
void __cdecl IHVerify_CellAddrToCoords(JsonObject Context);
// Conversion: CellCoordsToAddr -X <int> -Y <int>  → "0x..." | error
void __cdecl IHVerify_CellCoordsToAddr(JsonObject Context);

// Shroud
void __cdecl IHVerify_GetCellShroud(JsonObject Context);        // -X -Y → {IsShrouded,ShroudCounter}
void __cdecl IHVerify_UnshroudCell(JsonObject Context);         // -X -Y
void __cdecl IHVerify_AdjustCellShroud(JsonObject Context);     // -X -Y -Action Increase|Decrease|Clear

// Tiberium
void __cdecl IHVerify_GetCellTiberium(JsonObject Context);      // -X -Y → {Index,Value,TypeName}
void __cdecl IHVerify_AddCellTiberium(JsonObject Context);      // -X -Y -Index <int> -Amount <int>
void __cdecl IHVerify_RemoveCellTiberium(JsonObject Context);   // -X -Y -Amount <int>
void __cdecl IHVerify_SpreadCellTiberium(JsonObject Context);   // -X -Y [-Force]

// Radiation
void __cdecl IHVerify_GetCellRadiation(JsonObject Context);     // -X -Y → {IsRadiated,RadLevel}
void __cdecl IHVerify_IncreaseCellRadiation(JsonObject Context);// -X -Y -Amount <double>
void __cdecl IHVerify_DecreaseCellRadiation(JsonObject Context);// -X -Y -Amount <double>

// Action: GetFrameRateInfo — returns {CurrentFrame,InstantFPS,AverageFPS,GameSpeed,SpeedName}
void __cdecl IHVerify_GetFrameRateInfo(JsonObject Context);

// ═══════════════════════════════════════════════════════════
//  Production / Factory / TechnoType / Building queries
// ═══════════════════════════════════════════════════════════

// Action: CheckBuildability — CanBuild + HasFactoryForObject + GetFactoryProducing + GetPrimaryFactory
//   -House <int>  -TypeID <str>  [-AllowInProduction false]
void __cdecl IHVerify_CheckBuildability(JsonObject Context);

// Action: GetFactoryInfo — query factory state
//   -Address <0x...>
void __cdecl IHVerify_GetFactoryInfo(JsonObject Context);

// Action: FactoryProduce — fire PRODUCE event (equiv. clicking cameo)
//   -House <int>  -TypeID <str>
void __cdecl IHVerify_FactoryProduce(JsonObject Context);

// Action: FactoryPlace — fire PLACE event (equiv. placing building)
//   -House <int>  -TypeID <str>  -X <int>  -Y <int>
void __cdecl IHVerify_FactoryPlace(JsonObject Context);

// Action: FactorySuspend — fire SUSPEND event
//   -House <int>  -TypeID <str>
void __cdecl IHVerify_FactorySuspend(JsonObject Context);

// Action: FactoryAbandon — fire ABANDON event
//   -House <int>  -TypeID <str>
void __cdecl IHVerify_FactoryAbandon(JsonObject Context);

// Action: FactoryCompleteProduction  -Address <0x...>
//   Calls CompletedProduction() to finalize the current item and start next in queue
void __cdecl IHVerify_FactoryCompleteProduction(JsonObject Context);

// Condition: FindFactory — FactoryClass::FindByOwnerAndProduct
//   -House <int>  -TypeID <str>
void __cdecl IHVerify_FindFactory(JsonObject Context);

// Action: GetHouseTechTree — enumerate all types that a House can build
//   -House <int>  (default 0)
//   Returns {House, Buildable:[{WhatAmI,ID,Name,UIName,Cost,TechLevel,...}], TotalCount}
void __cdecl IHVerify_GetHouseTechTree(JsonObject Context);

// Action: GetTechBuildingTypes — enumerate Neutral Tech Building Types.
//  (no args)
//   Returns {Buildable:[{OwningHouseID,WhatAmI,ID,Name,UIName,Cost,TechLevel,...}], TotalCount}
void __cdecl IHVerify_GetTechBuildingTypes(JsonObject Context);

// Action: GetTechnoTypeInfo — static TechnoType properties
//   -TypeID <str>
void __cdecl IHVerify_GetTechnoTypeInfo(JsonObject Context);

// Action: GetBuildingStatus — building placement/construction state
//   -Address <0x...>
void __cdecl IHVerify_GetBuildingStatus(JsonObject Context);

// Condition: CanPlaceBuilding — BuildingTypeClass::CanPlaceHere
//   -TypeID <str>  -X <int>  -Y <int>  [-House 0]
void __cdecl IHVerify_CanPlaceBuilding(JsonObject Context);

// Action: GetHouseProduction — current production type indices
//   -House <int>
void __cdecl IHVerify_GetHouseProduction(JsonObject Context);

// Action: AIProduce — directly set ProducingXxxTypeIndex for AI house
//   -House <int>  -TypeID <str>
//   BuildingClass::AI picks it up next frame and calls BeginProduction.
//   This is the AI path — no EventClass involved. Works for AI houses.
void __cdecl IHVerify_AIProduce(JsonObject Context);

// Action: AIStopProduction — clear ProducingXxxTypeIndex back to -1
//   -House <int>  -WhatAmI <str>   (UnitType|BuildingType|InfantryType|AircraftType)
//   or  -All  to clear all four at once
void __cdecl IHVerify_AIStopProduction(JsonObject Context);

// Action: ListAllFactories — iterate FactoryClass::Array, return all factory addresses with owner
//   (no args)
void __cdecl IHVerify_ListAllFactories(JsonObject Context);

// Action: WatchPointer -Address <0x...> -CallbackLib <str> -CallbackMethod <str> [-RemovedOnly bool]
void __cdecl IHVerify_WatchPointer(JsonObject Context);
// Action: WatchBuildingReady -Address <0x...> -CallbackLib <str> -CallbackMethod <str>
//   Calls back when building is ActuallyPlacedOnMap && HasBuildUp && !InLimbo
void __cdecl IHVerify_WatchBuildingReady(JsonObject Context);
// Called every frame via IHCore::FrameUpdate broadcast
void __cdecl IHVerify_FrameUpdate(JsonObject Context);

// ═══════════════════════════════════════════════════════════
//  Marker system
// ═══════════════════════════════════════════════════════════

// Mark an AbstractClass* with a label (used in ACP + cleanup)
void MarkObject(DWORD addr, const std::u8string& label);

// Remove a mark
void UnmarkObject(DWORD addr);

// Check if an address is marked, return true and fill label
bool IsObjectMarked(DWORD addr, std::string& outLabel);

// Action: MarkObject -Address <0x...> -Label <string>
void __cdecl IHVerify_MarkObject(JsonObject Context);

// Action: UnmarkObject -Address <0x...>
void __cdecl IHVerify_UnmarkObject(JsonObject Context);

// Action: ListMarks — output all current marks to console
void __cdecl IHVerify_ListMarks(JsonObject Context);

// Action: ClearMarks — remove all marks
void __cdecl IHVerify_ClearMarks(JsonObject Context);

// ACP callback (registered in Reset)
const char* __cdecl MarkerACP(const AddressCommentInfo& AddrInfo);

// Manual test 
void __cdecl IHVerify_ManualTest(JsonObject Context);

// Shared game-started flag
extern std::atomic<bool> _GameStarted;
