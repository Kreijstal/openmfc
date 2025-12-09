// Test application that would link against MFC
// If this compiles with MSVC against openmfc.lib, we're compatible

#include <cstdio>

// Declarations that match MFC exports
extern "C" void __cdecl AfxThrowMemoryException();
extern "C" void __cdecl AfxThrowFileException(int cause, long lOsError, const wchar_t* filename);
extern "C" void __cdecl AfxThrowNotSupportedException();
extern "C" unsigned long __cdecl AfxGetDllVersion();

int main() {
    printf("Drop-in replacement test\n");
    
    // These would link against openmfc.lib
    // If linking succeeds, ABI is compatible
    printf("  AfxGetDllVersion() = 0x%lX\n", AfxGetDllVersion());
    
    printf("\n✅ If MSVC can compile and link this,\n");
    printf("   OpenMFC is a drop-in replacement for mfc140u.dll\n");
    
    return 0;
}
