// Existing application construction remains partial. Keep its pseudo-handle
// teardown separate from CWinThread's owned kernel handle.
#define OPENMFC_APPCORE_IMPL
#include "detail/CWinAppSupport.h"
#include "openmfc/afxstat_.h"

// Symbol: ??0CWinApp@@QEAA@PEB_W@Z
extern "C" CWinApp* MS_ABI impl___0CWinApp__QEAA_PEB_W_Z(CWinApp* pThis, const wchar_t* lpszAppName) {
    return ::new(pThis) CWinApp(lpszAppName);
}

// Symbol: ??1CWinApp@@UEAA@XZ
extern "C" void MS_ABI impl___1CWinApp__UEAA_XZ(CWinApp* pThis) {
    pThis->CWinApp::~CWinApp();
}

CWinApp::~CWinApp() {
    // The application stores GetCurrentThread's pseudo handle. Native app
    // teardown clears it before the owning CWinThread destructor runs.
    // Other native application resource cleanup remains to be restored.
    m_hThread = nullptr;
    if (g_pApp == this) g_pApp = nullptr;
}

CWinApp::CWinApp(const wchar_t* lpszAppName) {
    m_pMainWnd = nullptr;
    m_pActiveWnd = nullptr;
    m_nThreadID = ::GetCurrentThreadId();
    m_hThread = ::GetCurrentThread();
    m_bAutoDelete = FALSE;
    m_hInstance = nullptr;
    m_lpCmdLine = nullptr;
    m_nCmdShow = SW_SHOW;
    m_pszAppName = lpszAppName;
    m_pszExeName = nullptr;
    m_pszHelpFilePath = nullptr;
    m_pszProfileName = nullptr;
    m_pszRegistryKey = nullptr;
    memset(&AfxGetThreadState()->m_msgCur, 0, sizeof(AfxGetThreadState()->m_msgCur));
    g_pApp = this;
}
