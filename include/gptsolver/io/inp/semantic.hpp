#pragma once
#include <string>
#include <vector>

#include "gptsolver/io/inp/ast.hpp"

namespace gptsolver::inp {
struct CompatibilityIssue { std::string keyword; std::string message; SourcePos pos; };
std::vector<CompatibilityIssue> semantic_check(const ModelAst& ast);
}
