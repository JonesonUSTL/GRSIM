@echo off
setlocal
pushd "%~dp0.." || exit /b 1

echo [1/5] 配置...
cmake --preset default || goto :fail

echo [2/5] 构建...
cmake --build --preset default -j 8 || goto :fail

echo [3/5] 测试...
ctest --test-dir build/default --output-on-failure || goto :fail

echo [4/5] 冒烟运行...
build\default\gptsolver.exe run examples\inp\static\coupled_plate.inp --out output\deploy_windows_bat --threads 8 || goto :fail

echo [5/5] 完成
popd
endlocal
exit /b 0

:fail
echo [ERROR] 执行失败，请确认在 Developer Command Prompt 或已加载 MSVC 环境的终端中运行。
popd
endlocal
exit /b 1
