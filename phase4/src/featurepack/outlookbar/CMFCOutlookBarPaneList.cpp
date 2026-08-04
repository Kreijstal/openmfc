// CMFCOutlookBarPaneList — OpenMFC implementation.
// Sources: global_cmfcoutlookbarpanelist.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCOutlookBarPaneListSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?OnSendCommand@CMFCOutlookBarPaneList@@EEAAHPEBVCMFCToolBarButton@@@Z
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?EnsureVisible@CMFCOutlookBarPaneList@@AEAAXH@Z
// Symbol: ?GetMessageMap@CMFCOutlookBarPaneList@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" void* MS_ABI impl__GetMessageMap_CMFCOutlookBarPaneList__MEBAPEBUAFX_MSGMAP__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisMessageMap@CMFCOutlookBarPaneList@@KAPEBUAFX_MSGMAP@@XZ
extern "C" void* MS_ABI impl__GetThisMessageMap_CMFCOutlookBarPaneList__KAPEBUAFX_MSGMAP__XZ() {
    return nullptr;
}
// Symbol: ?EnsureVisible@CMFCOutlookBarPaneList@@AEAAXH@Z
extern "C" void MS_ABI impl__EnsureVisible_CMFCOutlookBarPaneList__AEAAXH_Z(
    void* pThis, int bVisible) {
    struct CMFCOutlookBarPaneListLayout {
        void* vfptr;
        char _opaque[11120 - 8 - 8];
        void* m_pParent;
    };

    const auto* self = static_cast<const CMFCOutlookBarPaneListLayout*>(pThis);
    if (self == nullptr) {
        return;
    }

    (void)bVisible;
    (void)self->m_pParent;
}
// Symbol: ?OnSendCommand@CMFCOutlookBarPaneList@@EEAAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSendCommand_CMFCOutlookBarPaneList__EEAAHPEBVCMFCToolBarButton___Z(
    void* pThis, const void* pButton) {
    if (pThis == nullptr || pButton == nullptr) {
        return 0;
    }
    return 1;
}
