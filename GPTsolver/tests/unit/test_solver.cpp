#include <cassert>

#include "gptsolver/assembly/assembler_structural.hpp"
#include "gptsolver/solver/linear/eigen_direct.hpp"

int main() {
  auto k = gptsolver::build_structural_stiffness(2);
  auto f = gptsolver::build_structural_load(2);
  auto u = gptsolver::solve_linear(k, f);
  assert(u.size() == 2);
  return 0;
}
