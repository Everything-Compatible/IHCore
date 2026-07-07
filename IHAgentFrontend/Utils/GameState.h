// GameState.h
// 作用：定义从后端 v2 命名查询返回的结构化态势。
//       v2 不再有"全量快照"——前端按需调用 GetHouseInfo/GetTechnoInRange/GetProductionState 等，
//       结果填入以下结构。规则做态势预处理后才喂给 LLM（ADR-07）。

#pragma once

#include "IdTypes.h"
#include "Cell.h"
#include "CommonTypes.h"
#include <string>
#include <vector>
#include <map>
#include <optional>

namespace ihagent {

// 对象类别（对齐后端 whatami 字段：0=Unit, 1=Infantry, 2=Building, 3=Aircraft）
enum class ObjectKind {
    Unit = 0,
    Infantry = 1,
    Building = 2,
    Aircraft = 3,
    Unknown = -1
};

// 单个 Techno 信息（对齐后端 GetTechnoInRange 返回字段）
struct TechnoInfo {
    ObjectAddress addr;          // "0x........"
    ObjectKind whatami = ObjectKind::Unknown;
    std::string whatami_str;     // "Unit"/"Infantry"/"Building"/"Aircraft"
    TypeID type;                 // TechnoType ID，如 "E1"
    int hp = 0;
    int max_hp = 0;
    int mission = 0;             // 后端 Mission 枚举 int 值
    Cell cell = {};
    HouseId house = -1;
    bool selected = false;
    bool in_limbo = false;
};

// House 信息（对齐后端 GetHouseInfo 返回字段）
struct HouseInfo {
    HouseId index = -1;
    std::string name;
    std::string parent_country;   // 阵营 ID
    std::string country;
    int side_index = -1;
    int money = 0;
    int owned_units = 0;
    int owned_buildings = 0;
    int owned_infantry = 0;
    int owned_aircraft = 0;
    bool is_player = false;
    bool full_info = false;       // 非同盟 House 仅有限信息
    // 聚类检测（仅己方 House 有）
    struct Cluster {
        Cell cell = {};
        int total = 0;
        std::map<TypeID, int> type_counts;
    };
    std::vector<Cluster> clusters;
};

// 工厂状态（对齐后端 GetProductionState.Factories 元素）
struct FactoryState {
    ObjectAddress address;
    TypeID type_id;
    std::vector<TypeID> queue;
    float progress = 0.0f;
    bool suspended = false;
    bool is_primary = false;
    bool is_manual = false;
    bool on_hold = false;
    bool is_done = false;
    int cost_per_step = 0;
    int balance = 0;
    std::string special_item;     // 超武等特殊生产项
};

// 产线状态（对齐后端 GetProductionState 顶层）
struct ProductionState {
    HouseId house = -1;
    TypeID producing_building;    // 当前在造的建筑
    TypeID producing_unit;
    TypeID producing_infantry;
    TypeID producing_aircraft;
    std::vector<FactoryState> factories;
};

// 兵力分布（对齐后端 GetForceDistribution）
struct ForceDistribution {
    HouseId house = -1;
    int total = 0;
    struct ByType { int infantry = 0; int unit = 0; int building = 0; int aircraft = 0; };
    ByType by_type;
    struct Region {
        int rx = 0, ry = 0;       // 3×3 区域索引 (0..2)
        ByType counts;
        int total = 0;
    };
    std::vector<Region> by_region;   // 9 项
};

// 地图信息（对齐后端 GetMapInfo）
struct MapInfo {
    int width = 0;
    int height = 0;
    int left = 0, top = 0, right = 0, bottom = 0;
};

// 当前时间（对齐后端 GetCurrentTime）
struct GameTime {
    int frame = 0;
    double game_seconds = 0.0;
};

// 玩家视口轨迹点（对齐后端 GetPlayerViewRoute 元素）
struct ViewPoint {
    double time = 0.0;
    Cell cell = {};
};

// 单元格地形属性（对齐后端 GetCellInfo）
struct CellInfo {
    bool visible = false;
    std::string land_type;
    int occupation_flags = 0;
    int alt_occupation_flags = 0;
    bool contains_bridge = false;
    int slope_index = 0;
};

// 寻路结果（对齐后端 FindPath）
struct PathResult {
    bool success = false;
    std::string reason;
    std::vector<Cell> waypoints;
};

// 玩家隐式输入（镜头轨迹 + Beacon，由 GetPlayerViewRoute + OnBeacon 回调聚合）
struct ImplicitInput {
    std::vector<ViewPoint> recent_view;     // 最近关注区域
    std::vector<Cell> beacons;              // 放置的信标
    int64_t last_update_ms = 0;
};

// 态势摘要（规则预处理后的结构化结果，喂给战略/调度 LLM）
struct SituationSummary {
    std::string overview;                          // 总体态势文字摘要
    std::vector<std::string> hostile_units_near_base;     // 基地附近敌方单位
    std::vector<std::string> hostile_units_near_front;    // 前线敌方单位
    std::vector<std::string> vulnerable_buildings;         // 危险建筑
    int self_unit_count = 0;
    int enemy_unit_count_estimate = 0;                     // 估计值
    float economy_strength = 0.0f;                         // 我方经济强度 0~1
    float military_strength = 0.0f;                        // 我方军事强度 0~1
    float enemy_military_strength_estimate = 0.0f;
    int64_t timestamp_ms = 0;
};

} // namespace ihagent