#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT_DIR/build}"

mkdir -p "$BUILD_DIR"

CXX=${CXX:-x86_64-w64-mingw32-g++}
CXXFLAGS=(
  -std=c++17
  -O2
  -Wall -Wextra
  -DABI_INHERIT_EXPORTS
  -I"$ROOT_DIR/include"
)
LDFLAGS=(
  -shared
  -static-libgcc -static-libstdc++
  -Wl,--out-implib,"$BUILD_DIR/libabi_inherit.a"
)

echo "Building abi_inherit.dll into $BUILD_DIR"

"$CXX" "${CXXFLAGS[@]}" -c "$ROOT_DIR/src/abi_inherit_dllmain.cpp" -o "$BUILD_DIR/abi_inherit_dllmain.o"
"$CXX" -c "$ROOT_DIR/src/shim_inherit.s" -o "$BUILD_DIR/shim_inherit.o"

"$CXX" "$BUILD_DIR/shim_inherit.o" "$BUILD_DIR/abi_inherit_dllmain.o" "${LDFLAGS[@]}" -o "$BUILD_DIR/abi_inherit.dll"

echo "Output:"
ls -l "$BUILD_DIR"/abi_inherit.dll "$BUILD_DIR"/libabi_inherit.a 2>/dev/null || true
