#include "gptsolver/solver/nonlinear/increment_controller.hpp"

#include <algorithm>

namespace gptsolver {
double update_arc_radius(double current_radius, bool converged, double min_radius, double max_radius) {
  if (converged) return std::min(max_radius, current_radius * 1.2);
  return std::max(min_radius, current_radius * 0.5);
}
}  // namespace gptsolver
