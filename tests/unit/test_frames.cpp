#include <cassert>
#include <filesystem>

#include "gptsolver/physics/thermal/thermal_problem.hpp"

int main() {
  const std::string out = "tmp_frames";
  std::filesystem::remove_all(out);
  gptsolver::run_thermal_problem(out, 5);
  int cnt = 0;
  for (auto& p : std::filesystem::directory_iterator(out + "/results/step_1")) {
    if (p.path().extension() == ".vtu") ++cnt;
  }
  assert(cnt == 5);
  return 0;
}
