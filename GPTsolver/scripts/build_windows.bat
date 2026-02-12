@echo off
setlocal
where cmake >nul 2>nul || (echo 未找到 CMake，请先运行 scripts\deploy_windows.ps1 & exit /b 1)
where ninja >nul 2>nul || (echo 未找到 Ninja，请先运行 scripts\deploy_windows.ps1 & exit /b 1)
cmake --preset default || exit /b 1
cmake --build --preset default -j 8 || exit /b 1
ctest --test-dir build/default --output-on-failure || exit /b 1
endlocal
