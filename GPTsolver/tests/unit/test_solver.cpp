#include <cassert>

#include "gptsolver/assembly/assembler_structural.hpp"
#include "gptsolver/solver/linear/eigen_direct.hpp"

int main() {
  auto k = gptsolver::build_structural_stiffness_sparse(4);
  auto f = gptsolver::build_structural_load(4);
  auto u = gptsolver::solve_linear_sparse(k, f);
  assert(u.size() == 4);
  return 0;
}
