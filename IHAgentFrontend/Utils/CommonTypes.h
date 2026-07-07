// CommonTypes.h
// 作用：定义三层 Agent 间共享的核心数据结构——战略指令、调度计划、介入输出等。
//       v2 对齐后端：Mission 枚举对应 IssueOrder 的 -Mission 参数；
//       微操指令 MicroOrder 对应 IssueOrder 一次调用；
//       目标用对象地址或格子坐标。

#pragma once

#include "IdTypes.h"
#include "Cell.h"
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <variant>

namespace ihagent {

// ===================== 战略 Agent 输出 =====================

// 战略方向：直取 / 扩张 / 防守 / 骚扰（见 §3.5）
enum class Direction {
    DirectStrike,   // 直取
    Expand,          // 扩张
    Defend,          // 防守
    Harass           // 骚扰
};

// 各战线权重
struct PriorityWeights {
    std::map<std::string, float> weights;   // key: 战线名, value: 0~1 权重
};

// LLM 给出的威胁评估
struct ThreatAssessment {
    std::vector<std::string> major_threats;     // 主要威胁描述
    std::vector<std::string> minor_threats;     // 次要威胁描述
    float overall_risk = 0.0f;                   // 总体风险 0~1
};

// 资源分配建议
struct ResourceAllocation {
    std::map<std::string, float> allocations;   // key: 战线/产线, value: 比例
};

// 全局目标
struct GlobalObjective {
    std::string description;            // 自然语言描述的全局目标
    Cell focus_cell = {};               // 关注焦点单元格（可选）
};

// 战略清单（战略 Agent 输出，§3.5）
struct StrategyDirective {
    Direction direction = Direction::Defend;
    PriorityWeights priorities;
    ThreatAssessment threats;
    ResourceAllocation resources;
    GlobalObjective objective;
    std::string rationale;             // 决策理由（便于调试与上下文延续）
    int64_t timestamp_ms = 0;          // 生成时间戳
};

// ===================== 调度 Agent 输出 =====================

// 目标：可指定单位地址 / 格子坐标 / 建筑 / 区域中心之一
struct Target {
    enum class Kind { Object, Cell, Area } kind = Kind::Cell;
    ObjectAddress object_addr;          // 当 kind==Object 时有效
    Cell cell = {};                     // 当 kind==Cell 或 Area 时有效
    Cell fallback_cell = {};            // 目标失效时回退点
};

// 任务优先级
enum class Priority {
    Low = 0,
    Normal = 1,
    High = 2,
    Emergency = 3       // 紧急覆盖
};

// 玩家对该任务的具体要求（结构化 + 自然语言片段）
struct PlayerRequirement {
    std::string instruction;     // 玩家原始自然语言片段
    bool must_win = false;                        // 是否必须胜利
    bool preserve_units = false;                  // 是否要求尽量保存单位
    std::vector<std::string> constraints;          // 其它显式约束
};

// 单个 Team 任务（§4.5）
struct TeamTask {
    TeamId team;
    std::vector<ObjectAddress> units;     // 分配给该 Team 的单位地址
    BehaviorTemplateId template_id;       // 行为链模板 ID
    Target target;
    Priority priority = Priority::Normal;
    PlayerRequirement requirement;
    int64_t deadline_ms = 0;               // 截止时间（0 表示无）
};

// 产线安排（对应后端 Produce/Suspend/Abandon/Place 等操作）
struct ProductionPlan {
    struct QueueItem {
        TypeID type_id;            // 要生产的 TypeID（如 "E1"、"AIPAD"）
        int count = 1;
        bool suspend_existing = false;   // 是否先 Suspend 同类型
    };
    std::vector<QueueItem> queues;
    std::vector<TypeID> place_buildings; // 待 Place 的建筑 TypeID（位置由前端定）
    std::string rationale;          // 调度层对产线的说明（供调试）
};

// 紧急覆盖（如基地被攻时全员回防）
struct EmergencyOverride {
    bool active = false;
    std::string reason;
    std::vector<TeamId> affected_teams;
    Cell rally_cell = {};
};

// 转发给战略 Agent 的战略级诉求
struct StrategyForward {
    bool active = false;
    std::string strategic_intent;          // 结构化意图
    std::string natural_language_fragment;  // 玩家自然语言片段
    Priority priority = Priority::Normal;
};

// 调度计划（§4.5）
struct DispatchPlan {
    std::vector<TeamTask> team_tasks;
    ProductionPlan production;
    EmergencyOverride emergency;
    StrategyForward forward;
    int64_t timestamp_ms = 0;
};

// ===================== 介入 Agent 输出 =====================

// 微操指令 Mission 枚举，对齐后端 IssueOrder 的 -Mission 参数
// （后端 Mission 枚举共 31 种，前端可用子集见 API-SPEC §2.1）
enum class Mission {
    Move,          // 移动到目标格
    Attack,        // 攻击目标对象
    AttackMove,    // 攻击移动到目标格
    Guard,         // 警戒
    AreaGuard,     // 区域警戒
    Stop,          // 停止
    Hunt,          // 狩猎
    Retreat,       // 撤退
    Harvest,       // 采矿
    Enter,         // 进入/搭载
    Capture,       // 占领
    Patrol,        // 巡逻
    Unload,        // 卸载
    QMove,         // 排队移动
    Sticky,        // 粘滞
    Ambush,        // 埋伏
    Sabotage,      // 破坏
    Construction   // 建造
};

// 把 Mission 枚举转成后端 IssueOrder 接受的字符串
inline const char* mission_to_string(Mission m) {
    switch (m) {
        case Mission::Move:         return "Move";
        case Mission::Attack:       return "Attack";
        case Mission::AttackMove:   return "AttackMove";
        case Mission::Guard:        return "Guard";
        case Mission::AreaGuard:    return "AreaGuard";
        case Mission::Stop:         return "Stop";
        case Mission::Hunt:         return "Hunt";
        case Mission::Retreat:      return "Retreat";
        case Mission::Harvest:      return "Harvest";
        case Mission::Enter:        return "Enter";
        case Mission::Capture:      return "Capture";
        case Mission::Patrol:       return "Patrol";
        case Mission::Unload:       return "Unload";
        case Mission::QMove:        return "QMove";
        case Mission::Sticky:       return "Sticky";
        case Mission::Ambush:       return "Ambush";
        case Mission::Sabotage:     return "Sabotage";
        case Mission::Construction: return "Construction";
    }
    return "Stop";
}

// 微操指令（对应后端一次 IssueOrder 调用）
// 不带 queue_mode——后端 IssueOrder 对 -Units 列表逐单位发 MEGAMISSION，
// 排队语义用 Mission::QMove 表达。
struct MicroOrder {
    std::vector<ObjectAddress> units;    // 受指令影响的单位地址（-Units）
    Mission mission = Mission::Stop;       // -Mission
    ObjectAddress target_object;          // -Target（攻击/进入/占领对象）
    Cell target_cell = {};                // -X -Y（移动/巡逻/撤退目标格）
    bool use_selected_if_empty = true;    // -Units 为空时用会话 House 当前选中单位
};

// 执行反馈（由后端回调 OnBuildingReady/OnUnitDestroyed/OnUnderAttack 等聚合而来）
struct ExecutionFeedback {
    enum class Status { InProgress, Success, Failed, Cancelled, Stalled } status = Status::InProgress;
    float progress = 0.0f;                 // 0~1
    int losses = 0;                         // 损失单位数
    std::vector<std::string> issues;       // 遇到的问题
    int64_t timestamp_ms = 0;
};

// 介入输出（§5.6）
struct InterventionOutput {
    std::vector<MicroOrder> orders;
    ExecutionFeedback feedback;
};

} // namespace ihagent