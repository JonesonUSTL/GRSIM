@echo off
setlocal
pushd "%~dp0.." || exit /b 1

build\default\gptsolver.exe run examples\inp\static\coupled_plate.inp --out output\demo_coupled_win --threads 8 || goto :fail
build\default\gptsolver.exe run examples\inp\contact\contact_demo_120el.inp --out output\demo_contact_win --threads 8 || goto :fail

echo [OK] demo 完成
popd
endlocal
exit /b 0

:fail
echo [ERROR] demo 执行失败，请先完成构建并确认运行目录正确。
popd
endlocal
exit /b 1
