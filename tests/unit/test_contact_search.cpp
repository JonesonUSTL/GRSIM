#include <cassert>

#include "gptsolver/model/interaction/contact_pair.hpp"

int main() {
  gptsolver::SurfaceBBox a{0, 1, 0, 1, 0, 1};
  gptsolver::SurfaceBBox b{0.5, 1.5, 0.5, 1.5, 0.5, 1.5};
  gptsolver::SurfaceBBox c{2, 3, 2, 3, 2, 3};
  assert(gptsolver::bbox_overlap(a, b));
  assert(!gptsolver::bbox_overlap(a, c));
  auto cand = gptsolver::build_contact_candidates({a}, {b, c});
  assert(cand.size() == 1);
  return 0;
}
