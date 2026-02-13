#!/usr/bin/env bash
set -e
bin=./build/default/gptsolver
$bin check examples/inp/contact/contact_demo_120el.inp
$bin run examples/inp/static/static_bar.inp --out output/reg_static
$bin run examples/inp/heat/heat_rod.inp --out output/reg_heat
$bin run examples/inp/static/coupled_plate.inp --out output/reg_coupled
$bin run examples/inp/contact/contact_demo_120el.inp --out output/reg_contact
$bin run examples/inp/static/large_mesh_120el.inp --out output/reg_large
