// SessionLifecycle.h
// 作用：会话生命周期管理。封装 BeginSession/EndSession/Heartbeat，
//       维护当前 SessionHandle，提供 session_id() 给各 Agent 调后端时使用。
//       心跳由 Coordinator 的定时器驱动（默认 10s 一次，低于后端 30s 超时）。

#pragma once

#include "BackendInterface.h"
#include <optional>
#include <string>

namespace ihagent {

class SessionLifecycle {
public:
    // 构造，注入后端客户端
    explicit SessionLifecycle(IBackendClient* backend);

    // 建立会话，接管指定 House。成功后 session_id() 可用
    bool start(HouseId house, bool cheat, int timeout_sec);

    // 主动结束会话
    void stop();

    // 心跳：由外部定时器周期调用，刷新会话
    bool heartbeat();

    // 当前会话句柄（未建立时返回 std::nullopt）
    const std::optional<SessionHandle>& handle() const { return handle_; }

    // 便捷获取当前 session_id（未建立时返回空字符串）
    std::string session_id() const;

    // 当前接管 House（未建立时返回 -1）
    HouseId house() const;

    // 是否处于活动会话
    bool active() const { return handle_.has_value(); }

private:
    IBackendClient* backend_ = nullptr;
    std::optional<SessionHandle> handle_;
    int64_t last_heartbeat_ms_ = 0;
};

} // namespace ihagent