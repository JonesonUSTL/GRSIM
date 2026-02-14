#include "gptsolver/fem/constitutive/plastic/j2_return_mapping.hpp"


#include <algorithm>
#include <cmath>

#include <Eigen/Dense>

namespace gptsolver {
namespace {
std::array<double, 6> deviator(const std::array<double, 6>& s) {
  const double m = (s[0] + s[1] + s[2]) / 3.0;
  return {s[0] - m, s[1] - m, s[2] - m, s[3], s[4], s[5]};
}

std::array<double, 6> sub6(const std::array<double, 6>& a, const std::array<double, 6>& b) {
  return {a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3], a[4] - b[4], a[5] - b[5]};
}

std::array<double, 6> add6(const std::array<double, 6>& a, const std::array<double, 6>& b) {
  return {a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3], a[4] + b[4], a[5] + b[5]};
}

std::array<double, 6> mul6(const std::array<double, 6>& a, double s) {
  return {a[0] * s, a[1] * s, a[2] * s, a[3] * s, a[4] * s, a[5] * s};
}

double j2_norm(const std::array<double, 6>& sdev) {
  const double j2 = 0.5 * (sdev[0] * sdev[0] + sdev[1] * sdev[1] + sdev[2] * sdev[2]) +
                    sdev[3] * sdev[3] + sdev[4] * sdev[4] + sdev[5] * sdev[5];
  return std::sqrt(std::max(0.0, 3.0 * j2));
}

double temperature_corrected_yield(const J2Material& mat, const J2State& state) {
  return mat.sigma_y0 + mat.H_iso * state.eqp - mat.dsy_dT * (state.temperature - mat.refT);
}
}  // namespace

double j2_demo_update(double trial_stress, double yield_stress, double hardening) {
  if (trial_stress <= yield_stress) return trial_stress;
  return yield_stress + hardening * (trial_stress - yield_stress);
}

J2UpdateResult j2_radial_return(const std::array<double, 6>& trial_stress, const J2Material& mat, J2State& state) {
  J2UpdateResult out;
  out.stress = trial_stress;
  out.alpha = state.alpha;

  const double G = mat.E / (2.0 * (1.0 + mat.nu));
  const auto sdev = deviator(trial_stress);
  const auto eta = sub6(sdev, state.alpha);
  const double seq_tr = j2_norm(eta);
  const double sy = temperature_corrected_yield(mat, state);

  out.sigma_eq = seq_tr;
  out.sigma_y = sy;

  if (seq_tr <= sy + 1e-12) {
    out.yielded = false;
    return out;
  }

  const double h_kin = (2.0 / 3.0) * mat.C_kin;
  const double h_total = mat.H_iso + h_kin;
  const double dgamma = (seq_tr - sy) / (3.0 * G + h_total);

  const double inv = std::max(seq_tr, 1e-12);
  auto n = mul6(eta, 1.0 / inv);
  const double scale = std::max(0.0, 1.0 - (3.0 * G * dgamma) / inv);

  const double p = (trial_stress[0] + trial_stress[1] + trial_stress[2]) / 3.0;
  auto s_new = mul6(sdev, scale);
  out.stress = {p + s_new[0], p + s_new[1], p + s_new[2], s_new[3], s_new[4], s_new[5]};

  auto alpha_new = add6(state.alpha, mul6(n, h_kin * dgamma));
  alpha_new = add6(alpha_new, mul6(state.alpha, -mat.gamma_kin * dgamma));

  state.alpha = alpha_new;
  state.eqp += dgamma;

  out.alpha = state.alpha;
  out.dgamma = dgamma;
  out.sigma_y = temperature_corrected_yield(mat, state);
  out.sigma_eq = j2_norm(sub6(deviator(out.stress), out.alpha));
  out.yielded = true;
  return out;
}


Eigen::Matrix<double, 6, 6> isotropic_elastic_matrix(double E, double nu) {
  Eigen::Matrix<double, 6, 6> C = Eigen::Matrix<double, 6, 6>::Zero();
  const double lmbda = E * nu / ((1.0 + nu) * (1.0 - 2.0 * nu));
  const double G = E / (2.0 * (1.0 + nu));
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) C(i, j) = (i == j) ? (lmbda + 2.0 * G) : lmbda;
  }
  C(3, 3) = G;
  C(4, 4) = G;
  C(5, 5) = G;
  return C;
}

Eigen::Matrix<double, 6, 6> j2_consistent_tangent(const J2Material& mat, const J2State& state,
                                                  const J2UpdateResult& update) {
  const auto Ce = isotropic_elastic_matrix(mat.E, mat.nu);
  if (!update.yielded) return Ce;

  const auto sdev = deviator(update.stress);
  const auto eta = sub6(sdev, state.alpha);
  const double nrm = std::max(j2_norm(eta), 1e-12);
  Eigen::Matrix<double, 6, 1> n;
  n << eta[0] / nrm, eta[1] / nrm, eta[2] / nrm, eta[3] / nrm, eta[4] / nrm, eta[5] / nrm;

  const double G = mat.E / (2.0 * (1.0 + mat.nu));
  const double h_kin = (2.0 / 3.0) * mat.C_kin;
  const double H = mat.H_iso + h_kin;
  const double denom = std::max(3.0 * G + H, 1e-12);

  Eigen::Matrix<double, 6, 6> Cep = Ce - ((6.0 * G * G) / denom) * (n * n.transpose());
  return Cep;
}

}  // namespace gptsolver
