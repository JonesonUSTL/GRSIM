#include "gptsolver/io/inp/semantic.hpp"

#include "gptsolver/io/inp/keyword_dispatcher.hpp"

namespace gptsolver::inp {
std::vector<CompatibilityIssue> semantic_check(const ModelAst& ast) {
  std::vector<CompatibilityIssue> issues;
  for (const auto& b : ast.blocks) {
    auto tier = classify_keyword(b.keyword);
    if (tier == KeywordTier::Unknown) {
      issues.push_back({b.keyword, "未识别关键字（可继续运行，但建议检查拼写或版本）", b.pos});
    } else if (tier == KeywordTier::ParsedNotSolved) {
      issues.push_back({b.keyword, "已解析但当前版本未进入可执行求解主链", b.pos});
    }
    if (b.keyword == "PLASTIC" && b.data_lines.empty()) issues.push_back({b.keyword, "检测到 *PLASTIC 但缺少硬化数据", b.pos});
  }
  return issues;
}
}  // namespace gptsolver::inp
