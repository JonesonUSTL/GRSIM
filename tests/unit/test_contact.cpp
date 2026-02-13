#include <cassert>

#include "gptsolver/assembly/assembler_contact_placeholder.hpp"

int main() {
  gptsolver::SparseMatrix k(4, 4);
  k.setIdentity();
  gptsolver::DenseVector r = gptsolver::DenseVector::Zero(4);
  std::vector<gptsolver::ContactPointState> cps = {{0, 1, -1e-3, 5e-4, 0, true}};
  gptsolver::assemble_contact_terms(cps, gptsolver::ContactParams{}, k, r);
  assert(r(0) > 0.0);
  return 0;
}
