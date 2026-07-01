# IHAgentBackend v2 迁移 — Phase D + E 执行计划

## Context

这是已批准的 [v2 迁移计划](file:///f:/RA2%20Engine%20Extension/IHCore/.trae/documents/IHAgentBackend-v2-migration-plan.md) 的收尾阶段。Phase A/B/C 已完成：

- **Phase A** ([Agent.Session.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Session.cpp)) — AgentSession 结构、UUID 生成、注册表、可见性 helper、DispatchCallback、EndSessionInternal（已派发 OnSessionClosed）、Session_FrameUpdate（超时检测 + 视口采样）、Session_OnGameReset、4 个 Action 处理器（BeginSession/EndSession/SessionHeartbeat/GetToolList）
- **Phase B** ([Agent.Actions.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Actions.cpp)) — 18 个操作 Action 全部基于 EventClass::AddEvent
- **Phase C** ([Agent.Coord.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Coord.cpp)) — 14 个查询 Action，全部 RequireSession 门控 + 可见性过滤

剩余工作：Phase D（[Agent.Marker.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Marker.cpp) 回调层重构）+ Phase E（编写 STUBS-TODO.md 汇总 stub 项）。

## 当前状态分析（Phase 1 探索结果）

### Agent.Marker.cpp 现状（v1）

文件包含以下内容：
- `_Marks` 存储 + `MarkObject`/`UnmarkObject`/`IsObjectMarked` helper（**保留**）
- `IHAgentBackend_MarkObject`/`UnmarkObject`/`ListMarks`/`ClearMarks` 4 个 Action（**保留**）
- `MarkerACP` + `MoveToIHCore`（**保留**，ACP 调试用）
- `WatchEntry` struct + `_Watches` + `_WatchMutex`（**Phase D 删除**）
- `BuildingReadyWatch` struct + `_BuildingWatches` + `_BuildingWatchMutex`（**Phase D 删除**）
- `PointerExpiredProcess`：当前做 UnmarkObject 清理 + 遍历 `_Watches` 派发 PostAsyncRemoteCall（**Phase D 改造**：保留 UnmarkObject，移除 watch 派发，新增 OnUnitDestroyed 派发）
- `IHAgentBackend_WatchPointer`/`WatchBuildingReady` 2 个 Action（**Phase D 删除**）
- `IHAgentBackend_FrameUpdate`：调用 `Session_FrameUpdate()` 后轮询 `_BuildingWatches`（**Phase D 改造**：调用 `Session_FrameUpdate()` 后改为调用 `Session_BuildingReadyPoll()`）

### 引擎 API 已确认可用

- `HouseClass::Buildings` 是 `DynamicVectorClass<BuildingClass*>`（[HouseClass.h:758](file:///f:/RA2%20Engine%20Extension/IHCore/IHCore/YRPP/HouseClass.h#L758)），可用 `for (auto* pBld : pHouse->Buildings)` 迭代
- `BuildingClass::ActuallyPlacedOnMap` ([BuildingClass.h:319](file:///f:/RA2%20Engine%20Extension/IHCore/IHCore/YRPP/BuildingClass.h#L319)) + `HasBuildUp` ([BuildingClass.h:324](file:///f:/RA2%20Engine%20Extension/IHCore/IHCore/YRPP/BuildingClass.h#L324)) + `InLimbo`（继承自 ObjectClass）
- `AbstractClass::GetOwningHouse()` ([AbstractClass.h:117](file:///f:/RA2%20Engine%20Extension/IHCore/IHCore/YRPP/AbstractClass.h#L117)) 返回 `HouseClass*`，`HouseClass::ArrayIndex` 取索引
- `ObjectClass::GetMapCoords()` / `GetCell()` 可获取建筑坐标
- `AbstractType` 枚举值：`Unit=1`、`Aircraft=2`、`Building=6`、`Infantry=15`（[GeneralDefinitions.h:23-37](file:///f:/RA2%20Engine%20Extension/IHCore/IHCore/YRPP/GeneralDefinitions.h#L23)）
- `DispatchCallback(AgentSession*, const char*, cJSON*)` 已在 Phase A 实现，可直接复用

### 已存在的基础设施（Phase A 提供）

- `g_SessionMutex` + `g_SessionsByID` + `g_SessionByHouse` 注册表（私有于 Agent.Session.cpp）
- `FindSessionByID` / `FindSessionByHouse` / `RequireSession` 助手（已暴露在 AgentBackend.h）
- `AgentSession::AnnouncedBuildings` 字段（去重用，已在结构中）
- `EndSessionInternal` 已派发 `OnSessionClosed`（reason="released" 或 "timeout"）— **Phase D 不需要再实现 OnSessionClosed**
- `Session_FrameUpdate` 已在 `IHAgentBackend_FrameUpdate` 开头调用

## 提议改动

### 文件 1：[Agent.Session.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Session.cpp) — 新增 `Session_BuildingReadyPoll()`

**Why**：OnBuildingReady 需要遍历会话注册表 + 修改 `AnnouncedBuildings` 字段，这两个都是 Agent.Session.cpp 私有数据。把轮询逻辑放在 Agent.Session.cpp 内部最自然，避免暴露注册表迭代器。

**What**：在 `Session_FrameUpdate()` 函数之后追加新函数 `Session_BuildingReadyPoll()`：

```cpp
// 在 Session_FrameUpdate 末尾后追加：

// ════════════════════════════════════════════════════════════════════════════
//  Session_BuildingReadyPoll — 每帧调用，遍历所有会话的接管 House 的
//  Buildings 列表，对刚进入 "ready" 状态（ActuallyPlacedOnMap && HasBuildUp
//  && !InLimbo）且未在 AnnouncedBuildings 中去重过的建筑派发 OnBuildingReady
//  回调。
//
//  算法：
//    1. 在 g_SessionMutex 下快照所有会话的 (ID, House, CallbackLib) — 避免
//       持有 mutex 调用 DispatchCallback（PostAsyncRemoteCall 可能阻塞）。
//    2. 对每个会话，遍历 HouseClass::Array->Items[House]->Buildings。
//    3. 对每个建筑检查 ready 三条件；满足且不在 AnnouncedBuildings 中，
//       则构造 payload 并 DispatchCallback。
//    4. 把地址加入 AnnouncedBuildings（再锁一次 mutex 修改）。
//
//  注意：AnnouncedBuildings 永不收缩——会话生命周期内同一建筑只会派发一次。
//  这是有意的：避免 ready 状态短暂闪烁（如被维修导致 HasBuildUp 切换）触发
//  重复回调。
// ════════════════════════════════════════════════════════════════════════════

void Session_BuildingReadyPoll()
{
    if (!_GameStarted.load(std::memory_order_acquire)) return;

    // 快照会话信息（不持锁调用 DispatchCallback）
    struct SessionSnap { std::string ID; int House; std::string CallbackLib; };
    std::vector<SessionSnap> snaps;

    {
        std::lock_guard<std::mutex> lk(g_SessionMutex);
        snaps.reserve(g_SessionsByID.size());
        for (auto& kv : g_SessionsByID) {
            snaps.push_back({ kv.first, kv.second.House, kv.second.CallbackLib });
        }
    }

    for (auto& snap : snaps) {
        if (snap.House < 0 || snap.House >= HouseClass::Array->Count) continue;
        auto pHouse = HouseClass::Array->Items[snap.House];
        if (!pHouse) continue;

        // 收集本帧需要派发的回调（先收集再派发，避免在迭代 Buildings 时
        // 跨函数调用导致 mutex 死锁）
        struct ReadyEntry { DWORD Address; const char* TypeID; int X; int Y; };
        std::vector<ReadyEntry> readyList;

        for (auto* pBld : pHouse->Buildings) {
            if (!pBld) continue;
            if (!pBld->ActuallyPlacedOnMap) continue;
            if (!pBld->HasBuildUp) continue;
            if (pBld->InLimbo) continue;

            DWORD addr = (DWORD)pBld;
            DWORD addrKey = addr;

            // 检查是否已派发过（在 mutex 下读 AnnouncedBuildings）
            bool alreadyAnnounced = false;
            {
                std::lock_guard<std::mutex> lk(g_SessionMutex);
                auto it = g_SessionsByID.find(snap.ID);
                if (it == g_SessionsByID.end()) {
                    alreadyAnnounced = true;  // 会话已死，跳过
                    break;
                }
                if (it->second.AnnouncedBuildings.count(addrKey)) {
                    alreadyAnnounced = true;
                }
            }
            if (alreadyAnnounced) continue;

            const char* typeId = pBld->Type ? pBld->Type->ID : "";
            auto cell = pBld->GetMapCoords();
            readyList.push_back({ addr, typeId, cell.X, cell.Y });
        }

        // 派发回调并更新 AnnouncedBuildings
        for (auto& r : readyList) {
            JsonFile payloadFile;
            auto payloadObj = payloadFile.GetObj();
            payloadObj.AddString("SessionID", snap.ID);
            payloadObj.AddInt   ("House",     snap.House);
            payloadObj.AddString("TypeID",    r.TypeID);

            char addrBuf[20];
            sprintf_s(addrBuf, "0x%08X", r.Address);
            payloadObj.AddString("Address", addrBuf);

            JsonFile cellFile;
            auto cellArr = cellFile.GetObj();
            cellArr.AddInt("0", r.X);
            cellArr.AddInt("1", r.Y);
            payloadObj.AddObject("Cell", cellArr);

            cJSON* payload = payloadFile.Release();

            // 用临时 AgentSession 让 DispatchCallback 读到 CallbackLib
            AgentSession tmp;
            tmp.CallbackLib = snap.CallbackLib;
            DispatchCallback(&tmp, "OnBuildingReady", payload);

            // 加入 AnnouncedBuildings
            {
                std::lock_guard<std::mutex> lk(g_SessionMutex);
                auto it = g_SessionsByID.find(snap.ID);
                if (it != g_SessionsByID.end()) {
                    it->second.AnnouncedBuildings.insert(r.Address);
                }
            }
        }
    }
}
```

**注意实现细节**：
- 三次加锁：① 快照会话列表 ② 每个建筑单独检查 AnnouncedBuildings ③ 派发后插入 AnnouncedBuildings。这种粒度避免在持锁期间调用 DispatchCallback（PostAsyncRemoteCall 可能阻塞）。
- 若会话在派发过程中被并发销毁，第三步的 `find` 会返回 end()，安全跳过——不会泄漏 AnnouncedBuildings 条目（因为会话整个结构都被 erase 了）。

### 文件 2：[AgentBackend.h](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/AgentBackend.h) — 删旧声明 + 加新声明

**删除**（Watch 段，第 230-231 行附近）：
```cpp
void __cdecl IHAgentBackend_WatchPointer(JsonObject Context);
void __cdecl IHAgentBackend_WatchBuildingReady(JsonObject Context);
```

**新增**（在 `Session_FrameUpdate` 声明附近）：
```cpp
void Session_BuildingReadyPoll();  // Phase D — OnBuildingReady 自动派发
```

### 文件 3：[Agent.Marker.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Agent.Marker.cpp) — 重写

**保留**：
- `_Marks` + `_MarksMutex` + `MarkObject`/`UnmarkObject`/`IsObjectMarked`
- `IHAgentBackend_MarkObject`/`UnmarkObject`/`ListMarks`/`ClearMarks`
- `MarkerACP` + `MoveToIHCore`
- `IHAgentBackend_FrameUpdate` 框架（调用 `Session_FrameUpdate()`）

**删除**：
- `WatchEntry` struct + `_Watches` + `_WatchMutex`
- `BuildingReadyWatch` struct + `_BuildingWatches` + `_BuildingWatchMutex`
- `IHAgentBackend_WatchPointer` 实现
- `IHAgentBackend_WatchBuildingReady` 实现
- `IHAgentBackend_FrameUpdate` 中的 `_BuildingWatches` 轮询循环

**改造 `PointerExpiredProcess`**：
```cpp
void CALLBACK PointerExpiredProcess(AbstractClass* pDyingObj, bool bRemoved)
{
    if (!pDyingObj) return;
    DWORD addr = (DWORD)pDyingObj;

    // 1) auto-mark cleanup（保留）
    if (bRemoved) UnmarkObject(addr);

    // 2) v2 OnUnitDestroyed 派发：仅对 bRemoved=true 且对象是 TechnoClass 子类时
    //    才派发，避免对象"暂时离场"（如进入运输艇）误触发。
    if (!bRemoved) return;

    auto wa = pDyingObj->WhatAmI();
    if (wa != AbstractType::Unit && wa != AbstractType::Infantry &&
        wa != AbstractType::Building && wa != AbstractType::Aircraft) {
        return;
    }

    auto pT = static_cast<TechnoClass*>(pDyingObj);
    auto pH = pT->GetOwningHouse();
    if (!pH) return;
    int houseIdx = pH->ArrayIndex;

    // 查找接管此 House 的会话
    AgentSession* pS = FindSessionByHouse(houseIdx);
    if (!pS) return;

    // 复制必要字段（pS 在 mutex 释放后可能失效）
    std::string sid = pS->ID;
    int         house = pS->House;
    std::string cbLib = pS->CallbackLib;

    // 构造 payload
    const char* typeId = "";
    if (auto* pType = pT->GetTechnoType()) typeId = pType->ID;

    JsonFile payloadFile;
    auto payloadObj = payloadFile.GetObj();
    payloadObj.AddString("SessionID", sid);
    payloadObj.AddInt   ("House",     house);
    payloadObj.AddString("TypeID",    typeId);

    char addrBuf[20];
    sprintf_s(addrBuf, "0x%08X", addr);
    payloadObj.AddString("Address", addrBuf);

    // KillerAddress 当前为 stub — 引擎 PointerExpiredProcess 不提供击杀者
    // 需 hook WeaponClass::Fire 或 TechnoClass::ReceiveDamage 跟踪最后攻击者
    payloadObj.AddString("KillerAddress", "0x00000000");

    cJSON* payload = payloadFile.Release();

    AgentSession tmp;
    tmp.CallbackLib = std::move(cbLib);
    DispatchCallback(&tmp, "OnUnitDestroyed", payload);
}
```

**注意**：原 `PointerExpiredProcess` 中调用 `IsPointerAlive` 之前用 `(AbstractClass*)addr` 访问 `WhatAmI()`——但根据 [Pointers.cpp:13](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/Pointers.cpp#L13) 注释 "DO NOT call GetClassName/vtable on pDyingObj — it has already been freed"。需要确认这个限制。

**重要 safety check**：`pDyingObj` 在 PointerExpiredProcess 触发时已经被引擎释放过（多次广播）——vtable 调用可能不安全。**需要降级方案**：先检查 bRemoved，再用 `IsPointerAlive(addr)` 验证指针仍在 AbstractClass::Array 中（如果是，则 vtable 安全）。如果已不在数组中，跳过 OnUnitDestroyed 派发——会漏掉部分死亡事件，但保证不崩溃。

降级后的 PointerExpiredProcess：
```cpp
if (!bRemoved) return;
if (!IsPointerAlive(addr)) return;  // 已被引擎释放，vtable 不安全
// ... 此后 pDyingObj 的 vtable 调用安全
```

**改造 `IHAgentBackend_FrameUpdate`**：
```cpp
void __cdecl IHAgentBackend_FrameUpdate(JsonObject)
{
    // v2: session timeout detection + viewport sampling
    Session_FrameUpdate();

    // v2: OnBuildingReady 自动派发
    Session_BuildingReadyPoll();

    // Marker 系统无需每帧轮询；旧 _BuildingWatches 已删除
}
```

### 文件 4：[dllmain.cpp](file:///f:/RA2%20Engine%20Extension/IHCore/IHAgentBackend/dllmain.cpp) — 删除注册

删除第 39-40 行：
```cpp
{"WatchPointer",FuncInfo{IHAgentBackend_WatchPointer,FuncType::Action}},
{"WatchBuildingReady",FuncInfo{IHAgentBackend_WatchBuildingReady,FuncType::Action}},
```

### 文件 5：`STUBS-TODO.md`（Phase E — 主 agent 编写）

新建 `f:\RA2 Engine Extension\IHCore\IHAgentBackend\STUBS-TODO.md`，汇总 6 个待补项：

1. **OnUnderAttack** — 需在 `TechnoClass::ReceiveDamage` 安装 `DEFINE_HOOK`，IDA 逆向定位地址
2. **OnChatMessage** — 需 hook 聊天接收函数
3. **OnBeacon** — 需 hook 信标 place/remove 函数
4. **FindPath** — 当前返回 ERROR_NOT_SUPPORTED；实现选项：a) 基于 CellClass 的 A*；b) 临时 UnitClass 实例调用 `AStarAttempt`
5. **OnUnitDestroyed KillerAddress** — 当前固定为 "0x00000000"；需 hook `WeaponClass::Fire` 或 `TechnoClass::ReceiveDamage` 跟踪最后攻击者
6. **接管 House 后跳过原版 AI** — CLAUDE.md 声明由 IHCore 层负责，但 IHCore 未实现；需 IHCore 添加 hook 读取本项目 `FindSessionByHouse(idx) != nullptr` 跳过 AI

每项包含：当前行为、未来实现路径、依赖（IDA 逆向 / IHCore 协作）、优先级建议。

## 执行策略

**Phase D**：用 sub-agent（general-purpose）执行，与 Phase A/B/C 一致。Sub-agent prompt 需自包含，包含：
- 当前 Agent.Marker.cpp 全文（305 行）
- 要做的 5 处改动（Agent.Session.cpp 加函数 / AgentBackend.h 改声明 / Agent.Marker.cpp 重写 / dllmain.cpp 删注册 / PointerExpiredProcess 的 vtable safety check）
- 引擎 API 参考（HouseClass::Buildings、AbstractClass::GetOwningHouse、BuildingClass 字段）
- Phase A 已有 helper（DispatchCallback、FindSessionByHouse）
- 验收清单

**Phase E**：主 agent 直接编写 STUBS-TODO.md。

## 假设与决策

1. **OnBuildingReady 轮询位置**：放在 Agent.Session.cpp 的 `Session_BuildingReadyPoll()` 函数中，从 `IHAgentBackend_FrameUpdate` 调用。原因：访问私有注册表 + 修改 AnnouncedBuildings 字段。
2. **PointerExpiredProcess 的 vtable safety**：先 `if (!IsPointerAlive(addr)) return;` 再访问 vtable。这会漏掉已彻底释放的对象，但保证不崩溃。这是保守方案，符合 Pointers.cpp 的 "DO NOT call vtable on pDyingObj" 注释。
3. **AnnouncedBuildings 永不收缩**：会话生命周期内同一建筑只派发一次 OnBuildingReady。避免维修导致 HasBuildUp 短暂闪烁触发重复回调。
4. **KillerAddress stub 为 "0x00000000"**：作为字符串而非数字，与 Address 字段格式一致。
5. **OnUnderAttack/OnChatMessage/OnBeacon 本阶段不注册为 Action**：它们是后端主动推送的回调，不是前端可调用接口。只是当前没有触发它们的 hook 入口。Phase E 文档说明即可。
6. **保留 `IHAgentBackend_FrameUpdate` 中 `Session_FrameUpdate()` 调用**：Phase A 已加入，Phase D 不动这个调用顺序。

## 验收清单

### Phase D 验收
- [ ] 编译零 error 零 warning（Release|Win32）
- [ ] Grep `WatchPointer|WatchBuildingReady|_Watches|_BuildingWatches` 在 .cpp/.h 中 0 匹配（CLAUDE.md/API-SPEC.md 除外）
- [ ] `dllmain.cpp` 注册表中 WatchPointer/WatchBuildingReady 已删除
- [ ] `AgentBackend.h` 中 WatchPointer/WatchBuildingReady 声明已删除，`Session_BuildingReadyPoll` 声明已加入
- [ ] `Agent.Session.cpp` 中 `Session_BuildingReadyPoll` 实现，正确使用 mutex 三段式
- [ ] `Agent.Marker.cpp` 中 `PointerExpiredProcess` 调用 `IsPointerAlive` 后再访问 vtable
- [ ] `Agent.Marker.cpp` 中 `IHAgentBackend_FrameUpdate` 调用 `Session_BuildingReadyPoll()`
- [ ] 保留：`_Marks`/`MarkObject`/`UnmarkObject`/`IsObjectMarked`/4 个 Mark Action/`MarkerACP`/`MoveToIHCore`
- [ ] 调用 `BeginSession -House 0` + 建筑完成建造 → 控制台日志可见 OnBuildingReady 派发尝试（前端未实现会失败，backend 日志可见 `frontend not available (discarded)`）
- [ ] 调用 `EndSession -SessionID <id>` → 触发 OnSessionClosed（Phase A 已实现，回归测试）

### Phase E 验收
- [ ] `STUBS-TODO.md` 存在，包含 6 个 stub 项
- [ ] 每项含：当前行为、未来实现路径、依赖、优先级
- [ ] 文档可独立阅读（无需查源码即可理解待补工作）

## 实施步骤

1. **启动 Phase D sub-agent**：用 general-purpose subagent_type，prompt 包含本计划 Phase D 部分的全部细节 + 当前 Agent.Marker.cpp 全文 + 引擎 API 参考。
2. **验证 Phase D 输出**：执行验收清单的静态检查项（grep/Read 文件）。
3. **主 agent 编写 Phase E**：编写 `STUBS-TODO.md`。
4. **最终汇总**：向用户报告 v2 迁移完成，附 STUBS-TODO.md 摘要。
