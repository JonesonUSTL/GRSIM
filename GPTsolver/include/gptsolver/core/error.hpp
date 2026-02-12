#pragma once
#include <stdexcept>
#include <string>

namespace gptsolver {

class SolverError : public std::runtime_error {
 public:
  explicit SolverError(const std::string& msg) : std::runtime_error(msg) {}
};

}  // namespace gptsolver
