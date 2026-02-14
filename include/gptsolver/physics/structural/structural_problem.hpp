#pragma once
#include <string>

namespace gptsolver {

struct ContactRuntimeControls {
  bool enable_general_contact{false};
  double penalty{1.0e4};
  double friction{0.2};
  double damping{0.0};
  double slip_tolerance{1e-2};
  double stick_stiff_ratio{0.1};
  double slip_stiffness{1e-8};
};

void set_contact_runtime_controls(const ContactRuntimeControls& controls);
void set_schur_blend_weight(double weight);
void run_structural_problem(const std::string& out_dir, int frames = 10);
void run_coupled_thermo_structural_problem(const std::string& out_dir, int frames = 10);
}  // namespace gptsolver
