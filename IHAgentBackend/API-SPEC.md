# IHAgentBackend — 前端接口规范 v2

> 本文档定义 IHAgentBackend 对前端暴露的全部接口。v2 基于会话管理重构，操作层全面对接 EventClass。

---

## 目录

1. [会话管理](#一会话管理--session-management)
2. [操作](#二操作--operations)
   - [2.1 单位操作](#21-单位操作)
   - [2.2 生产](#22-生产)
   - [2.3 建造](#23-建造)
   - [2.4 维修与出售](#24-维修与出售)
   - [2.5 其他](#25-其他)
   - [2.6 作弊用具](#26-作弊用具)
3. [共享状态查询（宏观）](#三共享状态查询宏观--shared-state-query)
4. [对象状态查询（微观）](#四对象状态查询微观--object-state-query)
5. [回调](#五回调--callbacks)
6. [可见性规则](#六可见性规则)
7. [与旧版对照](#七与旧版对照)

---

## 一、会话管理 —— Session Management

每个前端进程连接后端需先建立会话。每个会话接管**一个** HouseClass。接管期间该 House 的原版 AI 行为全部跳过（Hook 层实现）。会话断开或超时自动取消接管。

### BeginSession

```
BeginSession  -House <int>  [-Cheat true|false]  [-Timeout <int>]
```

| 参数 | 类型 | 默认 | 说明 |
|------|------|------|------|
| `-House` | int | 必填 | 要接管的作战方索引 |
| `-Cheat` | bool | false | 是否作弊模式。影响查询接口的可见性过滤和可用工具列表 |
| `-Timeout` | int | 30 | 心跳超时秒数 |

**成功返回**：

```json
{
  "Success": true,
  "SessionID": "550e8400-e29b-41d4-a716-446655440000",
  "House": 2,
  "Cheat": false,
  "Timeout": 30,
  "ExpireAt": 123456
}
```

**失败返回**（House 已被其他会话接管）：

```json
{
  "Success": false,
  "Reason": "HouseAlreadyTaken",
  "TakenBySession": "660e8400-e29b-41d4-a716-446655440001"
}
```

- 前端应先 `EndSession` 释放旧会话，再重新 `BeginSession`

### EndSession

```
EndSession  -SessionID <str>
```

- 结束会话，取消接管，恢复该 House 的原版 AI 行为
- 后端触发 `OnSessionClosed` 回调通知前端（reason: "released"）

### SessionHeartbeat

```
SessionHeartbeat  -SessionID <str>
```

- 刷新会话 `LastHeartbeat`，防止超时
- 后端在每帧 `FrameUpdate` 中检测超时会话并自动调用 `EndSession`

### GetToolList

```
GetToolList  [-SessionID <str>]
```

- 不带 `-SessionID`：返回全部前端可用工具名列表
- 带 `-SessionID`：根据会话的 `Cheat` 标志返回实际可用工具

返回：

```json
{
  "SessionID": "uuid",
  "Cheat": false,
  "Tools": [
    "IssueOrder", "Deploy", "Detonate", "Scatter",
    "Produce", "Suspend", "Abandon", "AbandonAll", "SetPrimary",
    "Place", "SpecialPlace",
    "Repair", "Sell", "SellCell",
    "Ally", "Cheer"
  ],
  "CheatTools": ["AIProduce", "AIStopProduction"],
  "StateQueries": [
    "GetHouseInfo", "GetForceDistribution", "GetMapInfo",
    "GetTechTree", "GetTypeInfo", "GetProductionState",
    "GetCurrentTime", "GetPlayerViewRoute",
    "GetTechnoInRange", "GetTechnoInfo", "GetObjectsOnCell",
    "GetAerialObjects", "GetCellInfo", "FindPath"
  ]
}
```

---

## 二、操作 —— Operations

所有操作均通过拼装对应 `EventType` 的 `EventClass` 并调用 `EventClass::AddEvent()` 实现。不做 `SetDestination` / `QueueMission` 等直接引擎调用。

### 2.1 单位操作

对应 `MEGAMISSION` / `MEGAMISSION_F` / `DEPLOY` / `DETONATE` / `SCATTER`。

#### IssueOrder

> 对应 EventType: **MEGAMISSION(4)** / **MEGAMISSION_F(5)**

```
IssueOrder  -SessionID <str>
            -Mission <Move|Attack|AttackMove|Guard|AreaGuard|Stop|
                      Hunt|Retreat|Harvest|Enter|Capture|Patrol|Unload|
                      QMove|Sticky|Ambush|Sabotage|Construction>
            [-Units [<0x...>, <0x...>]]
            [-Target <0x...>]
```

| 参数 | 说明 |
|------|------|
| `-Mission` | 移动/攻击/巡逻等 Mission 枚举值。后端使用 `Mission` 枚举（共 31 种），但前端可用子集为上表所列 |
| `-Units` | 执行操作的单个或多个单位地址。不提供则为当前选中的全部单位 |
| `-Target` | 目标对象或单元格地址。Move/Patrol/Retreat 等传入 Cell 地址，Attack/Enter/Capture 等传入 Techno 地址；Stop 等无需 Target |

#### Deploy

> 对应 EventType: **DEPLOY(9)**

```
Deploy  -SessionID <str>  -Address <0x...>
```

- 部署/收起单位（基地车展开、直升机着陆等）

#### Detonate

> 对应 EventType: **DETONATE(10)**

```
Detonate  -SessionID <str>  -Address <0x...>
```

- 引爆炸弹（疯狂伊文附着炸弹）

#### Scatter

> 对应 EventType: **SCATTER(7)**

```
Scatter  -SessionID <str>  -Address <0x...>
```

- 散开命令（X 键）

---

### 2.2 生产

| 接口 | EventType | 参数 |
|------|-----------|------|
| `Produce` | **PRODUCE(14)** | `-SessionID` `-TypeID <str>` |
| `Suspend` | **SUSPEND(15)** | `-SessionID` `-TypeID <str>` |
| `Abandon` | **ABANDON(16)** | `-SessionID` `-TypeID <str>` |
| `AbandonAll` | **ABANDON_ALL(46)** | `-SessionID` |
| `SetPrimary` | **PRIMARY(17)** | `-SessionID` `-TypeID <str>` |

- `Produce`：点击生产队列侧边栏图标（PRODUCE 事件）
- `Suspend`：暂停指定类型的生产
- `Abandon`：取消指定类型的生产队列项
- `AbandonAll`：取消全部生产队列
- `SetPrimary`：将指定类型的工厂设为主要工厂

---

### 2.3 建造

#### Place

> 对应 EventType: **PLACE(11)**

```
Place  -SessionID <str>  -TypeID <str>  -X <int>  -Y <int>
```

- 放置建筑到指定位置

#### SpecialPlace

> 对应 EventType: **SPECIAL_PLACE(18)**

```
SpecialPlace  -SessionID <str>  -TypeID <str>  -X <int>  -Y <int>
```

- 放置超级武器目标位置（闪电风暴、核弹、力场护盾等）

---

### 2.4 维修与出售

| 接口 | EventType | 参数 |
|------|-----------|------|
| `Repair` | **REPAIR(21)** | `-SessionID` `-Address <0x...>` |
| `Sell` | **SELL(22)** | `-SessionID` `-Address <0x...>` |
| `SellCell` | **SELLCELL(23)** | `-SessionID` `-X <int>` `-Y <int>` |

- `Repair`：切换建筑维修状态（扳手开关）
- `Sell`：出售建筑（对围墙无效）
- `SellCell`：出售围墙（按单元格坐标）

---

### 2.5 其他

| 接口 | EventType | 参数 |
|------|-----------|------|
| `Ally` | **ALLY(3)** | `-SessionID` `-TargetHouse <int>` |
| `Cheer` | **ALLCHEER(45)** | `-SessionID` |

---

### 2.6 作弊用具（仅 `Cheat=true` 会话可用）

| 接口 | 功能 | 参数 |
|------|------|------|
| `AIProduce` | 绕过 EventClass 走 AI 生产路径 | `-SessionID` `-TypeID <str>` |
| `AIStopProduction` | 直接清除生产索引 | `-SessionID` [`-WhatAmI <str>` | `-All`] |

- 普通会话 `GetToolList` 的 `Tools` 数组不包含这两个

---

## 三、共享状态查询（宏观）—— Shared State Query

所有查询使用 `-SessionID`，后端根据会话的 `Cheat` 标志和接管 House 身份决定返回信息的可见范围。详见[可见性规则](#六可见性规则)。

### GetHouseInfo

```
GetHouseInfo  -SessionID <str>  [-House <int>]
```

- `-House` 省略时默认返回会话接管 House 的信息
- 返回：资金、电力、各单位数量统计、阵营、科技等级、部队密集群检测等

### GetForceDistribution

```
GetForceDistribution  -SessionID <str>  [-House <int>]
```

- 接管 House 的兵力分布概览：按类型（Infantry/Unit/Building/Aircraft）× 按空间区域统计

### GetMapInfo

```
GetMapInfo  -SessionID <str>
```

- 返回：地图宽高、可用区域边界坐标

### GetTechTree

```
GetTechTree  -SessionID <str>  [-House <int>]
```

- 接管 House 当前可建造的所有 Type

### GetTypeInfo

```
GetTypeInfo  -SessionID <str>  -TypeID <str>
```

- 返回给定 TypeID 的静态属性（生命、护甲、速度、武器、费用等）

### GetProductionState

```
GetProductionState  -SessionID <str>
```

- 接管 House 的当前工厂状态：各工厂地址、生产队列、进度、暂停标志

### GetCurrentTime

```
GetCurrentTime  -SessionID <str>
```

- 返回当前游戏已经经过的时间（游戏内秒数/帧数）

### GetPlayerViewRoute

```
GetPlayerViewRoute  -SessionID <str>  -FromTime <double>  -ToTime <double>
```

- 返回指定时间范围内玩家屏幕中心位置的采样轨迹，每 0.5 秒一个坐标点
- 返回：`[{Time, Cell: [X,Y]}, ...]`

---

## 四、对象状态查询（微观）—— Object State Query

与宏观查询同理，遵守可见性规则。

### GetTechnoInRange

```
GetTechnoInRange  -SessionID <str>  -X <int>  -Y <int>  -Range <double>
                  [-WhatAmI <Unit|Infantry|Building|Aircraft>]
                  [-House <int>]  [-Type <str>]
                  [-Limit <int>]  [-Offset <int>]
```

- 返回指定范围内符合筛选条件的单位信息列表（地址/类型/HP/坐标/Mission/所属/选中态）
- 按可见性规则自动过滤

### GetTechnoInfo

```
GetTechnoInfo  -SessionID <str>  -Address <0x...>
```

- 单个单位详细信息（载员、驻军、建造状态、移动状态、Transporter 等）
- 目标不可见时对应字段返回受限信息

### GetObjectsOnCell

```
GetObjectsOnCell  -SessionID <str>  -X <int>  -Y <int>
```

- 指定格子上所有对象的地址列表，按可见性过滤

### GetAerialObjects

```
GetAerialObjects  -SessionID <str>  -X <int>  -Y <int>  -Range <double>
```

- 返回范围内空中单位的地址和类型

### GetCellInfo

```
GetCellInfo  -SessionID <str>  -X <int>  -Y <int>
```

- 返回指定单元格的地形属性：`LandType`、`OccupationFlags`、`ContainsBridge`、`SlopeIndex` 等

### FindPath

```
FindPath  -SessionID <str>  -FromX <int>  -FromY <int>  -ToX <int>  -ToY <int>
          [-TechnoTypeID <str>]
```

- A* 寻路，返回路径点列表 `[{X,Y}, ...]`
- `-TechnoTypeID` 可选，指定移动单位类型以考虑通行限制（两栖/飞行/步兵等）

---

## 五、回调 —— Callbacks

后端主动推送，替代旧版 `WatchPointer` / `WatchBuildingReady` 的前端注册机制。通过 `PostAsyncRemoteCall` 发送到会话关联的前端库。

| 回调 | 触发条件 | 载荷 |
|------|----------|------|
| `OnSessionClosed` | 会话超时或被 `EndSession` 关闭 | `{SessionID, House, Reason: "timeout" \| "released"}` |
| `OnBuildingReady` | 接管 House 的建筑完成建造（`ActuallyPlacedOnMap && HasBuildUp && !InLimbo`） | `{SessionID, House, TypeID, Address, Cell: [X,Y]}` |
| `OnUnitDestroyed` | 接管 House 的任意单位指针过期 | `{SessionID, House, TypeID, Address, KillerAddress}` |
| `OnUnderAttack` | 接管 House 的单位受到攻击 | `{SessionID, House, VictimAddress, AttackerAddress, Damage}` |
| `OnChatMessage` | 收到玩家的聊天消息 | `{SessionID, FromHouse, ToHouse, Message, IsAlly}` |
| `OnBeacon` | 玩家布置或删除信标 | `{SessionID, House, Action: "place"\|"remove", Cell: [X,Y]}` |

---

## 六、可见性规则

| 条件 | 返回内容 |
|------|----------|
| `Cheat=true` | 全部对象信息，包括隐形、Shroud 中、Limbo 中的对象 |
| `Cheat=false` + 接管 House 为玩家/玩家同盟 | 排除敌方 Shroud 中对象 |
| `Cheat=false` + 接管 House 为非同盟 AI | 排除 Shroud 中及敌方隐形对象 |

- `GetTechnoInfo` 对不可见目标仍可返回部分已知信息（如最后已知位置）
- 所有对象地址均由后端通过 `PointerExpired` 机制自动清理，前端不参与指针生命周期管理

---

## 七、与旧版对照

| 旧接口 | 新方案 | 变化 |
|--------|--------|------|
| 无会话管理 | `BeginSession` / `EndSession` / `SessionHeartbeat` / `GetToolList` | 新增 |
| `IssueOrder` | `IssueOrder` | 改为 MEGAMISSION EventClass 拼装；不再直接 SetDestination+QueueMission |
| `QueueMission` | — | 删除，内化到 IssueOrder |
| `SetDestination` | — | 删除，内化到 IssueOrder |
| `SelectObject` | — | 删除，由 IssueOrder 的 `-Units` / House 选中态自动处理 |
| `DeployObject` | `Deploy` | 统一为 EventClass 操作 |
| `StartRepairBuilding` | `Repair` | 统一为 EventClass 操作 |
| `FactoryProduce` | `Produce` | 重命名，语义一致 |
| `FactoryPlace` | `Place` | 重命名，语义一致 |
| `FactorySuspend` | `Suspend` | 重命名，语义一致 |
| `FactoryAbandon` | `Abandon` | 重命名，语义一致 |
| `FactoryCompleteProduction` | — | 删除，无对应玩家操作 |
| `FindFactory` | — | 删除，前端不操心工厂地址 |
| `ListAllFactories` | — | 删除，内化到 `GetProductionState` |
| `AIProduce` | `AIProduce` | Cheat=true 专属 |
| `AIStopProduction` | `AIStopProduction` | Cheat=true 专属 |
| `FindObjects` / `FindTechnoInfo` | `GetTechnoInRange` | 合并，范围查询 + 可见性过滤 |
| `FindObjectsOnCell` | `GetObjectsOnCell` | 重命名 |
| `FindAerialObjects` | `GetAerialObjects` | 重命名 |
| `FindObjectsInRange` | `GetTechnoInRange` | 合并 |
| `GetHouseInfo` | `GetHouseInfo` | 保留，增加可见性控制 |
| — | `GetForceDistribution` | 新增 |
| — | `FindPath` | 新增 |
| `IsObjectAlive` | — | 删除，后端内部校验 + PointerExpired 自动清理 |
| `CanEnterCell` | — | 删除，后端内部校验 |
| `CheckBuildability` | — | 删除，后端内部校验 |
| `CanPlaceBuilding` | — | 删除，后端内部校验 |
| `WatchPointer` | — | 删除，改为会话级回调 |
| `WatchBuildingReady` | `OnBuildingReady` | 会话级回调 |
| `GetMovementState` | 内化到 `GetTechnoInfo` | 不再单独暴露 |
| `GetBuildingStatus` | 内化到 `GetTechnoInfo` | 不再单独暴露 |
| `MarkObject` 系列 | 保留不变 | 调试工具，独立于新接口 |
| Shroud/Tiberium/Radiation 系列 | 保留不变 | 环境操作，独立于新接口 |
| `GetMapInfo` / `GetCellInfo` / `GetTypeInfo` / `GetTechTree` | 保留 | 增加 SessionID 和可见性 |
| `GetHouseTechTree` | `GetTechTree` | 简并命名 |
| `GetHouseProduction` | `GetProductionState` | 扩展为工厂详情 |
