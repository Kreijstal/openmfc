#!/usr/bin/env bash
set -eu

# Phase 3: Hollow stub DLL build (no real implementations)
# Tests MSVC linking compatibility only

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD="${BUILD:-$ROOT/build-phase3}"

echo "Phase 3: Building hollow stub DLL for MSVC linking test"

# Clean and create build directory
rm -rf "$BUILD"
mkdir -p "$BUILD"

# Use MinGW cross-compiler
CXX="${CXX:-x86_64-w64-mingw32-g++}"
if ! command -v "$CXX" >/dev/null 2>&1; then
    echo "Error: MinGW compiler not found: $CXX"
    exit 1
fi

# Compiler flags for Windows DLL
CFLAGS=(
    -std=c++17
    -O2
    -Wall
    -Wextra
    -fPIC
    -D_WIN32_WINNT=0x0601
    -DUNICODE
    -D_UNICODE
    -I"$ROOT/include"
)

# Linker flags for Windows DLL
LDFLAGS=(
    -shared
    -static-libgcc
    -static-libstdc++
    -Wl,--enable-stdcall-fixup
    -Wl,--enable-auto-import
    -Wl,--whole-archive
    -Wl,--no-whole-archive
    -Wl,--output-def,"$BUILD/openmfc.def"
)

# Generate stubs and .def file using complete ordinal mapping
echo "Generating stubs from complete ordinal mapping..."
python3 "$ROOT/tools/gen_stubs.py" \
    --mapping "$ROOT/mfc_complete_ordinal_mapping.json" \
    --out-def "$BUILD/openmfc.def" \
    --out-stubs "$BUILD/stubs.cpp"

# Generate RTTI stubs
echo "Generating RTTI stubs..."
python3 "$ROOT/tools/gen_rtti.py" \
    --exceptions "$ROOT/exceptions.json" \
    --out-c "$BUILD/generated_rtti.c" \
    --out-h "$BUILD/generated_rtti.h"

# Compile stubs only (no real implementations)
echo "Compiling stubs..."
"$CXX" "${CFLAGS[@]}" -c "$BUILD/stubs.cpp" -o "$BUILD/stubs.o"
"$CXX" "${CFLAGS[@]}" -c "$BUILD/generated_rtti.c" -o "$BUILD/generated_rtti.o"

# Link DLL with stubs only
echo "Linking hollow DLL..."
"$CXX" "$BUILD/stubs.o" "$BUILD/generated_rtti.o" "$BUILD/openmfc.def" "${LDFLAGS[@]}" -o "$BUILD/openmfc.dll"

echo "✅ Phase 3 DLL built: $BUILD/openmfc.dll"
echo "   Exports: $(grep -c '^    ' "$BUILD/openmfc.def") symbols"
echo "   .def file: $BUILD/openmfc.def"