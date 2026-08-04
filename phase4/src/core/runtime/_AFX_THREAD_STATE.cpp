// _AFX_THREAD_STATE — OpenMFC implementation.
// Sources: appcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"

// Symbol: ??0_AFX_THREAD_STATE@@QEAA@XZ
extern "C" _AFX_THREAD_STATE* MS_ABI impl___0_AFX_THREAD_STATE__QEAA_XZ(_AFX_THREAD_STATE* pThis) {
    if (!pThis) return nullptr;
    return new(pThis) _AFX_THREAD_STATE();
}
// Symbol: ??1_AFX_THREAD_STATE@@UEAA@XZ
extern "C" void MS_ABI impl___1_AFX_THREAD_STATE__UEAA_XZ(_AFX_THREAD_STATE* pThis) {
    if (pThis) pThis->~_AFX_THREAD_STATE();
}
