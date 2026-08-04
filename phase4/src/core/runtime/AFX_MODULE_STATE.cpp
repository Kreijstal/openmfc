// AFX_MODULE_STATE — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0AFX_MODULE_STATE@@QEAA@HP6A_JPEAUHWND__@@I_K_J@ZKH@Z
extern "C" void* MS_ABI impl___0AFX_MODULE_STATE__QEAA_HP6A_JPEAUHWND____I_K_J_ZKH_Z(
    void* pThis, void*, void*, int, unsigned long long, int, unsigned long long) {
    auto* pState = static_cast<LocalAFX_MODULE_STATE*>(pThis);
    if (pState == nullptr) {
        return pThis;
    }

    if (const auto* current = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()) {
        pState->m_hCurrentInstanceHandle = current->m_hCurrentInstanceHandle;
        pState->m_hCurrentResourceHandle = current->m_hCurrentResourceHandle;
        pState->m_pCurrentWinApp = current->m_pCurrentWinApp;
    } else {
        pState->m_hCurrentInstanceHandle = nullptr;
        pState->m_hCurrentResourceHandle = nullptr;
        pState->m_pCurrentWinApp = nullptr;
    }
    return pThis;
}
// Symbol: ??1AFX_MODULE_STATE@@UEAA@XZ
extern "C" void MS_ABI impl___1AFX_MODULE_STATE__UEAA_XZ(void* pThis) {
    auto* pState = static_cast<LocalAFX_MODULE_STATE*>(pThis);
    if (pState == nullptr) {
        return;
    }
    pState->m_hCurrentInstanceHandle = nullptr;
    pState->m_hCurrentResourceHandle = nullptr;
    pState->m_pCurrentWinApp = nullptr;
}
