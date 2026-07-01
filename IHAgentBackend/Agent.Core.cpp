#include "AgentBackend.h"
#include "Pointers.h"
#include <format>
#include <unordered_map>
#include <string>
#include <atomic>
#include <CRT.h>
#include <YRPP.h>
#include <ExtJson.h>

// 仅在游戏启动后可用 — 在 IHCore::Reset 向所有 DLL 及远程组件广播时设置
std::atomic<bool> _GameStarted{false};
static std::atomic<bool> _HasInit{false};
void IHAgentBackend_Init() { _GameStarted.store(false, std::memory_order_relaxed); }

// 由 ECInitLibrary 的 OrderedInit 回调调用 — 检查版本并设置监听器
void IHAgentBackend_OrderedInit()
{
    auto ihCore = Ext::GetLib("IHCore");
    if (ihCore.Version() < 514)
    {
        MessageBoxA(nullptr,
            "IHCore version < 514, IHAgentBackend cannot run.\nUpdate IHCore to 0.5.14 or later.",
            "IHAgentBackend — Fatal Error", MB_OK | MB_ICONERROR);
        CRT::exit(0);
    }
    ECListener::Listen_PointerExpired(PointerExpiredProcess);
    RegisterAddressCommentProvider("IHAgentBackend", MarkerACP);
    _HasInit.store(true, std::memory_order_relaxed);
}

// 游戏启动时通过 IHCore::Reset 广播调用 — 在所有 DLL 及远程组件上触发
void __cdecl IHAgentBackend_OnGameReset(JsonObject)
{
    _GameStarted.store(true, std::memory_order_release);
    // v2: 清除所有 Session — 前端必须在游戏重置后重新调用 BeginSession
    Session_OnGameReset();
}

// 状态变量缓存（返回的指针必须保持有效）
static std::unordered_map<std::string, std::string> _StatusVars;

static UTF8_CString SetStatusVar(const std::string& key, std::string&& val)
{
    auto& slot = _StatusVars[key];
    slot = std::move(val);
    return conv slot.c_str();
}

// ------------------------------------------------------------------
//  StatusRouter：<Status.Frame>、<Status.PlayerCount>、
//                <Status.House.0.Money> 等
// ------------------------------------------------------------------
UTF8_CString __cdecl StatusRouter(const char* Key)
{
    if (!_GameStarted.load(std::memory_order_acquire)) return nullptr;
    std::string_view k(Key);

    // 全局状态
    if (k == "Frame")
        return SetStatusVar("Frame", std::to_string(Unsorted::CurrentFrame));

    if (k == "PlayerCount")
        return SetStatusVar("PlayerCount", std::to_string(HouseClass::Array->Count));

    if (k == "CurrentPlayerID")
    {
        auto p = HouseClass::CurrentPlayer();
        return p ? SetStatusVar("CurrentPlayerID", std::to_string(p->ArrayIndex)) : nullptr;
    }

    if (k == "ObserverID")
    {
        auto p = HouseClass::CurrentPlayer();
        if (p && p->IsObserver()) return SetStatusVar("ObserverID", std::to_string(p->ArrayIndex));
        return nullptr;
    }

    auto& pSess = SessionClass::Instance;
    if (k == "Difficulty")
        return SetStatusVar("Difficulty", std::to_string((int)pSess->Config.AIDifficulty));

    // House.N.字段
    if (k.starts_with("House."))
    {
        auto rest = std::string(k.substr(6));
        auto dot = rest.find('.');
        if (dot == std::string::npos) return nullptr;
        auto idxStr = rest.substr(0, dot);
        auto field  = rest.substr(dot + 1);
        int idx = 0;
        try { idx = std::stoi(idxStr); } catch (...) { return nullptr; }
        if (idx < 0 || idx >= (*HouseClass::Array).Count) return nullptr;
        auto pHouse = (*HouseClass::Array)[idx];
        if (!pHouse) return nullptr;

        auto sk = std::string(k);

        if (field == "Name")
            return SetStatusVar(sk, pHouse->PlainName);
        if (field == "Money")
            return SetStatusVar(sk, std::to_string(pHouse->Available_Money()));
        if (field == "OwnedUnits")
            return SetStatusVar(sk, std::to_string(pHouse->OwnedUnits));
        if (field == "OwnedBuildings")
            return SetStatusVar(sk, std::to_string(pHouse->OwnedBuildings));
        if (field == "OwnedInfantry")
            return SetStatusVar(sk, std::to_string(pHouse->OwnedInfantry));
        if (field == "OwnedAircraft")
            return SetStatusVar(sk, std::to_string(pHouse->OwnedAircraft));
        if (field == "Player")
            return SetStatusVar(sk, pHouse->IsCurrentPlayer() ? "Yes" : "No");
        if (field == "Faction")
            return SetStatusVar(sk, pHouse->Type ? pHouse->Type->ID : "(unknown)");
        if (field == "Country" && pHouse->Type)
        {
            auto pc = pHouse->Type->ParentCountry;
            return pc ? SetStatusVar(sk, pc.operator const char*()) : nullptr;
        }
        if (field == "Color")
            return SetStatusVar(sk, std::format("{},{},{}", (int)pHouse->Color.R, (int)pHouse->Color.G, (int)pHouse->Color.B));
        if (field == "LaserColor")
            return SetStatusVar(sk, std::format("{},{},{}", (int)pHouse->LaserColor.R, (int)pHouse->LaserColor.G, (int)pHouse->LaserColor.B));
        return nullptr;
    }

    return nullptr;
}

// ════════════════════════════════════════════════════════════════════════════
// 内部辅助函数（保留供 Agent.Actions.cpp / Agent.Coord.cpp 使用）
//
// 这些函数遍历全局引擎数组，验证 DWORD 地址是否仍然指向存活对象。
// Phase B/C 代码通过 AgentBackend.h 中的声明使用它们。
// ════════════════════════════════════════════════════════════════════════════

bool IsPointerAlive(DWORD addr)
{
    if (!addr) return false;
    for (auto pAbs : *AbstractClass::Array)
    {
        if ((DWORD)pAbs == addr)
            return true;
    }
    return false;
}

bool IsFactoryAlive(DWORD addr)
{
    if (!addr) return false;
    for (auto pFact : *FactoryClass::Array)
    {
        if ((DWORD)pFact == addr)
            return true;
    }
    return false;
}
