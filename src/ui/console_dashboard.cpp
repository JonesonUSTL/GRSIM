#include "gptsolver/ui/console_dashboard.hpp"

#include <iostream>

namespace gptsolver {
void render_dashboard(const StatusSnapshot& s) {
  std::cout << "================ GPTsolver 状态面板 ================\n";
  std::cout << "工程: " << s.job_name << " | 输入: " << s.input_file << " | 输出: " << s.output_dir << "\n";
  std::cout << "Step/Inc/Iter: " << s.step << '/' << s.increment << '/' << s.iteration << "\n";
  std::cout << "残差: " << s.residual_norm << " | 增量范数: " << s.increment_norm << " | 能量误差: " << s.energy_error << "\n";
  std::cout << "线性迭代: " << s.linear_iterations << " | 已用时(s): " << s.elapsed_sec << " | 最近帧: " << s.latest_frame << "\n";
  std::cout << "接触活跃点: " << s.contact_active << " | 最大穿透: " << s.max_penetration << "\n";
}
}  // namespace gptsolver
