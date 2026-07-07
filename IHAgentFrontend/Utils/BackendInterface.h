// BackendInterface.h
// 作用：定义前端与后端 (IHAgentBackend v2) 交互的抽象接口。
//       v2 对齐：所有方法对应后端 EC SDK 注册的命名 Action，
//       参数走 JsonObject 风格（此处用结构化参数），返回查询结果或 bool。
//       所有调用携带 SessionID（由 SessionLifecycle 管理）。
//       见 API-SPEC.md。

#pragma once

#include "IdTypes.h"
#include "Cell.h"
#include "CommonTypes.h"
#include "GameState.h"
#include <string>
#include <vector>
#include <optional>

namespace ihagent {

// 会话句柄（由 BeginSession 返回）
struct SessionHandle {
    std::string session_id;     // UUID
    HouseId house = -1;         // 接管的作战方索引
    bool cheat = false;
    int timeout_sec = 30;
    int64_t expire_at_ms = 0;
    bool valid() const { return !session_id.empty(); }
};

// 后端客户端抽象接口（v2）
// 每个方法对应后端一个命名 Action，实现层负责通过 EC SDK RemoteCall 调用。
class IBackendClient {
public:
    virtual ~IBackendClient() = default;

    // ── 会话管理 ──
    // BeginSession -House <int> [-Cheat] [-Timeout <int>]
    virtual SessionHandle begin_session(HouseId house, bool cheat, int timeout_sec) = 0;
    // EndSession -SessionID
    virtual bool end_session(const std::string& session_id) = 0;
    // SessionHeartbeat -SessionID
    virtual bool session_heartbeat(const std::string& session_id) = 0;

    // ── 操作层（基于 EventClass）──
    // IssueOrder -SessionID -Mission -Units [-Target/-X -Y]
    virtual bool issue_order(const std::string& session_id, const MicroOrder& order) = 0;
    // Deploy -SessionID -Address
    virtual bool deploy(const std::string& session_id, const ObjectAddress& addr) = 0;
    // Detonate -SessionID -Address
    virtual bool detonate(const std::string& session_id, const ObjectAddress& addr) = 0;
    // Scatter -SessionID -Address
    virtual bool scatter(const std::string& session_id, const ObjectAddress& addr) = 0;
    // Produce -SessionID -TypeID
    virtual bool produce(const std::string& session_id, const TypeID& type_id) = 0;
    // Suspend -SessionID -TypeID
    virtual bool suspend(const std::string& session_id, const TypeID& type_id) = 0;
    // Abandon -SessionID -TypeID
    virtual bool abandon(const std::string& session_id, const TypeID& type_id) = 0;
    // AbandonAll -SessionID
    virtual bool abandon_all(const std::string& session_id) = 0;
    // SetPrimary -SessionID -TypeID
    virtual bool set_primary(const std::string& session_id, const TypeID& type_id) = 0;
    // Place -SessionID -TypeID -X -Y
    virtual bool place(const std::string& session_id, const TypeID& type_id, Cell cell) = 0;
    // SpecialPlace -SessionID -TypeID -X -Y
    virtual bool special_place(const std::string& session_id, const TypeID& type_id, Cell cell) = 0;
    // Repair -SessionID -Address
    virtual bool repair(const std::string& session_id, const ObjectAddress& addr) = 0;
    // Sell -SessionID -Address
    virtual bool sell(const std::string& session_id, const ObjectAddress& addr) = 0;
    // SellCell -SessionID -X -Y
    virtual bool sell_cell(const std::string& session_id, Cell cell) = 0;
    // Ally -SessionID -TargetHouse
    virtual bool ally(const std::string& session_id, HouseId target_house) = 0;
    // Cheer -SessionID
    virtual bool cheer(const std::string& session_id) = 0;

    // ── 查询层（宏观）──
    // GetHouseInfo [-House]（默认会话 House）
    virtual std::optional<HouseInfo> get_house_info(const std::string& session_id,
                                                    HouseId house = -1) = 0;
    // GetForceDistribution [-House]
    virtual std::optional<ForceDistribution> get_force_distribution(const std::string& session_id,
                                                                    HouseId house = -1) = 0;
    // GetMapInfo
    virtual std::optional<MapInfo> get_map_info(const std::string& session_id) = 0;
    // GetTechTree [-House]
    virtual std::vector<TypeID> get_tech_tree(const std::string& session_id,
                                              HouseId house = -1) = 0;
    // GetTypeInfo -TypeID
    virtual std::string get_type_info(const std::string& session_id,
                                      const TypeID& type_id) = 0;   // 返回原始 JSON
    // GetProductionState
    virtual std::optional<ProductionState> get_production_state(const std::string& session_id) = 0;
    // GetCurrentTime
    virtual std::optional<GameTime> get_current_time(const std::string& session_id) = 0;
    // GetPlayerViewRoute -FromTime -ToTime
    virtual std::vector<ViewPoint> get_player_view_route(const std::string& session_id,
                                                         double from_time, double to_time) = 0;

    // ── 查询层（微观）──
    // GetTechnoInRange -X -Y -Range [-WhatAmI] [-House] [-Type] [-Limit] [-Offset]
    virtual std::vector<TechnoInfo> get_techno_in_range(const std::string& session_id,
                                                        Cell center, int range,
                                                        ObjectKind filter = ObjectKind::Unknown,
                                                        HouseId house = -1,
                                                        const TypeID& type = "",
                                                        int limit = 50, int offset = 0) = 0;
    // GetTechnoInfo -Address
    virtual std::optional<TechnoInfo> get_techno_info(const std::string& session_id,
                                                      const ObjectAddress& addr) = 0;
    // GetObjectsOnCell -X -Y
    virtual std::vector<ObjectAddress> get_objects_on_cell(const std::string& session_id,
                                                           Cell cell) = 0;
    // GetAerialObjects -X -Y [-Range]
    virtual std::vector<ObjectAddress> get_aerial_objects(const std::string& session_id,
                                                          Cell center, int range) = 0;
    // GetCellInfo -X -Y
    virtual std::optional<CellInfo> get_cell_info(const std::string& session_id, Cell cell) = 0;
    // FindPath -FromX -FromY -ToX -ToY [-TechnoTypeID]
    virtual PathResult find_path(const std::string& session_id,
                                 Cell from, Cell to,
                                 const TypeID& techno_type_id = "") = 0;
};

} // namespace ihagent