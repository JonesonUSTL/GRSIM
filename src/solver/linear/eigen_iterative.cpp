#include "gptsolver/solver/linear/eigen_iterative.hpp"

#include <cmath>

#ifdef GPTSOLVER_USE_MPI
#include <mpi.h>
#endif

namespace gptsolver {
IterativeResult solve_linear_cg(const SparseMatrix& k, const DenseVector& f, int max_iter) {
  Eigen::ConjugateGradient<SparseMatrix, Eigen::Lower | Eigen::Upper> cg;
  cg.setMaxIterations(max_iter);
  cg.compute(k);
  IterativeResult r;
  r.x = cg.solve(f);
  r.iterations = cg.iterations();
  r.error = cg.error();
  return r;
}

IterativeResult solve_linear_cg_mpi(const SparseMatrix& k, const DenseVector& f, int max_iter) {
#ifdef GPTSOLVER_USE_MPI
  int init = 0;
  MPI_Initialized(&init);
  if (!init) {
    int argc = 0;
    char** argv = nullptr;
    MPI_Init(&argc, &argv);
  }
  DenseVector x = DenseVector::Zero(f.size());
  DenseVector r = f - k * x;
  DenseVector p = r;

  auto dot_mpi = [](const DenseVector& a, const DenseVector& b) {
    const double local = a.dot(b);
    double global = local;
    MPI_Allreduce(&local, &global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return global;
  };

  double rr = dot_mpi(r, r);
  IterativeResult out;
  for (int it = 0; it < max_iter; ++it) {
    DenseVector kp = k * p;
    const double denom = std::max(dot_mpi(p, kp), 1e-16);
    const double alpha = rr / denom;
    x += alpha * p;
    r -= alpha * kp;
    const double rr_new = dot_mpi(r, r);
    out.error = std::sqrt(std::max(0.0, rr_new));
    out.iterations = it + 1;
    if (out.error < 1e-10) break;
    const double beta = rr_new / std::max(rr, 1e-16);
    p = r + beta * p;
    rr = rr_new;
  }
  out.x = x;
  return out;
#else
  return solve_linear_cg(k, f, max_iter);
#endif
}
}  // namespace gptsolver
