// BranchResolver.cpp
// 作用：分支点规则决策实现。v2：触发条件检测可查后端态势（占位）。

#include "pch.h"
#include "BranchResolver.h"
#include "InterventionAgent.h"
#include "../Utils/Logger.h"
#include <map>

namespace ihagent {

// 规则注册表
static std::map<std::string, BranchResolver::RuleFn>& rule_registry() {
    static std::map<std::string, BranchResolver::RuleFn> reg;
    return reg;
}

// 注册规则
bool BranchResolver::register_rule(const std::string& name, RuleFn fn) {
    auto& reg = rule_registry();
    if (reg.count(name)) {
        IH_LOG_WARN("BranchResolver", "rule already registered: " + name);
        return false;
    }
    reg[name] = std::move(fn);
    return true;
}

// 判断触发条件是否满足（占位：根据 Trigger 类型做粗略判断）
// TODO: 真实实现需要查后端 GetTechnoInRange / OnUnderAttack 等态势
bool BranchResolver::trigger_active(Trigger tr, const ChainExecutionContext& /*ctx*/,
                                    const InterventionInput& /*in*/) {
    switch (tr) {
        case Trigger::EnemyContact:
            // TODO: 用 in.session_id 调 GetTechnoInRange 查本队单位附近敌方
            return false;
        default:
            return false;
    }
}

// 完整分支判定主入口
BranchDecision BranchResolver::resolve(const BranchPoint& bp,
                                       const ChainExecutionContext& ctx,
                                       const InterventionInput& in) {
    BranchDecision d;
    d.rule_name = bp.rule_name;

    if (!trigger_active(bp.trigger, ctx, in)) {
        d.kind = BranchDecision::Kind::ResolvedByRule;
        d.flow = BranchDecision::Flow::Continue;       // 未触发：正常继续
        d.summary = "trigger inactive";
        return d;
    }

    // 查表执行规则
    const auto& reg = rule_registry();
    auto it = reg.find(bp.rule_name);
    if (it != reg.end()) {
        d = it->second(ctx, in);
        d.rule_name = bp.rule_name;
        if (d.kind != BranchDecision::Kind::NeedLLM) return d;
    } else {
        IH_LOG_WARN("BranchResolver", "rule missing: " + bp.rule_name + ", using fallback");
    }

    // 规则未决断或未注册 → 按 fallback 处理
    if (bp.fallback == Fallback::LLM) {
        d.kind = BranchDecision::Kind::NeedLLM;
        d.flow = BranchDecision::Flow::Hold;
        d.summary = "rule undecided, defer to LLM";
    } else {
        d.kind = BranchDecision::Kind::FallbackDefault;
        d.flow = BranchDecision::Flow::Continue;
        d.summary = "rule fallback default: continue conservative";
    }
    return d;
}

// 借 LLM 做单次分支决策（§5.4 模式1）
std::string BranchResolver::llm_branch_decision(const BranchPoint& bp,
                                                const ChainExecutionContext& /*ctx*/,
                                                const InterventionInput& in,
                                                ILLMClient* llm) {
    if (!llm) return "no_llm";
    LLMRequest req;
    req.scene = "branch";
    req.system_prompt =
        "You are the intervention agent's branch decision maker. "
        "Output the chosen option ID and brief rationale.";
    req.user_messages.push_back("branch_rule=" + bp.rule_name);
    req.user_messages.push_back("session=" + in.session_id);
    req.max_tokens = 256;
    auto resp = llm->call(req);
    return resp.ok ? resp.text : "llm_failed";
}

} // namespace ihagent