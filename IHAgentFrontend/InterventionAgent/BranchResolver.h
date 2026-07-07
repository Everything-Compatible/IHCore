// BranchResolver.h
// 作用：分支点规则决策（§5.3）。行为链执行到分支点时按触发条件进入分支判定，
//       规则优先尝试决策；规则无法决断且 fallback=llm 时返回"需 LLM 介入"标记。
//       v2：触发条件检测用 InterventionInput（含 session_id）查后端态势。

#pragma once

#include "BehaviorChain.h"
#include "../Utils/LLMClient.h"
#include <optional>
#include <string>
#include <vector>
#include <functional>

namespace ihagent {

// 前向声明，避免与 InterventionAgent.h 循环 include
struct InterventionInput;

// 分支决策结果
struct BranchDecision {
    enum class Kind {
        ResolvedByRule,    // 规则已决断，继续执行
        NeedLLM,           // 规则无法决断，需唤醒 LLM
        FallbackDefault,    // 兜底（fallback=rule 时规则给默认）
    } kind = Kind::ResolvedByRule;

    // 决策后行为链继续指令：跳转到下一节点 / 重复当前节点 / 挂起
    enum class Flow { Continue, Repeat, Hold } flow = Flow::Continue;

    std::string rule_name;            // 实际执行的规则名（便于调试）
    std::string summary;              // 决策说明
    std::vector<std::string> llm_options;   // 分支决策模式下 LLM 输入的可选动作
};

class BranchResolver {
public:
    // 注册一条规则（按 rule_name 注册），返回是否注册成功
    using RuleFn = std::function<BranchDecision(const ChainExecutionContext&, const InterventionInput&)>;
    static bool register_rule(const std::string& name, RuleFn fn);

    // 判断触发条件是否满足（基于当前态势，规则为主）
    static bool trigger_active(Trigger tr, const ChainExecutionContext& ctx, const InterventionInput& in);

    // 完整分支判定：触发是否满足 → 规则尝试决策 → fallback 分流
    static BranchDecision resolve(const BranchPoint& bp,
                                  const ChainExecutionContext& ctx,
                                  const InterventionInput& in);

    // 借 LLM 做分支决策（§5.4 模式1），返回决策说明
    static std::string llm_branch_decision(const BranchPoint& bp,
                                           const ChainExecutionContext& ctx,
                                           const InterventionInput& in,
                                           ILLMClient* llm);
};

} // namespace ihagent