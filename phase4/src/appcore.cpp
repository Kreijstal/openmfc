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
// Forward declarations from wincore.cpp
// =============================================================================

// Cleanup stale temporary CWnd wrappers (called during idle processing)
void OpenMfcCleanupTempWrappers();

// =============================================================================
// CWinThread Implementation
// =============================================================================

// Core class implementations
IMPLEMENT_DYNAMIC(CCmdTarget, CObject)
// CWnd moved to wincore.cpp
// CWinThread and CWinApp implementations are further down

// Create MSVC symbol aliases for static class members
// These are needed because MSVC client code expects MSVC-mangled names
#ifdef __GNUC__
// CCmdTarget::classCCmdTarget
asm(".globl \"?classCCmdTarget@CCmdTarget@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCCmdTarget@CCmdTarget@@2UCRuntimeClass@@A\", _ZN10CCmdTarget15classCCmdTargetE\n");
// CWinThread::classCWinThread
asm(".globl \"?classCWinThread@CWinThread@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWinThread@CWinThread@@2UCRuntimeClass@@A\", _ZN10CWinThread15classCWinThreadE\n");
// CWinApp::classCWinApp
asm(".globl \"?classCWinApp@CWinApp@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWinApp@CWinApp@@2UCRuntimeClass@@A\", _ZN7CWinApp12classCWinAppE\n");
#endif

// Symbol: ?GetThisClass@CWinThread@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinThread__SAPEAUCRuntimeClass__XZ() {
    return &CWinThread::classCWinThread;
}

#ifdef __GNUC__
asm(".globl \"?GetThisClass@CWinThread@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CWinThread@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CWinThread__SAPEAUCRuntimeClass__XZ\n");
#endif

// Symbol: ?GetThisClass@CWinApp@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinApp__SAPEAUCRuntimeClass__XZ() {
    return &CWinApp::classCWinApp;
}

#ifdef __GNUC__
asm(".globl \"?GetThisClass@CWinApp@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CWinApp@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CWinApp__SAPEAUCRuntimeClass__XZ\n");
#endif

// Symbol: ?GetThisClass@CCmdTarget@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CCmdTarget__SAPEAUCRuntimeClass__XZ() {
    return &CCmdTarget::classCCmdTarget;
}

#ifdef __GNUC__
asm(".globl \"?GetThisClass@CCmdTarget@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CCmdTarget@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CCmdTarget__SAPEAUCRuntimeClass__XZ\n");
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

// Exception implementations
IMPLEMENT_DYNAMIC(CMemoryException, CException)
IMPLEMENT_DYNAMIC(CFileException, CException)

// CFileException::GetErrorMessage implementation
int CFileException::GetErrorMessage(wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext) const {
    if (pnHelpContext != nullptr) {
        *pnHelpContext = 0;
    }

    if (lpszError == nullptr || nMaxError == 0) {
        return 0;
    }

    const wchar_t* pszMessage = nullptr;

    switch (m_cause) {
    case none:
        pszMessage = L"No error";
        break;
    case genericException:
        pszMessage = L"Generic file error";
        break;
    case fileNotFound:
        pszMessage = L"File not found";
        break;
    case badPath:
        pszMessage = L"Invalid path";
        break;
    case tooManyOpenFiles:
        pszMessage = L"Too many open files";
        break;
    case accessDenied:
        pszMessage = L"Access denied";
        break;
    case invalidFile:
        pszMessage = L"Invalid file";
        break;
    case removeCurrentDir:
        pszMessage = L"Cannot remove current directory";
        break;
    case directoryFull:
        pszMessage = L"Directory is full";
        break;
    case badSeek:
        pszMessage = L"Invalid seek operation";
        break;
    case hardIO:
        pszMessage = L"Hardware I/O error";
        break;
    case sharingViolation:
        pszMessage = L"File sharing violation";
        break;
    case lockViolation:
        pszMessage = L"File lock violation";
        break;
    case diskFull:
        pszMessage = L"Disk is full";
        break;
    case endOfFile:
        pszMessage = L"Unexpected end of file";
        break;
    default:
        pszMessage = L"Unknown file error";
        break;
    }

    if (pszMessage) {
        wcsncpy(lpszError, pszMessage, nMaxError - 1);
        lpszError[nMaxError - 1] = L'\0';
        return 1;  // TRUE - message provided
    }

    lpszError[0] = L'\0';
    return 0;
}

// Define MS_ABI if not defined
#ifdef __GNUC__
  #ifndef MS_ABI
    #define MS_ABI __attribute__((ms_abi))
  #endif
#else
  #define MS_ABI
#endif

// Symbol: ?GetThisClass@CMemoryException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ() {
    return &CMemoryException::classCMemoryException;
}

#ifdef __GNUC__
asm(".globl \"?GetThisClass@CMemoryException@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CMemoryException@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ\n");
#endif

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

// Symbol: ?GetThisClass@CFileException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ() {
    return &CFileException::classCFileException;
}

#ifdef __GNUC__
asm(".globl \"?GetThisClass@CFileException@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CFileException@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ\n");
#endif

IMPLEMENT_DYNAMIC(CArchiveException, CException)

#ifdef __GNUC__
// CArchiveException::classCArchiveException - MSVC symbol alias
// Note: GCC mangling uses 22 for the member name length
asm(".globl \"?classCArchiveException@CArchiveException@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCArchiveException@CArchiveException@@2UCRuntimeClass@@A\", _ZN17CArchiveException22classCArchiveExceptionE\n");
#endif

// Symbol: ?GetThisClass@CArchiveException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CArchiveException__SAPEAUCRuntimeClass__XZ() {
    return &CArchiveException::classCArchiveException;
}

#ifdef __GNUC__
asm(".globl \"?GetThisClass@CArchiveException@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CArchiveException@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CArchiveException__SAPEAUCRuntimeClass__XZ\n");
#endif

CArchiveException::CArchiveException(int cause, const wchar_t* lpszArchiveName)
    : m_cause(cause)
{
    if (lpszArchiveName)
        m_strFileName = lpszArchiveName;
}

int CArchiveException::GetErrorMessage(wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext) const {
    if (pnHelpContext != nullptr) {
        *pnHelpContext = 0;
    }

    if (lpszError == nullptr || nMaxError == 0) {
        return 0;
    }

    const wchar_t* pszMessage = nullptr;

    switch (m_cause) {
    case none:
        pszMessage = L"No error";
        break;
    case generic:
        pszMessage = L"Archive error";
        break;
    case readOnly:
        pszMessage = L"Cannot write to read-only archive";
        break;
    case endOfFile:
        pszMessage = L"Unexpected end of file";
        break;
    case writeOnly:
        pszMessage = L"Cannot read from write-only archive";
        break;
    case badIndex:
        pszMessage = L"Invalid object index";
        break;
    case badClass:
        pszMessage = L"Invalid class found in archive";
        break;
    case badSchema:
        pszMessage = L"Schema mismatch in archive";
        break;
    case badFormat:
        pszMessage = L"Bad archive format";
        break;
    default:
        pszMessage = L"Unknown archive error";
        break;
    }

    if (pszMessage) {
        wcsncpy(lpszError, pszMessage, nMaxError - 1);
        lpszError[nMaxError - 1] = L'\0';
        return 1;  // TRUE - message provided
    }

    lpszError[0] = L'\0';
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
    if (lCount == 0) {
        // First idle pass - cleanup temporary CWnd wrappers for destroyed windows
        // This handles wrappers created by OpenMfcAttachCWnd for windows that don't
        // use our window procedure (e.g., dialog controls from GetDlgItem)
        OpenMfcCleanupTempWrappers();
    }
    return lCount == 0; // More work to do on first pass
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
            // WM_QUIT received - msg is valid, store it for ExitInstance()
            m_msgCur = msg;
        }
        // result == -1 is an error - msg has undefined content, don't use it
        return FALSE;
    }

    // Store the current message so callers can access it via m_msgCur
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


// Global application pointer (exported via openmfc_exports.cpp)
CWinApp* g_pApp = nullptr;

// Thread-local storage for current thread (used by worker threads)
// This is set by AfxBeginThread in synccore.cpp for worker threads
__thread CWinThread* g_pCurrentThread = nullptr;

// AfxGetThread - returns current thread (or app for main thread)
// C++ implementation for internal use
CWinThread* AfxGetThread() {
    // For worker threads, return the thread-local pointer
    if (g_pCurrentThread != nullptr) {
        return g_pCurrentThread;
    }
    // For main thread, return the app
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
