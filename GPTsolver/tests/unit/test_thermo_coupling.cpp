#include <cassert>
#include <cmath>

#include "gptsolver/assembly/assembler_thermal.hpp"

int main() {
  auto kut = gptsolver::build_thermal_expansion_tangent(8, 2.1e5, 1.2e-5, 50.0);
  assert(kut.rows() == 8 && kut.cols() == 8);
  assert(kut.coeff(0, 0) > 0.0);

  auto [kuu, k_ut, k_tu, ktt] = gptsolver::build_thermo_structural_blocks(8, 1.2e-5, 1.0, 2.1e5, 100.0);
  assert(kuu.rows() == 8 && ktt.rows() == 8);
  assert(k_ut.coeff(0, 0) > 0.0);
  assert(std::abs(k_tu.coeff(0, 0) - k_ut.coeff(0, 0)) < 1e-12);
  return 0;
}
