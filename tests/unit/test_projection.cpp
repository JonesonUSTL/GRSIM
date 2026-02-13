#include <cassert>

#include "gptsolver/model/interaction/surface.hpp"

int main() {
  gptsolver::TriangleFace f{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};
  auto r = gptsolver::project_point_to_face({0.2, 0.2, 0.5}, f);
  assert(r.has_value());
  assert(r->inside);
  assert(r->gap > 0.49 && r->gap < 0.51);
  return 0;
}
