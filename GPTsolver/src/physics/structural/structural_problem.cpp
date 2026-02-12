#include "gptsolver/physics/structural/structural_problem.hpp"

#include <filesystem>

#include "gptsolver/assembly/assembler_contact_placeholder.hpp"
#include "gptsolver/assembly/assembler_structural.hpp"
#include "gptsolver/assembly/assembler_thermal.hpp"
#include "gptsolver/assembly/csr_matrix.hpp"
#include "gptsolver/io/restart/checkpoint.hpp"
#include "gptsolver/io/vtk/pvd_writer.hpp"
#include "gptsolver/io/vtk/vtu_writer.hpp"
#include "gptsolver/solver/linear/eigen_iterative.hpp"
#include "gptsolver/solver/linear/schur_preconditioner.hpp"
#include "gptsolver/solver/nonlinear/newton_solver.hpp"

namespace gptsolver {

void run_structural_problem(const std::string& out_dir) {
  std::filesystem::create_directories(out_dir + "/results/step_1");
  constexpr int ndof = 128;

  auto k = build_structural_stiffness_sparse(ndof);
  auto f = build_structural_load(ndof);

  // 显式 MPC/Lagrange 约束：将 0 号与 1 号自由度耦合
  k += assemble_mpc_lagrange(ndof, 0, 1, 1.0);

  // 接触：第 20 个法向 DOF 与第 21 个切向 DOF，带少量穿透
  DenseVector rt = DenseVector::Zero(ndof);
  std::vector<ContactPointState> cps = {{20, 21, -1e-3, 5e-4, 0.0, true}};
  assemble_contact_terms(cps, ContactParams{}, k, rt);
  f += rt;

  ArcLengthOptions opt;
  opt.max_iter = 25;
  auto nr = solve_newton_with_arclength(k, f, DenseVector::Zero(ndof), opt);

  std::vector<double> coords;
  std::vector<double> u;
  coords.reserve(ndof * 3);
  u.reserve(ndof);
  for (int i = 0; i < ndof; ++i) {
    coords.push_back(static_cast<double>(i));
    coords.push_back(0.0);
    coords.push_back(0.0);
    u.push_back(nr.x(i));
  }
  vtkio::write_simple_vtu(out_dir + "/results/step_1/frame_0001.vtu", coords, u, "U");
  vtkio::write_pvd(out_dir + "/results/results.pvd", {{1.0, "step_1/frame_0001.vtu"}});
  restart::write_checkpoint(out_dir + "/checkpoint_0001.bin", u);
}

void run_coupled_thermo_structural_problem(const std::string& out_dir) {
  std::filesystem::create_directories(out_dir + "/results/step_1");
  constexpr int n = 64;
  auto kuu = build_structural_stiffness_sparse(n);
  auto ktt = build_thermal_matrix_sparse(n);
  auto kut = build_coupling_ut(n, 0.05);
  auto ktu = build_coupling_ut(n, 0.02);
  auto k = assemble_block_matrix(kuu, kut, ktu, ktt);

  DenseVector rhs(2 * n);
  rhs << build_structural_load(n), build_thermal_rhs(n);

  auto x_schur = solve_block_schur(kuu, kut, ktu, ktt, rhs);
  auto it = solve_linear_cg(k, rhs, 800);
  if (x_schur.size() == it.x.size()) it.x = 0.5 * it.x + 0.5 * x_schur;

  std::vector<double> coords;
  std::vector<double> temp;
  coords.reserve(n * 3);
  temp.reserve(n);
  for (int i = 0; i < n; ++i) {
    coords.push_back(static_cast<double>(i));
    coords.push_back(1.0);
    coords.push_back(0.0);
    temp.push_back(it.x(n + i));
  }
  vtkio::write_simple_vtu(out_dir + "/results/step_1/frame_0001.vtu", coords, temp, "TEMP");
  vtkio::write_pvd(out_dir + "/results/results.pvd", {{1.0, "step_1/frame_0001.vtu"}});
}

}  // namespace gptsolver
