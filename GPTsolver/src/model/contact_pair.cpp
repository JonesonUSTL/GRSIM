#include "gptsolver/model/interaction/contact_pair.hpp"

#include <cmath>
#include <set>

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

namespace {
int id(double v, double c) { return static_cast<int>(std::floor(v / c)); }
}

std::vector<std::pair<int, int>> build_contact_candidates_bucket(const std::vector<SurfaceBBox>& master,
                                                                 const std::vector<SurfaceBBox>& slave,
                                                                 double cell) {
  std::unordered_map<BucketKey, std::vector<int>, BucketKeyHash> m_bucket;
  for (int i = 0; i < static_cast<int>(master.size()); ++i) {
    const auto& b = master[i];
    for (int ix = id(b.xmin, cell); ix <= id(b.xmax, cell); ++ix)
      for (int iy = id(b.ymin, cell); iy <= id(b.ymax, cell); ++iy)
        for (int iz = id(b.zmin, cell); iz <= id(b.zmax, cell); ++iz) m_bucket[{ix, iy, iz}].push_back(i);
  }

  std::set<std::pair<int, int>> uniq;
  for (int j = 0; j < static_cast<int>(slave.size()); ++j) {
    const auto& b = slave[j];
    for (int ix = id(b.xmin, cell); ix <= id(b.xmax, cell); ++ix) {
      for (int iy = id(b.ymin, cell); iy <= id(b.ymax, cell); ++iy) {
        for (int iz = id(b.zmin, cell); iz <= id(b.zmax, cell); ++iz) {
          auto it = m_bucket.find({ix, iy, iz});
          if (it == m_bucket.end()) continue;
          for (int mi : it->second) {
            if (bbox_overlap(master[mi], b)) uniq.emplace(mi, j);
          }
        }
      }
    }
  }
  return {uniq.begin(), uniq.end()};
}

}  // namespace gptsolver
