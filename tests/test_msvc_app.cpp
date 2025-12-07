// MSVC test app for OpenMFC
// Tests linking against OpenMFC DLL using __declspec(dllimport)
#include <windows.h>
#include <stdio.h>

// Simulate MFC header declarations
#define AFX_IMPORT __declspec(dllimport)
#define AFXAPI __cdecl
#define AFX_IMPORT_FUNC AFX_IMPORT AFXAPI

// Declare MFC functions we want to test
// These should match what MFC headers declare
AFX_IMPORT_FUNC void AfxThrowMemoryException();

int main() {
    printf("MSVC OpenMFC Linking Test\n");
    
    // Test 1: Verify DLL can be loaded and has expected exports
    HMODULE hModule = LoadLibraryA("openmfc.dll");
    if (!hModule) {
        printf("ERROR: Could not load openmfc.dll\n");
        return 1;
    }
    
    // Check key exports exist
    FARPROC func1 = GetProcAddress(hModule, "?AfxThrowMemoryException@@YAXXZ");
    if (!func1) {
        printf("ERROR: ?AfxThrowMemoryException@@YAXXZ not found in DLL\n");
        FreeLibrary(hModule);
        return 1;
    }
    printf("✓ Found ?AfxThrowMemoryException@@YAXXZ\n");
    
    // Check by ordinal (2350 for AfxThrowMemoryException)
    FARPROC func2 = GetProcAddress(hModule, MAKEINTRESOURCEA(2350));
    if (!func2) {
        printf("ERROR: Ordinal 2350 not found in DLL\n");
        FreeLibrary(hModule);
        return 1;
    }
    printf("✓ Found ordinal 2350\n");
    
    if (func1 != func2) {
        printf("ERROR: Name and ordinal mismatch!\n");
        FreeLibrary(hModule);
        return 1;
    }
    
    FreeLibrary(hModule);
    
    // Test 2: Test linking via __declspec(dllimport)
    // This is the actual test - will fail at link time if import library is wrong
    printf("\nTesting __declspec(dllimport) linking...\n");
    
    // This call will be resolved at link time via import library
    // If linking succeeds, the program will run and call our stub function
    // The stub prints "Not Implemented" to stderr
    // We can't easily capture stderr in a simple test, so we just verify
    // that the program runs without crashing
    printf("Calling AfxThrowMemoryException() via __declspec(dllimport)...\n");
    AfxThrowMemoryException();
    
    // If we reach here, linking succeeded and stub was called
    // (stub prints to stderr but doesn't crash)
    printf("✓ __declspec(dllimport) linking succeeded!\n");
    printf("✓ Stub function called (check stderr for 'Not Implemented' message)\n");
    
    printf("\n✅ All tests passed! MSVC can link against OpenMFC DLL.\n");
    return 0;
}