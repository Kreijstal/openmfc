// MSVC test app for OpenMFC
#include <windows.h>
#include <stdio.h>

// Forward declaration matching MFC header
extern "C" void __cdecl AfxThrowMemoryException();

int main() {
    printf("MSVC OpenMFC Test App\n");
    
    // Try to call the stub function
    try {
        printf("Calling AfxThrowMemoryException...\n");
        AfxThrowMemoryException();
        printf("ERROR: Should have thrown!\n");
        return 1;
    } catch (...) {
        printf("SUCCESS: Exception caught as expected\n");
        return 0;
    }
}