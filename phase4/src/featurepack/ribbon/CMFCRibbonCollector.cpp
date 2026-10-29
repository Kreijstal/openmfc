// CMFCRibbonCollector — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CMFCRibbonCollector@@QEAA@AEAVCMFCRibbonInfo@@K@Z
extern "C" void* MS_ABI impl___0CMFCRibbonCollector__QEAA_AEAVCMFCRibbonInfo__K_Z(void* pThis, void* pInfo, unsigned long long unusedFlags) {
    // Existing stub: RVA 0xf1550 (mfc140u) installs a vftable and stores pInfo
    // at +8 and flags at +0x10; no compatible collector vtable is defined here.
    (void)pInfo;
    (void)unusedFlags;
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
    void* pThis, void* pImages, void* pInfo, int index) {
    // Stub: RVA 0xf3280 (mfc140u) conditionally copies an image resource ID,
    // then calls CMFCToolBarImages::CopyTo for the XImage image. The source
    // collection internals are unmodeled, and CopyTo remains a conservative stub.
    (void)pThis;
    (void)pImages;
    (void)pInfo;
    (void)index;
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
    // vtable slot 1. The pre-existing constructor leaves the vfptr unset, and
    // OpenMFC has no compatible collector vtable to dispatch through.
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

// Symbol: ?GetID@CMFCRibbonCollector@@MEAAXAEBVCMFCRibbonBaseElement@@AEAVXID@CMFCRibbonInfo@@@Z
extern "C" void MS_ABI impl__GetID_CMFCRibbonCollector__MEAAXAEBVCMFCRibbonBaseElement__AEAVXID_CMFCRibbonInfo___Z(
    void* pThis, const void* pBase, void* pID) {
    // Stub: RVA 0xf2b00 (mfc140u) reads m_nID at +0x118 and, for context
    // captions, m_uiID at +0x274. OpenMFC's CMFCRibbonBaseElement declaration
    // places m_nID at +8 and its context-caption declaration omits m_uiID, so
    // applying the retail offsets to these objects would read unrelated memory.
    (void)pThis;
    (void)pBase;
    (void)pID;
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
