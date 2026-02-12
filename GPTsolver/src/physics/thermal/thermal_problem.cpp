#include "gptsolver/physics/thermal/thermal_problem.hpp"

#include <filesystem>

#include "gptsolver/assembly/assembler_thermal.hpp"
#include "gptsolver/io/vtk/vtu_writer.hpp"
#include "gptsolver/solver/linear/eigen_direct.hpp"

namespace gptsolver {
void run_thermal_problem(const std::string& out_dir) {
  std::filesystem::create_directories(out_dir + "/results/step_1");
  auto k = build_thermal_matrix(4);
  auto q = build_thermal_rhs(4);
  auto t = solve_linear(k, q);
  std::vector<double> coords = {0,0,0,1,0,0,2,0,0,3,0,0};
  std::vector<double> temp(t.data(), t.data() + t.size());
  vtkio::write_simple_vtu(out_dir + "/results/step_1/frame_0001.vtu", coords, temp, "TEMP");
}
}  // namespace gptsolver
