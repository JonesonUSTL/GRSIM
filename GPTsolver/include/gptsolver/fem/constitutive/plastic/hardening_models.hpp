#pragma once

namespace gptsolver {

/**
 * @brief 线性各向同性硬化：sigma_y = sigma_y0 + H * eqp
 */
inline double linear_isotropic_yield(double sigma_y0, double hardening_modulus, double eqp) {
  return sigma_y0 + hardening_modulus * eqp;
}

}  // namespace gptsolver
