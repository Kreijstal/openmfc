// CMFCRibbonStatusBar — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

CMFCRibbonStatusBar::CMFCRibbonStatusBar() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonStatusBar::~CMFCRibbonStatusBar() {}

// The assigned methods below remain explicit stubs: retail CMFCRibbonStatusBar
// derives from CMFCRibbonBar and stores its own arrays after that base. The
// OpenMFC declaration in include/openmfc/afxmfc.h instead derives from CBasePane
// and provides only a 64-byte pad, so these retail fields are not safely
// addressable through an OpenMFC-constructed object. See headerRequests.

// Symbol: ?AddDynamicElement@CMFCRibbonStatusBar@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__AddDynamicElement_CMFCRibbonStatusBar__QEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonStatusBar* pThis, CMFCRibbonBaseElement* pElement) {}

// Retail RVA 0x125440 (mfc140u) reads and writes the status-bar arrays at
// +0x2168/+0x2118 and sets the element's visibility. The OpenMFC class layout
// does not contain those retail fields.
// Symbol: ?AddElement@CMFCRibbonStatusBar@@QEAAXPEAVCMFCRibbonBaseElement@@PEB_WH@Z
extern "C" void MS_ABI impl__AddElement_CMFCRibbonStatusBar__QEAAXPEAVCMFCRibbonBaseElement__PEB_WH_Z(
    CMFCRibbonStatusBar* pThis, CMFCRibbonBaseElement* pElement,
    const wchar_t* pszLabel, BOOL bVisible) {}

// Retail RVA 0x1254d0 (mfc140u) stores extended elements at +0x2190 and
// updates label/element state; that layout is absent from the OpenMFC class.
// Symbol: ?AddExtendedElement@CMFCRibbonStatusBar@@QEAAXPEAVCMFCRibbonBaseElement@@PEB_WH@Z
extern "C" void MS_ABI impl__AddExtendedElement_CMFCRibbonStatusBar__QEAAXPEAVCMFCRibbonBaseElement__PEB_WH_Z(
    CMFCRibbonStatusBar* pThis, CMFCRibbonBaseElement* pElement,
    const wchar_t* pszLabel, BOOL bVisible) {}

// Retail RVA 0x125590 (mfc140u) allocates and appends a separator element;
// the element construction and array layout are not represented here.
// Symbol: ?AddSeparator@CMFCRibbonStatusBar@@QEAAXXZ
extern "C" void MS_ABI impl__AddSeparator_CMFCRibbonStatusBar__QEAAXXZ(
    CMFCRibbonStatusBar* pThis) {}

// Retail RVA 0x125180 (mfc140u) performs window/DC-dependent fixed layout and
// reads the extended-element array. Neither the inherited retail layout nor
// the array representation is available in this class declaration.
// Symbol: ?CalcFixedLayout@CMFCRibbonStatusBar@@MEAA?AVCSize@@HH@Z
extern "C" CSize MS_ABI impl__CalcFixedLayout_CMFCRibbonStatusBar__MEAA_AVCSize__HH_Z(
    CMFCRibbonStatusBar* pThis, int bStretch, int bHorz) {
    return CSize(0, 0);
}

// Retail RVA 0x126d40 (mfc140u) destroys elements in the customize-item array
// at +0x21b8; its owning array layout is unavailable here.
// Symbol: ?CleanUpCustomizeItems@CMFCRibbonStatusBar@@IEAAXXZ
extern "C" void MS_ABI impl__CleanUpCustomizeItems_CMFCRibbonStatusBar__IEAAXXZ(
    CMFCRibbonStatusBar* pThis) {}

// Retail Create forwards to CreateEx; CreateEx uses the inherited CMFCRibbonBar
// vtable/layout and window-creation infrastructure not modeled by the declared
// OpenMFC CBasePane-based class.
// Symbol: ?Create@CMFCRibbonStatusBar@@QEAAHPEAVCWnd@@KI@Z
extern "C" int MS_ABI impl__Create_CMFCRibbonStatusBar__QEAAHPEAVCWnd__KI_Z(
    CMFCRibbonStatusBar* pThis, CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    return 0;
}

// Retail RVA 0x125000 (mfc140u) accesses the base vtable at slot 0x7a and
// initializes window/status-bar state; the required CMFCRibbonBar base is absent.
// Symbol: ?CreateEx@CMFCRibbonStatusBar@@QEAAHPEAVCWnd@@KKI@Z
extern "C" int MS_ABI impl__CreateEx_CMFCRibbonStatusBar__QEAAHPEAVCWnd__KKI_Z(
    CMFCRibbonStatusBar* pThis, CWnd* pParentWnd, DWORD dwCtrlStyle,
    DWORD dwStyle, UINT nID) {
    return 0;
}

// Retail RVA 0x125a30 (mfc140u) searches normal and extended element arrays,
// comparing each element's command ID at +0x118. Those arrays are absent here.
// Symbol: ?FindElement@CMFCRibbonStatusBar@@QEAAPEAVCMFCRibbonBaseElement@@I@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindElement_CMFCRibbonStatusBar__QEAAPEAVCMFCRibbonBaseElement__I_Z(
    CMFCRibbonStatusBar* pThis, UINT uiID) {
    return nullptr;
}

// No resolved RVA is available for this symbol in mfc140u, so its body cannot
// be verified; the declared OpenMFC object also lacks the retail base layout.
// Symbol: ?get_accChildCount@CMFCRibbonStatusBar@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCRibbonStatusBar__MEAAJPEAJ_Z(
    CMFCRibbonStatusBar* pThis, long* pcountChildren) {
    return 0;
}

// Retail RVA 0x125990 (mfc140u) returns the array count stored at +0x2178;
// OpenMFC's declared class has no such retail member/storage.
// Symbol: ?GetCount@CMFCRibbonStatusBar@@QEBAHXZ
extern "C" int MS_ABI impl__GetCount_CMFCRibbonStatusBar__QEBAHXZ(
    const CMFCRibbonStatusBar* pThis) {
    return 0;
}

// Retail RVA 0x1267c0 (mfc140u) searches both arrays and calls the elements'
// GetDroppedDown virtual; the required arrays and compatible ribbon base are
// unavailable in the OpenMFC class.
// Symbol: ?GetDroppedDown@CMFCRibbonStatusBar@@MEAAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetDroppedDown_CMFCRibbonStatusBar__MEAAPEAVCMFCRibbonBaseElement__XZ(
    CMFCRibbonStatusBar* pThis) {
    return nullptr;
}

// Retail RVA 0x1259b0 (mfc140u) bounds-checks against +0x2178 and indexes the
// normal-element array at +0x2170, neither of which is represented here.
// Symbol: ?GetElement@CMFCRibbonStatusBar@@QEAAPEAVCMFCRibbonBaseElement@@H@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetElement_CMFCRibbonStatusBar__QEAAPEAVCMFCRibbonBaseElement__H_Z(
    CMFCRibbonStatusBar* pThis, int nIndex) {
    return nullptr;
}

// Retail RVA 0x1259a0 (mfc140u) returns the extended-element count at +0x21a0;
// that retail member is absent from the declared OpenMFC object.
// Symbol: ?GetExCount@CMFCRibbonStatusBar@@QEBAHXZ
extern "C" int MS_ABI impl__GetExCount_CMFCRibbonStatusBar__QEBAHXZ(
    const CMFCRibbonStatusBar* pThis) {
    return 0;
}

// Retail RVA 0x1259f0 (mfc140u) bounds-checks against +0x21a0 and indexes the
// extended-element array at +0x2198, neither of which is represented here.
// Symbol: ?GetExElement@CMFCRibbonStatusBar@@QEAAPEAVCMFCRibbonBaseElement@@H@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetExElement_CMFCRibbonStatusBar__QEAAPEAVCMFCRibbonBaseElement__H_Z(
    CMFCRibbonStatusBar* pThis, int nIndex) {
    return nullptr;
}

// Retail RVA 0x125ad0 (mfc140u) scans the extended array and reads element
// rectangles at +0xc8; the status-bar array layout is not available here.
// Symbol: ?GetExtendedArea@CMFCRibbonStatusBar@@UEBAHAEAVCRect@@@Z
extern "C" int MS_ABI impl__GetExtendedArea_CMFCRibbonStatusBar__UEBAHAEAVCRect___Z(
    const CMFCRibbonStatusBar* pThis, CRect& rect) {
    return 0;
}

// Retail RVA 0x126560 (mfc140u) scans normal and extended arrays, tests each
// element rectangle at +0xc8 against the point, then calls a virtual on a hit.
// Those arrays and their representation are absent from this OpenMFC class.
// CPoint is the 8-byte by-value argument in the MS ABI.
// Symbol: ?HitTest@CMFCRibbonStatusBar@@MEAAPEAVCMFCRibbonBaseElement@@VCPoint@@HH@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__HitTest_CMFCRibbonStatusBar__MEAAPEAVCMFCRibbonBaseElement__VCPoint__HH_Z(
    CMFCRibbonStatusBar* pThis, long long point, BOOL bCheckActiveCategory,
    BOOL bCheckPanelCaption) {
    return nullptr;
}

// Retail RVA 0x1273a0 (mfc140u) compares the supplied pointer against elements
// in the extended array at +0x2198; that array is absent here.
// Symbol: ?IsExtendedElement@CMFCRibbonStatusBar@@QEBAHPEAVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__IsExtendedElement_CMFCRibbonStatusBar__QEBAHPEAVCMFCRibbonBaseElement___Z(
    const CMFCRibbonStatusBar* pThis, CMFCRibbonBaseElement* pElement) {
    return 0;
}

// Retail RVA 0x127040 (mfc140u) loads profile state, updates the element arrays,
// and depends on MFC profile/window infrastructure and the retail object layout.
// Symbol: ?LoadState@CMFCRibbonStatusBar@@MEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CMFCRibbonStatusBar__MEAAHPEB_WHI_Z(
    CMFCRibbonStatusBar* pThis, const wchar_t* pszProfileName, int nIndex,
    UINT uiID) {
    return 0;
}

// Retail RVA 0x127510 (mfc140u) draws the supplied information string in the
// supplied rectangle using CDC/font/text helper calls; those dependencies are
// not transcribed here.
// Symbol: ?OnDrawInformation@CMFCRibbonStatusBar@@MEAAXPEAVCDC@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawInformation_CMFCRibbonStatusBar__MEAAXPEAVCDC__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__VCRect___Z(
    CMFCRibbonStatusBar* pThis, CDC* pDC, CString& strInfo, CRect rect) {}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// CPoint is the 8-byte by-value argument in the MS ABI.
// Symbol: ?OnNcHitTest@CMFCRibbonStatusBar@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMFCRibbonStatusBar__IEAA_JVCPoint___Z(
    CMFCRibbonStatusBar* pThis, long long point) {
    return 0;
}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// Symbol: ?OnPaint@CMFCRibbonStatusBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCRibbonStatusBar__IEAAXXZ(
    CMFCRibbonStatusBar* pThis) {}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// CPoint is the 8-byte by-value argument in the MS ABI.
// Symbol: ?OnPaneContextMenu@CMFCRibbonStatusBar@@MEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnPaneContextMenu_CMFCRibbonStatusBar__MEAAXPEAVCWnd__VCPoint___Z(
    CMFCRibbonStatusBar* pThis, CWnd* pParentFrame, long long point) {}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// Symbol: ?OnRTLChanged@CMFCRibbonStatusBar@@MEAAXH@Z
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonStatusBar__MEAAXH_Z(
    CMFCRibbonStatusBar* pThis, BOOL bIsRTL) {}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// Symbol: ?OnShowWindow@CMFCRibbonStatusBar@@IEAAXHI@Z
extern "C" void MS_ABI impl__OnShowWindow_CMFCRibbonStatusBar__IEAAXHI_Z(
    CMFCRibbonStatusBar* pThis, BOOL bShow, UINT nStatus) {}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// Symbol: ?OnSize@CMFCRibbonStatusBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCRibbonStatusBar__IEAAXIHH_Z(
    CMFCRibbonStatusBar* pThis, UINT nType, int cx, int cy) {}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// Symbol: ?OnSysCommand@CMFCRibbonStatusBar@@IEAAXI_J@Z
extern "C" void MS_ABI impl__OnSysCommand_CMFCRibbonStatusBar__IEAAXI_J_Z(
    CMFCRibbonStatusBar* pThis, UINT nID, LPARAM lParam) {}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// Symbol: ?OnUpdateCmdUI@CMFCRibbonStatusBar@@MEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonStatusBar__MEAAXPEAVCFrameWnd__H_Z(
    CMFCRibbonStatusBar* pThis, CFrameWnd* pTarget, BOOL bDisableIfNoHandler) {}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// Symbol: ?OnUpdateShadows@CMFCRibbonStatusBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateShadows_CMFCRibbonStatusBar__IEAA_J_K_J_Z(
    CMFCRibbonStatusBar* pThis, WPARAM wParam, LPARAM lParam) {
    return 0;
}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// Symbol: ?PreCreateWindow@CMFCRibbonStatusBar@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMFCRibbonStatusBar__MEAAHAEAUtagCREATESTRUCTW___Z(
    CMFCRibbonStatusBar* pThis, CREATESTRUCTW& cs) {
    return 0;
}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// Symbol: ?RecalcLayout@CMFCRibbonStatusBar@@UEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CMFCRibbonStatusBar__UEAAXXZ(
    CMFCRibbonStatusBar* pThis) {}

// No resolved RVA is available for this symbol in mfc140u; the retail methods
// that expose its element arrays access fields absent from this declaration.
// Symbol: ?RemoveAll@CMFCRibbonStatusBar@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMFCRibbonStatusBar__QEAAXXZ(
    CMFCRibbonStatusBar* pThis) {}

// No resolved RVA is available for this symbol in mfc140u; the normal and
// extended arrays accessed by neighboring retail methods are absent here.
// Symbol: ?RemoveElement@CMFCRibbonStatusBar@@QEAAHI@Z
extern "C" int MS_ABI impl__RemoveElement_CMFCRibbonStatusBar__QEAAHI_Z(
    CMFCRibbonStatusBar* pThis, UINT uiID) {
    return 0;
}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// cannot be verified against the declared OpenMFC layout.
// Symbol: ?SaveState@CMFCRibbonStatusBar@@MEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CMFCRibbonStatusBar__MEAAHPEB_WHI_Z(
    CMFCRibbonStatusBar* pThis, const wchar_t* pszProfileName, int nIndex,
    UINT uiID) {
    return 0;
}

// No resolved RVA is available for this symbol in mfc140u, so its behavior
// and required state cannot be verified against the declared OpenMFC layout.
// Symbol: ?SetInformation@CMFCRibbonStatusBar@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetInformation_CMFCRibbonStatusBar__QEAAXPEB_W_Z(
    CMFCRibbonStatusBar* pThis, const wchar_t* pszInfo) {}
