#include "gptsolver/io/inp/semantic.hpp"

#include "gptsolver/io/inp/keyword_dispatcher.hpp"
#include "gptsolver/io/inp/lexer.hpp"

#include <set>

namespace gptsolver::inp {
namespace {
const std::set<std::string>& supported_element_types() {
  static const std::set<std::string> types = {
      "C3D4", "C3D6", "C3D8", "C3D8R", "C3D10", "C3D15", "C3D20", "C3D20R",
      "CPE4", "CPS4", "T3D2"};
  return types;
}
}  // namespace

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
  }
  return issues;
}
}  // namespace gptsolver::inp
