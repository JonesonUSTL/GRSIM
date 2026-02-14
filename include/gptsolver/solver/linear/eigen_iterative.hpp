#pragma once
#include "gptsolver/assembly/csr_matrix.hpp"

namespace gptsolver {
struct IterativeResult {
  DenseVector x;
  int iterations{0};
  double error{0.0};
};
IterativeResult solve_linear_cg(const SparseMatrix& k, const DenseVector& f, int max_iter = 500);
IterativeResult solve_linear_cg_mpi(const SparseMatrix& k, const DenseVector& f, int max_iter = 500);
}
