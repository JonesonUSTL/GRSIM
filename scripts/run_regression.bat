@echo off
build\default\gptsolver.exe check examples\inp\static\static_bar.inp
build\default\gptsolver.exe run examples\inp\static\static_bar.inp --out output\reg_static
build\default\gptsolver.exe run examples\inp\heat\heat_rod.inp --out output\reg_heat
