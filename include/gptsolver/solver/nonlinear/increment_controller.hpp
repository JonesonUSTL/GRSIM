#pragma once
namespace gptsolver {
/**
 * @brief 根据收敛行为调整弧长半径。
 */
double update_arc_radius(double current_radius, bool converged, double min_radius, double max_radius);
}
