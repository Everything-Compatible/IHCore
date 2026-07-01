// ════════════════════════════════════════════════════════════════════════════
//  Agent.Actions.cpp — v2 操作层（阶段 B）
//
//  所有操作接受 `-SessionID`，通过 `RequireSession(Context)` 验证，
//  并通过 `EventClass::AddEvent(...)` 下发指令，而非直接调用引擎
//  （v2 下禁止使用 SetDestination / QueueMission / ClickedEvent /
//  IsBeingRepaired = true）。
//
//  每个处理器的布局：
//    1. RequireSession(Context) — 检查 _GameStarted + SessionID 查找。
//       若缺失/无效则返回 nullptr 并设置错误。
//    2. 解析操作特定的参数。
//    3. 通过适当的构造函数构建 EventClass。
//    4. EventClass::AddEvent(event)。
//    5. 发出 JSON `{Action, ..., House, Success:bool}`。
//
//  AIProduce / AIStopProduction 是 Cheat-only 的逃逸操作，直接写入
//  `House->ProducingXxxTypeIndex`（引擎的 AI 生产路径）。
// ════════════════════════════════════════════════════════════════════════════

#include "AgentBackend.h"

#include <format>
#include <string>
#include <iostream>
#include <atomic>
#include <vector>

#include <EC.h>
#include <Debug.h>
#include <YRPP.h>
#include <EventClass.h>
#include <TargetClass.h>
#include <GeneralDefinitions.h>   // Mission 枚举
#include <TechnoTypeClass.h>
#include <TechnoClass.h>
#include <HouseClass.h>
#include <ExtJson.h>

// ════════════════════════════════════════════════════════════════════════════
//  内部辅助函数
// ════════════════════════════════════════════════════════════════════════════

// 解析任务名称（区分大小写）或数字字符串为 Mission 整数。
// 解析失败返回 -2（调用者应将其视为参数错误）。
static int ParseMission(const std::string& s)
{
    if (s.empty()) return -2;
    bool is_int = true;
    for (char c : s) { if (!isdigit((unsigned char)c) && c != '-') { is_int = false; break; } }
    if (is_int) return std::stoi(s);
    if (s == "Sleep")              return (int)Mission::Sleep;
    if (s == "Attack")             return (int)Mission::Attack;
    if (s == "Move")               return (int)Mission::Move;
    if (s == "QMove")              return (int)Mission::QMove;
    if (s == "Retreat")            return (int)Mission::Retreat;
    if (s == "Guard")              return (int)Mission::Guard;
    if (s == "Sticky")             return (int)Mission::Sticky;
    if (s == "Enter")              return (int)Mission::Enter;
    if (s == "Capture")            return (int)Mission::Capture;
    if (s == "Eaten")              return (int)Mission::Eaten;
    if (s == "Harvest")            return (int)Mission::Harvest;
    if (s == "Area_Guard")         return (int)Mission::Area_Guard;
    if (s == "Return")             return (int)Mission::Return;
    if (s == "Stop")               return (int)Mission::Stop;
    if (s == "Ambush")             return (int)Mission::Ambush;
    if (s == "Hunt")               return (int)Mission::Hunt;
    if (s == "Unload")             return (int)Mission::Unload;
    if (s == "Sabotage")           return (int)Mission::Sabotage;
    if (s == "Construction")       return (int)Mission::Construction;
    if (s == "Selling")            return (int)Mission::Selling;
    if (s == "Repair")             return (int)Mission::Repair;
    if (s == "Rescue")             return (int)Mission::Rescue;
    if (s == "Missile")            return (int)Mission::Missile;
    if (s == "Harmless")           return (int)Mission::Harmless;
    if (s == "Open")               return (int)Mission::Open;
    if (s == "Patrol")             return (int)Mission::Patrol;
    if (s == "ParadropApproach")   return (int)Mission::ParadropApproach;
    if (s == "ParadropOverfly")    return (int)Mission::ParadropOverfly;
    if (s == "Wait")               return (int)Mission::Wait;
    if (s == "AttackMove")         return (int)Mission::AttackMove;
    if (s == "SpyplaneApproach")   return (int)Mission::SpyplaneApproach;
    if (s == "SpyplaneOverfly")    return (int)Mission::SpyplaneOverfly;
    return -2;
}

// 解析 "0x..." 十六进制字符串或十进制字符串为 DWORD 地址。
// 接受 "0x1234ABCD" 和 "305353261" 两种格式。解析失败返回 false。
static bool ParseAddress(const std::string& s, DWORD& out)
{
    if (s.empty()) return false;
    try {
        if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
            out = (DWORD)std::stoul(s.substr(2), nullptr, 16);
        } else {
            out = (DWORD)std::stoul(s, nullptr, 10);
        }
        return true;
    } catch (...) {
        return false;
    }
}

// 从 JsonObject 中解析地址，字段可以是字符串（"0x..."）或
// 数字（原始 DWORD）。字段缺失或无效时返回 false。
static bool ResolveAddress(const JsonObject& o, DWORD& out)
{
    if (!o) return false;
    if (o.IsTypeString()) return ParseAddress(o.GetString(), out);
    if (o.IsTypeNumber()) { out = (DWORD)o.GetInt(); return true; }
    return false;
}

// 验证 DWORD 指向一个存活的 TechnoClass（Unit/Infantry/Building/Aircraft）。
// 返回转换后的 TechnoClass*，无效则返回 nullptr（并设置 ERROR_INVALID_ADDRESS）。
static TechnoClass* ResolveLiveTechno(DWORD addr)
{
    if (!addr || !IsPointerAlive(addr)) return nullptr;
    auto pAbs = (AbstractClass*)addr;
    auto at = pAbs->WhatAmI();
    if (at != AbstractType::Unit && at != AbstractType::Infantry &&
        at != AbstractType::Building && at != AbstractType::Aircraft) {
        return nullptr;
    }
    return (TechnoClass*)pAbs;
}

// 发出标准操作结果 JSON：{Action, House, Success, ...extra}。
// `extra` 按顺序追加；以交替的字符串形式传入键/值对。
//（通用输出模式的便捷封装。）
static void EmitActionResult(const char* action, int house, bool success)
{
    JsonFile F;
    auto Obj = F.GetObj();
    Obj.AddString("Action",  action);
    Obj.AddInt   ("House",   house);
    Obj.AddBool  ("Success", success);
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  IssueOrder — MEGAMISSION(4)
//    -Mission <name|int>                       （必需）
//    [-Units ["0x...", ...]]                   （默认：会话所属 House 的选中单位）
//    [-Target "0x..."]                          （要攻击的 Techno）
//    [-X <int> -Y <int>]                       （单元格目标）
//
//  每个单位触发一个 MEGAMISSION。如果省略 -Units，则遍历
//  TechnoClass::Array，筛选 IsSelected && GetOwningHouseIndex()==House。
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_IssueOrder(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;  // RequireSession 已设置错误

    // -Mission（必需）
    auto oMission = Context.GetObjectItem("Mission");
    if (!oMission || !oMission.IsTypeString()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    std::string missionName = oMission.GetString();
    int mis = ParseMission(missionName);
    if (mis < -1 || mis > 31) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }

    // 构建可选的 Target Techno TargetClass
    TargetClass targetTC;  // 默认为 null
    bool hasTarget = false;
    DWORD targetAddr = 0;
    auto oTarget = Context.GetObjectItem("Target");
    if (oTarget) {
        if (!ResolveAddress(oTarget, targetAddr)) {
            ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
        }
        auto pT = ResolveLiveTechno(targetAddr);
        if (!pT) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }
        targetTC = TargetClass((AbstractClass*)pT);
        hasTarget = true;
    }

    // 构建可选的单元格目标 TargetClass
    TargetClass destTC;  // 默认为 null
    bool hasDest = false;
    short destX = 0, destY = 0;
    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (oX && oY && oX.IsTypeNumber() && oY.IsTypeNumber()) {
        destX = (short)oX.GetInt();
        destY = (short)oY.GetInt();
        destTC = TargetClass(CellStruct{ destX, destY });
        hasDest = true;
    }

    // 收集要下发指令的单位
    std::vector<TargetClass> units;
    auto oUnits = Context.GetObjectItem("Units");
    if (oUnits && oUnits.IsTypeArray()) {
        size_t n = oUnits.GetArraySize();
        for (size_t i = 0; i < n; i++) {
            auto item = oUnits.GetArrayItem(i);
            DWORD addr;
            if (!ResolveAddress(item, addr)) continue;
            auto pT = ResolveLiveTechno(addr);
            if (!pT) continue;
            units.push_back(TargetClass((AbstractClass*)pT));
        }
    } else {
        // 遍历会话所属 House 的已选中 Techno
        for (auto pTechno : *TechnoClass::Array) {
            if (!pTechno) continue;
            if (pTechno->InLimbo) continue;
            if (!pTechno->IsSelected) continue;
            if (pTechno->GetOwningHouseIndex() != pS->House) continue;
            units.push_back(TargetClass((AbstractClass*)pTechno));
        }
    }

    if (units.empty()) {
        JsonFile F; auto Obj = F.GetObj();
        Obj.AddString("Action",  "IssueOrder");
        Obj.AddString("Mission", missionName);
        Obj.AddInt   ("House",   pS->House);
        Obj.AddBool  ("Success", false);
        Obj.AddString("Reason",  "NoUnits");
        std::string result = Obj.GetText();
        std::cout << result << std::endl;
        ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
        ECDebug::DoNotEcho();
        return;
    }

    // 每个单位触发一个 MEGAMISSION
    int  fired  = 0;
    bool allOk  = true;
    for (const auto& src : units) {
        bool ok = EventClass::AddEvent(EventClass(
            pS->House,
            src,
            (Mission)mis,
            targetTC,
            destTC,
            TargetClass()  // 空 follow
        ));
        if (ok) ++fired;
        else allOk = false;
    }

    JsonFile F; auto Obj = F.GetObj();
    Obj.AddString("Action",  "IssueOrder");
    Obj.AddString("Mission", missionName);
    Obj.AddInt   ("House",   pS->House);
    Obj.AddBool  ("Success", allOk);
    Obj.AddInt   ("Count",   fired);
    Obj.AddInt   ("Total",   (int)units.size());
    if (hasTarget) {
        Obj.AddString("Target", std::format("0x{:08X}", targetAddr));
    }
    if (hasDest) {
        Obj.AddInt("X", (int)destX);
        Obj.AddInt("Y", (int)destY);
    }
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  Deploy / Detonate / Scatter — DEPLOY(9) / DETONATE(10) / SCATTER(7)
//    -Address <"0x...">
//
//  这些事件类型共享 {TargetClass Whom} 数据布局，因此我们使用
//  Target 构造函数：EventClass(houseIndex, EventType, id, rtti)，其中
//  (id, rtti) 来自 TargetClass(AbstractClass*)::m_ID / m_RTTI。
// ════════════════════════════════════════════════════════════════════════════

static void IssueTargetEvent(JsonObject Context, EventType et, const char* actionName)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oAddr = Context.GetObjectItem("Address");
    DWORD addr;
    if (!ResolveAddress(oAddr, addr)) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    auto pT = ResolveLiveTechno(addr);
    if (!pT) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    TargetClass tc((AbstractClass*)pT);
    bool ok = EventClass::AddEvent(EventClass(
        pS->House,
        et,
        tc.m_ID,
        (int)tc.m_RTTI
    ));

    JsonFile F; auto Obj = F.GetObj();
    Obj.AddString("Action",  actionName);
    Obj.AddString("Address", std::format("0x{:08X}", addr));
    Obj.AddInt   ("House",   pS->House);
    Obj.AddBool  ("Success", ok);
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_Deploy  (JsonObject Context) { IssueTargetEvent(Context, EventType::DEPLOY,    "Deploy");   }
void __cdecl IHAgentBackend_Detonate(JsonObject Context) { IssueTargetEvent(Context, EventType::DETONATE,  "Detonate"); }
void __cdecl IHAgentBackend_Scatter (JsonObject Context) { IssueTargetEvent(Context, EventType::SCATTER,   "Scatter");  }

// ════════════════════════════════════════════════════════════════════════════
//  Repair / Sell — REPAIR(21) / SELL(22)
//    -Address <"0x...">
//
//  两者都使用 {TargetClass Whom} 数据布局 — Target 构造函数。
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_Repair(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oAddr = Context.GetObjectItem("Address");
    DWORD addr;
    if (!ResolveAddress(oAddr, addr)) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    auto pT = ResolveLiveTechno(addr);
    if (!pT) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    TargetClass tc((AbstractClass*)pT);
    bool ok = EventClass::AddEvent(EventClass(
        pS->House,
        EventType::REPAIR,
        tc.m_ID,
        (int)tc.m_RTTI
    ));

    JsonFile F; auto Obj = F.GetObj();
    Obj.AddString("Action",  "Repair");
    Obj.AddString("Address", std::format("0x{:08X}", addr));
    Obj.AddInt   ("House",   pS->House);
    Obj.AddBool  ("Success", ok);
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_Sell(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oAddr = Context.GetObjectItem("Address");
    DWORD addr;
    if (!ResolveAddress(oAddr, addr)) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    auto pT = ResolveLiveTechno(addr);
    if (!pT) { ECDebug::ReturnStdError(ERROR_INVALID_ADDRESS); return; }

    TargetClass tc((AbstractClass*)pT);
    bool ok = EventClass::AddEvent(EventClass(
        pS->House,
        EventType::SELL,
        tc.m_ID,
        (int)tc.m_RTTI
    ));

    JsonFile F; auto Obj = F.GetObj();
    Obj.AddString("Action",  "Sell");
    Obj.AddString("Address", std::format("0x{:08X}", addr));
    Obj.AddInt   ("House",   pS->House);
    Obj.AddBool  ("Success", ok);
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  Produce / Suspend / Abandon / SetPrimary
//    PRODUCE(14) / SUSPEND(15) / ABANDON(16) / PRIMARY(17)
//    -TypeID <str>     （例如 "GAPOWR"）
//
//  生产族构造函数：
//    EventClass(houseIndex, EventType, int rtti_id, int heap_id, BOOL is_naval)
// ════════════════════════════════════════════════════════════════════════════

static void IssueProductionEvent(JsonObject Context, EventType et, const char* actionName)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oType = Context.GetObjectItem("TypeID");
    if (!oType || !oType.IsTypeString()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    auto typeStr = oType.GetString();
    TechnoTypeClass* pType = TechnoTypeClass::Find(typeStr.c_str());
    if (!pType) { ECDebug::ReturnStdError(ERROR_NOT_FOUND); return; }

    auto AbsTypeOfType = pType->WhatAmI();
    bool ok = EventClass::AddEvent(EventClass(
        pS->House,
        et,
        (int)AbsTypeOfType,
        pType->GetArrayIndex(),
        pType->Naval
    ));

    JsonFile F; auto Obj = F.GetObj();
    Obj.AddString("Action",  actionName);
    Obj.AddString("TypeID",  pType->ID);
    Obj.AddInt   ("House",   pS->House);
    Obj.AddBool  ("Success", ok);
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_Produce   (JsonObject Context) { IssueProductionEvent(Context, EventType::PRODUCE, "Produce");    }
void __cdecl IHAgentBackend_Suspend   (JsonObject Context) { IssueProductionEvent(Context, EventType::SUSPEND, "Suspend");    }
void __cdecl IHAgentBackend_Abandon   (JsonObject Context) { IssueProductionEvent(Context, EventType::ABANDON, "Abandon");    }
void __cdecl IHAgentBackend_SetPrimary(JsonObject Context) { IssueProductionEvent(Context, EventType::PRIMARY, "SetPrimary"); }

// ════════════════════════════════════════════════════════════════════════════
//  AbandonAll — ABANDON_ALL(46)
//    （除 -SessionID 外无其他参数）
//
//  裸构造函数：EventClass(houseIndex, EventType)。
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_AbandonAll(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    bool ok = EventClass::AddEvent(EventClass(
        pS->House,
        EventType::ABANDON_ALL
    ));

    EmitActionResult("AbandonAll", pS->House, ok);
}

// ════════════════════════════════════════════════════════════════════════════
//  Place — PLACE(11)
//    -TypeID <str>  -X <int>  -Y <int>
//
//  Place 构造函数：
//    EventClass(houseIndex, EventType, AbstractType rttitype, int heapid,
//               int is_naval, const CellStruct& cell)
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_Place(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oType = Context.GetObjectItem("TypeID");
    if (!oType || !oType.IsTypeString()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }

    auto typeStr = oType.GetString();
    TechnoTypeClass* pType = TechnoTypeClass::Find(typeStr.c_str());
    if (!pType) { ECDebug::ReturnStdError(ERROR_NOT_FOUND); return; }

    short X = (short)oX.GetInt();
    short Y = (short)oY.GetInt();

    bool ok = EventClass::AddEvent(EventClass(
        pS->House,
        EventType::PLACE,
        pType->WhatAmI(),
        pType->GetArrayIndex(),
        pType->Naval,
        CellStruct{ X, Y }
    ));

    JsonFile F; auto Obj = F.GetObj();
    Obj.AddString("Action",  "Place");
    Obj.AddString("TypeID",  pType->ID);
    Obj.AddInt   ("House",   pS->House);
    Obj.AddInt   ("X",       (int)X);
    Obj.AddInt   ("Y",       (int)Y);
    Obj.AddBool  ("Success", ok);
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  SpecialPlace — SPECIAL_PLACE(18)
//    -TypeID <str>  -X <int>  -Y <int>
//
//  SpecialPlace 构造函数：
//    EventClass(houseIndex, EventType, int id, const CellStruct& cell)
//
//  注意：对于围墙/超级武器，"id" 是 BuildingTypeClass 数组索引
//  (heap_id)。我们使用 pType->GetArrayIndex() 获取。
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_SpecialPlace(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oType = Context.GetObjectItem("TypeID");
    if (!oType || !oType.IsTypeString()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }

    auto typeStr = oType.GetString();
    TechnoTypeClass* pType = TechnoTypeClass::Find(typeStr.c_str());
    if (!pType) { ECDebug::ReturnStdError(ERROR_NOT_FOUND); return; }

    short X = (short)oX.GetInt();
    short Y = (short)oY.GetInt();

    bool ok = EventClass::AddEvent(EventClass(
        pS->House,
        EventType::SPECIAL_PLACE,
        pType->GetArrayIndex(),
        CellStruct{ X, Y }
    ));

    JsonFile F; auto Obj = F.GetObj();
    Obj.AddString("Action",  "SpecialPlace");
    Obj.AddString("TypeID",  pType->ID);
    Obj.AddInt   ("House",   pS->House);
    Obj.AddInt   ("X",       (int)X);
    Obj.AddInt   ("Y",       (int)Y);
    Obj.AddBool  ("Success", ok);
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  SellCell — SELLCELL(23)
//    -X <int>  -Y <int>
//
//  SellCell 构造函数：
//    EventClass(houseIndex, EventType, const CellStruct& cell)
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_SellCell(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oX = Context.GetObjectItem("X"), oY = Context.GetObjectItem("Y");
    if (!oX || !oX.IsTypeNumber() || !oY || !oY.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }

    short X = (short)oX.GetInt();
    short Y = (short)oY.GetInt();

    bool ok = EventClass::AddEvent(EventClass(
        pS->House,
        EventType::SELLCELL,
        CellStruct{ X, Y }
    ));

    JsonFile F; auto Obj = F.GetObj();
    Obj.AddString("Action",  "SellCell");
    Obj.AddInt   ("House",   pS->House);
    Obj.AddInt   ("X",       (int)X);
    Obj.AddInt   ("Y",       (int)Y);
    Obj.AddBool  ("Success", ok);
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  Ally — ALLY(3)
//    -TargetHouse <int>     （要结盟的 House 的 HouseClass::ArrayIndex）
//
//  ALLY 事件数据布局为 {int HouseID}，与 TargetClass.m_ID 的前 4 字节重叠。
//  因此我们使用 Target 构造函数，id=targetHouseIndex，rtti=0
//  （ALLY 只读取第一个 int）。
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_Ally(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    auto oTH = Context.GetObjectItem("TargetHouse");
    if (!oTH || !oTH.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    int targetHouse = oTH.GetInt();
    if (targetHouse < 0 || targetHouse >= HouseClass::Array->Count) {
        ECDebug::ReturnStdError(ERROR_INVALID_INDEX); return;
    }

    bool ok = EventClass::AddEvent(EventClass(
        pS->House,
        EventType::ALLY,
        targetHouse,
        0
    ));

    JsonFile F; auto Obj = F.GetObj();
    Obj.AddString("Action",      "Ally");
    Obj.AddInt   ("House",        pS->House);
    Obj.AddInt   ("TargetHouse",  targetHouse);
    Obj.AddBool  ("Success",     ok);
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  Cheer — ALLCHEER(45)
//    （除 -SessionID 外无其他参数）
//
//  裸构造函数：EventClass(houseIndex, EventType)。
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_Cheer(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    bool ok = EventClass::AddEvent(EventClass(
        pS->House,
        EventType::ALLCHEER
    ));

    EmitActionResult("Cheer", pS->House, ok);
}

// ════════════════════════════════════════════════════════════════════════════
//  AIProduce (Cheat-only) — 直接写入 House->ProducingXxxTypeIndex
//    -TypeID <str>
//
//  AI 生产路径：BuildingClass::AI 在下一帧收集索引并调用 BeginProduction。
//  不涉及 EventClass。仅允许 Cheat=true 的会话使用。
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_AIProduce(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    if (!pS->Cheat) {
        ECDebug::ReturnStdError(ERROR_ACCESS_DENIED);
        return;
    }

    auto oType = Context.GetObjectItem("TypeID");
    if (!oType || !oType.IsTypeString()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
    }
    auto typeStr = oType.GetString();
    TechnoTypeClass* pType = TechnoTypeClass::Find(typeStr.c_str());
    if (!pType) { ECDebug::ReturnStdError(ERROR_NOT_FOUND); return; }

    auto pH = HouseClass::Array->Items[pS->House];
    if (!pH) { ECDebug::ReturnStdError(ERROR_INVALID_DATA); return; }

    auto at = pType->WhatAmI();
    int idx = pType->GetArrayIndex();

    bool ok = true;
    if      (at == AbstractType::BuildingType)  pH->ProducingBuildingTypeIndex  = idx;
    else if (at == AbstractType::UnitType)      pH->ProducingUnitTypeIndex      = idx;
    else if (at == AbstractType::InfantryType)  pH->ProducingInfantryTypeIndex  = idx;
    else if (at == AbstractType::AircraftType)  pH->ProducingAircraftTypeIndex  = idx;
    else { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }

    JsonFile F; auto Obj = F.GetObj();
    Obj.AddString("Action",  "AIProduce");
    Obj.AddString("TypeID",  pType->ID);
    Obj.AddInt   ("House",   pS->House);
    Obj.AddBool  ("Success", ok);
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}

// ════════════════════════════════════════════════════════════════════════════
//  AIStopProduction (Cheat-only) — 清除 House->ProducingXxxTypeIndex
//    -WhatAmI <UnitType|BuildingType|InfantryType|AircraftType>   或   -All
// ════════════════════════════════════════════════════════════════════════════

void __cdecl IHAgentBackend_AIStopProduction(JsonObject Context)
{
    auto* pS = RequireSession(Context);
    if (!pS) return;

    if (!pS->Cheat) {
        ECDebug::ReturnStdError(ERROR_ACCESS_DENIED);
        return;
    }

    auto pH = HouseClass::Array->Items[pS->House];
    if (!pH) { ECDebug::ReturnStdError(ERROR_INVALID_DATA); return; }

    bool clearAll = false;
    auto oAll = Context.GetObjectItem("All");
    if (oAll && oAll.IsTypeBool()) clearAll = oAll.GetBool();

    std::string cleared;
    bool ok = true;

    if (clearAll) {
        pH->ProducingBuildingTypeIndex = -1;
        pH->ProducingUnitTypeIndex     = -1;
        pH->ProducingInfantryTypeIndex = -1;
        pH->ProducingAircraftTypeIndex = -1;
        cleared = "all";
    } else {
        auto oWA = Context.GetObjectItem("WhatAmI");
        if (!oWA || !oWA.IsTypeString()) {
            ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return;
        }
        auto swa = oWA.GetString();
        if      (swa == "BuildingType"  || swa == "Building")  { pH->ProducingBuildingTypeIndex  = -1; cleared = "BuildingType";  }
        else if (swa == "UnitType"      || swa == "Unit")      { pH->ProducingUnitTypeIndex      = -1; cleared = "UnitType";      }
        else if (swa == "InfantryType"  || swa == "Infantry")  { pH->ProducingInfantryTypeIndex  = -1; cleared = "InfantryType";  }
        else if (swa == "AircraftType" || swa == "Aircraft")  { pH->ProducingAircraftTypeIndex  = -1; cleared = "AircraftType";  }
        else { ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS); return; }
    }

    JsonFile F; auto Obj = F.GetObj();
    Obj.AddString("Action",   "AIStopProduction");
    Obj.AddInt   ("House",    pS->House);
    Obj.AddString("Cleared",   cleared);
    Obj.AddBool  ("Success",   ok);
    std::string result = Obj.GetText();
    std::cout << result << std::endl;
    ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    ECDebug::DoNotEcho();
}
