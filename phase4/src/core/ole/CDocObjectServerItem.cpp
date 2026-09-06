// CDocObjectServerItem — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CDocObjectServerItem@@IEAA@PEAVCOleServerDoc@@H@Z
extern "C" void* MS_ABI impl___0CDocObjectServerItem__IEAA_PEAVCOleServerDoc__H_Z(
    void* pThis, void* pDoc, int unusedId) {
    (void)pDoc;
    (void)unusedId;
    return pThis;
}
// Symbol: ??1CDocObjectServerItem@@UEAA@XZ
extern "C" void MS_ABI impl___1CDocObjectServerItem__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        ClearCDocObjectServerItemState(pThis);
    }
}
// Symbol: ?OnDoVerb@CDocObjectServerItem@@MEAAXJ@Z
extern "C" void MS_ABI impl__OnDoVerb_CDocObjectServerItem__MEAAXJ_Z(void* pThis, int nVerb) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCDocObjectServerItemState(pThis);
    state.m_lastVerb = nVerb;
    state.m_visible = (nVerb != -7);
}
// Symbol: ?OnHide@CDocObjectServerItem@@MEAAXXZ
extern "C" void MS_ABI impl__OnHide_CDocObjectServerItem__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCDocObjectServerItemState(pThis);
    state.m_visible = false;
}
// Symbol: ?OnShow@CDocObjectServerItem@@MEAAXXZ
extern "C" void MS_ABI impl__OnShow_CDocObjectServerItem__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCDocObjectServerItemState(pThis);
    state.m_visible = true;
}
