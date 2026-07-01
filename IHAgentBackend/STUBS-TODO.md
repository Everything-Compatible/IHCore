# IHAgentBackend v2 — 待补 Stub 清单

> 本文档汇总 v2 迁移过程中因依赖外部资源（IDA 逆向 / IHCore 协作 / 引擎能力缺口）而暂时 stub 的功能项。
> 每项含：当前行为、未来实现路径、依赖、优先级。
> Phase A–D 已完成，本文档为 Phase E 产出。

---

## 1. OnUnderAttack 回调

**当前行为**：dispatch 入口已实现 —— `DispatchUnderAttack(pS, victimAddr, attackerAddr, damage)` 已就绪（[Agent.Session.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Session.cpp)），payload `{SessionID, House, VictimAddress, AttackerAddress, Damage}` 完整构造并调用 `DispatchCallback`。**无触发点** —— `TechnoClass::ReceiveDamage` 上的 DEFINE_HOOK 未安装，依赖 IDA 逆向定位函数地址后从 hook 中调用本 dispatch。

**未来实现路径**：
- 在 `TechnoClass::ReceiveDamage` 函数入口安装 `DEFINE_HOOK` 宏（[Syringe.h:298](file:///f:/RA2%20Engine%20Extension/IHCore/IHCore/YRPP/Syringe.h)）
- Hook 函数读取 `this->GetOwningHouse()->ArrayIndex`，调用 `FindSessionByHouse(idx)` 查找会话
- 若会话存在，构造 payload `{SessionID, House, VictimAddress, AttackerAddress, Damage}` 并 `DispatchCallback(pS, "OnUnderAttack", payload)`
- `AttackerAddress` 来自 ReceiveDamage 的参数（攻击方 TechnoClass*）
- `Damage` 来自 ReceiveDamage 的 damage 参数

**依赖**：IDA 逆向定位 `TechnoClass::ReceiveDamage` 的函数地址和参数布局。YRPP 头文件可能未导出该函数（需确认 [TechnoClass.h](file:///f:/RA2%20Engine%20Extension/IHCore/IHCore/YRPP/TechnoClass.h) 是否有 `ReceiveDamage` 声明，若无则需 IDA 查找）。

**优先级**：**高**。战斗感知是 Agent 战术决策的关键输入；无此回调 Agent 无法知道己方单位被攻击。

---

## 2. OnChatMessage 回调

**当前行为**：dispatch 入口已实现 —— `DispatchChatMessage(pS, fromHouse, toHouse, message, isAlly)` 已就绪（[Agent.Session.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Session.cpp)）。**无触发点** —— 聊天接收函数上的 DEFINE_HOOK 未安装，依赖 IDA 逆向定位函数地址。

**未来实现路径**：
- 在 YR 引擎的聊天接收函数（疑似 `HouseClass::ReceiveChat` 或类似）安装 hook
- Hook 函数读取 `FromHouse` / `ToHouse` / `Message` 参数
- 调用 `FindSessionByHouse(toHouse)` 查找接收方会话
- 若会话存在，构造 payload `{SessionID, FromHouse, ToHouse, Message, IsAlly}` 并派发

**依赖**：IDA 逆向定位聊天接收函数地址。YR 引擎的多人聊天系统在 [NetworkClass](file:///f:/RA2%20Engine%20Extension/IHCore/IHCore/YRPP) 相关头文件中（需探索确认具体类/方法）。

**优先级**：**低**。聊天主要发生在多人对战场景；当前 Agent 应用以单人战役/遭遇战为主，聊天不是关键路径。可推迟到多人模式支持阶段。

---

## 3. OnBeacon 回调

**当前行为**：dispatch 入口已实现 —— `DispatchBeacon(pS, action, cellX, cellY)` 已就绪（[Agent.Session.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Session.cpp)）。**无触发点** —— 信标 place/remove 函数上的 DEFINE_HOOK 未安装，依赖 IDA 逆向定位函数地址。

**未来实现路径**：
- 在 YR 引擎的信标 place/remove 函数安装 hook（信标是多人协作中的"看这里"标记）
- Hook 函数读取 `House` / `Action` ("place"|"remove") / `Cell` 参数
- 调用 `FindSessionByHouse(house)` 查找会话
- 若会话存在，构造 payload `{SessionID, House, Action, Cell:[X,Y]}` 并派发

**依赖**：IDA 逆向定位信标 place/remove 函数地址。YRPP 中可能有 `BeaconClass` 或类似定义（需探索 [BeaconClass.h](file:///f:/RA2%20Engine%20Extension/IHCore/IHCore/YRPP) 是否存在）。

**优先级**：**低**。与 OnChatMessage 同属多人协作功能，单人场景不出现信标。可推迟到多人模式支持阶段。

---

## 4. FindPath 实现

**当前行为**：[Agent.Coord.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Coord.cpp) 中 `IHAgentBackend_FindPath` 返回 `{Success:false, Reason:"NotImplemented"}` 并 `ECDebug::ReturnStdError(ERROR_NOT_SUPPORTED)`。

**未来实现路径**（两选一）：

**选项 A — 基于 CellClass 的自实现 A\***：
- 从 `MapClass::Instance->Cells` 取起始格 `CellClass*`
- 用开放/关闭表（`std::unordered_set<CellClass*>` + `std::priority_queue`）跑 A*
- 邻居通过 `CellClass->AdjacentCells`（确认 YRPP 是否暴露）或 8 方向 `MapCoords + (dx,dy)` 计算
- 通行性检查：`CellClass::IsShrouded()` 跳过 / `LandType` 检查（水域/陆地）/ `OccupationFlags` 检查建筑物阻挡
- 应用 `-TechnoTypeID` 的 `SpeedType` 和 `MovementZone` 过滤（如步兵可涉水、车辆不可）
- 返回路径点列表 `[{X,Y}, ...]`

**选项 B — 临时 UnitClass 实例调用 `AStarAttempt`**：
- 用 `UnitClass::AStarAttempt`（[UnitClass.h](file:///f:/RA2%20Engine%20Extension/IHCore/IHCore/YRPP/UnitClass.h)）— 但这是实例方法，需要 `UnitClass*`
- 创建临时 UnitClass 实例（or 借用现有单位）→ 设 `Type` 为对应 TechnoType → 调 `AStarAttempt`
- 风险：临时实例化 UnitClass 可能影响引擎内部状态、触发其他系统副作用

**依赖**：选项 A 需 YRPP 暴露 `CellClass::AdjacentCells` 或类似邻居访问；选项 B 需确认 `UnitClass::AStarAttempt` 的可调用性（实例化副作用）。

**优先级**：**中**。Agent 可在无寻路时通过 `IssueOrder -Mission Move -Target <cell>` 让引擎自行寻路（MEGAMISSION 事件由引擎处理路径），所以 FindPath 是冗余能力。仅当 Agent 需要预演路径（如评估可达性、对比多目标距离）时才必要。可延后。

---

## 5. OnUnitDestroyed 的 KillerAddress

**当前行为**：[Agent.Session.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Session.cpp) 中 `DispatchUnitDestroyed` 函数将 `KillerAddress` 固定为字符串 `"0x00000000"`。

**未来实现路径**：
- 在 `TechnoClass::ReceiveDamage` 或 `WeaponClass::Fire` 安装 hook（与 OnUnderAttack 共用入口）
- 维护全局 `std::unordered_map<DWORD victimAddr, DWORD attackerAddr> g_LastAttacker`（mutex 保护）
- 每次 ReceiveDamage 调用时更新 `g_LastAttacker[victim] = attacker`
- `PointerExpiredProcess` 触发 OnUnitDestroyed 时查 `g_LastAttacker[addr]` 作为 KillerAddress，erase 该条目
- 需定期清理过期条目（如每 N 帧扫一遍，删除已不在 `AbstractClass::Array` 的 victim）

**依赖**：与 #1 OnUnderAttack 共用 IDA 逆向成果。优先实现 #1 后顺带实现此项。

**优先级**：**中**。KillerAddress 是"战斗结果归因"的有用信息，但不阻塞核心战斗循环。可在 #1 完成后追加。

---

## 6. 接管 House 后跳过原版 AI

**当前行为**：IHAgentBackend 侧已实现 —— `extern "C" __declspec(dllexport) bool IsHouseTaken(int houseIdx)` 已导出（[Agent.Session.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Session.cpp)），包装 `FindSessionByHouse(houseIdx) != nullptr`。CLAUDE.md 第 33 行的约定（"原版 AI 跳过 Hook 的具体实现由 IHCore 层负责"）依然有效 —— **IHCore 侧的 House AI 跳过 hook 仍未实现**，需在 `HouseClass::AI` 或类似入口安装 DEFINE_HOOK，调用 `IsHouseTaken` 判断是否跳过。

**未来实现路径**：
- 在 IHCore 项目（`f:\RA2 Engine Extension\IHCore\IHCore\`）添加 hook
- Hook 点：YR 引擎的 House AI 决策入口（疑似 `HouseClass::AI` 或 `HouseClass::ComputerMiss`，需 IDA 确认）
- Hook 逻辑：
  ```cpp
  DEFINE_HOOK(<House AI entry addr>, IHCore_HouseAI_SkipIfTaken, ...)
  {
      int houseIdx = this->ArrayIndex;
      // 调用 IHAgentBackend 暴露的查询函数（需新增导出）
      if (IHAgentBackend::IsHouseTaken(houseIdx)) {
          return 0x<skip_addr>;  // 跳过原版 AI 处理
      }
      return 0x<original_next_addr>;
  }
  ```
- 需在 IHAgentBackend 新增并导出 `extern "C" __declspec(dllexport) bool IsHouseTaken(int houseIdx)` 函数，包装 `FindSessionByHouse(houseIdx) != nullptr`

**依赖**：IHCore 项目协作（不在 IHAgentBackend 控制范围内）；IDA 逆向定位 House AI 入口。

**优先级**：**高**。无此 hook，被接管的 House 会同时被 Agent 和原版 AI 控制，导致行为冲突（AI 可能取消 Agent 的生产队列、移动 Agent 已部署的单位）。建议在 v2 上线前必须由 IHCore 实现此 hook。

---

## 优先级汇总

| 优先级 | 项 | 阻塞性 |
|--------|-----|--------|
| **高** | #1 OnUnderAttack | dispatch 就绪，缺 ReceiveDamage hook 触发 |
| **高** | #6 接管 House 跳过 AI | IHAgentBackend 导出就绪，缺 IHCore 侧 hook |
| **中** | #4 FindPath | 仅影响路径预演能力，不阻塞操作 |
| **中** | #5 KillerAddress | 战斗归因辅助信息，可延后 |
| **低** | #2 OnChatMessage | 多人场景功能，单人无需 |
| **低** | #3 OnBeacon | 多人场景功能，单人无需 |

## IDA 逆向工作清单（汇总）

以下函数地址需通过 IDA Pro 逆向 YR 引擎二进制（`gamemd.mp.dll` 或 `gamemd.exe`）定位：

1. `TechnoClass::ReceiveDamage` — 同时支撑 #1 OnUnderAttack 和 #5 KillerAddress
2. 聊天接收函数（疑似 NetworkClass 或 HouseClass 方法）— 支撑 #2
3. 信标 place/remove 函数 — 支撑 #3
4. `HouseClass::AI` 或类似 AI 决策入口 — 支撑 #6

建议优先做 #1+#5（共用一次逆向工作）和 #6（关键阻塞项）。#2 和 #3 可纳入多人模式支持批次。

## 与 IHCore 项目协作的事项

- **#6 接管 House 跳过 AI** 需 IHCore 添加 hook 调用本项目 `IsHouseTaken` 导出函数（已导出，IHCore 侧 hook 仍待实现）
- ~~本项目需新增 `IsHouseTaken(int houseIdx)` 导出~~ ✅ 已完成（[Agent.Session.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Session.cpp) + [AgentBackend.h](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/AgentBackend.h)）
