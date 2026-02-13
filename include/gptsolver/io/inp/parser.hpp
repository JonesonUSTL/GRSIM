#pragma once
#include <string>

#include "gptsolver/io/inp/ast.hpp"

namespace gptsolver::inp {
ModelAst parse_file(const std::string& path);
}
