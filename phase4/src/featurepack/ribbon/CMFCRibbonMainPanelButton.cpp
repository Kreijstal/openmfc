// CMFCRibbonMainPanelButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonMainPanel__SAPEAUCRuntimeClass__XZ();
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
extern "C" void MS_ABI impl__OnDrawBorder_CMFCRibbonButton__UEAAXPEAVCDC___Z(
    CMFCRibbonButton* pThis, CDC* pDC);
extern "C" unsigned long MS_ABI impl__OnFillBackground_CMFCRibbonButton__UEAAKPEAVCDC___Z(
    CMFCRibbonButton* pThis, CDC* pDC);
extern "C" void MS_ABI impl__OnDrawRibbonMainPanelButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonButton* pButton);
extern "C" unsigned long MS_ABI impl__OnFillRibbonMainPanelButton_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonButton* pButton);

namespace {

// The runtime-class descriptor in RuntimeClasses.cpp and retail CreateObject
// both give this object a size of 0x270. The only instance field these methods
// read directly is the vptr at +0; the rest of the object is opaque here.
struct MainPanelButtonShadow {
    void* m_vftable;
    unsigned char m_rest[0x268];
};
static_assert(offsetof(MainPanelButtonShadow, m_vftable) == 0x0,
              "CMFCRibbonMainPanelButton vptr is at +0");
static_assert(sizeof(MainPanelButtonShadow) == 0x270,
              "CMFCRibbonMainPanelButton retail size is 0x270");
static_assert(sizeof(void*) == 8, "the retail ribbon vtable is x64");

// Retail obtains GetParentPanel from the button vtable at byte offset 0x270.
// Keep that one slot explicit because CMFCRibbonMainPanelButton is not modeled
// by the public OpenMFC declarations. The retail visual-manager slots are not
// used here: OpenMFC's CMFCVisualManager declaration omits OnFillRibbonMainPanelButton,
// so its local vtable cannot be indexed using the retail +0x4a0/+0x4a8 offsets.
struct MainPanelButtonVtableSlots {
    void* preceding[0x270 / sizeof(void*)];
    void* getParentPanel;
};
static_assert(offsetof(MainPanelButtonVtableSlots, getParentPanel) == 0x270,
              "GetParentPanel virtual slot is +0x270");

template <typename Function>
Function RibbonVirtual(void* pObject, std::size_t byteOffset) {
    void** vtable = *reinterpret_cast<void***>(pObject);
    return reinterpret_cast<Function>(vtable[byteOffset / sizeof(void*)]);
}

void* GetParentPanel(void* pThis) {
    using Function = void* (MS_ABI *)(const void*);
    return RibbonVirtual<Function>(pThis, 0x270)(pThis);
}

bool IsMainPanel(void* pThis) {
    void* pPanel = GetParentPanel(pThis);
    return pPanel != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(static_cast<CMFCRibbonPanel*>(pPanel)),
            impl__GetThisClass_CMFCRibbonMainPanel__SAPEAUCRuntimeClass__XZ()) != 0;
}

} // namespace

// Symbol: ??0CMFCRibbonMainPanelButton@@QEAA@IPEB_WH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonMainPanelButton__QEAA_IPEB_WH_Z(void* pThis, int id, const wchar_t* text) {
    (void)id;
    (void)text;
    return pThis;
}
// Symbol: ??0CMFCRibbonMainPanelButton@@QEAA@IPEB_WPEAUHICON__@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonMainPanelButton__QEAA_IPEB_WPEAUHICON_____Z(
    void* pThis, int id, const wchar_t* text, void* icon) {
    (void)id;
    (void)text;
    (void)icon;
    return pThis;
}

// Symbol: ??1CMFCRibbonMainPanelButton@@UEAA@XZ
extern "C" void* MS_ABI impl___1CMFCRibbonMainPanelButton__UEAA_XZ(void* pThis) {
    return pThis;
}

// Retail mfc140u RVA 0x110bf0 allocates 0x270 bytes and calls the default
// constructor at RVA 0x110c30 when allocation succeeds. The existing OpenMFC
// default-constructor thunk in this file only returns pThis, and this class has
// no ABI-complete declaration/vtable in include/openmfc; returning that raw
// allocation would not produce a constructed object. Leave the factory stubbed
// until that object layout/constructor support exists.
// Symbol: ?CreateObject@CMFCRibbonMainPanelButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonMainPanelButton__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail mfc140u RVA 0x110dc0 obtains GetParentPanel through vtable +0x270,
// checks that panel with CObject::IsKindOf(CMFCRibbonMainPanel), and on that
// path draws via visual-manager slot +0x4a8. Otherwise it calls the
// CMFCRibbonButton base implementation (RVA 0xe8720, mfc140u). The OpenMFC
// CMFCVisualManager declaration omits OnFillRibbonMainPanelButton, so its local
// vtable cannot safely use retail's +0x4a8 slot. Use the available visual-
// manager thunk instead; the thunk dispatches the declared local
// OnDrawRibbonMainPanelButtonBorder method rather than a retail-layout slot.
// OpenMFC's class declarations do not wire the specialized visual-manager
// overrides into this local vtable, so subclass-specific retail dispatch is
// not reproduced. This reads OpenMFC's manager mirror (not retail's lazy
// GetInstance helper), so a null mirror falls through to the base thunk.
// That existing OpenMFC base thunk is an approximation: retail's base entry
// at RVA 0xe8720 (mfc140u) saves/clears/restores the state word at +0x148 and
// calls visual-manager slot +0x490; the OpenMFC thunk instead calls its locally
// declared OnDrawRibbonButtonBorder path without those state transitions.
// Symbol: ?OnDrawBorder@CMFCRibbonMainPanelButton@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCRibbonMainPanelButton__MEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    if (IsMainPanel(pThis)) {
        auto* pManager = static_cast<CMFCVisualManager*>(
            impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
        if (pManager != nullptr) {
            impl__OnDrawRibbonMainPanelButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
                pManager, static_cast<CDC*>(pDC), static_cast<CMFCRibbonButton*>(pThis));
            return;
        }
    }
    impl__OnDrawBorder_CMFCRibbonButton__UEAAXPEAVCDC___Z(
        static_cast<CMFCRibbonButton*>(pThis), static_cast<CDC*>(pDC));
}

// Retail ANSI twin RVA 0x111ad0 (mfc140.dll) has the body corresponding to
// this export; the --u map does not resolve it. It follows the same panel
// check as OnDrawBorder and calls visual-manager slot +0x4a0 on that path,
// otherwise calling CMFCRibbonButton::OnFillBackground (RVA 0xe92a0,
// mfc140.dll). Since OpenMFC's local vtable omits OnFillRibbonMainPanelButton,
// call its available base thunk instead of using retail's +0x4a0 slot; that
// does not dispatch a visual-manager subclass override. This uses OpenMFC's
// manager mirror rather than retail's lazy GetInstance helper, so a null mirror
// falls through to the button base-color thunk. That existing OpenMFC base
// thunk simply returns FeaturePackBackgroundColor; retail's base entry at RVA
// 0xe92a0 (mfc140.dll) saves/clears/restores the state word at +0x148 and calls
// visual-manager slot +0x480.
// Symbol: ?OnFillBackground@CMFCRibbonMainPanelButton@@MEAAKPEAVCDC@@@Z
extern "C" unsigned long MS_ABI impl__OnFillBackground_CMFCRibbonMainPanelButton__MEAAKPEAVCDC___Z(
    void* pThis, void* pDC) {
    if (IsMainPanel(pThis)) {
        auto* pManager = static_cast<CMFCVisualManager*>(
            impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
        if (pManager != nullptr) {
            return impl__OnFillRibbonMainPanelButton_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
                pManager, static_cast<CDC*>(pDC), static_cast<CMFCRibbonButton*>(pThis));
        }
    }
    return impl__OnFillBackground_CMFCRibbonButton__UEAAKPEAVCDC___Z(
        static_cast<CMFCRibbonButton*>(pThis), static_cast<CDC*>(pDC));
}

// Symbol: ??0CMFCRibbonMainPanelButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonMainPanelButton__QEAA_XZ(void* pThis) {
    return pThis;
}
