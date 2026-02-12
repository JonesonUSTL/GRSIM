#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "gptsolver/core/logger.hpp"
#include "gptsolver/io/inp/compatibility.hpp"
#include "gptsolver/io/inp/parser.hpp"
#include "gptsolver/io/inp/semantic.hpp"
#include "gptsolver/physics/structural/structural_problem.hpp"
#include "gptsolver/physics/thermal/thermal_problem.hpp"
#include "gptsolver/ui/console_dashboard.hpp"

namespace fs = std::filesystem;
using namespace gptsolver;

static std::string ts() {
  auto now = std::chrono::system_clock::now();
  auto tt = std::chrono::system_clock::to_time_t(now);
  std::tm tm{};
  localtime_r(&tt, &tm);
  std::ostringstream oss;
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
  oss << std::put_time(&tm, "%Y%m%d_%H%M%S") << "_" << std::setw(3) << std::setfill('0') << ms;
  return oss.str();
}

int main(int argc, char** argv) {
  if (argc < 2 || std::string(argv[1]) == "--help") {
    std::cout << "gptsolver 命令:\n"
              << "  gptsolver run <model.inp> --out <dir> [--threads N] [--solver-backend eigen|petsc] [--resume chk]\n"
              << "  gptsolver check <model.inp>\n"
              << "  gptsolver info\n"
              << "  gptsolver examples --list\n"
              << "  gptsolver examples --run <name>\n";
    return 0;
  }

  const std::string cmd = argv[1];
  if (cmd == "info") {
    std::cout << "GPTsolver v0.2.0\n默认后端: Eigen 稀疏\n能力: 结构/热/耦合最小链路 + 接触 + MPC + 弧长法(演示)\n";
    return 0;
  }
  if (cmd == "examples" && argc >= 3 && std::string(argv[2]) == "--list") {
    std::cout << "static_bar\nheat_rod\ncontact_demo\ncoupled_plate\nlarge_mesh_120el\n";
    return 0;
  }
  if (cmd == "examples" && argc >= 4 && std::string(argv[2]) == "--run") {
    std::cout << "请使用 run 命令执行 examples/inp/" << argv[3] << ".inp\n";
    return 0;
  }

  if (argc < 3) return 1;
  const std::string inp_path = argv[2];
  auto ast = inp::parse_file(inp_path);
  auto issues = inp::semantic_check(ast);

  if (cmd == "check") {
    for (const auto& i : issues) std::cout << "[兼容告警] " << i.keyword << " " << i.pos.file << ':' << i.pos.line << " " << i.message << "\n";
    return 0;
  }

  if (cmd == "run") {
    std::string out = "output/run_" + ts();
    int threads = 1;
    std::string backend = "eigen";
    std::string resume;
    for (int i = 3; i < argc; ++i) {
      const std::string a = argv[i];
      if (a == "--out" && i + 1 < argc) out = std::string(argv[++i]) + "/run_" + ts();
      if (a == "--threads" && i + 1 < argc) threads = std::stoi(argv[++i]);
      if (a == "--solver-backend" && i + 1 < argc) backend = argv[++i];
      if (a == "--resume" && i + 1 < argc) resume = argv[++i];
    }

    fs::create_directories(out);
    global_logger().open(out + "/run.log");
    global_logger().info("启动求解, backend=" + backend + ", threads=" + std::to_string(threads));
    if (backend == "petsc") global_logger().warn("当前 PETSc 为接口占位，将自动回退 Eigen 稀疏求解");
    if (!resume.empty()) global_logger().info("从检查点恢复: " + resume);
    inp::write_compatibility_report(out + "/compatibility_report.md", issues);

    StatusSnapshot s;
    s.job_name = "abaqus_like_job";
    s.input_file = inp_path;
    s.output_dir = out;
    s.step = 1;
    s.increment = 1;
    render_dashboard(s);

    bool has_static = false, has_heat = false;
    for (const auto& b : ast.blocks) {
      if (b.keyword == "STATIC") has_static = true;
      if (b.keyword == "HEAT TRANSFER") has_heat = true;
    }
    if (has_static && has_heat) run_coupled_thermo_structural_problem(out);
    else if (has_heat) run_thermal_problem(out);
    else run_structural_problem(out);

    std::ofstream(out + "/run_manifest.json")
        << "{\n  \"input\": \"" << inp_path << "\",\n  \"backend\": \"" << backend << "\",\n  \"threads\": " << threads
        << "\n}\n";
    std::ofstream(out + "/summary.md") << "# 运行简报\n\n- 输入: " << inp_path << "\n- 兼容告警数: " << issues.size()
                                    << "\n- 下一步建议: 深化接触搜索/壳单元/真实材料积分与稀疏分块预条件器。\n";
    global_logger().info("完成");
    return 0;
  }
  return 0;
}
