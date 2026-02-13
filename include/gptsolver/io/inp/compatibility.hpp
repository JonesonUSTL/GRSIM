#pragma once
#include <string>
#include <vector>

#include "gptsolver/io/inp/semantic.hpp"

namespace gptsolver::inp {
void write_compatibility_report(const std::string& path, const std::vector<CompatibilityIssue>& issues);
}
