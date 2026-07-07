// Coordinator.h
// 作用：顶层协调器，驱动三层 Agent。负责：
//   1) 会话生命周期（BeginSession/EndSession/Heartbeat）；
//   2) 把玩家全部指令（显式+隐式）喂给调度 Agent（唯一入口）；
//   3) 接收战略 Agent 的 StrategyDirective 回流；
//   4) 把 DispatchPlan 中的 TeamTask 下发给各介入 Agent；
//   5) 收集介入 Agent 的执行反馈回流到调度/战略；
//   6) 按 5s/30s 定时器驱动各层；
//   7) 路由后端回调（OnBuildingReady/OnUnitDestroyed/OnUnderAttack/OnChatMessage/OnBeacon）。
//   见 §1 整体架构与数据流。

#pragma once

#include "StrategicAgent/StrategicAgent.h"
#include "DispatchAgent/DispatchAgent.h"
#include "InterventionAgent/InterventionAgent.h"
#include "Utils/BackendInterface.h"
#include "Utils/LLMClient.h"
#include "Utils/SessionLifecycle.h"
#include "Utils/BackendCallbacks.h"
#include <memory>
#include <unordered_map>
#include <chrono>

namespace ihagent {

class Coordinator : public IBackendCallbacks {
public:
    // 构造：注入后端客户端与 LLM 客户端，内部创建三层 Agent 与会话管理
    Coordinator(IBackendClient* backend, ILLMClient* llm);

    // 建立会话，接管指定 House。成功后各 Agent 才能工作
    bool start_session(HouseId house, bool cheat, int timeout_sec);

    // 结束会话
    void stop_session();

    // 主 tick：由外部驱动（游戏帧或定时任务）。每次执行调度+介入一轮
    void tick();

    // 玩家指令入口（唯一）：调用后立即触发调度 Agent 唤醒
    void submit_player_command(PlayerCommand cmd);

    // 每 30s 触发战略 Agent 的定时器回调（外部用 timer 调用）
    void tick_strategic();

    // 心跳：外部定时器周期调用（默认 10s 一次，低于后端 30s 超时）
    bool heartbeat();

    // ── IBackendCallbacks 实现：路由后端回调 ──
    void on_session_closed(const CBSessionClosed& cb) override;
    void on_building_ready(const CBBuildingReady& cb) override;
    void on_unit_destroyed(const CBUnitDestroyed& cb) override;
    void on_under_attack(const CBUnderAttack& cb) override;
    void on_chat_message(const CBChatMessage& cb) override;
    void on_beacon(const CBBeacon& cb) override;

    // 当前会话是否活动
    bool active() const { return session_.active(); }

private:
    // 内部：把 DispatchPlan 下发到各介入 Agent
    void dispatch_tasks(const DispatchPlan& plan);

    // 构造 StrategicInput / DispatchInput / InterventionInput 的公共字段
    void fill_common_session_fields();

    IBackendClient* backend_ = nullptr;
    ILLMClient* llm_ = nullptr;
    SessionLifecycle session_;
    std::unique_ptr<StrategicAgent> strategic_;
    std::unique_ptr<DispatchAgent> dispatch_;
    std::unique_ptr<InterventionAgent> intervention_;

    // 每个 Team 上一次下发的任务（便于持续喂给介入 Agent）
    std::unordered_map<TeamId, TeamTask, TeamIdHash> last_tasks_;

    // 缓存的会话公共字段
    std::string session_id_;
    HouseId self_house_ = -1;
    HouseId enemy_house_ = -1;

    // 上次心跳与定时唤醒时间
    int64_t last_dispatch_ms_ = 0;
    int64_t last_strategic_ms_ = 0;
};

} // namespace ihagent