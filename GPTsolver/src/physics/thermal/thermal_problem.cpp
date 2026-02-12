#include "gptsolver/physics/thermal/thermal_problem.hpp"

#include <filesystem>

#include "gptsolver/assembly/assembler_thermal.hpp"
#include "gptsolver/io/vtk/vtu_writer.hpp"
#include "gptsolver/solver/linear/eigen_direct.hpp"

namespace gptsolver {
void run_thermal_problem(const std::string& out_dir) {
  std::filesystem::create_directories(out_dir + "/results/step_1");
  auto k = build_thermal_matrix_sparse(16);
  auto q = build_thermal_rhs(16);
  auto t = solve_linear_sparse(k, q);
  std::vector<double> coords;
  std::vector<double> temp;
  for (int i = 0; i < t.size(); ++i) {
    coords.insert(coords.end(), {static_cast<double>(i), 0.0, 0.0});
    temp.push_back(t(i));
  }
  vtkio::write_simple_vtu(out_dir + "/results/step_1/frame_0001.vtu", coords, temp, "TEMP");
}
}  // namespace gptsolver
