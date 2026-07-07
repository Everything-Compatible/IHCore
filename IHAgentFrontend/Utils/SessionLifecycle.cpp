// SessionLifecycle.cpp
// 作用：会话生命周期管理实现。

#include "pch.h"
#include "SessionLifecycle.h"
#include "Logger.h"

namespace ihagent {

// 构造，注入后端客户端
SessionLifecycle::SessionLifecycle(IBackendClient* backend) : backend_(backend) {}

// 建立会话：调 BeginSession，成功则保存句柄
bool SessionLifecycle::start(HouseId house, bool cheat, int timeout_sec) {
    if (!backend_) {
        IH_LOG_ERROR("SessionLifecycle", "no backend client");
        return false;
    }
    if (handle_) {
        IH_LOG_WARN("SessionLifecycle", "session already active, stop first");
        return false;
    }
    auto h = backend_->begin_session(house, cheat, timeout_sec);
    if (!h.valid()) {
        IH_LOG_ERROR("SessionLifecycle", "BeginSession failed for house " + std::to_string(house));
        return false;
    }
    handle_ = h;
    IH_LOG_INFO("SessionLifecycle", "session started: " + h.session_id + " house=" + std::to_string(h.house));
    return true;
}

// 主动结束会话：调 EndSession，清空句柄
void SessionLifecycle::stop() {
    if (!handle_) return;
    if (backend_) backend_->end_session(handle_->session_id);
    IH_LOG_INFO("SessionLifecycle", "session stopped: " + handle_->session_id);
    handle_.reset();
}

// 心跳：调 SessionHeartbeat，失败时（可能已被后端超时释放）清空句柄
bool SessionLifecycle::heartbeat() {
    if (!handle_ || !backend_) return false;
    if (!backend_->session_heartbeat(handle_->session_id)) {
        IH_LOG_WARN("SessionLifecycle", "heartbeat failed, session likely expired");
        handle_.reset();
        return false;
    }
    return true;
}

// 当前 session_id（未建立返回空串）
std::string SessionLifecycle::session_id() const {
    return handle_ ? handle_->session_id : std::string{};
}

// 当前接管 House（未建立返回 -1）
HouseId SessionLifecycle::house() const {
    return handle_ ? handle_->house : -1;
}

} // namespace ihagent