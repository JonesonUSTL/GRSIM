# GPTsolver

工业级 CAE/FEM 平台骨架（对标 Abaqus 的长期路线）。

## 构建
```bash
cmake --preset default
cmake --build --preset default
ctest --preset default
```

## 运行
```bash
./build/default/gptsolver run examples/inp/static/static_bar.inp --out output/demo
```
