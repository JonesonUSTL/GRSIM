#!/usr/bin/env bash
set -e
# Apple Silicon (M4/ARM64) 推荐使用 clang + ninja
cmake --preset default -DCMAKE_OSX_ARCHITECTURES=arm64
cmake --build --preset default -j 8
ctest --test-dir build/default --output-on-failure
