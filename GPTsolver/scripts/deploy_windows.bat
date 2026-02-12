@echo off
setlocal

echo [1/5] 配置...
cmake --preset default || exit /b 1

echo [2/5] 构建...
cmake --build --preset default -j 8 || exit /b 1

echo [3/5] 测试...
ctest --test-dir build/default --output-on-failure || exit /b 1

echo [4/5] 冒烟运行...
build\default\gptsolver.exe run examples\inp\static\coupled_plate.inp --out output\deploy_windows_bat --threads 8 || exit /b 1

echo [5/5] 完成
endlocal
