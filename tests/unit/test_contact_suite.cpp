#include <cassert>

#include "gptsolver/assembly/assembler_contact_placeholder.hpp"
#include "gptsolver/model/interaction/contact_pair.hpp"
#include "gptsolver/model/interaction/surface.hpp"

int main() {
  {
    gptsolver::SparseMatrix k(4, 4);
    k.setIdentity();
    gptsolver::DenseVector r = gptsolver::DenseVector::Zero(4);
    std::vector<gptsolver::ContactPointState> cps = {{0, 1, -1e-3, 5e-4, 0, true}};
    gptsolver::assemble_contact_terms(cps, gptsolver::ContactParams{}, k, r);
    assert(r(0) > 0.0);
  }

  {
    gptsolver::SurfaceBBox a{0, 1, 0, 1, 0, 1};
    gptsolver::SurfaceBBox b{0.5, 1.5, 0.5, 1.5, 0.5, 1.5};
    gptsolver::SurfaceBBox c{2, 3, 2, 3, 2, 3};
    assert(gptsolver::bbox_overlap(a, b));
    assert(!gptsolver::bbox_overlap(a, c));
    auto cand = gptsolver::build_contact_candidates({a}, {b, c});
    assert(cand.size() == 1);
  }

  {
    std::vector<gptsolver::SurfaceBBox> m = {{0, 1, 0, 1, 0, 1}, {5, 6, 5, 6, 5, 6}};
    std::vector<gptsolver::SurfaceBBox> s = {{0.5, 1.2, 0.5, 1.2, 0.5, 1.2}, {5.1, 5.9, 5.1, 5.9, 5.1, 5.9}};
    auto cand = gptsolver::build_contact_candidates_bucket(m, s, 1.0);
    assert(cand.size() >= 2);
  }

  {
    gptsolver::TriangleFace f{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};
    auto r = gptsolver::project_point_to_face({0.2, 0.2, 0.5}, f);
    assert(r.has_value());
    assert(r->inside);
    assert(r->gap > 0.49 && r->gap < 0.51);
  }

  {
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
  }

  return 0;
}
