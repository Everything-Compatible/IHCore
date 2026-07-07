// Cell.h
// 作用：定义单元格坐标类型，与后端 EventClass/MapClass 的 CellStruct 对齐。
//       后端所有坐标均为整数格子坐标（short X/Y），前端不再使用浮点 Vec2。

#pragma once

#include <cstdint>

namespace ihagent {

// 单元格坐标（对齐后端 CellStruct）
struct Cell {
    int32_t X = 0;
    int32_t Y = 0;
};

// 判定两个单元格是否相同
inline bool operator==(const Cell& a, const Cell& b) { return a.X == b.X && a.Y == b.Y; }
inline bool operator!=(const Cell& a, const Cell& b) { return !(a == b); }

} // namespace ihagent