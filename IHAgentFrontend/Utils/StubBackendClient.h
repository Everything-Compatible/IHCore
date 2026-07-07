// StubBackendClient.h
// 作用：提供一个用于本地测试的空后端实现，便于在无真实引擎环境下编译与单元测试。
//       真实运行时由引擎侧通过依赖注入的方式替换为对接 EC SDK RemoteCall 的实现。

#pragma once

#include "BackendInterface.h"

namespace ihagent {

// 测试用空实现：会话返回固定 ID，操作返回 true，查询返回空/默认值
class StubBackendClient : public IBackendClient {
public:
    // BeginSession：返回固定会话 ID "stub-session"
    SessionHandle begin_session(HouseId house, bool cheat, int /*timeout_sec*/) override {
        SessionHandle h;
        h.session_id = "stub-session";
        h.house = house;
        h.cheat = cheat;
        return h;
    }
    // EndSession：固定返回 true
    bool end_session(const std::string& /*session_id*/) override { return true; }
    // SessionHeartbeat：固定返回 true
    bool session_heartbeat(const std::string& /*session_id*/) override { return true; }

    // ── 操作层：统一返回 true（不真实下发）──
    bool issue_order(const std::string&, const MicroOrder&) override { return true; }
    bool deploy(const std::string&, const ObjectAddress&) override { return true; }
    bool detonate(const std::string&, const ObjectAddress&) override { return true; }
    bool scatter(const std::string&, const ObjectAddress&) override { return true; }
    bool produce(const std::string&, const TypeID&) override { return true; }
    bool suspend(const std::string&, const TypeID&) override { return true; }
    bool abandon(const std::string&, const TypeID&) override { return true; }
    bool abandon_all(const std::string&) override { return true; }
    bool set_primary(const std::string&, const TypeID&) override { return true; }
    bool place(const std::string&, const TypeID&, Cell) override { return true; }
    bool special_place(const std::string&, const TypeID&, Cell) override { return true; }
    bool repair(const std::string&, const ObjectAddress&) override { return true; }
    bool sell(const std::string&, const ObjectAddress&) override { return true; }
    bool sell_cell(const std::string&, Cell) override { return true; }
    bool ally(const std::string&, HouseId) override { return true; }
    bool cheer(const std::string&) override { return true; }

    // ── 查询层：返回空/默认 ──
    std::optional<HouseInfo> get_house_info(const std::string&, HouseId = -1) override { return std::nullopt; }
    std::optional<ForceDistribution> get_force_distribution(const std::string&, HouseId = -1) override { return std::nullopt; }
    std::optional<MapInfo> get_map_info(const std::string&) override { return std::nullopt; }
    std::vector<TypeID> get_tech_tree(const std::string&, HouseId = -1) override { return {}; }
    std::string get_type_info(const std::string&, const TypeID&) override { return "{}"; }
    std::optional<ProductionState> get_production_state(const std::string&) override { return std::nullopt; }
    std::optional<GameTime> get_current_time(const std::string&) override { return std::nullopt; }
    std::vector<ViewPoint> get_player_view_route(const std::string&, double, double) override { return {}; }
    std::vector<TechnoInfo> get_techno_in_range(const std::string&, Cell, int, ObjectKind = ObjectKind::Unknown,
                                                HouseId = -1, const TypeID& = "", int = 50, int = 0) override { return {}; }
    std::optional<TechnoInfo> get_techno_info(const std::string&, const ObjectAddress&) override { return std::nullopt; }
    std::vector<ObjectAddress> get_objects_on_cell(const std::string&, Cell) override { return {}; }
    std::vector<ObjectAddress> get_aerial_objects(const std::string&, Cell, int) override { return {}; }
    std::optional<CellInfo> get_cell_info(const std::string&, Cell) override { return std::nullopt; }
    PathResult find_path(const std::string&, Cell, Cell, const TypeID& = "") override {
        PathResult r; r.reason = "StubBackendClient: not implemented"; return r;
    }
};

} // namespace ihagent