#pragma once
#include <vector>

namespace gptsolver::subroutine {
struct MaterialPointData {
  std::vector<double> strain;
  std::vector<double> stress;
  std::vector<double> statev;
  double dtime{0.0};
};
}
