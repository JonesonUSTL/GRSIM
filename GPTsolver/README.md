# GPTsolver（Abaqus 对标路线：非线性结构静力 + 热学 + 耦合）

> 这是一个“可运行 + 可扩展”的 C++20 FEM 平台。当前版本强调**工程骨架完整性**与**可持续扩展**，并已具备接触/MPC/弧长法/结构-热耦合的可执行演示链路，并新增接触 bbox 粗搜索与壳 hourglass 稳定项占位。

---

## 1. 你现在可以直接做什么

- 读取 Abaqus 风格 `.inp` 并执行 `check`/`run`。
- 跑结构静力、稳态热、结构-热耦合演示算例。
- 使用接触（法向 penalty + 摩擦限幅）与 MPC 约束演示。
- 使用非线性弧长法（cutback + 半径自适应）演示。
- 输出 VTU/PVD、run 日志、兼容报告、checkpoint、summary。
- 在 macOS（Apple Silicon/M4）与 Windows 使用一键部署脚本。

---

## 2. 一键部署（含环境配置）

### 2.1 macOS（Apple Silicon/M4）
```bash
./scripts/deploy_macos_m4.sh
```
脚本会自动执行：平台检查、Homebrew 检查与安装、依赖安装（cmake/ninja/llvm）、构建、测试、冒烟运行。

### 2.2 Windows（PowerShell）
```powershell
.\scripts\deploy_windows.ps1
```
脚本会自动执行：winget 安装 CMake/Ninja/Git、构建、测试、冒烟运行。

### 2.3 Windows（CMD）
```bat
scripts\deploy_windows.bat
```

---

## 3. 快速开始

```bash
cmake --preset default
cmake --build --preset default -j 8
ctest --test-dir build/default --output-on-failure
./build/default/gptsolver examples --list
./build/default/gptsolver run examples/inp/static/coupled_plate.inp --out output/demo --threads 8
```

---

## 4. 命令行

```bash
gptsolver run <model.inp> --out <dir> [--threads N] [--solver-backend eigen|petsc] [--resume checkpoint.bin]
gptsolver check <model.inp>
gptsolver info
gptsolver examples --list
gptsolver examples --run <name>
```

---

## 5. 支持清单（重点）

### 5.1 单元支持现状

| 类别 | 单元 | 状态 |
|---|---|---|
| 实体 | C3D8 / C3D8R | 接口/占位 + 求解演示链路 |
| 平面 | CPS4 / CPE4 | 接口/占位 + 求解演示链路 |
| 杆 | T3D2 | 可用于示例模型 |
| 梁 | beam_placeholder | 占位 |
| 壳 | shell_placeholder | 已提供积分点策略与 hourglass 稳定项占位 |

### 5.2 算法支持现状

| 模块 | 能力 | 状态 |
|---|---|---|
| 线性求解 | Sparse LDLT | 已实现 |
| 迭代求解 | CG | 已实现 |
| 非线性 | Newton + 弧长法 | 已实现（演示级） |
| 线搜索 | Backtracking | 已实现 |
| 增量控制 | cutback + radius 自适应 | 已实现 |
| 接触 | 法向 penalty + 切向摩擦限幅 | 已实现（演示级） |
| 约束 | MPC/Lagrange（penalty 近似） | 已实现 |
| 耦合 | 结构-热分块组装与联立求解 | 已实现 |
| 并行 | OpenMP 装配并行 | 已实现 |
| PETSc | 后端接口 | 占位（CLI 会给出回退提示） |

### 5.3 功能支持现状

- [x] inp 解析 + AST + 兼容检查
- [x] INCLUDE 递归 + 循环保护
- [x] 日志 + 清单 + 兼容报告 + 检查点
- [x] VTU/PVD 多文件输出
- [x] 运行状态面板（TUI）
- [ ] 壳单元多积分点 + hourglass 控制（下一阶段）
- [ ] 完整 J2 返回映射与多材料库耦合（下一阶段）

### 5.4 inp 关键字（当前）

**Supported-Executable / 部分可执行**
- `*HEADING, *NODE, *ELEMENT, *NSET, *ELSET`
- `*STEP, *STATIC, *HEAT TRANSFER, *END STEP`
- `*BOUNDARY, *CLOAD, *TEMPERATURE`
- `*SURFACE, *CONTACT PAIR, *SURFACE INTERACTION, *FRICTION`
- `*MPC, *INCLUDE, *OUTPUT, *NODE OUTPUT, *ELEMENT OUTPUT`

**Parsed-Not-Solved**
- `*PLASTIC, *USER MATERIAL, *DEPVAR, *DLOAD, *DSLOAD`（解析有，主链路仍在完善）

---

## 6. 案例与回归

```bash
./build/default/gptsolver run examples/inp/static/static_bar.inp --out output/reg
./build/default/gptsolver run examples/inp/heat/heat_rod.inp --out output/reg
./build/default/gptsolver run examples/inp/static/coupled_plate.inp --out output/reg
./build/default/gptsolver run examples/inp/contact/contact_demo_120el.inp --out output/reg
./build/default/gptsolver run examples/inp/static/large_mesh_120el.inp --out output/reg
```

新增中等规模案例：
- `examples/inp/static/large_mesh_120el.inp`
- `examples/inp/contact/contact_demo_120el.inp`
- `examples/inp/static/coupled_plate.inp`

---

## 7. 文档导航

- `docs/03_用户手册_从零开始.md`
- `docs/06_求解器算法说明.md`
- `docs/07_接触与弹塑性设计说明.md`
- `docs/05_ABAQUS_inp兼容矩阵.md`
- `docs/12_已知限制与路线图.md`

---

## 8. 下一步升级建议（本版后）

1. 壳/实体真实积分点与 hourglass 控制（优先）。
2. 接触搜索从 bbox 粗筛升级到网格桶 + 面-面投影 + 一致切线。
3. 稀疏分块预条件器（Schur）+ PETSc/MPI 后端打通。
4. 更完整的 Abaqus keyword 行为对齐与回归基线。
