// Test AfxGetDllVersion implementation
// Build with MSVC: cl /nologo /EHsc /MD test_version.cpp openmfc.lib

#include <cstdio>
#include <cstdint>
#include <windows.h>

typedef uint32_t (__cdecl *AfxGetDllVersionFunc)();

int main() {
    printf("Testing AfxGetDllVersion()...\n");

    HMODULE hDll = LoadLibraryA("openmfc.dll");
    if (!hDll) {
        printf("FAIL: Could not load openmfc.dll\n");
        return 1;
    }

    // ?AfxGetDllVersion@@YAKXZ
    AfxGetDllVersionFunc pfnAfxGetDllVersion = (AfxGetDllVersionFunc)GetProcAddress(hDll, "?AfxGetDllVersion@@YAKXZ");
    
    if (!pfnAfxGetDllVersion) {
        printf("FAIL: Could not find ?AfxGetDllVersion@@YAKXZ\n");
        FreeLibrary(hDll);
        return 1;
    }

    uint32_t version = pfnAfxGetDllVersion();

    printf("  AfxGetDllVersion() = 0x%04X\n", version);
    printf("  Major: %d, Minor: %d\n", (version >> 8) & 0xFF, version & 0xFF);

    FreeLibrary(hDll);

    if (version == 0x0E00) {
        printf("\nPASS: Version matches MFC 14.0 (0x0E00)\n");
        return 0;
    } else {
        printf("\nFAIL: Expected 0x0E00, got 0x%04X\n", version);
        return 1;
    }
}
