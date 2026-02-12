#include "gptsolver/model/interaction/surface.hpp"

#include <cmath>

namespace gptsolver {
namespace {
std::array<double, 3> sub(const std::array<double, 3>& x, const std::array<double, 3>& y) {
  return {x[0] - y[0], x[1] - y[1], x[2] - y[2]};
}
double dot(const std::array<double, 3>& x, const std::array<double, 3>& y) { return x[0] * y[0] + x[1] * y[1] + x[2] * y[2]; }
std::array<double, 3> cross(const std::array<double, 3>& x, const std::array<double, 3>& y) {
  return {x[1] * y[2] - x[2] * y[1], x[2] * y[0] - x[0] * y[2], x[0] * y[1] - x[1] * y[0]};
}
std::array<double, 3> scale(const std::array<double, 3>& x, double s) { return {x[0] * s, x[1] * s, x[2] * s}; }
}

std::optional<ProjectionResult> project_point_to_face(const std::array<double, 3>& p, const TriangleFace& f) {
  const auto ab = sub(f.b, f.a);
  const auto ac = sub(f.c, f.a);
  auto n = cross(ab, ac);
  const double n2 = dot(n, n);
  if (n2 < 1e-16) return std::nullopt;
  const double invn = 1.0 / std::sqrt(n2);
  n = scale(n, invn);

  const auto ap = sub(p, f.a);
  const double gap = dot(ap, n);
  const auto proj = sub(p, scale(n, gap));

  const auto v0 = ab;
  const auto v1 = ac;
  const auto v2 = sub(proj, f.a);
  const double d00 = dot(v0, v0), d01 = dot(v0, v1), d11 = dot(v1, v1);
  const double d20 = dot(v2, v0), d21 = dot(v2, v1);
  const double den = d00 * d11 - d01 * d01;
  if (std::abs(den) < 1e-16) return std::nullopt;
  const double v = (d11 * d20 - d01 * d21) / den;
  const double w = (d00 * d21 - d01 * d20) / den;
  const double u = 1.0 - v - w;
  const bool inside = (u >= -1e-10 && v >= -1e-10 && w >= -1e-10);
  return ProjectionResult{proj, gap, inside};
}

}  // namespace gptsolver
