#pragma once
#include <string>
#include <vector>

namespace gptsolver::restart {
void write_checkpoint(const std::string& path, const std::vector<double>& values);
std::vector<double> read_checkpoint(const std::string& path);
}
