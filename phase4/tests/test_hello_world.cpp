// OpenMFC Hello World Test
//
// This test demonstrates a minimal MFC-style Hello World application.
// It creates a frame window and shows it briefly, then exits.
//
// Build with MSVC:
//   cl /nologo /EHsc /MD /D_AFXDLL test_hello_world.cpp openmfc.lib /Fe:test_hello_world.exe /link /NODEFAULTLIB:mfc140u.lib
//
// Expected behavior:
//   1. CWinApp constructor registers global app instance
//   2. AfxWinMain is called by the CRT
//   3. InitInstance creates a CFrameWnd
//   4. ShowWindow displays the window
//   5. Run() processes messages
//   6. Window is closed after a short timeout
//   7. ExitInstance returns 0

#include <cstdio>
#include "openmfc/afxwin.h"

// Test application class
class CTestApp : public CWinApp {
public:
    CTestApp() : CWinApp(L"TestApp") {
        printf("CTestApp constructor called\n");
    }

    virtual BOOL InitInstance() override {
        printf("InitInstance called\n");

        // Create frame window
        CFrameWnd* pFrame = new CFrameWnd;
        if (!pFrame) {
            printf("Failed to allocate CFrameWnd\n");
            return FALSE;
        }

        // Create the window
        RECT rect = {100, 100, 500, 400};
        // Avoid virtual dispatch here: our Phase 4 implementation exports MSVC-mangled
        // methods via a .def alias table, but we don't (yet) provide a reliable MSVC
        // vtable for CFrameWnd.
        if (!pFrame->CFrameWnd::Create(nullptr, L"OpenMFC Hello World", WS_OVERLAPPEDWINDOW, rect)) {
            printf("Failed to create window\n");
            delete pFrame;
            return FALSE;
        }

        printf("Window created successfully\n");

        // Show the window
        pFrame->ShowWindow(SW_SHOW);
        printf("ShowWindow called\n");

        // Set as main window
        m_pMainWnd = pFrame;

        // Post quit message after a short delay (for automated testing)
        // In a real app, this would not be done
        SetTimer(pFrame->GetSafeHwnd(), 1, 1000, nullptr);  // 1 second timeout

        return TRUE;
    }

    virtual int ExitInstance() override {
        printf("ExitInstance called\n");
        return CWinApp::ExitInstance();
    }
};

// Global application instance
CTestApp theApp;

// Main entry point - for non-Windows testing
#ifndef _WIN32
int main() {
    printf("Hello World test (non-Windows stub)\n");
    printf("This test requires Windows to run properly.\n");

    // Verify app was constructed
    CWinApp* pApp = AfxGetApp();
    if (pApp) {
        printf("PASS: AfxGetApp() returns non-null\n");
        printf("PASS: App name: %ls\n", pApp->m_pszAppName ? pApp->m_pszAppName : L"(null)");
        return 0;
    } else {
        printf("FAIL: AfxGetApp() returns null\n");
        return 1;
    }
}
#else
// On Windows, WinMain is provided by the CRT and calls AfxWinMain
// For testing, we can also provide our own main:

// Override WM_TIMER to exit
LRESULT CALLBACK TestWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_TIMER) {
        printf("Timer fired, posting quit message\n");
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

int main() {
    printf("=== OpenMFC Hello World Test ===\n\n");

    // Get the app instance
    CWinApp* pApp = AfxGetApp();
    if (!pApp) {
        printf("FAIL: No application instance\n");
        return 1;
    }
    printf("PASS: Application instance created\n");

    // Call InitInstance
    if (!pApp->InitInstance()) {
        printf("FAIL: InitInstance returned FALSE\n");
        return 1;
    }
    printf("PASS: InitInstance succeeded\n");

    // Check main window
    if (!pApp->m_pMainWnd || !pApp->m_pMainWnd->GetSafeHwnd()) {
        printf("FAIL: Main window not created\n");
        return 1;
    }
    printf("PASS: Main window created (HWND=%p)\n", (void*)pApp->m_pMainWnd->GetSafeHwnd());

    // Run message loop briefly
    printf("Running message loop (will exit after 1 second)...\n");
    int result = pApp->Run();

    printf("\n=== Test Complete ===\n");
    printf("Exit code: %d\n", result);

    return result;
}
#endif
