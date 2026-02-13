#include <cassert>

#include "gptsolver/assembly/assembler_structural.hpp"
#include "gptsolver/assembly/assembler_thermal.hpp"
#include "gptsolver/solver/linear/schur_preconditioner.hpp"

int main() {
  const int n = 4;
  auto kuu = gptsolver::build_structural_stiffness_sparse(n);
  auto ktt = gptsolver::build_thermal_matrix_sparse(n);
  auto kut = gptsolver::build_coupling_ut(n, 0.01);
  auto ktu = gptsolver::build_coupling_ut(n, 0.02);
  gptsolver::DenseVector rhs(2 * n);
  rhs << gptsolver::build_structural_load(n), gptsolver::build_thermal_rhs(n);
  auto x = gptsolver::solve_block_schur(kuu, kut, ktu, ktt, rhs);
  assert(x.size() == 2 * n);
  return 0;
}
