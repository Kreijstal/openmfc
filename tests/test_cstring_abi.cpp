// Test CString ABI compatibility
// Verifies CString functions can be called through MSVC ABI

#include <openmfc/afxstr.h>
#include <cstdint>

// Import CString functions we want to test
extern "C" {
    // CString constructor/destructor
    __declspec(dllimport) void* __cdecl ??0CString@@QEAA@XZ();  // CString::CString()
    __declspec(dllimport) void __cdecl ??1CString@@QEAA@XZ(void*);  // CString::~CString()
    
    // CString methods
    __declspec(dllimport) int __cdecl ?GetLength@CString@@QEBAHXZ(void*);  // CString::GetLength()
    __declspec(dllimport) wchar_t* __cdecl ?GetString@CString@@QEBAPEB_WXZ(void*);  // CString::GetString()
    __declspec(dllimport) BOOL __cdecl ?IsEmpty@CString@@QEBA_NXZ(void*);  // CString::IsEmpty()
}

int test_cstring_constructor() {
    void* str = ??0CString@@QEAA@XZ();
    if (!str) return 1;
    
    int length = ?GetLength@CString@@QEBAHXZ(str);
    if (length != 0) return 2;
    
    BOOL empty = ?IsEmpty@CString@@QEBA_NXZ(str);
    if (!empty) return 3;
    
    ??1CString@@QEAA@XZ(str);
    return 0;
}

int test_cstring_from_literal() {
    // Note: We can't test constructor with literal without knowing exact mangling
    // This test would need the actual constructor symbol
    return 0; // Skip for now
}

int main() {
    int result = 0;
    
    result = test_cstring_constructor();
    if (result != 0) return result;
    
    result = test_cstring_from_literal();
    if (result != 0) return result;
    
    return 0;
}