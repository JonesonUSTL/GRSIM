#include "gptsolver/solver/linear/eigen_direct.hpp"

#include <iostream>

#include "gptsolver/solver/linear/petsc_backend.hpp"

namespace gptsolver {
DenseVector solve_linear_sparse(const SparseMatrix& k, const DenseVector& f) {
  if (get_linear_solver_backend() == "petsc") {
    bool ok = false;
    auto x = solve_linear_petsc(k, f, ok);
    if (ok) return x;
    std::cerr << "[gptsolver] PETSc backend 不可用，自动回退 Eigen。\n";
  }

  Eigen::SimplicialLDLT<SparseMatrix> ldlt;
  ldlt.compute(k);
  if (ldlt.info() != Eigen::Success) return DenseVector::Zero(f.size());
  return ldlt.solve(f);
}
}  // namespace gptsolver
