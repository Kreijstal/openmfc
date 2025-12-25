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

// =============================================================================
// CCmdTarget Implementation
// =============================================================================

CCmdTarget::~CCmdTarget() {
    // Base class destructor - nothing specific to clean up
}

// CCmdTarget message map (root)
const AFX_MSGMAP* AFXAPI CCmdTarget::GetThisMessageMap()
{
    return &CCmdTarget::messageMap;
}

const AFX_MSGMAP* CCmdTarget::GetMessageMap() const
{
    return GetThisMessageMap();
}

const AFX_MSGMAP CCmdTarget::messageMap =
{
    nullptr, // No base class with message map
    &CCmdTarget::_messageEntries[0]
};

const AFX_MSGMAP_ENTRY CCmdTarget::_messageEntries[] =
{
    {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
};

int CCmdTarget::OnCmdMsg(unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo)
{
    // Simple command routing
    const AFX_MSGMAP* pMap = GetMessageMap();
    
    while (pMap != nullptr)
    {
        const AFX_MSGMAP_ENTRY* lpEntry = pMap->lpEntries;
        while (lpEntry->nSig != AfxSig_end)
        {
            if (lpEntry->nID == nID && lpEntry->nCode == (UINT)nCode)
            {
                // Found a match
                return DispatchCmdMsg(this, nID, nCode, lpEntry->pfn, pExtra, lpEntry->nSig, pHandlerInfo);
            }
            lpEntry++;
        }
        
        // Get base class message map
        if (pMap->pfnGetBaseMap != nullptr)
            pMap = (*pMap->pfnGetBaseMap)();
        else
            pMap = nullptr;
    }
    
    return FALSE; // Not handled
}

int PASCAL CCmdTarget::DispatchCmdMsg(CCmdTarget* pTarget, unsigned int nID, int nCode,
                                      AFX_PMSG pfn, void* pExtra, unsigned int nSig, void* pHandlerInfo)
{
    (void)nID; (void)nCode; (void)pExtra; (void)pHandlerInfo;
    
    switch (nSig)
    {
    case AfxSig_vv:
        (pTarget->*pfn)();
        return TRUE;
    default:
        return FALSE;
    }
}

// =============================================================================
// CWinThread Implementation
// =============================================================================

CWinThread::CWinThread() {
    m_pMainWnd = nullptr;
    m_nThreadID = 0;
    m_hThread = nullptr;
    m_bAutoDelete = TRUE;
    memset(&m_msgCur, 0, sizeof(m_msgCur));
}

CWinThread::~CWinThread() {
}

BOOL CWinThread::InitInstance() {
    return TRUE;
}

int CWinThread::ExitInstance() {
    return static_cast<int>(m_msgCur.wParam);
}

int CWinThread::Run() {
    MSG msg;
    LONG idleCount = 0;

    for (;;) {
        // Idle processing when no messages are pending
        while (!PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
            if (!OnIdle(idleCount++)) {
                idleCount = 0;
                ::WaitMessage();
                break;
            }
        }

        // Pump all queued messages
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
            if (!PumpMessage()) {
                return ExitInstance();
            }
            if (IsIdleMessage(&m_msgCur)) {
                idleCount = 0;
            }
        }
    }
}

BOOL CWinThread::PreTranslateMessage(MSG* pMsg) {
    (void)pMsg;
    return FALSE;
}

BOOL CWinThread::OnIdle(LONG lCount) {
    (void)lCount;
    return lCount == 0; // More work to do on the first pass
}

BOOL CWinThread::IsIdleMessage(MSG* pMsg) {
    if (!pMsg) {
        return TRUE;
    }
    switch (pMsg->message) {
    case WM_MOUSEMOVE:
    case WM_NCMOUSEMOVE:
    case WM_MOUSEWHEEL:
#ifdef WM_NCMOUSEWHEEL
    case WM_NCMOUSEWHEEL:
#endif
    case WM_PAINT:
#ifdef WM_SYSTIMER
    case WM_SYSTIMER:
#endif
        return FALSE;
    default:
        return TRUE;
    }
}

BOOL CWinThread::PumpMessage() {
    MSG msg;
    if (!PrePumpMessage()) {
        return FALSE;
    }

    int result = ::GetMessageW(&msg, nullptr, 0, 0);
    if (result <= 0) {
        if (result == 0) {
            m_msgCur = msg;
        }
        return FALSE;
    }

    m_msgCur = msg;
    if (!PreTranslateMessage(&msg)) {
        ::TranslateMessage(&msg);
        ::DispatchMessageW(&msg);
    }
    return PostPumpMessage();
}

BOOL CWinThread::PrePumpMessage() {
    return TRUE;
}

BOOL CWinThread::PostPumpMessage() {
    return TRUE;
}
