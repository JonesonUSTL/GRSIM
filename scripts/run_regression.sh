#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

bin=./build/default/gptsolver

$bin check examples/inp/contact/contact_demo_120el.inp
$bin check examples/inp/official_like/official_dynamic_parse_only.inp

$bin run examples/inp/static/truss_tension.inp --out output/reg_truss
$bin run examples/inp/static/static_bar.inp --out output/reg_static
$bin run examples/inp/static/cantilever_beam.inp --out output/reg_cantilever
$bin run examples/inp/static/cantilever_tip_moment.inp --out output/reg_tip_moment
$bin run examples/inp/static/shear_patch.inp --out output/reg_shear

$bin run examples/inp/heat/heat_rod.inp --out output/reg_heat
$bin run examples/inp/heat/heat_flux_plate.inp --out output/reg_heat_flux
$bin run examples/inp/heat/heat_source_block.inp --out output/reg_heat_source

$bin run examples/inp/static/coupled_plate.inp --out output/reg_coupled
$bin run examples/inp/static/thermal_expansion_bar.inp --out output/reg_thermexp

$bin run examples/inp/contact/contact_demo.inp --out output/reg_contact_small
$bin run examples/inp/contact/contact_pair_friction_step.inp --out output/reg_contact_fric
$bin run examples/inp/contact/contact_demo_120el.inp --out output/reg_contact

$bin run examples/inp/plastic/plastic_demo.inp --out output/reg_plastic
$bin run examples/inp/plastic/j2_uniaxial_cycle.inp --out output/reg_j2

$bin run examples/inp/static/large_mesh_120el.inp --out output/reg_large

echo "[OK] regression 完成"
