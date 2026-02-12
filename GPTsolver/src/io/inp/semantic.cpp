#include "gptsolver/io/inp/semantic.hpp"

#include <set>

namespace gptsolver::inp {
std::vector<CompatibilityIssue> semantic_check(const ModelAst& ast) {
  static const std::set<std::string> known = {
      "HEADING", "PREPRINT", "PART", "ASSEMBLY", "INSTANCE", "END PART", "END ASSEMBLY", "NODE", "ELEMENT",
      "NSET", "ELSET", "SOLID SECTION", "MATERIAL", "ELASTIC", "DENSITY", "CONDUCTIVITY", "PLASTIC",
      "USER MATERIAL", "DEPVAR", "STEP", "STATIC", "HEAT TRANSFER", "END STEP", "BOUNDARY", "CLOAD",
      "DLOAD", "DSLOAD", "TEMPERATURE", "SURFACE", "CONTACT PAIR", "SURFACE INTERACTION", "FRICTION",
      "OUTPUT", "NODE OUTPUT", "ELEMENT OUTPUT", "RESTART", "INCLUDE", "MPC"};

  std::vector<CompatibilityIssue> issues;
  for (const auto& b : ast.blocks) {
    if (!known.count(b.keyword)) issues.push_back({b.keyword, "已解析但当前版本未进入可执行求解主链", b.pos});
    if (b.keyword == "PLASTIC" && b.data_lines.empty()) issues.push_back({b.keyword, "检测到 *PLASTIC 但缺少硬化数据", b.pos});
  }
  return issues;
}
}  // namespace gptsolver::inp
