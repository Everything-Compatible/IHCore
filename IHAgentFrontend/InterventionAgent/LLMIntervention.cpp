// LLMIntervention.cpp
// 作用：介入层 LLM 三种调用模式实现。v2：接管模式输出 MicroOrder（含 Mission）。

#include "pch.h"
#include "LLMIntervention.h"
#include "../Utils/Logger.h"

namespace ihagent {

// 模式1：分支决策（单次），封装 BranchResolver 的 LLM 调用
std::string LLMIntervention::branch_decision(const BranchPoint& bp,
                                             const ChainExecutionContext& ctx,
                                             const InterventionInput& in,
                                             ILLMClient* llm) {
    return BranchResolver::llm_branch_decision(bp, ctx, in, llm);
}

// 模式2：接管模式单步决策，输出 MicroOrder 列表
std::vector<MicroOrder> LLMIntervention::takeover_step(const TeamTask& task,
                                                       const InterventionContext& /*ctx*/,
                                                       const InterventionInput& in,
                                                       ILLMClient* llm) {
    std::vector<MicroOrder> orders;
    if (!llm) return orders;

    LLMRequest req;
    req.scene = "takeover";
    req.system_prompt =
        "You are the intervention agent in LLM takeover mode. "
        "Output micro orders as JSON array, each with mission/units/target.";
    req.user_messages.push_back("template=" + task.template_id);
    req.user_messages.push_back("player_requirement=" + task.requirement.instruction);
    req.user_messages.push_back("session=" + in.session_id);
    req.max_tokens = 1024;

    auto resp = llm->call(req);
    if (!resp.ok) {
        IH_LOG_WARN("LLMIntervention", "takeover LLM call failed: " + resp.error);
        return orders;
    }
    // TODO: 解析 resp.text 为 MicroOrder 列表（含 Mission 枚举、units 地址、target_cell/object）
    (void)in;
    return orders;
}

// 模式3：行为校验
LLMIntervention::Verdict LLMIntervention::verify(const MicroOrder& /*order*/,
                                                  const PlayerRequirement& /*requirement*/,
                                                  const InterventionInput& in,
                                                  ILLMClient* llm,
                                                  std::string& reason) {
    if (!llm) { reason = "no_llm"; return Verdict::Allow; }
    LLMRequest req;
    req.scene = "verify";
    req.system_prompt =
        "Decide if the proposed micro order is consistent with player requirements. "
        "Output JSON with field \"verdict\": \"allow\"|\"deny\"|\"modify\" and \"reason\".";
    req.user_messages.push_back("session=" + in.session_id);
    req.max_tokens = 256;
    auto resp = llm->call(req);
    if (!resp.ok) { reason = "llm_failed"; return Verdict::Allow; }
    // TODO: 解析 resp.text 中的 verdict / reason
    reason = "TODO: parse verdict";
    return Verdict::Allow;
}

// 升级判定：时间窗口 T 内连续分支冲突次数 N=3
bool LLMIntervention::should_upgrade_to_takeover(const InterventionContext& ctx) {
    // 默认阈值（待精调，§8.2）
    constexpr int kConflictThreshold = 3;
    return ctx.recent_branch_conflict_count() >= kConflictThreshold;
}

// 降级判定：接管中连续 K 次无新危机的常规分支决策
bool LLMIntervention::should_downgrade_from_takeover(const InterventionContext& ctx) {
    constexpr int kCalmThreshold = 2;   // TODO: §8.2 待精调
    return ctx.consecutive_calm_branch_count() >= kCalmThreshold;
}

} // namespace ihagent