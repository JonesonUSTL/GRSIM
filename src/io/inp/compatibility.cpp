#include "gptsolver/io/inp/compatibility.hpp"

#include <fstream>

#include "gptsolver/io/inp/keyword_dispatcher.hpp"

namespace gptsolver::inp {
void write_compatibility_report(const std::string& path, const std::vector<CompatibilityIssue>& issues) {
  std::ofstream ofs(path);
  ofs << "# Abaqus inp 兼容性报告\n\n";
  ofs << "## Supported-Executable\n";
  ofs << "- 线性/非线性结构静力最小链路\n- 稳态传热最小链路\n- 接触残量/切线刚度骨架\n- MPC 约束显式组装\n\n";
  ofs << "## Parsed-Not-Solved\n";
  for (const auto& i : issues) {
    if (classify_keyword(i.keyword) == KeywordTier::ParsedNotSolved) {
      ofs << "- " << i.keyword << " @ " << i.pos.file << ":" << i.pos.line << " - " << i.message << "\n";
    }
  }
  ofs << "\n## Unknown\n";
  for (const auto& i : issues) {
    if (classify_keyword(i.keyword) == KeywordTier::Unknown) {
      ofs << "- " << i.keyword << " @ " << i.pos.file << ":" << i.pos.line << " - " << i.message << "\n";
    }
  }
}
}  // namespace gptsolver::inp
