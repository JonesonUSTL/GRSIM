#pragma once
#include "gptsolver/assembly/csr_matrix.hpp"

namespace gptsolver {

/**
 * @brief 分块 Schur 近似求解（演示版）。
 * 输入块矩阵 [[Kuu,Kut],[Ktu,Ktt]] 与 rhs=[ru;rt]。
 */
DenseVector solve_block_schur(const SparseMatrix& kuu, const SparseMatrix& kut, const SparseMatrix& ktu,
                              const SparseMatrix& ktt, const DenseVector& rhs);

}  // namespace gptsolver
