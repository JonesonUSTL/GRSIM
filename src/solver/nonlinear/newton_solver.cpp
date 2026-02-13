#include "gptsolver/solver/nonlinear/newton_solver.hpp"

#include "gptsolver/solver/linear/eigen_direct.hpp"
#include "gptsolver/solver/nonlinear/increment_controller.hpp"
#include "gptsolver/solver/nonlinear/line_search.hpp"

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
      const double alpha = backtracking_line_search(k, fext, r.x, du, trial_l);
      r.x = r.x + alpha * du;
      lambda = trial_l;
      radius = update_arc_radius(radius, true, opt.radius_min, opt.radius_max);
      cutback = 0;
    } else {
      radius = update_arc_radius(radius, false, opt.radius_min, opt.radius_max);
      ++cutback;
      if (cutback > opt.max_cutback) break;
    }
  }

  r.load_factor = lambda;
  return r;
}

}  // namespace gptsolver
