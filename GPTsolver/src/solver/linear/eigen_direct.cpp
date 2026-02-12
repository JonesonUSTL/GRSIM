#include "gptsolver/solver/linear/eigen_direct.hpp"

namespace gptsolver {
DenseVector solve_linear_sparse(const SparseMatrix& k, const DenseVector& f) {
  Eigen::SimplicialLDLT<SparseMatrix> ldlt;
  ldlt.compute(k);
  if (ldlt.info() != Eigen::Success) return DenseVector::Zero(f.size());
  return ldlt.solve(f);
}
}  // namespace gptsolver
