// TaskDecomposer.h
// 作用：LLM 把战略清单 StrategyDirective 拆解为 TeamTask 列表（§4.4）。
//       v2：拆解后单位地址列表由 assign_units 用 GetTechnoInRange 填充。

#pragma once

#include "../Utils/CommonTypes.h"
#include "../Utils/BackendInterface.h"
#include "../Utils/LLMClient.h"
#include "DispatchAgent.h"
#include <vector>

namespace ihagent {

class TaskDecomposer {
public:
    // 把 StrategyDirective 拆解为一组 TeamTask
    static std::vector<TeamTask> decompose(const StrategyDirective& directive,
                                           const ParsedPlayerIntent& parsed,
                                           const DispatchInput& in,
                                           ILLMClient* llm);

    // 给 TeamTask 分配单位（规则：按单位类型与方向匹配）占位
    // TODO: 真实实现需考虑单位可用性、距离、模板需求
    static void assign_units(std::vector<TeamTask>& tasks,
                             IBackendClient* backend, const std::string& session_id,
                             HouseId self_house);
};

} // namespace ihagent