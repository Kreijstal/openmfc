#pragma once
// Shared internals of the former global_cmfcoutlookbartabctrl.cpp translation unit.
// Definitions live in detail/CMFCOutlookBarTabCtrlSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcoutlookbartabctrl {} } }
using namespace openmfc::detail::cmfcoutlookbartabctrl;
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


//---------------------------------------------------------------------------
// Layout mirror (offsets harvested from the retail bodies).
//---------------------------------------------------------------------------

// CMFCOutlookBarTabCtrl, sizeof 11768 (0x2df8).  The first 0x40 bytes are the
// CObject/CWnd block (m_hWnd at 0x40); the CMFCBaseTabCtrl base owns the
// internal tab array (+0x108/+0x110), the count (+0x150) and the active tab
// (+0x154).  Only the fields this batch touches are named; the retail also
// accesses the +0x3fc scroll-button rect and the +0x400/+0x408 offsets, which
// alias that rect's top/bottom.
namespace openmfc { namespace detail { namespace cmfcoutlookbartabctrl {
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
} } }


//=============================================================================
// CMFCOutlookBarTabCtrl
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::AddControl(CWnd* pWnd, const wchar_t* lpszText,
//                                        int bIsInternal, UINT nID, DWORD dwStyle)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::CanShowFewerPageButtons() const
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::CanShowMorePageButtons() const
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::Create(const CRect& rect, CWnd* pParentWnd,
//                                    UINT nID)
//=============================================================================

//=============================================================================
// CObject* CMFCOutlookBarTabCtrl::CreateObject()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::DrawTabButton(CDC& dc, int nIndex, int bHighlight)
//=============================================================================

//=============================================================================
// CMFCOutlookBarTabCtrl
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::EnableAnimation(BOOL bEnable) [static]
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::EnableScrollButtons(BOOL bEnable,
//                                                 BOOL bShowUp,
//                                                 BOOL bShowDown)
//=============================================================================

//=============================================================================
// CWnd* CMFCOutlookBarTabCtrl::FindTargetWnd(const CPoint& point)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::GetTabArea(CRect& rectTabArea,
//                                        CRect& rectPaneArea) const
//=============================================================================

//=============================================================================
// int CMFCOutlookBarTabCtrl::GetTabNumberToDetach(int nIndex) const
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::IsMode2003() const
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::IsPtInTabArea(CPoint point) const
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::IsTabDetachable(int nIndex) const
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnCancelMode()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnChangeTabs()
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
//=============================================================================

//=============================================================================
// int CMFCOutlookBarTabCtrl::OnCreate(CREATESTRUCTW* lpCreateStruct)
//=============================================================================

//=============================================================================
// DROPEFFECT CMFCOutlookBarTabCtrl::OnDragEnter(COleDataObject*, DWORD,
//                                                CPoint)
//=============================================================================

//=============================================================================
// DROPEFFECT CMFCOutlookBarTabCtrl::OnDragOver(COleDataObject*, DWORD,
//                                              CPoint)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::OnEraseBkgnd(CDC* pDC)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnLButtonUp(UINT nFlags, CPoint point)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnPaint()
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest,
//                                         UINT nMessage)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnShowFewerPageButtons()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnShowMorePageButtons()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnShowOptions()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnSize(UINT nType, int cx, int cy)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::OnToolbarCommand(UINT uiCmd)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::RebuildToolBar()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::RecalcLayout()
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarTabCtrl::SetActiveTab(int nIndex)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::SetBorderSize(int nSize)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarTabCtrl::SetPageButtonTextAlign(UINT nAlign,
//                                                    BOOL bRedraw)
//=============================================================================

//=============================================================================
// CMFCOutlookBarTabCtrl
//=============================================================================

//=============================================================================
// int CMFCOutlookBarTabCtrl::SetToolbarImageList(UINT uiBitmapResID,
//                                                int cx,
//                                                ULONG nCols)
//=============================================================================
