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
echo "ℹ️  MSVC linking tests run on Windows CI instead."
echo ""
echo "✅ No MinGW-compatible functionality tests to run."
echo "   MSVC tests will run on Windows CI."

exit 0