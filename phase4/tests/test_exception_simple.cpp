// Simple exception test - catch with ... to verify basic throwing works
//
// Build with MSVC:
//   cl /nologo /EHsc /MD test_exception_simple.cpp openmfc.lib
//
// This test verifies that:
// 1. The DLL loads correctly
// 2. AfxThrowMemoryException can be called
// 3. An exception is actually thrown and can be caught

#include <cstdio>

// Import the throw function with C++ linkage
// MSVC will mangle this as ?AfxThrowMemoryException@@YAXXZ
// which is what our .def file exports
__declspec(dllimport) void AfxThrowMemoryException();

int main() {
    printf("=== Simple Exception Test ===\n");
    printf("This test verifies basic exception throwing works.\n\n");
    fflush(stdout);

    printf("Test 1: Call AfxThrowMemoryException and catch with ...\n");
    fflush(stdout);

    try {
        printf("  Calling AfxThrowMemoryException()...\n");
        fflush(stdout);

        AfxThrowMemoryException();

        printf("  ERROR: Should have thrown!\n");
        fflush(stdout);
        return 1;
    }
    catch (...) {
        printf("  SUCCESS: Caught exception with catch(...)\n");
        fflush(stdout);
    }

    printf("\n=== Test Complete ===\n");
    fflush(stdout);
    return 0;
}
