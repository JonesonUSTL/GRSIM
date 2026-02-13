#!/usr/bin/env bash
set -euo pipefail
cmake --preset default
cmake --build --preset default -j 8
ctest --test-dir build/default --output-on-failure
os=$(uname -s | tr '[:upper:]' '[:lower:]')
arch=$(uname -m)
name="gptsolver-${os}-${arch}-release"
rm -rf "${name}" "${name}.tar.gz"
mkdir -p "${name}/bin" "${name}/examples"
cp build/default/gptsolver "${name}/bin/"
cp -r examples/inp "${name}/examples/"
cp README.md "${name}/"
tar -czf "${name}.tar.gz" "${name}"
echo "package: ${name}.tar.gz"
