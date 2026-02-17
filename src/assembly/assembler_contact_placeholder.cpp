#include "gptsolver/assembly/assembler_contact_placeholder.hpp"

#include <algorithm>
#include <cmath>

namespace gptsolver {

void assemble_contact_terms(const std::vector<ContactPointState>& cps, const ContactParams& params, SparseMatrix& kt,
                            DenseVector& rt) {
  std::vector<Triplet> tri;
  for (const auto& cp : cps) {
    if (cp.normal_gap >= 0.0) continue;

    const double pn = -params.kn * cp.normal_gap;
    if (cp.dof_n >= 0 && cp.dof_n < rt.size()) rt(cp.dof_n) += pn;
    if (cp.dof_t >= 0 && cp.dof_t < rt.size()) rt(cp.dof_t) -= pn;

    // 一致法向切线：对 [u_n, u_t] 的 2x2 子块
    tri.emplace_back(cp.dof_n, cp.dof_n, params.kn);
    tri.emplace_back(cp.dof_t, cp.dof_t, params.kn);
    tri.emplace_back(cp.dof_n, cp.dof_t, -params.kn);
    tri.emplace_back(cp.dof_t, cp.dof_n, -params.kn);

    const double fric_lim = std::max(0.0, params.mu * pn);
    const double kt_t = cp.stick ? params.kn * std::max(1e-8, params.stick_stiff_ratio)
                                 : std::max(1e-12, params.slip_stiffness);
    double ft = -kt_t * cp.tangential_slip;
    if (std::abs(cp.tangential_slip) > params.slip_tol) {
      ft = std::clamp(ft, -fric_lim, fric_lim);
    }

    if (cp.dof_t >= 0 && cp.dof_t < rt.size()) rt(cp.dof_t) += ft;
    if (cp.dof_t >= 0 && cp.dof_t < kt.rows()) tri.emplace_back(cp.dof_t, cp.dof_t, kt_t + params.damping);

    if (params.damping > 0.0 && cp.dof_n >= 0 && cp.dof_n < rt.size()) {
      rt(cp.dof_n) += params.damping * std::abs(cp.normal_gap);
    }
  }
  SparseMatrix add(kt.rows(), kt.cols());
  add.setFromTriplets(tri.begin(), tri.end());
  kt += add;
}

}  // namespace gptsolver
