#include "gptsolver/physics/structural/structural_problem.hpp"

#include <filesystem>
#include <iomanip>
#include <sstream>

#include "gptsolver/assembly/assembler_contact_placeholder.hpp"
#include "gptsolver/assembly/assembler_structural.hpp"
#include "gptsolver/assembly/assembler_thermal.hpp"
#include "gptsolver/assembly/csr_matrix.hpp"
#include "gptsolver/io/restart/checkpoint.hpp"
#include "gptsolver/io/vtk/pvd_writer.hpp"
#include "gptsolver/io/vtk/vtu_writer.hpp"
#include "gptsolver/fem/element/shell_placeholder.hpp"
#include "gptsolver/model/interaction/surface.hpp"
#include "gptsolver/model/interaction/contact_pair.hpp"
#include "gptsolver/solver/linear/eigen_iterative.hpp"
#include "gptsolver/solver/linear/schur_preconditioner.hpp"
#include "gptsolver/solver/nonlinear/newton_solver.hpp"

namespace gptsolver {
namespace {
std::string frame_name(int i) {
  std::ostringstream oss;
  oss << "frame_" << std::setw(4) << std::setfill('0') << i << ".vtu";
  return oss.str();
}
}

void run_structural_problem(const std::string& out_dir, int frames) {
  std::filesystem::create_directories(out_dir + "/results/step_1");
  constexpr int ndof = 128;

  auto k = build_structural_stiffness_sparse(ndof);
  auto f = build_structural_load(ndof);
  k += assemble_mpc_lagrange(ndof, 0, 1, 1.0);

  DenseVector rt = DenseVector::Zero(ndof);

  // 接触窄相：从“面候选+从点”构造接触状态，再映射为装配输入。
  std::vector<ContactPointState> cps;
  const std::vector<SurfaceBBox> master_bbox = {{0.0, 1.0, 0.0, 1.0, -1e-6, 1e-6}};
  const std::vector<SurfaceBBox> slave_bbox = {{0.2, 0.3, 0.2, 0.3, -2e-3, -5e-4}};
  const auto cand = build_contact_candidates_bucket(master_bbox, slave_bbox, 0.5);
  const std::vector<std::array<std::array<double, 3>, 4>> master_faces = {
      {{{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}}}};
  const std::vector<std::array<double, 3>> slave_points = {{{0.25, 0.25, -1e-3}}};
  const std::vector<std::pair<int, int>> dof_pairs = {{20, 21}};
  auto face_states = build_face_contact_states(cand, master_faces, slave_points, dof_pairs);
  for (const auto& fs : face_states) cps.push_back(fs.to_point_state(5e-4));

  // 兼容旧的三角面投影链路，便于和既有最小示例对比。
  TriangleFace tri{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};
  auto prj = project_point_to_face({0.2, 0.2, -1e-3}, tri);
  if (prj && prj->inside) cps.push_back({22, 23, prj->gap, 2e-4, 0.0, true});

  assemble_contact_terms(cps, ContactParams{}, k, rt);
  f += rt;

  // 壳/实体积分规则与 hourglass 稳定项（演示版）进入主流程：
  // 让后续替换真实单元积分时，不需要改 CLI 或 step 主循环。
  const auto shell_rule = shell_integration_rule(false);
  const auto solid_rule = solid_c3d8_integration_rule(false);
  const double k_hg_shell = shell_hourglass_stiffness(0.01, 8.0e4, 1.0);
  const double k_hg_solid = solid_hourglass_scale(1.0, 8.0e4);
  if (!shell_rule.weights.empty() && !solid_rule.weights.empty()) {
    k.coeffRef(0, 0) += 1e-6 * (k_hg_shell + k_hg_solid);
  }

  ArcLengthOptions opt;
  opt.max_iter = 25;
  auto nr = solve_newton_with_arclength(k, f, DenseVector::Zero(ndof), opt);

  std::vector<double> coords;
  std::vector<double> u_final;
  coords.reserve(ndof * 3);
  u_final.reserve(ndof);
  for (int i = 0; i < ndof; ++i) {
    coords.push_back(static_cast<double>(i));
    coords.push_back(0.0);
    coords.push_back(0.0);
    u_final.push_back(nr.x(i));
  }

  std::vector<std::pair<double, std::string>> timeline;
  for (int i = 1; i <= frames; ++i) {
    const double alpha = static_cast<double>(i) / static_cast<double>(frames);
    std::vector<double> u = u_final;
    for (auto& v : u) v *= alpha;
    const auto fn = frame_name(i);
    vtkio::write_simple_vtu(out_dir + "/results/step_1/" + fn, coords, u, "U");
    timeline.emplace_back(alpha, "step_1/" + fn);
  }
  vtkio::write_pvd(out_dir + "/results/results.pvd", timeline);
  restart::write_checkpoint(out_dir + "/checkpoint_0001.bin", u_final);
}

void run_coupled_thermo_structural_problem(const std::string& out_dir, int frames) {
  std::filesystem::create_directories(out_dir + "/results/step_1");
  constexpr int n = 64;

  const auto [kuu, kut0, ktu0, ktt] = build_thermo_structural_blocks(
      n, 1.2e-5, 1.0, 2.1e5, 100.0);

  DenseVector rhs(2 * n);
  rhs << build_structural_load(n), build_thermal_rhs(n);

  DenseVector x = DenseVector::Zero(2 * n);
  for (int iter = 0; iter < 6; ++iter) {
    // 强耦合：热膨胀项随温度场更新，形成一致切线近似
    const auto t = x.tail(n);
    const double avg_dt = t.size() > 0 ? t.mean() : 0.0;
    auto kut = build_thermal_expansion_tangent(n, 2.1e5, 1.2e-5, 1.0 + avg_dt / 300.0);
    SparseMatrix ktu = kut.transpose();

    auto k = assemble_block_matrix(kuu, kut, ktu, ktt);
    DenseVector r = rhs - k * x;
    if (r.norm() < 1e-8) break;

    auto dx_schur = solve_block_schur(kuu, kut, ktu, ktt, r);
    auto dx_it = solve_linear_cg(k, r, 400).x;
    DenseVector dx = dx_it;
    if (dx_schur.size() == dx_it.size()) dx = 0.5 * dx_it + 0.5 * dx_schur;
    x += dx;
    if (dx.norm() < 1e-8) break;
  }

  std::vector<double> coords;
  std::vector<double> temp_final;
  coords.reserve(n * 3);
  temp_final.reserve(n);
  for (int i = 0; i < n; ++i) {
    coords.push_back(static_cast<double>(i));
    coords.push_back(1.0);
    coords.push_back(0.0);
    temp_final.push_back(x(n + i));
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
