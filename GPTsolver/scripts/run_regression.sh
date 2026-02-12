#!/usr/bin/env bash
set -e
./build/default/gptsolver check examples/inp/static/static_bar.inp
./build/default/gptsolver run examples/inp/static/static_bar.inp --out output/reg_static
./build/default/gptsolver run examples/inp/heat/heat_rod.inp --out output/reg_heat
