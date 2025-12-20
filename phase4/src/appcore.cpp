// App Core Implementation (CWinApp, CWinThread)
//
// Implements core application and thread classes.
// Currently stubs to satisfy linker dependencies.

// Define OPENMFC_APPCORE_IMPL to use extern declarations instead of inline stubs
#define OPENMFC_APPCORE_IMPL
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

// Create MSVC symbol aliases for static class members
// These are needed because MSVC client code expects MSVC-mangled names
#ifdef __GNUC__
// CCmdTarget::classCCmdTarget
asm(".globl \"?classCCmdTarget@CCmdTarget@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCCmdTarget@CCmdTarget@@2UCRuntimeClass@@A\", _ZN10CCmdTarget15classCCmdTargetE\n");
// CWnd::classCWnd
asm(".globl \"?classCWnd@CWnd@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWnd@CWnd@@2UCRuntimeClass@@A\", _ZN4CWnd9classCWndE\n");
#endif

// =============================================================================
// CCmdTarget Implementation
// =============================================================================

// CCmdTarget destructor
// Symbol: ??1CCmdTarget@@UEAA@XZ (x64)
CCmdTarget::~CCmdTarget() {
    // Base class destructor - nothing specific to clean up
}

// Exported stub for CCmdTarget destructor
extern "C" void MS_ABI impl___1CCmdTarget__UEAA_XZ(CCmdTarget* pThis) {
    if (pThis) {
        pThis->~CCmdTarget();
    }
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
    
    for (; pMap != nullptr; pMap = (*pMap->pfnGetBaseMap)())
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

// Exception implementations
IMPLEMENT_DYNAMIC(CMemoryException, CException)
IMPLEMENT_DYNAMIC(CFileException, CException)

#ifdef __GNUC__
// CMemoryException::classCMemoryException - MSVC symbol alias
// Note: GCC mangling uses 21 for the member name length (includes the full count)
asm(".globl \"?classCMemoryException@CMemoryException@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCMemoryException@CMemoryException@@2UCRuntimeClass@@A\", _ZN16CMemoryException21classCMemoryExceptionE\n");

// CFileException::classCFileException - MSVC symbol alias
// Note: GCC mangling uses 19 for the member name length
asm(".globl \"?classCFileException@CFileException@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCFileException@CFileException@@2UCRuntimeClass@@A\", _ZN14CFileException19classCFileExceptionE\n");
#endif

IMPLEMENT_DYNAMIC(CArchiveException, CException)

#ifdef __GNUC__
// CArchiveException::classCArchiveException - MSVC symbol alias
// Note: GCC mangling uses 22 for the member name length
asm(".globl \"?classCArchiveException@CArchiveException@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCArchiveException@CArchiveException@@2UCRuntimeClass@@A\", _ZN17CArchiveException22classCArchiveExceptionE\n");
#endif

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

#ifdef __GNUC__
// CWinThread::classCWinThread - MSVC symbol alias
asm(".globl \"?classCWinThread@CWinThread@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWinThread@CWinThread@@2UCRuntimeClass@@A\", _ZN10CWinThread15classCWinThreadE\n");
#endif

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
    // Real message loop implementation
    MSG msg;

    // Main message loop
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        // Allow PreTranslateMessage to filter
        if (!PreTranslateMessage(&msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        // Idle processing
        while (!PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
            if (!OnIdle(0)) {
                break;  // No more idle work
            }
        }
    }

    m_msgCur = msg;
    return ExitInstance();
}

// CWinThread::Run - exported stub
// Symbol: ?Run@CWinThread@@UAAHXZ
// Ordinal: 12614
extern "C" int MS_ABI impl__Run_CWinThread__UEAAHXZ(CWinThread* pThis) {
    return pThis->Run();
}

// CWinApp::Run - exported stub (delegates to CWinThread::Run)
// Symbol: ?Run@CWinApp@@UAAHXZ
// Ordinal: 12613
extern "C" int MS_ABI impl__Run_CWinApp__UEAAHXZ(CWinApp* pThis) {
    return pThis->Run();
}

// CWinApp::InitInstance - exported stub
// Symbol: ?InitInstance@CWinApp@@UAAHXZ
// Ordinal: 7726
extern "C" int MS_ABI impl__InitInstance_CWinApp__UEAAHXZ(CWinApp* pThis) {
    return pThis->InitInstance();
}

// CWinThread::InitInstance - exported stub
// Symbol: ?InitInstance@CWinThread@@UAAHXZ
// Ordinal: 7727
extern "C" int MS_ABI impl__InitInstance_CWinThread__UEAAHXZ(CWinThread* pThis) {
    return pThis->InitInstance();
}

// CWinThread::ExitInstance - exported stub
// Symbol: ?ExitInstance@CWinThread@@UAAHXZ
// Ordinal: 4457
extern "C" int MS_ABI impl__ExitInstance_CWinThread__UEAAHXZ(CWinThread* pThis) {
    return pThis->ExitInstance();
}

// CWinApp::ExitInstance - exported stub
// Symbol: ?ExitInstance@CWinApp@@UAAHXZ
// Ordinal: 4455
extern "C" int MS_ABI impl__ExitInstance_CWinApp__UEAAHXZ(CWinApp* pThis) {
    return pThis->ExitInstance();
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

#ifdef __GNUC__
// CWinApp::classCWinApp - MSVC symbol alias
asm(".globl \"?classCWinApp@CWinApp@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWinApp@CWinApp@@2UCRuntimeClass@@A\", _ZN7CWinApp12classCWinAppE\n");
#endif

// CWinApp methods are defined inline in afxwin.h
// We only need the runtime class implementation here


// Global application pointer (internal use only)
static CWinApp* g_pApp = nullptr;

// AfxGetThread - returns current thread (or app for main thread)
// C++ implementation for internal use
CWinThread* AfxGetThread() {
    return static_cast<CWinThread*>(g_pApp);
}

// AfxGetThread - exported stub for MSVC
// Symbol: ?AfxGetThread@@YAPEAVCWinThread@@XZ
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ() {
    return AfxGetThread();
}

// =============================================================================
// CWinApp Constructor/Destructor
// =============================================================================

// CWinApp constructor
// Symbol: ??0CWinApp@@QAA@PB_W@Z
// Ordinal: 983
extern "C" CWinApp* MS_ABI impl___0CWinApp__QEAA_PEB_W_Z(CWinApp* pThis, const wchar_t* lpszAppName) {
    // Initialize base class (CWinThread)
    pThis->m_pMainWnd = nullptr;
    pThis->m_nThreadID = GetCurrentThreadId();
    pThis->m_hThread = GetCurrentThread();
    pThis->m_bAutoDelete = FALSE;  // App object should not auto-delete

    // Initialize CWinApp members
    pThis->m_hInstance = nullptr;
    pThis->m_lpCmdLine = nullptr;
    pThis->m_nCmdShow = SW_SHOW;
    pThis->m_pszAppName = lpszAppName;
    pThis->m_pszExeName = nullptr;
    pThis->m_pszHelpFilePath = nullptr;
    pThis->m_pszProfileName = nullptr;
    pThis->m_pszRegistryKey = nullptr;

    // Register as the global app instance
    g_pApp = pThis;
    return pThis;
}

// CWinApp destructor
// Symbol: ??1CWinApp@@UAA@XZ
// Ordinal: 1450
extern "C" void MS_ABI impl___1CWinApp__UEAA_XZ(CWinApp* pThis) {
    if (g_pApp == pThis) {
        g_pApp = nullptr;
    }
}

// CWinThread default constructor
// Symbol: ??0CWinThread@@QAA@XZ
// Ordinal: 988
extern "C" CWinThread* MS_ABI impl___0CWinThread__QEAA_XZ(CWinThread* pThis) {
    pThis->m_pMainWnd = nullptr;
    pThis->m_nThreadID = 0;
    pThis->m_hThread = nullptr;
    pThis->m_bAutoDelete = TRUE;
    memset(&pThis->m_msgCur, 0, sizeof(pThis->m_msgCur));
    return pThis;
}

// CWinThread destructor
// Symbol: ??1CWinThread@@UAA@XZ
// Ordinal: 1453
extern "C" void MS_ABI impl___1CWinThread__UEAA_XZ(CWinThread* pThis) {
    pThis->~CWinThread();
}

// Additional global state needed for AfxGetInstanceHandle, etc.
static HINSTANCE g_hInstance = nullptr;
static HINSTANCE g_hResource = nullptr;

// AfxGetInstanceHandle implementation
HINSTANCE AFXAPI AfxGetInstanceHandle() {
    return g_hInstance;
}

// AfxGetResourceHandle implementation
HINSTANCE AFXAPI AfxGetResourceHandle() {
    return g_hResource ? g_hResource : g_hInstance;
}

// AfxSetResourceHandle implementation
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

// AfxGetMainWnd implementation
CWnd* AFXAPI AfxGetMainWnd() {
    CWinThread* pThread = AfxGetThread();
    return pThread ? pThread->m_pMainWnd : nullptr;
}

// Note: AfxGetThread, AfxGetInstanceHandle, AfxGetResourceHandle,
// AfxSetResourceHandle, AfxGetMainWnd, and AfxWinInit are now
// inline functions defined in afxwin.h
