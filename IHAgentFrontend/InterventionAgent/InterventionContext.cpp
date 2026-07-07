// InterventionContext.cpp
// 作用：介入 Agent 工作记忆实现。

#include "pch.h"
#include "InterventionContext.h"

namespace ihagent {

// 记录一次分支冲突
void InterventionContext::record_branch_conflict(int64_t now_ms) {
    branch_conflicts_.push_back(now_ms);
    consecutive_calm_ = 0;       // 冲突打断冷静计数
}

// 记录一次常规分支决策
void InterventionContext::record_calm_branch(int64_t /*now_ms*/) {
    ++consecutive_calm_;
}

// 清理时间窗口外的旧冲突记录
void InterventionContext::cleanup_old_records(int64_t now_ms, int64_t window_ms) {
    while (!branch_conflicts_.empty() && now_ms - branch_conflicts_.front() > window_ms) {
        branch_conflicts_.pop_front();
    }
}

// 进入接管模式
void InterventionContext::enter_takeover(int64_t now_ms) {
    takeover_active_ = true;
    takeover_start_ms_ = now_ms;
    consecutive_calm_ = 0;
    branch_conflicts_.clear();
}

// 退出接管模式
void InterventionContext::exit_takeover() {
    takeover_active_ = false;
    consecutive_calm_ = 0;
}

} // namespace ihagent