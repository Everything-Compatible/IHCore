// SituationPreprocessor.cpp
// 作用：态势预处理规则实现。v2：按需查后端，把零散结果降维为结构化摘要。

#include "pch.h"
#include "SituationPreprocessor.h"
#include "../Utils/IdTypes.h"
#include "../Utils/Logger.h"

namespace ihagent {

// 汇总入口：按需查后端，组装 SituationSummary
SituationSummary SituationPreprocessor::summarize(IBackendClient* backend,
                                                   const std::string& session_id,
                                                   HouseId self_house, HouseId enemy_house) {
    SituationSummary s;
    if (!backend || session_id.empty()) return s;

    // 1. 当前时间作为摘要时间戳
    if (auto t = backend->get_current_time(session_id)) {
        s.timestamp_ms = static_cast<int64_t>(t->game_seconds * 1000.0);
    }

    // 2. 己方 House 信息
    HouseInfo self_info;
    if (auto hi = backend->get_house_info(session_id, self_house)) self_info = *hi;

    // 3. 敌方 House 信息（可见性受限，full_info 可能为 false）
    HouseInfo enemy_info;
    if (enemy_house >= 0) {
        if (auto hi = backend->get_house_info(session_id, enemy_house)) enemy_info = *hi;
    }

    s.self_unit_count = self_info.owned_units + self_info.owned_infantry + self_info.owned_aircraft;
    s.enemy_unit_count_estimate = enemy_info.owned_units + enemy_info.owned_infantry + enemy_info.owned_aircraft;
    s.military_strength = compute_self_military_strength(self_info);
    s.enemy_military_strength_estimate = estimate_enemy_military_strength(enemy_info);

    // 4. 产线状态（用于经济强度）
    ProductionState prod;
    if (auto ps = backend->get_production_state(session_id)) prod = *ps;
    s.economy_strength = compute_economy_strength(self_info, prod);

    // 5. 基地附近敌方单位（基地中心由 HouseInfo clusters 或 OwnedBuildings 估算，此处占位）
    // TODO: 基地中心坐标需要从 GetForceDistribution 或建筑查询获取，暂用 (0,0)
    Cell base_center{};
    s.hostile_units_near_base = collect_hostiles_near_base(backend, session_id, base_center, 15);

    // 6. 前线附近敌方单位（前线锚点待定，暂用基地中心 +N）
    // TODO: 前线定义待细化，暂用 (0,0) 占位
    Cell front_center{};
    s.hostile_units_near_front = collect_hostiles_near_front(backend, session_id, front_center, 20);

    // 7. 危险建筑
    s.vulnerable_buildings = collect_vulnerable_buildings(backend, session_id, base_center, 20, 0.5f);

    // TODO: 总体文字摘要
    s.overview = "[summary] self=" + std::to_string(s.self_unit_count)
                 + " enemy_est=" + std::to_string(s.enemy_unit_count_estimate)
                 + " ec=" + std::to_string(static_cast<int>(s.economy_strength * 100))
                 + "% mil=" + std::to_string(static_cast<int>(s.military_strength * 100)) + "%";
    return s;
}

// 我方军事强度：综合 OwnedUnits/Infantry/Aircraft 数量
float SituationPreprocessor::compute_self_military_strength(const HouseInfo& info) {
    int combat = info.owned_units + info.owned_infantry + info.owned_aircraft;
    // TODO: 不同单位战斗力差异大，需查 GetTypeInfo 加权
    // 粗略映射到 0~1（100 单位封顶）
    float raw = combat / 100.0f;
    return raw > 1.0f ? 1.0f : raw;
}

// 敌方军事强度估算（可见性受限时为下界）
float SituationPreprocessor::estimate_enemy_military_strength(const HouseInfo& enemy_info) {
    if (!enemy_info.full_info) {
        // 非同盟敌方仅有限信息，返回 0.5 占位
        // TODO: 结合 GetTechnoInRange 探测结果修正
        return 0.5f;
    }
    int combat = enemy_info.owned_units + enemy_info.owned_infantry + enemy_info.owned_aircraft;
    float raw = combat / 100.0f;
    return raw > 1.0f ? 1.0f : raw;
}

// 基地附近敌方单位类型
std::vector<std::string> SituationPreprocessor::collect_hostiles_near_base(IBackendClient* backend,
                                                                            const std::string& session_id,
                                                                            Cell base_center, int range) {
    std::vector<std::string> out;
    if (!backend) return out;
    // GetTechnoInRange 返回可见范围内的全部 Techno，按 house 过滤敌方
    // TODO: 此处需要知道敌方 House 索引；签名暂未传，留作 TODO
    auto technos = backend->get_techno_in_range(session_id, base_center, range);
    for (const auto& t : technos) {
        // TODO: 此处 house=-1 不过滤；实际需要 self_house 对比判定敌方
        out.push_back(t.type);
    }
    return out;
}

// 前线附近敌方单位类型
std::vector<std::string> SituationPreprocessor::collect_hostiles_near_front(IBackendClient* backend,
                                                                             const std::string& session_id,
                                                                             Cell front_center, int range) {
    return collect_hostiles_near_base(backend, session_id, front_center, range);
}

// 危险建筑：HP 比例低于阈值
std::vector<std::string> SituationPreprocessor::collect_vulnerable_buildings(IBackendClient* backend,
                                                                              const std::string& session_id,
                                                                              Cell base_center, int range,
                                                                              float hp_ratio) {
    std::vector<std::string> out;
    if (!backend) return out;
    auto technos = backend->get_techno_in_range(session_id, base_center, range, ObjectKind::Building);
    for (const auto& t : technos) {
        if (t.max_hp <= 0) continue;
        if (static_cast<float>(t.hp) / t.max_hp < hp_ratio) out.push_back(t.type);
    }
    return out;
}

// 经济强度：资金 + 产线负荷
float SituationPreprocessor::compute_economy_strength(const HouseInfo& info, const ProductionState& prod) {
    // 资金维度：每 5000 资金折算 0.2
    float money_score = info.money / 25000.0f;
    // 产线维度：活跃工厂数 / 4
    int active = 0;
    for (const auto& f : prod.factories) {
        if (!f.suspended && !f.on_hold) ++active;
    }
    float factory_score = active / 4.0f;
    float raw = money_score * 0.5f + factory_score * 0.5f;
    return raw > 1.0f ? 1.0f : raw;
}

} // namespace ihagent