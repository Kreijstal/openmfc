// App Core Implementation (CWinApp, CWinThread)
//
// Implements core application and thread classes.
// Currently stubs to satisfy linker dependencies.

// Define OPENMFC_APPCORE_IMPL to use extern declarations instead of inline stubs
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cwctype>
#include <new>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

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

    if (!pszMessage) {
        lpszError[0] = L'\0';
        return 0;
    }

    if (!m_strFileName.IsEmpty()) {
        int written = _snwprintf(
            lpszError, nMaxError, L"%ls: %ls", pszMessage, static_cast<const wchar_t*>(m_strFileName));
        if (written < 0 || static_cast<UINT>(written) >= nMaxError) {
            lpszError[nMaxError - 1] = L'\0';
        }
    } else if (m_lOsError != -1) {
        int written = _snwprintf(lpszError, nMaxError, L"%ls (OS error %ld)", pszMessage, m_lOsError);
        if (written < 0 || static_cast<UINT>(written) >= nMaxError) {
            lpszError[nMaxError - 1] = L'\0';
        }
    } else {
        wcsncpy(lpszError, pszMessage, nMaxError - 1);
        lpszError[nMaxError - 1] = L'\0';
    }
    lpszError[nMaxError - 1] = L'\0';
    return 1;  // TRUE - message provided
}

// Define MS_ABI if not defined
#ifdef __GNUC__
  #ifndef MS_ABI
    #define MS_ABI __attribute__((ms_abi))
  #endif
#else
  #define MS_ABI
#endif

// Symbol: ?GetErrorMessage@CFileException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_CFileException__UEBAHPEA_WIPEAI_Z(
    const CFileException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext) {
    return pThis->GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}

#ifdef __GNUC__
asm(".globl \"?GetErrorMessage@CFileException@@UEBAHPEA_WIPEAI@Z\"\n"
    ".set \"?GetErrorMessage@CFileException@@UEBAHPEA_WIPEAI@Z\", impl__GetErrorMessage_CFileException__UEBAHPEA_WIPEAI_Z\n");
#endif

// Symbol: ?GetErrorMessage@CArchiveException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_CArchiveException__UEBAHPEA_WIPEAI_Z(
    const CArchiveException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext) {
    return pThis->GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}

#ifdef __GNUC__
asm(".globl \"?GetErrorMessage@CArchiveException@@UEBAHPEA_WIPEAI@Z\"\n"
    ".set \"?GetErrorMessage@CArchiveException@@UEBAHPEA_WIPEAI@Z\", impl__GetErrorMessage_CArchiveException__UEBAHPEA_WIPEAI_Z\n");
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

    if (!pszMessage) {
        lpszError[0] = L'\0';
        return 0;
    }

    if (!m_strFileName.IsEmpty()) {
        int written = _snwprintf(
            lpszError, nMaxError, L"%ls: %ls", pszMessage, static_cast<const wchar_t*>(m_strFileName));
        if (written < 0 || static_cast<UINT>(written) >= nMaxError) {
            lpszError[nMaxError - 1] = L'\0';
        }
    } else {
        wcsncpy(lpszError, pszMessage, nMaxError - 1);
        lpszError[nMaxError - 1] = L'\0';
    }
    lpszError[nMaxError - 1] = L'\0';
    return 1;  // TRUE - message provided
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

static int OpenMfcRunExportedMessageLoop(CWinThread* pThis) {
    MSG msg;

    for (;;) {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            pThis->m_msgCur = msg;
            if (msg.message == WM_QUIT) {
                return static_cast<int>(msg.wParam);
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        const BOOL result = GetMessageW(&msg, nullptr, 0, 0);
        pThis->m_msgCur = msg;

        if (result == 0) {
            return static_cast<int>(msg.wParam);
        }
        if (result < 0) {
            return -1;
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

// CWinThread::Run - exported lifecycle implementation
// Symbol: ?Run@CWinThread@@UEAAHXZ
// Ordinal: 12614
extern "C" int MS_ABI impl__Run_CWinThread__UEAAHXZ(CWinThread* pThis) {
    return OpenMfcRunExportedMessageLoop(pThis);
}

// CWinApp::Run - exported lifecycle implementation
// Symbol: ?Run@CWinApp@@UEAAHXZ
// Ordinal: 12613
extern "C" int MS_ABI impl__Run_CWinApp__UEAAHXZ(CWinApp* pThis) {
    return OpenMfcRunExportedMessageLoop(pThis);
}

// CWinApp::InitInstance - exported lifecycle implementation
// Symbol: ?InitInstance@CWinApp@@UEAAHXZ
// Ordinal: 7726
extern "C" int MS_ABI impl__InitInstance_CWinApp__UEAAHXZ(CWinApp* pThis) {
    return pThis->InitInstance();
}

// CWinThread::InitInstance - exported lifecycle implementation
// Symbol: ?InitInstance@CWinThread@@UEAAHXZ
// Ordinal: 7727
extern "C" int MS_ABI impl__InitInstance_CWinThread__UEAAHXZ(CWinThread* pThis) {
    return pThis->InitInstance();
}

// CWinThread::ExitInstance - exported lifecycle implementation
// Symbol: ?ExitInstance@CWinThread@@UEAAHXZ
// Ordinal: 4457
extern "C" int MS_ABI impl__ExitInstance_CWinThread__UEAAHXZ(CWinThread* pThis) {
    return pThis->ExitInstance();
}

// CWinApp::ExitInstance - exported lifecycle implementation
// Symbol: ?ExitInstance@CWinApp@@UEAAHXZ
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
// Source-backed defaults are provided here for _AFXDLL consumers.
extern CWinApp* g_pApp;

CWinApp::CWinApp(const wchar_t* lpszAppName) {
    m_pMainWnd = nullptr;
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
    memset(&m_msgCur, 0, sizeof(m_msgCur));
    g_pApp = this;
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
// Symbol: ??0CWinApp@@QEAA@PEB_W@Z
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
    memset(&pThis->m_msgCur, 0, sizeof(pThis->m_msgCur));

    // Register as the global app instance
    g_pApp = pThis;
    return pThis;
}

// CWinApp destructor
// Symbol: ??1CWinApp@@UEAA@XZ
// Ordinal: 1450
extern "C" void MS_ABI impl___1CWinApp__UEAA_XZ(CWinApp* pThis) {
    if (g_pApp == pThis) {
        g_pApp = nullptr;
    }
}

// CWinThread default constructor
// Symbol: ??0CWinThread@@QEAA@XZ
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
// Symbol: ??1CWinThread@@UEAA@XZ
// Ordinal: 1453
extern "C" void MS_ABI impl___1CWinThread__UEAA_XZ(CWinThread* pThis) {
    pThis->~CWinThread();
}

// Additional global state needed for AfxGetInstanceHandle, etc.
static HINSTANCE g_hInstance = nullptr;
static HINSTANCE g_hResource = nullptr;
constexpr size_t kOpaqueStateWordCount = 32;
static void* g_appModuleStateStorage[kOpaqueStateWordCount] = {};
static __thread void* g_threadStateStorage[kOpaqueStateWordCount] = {};

// AfxGetInstanceHandle implementation
HINSTANCE AFXAPI AfxGetInstanceHandle() {
    return g_hInstance;
}

// AfxGetResourceHandle implementation
HINSTANCE AFXAPI AfxGetResourceHandle() {
    return g_hResource ? g_hResource : g_hInstance;
}

// Symbol: ?AfxGetInstanceHandleHelper@@YAPEAUHINSTANCE__@@XZ
HINSTANCE AFXAPI AfxGetInstanceHandleHelper() {
    // Real MFC exposes a helper symbol used by some import libraries.
    // OpenMFC keeps it aligned with AfxGetInstanceHandle state.
    return AfxGetInstanceHandle();
}

extern "C" HINSTANCE MS_ABI impl__AfxGetInstanceHandleHelper__YAPEAUHINSTANCE____XZ() {
    return AfxGetInstanceHandleHelper();
}

// AfxSetResourceHandle implementation
void AFXAPI AfxSetResourceHandle(HINSTANCE hInstResource) {
    g_hResource = hInstResource;
}

// Symbol: ?AfxGetAppModuleState@@YAPEAVAFX_MODULE_STATE@@XZ
AFX_MODULE_STATE* AFXAPI AfxGetAppModuleState() {
    return reinterpret_cast<AFX_MODULE_STATE*>(g_appModuleStateStorage);
}

extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetAppModuleState__YAPEAVAFX_MODULE_STATE__XZ() {
    return AfxGetAppModuleState();
}

// Symbol: ?AfxGetThreadState@@YAPEAV_AFX_THREAD_STATE@@XZ
_AFX_THREAD_STATE* AFXAPI AfxGetThreadState() {
    return reinterpret_cast<_AFX_THREAD_STATE*>(g_threadStateStorage);
}

extern "C" _AFX_THREAD_STATE* MS_ABI impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ() {
    return AfxGetThreadState();
}

// AfxWinInit implementation
// Symbol: ?AfxWinInit@@YAHPEAUHINSTANCE__@@0PEA_WH@Z
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

extern "C" int MS_ABI impl__AfxWinInit__YAHPEAUHINSTANCE____0PEA_WH_Z(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    wchar_t* lpCmdLine,
    int nCmdShow)
{
    return AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

// AfxGetMainWnd implementation
// Symbol: ?AfxGetMainWnd@@YAPEAVCWnd@@XZ
CWnd* AFXAPI AfxGetMainWnd() {
    CWinThread* pThread = AfxGetThread();
    return pThread ? pThread->m_pMainWnd : nullptr;
}

extern "C" CWnd* MS_ABI impl__AfxGetMainWnd__YAPEAVCWnd__XZ() {
    return AfxGetMainWnd();
}

// Note: AfxGetThread, AfxGetInstanceHandle, AfxGetResourceHandle,
// AfxSetResourceHandle, AfxGetMainWnd, and AfxWinInit are now
// inline functions defined in afxwin.h

// =============================================================================
// Conservative app/runtime residual helpers
// =============================================================================
class CCommandLineInfo;
class CDocManager;
class CRecentFileList;

namespace {
struct CommandLineInfoState {
    int shellCommand = 0;
    int openRecentIndex = -1;
    bool hasFileName = false;
    std::wstring fileName;
};

struct DocManagerState {
    std::vector<CDocTemplate*> templates;
};

struct RecentFileListState {
    UINT start = 0;
    int maxEntries = 0;
    int maxDisplayLen = 0;
    std::wstring section;
    std::wstring entryFormat;
    std::vector<std::wstring> entries;
};

struct AppRuntimeState {
    std::vector<CDocTemplate*> templates;
    std::vector<std::wstring> recentEntries;
    unsigned int maxRecent = 4;
    std::unordered_map<std::wstring, unsigned int> profileInts;
    std::unordered_map<std::wstring, std::wstring> profileStrings;
    std::unordered_map<std::wstring, std::vector<unsigned char>> profileBinary;
    std::wstring registryRoot;
    std::wstring appId;
    bool modelessEnabled = true;
    bool shellOpenEnabled = false;
    bool taskbarEnabled = false;
    bool d2dEnabled = false;
    bool automated = false;
    bool embedded = false;
    HGLOBAL printerDevMode = nullptr;
    HGLOBAL printerDevNames = nullptr;
    ITaskbarList* taskbarList = nullptr;
    ITaskbarList3* taskbarList3 = nullptr;
};

std::unordered_map<const CCommandLineInfo*, CommandLineInfoState> g_commandLineInfoStates;
std::unordered_map<const CDocManager*, DocManagerState> g_docManagerStates;
std::unordered_map<const CRecentFileList*, RecentFileListState> g_recentFileListStates;
std::unordered_map<const CWinApp*, AppRuntimeState> g_appRuntimeStates;
std::unordered_map<const CCmdTarget*, int> g_waitCursorDepth;

constexpr int kShellCommandFileNothing = 0;
constexpr int kShellCommandFileNew = 1;
constexpr int kShellCommandFileOpen = 2;
constexpr int kShellCommandFilePrint = 3;
constexpr int kShellCommandFilePrintTo = 4;
constexpr int kShellCommandAppRegister = 5;
constexpr int kShellCommandAppUnregister = 6;
constexpr int kShellCommandFileDDE = 7;

std::wstring MakeProfileKey(const wchar_t* section, const wchar_t* entry) {
    std::wstring key = section ? section : L"";
    key.push_back(L'\x1f');
    key.append(entry ? entry : L"");
    return key;
}

std::wstring WideValue(const wchar_t* value) {
    return value ? value : L"";
}

std::wstring LowerWide(std::wstring value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); });
    return value;
}

std::wstring GetAppRegistryRoot(CWinApp* app) {
    AppRuntimeState& state = g_appRuntimeStates[app];
    if (!state.registryRoot.empty()) return state.registryRoot;
    if (app && app->m_pszRegistryKey && *app->m_pszRegistryKey) return app->m_pszRegistryKey;
    if (app && app->m_pszAppName && *app->m_pszAppName) return std::wstring(L"Software\\") + app->m_pszAppName;
    return L"Software\\OpenMFC";
}

std::wstring GetAppName(CWinApp* app) {
    if (app && app->m_pszAppName && *app->m_pszAppName) return app->m_pszAppName;
    if (app && app->m_pszExeName && *app->m_pszExeName) return app->m_pszExeName;
    return L"OpenMFC";
}

HWND GetAppMainHwnd(CWinApp* app) {
    return app && app->m_pMainWnd ? app->m_pMainWnd->GetSafeHwnd() : nullptr;
}

int CountTemplateDocuments(CDocTemplate* tpl) {
    if (!tpl) return 0;
    int count = 0;
    void* pos = tpl->GetFirstDocPosition();
    while (pos) {
        CDocument* doc = tpl->GetNextDoc(pos);
        if (doc) ++count;
    }
    return count;
}

bool SaveTemplateDocuments(CDocTemplate* tpl) {
    if (!tpl) return true;
    void* pos = tpl->GetFirstDocPosition();
    while (pos) {
        CDocument* doc = tpl->GetNextDoc(pos);
        if (doc && !doc->SaveModified()) return false;
    }
    return true;
}

void CloseTemplateDocuments(CDocTemplate* tpl) {
    if (!tpl) return;
    std::vector<CDocument*> docs;
    void* pos = tpl->GetFirstDocPosition();
    while (pos) {
        CDocument* doc = tpl->GetNextDoc(pos);
        if (doc) docs.push_back(doc);
    }
    for (CDocument* doc : docs) {
        doc->OnCloseDocument();
    }
}

const wchar_t* FindFileNamePart(const wchar_t* path) {
    if (path == nullptr) return L"";
    const wchar_t* name = path;
    for (const wchar_t* p = path; *p != L'\0'; ++p) {
        if (*p == L'\\' || *p == L'/') {
            name = p + 1;
        }
    }
    return name;
}

void AddRecentPath(std::vector<std::wstring>& entries, int maxEntries, const wchar_t* path) {
    if (path == nullptr || *path == L'\0') return;
    std::wstring value(path);
    entries.erase(std::remove(entries.begin(), entries.end(), value), entries.end());
    entries.insert(entries.begin(), std::move(value));
    if (maxEntries >= 0 && static_cast<int>(entries.size()) > maxEntries) {
        entries.resize(static_cast<size_t>(maxEntries));
    }
}

std::vector<std::wstring> TokenizeCommandLine(const wchar_t* cmdLine) {
    std::vector<std::wstring> tokens;
    if (cmdLine == nullptr) return tokens;

    const wchar_t* p = cmdLine;
    while (*p != L'\0') {
        while (*p == L' ' || *p == L'\t') ++p;
        if (*p == L'\0') break;

        std::wstring token;
        bool inQuotes = false;
        while (*p != L'\0') {
            if (*p == L'"') {
                inQuotes = !inQuotes;
                ++p;
                continue;
            }
            if (!inQuotes && (*p == L' ' || *p == L'\t')) break;
            token.push_back(*p++);
        }
        tokens.push_back(std::move(token));
        while (*p == L' ' || *p == L'\t') ++p;
    }
    return tokens;
}

void ParseCommandFlag(CommandLineInfoState& state, const wchar_t* flag) {
    if (flag == nullptr || *flag == L'\0') return;
    std::wstring lower(flag);
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); });

    if (lower == L"n" || lower == L"new") {
        state.shellCommand = kShellCommandFileNew;
    } else if (lower == L"open") {
        state.shellCommand = kShellCommandFileOpen;
    } else if (lower == L"p" || lower == L"print") {
        state.shellCommand = kShellCommandFilePrint;
    } else if (lower == L"pt" || lower == L"printto") {
        state.shellCommand = kShellCommandFilePrintTo;
    } else if (lower == L"dde") {
        state.shellCommand = kShellCommandFileDDE;
    } else if (lower == L"register") {
        state.shellCommand = kShellCommandAppRegister;
    } else if (lower == L"unregister") {
        state.shellCommand = kShellCommandAppUnregister;
    }
}

void ParseNonFlagToken(CommandLineInfoState& state, const wchar_t* token) {
    if (token == nullptr || *token == L'\0') return;
    state.fileName = token;
    state.hasFileName = true;
    if (state.shellCommand == kShellCommandFileNothing || state.shellCommand == kShellCommandFileNew) {
        state.shellCommand = kShellCommandFileOpen;
    }
}

CDocument* OpenWithTemplates(const std::vector<CDocTemplate*>& templates, const wchar_t* path, int makeVisible) {
    for (CDocTemplate* tpl : templates) {
        if (tpl == nullptr) continue;
        CDocument* doc = tpl->OpenDocumentFile(path, makeVisible);
        if (doc != nullptr) return doc;
    }
    return nullptr;
}
}  // namespace

// Symbol: ??0CCommandLineInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CCommandLineInfo__QEAA_XZ(void* pThis) {
    if (pThis) g_commandLineInfoStates[reinterpret_cast<CCommandLineInfo*>(pThis)] = CommandLineInfoState{};
    return pThis;
}

// Symbol: ??1CCommandLineInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1CCommandLineInfo__UEAA_XZ(CCommandLineInfo* pThis) {
    if (pThis) g_commandLineInfoStates.erase(pThis);
}

// Symbol: ?ParseParam@CCommandLineInfo@@UEAAXPEB_WHH@Z
extern "C" void MS_ABI impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(
    CCommandLineInfo* pThis, const wchar_t* pszParam, int bFlag, int bLast) {
    if (!pThis) return;
    CommandLineInfoState& state = g_commandLineInfoStates[pThis];
    if (bFlag) ParseCommandFlag(state, pszParam);
    else ParseNonFlagToken(state, pszParam);
    if (bLast && state.shellCommand == kShellCommandFileNothing && !state.hasFileName) {
        state.shellCommand = kShellCommandFileNew;
    }
}

// Symbol: ?ParseParam@CCommandLineInfo@@UEAAXPEBDHH@Z
extern "C" void MS_ABI impl__ParseParam_CCommandLineInfo__UEAAXPEBDHH_Z(
    CCommandLineInfo* pThis, const char* pszParam, int bFlag, int bLast) {
    if (pszParam == nullptr) {
        impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(pThis, nullptr, bFlag, bLast);
        return;
    }
    const int needed = ::MultiByteToWideChar(CP_ACP, 0, pszParam, -1, nullptr, 0);
    if (needed > 0) {
        std::wstring wide(static_cast<size_t>(needed - 1), L'\0');
        ::MultiByteToWideChar(CP_ACP, 0, pszParam, -1, wide.data(), needed);
        impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(pThis, wide.c_str(), bFlag, bLast);
        return;
    }
    std::wstring fallback;
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(pszParam); *p; ++p) {
        fallback.push_back(static_cast<wchar_t>(*p));
    }
    impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(pThis, fallback.c_str(), bFlag, bLast);
}

// Symbol: ?ParseParamFlag@CCommandLineInfo@@IEAAXPEBD@Z
extern "C" void MS_ABI impl__ParseParamFlag_CCommandLineInfo__IEAAXPEBD_Z(CCommandLineInfo* pThis, const char* pszParam) {
    impl__ParseParam_CCommandLineInfo__UEAAXPEBDHH_Z(pThis, pszParam, TRUE, FALSE);
}

// Symbol: ?ParseParamNotFlag@CCommandLineInfo@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__ParseParamNotFlag_CCommandLineInfo__IEAAXPEB_W_Z(CCommandLineInfo* pThis, const wchar_t* pszParam) {
    impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(pThis, pszParam, FALSE, FALSE);
}

// Symbol: ?ParseParamNotFlag@CCommandLineInfo@@IEAAXPEBD@Z
extern "C" void MS_ABI impl__ParseParamNotFlag_CCommandLineInfo__IEAAXPEBD_Z(CCommandLineInfo* pThis, const char* pszParam) {
    impl__ParseParam_CCommandLineInfo__UEAAXPEBDHH_Z(pThis, pszParam, FALSE, FALSE);
}

// Symbol: ?ParseLast@CCommandLineInfo@@IEAAXH@Z
extern "C" void MS_ABI impl__ParseLast_CCommandLineInfo__IEAAXH_Z(CCommandLineInfo* pThis, int bLast) {
    if (!pThis || !bLast) return;
    CommandLineInfoState& state = g_commandLineInfoStates[pThis];
    if (state.shellCommand == kShellCommandFileNothing && !state.hasFileName) {
        state.shellCommand = kShellCommandFileNew;
    }
}

// Symbol: ?ParseCommandLine@CWinApp@@QEAAXAEAVCCommandLineInfo@@@Z
extern "C" void MS_ABI impl__ParseCommandLine_CWinApp__QEAAXAEAVCCommandLineInfo___Z(
    CWinApp* pThis, CCommandLineInfo* pInfo) {
    if (!pThis || !pInfo) return;
    const std::vector<std::wstring> tokens = TokenizeCommandLine(pThis->m_lpCmdLine);
    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::wstring& token = tokens[i];
        const bool isFlag = !token.empty() && (token[0] == L'/' || token[0] == L'-');
        const wchar_t* value = isFlag ? token.c_str() + 1 : token.c_str();
        impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(
            pInfo, value, isFlag ? TRUE : FALSE, (i + 1) == tokens.size() ? TRUE : FALSE);
    }
    if (tokens.empty()) {
        impl__ParseLast_CCommandLineInfo__IEAAXH_Z(pInfo, TRUE);
    }
}

// Symbol: ?ProcessShellCommand@CWinApp@@QEAAHAEAVCCommandLineInfo@@@Z
extern "C" int MS_ABI impl__ProcessShellCommand_CWinApp__QEAAHAEAVCCommandLineInfo___Z(
    CWinApp* pThis, CCommandLineInfo* pInfo) {
    if (!pThis || !pInfo) return FALSE;
    CommandLineInfoState& state = g_commandLineInfoStates[pInfo];
    AppRuntimeState& appState = g_appRuntimeStates[pThis];

    switch (state.shellCommand) {
    case kShellCommandFileNothing:
        return TRUE;
    case kShellCommandFileNew:
        return OpenWithTemplates(appState.templates, nullptr, TRUE) != nullptr;
    case kShellCommandFileOpen:
        return state.hasFileName && OpenWithTemplates(appState.templates, state.fileName.c_str(), TRUE) != nullptr;
    case kShellCommandAppRegister:
    case kShellCommandAppUnregister:
        return TRUE;
    default:
        return FALSE;
    }
}

// Symbol: ?LoadStdProfileSettings@CWinApp@@IEAAXI@Z
extern "C" void MS_ABI impl__LoadStdProfileSettings_CWinApp__IEAAXI_Z(CWinApp* pThis, unsigned int nMaxMRU) {
    if (!pThis) return;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    state.maxRecent = nMaxMRU;
    if (state.recentEntries.size() > nMaxMRU) {
        state.recentEntries.resize(nMaxMRU);
    }
}

// Symbol: ?AddToRecentFileList@CWinApp@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__AddToRecentFileList_CWinApp__UEAAXPEB_W_Z(CWinApp* pThis, const wchar_t* pathName) {
    if (!pThis) return;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    AddRecentPath(state.recentEntries, static_cast<int>(state.maxRecent), pathName);
}

// Symbol: ?GetProfileIntW@CWinApp@@UEAAIPEB_W0H@Z
extern "C" unsigned int MS_ABI impl__GetProfileIntW_CWinApp__UEAAIPEB_W0H_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, int defaultValue) {
    if (!pThis) return static_cast<unsigned int>(defaultValue);
    const auto& map = g_appRuntimeStates[pThis].profileInts;
    const auto it = map.find(MakeProfileKey(section, entry));
    return it == map.end() ? static_cast<unsigned int>(defaultValue) : it->second;
}

// Symbol: ?GetProfileStringW@CWinApp@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W00@Z
extern "C" void MS_ABI impl__GetProfileStringW_CWinApp__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W00_Z(
    CString* ret, CWinApp* pThis, const wchar_t* section, const wchar_t* entry, const wchar_t* defaultValue) {
    if (!ret) return;
    if (!pThis) {
        new (ret) CString(defaultValue ? defaultValue : L"");
        return;
    }
    const auto& map = g_appRuntimeStates[pThis].profileStrings;
    const auto it = map.find(MakeProfileKey(section, entry));
    new (ret) CString(it == map.end() ? (defaultValue ? defaultValue : L"") : it->second.c_str());
}

// Symbol: ?GetProfileBinary@CWinApp@@UEAAHPEB_W0PEAPEAEPEAI@Z
extern "C" int MS_ABI impl__GetProfileBinary_CWinApp__UEAAHPEB_W0PEAPEAEPEAI_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, unsigned char** ppData, unsigned int* pBytes) {
    if (ppData) *ppData = nullptr;
    if (pBytes) *pBytes = 0;
    if (!pThis || !ppData || !pBytes) return FALSE;

    const auto& map = g_appRuntimeStates[pThis].profileBinary;
    const auto it = map.find(MakeProfileKey(section, entry));
    if (it == map.end()) return FALSE;

    const std::vector<unsigned char>& data = it->second;
    if (data.empty()) return FALSE;

    unsigned char* copy = new (std::nothrow) unsigned char[data.size()];
    if (!copy) return FALSE;
    std::memcpy(copy, data.data(), data.size());
    *ppData = copy;
    *pBytes = static_cast<unsigned int>(data.size());
    return TRUE;
}

// Symbol: ?WriteProfileInt@CWinApp@@UEAAHPEB_W0H@Z
extern "C" int MS_ABI impl__WriteProfileInt_CWinApp__UEAAHPEB_W0H_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, int value) {
    if (!pThis || !section || !entry) return FALSE;
    g_appRuntimeStates[pThis].profileInts[MakeProfileKey(section, entry)] = static_cast<unsigned int>(value);
    return TRUE;
}

// Symbol: ?WriteProfileStringW@CWinApp@@UEAAHPEB_W00@Z
extern "C" int MS_ABI impl__WriteProfileStringW_CWinApp__UEAAHPEB_W00_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, const wchar_t* value) {
    if (!pThis || !section || !entry) return FALSE;
    g_appRuntimeStates[pThis].profileStrings[MakeProfileKey(section, entry)] = value ? value : L"";
    return TRUE;
}

// Symbol: ?WriteProfileBinary@CWinApp@@UEAAHPEB_W0PEAEI@Z
extern "C" int MS_ABI impl__WriteProfileBinary_CWinApp__UEAAHPEB_W0PEAEI_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, unsigned char* data, unsigned int bytes) {
    if (!pThis || !section || !entry) return FALSE;
    std::vector<unsigned char> blob;
    if (data && bytes != 0) blob.assign(data, data + bytes);
    g_appRuntimeStates[pThis].profileBinary[MakeProfileKey(section, entry)] = std::move(blob);
    return TRUE;
}

// Symbol: ?DelRegTree@CWinApp@@QEAAJPEAUHKEY__@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCAtlTransactionManager@4@@Z
extern "C" long MS_ABI impl__DelRegTree_CWinApp__QEAAJPEAUHKEY____AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCAtlTransactionManager_4__Z(
    CWinApp* pThis, HKEY hParentKey, const CString* keyName, void*) {
    (void)pThis;
    if (!hParentKey || !keyName) return ERROR_INVALID_PARAMETER;
    return ::RegDeleteTreeW(hParentKey, static_cast<const wchar_t*>(*keyName));
}

// Symbol: ?DoPromptFileName@CWinApp@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IKHPEAVCDocTemplate@@@Z
extern "C" int MS_ABI impl__DoPromptFileName_CWinApp__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IKHPEAVCDocTemplate___Z(
    CWinApp* pThis, CString* fileName, unsigned int, unsigned long flags, int bOpenFileDialog, CDocTemplate*) {
    if (!fileName) return FALSE;
    CFileDialog dlg(bOpenFileDialog, nullptr, static_cast<const wchar_t*>(*fileName), flags, nullptr, pThis ? pThis->m_pMainWnd : nullptr);
    if (dlg.DoModal() != IDOK) return FALSE;
    *fileName = dlg.GetPathName();
    return TRUE;
}

// Symbol: ?RegisterWithRestartManager@CWinApp@@UEAAJHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" long MS_ABI impl__RegisterWithRestartManager_CWinApp__UEAAJHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CWinApp*, int bRegisterRecoveryCallback, const CString* restartIdentifier) {
    if (!bRegisterRecoveryCallback) {
        return ::UnregisterApplicationRestart();
    }
    const wchar_t* commandLine = restartIdentifier ? static_cast<const wchar_t*>(*restartIdentifier) : L"";
    return ::RegisterApplicationRestart(commandLine, 0);
}

static HCURSOR GetWaitCursorHandle() {
    static HCURSOR s_wait = ::LoadCursorW(nullptr, IDC_WAIT);
    return s_wait;
}

static HCURSOR GetArrowCursorHandle() {
    static HCURSOR s_arrow = ::LoadCursorW(nullptr, IDC_ARROW);
    return s_arrow;
}

// Symbol: ?BeginWaitCursor@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    if (!pThis) return;
    int& depth = g_waitCursorDepth[pThis];
    ++depth;
    ::SetCursor(GetWaitCursorHandle());
}

// Symbol: ?EndWaitCursor@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    if (!pThis) return;
    int& depth = g_waitCursorDepth[pThis];
    if (depth > 0) --depth;
    if (depth == 0) ::SetCursor(GetArrowCursorHandle());
}

// Symbol: ?RestoreWaitCursor@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__RestoreWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    if (!pThis) return;
    const int depth = g_waitCursorDepth[pThis];
    ::SetCursor(depth > 0 ? GetWaitCursorHandle() : GetArrowCursorHandle());
}

// Symbol: ??0CDocManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDocManager__QEAA_XZ(void* pThis) {
    if (pThis) g_docManagerStates[reinterpret_cast<CDocManager*>(pThis)] = DocManagerState{};
    return pThis;
}

// Symbol: ??1CDocManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CDocManager__UEAA_XZ(CDocManager* pThis) {
    if (pThis) g_docManagerStates.erase(pThis);
}

// Symbol: ?AddDocTemplate@CDocManager@@UEAAXPEAVCDocTemplate@@@Z
extern "C" void MS_ABI impl__AddDocTemplate_CDocManager__UEAAXPEAVCDocTemplate___Z(CDocManager* pThis, CDocTemplate* pTemplate) {
    if (!pThis || !pTemplate) return;
    auto& templates = g_docManagerStates[pThis].templates;
    if (std::find(templates.begin(), templates.end(), pTemplate) == templates.end()) {
        templates.push_back(pTemplate);
    }
}

// Symbol: ?GetBestTemplate@CDocManager@@UEAAPEAVCDocTemplate@@PEB_W@Z
extern "C" CDocTemplate* MS_ABI impl__GetBestTemplate_CDocManager__UEAAPEAVCDocTemplate__PEB_W_Z(
    CDocManager* pThis, const wchar_t* pathName) {
    (void)pathName;
    if (!pThis) return nullptr;
    auto& templates = g_docManagerStates[pThis].templates;
    return templates.empty() ? nullptr : templates.front();
}

// Symbol: ?GetFirstDocTemplatePosition@CDocManager@@UEBAPEAU__POSITION@@XZ
extern "C" void* MS_ABI impl__GetFirstDocTemplatePosition_CDocManager__UEBAPEAU__POSITION__XZ(const CDocManager* pThis) {
    if (!pThis) return nullptr;
    const auto& templates = g_docManagerStates[pThis].templates;
    return templates.empty() ? nullptr : reinterpret_cast<void*>(static_cast<uintptr_t>(1));
}

// Symbol: ?GetNextDocTemplate@CDocManager@@UEBAPEAVCDocTemplate@@AEAPEAU__POSITION@@@Z
extern "C" CDocTemplate* MS_ABI impl__GetNextDocTemplate_CDocManager__UEBAPEAVCDocTemplate__AEAPEAU__POSITION___Z(
    const CDocManager* pThis, void** pos) {
    if (!pThis || !pos || !*pos) return nullptr;
    const auto& templates = g_docManagerStates[pThis].templates;
    const uintptr_t index = reinterpret_cast<uintptr_t>(*pos) - 1;
    if (index >= templates.size()) {
        *pos = nullptr;
        return nullptr;
    }
    *pos = (index + 1 < templates.size()) ? reinterpret_cast<void*>(index + 2) : nullptr;
    return templates[index];
}

// Symbol: ?GetOpenDocumentCount@CDocManager@@UEAAHXZ
extern "C" int MS_ABI impl__GetOpenDocumentCount_CDocManager__UEAAHXZ(CDocManager* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?GetDocumentCount@CDocManager@@IEAAHXZ
extern "C" int MS_ABI impl__GetDocumentCount_CDocManager__IEAAHXZ(CDocManager* pThis) {
    if (!pThis) return 0;
    return static_cast<int>(g_docManagerStates[pThis].templates.size());
}

// Symbol: ?GetRuntimeClass@CDocManager@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDocManager__UEBAPEAUCRuntimeClass__XZ(const CDocManager* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CDocManager@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDocManager__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?OpenDocumentFile@CDocManager@@UEAAPEAVCDocument@@PEB_W@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CDocManager__UEAAPEAVCDocument__PEB_W_Z(
    CDocManager* pThis, const wchar_t* pathName) {
    if (!pThis) return nullptr;
    return OpenWithTemplates(g_docManagerStates[pThis].templates, pathName, TRUE);
}

// Symbol: ?OpenDocumentFile@CDocManager@@UEAAPEAVCDocument@@PEB_WH@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CDocManager__UEAAPEAVCDocument__PEB_WH_Z(
    CDocManager* pThis, const wchar_t* pathName, int bMakeVisible) {
    if (!pThis) return nullptr;
    return OpenWithTemplates(g_docManagerStates[pThis].templates, pathName, bMakeVisible);
}

// Symbol: ?OnFileNew@CDocManager@@UEAAXXZ
extern "C" void MS_ABI impl__OnFileNew_CDocManager__UEAAXXZ(CDocManager* pThis) {
    if (!pThis) return;
    (void)OpenWithTemplates(g_docManagerStates[pThis].templates, nullptr, TRUE);
}

// Symbol: ?OnFileOpen@CDocManager@@UEAAXXZ
extern "C" void MS_ABI impl__OnFileOpen_CDocManager__UEAAXXZ(CDocManager* pThis) {
    (void)pThis;
}

// Symbol: ?SaveAllModified@CDocManager@@UEAAHXZ
extern "C" int MS_ABI impl__SaveAllModified_CDocManager__UEAAHXZ(CDocManager* pThis) {
    (void)pThis;
    return TRUE;
}

// Symbol: ?CloseAllDocuments@CDocManager@@UEAAXH@Z
extern "C" void MS_ABI impl__CloseAllDocuments_CDocManager__UEAAXH_Z(CDocManager* pThis, int bEndSession) {
    (void)pThis;
    (void)bEndSession;
}

// Symbol: ?DoPromptFileName@CDocManager@@UEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IKHPEAVCDocTemplate@@@Z
extern "C" int MS_ABI impl__DoPromptFileName_CDocManager__UEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IKHPEAVCDocTemplate___Z(
    CDocManager* pThis, CString* fileName, unsigned int idTitle, unsigned long flags, int bOpenFileDialog, CDocTemplate* pTemplate) {
    (void)pThis;
    (void)idTitle;
    (void)flags;
    (void)bOpenFileDialog;
    (void)pTemplate;
    if (fileName) *fileName = L"";
    return FALSE;
}

// Symbol: ?OnDDECommand@CDocManager@@UEAAHPEA_W@Z
extern "C" int MS_ABI impl__OnDDECommand_CDocManager__UEAAHPEA_W_Z(CDocManager* pThis, wchar_t* command) {
    (void)pThis;
    (void)command;
    return FALSE;
}

// Symbol: ?RegisterShellFileTypes@CDocManager@@UEAAXH@Z
extern "C" void MS_ABI impl__RegisterShellFileTypes_CDocManager__UEAAXH_Z(CDocManager* pThis, int bCompat) {
    (void)pThis;
    (void)bCompat;
}

// Symbol: ?UnregisterShellFileTypes@CDocManager@@QEAAXXZ
extern "C" void MS_ABI impl__UnregisterShellFileTypes_CDocManager__QEAAXXZ(CDocManager* pThis) {
    (void)pThis;
}

// Symbol: ??0CRecentFileList@@QEAA@IPEB_W0HH@Z
extern "C" void* MS_ABI impl___0CRecentFileList__QEAA_IPEB_W0HH_Z(
    void* pThis, unsigned int start, const wchar_t* section, const wchar_t* entryFormat, int size, int maxDispLen) {
    if (pThis) {
        RecentFileListState state;
        state.start = start;
        state.maxEntries = size < 0 ? 0 : size;
        state.maxDisplayLen = maxDispLen;
        state.section = section ? section : L"";
        state.entryFormat = entryFormat ? entryFormat : L"";
        g_recentFileListStates[reinterpret_cast<CRecentFileList*>(pThis)] = std::move(state);
    }
    return pThis;
}

// Symbol: ??1CRecentFileList@@UEAA@XZ
extern "C" void MS_ABI impl___1CRecentFileList__UEAA_XZ(CRecentFileList* pThis) {
    if (pThis) g_recentFileListStates.erase(pThis);
}

// Symbol: ?Add@CRecentFileList@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__Add_CRecentFileList__UEAAXPEB_W_Z(CRecentFileList* pThis, const wchar_t* pathName) {
    if (!pThis) return;
    RecentFileListState& state = g_recentFileListStates[pThis];
    AddRecentPath(state.entries, state.maxEntries, pathName);
}

// Symbol: ?Add@CRecentFileList@@UEAAXPEB_W0@Z
extern "C" void MS_ABI impl__Add_CRecentFileList__UEAAXPEB_W0_Z(CRecentFileList* pThis, const wchar_t* pathName, const wchar_t* displayName) {
    const wchar_t* chosen = (displayName && *displayName) ? displayName : pathName;
    impl__Add_CRecentFileList__UEAAXPEB_W_Z(pThis, chosen);
}

// Symbol: ?Add@CRecentFileList@@QEAAXPEAUIShellItem@@PEB_W@Z
extern "C" void MS_ABI impl__Add_CRecentFileList__QEAAXPEAUIShellItem__PEB_W_Z(CRecentFileList* pThis, IShellItem* pItem, const wchar_t* displayName) {
    (void)pItem;
    impl__Add_CRecentFileList__UEAAXPEB_W0_Z(pThis, nullptr, displayName);
}

// Symbol: ?Add@CRecentFileList@@QEAAXPEAUIShellLinkW@@PEB_W@Z
extern "C" void MS_ABI impl__Add_CRecentFileList__QEAAXPEAUIShellLinkW__PEB_W_Z(CRecentFileList* pThis, IShellLinkW* pLink, const wchar_t* displayName) {
    (void)pLink;
    impl__Add_CRecentFileList__UEAAXPEB_W0_Z(pThis, nullptr, displayName);
}

// Symbol: ?Add@CRecentFileList@@QEAAXPEFAU_ITEMIDLIST@@PEB_W@Z
extern "C" void MS_ABI impl__Add_CRecentFileList__QEAAXPEFAU_ITEMIDLIST__PEB_W_Z(
    CRecentFileList* pThis, const ITEMIDLIST* pidl, const wchar_t* displayName) {
    (void)pidl;
    impl__Add_CRecentFileList__UEAAXPEB_W0_Z(pThis, nullptr, displayName);
}

// Symbol: ?Remove@CRecentFileList@@UEAAXH@Z
extern "C" void MS_ABI impl__Remove_CRecentFileList__UEAAXH_Z(CRecentFileList* pThis, int index) {
    if (!pThis) return;
    auto& entries = g_recentFileListStates[pThis].entries;
    if (index < 0 || static_cast<size_t>(index) >= entries.size()) return;
    entries.erase(entries.begin() + index);
}

// Symbol: ?ReadList@CRecentFileList@@UEAAXXZ
extern "C" void MS_ABI impl__ReadList_CRecentFileList__UEAAXXZ(CRecentFileList* pThis) {
    (void)pThis;
}

// Symbol: ?WriteList@CRecentFileList@@UEAAXXZ
extern "C" void MS_ABI impl__WriteList_CRecentFileList__UEAAXXZ(CRecentFileList* pThis) {
    (void)pThis;
}

// Symbol: ?UpdateMenu@CRecentFileList@@UEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__UpdateMenu_CRecentFileList__UEAAXPEAVCCmdUI___Z(CRecentFileList* pThis, CCmdUI* pCmdUI) {
    (void)pThis;
    (void)pCmdUI;
}

// Symbol: ?GetDisplayName@CRecentFileList@@UEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HPEB_WHH@Z
extern "C" int MS_ABI impl__GetDisplayName_CRecentFileList__UEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HPEB_WHH_Z(
    const CRecentFileList* pThis, CString* stringOut, int index, const wchar_t* curDir, int bAtLeastName, int bFullPath) {
    (void)curDir;
    (void)bAtLeastName;
    if (!pThis || !stringOut) return FALSE;
    const auto& entries = g_recentFileListStates[pThis].entries;
    if (index < 0 || static_cast<size_t>(index) >= entries.size()) return FALSE;
    const std::wstring& value = entries[static_cast<size_t>(index)];
    *stringOut = bFullPath ? value.c_str() : FindFileNamePart(value.c_str());
    return TRUE;
}

// Symbol: ?AddDocTemplate@CWinApp@@UEAAXPEAVCDocTemplate@@@Z
extern "C" void MS_ABI impl__AddDocTemplate_CWinApp__UEAAXPEAVCDocTemplate___Z(CWinApp* pThis, CDocTemplate* pTemplate) {
    if (!pThis || !pTemplate) return;
    auto& templates = g_appRuntimeStates[pThis].templates;
    if (std::find(templates.begin(), templates.end(), pTemplate) == templates.end()) {
        templates.push_back(pTemplate);
    }
}

// Symbol: ?GetFirstDocTemplatePosition@CWinApp@@QEBAPEAU__POSITION@@XZ
extern "C" void* MS_ABI impl__GetFirstDocTemplatePosition_CWinApp__QEBAPEAU__POSITION__XZ(const CWinApp* pThis) {
    if (!pThis) return nullptr;
    const auto& templates = g_appRuntimeStates[pThis].templates;
    return templates.empty() ? nullptr : reinterpret_cast<void*>(static_cast<uintptr_t>(1));
}

// Symbol: ?GetNextDocTemplate@CWinApp@@QEBAPEAVCDocTemplate@@AEAPEAU__POSITION@@@Z
extern "C" CDocTemplate* MS_ABI impl__GetNextDocTemplate_CWinApp__QEBAPEAVCDocTemplate__AEAPEAU__POSITION___Z(
    const CWinApp* pThis, void** pos) {
    if (!pThis || !pos || !*pos) return nullptr;
    const auto& templates = g_appRuntimeStates[pThis].templates;
    const uintptr_t index = reinterpret_cast<uintptr_t>(*pos) - 1;
    if (index >= templates.size()) {
        *pos = nullptr;
        return nullptr;
    }
    *pos = (index + 1 < templates.size()) ? reinterpret_cast<void*>(index + 2) : nullptr;
    return templates[index];
}

// Symbol: ?GetOpenDocumentCount@CWinApp@@QEAAHXZ
extern "C" int MS_ABI impl__GetOpenDocumentCount_CWinApp__QEAAHXZ(CWinApp* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?OpenDocumentFile@CWinApp@@UEAAPEAVCDocument@@PEB_W@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CWinApp__UEAAPEAVCDocument__PEB_W_Z(CWinApp* pThis, const wchar_t* pathName) {
    if (!pThis) return nullptr;
    return OpenWithTemplates(g_appRuntimeStates[pThis].templates, pathName, TRUE);
}

// Symbol: ?OpenDocumentFile@CWinApp@@UEAAPEAVCDocument@@PEB_WH@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CWinApp__UEAAPEAVCDocument__PEB_WH_Z(
    CWinApp* pThis, const wchar_t* pathName, int bMakeVisible) {
    if (!pThis) return nullptr;
    return OpenWithTemplates(g_appRuntimeStates[pThis].templates, pathName, bMakeVisible);
}

// Symbol: ?OnFileNew@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileNew_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (!pThis) return;
    (void)OpenWithTemplates(g_appRuntimeStates[pThis].templates, nullptr, TRUE);
}

// Symbol: ?OnFileOpen@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileOpen_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (!pThis || !pThis->m_lpCmdLine || pThis->m_lpCmdLine[0] == L'\0') return;
    (void)OpenWithTemplates(g_appRuntimeStates[pThis].templates, pThis->m_lpCmdLine, TRUE);
}

// Symbol: ?OnOpenRecentFile@CWinApp@@IEAAHI@Z
extern "C" int MS_ABI impl__OnOpenRecentFile_CWinApp__IEAAHI_Z(CWinApp* pThis, unsigned int index) {
    if (!pThis) return FALSE;
    const auto& entries = g_appRuntimeStates[pThis].recentEntries;
    if (index >= entries.size()) return FALSE;
    return OpenWithTemplates(g_appRuntimeStates[pThis].templates, entries[index].c_str(), TRUE) != nullptr;
}

// Symbol: ?OnUpdateRecentFileMenu@CWinApp@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecentFileMenu_CWinApp__IEAAXPEAVCCmdUI___Z(CWinApp* pThis, CCmdUI* pCmdUI) {
    (void)pThis;
    (void)pCmdUI;
}

namespace {
const AFX_MSGMAP_ENTRY g_emptyAppMessageEntries[] = {
    {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0}
};

const AFX_MSGMAP g_emptyAppMessageMap = {
    nullptr,
    g_emptyAppMessageEntries
};

DWORD WINAPI OpenMfcWinThreadEntry(LPVOID param) {
    CWinThread* thread = reinterpret_cast<CWinThread*>(param);
    if (!thread) return 0;
    g_pCurrentThread = thread;
    UINT result = 0;
    if (thread->InitInstance()) {
        result = static_cast<UINT>(thread->Run());
    }
    result = static_cast<UINT>(thread->ExitInstance());
    if (thread->m_bAutoDelete) {
        delete thread;
    }
    g_pCurrentThread = nullptr;
    return result;
}
}  // namespace

// Symbol: ?GetRuntimeClass@CWinThread@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWinThread__UEBAPEAUCRuntimeClass__XZ(const CWinThread*) {
    return &CWinThread::classCWinThread;
}

// Symbol: ?CommonConstruct@CWinThread@@QEAAXXZ
extern "C" void MS_ABI impl__CommonConstruct_CWinThread__QEAAXXZ(CWinThread* pThis) {
    if (!pThis) return;
    pThis->m_pMainWnd = nullptr;
    pThis->m_nThreadID = 0;
    pThis->m_hThread = nullptr;
    pThis->m_bAutoDelete = TRUE;
    std::memset(&pThis->m_msgCur, 0, sizeof(pThis->m_msgCur));
}

// Symbol: ?CreateThread@CWinThread@@QEAAHKIPEAU_SECURITY_ATTRIBUTES@@@Z
extern "C" int MS_ABI impl__CreateThread_CWinThread__QEAAHKIPEAU_SECURITY_ATTRIBUTES___Z(
    CWinThread* pThis, unsigned long createFlags, unsigned int stackSize, SECURITY_ATTRIBUTES* securityAttributes) {
    if (!pThis || pThis->m_hThread) return FALSE;
    DWORD threadId = 0;
    HANDLE thread = ::CreateThread(securityAttributes, stackSize, OpenMfcWinThreadEntry, pThis, createFlags, &threadId);
    if (!thread) return FALSE;
    pThis->m_hThread = thread;
    pThis->m_nThreadID = threadId;
    return TRUE;
}

// Symbol: ?Delete@CWinThread@@UEAAXXZ
extern "C" void MS_ABI impl__Delete_CWinThread__UEAAXXZ(CWinThread* pThis) {
    if (!pThis) return;
    if (pThis->m_hThread && pThis->m_hThread != ::GetCurrentThread()) {
        ::CloseHandle(pThis->m_hThread);
        pThis->m_hThread = nullptr;
    }
    if (pThis->m_bAutoDelete) delete pThis;
}

// Symbol: ?DispatchThreadMessageEx@CWinThread@@IEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__DispatchThreadMessageEx_CWinThread__IEAAHPEAUtagMSG___Z(CWinThread* pThis, MSG* msg) {
    if (!pThis || !msg) return FALSE;
    pThis->m_msgCur = *msg;
    if (pThis->PreTranslateMessage(msg)) return TRUE;
    ::TranslateMessage(msg);
    ::DispatchMessageW(msg);
    return TRUE;
}

// Symbol: ?DispatchThreadMessage@CWinThread@@IEAAXPEAUtagMSG@@@Z
extern "C" void MS_ABI impl__DispatchThreadMessage_CWinThread__IEAAXPEAUtagMSG___Z(CWinThread* pThis, MSG* msg) {
    (void)impl__DispatchThreadMessageEx_CWinThread__IEAAHPEAUtagMSG___Z(pThis, msg);
}

// Symbol: ?GetMainWnd@CWinThread@@UEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(CWinThread* pThis) {
    return pThis ? pThis->m_pMainWnd : nullptr;
}

// Symbol: ?ProcessMessageFilter@CWinThread@@UEAAHHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__ProcessMessageFilter_CWinThread__UEAAHHPEAUtagMSG___Z(CWinThread*, int, MSG*) {
    return FALSE;
}

// Symbol: ?ProcessWndProcException@CWinThread@@UEAA_JPEAVCException@@PEBUtagMSG@@@Z
extern "C" intptr_t MS_ABI impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z(
    CWinThread*, CException* exception, const MSG*) {
    if (exception) exception->GetErrorMessage(nullptr, 0, nullptr);
    return 0;
}

// Symbol: ?GetRuntimeClass@CWinApp@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWinApp__UEBAPEAUCRuntimeClass__XZ(const CWinApp*) {
    return &CWinApp::classCWinApp;
}

// Symbol: ?GetThisMessageMap@CWinApp@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWinApp__KAPEBUAFX_MSGMAP__XZ() {
    return &g_emptyAppMessageMap;
}

// Symbol: ?GetMessageMap@CWinApp@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CWinApp__MEBAPEBUAFX_MSGMAP__XZ(const CWinApp*) {
    return &g_emptyAppMessageMap;
}

// Symbol: ?AddDocTemplate@CWinApp@@QEAAXPEAVCDocTemplate@@@Z
extern "C" void MS_ABI impl__AddDocTemplate_CWinApp__QEAAXPEAVCDocTemplate___Z(CWinApp* pThis, CDocTemplate* pTemplate) {
    impl__AddDocTemplate_CWinApp__UEAAXPEAVCDocTemplate___Z(pThis, pTemplate);
}

// Symbol: ?CloseAllDocuments@CWinApp@@QEAAXH@Z
extern "C" void MS_ABI impl__CloseAllDocuments_CWinApp__QEAAXH_Z(CWinApp* pThis, int) {
    if (!pThis) return;
    for (CDocTemplate* tpl : g_appRuntimeStates[pThis].templates) CloseTemplateDocuments(tpl);
}

// Symbol: ?SaveAllModified@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__SaveAllModified_CWinApp__UEAAHXZ(CWinApp* pThis) {
    if (!pThis) return FALSE;
    for (CDocTemplate* tpl : g_appRuntimeStates[pThis].templates) {
        if (!SaveTemplateDocuments(tpl)) return FALSE;
    }
    return TRUE;
}

// Symbol: ?GetOpenDocumentCount@CWinApp@@QEAAHXZ
extern "C" int MS_ABI impl__GetOpenDocumentCount_CWinApp__QEAAHXZ(CWinApp* pThis);

// Symbol: ?CreatePrinterDC@CWinApp@@QEAAHAEAVCDC@@@Z
extern "C" int MS_ABI impl__CreatePrinterDC_CWinApp__QEAAHAEAVCDC___Z(CWinApp* pThis, CDC* dc) {
    if (!pThis || !dc) return FALSE;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    HDC hdc = ::CreateDCW(L"WINSPOOL", nullptr, nullptr, nullptr);
    if (!hdc) return FALSE;
    dc->m_hDC = hdc;
    dc->m_hAttribDC = hdc;
    (void)state;
    return TRUE;
}

// Symbol: ?DevModeChange@CWinApp@@QEAAXPEA_W@Z
extern "C" void MS_ABI impl__DevModeChange_CWinApp__QEAAXPEA_W_Z(CWinApp*, wchar_t*) {
}

// Symbol: ?DoEnableModeless@CWinApp@@SAXH@Z
extern "C" void MS_ABI impl__DoEnableModeless_CWinApp__SAXH_Z(int enable) {
    if (g_pApp) g_appRuntimeStates[g_pApp].modelessEnabled = enable != FALSE;
}

// Symbol: ?ShowAppMessageBox@CWinApp@@SAHPEAV1@PEB_WII@Z
extern "C" int MS_ABI impl__ShowAppMessageBox_CWinApp__SAHPEAV1_PEB_WII_Z(
    CWinApp* app, const wchar_t* prompt, unsigned int type, unsigned int) {
    return ::MessageBoxW(GetAppMainHwnd(app), prompt ? prompt : L"", GetAppName(app).c_str(), type);
}

// Symbol: ?DoMessageBox@CWinApp@@UEAAHPEB_WII@Z
extern "C" int MS_ABI impl__DoMessageBox_CWinApp__UEAAHPEB_WII_Z(
    CWinApp* pThis, const wchar_t* prompt, unsigned int type, unsigned int idPrompt) {
    return impl__ShowAppMessageBox_CWinApp__SAHPEAV1_PEB_WII_Z(pThis, prompt, type, idPrompt);
}

// Symbol: ?DoPrintDialog@CWinApp@@QEAA_JPEAVCPrintDialog@@@Z
extern "C" intptr_t MS_ABI impl__DoPrintDialog_CWinApp__QEAA_JPEAVCPrintDialog___Z(CWinApp*, CPrintDialog* dialog) {
    return dialog ? dialog->DoModal() : IDCANCEL;
}

// Symbol: ?DoWaitCursor@CWinApp@@UEAAXH@Z
extern "C" void MS_ABI impl__DoWaitCursor_CWinApp__UEAAXH_Z(CWinApp*, int code) {
    ::SetCursor(code > 0 ? GetWaitCursorHandle() : GetArrowCursorHandle());
}

// Symbol: ?EnableD2DSupport@CWinApp@@QEAAHW4D2D1_FACTORY_TYPE@@W4DWRITE_FACTORY_TYPE@@@Z
extern "C" int MS_ABI impl__EnableD2DSupport_CWinApp__QEAAHW4D2D1_FACTORY_TYPE__W4DWRITE_FACTORY_TYPE___Z(CWinApp* pThis, int, int) {
    if (!pThis) return FALSE;
    g_appRuntimeStates[pThis].d2dEnabled = true;
    return TRUE;
}

// Symbol: ?EnableModeless@CWinApp@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableModeless_CWinApp__QEAAXH_Z(CWinApp* pThis, int enable) {
    if (pThis) g_appRuntimeStates[pThis].modelessEnabled = enable != FALSE;
}

// Symbol: ?EnableShellOpen@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__EnableShellOpen_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (pThis) g_appRuntimeStates[pThis].shellOpenEnabled = true;
}

// Symbol: ?EnableTaskbarInteraction@CWinApp@@QEAAHH@Z
extern "C" int MS_ABI impl__EnableTaskbarInteraction_CWinApp__QEAAHH_Z(CWinApp* pThis, int enable) {
    if (!pThis) return FALSE;
    g_appRuntimeStates[pThis].taskbarEnabled = enable != FALSE;
    return TRUE;
}

// Symbol: ?GetAppRegistryKey@CWinApp@@QEAAPEAUHKEY__@@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" HKEY MS_ABI impl__GetAppRegistryKey_CWinApp__QEAAPEAUHKEY____PEAVCAtlTransactionManager_ATL___Z(CWinApp* pThis, void*) {
    if (!pThis) return nullptr;
    HKEY key = nullptr;
    return ::RegCreateKeyExW(HKEY_CURRENT_USER, GetAppRegistryRoot(pThis).c_str(), 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, nullptr) == ERROR_SUCCESS ? key : nullptr;
}

// Symbol: ?GetSectionKey@CWinApp@@QEAAPEAUHKEY__@@PEB_WPEAVCAtlTransactionManager@ATL@@@Z
extern "C" HKEY MS_ABI impl__GetSectionKey_CWinApp__QEAAPEAUHKEY____PEB_WPEAVCAtlTransactionManager_ATL___Z(CWinApp* pThis, const wchar_t* section, void*) {
    if (!pThis || !section) return nullptr;
    std::wstring path = GetAppRegistryRoot(pThis);
    if (!path.empty() && path.back() != L'\\') path.push_back(L'\\');
    path.append(section);
    HKEY key = nullptr;
    return ::RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, nullptr) == ERROR_SUCCESS ? key : nullptr;
}

// Symbol: ?GetDataRecoveryHandler@CWinApp@@UEAAPEAVCDataRecoveryHandler@@XZ
extern "C" void* MS_ABI impl__GetDataRecoveryHandler_CWinApp__UEAAPEAVCDataRecoveryHandler__XZ(CWinApp*) {
    return nullptr;
}

// Symbol: ?GetITaskbarList@CWinApp@@QEAAPEAUITaskbarList@@XZ
extern "C" ITaskbarList* MS_ABI impl__GetITaskbarList_CWinApp__QEAAPEAUITaskbarList__XZ(CWinApp* pThis) {
    if (!pThis) return nullptr;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    if (!state.taskbarEnabled) return nullptr;
    if (!state.taskbarList) {
        ::CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskbarList, reinterpret_cast<void**>(&state.taskbarList));
        if (state.taskbarList) state.taskbarList->HrInit();
    }
    return state.taskbarList;
}

// Symbol: ?GetITaskbarList3@CWinApp@@QEAAPEAUITaskbarList3@@XZ
extern "C" ITaskbarList3* MS_ABI impl__GetITaskbarList3_CWinApp__QEAAPEAUITaskbarList3__XZ(CWinApp* pThis) {
    if (!pThis) return nullptr;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    if (!state.taskbarEnabled) return nullptr;
    if (!state.taskbarList3) {
        ::CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, reinterpret_cast<void**>(&state.taskbarList3));
        if (state.taskbarList3) state.taskbarList3->HrInit();
    }
    return state.taskbarList3;
}

// Symbol: ?ReleaseTaskBarRefs@CWinApp@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseTaskBarRefs_CWinApp__QEAAXXZ(CWinApp* pThis) {
    if (!pThis) return;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    if (state.taskbarList3) state.taskbarList3->Release();
    if (state.taskbarList) state.taskbarList->Release();
    state.taskbarList3 = nullptr;
    state.taskbarList = nullptr;
}

// Symbol: ?GetPrinterDeviceDefaults@CWinApp@@QEAAHPEAUtagPDW@@@Z
extern "C" int MS_ABI impl__GetPrinterDeviceDefaults_CWinApp__QEAAHPEAUtagPDW___Z(CWinApp* pThis, PRINTDLGW* pd) {
    if (!pThis || !pd) return FALSE;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    pd->hDevMode = state.printerDevMode;
    pd->hDevNames = state.printerDevNames;
    return TRUE;
}

// Symbol: ?GetSysPolicyValue@CWinApp@@QEAAHKPEAH@Z
extern "C" int MS_ABI impl__GetSysPolicyValue_CWinApp__QEAAHKPEAH_Z(CWinApp*, unsigned long, int* value) {
    if (value) *value = 0;
    return FALSE;
}

// Symbol: ?HideApplication@CWinApp@@QEAAXXZ
extern "C" void MS_ABI impl__HideApplication_CWinApp__QEAAXXZ(CWinApp* pThis) {
    HWND hwnd = GetAppMainHwnd(pThis);
    if (hwnd) ::ShowWindow(hwnd, SW_HIDE);
}

// Symbol: ?HtmlHelpW@CWinApp@@UEAAX_KI@Z
extern "C" void MS_ABI impl__HtmlHelpW_CWinApp__UEAAX_KI_Z(CWinApp*, unsigned long long, unsigned int) {
}

// Symbol: ?InitLibId@CWinApp@@UEAAXXZ
extern "C" void MS_ABI impl__InitLibId_CWinApp__UEAAXXZ(CWinApp*) {
}

// Symbol: ?IsTaskbarInteractionEnabled@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__IsTaskbarInteractionEnabled_CWinApp__UEAAHXZ(CWinApp* pThis) {
    return pThis && g_appRuntimeStates[pThis].taskbarEnabled ? TRUE : FALSE;
}

// Symbol: ?IsWindows7@CWinApp@@QEAAHXZ
extern "C" int MS_ABI impl__IsWindows7_CWinApp__QEAAHXZ(CWinApp*) {
    return TRUE;
}

// Symbol: ?LoadAppLangResourceDLL@CWinApp@@UEAAPEAUHINSTANCE__@@XZ
extern "C" HINSTANCE MS_ABI impl__LoadAppLangResourceDLL_CWinApp__UEAAPEAUHINSTANCE____XZ(CWinApp*) {
    return AfxGetResourceHandle();
}

// Symbol: ?LoadSysPolicies@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__LoadSysPolicies_CWinApp__UEAAHXZ(CWinApp*) {
    return TRUE;
}

// Symbol: ?OnAppExit@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnAppExit_CWinApp__IEAAXXZ(CWinApp* pThis) {
    impl__CloseAllDocuments_CWinApp__QEAAXH_Z(pThis, FALSE);
    ::PostQuitMessage(0);
}

// Symbol: ?OnContextHelp@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_CWinApp__IEAAXXZ(CWinApp*) {
}

// Symbol: ?OnDDECommand@CWinApp@@UEAAHPEA_W@Z
extern "C" int MS_ABI impl__OnDDECommand_CWinApp__UEAAHPEA_W_Z(CWinApp*, wchar_t*) {
    return FALSE;
}

// Symbol: ?OnFilePrintSetup@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnFilePrintSetup_CWinApp__IEAAXXZ(CWinApp* pThis) {
    CPrintDialog dialog(TRUE);
    (void)impl__DoPrintDialog_CWinApp__QEAA_JPEAVCPrintDialog___Z(pThis, &dialog);
}

// Symbol: ?OnHelp@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnHelp_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (pThis && pThis->m_pszHelpFilePath) ::ShellExecuteW(GetAppMainHwnd(pThis), L"open", pThis->m_pszHelpFilePath, nullptr, nullptr, SW_SHOWNORMAL);
}

// Symbol: ?OnHelpFinder@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnHelpFinder_CWinApp__IEAAXXZ(CWinApp* pThis) {
    impl__OnHelp_CWinApp__IEAAXXZ(pThis);
}

// Symbol: ?OnHelpIndex@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnHelpIndex_CWinApp__IEAAXXZ(CWinApp* pThis) {
    impl__OnHelp_CWinApp__IEAAXXZ(pThis);
}

// Symbol: ?OnHelpUsing@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__OnHelpUsing_CWinApp__IEAAXXZ(CWinApp* pThis) {
    impl__OnHelp_CWinApp__IEAAXXZ(pThis);
}

// Symbol: ?OnIdle@CWinApp@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnIdle_CWinApp__UEAAHJ_Z(CWinApp* pThis, long count) {
    return pThis ? pThis->CWinThread::OnIdle(count) : FALSE;
}

// Symbol: ?ProcessWndProcException@CWinApp@@UEAA_JPEAVCException@@PEBUtagMSG@@@Z
extern "C" intptr_t MS_ABI impl__ProcessWndProcException_CWinApp__UEAA_JPEAVCException__PEBUtagMSG___Z(
    CWinApp*, CException* exception, const MSG*) {
    if (exception) {
        wchar_t buffer[512] = {};
        exception->GetErrorMessage(buffer, 512, nullptr);
        if (buffer[0] != L'\0') ::MessageBoxW(nullptr, buffer, L"OpenMFC", MB_OK | MB_ICONERROR);
    }
    return 0;
}

// Symbol: ?Register@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__Register_CWinApp__UEAAHXZ(CWinApp*) {
    return TRUE;
}

// Symbol: ?RegisterShellFileTypes@CWinApp@@IEAAXH@Z
extern "C" void MS_ABI impl__RegisterShellFileTypes_CWinApp__IEAAXH_Z(CWinApp* pThis, int) {
    if (pThis) g_appRuntimeStates[pThis].shellOpenEnabled = true;
}

// Symbol: ?RegisterWithRestartManager@CWinApp@@UEAAJPEB_WKP6AKPEAX@Z1KK@Z
extern "C" long MS_ABI impl__RegisterWithRestartManager_CWinApp__UEAAJPEB_WKP6AKPEAX_Z1KK_Z(
    CWinApp*, const wchar_t* restartIdentifier, unsigned long flags, DWORD (WINAPI*)(void*), void*, unsigned long, unsigned long) {
    return ::RegisterApplicationRestart(restartIdentifier ? restartIdentifier : L"", flags);
}

// Symbol: ?RestartInstance@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__RestartInstance_CWinApp__UEAAHXZ(CWinApp*) {
    return FALSE;
}

// Symbol: ?RunAutomated@CWinApp@@QEAAHXZ
extern "C" int MS_ABI impl__RunAutomated_CWinApp__QEAAHXZ(CWinApp* pThis) {
    if (!pThis) return FALSE;
    std::wstring cmd = LowerWide(WideValue(pThis->m_lpCmdLine));
    bool automated = cmd.find(L"/automation") != std::wstring::npos || cmd.find(L"-automation") != std::wstring::npos;
    g_appRuntimeStates[pThis].automated = automated;
    return automated ? TRUE : FALSE;
}

// Symbol: ?RunEmbedded@CWinApp@@QEAAHXZ
extern "C" int MS_ABI impl__RunEmbedded_CWinApp__QEAAHXZ(CWinApp* pThis) {
    if (!pThis) return FALSE;
    std::wstring cmd = LowerWide(WideValue(pThis->m_lpCmdLine));
    bool embedded = cmd.find(L"/embedding") != std::wstring::npos || cmd.find(L"-embedding") != std::wstring::npos;
    g_appRuntimeStates[pThis].embedded = embedded;
    return embedded ? TRUE : FALSE;
}

// Symbol: ?SaveStdProfileSettings@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__SaveStdProfileSettings_CWinApp__IEAAXXZ(CWinApp*) {
}

// Symbol: ?SelectPrinter@CWinApp@@QEAAXPEAX0H@Z
extern "C" void MS_ABI impl__SelectPrinter_CWinApp__QEAAXPEAX0H_Z(CWinApp* pThis, void* devNames, void* devMode, int) {
    if (!pThis) return;
    AppRuntimeState& state = g_appRuntimeStates[pThis];
    state.printerDevNames = static_cast<HGLOBAL>(devNames);
    state.printerDevMode = static_cast<HGLOBAL>(devMode);
}

// Symbol: ?SetAppID@CWinApp@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetAppID_CWinApp__IEAAXPEB_W_Z(CWinApp* pThis, const wchar_t* appId) {
    if (pThis) g_appRuntimeStates[pThis].appId = WideValue(appId);
}

// Symbol: ?SetCurrentHandles@CWinApp@@QEAAXXZ
extern "C" void MS_ABI impl__SetCurrentHandles_CWinApp__QEAAXXZ(CWinApp* pThis) {
    if (!pThis) return;
    pThis->m_hInstance = AfxGetInstanceHandle();
    pThis->m_nThreadID = ::GetCurrentThreadId();
    pThis->m_hThread = ::GetCurrentThread();
}

// Symbol: ?SetRegistryKey@CWinApp@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetRegistryKey_CWinApp__IEAAXPEB_W_Z(CWinApp* pThis, const wchar_t* key) {
    if (pThis) g_appRuntimeStates[pThis].registryRoot = key && *key ? std::wstring(L"Software\\") + key : L"Software\\OpenMFC";
}

// Symbol: ?SetRegistryKey@CWinApp@@IEAAXI@Z
extern "C" void MS_ABI impl__SetRegistryKey_CWinApp__IEAAXI_Z(CWinApp* pThis, unsigned int id) {
    if (!pThis) return;
    wchar_t buffer[64] = {};
    std::swprintf(buffer, 64, L"OpenMFC\\%u", id);
    g_appRuntimeStates[pThis].registryRoot = std::wstring(L"Software\\") + buffer;
}

// Symbol: ?Unregister@CWinApp@@UEAAHXZ
extern "C" int MS_ABI impl__Unregister_CWinApp__UEAAHXZ(CWinApp*) {
    return TRUE;
}

// Symbol: ?UnregisterShellFileTypes@CWinApp@@IEAAXXZ
extern "C" void MS_ABI impl__UnregisterShellFileTypes_CWinApp__IEAAXXZ(CWinApp* pThis) {
    if (pThis) g_appRuntimeStates[pThis].shellOpenEnabled = false;
}

// Symbol: ?UpdatePrinterSelection@CWinApp@@IEAAXH@Z
extern "C" void MS_ABI impl__UpdatePrinterSelection_CWinApp__IEAAXH_Z(CWinApp*, int) {
}

// Symbol: ?WinHelpInternal@CWinApp@@UEAAX_KI@Z
extern "C" void MS_ABI impl__WinHelpInternal_CWinApp__UEAAX_KI_Z(CWinApp* pThis, unsigned long long data, unsigned int command) {
    if (pThis && pThis->m_pszHelpFilePath) ::WinHelpW(GetAppMainHwnd(pThis), pThis->m_pszHelpFilePath, command, static_cast<ULONG_PTR>(data));
}

// Symbol: ?WinHelpW@CWinApp@@UEAAX_KI@Z
extern "C" void MS_ABI impl__WinHelpW_CWinApp__UEAAX_KI_Z(CWinApp* pThis, unsigned long long data, unsigned int command) {
    impl__WinHelpInternal_CWinApp__UEAAX_KI_Z(pThis, data, command);
}

// Symbol: ?ApplicationRecoveryCallback@CWinApp@@UEAAKPEAX@Z
extern "C" unsigned long MS_ABI impl__ApplicationRecoveryCallback_CWinApp__UEAAKPEAX_Z(CWinApp*, void*) {
    return 0;
}
