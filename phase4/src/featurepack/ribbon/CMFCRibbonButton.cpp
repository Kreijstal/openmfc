// CMFCRibbonButton — OpenMFC implementation.
// Sources: cbarcore.cpp, global_mfc-feature-22_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCRibbonBaseElementSupport.h"
#include "detail/MfccoreSupport.h"

extern "C" void MS_ABI impl__GetToolTipText_CMFCRibbonBaseElement__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pResult, const CMFCRibbonBaseElement* pThis);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByID_CMFCRibbonBaseElement__UEAAPEAV1_I_Z(
    CMFCRibbonBaseElement* pThis, unsigned int nID);
extern "C" void MS_ABI impl__GetElementsByID_CMFCRibbonBaseElement__UEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonBaseElement* pThis, unsigned int nID,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& elements);

// Symbol: ??0CMFCRibbonButton@@QEAA@IPEB_WPEAUHICON__@@H1HH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonButton__QEAA_IPEB_WPEAUHICON____H1HH_Z(
    void* pThis, UINT nID, const wchar_t* lpszText, HICON hIcon, int bAlwaysShowDescription,
    HICON hIconSmall, int bAutoDestroyIcon, int bAlphaBlendIcon) {
    if (!pThis) return nullptr;
    return new(pThis) CMFCRibbonButton(nID, lpszText, hIcon, bAlwaysShowDescription, hIconSmall, bAutoDestroyIcon, bAlphaBlendIcon);
}
// Symbol: ?SetText@CMFCRibbonButton@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCRibbonButton__UEAAXPEB_W_Z(CMFCRibbonButton* pThis, const wchar_t* lpszText) {
    if (!pThis) return;
    pThis->CMFCRibbonBaseElement::SetText(lpszText);
}
// Symbol: ?CanBeStretched@CMFCRibbonButton@@UEAAHXZ
extern "C" int MS_ABI impl__CanBeStretched_CMFCRibbonButton__UEAAHXZ(CMFCRibbonButton* pThis) {
    return pThis ? TRUE : FALSE;
}
// Symbol: ?GetRegularSize@CMFCRibbonButton@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonButton* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 32 + RibbonTextPixels(pThis), 22);
}
// Symbol: ?GetCompactSize@CMFCRibbonButton@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetCompactSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonButton* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 22 + RibbonTextPixels(pThis), 22);
}
// Symbol: ?GetIntermediateSize@CMFCRibbonButton@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetIntermediateSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonButton* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 26 + RibbonTextPixels(pThis), 22);
}
// Symbol: ?OnCalcTextSize@CMFCRibbonButton@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnCalcTextSize_CMFCRibbonButton__UEAAXPEAVCDC___Z(CMFCRibbonButton* pThis, CDC* pDC) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonTextSizeById[pThis] = MeasureRibbonText(pThis, pDC);
}
// Symbol: ?OnDraw@CMFCRibbonButton@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonButton__UEAAXPEAVCDC___Z(CMFCRibbonButton* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    if (CMFCVisualManager* manager = CMFCVisualManager::GetInstance(); manager != nullptr) {
        manager->OnDrawRibbonDefaultPaneButton(pDC, pThis);
    }
}
// Symbol: ?OnDrawBorder@CMFCRibbonButton@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCRibbonButton__UEAAXPEAVCDC___Z(CMFCRibbonButton* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    if (CMFCVisualManager* manager = CMFCVisualManager::GetInstance(); manager != nullptr) {
        manager->OnDrawRibbonButtonBorder(pDC, pThis);
    }
}
// Symbol: ?OnFillBackground@CMFCRibbonButton@@UEAAKPEAVCDC@@@Z
extern "C" unsigned long MS_ABI impl__OnFillBackground_CMFCRibbonButton__UEAAKPEAVCDC___Z(CMFCRibbonButton* /*pThis*/, CDC* /*pDC*/) {
    const COLORREF color = FeaturePackBackgroundColor();
    return color;
}
// CMFCRibbonButton::OnLButtonDown(CPoint) -- retail hit-tests the menu-area
// rects (+0x198/+0x1a8), reads flags at +0x148/+0x1ec, and dispatches to the
// base OnLButtonDown / virtual IsDisabled (vtable+0x280) and OnShowPopupMenu
// (vtable+0x288). None of these members or slots exist in the declaration.
// TODO(clean-room): transcribed partially -- button geometry/menu state is
// not modeled.
// Symbol: ?OnLButtonDown@CMFCRibbonButton@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonButton__MEAAXVCPoint___Z(
    CMFCRibbonButton* pThis, long long /*point*/)
{
    if (!pThis) return;
}
// CMFCRibbonButton::OnLButtonUp(CPoint) -- retail reads the flag words
// (+0x144/+0x148/+0x13c/+0x154), the rect at +0x1a8, and dispatches through
// vtable+0x4d8.  None of these exist in the clean-room declaration.
// TODO(clean-room): transcribed partially -- button interaction state is not
// modeled.
// Symbol: ?OnLButtonUp@CMFCRibbonButton@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonButton__MEAAXVCPoint___Z(
    CMFCRibbonButton* pThis, long long /*point*/)
{
    if (!pThis) return;
}
// CMFCRibbonButton::OnMouseMove(CPoint) -- retail checks virtual IsDisabled
// (vtable+0x280), the ID range (+0x118), the rects at +0x198/+0x1a8 and the
// flags at +0x1f8/+0x1fc.  The ID is modeled but nothing else is, and the
// body largely drives mouse-over highlight state, which is not modeled.
// TODO(clean-room): transcribed partially -- button hover state is not
// modeled.
// Symbol: ?OnMouseMove@CMFCRibbonButton@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonButton__MEAAXVCPoint___Z(
    CMFCRibbonButton* pThis, long long /*point*/)
{
    if (!pThis) return;
}
// CMFCRibbonButton::OnShowPopupMenu() -- retail (RVA 0xe7550 (mfc140u),
// about 0x490 bytes)
// builds the whole popup menu from the button's sub-items, sizes the menu
// window, tracks it, and unwinds the drop-down state.  The sub-item array
// (+0x248/+0x250/+0x258), menu state and geometry are not modeled.
// TODO(clean-room): transcribed partially -- popup-menu construction is not
// modeled.
// Symbol: ?OnShowPopupMenu@CMFCRibbonButton@@MEAAXXZ
extern "C" void MS_ABI impl__OnShowPopupMenu_CMFCRibbonButton__MEAAXXZ(
    CMFCRibbonButton* pThis)
{
    if (!pThis) return;
}
// CMFCRibbonButton::RemoveAllSubItems() -- retail walks the sub-item CArray
// (+0x248/+0x250/+0x258), deletes each sub-item via its scalar-deleting
// destructor and calls CArray::SetSize(0).  The sub-item array is not
// modeled.
// TODO(clean-room): transcribed partially -- sub-items are not modeled.
// Symbol: ?RemoveAllSubItems@CMFCRibbonButton@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllSubItems_CMFCRibbonButton__QEAAXXZ(
    CMFCRibbonButton* pThis)
{
    if (!pThis) return;
}
// CMFCRibbonButton::RemoveSubItem(int) -- retail validates nIndex against the
// sub-item count (+0x258), deletes a non-null item, erases a valid entry, and
// returns TRUE; it returns FALSE for an out-of-range index. The old OpenMFC
// fallback always returns FALSE because the sub-item array is not modeled.
// TODO(clean-room): sub-items are not modeled; retain the safe failure fallback.
// Symbol: ?RemoveSubItem@CMFCRibbonButton@@QEAAHH@Z
extern "C" int MS_ABI impl__RemoveSubItem_CMFCRibbonButton__QEAAHH_Z(
    CMFCRibbonButton* pThis, int /*nIndex*/)
{
    if (!pThis) return FALSE;
    return FALSE;
}
// CMFCRibbonButton::SetACCData(CWnd*, CAccessibilityData&) -- retail first
// delegates to the base routine (RVA 0x11200 (mfc140u), which initializes the
// data block), then derives the role and state flags from virtual calls
// including vtable+0x4b8. For non-null this, the OpenMFC body below only
// returns TRUE; it does not initialize/populate the block. The fields/virtuals are unmodeled.
// TODO(clean-room): retained pre-existing return-value approximation; output data differs from retail.
// Symbol: ?SetACCData@CMFCRibbonButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonButton* pThis, CWnd* /*pWndParent*/, CAccessibilityData& /*data*/)
{
    if (!pThis) return FALSE;
    return TRUE;
}
// CMFCRibbonButton::SetDescription(const wchar_t*) -- retail forwards to
// CMFCRibbonBaseElement::SetDescription and, when the flag at +0x210 is set,
// zeroes the +0x180/+0x188 pointers.  The base forwarding is implemented
// (ribbon_state side table); the flag/pointer members are not modeled.
// TODO(clean-room): transcribed partially -- the +0x180/+0x188 pointer reset
// behind the +0x210 flag is not modeled.
// Symbol: ?SetDescription@CMFCRibbonButton@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetDescription_CMFCRibbonButton__UEAAXPEB_W_Z(
    CMFCRibbonButton* pThis, const wchar_t* lpszText)
{
    if (!pThis) return;
    impl__SetDescription_CMFCRibbonBaseElement__UEAAXPEB_W_Z(pThis, lpszText);
}
// CMFCRibbonButton::SetMenu(UINT, int, int) -- retail loads the menu resource
// through the app resource handle, delegates to the HMENU overload
// (RVA 0xe52c0 (mfc140u)) and sets the resource-menu flag (+0x1f0).
// Menu/resource state is not modeled.
// TODO(clean-room): transcribed partially -- menu resource state is not
// modeled.
// Symbol: ?SetMenu@CMFCRibbonButton@@QEAAXIHH@Z
extern "C" void MS_ABI impl__SetMenu_CMFCRibbonButton__QEAAXIHH_Z(
    CMFCRibbonButton* pThis, unsigned int /*uiMenuResID*/, int /*x*/, int /*y*/)
{
    if (!pThis) return;
}
// CMFCRibbonButton::SetMenu(HMENU, int, int) -- retail (RVA 0xe52c0 (mfc140u),
// about 0x2c0 bytes) resets +0x21c/+0x1c4, releases a non-null owned menu
// (+0x1d0 when +0x1f0 is set), and clears +0x1f0. A global branch gate then
// either stores the supplied HMENU at +0x1d0 and returns, or parses its items
// into the sub-item CArray (+0x248/+0x250/+0x258) and updates ghost-menu state
// as applicable. None of this state is modeled.
// TODO(clean-room): transcribed partially -- menu/sub-item state is not
// modeled.
// Symbol: ?SetMenu@CMFCRibbonButton@@QEAAXPEAUHMENU__@@HH@Z
extern "C" void MS_ABI impl__SetMenu_CMFCRibbonButton__QEAAXPEAUHMENU____HH_Z(
    CMFCRibbonButton* pThis, HMENU /*hMenu*/, int /*x*/, int /*y*/)
{
    if (!pThis) return;
}
// CMFCRibbonButton::SetOriginal(CMFCRibbonBaseElement*) -- retail walks the
// pOriginal chain (+0xe8) to the last non-null link and stores it at +0xe8.
// If the source is a CMFCRibbonButton with the same sub-item count, it clones
// those sub-items into this button. The link and sub-item array are not modeled.
// TODO(clean-room): transcribed partially -- the original-link chain and
// sub-items are not modeled.
// Symbol: ?SetOriginal@CMFCRibbonButton@@MEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__SetOriginal_CMFCRibbonButton__MEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonButton* pThis, CMFCRibbonBaseElement* /*pOriginal*/)
{
    if (!pThis) return;
}
// CMFCRibbonButton::SetParentCategory(CMFCRibbonCategory*) -- retail stores
// the category at +0xe0 and propagates it to every sub-item (array
// +0x250/+0x258) through the virtual SetParentCategory (vtable+0x2d8), also
// setting each sub-item's flag at +0x160.  None of this state is modeled.
// TODO(clean-room): transcribed partially -- category/sub-item state is not
// modeled.
// Symbol: ?SetParentCategory@CMFCRibbonButton@@UEAAXPEAVCMFCRibbonCategory@@@Z
extern "C" void MS_ABI impl__SetParentCategory_CMFCRibbonButton__UEAAXPEAVCMFCRibbonCategory___Z(
    CMFCRibbonButton* pThis, CMFCRibbonCategory* /*pCategory*/)
{
    if (!pThis) return;
}
// CMFCRibbonButton::SetParentRibbonBar(CMFCRibbonBar*) -- retail stores the
// bar at +0xd8 and propagates it to every sub-item (array +0x250/+0x258)
// through the virtual SetParentRibbonBar (vtable+0x278).  None of this state
// is modeled.
// TODO(clean-room): transcribed partially -- ribbon-bar/sub-item state is not
// modeled.
// Symbol: ?SetParentRibbonBar@CMFCRibbonButton@@MEAAXPEAVCMFCRibbonBar@@@Z
extern "C" void MS_ABI impl__SetParentRibbonBar_CMFCRibbonButton__MEAAXPEAVCMFCRibbonBar___Z(
    CMFCRibbonButton* pThis, CMFCRibbonBar* /*pBar*/)
{
    if (!pThis) return;
}
CMFCRibbonButton::CMFCRibbonButton() {
    memset(_ribbonbutton_padding, 0, sizeof(_ribbonbutton_padding));
}
CMFCRibbonButton::CMFCRibbonButton(UINT nID, const wchar_t* lpszText, int, int, BOOL)
    : CMFCRibbonBaseElement() {
    SetID(nID);
    if (lpszText) SetText(lpszText);
    memset(_ribbonbutton_padding, 0, sizeof(_ribbonbutton_padding));
}
CMFCRibbonButton::CMFCRibbonButton(UINT nID, const wchar_t* lpszText, HICON, BOOL, HICON, BOOL, BOOL)
    : CMFCRibbonBaseElement() {
    SetID(nID);
    if (lpszText) SetText(lpszText);
    memset(_ribbonbutton_padding, 0, sizeof(_ribbonbutton_padding));
}
CMFCRibbonButton::~CMFCRibbonButton() {}
// Stub: mfc140u RVA 0xe84a0 reads the parent-category pointer, sub-item array
// (+0x248/+0x250/+0x258), and element state; these button fields are unmodeled.
// Symbol: ?AddSubItem@CMFCRibbonButton@@QEAAXPEAVCMFCRibbonBaseElement@@H@Z
extern "C" void MS_ABI impl__AddSubItem_CMFCRibbonButton__QEAAXPEAVCMFCRibbonBaseElement__H_Z(
    CMFCRibbonButton* pThis, CMFCRibbonBaseElement* pSubItem, int nIndex) {
    (void)pThis; (void)pSubItem; (void)nIndex;
}

// Stub: mfc140u RVA 0xe87a0 calls the base list-box handler; only when the
// deep flag is set and +0x218 is clear does it visit sub-items (+0x250/+0x258).
// Symbol: ?AddToListBox@CMFCRibbonButton@@MEAAHPEAVCMFCRibbonCommandsListBox@@H@Z
extern "C" int MS_ABI impl__AddToListBox_CMFCRibbonButton__MEAAHPEAVCMFCRibbonCommandsListBox__H_Z(
    CMFCRibbonButton* pThis, void* pWndListBox, int bDeep) {
    (void)pThis; (void)pWndListBox; (void)bDeep;
    return 0;
}

// Stub: mfc140u RVA 0xe8850 walks the sub-item array (+0x250/+0x258), calls
// each child's vtable byte offset +0x340, then tail-jumps to
// CMFCRibbonBaseElement::ClosePopupMenu (RVA 0x110a0, mfc140u). The declared
// button has neither the retail array layout nor matching virtual slots.
// Symbol: ?ClosePopupMenu@CMFCRibbonButton@@UEAAXXZ
extern "C" void MS_ABI impl__ClosePopupMenu_CMFCRibbonButton__UEAAXXZ(CMFCRibbonButton* pThis) {
    (void)pThis;
}

// Stub: mfc140u RVA 0xe5080 writes button state fields in +0x180..+0x218
// and calls the same indirect helper with addresses at +0x198 and +0x1a8.
// These retail fields are absent from the declared button layout.
// Symbol: ?CommonInit@CMFCRibbonButton@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonButton__IEAAXXZ(CMFCRibbonButton* pThis) {
    (void)pThis;
}

// Stub: mfc140u RVA 0xe7aa0 copies menu/icon, sizing, and sub-item state across
// the button layout; those fields are not modeled.
// Symbol: ?CopyFrom@CMFCRibbonButton@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonButton__MEAAXAEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonButton* pThis, const CMFCRibbonBaseElement* pSource) {
    (void)pThis; (void)pSource;
}

// Partial: mfc140u RVA 0xe97e0 measures/draws text using button and parent state
// that OpenMFC does not model. Initialize the MS ABI CSize result buffer safely.
// Symbol: ?DrawBottomText@CMFCRibbonButton@@QEAA?AVCSize@@PEAVCDC@@H@Z
extern "C" CSize* MS_ABI impl__DrawBottomText_CMFCRibbonButton__QEAA_AVCSize__PEAVCDC__H_Z(
    CMFCRibbonButton* /*pThis*/, CSize* pResult, CDC* /*pDC*/, int /*bCalcOnly*/) {
    BuildCSizeResult(pResult, 0, 0);
    return pResult;
}

// Stub: mfc140u RVA 0xe7dd0 follows the original-element link and uses button
// flags/geometry while drawing; those fields are not modeled.
// Symbol: ?DrawImage@CMFCRibbonButton@@UEAAXPEAVCDC@@W4RibbonImageType@CMFCRibbonBaseElement@@VCRect@@@Z
extern "C" void MS_ABI impl__DrawImage_CMFCRibbonButton__UEAAXPEAVCDC__W4RibbonImageType_CMFCRibbonBaseElement__VCRect___Z(
    CMFCRibbonButton* pThis, CDC* pDC, int type, const CRect* rectImage) {
    (void)pThis; (void)pDC; (void)type; (void)rectImage;
}

// Stub: mfc140u RVA 0xe8a30 checks text-rendering context and performs text
// drawing; the required object/layout and CDC behavior are not modeled.
// Symbol: ?DrawRibbonText@CMFCRibbonButton@@UEAAHPEAVCDC@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@VCRect@@IK@Z
extern "C" int MS_ABI impl__DrawRibbonText_CMFCRibbonButton__UEAAHPEAVCDC__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__VCRect__IK_Z(
    CMFCRibbonButton* pThis, CDC* pDC, const CString* strText,
    const CRect* rectText, unsigned int uiDTFlags, unsigned long clrText) {
    (void)pThis; (void)pDC; (void)strText; (void)rectText; (void)uiDTFlags; (void)clrText;
    return 0;
}

// Stub: mfc140u RVA 0xe9430 clears/rebuilds window-list sub-items, traversing
// the array at +0x248/+0x250/+0x258 and using button/window state. That array
// and the required retail button fields are absent from the declared layout.
// Symbol: ?FillWindowList@CMFCRibbonButton@@IEAAXXZ
extern "C" void MS_ABI impl__FillWindowList_CMFCRibbonButton__IEAAXXZ(CMFCRibbonButton* pThis) {
    (void)pThis;
}

// Stub: mfc140u RVA 0xe8950 compares base data (+0x110), then searches the
// sub-item array recursively; base data and button sub-items are not modeled.
// Symbol: ?FindByData@CMFCRibbonButton@@MEAAPEAVCMFCRibbonBaseElement@@_K@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByData_CMFCRibbonButton__MEAAPEAVCMFCRibbonBaseElement___K_Z(
    CMFCRibbonButton* pThis, unsigned __int64 dwData) {
    (void)pThis; (void)dwData;
    return nullptr;
}

// Partial: mfc140u RVA 0xe88c0 checks this element's ID before searching its
// sub-items. OpenMFC can check its modeled own ID but has no sub-item storage.
// Symbol: ?FindByID@CMFCRibbonButton@@MEAAPEAVCMFCRibbonBaseElement@@I@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByID_CMFCRibbonButton__MEAAPEAVCMFCRibbonBaseElement__I_Z(
    CMFCRibbonButton* pThis, unsigned int uiCmdID) {
    return impl__FindByID_CMFCRibbonBaseElement__UEAAPEAV1_I_Z(pThis, uiCmdID);
}

// Stub: mfc140u RVA 0xe8560 scans the sub-item array (+0x250/+0x258) and
// compares each element's ID at +0x118; that layout is not modeled.
// Symbol: ?FindSubItemIndexByID@CMFCRibbonButton@@QEBAHI@Z
extern "C" int MS_ABI impl__FindSubItemIndexByID_CMFCRibbonButton__QEBAHI_Z(
    const CMFCRibbonButton* pThis, unsigned int uiID) {
    (void)pThis; (void)uiID;
    return -1;
}

// Partial: mfc140u RVA 0xe9110 appends this element, then recursively appends
// sub-items. OpenMFC has no sub-item storage, but can append the button itself.
// Symbol: ?GetElements@CMFCRibbonButton@@MEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetElements_CMFCRibbonButton__MEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonButton* pThis,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& elements) {
    if (pThis) elements.Add(pThis);
}

// Partial: mfc140u RVA 0xe90a0 delegates to the base ID search and visits
// sub-items; OpenMFC can append the button itself but has no sub-item storage.
// Symbol: ?GetElementsByID@CMFCRibbonButton@@MEAAXIAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetElementsByID_CMFCRibbonButton__MEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonButton* pThis, unsigned int uiCmdID,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& elements) {
    if (pThis) {
        impl__GetElementsByID_CMFCRibbonBaseElement__UEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
            pThis, uiCmdID, elements);
    }
}

// Stub: mfc140u RVA 0xe9790 reads this+0xf0, the referenced object's +0x190,
// and conditionally this+0x100 to compute 0, 1, or 2. Those base/button/group
// fields are not represented at the retail offsets in the declared layout.
// Symbol: ?GetGroupButtonExtraWidth@CMFCRibbonButton@@IEAAHXZ
extern "C" int MS_ABI impl__GetGroupButtonExtraWidth_CMFCRibbonButton__IEAAHXZ(CMFCRibbonButton* pThis) {
    (void)pThis;
    return 0;
}

// Partial: mfc140u RVA 0xe81c0 uses original-element, icon and resource state
// that is not modeled. Initialize its MS ABI CSize result buffer safely.
// Symbol: ?GetImageSize@CMFCRibbonButton@@UEBA?AVCSize@@W4RibbonImageType@CMFCRibbonBaseElement@@@Z
extern "C" CSize* MS_ABI impl__GetImageSize_CMFCRibbonButton__UEBA_AVCSize__W4RibbonImageType_CMFCRibbonBaseElement___Z(
    const CMFCRibbonButton* /*pThis*/, CSize* pResult, int /*type*/) {
    BuildCSizeResult(pResult, 0, 0);
    return pResult;
}

// Partial: mfc140u RVA 0xe8c20 calls a virtual sizing method and reads button
// geometry that is not modeled. Initialize its MS ABI CRect result buffer safely.
// Symbol: ?GetKeyTipRect@CMFCRibbonButton@@MEAA?AVCRect@@PEAVCDC@@H@Z
extern "C" CRect* MS_ABI impl__GetKeyTipRect_CMFCRibbonButton__MEAA_AVCRect__PEAVCDC__H_Z(
    CMFCRibbonButton* /*pThis*/, CRect* pResult, CDC* /*pDC*/, int /*bIsMenu*/) {
    if (!pResult) return nullptr;
    return new (pResult) CRect();
}

// Partial: mfc140u RVA 0xe89e0 can substitute a button-specific tooltip
// string based on unmodeled flags. Use the base implementation's safe fallback.
// Symbol: ?GetToolTipText@CMFCRibbonButton@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" CString* MS_ABI impl__GetToolTipText_CMFCRibbonButton__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CMFCRibbonButton* pThis, CString* pResult) {
    if (!pResult) return nullptr;
    impl__GetToolTipText_CMFCRibbonBaseElement__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
        pResult, pThis);
    return pResult;
}

// Stub: mfc140u RVA 0xe7500 calls NotifyCommand(this, 1) and returns if it is
// nonzero; otherwise it calls virtual byte slot +0x280, then calls +0x288 only
// if the first slot returns nonzero. The NotifyCommand thunk is itself a stub,
// and this declaration does not model the retail vtable slots.
// Symbol: ?OnAccDefaultAction@CMFCRibbonButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnAccDefaultAction_CMFCRibbonButton__UEAAXXZ(CMFCRibbonButton* pThis) {
    (void)pThis;
}

// Stub: mfc140u RVA 0xe91b0 reads base strings and calls virtual slots +0x368,
// +0x4b0 and +0x238, then updates button state including +0x214; unmodeled here.
// Symbol: ?OnAfterChangeRect@CMFCRibbonButton@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonButton__MEAAXPEAVCDC___Z(
    CMFCRibbonButton* pThis, CDC* pDC) {
    (void)pThis; (void)pDC;
}

// Stub: mfc140u RVA 0xe7480 checks interaction/highlight state, the command
// rect (+0x1a8), and parent-menu state (+0xf8); those fields are not modeled.
// CPoint is passed as an 8-byte MS ABI aggregate in RDX.
// Symbol: ?OnClick@CMFCRibbonButton@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnClick_CMFCRibbonButton__UEAAXVCPoint___Z(
    CMFCRibbonButton* pThis, long long point) {
    (void)pThis; (void)point;
}

// Stub: mfc140u RVA 0xe6750 reads button flags/geometry and invokes virtual
// sizing/drawing methods; the button and text-rendering state are not modeled.
// Symbol: ?OnDrawOnList@CMFCRibbonButton@@MEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonButton__MEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(
    CMFCRibbonButton* pThis, CDC* pDC, CString* strText, int nTextOffset,
    const CRect* rect, int bIsSelected, int bHighlighted) {
    (void)pThis; (void)pDC; (void)strText; (void)nTextOffset;
    (void)rect; (void)bIsSelected; (void)bHighlighted;
}

// Stub: the ANSI twin's body at RVA 0xe9b40 (mfc140) dispatches through
// several virtual slots and reads button/menu state; the mfc140u symbol is
// unresolved by the supplied map, and that state/slot layout is not modeled.
// Symbol: ?OnKey@CMFCRibbonButton@@MEAAHH@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonButton__MEAAHH_Z(
    CMFCRibbonButton* pThis, int bIsMenuKey) {
    (void)pThis; (void)bIsMenuKey;
    return 0;
}
