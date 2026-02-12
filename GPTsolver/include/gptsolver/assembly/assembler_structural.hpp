#pragma once
#include <Eigen/Dense>

namespace gptsolver {
Eigen::MatrixXd build_structural_stiffness(int n);
Eigen::VectorXd build_structural_load(int n);
}
