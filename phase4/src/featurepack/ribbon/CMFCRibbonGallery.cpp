// CMFCRibbonGallery — OpenMFC implementation.
// Sources: cbarcore.cpp, global_mfc-feature-17_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCRibbonCategorySupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ??0CMFCRibbonGallery@@QEAA@IPEB_WHHAEAVCMFCToolBarImages@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonGallery__QEAA_IPEB_WHHAEAVCMFCToolBarImages___Z(
    void* pThis, UINT nID, const wchar_t* lpszText, int nSmallImageIndex, int nLargeImageIndex, CMFCToolBarImages* /*pImages*/) {
    if (!pThis) return nullptr;
    auto* pGallery = new(pThis) CMFCRibbonGallery();
    pGallery->SetID(nID);
    pGallery->SetText(lpszText);
    (void)nSmallImageIndex;
    (void)nLargeImageIndex;
    return pGallery;
}
// Symbol: ??0CMFCRibbonGallery@@QEAA@IPEB_WHHVCSize@@HH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonGallery__QEAA_IPEB_WHHVCSize__HH_Z(
    void* pThis, UINT nID, const wchar_t* lpszText, int nSmallImageIndex, int nLargeImageIndex,
    CSize /*sizeIcon*/, int /*nIconsInRow*/, int /*nRows*/) {
    if (!pThis) return nullptr;
    auto* pGallery = new(pThis) CMFCRibbonGallery();
    pGallery->SetID(nID);
    pGallery->SetText(lpszText);
    (void)nSmallImageIndex;
    (void)nLargeImageIndex;
    return pGallery;
}
// Symbol: ?GetRegularSize@CMFCRibbonGallery@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonGallery__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonGallery* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 42);
}
// Symbol: ?GetCompactSize@CMFCRibbonGallery@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetCompactSize_CMFCRibbonGallery__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonGallery* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 64 + RibbonTextPixels(pThis), 32);
}
// Symbol: ?OnDraw@CMFCRibbonGallery@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonGallery__UEAAXPEAVCDC___Z(CMFCRibbonGallery* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    (void)CMFCVisualManager::GetInstance();
}
// Symbol: ?OnEnable@CMFCRibbonGallery@@UEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CMFCRibbonGallery__UEAAXH_Z(CMFCRibbonGallery* pThis, int bEnable) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonElementsEnabled[pThis] = (bEnable != FALSE);
}
// Symbol: ?SelectItem@CMFCRibbonGallery@@QEAAXH@Z
extern "C" void MS_ABI impl__SelectItem_CMFCRibbonGallery__QEAAXH_Z(CMFCRibbonGallery* pThis, int nItem) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_galleryLastSelectedByID[pThis->GetID()] = nItem;
}
// Symbol: ?GetLastSelectedItem@CMFCRibbonGallery@@SAHI@Z
extern "C" int MS_ABI impl__GetLastSelectedItem_CMFCRibbonGallery__SAHI_Z(UINT nGalleryID) {
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_galleryLastSelectedByID.find(nGalleryID);
    return (it != g_galleryLastSelectedByID.end()) ? it->second : -1;
}
// Symbol: ?m_mapSelectedItems@CMFCRibbonGallery@@1V?$CMap@IIHH@@A
extern "C" CMap_IIHH_56Bytes impl__m_mapSelectedItems_CMFCRibbonGallery__1V__CMap_IIHH__A = {};
// CMFCRibbonGallery::RemoveItemToolTips() -- retail walks m_Items (+0x480)
// releasing each item's tool-tip string (and its refcount); OpenMFC has no
// gallery item objects.
// TODO(clean-room): transcribed partially -- gallery items are not modeled.
// Symbol: ?RemoveItemToolTips@CMFCRibbonGallery@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveItemToolTips_CMFCRibbonGallery__QEAAXXZ(
    CMFCRibbonGallery* pThis)
{
    if (!pThis) return;
}
// CMFCRibbonGallery::SetGroupName(int, const wchar_t*) -- retail replaces the
// CString name of the group at m_arGroups[nIndex] (+0x2a0/+0x2a8) and, when a
// parent menu button exists, of the corresponding button in the drop-down
// menu. No group state is modeled.
// TODO(clean-room): transcribed partially -- gallery groups are not modeled.
// Symbol: ?SetGroupName@CMFCRibbonGallery@@QEAAXHPEB_W@Z
extern "C" void MS_ABI impl__SetGroupName_CMFCRibbonGallery__QEAAXHPEB_W_Z(
    CMFCRibbonGallery* pThis, int /*nIndex*/, const wchar_t* /*lpszName*/)
{
    if (!pThis) return;
}
// CMFCRibbonGallery::SetItemToolTip(int, const wchar_t*) -- retail stores the
// tool-tip string into the item at m_Items[nIndex] (+0x480/+0x488/+0x490).
// No item state is modeled.
// TODO(clean-room): transcribed partially -- gallery items are not modeled.
// Symbol: ?SetItemToolTip@CMFCRibbonGallery@@QEAAXHPEB_W@Z
extern "C" void MS_ABI impl__SetItemToolTip_CMFCRibbonGallery__QEAAXHPEB_W_Z(
    CMFCRibbonGallery* pThis, int /*nIndex*/, const wchar_t* /*lpszToolTip*/)
{
    if (!pThis) return;
}
// CMFCRibbonGallery::SetNotifyParentID(int) -- retail stores nID into
// m_nNotifyParentID (+0x4d0) and propagates it to the parent menu button
// (+0xe8). Neither member is modeled.
// TODO(clean-room): transcribed partially -- gallery parent/notify state is
// not modeled.
// Symbol: ?SetNotifyParentID@CMFCRibbonGallery@@IEAAXH@Z
extern "C" void MS_ABI impl__SetNotifyParentID_CMFCRibbonGallery__IEAAXH_Z(
    CMFCRibbonGallery* pThis, int /*nID*/)
{
    if (!pThis) return;
}
// CMFCRibbonGallery::SetPalette(CMFCToolBarImages&) -- retail (when not yet
// built, m_bBuilt +0x4e0 == 0) copies the images into m_Images (+0x2e8),
// records the image count in m_nIconsInRow (+0x4c8) and rebuilds the gallery.
// The image list and rebuild path are not modeled.
// TODO(clean-room): transcribed partially -- gallery image state is not
// modeled.
// Symbol: ?SetPalette@CMFCRibbonGallery@@QEAAXAEAVCMFCToolBarImages@@@Z
extern "C" void MS_ABI impl__SetPalette_CMFCRibbonGallery__QEAAXAEAVCMFCToolBarImages___Z(
    CMFCRibbonGallery* pThis, CMFCToolBarImages& /*images*/)
{
    if (!pThis) return;
}
// CMFCRibbonGallery::SetPalette(unsigned int, int) -- retail loads the bitmap
// resource uiImageResID into m_Images (+0x2e8), sets icon layout
// (m_nIconsInRow +0x4c8, icon size +0x2e8+0x68), and rebuilds. Not modeled.
// TODO(clean-room): transcribed partially -- gallery image state is not
// modeled.
// Symbol: ?SetPalette@CMFCRibbonGallery@@QEAAXIH@Z
extern "C" void MS_ABI impl__SetPalette_CMFCRibbonGallery__QEAAXIH_Z(
    CMFCRibbonGallery* pThis, unsigned int /*uiImageResID*/, int /*bRTL*/)
{
    if (!pThis) return;
}
// CMFCRibbonGallery::SetParentCategory(CMFCRibbonCategory*) -- retail first
// delegates to CMFCRibbonBaseElement::SetParentCategory (not declared in the
// clean-room header) and then propagates the category to every item in the
// gallery. Neither the base setter nor the items are modeled.
// TODO(clean-room): transcribed partially -- the base element setter and the
// gallery items are not modeled.
// Symbol: ?SetParentCategory@CMFCRibbonGallery@@MEAAXPEAVCMFCRibbonCategory@@@Z
extern "C" void MS_ABI impl__SetParentCategory_CMFCRibbonGallery__MEAAXPEAVCMFCRibbonCategory___Z(
    CMFCRibbonGallery* pThis, CMFCRibbonCategory* /*pCategory*/)
{
    if (!pThis) return;
}
CMFCRibbonGallery::CMFCRibbonGallery() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonGallery::~CMFCRibbonGallery() {}
// Symbol: ?AddGroup@CMFCRibbonGallery@@QEAAXPEB_WAEAVCMFCToolBarImages@@@Z
extern "C" void MS_ABI impl__AddGroup_CMFCRibbonGallery__QEAAXPEB_WAEAVCMFCToolBarImages___Z(const wchar_t* p0, void* /*class*/* p1) {}

// Symbol: ?AddGroup@CMFCRibbonGallery@@QEAAXPEB_WH@Z
extern "C" void MS_ABI impl__AddGroup_CMFCRibbonGallery__QEAAXPEB_WH_Z(const wchar_t* p0, int p1) {}

// Symbol: ?AddGroup@CMFCRibbonGallery@@QEAAXPEB_WIH@Z
extern "C" void MS_ABI impl__AddGroup_CMFCRibbonGallery__QEAAXPEB_WIH_Z(const wchar_t* p0, unsigned int p1, int p2) {}

// Symbol: ?AddSubItem@CMFCRibbonGallery@@QEAAXPEAVCMFCRibbonBaseElement@@HH@Z
extern "C" void MS_ABI impl__AddSubItem_CMFCRibbonGallery__QEAAXPEAVCMFCRibbonBaseElement__HH_Z(void* /*class*/* p0, int p1, int p2) {}

// Symbol: ?Clear@CMFCRibbonGallery@@UEAAXXZ
extern "C" void MS_ABI impl__Clear_CMFCRibbonGallery__UEAAXXZ() {}

// Symbol: ?CommonInit@CMFCRibbonGallery@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonGallery__IEAAXXZ() {}

// Symbol: ?CopyFrom@CMFCRibbonGallery@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonGallery__MEAAXAEBVCMFCRibbonBaseElement___Z(const void* /*class*/* p0) {}

// Symbol: ?CreateIcons@CMFCRibbonGallery@@IEAAXXZ
extern "C" void MS_ABI impl__CreateIcons_CMFCRibbonGallery__IEAAXXZ() {}

// Symbol: ?GetDroppedDown@CMFCRibbonGallery@@UEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetDroppedDown_CMFCRibbonGallery__UEAAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// Symbol: ?GetGroupName@CMFCRibbonGallery@@QEBAPEB_WH@Z
extern "C" void* MS_ABI impl__GetGroupName_CMFCRibbonGallery__QEBAPEB_WH_Z(int p0) {
    return nullptr;
}

// Symbol: ?GetHighlighted@CMFCRibbonGallery@@MEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetHighlighted_CMFCRibbonGallery__MEAAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// Symbol: ?GetIconDescription@CMFCRibbonGallery@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEBVCMFCRibbonGalleryIcon@@@Z
extern "C" void* MS_ABI impl__GetIconDescription_CMFCRibbonGallery__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEBVCMFCRibbonGalleryIcon___Z(void* /*class*/* p0, void** p1, void* p2, const void* /*class*/* p3) {
    return nullptr;
}

// Symbol: ?GetIconSize@CMFCRibbonGallery@@MEBA?AVCSize@@XZ
extern "C" void* MS_ABI impl__GetIconSize_CMFCRibbonGallery__MEBA_AVCSize__XZ(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?GetIconToolTip@CMFCRibbonGallery@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEBVCMFCRibbonGalleryIcon@@@Z
extern "C" void* MS_ABI impl__GetIconToolTip_CMFCRibbonGallery__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEBVCMFCRibbonGalleryIcon___Z(void* /*class*/* p0, void** p1, void* p2, const void* /*class*/* p3) {
    return nullptr;
}

// Symbol: ?GetItemToolTip@CMFCRibbonGallery@@QEBAPEB_WH@Z
extern "C" void* MS_ABI impl__GetItemToolTip_CMFCRibbonGallery__QEBAPEB_WH_Z(int p0) {
    return nullptr;
}

// Symbol: ?GetKeyTipRect@CMFCRibbonGallery@@MEAA?AVCRect@@PEAVCDC@@H@Z
extern "C" void* MS_ABI impl__GetKeyTipRect_CMFCRibbonGallery__MEAA_AVCRect__PEAVCDC__H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {
    return nullptr;
}

// Symbol: ?GetMenuItems@CMFCRibbonGallery@@IEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetMenuItems_CMFCRibbonGallery__IEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?GetMenuRowHeight@CMFCRibbonGallery@@IEBAHXZ
extern "C" int MS_ABI impl__GetMenuRowHeight_CMFCRibbonGallery__IEBAHXZ() {
    return 0;
}

// Symbol: ?GetPressed@CMFCRibbonGallery@@MEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetPressed_CMFCRibbonGallery__MEAAPEAVCMFCRibbonBaseElement__XZ() {
    return nullptr;
}

// Symbol: ?HitTest@CMFCRibbonGallery@@MEAAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" void* MS_ABI impl__HitTest_CMFCRibbonGallery__MEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?OnAfterChangeRect@CMFCRibbonGallery@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonGallery__UEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?OnClickPaletteIcon@CMFCRibbonGallery@@MEAAXPEAVCMFCRibbonGalleryIcon@@@Z
extern "C" void MS_ABI impl__OnClickPaletteIcon_CMFCRibbonGallery__MEAAXPEAVCMFCRibbonGalleryIcon___Z(void* /*class*/* p0) {}

// Symbol: ?OnDrawPaletteIcon@CMFCRibbonGallery@@MEAAXPEAVCDC@@VCRect@@HPEAVCMFCRibbonGalleryIcon@@K@Z
extern "C" void MS_ABI impl__OnDrawPaletteIcon_CMFCRibbonGallery__MEAAXPEAVCDC__VCRect__HPEAVCMFCRibbonGalleryIcon__K_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, void* /*class*/* p3, unsigned long p4) {}

// Symbol: ?OnKey@CMFCRibbonGallery@@MEAAHH@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonGallery__MEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?OnRTLChanged@CMFCRibbonGallery@@UEAAXH@Z
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonGallery__UEAAXH_Z(int p0) {}

// Symbol: ?OnSetFocus@CMFCRibbonGallery@@UEAAXH@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCRibbonGallery__UEAAXH_Z(int p0) {}

// Symbol: ?OnShowPopupMenu@CMFCRibbonGallery@@MEAAXXZ
extern "C" void MS_ABI impl__OnShowPopupMenu_CMFCRibbonGallery__MEAAXXZ() {}

// Symbol: ?RebuildIconLocations@CMFCRibbonGallery@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildIconLocations_CMFCRibbonGallery__IEAAXXZ() {}

// Symbol: ?RedrawIcons@CMFCRibbonGallery@@QEAAXXZ
extern "C" void MS_ABI impl__RedrawIcons_CMFCRibbonGallery__QEAAXXZ() {}

// Symbol: ?RemoveAll@CMFCRibbonGallery@@IEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMFCRibbonGallery__IEAAXXZ() {}
