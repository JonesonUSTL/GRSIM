#pragma once
#include <array>
#include <vector>

namespace gptsolver {

/**
 * @brief 壳单元积分配置（演示版，支持 1x1 与 2x2）。
 */
struct ShellIntegrationRule {
  std::vector<std::array<double, 2>> points;  // (xi, eta)
  std::vector<double> weights;
};

/**
 * @brief 壳单元 hourglass 稳定项计算。
 */
double shell_hourglass_stiffness(double thickness, double shear_modulus, double area);

/**
 * @brief 返回壳单元积分点数量建议（reduced: 1, full: 4）。
 */
int shell_integration_points(bool reduced);

/**
 * @brief 获取壳单元二维高斯积分规则。
 */
ShellIntegrationRule shell_integration_rule(bool reduced);

/**
 * @brief 估算实体单元 hourglass 稳定系数（用于 C3D8R/C3D20R 等降阶单元）。
 */
double solid_hourglass_scale(double volume, double shear_modulus, double alpha = 0.06);

}  // namespace gptsolver
