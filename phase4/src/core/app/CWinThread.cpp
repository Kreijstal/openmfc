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
BOOL CWinThread::PrePumpMessage() {
    return TRUE;
}
BOOL CWinThread::PostPumpMessage() {
    return TRUE;
}
