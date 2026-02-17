#pragma once
#include <vector>

#include "gptsolver/assembly/csr_matrix.hpp"

namespace gptsolver {

/**
 * @brief 接触状态（最小可运行版本）。
 * normal_gap < 0 表示穿透；pressure 为罚函数法法向接触力。
 */
struct ContactPointState {
  int dof_n{0};
  int dof_t{0};
  double normal_gap{0.0};
  double tangential_slip{0.0};
  double pressure{0.0};
  bool stick{true};
};

/**
 * @brief 接触参数：全参数控制（CONTACT CONTROLS 联动）。
 */
struct ContactParams {
  double kn{1.0e4};
  double mu{0.2};
  double damping{0.0};
  double slip_tol{1.0e-2};
  double stick_stiff_ratio{0.1};
  double slip_stiffness{1.0e-8};
};

/**
 * @brief 装配接触残量与切线刚度（法向接触+库仑摩擦）。
 */
void assemble_contact_terms(const std::vector<ContactPointState>& cps, const ContactParams& params, SparseMatrix& kt,
                            DenseVector& rt);

}  // namespace gptsolver
