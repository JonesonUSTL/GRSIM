#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

./build/default/gptsolver run examples/inp/static/coupled_plate.inp --out output/demo_coupled_mac --threads 8
./build/default/gptsolver run examples/inp/contact/contact_demo_120el.inp --out output/demo_contact_mac --threads 8

echo "[OK] mac demo 运行完成"
