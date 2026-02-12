#include "gptsolver/solver/linear/eigen_direct.hpp"

namespace gptsolver {
Eigen::VectorXd solve_linear(const Eigen::MatrixXd& k, const Eigen::VectorXd& f) { return k.colPivHouseholderQr().solve(f); }
}  // namespace gptsolver
