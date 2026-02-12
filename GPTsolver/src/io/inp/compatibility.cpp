#include "gptsolver/io/inp/compatibility.hpp"

#include <fstream>

namespace gptsolver::inp {
void write_compatibility_report(const std::string& path, const std::vector<CompatibilityIssue>& issues) {
  std::ofstream ofs(path);
  ofs << "# Abaqus inp 兼容性报告\n\n";
  ofs << "## Parsed-Not-Solved\n";
  for (const auto& i : issues) {
    ofs << "- " << i.keyword << " @ " << i.pos.file << ":" << i.pos.line << " - " << i.message << "\n";
  }
}
}  // namespace gptsolver::inp
