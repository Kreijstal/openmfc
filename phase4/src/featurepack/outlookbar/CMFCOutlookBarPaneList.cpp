// CMFCOutlookBarPaneList — OpenMFC implementation.
// Sources: global_cmfcoutlookbarpanelist.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCOutlookBarPaneListSupport.h"
#include "detail/Mfc05MsgmapSupport.h"

// CMFCOutlookBarPaneList inherits CMFCOutlookBarPane without declaring its own
// message map, so both accessors use the base class's static map.
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCOutlookBarPane__KAPEBUAFX_MSGMAP__XZ();

// Symbol: ?GetMessageMap@CMFCOutlookBarPaneList@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CMFCOutlookBarPaneList__MEBAPEBUAFX_MSGMAP__XZ(
    void* pThis) {
    (void)pThis;
    return impl__GetThisMessageMap_CMFCOutlookBarPane__KAPEBUAFX_MSGMAP__XZ();
}
// Symbol: ?GetThisMessageMap@CMFCOutlookBarPaneList@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCOutlookBarPaneList__KAPEBUAFX_MSGMAP__XZ() {
    return impl__GetThisMessageMap_CMFCOutlookBarPane__KAPEBUAFX_MSGMAP__XZ();
}
// Symbol: ?EnsureVisible@CMFCOutlookBarPaneList@@AEAAXH@Z
// Pre-existing stub retained: retail has a geometry-dependent scrolling path;
// this body only checks null and reads the shadow parent slot, so it does not scroll.
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
// Pre-existing approximation retained: retail runs a multi-call command path;
// this body only rejects nulls and returns success without that dispatch.
extern "C" int MS_ABI impl__OnSendCommand_CMFCOutlookBarPaneList__EEAAHPEBVCMFCToolBarButton___Z(
    void* pThis, const void* pButton) {
    if (pThis == nullptr || pButton == nullptr) {
        return 0;
    }
    return 1;
}
