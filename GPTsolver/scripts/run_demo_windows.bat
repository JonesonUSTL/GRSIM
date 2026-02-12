@echo off
build\default\gptsolver.exe run examples\inp\static\coupled_plate.inp --out output\demo_coupled_win --threads 8
build\default\gptsolver.exe run examples\inp\contact\contact_demo_120el.inp --out output\demo_contact_win --threads 8
