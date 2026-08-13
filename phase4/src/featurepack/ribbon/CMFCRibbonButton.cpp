// CMFCRibbonButton — OpenMFC implementation.
// Sources: cbarcore.cpp, global_mfc-feature-22_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCRibbonBaseElementSupport.h"
#include "detail/MfccoreSupport.h"

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
// rects (+0x198/+0x1a8), reads the mode flags (+0x148/+0x13c/+0x1ec) and
// dispatches to the base OnLButtonDown / virtual IsDisabled (vtable+0x280)
// and OnShowPopupMenu (vtable+0x288).  None of these members or vtable slots
// exist in the clean-room declaration.
// TODO(clean-room): transcribed partially -- button geometry/menu state is
// not modeled.
// Symbol: ?OnLButtonDown@CMFCRibbonButton@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonButton__MEAAXVCPoint___Z(
    CMFCRibbonButton* pThis, CPoint /*pt*/)
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
    CMFCRibbonButton* pThis, CPoint /*pt*/)
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
    CMFCRibbonButton* pThis, CPoint /*pt*/)
{
    if (!pThis) return;
}
// CMFCRibbonButton::OnShowPopupMenu() -- retail (RVA 0xe7550, 0x500+ bytes)
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
// sub-item count (+0x258), deletes the item and erases it from the CArray,
// returning TRUE on success and FALSE for an out-of-range index.  The
// sub-item array is not modeled, so nothing can be removed; FALSE is the
// truthful result.
// TODO(clean-room): transcribed partially -- sub-items are not modeled.
// Symbol: ?RemoveSubItem@CMFCRibbonButton@@QEAAHH@Z
extern "C" int MS_ABI impl__RemoveSubItem_CMFCRibbonButton__QEAAHH_Z(
    CMFCRibbonButton* pThis, int /*nIndex*/)
{
    if (!pThis) return FALSE;
    return FALSE;
}
// CMFCRibbonButton::SetACCData(CWnd*, CAccessibilityData&) -- retail first
// delegates to CMFCRibbonBaseElement::SetACCData (0x180011200, which clears
// the block), then sets m_nAccRole from a role-string lookup and m_bAccState
// from a virtual result (vtable+0x4b8).  The role/state computation and the
// virtual slot are not modeled; the retail function always returns TRUE.
// TODO(clean-room): transcribed partially -- accessibility role/state data is
// not modeled.
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
// (0x1800e52c0) and sets the "menu was set from resource" flag (+0x1f0).
// Menu/resource state is not modeled.
// TODO(clean-room): transcribed partially -- menu resource state is not
// modeled.
// Symbol: ?SetMenu@CMFCRibbonButton@@QEAAXIHH@Z
extern "C" void MS_ABI impl__SetMenu_CMFCRibbonButton__QEAAXIHH_Z(
    CMFCRibbonButton* pThis, unsigned int /*uiMenuResID*/, int /*x*/, int /*y*/)
{
    if (!pThis) return;
}
// CMFCRibbonButton::SetMenu(HMENU, int, int) -- retail (RVA 0xe52c0, ~0x3a0
// bytes) resets +0x21c/+0x1c4, releases a previously owned menu (+0x1d0 when
// +0x1f0 set), stores the new HMENU at +0x1d0, enumerates the menu items
// (GetMenuItemCount/GetMenuItemID/GetSubMenu) into the sub-item array at
// +0x1a0 and rebuilds ghost-menu state.  None of this state is modeled.
// TODO(clean-room): transcribed partially -- menu/sub-item state is not
// modeled.
// Symbol: ?SetMenu@CMFCRibbonButton@@QEAAXPEAUHMENU__@@HH@Z
extern "C" void MS_ABI impl__SetMenu_CMFCRibbonButton__QEAAXPEAUHMENU____HH_Z(
    CMFCRibbonButton* pThis, HMENU /*hMenu*/, int /*x*/, int /*y*/)
{
    if (!pThis) return;
}
// CMFCRibbonButton::SetOriginal(CMFCRibbonBaseElement*) -- retail walks the
// pOriginal chain (+0xe8) to the last non-null link, stores it at +0xe8, and
// when the source is a CMFCRibbonButton clones its sub-items into this
// button.  The pOriginal member and sub-item array are not modeled.
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
// Symbol: ?AddSubItem@CMFCRibbonButton@@QEAAXPEAVCMFCRibbonBaseElement@@H@Z
extern "C" void MS_ABI impl__AddSubItem_CMFCRibbonButton__QEAAXPEAVCMFCRibbonBaseElement__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?AddToListBox@CMFCRibbonButton@@MEAAHPEAVCMFCRibbonCommandsListBox@@H@Z
extern "C" int MS_ABI impl__AddToListBox_CMFCRibbonButton__MEAAHPEAVCMFCRibbonCommandsListBox__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?ClosePopupMenu@CMFCRibbonButton@@UEAAXXZ
extern "C" void MS_ABI impl__ClosePopupMenu_CMFCRibbonButton__UEAAXXZ() {}

// Symbol: ?CommonInit@CMFCRibbonButton@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonButton__IEAAXXZ() {}

// Symbol: ?CopyFrom@CMFCRibbonButton@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonButton__MEAAXAEBVCMFCRibbonBaseElement___Z(const void* /*class*/* p0) {}

// Symbol: ?DrawBottomText@CMFCRibbonButton@@QEAA?AVCSize@@PEAVCDC@@H@Z
extern "C" void* MS_ABI impl__DrawBottomText_CMFCRibbonButton__QEAA_AVCSize__PEAVCDC__H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {
    return nullptr;
}

// Symbol: ?DrawImage@CMFCRibbonButton@@UEAAXPEAVCDC@@W4RibbonImageType@CMFCRibbonBaseElement@@VCRect@@@Z
extern "C" void MS_ABI impl__DrawImage_CMFCRibbonButton__UEAAXPEAVCDC__W4RibbonImageType_CMFCRibbonBaseElement__VCRect___Z(void* /*class*/* p0, int /*enum*/ p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, unsigned int p8, void* p9, void* p10, void* p11, void* p12, void* p13, void* p14, void* p15, void* p16, void* p17, float p18, short p19, void* p20, void* p21, void* p22, void* p23, void* p24, void* p25, void* p26, void* p27, void* p28, void* p29, void* p30, unsigned char p31, void* p32, void* p33, void* p34, void* p35, void* p36, void* p37, void* /*class*/ p38) {}

// Symbol: ?DrawRibbonText@CMFCRibbonButton@@UEAAHPEAVCDC@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@VCRect@@IK@Z
extern "C" int MS_ABI impl__DrawRibbonText_CMFCRibbonButton__UEAAHPEAVCDC__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__VCRect__IK_Z(void* /*class*/* p0, const void* /*class*/* p1, void** p2, void* p3, void* /*class*/ p4, unsigned int p5, unsigned long p6) {
    return 0;
}

// Symbol: ?FillWindowList@CMFCRibbonButton@@IEAAXXZ
extern "C" void MS_ABI impl__FillWindowList_CMFCRibbonButton__IEAAXXZ() {}

// Symbol: ?FindByData@CMFCRibbonButton@@MEAAPEAVCMFCRibbonBaseElement@@_K@Z
extern "C" void* MS_ABI impl__FindByData_CMFCRibbonButton__MEAAPEAVCMFCRibbonBaseElement___K_Z(unsigned __int64 p0) {
    return nullptr;
}

// Symbol: ?FindByID@CMFCRibbonButton@@MEAAPEAVCMFCRibbonBaseElement@@I@Z
extern "C" void* MS_ABI impl__FindByID_CMFCRibbonButton__MEAAPEAVCMFCRibbonBaseElement__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?FindSubItemIndexByID@CMFCRibbonButton@@QEBAHI@Z
extern "C" int MS_ABI impl__FindSubItemIndexByID_CMFCRibbonButton__QEBAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?GetElements@CMFCRibbonButton@@MEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetElements_CMFCRibbonButton__MEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?GetElementsByID@CMFCRibbonButton@@MEAAXIAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetElementsByID_CMFCRibbonButton__MEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(unsigned int p0, void* /*class*/* p1, void* /*class*/* p2) {}

// Symbol: ?GetGroupButtonExtraWidth@CMFCRibbonButton@@IEAAHXZ
extern "C" int MS_ABI impl__GetGroupButtonExtraWidth_CMFCRibbonButton__IEAAHXZ() {
    return 0;
}

// Symbol: ?GetImageSize@CMFCRibbonButton@@UEBA?AVCSize@@W4RibbonImageType@CMFCRibbonBaseElement@@@Z
extern "C" void* MS_ABI impl__GetImageSize_CMFCRibbonButton__UEBA_AVCSize__W4RibbonImageType_CMFCRibbonBaseElement___Z(void* /*class*/* p0, int /*enum*/ p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, unsigned int p8, void* p9, void* p10, void* p11, void* p12, void* p13, void* p14, void* p15, void* p16, void* p17, float p18, short p19, void* p20, void* p21, void* p22, void* p23, void* p24, void* p25, void* p26, void* p27, void* p28, void* p29, void* p30, unsigned char p31, void* p32, void* p33, void* p34, void* p35, void* p36, void* p37) {
    return nullptr;
}

// Symbol: ?GetKeyTipRect@CMFCRibbonButton@@MEAA?AVCRect@@PEAVCDC@@H@Z
extern "C" void* MS_ABI impl__GetKeyTipRect_CMFCRibbonButton__MEAA_AVCRect__PEAVCDC__H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {
    return nullptr;
}

// Symbol: ?GetToolTipText@CMFCRibbonButton@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* MS_ABI impl__GetToolTipText_CMFCRibbonButton__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(void* /*class*/* p0, void** p1, void* p2) {
    return nullptr;
}

// Symbol: ?OnAccDefaultAction@CMFCRibbonButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnAccDefaultAction_CMFCRibbonButton__UEAAXXZ() {}

// Symbol: ?OnAfterChangeRect@CMFCRibbonButton@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonButton__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?OnClick@CMFCRibbonButton@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnClick_CMFCRibbonButton__UEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?OnDrawOnList@CMFCRibbonButton@@MEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonButton__MEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(void* /*class*/* p0, void* /*class*/ p1, void** p2, void* p3, int p4, void* /*class*/ p5, int p6, int p7) {}

// Symbol: ?OnKey@CMFCRibbonButton@@MEAAHH@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonButton__MEAAHH_Z(int p0) {
    return 0;
}
