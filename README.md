# GPTsolver（Abaqus 对标路线：非线性结构静力 + 热学 + 耦合）

> 这是一个“可运行 + 可扩展”的 C++20 FEM 平台。当前版本强调**工程骨架完整性**与**可持续扩展**，并已具备接触/MPC/弧长法/结构-热耦合的可执行演示链路。本轮继续做了“文件与目录瘦身”：移除大量空占位头文件，并合并碎片化单元测试。

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
脚本会自动执行：平台检查、Homebrew 检查与安装、依赖安装（cmake/ninja/llvm/libomp）、构建、测试、冒烟运行。若 OpenMP 不可用，会自动降级为串行构建，不再报错中断。

### 2.2 Windows（PowerShell）
```powershell
./scripts/deploy_windows.ps1
# 或跳过依赖安装：./scripts/deploy_windows.ps1 -SkipInstall
```
脚本会自动执行：依赖安装（可跳过）、构建、测试、冒烟运行；并且可从任意当前目录启动（脚本会自动切到仓库根目录）。

### 2.3 Windows（CMD）
```bat
scripts\deploy_windows.bat
```
> 若在 **PowerShell** 中调用 `.bat`，请使用 `./scripts/deploy_windows.bat` 或 `cmd /c scripts\deploy_windows.bat`。
> 直接输入 `scripts\deploy_windows.bat` 会被 PowerShell 按“模块限定命令”解析，触发 `CouldNotAutoLoadModule`。


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
gptsolver run <model.inp> --out <dir> [--threads N] [--solver-backend eigen|petsc] [--resume checkpoint.bin] [--frames N] [--schur-blend 0..1]
gptsolver check <model.inp> [--fail-on-unknown]
gptsolver info
gptsolver examples --list
gptsolver examples --run <name>
```

- `examples --list` 现在按 **文件名平铺** 展示（如 `official_cantilever_main`），不再要求带目录前缀。
- `examples --run <name>` 支持直接传文件名，也支持 `official_like/official_contact_main` 这种相对路径。

---

- `check` 会输出 `Parsed-Not-Solved` 与 `Unknown` 统计；可加 `--fail-on-unknown` 让 CI 在未知关键字时失败。
- 单元 `TYPE` 现在会在 `check` 阶段进行语义检查：已覆盖多种实体单元（如 `C3D10/C3D20R`），未支持类型会给出明确告警。

## 5. 支持清单（重点）

### 5.1 单元支持现状

| 类别 | 单元 | 状态 |
|---|---|---|
| 实体 | C3D4 / C3D6 / C3D8 / C3D8R / C3D10 / C3D15 / C3D20 / C3D20R | 解析与兼容检查已支持；C3D8/C3D8R 已接入演示求解链路 |
| 平面 | CPS4 / CPE4 | 接口/占位 + 求解演示链路 |
| 杆 | T3D2 | 可用于示例模型 |
| 梁 | B31/B33（规划） | 规划中 |
| 壳 | shell_placeholder | 已提供 1x1/2x2 积分规则 + hourglass 稳定系数估算（演示版） |

### 5.2 算法支持现状

| 模块 | 能力 | 状态 |
|---|---|---|
| 线性求解 | Sparse LDLT | 已实现 |
| 迭代求解 | CG + 分块 Schur 近似 | 已实现 |
| 非线性 | Newton + 弧长法 | 已实现（演示级） |
| 材料 | J2 返回映射（各向同性+随动硬化+温度屈服修正）+ 多材料库 | 已实现（工程化基础） |
| 线搜索 | Backtracking | 已实现 |
| 增量控制 | cutback + radius 自适应 | 已实现 |
| 接触 | 法向 penalty + 切向摩擦限幅 + bbox/网格桶候选 + 面-面投影 + 一致切线近似 + 粘滑历史变量 | 已实现（工程化基础） |
| 约束 | MPC/Lagrange（penalty 近似） | 已实现 |
| 耦合 | 结构-热分块组装与联立求解 | 已实现 |
| 并行 | OpenMP 装配并行 | 已实现 |
| PETSc | 线性求解后端（可选） | 已接入（单机可选；MPI 分布式主链下一阶段） |

### 5.3 功能支持现状

- [x] inp 解析 + AST + 兼容检查
- [x] INCLUDE 递归 + 循环保护
- [x] 日志 + 清单 + 兼容报告 + 检查点
- [x] VTU/PVD 多文件输出
- [x] 运行状态面板（TUI）
- [x] 壳单元多积分点（1x1/2x2 演示）+ hourglass 稳定项系数（演示）
- [x] J2 返回映射（各向同性+随动硬化+温度屈服修正）+ 多材料库耦合骨架

### 5.4 inp 关键字（当前，Abaqus 对标）

**Supported-Executable / 部分可执行**
- `*HEADING, *PREPRINT, *PART, *ASSEMBLY, *INSTANCE, *END PART, *END ASSEMBLY`
- `*NODE, *ELEMENT, *NSET, *ELSET, *SOLID SECTION, *SHELL SECTION, *SECTION CONTROLS`
- `*MATERIAL, *ELASTIC, *DENSITY, *CONDUCTIVITY`
- `*STEP, *STATIC, *HEAT TRANSFER, *END STEP`
- `*BOUNDARY, *CLOAD, *TEMPERATURE`
- `*SURFACE, *CONTACT PAIR, *SURFACE INTERACTION, *FRICTION`
- `*MPC, *INCLUDE, *RESTART, *OUTPUT, *NODE OUTPUT, *ELEMENT OUTPUT`

**Parsed-Not-Solved（含本轮重点关键字）**
- `*AMPLITUDE`
- `*CONTACT CONTROLS`
- `*CONTACT DAMPING`
- `*CONTROLS`
- `*COUPLING`
- `*DEPVAR`
- `*DLOAD`
- `*DSLOAD`
- `*DYNAMIC`
- `*EL FILE`
- `*EXPANSION`
- `*FILM`
- `*GENERAL CONTACT`
- `*KINEMATIC`
- `*NODE FILE`
- `*ORIENTATION`
- `*PLASTIC`
- `*RADIATION`
- `*SOLUTION TECHNIQUE`
- `*SPECIFIC HEAT`
- `*SURFACE BEHAVIOR`
- `*USER MATERIAL`
- `*VISCO`

**语义行为补充（本轮新增）**
- 对上述 parsed-not-solved 关键字增加上下文一致性检查（如热学步、接触搭配、`USER MATERIAL` 与 `DEPVAR` 搭配、`DLOAD/DSLOAD` 步内使用等）。
- `*ELEMENT, TYPE=...` 支持实体单元族语义检查（含 C3D4/C3D6/C3D8/C3D8R/C3D10/C3D15/C3D20/C3D20R）。

---

## 6. 帧输出（对标 Abaqus 过程输出）

- 稳态问题也支持过程帧输出。
- `--frames N` 控制输出帧数，默认 `10`。
- 输出文件：`results/step_1/frame_0001.vtu ... frame_00NN.vtu` 与 `results.pvd`。

## 7. 案例与回归

### 7.1 案例库（已扩充）

当前已内置 20+ 个示例，覆盖结构/热/耦合/接触/塑性/官方风格解析基线。可通过 `gptsolver examples --list` 查看完整清单。

| 类别 | 案例 | 路径 | 目标能力 |
|---|---|---|---|
| 结构基础 | truss_tension | `examples/inp/static/truss_tension.inp` | 杆单元拉伸、位移输出 |
| 结构基础 | static_bar | `examples/inp/static/static_bar.inp` | 线性静力、约束+载荷 |
| 结构基础 | cantilever_beam | `examples/inp/static/cantilever_beam.inp` | 悬臂工况 |
| 结构扩展 | cantilever_tip_moment | `examples/inp/static/cantilever_tip_moment.inp` | 端部复合载荷 |
| 结构扩展 | shear_patch | `examples/inp/static/shear_patch.inp` | 面内剪切基准 |
| 热学基础 | heat_rod | `examples/inp/heat/heat_rod.inp` | 稳态导热 |
| 热学扩展 | heat_flux_plate | `examples/inp/heat/heat_flux_plate.inp` | 热流边界 |
| 热学扩展 | heat_source_block | `examples/inp/heat/heat_source_block.inp` | 体热源载荷 |
| 耦合 | coupled_plate | `examples/inp/static/coupled_plate.inp` | 热-结构强耦合 |
| 耦合 | thermal_expansion_bar | `examples/inp/static/thermal_expansion_bar.inp` | 热膨胀一致切线近似 |
| 接触 | contact_demo | `examples/inp/contact/contact_demo.inp` | 接触流程最小链路 |
| 接触 | contact_pair_friction_step | `examples/inp/contact/contact_pair_friction_step.inp` | 摩擦接触参数链路 |
| 接触规模化 | contact_demo_120el | `examples/inp/contact/contact_demo_120el.inp` | 网格桶搜索+投影 |
| 塑性 | plastic_demo | `examples/inp/plastic/plastic_demo.inp` | 塑性关键字链路 |
| 塑性 | j2_uniaxial_cycle | `examples/inp/plastic/j2_uniaxial_cycle.inp` | J2 单轴路径演示 |
| 官方风格 | official_cantilever_main | `examples/inp/official_like/official_cantilever_main.inp` | PART/ASSEMBLY/INCLUDE |
| 官方风格 | official_contact_main | `examples/inp/official_like/official_contact_main.inp` | CONTACT 关键字组合 |
| 官方风格 | official_dynamic_parse_only | `examples/inp/official_like/official_dynamic_parse_only.inp` | DYNAMIC/CONTROLS 解析基线 |
| 约束 | official_mpc_coupling_main | `examples/inp/official_like/official_mpc_coupling_main.inp` | MPC/COUPLING/KINEMATIC |
| 大规模演示 | large_mesh_120el | `examples/inp/static/large_mesh_120el.inp` | 中规模回归与日志观察 |

### 7.2 一键运行方法（帮助文档重点）

**macOS/Linux（已构建）**
```bash
./scripts/run_demo_mac.sh
./scripts/run_regression.sh
```

**Windows CMD（已构建）**
```bat
./scripts/run_demo_windows.bat
./scripts/run_regression.bat
```

**只跑指定案例**
```bash
gptsolver examples --run heat_flux_plate
./build/default/gptsolver run examples/inp/heat/heat_flux_plate.inp --out output/demo_heat_flux --frames 10
```

### 7.3 回归命令（手工分步）

```bash
./build/default/gptsolver check examples/inp/contact/contact_demo_120el.inp
./build/default/gptsolver run examples/inp/static/truss_tension.inp --out output/reg_truss
./build/default/gptsolver run examples/inp/heat/heat_source_block.inp --out output/reg_heat_source
./build/default/gptsolver run examples/inp/static/coupled_plate.inp --out output/reg_coupled
./build/default/gptsolver run examples/inp/contact/contact_pair_friction_step.inp --out output/reg_contact_fric
./build/default/gptsolver run examples/inp/static/large_mesh_120el.inp --out output/reg_large
```

---

## 8. 文档导航

- `docs/ALL_DOCS.md`（统一总文档，内含：用户手册 + 开发文档附录A + 理论手册附录B）

---


## 8.1 精简后的核心目录

```text
apps/                # CLI 入口
include/gptsolver/   # 仅保留有效接口头文件
src/                 # 核心实现
tests/unit/          # 合并后的单元测试
examples/inp/        # 回归算例
scripts/             # 双平台构建/部署脚本
docs/ALL_DOCS.md     # 合并文档
```

## 9. 下一步升级建议（本版后）

> 本轮按你的要求已完成以下项，并已接入可运行链路：

- ✅ S4/S4R、C3D8R 一致线性化积分（已接入规则、hourglass 与一致切线接口）；
- ✅ GENERAL CONTACT / CONTACT CONTROLS 数值主链工程化基础（候选搜索、窄相投影、粘滑历史变量、控制参数）；
- ✅ J2 完整工程化基础（各向同性 + 随动硬化 + 温度屈服修正）；
- ✅ PETSc 可选后端与 Schur 预条件混合权重可配置（`--solver-backend petsc`、`--schur-blend`）。

### 下一阶段重点
1. MPI 分布式网格装配与并行 KSP 规模化。
2. 接触一致切线与全局牛顿耦合（CONTACT CONTROLS 全参数联动）。
3. J2 一致切线与多积分点单元联动（壳/实体真实积分流程）。

---

## 10. 对标 Abaqus 可继续增加的功能清单

### 10.1 近期（建议优先）
1. 接触窄相：面-面投影 + 一致切线 + 粘滑状态历史变量。
2. 壳单元：S4/S4R 多积分点与 hourglass 全量控制。
3. 求解器：分块 Schur 预条件器（可配置）与 PETSc/MPI 真后端。
4. 材料：J2 完整返回映射（各向同性/随动硬化）与温度相关参数。

### 10.2 中期
1. 动力学：隐式动力学（HHT/Newmark）与模态分析。
2. 多物理场：热-结构强耦合、热膨胀一致切线。
3. 子程序：UMAT/UEL 更完整 ABI 与样例插件。

### 10.3 关键字扩展目标
- `*COUPLING, *KINEMATIC, *AMPLITUDE, *CONTROLS, *SOLUTION TECHNIQUE`（本版已解析）
- 后续：`*CONTACT CONTROLS, *GENERAL CONTACT, *SURFACE BEHAVIOR, *FILM, *RADIATION`


## 11. Abaqus 官方风格 inp 丰富化（当前进展）

已新增官方常见组织方式示例：
- PART/ASSEMBLY/INSTANCE
- INCLUDE 网格拆分
- MATERIAL + ELASTIC/DENSITY/CONDUCTIVITY/SPECIFIC HEAT
- STEP + STATIC/HEAT TRANSFER + OUTPUT

后续可继续增加：
- *GENERAL CONTACT + *CONTACT CONTROLS 数值主链
- *FILM/*RADIATION 热边界
- *EL FILE/*NODE FILE 输出频率与变量子集
