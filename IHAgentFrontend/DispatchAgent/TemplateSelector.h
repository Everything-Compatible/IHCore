// TemplateSelector.h
// 作用：LLM 介入行为链模板选择（§4.4）。从 18 标准模板+1 通用接管模板中
//       根据 TeamTask 与单位构成/目标类型选择最合适模板。
//       v2：选择时可查 GetTechnoInfo 获取单位类型属性。

#pragma once

#include "../Utils/CommonTypes.h"
#include "../Utils/BackendInterface.h"
#include "../Utils/LLMClient.h"
#include "DispatchAgent.h"
#include <vector>
#include <string>

namespace ihagent {

class TemplateSelector {
public:
    // 返回所有可用模板ID（6 类 18 个 + llm_full_control）
    static std::vector<BehaviorTemplateId> available_templates();

    // 选择最合适的模板
    static BehaviorTemplateId select(const TeamTask& task,
                                     const std::vector<BehaviorTemplateId>& candidates,
                                     const DispatchInput& in,
                                     ILLMClient* llm);
};

} // namespace ihagent