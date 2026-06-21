#include "IHVerify.h"
#include "Pointers.h"
#include <format>
#include <iostream>
#include <mutex>
#include <unordered_set>
#include <YRPP.h>

// ── Mark storage ──
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

// ── Watch storage ──
struct WatchEntry {
    DWORD address;
    std::string callbackLib;
    std::string callbackMethod;
    bool removedOnly;
};
static std::vector<WatchEntry> _Watches;
static std::mutex _WatchMutex;

// ── PointerExpiredProcess — auto cleanup on object death ──
void CALLBACK PointerExpiredProcess(AbstractClass* pDyingObj, bool bRemoved)
{
    DWORD addr = (DWORD)pDyingObj;

    // 1) auto-mark cleanup
    if (bRemoved) UnmarkObject(addr);

    // 2) fire watch callbacks
    std::lock_guard<std::mutex> lk(_WatchMutex);
    for (auto it = _Watches.begin(); it != _Watches.end(); ) {
        if (it->address == addr) {
            if (!it->removedOnly || bRemoved) {
                auto* root = cJSON_CreateObject();
                char buf[20];
                sprintf_s(buf, "0x%08X", addr);
                cJSON_AddStringToObject(root, "Pointer", buf);
                cJSON_AddNumberToObject(root, "Removed", bRemoved ? 1 : 0);
                Ext::PostAsyncRemoteCall(
                    it->callbackLib.c_str(),
                    it->callbackMethod.c_str(),
                    2147483647,
                    JsonObject(root)
                );
                it = _Watches.erase(it);
                continue;
            }
        }
        ++it;
    }
}

// ── EC Actions ──

void __cdecl IHVerify_WatchPointer(JsonObject Context)
{
    auto oAddr = Context.GetObjectItem("Address");
    if (!oAddr || !oAddr.IsTypeNumber()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    DWORD addr = (DWORD)oAddr.GetInt();

    auto oLib = Context.GetObjectItem("CallbackLib");
    if (!oLib || !oLib.IsTypeString()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    std::string cbLib = oLib.GetString();

    auto oMethod = Context.GetObjectItem("CallbackMethod");
    if (!oMethod || !oMethod.IsTypeString()) {
        ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
        return;
    }
    std::string cbMethod = oMethod.GetString();

    bool removedOnly = true;
    auto oRO = Context.GetObjectItem("RemovedOnly");
    if (oRO && oRO.IsTypeBool()) removedOnly = oRO.GetBool();

    std::lock_guard<std::mutex> lk(_WatchMutex);
    _Watches.push_back({ addr, cbLib, cbMethod, removedOnly });

    std::cout << std::format("WatchPointer 0x{:08X} callback={}.{} removedOnly={}",
        addr, cbLib, cbMethod, removedOnly ? "true" : "false") << std::endl;
    ECDebug::DoNotEcho();
}

void __cdecl IHVerify_FrameUpdate(JsonObject)
{
    // Watches are fired directly from PointerExpiredProcess via PostAsyncRemoteCall.
    // This handler exists to keep IHVerify registered in the IHCore::FrameUpdate broadcast.
}

void __cdecl IHVerify_MarkObject(JsonObject Context)
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

void __cdecl IHVerify_UnmarkObject(JsonObject Context)
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

void __cdecl IHVerify_ListMarks(JsonObject Context)
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

void __cdecl IHVerify_ClearMarks(JsonObject Context)
{
    std::lock_guard<std::mutex> lk(_MarksMutex);
    size_t n = _Marks.size();
    _Marks.clear();
    std::cout << std::format("Cleared {} mark(s)", n) << std::endl;
    ECDebug::DoNotEcho();
}

// ── ACP (Address Comment Provider) ──

// Move string to IH-managed heap (same pattern as IHInspector)
static const char* MoveToIHCore(const std::string& S)
{
    char* p = (char*)IH::Malloc(S.size() + 1);
    strncpy(p, S.c_str(), S.size() + 1);
    return p;
}

const char* __cdecl MarkerACP(const AddressCommentInfo& AddrInfo)
{
    if (!AddrInfo.CanRead) return nullptr;
    DWORD data = *((DWORD*)AddrInfo.Addr);
    if (data == 0) return nullptr;

    std::string label;
    if (!IsObjectMarked(data, label)) return nullptr;

    std::string msg;
    if (AddrInfo.FirstAddrOfReport)
    {
        msg = std::format("--- IHVerify Marked Objects ({}) ---\n", _Marks.size());
    }
    msg += std::format(" [IHVerify: \"{}\"]", label);
    return MoveToIHCore(msg);
}
