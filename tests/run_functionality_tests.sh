#!/usr/bin/env bash
set -eu

# Run MFC functionality tests (not just ABI tests)
# Tests actual behavior of implemented MFC functions
#
# IMPORTANT: MinGW cannot compile tests that use __declspec(dllimport) 
# declarations because MinGW generates GCC/Itanium mangled names
# while the OpenMFC DLL exports MSVC-mangled names.
#
# All MSVC-style tests (test_mfc_smoke.cpp, test_msvc_app.cpp, etc.)
# must be compiled and run on Windows with actual MSVC.
# See .github/workflows/phase3_verify.yml for the Windows CI tests.

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="${BUILD:-$ROOT/build-phase4}"

echo "=== OpenMFC Functionality Tests (MinGW/Wine) ==="
echo ""

# Check if DLL exists
if [[ ! -f "$BUILD/openmfc.dll" ]]; then
    echo "❌ OpenMFC DLL not found: $BUILD/openmfc.dll"
    echo "Build Phase 4 first: ./phase4/scripts/build_phase4.sh"
    exit 1
fi

# Note: test_mfc_smoke.cpp and test_msvc_app.cpp use __declspec(dllimport) 
# declarations that only work with MSVC. They cannot be compiled with MinGW 
# because MinGW generates GCC/Itanium mangled names while the DLL exports 
# MSVC-mangled names.
#
# These tests are run in CI on Windows with actual MSVC.
# See: .github/workflows/phase3_verify.yml
# See: .github/workflows/phase4_abi_safety.yml
# See: .github/workflows/phase4_msvc_linking.yml

echo "ℹ️  MinGW cannot directly link against MSVC-mangled exports."
echo "ℹ️  Running C/GetProcAddress behaviour tests; direct-link tests use MSVC CI."
echo ""

if ! command -v x86_64-w64-mingw32-gcc >/dev/null 2>&1; then
    echo "❌ x86_64-w64-mingw32-gcc is required"
    exit 1
fi

if command -v wine64 >/dev/null 2>&1; then
    WINE_BIN=wine64
elif command -v wine >/dev/null 2>&1; then
    WINE_BIN=wine
else
    echo "❌ Wine is required"
    exit 1
fi

TEST_DIR="$(mktemp -d /tmp/openmfc-functionality.XXXXXX)"
trap 'rm -rf "$TEST_DIR"' EXIT

cp "$BUILD/openmfc.dll" "$TEST_DIR/"
x86_64-w64-mingw32-gcc -std=c11 -O2 \
    "$ROOT/tests/test_animation_controller_state.c" \
    -o "$TEST_DIR/test_animation_controller_state.exe"

echo "Running CAnimationController state test..."
(
    cd "$TEST_DIR"
    WINEDEBUG=-all "$WINE_BIN" ./test_animation_controller_state.exe
)

echo ""
echo "✅ MinGW/Wine GetProcAddress functionality tests passed."
echo "   MSVC direct-link tests run separately on Windows CI."

exit 0
