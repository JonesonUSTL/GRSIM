#pragma once
#include "gptsolver/assembly/csr_matrix.hpp"

namespace gptsolver {
DenseVector solve_linear_sparse(const SparseMatrix& k, const DenseVector& f);
}
