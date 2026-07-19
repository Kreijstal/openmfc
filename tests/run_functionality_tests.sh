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
#
# That restriction applies to *linking* only. A test that resolves exports
# dynamically -- LoadLibrary plus GetProcAddress on the exact mangled name --
# needs no import library and no compatible mangling, so it builds fine with
# MinGW and runs under Wine. The tests below use that approach.

CXX="${CXX_MINGW:-x86_64-w64-mingw32-g++}"

# Missing tooling means the tests do not run. Locally that should be a friendly
# skip, but in CI a silent skip is indistinguishable from a pass and would hide
# exactly the regressions these tests exist to catch -- so CI sets
# REQUIRE_MINGW=1 and a missing toolchain fails the job instead.
missing_tool() {
    if [[ "${REQUIRE_MINGW:-0}" == "1" ]]; then
        echo "❌ $1"
        echo "   REQUIRE_MINGW=1 is set, so this is a failure rather than a skip."
        exit 1
    fi
    echo "⚠️  $1 -- skipping runtime tests."
    exit 0
}

if ! command -v "$CXX" >/dev/null 2>&1; then
    missing_tool "MinGW compiler not found ($CXX); install mingw-w64"
fi

if ! command -v wine >/dev/null 2>&1 && ! command -v wine64 >/dev/null 2>&1; then
    missing_tool "Wine not found"
fi

WINE="$(command -v wine || command -v wine64)"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

# The DLL is loaded by name, so it has to sit next to the executable.
cp "$BUILD/openmfc.dll" "$WORK/"

# Dynamically-loaded runtime tests. Add new ones here.
TESTS=(
    test_embedded_subobject_vptr
)

total_fail=0

for t in "${TESTS[@]}"; do
    src="$ROOT/tests/$t.cpp"
    if [[ ! -f "$src" ]]; then
        echo "❌ missing test source: $src"
        total_fail=$((total_fail + 1))
        continue
    fi

    echo "--- $t ---"
    if ! "$CXX" -O1 "$src" -o "$WORK/$t.exe" 2>&1; then
        echo "❌ $t failed to compile"
        total_fail=$((total_fail + 1))
        continue
    fi

    # timeout is GNU coreutils and not present everywhere (macOS, slim images),
    # so fall back to running without it rather than reporting a spurious
    # failure. Unquoted on purpose: empty must expand to no argument at all.
    TIMEOUT_CMD=""
    if command -v timeout >/dev/null 2>&1; then
        TIMEOUT_CMD="timeout 60s"
    fi

    # WINEDEBUG quiets the loader chatter that otherwise buries the results.
    if (cd "$WORK" && WINEDEBUG=-all $TIMEOUT_CMD "$WINE" "$t.exe" 2>/dev/null); then
        echo "✅ $t passed"
    else
        echo "❌ $t FAILED"
        total_fail=$((total_fail + 1))
    fi
    echo ""
done

if [[ $total_fail -ne 0 ]]; then
    echo "❌ $total_fail functionality test(s) failed"
    exit 1
fi

echo "✅ All MinGW/Wine functionality tests passed."
echo "   MSVC linking tests run on Windows CI."