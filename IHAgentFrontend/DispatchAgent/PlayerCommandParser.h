// PlayerCommandParser.h
// 作用：玩家指令解析（LLM 主导，§4.4）。把显式自然语言 + 隐式镜头轨迹/Beacon
//       统一解析为结构化意图，并识别战略级诉求以转发给战略 Agent（§2.3）。
//       v2：隐式输入用 GetPlayerViewRoute/OnBeacon 来源的 Cell/ViewPoint。

#pragma once

#include "DispatchAgent.h"
#include "../Utils/LLMClient.h"
#include <vector>
#include <string>

namespace ihagent {

class PlayerCommandParser {
public:
    // 用 LLM 解析玩家指令；返回解析后的玩家意图
    static ParsedPlayerIntent parse(const PlayerCommand& cmd,
                                    const DispatchInput& in,
                                    ILLMClient* llm);

    // 把隐式输入（镜头轨迹/Beacon）转为结构化意图（规则部分）
    // TODO: 镜头轨迹/Beacon 如何转化为结构化意图待定（§8.8）
    static std::vector<std::string> interpret_implicit(const PlayerCommand& cmd,
                                                       const DispatchInput& in);
};

} // namespace ihagent