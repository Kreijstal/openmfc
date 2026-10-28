// CWinThread — OpenMFC implementation.
// Sources: appcore.cpp, ole_olectors_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"
#include "openmfc/afxstat_.h"

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
// Symbol: ?GetRuntimeClass@CWinThread@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWinThread__UEBAPEAUCRuntimeClass__XZ(const CWinThread*) {
    return &CWinThread::classCWinThread;
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
// Symbol: ?DispatchThreadMessageEx@CWinThread@@IEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__DispatchThreadMessageEx_CWinThread__IEAAHPEAUtagMSG___Z(CWinThread* pThis, MSG* msg) {
    if (!pThis || !msg) return FALSE;
    AfxGetThreadState()->m_msgCur = *msg;
    if (pThis->PreTranslateMessage(msg)) return TRUE;
    ::TranslateMessage(msg);
    ::DispatchMessageW(msg);
    return TRUE;
}
// Symbol: ?DispatchThreadMessage@CWinThread@@IEAAXPEAUtagMSG@@@Z
extern "C" void MS_ABI impl__DispatchThreadMessage_CWinThread__IEAAXPEAUtagMSG___Z(CWinThread* pThis, MSG* msg) {
    (void)impl__DispatchThreadMessageEx_CWinThread__IEAAHPEAUtagMSG___Z(pThis, msg);
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

BOOL CWinThread::PrePumpMessage() {
    return TRUE;
}
BOOL CWinThread::PostPumpMessage() {
    return TRUE;
}
