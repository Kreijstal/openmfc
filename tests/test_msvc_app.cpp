// MSVC test app for OpenMFC
#include <windows.h>
#include <stdio.h>

// Forward declaration matching MFC header
// Use __declspec(dllimport) like real MFC headers
#define AFX_IMPORT __declspec(dllimport)
#define AFXAPI __cdecl
#define AFX_IMPORT_FUNC AFX_IMPORT AFXAPI

// MSVC mangles C++ function names
// The DLL exports ?AfxThrowMemoryException@@YAXXZ at ordinal 2350
// Due to MSVC import library creation issues with .def files,
// we need to work around the name mangling problem
// Option 1: Use GetProcAddress by ordinal (works but doesn't test linking)
// Option 2: Use linker pragma to map unmangled to mangled name
// Let's try option 2 with a pragma comment

// Linker pragma to map AfxThrowMemoryException to its mangled export
// Format: #pragma comment(linker, "/export:Symbol=ActualExport")
// But this only works for exports, not imports

// Actually, let's use a different approach: declare with mangled name
// using a .def file alias or linker trick
// For now, use GetProcAddress to test the DLL works

typedef void (AFXAPI *AfxThrowMemoryExceptionFunc)();

int main() {
    printf("MSVC OpenMFC Test App\n");
    
    // Load the DLL
    HMODULE hModule = LoadLibraryA("openmfc.dll");
    if (!hModule) {
        printf("ERROR: Could not load openmfc.dll\n");
        return 1;
    }
    
    // Get function by ordinal (2350 for AfxThrowMemoryException)
    AfxThrowMemoryExceptionFunc func = (AfxThrowMemoryExceptionFunc)GetProcAddress(
        hModule, MAKEINTRESOURCEA(2350));
    
    if (!func) {
        printf("ERROR: Could not find AfxThrowMemoryException at ordinal 2350\n");
        FreeLibrary(hModule);
        return 1;
    }
    
    // Try to call the stub function
    try {
        printf("Calling AfxThrowMemoryException via ordinal 2350...\n");
        func();
        printf("ERROR: Should have thrown!\n");
        FreeLibrary(hModule);
        return 1;
    } catch (...) {
        printf("SUCCESS: Exception caught as expected\n");
        FreeLibrary(hModule);
        return 0;
    }
}