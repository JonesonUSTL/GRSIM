#include <cassert>

#include "gptsolver/fem/element/shell_placeholder.hpp"

int main() {
  const auto k = gptsolver::shell_hourglass_stiffness(0.01, 8.0e4, 1.2);
  assert(k > 0.0);
  assert(gptsolver::shell_integration_points(true) == 1);
  assert(gptsolver::shell_integration_points(false) == 4);

  const auto reduced = gptsolver::shell_integration_rule(true);
  assert(reduced.points.size() == 1 && reduced.weights.size() == 1);
  assert(reduced.weights[0] > 0.0);

  const auto full = gptsolver::shell_integration_rule(false);
  assert(full.points.size() == 4 && full.weights.size() == 4);

  const double khg_solid = gptsolver::solid_hourglass_scale(0.125, 8.0e4);
  assert(khg_solid > 0.0);
  return 0;
}
