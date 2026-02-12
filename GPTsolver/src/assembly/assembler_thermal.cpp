#include "gptsolver/assembly/assembler_thermal.hpp"

namespace gptsolver {
SparseMatrix build_thermal_matrix_sparse(int n) {
  SparseMatrix k(n, n);
  std::vector<Triplet> tri;
  for (int i = 0; i < n; ++i) tri.emplace_back(i, i, 5.0);
  k.setFromTriplets(tri.begin(), tri.end());
  return k;
}

DenseVector build_thermal_rhs(int n) { return DenseVector::Ones(n) * 100.0; }

SparseMatrix build_coupling_ut(int n, double alpha) {
  SparseMatrix c(n, n);
  std::vector<Triplet> tri;
  for (int i = 0; i < n; ++i) tri.emplace_back(i, i, alpha);
  c.setFromTriplets(tri.begin(), tri.end());
  return c;
}
}  // namespace gptsolver
