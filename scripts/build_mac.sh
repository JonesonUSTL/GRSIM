#!/usr/bin/env bash
set -euo pipefail
if ! command -v cmake >/dev/null 2>&1; then
  echo "缺少 cmake，正在尝试安装（需 brew）";
  command -v brew >/dev/null 2>&1 || { echo "请先安装 Homebrew"; exit 1; }
  brew install cmake ninja
fi
cmake --preset default -DCMAKE_OSX_ARCHITECTURES=arm64
cmake --build --preset default -j 8
ctest --test-dir build/default --output-on-failure
