// IdTypes.h
// 作用：定义全局通用的强类型 ID。v2 对齐后端：单位/建筑地址用 "0x........" 字符串
//       （DWORD 地址十六进制），TeamId 仍为前端内部编组号（int32）。

#pragma once

#include <cstdint>
#include <string>
#include <functional>

namespace ihagent {

// Team 标识，前端内部编组号（介入 Agent 带领的一支单位编组）
struct TeamId {
    int32_t value = -1;       // -1 表示无效
    bool valid() const { return value >= 0; }
    bool operator==(const TeamId& o) const { return value == o.value; }
    bool operator!=(const TeamId& o) const { return value != o.value; }
};

// 用作 unordered_map<TeamId, ...> 的 hash 函数
struct TeamIdHash {
    std::size_t operator()(const TeamId& t) const noexcept {
        return std::hash<int32_t>{}(t.value);
    }
};

// 对象地址（单位/建筑/Techno 通用），格式 "0x........"（十六进制 DWORD）
// 对齐后端 GetTechnoInRange / GetTechnoInfo 等返回的 "addr" 字段
using ObjectAddress = std::string;

// 玩家标识（己方/敌方作战方索引，对齐后端 HouseClass::ArrayIndex）
using HouseId = int32_t;

// 行为链模板 ID（字符串形式，如 "offense_push"、"defense_hold"）
using BehaviorTemplateId = std::string;

// 单位/建筑 TypeID（对齐后端 -TypeID 参数，如 "E1"、"GAPILE"）
using TypeID = std::string;

// 判定地址字符串是否非空且有效（简单非空校验）
inline bool address_valid(const ObjectAddress& a) { return !a.empty(); }

} // namespace ihagent