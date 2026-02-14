@echo off
setlocal
pushd "%~dp0.." || exit /b 1

set BIN=build\default\gptsolver.exe

%BIN% check examples\inp\contact\contact_demo_120el.inp || goto :fail
%BIN% run examples\inp\static\static_bar.inp --out output\reg_static || goto :fail
%BIN% run examples\inp\heat\heat_rod.inp --out output\reg_heat || goto :fail
%BIN% run examples\inp\static\coupled_plate.inp --out output\reg_coupled || goto :fail
%BIN% run examples\inp\contact\contact_demo_120el.inp --out output\reg_contact || goto :fail
%BIN% run examples\inp\static\large_mesh_120el.inp --out output\reg_large || goto :fail

echo [OK] regression 完成
popd
endlocal
exit /b 0

:fail
echo [ERROR] regression 失败，请先完成构建。
popd
endlocal
exit /b 1
