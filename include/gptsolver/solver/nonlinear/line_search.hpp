#pragma once
#include "gptsolver/assembly/csr_matrix.hpp"

namespace gptsolver {
/**
 * @brief 简化回溯线搜索。
 * @param k 切线刚度
 * @param rhs 外载
 * @param x 当前位移
 * @param dx 牛顿增量
 * @param lambda 载荷系数
 * @return alpha 缩放系数 (0,1]
 */
double backtracking_line_search(const SparseMatrix& k, const DenseVector& rhs, const DenseVector& x, const DenseVector& dx,
                                double lambda);
}
