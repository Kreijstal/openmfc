#!/usr/bin/env bash
set -eu

# Phase 4: Real implementations with weak stub fallbacks
#
# CRITICAL: All implementations must be extern "C" with stub names!
# We cannot use C++ classes directly - GCC mangles differently than MSVC.
#
# Strategy:
# 1. Generate weak stubs for ALL 14k exports (ABI safety net)
# 2. Implement specific functions by overriding their stub_xxx names
# 3. The .def file maps stub names → MSVC-mangled exports
# 4. Verify all required exports are present

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD="${BUILD:-$ROOT/build-phase4}"

echo "Phase 4: Building OpenMFC with real implementations"
echo "=================================================="

# Clean and create build directory
rm -rf "$BUILD"
mkdir -p "$BUILD/include/openmfc"

# Compiler setup
CXX="${CXX:-x86_64-w64-mingw32-g++}"

if ! command -v "$CXX" >/dev/null 2>&1; then
    echo "Error: MinGW compiler not found: $CXX"
    exit 1
fi

# Common flags
CFLAGS=(
    -std=c++17
    -O2
    -Wall
    -fPIC
    -D_WIN32_WINNT=0x0601
    -DUNICODE
    -D_UNICODE
    -DOPENMFC_EXPORTS
    -I"$ROOT/include"
    -I"$BUILD/include"
    -Wno-attributes
)

# Step 1: Generate weak stubs
echo ""
echo "[1/4] Generating weak stubs..."

# Symbols to exclude (already implemented)
EXCLUDED_SYMBOLS="?AfxThrowMemoryException@@YAXXZ,?AfxThrowFileException@@YAXHJPB_W@Z,?AfxThrowResourceException@@YAXXZ,?AfxThrowNotSupportedException@@YAXXZ,?AfxThrowUserException@@YAXXZ,?AfxThrowInvalidArgException@@YAXXZ,?AfxAbort@@YAXXZ,?AfxGetDllVersion@@YAKXZ"

python3 "$ROOT/tools/gen_weak_stubs.py" \
    --mapping "$ROOT/mfc_complete_ordinal_mapping.json" \
    --out-def "$BUILD/openmfc.def" \
    --out-stubs "$BUILD/weak_stubs.cpp" \
    --exclude "$EXCLUDED_SYMBOLS"

# Step 2: Generate RTTI metadata
echo ""
echo "[2/4] Generating RTTI metadata..."
python3 "$ROOT/tools/gen_rtti.py" \
    --exceptions "$ROOT/exceptions.json" \
    --out-c "$BUILD/generated_rtti.c" \
    --out-h "$BUILD/include/openmfc/eh_rtti.h"

# Step 3: Compile everything
echo ""
echo "[3/4] Compiling..."
"$CXX" "${CFLAGS[@]}" -c "$BUILD/weak_stubs.cpp" -o "$BUILD/weak_stubs.o"
"$CXX" "${CFLAGS[@]}" -c "$BUILD/generated_rtti.c" -o "$BUILD/generated_rtti.o"

# Compile implementation files
IMPL_SOURCES=(
    "$ROOT/phase4/src/mfc_exceptions.cpp"
    "$ROOT/phase4/src/version_impl.cpp"
    # Add more implementation files here as they are created
)

for src in "${IMPL_SOURCES[@]}"; do
    if [[ -f "$src" ]]; then
        obj="${src%.cpp}.o"
        echo "  Compiling implementation: $(basename "$src")"
        "$CXX" "${CFLAGS[@]}" -c "$src" -o "$obj"
    fi
done

# Step 4: Link DLL
echo ""
echo "[4/4] Linking openmfc.dll..."

LDFLAGS=(
    -shared
    -static-libgcc
    -static-libstdc++
    -Wl,--enable-stdcall-fixup
    -Wl,--enable-auto-import
    -Wl,"$BUILD/openmfc.def"
    -Wl,--out-implib,"$BUILD/libopenmfc.a"
)

# Collect all object files
OBJ_FILES=(
    "$BUILD/generated_rtti.o"
    "$BUILD/weak_stubs.o"
)

# Add implementation object files
for src in "${IMPL_SOURCES[@]}"; do
    if [[ -f "$src" ]]; then
        obj="${src%.cpp}.o"
        OBJ_FILES+=("$obj")
    fi
done

echo "  Linking with ${#OBJ_FILES[@]} object files"
"$CXX" \
    "${OBJ_FILES[@]}" \
    "$BUILD/openmfc.def" \
    "${LDFLAGS[@]}" \
    -o "$BUILD/openmfc.dll"

echo ""
echo "=================================================="
echo "Build complete!"
echo "  DLL: $BUILD/openmfc.dll"
echo "  Import lib: $BUILD/libopenmfc.a"

# Verify ABI
echo ""
echo "Verifying ABI exports..."
python3 "$ROOT/scripts/verify_abi_exports.py" \
    --mapping "$ROOT/mfc_complete_ordinal_mapping.json" \
    --dll "$BUILD/openmfc.dll"

# Run comprehensive safety check
echo ""
echo "Running comprehensive ABI safety check..."
"$ROOT/scripts/phase4_safety_check.sh"


