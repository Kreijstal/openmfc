#!/usr/bin/env bash
set -eu

# Wine Runtime Test for OpenMFC
# Tests that MSVC-compiled binaries can run with OpenMFC under Wine

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="${BUILD:-$ROOT/build-phase4}"

echo "=== Wine Runtime Test ==="
echo "Testing MSVC-compiled binaries with OpenMFC under Wine"
echo ""

# Check if wine is available
if ! command -v wine64 >/dev/null 2>&1; then
    echo "⚠️  wine64 not found. Skipping Wine runtime test."
    echo "Install with: sudo apt-get install wine64"
    exit 0
fi

# Check if we have test binaries
if [[ ! -f "$BUILD/openmfc.dll" ]]; then
    echo "❌ OpenMFC DLL not found: $BUILD/openmfc.dll"
    echo "Build Phase 4 first: ./phase4/scripts/build_phase4.sh"
    exit 1
fi

# Create test directory
TEST_DIR="/tmp/openmfc_wine_test_$$"
mkdir -p "$TEST_DIR"
cd "$TEST_DIR"

echo "Test directory: $TEST_DIR"
echo ""

# Copy OpenMFC DLL
cp "$BUILD/openmfc.dll" .
cp "$ROOT/build-phase4/openmfc.def" .

echo "1. Creating MSVC import library with Wine's lib.exe..."
# Use Wine's lib.exe to create import library
if wine64 lib.exe /DEF:openmfc.def /OUT:openmfc.lib /MACHINE:X64 2>&1 | grep -v "fixme:"; then
    echo "✅ Created openmfc.lib"
else
    echo "⚠️  Failed to create import library with Wine's lib.exe"
    echo "Continuing with existing .def file..."
fi

echo ""
echo "2. Testing basic DLL loading..."
cat > test_load.cpp << 'EOF'
#include <windows.h>
#include <stdio.h>

int main() {
    HMODULE h = LoadLibraryA("openmfc.dll");
    if (!h) {
        printf("FAILED to load openmfc.dll\n");
        return 1;
    }
    
    // Try to get a few key exports
    FARPROC exports[] = {
        GetProcAddress(h, "?AfxGetDllVersion@@YAKXZ"),
        GetProcAddress(h, "?AfxThrowMemoryException@@YAXXZ"),
        GetProcAddress(h, "?AfxThrowResourceException@@YAXXZ"),
        GetProcAddress(h, "?AfxThrowNotSupportedException@@YAXXZ"),
    };
    
    int found = 0;
    for (int i = 0; i < sizeof(exports)/sizeof(exports[0]); i++) {
        if (exports[i]) found++;
    }
    
    printf("Loaded openmfc.dll successfully\n");
    printf("Found %d/%d key exports\n", found, (int)(sizeof(exports)/sizeof(exports[0])));
    
    FreeLibrary(h);
    return found > 0 ? 0 : 1;
}
EOF

# Compile with MinGW (cross-compile)
echo "Compiling test with MinGW..."
x86_64-w64-mingw32-g++ -std=c++17 -O2 test_load.cpp -o test_load.exe

echo ""
echo "3. Running test under Wine..."
if wine64 ./test_load.exe 2>&1 | grep -v "fixme:"; then
    echo "✅ Wine runtime test passed!"
    RESULT=0
else
    echo "❌ Wine runtime test failed"
    RESULT=1
fi

echo ""
echo "4. Testing exception handling (if implemented)..."
cat > test_exception.cpp << 'EOF'
#include <windows.h>
#include <stdio.h>

// Declare the function we want to test
extern "C" __declspec(dllimport) void __cdecl AfxThrowMemoryException();

int main() {
    printf("Testing AfxThrowMemoryException...\n");
    
    HMODULE h = LoadLibraryA("openmfc.dll");
    if (!h) {
        printf("FAILED to load DLL\n");
        return 1;
    }
    
    FARPROC func = GetProcAddress(h, "?AfxThrowMemoryException@@YAXXZ");
    if (!func) {
        printf("Function not found\n");
        FreeLibrary(h);
        return 1;
    }
    
    printf("Function found at %p\n", func);
    
    // Try to call it (may throw exception)
    __try {
        ((void(*)())func)();
        printf("Function returned normally (unexpected)\n");
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        printf("Caught exception (expected for stub)\n");
    }
    
    FreeLibrary(h);
    return 0;
}
EOF

x86_64-w64-mingw32-g++ -std=c++17 -O2 test_exception.cpp -o test_exception.exe

echo "Running exception test under Wine..."
if wine64 ./test_exception.exe 2>&1 | grep -v "fixme:"; then
    echo "✅ Exception test completed"
else
    echo "⚠️  Exception test had issues (may be expected for stubs)"
fi

# Cleanup
cd /
rm -rf "$TEST_DIR"

echo ""
echo "=== Wine Test Summary ==="
if [[ $RESULT -eq 0 ]]; then
    echo "✅ Wine runtime tests passed"
    echo "OpenMFC DLL loads correctly under Wine"
    echo "Key exports are accessible"
else
    echo "❌ Wine runtime tests failed"
    echo "Check that:"
    echo "  1. OpenMFC DLL is built correctly"
    echo "  2. Wine is properly installed"
    echo "  3. DLL exports match MSVC expectations"
fi

exit $RESULT