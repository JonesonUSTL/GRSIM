# GPTsolver（Abaqus 对标路线的结构-热非线性 FEM 平台）

> 当前版本：v0.2（可运行核心 + 可扩展骨架）
>
> 说明：本项目已实现结构静力、稳态热、结构-热耦合、接触残量/切线刚度（法向+摩擦最小实现）、MPC/Lagrange 显式约束装配、弧长法（cutback + RIKS 风格半径调节）演示链路。完整工业级求解能力仍在迭代中。

---

## 1. 功能总览

- **结构静力**：稀疏装配 + 非线性弧长法求解。
- **稳态传热**：稀疏导热矩阵求解。
- **结构-热耦合**：显式分块矩阵组装 `[[Kuu,Kut],[Ktu,Ktt]]`。
- **接触**：法向 penalty + 切向库仑摩擦（stick/slip 骨架）。
- **约束**：MPC/Lagrange 方程显式装配（penalty 近似）。
- **并行**：OpenMP 装配并行。
- **I/O**：Abaqus 风格 inp 解析、兼容报告、VTU/PVD、checkpoint、run_manifest。
- **CLI**：`run / check / info / examples`。
- **跨平台**：Windows / macOS（Apple Silicon ARM64）脚本。

---

## 2. 一键构建与运行

### Linux/macOS（含 M4 ARM）
```bash
./scripts/build_mac.sh
./scripts/run_demo_mac.sh
```

### Windows
```bat
scripts\build_windows.bat
scripts\run_demo_windows.bat
```

---

## 3. 命令行使用

```bash
gptsolver run <model.inp> --out <dir> [--threads N] [--solver-backend eigen|petsc] [--resume checkpoint.bin]
gptsolver check <model.inp>
gptsolver info
gptsolver examples --list
gptsolver examples --run <name>
```

---

## 4. 经典案例（包含百单元级）

- `examples/inp/static/static_bar.inp`
- `examples/inp/static/large_mesh_120el.inp`（120 单元）
- `examples/inp/static/coupled_plate.inp`（结构+热同一步）
- `examples/inp/contact/contact_demo_120el.inp`（接触+摩擦+百单元级）
- `examples/inp/heat/heat_rod.inp`

---

## 5. 理论与算法手册（简版）

### 5.1 接触残量与切线刚度
- 法向：`pn = -kn * gap (gap<0)`。
- 切向：`ft = -kt * slip`，并施加 `|ft| <= mu*pn`。
- 切线：法向 `kn`，切向 `kt(粘着)/~0(滑移)`。

### 5.2 MPC/Lagrange 显式装配
- 约束 `u_m - r*u_s = 0`。
- 通过 penalty 近似加入刚度：
  - `K_mm += p`
  - `K_ss += p*r^2`
  - `K_ms += -p*r`
  - `K_sm += -p*r`

### 5.3 弧长法（RIKS 风格最小实现）
- 迭代中控制增量长度 `||Δu|| <= radius`。
- 试探步收敛改善时放大半径；否则 cutback 降半径。
- 支持最大 cutback 次数限制。

---

## 6. 运行输出目录

每次运行创建：`output/run_YYYYMMDD_HHMMSS/`

包含：
- `run.log`
- `run_manifest.json`
- `compatibility_report.md`
- `summary.md`
- `results/step_x/frame_yyyy.vtu`
- `results/results.pvd`
- `checkpoint_*.bin`（结构链路）

---

## 7. 输入关键字支持（本版）

已解析并进入可执行或半执行链路：
- `*STEP, *STATIC, *HEAT TRANSFER, *BOUNDARY, *CLOAD, *TEMPERATURE`
- `*SURFACE, *CONTACT PAIR, *SURFACE INTERACTION, *FRICTION`
- `*MPC, *INCLUDE, *OUTPUT, *NODE OUTPUT, *ELEMENT OUTPUT`

其余关键字：
- 不崩溃；记录在兼容报告中并给出告警。

---

## 8. 开发文档（快速）

- 核心代码入口：`apps/gptsolver_cli/main.cpp`
- 解析链路：`src/io/inp/*`
- 装配链路：`src/assembly/*`
- 非线性与弧长：`src/solver/nonlinear/newton_solver.cpp`
- 耦合/接触/MPC 示例：`src/physics/structural/structural_problem.cpp`

扩展建议：
1. 将接触点状态迁移到元素/面高斯点级别并加入邻域搜索。
2. 引入真实壳/实体积分点与 hourglass 控制（B-bar / assumed strain）。
3. 引入分块预条件（Schur complement）提升耦合求解规模。
4. 完善 PETSc 后端与 MPI 并行。

---

## 9. 下一步升级建议（每次运行建议）

当前程序在 `summary.md` 中自动写出下一步升级建议。建议优先级：
1. 接触搜索与一致切线（提高收敛稳健性）
2. 壳单元与 hourglass 控制
3. 塑性积分与材料库扩展
4. 动力学与多物理场耦合
