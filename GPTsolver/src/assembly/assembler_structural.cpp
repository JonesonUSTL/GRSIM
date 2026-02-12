#include "gptsolver/assembly/assembler_structural.hpp"

namespace gptsolver {
Eigen::MatrixXd build_structural_stiffness(int n) {
  Eigen::MatrixXd k = Eigen::MatrixXd::Zero(n, n);
#ifdef GPTSOLVER_USE_OPENMP
#pragma omp parallel for
#endif
  for (int i = 0; i < n; ++i) {
    k(i, i) = 10.0;
  }
  return k;
}

Eigen::VectorXd build_structural_load(int n) {
  Eigen::VectorXd f = Eigen::VectorXd::Zero(n);
  if (n > 0) f(n - 1) = 1.0;
  return f;
}
}  // namespace gptsolver
