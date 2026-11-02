// CMFCRibbonCollector — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// CMFCRibbonCollector is not declared in include/openmfc. Retail's constructor
// stores m_Info at +8 and DWORD m_dwFlags at +0x10 (RVA 0xf1550, mfc140u);
// its destructor writes the vfptr at +0 (RVA 0xf15a0, mfc140u).
struct CMFCRibbonCollectorLayout {
    void* vfptr;
    void* m_Info;
    std::uint32_t m_dwFlags;
    std::uint32_t _pad_14;
};
static_assert(offsetof(CMFCRibbonCollectorLayout, m_Info) == 0x08, "CMFCRibbonCollector::m_Info");
static_assert(offsetof(CMFCRibbonCollectorLayout, m_dwFlags) == 0x10, "CMFCRibbonCollector::m_dwFlags");
// The constructor only establishes fields through +0x13; this is the local view's
// rounded size, not evidence that the retail class has no further members.
static_assert(sizeof(CMFCRibbonCollectorLayout) == 0x18, "CMFCRibbonCollector layout view size");

// Symbol: ??0CMFCRibbonCollector@@QEAA@AEAVCMFCRibbonInfo@@K@Z
extern "C" void* MS_ABI impl___0CMFCRibbonCollector__QEAA_AEAVCMFCRibbonInfo__K_Z(
    void* pThis, void* pInfo, std::uint32_t flags) {
    // Partial transcription of RVA 0xf1550 (mfc140u): retail stores pInfo at
    // +8 and the 32-bit flags at +0x10, and also installs its collector
    // vftable. The latter cannot be transplanted as an absolute retail pointer;
    // no compatible local collector vftable is defined in this translation unit.
    auto* layout = static_cast<CMFCRibbonCollectorLayout*>(pThis);
    layout->m_Info = pInfo;
    layout->m_dwFlags = flags;
    return pThis;
}

// Symbol: ??1CMFCRibbonCollector@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonCollector__UEAA_XZ(void* pThis) {
    // Stub: RVA 0xf15a0 (mfc140u) stores the retail collector vftable at +0;
    // OpenMFC does not define a compatible, relocatable collector vtable.
    (void)pThis;
}

// Symbol: ?CollectBaseElement@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonBaseElement@@AEAVXElement@CMFCRibbonInfo@@H@Z
extern "C" void MS_ABI impl__CollectBaseElement_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonBaseElement__AEAVXElement_CMFCRibbonInfo__H_Z(
    void* pThis, const void* pBase, void* pElement, int index) {
    // Stub: RVA 0xf2900 (mfc140u) writes element metadata and dispatches
    // type-dependent work. RI_XElement shadows exist in CMFCRibbonInfo.cpp but
    // are translation-unit-local; source element views are not available here.
    (void)pThis;
    (void)pBase;
    (void)pElement;
    (void)index;
}

// Symbol: ?CollectCategory@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonCategory@@AEAVXCategory@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__CollectCategory_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonCategory__AEAVXCategory_CMFCRibbonInfo___Z(
    void* pThis, const void* pCategory, void* pInfo) {
    // Stub: RVA 0xf1be0 (mfc140u) traverses category/panel data into nested
    // XCategory arrays. RI_XCategory/RI_XPanel shadows are local to
    // CMFCRibbonInfo.cpp; retail-compatible source collection views are absent here.
    (void)pThis;
    (void)pCategory;
    (void)pInfo;
}

// Symbol: ?CollectCategoryMain@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonBar@@AEAVXCategoryMain@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__CollectCategoryMain_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonBar__AEAVXCategoryMain_CMFCRibbonInfo___Z(
    void* pThis, const void* pBar, void* pCategoryMain) {
    // Stub: RVA 0xf1a10 (mfc140u) walks bar-owned collections and builds
    // XCategoryMain data. RI_XCategoryMain is shadowed in CMFCRibbonInfo.cpp,
    // but that view is translation-unit-local and source collection views are absent here.
    (void)pThis;
    (void)pBar;
    (void)pCategoryMain;
}

// Symbol: ?CollectElement@CMFCRibbonCollector@@MEAAPEAVXElement@CMFCRibbonInfo@@AEBVCMFCRibbonBaseElement@@@Z
extern "C" void* MS_ABI impl__CollectElement_CMFCRibbonCollector__MEAAPEAVXElement_CMFCRibbonInfo__AEBVCMFCRibbonBaseElement___Z(
    void* pThis, void* pInfo, const void* pBase) {
    // Stub: RVA 0xf1ec0 (mfc140u) selects and constructs XElement subclasses
    // by runtime type. RI_XElement shadows are local to CMFCRibbonInfo.cpp;
    // source subtype views and the construction/dispatch support are absent here.
    (void)pThis;
    (void)pInfo;
    (void)pBase;
    return nullptr;
}

// Symbol: ?CollectElement@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonBaseElement@@AEAVXElement@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__CollectElement_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonBaseElement__AEAVXElement_CMFCRibbonInfo___Z(
    void* pThis, const void* pBase, void* pInfo) {
    // Stub: RVA 0xf27a0 (mfc140u) dispatches by XElement type and populates
    // subtype-specific fields. The RI_X* shadows are local to CMFCRibbonInfo.cpp;
    // source element views and compatible virtual dispatch support are absent here.
    (void)pThis;
    (void)pBase;
    (void)pInfo;
}

// Symbol: ?CollectImageInfo@CMFCRibbonCollector@@MEAAXAEAVCMFCToolBarImages@@AEAVXImage@CMFCRibbonInfo@@H@Z
extern "C" void MS_ABI impl__CollectImageInfo_CMFCRibbonCollector__MEAAXAEAVCMFCToolBarImages__AEAVXImage_CMFCRibbonInfo__H_Z(
    void* pThis, void* pImages, void* pInfo, int bCollectID) {
    // Stub: at RVA 0xf3280 (mfc140u), when bCollectID is true, retail tests
    // the qword at pImages+0x108 for 1 and the pointer at +0xf8 for non-null;
    // it then copies the pointed-to record's dword at +0x10 into pInfo+8.
    // Without a modeled source collection view, that condition cannot safely
    // be interpreted as an element count. Retail then tail-jumps to
    // CMFCToolBarImages::CopyTo (RVA 0x16f2f0, mfc140u), whose OpenMFC body is
    // still a conservative stub, so the complete operation is unavailable.
    (void)pThis;
    (void)pImages;
    (void)pInfo;
    (void)bCollectID;
}

// Symbol: ?CollectPanel@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonPanel@@AEAVXPanel@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__CollectPanel_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonPanel__AEAVXPanel_CMFCRibbonInfo___Z(
    void* pThis, const void* pPanel, void* pInfo) {
    // Stub: RVA 0xf1da0 (mfc140u) copies panel state and iterates its elements
    // into XPanel arrays. RI_XPanel is local to CMFCRibbonInfo.cpp; source panel
    // collections and element views are not available here.
    (void)pThis;
    (void)pPanel;
    (void)pInfo;
}

// Symbol: ?CollectQATElements@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonBar@@AEAVXRibbonBar@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__CollectQATElements_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonBar__AEAVXRibbonBar_CMFCRibbonInfo___Z(
    void* pThis, const void* pBar, void* pInfo) {
    // Stub: RVA 0xf1910 (mfc140u) walks QAT entries and constructs XQAT items.
    // RI_XQAT and RI_XQATItem are local to CMFCRibbonInfo.cpp; source bar/QAT
    // collections and construction support are not available here.
    (void)pThis;
    (void)pBar;
    (void)pInfo;
}

// Symbol: ?CollectRibbonBar@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonBar@@AEAVXRibbonBar@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__CollectRibbonBar_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonBar__AEAVXRibbonBar_CMFCRibbonInfo___Z(
    void* pThis, const void* pBar, void* pInfo) {
    // Stub: RVA 0xf15d0 (mfc140u) populates XRibbonBar fields and traverses
    // image, category, panel, and QAT collections. RI_XRibbonBar is local to
    // CMFCRibbonInfo.cpp; source collection views and dispatch support are absent here.
    (void)pThis;
    (void)pBar;
    (void)pInfo;
}

// Symbol: ?CollectRibbonBar@CMFCRibbonCollector@@UEAAXAEBVCMFCRibbonBar@@@Z
extern "C" void MS_ABI impl__CollectRibbonBar_CMFCRibbonCollector__UEAAXAEBVCMFCRibbonBar___Z(void* pThis, const void* pBar) {
    // Stub: RVA 0xf15b0 (mfc140u) forwards m_Info + 0x18 through collector
    // vtable slot 1. The OpenMFC constructor cannot install a compatible local
    // collector vftable, so its vfptr remains unset and this dispatch is unavailable.
    (void)pThis;
    (void)pBar;
}

// Symbol: ?GetCategoryImages@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonCategory@@AEAVXCategory@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__GetCategoryImages_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonCategory__AEAVXCategory_CMFCRibbonInfo___Z(
    void* pThis, const void* pCategory, void* pXCategory) {
    // Stub: RVA 0xf2ec0 (mfc140u) traverses category/panel/group images and
    // writes category image indexes. RI_XCategory/RI_XImage shadows are local
    // to CMFCRibbonInfo.cpp; retail-compatible source collection views are absent here.
    (void)pThis;
    (void)pCategory;
    (void)pXCategory;
}

// Symbol: ?GetCategoryImages@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonCategory@@AEAVXImage@CMFCRibbonInfo@@1@Z
extern "C" void MS_ABI impl__GetCategoryImages_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonCategory__AEAVXImage_CMFCRibbonInfo__1_Z(
    void* pThis, const void* pCategory, void* pImage) {
    // Stub: RVA 0xf3150 (mfc140u) collects small/large category images and
    // conditionally copies resource IDs. RI_XImage is local to CMFCRibbonInfo.cpp;
    // source category image views and compatible virtual dispatch support are absent here.
    (void)pThis;
    (void)pCategory;
    (void)pImage;
}

// Symbol: ?GetElementImages@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonBaseElement@@AEAVXImage@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__GetElementImages_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonBaseElement__AEAVXImage_CMFCRibbonInfo___Z(
    void* pThis, const void* pBase, void* pImage) {
    // Stub: RVA 0xf31e0 (mfc140u) runtime-checks element subtypes and collects
    // their image sources. RI_XImage/subtype shadows are local to CMFCRibbonInfo.cpp;
    // source element image views and compatible virtual dispatch support are absent here.
    (void)pThis;
    (void)pBase;
    (void)pImage;
}

// The collector's ordinary base-element IDs use OpenMFC's compact prefix;
// do not apply retail +0x118 to this declaration. The destination is XID, whose
// value field follows its 8-byte CString. These views are local because the
// richer ribbon-info model remains private to CMFCRibbonInfo.cpp.
struct RibbonCollectorElementIDView {
    void* vfptr;
    std::uint32_t m_nID;
};
static_assert(offsetof(RibbonCollectorElementIDView, m_nID) == sizeof(CObject),
              "OpenMFC CMFCRibbonBaseElement::m_nID follows CObject");
struct RibbonCollectorXIDValueView {
    std::uint8_t _name[8];
    std::uint32_t m_Value;
};
static_assert(offsetof(RibbonCollectorXIDValueView, m_Value) == 0x08,
              "XID::m_Value follows CString");

// Symbol: ?GetID@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonBaseElement@@AEAVXID@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__GetID_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonBaseElement__AEAVXID_CMFCRibbonInfo___Z(
    void* pThis, const void* pBase, void* pID) {
    // Partial implementation of retail RVA 0xf2b00 (mfc140u): ordinary
    // CMFCRibbonBaseElement IDs come from the compact +8 field and UINT_MAX is
    // normalized to zero. Retail additionally detects CMFCRibbonContextCaption
    // for IDs 0/UINT_MAX and reads m_uiID at +0x274; OpenMFC's caption is a
    // separate CObject shell with no such field, so that exceptional branch is
    // intentionally not applied to this differently laid-out object.
    (void)pThis;
    if (pBase == nullptr || pID == nullptr) return;

    const auto* base = static_cast<const RibbonCollectorElementIDView*>(pBase);
    auto* xid = static_cast<RibbonCollectorXIDValueView*>(pID);
    xid->m_Value = (base->m_nID == 0xffffffffu) ? 0u : base->m_nID;
}

// Symbol: ?GetRibbonBarImages@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonBar@@AEAVXRibbonBar@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__GetRibbonBarImages_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonBar__AEAVXRibbonBar_CMFCRibbonInfo___Z(
    void* pThis, const void* pBar, void* pImage) {
    // Stub: RVA 0xf2b60 (mfc140u) traverses ribbon/category/panel image sources
    // and assigns indexes. RI_XRibbonBar/RI_XImage are local to CMFCRibbonInfo.cpp;
    // retail-compatible source collection views are absent here.
    (void)pThis;
    (void)pBar;
    (void)pImage;
}
