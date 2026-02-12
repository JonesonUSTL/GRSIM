#pragma once
#include <Eigen/Dense>

namespace gptsolver {
Eigen::MatrixXd build_thermal_matrix(int n);
Eigen::VectorXd build_thermal_rhs(int n);
}
