#include "gptsolver/assembly/assembler_thermal.hpp"

#include "gptsolver/assembly/assembler_structural.hpp"

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

SparseMatrix build_thermal_expansion_tangent(int n, double young_modulus, double expansion_coeff, double delta_t) {
  SparseMatrix kut(n, n);
  std::vector<Triplet> tri;
  const double coeff = young_modulus * expansion_coeff * delta_t;
  for (int i = 0; i < n; ++i) tri.emplace_back(i, i, coeff);
  kut.setFromTriplets(tri.begin(), tri.end());
  return kut;
}

std::tuple<SparseMatrix, SparseMatrix, SparseMatrix, SparseMatrix> build_thermo_structural_blocks(
    int n, double expansion_coeff, double conductivity_scale, double young_modulus, double delta_t) {
  auto kuu = build_structural_stiffness_sparse(n);
  auto ktt = build_thermal_matrix_sparse(n);
  ktt *= conductivity_scale;
  auto kut = build_thermal_expansion_tangent(n, young_modulus, expansion_coeff, delta_t);
  SparseMatrix ktu = kut.transpose();
  return {kuu, kut, ktu, ktt};
}
}  // namespace gptsolver
