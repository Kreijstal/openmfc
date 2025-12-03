#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT_DIR/build}"
DEF_FILE="${DEF_FILE:-$ROOT_DIR/def/mfc140u.def}"
DLL_NAME="openmfc.dll"

mkdir -p "$BUILD_DIR"

CXX=${CXX:-x86_64-w64-mingw32-g++}
CXXFLAGS=(
  -std=c++17
  -O2
  -Wall -Wextra
  -DOPENMFC_EXPORTS
  -I"$ROOT_DIR/include"
)
LDFLAGS=(
  -shared
  -static-libgcc -static-libstdc++
  -Wl,--out-implib,"$BUILD_DIR/libopenmfc.a"
)

echo "Building $DLL_NAME into $BUILD_DIR"

# Compile C and C++
"$CXX" "${CXXFLAGS[@]}" -c "$ROOT_DIR/src/mfc_dll.cpp" -o "$BUILD_DIR/mfc_dll.o"
"$CXX" "${CXXFLAGS[@]}" -c "$ROOT_DIR/src/object_impl.cpp" -o "$BUILD_DIR/object_impl.o"
"$CXX" "${CXXFLAGS[@]}" -c "$ROOT_DIR/src/dumpcont.cpp" -o "$BUILD_DIR/dumpcont.o"

# Assemble shim
"$CXX" -c "$ROOT_DIR/src/object.s" -o "$BUILD_DIR/object.o"

link_inputs=(
  "$BUILD_DIR/mfc_dll.o"
  "$BUILD_DIR/object_impl.o"
  "$BUILD_DIR/dumpcont.o"
  "$BUILD_DIR/object.o"
)

if [ -f "$DEF_FILE" ]; then
  link_inputs+=("$DEF_FILE")
fi

"$CXX" "${link_inputs[@]}" "${LDFLAGS[@]}" -o "$BUILD_DIR/$DLL_NAME"

echo "Output:"
ls -l "$BUILD_DIR"/$DLL_NAME "$BUILD_DIR"/libopenmfc.a 2>/dev/null || true
