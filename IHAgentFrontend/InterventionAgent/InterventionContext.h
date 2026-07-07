// InterventionContext.h
// 作用：介入 Agent 工作记忆——Team 级上下文，含行为链执行游标、分支触发记录、
//       接管模式状态（持续时长 Δt / 上限 M / 降级计数），见 §5.4 §6.2。

#pragma once

#include "BehaviorChain.h"
#include "../Utils/CommonTypes.h"
#include <deque>
#include <vector>
#include <chrono>

namespace ihagent {

class InterventionContext {
public:
    // 标记一次分支冲突（规则未决断转 LLM）
    void record_branch_conflict(int64_t now_ms);

    // 标记一次常规分支决策（接管模式下用于降级判定）
    void record_calm_branch(int64_t now_ms);

    // 清理过老记录（保留时间窗口 T，§8.2）
    void cleanup_old_records(int64_t now_ms, int64_t window_ms);

    // 进入接管模式
    void enter_takeover(int64_t now_ms);
    // 退出接管模式
    void exit_takeover();
    bool in_takeover() const { return takeover_active_; }
    int64_t takeover_start_ms() const { return takeover_start_ms_; }

    int recent_branch_conflict_count() const { return static_cast<int>(branch_conflicts_.size()); }
    int consecutive_calm_branch_count() const { return consecutive_calm_; }

    // 行为链执行上下文（每 Team 一个）
    ChainExecutionContext chain_ctx;

private:
    std::deque<int64_t> branch_conflicts_;   // 时间窗口内的分支冲突时间戳
    int consecutive_calm_ = 0;                 // 接管模式中连续冷静分支数
    bool takeover_active_ = false;
    int64_t takeover_start_ms_ = 0;
};

} // namespace ihagent