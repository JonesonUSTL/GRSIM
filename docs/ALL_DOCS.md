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
