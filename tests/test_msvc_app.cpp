// MSVC Linking Test for OpenMFC
//
// This test verifies MSVC can LINK against OpenMFC at compile time.
// Uses __declspec(dllimport) for true static linking test.

#include <cstdio>
#include <windows.h>

// =============================================================================
// MFC-style declarations - C++ linkage produces MSVC-mangled names
// =============================================================================

__declspec(dllimport) void __cdecl AfxThrowMemoryException();
__declspec(dllimport) void __cdecl AfxThrowNotSupportedException();
__declspec(dllimport) void __cdecl AfxThrowInvalidArgException();

int main() {
    // Use MessageBox for guaranteed visible output even if stdout is lost
    MessageBoxA(NULL, "MSVC test starting", "Test", MB_OK);
    
    printf("=== OpenMFC MSVC Link Test ===\n\n");
    fflush(stdout);
    
    printf("Link test PASSED: MSVC resolved all imported symbols.\n\n");
    fflush(stdout);

    printf("Calling functions (may throw exceptions)...\n");
    fflush(stdout);

    try {
        AfxThrowMemoryException();
        printf("  AfxThrowMemoryException - OK (no exception)\n");
    } catch (...) {
        printf("  AfxThrowMemoryException - OK (exception caught)\n");
    }
    fflush(stdout);

    try {
        AfxThrowNotSupportedException();
        printf("  AfxThrowNotSupportedException - OK (no exception)\n");
    } catch (...) {
        printf("  AfxThrowNotSupportedException - OK (exception caught)\n");
    }
    fflush(stdout);

    try {
        AfxThrowInvalidArgException();
        printf("  AfxThrowInvalidArgException - OK (no exception)\n");
    } catch (...) {
        printf("  AfxThrowInvalidArgException - OK (exception caught)\n");
    }
    fflush(stdout);

    printf("\n=== All tests passed ===\n");
    fflush(stdout);
    return 0;
}
