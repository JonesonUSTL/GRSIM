#pragma once
#include <string>
#include <vector>

namespace gptsolver::vtkio {
void write_simple_vtu(const std::string& path, const std::vector<double>& coords, const std::vector<double>& scalar,
                      const std::string& scalar_name);
}
