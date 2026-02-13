#include "gptsolver/io/vtk/vtu_writer.hpp"

#include <fstream>

namespace gptsolver::vtkio {
void write_simple_vtu(const std::string& path, const std::vector<double>& coords, const std::vector<double>& scalar,
                      const std::string& scalar_name) {
  std::ofstream ofs(path);
  const size_t n = scalar.size();
  ofs << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
  ofs << "<UnstructuredGrid><Piece NumberOfPoints=\"" << n << "\" NumberOfCells=\"" << n << "\">\n";
  ofs << "<Points><DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\">\n";
  for (size_t i = 0; i < n; ++i) ofs << coords[i * 3] << ' ' << coords[i * 3 + 1] << ' ' << coords[i * 3 + 2] << ' ';
  ofs << "\n</DataArray></Points>\n";
  ofs << "<Cells><DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n";
  for (size_t i = 0; i < n; ++i) ofs << i << ' ';
  ofs << "\n</DataArray><DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n";
  for (size_t i = 0; i < n; ++i) ofs << i + 1 << ' ';
  ofs << "\n</DataArray><DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n";
  for (size_t i = 0; i < n; ++i) ofs << 1 << ' ';
  ofs << "\n</DataArray></Cells>\n";
  ofs << "<PointData><DataArray type=\"Float64\" Name=\"" << scalar_name << "\" format=\"ascii\">\n";
  for (double v : scalar) ofs << v << ' ';
  ofs << "\n</DataArray></PointData>\n";
  ofs << "</Piece></UnstructuredGrid></VTKFile>\n";
}
}  // namespace gptsolver::vtkio
