// CMFCColorBarCmdUI — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CMFCColorBarCmdUI@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCColorBarCmdUI__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return pThis;
    }
    return new (pThis) CCmdUI();
}
// Symbol: ?Enable@CMFCColorBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CMFCColorBarCmdUI__UEAAXH_Z(void* pThis, int bOn) {
    if (auto* pUI = static_cast<CCmdUI*>(pThis)) {
        pUI->Enable(bOn != 0);
    }
}
