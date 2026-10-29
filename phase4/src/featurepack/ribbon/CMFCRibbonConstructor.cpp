// CMFCRibbonConstructor — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

namespace {
struct RibbonConstructorXIDView {
    CString m_Name;
    unsigned int m_Value;
    unsigned int _pad_0c;
};
static_assert(offsetof(RibbonConstructorXIDView, m_Value) == 0x08,
              "CMFCRibbonInfo::XID::m_Value offset");
static_assert(sizeof(RibbonConstructorXIDView) == 0x10,
              "CMFCRibbonInfo::XID size");

struct RibbonConstructorView {
    void* m_vfptr;
    const void* m_Info;
};
static_assert(offsetof(RibbonConstructorView, m_Info) == 0x08,
              "CMFCRibbonConstructor::m_Info offset");
static_assert(sizeof(RibbonConstructorView) == 0x10,
              "CMFCRibbonConstructor modeled prefix size");
}

extern "C" void MS_ABI impl__SetID_CMFCRibbonBaseElement__UEAAXI_Z(
    CMFCRibbonBaseElement* pElement, unsigned int id);

// Retail ctor writes m_Info at +0x08 and installs its own vfptr (RVA 0xf80a0, mfc140u).
// OpenMFC has no relocatable constructor vftable, so keep the reference and clear the unusable vfptr.
// Symbol: ??0CMFCRibbonConstructor@@QEAA@AEBVCMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonConstructor__QEAA_AEBVCMFCRibbonInfo___Z(
    void* pThis, const void* info) {
    auto* layout = static_cast<RibbonConstructorView*>(pThis);
    layout->m_vfptr = nullptr;
    layout->m_Info = info;
    return pThis;
}

// Stub: RVA 0xf80f0 (mfc140u) only rewrites +0 with the class retail vfptr; OpenMFC declares no CMFCRibbonConstructor type or relocatable vtable.
// Symbol: ??1CMFCRibbonConstructor@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonConstructor__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Stub: RVA 0xf8c50 (mfc140u) uses several target virtual slots and writes target offsets +0x164, +0x1b8/+0x1bc and +0x1ec. RI_XElement is pinned privately in CMFCRibbonInfo.cpp but not shared with this TU; the destination's required retail fields/slots are absent from the compact element model.
// Symbol: ?ConstructBaseElement@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBaseElement@@AEBVXElement@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructBaseElement_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBaseElement__AEBVXElement_CMFCRibbonInfo___Z(
    void* pThis, void* pBase, const void* pInfo) {
    (void)pThis;
    (void)pBase;
    (void)pInfo;
}

// Stub: RVA 0xf8610 (mfc140u) calls a helper on two input/target subobjects and walks category child arrays through virtual calls. RI_XCategory is pinned privately in CMFCRibbonInfo.cpp but not shared here, and the destination retail offsets/slots are absent from the compact category model.
// Symbol: ?ConstructCategory@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonCategory@@AEBVXCategory@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructCategory_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonCategory__AEBVXCategory_CMFCRibbonInfo___Z(
    void* pThis, void* pCategory, const void* pInfo) {
    (void)pThis;
    (void)pCategory;
    (void)pInfo;
}

// Stub: RVA 0xf8500 (mfc140u) walks category-main elements and invokes main-panel virtuals. RI_XCategoryMain is pinned privately in CMFCRibbonInfo.cpp but not shared here, and the required destination main-panel state/slots are absent from the compact model.
// Symbol: ?ConstructCategoryMain@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBar@@AEBVXCategoryMain@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructCategoryMain_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBar__AEBVXCategoryMain_CMFCRibbonInfo___Z(
    void* pThis, void* pBar, const void* pInfo) {
    (void)pThis;
    (void)pBar;
    (void)pInfo;
}

// Stub: RVA 0xf8a60 (mfc140u) branches on XElement type and populates type-specific elements using retail virtual slots and member offsets absent from the OpenMFC element models.
// Symbol: ?ConstructElement@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBaseElement@@AEBVXElement@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructElement_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBaseElement__AEBVXElement_CMFCRibbonInfo___Z(
    void* pThis, void* pElement, const void* pInfo) {
    (void)pThis;
    (void)pElement;
    (void)pInfo;
}

// Stub: RVA 0xf8750 (mfc140u) copies panel state and iterates XPanel elements, dispatching through panel and element virtual slots. RI_XPanel is pinned privately in CMFCRibbonInfo.cpp but not shared here; the compact destination panel model lacks the required retail offsets/slots.
// Symbol: ?ConstructPanel@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonPanel@@AEBVXPanel@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructPanel_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonPanel__AEBVXPanel_CMFCRibbonInfo___Z(
    void* pThis, void* pPanel, const void* pInfo) {
    (void)pThis;
    (void)pPanel;
    (void)pInfo;
}

// Stub: RVA 0xf8880 (mfc140u) builds temporary lookup data from the input, calls private helpers and copies info +0x220 to bar +0x434. RI_XRibbonBar/RI_XQAT are pinned privately in CMFCRibbonInfo.cpp but not shared here; the matching destination QAT state is not modeled by the compact bar API.
// Symbol: ?ConstructQATElements@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBar@@AEBVXRibbonBar@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructQATElements_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBar__AEBVXRibbonBar_CMFCRibbonInfo___Z(
    void* pThis, void* pBar, const void* pInfo) {
    (void)pThis;
    (void)pBar;
    (void)pInfo;
}

// Stub: RVA 0xf82d0 (mfc140u) performs a long sequence of ribbon-bar field writes, global-state updates and virtual calls; the needed retail state/layouts are not modeled in this file.
// Symbol: ?ConstructRibbonBar@CMFCRibbonConstructor@@UEBAXAEAVCMFCRibbonBar@@@Z
extern "C" void MS_ABI impl__ConstructRibbonBar_CMFCRibbonConstructor__UEBAXAEAVCMFCRibbonBar___Z(
    void* pThis, void* pBar) {
    (void)pThis;
    (void)pBar;
}

// Stub: RVA 0xf8990 (mfc140u) iterates tab-element arrays and dispatches through constructor/bar virtual slots. RI_XRibbonBar's array offsets are pinned privately in CMFCRibbonInfo.cpp but not shared here; the destination insertion state/slots are absent from the compact bar model.
// Symbol: ?ConstructTabElements@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBar@@AEBVXRibbonBar@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__ConstructTabElements_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBar__AEBVXRibbonBar_CMFCRibbonInfo___Z(
    void* pThis, void* pBar, const void* pInfo) {
    (void)pThis;
    (void)pBar;
    (void)pInfo;
}

// Stub: RVA 0xf8230 (mfc140u) allocates and initializes a 0x5a0-byte application-button object, installs its retail vfptr and links it to the bar; that object/vfptr layout is not modeled.
// Symbol: ?CreateApplicationButton@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonApplicationButton@@AEAVCMFCRibbonBar@@@Z
extern "C" void* MS_ABI impl__CreateApplicationButton_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonApplicationButton__AEAVCMFCRibbonBar___Z(
    void* pThis, void* pBar) {
    (void)pThis;
    (void)pBar;
    return nullptr;
}

// Stub: RVA 0xf8150 (mfc140u) marshals constructor/info fields to a private category factory helper; no OpenMFC thunk or offset-pinned construction layout is available for that helper path.
// Symbol: ?CreateCategory@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonCategory@@AEAVCMFCRibbonBar@@AEBVXCategory@CMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl__CreateCategory_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonCategory__AEAVCMFCRibbonBar__AEBVXCategory_CMFCRibbonInfo___Z(
    void* pThis, void* pBar, const void* pInfo) {
    (void)pThis;
    (void)pBar;
    (void)pInfo;
    return nullptr;
}

// Stub: RVA 0xf81a0 (mfc140u) marshals context/category data to a private factory helper. RI_XContext/RI_XCategory are pinned privately in CMFCRibbonInfo.cpp but not shared here, and this helper has no OpenMFC thunk for safe delegation.
// Symbol: ?CreateCategoryContext@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonCategory@@AEAVCMFCRibbonBar@@AEBVXContext@CMFCRibbonInfo@@AEBVXCategory@5@@Z
extern "C" void* MS_ABI impl__CreateCategoryContext_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonCategory__AEAVCMFCRibbonBar__AEBVXContext_CMFCRibbonInfo__AEBVXCategory_5__Z(
    void* pThis, void* pBar, const void* pContext, const void* pCategoryList) {
    (void)pThis;
    (void)pBar;
    (void)pContext;
    (void)pCategoryList;
    return nullptr;
}

// Stub: RVA 0xf81f0 (mfc140u) marshals constructor/category-main fields to a private main-panel factory helper; that helper has no available OpenMFC thunk or layout-backed implementation.
// Symbol: ?CreateCategoryMain@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonMainPanel@@AEAVCMFCRibbonBar@@AEBVXCategoryMain@CMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl__CreateCategoryMain_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonMainPanel__AEAVCMFCRibbonBar__AEBVXCategoryMain_CMFCRibbonInfo___Z(
    void* pThis, void* pBar, const void* pCategoryMainInfo) {
    (void)pThis;
    (void)pBar;
    (void)pCategoryMainInfo;
    return nullptr;
}

// Stub: RVA 0xf8e30 (mfc140u) dispatches on XElement type and allocates/initializes many concrete element classes; the required type-specific retail object layouts are not modeled.
// Symbol: ?CreateElement@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonBaseElement@@AEBVXElement@CMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl__CreateElement_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonBaseElement__AEBVXElement_CMFCRibbonInfo___Z(
    void* pThis, const void* pInfo) {
    (void)pThis;
    (void)pInfo;
    return nullptr;
}

// Stub: RVA 0xf8100 (mfc140u) conditionally processes info +0x20 through a helper then tail-calls a private panel factory helper; the destination panel layout is not modeled.
// Symbol: ?CreatePanel@CMFCRibbonConstructor@@MEBAPEAVCMFCRibbonPanel@@AEAVCMFCRibbonCategory@@AEBVXPanel@CMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl__CreatePanel_CMFCRibbonConstructor__MEBAPEAVCMFCRibbonPanel__AEAVCMFCRibbonCategory__AEBVXPanel_CMFCRibbonInfo___Z(
    void* pThis, void* pCategory, const void* pInfo) {
    (void)pThis;
    (void)pCategory;
    (void)pInfo;
    return nullptr;
}

// Stub: the mfc140u symbol map does not resolve this name; the mfc140 ANSI twin's body updates button fields +0x1b8/+0x1bc, +0x1d8/+0x1e0 and +0x200/+0x204 and manages image state through helpers. The compact button/image model does not expose that state.
// Symbol: ?SetIcon@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonButton@@W4RibbonImageType@CMFCRibbonBaseElement@@AEBVCMFCToolBarImages@@H@Z
extern "C" void MS_ABI impl__SetIcon_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonButton__W4RibbonImageType_CMFCRibbonBaseElement__AEBVCMFCToolBarImages__H_Z(
    void* pThis, void* pButton, int imageType, const void* pImages, int index) {
    (void)pThis;
    (void)pButton;
    (void)imageType;
    (void)pImages;
    (void)index;
}

// The mfc140 ANSI-twin body tail-dispatches to the destination element's SetID slot with XID::m_Value (+0x08).
// OpenMFC stores the ID in its compact element model, so use its verified SetID thunk rather than writing retail +0x118.
// Symbol: ?SetID@CMFCRibbonConstructor@@MEBAXAEAVCMFCRibbonBaseElement@@AEBVXID@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__SetID_CMFCRibbonConstructor__MEBAXAEAVCMFCRibbonBaseElement__AEBVXID_CMFCRibbonInfo___Z(
    void* pThis, void* pElement, const void* id) {
    (void)pThis;
    const auto* xid = static_cast<const RibbonConstructorXIDView*>(id);
    impl__SetID_CMFCRibbonBaseElement__UEAAXI_Z(
        static_cast<CMFCRibbonBaseElement*>(pElement), xid->m_Value);
}
