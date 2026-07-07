// DispatchValidator.h
// 作用：规则校验层（§4.4）。在 DispatchPlan 输出前做"防送死/越权/资源浪费"检查。
//       v2：用 GetTechnoInRange 检查目标区敌我兵力比；越权检查用会话 House 单位列表。

#pragma once

#include "../Utils/CommonTypes.h"
#include "../Utils/BackendInterface.h"
#include "../Utils/GameState.h"
#include "DispatchAgent.h"

namespace ihagent {

class DispatchValidator {
public:
    // 对整张 DispatchPlan 做校验，必要时就地修正；返回整体是否合法
    static bool validate(DispatchPlan& plan, IBackendClient* backend,
                         const std::string& session_id, HouseId self_house);

    // 单个 TeamTask 是否"送死"（目标区敌兵力远超我方且无支援）
    static bool is_suicide(const TeamTask& task, IBackendClient* backend,
                           const std::string& session_id, HouseId self_house);

    // 单个 TeamTask 是否越权（操作了未分配给该 Team 的单位）
    static bool is_overreach(const TeamTask& task, IBackendClient* backend,
                             const std::string& session_id, HouseId self_house);

    // DispatchPlan 是否资源浪费（重复任务、产线冲突）
    static bool has_resource_waste(const DispatchPlan& plan, IBackendClient* backend,
                                   const std::string& session_id);
};

} // namespace ihagent