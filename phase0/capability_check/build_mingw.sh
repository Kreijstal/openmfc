#!/usr/bin/env bash
set -e

# Ensure output directory exists
mkdir -p build_output

# Compiler (adjust if your system uses a different alias)
CXX=${CXX:-x86_64-w64-mingw32-g++}

echo "Building mingw_dll.dll..."
$CXX -shared -o build_output/mingw_dll.dll mingw_dll.cpp mingw_dll.def \
    -static-libgcc -static-libstdc++ \
    -Wl,--kill-at

echo "Done. Artifact: build_output/mingw_dll.dll"
