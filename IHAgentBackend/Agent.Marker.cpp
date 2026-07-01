#include "AgentBackend.h"
#include "Pointers.h"
#include <format>
#include <iostream>
#include <mutex>
#include <unordered_set>
#include <YRPP.h>

// ── 标记存储 ──
static std::unordered_map<DWORD, std::u8string> _Marks;
static std::mutex _MarksMutex;

void MarkObject(DWORD addr, const std::u8string& label)
{
    std::lock_guard<std::mutex> lk(_MarksMutex);
    _Marks[addr] = label;
}

void UnmarkObject(DWORD addr)
{
    std::lock_guard<std::mutex> lk(_MarksMutex);
    _Marks.erase(addr);
}

bool IsObjectMarked(DWORD addr, std::string& outLabel)
{
    std::lock_guard<std::mutex> lk(_MarksMutex);
    auto it = _Marks.find(addr);
    if (it != _Marks.end())
    {
        outLabel = ~it->second;
        return true;
    }
    return false;
}

// ── PointerExpiredProcess — 对象销毁时自动清理 ──
// v2: 同时为属于某个 Session 所属 House 的 TechnoClass 对象分发
//     OnUnitDestroyed 事件。vtable 安全：仅在
//     IsPointerAlive(addr) 确认对象仍在 AbstractClass::Array 中之后，
//     才能访问 pDyingObj 的 vtable。根据 Pointers.cpp:13 的警告，
//     不要对已释放的对象调用 vtable 方法。
void CALLBACK PointerExpiredProcess(AbstractClass* pDyingObj, bool bRemoved)
{
    if (!pDyingObj) return;
    DWORD addr = (DWORD)pDyingObj;

    // 1) auto-mark cleanup (preserve v1 behavior)
    if (bRemoved) UnmarkObject(addr);

    // 2) v2 OnUnitDestroyed dispatch — only for final removal (not temp
    //    limbo transitions like entering a transport)
    if (!bRemoved) return;

    // vtable safety: if the object is no longer in AbstractClass::Array,
    // its vtable may have been freed — skip dispatch
    if (!IsPointerAlive(addr)) return;

    auto wa = pDyingObj->WhatAmI();
    if (wa != AbstractType::Unit && wa != AbstractType::Infantry &&
        wa != AbstractType::Building && wa != AbstractType::Aircraft) {
        return;
    }

    auto pT = static_cast<TechnoClass*>(pDyingObj);
    auto pH = pT->GetOwningHouse();
    if (!pH) return;
    int houseIdx = pH->ArrayIndex;

    // 查找控制该 House 的 Session
    AgentSession* pS = FindSessionByHouse(houseIdx);
    if (!pS) return;

    // 在分发前查找类型 ID — pS 可能因并发的 Session 删除而失效
    // session erase, so we copy nothing here and let DispatchUnitDestroyed
    // handle field copying under its own mutex discipline.
    const char* typeId = "";
    auto* pType = pT->GetTechnoType();
    if (pType) typeId = pType->ID;

    DispatchUnitDestroyed(pS, typeId, addr);
}

// ── EC 动作 ──

void __cdecl IHAgentBackend_FrameUpdate(JsonObject)
{
    // v2: session timeout detection + viewport sampling (Phase A)
    Session_FrameUpdate();

    // v2: OnBuildingReady auto-dispatch (Phase D)
    Session_BuildingReadyPoll();

    // Marker 系统不需要每帧轮询。
    // 旧的每个建筑的 watch 轮询已在 Phase D 中移除，
    // 改为 Session 级别的 OnBuildingReady 自动分发（Session_BuildingReadyPoll）。
}

void __cdecl IHAgentBackend_MarkObject(JsonObject Context)
{
    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber())
    {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    DWORD addr = (DWORD)oAddr.GetInt();

    auto oLabel = Context.GetObjectItem("Label");
    if (!oLabel || !oLabel.IsTypeString())
    {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    std::u8string label = ~oLabel.GetString();

    MarkObject(addr, label);
    std::cout << std::format("Marked 0x{:08X} as \"{}\"", (int)addr, ~label) << std::endl;
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_UnmarkObject(JsonObject Context)
{
    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber())
    {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    DWORD addr = (DWORD)oAddr.GetInt();

    UnmarkObject(addr);
    std::cout << std::format("Unmarked 0x{:08X}", (int)addr) << std::endl;
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_ListMarks(JsonObject Context)
{
    std::lock_guard<std::mutex> lk(_MarksMutex);
    if (_Marks.empty())
    {
        std::cout << "(no marked objects)" << std::endl;
        ECDebug::ReturnString({ (const char8_t*)u8"[]", 2 });
    }
    else
    {
        std::string result = "[";
        bool first = true;
        for (auto& [addr, label] : _Marks)
        {
            if (!first) result += ",";
            first = false;
            result += std::format(R"({{"addr":"0x{:08X}","label":"{}"}})", (int)addr, ~label);
        }
        result += "]";
        std::cout << result << std::endl;
        ECDebug::ReturnString({ (const char8_t*)result.c_str(), result.size() });
    }
    ECDebug::DoNotEcho();
}

void __cdecl IHAgentBackend_ClearMarks(JsonObject Context)
{
    std::lock_guard<std::mutex> lk(_MarksMutex);
    size_t n = _Marks.size();
    _Marks.clear();
    std::cout << std::format("Cleared {} mark(s)", n) << std::endl;
    ECDebug::DoNotEcho();
}

// ── ACP (Address Comment Provider) ──

// 将字符串移动到 IH 管理的堆上（与 IHInspector 相同模式）
static const char* MoveToIHCore(const std::string& S)
{
    char* p = (char*)IH::Malloc(S.size() + 1);
    strncpy(p, S.c_str(), S.size() + 1);
    return p;
}

const char* __cdecl MarkerACP(const AddressCommentInfo& AddrInfo)
{
    if (!AddrInfo.CanRead) return "";
    DWORD data = *((DWORD*)AddrInfo.Addr);
    if (data == 0) return "";

    std::string label;
    if (!IsObjectMarked(data, label)) return "";

    std::string msg;
    if (AddrInfo.FirstAddrOfReport)
    {
        msg = std::format("--- IHAgentBackend Marked Objects ({}) ---\n", _Marks.size());
    }
    msg += std::format(" [IHAgentBackend: \"{}\"]", label);
    return MoveToIHCore(msg);
}
