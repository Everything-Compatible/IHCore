// StrategicContext.h
// 作用：战略 Agent 的工作记忆（§6.1）。保存最近态势摘要、上轮Directive、
//       调度反馈、转发的战略诉求等"待执行事务"，便于后续决策引用，避免前后矛盾。
//       本上下文仅在该 Agent 内部可见，不与调度 Agent 共享（§6.2 待确认）。

#pragma once

#include "../Utils/CommonTypes.h"
#include "../Utils/GameState.h"
#include <deque>
#include <vector>

namespace ihagent {

class StrategicContext {
public:
    // 记录新一轮态势摘要（保留最近 N 轮）
    void record_situation(const SituationSummary& s);

    // 记录调度转发的战略级诉求
    void record_forward(const StrategyForward& f);

    // 记录本轮生成的战略指令（也用于"待执行事务"追踪）
    void record_directive(const StrategyDirective& d);

    // 记录调度上报的执行反馈
    void record_feedback(const ExecutionFeedback& fb);

    // 清理已完成/失败/取消的事务（§6.1 清理规则）
    void cleanup_finished_transactions();

    // 获取最近一轮态势
    const SituationSummary* latest_situation() const;
    const StrategyDirective* latest_directive() const;

private:
    std::deque<SituationSummary> recent_situations_;        // 最近几轮态势
    std::deque<StrategyForward> pending_forwards_;          // 待处理的战略诉求
    std::deque<StrategyDirective> pending_directives_;      // 待执行/已执行事务
    std::deque<ExecutionFeedback> recent_feedbacks_;        // 近期执行反馈

    static constexpr size_t kMaxHistory = 8;
};

} // namespace ihagent