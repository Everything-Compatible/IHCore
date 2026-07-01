# IHAgentBackend v2 迁移计划

## Context

`IHAgentBackend` 当前实现是 v1 架构：直接调用 `SetDestination` / `QueueMission` / `ClickedEvent` / `IsBeingRepaired=true` 等引擎底层接口，无会话管理，无可见性过滤，回调依赖前端主动 `WatchPointer` / `WatchBuildingReady` 注册。

[CLAUDE.md](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/CLAUDE.md) 和 [API-SPEC.md](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/API-SPEC.md) 已定义 v2 架构：基于会话（SessionID + House 接管 + Cheat 标志）+ 操作层全面 EventClass 化 + 可见性过滤 + 会话级主动回调。本计划执行该迁移。

## 用户已确认的关键决策

1. **推进方式**：串行调用 4 个子 agent，依次完成 Phase A/B/C/D
2. **无法实现的回调**：先 stub，最后汇总成补充清单文档
3. **FindPath**：本阶段 stub
4. **旧 API**：全部删除（旧 Action 注册和实现一并清除）

## 引擎能力盘点（已通过 Explore 确认）

### 可直接使用
- **EventClass**：`f:\RA2 Engine Extension\IHCore\IHCore\YRPP\EventClass.h` 已定义全部 17 种 EventType 和 9 种构造函数。`static bool EventClass::AddEvent(const EventClass&)`
- **PostAsyncRemoteCall**：`f:\RA2 Engine Extension\IHCore\Common\IHLoader\IH.Ext.h:256` 声明 `Ext::PostAsyncRemoteCall(const char* pLib, const char* pFunc, int Version, JsonObject)`
- **指针过期监听**：`ECListener::Listen_PointerExpired` 已在 `Agent.Core.cpp:35` 注册，回调 `PointerExpiredProcess`
- **视口**：`TacticalClass::Instance->TacticalPos`（屏幕左上像素），`TacticalClass::CoordsToCell` / `ClientToCoords` 可转单元格
- **可见性原语**：`CellClass::IsShrouded()` / `HouseClass::IsAlliedWith(...)` 4 重载 / `MapClass::IsLocationShrouded(...)` / `TechnoClass::Cloakable` / `TechnoClass::HasBeenAttacked`
- **House 迭代**：`HouseClass::Array` / `HouseClass::CurrentPlayer()` / `IsControlledByHuman()` / `IsObserver()`
- **Patch / Hook**：`Patch::Apply_LJMP(offset, fn)` / `DEFINE_HOOK(addr, name, size)` 宏（`f:\RA2 Engine Extension\IHCore\IHCore\YRPP\Syringe.h:298`）— 用于未来扩展 inline hook
- **现有工具复用**：`IsPointerAlive` / `IsFactoryAlive`（保留为内部 helper）、`MarkerACP` / `MarkObject` 系列（保留调试用）

### 缺失（本阶段 stub）
- **OnChatMessage / OnBeacon / OnUnderAttack 的引擎 hook 地址**：未通过 IDA 定位，三个回调本阶段 stub
- **FindPath 的 A\***：仅有 `UnitClass::AStarAttempt`（实例方法，需 UnitClass\*），不符合 `-TechnoTypeID` 可选参数语义，本阶段 stub
- **UUID 生成库**：自实现 RFC 4122 v4 via `std::mt19937_64` + `std::random_device`
- **"接管 House 后原版 AI 跳过"**：CLAUDE.md 明确由 IHCore 层负责，本项目仅暴露会话状态供其读取，不实现 hook

### 命名冲突规避
YR 引擎已有 `SessionClass::Instance`（`f:\RA2 Engine Extension\IHCore\IHCore\YRPP\SessionClass.h`）。本项目的会话结构命名为 `AgentSession`，文件 `Agent.Session.cpp`，避免与引擎 SessionClass 混淆。

## 架构设计

### Session 数据结构（Agent.Session.cpp 新建）

```cpp
struct AgentSession {
    std::string ID;                  // UUID v4
    int House = -1;                  // HouseClass::ArrayIndex
    bool Cheat = false;
    int TimeoutSec = 30;
    int64 LastHeartbeatFrame = 0;    // 用 Unsorted::CurrentFrame 而非时间戳
    std::string CallbackLib = "IHAgentFrontend";  // 默认前端库名
    // OnBuildingReady 去重：已通知的建筑地址
    std::unordered_set<DWORD> AnnouncedBuildings;
    // GetPlayerViewRoute 0.5s 采样环形缓冲
    struct ViewSample { int64 Frame; CellStruct Cell; };
    std::vector<ViewSample> ViewHistory;  // 最多保留 60s（120 个采样）
    int64 LastViewSampleFrame = 0;
};

// 全局注册表（mutex 保护）
static std::mutex g_SessionMutex;
static std::unordered_map<std::string, AgentSession> g_SessionsByID;
static std::unordered_map<int, std::string> g_SessionByHouse;  // HouseIdx → SessionID

// 公共 API
std::string GenerateUUIDv4();
AgentSession* FindSessionByID(const std::string& id);
AgentSession* FindSessionByHouse(int houseIdx);
AgentSession* RequireSession(JsonObject Context);  // 读 -SessionID，找不到返回 nullptr 并设置错误
bool IsTechnoVisibleTo(TechnoClass* pT, AgentSession* pS);
bool IsCellVisibleTo(CellClass* pC, AgentSession* pS);
void Session_FrameUpdate();  // 超时检测 + 视口采样
void Session_OnGameReset();  // 清空所有会话
```

### 可见性算法

```cpp
bool IsTechnoVisibleTo(TechnoClass* pT, AgentSession* pS) {
    if (pS->Cheat) return true;
    auto pCell = pT->GetCell();
    if (pCell && pCell->IsShrouded()) return false;
    auto pH = pT->GetOwningHouse();
    if (!pH) return true;  // neutral
    auto pSelf = HouseClass::Array->Items[pS->House];
    if (pSelf->IsAlliedWith(pH)) return true;
    // 非同盟：AI 接管时排除隐形
    if (!pSelf->IsControlledByHuman() && pT->Cloakable && !pT->HasBeenAttacked) return false;
    return true;
}
```

### 回调派发约定

每个会话存 `CallbackLib`（默认 `"IHAgentFrontend"`，BeginSession 可通过 `-CallbackLib` 覆盖）。方法名固定为回调名（`"OnSessionClosed"` / `"OnBuildingReady"` / 等）。

派发助手：
```cpp
void DispatchCallback(AgentSession* pS, const char* methodName, cJSON* payload);
// 内部调用 Ext::PostAsyncRemoteCall(pS->CallbackLib.c_str(), methodName, INT_MAX, JsonObject(payload))
```

## 推进阶段

### Phase A — 会话管理基础（Sub-agent 1）

**新建文件：**
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Session.cpp`

**Agent.Session.cpp 包含：**
- `AgentSession` 结构定义
- 全局注册表（mutex 保护）
- `GenerateUUIDv4()`：基于 `std::mt19937_64` + `std::random_device`，RFC 4122 v4 格式
- `FindSessionByID` / `FindSessionByHouse` / `RequireSession(JsonObject)` 助手
- `IsTechnoVisibleTo` / `IsCellVisibleTo` 助手
- `Session_FrameUpdate()`：超时检测（`LastHeartbeatFrame + TimeoutSec * 15 < CurrentFrame` 触发 EndSession + OnSessionClosed 回调）+ 视口采样（每 7-8 帧采样一次 TacticalPos 转 Cell 存入 ViewHistory）
- `Session_OnGameReset()`：清空全部会话
- `DispatchCallback(AgentSession*, const char*, cJSON*)` 助手
- Action 实现：`IHAgentBackend_BeginSession` / `IHAgentBackend_EndSession` / `IHAgentBackend_SessionHeartbeat` / `IHAgentBackend_GetToolList`
  - BeginSession：检查 House 是否已被接管 → 生成 UUID → 注册 → 返回 spec 中规定的 JSON
  - EndSession：触发 OnSessionClosed（reason "released"）→ 从注册表移除
  - GetToolList：无 `-SessionID` 返回全部工具名；带则按 Cheat 过滤

**修改文件：**
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\AgentBackend.h`：追加 Agent.Session.cpp 的声明（additive，不动旧声明）
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Core.cpp`：在 `IHAgentBackend_OnGameReset` 中调用 `Session_OnGameReset()`；不动其他
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Marker.cpp`：在 `IHAgentBackend_FrameUpdate` 开头调用 `Session_FrameUpdate()`（在现有建筑轮询之前）
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\dllmain.cpp`：在 ECInitLibrary 注册表中追加 `BeginSession` / `EndSession` / `SessionHeartbeat` / `GetToolList`
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\IHAgentBackend.vcxproj` 和 `.filters`：加入 Agent.Session.cpp

**Phase A 验收**：能编译，能调用 BeginSession 返回 SessionID，EndSession 触发 OnSessionClosed 回调（前端未实现不影响后端单独测试，回调失败容错即可）。

### Phase B — 操作层 EventClass 化（Sub-agent 2）

**重写文件：**
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Actions.cpp`

**Agent.Actions.cpp 新内容：**
所有操作接受 `-SessionID`，通过 `RequireSession(Context)` 校验，再下发 EventClass：

| Action | EventType | 关键参数 |
|--------|-----------|---------|
| `IssueOrder` | MEGAMISSION(4) | -Mission -Units [list] -Target 0x... / -X -Y |
| `Deploy` | DEPLOY(9) | -Address |
| `Detonate` | DETONATE(10) | -Address |
| `Scatter` | SCATTER(7) | -Address |
| `Produce` | PRODUCE(14) | -TypeID |
| `Suspend` | SUSPEND(15) | -TypeID |
| `Abandon` | ABANDON(16) | -TypeID |
| `AbandonAll` | ABANDON_ALL(46) | （无） |
| `SetPrimary` | PRIMARY(17) | -TypeID |
| `Place` | PLACE(11) | -TypeID -X -Y |
| `SpecialPlace` | SPECIAL_PLACE(18) | -TypeID -X -Y |
| `Repair` | REPAIR(21) | -Address |
| `Sell` | SELL(22) | -Address |
| `SellCell` | SELLCELL(23) | -X -Y |
| `Ally` | ALLY(3) | -TargetHouse |
| `Cheer` | ALLCHEER(45) | （无） |
| `AIProduce` (Cheat) | — 直接设 ProducingXxxTypeIndex | -TypeID |
| `AIStopProduction` (Cheat) | — 直接清 ProducingXxxTypeIndex | -WhatAmI / -All |

IssueOrder 的 -Units 解析：JSON 数组 `["0x...", "0x..."]`，对每个 unit 调用一次 `MEGAMISSION` AddEvent。无 -Units 则使用当前选中单位（`HouseClass::CurrentPlayer` 的选中态枚举）。

**删除的旧 API**（从 `AgentBackend.h` 声明和 `dllmain.cpp` 注册表移除，并删除实现）：
- `IHAgentBackend_SelectObject` → 删
- `IHAgentBackend_DeployObject` → 改名为 `IHAgentBackend_Deploy`，重写
- `IHAgentBackend_QueueMission` → 删
- `IHAgentBackend_SetDestination` → 删
- `IHAgentBackend_StartRepairBuilding` → 改名为 `IHAgentBackend_Repair`，重写
- `IHAgentBackend_FactoryProduce` → 改名 `IHAgentBackend_Produce`，重写
- `IHAgentBackend_FactoryPlace` → 改名 `IHAgentBackend_Place`，重写
- `IHAgentBackend_FactorySuspend` → 改名 `IHAgentBackend_Suspend`，重写
- `IHAgentBackend_FactoryAbandon` → 改名 `IHAgentBackend_Abandon`，重写
- `IHAgentBackend_FactoryCompleteProduction` → 删（无对应玩家操作）
- `IHAgentBackend_FindFactory` → 删（前端不操心工厂地址）
- `IHAgentBackend_ListAllFactories` → 删（内化到 GetProductionState）
- `IHAgentBackend_CanEnterCell` → 删（后端内部校验）
- `IHAgentBackend_CheckBuildability` → 删（后端内部校验）
- `IHAgentBackend_CanPlaceBuilding` → 删（后端内部校验）
- `IHAgentBackend_GetMovementState` → 删（内化到 GetTechnoInfo）
- `IHAgentBackend_GetBuildingStatus` → 删（内化到 GetTechnoInfo）

**保留的内部 helper**（不注册为 Action）：`IsPointerAlive` / `IsFactoryAlive` / `ParseMission` / `FindTechnoTypeByID`（如需跨文件，移到 AgentBackend.h 的 internal 区段）。

**IssueOrder 实现**：用 `EventClass(houseIndex, src TargetClass, Mission, target TargetClass, dest TargetClass, follow TargetClass)` 构造 MEGAMISSION 事件。TargetClass 包装 TechnoClass\* 或 CellClass\* 或 CellStruct。

**修改文件**：
- `AgentBackend.h`：移除旧 Action 声明，添加新 Action 声明
- `dllmain.cpp`：更新注册表
- `Agent.Coord.cpp`：删除 `FindTechnoTypeByID` 如果与 Actions.cpp 重复 → 实际放在一个共享位置（建议移到 AgentBackend.h 的 inline static 或独立 internal helper 区段）

**Phase B 验收**：编译通过；通过 EC 调用 `BeginSession` + `Produce -TypeID` 能正确触发 PRODUCE 事件；旧 API `QueueMission` 等已不可调用（返回 not found）。

### Phase C — 查询层重命名合并 + 新增（Sub-agent 3）

**重写文件：**
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Coord.cpp`

**重命名 + SessionID 化：**
| 旧名 | 新名 |
|------|------|
| `GetHouseTechTree` | `GetTechTree` |
| `GetTechnoTypeInfo` | `GetTypeInfo` |
| `FindObjectsOnCell` | `GetObjectsOnCell` |
| `FindAerialObjects` | `GetAerialObjects` |
| `GetHouseProduction` + `GetFactoryInfo` + `ListAllFactories` | `GetProductionState` |
| `GetHouseInfo` | 保留，加 -SessionID 和可见性 |

**合并：**
- `FindObjects` + `FindTechnoInfo` + `FindObjectsInRange` → **`GetTechnoInRange`**（参数 -SessionID -X -Y -Range [-WhatAmI] [-House] [-Type] [-Limit] [-Offset]，返回结构化数组：addr/whatami/type/hp/maxhp/mission/cell/house/selected/inlimbo，应用可见性过滤）

**新增：**
- `GetForceDistribution`：接管 House 的兵力按类型（Infantry/Unit/Building/Aircraft）× 空间区域（3×3 网格分桶）统计
- `GetCurrentTime`：返回 `{Frame, GameSeconds}`，GameSeconds = Frame / 15
- `GetPlayerViewRoute -FromTime -ToTime`：从该会话的 `ViewHistory` 过滤 Frame 在 [FromTime, ToTime] 范围内的采样
- `FindPath`：**stub**，返回 `{"Success": false, "Reason": "NotImplemented"}` 并 `ECDebug::ReturnStdError(ERROR_NOT_SUPPORTED)`
- `GetProductionState`：枚举接管 House 的所有工厂（`HouseClass::Buildings` 遍历找 `Factory` 非空），返回 `[{Address, TypeID, Queue:[...], Progress, Suspended, IsPrimary}, ...]`

**可见性应用**：
- 所有 GetXxxInfo / GetTechnoInRange / GetObjectsOnCell / GetAerialObjects / GetCellInfo 调用 `IsTechnoVisibleTo` / `IsCellVisibleTo` 过滤
- 不可见的 Techno 跳过；不可见的 Cell 返回 LandType 但 OccupationFlags 等置 null

**修改文件**：
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Core.cpp`：移除已迁出的函数（FindObjects/FindTechnoInfo/FindTypes/GetHouseTechTree/GetTechnoInfo/GetHouseInfo/IsObjectAlive Action），保留 StatusRouter / IHAgentBackend_Init / IHAgentBackend_OrderedInit / IHAgentBackend_OnGameReset / IsPointerAlive / IsFactoryAlive
- `AgentBackend.h`：更新声明
- `dllmain.cpp`：更新注册表
- `Agent.Coord.cpp`：上述全部查询实现

**删除的旧 API**：`FindTypes` / `GetObjectCoords` / `IsOnMap` / `CalcCellSpread` / `GetTechBuildingTypes` / `GetHouseTechTree`（重命名不删实现）/ `GetHouseProduction` / `GetFactoryInfo` / `IsObjectAlive`（Action 注册删除，函数留作内部 helper）

**Phase C 验收**：编译通过；调用 `BeginSession` + `GetTechnoInRange -X -Y -Range 10` 返回结构化 JSON；`GetProductionState` 返回工厂详情；`FindPath` 返回 NotImplemented。

### Phase D — 回调层重构（Sub-agent 4）

**重写文件：**
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Marker.cpp`

**移除：**
- `IHAgentBackend_WatchPointer` / `IHAgentBackend_WatchBuildingReady` 的 Action 注册和实现
- `_Watches` / `_BuildingWatches` 全局存储
- `PointerExpiredProcess` 中的 watch 派发逻辑（保留 UnmarkObject 清理）

**新增（会话级自动派发）：**
- **OnSessionClosed**：在 `EndSession` 和 `Session_FrameUpdate` 超时分支中调用 `DispatchCallback(pS, "OnSessionClosed", payload)`，payload `{SessionID, House, Reason: "released"|"timeout"}`
- **OnBuildingReady**：在 `Session_FrameUpdate` 中遍历会话 House 的 `pHouse->Buildings`，对满足 `ActuallyPlacedOnMap && HasBuildUp && !InLimbo` 且未在 `AnnouncedBuildings` 中的建筑派发 `{SessionID, House, TypeID, Address, Cell:[X,Y]}` 并加入 AnnouncedBuildings
- **OnUnitDestroyed**：在 `PointerExpiredProcess` 中，对每个 dying Techno 检查 `GetOwningHouseIndex() == session->House`，匹配则派发 `{SessionID, House, TypeID, Address, KillerAddress}`（KillerAddress 暂为 0x00000000，引擎不直接提供）
- **OnUnderAttack**：**stub** — 注册 Action 但内部不触发；依赖未来 DEFINE_HOOK 实现
- **OnChatMessage**：**stub** — 同上
- **OnBeacon**：**stub** — 同上

**修改 `IHAgentBackend_FrameUpdate`**：调用 `Session_FrameUpdate()` 后再做 OnBuildingReady 轮询（可合并到 Session_FrameUpdate 内部）。

**修改 `PointerExpiredProcess`**：保留 UnmarkObject 清理；增加会话 House 匹配 → OnUnitDestroyed 派发。

**修改文件**：
- `AgentBackend.h`：移除 WatchPointer/WatchBuildingReady 声明
- `dllmain.cpp`：移除 WatchPointer/WatchBuildingReady 注册
- `Agent.Marker.cpp`：上述重构

**Phase D 验收**：编译通过；调用 `BeginSession` + 让某建筑完成建造，能触发 OnBuildingReady 回调（前端未实现，PostAsyncRemoteCall 失败时静默忽略）；调用 `EndSession` 触发 OnSessionClosed。

### Phase E — 汇总补充清单（主 agent）

4 个子 agent 完成后，主 agent 编写 `f:\RA2 Engine Extension\IHCore\IHAgentBackend\STUBS-TODO.md`，列出：

1. **OnUnderAttack 回调** — 需要在 `TechnoClass::ReceiveDamage` 或类似地址安装 `DEFINE_HOOK`，需要 IDA 逆向定位地址
2. **OnChatMessage 回调** — 需要在聊天接收函数安装 hook
3. **OnBeacon 回调** — 需要在信标 place/remove 函数安装 hook
4. **FindPath 实现** — 当前 stub，需要：a) 实现基于 CellClass 的 A\*；或 b) 用临时 UnitClass 实例调用 `AStarAttempt`
5. **OnUnitDestroyed 的 KillerAddress** — 当前为 0x0，引擎 `PointerExpiredProcess` 不提供击杀者；需 hook `WeaponClass::Fire` 或 `TechnoClass::ReceiveDamage` 跟踪最后攻击者
6. **"接管 House 后原版 AI 跳过"** — CLAUDE.md 声明由 IHCore 层负责，但当前 IHCore 未实现。需要 IHCore 添加 hook 读取本项目的 `FindSessionByHouse(idx) != nullptr` 来跳过 AI

## 跨阶段约定

- **内部 helper 共享**：`IsPointerAlive` / `IsFactoryAlive` / `FindTechnoTypeByID` / `AbsTypeName` / `BuildCatName` 等内部 helper 在 `AgentBackend.h` 末尾用 `// === internal helpers ===` 段集中声明，避免 Phase B/C 重复定义
- **错误码**：未授权 / 会话不存在 → `ERROR_NOT_FOUND`；House 已被接管 → `ERROR_INVALID_DATA` + JSON `{Success:false, Reason:"HouseAlreadyTaken"}`；游戏未开始 → `ERROR_NOT_READY`
- **JSON 输出**：所有 Action 用 `JsonFile F; auto Obj = F.GetObj(); ... Obj.AddXxx(...); std::string result = Obj.GetText(); ECDebug::ReturnString(...)` 模式（与现 GetTechnoInfo 一致）
- **测试方式**：后端单独测试可通过 EC 的 `RunCommand` 接口直接调用 Action 名 + JSON 参数；前端尚未实现不影响后端编译和单独验证

## 验证（端到端）

1. **编译**：用 Visual Studio 打开 `IHCore.sln`，编译 IHAgentBackend 项目（Release|Win32），确保零 error 零 warning（新增代码）
2. **加载**：在 YR（红警2尤里复仇）注入 Syringe + IHCore.dll + IHAgentBackend.dll 启动游戏
3. **会话流程**：
   - `BeginSession -House 0` → 返回 SessionID
   - `GetToolList -SessionID <id>` → 返回 Tools 列表
   - `SessionHeartbeat -SessionID <id>` → 刷新成功
   - `EndSession -SessionID <id>` → 触发 OnSessionClosed
4. **操作层**：`BeginSession` + `Produce -TypeID GAPOWR`（电厂）→ 游戏内出现生产队列
5. **查询层**：`BeginSession` + `GetTechnoInRange -X 100 -Y 100 -Range 5` → 返回 JSON 数组
6. **回调**：`BeginSession` + 让建筑完成建造 → 控制台观察 OnBuildingReady 派发尝试（前端未实现会失败，但 backend 日志可见）
7. **超时**：`BeginSession -Timeout 2`，2 秒内不心跳 → 自动 EndSession + OnSessionClosed(reason=timeout)
8. **Stub 验证**：`FindPath -FromX 0 -FromY 0 -ToX 10 -ToY 10` → 返回 NotImplemented

## 文件改动清单（总览）

**新建：**
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Session.cpp`
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\STUBS-TODO.md`（Phase E）

**重写：**
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Actions.cpp`（Phase B）
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Coord.cpp`（Phase C）
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Marker.cpp`（Phase D）
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\AgentBackend.h`（每阶段追加）
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\dllmain.cpp`（每阶段追加）

**修改：**
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Agent.Core.cpp`（Phase C 移除迁出函数，Phase A 增 OnGameReset 调用）
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\IHAgentBackend.vcxproj` 和 `.filters`（Phase A 加入 Agent.Session.cpp）
- `f:\RA2 Engine Extension\IHCore\IHAgentBackend\Pointers.cpp`（不动，ManualTest 保留）

## 子 Agent 调用顺序

1. **Sub-agent 1**：执行 Phase A，完成后报告 SessionID 生成 / 注册表 / 可见性 helper 可用
2. **Sub-agent 2**：执行 Phase B，依赖 Phase A 的 `RequireSession` helper
3. **Sub-agent 3**：执行 Phase C，依赖 Phase A 的可见性 helper 和 Phase B 的内部 helpers
4. **Sub-agent 4**：执行 Phase D，依赖 Phase A 的 `DispatchCallback` 和会话注册表
5. **主 agent**：编写 STUBS-TODO.md

每个子 agent 完成后，主 agent 验证文件改动与计划一致再启动下一个。
