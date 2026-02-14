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

SolidIntegrationRule solid_c3d8_integration_rule(bool reduced) {
  SolidIntegrationRule rule;
  if (reduced) {
    rule.points = {{{0.0, 0.0, 0.0}}};
    rule.weights = {8.0};
    return rule;
  }

  const double g = 1.0 / std::sqrt(3.0);
  for (double xi : {-g, g}) {
    for (double eta : {-g, g}) {
      for (double zeta : {-g, g}) {
        rule.points.push_back({xi, eta, zeta});
        rule.weights.push_back(1.0);
      }
    }
  }
  return rule;
}

ShellIntegrationRule s4_integration_rule(bool reduced) { return shell_integration_rule(reduced); }

SolidIntegrationRule c3d8r_integration_rule(bool reduced) { return solid_c3d8_integration_rule(reduced); }

double solid_hourglass_scale(double volume, double shear_modulus, double alpha) {
  const double v = std::max(volume, 1e-12);
  const double a = std::max(alpha, 0.0);
  return a * shear_modulus * v;
}

double hourglass_energy(double strain_energy, const HourglassControl& ctrl) {
  const double e = std::max(strain_energy, 0.0);
  const double scale = ctrl.enhanced ? 1.5 : 1.0;
  return scale * ctrl.alpha * e;
}

}  // namespace gptsolver
