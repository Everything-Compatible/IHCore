// EmergencyClassifier.cpp
// 作用：紧急度判定规则实现。v2：用 GetTechnoInRange 查基地附近敌方。

#include "pch.h"
#include "EmergencyClassifier.h"

namespace ihagent {

// 判定紧急：关键词/连点同区域/基地被攻击
bool EmergencyClassifier::is_emergency(const PlayerCommand& cmd, IBackendClient* backend,
                                       const std::string& session_id, HouseId self_house) {
    // 1. 关键词
    for (auto kw : {"立即", "紧急", "快", "救命", "attack now", "attack"}) {
        if (cmd.text.find(kw) != std::string::npos) return true;
    }
    // 2. 来源：玩家连续点击同区域（占位）
    // TODO: 需要历史镜头/点击序列记录，跨调用累积
    (void)cmd.recent_view;
    // 3. 态势：基地被攻击
    if (base_under_attack(backend, session_id, self_house)) return true;
    return false;
}

// 基地是否被攻击：基地附近范围内有敌方 Techno 且其 mission 为 Attack/AttackMove
bool EmergencyClassifier::base_under_attack(IBackendClient* backend, const std::string& session_id,
                                            HouseId self_house) {
    if (!backend) return false;
    Cell center = base_center(backend, session_id, self_house);
    if (center.X == 0 && center.Y == 0) return false;
    // 查基地附近 25 格内的 Techno
    auto technos = backend->get_techno_in_range(session_id, center, 25);
    // 后端 mission int 值：Attack=2, AttackMove=4（RA2 Mission 枚举，对齐后端）
    // TODO: 准确的 mission int 映射需对齐后端 Mission 枚举定义
    for (const auto& ti : technos) {
        if (ti.house == self_house) continue;
        if (ti.mission == 2 || ti.mission == 4) return true;   // 粗略：敌方在攻击/攻击移动
    }
    return false;
}

// 我方基地中心（用 GetForceDistribution 找建筑最密集区域，占位返回 (0,0)）
Cell EmergencyClassifier::base_center(IBackendClient* /*backend*/, const std::string& /*session_id*/,
                                      HouseId /*self_house*/) {
    // TODO: 真实实现需要查己方建筑重心。GetForceDistribution 按 3×3 区域给兵力分布，
    //       可选 Building 数最多的区域中心；或直接 GetTechnoInRange(filter=Building, house=self)
    //       求重心。暂用 (0,0) 占位。
    return Cell{};
}

} // namespace ihagent