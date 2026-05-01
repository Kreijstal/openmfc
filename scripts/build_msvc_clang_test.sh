#!/usr/bin/env bash
# Build and run MFC C++ tests using clang-cl + MSVC SDK + Wine
#
# Prerequisites:
#   - clang-cl / clang++ (with MSVC target support)
#   - lld-link / ld.lld
#   - MSVC SDK downloaded via msvc-wine (vsdownload.py + headers/lib files)
#   - Wine
#
# Usage:
#   MSVC_DIR=/tmp/msvc ./scripts/build_msvc_clang_test.sh
#   MSVC_DIR=/tmp/msvc ./scripts/build_msvc_clang_test.sh --test path/to/test.cpp
#
# Environment variables:
#   MSVC_DIR    - Path to MSVC SDK root (default: /tmp/msvc/unpack)
#   WINE_DEBUG  - Wine debug level (default: -all)
#   BUILD_DIR   - Output directory (default: /tmp/openmfc_msvc_test)

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "${SCRIPT_DIR}/.." && pwd)"

MSVC_DIR="${MSVC_DIR:-/tmp/msvc/unpack}"
BUILD_DIR="${BUILD_DIR:-/tmp/openmfc_msvc_test}"
WINE_DEBUG="${WINE_DEBUG:--all}"

# Auto-detect MSVC and SDK versions
MSVC_VER=$(ls -1 "$MSVC_DIR/VC/Tools/MSVC" 2>/dev/null | sort -V | tail -1 || true)
SDK_VER=$(ls -1 "$MSVC_DIR/Windows Kits/10/Include" 2>/dev/null | sort -V | tail -1 || true)

if [[ -z "$MSVC_VER" || -z "$SDK_VER" ]]; then
    echo "ERROR: MSVC SDK not found at $MSVC_DIR"
    echo "  Download with msvc-wine:"
    echo "    git clone --depth=1 https://github.com/mstorsjo/msvc-wine.git /tmp/msvc-wine"
    echo "    python3 /tmp/msvc-wine/vsdownload.py --accept-license --dest /tmp/msvc-dl"
    echo "    /tmp/msvc-wine/install.sh /tmp/msvc-dl"
    echo "  Then set MSVC_DIR=/tmp/msvc-dl"
    exit 1
fi

INC_MSVC="$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/include"
INC_UCRT="$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/ucrt"
INC_UM="$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/um"
INC_SHARED="$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/shared"

LIB_MSVC="$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/lib/x64"
LIB_UCRT="$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/ucrt/x64"
LIB_UM="$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/um/x64"

# Ensure case-insensitive symlinks exist on Linux
ensure_case_symlinks() {
    local dir="$1"
    for f in "$dir"/*.h "$dir"/*.idl "$dir"/*.Lib "$dir"/*.lib 2>/dev/null; do
        [ -f "$f" ] || continue
        local base lower upper
        base=$(basename "$f")
        lower=$(echo "$base" | tr '[:upper:]' '[:lower:]')
        upper=$(echo "$base" | tr '[:lower:]' '[:upper:]')
        if [ "$base" != "$lower" ] && [ ! -e "$dir/$lower" ]; then
            ln -sf "$base" "$dir/$lower"
        fi
        if [ "$base" != "$upper" ] && [ ! -e "$dir/$upper" ]; then
            ln -sf "$base" "$dir/$upper"
        fi
    done
}

for d in "$INC_UM" "$INC_SHARED" "$INC_UCRT" "$LIB_UM" "$LIB_UCRT" "$LIB_MSVC"; do
    [ -d "$d" ] && ensure_case_symlinks "$d"
done

mkdir -p "$BUILD_DIR"

# Step 1: Create import library from .def file
echo "=== Creating MSVC import library ==="
if [ ! -f "$BUILD_DIR/openmfc.lib" ] || [ "$REPO_ROOT/build-phase4/openmfc.def" -nt "$BUILD_DIR/openmfc.lib" ]; then
    llvm-lib /DEF:"$REPO_ROOT/build-phase4/openmfc.def" /OUT:"$BUILD_DIR/openmfc.lib" /MACHINE:X64
fi

# Step 2: Copy DLL
cp -f "$REPO_ROOT/build-phase4/openmfc.dll" "$BUILD_DIR/" 2>/dev/null || true

# Step 3: Build test
TEST_SRC="${1:-$REPO_ROOT/phase4/tests/test_mfc_abi_compat.cpp}"
OBJ_FILE="$BUILD_DIR/$(basename "${TEST_SRC%.cpp}").obj"
EXE_FILE="$BUILD_DIR/$(basename "${TEST_SRC%.cpp}").exe"

echo "=== Compiling $TEST_SRC ==="
clang-cl /nologo /EHsc /MD /std:c++17 /D_AFXDLL /W0 \
    -imsvc "$INC_MSVC" \
    -imsvc "$INC_UCRT" \
    -imsvc "$INC_UM" \
    -imsvc "$INC_SHARED" \
    -I"$REPO_ROOT/include" \
    -c "$TEST_SRC" \
    /Fo"$OBJ_FILE"

echo "=== Linking ==="
lld-link \
    /OUT:"$EXE_FILE" \
    /LIBPATH:"$BUILD_DIR" \
    /LIBPATH:"$LIB_MSVC" \
    /LIBPATH:"$LIB_UM" \
    /LIBPATH:"$LIB_UCRT" \
    /SUBSYSTEM:CONSOLE \
    /NODEFAULTLIB:mfc140u.lib \
    "$OBJ_FILE" \
    openmfc.lib user32.lib gdi32.lib kernel32.lib \
    msvcrt.lib vcruntime.lib ucrt.lib

echo "=== Running under Wine ==="
cd "$BUILD_DIR"
WINEDEBUG="$WINE_DEBUG" timeout 30 wine "./$(basename "$EXE_FILE")"
