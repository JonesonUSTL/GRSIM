#include "gptsolver/solver/linear/schur_preconditioner.hpp"

#include <Eigen/SparseLU>

namespace gptsolver {

DenseVector solve_block_schur(const SparseMatrix& kuu, const SparseMatrix& kut, const SparseMatrix& ktu,
                              const SparseMatrix& ktt, const DenseVector& rhs) {
  const int nu = static_cast<int>(kuu.rows());
  const int nt = static_cast<int>(ktt.rows());
  DenseVector ru = rhs.head(nu);
  DenseVector rt = rhs.tail(nt);

  Eigen::SimplicialLDLT<SparseMatrix> luu;
  luu.compute(kuu);
  if (luu.info() != Eigen::Success) return DenseVector::Zero(rhs.size());

  // x_u = Kuu^-1 * ru
  DenseVector xu0 = luu.solve(ru);
  // 近似 Schur: S = Ktt - Ktu*diag(Kuu)^-1*Kut（演示简化）
  SparseMatrix dInv(nu, nu);
  std::vector<Triplet> dtri;
  dtri.reserve(nu);
  for (int i = 0; i < nu; ++i) {
    const double d = kuu.coeff(i, i);
    dtri.emplace_back(i, i, std::abs(d) > 1e-14 ? 1.0 / d : 1.0);
  }
  dInv.setFromTriplets(dtri.begin(), dtri.end());
  SparseMatrix S = ktt - ktu * dInv * kut;
  DenseVector rs = rt - ktu * xu0;

  Eigen::ConjugateGradient<SparseMatrix, Eigen::Lower | Eigen::Upper> cg;
  cg.compute(S);
  DenseVector xt = cg.solve(rs);

  DenseVector xu = luu.solve(ru - kut * xt);
  DenseVector x(rhs.size());
  x << xu, xt;
  return x;
}

}  // namespace gptsolver
