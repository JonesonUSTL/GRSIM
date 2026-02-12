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

std::string ts() {
  auto now = std::chrono::system_clock::now();
  auto tt = std::chrono::system_clock::to_time_t(now);
  std::tm tm{};
  localtime_r(&tt, &tm);
  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
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
    std::cout << "GPTsolver v0.1.0\n默认后端: Eigen\nOpenMP: enabled-if-compiled\nPETSc: placeholder\n";
    return 0;
  }

  if (cmd == "examples") {
    if (argc >= 3 && std::string(argv[2]) == "--list") {
      std::cout << "static_bar\nheat_rod\ncontact_demo\nplastic_demo\nsubroutine_demo\n";
      return 0;
    }
    if (argc >= 4 && std::string(argv[2]) == "--run") {
      std::cout << "请使用 run 命令执行 examples/inp/" << argv[3] << ".inp\n";
      return 0;
    }
  }

  if (argc < 3) return 1;
  const std::string inp_path = argv[2];
  auto ast = inp::parse_file(inp_path);
  auto issues = inp::semantic_check(ast);

  if (cmd == "check") {
    for (const auto& i : issues) {
      std::cout << "[兼容告警] " << i.keyword << " " << i.pos.file << ":" << i.pos.line << " " << i.message << "\n";
    }
    return 0;
  }

  if (cmd == "run") {
    std::string out = "output/run_" + ts();
    for (int i = 3; i < argc; ++i) {
      if (std::string(argv[i]) == "--out" && i + 1 < argc) out = argv[++i];
    }
    fs::create_directories(out);
    global_logger().open(out + "/run.log");
    global_logger().info("启动求解");
    inp::write_compatibility_report(out + "/compatibility_report.md", issues);

    StatusSnapshot s;
    s.job_name = "demo_job";
    s.input_file = inp_path;
    s.output_dir = out;
    s.step = 1;
    s.increment = 1;
    render_dashboard(s);

    bool thermal = false;
    for (const auto& b : ast.blocks) {
      if (b.keyword == "HEAT TRANSFER") thermal = true;
    }
    if (thermal) run_thermal_problem(out); else run_structural_problem(out);

    std::ofstream(out + "/run_manifest.json") << "{\n  \"input\": \"" << inp_path << "\",\n  \"backend\": \"eigen\"\n}\n";
    std::ofstream(out + "/summary.md") << "# 运行简报\n\n- 输入: " << inp_path << "\n- 兼容告警数: " << issues.size() << "\n";
    global_logger().info("完成");
    return 0;
  }
  return 0;
}
