// AFX_MODULE_THREAD_STATE — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0AFX_MODULE_THREAD_STATE@@QEAA@XZ
extern "C" void* MS_ABI impl___0AFX_MODULE_THREAD_STATE__QEAA_XZ(void* pThis) {
    auto* pThreadState = static_cast<AFX_MODULE_THREAD_STATE_ManualSmallStubImplementations*>(pThis);
    if (pThreadState == nullptr) {
        return pThis;
    }
    if (auto* current = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()) {
        pThreadState->m_nTempMapLock = current->m_nTempMapLock;
    } else {
        pThreadState->m_nTempMapLock = 0;
    }
    return pThis;
}
// Symbol: ??1AFX_MODULE_THREAD_STATE@@UEAA@XZ
extern "C" void MS_ABI impl___1AFX_MODULE_THREAD_STATE__UEAA_XZ(void* pThis) {
    auto* pThreadState = static_cast<AFX_MODULE_THREAD_STATE_ManualSmallStubImplementations*>(pThis);
    if (pThreadState == nullptr) {
        return;
    }
    pThreadState->m_nTempMapLock = 0;
}
