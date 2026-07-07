// BackendCallbacks.h
// 作用：定义后端 v2 主动推送的回调载荷结构与接收接口。
//       对齐 API-SPEC §5：OnSessionClosed/OnBuildingReady/OnUnitDestroyed/
//       OnUnderAttack/OnChatMessage/OnBeacon。
//       后端通过 PostAsyncRemoteCall 推到会话关联的前端库，
//       前端的 EC Action 入口解析后调用 IBackendCallbacks 对应方法。

#pragma once

#include "IdTypes.h"
#include "Cell.h"
#include <string>

namespace ihagent {

// OnSessionClosed 载荷
struct CBSessionClosed {
    std::string session_id;
    HouseId house = -1;
    std::string reason;        // "timeout" | "released"
};

// OnBuildingReady 载荷
struct CBBuildingReady {
    std::string session_id;
    HouseId house = -1;
    TypeID type_id;
    ObjectAddress address;
    Cell cell = {};
};

// OnUnitDestroyed 载荷
struct CBUnitDestroyed {
    std::string session_id;
    HouseId house = -1;
    TypeID type_id;
    ObjectAddress address;
    ObjectAddress killer_address;   // 暂为 "0x00000000"（后端 STUBS-TODO）
};

// OnUnderAttack 载荷
struct CBUnderAttack {
    std::string session_id;
    HouseId house = -1;
    ObjectAddress victim_address;
    ObjectAddress attacker_address;
    int damage = 0;
};

// OnChatMessage 载荷
struct CBChatMessage {
    std::string session_id;
    HouseId from_house = -1;
    HouseId to_house = -1;
    std::string message;
    bool is_ally = false;
};

// OnBeacon 载荷
struct CBBeacon {
    std::string session_id;
    HouseId house = -1;
    std::string action;        // "place" | "remove"
    Cell cell = {};
};

// 回调接收接口（Coordinator 实现，路由到各 Agent）
class IBackendCallbacks {
public:
    virtual ~IBackendCallbacks() = default;
    virtual void on_session_closed(const CBSessionClosed& cb) = 0;
    virtual void on_building_ready(const CBBuildingReady& cb) = 0;
    virtual void on_unit_destroyed(const CBUnitDestroyed& cb) = 0;
    virtual void on_under_attack(const CBUnderAttack& cb) = 0;
    virtual void on_chat_message(const CBChatMessage& cb) = 0;
    virtual void on_beacon(const CBBeacon& cb) = 0;
};

} // namespace ihagent