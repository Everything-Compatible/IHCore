// StrategicContext.cpp
// 作用：战略 Agent 工作记忆实现。

#include "pch.h"
#include "StrategicContext.h"

namespace ihagent {

// 记录新一轮态势，超出上限丢弃最旧
void StrategicContext::record_situation(const SituationSummary& s) {
    recent_situations_.push_back(s);
    while (recent_situations_.size() > kMaxHistory) recent_situations_.pop_front();
}

// 记录战略诉求，标记为待处理事务
void StrategicContext::record_forward(const StrategyForward& f) {
    if (f.active) pending_forwards_.push_back(f);
}

// 记录战略指令，加入"待执行事务"
void StrategicContext::record_directive(const StrategyDirective& d) {
    pending_directives_.push_back(d);
}

// 记录执行反馈（用于检验上轮指令是否完成）
void StrategicContext::record_feedback(const ExecutionFeedback& fb) {
    recent_feedbacks_.push_back(fb);
    while (recent_feedbacks_.size() > kMaxHistory) recent_feedbacks_.pop_front();
}

// 清理已完成/失败/取消的事务（§6.1）
// TODO: 真实实现需把 Directive 与对应反馈配对，匹配规则待定（缺统一ID）
void StrategicContext::cleanup_finished_transactions() {
    // 占位：目前无Directive↔Feedback配对机制，简单清空已结束反馈
    while (!recent_feedbacks_.empty()) {
        auto& f = recent_feedbacks_.back();
        if (f.status == ExecutionFeedback::Status::Success ||
            f.status == ExecutionFeedback::Status::Failed ||
            f.status == ExecutionFeedback::Status::Cancelled) {
            recent_feedbacks_.pop_back();
        } else break;
    }
}

const SituationSummary* StrategicContext::latest_situation() const {
    return recent_situations_.empty() ? nullptr : &recent_situations_.back();
}

const StrategyDirective* StrategicContext::latest_directive() const {
    return pending_directives_.empty() ? nullptr : &pending_directives_.back();
}

} // namespace ihagent