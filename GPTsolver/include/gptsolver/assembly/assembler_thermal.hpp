#pragma once
#include <tuple>

#include "gptsolver/assembly/csr_matrix.hpp"

namespace gptsolver {
SparseMatrix build_thermal_matrix_sparse(int n);
DenseVector build_thermal_rhs(int n);
SparseMatrix build_coupling_ut(int n, double alpha);

/**
 * @brief 热膨胀一致切线近似项 dR_u/dT（演示版，按对角线组装）。
 */
SparseMatrix build_thermal_expansion_tangent(int n, double young_modulus, double expansion_coeff, double delta_t);

/**
 * @brief 构造强耦合热-结构分块矩阵（Kuu,Kut,Ktu,Ktt），其中 Ktu=Kut^T 保证一致性近似。
 */
std::tuple<SparseMatrix, SparseMatrix, SparseMatrix, SparseMatrix> build_thermo_structural_blocks(
    int n, double expansion_coeff, double conductivity_scale, double young_modulus, double delta_t);
}  // namespace gptsolver
