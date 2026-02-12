#include "gptsolver/model/interaction/contact_pair.hpp"

namespace gptsolver {

bool bbox_overlap(const SurfaceBBox& a, const SurfaceBBox& b) {
  return !(a.xmax < b.xmin || b.xmax < a.xmin || a.ymax < b.ymin || b.ymax < a.ymin || a.zmax < b.zmin ||
           b.zmax < a.zmin);
}

std::vector<std::pair<int, int>> build_contact_candidates(const std::vector<SurfaceBBox>& master,
                                                          const std::vector<SurfaceBBox>& slave) {
  std::vector<std::pair<int, int>> out;
  for (int i = 0; i < static_cast<int>(master.size()); ++i) {
    for (int j = 0; j < static_cast<int>(slave.size()); ++j) {
      if (bbox_overlap(master[i], slave[j])) out.emplace_back(i, j);
    }
  }
  return out;
}

}  // namespace gptsolver
