#include "gptsolver/io/vtk/pvd_writer.hpp"

#include <fstream>

namespace gptsolver::vtkio {
void write_pvd(const std::string& path, const std::vector<std::pair<double, std::string>>& frames) {
  std::ofstream ofs(path);
  ofs << "<VTKFile type=\"Collection\" version=\"0.1\" byte_order=\"LittleEndian\">\n<Collection>\n";
  for (auto& f : frames) ofs << "<DataSet timestep=\"" << f.first << "\" file=\"" << f.second << "\"/>\n";
  ofs << "</Collection></VTKFile>\n";
}
}  // namespace gptsolver::vtkio
