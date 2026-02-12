#pragma once
#include <vector>

namespace gptsolver {

/**
 * @brief 接触面包围盒，用于快速粗搜索。
 */
struct SurfaceBBox {
  double xmin{0}, xmax{0}, ymin{0}, ymax{0}, zmin{0}, zmax{0};
};

/**
 * @brief 判断两个包围盒是否相交。
 */
bool bbox_overlap(const SurfaceBBox& a, const SurfaceBBox& b);

/**
 * @brief 输出候选接触对索引列表（master, slave）。
 */
std::vector<std::pair<int, int>> build_contact_candidates(const std::vector<SurfaceBBox>& master,
                                                          const std::vector<SurfaceBBox>& slave);

}  // namespace gptsolver
