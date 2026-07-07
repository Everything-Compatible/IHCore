// StrategicAgent.h
// 作用：战略 Agent 顶层模块。定位"军区司令"，LLM 主导意图生成，
//       每 30 秒定时唤醒或由调度转发战略级诉求触发。
//       v2：通过 IBackendClient 按需查询态势，不再用全量快照。见 §3。

#pragma once

#include "../Utils/CommonTypes.h"
#include "../Utils/GameState.h"
#include "../Utils/LLMClient.h"
#include "../Utils/BackendInterface.h"
#include "StrategicContext.h"
#include <optional>
#include <string>

namespace ihagent {

// 战略 Agent 输入（v2：用 session_id 让 Agent 自行查询；可选传入调度转发的诉求和反馈）
struct StrategicInput {
    std::string session_id;                                 // 当前会话 ID
    HouseId self_house = -1;                                 // 己方 House（接管方）
    HouseId enemy_house = -1;                                // 敌方 House（估算）
    std::optional<StrategyForward> forward;                  // 调度转发的战略级诉求
    std::optional<ExecutionFeedback> dispatch_feedback;      // 调度上报的执行进度
};

class StrategicAgent {
public:
    // 构造，注入后端客户端与 LLM 客户端
    StrategicAgent(IBackendClient* backend, ILLMClient* llm);

    // 唤醒决策（定时/事件触发皆可调用本函数）
    std::optional<StrategyDirective> wake(const StrategicInput& in);

    // 获取最近一次生成的战略清单
    const std::optional<StrategyDirective>& last_directive() const { return last_directive_; }

private:
    // 规则预处理：按需查询后端 + 汇总为结构化 SituationSummary（ADR-07）
    SituationSummary preprocess_situation(const StrategicInput& in);

    // 构造给 LLM 的提示词
    std::string build_prompt(const SituationSummary& summary,
                             const StrategicInput& in,
                             const StrategyDirective* prev);

    // 调用 LLM 并解析输出为 StrategyDirective
    std::optional<StrategyDirective> call_llm(const std::string& prompt);

    // 上下文（工作记忆，§6.1）
    StrategicContext context_;
    IBackendClient* backend_ = nullptr;
    ILLMClient* llm_ = nullptr;
    std::optional<StrategyDirective> last_directive_;
};

} // namespace ihagent