#pragma once

#include <array>

namespace gptsolver {

double j2_demo_update(double trial_stress, double yield_stress, double hardening);

struct J2State {
  double eqp{0.0};
};

struct J2Material {
  double E{210e3};
  double nu{0.3};
  double sigma_y0{250.0};
  double H{1000.0};
};

struct J2UpdateResult {
  std::array<double, 6> stress{};  // Voigt: xx yy zz xy yz xz
  double dgamma{0.0};
  double sigma_eq{0.0};
  double sigma_y{0.0};
  bool yielded{false};
};

/**
 * @brief 小变形 J2 径向回归（各向同性硬化，演示版）。
 * @param trial_stress 试应力
 * @param state 等效塑性应变状态（将被更新）
 */
J2UpdateResult j2_radial_return(const std::array<double, 6>& trial_stress, const J2Material& mat, J2State& state);

}  // namespace gptsolver
