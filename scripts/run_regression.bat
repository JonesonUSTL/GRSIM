@echo off
setlocal
pushd "%~dp0.." || exit /b 1

set BIN=build\default\gptsolver.exe

%BIN% check examples\inp\contact\contact_demo_120el.inp || goto :fail
%BIN% check examples\inp\official_like\official_dynamic_parse_only.inp || goto :fail

%BIN% run examples\inp\static\truss_tension.inp --out output\reg_truss || goto :fail
%BIN% run examples\inp\static\static_bar.inp --out output\reg_static || goto :fail
%BIN% run examples\inp\static\cantilever_beam.inp --out output\reg_cantilever || goto :fail
%BIN% run examples\inp\static\cantilever_tip_moment.inp --out output\reg_tip_moment || goto :fail
%BIN% run examples\inp\static\shear_patch.inp --out output\reg_shear || goto :fail

%BIN% run examples\inp\heat\heat_rod.inp --out output\reg_heat || goto :fail
%BIN% run examples\inp\heat\heat_flux_plate.inp --out output\reg_heat_flux || goto :fail
%BIN% run examples\inp\heat\heat_source_block.inp --out output\reg_heat_source || goto :fail

%BIN% run examples\inp\static\coupled_plate.inp --out output\reg_coupled || goto :fail
%BIN% run examples\inp\static\thermal_expansion_bar.inp --out output\reg_thermexp || goto :fail

%BIN% run examples\inp\contact\contact_demo.inp --out output\reg_contact_small || goto :fail
%BIN% run examples\inp\contact\contact_pair_friction_step.inp --out output\reg_contact_fric || goto :fail
%BIN% run examples\inp\contact\contact_demo_120el.inp --out output\reg_contact || goto :fail

%BIN% run examples\inp\plastic\plastic_demo.inp --out output\reg_plastic || goto :fail
%BIN% run examples\inp\plastic\j2_uniaxial_cycle.inp --out output\reg_j2 || goto :fail

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
