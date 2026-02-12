#include <cassert>

#include "gptsolver/model/interaction/contact_pair.hpp"

int main() {
  std::vector<gptsolver::SurfaceBBox> m = {{0, 1, 0, 1, 0, 1}, {5, 6, 5, 6, 5, 6}};
  std::vector<gptsolver::SurfaceBBox> s = {{0.5, 1.2, 0.5, 1.2, 0.5, 1.2}, {5.1, 5.9, 5.1, 5.9, 5.1, 5.9}};
  auto cand = gptsolver::build_contact_candidates_bucket(m, s, 1.0);
  assert(cand.size() >= 2);
  return 0;
}
