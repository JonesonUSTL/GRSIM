#include "gptsolver/assembly/csr_matrix.hpp"

namespace gptsolver {

SparseMatrix assemble_block_matrix(const SparseMatrix& kuu, const SparseMatrix& kut, const SparseMatrix& ktu,
                                   const SparseMatrix& ktt) {
  const int nu = static_cast<int>(kuu.rows());
  const int nt = static_cast<int>(ktt.rows());
  SparseMatrix out(nu + nt, nu + nt);
  std::vector<Triplet> t;
  t.reserve(static_cast<size_t>(kuu.nonZeros() + kut.nonZeros() + ktu.nonZeros() + ktt.nonZeros()));

  for (int k = 0; k < kuu.outerSize(); ++k) for (SparseMatrix::InnerIterator it(kuu, k); it; ++it) t.emplace_back(it.row(), it.col(), it.value());
  for (int k = 0; k < kut.outerSize(); ++k) for (SparseMatrix::InnerIterator it(kut, k); it; ++it) t.emplace_back(it.row(), it.col() + nu, it.value());
  for (int k = 0; k < ktu.outerSize(); ++k) for (SparseMatrix::InnerIterator it(ktu, k); it; ++it) t.emplace_back(it.row() + nu, it.col(), it.value());
  for (int k = 0; k < ktt.outerSize(); ++k) for (SparseMatrix::InnerIterator it(ktt, k); it; ++it) t.emplace_back(it.row() + nu, it.col() + nu, it.value());

  out.setFromTriplets(t.begin(), t.end());
  return out;
}

}  // namespace gptsolver
