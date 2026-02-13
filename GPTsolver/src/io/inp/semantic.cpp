#include "gptsolver/io/inp/semantic.hpp"

#include "gptsolver/io/inp/keyword_dispatcher.hpp"
#include "gptsolver/io/inp/lexer.hpp"

#include <set>

namespace gptsolver::inp {
namespace {
const std::set<std::string>& supported_element_types() {
  static const std::set<std::string> types = {"C3D4", "C3D6", "C3D8", "C3D8R", "C3D10", "C3D15", "C3D20", "C3D20R",
                                              "CPE4", "CPS4", "T3D2"};
  return types;
}

bool has_keyword(const ModelAst& ast, const std::string& k) {
  for (const auto& b : ast.blocks)
    if (b.keyword == k) return true;
  return false;
}
}  // namespace

std::vector<CompatibilityIssue> semantic_check(const ModelAst& ast) {
  std::vector<CompatibilityIssue> issues;
  const bool has_heat = has_keyword(ast, "HEAT TRANSFER");
  const bool has_step = has_keyword(ast, "STEP");
  const bool has_depvar = has_keyword(ast, "DEPVAR");
  const bool has_contact_controls = has_keyword(ast, "CONTACT CONTROLS");
  const bool has_surface_behavior = has_keyword(ast, "SURFACE BEHAVIOR");
  const bool has_dynamic = has_keyword(ast, "DYNAMIC");
  const bool has_static = has_keyword(ast, "STATIC");

  for (const auto& b : ast.blocks) {
    auto tier = classify_keyword(b.keyword);
    if (tier == KeywordTier::Unknown) {
      issues.push_back({b.keyword, "未识别关键字（可继续运行，但建议检查拼写或版本）", b.pos});
    } else if (tier == KeywordTier::ParsedNotSolved) {
      issues.push_back({b.keyword, "已解析但当前版本未进入可执行求解主链", b.pos});
    }

    if (b.keyword == "PLASTIC" && b.data_lines.empty()) issues.push_back({b.keyword, "检测到 *PLASTIC 但缺少硬化数据", b.pos});

    if (b.keyword == "ELEMENT") {
      auto it = b.params.find("TYPE");
      if (it == b.params.end()) {
        issues.push_back({b.keyword, "*ELEMENT 缺少 TYPE 参数", b.pos});
      } else {
        const auto type = normalize_keyword(it->second);
        if (!supported_element_types().count(type)) {
          issues.push_back({b.keyword, "单元类型 " + type + " 暂未支持（可解析但不会进入求解主链）", b.pos});
        }
      }
    }

    if (b.keyword == "AMPLITUDE" && b.data_lines.empty())
      issues.push_back({b.keyword, "*AMPLITUDE 缺少时程数据（t,value）", b.pos});

    if (b.keyword == "DEPVAR" && b.data_lines.empty())
      issues.push_back({b.keyword, "*DEPVAR 缺少状态变量数量定义", b.pos});

    if (b.keyword == "USER MATERIAL" && !has_depvar)
      issues.push_back({b.keyword, "*USER MATERIAL 建议搭配 *DEPVAR 声明状态变量", b.pos});

    if ((b.keyword == "DLOAD" || b.keyword == "DSLOAD") && !has_step)
      issues.push_back({b.keyword, "*DLOAD/*DSLOAD 应位于 *STEP 中", b.pos});

    if ((b.keyword == "FILM" || b.keyword == "RADIATION" || b.keyword == "SPECIFIC HEAT") && !has_heat)
      issues.push_back({b.keyword, "热学关键字建议在 *HEAT TRANSFER 步中使用", b.pos});

    if (b.keyword == "GENERAL CONTACT") {
      if (!has_contact_controls) issues.push_back({b.keyword, "建议配套 *CONTACT CONTROLS", b.pos});
      if (!has_surface_behavior) issues.push_back({b.keyword, "建议配套 *SURFACE BEHAVIOR", b.pos});
    }

    if (b.keyword == "COUPLING" && !has_keyword(ast, "KINEMATIC"))
      issues.push_back({b.keyword, "*COUPLING 常与 *KINEMATIC 联用", b.pos});

    if (b.keyword == "ORIENTATION" && b.data_lines.empty())
      issues.push_back({b.keyword, "*ORIENTATION 缺少方向定义数据", b.pos});

    if (b.keyword == "EXPANSION" && b.data_lines.empty())
      issues.push_back({b.keyword, "*EXPANSION 缺少热膨胀系数数据", b.pos});

    if ((b.keyword == "NODE FILE" || b.keyword == "EL FILE") && !has_keyword(ast, "OUTPUT"))
      issues.push_back({b.keyword, "建议与 *OUTPUT 配套以明确输出频率/变量", b.pos});

    if (b.keyword == "CONTACT DAMPING" && !has_keyword(ast, "GENERAL CONTACT") && !has_keyword(ast, "CONTACT PAIR"))
      issues.push_back({b.keyword, "*CONTACT DAMPING 需要接触定义（*GENERAL CONTACT 或 *CONTACT PAIR）", b.pos});

    if (b.keyword == "VISCO" && !has_dynamic)
      issues.push_back({b.keyword, "*VISCO 通常用于 *DYNAMIC 步", b.pos});

    if (b.keyword == "SOLUTION TECHNIQUE" && b.params.find("TYPE") == b.params.end())
      issues.push_back({b.keyword, "*SOLUTION TECHNIQUE 建议声明 TYPE 参数", b.pos});
  }

  if (has_dynamic && has_static)
    issues.push_back({"STEP", "模型同时包含 *STATIC 与 *DYNAMIC，当前仅支持最小演示链路，请确认工况组合", {"", 0, 0}});

  return issues;
}
}  // namespace gptsolver::inp
