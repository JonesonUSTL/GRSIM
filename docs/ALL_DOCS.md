# GPTsolver 文档总览（合并版）


---

## 来源：00_项目总览.md

# 项目总览

本项目目标是构建可持续扩展的工业级有限元平台。当前版本提供结构静力与稳态传热最小可运行链路。


---

## 来源：01_快速开始_Windows.md

# Windows 快速开始

1. 安装 Visual Studio + CMake + Ninja。
2. 在 **CMD** 中运行 `scripts\build_windows.bat`。
3. 在 **CMD** 中运行 `scripts\run_demo_windows.bat`。
4. 若使用 **PowerShell**：优先 `./scripts/deploy_windows.ps1`；如需运行批处理，请写 `./scripts/deploy_windows.bat`（不要省略 `./`）。
5. 所有 Windows 脚本都支持从任意目录启动，会自动切换到仓库根目录执行。


---

## 来源：02_快速开始_macOS.md

# macOS 快速开始

1. 安装 Xcode command line tools。
2. 运行 `./scripts/build_mac.sh`。
3. 运行 `./scripts/run_demo_mac.sh`。


---

## 来源：03_用户手册_从零开始.md

# 用户手册

`gptsolver run <inp> --out <dir>` 运行算例。`gptsolver check <inp>` 仅检查兼容性。


---

## 来源：04_架构设计说明.md

# 架构说明

采用 core/io/model/fem/physics/assembly/solver/ui 分层，便于后续接触、材料与多物理场扩展。


---

## 来源：05_ABAQUS_inp兼容矩阵.md

# Abaqus inp 兼容矩阵

## Supported-Executable
- *HEADING *PREPRINT *PART *ASSEMBLY *INSTANCE *END PART *END ASSEMBLY
- *NODE, *ELEMENT, *NSET, *ELSET
- *STEP, *STATIC, *HEAT TRANSFER, *END STEP
- *BOUNDARY, *CLOAD, *TEMPERATURE
- *SURFACE, *CONTACT PAIR, *SURFACE INTERACTION, *FRICTION（最小接触链路）
- *MPC（显式约束方程装配）
- *INCLUDE（递归解析，循环保护）
- *OUTPUT, *NODE OUTPUT, *ELEMENT OUTPUT（输出请求骨架）
- *SHELL SECTION, *SECTION CONTROLS（壳/稳定控制关键字骨架）

## Parsed-Not-Solved（对标 Abaqus 关键字）
- *AMPLITUDE
- *CONTACT CONTROLS
- *CONTACT DAMPING
- *CONTROLS
- *COUPLING
- *DEPVAR
- *DLOAD
- *DSLOAD
- *DYNAMIC
- *EL FILE
- *EXPANSION
- *FILM
- *GENERAL CONTACT
- *KINEMATIC
- *NODE FILE
- *ORIENTATION
- *PLASTIC
- *RADIATION
- *SOLUTION TECHNIQUE
- *SPECIFIC HEAT
- *SURFACE BEHAVIOR
- *USER MATERIAL
- *VISCO

## Semantic Checks（新增行为）
- *DLOAD/*DSLOAD：检查是否位于 *STEP 上下文。
- *FILM/*RADIATION/*SPECIFIC HEAT：检查是否处于热传导步。
- *GENERAL CONTACT：检查 *CONTACT CONTROLS/*SURFACE BEHAVIOR 配套。
- *USER MATERIAL：检查 *DEPVAR 配套。
- *NODE FILE/*EL FILE：检查 *OUTPUT 配套。
- *VISCO：检查 *DYNAMIC 步搭配。

## *ELEMENT TYPE 支持现状
- 已支持语义检查：`C3D4/C3D6/C3D8/C3D8R/C3D10/C3D15/C3D20/C3D20R`、`CPE4/CPS4`、`T3D2`。
- 其中 `C3D8/C3D8R` 已接入演示求解主链；其余类型当前以“可解析+可检查”为主，未接入完整求解积分流程。


---

## 来源：06_求解器算法说明.md

# 求解器算法

线性求解：`Ku=f`。
非线性框架：Newton-Raphson 演示实现，支持残差判据与迭代信息输出。


---

## 来源：07_接触与弹塑性设计说明.md

# 接触与弹塑性

接触模块提供表面、接触对、摩擦参数与面投影/法向一致切线近似。
J2 返回映射提供小变形径向回归与线性硬化演示实现。


---

## 来源：08_子程序接口指南.md

# 子程序接口

当前保留统一 ABAQUS-like ABI 与 UMAT 注册/调用链；UEL/DISP/UEXTERNALDB 处于规划阶段。


---

## 来源：09_并行与性能调优.md

# 并行与性能调优

- OpenMP：用于装配并行，建议在 4~16 线程区间做性能扫描。
- 线性求解：当前提供 Sparse LDLT、CG、分块 Schur 近似（耦合系统）。
- 接触搜索：新增 bbox + 网格桶候选构建，降低接触粗搜索常数开销。

## 调优建议
1. 优先减少接触候选数量（网格桶尺度 cell 建议与单元边长同量级）。
2. 对耦合系统可优先尝试 Schur 近似作为初值/预条件思路。
3. 大规模模型建议后续切换 PETSc/MPI（当前接口占位）。


---

## 来源：10_验证算例与对比.md

# 验证算例与对比

| 算例 | 规模 | 物理类型 | 状态 |
|---|---:|---|---|
| static_bar | 1 单元 | 结构静力 | 通过 |
| heat_rod | 1 单元 | 稳态传热 | 通过 |
| coupled_plate | 64 节点 | 结构-热耦合 | 通过（演示） |
| large_mesh_120el | 120 单元 | 非线性结构 | 通过（演示） |
| contact_demo_120el | 101+ 单元 | 接触+摩擦 | 通过（演示） |

> 说明：以上“通过”表示程序链路可运行并输出，不代表已与 Abaqus 数值逐点对齐。


---

## 来源：11_故障排查与FAQ.md

# FAQ

- 若报 Eigen 找不到：请安装 `libeigen3-dev`。
- 若 OpenMP 链接失败：关闭 `GPTSOLVER_ENABLE_OPENMP`。


---

## 来源：12_已知限制与路线图.md

# 已知限制与路线图

## 当前已完成
- 结构/热/耦合最小链路可运行。
- 接触残量/切线（法向+摩擦限幅）可运行。
- 弧长法 cutback + 半径调节 + 线搜索可运行。
- 接触 bbox 粗搜索与壳 hourglass 稳定项占位已加入。

## 仍在推进
1. 壳/实体真实多积分点与 hourglass 全量控制。
2. 接触搜索精化（网格桶 + 面面投影 + 一致切线）。
3. PETSc/MPI 真后端接入与分块预条件器。
4. Abaqus 关键字覆盖与行为对齐提升。

---

## 附录A：开发文档（目录结构 / 文件职责 / 调用关系）

> 本附录用于替代碎片化开发说明，后续每次代码变更均应同步更新本节。

### A.1 当前目录结构（精简后）

```text
apps/
  gptsolver_cli/main.cpp
cmake/
  CompilerWarnings.cmake
  Dependencies.cmake
  Options.cmake
docs/
  ALL_DOCS.md
examples/inp/
  contact/  heat/  official_like/  plastic/  static/  subroutine/
include/gptsolver/
  assembly/  core/  fem/  io/  model/  physics/  solver/  subroutine/  ui/
scripts/
  build_mac.sh  build_windows.bat  deploy_macos_m4.sh  deploy_windows.bat  deploy_windows.ps1
  package_release_local.sh  run_demo_mac.sh  run_demo_windows.bat  run_regression.bat  run_regression.sh
src/
  assembly/  core/  io/  model/  physics/  solver/  subroutine/  ui/  fem_shell_placeholder.cpp
tests/
  CMakeLists.txt
  unit/*.cpp
```

### A.2 核心文件职责（按调用链）

#### A.2.1 程序入口与调度
- `apps/gptsolver_cli/main.cpp`：命令行入口，解析 `run/check/info/examples/capabilities/roadmap`，组织运行目录、兼容报告、求解分发。

#### A.2.2 输入与兼容层
- `include/gptsolver/io/inp/lexer.hpp` + `src/io/inp/lexer.cpp`：词法拆分（关键字行/数据行）。
- `include/gptsolver/io/inp/parser.hpp` + `src/io/inp/parser.cpp`：构建 AST，处理 `*INCLUDE` 与循环保护。
- `include/gptsolver/io/inp/ast.hpp`：AST 结构定义（块、参数、数据、源位置信息）。
- `include/gptsolver/io/inp/keyword_dispatcher.hpp` + `src/io/inp/keyword_dispatcher.cpp`：关键字分级（可执行/已解析未求解/未知）。
- `include/gptsolver/io/inp/semantic.hpp` + `src/io/inp/semantic.cpp`：语义规则检查与告警生成。
- `include/gptsolver/io/inp/compatibility.hpp` + `src/io/inp/compatibility.cpp`：兼容报告落盘。

#### A.2.3 组装与线性系统
- `include/gptsolver/assembly/csr_matrix.hpp` + `src/assembly/csr_matrix.cpp`：CSR/稀疏矩阵辅助。
- `include/gptsolver/assembly/assembler_structural.hpp` + `src/assembly/assembler_structural.cpp`：结构刚度/载荷/MPC 组装。
- `include/gptsolver/assembly/assembler_thermal.hpp` + `src/assembly/assembler_thermal.cpp`：热方程组装、热-结构分块耦合切线。
- `include/gptsolver/assembly/assembler_contact_placeholder.hpp` + `src/assembly/assembler_contact_placeholder.cpp`：接触残量与切线近似装配。

#### A.2.4 求解器
- `include/gptsolver/solver/linear/eigen_direct.hpp` + `src/solver/linear/eigen_direct.cpp`：直接稀疏求解。
- `include/gptsolver/solver/linear/eigen_iterative.hpp` + `src/solver/linear/eigen_iterative.cpp`：CG 等迭代求解。
- `include/gptsolver/solver/linear/schur_preconditioner.hpp` + `src/solver/linear/schur_preconditioner.cpp`：分块 Schur 近似求解/预条件思路。
- `include/gptsolver/solver/nonlinear/newton_solver.hpp` + `src/solver/nonlinear/newton_solver.cpp`：Newton 与弧长流程。
- `include/gptsolver/solver/nonlinear/line_search.hpp` + `src/solver/nonlinear/line_search.cpp`：回溯线搜索。
- `include/gptsolver/solver/nonlinear/increment_controller.hpp` + `src/solver/nonlinear/increment_controller.cpp`：cutback 与步长调节。

#### A.2.5 物理问题层
- `include/gptsolver/physics/structural/structural_problem.hpp` + `src/physics/structural/structural_problem.cpp`：结构与热-结构耦合主链。
- `include/gptsolver/physics/thermal/thermal_problem.hpp` + `src/physics/thermal/thermal_problem.cpp`：稳态热主链。

#### A.2.6 接触、材料、单元辅助
- `include/gptsolver/model/interaction/surface.hpp` + `src/model/surface.cpp`：三角面投影。
- `include/gptsolver/model/interaction/contact_pair.hpp` + `src/model/contact_pair.cpp`：bbox/网格桶候选、四边形面投影、面状态到点状态映射。
- `include/gptsolver/model/material.hpp`：材料记录与材料库。
- `include/gptsolver/fem/constitutive/plastic/hardening_models.hpp`：硬化参数函数。
- `include/gptsolver/fem/constitutive/plastic/j2_return_mapping.hpp` + `src/fem/constitutive/plastic/j2_return_mapping.cpp`：J2 径向回归。
- `include/gptsolver/fem/element/shell_placeholder.hpp` + `src/fem_shell_placeholder.cpp`：壳/实体积分规则与 hourglass 稳定项估算。

#### A.2.7 输出、日志与子程序
- `include/gptsolver/io/vtk/vtu_writer.hpp` + `src/io/vtk/vtu_writer.cpp`：VTU 写出。
- `include/gptsolver/io/vtk/pvd_writer.hpp` + `src/io/vtk/pvd_writer.cpp`：PVD 时间序列索引。
- `include/gptsolver/io/restart/checkpoint.hpp` + `src/io/restart/checkpoint.cpp`：检查点保存/加载。
- `include/gptsolver/core/logger.hpp` + `src/core/logger.cpp`：日志写入。
- `include/gptsolver/core/error.hpp` + `src/core/error.cpp`：错误类型。
- `include/gptsolver/ui/console_dashboard.hpp` + `src/ui/console_dashboard.cpp`：控制台状态面板。
- `include/gptsolver/subroutine/abi/abaqus_like_types.hpp`：子程序 ABI 数据结构。
- `include/gptsolver/subroutine/abi/subroutine_api.hpp` + `src/subroutine/subroutine_api.cpp`：UMAT 注册与调用。

#### A.2.8 构建与测试
- `CMakeLists.txt`：目标、源文件、依赖与测试入口。
- `tests/CMakeLists.txt`：统一注册单测。
- `tests/unit/*.cpp`：单元测试（解析、接触、壳/实体积分、J2、Schur、耦合）。

### A.3 主要调用关系

1. **CLI 总流程**：
   `main.cpp -> parse/check -> (structural_problem | thermal_problem) -> vtk/restart/logger`。
2. **结构求解流程**：
   `structural_problem -> assembler_structural/contact -> newton_solver -> eigen/schur -> vtu/pvd`。
3. **热-结构耦合流程**：
   `structural_problem(coupled) -> assembler_thermal::build_thermo_structural_blocks -> solve_block_schur + CG`。
4. **接触流程**：
   `contact_pair (候选搜索+面投影) -> FaceContactState -> ContactPointState -> assemble_contact_terms`。
5. **材料流程**：
   `material library -> j2_return_mapping -> (后续并入单元积分点循环)`。

---

## 附录B：理论手册（教材版）

> 本附录给出当前代码包涉及的基础理论。后续每次算法更新，需要同步补充对应理论条目。

### B.1 有限元离散基础
1. **弱式与离散**：
   将控制方程通过加权残量转化为弱式，按单元形函数离散得到线性代数方程。
2. **单元积分**：
   采用高斯积分在参考单元上积分。当前支持壳单元 1x1/2x2 与实体 C3D8 1 点/8 点规则（演示）。
3. **装配**：
   单元刚度与载荷按自由度映射组装到全局矩阵。

### B.2 结构静力学
1. **线性问题**：
   \(K u = f\)，其中 \(K\) 为结构刚度，\(u\) 为位移，\(f\) 为外载。
2. **边界条件**：
   通过位移约束、载荷向量或罚/Lagrange 方式施加。

### B.3 稳态热传导
1. **控制方程**：
   \(-\nabla \cdot (k \nabla T) = q\)。
2. **离散形式**：
   \(K_t T = Q_t\)。

### B.4 热-结构耦合
1. **分块系统**：
   \[
   \begin{bmatrix}
   K_{uu} & K_{uT}\\
   K_{Tu} & K_{TT}
   \end{bmatrix}
   \begin{bmatrix}
   \Delta u\\
   \Delta T
   \end{bmatrix}
   =
   \begin{bmatrix}
   r_u\\
   r_T
   \end{bmatrix}
   \]
2. **热膨胀一致切线近似**：
   通过温度更新 \(K_{uT}\)，并取 \(K_{Tu} \approx K_{uT}^T\) 形成一致近似。

### B.5 非线性求解
1. **Newton-Raphson**：
   迭代解 \(K_t \Delta x = -R\)，更新状态直到残差与增量收敛。
2. **线搜索**：
   回溯缩放 \(\Delta x\) 提升收敛稳定性。
3. **cutback 与弧长法**：
   不收敛时减小增量；弧长法通过路径约束跨越极限点（RIKS 思想）。

### B.6 接触与摩擦
1. **法向接触（Penalty）**：
   若法向间隙 \(g_n < 0\)，施加 \(p_n = -k_n g_n\)。
2. **切向摩擦（Coulomb）**：
   \(|t_t| \le \mu p_n\)；超限时进入滑移投影。
3. **接触搜索**：
   先 bbox/网格桶粗筛，再进行面投影窄相。
4. **一致切线近似**：
   当前采用法向 penalty 导数近似 \(d f_n / d g_n\)。

### B.7 约束方程（MPC/Lagrange）
1. **MPC**：
   以线性约束关系绑定自由度。
2. **Lagrange/罚法近似**：
   将约束并入增广系统或等效刚度项。

### B.8 弹塑性 J2 返回映射
1. **屈服函数**：
   \(f = \sigma_{eq} - (\sigma_{y0} + H \bar\varepsilon_p)\)。
2. **径向回归**：
   试应力超屈服后，沿偏应力方向回归到屈服面。
3. **内部变量更新**：
   更新等效塑性应变与应力状态。

### B.9 分块 Schur 求解
1. **Schur 消元思想**：
   对耦合分块系统先消元一部分变量，降低耦合难度。
2. **工程价值**：
   可作为预条件器或初值求解器，提升大规模耦合系统效率。

### B.10 输出与后处理
1. **VTU/PVD**：
   VTU 保存场变量，PVD 组织时间序列。
2. **过程帧输出**：
   稳态也可按 `--frames` 输出过程帧，便于与 Abaqus 过程输出体验对齐。
