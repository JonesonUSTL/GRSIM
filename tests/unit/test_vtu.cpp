#include <filesystem>

#include "gptsolver/io/vtk/vtu_writer.hpp"

int main() {
  std::filesystem::create_directories("tmpv");
  gptsolver::vtkio::write_simple_vtu("tmpv/a.vtu", {0,0,0}, {1.0}, "TEMP");
  return std::filesystem::exists("tmpv/a.vtu") ? 0 : 1;
}
