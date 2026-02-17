#pragma once

#include <string>

#include "gptsolver/assembly/csr_matrix.hpp"

namespace gptsolver {

void set_linear_solver_backend(const std::string& backend_name);
std::string get_linear_solver_backend();

/**
 * @brief PETSc 线性求解入口。若当前构建未启用 PETSc，返回零向量并置 ok=false。
 */
DenseVector solve_linear_petsc(const SparseMatrix& k, const DenseVector& f, bool& ok);

}  // namespace gptsolver
