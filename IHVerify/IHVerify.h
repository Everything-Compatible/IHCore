#pragma once
#include <EC.h>
#include <Debug.h>
#include <unordered_map>

void IHVerify_Init();
void IHVerify_OrderedInit();  // checks IHCore version, sets up listeners + ACP
void __cdecl IHVerify_OnGameReset(JsonObject);  // called via IHCore::Reset at game start
FuncInfo* IHVerify_GetFunc(const char* Name, int Version);

// Route: <Status.xxx> / <Status.House.N.xxx>
UTF8_CString __cdecl StatusRouter(const char* Key);

// Action: FindObjects -Type -House -WhatAmI -Mission -Limit -Offset
void __cdecl IHVerify_FindObjects(JsonObject Context);

// Action: FindTypes -ID <pattern> -WhatAmI <UnitType|BuildingType|...> -Limit -Offset
void __cdecl IHVerify_FindTypes(JsonObject Context);

// Action: GetHouseInfo -House <index>  (default 0)
void __cdecl IHVerify_GetHouseInfo(JsonObject Context);

// Action: QueueMission -Address <0x...> -Mission <name|int> (Guard/Harvest/...)
void __cdecl IHVerify_QueueMission(JsonObject Context);

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

// Shared game-started flag
extern std::atomic<bool> _GameStarted;
