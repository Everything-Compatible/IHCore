// DispatchValidator.cpp
// 作用：DispatchPlan 规则校验实现（占位）。
//       v2：用 GetTechnoInRange 检查目标区敌我兵力比。

#include "pch.h"
#include "DispatchValidator.h"
#include "../Utils/Logger.h"

namespace ihagent {

// 整体校验主入口
bool DispatchValidator::validate(DispatchPlan& plan, IBackendClient* backend,
                                 const std::string& session_id, HouseId self_house) {
    bool ok = true;
    for (auto& t : plan.team_tasks) {
        if (is_suicide(t, backend, session_id, self_house)) {
            IH_LOG_WARN("DispatchValidator", "team " + std::to_string(t.team.value) + " suicide; downgraded to maneuver");
            // TODO: 把送死任务降级或撤销；此处暂降为重新集结
            t.template_id = "maneuver_regroup";
            t.priority = Priority::Low;
            ok = false;
        }
        if (is_overreach(t, backend, session_id, self_house)) {
            IH_LOG_WARN("DispatchValidator", "team " + std::to_string(t.team.value) + " overreach; units trimmed");
            // TODO: 删除越权单位
            ok = false;
        }
    }
    if (has_resource_waste(plan, backend, session_id)) {
        // TODO: 修正或合并产线/任务
        ok = false;
    }
    return ok;
}

// 判断送死：用 GetTechnoInRange 比较目标区敌我兵力
bool DispatchValidator::is_suicide(const TeamTask& task, IBackendClient* backend,
                                   const std::string& session_id, HouseId self_house) {
    if (!backend) return false;
    Cell target_cell{};
    if (task.target.kind == Target::Kind::Cell || task.target.kind == Target::Kind::Area) {
        target_cell = task.target.cell;
    } else {
        // 对象目标：查对象坐标
        if (auto info = backend->get_techno_info(session_id, task.target.object_addr)) {
            target_cell = info->cell;
        }
    }
    // TODO: 真实实现需估计目标区敌我兵力比，结合 template_id 是否为进攻类
    // 此处仅查询目标区附近的可见 Techno 数量作占位
    auto technos = backend->get_techno_in_range(session_id, target_cell, 10);
    int enemy = 0, selfn = 0;
    for (const auto& ti : technos) {
        if (ti.house == self_house) ++selfn;
        else if (ti.house >= 0) ++enemy;
    }
    (void)selfn; (void)enemy;
    return false;
}

// 判断越权：检查 task.units 是否真的是会话 House 的单位
bool DispatchValidator::is_overreach(const TeamTask& task, IBackendClient* backend,
                                     const std::string& session_id, HouseId self_house) {
    if (!backend) return false;
    for (const auto& addr : task.units) {
        if (auto info = backend->get_techno_info(session_id, addr)) {
            if (info->house != self_house) return true;
        }
    }
    return false;
}

// 判断资源浪费：占位——产线队列中重复生产同类型超过上限
bool DispatchValidator::has_resource_waste(const DispatchPlan& /*plan*/, IBackendClient* /*backend*/,
                                           const std::string& /*session_id*/) {
    // TODO: 查 GetProductionState 与 plan.production 比对
    return false;
}

} // namespace ihagent