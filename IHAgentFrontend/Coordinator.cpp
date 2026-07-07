// Coordinator.cpp
// 作用：顶层协调器实现——会话生命周期、玩家指令入口、定时驱动、回调路由。

#include "pch.h"
#include "Coordinator.h"
#include "Utils/Logger.h"

namespace ihagent {

// 构造：注入后端与 LLM 客户端，创建三层 Agent 与会话管理
Coordinator::Coordinator(IBackendClient* backend, ILLMClient* llm)
    : backend_(backend), llm_(llm), session_(backend) {
    strategic_ = std::make_unique<StrategicAgent>(backend, llm);
    dispatch_ = std::make_unique<DispatchAgent>(backend, llm, strategic_.get());
    intervention_ = std::make_unique<InterventionAgent>(backend, llm);
}

// 建立会话：调 BeginSession，缓存 session_id / self_house
bool Coordinator::start_session(HouseId house, bool cheat, int timeout_sec) {
    if (!session_.start(house, cheat, timeout_sec)) return false;
    fill_common_session_fields();
    // TODO: enemy_house 需要从 GetHouseInfo 或场景配置获取，暂设为 -1
    enemy_house_ = -1;
    return true;
}

// 结束会话：调 EndSession
void Coordinator::stop_session() {
    session_.stop();
    session_id_.clear();
    self_house_ = -1;
    enemy_house_ = -1;
    last_tasks_.clear();
}

// 主 tick：5s 调度周期
void Coordinator::tick() {
    if (!session_.active()) return;
    fill_common_session_fields();

    // 收集介入反馈（用于调度与战略上下文）
    std::vector<ExecutionFeedback> feedbacks;
    for (const auto& [tid, task] : last_tasks_) {
        (void)task;
        // TODO: v2 没有主动 read_feedback 接口，反馈由 OnUnitDestroyed/OnUnderAttack 回调聚合
        //       此处留空，反馈通过 on_unit_destroyed/on_under_attack 回填到 contexts
        (void)tid;
    }

    // 唤醒调度 Agent
    DispatchInput din;
    din.session_id = session_id_;
    din.self_house = self_house_;
    din.enemy_house = enemy_house_;
    din.intervention_feedbacks = std::move(feedbacks);
    if (auto plan = dispatch_->wake(din)) {
        dispatch_tasks(*plan);
    }
}

// 玩家指令入口（唯一）：立即唤醒调度 Agent
void Coordinator::submit_player_command(PlayerCommand cmd) {
    if (!session_.active()) {
        IH_LOG_WARN("Coordinator", "player command dropped: no active session");
        return;
    }
    fill_common_session_fields();
    IH_LOG_INFO("Coordinator", "player command: " + cmd.text);

    DispatchInput din;
    din.session_id = session_id_;
    din.self_house = self_house_;
    din.enemy_house = enemy_house_;
    din.player_command = std::move(cmd);
    if (auto plan = dispatch_->wake(din)) {
        // 转发战略诉求给战略 Agent（非紧急先转发等回流）
        if (plan->forward.active && strategic_) {
            StrategicInput sin;
            sin.session_id = session_id_;
            sin.self_house = self_house_;
            sin.enemy_house = enemy_house_;
            sin.forward = plan->forward;
            strategic_->wake(sin);
        }
        dispatch_tasks(*plan);
    }
}

// 每 30s 战略定时
void Coordinator::tick_strategic() {
    if (!session_.active() || !strategic_) return;
    fill_common_session_fields();
    StrategicInput sin;
    sin.session_id = session_id_;
    sin.self_house = self_house_;
    sin.enemy_house = enemy_house_;
    strategic_->wake(sin);
}

// 心跳：刷新会话
bool Coordinator::heartbeat() {
    return session_.heartbeat();
}

// ── IBackendCallbacks 实现 ──

// 会话关闭：清空本地状态
void Coordinator::on_session_closed(const CBSessionClosed& cb) {
    IH_LOG_WARN("Coordinator", "session closed: " + cb.session_id + " reason=" + cb.reason);
    session_id_.clear();
    self_house_ = -1;
    enemy_house_ = -1;
    last_tasks_.clear();
    // TODO: 通知各 Agent 中止当前任务
}

// 建筑完成：通知调度 Agent 产线状态变化
void Coordinator::on_building_ready(const CBBuildingReady& cb) {
    IH_LOG_INFO("Coordinator", "building ready: " + cb.type_id + " @ " + cb.address);
    // TODO: 把这个事件回填到 DispatchContext 的产线状态，供下一轮拆解参考
}

// 单位被毁：更新对应 Team 的损失统计
void Coordinator::on_unit_destroyed(const CBUnitDestroyed& cb) {
    IH_LOG_INFO("Coordinator", "unit destroyed: " + cb.type_id + " @ " + cb.address);
    // TODO: 找到该 address 所属 Team，更新 InterventionContext 的损失计数
    //       v2 没有主动 read_feedback，损失统计完全靠此回调
}

// 受到攻击：可能触发紧急路径
void Coordinator::on_under_attack(const CBUnderAttack& cb) {
    IH_LOG_INFO("Coordinator", "under attack: victim=" + cb.victim_address
                + " attacker=" + cb.attacker_address + " dmg=" + std::to_string(cb.damage));
    // TODO: 若基地附近受攻击，立即触发调度 Agent 紧急路径（不等 5s 周期）
}

// 聊天消息：作为玩家显式指令进入调度 Agent
void Coordinator::on_chat_message(const CBChatMessage& cb) {
    if (cb.message.empty()) return;
    PlayerCommand cmd;
    cmd.text = cb.message;
    cmd.received_ms = 0;   // TODO: 用 GetCurrentTime 时间戳
    submit_player_command(std::move(cmd));
}

// 信标：作为玩家隐式指令进入调度 Agent
void Coordinator::on_beacon(const CBBeacon& cb) {
    PlayerCommand cmd;
    if (cb.action == "place") {
        cmd.beacons.push_back(cb.cell);
    }
    cmd.received_ms = 0;
    submit_player_command(std::move(cmd));
}

// 内部：把 DispatchPlan 中的 TeamTask 下发各介入 Agent
void Coordinator::dispatch_tasks(const DispatchPlan& plan) {
    for (const auto& t : plan.team_tasks) {
        last_tasks_[t.team] = t;
        InterventionInput iin;
        iin.session_id = session_id_;
        iin.self_house = self_house_;
        iin.team = t.team;
        iin.units = t.units;
        iin.template_id = t.template_id;
        iin.target = t.target;
        iin.requirement = t.requirement;
        iin.priority = t.priority;
        intervention_->wake(iin);
    }
}

// 刷新缓存的会话公共字段
void Coordinator::fill_common_session_fields() {
    session_id_ = session_.session_id();
    self_house_ = session_.house();
}

} // namespace ihagent