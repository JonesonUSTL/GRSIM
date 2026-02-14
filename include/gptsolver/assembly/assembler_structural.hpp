#pragma once
#include "gptsolver/assembly/csr_matrix.hpp"

namespace gptsolver {

/**
 * @brief 生成结构主刚度（演示：对角稀疏刚度）。
 */
SparseMatrix build_structural_stiffness_sparse(int n);
DenseVector build_structural_load(int n);

/**
 * @brief MPI 分布式组装演示：按 rank 切分行段并归并全局对角刚度/载荷。
 */
SparseMatrix build_structural_stiffness_distributed(int n);
DenseVector build_structural_load_distributed(int n);

/**
 * @brief 显式组装 MPC/Lagrange 约束方程。
 * 约束形式：u(master)-ratio*u(slave)=value。
 */
SparseMatrix assemble_mpc_lagrange(int ndof, int master, int slave, double ratio, double penalty = 1e8);

}  // namespace gptsolver
