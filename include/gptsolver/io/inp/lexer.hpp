#pragma once
#include <string>
#include <vector>

namespace gptsolver::inp {
std::vector<std::string> lex_lines(const std::string& content);
std::string normalize_keyword(const std::string& text);
}  // namespace gptsolver::inp
