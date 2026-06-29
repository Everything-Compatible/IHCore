# IHAgentBackend — 项目说明

> 配套 API 规范：[API-SPEC.md](API-SPEC.md)。本文件描述项目定位、架构和开发约定。

## 项目定位

IHAgentBackend 是 IHCore 引擎扩展的 **后端层**，负责会话管理、游戏状态查询与操作指令下发，为前端 Agent 系统提供唯一的引擎交互通道。

根据 [Agent架构.md](../Agent架构.md) 的划分，整个系统分为：

- **前端**（由 IHAgentFrontend 负责）：战略Agent、调度Agent、介入Agent、玩家指令接口
- **后端**（本项目负责）：会话管理、共享状态查询、操作指令下发（基于 EventClass）、事件回调

---

## 范围（Scope）

### 本项目负责

| 模块 | 说明 |
|------|------|
| **会话管理** (Session Management) | 前端会话生命周期：BeginSession / EndSession / SessionHeartbeat / GetToolList。接管 House 后的原版 AI 跳过逻辑 |
| **共享状态查询（宏观）** (Shared State Query) | GetHouseInfo / GetForceDistribution / GetMapInfo / GetTechTree / GetTypeInfo / GetProductionState / GetCurrentTime / GetPlayerViewRoute。含可见性控制 |
| **对象状态查询（微观）** (Object State Query) | GetTechnoInRange / GetTechnoInfo / GetObjectsOnCell / GetAerialObjects / GetCellInfo / FindPath。含可见性控制 |
| **操作指令** (Operations) | 基于 EventClass 的玩家操作模拟：IssueOrder / Deploy / Detonate / Scatter / Produce / Place / SpecialPlace / Repair / Sell 等 |
| **事件回调** (Callbacks) | OnSessionClosed / OnBuildingReady / OnUnitDestroyed / OnUnderAttack，会话级自动推送 |
| **辅助功能** | 单元格操作（Shroud/Tiberium/Radiation 读写）、标记系统（MarkObject）、StatusRouter、帧更新钩子 |

### 本项目不负责

- AI 决策、战略规划 —— 由前端 Agent 负责
- 任务拆解、Team 管理 —— 由前端的调度Agent和介入Agent负责
- 原版 AI 跳过 Hook 的具体实现 —— 由 IHCore 层负责（本项目仅通过会话状态驱动是否跳过）
- 指针存活验证 —— 由 `PointerExpiredProcess` 和内部校验自动处理，前端不可见

---

## 架构概览（v2）

```
┌─────────────────────────────────────────────────────────────┐
│                   前端层 (IHAgentFrontend)                    │
│  战略Agent · 调度Agent · 介入Agent · 玩家指令接口             │
└──────────────────────┬──────────────────────────────────────┘
                       │ ①BeginSession   ②操作/查询 (+SessionID)
                       ▼
┌─────────────────────────────────────────────────────────────┐
│                    IHAgentBackend 核心 (v2)                   │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────────────────────────────────────────────┐   │
│  │              会话管理                                │   │
│  │  BeginSession · EndSession · SessionHeartbeat       │   │
│  │  GetToolList · 超时检测(FrameUpdate)                │   │
│  └─────────────────────────────────────────────────────┘   │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              操作 (基于 EventClass)                  │   │
│  │  IssueOrder · Deploy · Detonate · Scatter           │   │
│  │  Produce · Suspend · Abandon · AbandonAll           │   │
│  │  Place · SpecialPlace · Repair · Sell · SellCell    │   │
│  │  SetPrimary · Ally · Cheer                          │   │
│  │  [Cheat] AIProduce · AIStopProduction                │   │
│  └─────────────────────────────────────────────────────┘   │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              共享状态查询 (宏观)                     │   │
│  │  GetHouseInfo · GetForceDistribution · GetMapInfo   │   │
│  │  GetTechTree · GetTypeInfo · GetProductionState     │   │
│  └─────────────────────────────────────────────────────┘   │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              对象状态查询 (微观)                     │   │
│  │  GetTechnoInRange · GetTechnoInfo                   │   │
│  │  GetObjectsOnCell · GetAerialObjects                │   │
│  │  GetCellInfo · FindPath                             │   │
│  └─────────────────────────────────────────────────────┘   │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              事件回调                                │   │
│  │  OnSessionClosed · OnBuildingReady                  │   │
│  │  OnUnitDestroyed · OnUnderAttack                    │   │
│  └─────────────────────────────────────────────────────┘   │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              辅助功能                                │   │
│  │  MarkObject/UnmarkObject · 单元格操作               │   │
│  │  StatusRouter · CellAddrToCoords/CoordsToAddr       │   │
│  └─────────────────────────────────────────────────────┘   │
└──────────────────────┬──────────────────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────────────────┐
│                    游戏引擎 (YR Engine)                       │
│  EventClass 系统 · Mission 系统 · 寻路 · 武器 · 碰撞        │
└─────────────────────────────────────────────────────────────┘
```

---

## 模块说明

### 1. 会话管理

前端与后端的连接生命周期管理。每个会话接管一个 HouseClass。

| 接口 | 功能 |
|------|------|
| `BeginSession` | 建立会话，接管指定 House。若 House 已被接管返回失败 |
| `EndSession` | 结束会话，释放 House |
| `SessionHeartbeat` | 心跳保活 |
| `GetToolList` | 返回当前会话可用的操作和查询工具列表 |

后端在 `FrameUpdate` 中检测超时会话（默认 30s），超时自动 `EndSession` 并触发 `OnSessionClosed` 回调。

### 2. 操作层

所有操作均通过拼装 `EventClass` 并调用 `EventClass::AddEvent()` 下发，**不对接 SetDestination/QueueMission 等引擎底层调用**。

| 类别 | 接口 | 对应 EventType |
|------|------|---------------|
| 单位操作 | `IssueOrder` | MEGAMISSION(4) / MEGAMISSION_F(5) |
| 单位操作 | `Deploy` | DEPLOY(9) |
| 单位操作 | `Detonate` | DETONATE(10) |
| 单位操作 | `Scatter` | SCATTER(7) |
| 生产 | `Produce` | PRODUCE(14) |
| 生产 | `Suspend` | SUSPEND(15) |
| 生产 | `Abandon` | ABANDON(16) |
| 生产 | `AbandonAll` | ABANDON_ALL(46) |
| 生产 | `SetPrimary` | PRIMARY(17) |
| 建造 | `Place` | PLACE(11) |
| 建造 | `SpecialPlace` | SPECIAL_PLACE(18) |
| 维修/出售 | `Repair` | REPAIR(21) |
| 维修/出售 | `Sell` | SELL(22) |
| 维修/出售 | `SellCell` | SELLCELL(23) |
| 其他 | `Ally` | ALLY(3) |
| 其他 | `Cheer` | ALLCHEER(45) |

作弊用具（仅 `Cheat=true` 会话可见）：
| 接口 | 功能 |
|------|------|
| `AIProduce` | 绕过 EventClass 直接生产 |
| `AIStopProduction` | 直接清除生产索引 |

### 3. 共享状态查询（宏观）

提供战场宏观态势，供前端做战略决策。所有查询携带 `-SessionID`，按会话的可见性规则过滤（详见 [API-SPEC.md](API-SPEC.md) 可见性规则一节）。

| 接口 | 功能 |
|------|------|
| `GetHouseInfo` | House 资金、单位统计、部队聚集检测 |
| `GetForceDistribution` | 兵力按类型 × 空间区域分布 |
| `GetMapInfo` | 地图宽高和边界 |
| `GetTechTree` | House 可建造的全部 Type |
| `GetTypeInfo` | Type 静态属性 |
| `GetProductionState` | 当前工厂状态（队列/进度/暂停） |

### 4. 对象状态查询（微观）

提供战场微观信息，供前端做战术决策。同样遵守可见性规则。

| 接口 | 功能 |
|------|------|
| `GetTechnoInRange` | 范围内的单位结构化列表 |
| `GetTechnoInfo` | 单个单位详情（含载员/驻军/建造状态） |
| `GetObjectsOnCell` | 指定格子上的对象列表 |
| `GetAerialObjects` | 范围内的空中单位 |
| `GetCellInfo` | 单元格地形属性 |
| `FindPath` | A* 寻路 |

### 5. 事件回调

后端主动向会话关联的前端库推送事件（`PostAsyncRemoteCall`），替代旧版 WatchPointer/WatchBuildingReady 的前端主动注册模式。

| 回调 | 触发条件 |
|------|----------|
| `OnSessionClosed` | 会话超时或被 EndSession 关闭 |
| `OnBuildingReady` | 接管 House 的建筑完成建造 |
| `OnUnitDestroyed` | 接管 House 的单位被摧毁 |
| `OnUnderAttack` | 接管 House 的单位受到攻击 |

### 6. 辅助功能

| 类别 | 接口 |
|------|------|
| 标记系统 | `MarkObject` / `UnmarkObject` / `ListMarks` / `ClearMarks`（调试用） |
| 迷雾 | `GetCellShroud` / `UnshroudCell` / `AdjustCellShroud` |
| 泰伯利亚 | `GetCellTiberium` / `AddCellTiberium` / `RemoveCellTiberium` / `SpreadCellTiberium` |
| 辐射 | `GetCellRadiation` / `IncreaseCellRadiation` / `DecreaseCellRadiation` |
| 坐标转换 | `CellAddrToCoords` / `CellCoordsToAddr` |
| 帧率 | `GetFrameRateInfo` |
| 变量路由 | `StatusRouter`（`<Status.Frame>` / `<Status.House.N.Money>` 等） |

### 7. 基础架构

- **dllmain.cpp** — DLL 入口，ECInitLibrary 注册所有 Action/Condition 回调
- **Pointers.h / Pointers.cpp** — 指针过期回调处理（`PointerExpiredProcess`，自动清理已销毁对象引用）
- **AgentBackend.h** — 内部函数声明
- **[API-SPEC.md](API-SPEC.md)** — 前端接口规范（权威参考）

---

## 外部接口（对前端暴露）

前端 (IHAgentFrontend) 通过以下方式与本项目交互：

| 交互方式 | 说明 |
|---------|------|
| **EC SDK Action/Condition** | 在 ECInitLibrary 中注册的命名函数，前端通过名称 + SessionID 调用 |
| **回调机制** | OnSessionClosed / OnBuildingReady / OnUnitDestroyed / OnUnderAttack 通过 `PostAsyncRemoteCall` 推送 |
| **StatusRouter** | `<Status.Frame>`、`<Status.House.N.Money>` 等变量路由 |
| **IHCore::Reset / FrameUpdate** | 由 IHCore 框架广播，本项目用于重置会话状态和超时检测 |

---

## 关键原则

1. **职责纯粹**：本项目只做三件事——会话管理、读游戏状态（含可见性过滤）、写 EventClass 指令，不做任何决策
2. **操作 = EventClass**：所有写入操作的语义对应玩家可用输入，不作 SetDestination/QueueMission 等底层暴露
3. **前端不碰指针**：`IsObjectAlive` 等指针校验由后端内部完成；对象地址由 PointerExpiredProcess 自动清理，前端零负担
4. **可见性由后端控制**：Cheat 标志 + House 身份决定查询返回的信息范围，前端无需自行判断战争迷雾
5. **会话超时自动释放**：FrameUpdate 中检测心跳超时，自动 EndSession 并回调 OnSessionClosed

---

## 开发约定

- 使用 C++17/20 标准
- 所有 Action 参数通过 `JsonObject` 传入，输出通过 `ECDebug::ReturnString` / `std::cout` 返回
- 操作层通过 `EventClass::AddEvent()` 下发，禁止直接 `pTechno->SetDestination()` / `pTechno->QueueMission()` 等调用
- 会话相关逻辑集中在独立模块（Agent.Session.cpp 等），与查询/操作模块解耦
- 帧更新相关轮询（超时检测、BuildingReady 检测）集中在 `FrameUpdate` 回调
- 所有新接口必须携带 `-SessionID`（除 `GetToolList` 无会话查询模式）

## 相关文件

| 文件 | 用途 |
|------|------|
| [API-SPEC.md](API-SPEC.md) | **前端接口规范 v2**（权威参考，定义全部接口签名、参数、EventType 映射） |
| AgentBackend.h | 内部函数声明 |
| dllmain.cpp | DLL 入口 + ECInitLibrary 注册 |
| Agent.Session.cpp | 会话管理实现 |
| Agent.Core.cpp | 共享状态查询 + StatusRouter |
| Agent.Coord.cpp | 对象状态查询 + 生产/建造/地图相关查询 + 单元格操作 |
| Agent.Actions.cpp | 操作层（EventClass 拼装与下发） |
| Agent.Marker.cpp | 标记系统 + 回调机制 |
| Pointers.h / Pointers.cpp | 指针过期回调 |
