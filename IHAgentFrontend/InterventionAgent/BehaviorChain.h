// BehaviorChain.h
// 作用：定义行为链模板数据结构（§5.2）——顺序节点 + 分支点 + 终止条件。
//       规则主导执行，分支点规则优先决断，规则无法决断时按 fallback 回退到 LLM。

#pragma once

#include "../Utils/CommonTypes.h"
#include "../Utils/GameState.h"
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <variant>

namespace ihagent {

// 行为节点动作类型枚举（§5.2 §8.6 待细化）
enum class Action {
    GatherAt,
    GatherAtBase,
    GatherAttackForceWithMCV,
    MoveTo,
    EstablishFront,
    AdvanceTo,
    ConsolidatePosition,
    DeployMCV,
    EstablishDefensivePerimeter,
    AttackFromForwardBase,
    PackMCVAndAdvance,
    Strike,
    RetreatTo,
    Wait,
    GoToNode,
    EstablishFrontLine,
    Encircle,
    CutReinforcements,
    Attrition,
    Assault,
    SelectInsertionMethod,
    InfiltrateTo,
    FocusFire,
    Exfiltrate,
    PositionSupportForce,
    FireSuperWeapon,
    WaitForImpact,
    MoveInAndMopUp,
    HoldOrWithdraw,
    OccupyTerrain,
    DeployFormations,
    HoldFireUntilOptimalRange,
    Resist,
    AlertAllDefenders,
    ConvergeOnThreat,
    EliminateInvaders,
    RepairCritical,
    Disengage,
    Regroup,
    Rendezvous,
    FormEscortFormation,
    ScanForThreats,
    DefendEscortee,
    DeliverAndReturn,
    Prepare,
    EstablishMining,
    DeployDefense,
    AssignEscortToHarvester,
    PatrolAlong,
    EngageThreats,
    HarvesterStatusCheck,
    DeployAndBuild,
    SetProductionQueue,
    AllocateGuardForce,
    InitiateResearch,
    PatrolAndDefend,
    CompleteAndReport,
    Reconnaissance,
    ReportFindings,
    WithdrawOrLinger,
    ScoutAndIdentify,
    QuickStrike,
    WaitForStragglers,
    ReformFormation,
    DeployInNewPosition,
    ConcealInTerrain,
    WaitForEnemy,
    SpringAmbush,
    MopUpOrWithdraw,
    Unknown
};

// 触发条件类型（§5.2 §8.6 待细化）
enum class Trigger {
    EnemyContact,
    StrongResistance,
    FlankExposed,
    SupportUnavailable,
    EnemyReinforcementArrives,
    PursuitDetected,
    DetectionRisk,
    EnemyTurnsToFace,
    EnemyReinforces,
    MainForceReady,
    InsufficientForceToEncircle,
    EnemyBreakoutAttempt,
    InsertionSpottedEarly,
    PrimaryTargetDestroyedAndForceAlive,
    TargetNotDestroyed,
    EnemyReinforcementFloodsIn,
    EnemyBypasses,
    LineBreached,
    MultipleBreachPoints,
    InvaderTooStrong,
    EnemyPursuesAggressively,
    RetreatRouteBlocked,
    AmbushDetected,
    EscorteeHeavilyDamaged,
    EnemyPresenceDetected,
    InsufficientDefenseBudget,
    ThreatExceedsEscortCapability,
    HarvesterStrandedDeepEnemy,
    InsufficientFunds,
    TechBuildingUnderAttack,
    EnemyEconomicStrikeElsewhere,
    EnemyDetectorNearby,
    HighValueIntelSpotted,
    NoWeakTargetFound,
    EnemyResponseTooStrong,
    EnemyInterceptUnexpected,
    WrongEnemyGroupAppears,
    AmbushPartiallySuccessful,
    None
};

// 回退方式
enum class Fallback { Rule, LLM };

// 动作参数（宽松的 variant 集合）
struct ActionParams {
    // 典型键：target, radius, formation, mode, type, count, sector, path, etc.
    std::map<std::string, std::string> kv;
};

// 行为节点
struct BehaviorNode {
    Action action = Action::Unknown;
    ActionParams params;
};

// 分支点
struct BranchPoint {
    int at_node = 0;            // 在第几个节点后触发
    Trigger trigger = Trigger::None;
    std::string rule_name;       // 规则函数名（由 BranchResolver 注册）
    Fallback fallback = Fallback::Rule;
};

// 终止条件描述（自然语言/组合，便于调试；可由 LLM 解析）
struct EndCondition {
    std::vector<std::string> clauses;   // 例 "objective_reached", "losses_exceed(50%)"
};

// 行为链模板
struct BehaviorChain {
    BehaviorTemplateId template_id;
    std::vector<BehaviorNode> nodes;
    std::vector<BranchPoint> branches;
    EndCondition end;
    // llm_full_control 专用：进入接管模式
    bool continuous_llm_takeover = false;
};

// 行为链执行上下文（节点游标 + 本Team资源）
struct ChainExecutionContext {
    int current_node = 0;
    int epoch = 0;                       // 该节点已尝试次数（防死循环）
    bool chain_paused = false;           // 等待 LLM 介入时挂起
    std::vector<ObjectAddress> units;    // 本 Team 所辖单位地址（"0x........"）
    Target target;
};

} // namespace ihagent