// EmergencyClassifier.h
// 作用：紧急度判定规则（§2.3 §8.3 待定）。基于玩家指令关键词、来源（连点同区域）、
//       战场态势（基地被攻击）等综合判定紧急度，并选择时序路径。
//       v2：基地被攻击用 GetTechnoInRange 在基地附近查敌方开火单位。

#pragma once

#include "DispatchAgent.h"
#include "../Utils/BackendInterface.h"
#include "../Utils/GameState.h"

namespace ihagent {

class EmergencyClassifier {
public:
    // 判定玩家指令是否紧急（基于关键词/来源/态势）
    static bool is_emergency(const PlayerCommand& cmd, IBackendClient* backend,
                             const std::string& session_id, HouseId self_house);

    // 基地是否正在被攻击
    static bool base_under_attack(IBackendClient* backend, const std::string& session_id,
                                  HouseId self_house);

    // 我方基地中心坐标（用 GetForceDistribution 或建筑重心估算）
    static Cell base_center(IBackendClient* backend, const std::string& session_id,
                            HouseId self_house);
};

} // namespace ihagent