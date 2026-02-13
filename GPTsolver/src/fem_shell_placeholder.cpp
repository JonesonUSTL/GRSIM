#include "gptsolver/fem/element/shell_placeholder.hpp"

#include <algorithm>
#include <cmath>

namespace gptsolver {

double shell_hourglass_stiffness(double thickness, double shear_modulus, double area) {
  const double t = std::max(thickness, 1e-12);
  const double a = std::max(area, 1e-12);
  return 0.1 * t * shear_modulus * a;
}

int shell_integration_points(bool reduced) { return reduced ? 1 : 4; }

ShellIntegrationRule shell_integration_rule(bool reduced) {
  ShellIntegrationRule rule;
  if (reduced) {
    rule.points = {{{0.0, 0.0}}};
    rule.weights = {4.0};
    return rule;
  }

  const double g = 1.0 / std::sqrt(3.0);
  rule.points = {{{-g, -g}}, {{g, -g}}, {{g, g}}, {{-g, g}}};
  rule.weights = {1.0, 1.0, 1.0, 1.0};
  return rule;
}

double solid_hourglass_scale(double volume, double shear_modulus, double alpha) {
  const double v = std::max(volume, 1e-12);
  const double a = std::max(alpha, 0.0);
  return a * shear_modulus * v;
}

}  // namespace gptsolver
