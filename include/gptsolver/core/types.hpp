#pragma once
#include <string>
#include <vector>

namespace gptsolver {

struct RunOptions {
  std::string input_file;
  std::string output_dir;
  int threads{1};
  std::string backend{"eigen"};
  std::string resume_checkpoint;
};

struct StatusSnapshot {
  std::string job_name;
  std::string input_file;
  std::string output_dir;
  int step{0};
  int increment{0};
  int iteration{0};
  double residual_norm{0.0};
  double increment_norm{0.0};
  double energy_error{0.0};
  int linear_iterations{0};
  double elapsed_sec{0.0};
  int contact_active{0};
  double max_penetration{0.0};
  int latest_frame{0};
};

}  // namespace gptsolver
