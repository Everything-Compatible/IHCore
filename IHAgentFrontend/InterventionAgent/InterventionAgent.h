// InterventionAgent.h
// 作用：介入 Agent 顶层模块。定位"班长"——带领具体 Team 执行行为链，
//       规则主导 + LLM 按需介入。三种 LLM 调用模式由 §5.4 规定。见 §5。
//       v2：通过 IBackendClient.issue_order 下发微操（对应后端 MEGAMISSION EventClass）。

#pragma once

#include "../Utils/CommonTypes.h"
#include "../Utils/GameState.h"
#include "../Utils/BackendInterface.h"
#include "../Utils/LLMClient.h"
#include "BehaviorChain.h"
#include "BranchResolver.h"
#include "InterventionContext.h"
#include <optional>
#include <vector>
#include <unordered_map>
#include <string>

namespace ihagent {

// 介入 Agent 输入（v2：用 session_id 让 Agent 自行查询 + 下发）
struct InterventionInput {
    std::string session_id;             // 当前会话 ID
    HouseId self_house = -1;
    TeamId team;
    std::vector<ObjectAddress> units;   // 本 Team 当前所辖单位地址（来自调度）
    BehaviorTemplateId template_id;     // 行为链模板
    Target target;
    PlayerRequirement requirement;
    Priority priority = Priority::Normal;
};

class InterventionAgent {
public:
    // 构造：注入后端客户端（写微操）+ LLM 客户端
    InterventionAgent(IBackendClient* backend, ILLMClient* llm);

    // 唤醒执行一步（在调度周期或事件触发时调用）
    InterventionOutput wake(const InterventionInput& in);

    // 获取本 Team 当前上下文（调试/可视化）
    InterventionContext& context(TeamId team) { return contexts_[team]; }

private:
    // 把 InterventionInput 转为本 Team 上下文里的 chain_ctx（更新游标/单位）
    void sync_chain_ctx(const InterventionInput& in, InterventionContext& ctx);

    // 执行当前节点：把 Action 翻译为 MicroOrder 列表（对应 IssueOrder 调用）
    std::vector<MicroOrder> execute_node(const BehaviorNode& node,
                                         ChainExecutionContext& cctx,
                                         const InterventionInput& in);

    // 检查终止条件是否满足（§5.2 end clauses）
    bool end_clause_met(const EndCondition& end, const InterventionInput& in);

    // 处理分支点：规则→(LLM)→决策
    BranchDecision handle_branch(const BranchPoint& bp,
                                 InterventionContext& ctx,
                                 const InterventionInput& in);

    // 接管模式单步
    std::vector<MicroOrder> step_takeover(const InterventionInput& in, InterventionContext& ctx);

    IBackendClient* backend_ = nullptr;
    ILLMClient* llm_ = nullptr;
    std::unordered_map<TeamId, InterventionContext, TeamIdHash> contexts_;
    std::unordered_map<TeamId, BehaviorChain, TeamIdHash> chains_;
};

} // namespace ihagent