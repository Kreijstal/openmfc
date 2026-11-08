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
