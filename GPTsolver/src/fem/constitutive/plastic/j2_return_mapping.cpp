#include "gptsolver/fem/constitutive/plastic/j2_return_mapping.hpp"

#include "gptsolver/fem/constitutive/plastic/hardening_models.hpp"

#include <algorithm>
#include <cmath>

namespace gptsolver {
namespace {
std::array<double, 6> deviator(const std::array<double, 6>& s) {
  const double m = (s[0] + s[1] + s[2]) / 3.0;
  return {s[0] - m, s[1] - m, s[2] - m, s[3], s[4], s[5]};
}

double j2_norm(const std::array<double, 6>& sdev) {
  const double j2 = 0.5 * (sdev[0] * sdev[0] + sdev[1] * sdev[1] + sdev[2] * sdev[2]) +
                    sdev[3] * sdev[3] + sdev[4] * sdev[4] + sdev[5] * sdev[5];
  return std::sqrt(std::max(0.0, 3.0 * j2));
}
}  // namespace

double j2_demo_update(double trial_stress, double yield_stress, double hardening) {
  if (trial_stress <= yield_stress) return trial_stress;
  return yield_stress + hardening * (trial_stress - yield_stress);
}

J2UpdateResult j2_radial_return(const std::array<double, 6>& trial_stress, const J2Material& mat, J2State& state) {
  J2UpdateResult out;
  out.stress = trial_stress;

  const double G = mat.E / (2.0 * (1.0 + mat.nu));
  const auto sdev = deviator(trial_stress);
  const double seq_tr = j2_norm(sdev);
  const double sy = linear_isotropic_yield(mat.sigma_y0, mat.H, state.eqp);
  out.sigma_eq = seq_tr;
  out.sigma_y = sy;

  if (seq_tr <= sy + 1e-12) {
    out.yielded = false;
    return out;
  }

  const double dgamma = (seq_tr - sy) / (3.0 * G + mat.H);
  const double scale = std::max(0.0, 1.0 - (3.0 * G * dgamma) / seq_tr);

  const double p = (trial_stress[0] + trial_stress[1] + trial_stress[2]) / 3.0;
  out.stress = {p + scale * sdev[0], p + scale * sdev[1], p + scale * sdev[2],
                scale * sdev[3],     scale * sdev[4],     scale * sdev[5]};

  out.dgamma = dgamma;
  state.eqp += dgamma;
  out.sigma_y = linear_isotropic_yield(mat.sigma_y0, mat.H, state.eqp);
  out.sigma_eq = j2_norm(deviator(out.stress));
  out.yielded = true;
  return out;
}

}  // namespace gptsolver
