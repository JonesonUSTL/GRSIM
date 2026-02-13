#include "gptsolver/assembly/assembler_structural.hpp"

namespace gptsolver {

SparseMatrix build_structural_stiffness_sparse(int n) {
  SparseMatrix k(n, n);
  std::vector<Triplet> tri;
  tri.reserve(static_cast<size_t>(n));
#ifdef GPTSOLVER_USE_OPENMP
#pragma omp parallel
  {
    std::vector<Triplet> local;
#pragma omp for nowait
    for (int i = 0; i < n; ++i) local.emplace_back(i, i, 10.0 + (i % 5));
#pragma omp critical
    tri.insert(tri.end(), local.begin(), local.end());
  }
#else
  for (int i = 0; i < n; ++i) tri.emplace_back(i, i, 10.0 + (i % 5));
#endif
  k.setFromTriplets(tri.begin(), tri.end());
  return k;
}

DenseVector build_structural_load(int n) {
  DenseVector f = DenseVector::Zero(n);
  if (n > 0) f(n - 1) = 1.0;
  return f;
}

SparseMatrix assemble_mpc_lagrange(int ndof, int master, int slave, double ratio, double penalty) {
  SparseMatrix k(ndof, ndof);
  std::vector<Triplet> tri;
  tri.emplace_back(master, master, penalty);
  tri.emplace_back(slave, slave, penalty * ratio * ratio);
  tri.emplace_back(master, slave, -penalty * ratio);
  tri.emplace_back(slave, master, -penalty * ratio);
  k.setFromTriplets(tri.begin(), tri.end());
  return k;
}

}  // namespace gptsolver
