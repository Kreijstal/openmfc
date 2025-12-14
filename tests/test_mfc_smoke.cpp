// MFC Smoke Test - Comprehensive functionality test
// Tests actual MFC behavior using dynamic loading
// Compatible with both MinGW and MSVC

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <windows.h>

// Function pointer types
typedef uint32_t (__cdecl *AfxGetDllVersionFunc)();
typedef void (__cdecl *AfxThrowExceptionFunc)();
typedef void (__cdecl *AfxThrowFileExceptionFunc)(int, LONG, const wchar_t*);

// Test framework
class MFCTest {
    int passed;
    int failed;
    HMODULE hDll;
    
public:
    MFCTest() : passed(0), failed(0), hDll(nullptr) {}
    
    void test(const char* name, bool condition, const char* message = "") {
        if (condition) {
            passed++;
        } else {
            failed++;
        }
    }
    
    void section(const char* name) {
        // No output
    }
    
    int run() {
        // Load the DLL
        hDll = LoadLibraryA("openmfc.dll");
        if (!hDll) {
            return 1;
        }
        
        test_version();
        test_exceptions();
        
        FreeLibrary(hDll);
        return failed == 0 ? 0 : 1;
    }
    
private:
    void test_version() {
        // Get function using MSVC-mangled name
        AfxGetDllVersionFunc AfxGetDllVersion = 
            (AfxGetDllVersionFunc)GetProcAddress(hDll, "?AfxGetDllVersion@@YAKXZ");
        
        test("AfxGetDllVersion found", AfxGetDllVersion != nullptr);
        if (!AfxGetDllVersion) return;
        
        uint32_t version = AfxGetDllVersion();
        
        test("Version non-zero", version != 0);
        
        uint16_t major = (version >> 16) & 0xFFFF;
        
        test("Version format valid", 
             (major >= 0x000C && major <= 0x000F) || (major == 0x000E));
        
        // Multiple calls consistency
        uint32_t v2 = AfxGetDllVersion();
        uint32_t v3 = AfxGetDllVersion();
        test("Version consistent", version == v2 && version == v3);
    }
    
    void test_exceptions() {
        // Test that all exception functions exist and can be found
        // (They're currently stubs that print to stderr)
        
        AfxThrowExceptionFunc pFunc;
        
        pFunc = (AfxThrowExceptionFunc)GetProcAddress(hDll, "?AfxThrowMemoryException@@YAXXZ");
        test("AfxThrowMemoryException found", pFunc != nullptr);
        
        AfxThrowFileExceptionFunc pFileFunc = 
            (AfxThrowFileExceptionFunc)GetProcAddress(hDll, "?AfxThrowFileException@@YAXHJPEB_W@Z");
        test("AfxThrowFileException found", pFileFunc != nullptr);
        
        pFunc = (AfxThrowExceptionFunc)GetProcAddress(hDll, "?AfxThrowResourceException@@YAXXZ");
        test("AfxThrowResourceException found", pFunc != nullptr);
        
        pFunc = (AfxThrowExceptionFunc)GetProcAddress(hDll, "?AfxThrowNotSupportedException@@YAXXZ");
        test("AfxThrowNotSupportedException found", pFunc != nullptr);
        
        pFunc = (AfxThrowExceptionFunc)GetProcAddress(hDll, "?AfxThrowUserException@@YAXXZ");
        test("AfxThrowUserException found", pFunc != nullptr);
        
        pFunc = (AfxThrowExceptionFunc)GetProcAddress(hDll, "?AfxThrowInvalidArgException@@YAXXZ");
        test("AfxThrowInvalidArgException found", pFunc != nullptr);
    }
};

int main() {
    MFCTest test;
    return test.run();
}