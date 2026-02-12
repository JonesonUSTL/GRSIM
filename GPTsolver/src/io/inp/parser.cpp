#include "gptsolver/io/inp/parser.hpp"

#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>

#include "gptsolver/io/inp/lexer.hpp"

namespace gptsolver::inp {
namespace {
void parse_recursive(const std::filesystem::path& path, ModelAst& ast, std::set<std::string>& stack) {
  const auto abs = std::filesystem::absolute(path).string();
  if (stack.count(abs)) return;
  stack.insert(abs);

  std::ifstream ifs(path);
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  auto lines = lex_lines(buffer.str());

  KeywordBlock current;
  bool in_block = false;

  for (size_t i = 0; i < lines.size(); ++i) {
    std::string line = lines[i];
    if (line.empty() || (!line.empty() && line[0] == '*' && line.size() > 1 && line[1] == '*')) continue;

    if (!line.empty() && line[0] == '*') {
      // 关键字续行：如 *STEP, NLGEOM=YES,
      while (!line.empty() && line.back() == ',' && i + 1 < lines.size() && !lines[i + 1].empty() && lines[i + 1][0] != '*') {
        line += lines[++i];
      }

      if (in_block) ast.blocks.push_back(current);
      in_block = true;
      current = {};
      current.pos = {path.string(), static_cast<int>(i + 1), 1};

      std::stringstream hs(line.substr(1));
      std::string token;
      bool first = true;
      while (std::getline(hs, token, ',')) {
        if (token.empty()) continue;
        if (first) {
          current.keyword = normalize_keyword(token);
          first = false;
        } else {
          const auto eq = token.find('=');
          if (eq != std::string::npos) {
            current.params[normalize_keyword(token.substr(0, eq))] = token.substr(eq + 1);
          }
        }
      }

      if (current.keyword == "INCLUDE") {
        auto it = current.params.find("INPUT");
        if (it != current.params.end()) parse_recursive(path.parent_path() / it->second, ast, stack);
      }
    } else if (in_block) {
      current.data_lines.push_back(line);
    }
  }
  if (in_block) ast.blocks.push_back(current);
  stack.erase(abs);
}
}  // namespace

ModelAst parse_file(const std::string& path) {
  ModelAst ast;
  std::set<std::string> stack;
  parse_recursive(path, ast, stack);
  return ast;
}

}  // namespace gptsolver::inp
