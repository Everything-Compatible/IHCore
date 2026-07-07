// Geometry.h
// 作用：提供格子坐标的距离等几何工具，规则层决策大量使用（如门限判断）。
//       v2：用整数 Cell 坐标（对齐后端 CellStruct）。

#pragma once

#include "Cell.h"
#include <cmath>
#include <cstdint>

namespace ihagent {

// 计算两格欧式距离（浮点）
inline double distance(const Cell& a, const Cell& b) {
    double dx = a.X - b.X;
    double dy = a.Y - b.Y;
    return std::sqrt(dx * dx + dy * dy);
}

// 计算两格距离平方（避免开方，用于比较）
inline int64_t distance_sq(const Cell& a, const Cell& b) {
    int64_t dx = a.X - b.X;
    int64_t dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}

// 判断 c 是否在以 center 为圆心、radius 为半径的圆内
inline bool within_radius(const Cell& c, const Cell& center, int radius) {
    return distance_sq(c, center) <= static_cast<int64_t>(radius) * radius;
}

// 切比雪夫距离（格子"步数"距离，用于范围估算）
inline int chebyshev(const Cell& a, const Cell& b) {
    int dx = std::abs(a.X - b.X);
    int dy = std::abs(a.Y - b.Y);
    return dx > dy ? dx : dy;
}

} // namespace ihagent