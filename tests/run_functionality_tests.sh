#!/usr/bin/env bash
set -eu

# Run MFC functionality tests (not just ABI tests)
# Tests actual behavior of implemented MFC functions

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="${BUILD:-$ROOT/build-phase4}"



# Check if DLL exists
if [[ ! -f "$BUILD/openmfc.dll" ]]; then
    echo "❌ OpenMFC DLL not found: $BUILD/openmfc.dll"
    echo "Build Phase 4 first: ./phase4/scripts/build_phase4.sh"
    exit 1
fi

# Create test directory
TEST_DIR="/tmp/openmfc_func_test_$$"
mkdir -p "$TEST_DIR"
cd "$TEST_DIR"



# Copy DLL and create import library
cp "$BUILD/openmfc.dll" .
cp "$BUILD/openmfc.def" .

x86_64-w64-mingw32-dlltool -d openmfc.def -l libopenmfc.a -D openmfc.dll
if [[ $? -ne 0 ]]; then
    exit 1
fi

# Compile each test
TESTS=(
    "test_version_functionality.cpp"
    "test_exception_functionality.cpp"
    "test_mfc_smoke.cpp"
)

FAILED_TESTS=()
PASSED_TESTS=()

for test_src in "${TESTS[@]}"; do
    test_name="${test_src%.cpp}"
    
    x86_64-w64-mingw32-g++ -std=c++17 -O2 \
        -I "$ROOT/include" \
        "$ROOT/tests/$test_src" \
        -L. -lopenmfc \
        -o "$test_name.exe"
    
    if [[ $? -ne 0 ]]; then
        FAILED_TESTS+=("$test_name")
        continue
    fi
    
    if wine64 "$test_name.exe" 2>&1 | grep -v "fixme:" >/dev/null; then
        if [[ ${PIPESTATUS[0]} -eq 0 ]]; then
            PASSED_TESTS+=("$test_name")
        else
            FAILED_TESTS+=("$test_name")
        fi
    else
        if [[ ${PIPESTATUS[0]} -eq 0 ]]; then
            PASSED_TESTS+=("$test_name")
        else
            FAILED_TESTS+=("$test_name")
        fi
    fi
done

x86_64-w64-mingw32-g++ -std=c++17 -O2 \
    -I "$ROOT/include" \
    "$ROOT/tests/test_msvc_app.cpp" \
    -L. -lopenmfc \
    -o test_msvc_app.exe

if wine64 ./test_msvc_app.exe 2>&1 | grep -v "fixme:" | grep -q "All tests passed"; then
    PASSED_TESTS+=("test_msvc_app")
else
    FAILED_TESTS+=("test_msvc_app")
fi

if [[ ${#FAILED_TESTS[@]} -eq 0 ]]; then
    RESULT=0
else
    RESULT=1
fi

# Cleanup
cd /
rm -rf "$TEST_DIR"

exit $RESULT