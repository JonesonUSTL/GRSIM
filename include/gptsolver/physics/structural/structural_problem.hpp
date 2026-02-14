#pragma once
#include <string>

namespace gptsolver {
void set_schur_blend_weight(double weight);
void run_structural_problem(const std::string& out_dir, int frames = 10);
void run_coupled_thermo_structural_problem(const std::string& out_dir, int frames = 10);
}  // namespace gptsolver
