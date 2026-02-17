#pragma once
#include <array>
#include <optional>

namespace gptsolver {

/**
 * @brief 三角面（最小几何体），用于接触窄相检测演示。
 */
struct TriangleFace {
  std::array<double, 3> a{};
  std::array<double, 3> b{};
  std::array<double, 3> c{};
};

/**
 * @brief 点到三角面的正交投影结果。
 */
struct ProjectionResult {
  std::array<double, 3> proj{};
  double gap{0.0};
  bool inside{false};
};

/**
 * @brief 将点投影到三角面平面，并判断投影是否落在三角形内部。
 */
std::optional<ProjectionResult> project_point_to_face(const std::array<double, 3>& p, const TriangleFace& f);

}  // namespace gptsolver
