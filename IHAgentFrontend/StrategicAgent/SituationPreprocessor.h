// SituationPreprocessor.h
// 作用：规则做态势预处理——按需调用后端 v2 查询接口，把零散查询结果汇总为
//       结构化 SituationSummary，避免直接把生数据喂给 LLM（ADR-07）。
//       v2：不再吃全量 GameState，而是按需 GetHouseInfo/GetTechnoInRange/GetProductionState。

#pragma once

#include "../Utils/GameState.h"
#include "../Utils/BackendInterface.h"
#include <string>

namespace ihagent {

class SituationPreprocessor {
public:
    // 汇总入口：按需查后端，组装 SituationSummary
    static SituationSummary summarize(IBackendClient* backend, const std::string& session_id,
                                      HouseId self_house, HouseId enemy_house);

    // 计算我方军事强度（0~1，基于 HouseInfo 的 OwnedUnits/Infantry/Aircraft）
    static float compute_self_military_strength(const HouseInfo& info);

    // 估算敌方军事强度（0~1，敌方 OwnedXxx 可能为部分可见）
    static float estimate_enemy_military_strength(const HouseInfo& enemy_info);

    // 列出基地周边范围内的敌方单位类型
    static std::vector<std::string> collect_hostiles_near_base(IBackendClient* backend,
                                                                const std::string& session_id,
                                                                Cell base_center, int range);

    // 列出前线周边范围内的敌方单位类型
    static std::vector<std::string> collect_hostiles_near_front(IBackendClient* backend,
                                                                 const std::string& session_id,
                                                                 Cell front_center, int range);

    // 列出 HP 比例低于阈值的危险建筑类型
    static std::vector<std::string> collect_vulnerable_buildings(IBackendClient* backend,
                                                                  const std::string& session_id,
                                                                  Cell base_center, int range,
                                                                  float hp_ratio);

    // 判定我方经济强度 0~1（基于资金 + 产线负荷）
    static float compute_economy_strength(const HouseInfo& info, const ProductionState& prod);
};

} // namespace ihagent