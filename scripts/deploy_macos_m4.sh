#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

echo "[1/7] 检查平台..."
uname -a
if [[ "$(uname -s)" != "Darwin" ]]; then
  echo "错误：该脚本仅用于 macOS"; exit 1
fi

echo "[2/7] 检查 Homebrew..."
if ! command -v brew >/dev/null 2>&1; then
  echo "安装 Homebrew..."
  /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

echo "[3/7] 安装依赖 cmake/ninja/llvm/libomp..."
brew update
brew install cmake ninja llvm libomp

echo "[4/7] 配置与构建 (ARM64)..."
cmake --preset default \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCMAKE_CXX_COMPILER=clang++
cmake --build --preset default -j 8

echo "[5/7] 运行测试..."
ctest --test-dir build/default --output-on-failure

echo "[6/7] 冒烟运行..."
./build/default/gptsolver run examples/inp/static/coupled_plate.inp --out output/deploy_macos_m4 --threads 8

echo "[7/7] 完成。若日志提示 OpenMP 不可用，已自动降级为串行构建。"
