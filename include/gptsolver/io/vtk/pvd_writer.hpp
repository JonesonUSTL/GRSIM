#pragma once
#include <string>
#include <vector>

namespace gptsolver::vtkio {
void write_pvd(const std::string& path, const std::vector<std::pair<double, std::string>>& frames);
}
