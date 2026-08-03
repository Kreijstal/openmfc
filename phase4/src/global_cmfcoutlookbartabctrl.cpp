// OpenMFC: CMFCOutlookBarTabCtrl exports.
//
// All 37 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the public headers (only its base
// CMFCBaseTabCtrl is, in include/openmfc/afxmfc.h), so `this` is taken as void*
// and the touched member offsets are mirrored with the file-local layout struct
// below.  CMFCOutlookBarTabCtrl derives from CMFCBaseTabCtrl and is 11768 bytes
// (0x2df8).  The stub constructors in manual_small_stub_implementations.cpp
// leave the object uninitialized, so every pointer access is NULL-guarded.
//
// Layout offsets harvested from the retail bodies:
//   * CWnd::m_hWnd at +0x40; m_bCreated +0xf0 (set by Create).
//   * Tab records: the internal array at +0x108 (ptr) / +0x110 (size), the count
//     at +0x150 and the active tab at +0x154 -- all unmodeled (the side table in
//     mfccore.cpp is the only tab storage OpenMFC has).
//   * m_nTabButtonHeight +0x1d8 (the nonzero gate for RecalcLayout /
//     OnMouseMove) and m_nTabToDetach +0x1e0.
//   * Page-button state: border +0x3c0, visible +0x3c4, total +0x3c8, the
//     scroll-button capture flag +0x3cc, painted +0x3d0, enable-scroll +0x3d4
//     and text-align +0x3d8.
//   * Scroll-button rect +0x3fc (16 bytes; its top@+0x400 and bottom@+0x408 are
//     also read as the scroll offsets in OnMouseMove).
//   * Scroll-button children: CWnd sub-objects at +0x17b8 (m_hWnd +0x17f8) and
//     +0x22d8 (m_hWnd +0x2318), created by OnCreate and not modeled.
//
// Faithfully transcribed: EnableAnimation / EnableScrollButtons /
// GetTabNumberToDetach / IsMode2003 / IsTabDetachable / OnCancelMode /
// OnChangeTabs / OnEraseBkgnd / OnLButtonDown / OnLButtonUp /
// SetPageButtonTextAlign, plus the guarded fall-through paths of FindTargetWnd /
// GetTabArea / OnMouseMove / OnSetCursor / OnSize / OnShowFewerPageButtons /
// OnShowMorePageButtons / RebuildToolBar / RecalcLayout / SetActiveTab /
// SetBorderSize.  The vslot-0x318 relayout virtual, the +0x108 tab-record array,
// the internal scroll-button / toolbar children and the AFX_GLOBAL_DATA /
// COutlookOptionsDlg objects are not modeled, so those parts are kept
// conservative with `// TODO(clean-room)` markers (the relayout redraw is
// mirrored with ::RedrawWindow on m_hWnd).
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" int MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
    CCreateContext* pContext);
extern "C" void* MS_ABI impl___0CMFCOutlookBarTabCtrl__QEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam);
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCOutlookBar__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" int MS_ABI impl__IsTabDetachable_CMFCBaseTabCtrl__UEBAHH_Z(
    const CMFCBaseTabCtrl* pThis, int nIndex);
extern "C" int MS_ABI impl__OnCreate_CMFCBaseTabCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMFCBaseTabCtrl* pThis, CREATESTRUCTW* lpCreateStruct);
extern "C" void MS_ABI impl__OnCancelMode_CMFCBaseTabCtrl__IEAAXXZ(CMFCBaseTabCtrl* pThis);
extern "C" void MS_ABI impl__OnLButtonDown_CMFCBaseTabCtrl__IEAAXIVCPoint___Z(
    CMFCBaseTabCtrl* pThis, unsigned int nFlags, CPoint point);
extern "C" void MS_ABI impl__OnLButtonUp_CMFCBaseTabCtrl__IEAAXIVCPoint___Z(
    CMFCBaseTabCtrl* pThis, unsigned int nFlags, CPoint point);
extern "C" void MS_ABI impl__OnMouseMove_CMFCBaseTabCtrl__IEAAXIVCPoint___Z(
    CMFCBaseTabCtrl* pThis, unsigned int nFlags, CPoint point);
// Class static defined in feature_static_data.cpp (retail 0x1803be25c).
extern "C" std::int32_t impl__m_bEnableAnimation_CMFCOutlookBarTabCtrl__1HA;
// Sibling exports defined later in this file (used before their definitions).
extern "C" int MS_ABI impl__IsMode2003_CMFCOutlookBarTabCtrl__QEBAHXZ(const void* pThis);
extern "C" void MS_ABI impl__GetTabArea_CMFCOutlookBarTabCtrl__UEBAXAEAVCRect__0_Z(
    const void* pThis, CRect& rectTabArea, CRect& rectPaneArea);
extern "C" void MS_ABI impl__OnShowMorePageButtons_CMFCOutlookBarTabCtrl__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnShowFewerPageButtons_CMFCOutlookBarTabCtrl__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnShowOptions_CMFCOutlookBarTabCtrl__UEAAXXZ(void* pThis);

namespace {

//---------------------------------------------------------------------------
// Layout mirror (offsets harvested from the retail bodies).
//---------------------------------------------------------------------------

// CMFCOutlookBarTabCtrl, sizeof 11768 (0x2df8).  The first 0x40 bytes are the
// CObject/CWnd block (m_hWnd at 0x40); the CMFCBaseTabCtrl base owns the
// internal tab array (+0x108/+0x110), the count (+0x150) and the active tab
// (+0x154).  Only the fields this batch touches are named; the retail also
// accesses the +0x3fc scroll-button rect and the +0x400/+0x408 offsets, which
// alias that rect's top/bottom.
struct OutlookBarTabCtrl
{
    void* vfptr;                        // 0x00 CObject vfptr
    char  _pad0[0x40 - 0x08];           // 0x08..0x3f
    HWND  m_hWnd;                       // 0x40 CWnd::m_hWnd
    char  _pad1[0xf0 - 0x48];           // 0x48..0xef
    int   m_bCreated;                   // 0xf0 set to 1 by Create
    char  _pad2[0x1d8 - 0xf4];          // 0xf4..0x1d7 (incl. the +0x108 tab array)
    int   m_nTabButtonHeight;           // 0x1d8 tab-button height
    char  _pad2a[0x1e0 - 0x1dc];        // 0x1dc..0x1df
    int   m_nTabToDetach;               // 0x1e0 tab to detach (drag state)
    char  _pad3[0x3c0 - 0x1e4];         // 0x1e4..0x3bf
    int   m_nBorderSize;                // 0x3c0 tab border size
    int   m_nPageButtonsVisible;        // 0x3c4 visible page buttons
    int   m_nPageButtonsTotal;          // 0x3c8 total page buttons
    int   m_bScrollButtonPressed;       // 0x3cc scroll-button capture flag
    int   m_bPageButtonsPainted;        // 0x3d0 page-buttons-painted flag
    int   m_bEnableScrollButtons;       // 0x3d4 scroll-button enable flag
    int   m_nPageButtonTextAlign;       // 0x3d8 page-button text alignment
    char  _pad4[0x3fc - 0x3dc];         // 0x3dc..0x3fb
    RECT  m_rcScrollButtons;            // 0x3fc scroll-button rect (16 bytes)
    // NOTE: the retail OnMouseMove reads the rect's top (+0x400) and bottom
    // (+0x408) directly as the scroll offset / scroll-top integers.
    char  _pad5[0x2df8 - 0x40c];        // 0x40c..0x2df7
};
static_assert(sizeof(OutlookBarTabCtrl) == 0x2df8, "CMFCOutlookBarTabCtrl size mismatch");

} // namespace

//=============================================================================
// CMFCOutlookBarTabCtrl
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::AddControl(CWnd* pWnd, const wchar_t* lpszText,
//                                        int bIsInternal, UINT nID, DWORD dwStyle)
//=============================================================================
// Retail (0x18009bd80, ~0x220 bytes): IsMode2003 check, InternalAddTab via
// vslot 0x328, style/colour bookkeeping and SetActiveTab.  Not transcribed.
// Symbol: ?AddControl@CMFCOutlookBarTabCtrl@@QEAAXPEAVCWnd@@PEB_WHHK@Z
extern "C" void MS_ABI impl__AddControl_CMFCOutlookBarTabCtrl__QEAAXPEAVCWnd__PEB_WHHK_Z(
    void* pThis, void* pWnd, const wchar_t* lpszText, int bIsInternal,
    unsigned int nID, unsigned long dwStyle)
{
    (void)pWnd; (void)lpszText; (void)bIsInternal; (void)nID; (void)dwStyle;
    if (pThis == nullptr)
        return;
    // TODO(clean-room): not transcribed -- tab insertion via vslot 0x328 /
    // IsMode2003 / InternalAddTab (retail 0x18009bd80).
}

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::CanShowFewerPageButtons() const
//=============================================================================
// Retail (0x18009e5f0): `return *(int*)(+0x3c4) > 0;`.
// Symbol: ?CanShowFewerPageButtons@CMFCOutlookBarTabCtrl@@UEBAHXZ
extern "C" int MS_ABI impl__CanShowFewerPageButtons_CMFCOutlookBarTabCtrl__UEBAHXZ(
    const void* pThis)
{
    if (pThis == nullptr)
        return FALSE;
    return static_cast<const OutlookBarTabCtrl*>(pThis)->m_nPageButtonsVisible > 0
               ? TRUE : FALSE;
}

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::CanShowMorePageButtons() const
//=============================================================================
// Retail (0x18009e5d0): `return *(int*)(+0x3c8) > *(int*)(+0x3c4);`.
// Symbol: ?CanShowMorePageButtons@CMFCOutlookBarTabCtrl@@UEBAHXZ
extern "C" int MS_ABI impl__CanShowMorePageButtons_CMFCOutlookBarTabCtrl__UEBAHXZ(
    const void* pThis)
{
    if (pThis == nullptr)
        return FALSE;
    const OutlookBarTabCtrl* pTab = static_cast<const OutlookBarTabCtrl*>(pThis);
    return pTab->m_nPageButtonsTotal > pTab->m_nPageButtonsVisible ? TRUE : FALSE;
}

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::Create(const CRect& rect, CWnd* pParentWnd,
//                                    UINT nID)
//=============================================================================
// Retail (0x18009bc60): CWnd::Create(this, lpszClassName=NULL,
// lpszWindowName=@0x18033d19c (an empty wide string), 0x56000000, rect,
// pParentWnd, nID, NULL); on success calls vslot 0x60 (tab layout) and
// sets +0xf0 = 1.
// Symbol: ?Create@CMFCOutlookBarTabCtrl@@UEAAHAEBVCRect@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CMFCOutlookBarTabCtrl__UEAAHAEBVCRect__PEAVCWnd__I_Z(
    void* pThis, const RECT& rect, CWnd* pParentWnd, unsigned int nID)
{
    if (pThis == nullptr)
        return FALSE;

    int nRet = impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        reinterpret_cast<CWnd*>(pThis), nullptr, L"", 0x56000000,
        rect, pParentWnd, static_cast<UINT>(nID), nullptr);
    if (nRet) {
        // TODO(clean-room): transcribed partially -- retail invokes the tab
        // layout virtual (vslot 0x60) here; not modeled.
        static_cast<OutlookBarTabCtrl*>(pThis)->m_bCreated = 1;
    }
    return nRet;
}

//=============================================================================
// CObject* CMFCOutlookBarTabCtrl::CreateObject()
//=============================================================================
// Retail (0x18009b9f0):
//     void* p = operator new(0x2df8);
//     if (p) CMFCOutlookBarTabCtrl::CMFCOutlookBarTabCtrl(p);   // 0x18009ba30
//     return p;
// Symbol: ?CreateObject@CMFCOutlookBarTabCtrl@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCOutlookBarTabCtrl__SAPEAVCObject__XZ(void)
{
    void* pObj = ::operator new(0x2df8, std::nothrow);
    if (pObj != nullptr) {
        impl___0CMFCOutlookBarTabCtrl__QEAA_XZ(pObj);
    }
    return pObj;
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::DrawTabButton(CDC& dc, int nIndex, int bHighlight)
//=============================================================================
// Retail (0x18009d4a0, ~0x400 bytes): paints the tab image + text through the
// visual manager, the tab button images and AFX_GLOBAL_DATA.  Not transcribed.
// Symbol: ?DrawTabButton@CMFCOutlookBarTabCtrl@@IEAAXAEAVCDC@@HH@Z
extern "C" void MS_ABI impl__DrawTabButton_CMFCOutlookBarTabCtrl__IEAAXAEAVCDC__HH_Z(
    void* pThis, void* pDC, int nIndex, int bHighlight)
{
    (void)pDC; (void)nIndex; (void)bHighlight;
    if (pThis == nullptr)
        return;
    // TODO(clean-room): not transcribed -- tab image/text painting via the
    // visual manager (retail 0x18009d4a0).
}

//=============================================================================
// CMFCOutlookBarTabCtrl
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::EnableAnimation(BOOL bEnable) [static]
//=============================================================================
// Retail (0x18009daf0): `mov %ecx,0x320766(%rip); ret` -- stores bEnable into
// the class static m_bEnableAnimation (retail 0x1803be25c, modeled as
// impl__m_bEnableAnimation_CMFCOutlookBarTabCtrl__1HA in feature_static_data.cpp).
// Symbol: ?EnableAnimation@CMFCOutlookBarTabCtrl@@SAXH@Z
extern "C" void MS_ABI impl__EnableAnimation_CMFCOutlookBarTabCtrl__SAXH_Z(int bEnable)
{
    impl__m_bEnableAnimation_CMFCOutlookBarTabCtrl__1HA = bEnable;
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::EnableScrollButtons(BOOL bEnable,
//                                                 BOOL bShowUp,
//                                                 BOOL bShowDown)
//=============================================================================
// Retail (0x18009db00):
//     old = m_bEnableScrollButtons@+0x3d4
//     new = IsMode2003() ? 0 : bEnable;       // force-off in Outlook 2003 mode
//     +0x3d4 = new
//     if (new) { show both child buttons (ShowWindow 4, MoveWindow, internal
//                button-state helper 0x18001dd80 with ids 0x18/0xd); }
//     else     { hide both child buttons (ShowWindow 0); }
//     RedrawWindow(child m_hWnd@+0x17f8, 0, 0, 0x105);
//     RedrawWindow(child m_hWnd@+0x2318, 0, 0, 0x105);
//     if (old != new) vslot 0x318 (relayout);
// Symbol: ?EnableScrollButtons@CMFCOutlookBarTabCtrl@@QEAAXHHH@Z
extern "C" void MS_ABI impl__EnableScrollButtons_CMFCOutlookBarTabCtrl__QEAAXHHH_Z(
    void* pThis, int bEnable, int bShowUp, int bShowDown)
{
    (void)bShowUp; (void)bShowDown;
    if (pThis == nullptr)
        return;

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    const int nOldEnable = pTab->m_bEnableScrollButtons;

    const int nNewEnable =
        impl__IsMode2003_CMFCOutlookBarTabCtrl__QEBAHXZ(pThis) ? 0 : bEnable;
    pTab->m_bEnableScrollButtons = nNewEnable;

    // The two scroll-button children live at +0x17b8 (m_hWnd +0x17f8) and
    // +0x22d8 (m_hWnd +0x2318); they are created in OnCreate which is not
    // modeled, so the hwnds are NULL here in practice.
    HWND hwndUp   = *reinterpret_cast<HWND*>(static_cast<char*>(pThis) + 0x17f8);
    HWND hwndDown = *reinterpret_cast<HWND*>(static_cast<char*>(pThis) + 0x2318);

    if (nNewEnable) {
        if (hwndUp != nullptr)
            ::ShowWindow(hwndUp, SW_SHOWNOACTIVATE);
        if (hwndDown != nullptr)
            ::ShowWindow(hwndDown, SW_SHOWNOACTIVATE);
        // TODO(clean-room): transcribed partially -- retail also calls the
        // MoveWindow-style helper (0x1802a9b30) with bShowUp/bShowDown and the
        // internal button-state helper (0x18001dd80, ids 0x18/0xd); not
        // modeled.
    } else {
        if (hwndUp != nullptr)
            ::ShowWindow(hwndUp, SW_HIDE);
        if (hwndDown != nullptr)
            ::ShowWindow(hwndDown, SW_HIDE);
    }

    if (hwndUp != nullptr)
        ::RedrawWindow(hwndUp, nullptr, nullptr, 0x105);
    if (hwndDown != nullptr)
        ::RedrawWindow(hwndDown, nullptr, nullptr, 0x105);

    if (nOldEnable != nNewEnable) {
        // TODO(clean-room): transcribed partially -- retail invokes the
        // relayout virtual (vslot 0x318); not modeled.
        if (pTab->m_hWnd != nullptr)
            ::RedrawWindow(pTab->m_hWnd, nullptr, nullptr, 0x505);
    }
}

//=============================================================================
// CWnd* CMFCOutlookBarTabCtrl::FindTargetWnd(const CPoint& point)
//=============================================================================
// Retail (0x18009cd90): for each tab record in the +0x108/+0x110 array (count
// +0x150) whose +0x48 "non-detachable" flag is set, returns NULL when the
// point is inside the tab rect (+0x20); otherwise returns
// CWnd::FromHandlePermanent(GetParent(m_hWnd)).  The per-tab records are
// unmodeled, so the parent-window fall-through is the live path.
// Symbol: ?FindTargetWnd@CMFCOutlookBarTabCtrl@@UEAAPEAVCWnd@@AEBVCPoint@@@Z
extern "C" CWnd* MS_ABI impl__FindTargetWnd_CMFCOutlookBarTabCtrl__UEAAPEAVCWnd__AEBVCPoint___Z(
    void* pThis, const CPoint& point)
{
    (void)point;
    if (pThis == nullptr)
        return nullptr;
    // TODO(clean-room): transcribed partially -- retail returns NULL when the
    // point falls on a tab whose +0x48 flag is set; the per-tab records
    // (array +0x108/+0x110, count +0x150) are not modeled.
    HWND hWnd = static_cast<OutlookBarTabCtrl*>(pThis)->m_hWnd;
    return impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hWnd));
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::GetTabArea(CRect& rectTabArea,
//                                        CRect& rectPaneArea) const
//=============================================================================
// Retail (0x18009d860): SetRectEmpty both rects, then unions every tab rect
// (+0x20 of the records behind the +0x108/+0x110 array, count +0x150) into
// rectTabArea / rectPaneArea, and finally ClientToScreen's both rects.  The
// per-tab records are unmodeled, so the empty-rect + ClientToScreen path is
// the faithful result for a control without modeled tabs.
// Symbol: ?GetTabArea@CMFCOutlookBarTabCtrl@@UEBAXAEAVCRect@@0@Z
extern "C" void MS_ABI impl__GetTabArea_CMFCOutlookBarTabCtrl__UEBAXAEAVCRect__0_Z(
    const void* pThis, CRect& rectTabArea, CRect& rectPaneArea)
{
    if (pThis == nullptr)
        return;

    rectTabArea.SetRectEmpty();
    rectPaneArea.SetRectEmpty();

    // TODO(clean-room): transcribed partially -- retail unions the per-tab
    // rects (+0x20 of the records behind array +0x108/+0x110); not modeled.
    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(
        reinterpret_cast<const CWnd*>(pThis), rectTabArea);
    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(
        reinterpret_cast<const CWnd*>(pThis), rectPaneArea);
}

//=============================================================================
// int CMFCOutlookBarTabCtrl::GetTabNumberToDetach(int nIndex) const
//=============================================================================
// Retail (0x18009da10):
//     if (nIndex == -1) nIndex = m_nTabToDetach@+0x1e0;
//     return nIndex;
// Symbol: ?GetTabNumberToDetach@CMFCOutlookBarTabCtrl@@MEBAHH@Z
extern "C" int MS_ABI impl__GetTabNumberToDetach_CMFCOutlookBarTabCtrl__MEBAHH_Z(
    const void* pThis, int nIndex)
{
    if (pThis == nullptr)
        return nIndex;
    if (nIndex == -1)
        return static_cast<const OutlookBarTabCtrl*>(pThis)->m_nTabToDetach;
    return nIndex;
}

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::IsMode2003() const
//=============================================================================
// Retail (0x18009deb0):
//     pParent = CWnd::FromHandlePermanent(GetParent(m_hWnd));
//     if (pParent && pParent->IsKindOf(&CMFCOutlookBar::class@0x1803b1328)
//         && *(int*)(pParent+0x550) != 0) return 1;
//     return 0;
// Symbol: ?IsMode2003@CMFCOutlookBarTabCtrl@@QEBAHXZ
extern "C" int MS_ABI impl__IsMode2003_CMFCOutlookBarTabCtrl__QEBAHXZ(const void* pThis)
{
    if (pThis == nullptr)
        return FALSE;

    const OutlookBarTabCtrl* pTab = static_cast<const OutlookBarTabCtrl*>(pThis);
    void* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(
        ::GetParent(pTab->m_hWnd));
    if (pParent == nullptr)
        return FALSE;

    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pParent),
            impl__GetThisClass_CMFCOutlookBar__SAPEAUCRuntimeClass__XZ()))
        return FALSE;

    return *reinterpret_cast<const int*>(static_cast<const char*>(pParent) + 0x550) != 0
               ? TRUE : FALSE;
}

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::IsPtInTabArea(CPoint point) const
//=============================================================================
// Retail (0x18009bcc0): calls GetTabArea (vslot 0x2e0) into two stack rects,
// ClientToScreen's both, then returns
// PtInRect(rectTabArea, point) || PtInRect(rectPaneArea, point).  The
// GetTabArea OpenMFC path empties both rects (no modeled tab records), so
// FALSE is the terminal result.
// Symbol: ?IsPtInTabArea@CMFCOutlookBarTabCtrl@@UEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__IsPtInTabArea_CMFCOutlookBarTabCtrl__UEBAHVCPoint___Z(
    const void* pThis, CPoint point)
{
    if (pThis == nullptr)
        return FALSE;

    CRect rectTabArea;
    CRect rectPaneArea;
    impl__GetTabArea_CMFCOutlookBarTabCtrl__UEBAXAEAVCRect__0_Z(
        pThis, rectTabArea, rectPaneArea);

    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(
        reinterpret_cast<const CWnd*>(pThis), rectTabArea);
    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(
        reinterpret_cast<const CWnd*>(pThis), rectPaneArea);

    return (::PtInRect(rectTabArea, point) || ::PtInRect(rectPaneArea, point))
               ? TRUE : FALSE;
}

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::IsTabDetachable(int nIndex) const
//=============================================================================
// Retail (0x18009df00): when IsMode2003() is set returns 0; otherwise
// forwards to CMFCBaseTabCtrl::IsTabDetachable (0x180016dd0).
// Symbol: ?IsTabDetachable@CMFCOutlookBarTabCtrl@@UEBAHH@Z
extern "C" int MS_ABI impl__IsTabDetachable_CMFCOutlookBarTabCtrl__UEBAHH_Z(
    const void* pThis, int nIndex)
{
    if (pThis == nullptr)
        return FALSE;
    if (impl__IsMode2003_CMFCOutlookBarTabCtrl__QEBAHXZ(pThis))
        return FALSE;
    return impl__IsTabDetachable_CMFCBaseTabCtrl__UEBAHH_Z(
        static_cast<const CMFCBaseTabCtrl*>(pThis), nIndex);
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnCancelMode()
//=============================================================================
// Retail (0x18009e120): CMFCBaseTabCtrl::OnCancelMode (0x180018190), then when
// the scroll-button capture flag +0x3cc is set, ReleaseCapture and clear it.
// Symbol: ?OnCancelMode@CMFCOutlookBarTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCOutlookBarTabCtrl__IEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;

    impl__OnCancelMode_CMFCBaseTabCtrl__IEAAXXZ(static_cast<CMFCBaseTabCtrl*>(pThis));

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    if (pTab->m_bScrollButtonPressed) {
        ::ReleaseCapture();
        pTab->m_bScrollButtonPressed = 0;
    }
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnChangeTabs()
//=============================================================================
// Retail (0x18009e600): `movl $0xffffffff,0x3c4(%rcx); ret` -- marks the
// page-button layout dirty.
// Symbol: ?OnChangeTabs@CMFCOutlookBarTabCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnChangeTabs_CMFCOutlookBarTabCtrl__UEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;
    static_cast<OutlookBarTabCtrl*>(pThis)->m_nPageButtonsVisible = -1;
}

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
//=============================================================================
// Retail (0x18009ddb0): resolves the parent (FromHandlePermanent(GetParent));
// when the parent IsKindOf(CMFCOutlookBar) and lParam matches the m_hWnd of
// one of the two internal scroll buttons (+0x17b8 / +0x22d8) it dispatches
// through the parent's vslot-0x7f0 virtual and returns 1; everything else
// falls through to CWnd::OnCommand (0x18028e040).  The scroll-button children
// and the vslot are unmodeled, so the base routing is the safe live path.
// Symbol: ?OnCommand@CMFCOutlookBarTabCtrl@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMFCOutlookBarTabCtrl__MEAAH_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam)
{
    if (pThis == nullptr)
        return FALSE;
    // TODO(clean-room): transcribed partially -- retail special-cases the two
    // internal scroll-button windows (+0x17b8/+0x22d8, created in OnCreate)
    // and routes their commands through the parent's vslot-0x7f0 virtual;
    // neither the children nor the virtual are modeled, so everything is
    // forwarded to the base handler (non-destructive for unrelated commands).
    return impl__OnCommand_CWnd__MEAAH_K_J_Z(static_cast<CWnd*>(pThis),
        static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam));
}

//=============================================================================
// int CMFCOutlookBarTabCtrl::OnCreate(CREATESTRUCTW* lpCreateStruct)
//=============================================================================
// Retail (0x18009dc10): CMFCBaseTabCtrl::OnCreate (0x180019ff0) first (-1
// aborts); then creates the two internal scroll buttons (CButton at +0x17b8 /
// +0x22d8, ids 0x18/0xd, style 0x40000000), stores +0x18fc/+0x241c = 0x64,
// creates the internal CMFCToolBar at +0x428 (id 0x800) and wires the parent
// hwnd at +0x4c8, then returns 0.  The child-object Create virtuals (vslot
// 0x2d8/0x658) are unmodeled.
// Symbol: ?OnCreate@CMFCOutlookBarTabCtrl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCOutlookBarTabCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct)
{
    if (pThis == nullptr)
        return -1;

    const int nRet = impl__OnCreate_CMFCBaseTabCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(
        static_cast<CMFCBaseTabCtrl*>(pThis), lpCreateStruct);
    if (nRet == -1)
        return -1;

    // TODO(clean-room): transcribed partially -- retail creates the two scroll
    // buttons (+0x17b8/+0x22d8, ids 0x18/0xd, style 0x40000000), stores
    // +0x18fc/+0x241c = 0x64, creates the internal toolbar (+0x428, id 0x800,
    // +0x10bc = 0) and wires the parent hwnd at +0x4c8; the child-object
    // Create virtuals are not modeled.
    return 0;
}

//=============================================================================
// DROPEFFECT CMFCOutlookBarTabCtrl::OnDragEnter(COleDataObject*, DWORD,
//                                                CPoint)
//=============================================================================
// Retail (0x18009da20) is a single tail-call to the owner drop-target virtual
// (vslot 0x580) which forwards to the drag logic shared with OnDragOver.  The
// drop machinery is unmodeled; DROPEFFECT_NONE is the safe default.
// Symbol: ?OnDragEnter@CMFCOutlookBarTabCtrl@@MEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI
impl__OnDragEnter_CMFCOutlookBarTabCtrl__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    void* pThis, void* /*pDataObject*/, unsigned long /*dwKeyState*/, CPoint /*point*/)
{
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): partially transcribed -- retail forwards to the
    // vslot-0x580 drop-target virtual (the shared drag-enter/over logic in
    // CMFCBaseTabCtrl); that virtual is not modeled.
    return 0;   // DROPEFFECT_NONE
}

//=============================================================================
// DROPEFFECT CMFCOutlookBarTabCtrl::OnDragOver(COleDataObject*, DWORD,
//                                              CPoint)
//=============================================================================
// Retail (0x18009da40): resolves the drop source (COleDataObject query
// 0x18015c790), accepts it only when IsKindOf(CMFCOutlookBarPaneButton, class
// 0x1803b13b8), hit-tests the tab (vslot 0x440) and returns DROPEFFECT_MOVE
// (2) when MK_LBUTTON is held, else DROPEFFECT_NONE.  The drop-source
// machinery is unmodeled; DROPEFFECT_NONE is the safe default.
// Symbol: ?OnDragOver@CMFCOutlookBarTabCtrl@@MEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI
impl__OnDragOver_CMFCOutlookBarTabCtrl__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    void* pThis, void* /*pDataObject*/, unsigned long /*dwKeyState*/, CPoint /*point*/)
{
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): partially transcribed -- retail accepts only an
    // CMFCOutlookBarPaneButton drop source and returns DROPEFFECT_MOVE (2)
    // when MK_LBUTTON is set; the source lookup / IsKindOf / hit-test are not
    // modeled.
    return 0;   // DROPEFFECT_NONE
}

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::OnEraseBkgnd(CDC* pDC)
//=============================================================================
// Retail (0x180003a60): `mov $0x1,%eax; ret` -- always returns TRUE.
// Symbol: ?OnEraseBkgnd@CMFCOutlookBarTabCtrl@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCOutlookBarTabCtrl__IEAAHPEAVCDC___Z(
    void* pThis, void* /*pDC*/)
{
    if (pThis == nullptr)
        return TRUE;
    return TRUE;
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
//=============================================================================
// Retail (0x18009df40): when the point hits the scroll-button rect at +0x3fc
// it sets the +0x3cc capture flag and calls SetCapture(m_hWnd); otherwise it
// forwards to CMFCBaseTabCtrl::OnLButtonDown (0x180017660).
// Symbol: ?OnLButtonDown@CMFCOutlookBarTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCOutlookBarTabCtrl__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, CPoint point)
{
    if (pThis == nullptr)
        return;

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    if (::PtInRect(&pTab->m_rcScrollButtons, point)) {
        pTab->m_bScrollButtonPressed = 1;
        ::SetCapture(pTab->m_hWnd);
    } else {
        impl__OnLButtonDown_CMFCBaseTabCtrl__IEAAXIVCPoint___Z(
            static_cast<CMFCBaseTabCtrl*>(pThis), nFlags, point);
    }
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnLButtonUp(UINT nFlags, CPoint point)
//=============================================================================
// Retail (0x18009dfb0): when the +0x3cc capture flag is set, ReleaseCapture
// and clear it, then tail-calls CMFCBaseTabCtrl::OnLButtonUp (0x180017ae0).
// Symbol: ?OnLButtonUp@CMFCOutlookBarTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCOutlookBarTabCtrl__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, CPoint point)
{
    if (pThis == nullptr)
        return;

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    if (pTab->m_bScrollButtonPressed) {
        ::ReleaseCapture();
        pTab->m_bScrollButtonPressed = 0;
    }

    impl__OnLButtonUp_CMFCBaseTabCtrl__IEAAXIVCPoint___Z(
        static_cast<CMFCBaseTabCtrl*>(pThis), nFlags, point);
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
//=============================================================================
// Retail (0x18009e000): when the +0x3cc capture flag is clear it forwards to
// CMFCBaseTabCtrl::OnMouseMove (0x180017e50).  Otherwise it recomputes the
// page-button index from the horizontal drag:
//     new = current(+0x3c4) + (scrollOffset@+0x400 - point.x) / tabHeight@+0x1d8
// clamps it to [0, count] (vslot 0x358 GetTabsCount), and when it changed
// runs the vslot-0x318 relayout and refreshes the scroll info (pos =
// (m_nScrollTop@+0x408 + scrollOffset@+0x400) / 2).  The +0x400/+0x408 values
// alias the scroll-button rect's top/bottom.
// Symbol: ?OnMouseMove@CMFCOutlookBarTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCOutlookBarTabCtrl__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, CPoint point)
{
    if (pThis == nullptr)
        return;

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    if (pTab->m_bScrollButtonPressed == 0) {
        impl__OnMouseMove_CMFCBaseTabCtrl__IEAAXIVCPoint___Z(
            static_cast<CMFCBaseTabCtrl*>(pThis), nFlags, point);
        return;
    }

    if (pTab->m_nTabButtonHeight == 0)
        return;

    const int nCur = pTab->m_nPageButtonsVisible;
    if (nCur == -1)
        return;

    const int nScrollOffset = pTab->m_rcScrollButtons.top;   // retail +0x400
    const int nDelta = (nScrollOffset - point.x) / pTab->m_nTabButtonHeight;
    if (nDelta == 0)
        return;

    const int nNew = nDelta + nCur;

    // Retail clamps the new index into [0, count] using the GetTabsCount
    // virtual (vslot 0x358).
    int nClamped = nNew < 0 ? 0 : nNew;
    const int nCount = static_cast<CMFCBaseTabCtrl*>(pThis)->GetTabsCount();
    if (nCount < nClamped)
        nClamped = nCount;

    pTab->m_nPageButtonsVisible = nClamped;
    if (nClamped != nCur) {
        pTab->m_bPageButtonsPainted = 1;
        // TODO(clean-room): transcribed partially -- retail invokes the
        // relayout virtual (vslot 0x318) and refreshes the internal scroll
        // bar's info (pos = (0x408 + 0x400) / 2); neither is modeled.
        if (pTab->m_hWnd != nullptr)
            ::RedrawWindow(pTab->m_hWnd, nullptr, nullptr, 0x505);
        pTab->m_bPageButtonsPainted = 0;
    }
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnPaint()
//=============================================================================
// Retail (0x18009ce50): constructs a CPaintDC, fills the client rect with the
// global background brush and draws each tab through the tab-button painter
// (vslot 0x358 count + per-tab DrawTabButton).  Those drawing helpers and
// AFX_GLOBAL_DATA are unmodeled; the conservative version keeps the
// BeginPaint/EndPaint bracket that validates the update region (preventing
// repaint loops).
// Symbol: ?OnPaint@CMFCOutlookBarTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCOutlookBarTabCtrl__IEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;

    HWND hWnd = static_cast<OutlookBarTabCtrl*>(pThis)->m_hWnd;
    if (hWnd == nullptr)
        return;

    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(hWnd, &ps);
    (void)hdc;
    // TODO(clean-room): partially transcribed -- retail paints the background
    // and the tab buttons through AFX_GLOBAL_DATA / the visual manager
    // (vslot 0x358 count loop + DrawTabButton); not modeled.
    ::EndPaint(hWnd, &ps);
}

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest,
//                                         UINT nMessage)
//=============================================================================
// Retail (0x18009d950): GetCursorPos -> ScreenToClient(m_hWnd); when the
// point hits the scroll-button rect at +0x3fc it sets cursor resource 0x7f85
// (LoadCursorW(NULL, 0x7f85)) and returns TRUE; otherwise it hit-tests the
// tabs (vslot 0x440) and either sets the AFX_GLOBAL_DATA drag cursor
// (returning TRUE) or defers to the base CWnd handler (0x18028ac80).
// Symbol: ?OnSetCursor@CMFCOutlookBarTabCtrl@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCOutlookBarTabCtrl__IEAAHPEAVCWnd__II_Z(
    void* pThis, void* /*pWnd*/, unsigned int /*nHitTest*/, unsigned int /*nMessage*/)
{
    if (pThis == nullptr)
        return FALSE;

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    POINT pt = {};
    ::GetCursorPos(&pt);
    if (pTab->m_hWnd != nullptr)
        ::ScreenToClient(pTab->m_hWnd, &pt);

    if (::PtInRect(&pTab->m_rcScrollButtons, pt)) {
        ::SetCursor(::LoadCursorW(nullptr, MAKEINTRESOURCEW(0x7f85)));
        return TRUE;
    }

    // TODO(clean-room): transcribed partially -- retail hit-tests the tabs
    // (vslot 0x440) and, on a hit, sets the AFX_GLOBAL_DATA drag cursor and
    // returns TRUE; without a modeled hit test the base CWnd::Default tail is
    // the faithful path for a control with no tab records.
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis)));
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnShowFewerPageButtons()
//=============================================================================
// Retail (0x18009e590): `decl 0x3c4; 0x3d0 = 1; vslot 0x318; 0x3d0 = 0`.
// Symbol: ?OnShowFewerPageButtons@CMFCOutlookBarTabCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnShowFewerPageButtons_CMFCOutlookBarTabCtrl__UEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr)
        return;

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    --pTab->m_nPageButtonsVisible;
    pTab->m_bPageButtonsPainted = 1;
    // TODO(clean-room): transcribed partially -- retail invokes the relayout
    // virtual (vslot 0x318); not modeled.
    if (pTab->m_hWnd != nullptr)
        ::RedrawWindow(pTab->m_hWnd, nullptr, nullptr, 0x505);
    pTab->m_bPageButtonsPainted = 0;
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnShowMorePageButtons()
//=============================================================================
// Retail (0x18009e550): `incl 0x3c4; 0x3d0 = 1; vslot 0x318; 0x3d0 = 0`.
// Symbol: ?OnShowMorePageButtons@CMFCOutlookBarTabCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnShowMorePageButtons_CMFCOutlookBarTabCtrl__UEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr)
        return;

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    ++pTab->m_nPageButtonsVisible;
    pTab->m_bPageButtonsPainted = 1;
    // TODO(clean-room): transcribed partially -- retail invokes the relayout
    // virtual (vslot 0x318); not modeled.
    if (pTab->m_hWnd != nullptr)
        ::RedrawWindow(pTab->m_hWnd, nullptr, nullptr, 0x505);
    pTab->m_bPageButtonsPainted = 0;
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnShowOptions()
//=============================================================================
// Retail (0x18009f220): constructs a local COutlookOptionsDlg (ctor
// 0x18009e850) and, when DoModal() (0x1802088b0) returns IDOK (1), sets
// +0x3d0, runs the vslot-0x318 relayout and clears +0x3d0.  The dialog class
// is unmodeled.
// Symbol: ?OnShowOptions@CMFCOutlookBarTabCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnShowOptions_CMFCOutlookBarTabCtrl__UEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;
    // TODO(clean-room): not transcribed -- retail shows the modal
    // COutlookOptionsDlg (ctor 0x18009e850, ~0x530 bytes on the stack) and,
    // on IDOK, runs the relayout virtual (vslot 0x318); neither the dialog
    // nor the virtual is modeled.
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnSize(UINT nType, int cx, int cy)
//=============================================================================
// Retail (0x18009ce20): calls CWnd::Default (0x18028ac80) then tail-calls the
// relayout virtual (vslot 0x318).
// Symbol: ?OnSize@CMFCOutlookBarTabCtrl@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCOutlookBarTabCtrl__IEAAXIHH_Z(
    void* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/)
{
    if (pThis == nullptr)
        return;

    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));

    // TODO(clean-room): transcribed partially -- retail tail-calls the
    // relayout virtual (vslot 0x318); not modeled.
    HWND hWnd = static_cast<OutlookBarTabCtrl*>(pThis)->m_hWnd;
    if (hWnd != nullptr)
        ::RedrawWindow(hWnd, nullptr, nullptr, 0x505);
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnToolbarCommand(UINT uiCmd)
//=============================================================================
// Retail (0x18009e790): `switch (uiCmd - 0xf200) { case 0: vslot 0x5c0;
// case 1: vslot 0x5c8; case 2: vslot 0x5e0; }` -- the virtuals dispatch to
// the Show-more / Show-fewer / Options handlers respectively.
// Symbol: ?OnToolbarCommand@CMFCOutlookBarTabCtrl@@IEAAXI@Z
extern "C" void MS_ABI impl__OnToolbarCommand_CMFCOutlookBarTabCtrl__IEAAXI_Z(
    void* pThis, unsigned int uiCmd)
{
    if (pThis == nullptr)
        return;

    // TODO(clean-room): transcribed partially -- retail dispatches through the
    // vslots 0x5c0/0x5c8/0x5e0; the exported handlers are called directly.
    switch (uiCmd - 0xf200) {
    case 0:
        impl__OnShowMorePageButtons_CMFCOutlookBarTabCtrl__UEAAXXZ(pThis);
        break;
    case 1:
        impl__OnShowFewerPageButtons_CMFCOutlookBarTabCtrl__UEAAXXZ(pThis);
        break;
    case 2:
        impl__OnShowOptions_CMFCOutlookBarTabCtrl__UEAAXXZ(pThis);
        break;
    default:
        break;
    }
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::RebuildToolBar()
//=============================================================================
// Retail (0x18009e150, ~0x3e0 bytes): no-op unless IsMode2003(); then rebuilds
// the internal toolbar (CMFCToolBar at +0x428, vslot-0x6b0 button rebuild, the
// +0x1778 button map and the +0x418 CImageList) and updates the +0x5d0/+0x5d8
// enable-state virtuals.  The toolbar object, the button list and the
// virtuals are unmodeled.
// Symbol: ?RebuildToolBar@CMFCOutlookBarTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildToolBar_CMFCOutlookBarTabCtrl__IEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;

    if (impl__IsMode2003_CMFCOutlookBarTabCtrl__QEBAHXZ(pThis) == 0)
        return;

    // TODO(clean-room): not transcribed -- the toolbar object at +0x428, its
    // vslot-0x6b0 button rebuild, the +0x1778 button map, the +0x418
    // CImageList and the enable-state virtuals (vslot 0x5d0/0x5d8) are not
    // modeled.
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::RecalcLayout()
//=============================================================================
// Retail (0x18009be80, ~0x670 bytes): early-outs when pThis is NULL, m_hWnd
// (+0x40) is NULL or the tab-button height (+0x1d8) is 0; otherwise it lays
// out the page buttons / scroll buttons and the internal toolbar using
// AFX_GLOBAL_DATA and the tab records.  The layout machinery is unmodeled.
// Symbol: ?RecalcLayout@CMFCOutlookBarTabCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CMFCOutlookBarTabCtrl__UEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    if (pTab->m_hWnd == nullptr || pTab->m_nTabButtonHeight == 0)
        return;

    // TODO(clean-room): not transcribed -- the page-button / scroll-button
    // layout and the internal toolbar placement depend on AFX_GLOBAL_DATA and
    // the per-tab records (array +0x108/+0x110); not modeled.
}

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::SetActiveTab(int nIndex)
//=============================================================================
// Retail (0x18009c590): bounds-checks `0 <= nIndex < count(+0x150) < size
// (+0x110)`, returns TRUE immediately when nIndex equals the active tab
// (+0x154), and otherwise performs the tab switch (IsMode2003 gate, parent
// virtuals vslot 0x800/0x808, EnableScrollButtons, vslot-0x318 relayout) and
// returns TRUE.  The internal records are unmodeled, so the switch is mirrored
// through the declared CMFCBaseTabCtrl side-table methods.
// Symbol: ?SetActiveTab@CMFCOutlookBarTabCtrl@@UEAAHH@Z
extern "C" int MS_ABI impl__SetActiveTab_CMFCOutlookBarTabCtrl__UEAAHH_Z(
    void* pThis, int nIndex)
{
    if (pThis == nullptr)
        return FALSE;

    CMFCBaseTabCtrl* pBase = static_cast<CMFCBaseTabCtrl*>(pThis);
    if (nIndex < 0 || nIndex >= pBase->GetTabsCount())
        return FALSE;
    if (pBase->GetActiveTab() == nIndex)
        return TRUE;

    // TODO(clean-room): transcribed partially -- retail switches the tab via
    // the parent virtuals (vslot 0x800/0x808), the scroll-button state and
    // the vslot-0x318 relayout; only the base side-table switch is applied.
    pBase->SetActiveTab(nIndex);

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    if (pTab->m_hWnd != nullptr)
        ::RedrawWindow(pTab->m_hWnd, nullptr, nullptr, 0x505);
    return TRUE;
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::SetBorderSize(int nSize)
//=============================================================================
// Retail (0x18009d930): `mov %edx,0x3c0(%rcx)` then tail-call vslot 0x318.
// Symbol: ?SetBorderSize@CMFCOutlookBarTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__SetBorderSize_CMFCOutlookBarTabCtrl__QEAAXH_Z(
    void* pThis, int nSize)
{
    if (pThis == nullptr)
        return;

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    pTab->m_nBorderSize = nSize;

    // TODO(clean-room): transcribed partially -- retail tail-calls the
    // relayout virtual (vslot 0x318); not modeled.
    if (pTab->m_hWnd != nullptr)
        ::RedrawWindow(pTab->m_hWnd, nullptr, nullptr, 0x505);
}

//=============================================================================
// void CMFCOutlookBarTabCtrl::SetPageButtonTextAlign(UINT nAlign,
//                                                    BOOL bRedraw)
//=============================================================================
// Retail (0x18009de80): stores nAlign at +0x3d8; when bRedraw is set and
// m_hWnd is valid it RedrawWindow's with flags 0x105.
// Symbol: ?SetPageButtonTextAlign@CMFCOutlookBarTabCtrl@@QEAAXIH@Z
extern "C" void MS_ABI impl__SetPageButtonTextAlign_CMFCOutlookBarTabCtrl__QEAAXIH_Z(
    void* pThis, unsigned int nAlign, int bRedraw)
{
    if (pThis == nullptr)
        return;

    OutlookBarTabCtrl* pTab = static_cast<OutlookBarTabCtrl*>(pThis);
    pTab->m_nPageButtonTextAlign = static_cast<int>(nAlign);

    if (bRedraw && pTab->m_hWnd != nullptr) {
        ::RedrawWindow(pTab->m_hWnd, nullptr, nullptr, 0x105);
    }
}

//=============================================================================
// CMFCOutlookBarTabCtrl
//=============================================================================

//=============================================================================
// int CMFCOutlookBarTabCtrl::SetToolbarImageList(UINT uiBitmapResID,
//                                                int cx,
//                                                ULONG nCols)
//=============================================================================
// Retail (RVA 0x9e610): mode-2003 only; loads the bitmap resource, builds a
// CImageList of cx x bm.bmHeight with a gutter based on nCols and installs it
// on the internal toolbar (+0x418/+0x420).  The image-list machinery and the
// internal toolbar are not modeled, so the failure path (0) is returned.
// Symbol: ?SetToolbarImageList@CMFCOutlookBarTabCtrl@@QEAAHIHK@Z
extern "C" int MS_ABI impl__SetToolbarImageList_CMFCOutlookBarTabCtrl__QEAAHIHK_Z(
    void* pThis, unsigned int uiBitmapResID, int cx, unsigned long nCols)
{
    (void)uiBitmapResID; (void)cx; (void)nCols;
    if (!pThis) return 0;
    if (!impl__IsMode2003_CMFCOutlookBarTabCtrl__QEBAHXZ(pThis)) return 0;
    // TODO(clean-room): transcribed partially -- bitmap/image-list creation and
    // the internal toolbar install are not modeled.
    return 0;
}
