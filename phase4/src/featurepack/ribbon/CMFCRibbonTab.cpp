// CMFCRibbonTab — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

CMFCRibbonTab::CMFCRibbonTab() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonTab::~CMFCRibbonTab() {}

// Retail mfc140u RVA 0xeca40 checks the output pointer, then writes VT_I4 / 0.
// The coordinates and this pointer do not affect the result.
// Symbol: ?accHitTest@CMFCRibbonTab@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCRibbonTab__UEAAJJJPEAUtagVARIANT___Z(
    CMFCRibbonTab* /*pThis*/, long /*xLeft*/, long /*yTop*/, VARIANT* pvarChild)
{
    if (!pvarChild) return E_INVALIDARG;
    pvarChild->vt = VT_I4;
    pvarChild->lVal = 0;
    return S_OK;
}

// Retail mfc140u RVA 0xecae0 needs the element rectangle plus its parent
// category's ribbon-bar/tab-group state; those layouts are not modeled here.
// Symbol: ?accLocation@CMFCRibbonTab@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCRibbonTab__UEAAJPEAJ000UtagVARIANT___Z(
    CMFCRibbonTab* /*pThis*/, long* /*pxLeft*/, long* /*pyTop*/, long* /*pcxWidth*/,
    long* /*pcyHeight*/, VARIANT /*varChild*/)
{
    return 0;
}

// Retail mfc140u RVA 0xecc40 walks the parent ribbon bar's tab group (reached
// through the category) and returns an accessibility dispatch for an adjacent
// tab; that parent/group layout is not modeled here.
// Symbol: ?accNavigate@CMFCRibbonTab@@UEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonTab__UEAAJJUtagVARIANT__PEAU2__Z(
    CMFCRibbonTab* /*pThis*/, long /*navDir*/, VARIANT /*varStart*/, VARIANT* /*pvarEndUpAt*/)
{
    return 0;
}

// Retail mfc140u RVA 0xec2f0 delegates to the base CopyFrom thunk, then copies
// fields at +0x180 and +0x188. The local class declaration has no shadow fields
// pinned at those offsets, so this remains a documented stub.
// Symbol: ?CopyFrom@CMFCRibbonTab@@UEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonTab__UEAAXAEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonTab* /*pThis*/, const CMFCRibbonBaseElement& /*src*/)
{
}

// Retail mfc140u RVA 0xecbc0 returns the literal "Switch" as a BSTR for
// CHILDID_SELF; the allocation/ownership path is not represented here.
// Symbol: ?get_accDefaultAction@CMFCRibbonTab@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDefaultAction_CMFCRibbonTab__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCRibbonTab* /*pThis*/, VARIANT /*varChild*/, wchar_t** /*pszDefaultAction*/)
{
    return 0;
}

// Retail mfc140u RVA 0xeca60 AddRefs the parent ribbon bar's tab group
// (reached through the category) and obtains its accessible interface; that
// parent/group state is not modeled here.
// Symbol: ?get_accParent@CMFCRibbonTab@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonTab__UEAAJPEAPEAUIDispatch___Z(
    CMFCRibbonTab* /*pThis*/, IDispatch** /*ppdispParent*/)
{
    return 0;
}

// Retail mfc140u RVA 0xec4c0 derives the key-tip rectangle from the element
// rect, category state, and CDC virtual/metric calls; that layout/rendering
// state is not modeled here.
// Symbol: ?GetKeyTipRect@CMFCRibbonTab@@UEAA?AVCRect@@PEAVCDC@@H@Z
extern "C" CRect* MS_ABI impl__GetKeyTipRect_CMFCRibbonTab__UEAA_AVCRect__PEAVCDC__H_Z(
    CMFCRibbonTab* /*pThis*/, CRect* /*pRet*/, CDC* /*pDC*/, int /*bIsMenu*/)
{
    return nullptr;
}

// The mfc140u RVA map does not resolve this export, so its body could not be
// transcribed. Keep the correct hidden CSize return-buffer parameter order.
// Symbol: ?GetRegularSize@CMFCRibbonTab@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" CSize* MS_ABI impl__GetRegularSize_CMFCRibbonTab__UEAA_AVCSize__PEAVCDC___Z(
    CMFCRibbonTab* /*pThis*/, CSize* /*pRet*/, CDC* /*pDC*/)
{
    return nullptr;
}

// Retail mfc140u RVA 0xec250 branches on the truncation flag at +0x184 and,
// when set, builds a tooltip through the parent category. Neither field is
// available in the local shadow layout.
// Symbol: ?GetToolTipText@CMFCRibbonTab@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" CString* MS_ABI impl__GetToolTipText_CMFCRibbonTab__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CMFCRibbonTab* /*pThis*/, CString* /*pRet*/)
{
    return nullptr;
}

// Retail mfc140u RVA 0xdef0 returns the dword at +0x140; that field is not
// present in the local class shadow declaration.
// Symbol: ?IsSelected@CMFCRibbonTab@@QEBAHXZ
extern "C" int MS_ABI impl__IsSelected_CMFCRibbonTab__QEBAHXZ(
    const CMFCRibbonTab* /*pThis*/)
{
    return 0;
}

// Retail mfc140u RVA 0xec8c0 dispatches through the parent ribbon bar and
// category state, which are not modeled for this class.
// Symbol: ?OnAccDefaultAction@CMFCRibbonTab@@UEAAXXZ
extern "C" void MS_ABI impl__OnAccDefaultAction_CMFCRibbonTab__UEAAXXZ(
    CMFCRibbonTab* /*pThis*/)
{
}

// Retail mfc140u RVA 0xec330 uses visual-manager drawing calls and the
// element/category geometry; those renderer and layout paths are not modeled.
// Symbol: ?OnDraw@CMFCRibbonTab@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonTab__UEAAXPEAVCDC___Z(
    CMFCRibbonTab* /*pThis*/, CDC* /*pDC*/)
{
}

// The mfc140u RVA map does not resolve this export. The ANSI-twin body depends
// on parent category/ribbon state and virtual dispatch not represented here.
// Symbol: ?OnKey@CMFCRibbonTab@@UEAAHH@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonTab__UEAAHH_Z(
    CMFCRibbonTab* /*pThis*/, int /*bIsMenuKey*/)
{
    return 0;
}

// The mfc140u RVA map does not resolve this export; the ANSI-twin body checks
// elapsed click time against the double-click interval and redraws through the
// parent ribbon-bar HWND. That timing/window state is not modeled here.
// CPoint is an 8-byte by-value MS_ABI aggregate.
// Symbol: ?OnLButtonDblClk@CMFCRibbonTab@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCRibbonTab__UEAAXVCPoint___Z(
    CMFCRibbonTab* /*pThis*/, long long /*point*/)
{
}

// The mfc140u RVA map does not resolve this export; the ANSI-twin body
// forwards through parent category/ribbon state that is not modeled here.
// CPoint is an 8-byte by-value MS_ABI aggregate.
// Symbol: ?OnLButtonDown@CMFCRibbonTab@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonTab__UEAAXVCPoint___Z(
    CMFCRibbonTab* /*pThis*/, long long /*point*/)
{
}

// The mfc140u RVA map does not resolve this export; the ANSI-twin body checks
// the element rectangle and calls RedrawWindow on the parent ribbon-bar HWND.
// Symbol: ?Redraw@CMFCRibbonTab@@UEAAXXZ
extern "C" void MS_ABI impl__Redraw_CMFCRibbonTab__UEAAXXZ(
    CMFCRibbonTab* /*pThis*/)
{
}

// The mfc140u RVA map does not resolve this export; the ANSI-twin body fills
// accessibility data from parent category/ribbon state and fixed string values.
// Symbol: ?SetACCData@CMFCRibbonTab@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonTab__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonTab* /*pThis*/, CWnd* /*pParentWnd*/, CAccessibilityData& /*data*/)
{
    return 0;
}
