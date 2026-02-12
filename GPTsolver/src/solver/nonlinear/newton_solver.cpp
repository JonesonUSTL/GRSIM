#include "gptsolver/solver/nonlinear/newton_solver.hpp"

namespace gptsolver {
NewtonResult solve_newton_demo(const Eigen::MatrixXd& k, const Eigen::VectorXd& f, int max_iter) {
  Eigen::VectorXd x = Eigen::VectorXd::Zero(f.size());
  double res = (k * x - f).norm();
  int iter = 0;
  for (; iter < max_iter && res > 1e-10; ++iter) {
    const Eigen::VectorXd dx = k.colPivHouseholderQr().solve(f - k * x);
    x += dx;
    res = (k * x - f).norm();
  }
  return {x, iter, res};
}
}  // namespace gptsolver
