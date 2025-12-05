// Test for Core Application Functions
// Build: g++ -std=c++17 -I../include -o test_core_app test_core_app.cpp

#include <openmfc/afxwin.h>
#include <cstdio>
#include <cstring>

#define TEST(name) void test_##name()
#define ASSERT(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL: %s at %d\n", #cond, __LINE__); failed++; } else { passed++; } } while(0)

static int passed = 0;
static int failed = 0;

// Test App class
class CTestApp : public CWinApp {
public:
    CTestApp() : CWinApp(L"TestCoreApp") {}
};

TEST(afx_globals) {
    // Before app creation, globals should be null or default
    // Note: In this test, we might run after other tests, so state might be dirty if not careful.
    // But since we run as a standalone executable, it's fine.
    
    // Create app object
    CTestApp app;
    
    // AfxGetApp should return the app
    ASSERT(AfxGetApp() == &app);
    
    // AfxGetThread should return the app (as it is a thread)
    ASSERT(AfxGetThread() == &app);
    
    // App name should be set
    ASSERT(std::wcscmp(app.m_pszAppName, L"TestCoreApp") == 0);
}

TEST(afx_win_init) {
    HINSTANCE hInst = (HINSTANCE)(uintptr_t)0x10000000;
    
    // Initialize MFC
    ASSERT(AfxWinInit(hInst, nullptr, nullptr, 0) == TRUE);
    
    // Check instance handles
    ASSERT(AfxGetInstanceHandle() == hInst);
    ASSERT(AfxGetResourceHandle() == hInst);
    
    // Change resource handle
    HINSTANCE hRes = (HINSTANCE)(uintptr_t)0x20000000;
    AfxSetResourceHandle(hRes);
    ASSERT(AfxGetResourceHandle() == hRes);
    ASSERT(AfxGetInstanceHandle() == hInst); // Instance handle shouldn't change
}

TEST(afx_main_wnd) {
    CTestApp app;
    CWnd wnd;
    wnd.m_hWnd = (HWND)(uintptr_t)0x1234;
    
    // Set main window
    app.m_pMainWnd = &wnd;
    
    // AfxGetMainWnd should return it
    ASSERT(AfxGetMainWnd() == &wnd);
}

TEST(exceptions) {
    // Test AfxThrowMemoryException
    try {
        AfxThrowMemoryException();
        ASSERT(false); // Should not reach here
    } catch (CMemoryException* e) {
        ASSERT(e->IsKindOf(RUNTIME_CLASS(CMemoryException)));
        e->Delete();
    } catch (...) {
        ASSERT(false); // Wrong exception type
    }
    
    // Test AfxThrowFileException
    try {
        AfxThrowFileException(CFileException::fileNotFound, 5, L"test.txt");
        ASSERT(false);
    } catch (CFileException* e) {
        ASSERT(e->IsKindOf(RUNTIME_CLASS(CFileException)));
        ASSERT(e->m_cause == CFileException::fileNotFound);
        ASSERT(e->m_lOsError == 5);
        // ASSERT(e->m_strFileName == L"test.txt"); // CString comparison
        e->Delete();
    } catch (...) {
        ASSERT(false);
    }
}

int main() {
    std::printf("Running Core App tests...\n");
    
    test_afx_globals();
    test_afx_win_init();
    test_afx_main_wnd();
    test_exceptions();
    
    std::printf("Results: %d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
