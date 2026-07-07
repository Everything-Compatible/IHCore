// StrategicAgent.cpp
// 作用：战略 Agent 主流程实现。v2：按需查询后端 → 态势预处理 → LLM → StrategyDirective。

#include "pch.h"
#include "StrategicAgent.h"
#include "SituationPreprocessor.h"
#include "../Utils/Logger.h"

namespace ihagent {

// 构造：注入后端客户端与 LLM 客户端
StrategicAgent::StrategicAgent(IBackendClient* backend, ILLMClient* llm)
    : backend_(backend), llm_(llm) {}

// 唤醒决策主流程：态势预处理 → 构造提示词 → 调 LLM → 解析 → 写入上下文
std::optional<StrategyDirective> StrategicAgent::wake(const StrategicInput& in) {
    if (in.session_id.empty()) {
        IH_LOG_WARN("StrategicAgent", "wake called without session_id");
        return std::nullopt;
    }

    // 1. 规则做态势预处理（内部按需查后端）
    SituationSummary summary = preprocess_situation(in);

    // 2. 把摘要和上轮Directive写入上下文（工作记忆）
    context_.record_situation(summary);
    if (in.forward) context_.record_forward(*in.forward);
    if (in.dispatch_feedback) context_.record_feedback(*in.dispatch_feedback);

    // 3. 构造提示词
    const StrategyDirective* prev = last_directive_ ? &(*last_directive_) : nullptr;
    std::string prompt = build_prompt(summary, in, prev);

    // 4. 调用 LLM 生成战略清单
    auto directive = call_llm(prompt);
    if (directive) {
        context_.record_directive(*directive);
        last_directive_ = directive;
        IH_LOG_INFO("StrategicAgent", "StrategyDirective generated");
    } else {
        IH_LOG_WARN("StrategicAgent", "LLM failed to produce directive, fallback to previous");
        // TODO: 失败时是否沿用上一轮Directive还是进入默认防守态？待定（§8）
    }
    return directive;
}

// 规则做态势预处理：按需查后端 + 汇总为结构化摘要（ADR-07）
SituationSummary StrategicAgent::preprocess_situation(const StrategicInput& in) {
    return SituationPreprocessor::summarize(backend_, in.session_id,
                                            in.self_house, in.enemy_house);
}

// 构造提示词（占位实现：拼一个简洁的文本，真实实现需结合 prompt 模板）
std::string StrategicAgent::build_prompt(const SituationSummary& summary,
                                         const StrategicInput& in,
                                         const StrategyDirective* prev) {
    // TODO: 引入专门的 prompt 模板，结构化注入各字段（§3.3）
    std::string p;
    p += "[SituationSummary]\n";
    p += summary.overview + "\n";
    p += "self_units=" + std::to_string(summary.self_unit_count) + "\n";
    p += "enemy_units_est=" + std::to_string(summary.enemy_unit_count_estimate) + "\n";

    if (in.forward) {
        p += "[StrategicForward]\n" + in.forward->strategic_intent + "\n";
        p += in.forward->natural_language_fragment + "\n";
    }
    if (prev) {
        p += "[PreviousDirective]\n" + prev->rationale + "\n";
    }
    return p;
}

// 调用 LLM 并解析输出为 StrategyDirective（占位，需对接LLM客户端并解析JSON）
std::optional<StrategyDirective> StrategicAgent::call_llm(const std::string& prompt) {
    // TODO: 真实实现需以 scene="strategy" 调用 LLM，并解析返回 JSON 为 StrategyDirective
    if (!llm_) return std::nullopt;

    LLMRequest req;
    req.scene = "strategy";
    req.system_prompt = "You are the strategic agent for an RA2 AI. Output a StrategyDirective as JSON.";
    req.user_messages.push_back(prompt);
    req.max_tokens = 1024;
    auto resp = llm_->call(req);
    if (!resp.ok || resp.text.empty()) return std::nullopt;

    // TODO: 解析 resp.text 为 StrategyDirective（见 §3.5 字段）
    StrategyDirective d;
    d.rationale = "LLM response parsing not yet implemented. Raw: " + resp.text.substr(0, 200);
    return d;
}

} // namespace ihagent