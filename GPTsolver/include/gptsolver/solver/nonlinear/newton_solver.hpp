#pragma once
#include <Eigen/Dense>

namespace gptsolver {
struct NewtonResult { Eigen::VectorXd x; int iterations; double residual; };
NewtonResult solve_newton_demo(const Eigen::MatrixXd& k, const Eigen::VectorXd& f, int max_iter);
}
