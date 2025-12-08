// Test AfxGetDllVersion implementation
// Build with MSVC: cl /nologo /EHsc /MD test_version.cpp openmfc.lib

#include <cstdio>
#include <cstdint>

// C++ linkage - MSVC mangles to ?AfxGetDllVersion@@YAKXZ
__declspec(dllimport) uint32_t __cdecl AfxGetDllVersion();

int main() {
    printf("Testing AfxGetDllVersion()...\n");

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
