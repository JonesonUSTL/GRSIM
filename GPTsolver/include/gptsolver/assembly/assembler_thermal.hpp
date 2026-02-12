#pragma once
#include "gptsolver/assembly/csr_matrix.hpp"

namespace gptsolver {
SparseMatrix build_thermal_matrix_sparse(int n);
DenseVector build_thermal_rhs(int n);
SparseMatrix build_coupling_ut(int n, double alpha);
}
