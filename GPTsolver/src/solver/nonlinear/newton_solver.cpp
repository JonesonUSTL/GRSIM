#include "gptsolver/solver/nonlinear/newton_solver.hpp"

#include "gptsolver/solver/linear/eigen_direct.hpp"

namespace gptsolver {

NewtonResult solve_newton_with_arclength(const SparseMatrix& k, const DenseVector& fext, const DenseVector& x0,
                                         const ArcLengthOptions& opt) {
  NewtonResult r;
  r.x = x0;
  double lambda = 0.0;
  double radius = opt.radius;
  int cutback = 0;

  for (int iter = 0; iter < opt.max_iter; ++iter) {
    DenseVector res = k * r.x - lambda * fext;
    const double nrm = res.norm();
    r.residual = nrm;
    r.iterations = iter + 1;
    if (nrm < 1e-8) {
      r.converged = true;
      break;
    }

    DenseVector du = solve_linear_sparse(k, -res);
    const double s = std::sqrt(std::abs(du.dot(du)) + 1e-14);
    if (s > radius) du *= radius / s;
    const double dlam = radius / (fext.norm() + 1e-12);

    DenseVector trial_u = r.x + du;
    double trial_l = lambda + dlam;
    DenseVector trial_res = k * trial_u - trial_l * fext;

    if (trial_res.norm() < nrm) {
      r.x = trial_u;
      lambda = trial_l;
      radius = std::min(opt.radius_max, radius * 1.15);  // RIKS 风格半径放大
      cutback = 0;
    } else {
      radius = std::max(opt.radius_min, radius * 0.5);  // cutback
      ++cutback;
      if (cutback > opt.max_cutback) break;
    }
  }

  r.load_factor = lambda;
  return r;
}

}  // namespace gptsolver
