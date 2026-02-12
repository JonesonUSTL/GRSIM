# Abaqus inp 兼容矩阵

## Supported-Executable
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
