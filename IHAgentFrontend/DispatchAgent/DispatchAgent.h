// DispatchAgent.h
// 作用：调度 Agent 顶层模块。定位"前线指挥部"，LLM 主导指令解析+任务拆解，
//       规则做紧急度判定与校验。玩家指令唯一入口。见 §4。
//       v2：通过 IBackendClient 按需查询态势 + 下发操作（IssueOrder/Produce/Place）。

#pragma once

#include "../Utils/CommonTypes.h"
#include "../Utils/GameState.h"
#include "../Utils/LLMClient.h"
#include "../Utils/BackendInterface.h"
#include "DispatchContext.h"
#include "../StrategicAgent/StrategicAgent.h"
#include <optional>
#include <string>

namespace ihagent {

// 调度 Agent 输入（v2：用 session_id 让 Agent 自行查询）
struct DispatchInput {
    std::string session_id;                                 // 当前会话 ID
    HouseId self_house = -1;
    HouseId enemy_house = -1;
    std::optional<StrategyDirective> directive;             // 新战略清单
    std::optional<PlayerCommand> player_command;             // 玩家指令（唯一入口）
    std::vector<ExecutionFeedback> intervention_feedbacks;  // 介入 Agent 状态上报
};

class DispatchAgent {
public:
    // 构造：注入后端客户端、LLM 客户端，以及战略 Agent 引用（用于转发战略诉求）
    DispatchAgent(IBackendClient* backend, ILLMClient* llm, StrategicAgent* strategic);

    // 唤醒决策，输出 DispatchPlan
    std::optional<DispatchPlan> wake(const DispatchInput& in);

    const DispatchPlan* last_plan() const { return last_plan_ ? &(*last_plan_) : nullptr; }

private:
    // 玩家指令解析（LLM）
    ParsedPlayerIntent parse_player_command(const PlayerCommand& cmd, const DispatchInput& in);

    // 紧急度判定（规则）→ 选择时序路径：先拆解后调整 / 先转发等回流
    bool is_emergency(const PlayerCommand& cmd, const DispatchInput& in);

    // 任务拆解（LLM）：StrategyDirective → TeamTask 列表
    std::vector<TeamTask> decompose_tasks(const StrategyDirective& d,
                                          const ParsedPlayerIntent& parsed,
                                          const DispatchInput& in);

    // 行为链模板选择（LLM 从模板库匹配）
    BehaviorTemplateId select_template(const TeamTask& task, const DispatchInput& in);

    // 规则校验：防送死/越权/资源浪费
    bool validate_plan(DispatchPlan& plan, const DispatchInput& in);

    // 产线安排（LLM + 规则校验）
    ProductionPlan arrange_production(const StrategyDirective& d, const DispatchInput& in);

    IBackendClient* backend_ = nullptr;
    ILLMClient* llm_ = nullptr;
    StrategicAgent* strategic_ = nullptr;
    DispatchContext context_;
    std::optional<DispatchPlan> last_plan_;
};

} // namespace ihagent