#include "gptsolver/solver/nonlinear/line_search.hpp"

namespace gptsolver {
double backtracking_line_search(const SparseMatrix& k, const DenseVector& rhs, const DenseVector& x, const DenseVector& dx,
                                double lambda) {
  double alpha = 1.0;
  const double r0 = (k * x - lambda * rhs).norm();
  for (int i = 0; i < 8; ++i) {
    const double rt = (k * (x + alpha * dx) - lambda * rhs).norm();
    if (rt < r0) return alpha;
    alpha *= 0.5;
  }
  return alpha;
}
}  // namespace gptsolver
