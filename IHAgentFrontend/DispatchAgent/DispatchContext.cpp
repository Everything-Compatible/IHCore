// DispatchContext.cpp
// 作用：调度 Agent 工作记忆实现。

#include "pch.h"
#include "DispatchContext.h"

namespace ihagent {

// 记录玩家指令和解析结果
void DispatchContext::record_player_command(const PlayerCommand& cmd, const ParsedPlayerIntent& intent) {
    recent_cmds_.push_back(cmd);
    recent_intents_.push_back(intent);
    if (recent_cmds_.size() > kMaxHistory) recent_cmds_.pop_front();
    if (recent_intents_.size() > kMaxHistory) recent_intents_.pop_front();
}

// 记录本轮计划
void DispatchContext::record_plan(const DispatchPlan& plan) {
    recent_plans_.push_back(plan);
    if (recent_plans_.size() > kMaxHistory) recent_plans_.pop_front();
}

// 上一轮 TeamTask 列表
const std::vector<TeamTask>* DispatchContext::last_tasks() const {
    return recent_plans_.empty() ? nullptr : &recent_plans_.back().team_tasks;
}

// 清理已完成事务（占位，需要 TeamTask↔反馈配对）
void DispatchContext::cleanup_finished_transactions() {
    // TODO: 真实实现需要把 plan 与介入反馈配对，反馈结束后从待执行事务移除
}

} // namespace ihagent