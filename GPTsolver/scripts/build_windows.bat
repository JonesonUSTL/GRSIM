@echo off
setlocal
pushd "%~dp0.." || exit /b 1
where cmake >nul 2>nul || (echo 未找到 CMake，请先运行 .\scripts\deploy_windows.ps1（PowerShell）或在 CMD 中执行 scripts\deploy_windows.bat & goto :fail)
where ninja >nul 2>nul || (echo 未找到 Ninja，请先运行 .\scripts\deploy_windows.ps1（PowerShell）或在 CMD 中执行 scripts\deploy_windows.bat & goto :fail)
cmake --preset default || goto :fail
cmake --build --preset default -j 8 || goto :fail
ctest --test-dir build/default --output-on-failure || goto :fail

echo [OK] build + test 完成
popd
endlocal
exit /b 0

:fail
popd
endlocal
exit /b 1
