// LLMIntervention.h
// 作用：介入层 LLM 三种调用模式实现（§5.4）——分支决策 / 接管模式 / 行为校验。
//       含升级（N次连续分支冲突→接管）与降级机制。
//       v2：接管模式输出 MicroOrder（含 Mission + 目标），由介入 Agent 下发 issue_order。

#pragma once

#include "../Utils/CommonTypes.h"
#include "../Utils/LLMClient.h"
#include "BranchResolver.h"
#include "InterventionContext.h"
#include "InterventionAgent.h"
#include <vector>
#include <optional>
#include <string>

namespace ihagent {

// LLM 调用模式
enum class LLMMode {
    BranchDecision,   // 单次分支决策
    Takeover,          // 接管模式：每 Δt 持续决策
    BehaviorVerify     // 行为校验
};

class LLMIntervention {
public:
    // 模式1：分支决策（单次）
    static std::string branch_decision(const BranchPoint& bp,
                                       const ChainExecutionContext& ctx,
                                       const InterventionInput& in,
                                       ILLMClient* llm);

    // 模式2：接管模式，输出一个时刻的微操指令序列
    //   Δt 间隔由调用方控制；达到 M 秒上限后调用方应停止接管
    static std::vector<MicroOrder> takeover_step(const TeamTask& task,
                                                 const InterventionContext& ctx,
                                                 const InterventionInput& in,
                                                 ILLMClient* llm);

    // 模式3：行为校验，输出 allow/deny/modify
    enum class Verdict { Allow, Deny, Modify };
    static Verdict verify(const MicroOrder& order,
                         const PlayerRequirement& requirement,
                         const InterventionInput& in,
                         ILLMClient* llm,
                         std::string& reason);

    // 判定是否升级为接管模式：T 窗口内 LLM 分支冲突次数 >= N
    static bool should_upgrade_to_takeover(const InterventionContext& ctx);
    // 判定是否降级：接管中连续 K 次常规分支决策无新危机
    static bool should_downgrade_from_takeover(const InterventionContext& ctx);
};

} // namespace ihagent