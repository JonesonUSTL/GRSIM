#include <cassert>

#include "gptsolver/fem/element/shell_placeholder.hpp"

int main() {
  const auto k = gptsolver::shell_hourglass_stiffness(0.01, 8.0e4, 1.2);
  assert(k > 0.0);
  assert(gptsolver::shell_integration_points(true) == 1);
  assert(gptsolver::shell_integration_points(false) == 4);
  return 0;
}
