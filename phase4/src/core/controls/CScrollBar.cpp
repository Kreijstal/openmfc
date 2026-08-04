// CScrollBar — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/CtrlcoreSupport.h"

// Symbol: ?GetRuntimeClass@CScrollBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CScrollBar__UEBAPEAUCRuntimeClass__XZ(
    const CScrollBar* pThis) {
    return CScrollBar::GetThisClass();
}
// Symbol: ?GetThisClass@CScrollBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CScrollBar__SAPEAUCRuntimeClass__XZ() {
    return CScrollBar::GetThisClass();
}
