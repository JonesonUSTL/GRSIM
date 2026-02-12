#include "gptsolver/io/inp/semantic.hpp"

#include <set>

namespace gptsolver::inp {
std::vector<CompatibilityIssue> semantic_check(const ModelAst& ast) {
  static const std::set<std::string> known = {
      "HEADING", "NODE", "ELEMENT", "NSET", "ELSET", "MATERIAL", "ELASTIC", "DENSITY", "CONDUCTIVITY",
      "STEP", "STATIC", "HEAT TRANSFER", "BOUNDARY", "CLOAD", "OUTPUT", "NODE OUTPUT", "ELEMENT OUTPUT",
      "END STEP", "PLASTIC", "SURFACE", "CONTACT PAIR", "SURFACE INTERACTION", "FRICTION", "RESTART", "INCLUDE"};
  std::vector<CompatibilityIssue> issues;
  for (const auto& b : ast.blocks) {
    if (!known.count(b.keyword)) {
      issues.push_back({b.keyword, "已解析但未实现求解链路", b.pos});
    }
  }
  return issues;
}
}  // namespace gptsolver::inp
