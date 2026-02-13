#include <cassert>

#include "gptsolver/model/interaction/contact_pair.hpp"

int main() {
  const std::array<std::array<double, 3>, 4> face = {{{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}}};
  const std::array<double, 3> p = {0.25, 0.75, 0.2};

  auto proj = gptsolver::project_point_to_quad_face(p, face);
  assert(proj.inside);
  assert(proj.gap > 0.19 && proj.gap < 0.21);
  assert(proj.projected[2] == 0.0);

  double sumN = 0.0;
  for (double n : proj.N) sumN += n;
  assert(sumN > 0.999 && sumN < 1.001);

  assert(gptsolver::normal_contact_tangent(1234.0) == 1234.0);
  return 0;
}
