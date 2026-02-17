#include "gptsolver/io/inp/lexer.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace gptsolver::inp {
namespace {
std::string trim(std::string s) {
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin());
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
  return s;
}
}

std::vector<std::string> lex_lines(const std::string& content) {
  std::istringstream iss(content);
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(iss, line)) {
    if (!line.empty() && line.back() == '\r') line.pop_back();
    lines.push_back(trim(line));
  }
  return lines;
}

std::string normalize_keyword(const std::string& text) {
  std::string out = trim(text);
  std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
  return out;
}
}  // namespace gptsolver::inp
