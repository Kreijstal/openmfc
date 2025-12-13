// App Core Implementation (CWinApp, CWinThread)
//
// Implements core application and thread classes.
// Currently stubs to satisfy linker dependencies.

#include "openmfc/afxwin.h"
#include <windows.h>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// CWinThread Implementation
// =============================================================================

// Core class implementations
IMPLEMENT_DYNAMIC(CCmdTarget, CObject)
IMPLEMENT_DYNAMIC(CWnd, CCmdTarget)

// Exception implementations
IMPLEMENT_DYNAMIC(CMemoryException, CException)
IMPLEMENT_DYNAMIC(CFileException, CException)
IMPLEMENT_DYNAMIC(CArchiveException, CException)

CArchiveException::CArchiveException(int cause, const wchar_t* lpszArchiveName)
    : m_cause(cause)
{
    if (lpszArchiveName)
        m_strFileName = lpszArchiveName;
}

int CArchiveException::GetErrorMessage(wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext) const {
    // Stub implementation
    if (lpszError && nMaxError > 0) {
        lpszError[0] = 0;
    }
    return 0;
}

IMPLEMENT_DYNAMIC(CWinThread, CCmdTarget)

CWinThread::CWinThread() {
    m_pMainWnd = nullptr;
    m_nThreadID = 0;
    m_hThread = nullptr;
    m_bAutoDelete = TRUE;
}

CWinThread::~CWinThread() {
}

BOOL CWinThread::InitInstance() {
    return TRUE;
}

int CWinThread::ExitInstance() {
    return m_msgCur.wParam; // Default exit code
}

int CWinThread::Run() {
    return ExitInstance();
}

BOOL CWinThread::PreTranslateMessage(MSG* pMsg) {
    (void)pMsg;
    return FALSE;
}

BOOL CWinThread::OnIdle(LONG lCount) {
    (void)lCount;
    return lCount == 0; // More work to do?
}

BOOL CWinThread::IsIdleMessage(MSG* pMsg) {
    (void)pMsg;
    return TRUE;
}

BOOL CWinThread::PumpMessage() {
    return FALSE;
}

BOOL CWinThread::PrePumpMessage() {
    return TRUE;
}

BOOL CWinThread::PostPumpMessage() {
    return TRUE;
}

// =============================================================================
// CWinApp Implementation
// =============================================================================

IMPLEMENT_DYNAMIC(CWinApp, CWinThread)

// CWinApp methods are defined inline in afxwin.h
// We only need the runtime class implementation here


// Global application pointer (stub)
static CWinApp* g_pApp = nullptr;

CWinApp* AFXAPI AfxGetApp() {
    return g_pApp;
}

CWinThread* AFXAPI AfxGetThread() {
    return (CWinThread*)g_pApp;
}

HINSTANCE AFXAPI AfxGetInstanceHandle() {
    return g_pApp ? g_pApp->m_hInstance : nullptr;
}

HINSTANCE AFXAPI AfxGetResourceHandle() {
    return g_pApp ? g_pApp->m_hInstance : nullptr;
}

void AFXAPI AfxSetResourceHandle(HINSTANCE hInstResource) {
    // TODO
    (void)hInstResource;
}

CWnd* AFXAPI AfxGetMainWnd() {
    return g_pApp ? g_pApp->m_pMainWnd : nullptr;
}

int AFXAPI AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    (void)hInstance; (void)hPrevInstance; (void)lpCmdLine; (void)nCmdShow;
    return TRUE;
}

