#pragma once

#include <array>

#include <Eigen/Dense>

namespace gptsolver {

double j2_demo_update(double trial_stress, double yield_stress, double hardening);

struct J2State {
  double eqp{0.0};
  std::array<double, 6> alpha{};  // 随动硬化背应力（Voigt）
  double temperature{293.15};
};

struct J2Material {
  double E{210e3};
  double nu{0.3};
  double sigma_y0{250.0};
  double H_iso{1000.0};
  double C_kin{200.0};
  double gamma_kin{5.0};
  double dsy_dT{0.0};
  double refT{293.15};
};

struct J2UpdateResult {
  std::array<double, 6> stress{};  // Voigt: xx yy zz xy yz xz
  std::array<double, 6> alpha{};
  double dgamma{0.0};
  double sigma_eq{0.0};
  double sigma_y{0.0};
  bool yielded{false};
};

/**
 * @brief 小变形 J2 径向回归（各向同性 + 随动硬化 + 温度屈服修正）。
 * @param trial_stress 试应力
 * @param state 等效塑性应变与背应力状态（将被更新）
 */
J2UpdateResult j2_radial_return(const std::array<double, 6>& trial_stress, const J2Material& mat, J2State& state);

/**
 * @brief 基于返回映射状态给出 6x6 一致切线近似（Voigt）。
 */
Eigen::Matrix<double, 6, 6> j2_consistent_tangent(const J2Material& mat, const J2State& state,
                                                  const J2UpdateResult& update);

/**
 * @brief 线弹性 6x6 矩阵（Voigt）。
 */
Eigen::Matrix<double, 6, 6> isotropic_elastic_matrix(double E, double nu);

}  // namespace gptsolver
