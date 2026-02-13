#pragma once
#include <Eigen/Sparse>
#include <vector>

namespace gptsolver {

/**
 * @brief 稀疏矩阵与向量类型别名。
 *
 * 说明：
 * - 这里统一封装 Eigen 的稀疏结构，后续如果切换 PETSc 仅需替换适配层。
 * - 所有装配模块都应优先写入 Triplet，再一次性构建 CSR。
 */
using SparseMatrix = Eigen::SparseMatrix<double, Eigen::RowMajor>;
using Triplet = Eigen::Triplet<double>;
using DenseVector = Eigen::VectorXd;

/**
 * @brief 将分块矩阵 [[Kuu, Kut],[Ktu, Ktt]] 组装成总稀疏矩阵。
 */
SparseMatrix assemble_block_matrix(const SparseMatrix& kuu, const SparseMatrix& kut, const SparseMatrix& ktu,
                                   const SparseMatrix& ktt);

}  // namespace gptsolver
