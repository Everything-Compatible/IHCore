#pragma once
#include <EC.h>
#include <Debug.h>
#include <unordered_map>

void IHAgentBackend_Init();
void IHAgentBackend_OrderedInit();  // 检查 IHCore 版本，设置监听器 + ACP
void __cdecl IHAgentBackend_OnGameReset(JsonObject);  // 游戏启动时通过 IHCore::Reset 调用

// 辅助函数：检查指针是否仍在 AbstractClass::Array 中（存活）
bool IsPointerAlive(DWORD addr);
// 辅助函数：检查工厂指针是否仍在 FactoryClass::Array 中
bool IsFactoryAlive(DWORD addr);

// Route: <Status.xxx> / <Status.House.N.xxx>
UTF8_CString __cdecl StatusRouter(const char* Key);

// ═══════════════════════════════════════════════════════════
//  v2 查询层（Agent.Coord.cpp — Phase C）
//
//  所有查询接受 `-SessionID` 并通过 RequireSession(Context) 验证
//  （该函数守卫 _GameStarted + SessionID 查找）。返回 Techno/
//  Cell 信息的查询通过 Phase A 的 IsTechnoVisibleTo /
//  IsCellVisibleTo 应用可见性过滤。作弊 Session 可以看到全部。
//
//  重命名（v1 → v2）：
//    GetHouseTechTree    → GetTechTree
//    GetTechnoTypeInfo   → GetTypeInfo
//    FindObjectsOnCell   → GetObjectsOnCell
//    FindAerialObjects   → GetAerialObjects
//    GetHouseProduction + GetFactoryInfo + ListAllFactories → GetProductionState
//  合并：
//    FindObjects + FindTechnoInfo + FindObjectsInRange → GetTechnoInRange
//  增强：
//    GetTechnoInfo 现在吸收了 GetMovementState + GetBuildingStatus
//  新增：
//    GetForceDistribution、GetCurrentTime、GetPlayerViewRoute、FindPath（存根）
//
//  已移除（不再注册）：
//    FindObjects、FindTechnoInfo、FindTypes、GetHouseTechTree、
//    GetTechnoTypeInfo、FindObjectsOnCell、FindObjectsInRange、FindAerialObjects、
//    GetMovementState、GetHouseProduction、GetFactoryInfo、GetBuildingStatus、
//    GetTechBuildingTypes、IsObjectAlive（动作）、IsOnMap、GetObjectCoords、
//    CalcCellSpread
// ═══════════════════════════════════════════════════════════

// 动作：GetMapInfo -SessionID — 返回地图尺寸 {Width,Height,Left,Top,Right,Bottom}
void __cdecl IHAgentBackend_GetMapInfo(JsonObject Context);

// 动作：GetCellInfo -SessionID -X -Y — 地形属性（可见性过滤）
//   可见时返回 {Visible:true, LandType, OccupationFlags, AltOccupationFlags,
//            ContainsBridge, SlopeIndex}；
//   不可见时返回 {Visible:false, Cell:[X,Y]}。
void __cdecl IHAgentBackend_GetCellInfo(JsonObject Context);

// 动作：GetHouseInfo -SessionID [-House <int>] — 结构化 House 信息
//   默认 House = Session 所属的 House。非同盟 House 获得有限信息
//   （Name/Faction/IsPlayer/FullInfo:false）。己方/同盟/作弊获得完整信息。
//   聚类检测（3×3 范围内 ≥5 个同 House 的 Unit/Infantry）仅对己方 House 执行。
void __cdecl IHAgentBackend_GetHouseInfo(JsonObject Context);

// 动作：GetTechnoInfo -SessionID -Address <0x...|int>
//   Unit/Infantry/Building/Aircraft。可见性过滤。
//   不可见时：{Visible:false, Address:"0x...", LastKnownCell:null}
//   可见时：完整 Techno 状态 + 吸收的 GetMovementState 字段
//   （CurrentMission/QueuedMission/MissionStatus/IsMoving/Destination/Target/
//    HeadToCoord/SpeedPercentage/MovementZone/SpeedType）+ 吸收的
//   建筑物 GetBuildingStatus 字段（ActuallyPlacedOnMap/IsReadyToCommence/
//    HasBuildUp/BState/QueueBState/Factory/FactorySuspended/FactoryManual/
//    HasPower/UpgradeLevel/BeingProduced/ShouldRebuild/HasBeenCaptured/IsDamaged）
//   + Transporter/Passengers/Occupants/GarrisonBuilding/BridgeStatus/HealthStatus
void __cdecl IHAgentBackend_GetTechnoInfo(JsonObject Context);

// 动作：GetTechTree -SessionID [-House <int>]
//   （从 GetHouseTechTree 重命名）。枚举该 House 可建造的 UnitType/InfantryType/
//   BuildingType/AircraftType（CanBuild >= 1）。
//   非己方 House 需要作弊或同盟；否则返回 ERROR_ACCESS_DENIED。
//   返回 {House, Buildable:[{WhatAmI,WhatAmIInt,ID,Name,UIName,Cost,...}], TotalCount}
void __cdecl IHAgentBackend_GetTechTree(JsonObject Context);

// 动作：GetTypeInfo -SessionID -TypeID <str>
//   （从 GetTechnoTypeInfo 重命名）。静态 TechnoType 属性（不区分大小写的
//   ID 匹配）。返回 {TypeID,Name,WhatAmI,Cost,Strength,Armor,TechLevel,
//   BuildLimit,Sight,Speed,ROT,Passengers,Storage,Weight,PhysicalSize,Naval,
//   Organic,Cloakable,...,BuildCat,TurretCount,WeaponCount,Subterranean,
//   Underwater,JumpJet,GuardRange,Category,SpeedType,MovementZone,DeployTime,
//   UndeployDelay}。无可见性过滤（静态类型信息）。
void __cdecl IHAgentBackend_GetTypeInfo(JsonObject Context);

// 动作：GetObjectsOnCell -SessionID -X -Y — 单元格上的 Techno 地址
//   （从 FindObjectsOnCell 重命名）。可见性过滤：跳过不可见的 Techno。
//   返回 ["0x...", ...]
void __cdecl IHAgentBackend_GetObjectsOnCell(JsonObject Context);

// 动作：GetAerialObjects -SessionID -X -Y [-Range <int>]
//   （从 FindAerialObjects 重命名）。枚举 (X,Y) 周围 ±rangeCells 范围内的
//   AircraftClass::Array。可见性过滤。返回 [{addr,whatami:2}, ...]
void __cdecl IHAgentBackend_GetAerialObjects(JsonObject Context);

// Action: GetTechnoInRange -SessionID -X -Y -Range [-WhatAmI Unit|Infantry|Building|Aircraft]
//        [-House <int>] [-Type <str>] [-Limit <int>] [-Offset <int>]
//   (merges FindObjects + FindTechnoInfo + FindObjectsInRange). Iterates
//   CellSpread::NumCells(range) cells around (X,Y), enumerating each cell's
//   FirstObject chain. Visibility-filtered. Returns [{addr,whatami,whatami_str,
//   type,hp,maxhp,mission,cell:[X,Y],house,selected,inlimbo}, ...].
void __cdecl IHAgentBackend_GetTechnoInRange(JsonObject Context);

// 动作：GetProductionState -SessionID
//   （合并了 GetHouseProduction + GetFactoryInfo + ListAllFactories）。枚举
//   Session 所属 House 中 Factory!=nullptr 的建筑。返回
//   {House, ProducingBuildingTypeID, ProducingUnitTypeID, ProducingInfantryTypeID,
//    ProducingAircraftTypeID, ProducingXxxTypeIndex（4 个整数）,
//    Factories:[{Address,TypeID,Queue:[...],Progress,Suspended,IsPrimary,IsManual,
//               OnHold,IsDone,CostPerStep,Balance,SpecialItem}, ...]}
void __cdecl IHAgentBackend_GetProductionState(JsonObject Context);

// Action: GetForceDistribution -SessionID [-House <int>]
//   Buckets the House's Technos by type (Infantry/Unit/Building/Aircraft) × 3×3
//   spatial region grid over the map bounds. Returns
//   {House, Total, ByType:{Infantry,Unit,Building,Aircraft},
//    ByRegion:[{Region:[rx,ry],Infantry,Unit,Building,Aircraft,Total}, ...]}
//   (9 entries in order (0,0)..(2,2)).
void __cdecl IHAgentBackend_GetForceDistribution(JsonObject Context);

// 动作：GetCurrentTime -SessionID — {Frame, GameSeconds=Frame/15.0}
void __cdecl IHAgentBackend_GetCurrentTime(JsonObject Context);

// 动作：GetPlayerViewRoute -SessionID -FromTime <double> -ToTime <double>
//   过滤 Session 的 ViewHistory（Frame 在 [FromTime*15, ToTime*15] 范围内）。
//   返回 [{Time, Cell:[X,Y]}, ...]
void __cdecl IHAgentBackend_GetPlayerViewRoute(JsonObject Context);

// 动作：FindPath -SessionID -FromX -FromY -ToX -ToY [-TechnoTypeID <str>]
//   存根。返回 {Success:false, Reason:"NotImplemented"} 和
//   ERROR_NOT_SUPPORTED。实现待定：基于 CellClass 的 A* 或
//   UnitClass::AStarAttempt 封装 — 参见 STUBS-TODO.md（Phase E）
void __cdecl IHAgentBackend_FindPath(JsonObject Context);

// ═══════════════════════════════════════════════════════════
//  v2 Operation layer (Agent.Actions.cpp — Phase B)
//  All handlers gate on RequireSession(Context) which reads -SessionID.
//  All writes go through EventClass::AddEvent; no direct engine calls.
// ═══════════════════════════════════════════════════════════

// Action: IssueOrder -SessionID <id> -Mission <name|int>
//        [-Units ["0x...", ...]] [-Target "0x..."] [-X -Y]
//   Fires one MEGAMISSION per unit. If -Units omitted, uses session House's
//   currently selected Technos (IsSelected && GetOwningHouseIndex()==House).
void __cdecl IHAgentBackend_IssueOrder(JsonObject Context);

// Action: Deploy -SessionID -Address <"0x...">       (EventType::DEPLOY)
void __cdecl IHAgentBackend_Deploy(JsonObject Context);
// Action: Detonate -SessionID -Address <"0x...">     (EventType::DETONATE)
void __cdecl IHAgentBackend_Detonate(JsonObject Context);
// Action: Scatter -SessionID -Address <"0x...">      (EventType::SCATTER)
void __cdecl IHAgentBackend_Scatter(JsonObject Context);

// Action: Produce -SessionID -TypeID <str>           (EventType::PRODUCE)
void __cdecl IHAgentBackend_Produce(JsonObject Context);
// 动作：Suspend -SessionID -TypeID <str>           (EventType::SUSPEND)
void __cdecl IHAgentBackend_Suspend(JsonObject Context);
// 动作：Abandon -SessionID -TypeID <str>           (EventType::ABANDON)
void __cdecl IHAgentBackend_Abandon(JsonObject Context);
// 动作：AbandonAll -SessionID                     (EventType::ABANDON_ALL)
void __cdecl IHAgentBackend_AbandonAll(JsonObject Context);
// 动作：SetPrimary -SessionID -TypeID <str>       (EventType::PRIMARY)
void __cdecl IHAgentBackend_SetPrimary(JsonObject Context);

// 动作：Place -SessionID -TypeID <str> -X -Y      (EventType::PLACE)
void __cdecl IHAgentBackend_Place(JsonObject Context);
// 动作：SpecialPlace -SessionID -TypeID <str> -X -Y (EventType::SPECIAL_PLACE)
void __cdecl IHAgentBackend_SpecialPlace(JsonObject Context);

// Action: Repair -SessionID -Address <"0x...">       (EventType::REPAIR)
void __cdecl IHAgentBackend_Repair(JsonObject Context);
// Action: Sell -SessionID -Address <"0x...">          (EventType::SELL)
void __cdecl IHAgentBackend_Sell(JsonObject Context);
// 动作：SellCell -SessionID -X -Y                   (EventType::SELLCELL)
void __cdecl IHAgentBackend_SellCell(JsonObject Context);

// 动作：Ally -SessionID -TargetHouse <int>          (EventType::ALLY)
void __cdecl IHAgentBackend_Ally(JsonObject Context);
// 动作：Cheer -SessionID                            (EventType::ALLCHEER)
void __cdecl IHAgentBackend_Cheer(JsonObject Context);

// Action: AIProduce — directly set ProducingXxxTypeIndex for the session's House.
//   -SessionID <id>  -TypeID <str>
//   Cheat-only. BuildingClass::AI picks the index up next frame and calls
//   BeginProduction. This is the AI path — no EventClass involved.
void __cdecl IHAgentBackend_AIProduce(JsonObject Context);

// 动作：AIStopProduction — 将 ProducingXxxTypeIndex 清空为 -1
//   -SessionID <id>  -WhatAmI <str>   （UnitType|BuildingType|InfantryType|AircraftType）
//   或 -All 一次性清除全部四个。仅作弊。
void __cdecl IHAgentBackend_AIStopProduction(JsonObject Context);

// ═══════════════════════════════════════════════════════════
//  单元格操作 — Shroud / Tiberium / Radiation / 坐标转换
//  辅助（Phase D 范畴）。不需要 -SessionID。
// ═══════════════════════════════════════════════════════════

// Conversion: CellAddrToCoords -Address <cell 0x...>  → {X,Y}
void __cdecl IHAgentBackend_CellAddrToCoords(JsonObject Context);
// Conversion: CellCoordsToAddr -X <int> -Y <int>  → "0x..." | error
void __cdecl IHAgentBackend_CellCoordsToAddr(JsonObject Context);

// Shroud（战争迷雾）
void __cdecl IHAgentBackend_GetCellShroud(JsonObject Context);        // -X -Y → {IsShrouded,ShroudCounter}
void __cdecl IHAgentBackend_UnshroudCell(JsonObject Context);         // -X -Y
void __cdecl IHAgentBackend_AdjustCellShroud(JsonObject Context);     // -X -Y -Action Increase|Decrease|Clear

// Tiberium（泰伯利亚矿）
void __cdecl IHAgentBackend_GetCellTiberium(JsonObject Context);      // -X -Y → {Index,Value,TypeName}
void __cdecl IHAgentBackend_AddCellTiberium(JsonObject Context);      // -X -Y -Index <int> -Amount <int>
void __cdecl IHAgentBackend_RemoveCellTiberium(JsonObject Context);   // -X -Y -Amount <int>
void __cdecl IHAgentBackend_SpreadCellTiberium(JsonObject Context);   // -X -Y [-Force]

// Radiation（辐射）
void __cdecl IHAgentBackend_GetCellRadiation(JsonObject Context);     // -X -Y → {IsRadiated,RadLevel}
void __cdecl IHAgentBackend_IncreaseCellRadiation(JsonObject Context);// -X -Y -Amount <double>
void __cdecl IHAgentBackend_DecreaseCellRadiation(JsonObject Context);// -X -Y -Amount <double>

// 动作：GetFrameRateInfo — 返回 {CurrentFrame,InstantFPS,AverageFPS,GameSpeed,SpeedName}
void __cdecl IHAgentBackend_GetFrameRateInfo(JsonObject Context);

// ═══════════════════════════════════════════════════════════
//  帧更新（Phase D — Watch* 动作已移除；回调现在
//  从 Session_FrameUpdate + Session_BuildingReadyPoll
//  以及 Agent.Marker.cpp 中的 PointerExpiredProcess 自动推送）
// ═══════════════════════════════════════════════════════════

// 每帧通过 IHCore::FrameUpdate 广播调用。
// v2：驱动 Session_FrameUpdate（超时 + 视口采样）和
//     Session_BuildingReadyPoll（OnBuildingReady 自动分发）。
void __cdecl IHAgentBackend_FrameUpdate(JsonObject Context);

// ═══════════════════════════════════════════════════════════
//  标记系统（Marker）
// ═══════════════════════════════════════════════════════════

// 用标签标记一个 AbstractClass*（用于 ACP + 清理）
void MarkObject(DWORD addr, const std::u8string& label);

// 移除标记
void UnmarkObject(DWORD addr);

// 检查地址是否被标记，返回 true 并填充标签
bool IsObjectMarked(DWORD addr, std::string& outLabel);

// 动作：MarkObject -Address <0x...> -Label <string>
void __cdecl IHAgentBackend_MarkObject(JsonObject Context);

// 动作：UnmarkObject -Address <0x...>
void __cdecl IHAgentBackend_UnmarkObject(JsonObject Context);

// 动作：ListMarks — 将所有当前标记输出到控制台
void __cdecl IHAgentBackend_ListMarks(JsonObject Context);

// Action: ClearMarks — remove all marks
void __cdecl IHAgentBackend_ClearMarks(JsonObject Context);

// ACP 回调（在 Reset 中注册）
const char* __cdecl MarkerACP(const AddressCommentInfo& AddrInfo);

// 手动测试
void __cdecl IHAgentBackend_ManualTest(JsonObject Context);

// 共享的游戏启动标志
extern std::atomic<bool> _GameStarted;

// ═══════════════════════════════════════════════════════════
//  v2 Session management (Agent.Session.cpp)
//
//  Naming: YR engine already has SessionClass::Instance (single/multi-player
//  session). Our struct is AgentSession to avoid collision.
//
//  Thread safety contract for returned raw AgentSession*:
//    All registry mutations (insert/erase) hold g_SessionMutex. The raw
//    pointers returned by Find*/Require* are valid only until the next call
//    that might erase sessions (EndSession / Session_OnGameReset /
//  Session_FrameUpdate's timeout branch). Use them for a short critical
//    region only, or copy the needed fields out under mutex.
// ═══════════════════════════════════════════════════════════

struct AgentSession;  // 前向声明 — 完整定义在 Agent.Session.cpp 中

std::string     GenerateUUIDv4();
AgentSession*   FindSessionByID(const std::string& id);
AgentSession*   FindSessionByHouse(int houseIdx);
AgentSession*   RequireSession(JsonObject Context);
bool            IsTechnoVisibleTo(TechnoClass* pT, AgentSession* pS);
bool            IsCellVisibleTo(CellClass* pC, AgentSession* pS);
void            DispatchCallback(AgentSession* pS, const char* methodName, cJSON* payload);
// Phase D 辅助函数，供 PointerExpiredProcess（Agent.Marker.cpp）使用：
// 从 Session 的私有字段 + 已死亡单位的类型信息构建 OnUnitDestroyed 载荷，
// 然后转发给 DispatchCallback。AgentSession 的完整定义对 Agent.Session.cpp
// 是私有的，因此此辅助函数隐藏了字段读取。KillerAddress 暂为存根
// "0x00000000"（参见 STUBS-TODO.md）。
void            DispatchUnitDestroyed(AgentSession* pS, const char* typeId, DWORD addr);
// Phase E stub — dispatch 入口已实现，等待 hook 触发点（参见 STUBS-TODO.md #1/#2/#3）
void            DispatchUnderAttack(AgentSession* pS, DWORD victimAddr, DWORD attackerAddr, int damage);
void            DispatchChatMessage(AgentSession* pS, int fromHouse, int toHouse, const char* message, bool isAlly);
void            DispatchBeacon(AgentSession* pS, const char* action, int cellX, int cellY);
void            Session_FrameUpdate();
void            Session_BuildingReadyPoll();  // Phase D — OnBuildingReady 自动分发
void            Session_OnGameReset();

// v2 动作处理程序
void __cdecl    IHAgentBackend_BeginSession(JsonObject Context);
void __cdecl    IHAgentBackend_EndSession(JsonObject Context);
void __cdecl    IHAgentBackend_SessionHeartbeat(JsonObject Context);
void __cdecl    IHAgentBackend_GetToolList(JsonObject Context);

// ═══════════════════════════════════════════════════════════
//  导出给 IHCore 项目使用的 C 接口（非 EC Action）
// ═══════════════════════════════════════════════════════════

// 查询指定 House 是否已被 Agent Session 接管。供 IHCore 的 House AI
// 跳过 hook 调用。参见 STUBS-TODO.md #6。
extern "C" __declspec(dllexport) bool IsHouseTaken(int houseIdx);

