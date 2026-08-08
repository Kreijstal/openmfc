// Define OPENMFC_APPCORE_IMPL to use extern declarations instead of inline stubs
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <vector>

// Global state
static CWinApp* g_pApp = nullptr;
static CWinThread* g_pThread = nullptr;
static HINSTANCE g_hInstance = nullptr;
static HINSTANCE g_hResource = nullptr;
thread_local CWinThread* g_threadState = nullptr;

// Runtime class implementations
IMPLEMENT_DYNAMIC(CException, CObject)
IMPLEMENT_DYNAMIC(CMemoryException, CException)
IMPLEMENT_DYNAMIC(CFileException, CException)
IMPLEMENT_DYNAMIC(CCmdTarget, CObject)
IMPLEMENT_DYNAMIC(CWnd, CCmdTarget)
IMPLEMENT_DYNAMIC(CWinThread, CCmdTarget)
IMPLEMENT_DYNAMIC(CWinApp, CWinThread)

// Thread local storage for thread state. This supports worker threads
// that are executing with their own CWinThread context.

// Note: AfxGetApp is defined inline in afxwin.h

CWinThread* AFXAPI AfxGetThread() {
    if (g_threadState != nullptr) {
        return g_threadState;
    }
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
    (void)hPrevInstance;
    
    g_hInstance = hInstance;
    g_hResource = hInstance;

    if (g_pApp != nullptr) {
        g_pApp->m_hInstance = hInstance;
        g_pApp->m_lpCmdLine = lpCmdLine;
        g_pApp->m_nCmdShow = nCmdShow;
    }
    
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
    , m_pszRegistryKey(nullptr)
    , m_hInstance(nullptr)
    , m_lpCmdLine(nullptr)
    , m_nCmdShow(SW_SHOW)
{
    memset(_winapp_padding, 0, sizeof(_winapp_padding));
    
    // Set global app pointer
    if (g_pApp == nullptr) {
        g_pApp = this;
        g_pThread = this;
        if (g_threadState == nullptr) {
            g_threadState = this;
        }
    }
}

int CWinApp::InitApplication() {
    return TRUE;
}

BOOL CWinApp::InitInstance() {
    return TRUE;
}

int CWinApp::ExitInstance() {
    return CWinThread::ExitInstance();
}

int CWinApp::Run() {
    return CWinThread::Run();
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
    (void)nID; (void)nCode; (void)pHandlerInfo;

    // Define member function pointer types for various signatures
    typedef void (CCmdTarget::*AFX_PMSG_v)();
    typedef int (CCmdTarget::*AFX_PMSG_b)();
    typedef void (CCmdTarget::*AFX_PMSG_vw)(UINT);
    typedef void (CCmdTarget::*AFX_PMSG_vww)(UINT, UINT);
    typedef void (CCmdTarget::*AFX_PMSG_vwww)(UINT, UINT, UINT);
    typedef void (CCmdTarget::*AFX_PMSG_vwl)(UINT, LONG);
    typedef LRESULT (CCmdTarget::*AFX_PMSG_lwl)(WPARAM, LPARAM);
    typedef void (CCmdTarget::*AFX_PMSG_vb)(BOOL);
    typedef int (CCmdTarget::*AFX_PMSG_bh)(HANDLE);
    typedef void (CCmdTarget::*AFX_PMSG_cmdui)(void*);
    typedef int (CCmdTarget::*AFX_PMSG_bwl)(UINT, LONG);
    typedef void (CCmdTarget::*AFX_PMSG_vwwh)(UINT, UINT, HANDLE);
    typedef int (CCmdTarget::*AFX_PMSG_iw)(UINT);
    typedef int (CCmdTarget::*AFX_PMSG_iww)(UINT, UINT);

    union MessageMapFunctions {
        AFX_PMSG pfn;
        AFX_PMSG_v pfn_v;
        AFX_PMSG_b pfn_b;
        AFX_PMSG_vw pfn_vw;
        AFX_PMSG_vww pfn_vww;
        AFX_PMSG_vwww pfn_vwww;
        AFX_PMSG_vwl pfn_vwl;
        AFX_PMSG_lwl pfn_lwl;
        AFX_PMSG_vb pfn_vb;
        AFX_PMSG_bh pfn_bh;
        AFX_PMSG_cmdui pfn_cmdui;
        AFX_PMSG_bwl pfn_bwl;
        AFX_PMSG_vwwh pfn_vwwh;
        AFX_PMSG_iw pfn_iw;
        AFX_PMSG_iww pfn_iww;
    };

    MessageMapFunctions mmf;
    mmf.pfn = pfn;

    switch (nSig)
    {
    case AfxSig_vv:
        (pTarget->*mmf.pfn_v)();
        return TRUE;

    case AfxSig_bv:
        return (pTarget->*mmf.pfn_b)();

    case AfxSig_vw:
        (pTarget->*mmf.pfn_vw)(nID);
        return TRUE;

    case AfxSig_vww:
        {
            UINT* pParams = static_cast<UINT*>(pExtra);
            (pTarget->*mmf.pfn_vww)(pParams ? pParams[0] : 0, pParams ? pParams[1] : 0);
        }
        return TRUE;

    case AfxSig_vwww:
        {
            UINT* pParams = static_cast<UINT*>(pExtra);
            (pTarget->*mmf.pfn_vwww)(pParams ? pParams[0] : 0, pParams ? pParams[1] : 0, pParams ? pParams[2] : 0);
        }
        return TRUE;

    case AfxSig_vwl:
        {
            LONG* pParam = static_cast<LONG*>(pExtra);
            (pTarget->*mmf.pfn_vwl)(nID, pParam ? *pParam : 0);
        }
        return TRUE;

    case AfxSig_lwl:
        {
            WPARAM wParam = static_cast<WPARAM>(nID);
            LPARAM lParam = pExtra ? *static_cast<LPARAM*>(pExtra) : 0;
            return static_cast<int>((pTarget->*mmf.pfn_lwl)(wParam, lParam));
        }

    case AfxSig_v_b:
    case AfxSig_vb:
        (pTarget->*mmf.pfn_vb)(pExtra ? *static_cast<BOOL*>(pExtra) : FALSE);
        return TRUE;

    case AfxSig_bh:
        {
            HANDLE h = pExtra ? *static_cast<HANDLE*>(pExtra) : nullptr;
            return (pTarget->*mmf.pfn_bh)(h);
        }

    case AfxSig_cmdui:
        (pTarget->*mmf.pfn_cmdui)(pExtra);
        return TRUE;

    case AfxSig_bwl:
        {
            LONG* pParam = static_cast<LONG*>(pExtra);
            return (pTarget->*mmf.pfn_bwl)(nID, pParam ? *pParam : 0);
        }

    case AfxSig_vwwh:
        {
            void** pParams = static_cast<void**>(pExtra);
            UINT u1 = pParams ? static_cast<UINT>(reinterpret_cast<UINT_PTR>(pParams[0])) : 0;
            UINT u2 = pParams ? static_cast<UINT>(reinterpret_cast<UINT_PTR>(pParams[1])) : 0;
            HANDLE h = pParams ? static_cast<HANDLE>(pParams[2]) : nullptr;
            (pTarget->*mmf.pfn_vwwh)(u1, u2, h);
        }
        return TRUE;

    case AfxSig_iw:
        return (pTarget->*mmf.pfn_iw)(nID);

    case AfxSig_iww:
        {
            UINT* pParams = static_cast<UINT*>(pExtra);
            return (pTarget->*mmf.pfn_iww)(pParams ? pParams[0] : 0, pParams ? pParams[1] : 0);
        }

    case AfxSig_vv_i:
        (pTarget->*mmf.pfn_v)();
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
        if (PeekMessageW(&msg, nullptr, WM_QUIT, WM_QUIT, PM_REMOVE)) {
            m_msgCur = msg;
            return static_cast<int>(m_msgCur.wParam);
        }

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
                return static_cast<int>(m_msgCur.wParam);
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
