// DispatchAgent.cpp
// 作用：调度 Agent 主流程实现。v2：用 session_id 查后端 + 下发操作。

#include "pch.h"
#include "DispatchAgent.h"
#include "PlayerCommandParser.h"
#include "TaskDecomposer.h"
#include "TemplateSelector.h"
#include "DispatchValidator.h"
#include "EmergencyClassifier.h"
#include "../Utils/Logger.h"

namespace ihagent {

// 构造，注入后端、LLM 客户端与战略 Agent 引用
DispatchAgent::DispatchAgent(IBackendClient* backend, ILLMClient* llm, StrategicAgent* strategic)
    : backend_(backend), llm_(llm), strategic_(strategic) {}

// 唤醒主流程：解析玩家指令 → 识别战略级诉求 → 紧急度分流 → 任务拆解 → 校验 → 输出
std::optional<DispatchPlan> DispatchAgent::wake(const DispatchInput& in) {
    if (in.session_id.empty()) {
        IH_LOG_WARN("DispatchAgent", "wake called without session_id");
        return std::nullopt;
    }

    DispatchPlan plan;
    if (auto t = backend_->get_current_time(in.session_id)) {
        plan.timestamp_ms = static_cast<int64_t>(t->game_seconds * 1000.0);
    }

    // 1. 玩家指令解析
    ParsedPlayerIntent parsed;
    if (in.player_command) {
        parsed = parse_player_command(*in.player_command, in);
        if (parsed.is_strategic_request) {
            plan.forward = parsed.forward;
        }
        context_.record_player_command(*in.player_command, parsed);
    }

    // 2. 紧急度分流
    bool urgent = in.player_command && is_emergency(*in.player_command, in);
    context_.set_emergency(urgent);

    // 3. 任务拆解（依赖战略清单）
    const bool has_directive = in.directive.has_value();
    const bool has_last_plan = last_plan_.has_value();
    const bool non_strategic_cmd = !parsed.is_strategic_request;
    if (has_directive || (has_last_plan && non_strategic_cmd)) {
        const StrategyDirective* directive = in.directive ? &(*in.directive) : nullptr;
        if (!directive && strategic_) {
            const auto& sd = strategic_->last_directive();
            if (sd) directive = &(*sd);
        }

        if (directive) {
            auto tasks = decompose_tasks(*directive, parsed, in);
            plan.team_tasks = std::move(tasks);
            for (auto& t : plan.team_tasks) {
                if (t.template_id.empty()) t.template_id = select_template(t, in);
            }
        } else if (urgent) {
            // 紧急路径：先做初步TeamTask拆解（不等战略清单）
            // TODO: 调度Agent立即做初步拆解，战略清单到达后再调整（§2.3 紧急路径）
            IH_LOG_INFO("DispatchAgent", "urgent path: initial decomposition without directive");
        }
    }

    // 5. 产线安排
    if (in.directive) plan.production = arrange_production(*in.directive, in);

    // 6. 规则校验：防送死/越权/资源浪费
    if (!validate_plan(plan, in)) {
        IH_LOG_WARN("DispatchAgent", "plan validation failed, plan dropped or adjusted");
    }

    // 7. 紧急覆盖（基地被攻时全员回防）
    if (EmergencyClassifier::base_under_attack(backend_, in.session_id, in.self_house)) {
        plan.emergency.active = true;
        plan.emergency.reason = "base under attack";
        plan.emergency.rally_cell = EmergencyClassifier::base_center(backend_, in.session_id, in.self_house);
        // TODO: 把所有可调动 Team 加入 affected_teams
    }

    context_.record_plan(plan);
    last_plan_ = plan;
    return plan;
}

// 玩家指令解析（LLM 主导）
ParsedPlayerIntent DispatchAgent::parse_player_command(const PlayerCommand& cmd, const DispatchInput& in) {
    return PlayerCommandParser::parse(cmd, in, llm_);
}

// 紧急度判定（规则），见 §2.3
bool DispatchAgent::is_emergency(const PlayerCommand& cmd, const DispatchInput& in) {
    return EmergencyClassifier::is_emergency(cmd, backend_, in.session_id, in.self_house);
}

// 任务拆解（LLM）
std::vector<TeamTask> DispatchAgent::decompose_tasks(const StrategyDirective& d,
                                                      const ParsedPlayerIntent& parsed,
                                                      const DispatchInput& in) {
    return TaskDecomposer::decompose(d, parsed, in, llm_);
}

// 行为链模板选择（LLM 从模板库匹配）
BehaviorTemplateId DispatchAgent::select_template(const TeamTask& task, const DispatchInput& in) {
    return TemplateSelector::select(task, TemplateSelector::available_templates(), in, llm_);
}

// 规则校验
bool DispatchAgent::validate_plan(DispatchPlan& plan, const DispatchInput& in) {
    return DispatchValidator::validate(plan, backend_, in.session_id, in.self_house);
}

// 产线安排（LLM + 规则校验，占位）
ProductionPlan DispatchAgent::arrange_production(const StrategyDirective& d, const DispatchInput& in) {
    ProductionPlan p;
    // TODO: 真实实现需 LLM 给队列建议 + 规则校验（资金/产线空位）
    //       下发用 backend_->produce(session_id, type_id) / place(...)
    p.rationale = "Placeholder production plan (TODO)";
    return p;
}

} // namespace ihagent