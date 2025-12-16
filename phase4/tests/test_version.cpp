// Test AfxGetDllVersion implementation
// Build with MSVC: cl /nologo /EHsc /MD test_version.cpp openmfc.lib
//
// This test verifies MSVC can link against OpenMFC at compile time.
// NO runtime checks (LoadLibrary/GetProcAddress) - those are cheating.

#include <cstdio>
#include <cstdint>

// Declare the function with __declspec(dllimport) for compile-time linking
// When MSVC compiles this, it generates the mangled name: ?AfxGetDllVersion@@YAKXZ
__declspec(dllimport) unsigned long __cdecl AfxGetDllVersion();

int main() {
    printf("Testing AfxGetDllVersion() (compile-time linked)...\n");

    // If we reach main(), MSVC successfully linked against openmfc.lib.
    printf("Link test PASSED: MSVC resolved AfxGetDllVersion symbol.\n\n");

    uint32_t version = AfxGetDllVersion();

    printf("  AfxGetDllVersion() = 0x%04X\n", version);
    printf("  Major: %d, Minor: %d\n", (version >> 8) & 0xFF, version & 0xFF);

    if (version == 0x0E00) {
        printf("\nPASS: Version matches MFC 14.0 (0x0E00)\n");
        return 0;
    } else {
        printf("\nFAIL: Expected 0x0E00, got 0x%04X\n", version);
        return 1;
    }
}
