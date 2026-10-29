// CMFCRibbonGallery — OpenMFC implementation.
// Sources: cbarcore.cpp, global_mfc-feature-17_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCRibbonCategorySupport.h"
#include "detail/MfccoreSupport.h"

class CMFCRibbonGalleryIcon;

extern "C" int MS_ABI impl__OnKey_CMFCRibbonButton__MEAAHH_Z(
    CMFCRibbonButton* pThis, int bIsMenuKey);

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
// This preserves the exported 56-byte data size, but is opaque zeroed storage rather than a
// modeled CMap; GetLastSelectedItem currently tracks values in a separate clean-room side table.
extern "C" CMap_IIHH_56Bytes impl__m_mapSelectedItems_CMFCRibbonGallery__1V__CMap_IIHH__A = {};
// CMFCRibbonGallery::RemoveItemToolTips() -- retail removes the per-item tooltip CString
// array at +0x480 (releasing its entries); that array is not modeled here.
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
// CMFCRibbonGallery::SetItemToolTip(int, const wchar_t*) -- for nonnegative indices, retail
// extends and updates the per-item tooltip CString array (header +0x480, data +0x488, count
// +0x490); negative indices return without changes. That array is not modeled here.
// TODO(clean-room): transcribed partially -- the tooltip array is not modeled.
// Symbol: ?SetItemToolTip@CMFCRibbonGallery@@QEAAXHPEB_W@Z
extern "C" void MS_ABI impl__SetItemToolTip_CMFCRibbonGallery__QEAAXHPEB_W_Z(
    CMFCRibbonGallery* pThis, int /*nIndex*/, const wchar_t* /*lpszToolTip*/)
{
    if (!pThis) return;
}
// CMFCRibbonGallery::SetNotifyParentID(int) -- retail stores nID into
// m_nNotifyParentID (+0x4d0) and, when +0xe8 points to the expected menu-button
// type, propagates the value there too. Neither member is modeled.
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
// records the palette icon count at +0x4c8 and rebuilds the gallery.
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
// resource uiImageResID into m_Images (+0x2e8), stores the palette icon count at
// +0x4c8 and the requested image width at +0x2e8+0x68, then rebuilds; the built
// gallery path returns without these changes. Not modeled.
// TODO(clean-room): transcribed partially -- gallery image state is not
// modeled.
// Symbol: ?SetPalette@CMFCRibbonGallery@@QEAAXIH@Z
extern "C" void MS_ABI impl__SetPalette_CMFCRibbonGallery__QEAAXIH_Z(
    CMFCRibbonGallery* pThis, unsigned int /*uiImageResID*/, int /*cxPaletteImage*/)
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
// Stub: at mfc140u RVA 0x112960 retail does this only while m_bBuilt (+0x4e0) is false:
// it appends the image group, updates palette/group state, then removes existing icon objects.
// Those members are not modeled here; the already-built path returns without doing this work.
// Symbol: ?AddGroup@CMFCRibbonGallery@@QEAAXPEB_WAEAVCMFCToolBarImages@@@Z
extern "C" void MS_ABI impl__AddGroup_CMFCRibbonGallery__QEAAXPEB_WAEAVCMFCToolBarImages___Z(
    CMFCRibbonGallery* pThis, const wchar_t* lpszGroupName, CMFCToolBarImages* pImages) {
    (void)pThis; (void)lpszGroupName; (void)pImages;
}

// Stub: at mfc140u RVA 0x112a10 retail appends a named user-icon group and updates the palette
// icon count, then removes existing icon objects, only when m_bBuilt (+0x4e0) is true; the false
// path returns without doing so. The group arrays and icon collection are absent from the declaration.
// Symbol: ?AddGroup@CMFCRibbonGallery@@QEAAXPEB_WH@Z
extern "C" void MS_ABI impl__AddGroup_CMFCRibbonGallery__QEAAXPEB_WH_Z(
    CMFCRibbonGallery* pThis, const wchar_t* lpszGroupName, int nIconsNum) {
    (void)pThis; (void)lpszGroupName; (void)nIconsNum;
}

// Stub: at mfc140u RVA 0x1127e0, a non-null name and an unbuilt gallery are required; retail
// loads the bitmap resource and delegates to the image-group overload, which clears existing icon
// objects. This entry does not rebuild icon locations; the resource-backed members are not modeled.
// Symbol: ?AddGroup@CMFCRibbonGallery@@QEAAXPEB_WIH@Z
extern "C" void MS_ABI impl__AddGroup_CMFCRibbonGallery__QEAAXPEB_WIH_Z(
    CMFCRibbonGallery* pThis, const wchar_t* lpszGroupName, unsigned int uiImagesPaletteResID,
    int cxPaletteImage) {
    (void)pThis; (void)lpszGroupName; (void)uiImagesPaletteResID; (void)cxPaletteImage;
}

// Stub: mfc140u entry RVA 0x112ec0 stores bOnTop at subitem +0x16c, then tail-jumps to the
// button subitem insertion routine. Neither the gallery layout nor that button state is modeled.
// Symbol: ?AddSubItem@CMFCRibbonGallery@@QEAAXPEAVCMFCRibbonBaseElement@@HH@Z
extern "C" void MS_ABI impl__AddSubItem_CMFCRibbonGallery__QEAAXPEAVCMFCRibbonBaseElement__HH_Z(
    CMFCRibbonGallery* pThis, CMFCRibbonBaseElement* pSubItem, int nIndex, int bOnTop) {
    (void)pThis; (void)pSubItem; (void)nIndex; (void)bOnTop;
}

// Stub: mfc140u RVA 0x112cf0 passes this gallery's ID (falling back to inherited +0x118 when
// +0x4a8 is zero) to the selected-item-map helper, then removes icons, groups, and tooltips,
// cleans the image list, and resets palette fields. Those collections are not represented here.
// Symbol: ?Clear@CMFCRibbonGallery@@UEAAXXZ
extern "C" void MS_ABI impl__Clear_CMFCRibbonGallery__UEAAXXZ(CMFCRibbonGallery* pThis) {
    (void)pThis;
}

// Stub: mfc140u entry RVA 0x112ed0 initializes the gallery defaults at +0x1ec and +0x4a8..+0x4ec;
// none of these retail fields are present in the OpenMFC declaration.
// Symbol: ?CommonInit@CMFCRibbonGallery@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonGallery__IEAAXXZ(CMFCRibbonGallery* pThis) {
    (void)pThis;
}

// Stub: mfc140u RVA 0x113f60 first copies base-element state; only if the source passes the
// gallery runtime-class check does retail copy image-list/scalar state and group/tooltip arrays,
// then rebuild icons from that state. The required retail layout and array ownership are not modeled.
// Symbol: ?CopyFrom@CMFCRibbonGallery@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonGallery__MEAAXAEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonGallery* pThis, const CMFCRibbonBaseElement* pSource) {
    (void)pThis; (void)pSource;
}

// Stub: mfc140u entry RVA 0x113650 builds icon objects and their locations from the image,
// group, and layout arrays; these objects and arrays are not modeled.
// Symbol: ?CreateIcons@CMFCRibbonGallery@@IEAAXXZ
extern "C" void MS_ABI impl__CreateIcons_CMFCRibbonGallery__IEAAXXZ(CMFCRibbonGallery* pThis) {
    (void)pThis;
}

// Stub: mfc140u entry RVA 0x1149a0 checks the final gallery icon through its virtual state
// query and otherwise delegates to a base routine; the icon array and base dispatch are unavailable.
// Symbol: ?GetDroppedDown@CMFCRibbonGallery@@UEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetDroppedDown_CMFCRibbonGallery__UEAAPEAVCMFCRibbonBaseElement__XZ(
    CMFCRibbonGallery* pThis) {
    (void)pThis;
    return nullptr;
}

// Stub: mfc140u RVA 0x112bd0 returns the group-name CString data after checking the index
// against the array at +0x2a0/+0x2a8; an invalid index follows retail's failure path, not a null
// return. The group-name array is not modeled.
// Symbol: ?GetGroupName@CMFCRibbonGallery@@QEBAPEB_WH@Z
extern "C" const wchar_t* MS_ABI impl__GetGroupName_CMFCRibbonGallery__QEBAPEB_WH_Z(
    const CMFCRibbonGallery* pThis, int nGroupIndex) {
    (void)pThis; (void)nGroupIndex;
    return nullptr;
}

// Stub: mfc140u RVA 0x113d20 delegates to the base highlighted query when gallery-mode checks
// fail; otherwise it searches the icon array through each icon's highlighted query. The icon
// array and mode fields are unavailable.
// Symbol: ?GetHighlighted@CMFCRibbonGallery@@MEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetHighlighted_CMFCRibbonGallery__MEAAPEAVCMFCRibbonBaseElement__XZ(
    CMFCRibbonGallery* pThis) {
    (void)pThis;
    return nullptr;
}

// Stub: mfc140u entry RVA 0x114800 selects a CString from the icon index, using gallery text
// for the three special indices and a constant string otherwise; that text state is not modeled.
// Symbol: ?GetIconDescription@CMFCRibbonGallery@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEBVCMFCRibbonGalleryIcon@@@Z
extern "C" CString* MS_ABI impl__GetIconDescription_CMFCRibbonGallery__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEBVCMFCRibbonGalleryIcon___Z(
    const CMFCRibbonGallery* pThis, CString* pResult, const CMFCRibbonGalleryIcon* pIcon) {
    (void)pThis; (void)pIcon;
    return pResult;
}

// Stub: mfc140u entry RVA 0x114c70 returns the CSize stored at +0x350; the retail image-list
// member and that size are not present in the declared gallery layout.
// Symbol: ?GetIconSize@CMFCRibbonGallery@@MEBA?AVCSize@@XZ
extern "C" CSize* MS_ABI impl__GetIconSize_CMFCRibbonGallery__MEBA_AVCSize__XZ(
    const CMFCRibbonGallery* pThis, CSize* pResult) {
    (void)pThis;
    return pResult;
}

// Stub: mfc140u RVA 0x114680 selects a tooltip using the special-icon index, per-item tooltip
// array (+0x488/+0x490), and fallback resource labels; those members and their ownership are not
// modeled.
// Symbol: ?GetIconToolTip@CMFCRibbonGallery@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEBVCMFCRibbonGalleryIcon@@@Z
extern "C" CString* MS_ABI impl__GetIconToolTip_CMFCRibbonGallery__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEBVCMFCRibbonGalleryIcon___Z(
    const CMFCRibbonGallery* pThis, CString* pResult, const CMFCRibbonGalleryIcon* pIcon) {
    (void)pThis; (void)pIcon;
    return pResult;
}

// Stub: mfc140u RVA 0x114640 returns the selected tooltip from +0x488 when the index is in
// range (+0x490); negative or out-of-range indices return null. That array is absent here.
// Symbol: ?GetItemToolTip@CMFCRibbonGallery@@QEBAPEB_WH@Z
extern "C" const wchar_t* MS_ABI impl__GetItemToolTip_CMFCRibbonGallery__QEBAPEB_WH_Z(
    const CMFCRibbonGallery* pThis, int nItemIndex) {
    (void)pThis; (void)nItemIndex;
    return nullptr;
}

// Stub: mfc140u RVA 0x114ad0 delegates to CMFCRibbonButton::GetKeyTipRect when gallery
// flags/count do not permit custom handling; otherwise it uses the sizing vtable slot, inherited
// rectangle, and final special icon's rect. The required layout and geometry are unavailable.
// Symbol: ?GetKeyTipRect@CMFCRibbonGallery@@MEAA?AVCRect@@PEAVCDC@@H@Z
extern "C" CRect* MS_ABI impl__GetKeyTipRect_CMFCRibbonGallery__MEAA_AVCRect__PEAVCDC__H_Z(
    CMFCRibbonGallery* pThis, CRect* pResult, CDC* pDC, int bIsMenu) {
    (void)pThis; (void)pDC; (void)bIsMenu;
    return pResult;
}

// Stub: mfc140u entry RVA 0x114880 appends the button subitems at +0x250/+0x258 and gallery
// icons (excluding the final three special entries) to the supplied CArray; those arrays are absent.
// Symbol: ?GetMenuItems@CMFCRibbonGallery@@IEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetMenuItems_CMFCRibbonGallery__IEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonGallery* pThis,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& arButtons) {
    (void)pThis; (void)arButtons;
}

// Stub: mfc140u entry RVA 0x114960 asks the regular-size vtable slot and adds 8 to its height
// when +0x4d4 is zero; the layout and sizing dispatch are not available.
// Symbol: ?GetMenuRowHeight@CMFCRibbonGallery@@IEBAHXZ
extern "C" int MS_ABI impl__GetMenuRowHeight_CMFCRibbonGallery__IEBAHXZ(
    const CMFCRibbonGallery* pThis) {
    (void)pThis;
    return 0;
}

// Stub: mfc140u RVA 0x113cb0 scans the icon array and returns the first icon whose virtual
// pressed query is non-null (or null if none); that array is not represented here.
// Symbol: ?GetPressed@CMFCRibbonGallery@@MEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetPressed_CMFCRibbonGallery__MEAAPEAVCMFCRibbonBaseElement__XZ(
    CMFCRibbonGallery* pThis) {
    (void)pThis;
    return nullptr;
}

// Stub: mfc140u RVA 0x113b90 returns the gallery itself on its mode/virtual-query bypass paths;
// otherwise it hit-tests icon rectangles using the packed CPoint and returns a matching icon or
// null. The icon collection and gallery geometry are not modeled.
// Symbol: ?HitTest@CMFCRibbonGallery@@MEAAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__HitTest_CMFCRibbonGallery__MEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(
    CMFCRibbonGallery* pThis, long long point) {
    (void)pThis; (void)point;
    return nullptr;
}

// Stub: mfc140u RVA 0x113320 first calls CMFCRibbonButton::OnAfterChangeRect, then conditionally
// recomputes palette dimensions and icon rectangles (or clears the row/column counts) and rebuilds
// icon state. The required image/icon members are absent.
// Symbol: ?OnAfterChangeRect@CMFCRibbonGallery@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonGallery__UEAAXPEAVCDC___Z(
    CMFCRibbonGallery* pThis, CDC* pDC) {
    (void)pThis; (void)pDC;
}

// Stub: mfc140u entry RVA 0x113dd0 handles special icon indices, updates selection, and routes
// palette-click behavior through gallery/menu state; those states and dispatch paths are absent.
// Symbol: ?OnClickPaletteIcon@CMFCRibbonGallery@@MEAAXPEAVCMFCRibbonGalleryIcon@@@Z
extern "C" void MS_ABI impl__OnClickPaletteIcon_CMFCRibbonGallery__MEAAXPEAVCMFCRibbonGalleryIcon___Z(
    CMFCRibbonGallery* pThis, CMFCRibbonGalleryIcon* pIcon) {
    (void)pThis; (void)pIcon;
}

// Stub: mfc140u RVA 0x1135b0 draws only on the unbuilt path, checks +0x4d4, calls a gallery
// vtable slot, and dispatches rendering through the image-list member at +0x2e8. Those members
// and palette state are not modeled.
// Symbol: ?OnDrawPaletteIcon@CMFCRibbonGallery@@MEAAXPEAVCDC@@VCRect@@HPEAVCMFCRibbonGalleryIcon@@K@Z
extern "C" void MS_ABI impl__OnDrawPaletteIcon_CMFCRibbonGallery__MEAAXPEAVCDC__VCRect__HPEAVCMFCRibbonGalleryIcon__K_Z(
    CMFCRibbonGallery* pThis, CDC* pDC, CRect rect, int nIconIndex,
    CMFCRibbonGalleryIcon* pIcon, unsigned long clrText) {
    (void)pThis; (void)pDC; (void)rect; (void)nIconIndex; (void)pIcon; (void)clrText;
}

// Retail ANSI-twin entry RVA 0x115850 (mfc140.dll) sets the argument to TRUE and tail-jumps
// to CMFCRibbonButton::OnKey; the Unicode lookup did not resolve this export. Forward to the
// sibling thunk so this wrapper preserves retail dispatch (the sibling currently returns 0).
// Symbol: ?OnKey@CMFCRibbonGallery@@MEAAHH@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonGallery__MEAAHH_Z(
    CMFCRibbonGallery* pThis, int /*bIsMenuKey*/) {
    return impl__OnKey_CMFCRibbonButton__MEAAHH_Z(
        reinterpret_cast<CMFCRibbonButton*>(pThis), TRUE);
}

// Stub: the Unicode name lookup did not resolve this export. Its ANSI twin entry RVA 0x115a10
// forwards bIsRTL to the embedded image-list object at +0x2e8, which is not modeled.
// Symbol: ?OnRTLChanged@CMFCRibbonGallery@@UEAAXH@Z
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonGallery__UEAAXH_Z(
    CMFCRibbonGallery* pThis, int bIsRTL) {
    (void)pThis; (void)bIsRTL;
}

// Stub: the Unicode name lookup did not resolve this export. Its ANSI twin entry RVA 0x115a20
// walks the icon array and updates focus state on the special icon; the array is not modeled.
// Symbol: ?OnSetFocus@CMFCRibbonGallery@@UEAAXH@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCRibbonGallery__UEAAXH_Z(
    CMFCRibbonGallery* pThis, int bSet) {
    (void)pThis; (void)bSet;
}

// Stub: the Unicode name lookup did not resolve this export. Its ANSI twin entry RVA 0x114f70
// sets up a popup-menu object from gallery geometry, icon arrays, and menu state absent here.
// Symbol: ?OnShowPopupMenu@CMFCRibbonGallery@@MEAAXXZ
extern "C" void MS_ABI impl__OnShowPopupMenu_CMFCRibbonGallery__MEAAXXZ(
    CMFCRibbonGallery* pThis) {
    (void)pThis;
}

// Stub: the Unicode name lookup did not resolve this export. Its ANSI twin entry RVA 0x1146b0
// lays out icon rectangles and special icons from gallery/image geometry that is not modeled.
// Symbol: ?RebuildIconLocations@CMFCRibbonGallery@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildIconLocations_CMFCRibbonGallery__IEAAXXZ(
    CMFCRibbonGallery* pThis) {
    (void)pThis;
}

// Stub: the Unicode name lookup did not resolve this export. Its ANSI twin entry RVA 0x113b10
// uses the popup object when available, otherwise dispatches over the icon array; that state is absent.
// Symbol: ?RedrawIcons@CMFCRibbonGallery@@QEAAXXZ
extern "C" void MS_ABI impl__RedrawIcons_CMFCRibbonGallery__QEAAXXZ(
    CMFCRibbonGallery* pThis) {
    (void)pThis;
}

// Stub: the Unicode name lookup did not resolve this export. Its ANSI twin entry RVA 0x113bd0
// deletes each icon through its virtual destructor and clears the icon array; neither is modeled.
// Symbol: ?RemoveAll@CMFCRibbonGallery@@IEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMFCRibbonGallery__IEAAXXZ(
    CMFCRibbonGallery* pThis) {
    (void)pThis;
}
