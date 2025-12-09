// Simple test to call exception functions
// Compile with: x86_64-w64-mingw32-g++ -o test_simple_exception.exe test_simple_exception.cpp -Lbuild-phase4 -lopenmfc

#include <stdio.h>
#include <windows.h>

// Function declarations - these should be resolved by the import library
extern "C" {
    // These are the MSVC-mangled names that the .def file maps to stub functions
    void __stdcall _AfxThrowMemoryException();
    void __stdcall _AfxThrowFileException(int cause, LONG lOsError, const wchar_t* lpszFileName);
    void __stdcall _AfxAbort();
}

// Helper to load by ordinal (more reliable)
typedef void (__stdcall *AfxThrowMemoryExceptionFunc)();
typedef void (__stdcall *AfxAbortFunc)();

int main() {
    printf("Testing OpenMFC exception implementations...\n");
    
    // Load the DLL
    HMODULE hModule = LoadLibraryA("build-phase4/openmfc.dll");
    if (!hModule) {
        printf("Failed to load DLL: %lu\n", GetLastError());
        return 1;
    }
    
    // Get function by ordinal (2350 for AfxThrowMemoryException)
    AfxThrowMemoryExceptionFunc pAfxThrowMemoryException = 
        (AfxThrowMemoryExceptionFunc)GetProcAddress(hModule, MAKEINTRESOURCEA(2350));
    
    AfxAbortFunc pAfxAbort = 
        (AfxAbortFunc)GetProcAddress(hModule, MAKEINTRESOURCEA(2145));
    
    if (!pAfxThrowMemoryException || !pAfxAbort) {
        printf("Failed to get functions: %lu\n", GetLastError());
        FreeLibrary(hModule);
        return 1;
    }
    
    printf("Successfully loaded functions from DLL\n");
    printf("Calling AfxThrowMemoryException (ordinal 2350)...\n");
    
    // This will raise an SEH exception
    pAfxThrowMemoryException();
    
    printf("Should not reach here - AfxThrowMemoryException should have raised exception\n");
    
    FreeLibrary(hModule);
    return 0;
}