// InterventionAgent.cpp
// 作用：介入 Agent 主流程实现——行为链执行 + 分支解析 + LLM 三模式调度。
//       v2：通过 backend_->issue_order(session_id, MicroOrder) 下发微操（对应后端 MEGAMISSION）。

#include "pch.h"
#include "InterventionAgent.h"
#include "BehaviorTemplates.h"
#include "BranchResolver.h"
#include "LLMIntervention.h"
#include "../Utils/Logger.h"
#include "../Utils/Geometry.h"

namespace ihagent {

// 默认行为链执行 Δt（接管模式）
static constexpr int64_t kTakeoverMaxMs = 30 * 1000;   // M=30s（待精调 §8.7）

// 构造，注入后端与 LLM 客户端
InterventionAgent::InterventionAgent(IBackendClient* backend, ILLMClient* llm)
    : backend_(backend), llm_(llm) {}

// 唤醒主流程：取模板 → 同步上下文 → 接管模式或行为链执行 → 检查终止
InterventionOutput InterventionAgent::wake(const InterventionInput& in) {
    InterventionOutput out;
    if (in.session_id.empty()) return out;

    // 取/绑定本 Team 对应模板
    auto& chain = chains_[in.team];
    if (chain.template_id != in.template_id) {
        const BehaviorChain* tpl = BehaviorTemplates::get(in.template_id);
        if (!tpl) {
            IH_LOG_WARN("InterventionAgent", "template not found: " + in.template_id + ", fallback to llm_full_control");
            chain = *BehaviorTemplates::get("llm_full_control");
            chain.template_id = "llm_full_control";
        } else {
            chain = *tpl;
        }
    }

    auto& ctx = contexts_[in.team];
    sync_chain_ctx(in, ctx);

    // 通用接管模板：直接进入接管模式
    if (chain.template_id == "llm_full_control" && !ctx.in_takeover()) {
        ctx.enter_takeover(0);   // TODO: 用 GetCurrentTime 的 game_seconds*1000 作时间戳
        IH_LOG_INFO("InterventionAgent", "team " + std::to_string(in.team.value) + " enter takeover (no template)");
    }
    // 升级判定：连续 N 次分支冲突 → 进接管
    else if (!ctx.in_takeover() && LLMIntervention::should_upgrade_to_takeover(ctx)) {
        ctx.enter_takeover(0);
        IH_LOG_INFO("InterventionAgent", "team " + std::to_string(in.team.value) + " upgrade to takeover");
    }

    std::vector<MicroOrder> orders;
    if (ctx.in_takeover()) {
        orders = step_takeover(in, ctx);
        // TODO: 接管上限检查（M 秒上限退出），需用 GetCurrentTime 算时长
        // 降级判定
        if (LLMIntervention::should_downgrade_from_takeover(ctx)) {
            ctx.exit_takeover();
            IH_LOG_INFO("InterventionAgent", "team " + std::to_string(in.team.value) + " downgrade to branch mode");
        }
    } else {
        // 行为链执行
        if (ctx.chain_ctx.current_node < static_cast<int>(chain.nodes.size())) {
            // 检查当前位置是否有分支点需要触发
            for (const auto& bp : chain.branches) {
                if (bp.at_node == ctx.chain_ctx.current_node) {
                    auto d = handle_branch(bp, ctx, in);
                    if (d.flow == BranchDecision::Flow::Hold) {
                        out.feedback.status = ExecutionFeedback::Status::Stalled;
                        out.feedback.issues.push_back("branch paused awaiting LLM");
                        return out;
                    }
                    if (d.flow == BranchDecision::Flow::Repeat) {
                        break;   // 留在当前节点重新执行
                    }
                    // Continue：跳过分支点继续
                }
            }
            const auto& node = chain.nodes[ctx.chain_ctx.current_node];
            orders = execute_node(node, ctx.chain_ctx, in);
            ctx.chain_ctx.current_node++;
            ctx.chain_ctx.epoch = 0;
        }
        // 检查终止
        if (ctx.chain_ctx.current_node >= static_cast<int>(chain.nodes.size()) ||
            end_clause_met(chain.end, in)) {
            out.feedback.status = ExecutionFeedback::Status::Success;
            out.feedback.progress = 1.0f;
        }
    }

    // 下发指令（每个 MicroOrder 对应后端一次 IssueOrder 调用）
    if (backend_) {
        for (const auto& o : orders) {
            backend_->issue_order(in.session_id, o);
        }
    }

    out.orders = std::move(orders);
    return out;
}

// 同步 chain_ctx（更新单位/目标）
void InterventionAgent::sync_chain_ctx(const InterventionInput& in, InterventionContext& ctx) {
    ctx.chain_ctx.units = in.units;
    ctx.chain_ctx.target = in.target;
    // 不重置 current_node（保留上一轮游标）
}

// 执行单个节点：Action → MicroOrder 列表（每个 MicroOrder 对应一次 IssueOrder）
std::vector<MicroOrder> InterventionAgent::execute_node(const BehaviorNode& node,
                                                       ChainExecutionContext& cctx,
                                                       const InterventionInput& in) {
    std::vector<MicroOrder> orders;
    MicroOrder o;
    o.units = cctx.units;
    o.use_selected_if_empty = false;   // 介入 Agent 总是显式指定 units

    // 按 Action 类型粗略映射到 Mission
    // TODO: 每种 Action 的具体语义与参数解析
    switch (node.action) {
        case Action::MoveTo:
        case Action::RetreatTo:
        case Action::AdvanceTo:
        case Action::DeployInNewPosition:
        case Action::GatherAt:
        case Action::GatherAtBase:
        case Action::Regroup:
        case Action::ReformFormation:
        case Action::FormEscortFormation:
            o.mission = Mission::Move;
            // 目标格子：取自参数 "target" 或任务 target
            // TODO: 解析 node.params.kv["target"] 为 Cell；此处暂用任务 target
            o.target_cell = in.target.cell;
            break;
        case Action::Strike:
        case Action::Assault:
        case Action::QuickStrike:
        case Action::FocusFire:
        case Action::AttackFromForwardBase:
            o.mission = Mission::Attack;
            o.target_object = in.target.object_addr;   // TODO: 优先攻击目标对象
            break;
        case Action::Wait:
        case Action::WaitForEnemy:
        case Action::WaitForImpact:
        case Action::WaitForStragglers:
            o.mission = Mission::Stop;
            break;
        case Action::GoToNode: {
            // 跳回指定节点（循环行为链，如 offense_harass）
            if (auto it = node.params.kv.find("node"); it != node.params.kv.end()) {
                try { cctx.current_node = std::stoi(it->second) - 1; }
                catch (...) {}
            }
            o.mission = Mission::Stop;
            break;
        }
        default:
            o.mission = Mission::Stop;
            break;
    }
    orders.push_back(o);
    return orders;
}

// 终止条件判定（占位）
bool InterventionAgent::end_clause_met(const EndCondition& end, const InterventionInput& /*in*/) {
    // TODO: 解析 end.clauses 中的条件，如 "objective_reached" "losses_exceed(50%)"
    //       v2: 损失统计用 OnUnitDestroyed 回调聚合；目标到达用 GetTechnoInfo 查 mission
    (void)end;
    return false;
}

// 分支点判定：规则 → 必要时 LLM
BranchDecision InterventionAgent::handle_branch(const BranchPoint& bp,
                                                InterventionContext& ctx,
                                                const InterventionInput& in) {
    // TODO: BranchResolver 需要查后端态势（GetTechnoInRange），签名待扩展
    //       临时用占位 ChainExecutionContext 传入
    auto d = BranchResolver::resolve(bp, ctx.chain_ctx, in);
    if (d.kind == BranchDecision::Kind::NeedLLM) {
        ctx.record_branch_conflict(0);   // TODO: 用 GetCurrentTime 时间戳
        auto summary = LLMIntervention::branch_decision(bp, ctx.chain_ctx, in, llm_);
        d.summary = "LLM: " + summary;
    } else {
        ctx.record_calm_branch(0);
    }
    return d;
}

// 接管模式单步
std::vector<MicroOrder> InterventionAgent::step_takeover(const InterventionInput& in,
                                                        InterventionContext& ctx) {
    // 构造 TeamTask 等价物给 LLMIntervention
    TeamTask task;
    task.team = in.team;
    task.units = in.units;
    task.template_id = in.template_id;
    task.target = in.target;
    task.priority = in.priority;
    task.requirement = in.requirement;
    return LLMIntervention::takeover_step(task, ctx, in, llm_);
}

} // namespace ihagent