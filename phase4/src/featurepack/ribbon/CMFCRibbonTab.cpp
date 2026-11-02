// CMFCRibbonTab — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();

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

// Retail RVA 0xecae0 (mfc140u) validates all four output pointers, then
// requires CHILDID_SELF and a parent category/ribbon-bar HWND before mapping
// the element rectangle. Those parent layouts are not modeled here.
// Symbol: ?accLocation@CMFCRibbonTab@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCRibbonTab__UEAAJPEAJ000UtagVARIANT___Z(
    CMFCRibbonTab* /*pThis*/, long* /*pxLeft*/, long* /*pyTop*/, long* /*pcxWidth*/,
    long* /*pcyHeight*/, VARIANT /*varChild*/)
{
    return 0;
}

// Retail RVA 0xecc40 (mfc140u) validates VT_I4, then navigates the tab group
// reached through the parent category and returns an adjacent tab's accessible
// dispatch. The parent/category/group layouts are not modeled here.
// Symbol: ?accNavigate@CMFCRibbonTab@@UEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonTab__UEAAJJUtagVARIANT__PEAU2__Z(
    CMFCRibbonTab* /*pThis*/, long /*navDir*/, VARIANT /*varStart*/, VARIANT* /*pvarEndUpAt*/)
{
    return 0;
}

// Retail RVA 0xec2f0 (mfc140u) calls CMFCRibbonBaseElement::CopyFrom (RVA
// 0x10350, mfc140u), then copies dwords at +0x180 and +0x188. The declared
// OpenMFC class has no retail-layout fields at those offsets, so this stays a stub.
// Symbol: ?CopyFrom@CMFCRibbonTab@@UEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonTab__UEAAXAEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonTab* /*pThis*/, const CMFCRibbonBaseElement& /*src*/)
{
}

// Retail RVA 0xecbc0 (mfc140u) accepts only VT_I4/CHILDID_SELF (otherwise
// S_FALSE), allocates the literal "Switch" with OLEAUT32 #4 (SysAllocStringLen),
// and throws the MFC memory exception if allocation fails.
// IAT slot 0x1802c6b50 (mfc140u) resolves to OLEAUT32 #4 via iatu.py.
// Symbol: ?get_accDefaultAction@CMFCRibbonTab@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDefaultAction_CMFCRibbonTab__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCRibbonTab* /*pThis*/, VARIANT varChild, wchar_t** pszDefaultAction)
{
    if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF) return S_FALSE;
    BSTR action = ::SysAllocStringLen(L"Switch", 6);
    if (action == nullptr) impl__AfxThrowMemoryException__YAXXZ();
    *pszDefaultAction = action;
    return S_OK;
}

// Retail RVA 0xeca60 (mfc140u) requires the parent category, ribbon-bar HWND,
// and tab group, then AddRefs the group and obtains its accessible interface.
// Those parent/group layouts are not modeled here.
// Symbol: ?get_accParent@CMFCRibbonTab@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonTab__UEAAJPEAPEAUIDispatch___Z(
    CMFCRibbonTab* /*pThis*/, IDispatch** /*ppdispParent*/)
{
    return 0;
}

// Retail RVA 0xec4c0 (mfc140u) uses the element rectangle, inherited virtual
// state, and CDC metrics to calculate the key-tip rectangle. The required base
// member layout and rendering state are not modeled here.
// Symbol: ?GetKeyTipRect@CMFCRibbonTab@@UEAA?AVCRect@@PEAVCDC@@H@Z
extern "C" CRect* MS_ABI impl__GetKeyTipRect_CMFCRibbonTab__UEAA_AVCRect__PEAVCDC__H_Z(
    CMFCRibbonTab* /*pThis*/, CRect* /*pRet*/, CDC* /*pDC*/, int /*bIsMenu*/)
{
    return nullptr;
}

// This export has no resolved RVA in either image map; its body could not be
// disassembled. Keep the hidden CSize return-buffer parameter order.
// Symbol: ?GetRegularSize@CMFCRibbonTab@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" CSize* MS_ABI impl__GetRegularSize_CMFCRibbonTab__UEAA_AVCSize__PEAVCDC___Z(
    CMFCRibbonTab* /*pThis*/, CSize* /*pRet*/, CDC* /*pDC*/)
{
    return nullptr;
}

// Retail RVA 0xec250 (mfc140u) branches on the truncation flag at +0x184;
// when set, it builds the tooltip through the parent category. The tab fields
// and parent/category layout are not represented by the OpenMFC declaration.
// Symbol: ?GetToolTipText@CMFCRibbonTab@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" CString* MS_ABI impl__GetToolTipText_CMFCRibbonTab__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CMFCRibbonTab* /*pThis*/, CString* /*pRet*/)
{
    return nullptr;
}

// Retail RVA 0xdef0 (mfc140u) returns the dword at +0x140. The declared
// OpenMFC base class does not model that retail field.
// Symbol: ?IsSelected@CMFCRibbonTab@@QEBAHXZ
extern "C" int MS_ABI impl__IsSelected_CMFCRibbonTab__QEBAHXZ(
    const CMFCRibbonTab* /*pThis*/)
{
    return 0;
}

// Retail RVA 0xec8c0 (mfc140u) checks parent-category/ribbon-bar state;
// it calls two ribbon-bar routines on one branch and the bar's vtable slot
// 0x668 on the other. Those layouts are not modeled.
// Symbol: ?OnAccDefaultAction@CMFCRibbonTab@@UEAAXXZ
extern "C" void MS_ABI impl__OnAccDefaultAction_CMFCRibbonTab__UEAAXXZ(
    CMFCRibbonTab* /*pThis*/)
{
}

// Retail RVA 0xec330 (mfc140u) skips an empty element rect; otherwise it uses
// the visual manager and CDC drawing calls with category/tab geometry. The
// renderer and base-element layout paths are not modeled.
// Symbol: ?OnDraw@CMFCRibbonTab@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonTab__UEAAXPEAVCDC___Z(
    CMFCRibbonTab* /*pThis*/, CDC* /*pDC*/)
{
}

// No Unicode RVA is resolved. ANSI-twin RVA 0xed1f0 (mfc140.dll) invokes a
// tab virtual with the key argument, checks the category's parent-ribbon state,
// and may dispatch through the ribbon bar's vtable slot 0x668. These layouts are unavailable.
// Symbol: ?OnKey@CMFCRibbonTab@@UEAAHH@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonTab__UEAAHH_Z(
    CMFCRibbonTab* /*pThis*/, int /*bIsMenuKey*/)
{
    return 0;
}

// No Unicode RVA is resolved. ANSI-twin RVA 0xed540 (mfc140.dll) checks the
// category's active/timestamp state against clock() and GetDoubleClickTime,
// then invokes tab/category and ribbon-bar routines and redraws the parent
// ribbon-bar HWND. The required state and timing/window integration are unavailable.
// CPoint is an 8-byte by-value MS_ABI aggregate.
// Symbol: ?OnLButtonDblClk@CMFCRibbonTab@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCRibbonTab__UEAAXVCPoint___Z(
    CMFCRibbonTab* /*pThis*/, long long /*point*/)
{
}

// No Unicode RVA is resolved. ANSI-twin RVA 0xed480 (mfc140.dll) tail-calls
// the parent ribbon bar's vtable slot 0x668 (slot 205), forwarding the by-value
// point. The category/ribbon-bar layout and compatible virtual dispatch are absent.
// CPoint is an 8-byte by-value MS_ABI aggregate.
// Symbol: ?OnLButtonDown@CMFCRibbonTab@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonTab__UEAAXVCPoint___Z(
    CMFCRibbonTab* /*pThis*/, long long /*point*/)
{
}

// No Unicode RVA is resolved. ANSI-twin RVA 0xed3f0 (mfc140.dll) checks the
// element rect, inflates it by 10 pixels, then calls RedrawWindow on the parent
// ribbon-bar HWND (IAT slot 0x1802c5388 in mfc140.dll, resolved with iat.py).
// The parent category/ribbon-bar layout is not modeled.
// Symbol: ?Redraw@CMFCRibbonTab@@UEAAXXZ
extern "C" void MS_ABI impl__Redraw_CMFCRibbonTab__UEAAXXZ(
    CMFCRibbonTab* /*pThis*/)
{
}

// No Unicode RVA is resolved. ANSI-twin RVA 0xed280 (mfc140.dll) returns
// FALSE when the parent category/ribbon HWND path is unavailable; otherwise it
// fills accessibility fields (including role 0x25 and fixed strings). The
// required category/group state is not modeled.
// Symbol: ?SetACCData@CMFCRibbonTab@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonTab__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonTab* /*pThis*/, CWnd* /*pParentWnd*/, CAccessibilityData& /*data*/)
{
    return 0;
}
