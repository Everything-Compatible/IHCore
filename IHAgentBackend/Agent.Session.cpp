// ════════════════════════════════════════════════════════════════════════════
//  Agent.Session.cpp — v2 Session 管理基础 (Phase A)
//
//  实现:
//    - AgentSession 结构体 + 线程安全的全局注册表
//    - UUID v4 生成器 (RFC 4122)
//    - FindSessionByID / FindSessionByHouse / RequireSession 辅助函数
//    - IsTechnoVisibleTo / IsCellVisibleTo 可见性辅助函数
//    - DispatchCallback 辅助函数 (PostAsyncRemoteCall 封装, 前端不存在时安全)
//    - Session_FrameUpdate: 超时检测 + 视口采样
//    - Session_OnGameReset: 清除所有 Session
//    - EndSessionInternal 辅助函数 (分发 OnSessionClosed, 然后擦除)
//    - Action 处理函数: IHAgentBackend_BeginSession / EndSession / SessionHeartbeat / GetToolList
//
//  命名: YR 引擎已有 SessionClass::Instance (单/多人游戏 Session).
//  该结构体命名为 AgentSession，文件命名为 Agent.Session.cpp 以避免冲突。
//
//  线程安全约定:
//    - 所有注册表修改 (insert/erase) 必须持有 g_SessionMutex。
//    - FindSessionByID/ByHouse 返回指向 map 中原始 AgentSession* 的指针；调用者
//      不得在可能擦除 Session 的调用 (EndSession /
//      Session_OnGameReset / Session_FrameUpdate 的超时分支) 之间持有此指针。
//      仅用于短临界区，或在持有 mutex 时将所需字段拷贝出来。
//    - Session 仅在 EndSessionInternal 中在 mutex 保护下擦除。
// ════════════════════════════════════════════════════════════════════════════

#include "AgentBackend.h"
#include "Pointers.h"

#include <format>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <EC.h>
#include <Debug.h>
#include <YRPP.h>
#include <HouseClass.h>
#include <BuildingClass.h>
#include <CellClass.h>
#include <TechnoClass.h>
#include <TacticalClass.h>
#include <Unsorted.h>
#include <ExtJson.h>
#include <IH.Ext.h>

// ════════════════════════════════════════════════════════════════════════════
//  AgentSession 结构体
// ════════════════════════════════════════════════════════════════════════════

struct AgentSession {
    std::string                ID;                       // UUID v4
    int                        House            = -1;    // HouseClass::ArrayIndex
    bool                       Cheat            = false;
    int                        TimeoutSec       = 30;
    int64_t                    LastHeartbeatFrame = 0;   // Unsorted::CurrentFrame
    std::string                CallbackLib = "IHAgentFrontend";
    // OnBuildingReady 去重: 已向前端宣布的建筑地址
    std::unordered_set<DWORD>  AnnouncedBuildings;
    // GetPlayerViewRoute 环形缓冲区 (≈0.5s 采样, 60s 窗口 = 120 条)
    struct ViewSample { int64_t Frame; CellStruct Cell; };
    std::vector<ViewSample>    ViewHistory;
    int64_t                    LastViewSampleFrame = 0;
};

// ════════════════════════════════════════════════════════════════════════════
//  全局注册表 (mutex 保护)
// ════════════════════════════════════════════════════════════════════════════

static std::mutex                                       g_SessionMutex;
static std::unordered_map<std::string, AgentSession>    g_SessionsByID;
static std::unordered_map<int, std::string>             g_SessionByHouse;  // HouseIdx → SessionID

// ════════════════════════════════════════════════════════════════════════════
//  UUID v4 生成器 (RFC 4122)
// ════════════════════════════════════════════════════════════════════════════

std::string GenerateUUIDv4()
{
    // 每个进程只播种一次; mt19937_64 仅在 C++20 之后对独立调用线程安全
    // — 但 Ext::PostAsyncRemoteCall 由游戏线程分发，EC Action 处理函数也在游戏线程中，因此这是安全的。
    static std::mt19937_64 rng{ std::random_device{}() };
    static const char*     hex = "0123456789abcdef";

    uint64_t a = rng();
    uint64_t b = rng();

    // 16 个原始字节，大端字节序适用于 UUID 格式化
    unsigned char bytes[16];
    for (int i = 0; i < 8; ++i) bytes[i]     = static_cast<unsigned char>((a >> (56 - 8 * i)) & 0xFF);
    for (int i = 0; i < 8; ++i) bytes[8 + i] = static_cast<unsigned char>((b >> (56 - 8 * i)) & 0xFF);

    // RFC 4122 v4: byte 6 的高 4 位 = 0x4
    bytes[6] = static_cast<unsigned char>((bytes[6] & 0x0F) | 0x40);
    // 变种: byte 8 的高 4 位 ∈ {0x8,0x9,0xa,0xb}
    bytes[8] = static_cast<unsigned char>((bytes[8] & 0x3F) | 0x80);

    // 布局: 8-4-4-4-12 十六进制字符，带连字符
    char buf[37];
    int  p = 0;
    for (int i = 0; i < 16; ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10) buf[p++] = '-';
        buf[p++] = hex[(bytes[i] >> 4) & 0xF];
        buf[p++] = hex[bytes[i] & 0xF];
    }
    buf[36] = '\0';
    return std::string(buf);
}

// ════════════════════════════════════════════════════════════════════════════
//  查找辅助函数
//
//  约定: 调用者不持有 g_SessionMutex。返回的原始指针仅在下一次可能擦除
//  Session 的调用 (EndSession / Session_OnGameReset / Session_FrameUpdate 超时分支)
//  之前有效。仅用于短临界区，或在持有 mutex 时将所需字段拷贝出来。
// ════════════════════════════════════════════════════════════════════════════

AgentSession* FindSessionByID(const std::string& id)
{
    std::lock_guard<std::mutex> lk(g_SessionMutex);
    auto it = g_SessionsByID.find(id);
    return it == g_SessionsByID.end() ? nullptr : &it->second;
}

AgentSession* FindSessionByHouse(int houseIdx)
{
    std::lock_guard<std::mutex> lk(g_SessionMutex);
    auto it = g_SessionByHouse.find(houseIdx);
    if (it == g_SessionByHouse.end()) return nullptr;
    auto sit = g_SessionsByID.find(it->second);
    return sit == g_SessionsByID.end() ? nullptr : &sit->second;
}

// 从 Context 中读取 SessionID; 如果缺失/无效或游戏未启动，返回 nullptr + 设置错误。
// 当 Action 需要时，调用者应单独检查 _GameStarted；RequireSession 自身为安全起见会报告 ERROR_NOT_READY。
AgentSession* RequireSession(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) {
        ECDebug::ReturnStdError(ERROR_NOT_READY);
        return nullptr;
    }
    auto oSID = Context.GetObjectItem("SessionID");
    if (!oSID || !oSID.IsTypeString()) {
        ECDebug::ReturnStdError(ERROR_NOT_FOUND);
        return nullptr;
    }
    std::string sid = oSID.GetString();
    AgentSession* pS = FindSessionByID(sid);
    if (!pS) {
        ECDebug::ReturnStdError(ERROR_NOT_FOUND);
        return nullptr;
    }
    return pS;
}

// ════════════════════════════════════════════════════════════════════════════
//  可见性辅助函数
//  算法按迁移计划:
//    Cheat                    → 可见
//    cell shrouded            → 隐藏
//    allied                   → 可见
//    非盟友 + AI 所属 + Cloakable + !HasBeenAttacked → 隐藏
//    其他                     → 可见
// ════════════════════════════════════════════════════════════════════════════

bool IsTechnoVisibleTo(TechnoClass* pT, AgentSession* pS)
{
    if (!pT || !pS) return false;
    if (pS->Cheat) return true;

    auto pCell = pT->GetCell();
    if (pCell && pCell->IsShrouded()) return false;

    auto pH = pT->GetOwningHouse();
    if (!pH) return true;  // 中立 — 无所属过滤条件

    if (pS->House < 0 || pS->House >= HouseClass::Array->Count) return false;
    auto pSelf = HouseClass::Array->Items[pS->House];
    if (!pSelf) return false;

    if (pSelf->IsAlliedWith(pH)) return true;

    // 非盟友: 隐藏尚未暴露自身的隐形 AI 单位
    if (!pSelf->IsControlledByHuman() && pT->Cloakable && !pT->HasBeenAttacked)
        return false;

    return true;
}

bool IsCellVisibleTo(CellClass* pC, AgentSession* pS)
{
    if (!pC || !pS) return false;
    if (pS->Cheat) return true;
    return !pC->IsShrouded();
}

// ════════════════════════════════════════════════════════════════════════════
//  DispatchCallback — 向 Session 的前端库发起 fire-and-forget 的 PostAsyncRemoteCall 调用。
//  取得 payload (cJSON*) 的所有权。
//
//  cJSON 所有权约定 (与现有 Agent.Marker.cpp 模式一致):
//    cJSON 树交由 JsonObject 处理，在此从不显式释放。
//    IHCore 的 PostAsyncRemoteCall 实现会取得所有权并在内部释放，无论返回值如何。
//
//  前端不存在时安全调用: PostAsyncRemoteCall 返回 false →
//  记录到 std::cout，不崩溃。
// ════════════════════════════════════════════════════════════════════════════

void DispatchCallback(AgentSession* pS, const char* methodName, cJSON* payload)
{
    if (!pS || !methodName || !payload) {
        // 防御性: 仅当未将 payload 移交给 PostAsyncRemoteCall (因为我们无法移交) 时释放
        if (payload) cJSON_Delete(payload);
        return;
    }

    std::string cbLib = pS->CallbackLib;
    bool ok = Ext::PostAsyncRemoteCall(
        cbLib.c_str(),
        methodName,
        2147483647,
        JsonObject(payload)  // 所有权已转移至 IHCore
    );

    if (!ok) {
        // 前端不存在 — 记录日志并继续。payload 现在由 IHCore 拥有。
        std::cout << std::format(
            "[Agent.Session] callback {}->{}: frontend not available (discarded)",
            cbLib, methodName) << std::endl;
    }
}

// ════════════════════════════════════════════════════════════════════════════
//  EndSessionInternal — 分发 OnSessionClosed，然后从注册表中擦除。
//  约定: 调用者不持有 g_SessionMutex。
//
//  迭代器安全: 首先按值拷贝 SessionID/House/CallbackLib，在 mutex 之外
//  分发，然后加锁并通过拷贝的 SessionID 擦除。Session 结构体自身可能在
//  擦除时被销毁，因此获取锁后不得再访问 pS。
// ════════════════════════════════════════════════════════════════════════════

static void EndSessionInternal(AgentSession* pS, const char* reason)
{
    if (!pS) return;

    // 在获取任何 mutex 之前拷贝所需的所有内容 — 在释放调用者的锁（如果有）
    // 和获取我们的锁之间，pS 可能已被另一个线程的擦除操作失效。
    std::string sid       = pS->ID;
    int         house     = pS->House;
    std::string cbLib     = pS->CallbackLib;

    // 使用 JsonFile 构建 OnSessionClosed payload，以便自动管理 cJSON 生命周期。
    JsonFile payloadFile;
    auto payloadObj = payloadFile.GetObj();
    payloadObj.AddString("SessionID", sid);
    payloadObj.AddInt   ("House",     house);
    payloadObj.AddString("Reason",    reason);
    // 以裸 cJSON* 形式取得 cJSON 树的所有权，用于 DispatchCallback。
    cJSON* payload = payloadFile.Release();

    // 在擦除之前分发（按计划）。当 Session 仍在注册表中时安全地异步触发 —
    // 前端重新发起 BeginSession 只会遇到 HouseAlreadyTaken，直到我们下面擦除为止。
    AgentSession tmp;
    tmp.CallbackLib = cbLib;
    DispatchCallback(&tmp, "OnSessionClosed", payload);

    // 从两个注册表中擦除
    {
        std::lock_guard<std::mutex> lk(g_SessionMutex);
        g_SessionsByID.erase(sid);
        g_SessionByHouse.erase(house);
    }
}

// ════════════════════════════════════════════════════════════════════════════
//  Session_OnGameReset — 清除所有 Session 和注册表
//  在设置 _GameStarted 后由 IHAgentBackend_OnGameReset (Agent.Core.cpp) 调用。
//  不分发回调 — 游戏重新开始，前端应重新 BeginSession。
// ════════════════════════════════════════════════════════════════════════════

void Session_OnGameReset()
{
    std::lock_guard<std::mutex> lk(g_SessionMutex);
    g_SessionsByID.clear();
    g_SessionByHouse.clear();
}

// ════════════════════════════════════════════════════════════════════════════
//  Session_FrameUpdate — 每帧 Session 维护。
//    1) 超时检测: LastHeartbeatFrame + TimeoutSec*15 < CurrentFrame
//       → EndSessionInternal(session, "timeout")
//    2) 视口采样: 每 7 帧 (15fps 时约 0.5s)，将中心单元格推入
//       ViewHistory，修剪至最近 120 条
//
//  算法: 在 mutex 下快照 Session ID，然后在 mutex 外处理每个 Session
//  (EndSessionInternal 会重新获取锁)。视口采样每 Session 重新加锁。
// ════════════════════════════════════════════════════════════════════════════

void Session_FrameUpdate()
{
    if (!_GameStarted.load(std::memory_order_acquire)) return;

    int64_t currentFrame = Unsorted::CurrentFrame;

    // 本帧需要关注的 Session 快照
    struct TimeoutEntry { std::string ID; int House; int64_t LastHeartbeat; int Timeout; };
    struct SampleEntry  { std::string ID; int64_t LastSample; };

    std::vector<TimeoutEntry> timeouts;
    std::vector<SampleEntry>  samples;

    {
        std::lock_guard<std::mutex> lk(g_SessionMutex);
        for (auto& kv : g_SessionsByID) {
            const auto& sid = kv.first;
            const auto& s   = kv.second;

            // 超时检查
            if (s.LastHeartbeatFrame + static_cast<int64_t>(s.TimeoutSec) * 15 < currentFrame) {
                timeouts.push_back({ sid, s.House, s.LastHeartbeatFrame, s.TimeoutSec });
            } else {
                // 仅对未超时的 Session 进行视口采样
                if (currentFrame - s.LastViewSampleFrame >= 7) {
                    samples.push_back({ sid, s.LastViewSampleFrame });
                }
            }
        }
    }

    // 在 mutex 外处理超时
    for (const auto& t : timeouts) {
        AgentSession* pS = FindSessionByID(t.ID);
        if (pS) {
            std::cout << std::format(
                "[Agent.Session] session {} (house {}) timed out (last HB frame={}, now={})",
                t.ID, t.House, t.LastHeartbeat, currentFrame) << std::endl;
            EndSessionInternal(pS, "timeout");
        }
    }

    // 视口采样
    // 为当前战术视图计算一次中心单元格。
    // 优先使用 VisibleCells[] 中部；回退到 ContainingMapCoords 中心；
    // 最后手段: TacticalPos 衍生的单元格（近似）。
    CellStruct centerCell{ 0, 0 };
    bool haveCenter = false;

    // TacticalClass::Instance 是 reference<TacticalClass*, 0x887324u>;
    // 通过 operator value_type& 的隐式转换得到指针值。
    TacticalClass* pTactical = TacticalClass::Instance;
    if (pTactical) {
        if (pTactical->VisibleCellCount > 0) {
            // VisibleCells 按渲染顺序填充；取中间元素
            auto* pCell = pTactical->VisibleCells[pTactical->VisibleCellCount / 2];
            if (pCell) {
                centerCell  = pCell->MapCoords;
                haveCenter  = true;
            }
        }
        if (!haveCenter) {
            // ContainingMapCoords 是 RectangleStruct {X, Y, Width, Height}
            // 以单元格为单位描述可见地图区域。
            const auto& r = pTactical->ContainingMapCoords;
            if (r.Width > 0 && r.Height > 0) {
                centerCell.X = static_cast<short>(r.X + r.Width  / 2);
                centerCell.Y = static_cast<short>(r.Y + r.Height / 2);
                haveCenter  = true;
            }
        }
    }

    // 在 mutex 下更新每个 Session 的 ViewHistory
    if (haveCenter) {
        std::lock_guard<std::mutex> lk(g_SessionMutex);
        for (const auto& s : samples) {
            auto it = g_SessionsByID.find(s.ID);
            if (it == g_SessionsByID.end()) continue;
            auto& sess = it->second;

            sess.ViewHistory.push_back({ currentFrame, centerCell });
            sess.LastViewSampleFrame = currentFrame;

            // 修剪至最近 120 条 (0.5s 采样时约 60s)
            if (sess.ViewHistory.size() > 120) {
                sess.ViewHistory.erase(
                    sess.ViewHistory.begin(),
                    sess.ViewHistory.begin() + (sess.ViewHistory.size() - 120));
            }
        }
    }
}

// ════════════════════════════════════════════════════════════════════════════
//  Session_BuildingReadyPoll — Phase D。每帧由 IHAgentBackend_FrameUpdate 调用。
//  枚举每个 Session 的 House->Buildings 列表，对于刚刚进入"就绪"状态
//  (ActuallyPlacedOnMap && HasBuildUp && !InLimbo) 且尚未宣布过的建筑
//  (通过 AgentSession::AnnouncedBuildings 去重)，
//  分发 OnBuildingReady 回调。
//
//  算法 (三阶段 mutex 约定 — 绝不在 DispatchCallback 期间持有
//  g_SessionMutex，因为 PostAsyncRemoteCall 可能阻塞):
//    1. 在 mutex 下对所有 Session 做快照 (ID, House, CallbackLib)。
//    2. 对每个快照，遍历 pHouse->Buildings；对每个候选建筑，
//       重新加锁检查 AnnouncedBuildings + Session 是否仍存在。
//       将就绪条目收集到本地 vector。
//    3. 对每个就绪条目，构建 payload，DispatchCallback，然后重新加锁
//       将地址插入 AnnouncedBuildings。
//
//  注意: AnnouncedBuildings 在 Session 生命周期内不会缩小 —
//  同一建筑最多宣布一次。这避免了在 HasBuildUp 闪烁时
//  (如维修期间) 的虚假重新宣布。
// ════════════════════════════════════════════════════════════════════════════

void Session_BuildingReadyPoll()
{
    if (!_GameStarted.load(std::memory_order_acquire)) return;

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

        struct ReadyEntry { DWORD Address; const char* TypeID; int X; int Y; };
        std::vector<ReadyEntry> readyList;

        for (auto* pBld : pHouse->Buildings) {
            if (!pBld) continue;
            if (!pBld->ActuallyPlacedOnMap) continue;
            if (!pBld->HasBuildUp) continue;
            if (pBld->InLimbo) continue;

            DWORD addr = (DWORD)pBld;

            bool alreadyAnnounced = false;
            {
                std::lock_guard<std::mutex> lk(g_SessionMutex);
                auto it = g_SessionsByID.find(snap.ID);
                if (it == g_SessionsByID.end()) {
                    // 迭代过程中 Session 已死亡 — 跳出建筑循环
                    alreadyAnnounced = true;
                    break;
                }
                if (it->second.AnnouncedBuildings.count(addr)) {
                    alreadyAnnounced = true;
                }
            }
            if (alreadyAnnounced) continue;

            const char* typeId = pBld->Type ? pBld->Type->ID : "";
            auto cell = pBld->GetMapCoords();
            readyList.push_back({ addr, typeId, cell.X, cell.Y });
        }

        for (auto& r : readyList) {
            JsonFile payloadFile;
            auto payloadObj = payloadFile.GetObj();
            payloadObj.AddString("SessionID", snap.ID);
            payloadObj.AddInt   ("House",     snap.House);
            payloadObj.AddString("TypeID",    r.TypeID);

            char addrBuf[20];
            sprintf_s(addrBuf, "0x%08X", r.Address);
            payloadObj.AddString("Address", addrBuf);

            // 将 Cell 构建为 JSON 数组 [X, Y] — 模式来自 Agent.Coord.cpp:283-286
            cJSON* cellArr = cJSON_CreateIntArray(nullptr, 0);
            cJSON_AddItemToArray(cellArr, cJSON_CreateNumber(r.X));
            cJSON_AddItemToArray(cellArr, cJSON_CreateNumber(r.Y));
            payloadObj.AddObjectItem("Cell", cellArr);

            cJSON* payload = payloadFile.Release();

            AgentSession tmp;
            tmp.CallbackLib = snap.CallbackLib;
            DispatchCallback(&tmp, "OnBuildingReady", payload);

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

// ════════════════════════════════════════════════════════════════════════════
//  DispatchUnitDestroyed — PointerExpiredProcess (Agent.Marker.cpp) 的
//  Phase D 辅助函数。从 Session 字段 + 死亡单位的类型信息构建
//  OnUnitDestroyed payload，然后转发给 DispatchCallback。
//
//  为何在此处: AgentSession 的完整结构体定义对该 TU 是私有的
//  (按 AgentBackend.h 设计意图 — "forward decl — full
//  definition in Agent.Session.cpp")。来自其他 TU (如
//  PointerExpiredProcess) 的调用者无法直接读取 pS->ID / pS->House /
//  pS->CallbackLib，因此我们暴露一个按需构建的辅助函数，
//  在底层拷贝这些字段并在此构造 payload。
//
//  KillerAddress 被存根为 "0x00000000" — 引擎的 PointerExpiredProcess
//  不提供击杀者。记录在 STUBS-TODO.md (Phase E)。
// ════════════════════════════════════════════════════════════════════════════

void DispatchUnitDestroyed(AgentSession* pS, const char* typeId, DWORD addr)
{
    if (!pS) return;

    // 拷贝字段 — pS 可能被并发的 Session 擦除操作失效
    std::string sid   = pS->ID;
    int         house = pS->House;
    std::string cbLib = pS->CallbackLib;

    JsonFile payloadFile;
    auto payloadObj = payloadFile.GetObj();
    payloadObj.AddString("SessionID", sid);
    payloadObj.AddInt   ("House",     house);
    payloadObj.AddString("TypeID",    typeId ? typeId : "");

    char addrBuf[20];
    sprintf_s(addrBuf, "0x%08X", addr);
    payloadObj.AddString("Address", addrBuf);

    // STUB: KillerAddress 始终为 "0x00000000" — 引擎的 PointerExpiredProcess
    // 不提供击杀者。需要在 WeaponClass::Fire 或 TechnoClass::ReceiveDamage
    // 上添加 hook 来追踪最后攻击者。参见 STUBS-TODO.md。
    payloadObj.AddString("KillerAddress", "0x00000000");

    cJSON* payload = payloadFile.Release();

    AgentSession tmp;
    tmp.CallbackLib = std::move(cbLib);
    DispatchCallback(&tmp, "OnUnitDestroyed", payload);
}

// ════════════════════════════════════════════════════════════════════════════
//  DispatchUnderAttack — Phase E stub。
//
//  为 OnUnderAttack 回调提供 dispatch 入口。函数体完整可调用，
//  但当前无触发点 —— 需要在 TechnoClass::ReceiveDamage 上安装
//  DEFINE_HOOK（依赖 IDA 逆向定位函数地址），在 hook 中读取
//  this->GetOwningHouse()->ArrayIndex、attacker 参数、damage 参数后调用本函数。
//
//  payload: {SessionID, House, VictimAddress, AttackerAddress, Damage}
//  参见 STUBS-TODO.md #1。
// ════════════════════════════════════════════════════════════════════════════

void DispatchUnderAttack(AgentSession* pS, DWORD victimAddr, DWORD attackerAddr, int damage)
{
    if (!pS) return;

    std::string sid   = pS->ID;
    int         house = pS->House;
    std::string cbLib = pS->CallbackLib;

    JsonFile payloadFile;
    auto payloadObj = payloadFile.GetObj();
    payloadObj.AddString("SessionID", sid);
    payloadObj.AddInt   ("House",     house);

    char victimBuf[20];
    sprintf_s(victimBuf, "0x%08X", victimAddr);
    payloadObj.AddString("VictimAddress", victimBuf);

    char attackerBuf[20];
    sprintf_s(attackerBuf, "0x%08X", attackerAddr);
    payloadObj.AddString("AttackerAddress", attackerBuf);

    payloadObj.AddInt("Damage", damage);

    cJSON* payload = payloadFile.Release();

    AgentSession tmp;
    tmp.CallbackLib = std::move(cbLib);
    DispatchCallback(&tmp, "OnUnderAttack", payload);
}

// ════════════════════════════════════════════════════════════════════════════
//  DispatchChatMessage — Phase E stub。
//
//  为 OnChatMessage 回调提供 dispatch 入口。函数体完整可调用，
//  但当前无触发点 —— 需要在 YR 引擎的聊天接收函数（疑似
//  HouseClass::ReceiveChat 或 NetworkClass 相关方法）上安装 hook
//  （依赖 IDA 逆向），在 hook 中读取 FromHouse / ToHouse / Message
//  后调用本函数。
//
//  payload: {SessionID, FromHouse, ToHouse, Message, IsAlly}
//  参见 STUBS-TODO.md #2。
// ════════════════════════════════════════════════════════════════════════════

void DispatchChatMessage(AgentSession* pS, int fromHouse, int toHouse,
                         const char* message, bool isAlly)
{
    if (!pS) return;

    std::string sid   = pS->ID;
    std::string cbLib = pS->CallbackLib;

    JsonFile payloadFile;
    auto payloadObj = payloadFile.GetObj();
    payloadObj.AddString("SessionID", sid);
    payloadObj.AddInt   ("FromHouse", fromHouse);
    payloadObj.AddInt   ("ToHouse",   toHouse);
    payloadObj.AddString("Message",   message ? message : "");
    payloadObj.AddBool  ("IsAlly",    isAlly);

    cJSON* payload = payloadFile.Release();

    AgentSession tmp;
    tmp.CallbackLib = std::move(cbLib);
    DispatchCallback(&tmp, "OnChatMessage", payload);
}

// ════════════════════════════════════════════════════════════════════════════
//  DispatchBeacon — Phase E stub。
//
//  为 OnBeacon 回调提供 dispatch 入口。函数体完整可调用，
//  但当前无触发点 —— 需要在 YR 引擎的信标 place/remove 函数上安装
//  hook（依赖 IDA 逆向），在 hook 中读取 House / Action / Cell 后调用本函数。
//
//  payload: {SessionID, House, Action: "place"|"remove", Cell: [X,Y]}
//  参见 STUBS-TODO.md #3。
// ════════════════════════════════════════════════════════════════════════════

void DispatchBeacon(AgentSession* pS, const char* action, int cellX, int cellY)
{
    if (!pS) return;

    std::string sid   = pS->ID;
    int         house = pS->House;
    std::string cbLib = pS->CallbackLib;

    JsonFile payloadFile;
    auto payloadObj = payloadFile.GetObj();
    payloadObj.AddString("SessionID", sid);
    payloadObj.AddInt   ("House",     house);
    payloadObj.AddString("Action",    action ? action : "");

    cJSON* cellArr = cJSON_CreateIntArray(nullptr, 0);
    cJSON_AddItemToArray(cellArr, cJSON_CreateNumber(cellX));
    cJSON_AddItemToArray(cellArr, cJSON_CreateNumber(cellY));
    payloadObj.AddObjectItem("Cell", cellArr);

    cJSON* payload = payloadFile.Release();

    AgentSession tmp;
    tmp.CallbackLib = std::move(cbLib);
    DispatchCallback(&tmp, "OnBeacon", payload);
}

// ════════════════════════════════════════════════════════════════════════════
//  Action: IHAgentBackend_BeginSession
//    -House <int>            (必需)
//    -Cheat <bool>           (默认 false)
//    -Timeout <int>          (默认 30, 秒)
//    -CallbackLib <str>      (默认 "IHAgentFrontend")
//
//  返回:
//    {"Success":true, "SessionID":"...", "House":N, "Cheat":bool,
//     "Timeout":N, "ExpireAt":<frame>}
//  或 House 已被占用:
//    {"Success":false, "Reason":"HouseAlreadyTaken", "TakenBySession":"<id>"}
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_BeginSession(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) {
        ECDebug::ReturnStdError(ERROR_NOT_READY);
        return;
    }

    // -House (必需)
    auto oHouse = Context.GetObjectItem("House");
    if (!oHouse || !oHouse.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    int house = oHouse.GetInt();
    if (house < 0 || house >= HouseClass::Array->Count) {
        ECDebug::ReturnStdError(ERROR_INVALID_INDEX);
        return;
    }
    auto pHouse = HouseClass::Array->Items[house];
    if (!pHouse) {
        ECDebug::ReturnStdError(ERROR_INVALID_DATA);
        return;
    }

    // -Cheat (默认 false)
    bool cheat = false;
    auto oCheat = Context.GetObjectItem("Cheat");
    if (oCheat && oCheat.IsTypeBool()) cheat = oCheat.GetBool();
    else if (oCheat && oCheat.IsTypeString()) cheat = _IH_IsTrueString(oCheat.GetString());

    // -Timeout (默认 30)
    int timeout = 30;
    auto oTimeout = Context.GetObjectItem("Timeout");
    if (oTimeout && oTimeout.IsTypeNumber()) timeout = oTimeout.GetInt();
    if (timeout < 1) timeout = 1;

    // -CallbackLib (默认 "IHAgentFrontend")
    std::string cbLib = "IHAgentFrontend";
    auto oCb = Context.GetObjectItem("CallbackLib");
    if (oCb && oCb.IsTypeString()) cbLib = oCb.GetString();

    // 检查 House 是否已被占用 — 在 mutex 下读取
    std::string existingSessionID;
    {
        std::lock_guard<std::mutex> lk(g_SessionMutex);
        auto it = g_SessionByHouse.find(house);
        if (it != g_SessionByHouse.end()) {
            existingSessionID = it->second;
        }
    }
    if (!existingSessionID.empty()) {
        std::string result = std::format(
            R"({{"Success":false,"Reason":"HouseAlreadyTaken","TakenBySession":"{}"}})",
            existingSessionID);
        std::cout << result << std::endl;
        ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
        ECDebug::DoNotEcho();
        return;
    }

    // 生成 UUID + 创建 Session
    std::string sid = GenerateUUIDv4();
    int64_t now = Unsorted::CurrentFrame;
    int64_t expireAt = now + static_cast<int64_t>(timeout) * 15;

    {
        std::lock_guard<std::mutex> lk(g_SessionMutex);
        // 重新检查，以防在解锁和加锁之间另一个线程抢占了 House
        auto it = g_SessionByHouse.find(house);
        if (it != g_SessionByHouse.end()) {
            // 竞态: 另一个 Session 已抢占此 House
            std::string result = std::format(
                R"({{"Success":false,"Reason":"HouseAlreadyTaken","TakenBySession":"{}"}})",
                it->second);
            std::cout << result << std::endl;
            ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
            ECDebug::DoNotEcho();
            return;
        }

        AgentSession s;
        s.ID                 = sid;
        s.House              = house;
        s.Cheat              = cheat;
        s.TimeoutSec         = timeout;
        s.LastHeartbeatFrame = now;
        s.CallbackLib        = std::move(cbLib);
        s.LastViewSampleFrame = now;

        g_SessionsByID.emplace(sid, std::move(s));
        g_SessionByHouse.emplace(house, sid);
    }

    // 成功响应
    std::string result = std::format(
        R"({{"Success":true,"SessionID":"{}","House":{},"Cheat":{},"Timeout":{},"ExpireAt":{}}})",
        sid, house, cheat ? "true" : "false", timeout, expireAt);
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  Action: IHAgentBackend_EndSession
//    -SessionID <str>
//
//  返回:
//    {"Success":true, "SessionID":"...", "House":N}
//  触发 OnSessionClosed 回调，reason 为 "released"。
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_EndSession(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) {
        ECDebug::ReturnStdError(ERROR_NOT_READY);
        return;
    }
    auto oSID = Context.GetObjectItem("SessionID");
    if (!oSID || !oSID.IsTypeString()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    std::string sid = oSID.GetString();

    // 在 mutex 下拷贝 house (EndSessionInternal 会擦除)
    int house = -1;
    AgentSession* pS = nullptr;
    {
        std::lock_guard<std::mutex> lk(g_SessionMutex);
        auto it = g_SessionsByID.find(sid);
        if (it == g_SessionsByID.end()) {
            ECDebug::ReturnStdError(ERROR_NOT_FOUND);
            return;
        }
        pS   = &it->second;
        house = pS->House;
    }

    // EndSessionInternal 分发 OnSessionClosed(reason="released") 并擦除
    EndSessionInternal(pS, "released");

    std::string result = std::format(
        R"({{"Success":true,"SessionID":"{}","House":{}}})",
        sid, house);
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  Action: IHAgentBackend_SessionHeartbeat
//    -SessionID <str>
//
//  返回:
//    {"Success":true, "SessionID":"...", "ExpireAt":<frame>}
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_SessionHeartbeat(JsonObject Context)
{
    if (!_GameStarted.load(std::memory_order_acquire)) {
        ECDebug::ReturnStdError(ERROR_NOT_READY);
        return;
    }
    auto oSID = Context.GetObjectItem("SessionID");
    if (!oSID || !oSID.IsTypeString()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    std::string sid = oSID.GetString();

    int64_t now      = Unsorted::CurrentFrame;
    int     timeout  = 30;
    int64_t expireAt = now + 30 * 15;

    {
        std::lock_guard<std::mutex> lk(g_SessionMutex);
        auto it = g_SessionsByID.find(sid);
        if (it == g_SessionsByID.end()) {
            ECDebug::ReturnStdError(ERROR_NOT_FOUND);
            return;
        }
        auto& s = it->second;
        s.LastHeartbeatFrame = now;
        timeout  = s.TimeoutSec;
        expireAt = now + static_cast<int64_t>(timeout) * 15;
    }

    std::string result = std::format(
        R"({{"Success":true,"SessionID":"{}","ExpireAt":{}}})",
        sid, expireAt);
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  Action: IHAgentBackend_GetToolList
//    [-SessionID <str>]
//
//  无 Session: 返回所有工具名称。有 Session: 根据 Session 的 Cheat 标志
//  过滤 CheatTools。
//
//  返回形状按 API-SPEC.md 第 95-115 行:
//    {
//      "SessionID": "uuid" 或 null,
//      "Cheat": bool 或 null,
//      "Tools":      [ ... 16 个玩家工具 ... ],
//      "CheatTools": [ "AIProduce", "AIStopProduction" ] 或 [],
//      "StateQueries":[ ... 14 个查询名称 ... ]
//    }
//
//  注意: Phase B/C 可能尚未实现所有列出的工具 — 仍按规范列出；
//  它们将很快实现。
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_GetToolList(JsonObject Context)
{
    // GetToolList 在游戏前也很有用（前端可以在 BeginSession 之前获取工具列表），
    // 因此我们不检查 _GameStarted。

    std::string sid;
    bool cheat = false;
    bool hasSession = false;

    auto oSID = Context.GetObjectItem("SessionID");
    if (oSID && oSID.IsTypeString()) {
        sid = oSID.GetString();
        std::lock_guard<std::mutex> lk(g_SessionMutex);
        auto it = g_SessionsByID.find(sid);
        if (it != g_SessionsByID.end()) {
            cheat = it->second.Cheat;
            hasSession = true;
        } else {
            ECDebug::ReturnStdError(ERROR_NOT_FOUND);
            return;
        }
    }

    // 按 API-SPEC.md 第 95-115 行
    static const std::vector<std::string> kTools = {
        "IssueOrder", "Deploy", "Detonate", "Scatter",
        "Produce", "Suspend", "Abandon", "AbandonAll", "SetPrimary",
        "Place", "SpecialPlace",
        "Repair", "Sell", "SellCell",
        "Ally", "Cheer"
    };
    static const std::vector<std::string> kCheatTools = {
        "AIProduce", "AIStopProduction"
    };
    static const std::vector<std::string> kStateQueries = {
        "GetHouseInfo", "GetForceDistribution", "GetMapInfo",
        "GetTechTree", "GetTypeInfo", "GetProductionState",
        "GetCurrentTime", "GetPlayerViewRoute",
        "GetTechnoInRange", "GetTechnoInfo", "GetObjectsOnCell",
        "GetAerialObjects", "GetCellInfo", "FindPath"
    };

    JsonFile F;
    auto Obj = F.GetObj();

    if (hasSession) {
        Obj.AddString("SessionID", sid);
        Obj.AddBool  ("Cheat",     cheat);
    } else {
        Obj.AddNull ("SessionID");
        Obj.AddNull ("Cheat");
    }
    Obj.AddArrayString("Tools",       kTools);
    // CheatTools: 非作弊 Session 为空；作弊 Session 或无 Session 时为完整列表
    if (hasSession && !cheat) {
        Obj.AddArrayString("CheatTools", std::vector<std::string>{});
    } else {
        Obj.AddArrayString("CheatTools", kCheatTools);
    }
    Obj.AddArrayString("StateQueries", kStateQueries);

    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  IsHouseTaken — 导出给 IHCore 的 House AI 跳过 hook 使用。
//
//  包装 FindSessionByHouse(houseIdx) != nullptr。当某 House 已被 Agent
//  Session 接管时返回 true，IHCore 的 House AI 入口 hook 据此跳过原版
//  AI 处理（避免双重控制冲突）。
//
//  注意：本函数仅解决"IHAgentBackend 是否告知 House 已接管"的问题。
//  实际的 House AI 跳过 hook 需在 IHCore 项目中实现（在 HouseClass::AI
//  或类似入口安装 DEFINE_HOOK，调用本导出函数判断是否跳过）。
//  参见 STUBS-TODO.md #6。
// ════════════════════════════════════════════════════════════════════════════

extern "C" __declspec(dllexport) bool IsHouseTaken(int houseIdx)
{
    if (!_GameStarted.load(std::memory_order_acquire)) return false;
    return FindSessionByHouse(houseIdx) != nullptr;
}
