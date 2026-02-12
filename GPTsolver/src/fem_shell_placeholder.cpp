#include "gptsolver/fem/element/shell_placeholder.hpp"

namespace gptsolver {

double shell_hourglass_stiffness(double thickness, double shear_modulus, double area) {
  return 0.1 * thickness * shear_modulus * area;
}

int shell_integration_points(bool reduced) { return reduced ? 1 : 4; }

}  // namespace gptsolver
