// CWinThread — OpenMFC implementation.
// Sources: appcore.cpp, ole_olectors_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ?GetThisClass@CWinThread@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinThread__SAPEAUCRuntimeClass__XZ() {
    return &CWinThread::classCWinThread;
}
// CWinThread::Run - exported lifecycle implementation
// Symbol: ?Run@CWinThread@@UEAAHXZ
// Ordinal: 12614
extern "C" int MS_ABI impl__Run_CWinThread__UEAAHXZ(CWinThread* pThis) {
    return OpenMfcRunExportedMessageLoop(pThis);
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
extern "C" int MS_ABI impl__ProcessMessageFilter_CWinThread__UEAAHHPEAUtagMSG___Z(CWinThread* pThis, int, MSG* msg) {
    return (pThis && msg && pThis->PreTranslateMessage(msg)) ? TRUE : FALSE;
}
// Symbol: ?ProcessWndProcException@CWinThread@@UEAA_JPEAVCException@@PEBUtagMSG@@@Z
extern "C" intptr_t MS_ABI impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z(
    CWinThread*, CException* exception, const MSG*) {
    if (exception) exception->GetErrorMessage(nullptr, 0, nullptr);
    return 0;
}
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
// Symbol: ??0CWinThread@@QEAA@P6AIPEAX@Z0@Z
// CWinThread::CWinThread()
extern "C" void* MS_ABI impl___0CWinThread__QEAA_P6AIPEAX_Z0_Z(void* p0, void* p1) {
    (void)p0;
    (void)p1;
    return new CWinThread();
}
