// PlayerCommandParser.cpp
// 作用：玩家指令解析的 LLM 实现。v2：隐式输入来自 Cell/ViewPoint。

#include "pch.h"
#include "PlayerCommandParser.h"
#include "../Utils/Logger.h"

namespace ihagent {

// 用 LLM 解析玩家指令（含识别战略级诉求）
ParsedPlayerIntent PlayerCommandParser::parse(const PlayerCommand& cmd,
                                              const DispatchInput& in,
                                              ILLMClient* llm) {
    ParsedPlayerIntent intent;

    // 1. 隐式输入先做规则转换（不依赖 LLM）
    auto implicit_intents = interpret_implicit(cmd, in);
    (void)implicit_intents;  // TODO: 把含蓄意图作为约束传给 LLM 提示词

    // 2. 显式自然语言走 LLM
    if (!cmd.text.empty() && llm) {
        LLMRequest req;
        req.scene = "dispatch_parse";
        req.system_prompt =
            "You are the dispatch agent's command parser. "
            "Given a player command, decide if it's a strategic-level request "
            "(forward to strategic agent) or a tactical/micro request.";
        req.user_messages.push_back(cmd.text);
        req.max_tokens = 512;
        auto resp = llm->call(req);
        if (resp.ok) {
            // TODO: 解析返回 JSON 字段，填充 intent
            intent.natural_language = cmd.text;
            // 简易启发式占位（真实实现需 LLM 输出结构化字段）
            for (auto kw : {"立即", "紧急", "总攻", "战略", "扩张", "防守", "骚扰"}) {
                if (cmd.text.find(kw) != std::string::npos) {
                    intent.is_strategic_request = true;
                    intent.forward.active = true;
                    intent.forward.natural_language_fragment = cmd.text;
                    intent.forward.strategic_intent = "parsed: TODO";
                    break;
                }
            }
            if (!intent.is_strategic_request) {
                intent.micro_constraints.push_back(cmd.text);
            }
            return intent;
        }
    }

    // 3. 没文字、没 LLM 时的兜底：仅含隐式输入
    if (!cmd.recent_view.empty() || !cmd.beacons.empty()) {
        intent.micro_constraints.push_back("implicit_input_only");
    }
    return intent;
}

// 隐式输入解析（规则部分，§8 待定）
std::vector<std::string> PlayerCommandParser::interpret_implicit(const PlayerCommand& cmd,
                                                                 const DispatchInput& /*in*/) {
    std::vector<std::string> intents;
    // TODO: 把连续镜头停留/反复点击同区域识别为"关注区"
    // TODO: Beacon 标记识别为"重点点"
    for (const auto& b : cmd.beacons) {
        intents.push_back("beacon_at(" + std::to_string(b.X) + "," + std::to_string(b.Y) + ")");
    }
    for (const auto& v : cmd.recent_view) {
        intents.push_back("view(" + std::to_string(v.cell.X) + "," + std::to_string(v.cell.Y) + ")");
    }
    return intents;
}

} // namespace ihagent