#pragma once
#include <Eigen/Dense>

namespace gptsolver {
Eigen::VectorXd solve_linear(const Eigen::MatrixXd& k, const Eigen::VectorXd& f);
}
