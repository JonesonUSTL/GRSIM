#include "gptsolver/physics/thermal/thermal_problem.hpp"

#include <filesystem>
#include <iomanip>
#include <sstream>

#include "gptsolver/assembly/assembler_thermal.hpp"
#include "gptsolver/io/vtk/pvd_writer.hpp"
#include "gptsolver/io/vtk/vtu_writer.hpp"
#include "gptsolver/solver/linear/eigen_iterative.hpp"

namespace gptsolver {
namespace {
std::string frame_name(int i) {
  std::ostringstream oss;
  oss << "frame_" << std::setw(4) << std::setfill('0') << i << ".vtu";
  return oss.str();
}
}

void run_thermal_problem(const std::string& out_dir, int frames) {
  std::filesystem::create_directories(out_dir + "/results/step_1");
  auto k = build_thermal_matrix_sparse(16);
  auto q = build_thermal_rhs(16);
  auto t = solve_linear_cg(k, q, 300, true).x;
  std::vector<double> coords;
  std::vector<double> temp_final;
  for (int i = 0; i < t.size(); ++i) {
    coords.insert(coords.end(), {static_cast<double>(i), 0.0, 0.0});
    temp_final.push_back(t(i));
  }
  std::vector<std::pair<double, std::string>> timeline;
  for (int i = 1; i <= frames; ++i) {
    const double alpha = static_cast<double>(i) / static_cast<double>(frames);
    std::vector<double> temp = temp_final;
    for (auto& v : temp) v *= alpha;
    const auto fn = frame_name(i);
    vtkio::write_simple_vtu(out_dir + "/results/step_1/" + fn, coords, temp, "TEMP");
    timeline.emplace_back(alpha, "step_1/" + fn);
  }
  vtkio::write_pvd(out_dir + "/results/results.pvd", timeline);
}
}  // namespace gptsolver
