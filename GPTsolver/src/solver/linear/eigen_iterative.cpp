#include "gptsolver/solver/linear/eigen_iterative.hpp"

namespace gptsolver {
IterativeResult solve_linear_cg(const SparseMatrix& k, const DenseVector& f, int max_iter) {
  Eigen::ConjugateGradient<SparseMatrix, Eigen::Lower | Eigen::Upper> cg;
  cg.setMaxIterations(max_iter);
  cg.compute(k);
  IterativeResult r;
  r.x = cg.solve(f);
  r.iterations = cg.iterations();
  r.error = cg.error();
  return r;
}
}  // namespace gptsolver
