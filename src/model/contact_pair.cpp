#include "gptsolver/model/interaction/contact_pair.hpp"

#include <algorithm>
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

std::array<double, 3> sub3(const std::array<double, 3>& a, const std::array<double, 3>& b) {
  return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}

std::array<double, 3> add3(const std::array<double, 3>& a, const std::array<double, 3>& b) {
  return {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
}

std::array<double, 3> mul3(const std::array<double, 3>& a, double s) { return {a[0] * s, a[1] * s, a[2] * s}; }

double dot3(const std::array<double, 3>& a, const std::array<double, 3>& b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }

std::array<double, 3> cross3(const std::array<double, 3>& a, const std::array<double, 3>& b) {
  return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
}

double norm3(const std::array<double, 3>& a) { return std::sqrt(dot3(a, a)); }
}  // namespace

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

FaceProjectionResult project_point_to_quad_face(const std::array<double, 3>& p,
                                                const std::array<std::array<double, 3>, 4>& face_nodes) {
  FaceProjectionResult r;
  const auto& x1 = face_nodes[0];
  const auto& x2 = face_nodes[1];
  const auto& x4 = face_nodes[3];

  const auto t1 = sub3(x2, x1);
  const auto t2 = sub3(x4, x1);
  auto n = cross3(t1, t2);
  const double nn = norm3(n);
  if (nn < 1e-14) return r;
  n = mul3(n, 1.0 / nn);

  const auto xp = sub3(p, x1);
  const double gap = dot3(xp, n);
  const auto proj = sub3(p, mul3(n, gap));

  const auto dp = sub3(proj, x1);
  const double a11 = dot3(t1, t1);
  const double a12 = dot3(t1, t2);
  const double a22 = dot3(t2, t2);
  const double b1 = dot3(dp, t1);
  const double b2 = dot3(dp, t2);
  const double det = a11 * a22 - a12 * a12;
  if (std::abs(det) < 1e-16) return r;

  const double u = (a22 * b1 - a12 * b2) / det;
  const double v = (-a12 * b1 + a11 * b2) / det;

  const double xi = 2.0 * u - 1.0;
  const double eta = 2.0 * v - 1.0;
  r.N = {0.25 * (1 - xi) * (1 - eta), 0.25 * (1 + xi) * (1 - eta), 0.25 * (1 + xi) * (1 + eta),
         0.25 * (1 - xi) * (1 + eta)};

  std::array<double, 3> recon{0, 0, 0};
  for (int i = 0; i < 4; ++i) recon = add3(recon, mul3(face_nodes[i], r.N[i]));

  r.projected = recon;
  r.normal = n;
  r.uv = {u, v};
  r.gap = gap;
  r.inside = (u >= -1e-8 && u <= 1.0 + 1e-8 && v >= -1e-8 && v <= 1.0 + 1e-8);
  return r;
}



FaceProjectionResult project_face_to_face(const std::array<std::array<double, 3>, 4>& slave_face,
                                          const std::array<std::array<double, 3>, 4>& master_face) {
  std::array<double, 3> center{0.0, 0.0, 0.0};
  for (const auto& p : slave_face) {
    center[0] += p[0];
    center[1] += p[1];
    center[2] += p[2];
  }
  center = mul3(center, 0.25);
  return project_point_to_quad_face(center, master_face);
}

std::vector<FaceContactState> build_face_contact_states(
    const std::vector<std::pair<int, int>>& candidates,
    const std::vector<std::array<std::array<double, 3>, 4>>& master_faces,
    const std::vector<std::array<double, 3>>& slave_points,
    const std::vector<std::pair<int, int>>& dof_pairs,
    std::vector<ContactHistoryState>* history,
    double slip_tolerance) {
  std::vector<FaceContactState> states;
  for (const auto& c : candidates) {
    const int mi = c.first;
    const int si = c.second;
    if (mi < 0 || si < 0 || mi >= static_cast<int>(master_faces.size()) || si >= static_cast<int>(slave_points.size()) ||
        si >= static_cast<int>(dof_pairs.size())) {
      continue;
    }

    auto proj = project_point_to_quad_face(slave_points[si], master_faces[mi]);
    if (!proj.inside) continue;

    FaceContactState st;
    st.master_node_dof = dof_pairs[si].first;
    st.slave_node_dof = dof_pairs[si].second;
    st.projection = proj;

    if (history) {
      if (history->size() <= static_cast<size_t>(si)) history->resize(static_cast<size_t>(si) + 1);
      auto& hs = (*history)[static_cast<size_t>(si)];
      const double du = proj.uv[0] - hs.last_u;
      const double dv = proj.uv[1] - hs.last_v;
      const double dslip = std::sqrt(du * du + dv * dv);
      hs.accumulated_slip += dslip;
      hs.last_u = proj.uv[0];
      hs.last_v = proj.uv[1];
      hs.stick = hs.accumulated_slip < std::max(1e-8, slip_tolerance);
      st.stick = hs.stick;
      st.projection.gap = proj.gap;
      if (!hs.stick) st.projection.gap = std::min(0.0, proj.gap);
    }

    states.push_back(st);
  }
  return states;
}

}  // namespace gptsolver
