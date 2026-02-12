#include "gptsolver/physics/structural/structural_problem.hpp"

#include <filesystem>

#include "gptsolver/assembly/assembler_structural.hpp"
#include "gptsolver/io/restart/checkpoint.hpp"
#include "gptsolver/io/vtk/pvd_writer.hpp"
#include "gptsolver/io/vtk/vtu_writer.hpp"
#include "gptsolver/solver/nonlinear/newton_solver.hpp"

namespace gptsolver {
void run_structural_problem(const std::string& out_dir) {
  std::filesystem::create_directories(out_dir + "/results/step_1");
  auto k = build_structural_stiffness(4);
  auto f = build_structural_load(4);
  auto r = solve_newton_demo(k, f, 15);
  std::vector<double> coords = {0,0,0,1,0,0,2,0,0,3,0,0};
  std::vector<double> u(r.x.data(), r.x.data()+r.x.size());
  vtkio::write_simple_vtu(out_dir + "/results/step_1/frame_0001.vtu", coords, u, "U");
  vtkio::write_pvd(out_dir + "/results/results.pvd", {{1.0, "step_1/frame_0001.vtu"}});
  restart::write_checkpoint(out_dir + "/checkpoint_0001.bin", u);
}
}  // namespace gptsolver
