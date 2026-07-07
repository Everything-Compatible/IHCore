// TemplateSelector.cpp
// 作用：模板选择实现。LLM 失败时退化为按方向字段启发式匹配。

#include "pch.h"
#include "TemplateSelector.h"

namespace ihagent {

// 可用模板清单（对应 §5.2.1）
std::vector<BehaviorTemplateId> TemplateSelector::available_templates() {
    return {
        // 进攻类
        "offense_push", "offense_trench_push", "offense_harass", "offense_flank",
        "offense_feint", "offense_siege", "offense_surgical_strike", "offense_superweapon",
        // 防守类
        "defense_hold", "defense_base", "defense_retreat", "defense_escort",
        // 经济类
        "eco_expand", "eco_harv_protect", "eco_produce", "eco_tech_up",
        // 侦察类
        "recon_scout", "recon_harass",
        // 机动类
        "maneuver_regroup", "maneuver_reposition",
        // 特种
        "special_ambush",
        // 通用接管
        "llm_full_control"
    };
}

// 选择模板：LLM 调用 + 启发式回退
BehaviorTemplateId TemplateSelector::select(const TeamTask& task,
                                            const std::vector<BehaviorTemplateId>& candidates,
                                            const DispatchInput& /*in*/,
                                            ILLMClient* llm) {
    // TODO: 真实实现把 task 字段+单位类型+目标类型拼成 prompt 喂给 LLM
    // 简化：直接根据玩家原始要求/模板ID是否已填写启发式判断
    if (!task.template_id.empty()) return task.template_id;

    // LLM 不可用时的启发式回退（占位）
    if (!llm || candidates.empty()) return "llm_full_control";

    // 占位：选头一个（真实实现需按 task.target.kind / 玩家约束筛选）
    return candidates.front();
}

} // namespace ihagent