#!/usr/bin/env bash
set -e
cmake --preset default
cmake --build --preset default
ctest --preset default
