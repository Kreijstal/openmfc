// MSVC ABI Test - Tests actual MSVC linking with implemented functions
// Uses __declspec(dllimport) to test MSVC mangling compatibility

#include <cstdint>

// Test implemented functions
__declspec(dllimport) uint32_t __cdecl AfxGetDllVersion();
__declspec(dllimport) void __cdecl AfxThrowMemoryException();
__declspec(dllimport) void __cdecl AfxThrowFileException(int cause, LONG lOsError, const wchar_t* lpszFileName);
__declspec(dllimport) void __cdecl AfxThrowResourceException();
__declspec(dllimport) void __cdecl AfxThrowNotSupportedException();
__declspec(dllimport) void __cdecl AfxThrowUserException();
__declspec(dllimport) void __cdecl AfxThrowInvalidArgException();
__declspec(dllimport) void __cdecl AfxAbort();

// Test function calls
int test_afxgetdllversion() {
    uint32_t version = AfxGetDllVersion();
    if (version == 0) return 1;
    
    uint16_t major = (version >> 16) & 0xFFFF;
    if (major < 0x000C) return 2;
    
    return 0;
}

int test_exception_calls() {
    // Call all exception functions
    AfxThrowMemoryException();
    AfxThrowFileException(1, 2, L"test.txt");
    AfxThrowResourceException();
    AfxThrowNotSupportedException();
    AfxThrowUserException();
    AfxThrowInvalidArgException();
    
    return 0;
}

int main() {
    int result = 0;
    
    result = test_afxgetdllversion();
    if (result != 0) return result;
    
    result = test_exception_calls();
    if (result != 0) return result;
    
    return 0;
}