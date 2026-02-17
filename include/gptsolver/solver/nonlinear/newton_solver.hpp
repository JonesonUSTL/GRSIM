#pragma once
#include "gptsolver/assembly/csr_matrix.hpp"

namespace gptsolver {

struct NewtonResult {
  DenseVector x;
  int iterations{0};
  double residual{0.0};
  double load_factor{1.0};
  bool converged{false};
};

struct ArcLengthOptions {
  int max_iter{20};
  int max_cutback{4};
  double radius{0.2};
  double radius_min{0.02};
  double radius_max{1.0};
};

NewtonResult solve_newton_with_arclength(const SparseMatrix& k, const DenseVector& fext, const DenseVector& x0,
                                         const ArcLengthOptions& opt);

}  // namespace gptsolver
