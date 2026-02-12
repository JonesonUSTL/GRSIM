#include "gptsolver/assembly/assembler_thermal.hpp"

namespace gptsolver {
Eigen::MatrixXd build_thermal_matrix(int n) {
  return Eigen::MatrixXd::Identity(n, n) * 5.0;
}

Eigen::VectorXd build_thermal_rhs(int n) {
  return Eigen::VectorXd::Ones(n) * 100.0;
}
}  // namespace gptsolver
