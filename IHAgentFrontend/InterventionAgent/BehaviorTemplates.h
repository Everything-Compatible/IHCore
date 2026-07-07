// BehaviorTemplates.h
// 作用：定义 §5.2.1 全部 18 个标准模板 + llm_full_control 通用接管模板。
//       从设计文档 §5.2.1 翻译为 C++ 数据。

#pragma once

#include "BehaviorChain.h"
#include <map>

namespace ihagent {

class BehaviorTemplates {
public:
    // 返回 template_id → BehaviorChain 的完整模板表
    static const std::map<BehaviorTemplateId, BehaviorChain>& all();

    // 按ID获取模板（找不到返回 nullptr）
    static const BehaviorChain* get(const BehaviorTemplateId& id);
};

} // namespace ihagent