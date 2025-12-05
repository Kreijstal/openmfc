// Test for CWnd and related classes
// Build: g++ -std=c++17 -I../include -o test_cwnd test_cwnd.cpp

#include <openmfc/afxwin.h>
#include <cstdio>
#include <cstring>
#include <cstddef>

#define TEST(name) void test_##name()
#define ASSERT(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL: %s at %d\n", #cond, __LINE__); failed++; } else { passed++; } } while(0)

static int passed = 0;
static int failed = 0;

// Test derived window class
class CTestWnd : public CWnd {
    DECLARE_DYNAMIC(CTestWnd)
public:
    CTestWnd() : m_customData(42) {}
    int GetCustomData() const { return m_customData; }
private:
    int m_customData;
};

IMPLEMENT_DYNAMIC(CTestWnd, CWnd)

// Test CWinApp
class CTestApp : public CWinApp {
    DECLARE_DYNAMIC(CTestApp)
public:
    CTestApp() : CWinApp(L"TestApp") {}
    virtual BOOL InitInstance() override { return TRUE; }
};

IMPLEMENT_DYNAMIC(CTestApp, CWinApp)

TEST(sizeof_check) {
    // CWnd should be 232 bytes
    ASSERT(sizeof(CWnd) == 232);
    
    // CCmdTarget should be smaller
    ASSERT(sizeof(CCmdTarget) < sizeof(CWnd));
}

TEST(cwnd_construction) {
    CWnd wnd;
    
    // m_hWnd should be nullptr initially
    ASSERT(wnd.m_hWnd == nullptr);
    ASSERT(wnd.GetSafeHwnd() == nullptr);
}

TEST(cwnd_runtime_class) {
    CWnd wnd;
    
    CRuntimeClass* pClass = wnd.GetRuntimeClass();
    ASSERT(pClass != nullptr);
    ASSERT(pClass == RUNTIME_CLASS(CWnd));
    ASSERT(std::strcmp(pClass->m_lpszClassName, "CWnd") == 0);
    
    // CWnd derives from CCmdTarget
    ASSERT(wnd.IsKindOf(RUNTIME_CLASS(CCmdTarget)));
    ASSERT(wnd.IsKindOf(RUNTIME_CLASS(CObject)));
}

TEST(cwnd_derived) {
    CTestWnd testWnd;
    
    // Should be correct type
    ASSERT(testWnd.IsKindOf(RUNTIME_CLASS(CTestWnd)));
    ASSERT(testWnd.IsKindOf(RUNTIME_CLASS(CWnd)));
    ASSERT(testWnd.IsKindOf(RUNTIME_CLASS(CCmdTarget)));
    ASSERT(testWnd.IsKindOf(RUNTIME_CLASS(CObject)));
    
    // Custom data should work
    ASSERT(testWnd.GetCustomData() == 42);
}

TEST(cwnd_methods) {
    CWnd wnd;
    
    // These are stubs but should not crash
    wnd.ShowWindow(1);
    wnd.UpdateWindow();
    
    RECT rect = {0, 0, 100, 100};
    wnd.GetWindowRect(&rect);
    wnd.GetClientRect(&rect);
    
    wnd.SetWindowTextW(L"Test");
    wchar_t buf[100];
    wnd.GetWindowTextW(buf, 100);
    
    wnd.Invalidate(TRUE);
    wnd.RedrawWindow();
}

TEST(cwnd_destroy) {
    CWnd* pWnd = new CWnd();
    pWnd->m_hWnd = reinterpret_cast<HWND>(static_cast<uintptr_t>(0x12345678));
    
    ASSERT(pWnd->DestroyWindow() == TRUE);
    ASSERT(pWnd->m_hWnd == nullptr);
    
    delete pWnd;
}

TEST(cexception) {
    CException* pEx = new CMemoryException();
    
    ASSERT(pEx->IsKindOf(RUNTIME_CLASS(CMemoryException)));
    ASSERT(pEx->IsKindOf(RUNTIME_CLASS(CException)));
    ASSERT(pEx->IsKindOf(RUNTIME_CLASS(CObject)));
    
    wchar_t buf[100];
    ASSERT(pEx->GetErrorMessage(buf, 100) == FALSE); // Not implemented
    
    delete pEx;
}

TEST(cfileexception) {
    CFileException* pEx = new CFileException(CFileException::fileNotFound, 2);
    
    ASSERT(pEx->m_cause == CFileException::fileNotFound);
    ASSERT(pEx->m_lOsError == 2);
    
    delete pEx;
}

TEST(cwinthread) {
    CWinThread thread;
    
    ASSERT(thread.IsKindOf(RUNTIME_CLASS(CWinThread)));
    ASSERT(thread.IsKindOf(RUNTIME_CLASS(CCmdTarget)));
    ASSERT(thread.m_pMainWnd == nullptr);
}

TEST(cwinapp) {
    CTestApp app;
    
    ASSERT(app.IsKindOf(RUNTIME_CLASS(CWinApp)));
    ASSERT(app.IsKindOf(RUNTIME_CLASS(CWinThread)));
    ASSERT(std::wcscmp(app.m_pszAppName, L"TestApp") == 0);
    ASSERT(app.InitInstance() == TRUE);
}

int main() {
    std::printf("Running CWnd tests...\n");
    
    test_sizeof_check();
    test_cwnd_construction();
    test_cwnd_runtime_class();
    test_cwnd_derived();
    test_cwnd_methods();
    test_cwnd_destroy();
    test_cexception();
    test_cfileexception();
    test_cwinthread();
    test_cwinapp();
    
    std::printf("Results: %d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
