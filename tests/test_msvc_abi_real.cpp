// MSVC ABI Test - Must be compiled with MSVC cl.exe
// Tests that MSVC generates correct mangled names that match our .def file

#include <windows.h>
#include <cstdint>

// MSVC will mangle these C++ declarations automatically
// The mangled names MUST match what's in openmfc.def

// Version function
uint32_t AfxGetDllVersion();

// Exception functions  
void AfxThrowMemoryException();
void AfxThrowFileException(int cause, LONG lOsError, const wchar_t* lpszFileName);
void AfxThrowResourceException();
void AfxThrowNotSupportedException();
void AfxThrowUserException();
void AfxThrowInvalidArgException();
void AfxAbort();

// If MSVC compiles this and links against openmfc.lib successfully,
// then the mangled names match exactly.

int main() {
    // Test version function
    uint32_t version = AfxGetDllVersion();
    
    // Test exception calls
    AfxThrowMemoryException();
    AfxThrowFileException(1, ERROR_FILE_NOT_FOUND, L"test.txt");
    AfxThrowResourceException();
    AfxThrowNotSupportedException();
    AfxThrowUserException();
    AfxThrowInvalidArgException();
    
    return 0;
}