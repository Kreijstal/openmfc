// AFX_MAINTAIN_STATE — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0AFX_MAINTAIN_STATE@@QEAA@PEAVAFX_MODULE_STATE@@@Z
extern "C" void* MS_ABI impl___0AFX_MAINTAIN_STATE__QEAA_PEAVAFX_MODULE_STATE___Z(void* pThis, void* pState) {
    return enter_maintain_state(pThis, static_cast<LocalAFX_MODULE_STATE*>(pState), false);
}
// Symbol: ??1AFX_MAINTAIN_STATE@@UEAA@XZ
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE__UEAA_XZ(void* pThis) {
    leave_maintain_state(pThis);
}
// Symbol: ??1AFX_MAINTAIN_STATE@@QEAA@XZ
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE__QEAA_XZ(void* pThis) {
    leave_maintain_state(pThis);
}
