// DispatchContext.h
// 作用：定义调度 Agent 工作记忆及玩家指令/解析结果类型。
//       把 PlayerCommand / ParsedPlayerIntent 放在这里以避免与 DispatchAgent.h 循环 include。
//       v2：玩家隐式输入用 Cell 坐标（对齐后端 GetPlayerViewRoute/OnBeacon）。

#pragma once

#include "../Utils/CommonTypes.h"
#include "../Utils/Cell.h"
#include "../Utils/GameState.h"
#include <deque>

namespace ihagent {

// 玩家指令（显式自然语言 + 隐式镜头轨迹/Beacon），唯一入口
struct PlayerCommand {
    std::string text;                                      // 显式自然语言（可空）
    std::vector<ViewPoint> recent_view;                    // 隐式：镜头轨迹（来自 GetPlayerViewRoute）
    std::vector<Cell> beacons;                             // 隐式：Beacon 标记（来自 OnBeacon 回调）
    int64_t received_ms = 0;                                // 接收时间
};

// 玩家指令解析结果（由 LLM 解析得到，PlayerCommandParser 等模块使用）
struct ParsedPlayerIntent {
    bool is_strategic_request = false;
    StrategyForward forward;                          // 转发给战略 Agent
    std::vector<std::string> micro_constraints;        // 微操约束（来自玩家）
    std::string natural_language;                      // 原文片段
};

class DispatchContext {
public:
    // 记录玩家指令与对应解析结果（待执行事务）
    void record_player_command(const PlayerCommand& cmd, const ParsedPlayerIntent& intent);

    // 记录本轮生成的 DispatchPlan（待执行事务）
    void record_plan(const DispatchPlan& plan);

    // 记录紧急度标志
    void set_emergency(bool e) { emergency_ = e; }
    bool emergency() const { return emergency_; }

    // 上一轮任务列表（用于任务连续性差分）
    const std::vector<TeamTask>* last_tasks() const;

    // 清理已完成/失败事务（§6.1）
    void cleanup_finished_transactions();

private:
    std::deque<PlayerCommand> recent_cmds_;
    std::deque<ParsedPlayerIntent> recent_intents_;
    std::deque<DispatchPlan> recent_plans_;
    bool emergency_ = false;

    static constexpr size_t kMaxHistory = 16;
};

} // namespace ihagent