#pragma once
#include <array>
#include <unordered_map>
#include <utility>
#include <vector>

#include "gptsolver/assembly/assembler_contact_placeholder.hpp"

namespace gptsolver {

/**
 * @brief 接触面包围盒，用于快速粗搜索。
 */
struct SurfaceBBox {
  double xmin{0}, xmax{0}, ymin{0}, ymax{0}, zmin{0}, zmax{0};
};

/**
 * @brief 网格桶 key。
 */
struct BucketKey {
  int ix{0}, iy{0}, iz{0};
  bool operator==(const BucketKey& o) const { return ix == o.ix && iy == o.iy && iz == o.iz; }
};

struct BucketKeyHash {
  std::size_t operator()(const BucketKey& k) const {
    return static_cast<std::size_t>((k.ix * 73856093) ^ (k.iy * 19349663) ^ (k.iz * 83492791));
  }
};

struct FaceProjectionResult {
  std::array<double, 3> projected{};
  std::array<double, 4> N{};  // quad bilinear shape functions
  std::array<double, 3> normal{0.0, 0.0, 1.0};
  std::array<double, 2> uv{0.0, 0.0};
  double gap{0.0};
  bool inside{false};
};

/**
 * @brief 面-面投影生成的接触状态（用于装配层输入）。
 */
struct FaceContactState {
  int master_node_dof{0};
  int slave_node_dof{1};
  FaceProjectionResult projection;
  ContactPointState to_point_state(double tangential_slip = 0.0) const {
    return ContactPointState{master_node_dof, slave_node_dof, projection.gap, tangential_slip, 0.0, projection.inside};
  }
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

/**
 * @brief 通过统一体素网格桶建立候选对，降低 O(N*M) 的常数开销。
 */
std::vector<std::pair<int, int>> build_contact_candidates_bucket(const std::vector<SurfaceBBox>& master,
                                                                 const std::vector<SurfaceBBox>& slave,
                                                                 double cell);

/**
 * @brief 面-面接触中的点到四边形面的局部投影（平面近似 + 双线性形函数）。
 */
FaceProjectionResult project_point_to_quad_face(const std::array<double, 3>& p,
                                                const std::array<std::array<double, 3>, 4>& face_nodes);

/**
 * @brief 从一组候选面和从面代表点中构造接触状态。
 */
std::vector<FaceContactState> build_face_contact_states(
    const std::vector<std::pair<int, int>>& candidates,
    const std::vector<std::array<std::array<double, 3>, 4>>& master_faces,
    const std::vector<std::array<double, 3>>& slave_points,
    const std::vector<std::pair<int, int>>& dof_pairs);

/**
 * @brief 一致切线近似（法向 penalty）: dfn/dgap
 */
inline double normal_contact_tangent(double penalty) { return penalty > 0.0 ? penalty : 0.0; }

}  // namespace gptsolver
