# Abaqus inp 兼容矩阵

## Supported-Executable
- *HEADING *PREPRINT *PART *ASSEMBLY *INSTANCE *END PART *END ASSEMBLY
- *NODE, *ELEMENT, *NSET, *ELSET
- *STEP, *STATIC, *HEAT TRANSFER, *END STEP
- *BOUNDARY, *CLOAD, *TEMPERATURE
- *SURFACE, *CONTACT PAIR, *SURFACE INTERACTION, *FRICTION（最小接触链路）
- *MPC（显式约束方程装配）
- *INCLUDE（递归解析，循环保护）
- *COUPLING, *KINEMATIC, *AMPLITUDE, *CONTROLS, *SOLUTION TECHNIQUE（解析层）
- *OUTPUT, *NODE OUTPUT, *ELEMENT OUTPUT（输出请求骨架）

## Parsed-Not-Solved
- *CONTACT CONTROLS *GENERAL CONTACT *SURFACE BEHAVIOR（解析层）
- *PLASTIC（仅 J2 演示，不是完整材料积分）
- *USER MATERIAL / *DEPVAR（接口已连通，未完整数值耦合）
- *DLOAD / *DSLOAD（解析存在，尚未完整装配）

## Not-Parsed
- 复杂接触控制（general contact 全选项）
- 完整壳/梁关键字与截面库

## *ELEMENT TYPE 支持现状
- 已支持语义检查：`C3D4/C3D6/C3D8/C3D8R/C3D10/C3D15/C3D20/C3D20R`、`CPE4/CPS4`、`T3D2`。
- 其中 `C3D8/C3D8R` 已接入演示求解主链；其余类型当前以“可解析+可检查”为主，未接入完整求解积分流程。
