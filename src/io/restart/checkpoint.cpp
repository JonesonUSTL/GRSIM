#include "gptsolver/io/restart/checkpoint.hpp"

#include <fstream>

namespace gptsolver::restart {
void write_checkpoint(const std::string& path, const std::vector<double>& values) {
  std::ofstream ofs(path, std::ios::binary);
  size_t n = values.size();
  ofs.write(reinterpret_cast<const char*>(&n), sizeof(size_t));
  ofs.write(reinterpret_cast<const char*>(values.data()), static_cast<std::streamsize>(n * sizeof(double)));
}

std::vector<double> read_checkpoint(const std::string& path) {
  std::ifstream ifs(path, std::ios::binary);
  size_t n = 0;
  ifs.read(reinterpret_cast<char*>(&n), sizeof(size_t));
  std::vector<double> vals(n);
  ifs.read(reinterpret_cast<char*>(vals.data()), static_cast<std::streamsize>(n * sizeof(double)));
  return vals;
}
}  // namespace gptsolver::restart
