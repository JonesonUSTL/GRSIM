#pragma once
#include <vector>

namespace gptsolver {

/**
 * @brief 壳单元 hourglass 稳定项计算（占位演示）。
 *
 * 说明：当前仅返回与厚度、剪切模量相关的简化稳定因子，
 * 后续将替换为多积分点 + assumed strain/B-bar 形式。
 */
double shell_hourglass_stiffness(double thickness, double shear_modulus, double area);

/**
 * @brief 返回壳单元积分点数量建议（占位）。
 */
int shell_integration_points(bool reduced);

}  // namespace gptsolver
