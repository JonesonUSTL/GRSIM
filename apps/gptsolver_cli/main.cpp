#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "gptsolver/core/logger.hpp"
#include "gptsolver/io/inp/compatibility.hpp"
#include "gptsolver/io/inp/keyword_dispatcher.hpp"
#include "gptsolver/io/inp/parser.hpp"
#include "gptsolver/io/inp/semantic.hpp"
#include "gptsolver/physics/structural/structural_problem.hpp"
#include "gptsolver/physics/thermal/thermal_problem.hpp"
#include "gptsolver/solver/linear/petsc_backend.hpp"
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

static fs::path examples_root() {
  const fs::path p1 = "examples/inp";
  if (fs::exists(p1)) return p1;
  const fs::path p2 = "../examples/inp";
  if (fs::exists(p2)) return p2;
  return p1;
}

static std::vector<fs::path> discover_example_inp_files() {
  std::vector<fs::path> files;
  const auto root = examples_root();
  if (!fs::exists(root)) return files;
  for (const auto& e : fs::recursive_directory_iterator(root)) {
    if (!e.is_regular_file()) continue;
    if (e.path().extension() == ".inp") files.push_back(e.path());
  }
  std::sort(files.begin(), files.end());
  return files;
}

static std::vector<std::string> list_example_names() {
  std::map<std::string, fs::path> unique;
  const auto files = discover_example_inp_files();
  for (const auto& p : files) unique.emplace(p.stem().string(), p);
  std::vector<std::string> out;
  for (const auto& kv : unique) out.push_back(kv.first);
  return out;
}

static std::string resolve_example_to_inp(const std::string& name) {
  const auto root = examples_root();
  auto normalize = [](std::string s) {
    std::replace(s.begin(), s.end(), '\\', '/');
    return s;
  };
  const std::string n = normalize(name);

  if (n.find('/') != std::string::npos) {
    fs::path rel = n;
    if (rel.extension() != ".inp") rel += ".inp";
    const auto candidate = root / rel;
    if (fs::exists(candidate)) return candidate.string();
    return {};
  }

  fs::path direct = root / (n + ".inp");
  if (fs::exists(direct)) return direct.string();

  std::vector<fs::path> hits;
  for (const auto& p : discover_example_inp_files()) {
    if (p.stem() == n) hits.push_back(p);
  }
  if (hits.size() == 1) return hits.front().string();
  return {};
}

int main(int argc, char** argv) {
  if (argc < 2 || std::string(argv[1]) == "--help") {
    std::cout << "grsim 命令:\n"
              << "  grsim run <model.inp|example_name> [--out <dir>] [--threads N] [--solver-backend eigen|petsc] [--resume chk] [--frames N]\n"
              << "  grsim check <model.inp> [--fail-on-unknown]\n"
              << "  grsim info\n"
              << "  grsim examples --list\n"
              << "  grsim examples --run <name>\n"
              << "  grsim roadmap\n"
              << "  grsim capabilities\n";
    return 0;
  }

  const std::string cmd = argv[1];
  if (cmd == "info") {
    std::cout << "GRSIM v0.2.0\n默认后端: Eigen 稀疏\n能力: 结构/热/耦合最小链路 + 接触 + MPC + 弧长法(演示)\n";
    return 0;
  }
  if (cmd == "capabilities") {
    std::cout << "Supported-Executable keywords:\n";
    for (const auto& k : inp::all_supported_executable_keywords()) std::cout << "- " << k << "\n";
    std::cout << "\nParsed-Not-Solved keywords:\n";
    for (const auto& k : inp::all_parsed_not_solved_keywords()) std::cout << "- " << k << "\n";
    return 0;
  }

  if (cmd == "roadmap") {
    std::cout << "Abaqus 对标路线状态:\n"
              << "[本轮已完成]\n"
              << "1) S4/S4R、C3D8R 一致线性化积分（规则+hourglass+一致切线接口）\n"
              << "2) GENERAL CONTACT / CONTACT CONTROLS 数值主链（候选/窄相/历史变量/控制参数）\n"
              << "3) J2 完整工程化基础（各向同性+随动+温度修正）\n"
              << "4) PETSc 可选后端 + Schur 可配置混合权重\n"
              << "\n[下一阶段重点]\n"
              << "A) MPI 分布式网格装配与并行 KSP 规模化\n"
              << "B) 接触一致切线与全局牛顿耦合（含 CONTACT CONTROLS 全参数）\n"
              << "C) J2 一致切线与多积分点联动\n";
    return 0;
  }
  if (cmd == "examples" && argc >= 3 && std::string(argv[2]) == "--list") {
    const auto names = list_example_names();
    for (const auto& n : names) std::cout << n << "\n";
    return 0;
  }
  if (cmd == "examples" && argc >= 4 && std::string(argv[2]) == "--run") {
    const std::string resolved = resolve_example_to_inp(argv[3]);
    if (resolved.empty()) {
      std::cout << "未找到示例: " << argv[3] << "\n";
      std::cout << "可用示例名(可直接复制到 --run):\n";
      for (const auto& n : list_example_names()) std::cout << "- " << n << "\n";
      return 1;
    }
    std::cout << "已解析示例: " << argv[3] << " -> " << resolved << "\n";
    std::cout << "直接运行: grsim run " << resolved << "\n";
    return 0;
  }

  if (argc < 3) return 1;
  std::string inp_path = argv[2];
  if (cmd == "run" && !fs::exists(inp_path)) {
    const auto resolved = resolve_example_to_inp(inp_path);
    if (!resolved.empty()) inp_path = resolved;
  }
  auto ast = inp::parse_file(inp_path);
  auto issues = inp::semantic_check(ast);

  if (cmd == "check") {
    bool fail_on_unknown = false;
    for (int i = 3; i < argc; ++i) {
      if (std::string(argv[i]) == "--fail-on-unknown") fail_on_unknown = true;
    }

    int parsed = 0, unknown = 0;
    for (const auto& i : issues) {
      const auto tier = inp::classify_keyword(i.keyword);
      if (tier == inp::KeywordTier::ParsedNotSolved) ++parsed;
      if (tier == inp::KeywordTier::Unknown) ++unknown;
      std::cout << "[兼容告警] " << i.keyword << " " << i.pos.file << ':' << i.pos.line << " " << i.message << "\n";
    }
    std::cout << "[check统计] Parsed-Not-Solved=" << parsed << ", Unknown=" << unknown << "\n";
    if (fail_on_unknown && unknown > 0) {
      std::cout << "[check结果] 检测到 Unknown 关键字，按 --fail-on-unknown 返回失败\n";
      return 2;
    }
    return 0;
  }

  if (cmd == "run") {
    std::string out = "output/run";
    int threads = 1;
    std::string backend = "eigen";
    std::string resume;
    int frames = 10;
    double schur_blend = 0.5;
    for (int i = 3; i < argc; ++i) {
      const std::string a = argv[i];
      if (a == "--out" && i + 1 < argc) out = std::string(argv[++i]);
      if (a == "--threads" && i + 1 < argc) threads = std::stoi(argv[++i]);
      if (a == "--solver-backend" && i + 1 < argc) backend = argv[++i];
      if (a == "--resume" && i + 1 < argc) resume = argv[++i];
      if (a == "--frames" && i + 1 < argc) frames = std::max(1, std::stoi(argv[++i]));
      if (a == "--schur-blend" && i + 1 < argc) schur_blend = std::stod(argv[++i]);
    }

    fs::create_directories(out);
    set_linear_solver_backend(backend);
    set_schur_blend_weight(schur_blend);

    ContactRuntimeControls contact_ctrl;
    for (const auto& b : ast.blocks) {
      if (b.keyword == "GENERAL CONTACT") contact_ctrl.enable_general_contact = true;
      if (b.keyword == "CONTACT CONTROLS") {
        auto it = b.params.find("PENALTY");
        if (it != b.params.end()) contact_ctrl.penalty = std::stod(it->second);
        it = b.params.find("DAMPING");
        if (it != b.params.end()) contact_ctrl.damping = std::stod(it->second);
        it = b.params.find("SLIPTOL");
        if (it != b.params.end()) contact_ctrl.slip_tolerance = std::stod(it->second);
        it = b.params.find("STICKSTIFF");
        if (it != b.params.end()) contact_ctrl.stick_stiff_ratio = std::stod(it->second);
        it = b.params.find("SLIPSTIFF");
        if (it != b.params.end()) contact_ctrl.slip_stiffness = std::stod(it->second);
      }
      if (b.keyword == "FRICTION" && !b.data_lines.empty()) {
        const auto line = b.data_lines.front();
        auto comma = line.find(',');
        const auto v = (comma == std::string::npos) ? line : line.substr(0, comma);
        try {
          contact_ctrl.friction = std::stod(v);
        } catch (...) {
        }
      }
    }
    set_contact_runtime_controls(contact_ctrl);
    global_logger().open(out + "/run.log");
    global_logger().info("启动求解, backend=" + backend + ", threads=" + std::to_string(threads));
    if (backend == "petsc") global_logger().info("已请求 PETSc 后端；若当前构建未启用，将在求解器层自动回退 Eigen");
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
    if (has_static && has_heat)
      run_coupled_thermo_structural_problem(out, frames);
    else if (has_heat)
      run_thermal_problem(out, frames);
    else
      run_structural_problem(out, frames);

    std::ofstream(out + "/run_manifest.json")
        << "{\n  \"input\": \"" << inp_path << "\",\n  \"backend\": \"" << backend
        << "\",\n  \"threads\": " << threads << ",\n  \"frames\": " << frames
        << ",\n  \"schur_blend\": " << schur_blend << "\n}\n";
    std::ofstream(out + "/summary.md") << "# 运行简报\n\n- 输入: " << inp_path << "\n- 兼容告警数: " << issues.size()
                                      << "\n- 输出帧数: " << frames
                                      << "\n- Schur 混合权重: " << schur_blend
                                      << "\n- 下一步建议: 深化接触搜索/壳单元/真实材料积分与稀疏分块预条件器。\n";
    global_logger().info("完成");
    return 0;
  }
  return 0;
}
