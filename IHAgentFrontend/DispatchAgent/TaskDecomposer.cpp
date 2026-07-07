// TaskDecomposer.cpp
// 作用：任务拆解实现。LLM 给主结构，规则做单调的"分配单位"工作。
//       v2：单位用 ObjectAddress（"0x........" 字符串）。

#include "pch.h"
#include "TaskDecomposer.h"
#include "../Utils/Logger.h"

namespace ihagent {

// 调 LLM 拆解战略为 TeamTask 列表
std::vector<TeamTask> TaskDecomposer::decompose(const StrategyDirective& directive,
                                                const ParsedPlayerIntent& parsed,
                                                const DispatchInput& in,
                                                ILLMClient* llm) {
    std::vector<TeamTask> tasks;
    if (!llm) return tasks;

    LLMRequest req;
    req.scene = "dispatch_split";
    req.system_prompt =
        "You are the dispatch agent's task decomposer. "
        "Given a StrategyDirective, output TeamTask list as JSON.";
    req.user_messages.push_back(directive.rationale.empty()
        ? "direction=" + std::to_string(static_cast<int>(directive.direction))
        : directive.rationale);
    if (!parsed.natural_language.empty()) {
        req.user_messages.push_back("player_constraint=" + parsed.natural_language);
    }
    req.max_tokens = 1024;

    auto resp = llm->call(req);
    if (!resp.ok) {
        IH_LOG_WARN("TaskDecomposer", "LLM call failed: " + resp.error);
        return tasks;
    }

    // TODO: 解析 resp.text 为 TeamTask 列表（§4.5 字段）
    // 占位：根据战略方向生成 1 条容错任务
    TeamTask t;
    t.team.value = 0;
    t.priority = Priority::Normal;
    t.template_id = "maneuver_regroup";   // 默认占位模板
    t.requirement.instruction = parsed.natural_language;
    t.requirement.must_win = (directive.direction == Direction::DirectStrike);
    tasks.push_back(t);
    return tasks;
}

// 占位：单位分配规则未实现
void TaskDecomposer::assign_units(std::vector<TeamTask>& /*tasks*/,
                                  IBackendClient* /*backend*/, const std::string& /*session_id*/,
                                  HouseId /*self_house*/) {
    // TODO: 按 Team 的 template_id 与方向，用 GetTechnoInRange 从未分配单位池挑合适的分配
    // TODO: 避免重复分配（与上一轮任务进行差分）
}

} // namespace ihagent