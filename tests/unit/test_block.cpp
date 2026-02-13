#include <cassert>

#include "gptsolver/assembly/csr_matrix.hpp"

int main() {
  gptsolver::SparseMatrix a(2,2), b(2,2), c(2,2), d(2,2);
  a.setIdentity(); b.setIdentity(); c.setIdentity(); d.setIdentity();
  auto k = gptsolver::assemble_block_matrix(a,b,c,d);
  assert(k.rows()==4 && k.cols()==4);
  return 0;
}
