// Define OPENMFC_APPCORE_IMPL to use extern declarations instead of inline stubs
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <vector>

// Global state
static CWinApp* g_pApp = nullptr;
static CWinThread* g_pThread = nullptr;
static HINSTANCE g_hInstance = nullptr;
static HINSTANCE g_hResource = nullptr;

// Runtime class implementations
IMPLEMENT_DYNAMIC(CException, CObject)
IMPLEMENT_DYNAMIC(CMemoryException, CException)
IMPLEMENT_DYNAMIC(CFileException, CException)
IMPLEMENT_DYNAMIC(CCmdTarget, CObject)
IMPLEMENT_DYNAMIC(CWnd, CCmdTarget)
IMPLEMENT_DYNAMIC(CWinThread, CCmdTarget)
IMPLEMENT_DYNAMIC(CWinApp, CWinThread)

// Thread local storage for thread state
// For now, we use a simple global for single-threaded apps, 
// but in a real implementation this should be TLS.
// TODO: Implement proper TLS for multi-threading support.

// Note: AfxGetApp is defined inline in afxwin.h

CWinThread* AFXAPI AfxGetThread() {
    return g_pThread ? g_pThread : g_pApp;
}

HINSTANCE AFXAPI AfxGetInstanceHandle() {
    return g_hInstance;
}

HINSTANCE AFXAPI AfxGetResourceHandle() {
    return g_hResource ? g_hResource : g_hInstance;
}

void AFXAPI AfxSetResourceHandle(HINSTANCE hInstResource) {
    g_hResource = hInstResource;
}

// AfxWinInit implementation
BOOL AFXAPI AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                       LPWSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance; (void)lpCmdLine; (void)nCmdShow;
    
    g_hInstance = hInstance;
    g_hResource = hInstance;
    
    // Set global app pointer if it exists
    // CWinApp constructor sets itself as the global app usually, 
    // but we ensure it here if needed.
    if (g_pApp == nullptr && AfxGetApp() != nullptr) {
        g_pApp = AfxGetApp();
    }
    
    return TRUE;
}

// CWinApp constructor implementation
// This needs to be here to set the global pointer
CWinApp::CWinApp(LPCWSTR lpszAppName)
    : CWinThread()
    , m_pszAppName(lpszAppName)
    , m_pszExeName(nullptr)
    , m_pszHelpFilePath(nullptr)
    , m_pszProfileName(nullptr)
    , m_hInstance(nullptr)
{
    memset(_winapp_padding, 0, sizeof(_winapp_padding));
    
    // Set global app pointer
    if (g_pApp == nullptr) {
        g_pApp = this;
        g_pThread = this;
    }
}

// Global helper to get main window
CWnd* AFXAPI AfxGetMainWnd() {
    CWinThread* pThread = AfxGetThread();
    return pThread ? pThread->m_pMainWnd : nullptr;
}
