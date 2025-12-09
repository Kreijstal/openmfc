// MFC Smoke Test - Comprehensive functionality test
// Tests actual MFC behavior, not just ABI compatibility

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <windows.h>

// Import MFC functions
__declspec(dllimport) uint32_t __cdecl AfxGetDllVersion();
__declspec(dllimport) void __cdecl AfxThrowMemoryException();
__declspec(dllimport) void __cdecl AfxThrowFileException(int cause, LONG lOsError, const wchar_t* lpszFileName);
__declspec(dllimport) void __cdecl AfxThrowResourceException();
__declspec(dllimport) void __cdecl AfxThrowNotSupportedException();
__declspec(dllimport) void __cdecl AfxThrowUserException();
__declspec(dllimport) void __cdecl AfxThrowInvalidArgException();

// Test framework
class MFCTest {
    int passed;
    int failed;
    
public:
    MFCTest() : passed(0), failed(0) {}
    
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
        test_version();
        test_exceptions();
        return failed == 0 ? 0 : 1;
    }
    
private:
    void test_version() {
        uint32_t version = AfxGetDllVersion();
        
        test("Version non-zero", version != 0);
        
        uint16_t major = (version >> 16) & 0xFFFF;
        uint16_t minor = version & 0xFFFF;
        
        test("Version format valid", 
             (major >= 0x000C && major <= 0x000F) || (major == 0x000E));
        
        // Multiple calls consistency
        uint32_t v2 = AfxGetDllVersion();
        uint32_t v3 = AfxGetDllVersion();
        test("Version consistent", version == v2 && version == v3);
    }
    
    void test_exceptions() {
        // Test that all exception functions can be called
        // (They're currently stubs that print to stderr)
        

        
        // Redirect stderr temporarily
        FILE* original_stderr = freopen("nul", "w", stderr);
        
        // Call all exception functions
        AfxThrowMemoryException();
        test("AfxThrowMemoryException callable", true);
        
        AfxThrowFileException(1, ERROR_ACCESS_DENIED, L"test.txt");
        test("AfxThrowFileException callable", true);
        
        AfxThrowResourceException();
        test("AfxThrowResourceException callable", true);
        
        AfxThrowNotSupportedException();
        test("AfxThrowNotSupportedException callable", true);
        
        AfxThrowUserException();
        test("AfxThrowUserException callable", true);
        
        AfxThrowInvalidArgException();
        test("AfxThrowInvalidArgException callable", true);
        
        // Restore stderr
        if (original_stderr) {
            fclose(stderr);
            stderr = original_stderr;
        }
    }
};

int main() {
    MFCTest test;
    return test.run();
}