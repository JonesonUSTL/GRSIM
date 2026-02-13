#include "gptsolver/assembly/assembler_contact_placeholder.hpp"

namespace gptsolver {

void assemble_contact_terms(const std::vector<ContactPointState>& cps, const ContactParams& params, SparseMatrix& kt,
                            DenseVector& rt) {
  std::vector<Triplet> tri;
  for (const auto& cp : cps) {
    if (cp.normal_gap >= 0.0) continue;
    const double pn = -params.kn * cp.normal_gap;
    if (cp.dof_n < rt.size()) rt(cp.dof_n) += pn;
    tri.emplace_back(cp.dof_n, cp.dof_n, params.kn);

    const double fric_lim = params.mu * pn;
    const double kt_t = cp.stick ? params.kn * 0.1 : 1e-8;
    double ft = -kt_t * cp.tangential_slip;
    if (ft > fric_lim) ft = fric_lim;
    if (ft < -fric_lim) ft = -fric_lim;
    if (cp.dof_t < rt.size()) rt(cp.dof_t) += ft;
    tri.emplace_back(cp.dof_t, cp.dof_t, kt_t);
  }
  SparseMatrix add(kt.rows(), kt.cols());
  add.setFromTriplets(tri.begin(), tri.end());
  kt += add;
}

}  // namespace gptsolver
