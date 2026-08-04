// AFX_MAINTAIN_STATE2 — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0AFX_MAINTAIN_STATE2@@QEAA@PEAVAFX_MODULE_STATE@@@Z
extern "C" void* MS_ABI impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(void* pThis, void* pState) {
    return enter_maintain_state(pThis, static_cast<LocalAFX_MODULE_STATE*>(pState), true);
}
// Symbol: ??1AFX_MAINTAIN_STATE2@@UEAA@XZ
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE2__UEAA_XZ(void* pThis) {
    leave_maintain_state(pThis);
}
// Symbol: ??1AFX_MAINTAIN_STATE2@@QEAA@XZ
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(void* pThis) {
    leave_maintain_state(pThis);
}
