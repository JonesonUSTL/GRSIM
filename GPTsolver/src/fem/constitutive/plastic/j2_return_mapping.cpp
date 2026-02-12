#include "gptsolver/fem/constitutive/plastic/j2_return_mapping.hpp"

namespace gptsolver {
double j2_demo_update(double trial_stress, double yield_stress, double hardening) {
  if (trial_stress <= yield_stress) return trial_stress;
  return yield_stress + hardening * (trial_stress - yield_stress);
}
}  // namespace gptsolver
