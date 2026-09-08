// CDockingPanesRow — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The bodies below were decoded from the retail export, the way
// phase4/src/core/ole/COleControl.cpp describes; RVAs quoted in the comments are
// FUNCTION ENTRY POINTS in retail mfc140.dll (the ANSI twin of mfc140u — its
// function bodies are byte-identical, so offsets/flow read off it are valid, but
// its addresses are NOT mfc140u addresses).
//
// Instance layout (all offsets read out of the retail constructor
// ??0CDockingPanesRow@@QEAA@PEAVCDockSite@@HH@Z, RVA 0x4f3f0, cross-checked
// against the shipping afxdockingpanesrow.h member order and against the
// sizeof 112 already recorded for this class in
// phase4/src/featurepack/docking/RuntimeClasses.cpp):
//
//   0x00 vfptr                     0x28 CObList m_lstControlBars (56 bytes)
//   0x08 BOOL m_bIgnoreBarVisibility   ( head 0x30, tail 0x38, count 0x40 )
//   0x0C BOOL m_bVisible           0x60 DWORD m_dwRowAlignment
//   0x10 int  m_nRowHeight         0x68 CDockSite* m_pParentDockBar
//   0x14 int  m_nRowSavedHeight    0x70 == sizeof
//   0x18 int  m_nMinHeight
//   0x1C int  m_nRowOffset
//   0x20 int  m_nExtraSpace
//   0x24 int  m_nExtraAlignment
//
// Retail's own virtuals sit in the vftable that the constructor installs
// (mfc140 0x1802e4148): slot 6 GetAvailableSpace, 7 GetVisibleCount,
// 8 CalcFixedLayout, 9 AddPane, 10 AddPaneFromRow, 11 RemovePane,
// 12 ArrangePanes(CPane*), 13 ArrangePanes(int,int), 15 Resize, 16 Move,
// 17 RepositionPanes, 19 UpdateVisibleState, 20 ShowDockSiteRow,
// 21 ReplacePane, 22 ShowPane, 25 GetAvailableLength, 26 IsExclusiveRow,
// 27 OnInsertPane.  Where retail dispatches one of those through `this`, the
// code below calls the sibling impl__ thunk directly (this DLL has no C++
// method symbols to dispatch through) — a devirtualisation, noted per call.
//
// Two systematic deviations, stated once here:
//  * `pPane->IsVisible()` — retail calls CBasePane::IsVisible (0xb390), which
//    forwards to the tab container when IsTabbed(), returns
//    GetRecentVisibleState() while the module-wide "restoring state" flag is
//    set, and otherwise returns (GetStyle() & WS_VISIBLE) != 0.  OpenMFC's
//    impl__IsVisible_CBasePane__UEBAHXZ is still a `return 0` stub, which would
//    make every loop here a no-op, so PaneIsVisible() below implements the
//    third (ordinary-window) branch only.  The tabbed / restoring branches are
//    NOT implemented.
//  * Retail's `pWnd->ScreenToClient(CRect&)` (0x2a11f0) is reached here through
//    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z, which has a real body in
//    phase4/src/core/window/Thunks.cpp:1658.  Where the mapping target is a bare
//    HWND rather than a CWnd*, and for retail's ClientToScreen(CRect&) (0x2a1250),
//    the ScreenToClientRect() / ClientToScreenRect() helpers below expand the call
//    into the two ::ScreenToClient / ::ClientToScreen point calls that MFC's own
//    LPRECT overloads perform, so those sites need no CWnd temp map.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstring>

// ---------------------------------------------------------------------------
// Thunks defined elsewhere in this DLL (see briefing §1: C++ methods of these
// classes exist only as impl__ exports, so they must be called by thunk name).
// ---------------------------------------------------------------------------
extern "C" CObList::POSITION MS_ABI
    impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
extern "C" CObList::POSITION MS_ABI
    impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CObList* pThis, CObject* value);
extern "C" CObList::POSITION MS_ABI
    impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(
        CObList* pThis, CObList::POSITION* pPos, CObject* value);
extern "C" CObList::POSITION MS_ABI
    impl__InsertAfter_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(
        CObList* pThis, CObList::POSITION* pPos, CObject* value);
extern "C" void MS_ABI
    impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(CObList* pThis, CObList::POSITION* pPos);

extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis,
                                                                        const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__RecalcLayout_CPane__UEAAXXZ(CPane* pThis);
extern "C" void* MS_ABI impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
    CBasePane* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy,
    unsigned int nFlags, void* hdwp);

// Verified 2026-09-07 against phase4/src/featurepack/docking/CPane.cpp: of the
// four CPane helpers below, IsLeftOf (CPane.cpp:737), StretchPaneDeferWndPos
// (CPane.cpp:1491) and UpdateVirtualRect (CPane.cpp:1616) now have real bodies
// and already take an explicit `this`; only MoveByAlignment (CPane.cpp:817) is
// still a `return FALSE` stub.  They are re-declared here with the real
// (RCX = this) shape, which is what the export ABI actually passes.
extern "C" int MS_ABI impl__MoveByAlignment_CPane__QEAAHKH_Z(CPane* pThis,
                                                             unsigned long dwAlignment, int nOffset);
extern "C" int MS_ABI impl__IsLeftOf_CPane__QEBA_NVCRect___N_Z(const CPane* pThis, const CRect* pRect,
                                                               bool bWindowRect);
extern "C" int MS_ABI impl__StretchPaneDeferWndPos_CPane__UEAAHHAEAPEAX_Z(CPane* pThis, int nStretchSize,
                                                                          void** phdwp);
extern "C" void MS_ABI impl__UpdateVirtualRect_CPane__QEAAXXZ(CPane* pThis);
extern "C" int MS_ABI impl__ResizeRow_CDockSite__QEAAHPEAVCDockingPanesRow__HH_Z(
    void* pThis, void* pRow, int nNewSize, int bAdjustLayout);
extern "C" void MS_ABI impl__RemoveRow_CDockSite__QEAAXPEAVCDockingPanesRow___Z(void* pThis, void* pRow);
// Retail dispatches the per-pane fixed layout through CPane vslot 0x4d0, which
// is CControlBar::CalcFixedLayout.  NOTE the OpenMFC thunk takes the CSize
// return slot FIRST and `this` second (phase4/src/core/controlbar/CControlBar.cpp:116);
// it is called below in that declared order, not in the retail register order.
extern "C" void* MS_ABI impl__CalcFixedLayout_CControlBar__UEAA_AVCSize__HH_Z(
    void* pRet, void* pThis, int bStretch, int bHorz);

// Further thunks the bodies below call.  CPane::GetVirtualRect,
// GetAvailableExpandSize and GetAvailableStretchSize all have real bodies in
// phase4/src/featurepack/docking/CPane.cpp (lines 699, 627 and 649).
// CDockSite::ShowRow and CDockSite::RepositionPanes are still empty stubs in
// phase4/src/featurepack/docking/CDockSite.cpp (lines 277 and 264) and, like
// the CPane helpers above, are declared there without the implicit `this`;
// both are re-declared here with the real export shape.
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int  MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" void MS_ABI impl__GetVirtualRect_CPane__QEBAXAEAVCRect___Z(const CPane* pThis, RECT* pRect);
extern "C" int  MS_ABI impl__GetAvailableExpandSize_CPane__UEBAHXZ(const CPane* pThis);
extern "C" int  MS_ABI impl__GetAvailableStretchSize_CPane__UEBAHXZ(const CPane* pThis);
extern "C" void MS_ABI impl__RemoveAll_CObList__QEAAXXZ(CObList* pThis);
extern "C" void MS_ABI impl__ShowRow_CDockSite__QEAAXPEAVCDockingPanesRow__HH_Z(
    void* pThis, void* pRow, int bShow, int bAdjustLayout);
extern "C" void MS_ABI impl__RepositionPanes_CDockSite__UEAAXAEAVCRect___Z(void* pThis, RECT* pRect);

// ---------------------------------------------------------------------------
// Shadow layout + local helpers
// ---------------------------------------------------------------------------
namespace {

struct S_CDockingPanesRow {
    void*   vfptr;                    // 0x00
    BOOL    m_bIgnoreBarVisibility;   // 0x08
    BOOL    m_bVisible;               // 0x0C
    int     m_nRowHeight;             // 0x10
    int     m_nRowSavedHeight;        // 0x14
    int     m_nMinHeight;             // 0x18
    int     m_nRowOffset;             // 0x1C
    int     m_nExtraSpace;            // 0x20
    int     m_nExtraAlignment;        // 0x24
    CObList m_lstControlBars;         // 0x28
    DWORD   m_dwRowAlignment;         // 0x60
    DWORD   m_dwRowAlignmentPad;      // 0x64
    CWnd*   m_pParentDockBar;         // 0x68  (a CDockSite)
};

static_assert(offsetof(S_CDockingPanesRow, m_bIgnoreBarVisibility) == 0x08, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_bVisible)             == 0x0C, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_nRowHeight)           == 0x10, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_nRowOffset)           == 0x1C, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_nExtraSpace)          == 0x20, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_lstControlBars)       == 0x28, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_dwRowAlignment)       == 0x60, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_pParentDockBar)       == 0x68, "row layout");
static_assert(sizeof(S_CDockingPanesRow) == 0x70, "CDockingPanesRow is 112 bytes in retail");

// CBRS_ALIGN_* as MFC defines them (the copy in include/openmfc/afxole.h is the
// unrelated 0x000F flavour, so the values are spelled out here).  Retail tests
// `m_dwRowAlignment & 0xA000` for CDockingPanesRow::IsHorizontal().
const DWORD kAlignLeft   = 0x1000;
const DWORD kAlignTop    = 0x2000;
const DWORD kAlignRight  = 0x4000;
const DWORD kAlignBottom = 0x8000;
const DWORD kAlignAny    = 0xF000;

inline S_CDockingPanesRow* Row(void* p) { return static_cast<S_CDockingPanesRow*>(p); }
inline const S_CDockingPanesRow* Row(const void* p) { return static_cast<const S_CDockingPanesRow*>(p); }

inline bool RowIsHorz(const S_CDockingPanesRow* s) {
    return (s->m_dwRowAlignment & (kAlignTop | kAlignBottom)) != 0;
}

// Mirror of CList<CObject*, CObject*>::CNode (include/openmfc/afx.h) -- the node
// a CObList::POSITION points at.  CObList's own accessors (GetHeadPosition /
// GetNext / GetAt / GetCount) are inline in retail MFC, so they have no export
// and therefore no impl__ thunk to call; this read-only view plus the exported
// FindIndex thunk is how the list is walked here.  Never constructed.
struct ObNode {
    ObNode*  pNext;
    ObNode*  pPrev;
    CObject* data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");

inline ObNode* NodeFromPos(CObList::POSITION pos) {
    ObNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof(p));
    return p;
}

inline CObList::POSITION PosFromNode(ObNode* n) {
    CObList::POSITION pos(nullptr);
    std::memcpy(&pos, &n, sizeof(n));
    return pos;
}

// Retail calls the CRT `abs` / `labs` imports (0x1802c5910 / 0x1802c5908) all
// over this file; both are used on plain ints here.
inline int AbsInt(int v) { return v < 0 ? -v : v; }

// A CPoint passed BY VALUE.  MSVC x64 hands an 8-byte trivially-copyable class
// over in a single integer register (low dword = x, high dword = y), which is
// exactly what the retail bodies read -- e.g. MovePane(CPoint) (0x50300) takes
// the point in R8, uses EBX for x and the homed high dword for y.  Declaring
// the parameter as a 64-bit scalar reproduces that register shape exactly.
inline int PtX(long long pt) { return static_cast<int>(static_cast<unsigned long long>(pt) & 0xFFFFFFFFull); }
inline int PtY(long long pt) { return static_cast<int>(static_cast<unsigned long long>(pt) >> 32); }
inline long long MakePt(int x, int y) {
    return static_cast<long long>((static_cast<unsigned long long>(static_cast<unsigned int>(y)) << 32) |
                                  static_cast<unsigned int>(x));
}

inline ObNode* HeadNode(const S_CDockingPanesRow* s) {
    return NodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(&s->m_lstControlBars, 0));
}

inline ObNode* TailNode(const S_CDockingPanesRow* s) {
    ObNode* n = HeadNode(s);
    if (n == nullptr) return nullptr;
    while (n->pNext != nullptr) n = n->pNext;
    return n;
}

// Retail reads m_lstControlBars.m_nCount at +0x40 directly; OpenMFC's CObList
// keeps its contents in a side table and leaves that word at zero, so the count
// is recomputed by walking.
inline int ListCount(const S_CDockingPanesRow* s) {
    int n = 0;
    for (ObNode* p = HeadNode(s); p != nullptr; p = p->pNext) ++n;
    return n;
}

// include/openmfc/afx*.h models CRect as a standalone {left,top,right,bottom}
// class rather than deriving it from RECT, so the Win32 rect APIs need an
// explicit -- and layout-identical -- reinterpretation.
static_assert(sizeof(CRect) == sizeof(RECT), "CRect must be RECT-shaped");
static_assert(offsetof(CRect, left) == offsetof(RECT, left), "CRect must be RECT-shaped");
static_assert(offsetof(CRect, top) == offsetof(RECT, top), "CRect must be RECT-shaped");
static_assert(offsetof(CRect, right) == offsetof(RECT, right), "CRect must be RECT-shaped");
static_assert(offsetof(CRect, bottom) == offsetof(RECT, bottom), "CRect must be RECT-shaped");
inline RECT* RP(CRect* r) { return reinterpret_cast<RECT*>(r); }
inline RECT* RP(CRect& r) { return reinterpret_cast<RECT*>(&r); }
inline const RECT* RP(const CRect* r) { return reinterpret_cast<const RECT*>(r); }

inline CPane* PaneOf(ObNode* n) { return reinterpret_cast<CPane*>(n->data); }

// See the file header: only the ordinary-window branch of retail
// CBasePane::IsVisible (0xb390) is implemented.
BOOL PaneIsVisible(const CBasePane* p) {
    if (p == nullptr || p->m_hWnd == nullptr) return FALSE;
    return (::GetWindowLongPtrW(p->m_hWnd, GWL_STYLE) & WS_VISIBLE) != 0 ? TRUE : FALSE;
}

// The `pBar->IsVisible() || m_bIgnoreBarVisibility` guard retail repeats in
// every loop over the pane list (it tests m_bIgnoreBarVisibility first only in
// the sense that a non-zero flag makes the visibility answer irrelevant).
inline bool PaneCounts(const S_CDockingPanesRow* s, const CBasePane* p) {
    return PaneIsVisible(p) != FALSE || s->m_bIgnoreBarVisibility != 0;
}

// MFC's CWnd::ScreenToClient(LPRECT) / ClientToScreen(LPRECT) expanded.
inline void ScreenToClientRect(HWND hWnd, CRect* pRect) {
    if (hWnd == nullptr || pRect == nullptr) return;
    ::ScreenToClient(hWnd, reinterpret_cast<POINT*>(&pRect->left));
    ::ScreenToClient(hWnd, reinterpret_cast<POINT*>(&pRect->right));
}

inline void ClientToScreenRect(HWND hWnd, CRect* pRect) {
    if (hWnd == nullptr || pRect == nullptr) return;
    ::ClientToScreen(hWnd, reinterpret_cast<POINT*>(&pRect->left));
    ::ClientToScreen(hWnd, reinterpret_cast<POINT*>(&pRect->right));
}

// CPane::GetVirtualRect (0xa1ca0): copy m_rectVirtual (+0x1E0) and map it from
// the pane's parent client area to screen coordinates.  Verified 2026-09-07:
// impl__GetVirtualRect_CPane__QEBAXAEAVCRect___Z now has a real body
// (phase4/src/featurepack/docking/CPane.cpp:699), so this routes through it;
// the local expansion it replaced computed the same rect.
void GetPaneVirtualRect(const CPane* pPane, CRect& rect) {
    rect = CRect(0, 0, 0, 0);
    impl__GetVirtualRect_CPane__QEBAXAEAVCRect___Z(pPane, RP(&rect));
}

} // namespace

// Same-file thunks used before their definition.
extern "C" void MS_ABI impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(const void* pThis, CRect* pRect);
extern "C" void MS_ABI impl__GetClientRect_CDockingPanesRow__QEBAXAEAVCRect___Z(const void* pThis, CRect* pRect);
extern "C" int  MS_ABI impl__GetAvailableLength_CDockingPanesRow__UEBAHH_Z(const void* pThis, int bUseVirtualRect);
extern "C" int  MS_ABI impl__GetAvailableLengthRect_CDockingPanesRow__IEAAHXZ(void* pThis);
extern "C" int  MS_ABI impl__GetMaxPaneSize_CDockingPanesRow__QEBAHH_Z(const void* pThis, int bSkipHiddenBars);
extern "C" void MS_ABI impl__BeginTrans_CDockingPanesRow__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__CommitTrans_CDockingPanesRow__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__ArrangePanes_CDockingPanesRow__UEAAXPEAVCPane___Z(void* pThis, CPane* pInitialBar);
extern "C" void MS_ABI impl__ArrangePanesRect_CDockingPanesRow__IEAAXPEAVCPane___Z(void* pThis, CPane* pInitialBar);
extern "C" int  MS_ABI impl__StretchPaneRect_CDockingPanesRow__IEAAHPEAVCPane__H_Z(void* pThis, CPane* pBar, int nStretchSize);
extern "C" void MS_ABI impl__UpdateVisibleState_CDockingPanesRow__UEAAXH_Z(void* pThis, int bDelay);
extern "C" void MS_ABI impl__ExpandStretchedPanes_CDockingPanesRow__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__FixupVirtualRects_CDockingPanesRow__QEAAX_NPEAVCPane___Z(void* pThis, bool bMoveBackToVirtualRect, CPane* pBarToExclude);
extern "C" void MS_ABI impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCRect__AEAPEAX_Z(void* pThis, CPane* pBar, const CRect* pRectTarget, void** phdwp);
extern "C" void MS_ABI impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__H_NAEAPEAX_Z(void* pThis, CPane* pBar, int nOffset, bool bForward, void** phdwp);
extern "C" void MS_ABI impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCPoint__HAEAPEAX_Z(void* pThis, CPane* pBar, long long ptOffset, int nExtra, void** phdwp);
extern "C" void MS_ABI impl__MovePaneRect_CDockingPanesRow__IEAAXPEAVCPane__H_N_Z(void* pThis, CPane* pBar, int nOffset, bool bForward);
extern "C" int  MS_ABI impl__CheckPanes_CDockingPanesRow__IEAAHAEAVCRect__PEAVCPane___NVCPoint__HAEAPEAX_Z(void* pThis, CRect* pRect, CPane* pBar, bool bForward, long long ptOffset, int nExtra, void** phdwp);
extern "C" void MS_ABI impl__MoveTrailingPanes_CDockingPanesRow__AEAAXPEAU__POSITION__VCPoint___NPEAVCPane__AEAPEAX_Z(void* pThis, void* pos, long long ptOffset, bool bForward, CPane* pBarToExclude, void** phdwp);
extern "C" void MS_ABI impl__ResolveIntersection_CDockingPanesRow__AEAAXPEAVCPane___NAEAPEAX_Z(void* pThis, CPane* pBar, bool bForward, void** phdwp);
extern "C" void MS_ABI impl__ResolveIntersectionRect_CDockingPanesRow__IEAAXPEAVCPane___N_Z(void* pThis, CPane* pBar, bool bForward);
extern "C" void MS_ABI impl__ShiftPanes_CDockingPanesRow__AEAAXPEAVCPane__HH_Z(void* pThis, CPane* pBar, int nOffset, int bForward);
extern "C" void MS_ABI impl__ShiftPanesRect_CDockingPanesRow__IEAAXPEAVCPane__HH_Z(void* pThis, CPane* pBar, int nOffset, int bForward);
extern "C" void MS_ABI impl__ShowDockSiteRow_CDockingPanesRow__UEAAXHH_Z(void* pThis, int bShow, int bDelay);
extern "C" void MS_ABI impl__RepositionPanes_CDockingPanesRow__UEAAXAEAVCRect__IHH_Z(void* pThis, CRect* pRectNew, unsigned int nSide, int bAdjust, int nSizeDelta);
extern "C" void MS_ABI impl__AdjustPaneToRowArea_CDockingPanesRow__IEAAXPEAVCPane__AEBVCRect__AEAPEAX_Z(void* pThis, CPane* pPane, const CRect* pRectRow, void** phdwp);
extern "C" CPane* MS_ABI impl__FindFirstVisiblePane_CDockingPanesRow__IEAAPEAVCPane__H_Z(void* pThis, int bForward);
extern "C" int  MS_ABI impl__GetOutOfBoundsOffset_CDockingPanesRow__AEAAHPEAVCPane__H_Z(void* pThis, CPane* pBar, int bLeftTopBound);
extern "C" int  MS_ABI impl__GetOutOfBoundsOffsetRect_CDockingPanesRow__IEAAHPEAVCPane__H_Z(void* pThis, CPane* pBar, int bLeftTopBound);
extern "C" int  MS_ABI impl__IsEnoughSpaceToMove_CDockingPanesRow__AEAAHPEAVCPane___NAEAH_Z(void* pThis, CPane* pControlBar, bool bForward, int* pnAllowedOffset);
extern "C" int  MS_ABI impl__GetVisibleCount_CDockingPanesRow__UEAAHXZ(void* pThis);
extern "C" int  MS_ABI impl__IsExclusiveRow_CDockingPanesRow__UEBAHXZ(const void* pThis);
extern "C" void MS_ABI impl__OnInsertPane_CDockingPanesRow__MEAAXPEAVCPane___Z(void* pThis, CPane* pControlBar);

// Symbol: ??0CDockingPanesRow@@QEAA@PEAVCDockSite@@HH@Z
extern "C" void* MS_ABI impl___0CDockingPanesRow__QEAA_PEAVCDockSite__HH_Z(
    void* pThis, void* pDockSite, int nRows, int nCols) {
    (void)pDockSite;
    (void)nRows;
    (void)nCols;
    return pThis;
}
// Symbol: ??1CDockingPanesRow@@UEAA@XZ
// Retail 0x4f4f0 is six instructions: store the CDockingPanesRow vftable into
// *this, store the CObList vftable into m_lstControlBars (+0x28), and tail-jump
// into CPtrList::RemoveAll (0x83d0) with `this + 0x28` in RCX -- i.e. the whole
// destructor body is `m_lstControlBars.RemoveAll()`.  OpenMFC has no retail
// vftables to install, so only the list teardown is reproduced; that is the
// entire observable effect.
extern "C" void MS_ABI impl___1CDockingPanesRow__UEAA_XZ(void* pThis) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;
    impl__RemoveAll_CObList__QEAAXXZ(&s->m_lstControlBars);
}
// Symbol: ?AddPane@CDockingPanesRow@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@PEBUtagRECT@@H@Z
// Retail 0x4f510.  Not transcribed: it branches on AFX_DOCK_METHOD, calls
// CPane::DockByMouse/UndockPane paths and CDockSite bookkeeping that this tree
// does not model yet; the ordering of those side effects cannot be reproduced
// faithfully from the list state alone.  Left a stub deliberately.
extern "C" void MS_ABI impl__AddPane_CDockingPanesRow__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT__H_Z(void* /*class*/* p0, int /*enum*/ p1, short* p2, int p3, char p4, void* p5, void* p6, unsigned long p7, int p8, float p9, unsigned char p10, void* p11, int p12, void* p13, char p14, const void* /*struct*/* p15, int p16) {}

// Symbol: ?AddPaneFromRow@CDockingPanesRow@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@@Z
// Retail 0x4f6f0 (mfc140, the ANSI twin), transcribed in full -- 114 instructions:
//   CRect rectBar; ::GetWindowRect(pBar->m_hWnd, &rectBar);       // screen coords
//   int nOffset;
//   if (dockMethod == 1) {                        // cmp $0x1,%ebx / jne
//       CPoint pt; ::GetCursorPos(&pt);
//       ::ScreenToClient(m_pParentDockBar->m_hWnd, &pt);
//       CRect rectClient; ::GetClientRect(pBar->m_hWnd, &rectClient);
//       pBar->ClientToScreen(rectClient);                          // 0x2a1250
//       int nBorder = rectClient.left - rectBar.left;
//       pt.x -= nBorder + pBar->m_ptClientHotSpot.x;               // CPane +0x208
//       pt.y -= nBorder + pBar->m_ptClientHotSpot.y;               // CPane +0x20c
//       nOffset = IsHorizontal() ? pt.x : pt.y;   // test $0xa000,m_dwRowAlignment
//   } else {
//       m_pParentDockBar->ScreenToClient(rectBar);                 // 0x2a11f0
//       nOffset = IsHorizontal() ? rectBar.left : rectBar.top;
//   }
//   CRect rectNew;
//   if (IsHorizontal()) ::SetRect(&rectNew, nOffset, m_nRowOffset,
//                                 nOffset + rectBar.Width(),
//                                 m_nRowOffset + rectBar.Height());
//   else                ::SetRect(&rectNew, m_nRowOffset, nOffset,
//                                 m_nRowOffset + rectBar.Width(),
//                                 nOffset + rectBar.Height());
//   pBar->SetWindowPos(NULL, rectNew.left, rectNew.top,
//                      rectNew.right, rectNew.bottom, 0x15, NULL);  // vslot 0x480
//   OnInsertPane(pBar);                            // vslot 0xd8 == slot 27
//   pBar->UpdateVirtualRect();                     // 0xa1ae0
// Two things worth knowing before editing this: retail really does pass
// rectNew.right/bottom in the cx/cy slots of SetWindowPos, which is harmless
// only because 0x15 is SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE and cx/cy are
// therefore ignored; and on the dockMethod==1 path rectBar is never mapped out
// of screen coordinates, so only its WIDTH and HEIGHT feed rectNew there.
// `dockMethod` is compared against the literal 1 because this tree does not
// model the AFX_DOCK_METHOD enum; the retail instruction is `cmp $0x1,%ebx`.
extern "C" void MS_ABI impl__AddPaneFromRow_CDockingPanesRow__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(
    void* pThis, CPane* pBar, int dockMethod) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pBar == nullptr) return;

    CRect rectBar(0, 0, 0, 0);
    ::GetWindowRect(pBar->m_hWnd, RP(&rectBar));

    const bool bHorz = RowIsHorz(s);
    int nOffset;
    if (dockMethod == 1) {
        POINT pt = { 0, 0 };
        ::GetCursorPos(&pt);
        if (s->m_pParentDockBar != nullptr)
            ::ScreenToClient(s->m_pParentDockBar->m_hWnd, &pt);

        CRect rectClient(0, 0, 0, 0);
        ::GetClientRect(pBar->m_hWnd, RP(&rectClient));
        ClientToScreenRect(pBar->m_hWnd, &rectClient);

        const int nBorder = rectClient.left - rectBar.left;
        pt.x -= nBorder + pBar->m_ptClientHotSpot.x;
        pt.y -= nBorder + pBar->m_ptClientHotSpot.y;
        nOffset = bHorz ? pt.x : pt.y;
    } else {
        if (s->m_pParentDockBar != nullptr)
            impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(s->m_pParentDockBar, RP(&rectBar));
        nOffset = bHorz ? rectBar.left : rectBar.top;
    }

    CRect rectNew(0, 0, 0, 0);
    if (bHorz)
        ::SetRect(RP(&rectNew), nOffset, s->m_nRowOffset,
                  nOffset + rectBar.Width(), s->m_nRowOffset + rectBar.Height());
    else
        ::SetRect(RP(&rectNew), s->m_nRowOffset, nOffset,
                  s->m_nRowOffset + rectBar.Width(), nOffset + rectBar.Height());

    impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
        pBar, nullptr, rectNew.left, rectNew.top, rectNew.right, rectNew.bottom,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE, nullptr);

    impl__OnInsertPane_CDockingPanesRow__MEAAXPEAVCPane___Z(pThis, pBar);
    impl__UpdateVirtualRect_CPane__QEAAXXZ(pBar);
}

// Symbol: ?AdjustPaneToRowArea@CDockingPanesRow@@IEAAXPEAVCPane@@AEBVCRect@@AEAPEAX@Z
// Retail 0x520f0, transcribed in full (71 instructions).  It takes the pane's
// live window rect ONCE and then makes up to two MovePane(CPoint) calls to push
// it back inside rectRow along the row axis:
//   CRect rectPane; ::GetWindowRect(pPane->m_hWnd, &rectPane);
//   if (IsHorizontal()) {
//       if (rectPane.left  < rectRow.left)
//           MovePane(pPane, CPoint(rectRow.left - rectPane.left, 0), 0, hdwp);
//       if (rectPane.right > rectRow.right)
//           MovePane(pPane, CPoint(rectRow.right - rectPane.right, 0), 0, hdwp);
//   } else {
//       if (rectPane.top    < rectRow.top)
//           MovePane(pPane, CPoint(0, rectRow.top - rectPane.top), 0, hdwp);
//       if (rectPane.bottom > rectRow.bottom)
//           MovePane(pPane, CPoint(0, rectRow.bottom - rectPane.bottom), 0, hdwp);
//   }
// rectPane is deliberately NOT re-read between the two tests -- retail reloads
// the same stack slot (0x38(%rsp)/0x40(%rsp)) for the second comparison.
extern "C" void MS_ABI impl__AdjustPaneToRowArea_CDockingPanesRow__IEAAXPEAVCPane__AEBVCRect__AEAPEAX_Z(void* pThis, CPane* pPane, const CRect* pRectRow, void** phdwp) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pPane == nullptr || pRectRow == nullptr) return;

    CRect rectPane(0, 0, 0, 0);
    ::GetWindowRect(pPane->m_hWnd, RP(&rectPane));

    if (RowIsHorz(s)) {
        if (rectPane.left < pRectRow->left)
            impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCPoint__HAEAPEAX_Z(
                pThis, pPane, MakePt(pRectRow->left - rectPane.left, 0), 0, phdwp);
        if (rectPane.right > pRectRow->right)
            impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCPoint__HAEAPEAX_Z(
                pThis, pPane, MakePt(pRectRow->right - rectPane.right, 0), 0, phdwp);
    } else {
        if (rectPane.top < pRectRow->top)
            impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCPoint__HAEAPEAX_Z(
                pThis, pPane, MakePt(0, pRectRow->top - rectPane.top), 0, phdwp);
        if (rectPane.bottom > pRectRow->bottom)
            impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCPoint__HAEAPEAX_Z(
                pThis, pPane, MakePt(0, pRectRow->bottom - rectPane.bottom), 0, phdwp);
    }
}

// Symbol: ?ArrangePanes@CDockingPanesRow@@UEAAXHH@Z
// Retail 0x51510.  Not transcribed: the margin/spacing walk drives
// CPane::SetWindowPos through a DeferWindowPos chain interleaved with
// ResolveIntersection; left a stub deliberately.
extern "C" void MS_ABI impl__ArrangePanes_CDockingPanesRow__UEAAXHH_Z(int p0, int p1) {}

// Symbol: ?ArrangePanes@CDockingPanesRow@@UEAAXPEAVCPane@@@Z
// Retail 0x51740, transcribed in full.  Every helper it calls now has a real
// body in this file, so nothing here is a stand-in:
//   if (m_lstControlBars.GetCount() == 0) return;
//   CRect rectClient; GetClientRect(rectClient);            // 0x4fd50
//   if (::IsRectEmpty(&rectClient)) return;
//   HDWP hdwp = NULL;                                       // never handed to
//                                                           // ::DeferWindowPos
//   int nAvail = GetAvailableLength(FALSE);                 // vslot 0xc8
//   if (GetCount() == 1) {
//       if (pInitialBar == NULL) pInitialBar = GetHead();
//       if (nAvail < 0) {                                   // jns -> general path
//           pInitialBar->StretchPaneDeferWndPos(nAvail, hdwp);   // vslot 0x568
//           CRect rectBar; ::GetWindowRect(pInitialBar->m_hWnd, &rectBar);
//           m_pParentDockBar->ScreenToClient(rectBar);
//           ::OffsetRect(&rectBar, horz ? -rectBar.left : m_nRowOffset - rectBar.left,
//                                  horz ? m_nRowOffset - rectBar.top : -rectBar.top);
//           pInitialBar->SetWindowPos(NULL, rectBar.left, rectBar.top,
//                                     rectBar.Width(), rectBar.Height(),
//                                     SWP_NOZORDER|SWP_NOACTIVATE, NULL);
//           return;
//       }
//   }
//   if (pInitialBar != NULL) ResolveIntersection(pInitialBar, FALSE, hdwp);
//   else                     pInitialBar = GetHead();
//   ResolveIntersection(pInitialBar, TRUE, hdwp);
//   CPane* p = FindFirstVisiblePane(TRUE);
//   int n = GetOutOfBoundsOffset(p, TRUE);
//   if (n > 0) ShiftPanes(p, n, TRUE);
//   p = FindFirstVisiblePane(FALSE);
//   n = GetOutOfBoundsOffset(p, FALSE);
//   if (n <= 0) return;
//   if (nAvail > 0) { ShiftPanes(p, -n, FALSE); return; }
//   ShiftPanes(p, abs(nAvail) - n, FALSE);
//   if (nAvail >= 0) return;
//   for (node = tail; node != NULL; node = node->pPrev) {      // 0x5195b
//       pBar = node->data;
//       if (!pBar->IsVisible() && !m_bIgnoreBarVisibility) continue;
//       int nStretched = pBar->StretchPaneDeferWndPos(nAvail, hdwp);
//       MovePane(pBar, abs(nAvail) - abs(nStretched), FALSE, hdwp);
//       if (nStretched == nAvail) return;
//       nAvail -= nStretched;
//   }
// Note the count==1 / nAvail>=0 combination falls THROUGH into the general
// path with pInitialBar already defaulted to the head pane -- that is retail's
// `jns 0x180051893`, not a return.
extern "C" void MS_ABI impl__ArrangePanes_CDockingPanesRow__UEAAXPEAVCPane___Z(void* pThis, CPane* pInitialBar) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;

    ObNode* pHead = HeadNode(s);
    if (pHead == nullptr) return;

    CRect rectClient(0, 0, 0, 0);
    impl__GetClientRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, &rectClient);
    if (::IsRectEmpty(RP(&rectClient))) return;

    void* hdwp = nullptr;
    int nAvailable = impl__GetAvailableLength_CDockingPanesRow__UEBAHH_Z(pThis, FALSE);

    if (ListCount(s) == 1) {
        if (pInitialBar == nullptr) pInitialBar = PaneOf(pHead);
        if (nAvailable < 0) {
            if (pInitialBar == nullptr) return;
            impl__StretchPaneDeferWndPos_CPane__UEAAHHAEAPEAX_Z(pInitialBar, nAvailable, &hdwp);

            CRect rectBar(0, 0, 0, 0);
            ::GetWindowRect(pInitialBar->m_hWnd, RP(&rectBar));
            if (s->m_pParentDockBar != nullptr)
                impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(s->m_pParentDockBar, RP(&rectBar));

            if (RowIsHorz(s))
                ::OffsetRect(RP(&rectBar), -rectBar.left, s->m_nRowOffset - rectBar.top);
            else
                ::OffsetRect(RP(&rectBar), s->m_nRowOffset - rectBar.left, -rectBar.top);

            impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
                pInitialBar, nullptr, rectBar.left, rectBar.top,
                rectBar.Width(), rectBar.Height(), SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
            return;
        }
    }

    if (pInitialBar != nullptr)
        impl__ResolveIntersection_CDockingPanesRow__AEAAXPEAVCPane___NAEAPEAX_Z(pThis, pInitialBar, false, &hdwp);
    else
        pInitialBar = PaneOf(pHead);
    impl__ResolveIntersection_CDockingPanesRow__AEAAXPEAVCPane___NAEAPEAX_Z(pThis, pInitialBar, true, &hdwp);

    CPane* pFirst = impl__FindFirstVisiblePane_CDockingPanesRow__IEAAPEAVCPane__H_Z(pThis, TRUE);
    const int nLead = impl__GetOutOfBoundsOffset_CDockingPanesRow__AEAAHPEAVCPane__H_Z(pThis, pFirst, TRUE);
    if (nLead > 0)
        impl__ShiftPanes_CDockingPanesRow__AEAAXPEAVCPane__HH_Z(pThis, pFirst, nLead, TRUE);

    CPane* pLast = impl__FindFirstVisiblePane_CDockingPanesRow__IEAAPEAVCPane__H_Z(pThis, FALSE);
    const int nTrail = impl__GetOutOfBoundsOffset_CDockingPanesRow__AEAAHPEAVCPane__H_Z(pThis, pLast, FALSE);
    if (nTrail <= 0) return;

    if (nAvailable > 0) {
        impl__ShiftPanes_CDockingPanesRow__AEAAXPEAVCPane__HH_Z(pThis, pLast, -nTrail, FALSE);
        return;
    }
    impl__ShiftPanes_CDockingPanesRow__AEAAXPEAVCPane__HH_Z(
        pThis, pLast, AbsInt(nAvailable) - nTrail, FALSE);
    if (nAvailable >= 0) return;

    for (ObNode* n = TailNode(s); n != nullptr; ) {
        CPane* pBar = PaneOf(n);
        ObNode* pPrev = n->pPrev;
        if (pBar != nullptr && PaneCounts(s, pBar)) {
            const int nStretched =
                impl__StretchPaneDeferWndPos_CPane__UEAAHHAEAPEAX_Z(pBar, nAvailable, &hdwp);
            impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__H_NAEAPEAX_Z(
                pThis, pBar, AbsInt(nAvailable) - AbsInt(nStretched), false, &hdwp);
            if (nStretched == nAvailable) return;
            nAvailable -= nStretched;
        }
        n = pPrev;
    }
}

// Symbol: ?ArrangePanesRect@CDockingPanesRow@@IEAAXPEAVCPane@@@Z
// Retail 0x52730 -- the m_rectBar ("Rect") twin of ArrangePanes(CPane*),
// instruction-for-instruction the same shape with the staged-rect helpers
// substituted: GetAvailableLengthRect (0x525a0) for GetAvailableLength,
// ResolveIntersectionRect (0x52920), GetOutOfBoundsOffsetRect (0x52b10),
// ShiftPanesRect (0x52bd0) and MovePaneRect (0x52d60).  Two real differences:
//   * the single-pane / nAvail<0 case is one call to StretchPaneRect(pBar,
//     nAvail) (0x52660) instead of the stretch + ScreenToClient + SetWindowPos
//     sequence the window-rect twin runs;
//   * the trailing walk still calls the WINDOW-rect CPane::StretchPaneDeferWndPos
//     (vslot 0x568) -- there is no "Rect" stretch there -- and then MovePaneRect,
//     which takes no HDWP.
// The HDWP local exists in retail only to be handed to StretchPaneDeferWndPos.
extern "C" void MS_ABI impl__ArrangePanesRect_CDockingPanesRow__IEAAXPEAVCPane___Z(void* pThis, CPane* pInitialBar) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;

    ObNode* pHead = HeadNode(s);
    if (pHead == nullptr) return;

    CRect rectClient(0, 0, 0, 0);
    impl__GetClientRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, &rectClient);
    if (::IsRectEmpty(RP(&rectClient))) return;

    void* hdwp = nullptr;
    int nAvailable = impl__GetAvailableLengthRect_CDockingPanesRow__IEAAHXZ(pThis);

    if (ListCount(s) == 1) {
        if (pInitialBar == nullptr) pInitialBar = PaneOf(pHead);
        if (nAvailable < 0) {
            impl__StretchPaneRect_CDockingPanesRow__IEAAHPEAVCPane__H_Z(pThis, pInitialBar, nAvailable);
            return;
        }
    }

    if (pInitialBar != nullptr)
        impl__ResolveIntersectionRect_CDockingPanesRow__IEAAXPEAVCPane___N_Z(pThis, pInitialBar, false);
    else
        pInitialBar = PaneOf(pHead);
    impl__ResolveIntersectionRect_CDockingPanesRow__IEAAXPEAVCPane___N_Z(pThis, pInitialBar, true);

    CPane* pFirst = impl__FindFirstVisiblePane_CDockingPanesRow__IEAAPEAVCPane__H_Z(pThis, TRUE);
    const int nLead = impl__GetOutOfBoundsOffsetRect_CDockingPanesRow__IEAAHPEAVCPane__H_Z(pThis, pFirst, TRUE);
    if (nLead > 0)
        impl__ShiftPanesRect_CDockingPanesRow__IEAAXPEAVCPane__HH_Z(pThis, pFirst, nLead, TRUE);

    CPane* pLast = impl__FindFirstVisiblePane_CDockingPanesRow__IEAAPEAVCPane__H_Z(pThis, FALSE);
    const int nTrail = impl__GetOutOfBoundsOffsetRect_CDockingPanesRow__IEAAHPEAVCPane__H_Z(pThis, pLast, FALSE);
    if (nTrail <= 0) return;

    if (nAvailable > 0) {
        impl__ShiftPanesRect_CDockingPanesRow__IEAAXPEAVCPane__HH_Z(pThis, pLast, -nTrail, FALSE);
        return;
    }
    impl__ShiftPanesRect_CDockingPanesRow__IEAAXPEAVCPane__HH_Z(
        pThis, pLast, AbsInt(nAvailable) - nTrail, FALSE);
    if (nAvailable >= 0) return;

    for (ObNode* n = TailNode(s); n != nullptr; ) {
        CPane* pBar = PaneOf(n);
        ObNode* pPrev = n->pPrev;
        if (pBar != nullptr && PaneCounts(s, pBar)) {
            const int nStretched =
                impl__StretchPaneDeferWndPos_CPane__UEAAHHAEAPEAX_Z(pBar, nAvailable, &hdwp);
            impl__MovePaneRect_CDockingPanesRow__IEAAXPEAVCPane__H_N_Z(
                pThis, pBar, AbsInt(nAvailable) - AbsInt(nStretched), false);
            if (nStretched == nAvailable) return;
            nAvailable -= nStretched;
        }
        n = pPrev;
    }
}

// Symbol: ?BeginTrans@CDockingPanesRow@@IEAAXXZ
// Retail 0x52410: for every pane that counts as visible, seed its m_rectBar
// (+0x118) straight from ::GetWindowRect.  Nothing else.
extern "C" void MS_ABI impl__BeginTrans_CDockingPanesRow__IEAAXXZ(void* pThis) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr || !PaneCounts(s, pBar)) continue;
        ::GetWindowRect(pBar->m_hWnd, RP(&pBar->m_rectBar));
    }
}

// Symbol: ?CalcFixedLayout@CDockingPanesRow@@UEAA?AVCSize@@HH@Z
// Retail 0x4fae0.  Empty row (m_bVisible == 0) answers {0,0x7FFF} when the row
// is vertical and {0x7FFF,0x7FFF} when it is horizontal; otherwise it sums the
// panes' own CalcFixedLayout (CPane vslot 0x4d0) along the row and takes the max
// across it, then adds m_nExtraSpace to the summed axis if that axis is > 0.
// Panes that are not CPane-derived are passed as NULL to the virtual, exactly as
// retail does after its DYNAMIC_DOWNCAST.
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CDockingPanesRow__UEAA_AVCSize__HH_Z(
    void* pThis, CSize* pRet, int bStretch, int bHorz) {
    S_CDockingPanesRow* s = Row(pThis);
    if (pRet == nullptr) return pRet;
    const bool bRowHorz = (s != nullptr) && RowIsHorz(s);

    if (s == nullptr || s->m_bVisible == 0) {
        pRet->cx = bRowHorz ? 0x7FFF : 0;
        pRet->cy = 0x7FFF;
        return pRet;
    }

    int cx = 0;
    int cy = 0;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar != nullptr &&
            !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                reinterpret_cast<CObject*>(pBar), impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ())) {
            pBar = nullptr;
        }
        if (!PaneCounts(s, pBar)) continue;

        CSize sizeBar(0, 0);
        // NOTE the argument order: this thunk declares the CSize return slot
        // first and `this` second (core/controlbar/CControlBar.cpp).
        impl__CalcFixedLayout_CControlBar__UEAA_AVCSize__HH_Z(&sizeBar, pBar, bStretch, bHorz);

        if (bRowHorz) {
            cx += sizeBar.cx;
            if (cy <= sizeBar.cy) cy = sizeBar.cy;
        } else {
            if (cx <= sizeBar.cx) cx = sizeBar.cx;
            cy += sizeBar.cy;
        }
    }

    if (bRowHorz) {
        if (cy > 0) cy += s->m_nExtraSpace;
    } else {
        if (cx > 0) cx += s->m_nExtraSpace;
    }
    pRet->cx = cx;
    pRet->cy = cy;
    return pRet;
}

// Symbol: ?CalcLastPaneOffset@CDockingPanesRow@@IEAAHXZ
// Retail 0x522c0: empty row -> 0; otherwise the tail pane's window rect mapped
// into the dock site's client area, and its right edge (horizontal row) or
// bottom edge (vertical row).
extern "C" int MS_ABI impl__CalcLastPaneOffset_CDockingPanesRow__IEAAHXZ(void* pThis) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return 0;
    ObNode* n = TailNode(s);
    if (n == nullptr) return 0;
    CPane* pLast = PaneOf(n);
    if (pLast == nullptr) return 0;

    CRect rect(0, 0, 0, 0);
    ::GetWindowRect(pLast->m_hWnd, RP(&rect));
    if (s->m_pParentDockBar != nullptr)
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(s->m_pParentDockBar, RP(&rect));
    return RowIsHorz(s) ? rect.right : rect.bottom;
}

// Symbol: ?CheckPanes@CDockingPanesRow@@IEAAHAEAVCRect@@PEAVCPane@@_NVCPoint@@HAEAPEAX@Z
// Retail 0x507e0, transcribed.  It is the collision resolver MovePane(CPoint)
// runs after it has computed where pBar wants to land (rect, in SCREEN
// coordinates).  Every exit path is `mov $0x1,%eax`, so it ALWAYS returns TRUE;
// the return value tells the caller nothing.
//
//   if (m_lstControlBars.GetCount() < 2) return TRUE;
//   if (GetVisibleCount() < 2)           return TRUE;      // vslot 0x38
//   posBar = the node holding pBar (NULL if pBar is not in this row)
//   // first other visible pane whose window rect intersects rect:
//   for (node = head; node; node = node->pNext) {
//       p = node->data;
//       if (!p->IsVisible() && !m_bIgnoreBarVisibility) continue;
//       if (p == pBar) continue;
//       ::GetWindowRect(p->m_hWnd, &rectHit);
//       if (::IntersectRect(&rectIsect, &rectHit, &rect)) { hit = p; break; }
//   }
//   if (hit != NULL) {
//       hit->GetVirtualRect(rectVirtual);                   // 0xa1ca0
//       bSwap = ::IntersectRect(&dummy, &rectVirtual, &rect) && nExtra != 0 &&
//               (pBar->IsLeftOf(rectVirtual, TRUE) == bForward);   // 0xa1660
//       if (!bSwap) {
//           MovePane(hit, horz ? rectIsect.Width() : rectIsect.Height(),
//                    bForward, hdwp);                       // 0x505b0
//           ResolveIntersection(hit, bForward, hdwp);       // 0x50dd0
//           // and fall through to MoveTrailingPanes below
//       } else {
//           // park `hit` on the far side of rect, keeping its size ...
//           MovePane(hit, rectNew, hdwp);                   // 0x504e0
//           // ... and relink its node next to pBar's
//           m_lstControlBars.RemoveAt(posHit);
//           bForward ? InsertBefore(posBar, hit) : InsertAfter(posBar, hit);
//           ResolveIntersection(hit, !bForward, hdwp);
//           // then push everything past pBar back by the slack the swap left:
//           for (node = bForward ? posBar->pNext : posBar->pPrev; node; ) {
//               p = node->data; node = bForward ? node->pNext : node->pPrev;
//               if (!p->IsVisible() && !m_bIgnoreBarVisibility) continue;
//               ::GetWindowRect(p->m_hWnd, &rectWnd); p->GetVirtualRect(rectV);
//               if (::EqualRect(&rectWnd, &rectV)) continue;
//               MovePane(p, nSize - labs(horz ? ptOffset.x : ptOffset.y),
//                        !bForward, hdwp);
//           }
//           return TRUE;                                    // no MoveTrailingPanes
//       }
//   }
//   MoveTrailingPanes(posBar, ptOffset, bForward, pBar, hdwp);   // 0x50be0
//   return TRUE;
//
// `nSize` above is the moved pane's extent along the row axis; retail keeps it
// as the difference of the two edge values it had just stored for the new rect
// (0x3c(%rsp)-0x40(%rsp) horizontally, 0x38(%rsp)-0x34(%rsp) vertically), which
// is the same number.  Retail traps (call 0x180225b80 / int3 at 0x50bd3) if
// posBar is NULL on the swap path; the body below leaves the list untouched and
// returns instead.
extern "C" int MS_ABI impl__CheckPanes_CDockingPanesRow__IEAAHAEAVCRect__PEAVCPane___NVCPoint__HAEAPEAX_Z(void* pThis, CRect* pRect, CPane* pBar, bool bForward, long long ptOffset, int nExtra, void** phdwp) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pRect == nullptr) return 1;
    if (ListCount(s) < 2) return 1;
    if (impl__GetVisibleCount_CDockingPanesRow__UEAAHXZ(pThis) < 2) return 1;

    ObNode* pNodeBar = nullptr;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        if (n->data == reinterpret_cast<CObject*>(pBar)) { pNodeBar = n; break; }
    }

    ObNode* pNodeHit = nullptr;
    CPane*  pHit = nullptr;
    CRect   rectHit(0, 0, 0, 0);
    CRect   rectIsect(0, 0, 0, 0);
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* p = PaneOf(n);
        if (p == nullptr || !PaneCounts(s, p)) continue;
        if (p == pBar) continue;
        ::GetWindowRect(p->m_hWnd, RP(&rectHit));
        if (::IntersectRect(RP(&rectIsect), RP(&rectHit), RP(pRect))) {
            pNodeHit = n;
            pHit = p;
            break;
        }
    }

    const bool bHorz = RowIsHorz(s);

    if (pHit != nullptr) {
        CRect rectVirtual(0, 0, 0, 0);
        GetPaneVirtualRect(pHit, rectVirtual);

        CRect rectDummy(0, 0, 0, 0);
        bool bSwap = false;
        if (::IntersectRect(RP(&rectDummy), RP(&rectVirtual), RP(pRect)) && nExtra != 0) {
            CRect rectArg = rectVirtual;
            const bool bLeftOf =
                impl__IsLeftOf_CPane__QEBA_NVCRect___N_Z(pBar, &rectArg, true) != 0;
            bSwap = (bLeftOf == bForward);
        }

        if (!bSwap) {
            impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__H_NAEAPEAX_Z(
                pThis, pHit, bHorz ? rectIsect.Width() : rectIsect.Height(), bForward, phdwp);
            impl__ResolveIntersection_CDockingPanesRow__AEAAXPEAVCPane___NAEAPEAX_Z(
                pThis, pHit, bForward, phdwp);
        } else {
            const int nSize = bHorz ? rectHit.Width() : rectHit.Height();

            CRect rectNew = rectHit;
            if (bHorz) {
                if (bForward) { rectNew.left = pRect->left - nSize;  rectNew.right = pRect->left; }
                else          { rectNew.left = pRect->right;         rectNew.right = pRect->right + nSize; }
            } else {
                if (bForward) { rectNew.top = pRect->top - nSize;    rectNew.bottom = pRect->top; }
                else          { rectNew.top = pRect->bottom;         rectNew.bottom = pRect->bottom + nSize; }
            }
            impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCRect__AEAPEAX_Z(
                pThis, pHit, &rectNew, phdwp);

            // Retail relinks unconditionally and traps (call 0x180225b80 / int3
            // at 0x50bd3) when pBar has no node in this row, so there is no retail
            // behaviour to copy for that case.  BOTH halves of the relink are
            // skipped together: doing only the RemoveAt would take pHit out of the
            // row and never put it back.
            if (pNodeBar != nullptr) {
                CObList::POSITION posHit = PosFromNode(pNodeHit);
                impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(&s->m_lstControlBars, &posHit);
                CObList::POSITION posBar = PosFromNode(pNodeBar);
                if (bForward)
                    impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(
                        &s->m_lstControlBars, &posBar, reinterpret_cast<CObject*>(pHit));
                else
                    impl__InsertAfter_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(
                        &s->m_lstControlBars, &posBar, reinterpret_cast<CObject*>(pHit));
            }

            impl__ResolveIntersection_CDockingPanesRow__AEAAXPEAVCPane___NAEAPEAX_Z(
                pThis, pHit, !bForward, phdwp);

            if (pNodeBar == nullptr) return 1;

            const int nShift = nSize - AbsInt(bHorz ? PtX(ptOffset) : PtY(ptOffset));
            for (ObNode* n = bForward ? pNodeBar->pNext : pNodeBar->pPrev; n != nullptr; ) {
                CPane* p = PaneOf(n);
                ObNode* pNext = bForward ? n->pNext : n->pPrev;
                if (p != nullptr && PaneCounts(s, p)) {
                    CRect rectWnd(0, 0, 0, 0);
                    ::GetWindowRect(p->m_hWnd, RP(&rectWnd));
                    CRect rectV(0, 0, 0, 0);
                    GetPaneVirtualRect(p, rectV);
                    if (!::EqualRect(RP(&rectWnd), RP(&rectV))) {
                        impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__H_NAEAPEAX_Z(
                            pThis, p, nShift, !bForward, phdwp);
                    }
                }
                n = pNext;
            }
            return 1;
        }
    }

    impl__MoveTrailingPanes_CDockingPanesRow__AEAAXPEAU__POSITION__VCPoint___NPEAVCPane__AEAPEAX_Z(
        pThis, pNodeBar, ptOffset, bForward, pBar, phdwp);
    return 1;
}

// Symbol: ?CommitTrans@CDockingPanesRow@@IEAAXXZ
// Retail 0x52480: for every pane that counts as visible, compare the m_rectBar
// staged by BeginTrans against the pane's real window rect; when they differ,
// map m_rectBar into the pane's parent client area and push it there with
// SetWindowPos(NULL, ..., SWP_NOZORDER|SWP_NOACTIVATE, NULL).
extern "C" void MS_ABI impl__CommitTrans_CDockingPanesRow__IEAAXXZ(void* pThis) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr || !PaneCounts(s, pBar)) continue;

        CRect rectBar = pBar->m_rectBar;
        CRect rectWnd(0, 0, 0, 0);
        ::GetWindowRect(pBar->m_hWnd, RP(&rectWnd));
        if (::EqualRect(RP(&rectBar), RP(&rectWnd))) continue;

        ScreenToClientRect(::GetParent(pBar->m_hWnd), &rectBar);
        // retail: pBar->SetWindowPos(...) through CPane vslot 0x480.
        impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
            pBar, nullptr, rectBar.left, rectBar.top, rectBar.Width(), rectBar.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
    }
}

// Symbol: ?ExpandStretchedPanes@CDockingPanesRow@@QEAAXXZ
// Retail 0x4ffd0: empty row -> nothing.  Otherwise take GetAvailableLength(FALSE)
// (vslot 0xC8) and hand it, pane by pane, to CPane::StretchPaneDeferWndPos
// (vslot 0x568) through one NULL HDWP, stopping as soon as nothing is left; then
// ArrangePanes(NULL) (vslot 0x60).
extern "C" void MS_ABI impl__ExpandStretchedPanes_CDockingPanesRow__QEAAXXZ(void* pThis) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || ListCount(s) == 0) return;

    int nAvailable = impl__GetAvailableLength_CDockingPanesRow__UEBAHH_Z(pThis, FALSE);
    void* hdwp = nullptr;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr || !PaneCounts(s, pBar)) continue;
        nAvailable -= impl__StretchPaneDeferWndPos_CPane__UEAAHHAEAPEAX_Z(pBar, nAvailable, &hdwp);
        if (nAvailable <= 0) break;
    }
    impl__ArrangePanes_CDockingPanesRow__UEAAXPEAVCPane___Z(pThis, nullptr);
}

// Symbol: ?ExpandStretchedPanesRect@CDockingPanesRow@@QEAAXXZ
// Retail 0x50080: the m_rectBar twin of ExpandStretchedPanes -- BeginTrans,
// GetAvailableLengthRect, StretchPaneRect per visible pane until nothing is
// left, ArrangePanesRect(NULL), CommitTrans.
extern "C" void MS_ABI impl__ExpandStretchedPanesRect_CDockingPanesRow__QEAAXXZ(void* pThis) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || ListCount(s) == 0) return;

    impl__BeginTrans_CDockingPanesRow__IEAAXXZ(pThis);
    int nAvailable = impl__GetAvailableLengthRect_CDockingPanesRow__IEAAHXZ(pThis);
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr || !PaneCounts(s, pBar)) continue;
        nAvailable -= impl__StretchPaneRect_CDockingPanesRow__IEAAHPEAVCPane__H_Z(pThis, pBar, nAvailable);
        if (nAvailable <= 0) break;
    }
    impl__ArrangePanesRect_CDockingPanesRow__IEAAXPEAVCPane___Z(pThis, nullptr);
    impl__CommitTrans_CDockingPanesRow__IEAAXXZ(pThis);
}

// Symbol: ?FindFirstVisiblePane@CDockingPanesRow@@IEAAPEAVCPane@@H@Z
// Retail 0x51a10: empty row -> NULL.  Walk from the head forwards (bForward) or
// from the tail backwards, and return the first pane that counts as visible.
extern "C" CPane* MS_ABI impl__FindFirstVisiblePane_CDockingPanesRow__IEAAPEAVCPane__H_Z(
    void* pThis, int bForward) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return nullptr;
    ObNode* n = bForward ? HeadNode(s) : TailNode(s);
    while (n != nullptr) {
        CPane* pBar = PaneOf(n);
        n = bForward ? n->pNext : n->pPrev;
        if (s->m_bIgnoreBarVisibility != 0) return pBar;
        if (PaneIsVisible(pBar)) return pBar;
    }
    return nullptr;
}

// Symbol: ?FixupVirtualRects@CDockingPanesRow@@QEAAX_NPEAVCPane@@@Z
// Retail 0x51170: for each pane that counts as visible and is not pBarToExclude,
// read its window rect; when bMoveBackToVirtualRect is false just refresh the
// pane's virtual rect (CPane::UpdateVirtualRect, 0xa1ae0); otherwise, if the
// virtual rect differs from the window rect, move the pane back onto its virtual
// rect through MovePane(pane, CRect, hdwp) inside a
// BeginDeferWindowPos/EndDeferWindowPos pair sized by the pane count.
extern "C" void MS_ABI impl__FixupVirtualRects_CDockingPanesRow__QEAAX_NPEAVCPane___Z(
    void* pThis, bool bMoveBackToVirtualRect, CPane* pBarToExclude) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr || !PaneCounts(s, pBar)) continue;
        if (pBar == pBarToExclude) continue;

        CRect rectWnd(0, 0, 0, 0);
        ::GetWindowRect(pBar->m_hWnd, RP(&rectWnd));

        if (!bMoveBackToVirtualRect) {
            impl__UpdateVirtualRect_CPane__QEAAXXZ(pBar);
            continue;
        }

        CRect rectVirtual(0, 0, 0, 0);
        GetPaneVirtualRect(pBar, rectVirtual);
        if (::EqualRect(RP(&rectVirtual), RP(&rectWnd))) continue;

        // Retail hands MovePane an uninitialised HDWP& here (MovePane(CRect)
        // never reads it); this passes the real handle instead.
        HDWP hdwp = ::BeginDeferWindowPos(ListCount(s));
        void* hdwpArg = hdwp;
        impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCRect__AEAPEAX_Z(pThis, pBar, &rectVirtual, &hdwpArg);
        ::EndDeferWindowPos(static_cast<HDWP>(hdwpArg));
    }
}

// Symbol: ?GetAvailableLength@CDockingPanesRow@@UEBAHH@Z
// Retail 0x51280: the row's client length minus the summed length of every pane
// that counts as visible; each pane contributes its virtual rect when
// bUseVirtualRect is set, otherwise its window rect.
extern "C" int MS_ABI impl__GetAvailableLength_CDockingPanesRow__UEBAHH_Z(
    const void* pThis, int bUseVirtualRect) {
    const S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return 0;

    CRect rectClient(0, 0, 0, 0);
    impl__GetClientRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, &rectClient);

    const bool bHorz = RowIsHorz(s);
    int nUsed = 0;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr || !PaneCounts(s, pBar)) continue;

        CRect rect(0, 0, 0, 0);
        if (bUseVirtualRect) GetPaneVirtualRect(pBar, rect);
        else                 ::GetWindowRect(pBar->m_hWnd, RP(&rect));
        nUsed += bHorz ? rect.Width() : rect.Height();
    }
    return (bHorz ? rectClient.Width() : rectClient.Height()) - nUsed;
}

// Symbol: ?GetAvailableLengthRect@CDockingPanesRow@@IEAAHXZ
// Retail 0x525a0: identical to GetAvailableLength except that each pane
// contributes its staged m_rectBar (+0x118) instead of a live window rect.
extern "C" int MS_ABI impl__GetAvailableLengthRect_CDockingPanesRow__IEAAHXZ(void* pThis) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return 0;

    CRect rectClient(0, 0, 0, 0);
    impl__GetClientRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, &rectClient);

    const bool bHorz = RowIsHorz(s);
    int nUsed = 0;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr || !PaneCounts(s, pBar)) continue;
        nUsed += bHorz ? pBar->m_rectBar.Width() : pBar->m_rectBar.Height();
    }
    return (bHorz ? rectClient.Width() : rectClient.Height()) - nUsed;
}

// Symbol: ?GetAvailableSpace@CDockingPanesRow@@UEAAXAEAVCRect@@@Z
// Retail 0x51450: start from the row's window rect and, for every pane that
// counts as visible, ::InflateRect it by minus that pane's width (horizontal
// row) or minus its height (vertical row).  Note this is InflateRect, i.e. the
// shrink is applied to BOTH edges of the axis -- transcribed as retail has it.
extern "C" void MS_ABI impl__GetAvailableSpace_CDockingPanesRow__UEAAXAEAVCRect___Z(
    void* pThis, CRect* pRect) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pRect == nullptr) return;

    impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, pRect);
    const bool bHorz = RowIsHorz(s);
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr || !PaneCounts(s, pBar)) continue;

        CRect rect(0, 0, 0, 0);
        ::GetWindowRect(pBar->m_hWnd, RP(&rect));
        if (bHorz) ::InflateRect(RP(pRect), -rect.Width(), 0);
        else       ::InflateRect(RP(pRect), 0, -rect.Height());
    }
}

// Symbol: ?GetClientRect@CDockingPanesRow@@QEBAXAEAVCRect@@@Z
// Retail 0x4fd50: the row's window rect mapped into the dock site's client area,
// then re-anchored on the row band: the leading edge steps back by m_nRowOffset
// and the trailing edge is set one m_nRowHeight further on.
extern "C" void MS_ABI impl__GetClientRect_CDockingPanesRow__QEBAXAEAVCRect___Z(
    const void* pThis, CRect* pRect) {
    const S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pRect == nullptr) return;

    impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, pRect);
    if (s->m_pParentDockBar != nullptr)
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(s->m_pParentDockBar, RP(pRect));

    if (RowIsHorz(s)) {
        pRect->top -= s->m_nRowOffset;
        pRect->bottom = pRect->top + s->m_nRowHeight;
    } else {
        pRect->left -= s->m_nRowOffset;
        pRect->right = pRect->left + s->m_nRowHeight;
    }
}

// Symbol: ?GetGroupFromPane@CDockingPanesRow@@QEAAXPEAVCPane@@AEAVCObList@@@Z
// Retail 0x52380: locate pBar's node; walk backwards until a pane with
// m_bFirstInGroup (+0x1AC) is found (or the head is passed); then walk forwards
// from there adding every pane to `lst`, stopping after the one whose
// m_bLastInGroup (+0x1B0) is set.
extern "C" void MS_ABI impl__GetGroupFromPane_CDockingPanesRow__QEAAXPEAVCPane__AEAVCObList___Z(
    void* pThis, CPane* pBar, CObList* pList) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pList == nullptr) return;

    ObNode* pHead = HeadNode(s);
    ObNode* n = pHead;
    while (n != nullptr && n->data != reinterpret_cast<CObject*>(pBar)) n = n->pNext;
    if (n == nullptr) return;

    // Backwards to the start of the group.
    ObNode* pPrev = nullptr;
    while (n != nullptr) {
        CPane* p = PaneOf(n);
        pPrev = n->pPrev;
        if (p != nullptr && p->m_bFirstInGroup != 0) break;
        n = pPrev;
    }
    // Retail resumes at pPrev->pNext, or at the list head when it walked off the
    // front (pPrev == NULL).
    ObNode* pStart = (pPrev != nullptr) ? pPrev->pNext : pHead;
    if (pStart == nullptr) return;

    for (ObNode* p = pStart; p != nullptr; p = p->pNext) {
        CPane* pGroupBar = PaneOf(p);
        impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(pList, reinterpret_cast<CObject*>(pGroupBar));
        if (pGroupBar != nullptr && pGroupBar->m_bLastInGroup != 0) break;
    }
}

// Symbol: ?GetMaxPaneSize@CDockingPanesRow@@QEBAHH@Z
// Retail 0x51360: the largest pane thickness across the row -- height for a
// horizontal row, width for a vertical one.  A hidden pane is skipped only when
// bSkipHiddenBars is set and m_bIgnoreBarVisibility is clear.  A non-zero result
// gets m_nExtraSpace added.
extern "C" int MS_ABI impl__GetMaxPaneSize_CDockingPanesRow__QEBAHH_Z(
    const void* pThis, int bSkipHiddenBars) {
    const S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return 0;

    const bool bHorz = RowIsHorz(s);
    int nMax = 0;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        const BOOL bVisible = PaneIsVisible(pBar);
        if (!bVisible && bSkipHiddenBars && s->m_bIgnoreBarVisibility == 0) continue;

        CRect rect(0, 0, 0, 0);
        if (pBar != nullptr) ::GetWindowRect(pBar->m_hWnd, RP(&rect));
        const int nSize = bHorz ? rect.Height() : rect.Width();
        if (nMax <= nSize) nMax = nSize;
    }
    if (nMax != 0) nMax += s->m_nExtraSpace;
    return nMax;
}

// Symbol: ?GetOutOfBoundsOffset@CDockingPanesRow@@AEAAHPEAVCPane@@H@Z
// Retail 0x51a90: how far pBar sticks out of the row.  A NULL pBar means "the
// head pane" when bLeftTopBound is set and "the tail pane" otherwise.  The
// answer is (row.leading - pane.leading) for the leading bound and
// (pane.trailing - row.trailing) for the trailing one, on the row's axis.
extern "C" int MS_ABI impl__GetOutOfBoundsOffset_CDockingPanesRow__AEAAHPEAVCPane__H_Z(
    void* pThis, CPane* pBar, int bLeftTopBound) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return 0;

    CPane* p = pBar;
    if (p == nullptr) {
        ObNode* n = bLeftTopBound ? HeadNode(s) : TailNode(s);
        if (n == nullptr) return 0;
        p = PaneOf(n);
    }

    CRect rectBar(0, 0, 0, 0);
    if (p != nullptr) ::GetWindowRect(p->m_hWnd, RP(&rectBar));
    CRect rectRow(0, 0, 0, 0);
    impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, &rectRow);

    if (RowIsHorz(s))
        return bLeftTopBound ? (rectRow.left - rectBar.left) : (rectBar.right - rectRow.right);
    return bLeftTopBound ? (rectRow.top - rectBar.top) : (rectBar.bottom - rectRow.bottom);
}

// Symbol: ?GetOutOfBoundsOffsetRect@CDockingPanesRow@@IEAAHPEAVCPane@@H@Z
// Retail 0x52b10: the m_rectBar twin of GetOutOfBoundsOffset.
extern "C" int MS_ABI impl__GetOutOfBoundsOffsetRect_CDockingPanesRow__IEAAHPEAVCPane__H_Z(
    void* pThis, CPane* pBar, int bLeftTopBound) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return 0;

    CPane* p = pBar;
    if (p == nullptr) {
        ObNode* n = bLeftTopBound ? HeadNode(s) : TailNode(s);
        if (n == nullptr) return 0;
        p = PaneOf(n);
    }
    if (p == nullptr) return 0;

    const CRect rectBar = p->m_rectBar;
    CRect rectRow(0, 0, 0, 0);
    impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, &rectRow);

    if (RowIsHorz(s))
        return bLeftTopBound ? (rectRow.left - rectBar.left) : (rectBar.right - rectRow.right);
    return bLeftTopBound ? (rectRow.top - rectBar.top) : (rectBar.bottom - rectRow.bottom);
}

// Symbol: ?GetVisibleCount@CDockingPanesRow@@UEAAHXZ
// Retail 0x50790: number of panes that count as visible.
extern "C" int MS_ABI impl__GetVisibleCount_CDockingPanesRow__UEAAHXZ(void* pThis) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return 0;
    int nCount = 0;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        if (PaneCounts(s, PaneOf(n))) ++nCount;
    }
    return nCount;
}

// Symbol: ?GetWindowRect@CDockingPanesRow@@QEBAXAEAVCRect@@@Z
// Retail 0x4fcf0: ::SetRectEmpty, then -- only if m_pParentDockBar is non-NULL --
// the dock site's window rect narrowed to this row's band (leading edge advanced
// by m_nRowOffset, trailing edge one m_nRowHeight further on).
extern "C" void MS_ABI impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(
    const void* pThis, CRect* pRect) {
    const S_CDockingPanesRow* s = Row(pThis);
    if (pRect == nullptr) return;
    ::SetRectEmpty(RP(pRect));
    if (s == nullptr || s->m_pParentDockBar == nullptr) return;

    ::GetWindowRect(s->m_pParentDockBar->m_hWnd, RP(pRect));
    if (RowIsHorz(s)) {
        pRect->top += s->m_nRowOffset;
        pRect->bottom = pRect->top + s->m_nRowHeight;
    } else {
        pRect->left += s->m_nRowOffset;
        pRect->right = pRect->left + s->m_nRowHeight;
    }
}

// Symbol: ?IsEnoughSpaceToMove@CDockingPanesRow@@AEAAHPEAVCPane@@_NAEAH@Z
// Retail 0x50fc0: sum the lengths of the visible panes that lie on the side
// pControlBar is moving towards (CPane::IsLeftOf, 0xa1660, with bWindowRect
// TRUE; when moving forwards those are the panes that are NOT left of it), then
// nAllowedOffset = (row edge - pane edge) -/+ that sum, and the answer is
// nAllowedOffset > 0 moving forwards, nAllowedOffset < 0 moving backwards.
extern "C" int MS_ABI impl__IsEnoughSpaceToMove_CDockingPanesRow__AEAAHPEAVCPane___NAEAH_Z(
    void* pThis, CPane* pControlBar, bool bForward, int* pnAllowedOffset) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pControlBar == nullptr || pnAllowedOffset == nullptr) return 0;

    const bool bHorz = RowIsHorz(s);
    int nTotal = 0;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr || !PaneCounts(s, pBar)) continue;

        CRect rectPane(0, 0, 0, 0);
        ::GetWindowRect(pBar->m_hWnd, RP(&rectPane));
        if (pBar == pControlBar) continue;

        CRect rectArg = rectPane;
        const bool bLeftOf =
            impl__IsLeftOf_CPane__QEBA_NVCRect___N_Z(pControlBar, &rectArg, true) != 0;
        if (bForward ? bLeftOf : !bLeftOf) continue;
        nTotal += bHorz ? rectPane.Width() : rectPane.Height();
    }

    CRect rectBar(0, 0, 0, 0);
    ::GetWindowRect(pControlBar->m_hWnd, RP(&rectBar));
    CRect rectRow(0, 0, 0, 0);
    impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, &rectRow);

    *pnAllowedOffset = 0;
    int nOffset;
    if (bForward) nOffset = bHorz ? (rectRow.right - rectBar.right) : (rectRow.bottom - rectBar.bottom);
    else          nOffset = bHorz ? (rectRow.left - rectBar.left)   : (rectRow.top - rectBar.top);
    nOffset = bForward ? (nOffset - nTotal) : (nOffset + nTotal);
    *pnAllowedOffset = nOffset;
    return bForward ? (nOffset > 0 ? 1 : 0) : (nOffset < 0 ? 1 : 0);
}

// Symbol: ?IsExclusiveRow@CDockingPanesRow@@UEBAHXZ
// Retail 0x52270: TRUE as soon as one pane answers FALSE to
// DoesAllowSiblingBars (CPane vslot 0x510, whose whole body is
// `return m_bExclusiveRow == 0;` at +0x218), else FALSE.  The virtual is read
// here as the member it returns -- OpenMFC has no thunk for it.
extern "C" int MS_ABI impl__IsExclusiveRow_CDockingPanesRow__UEBAHXZ(const void* pThis) {
    const S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return 0;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr) continue;
        if (pBar->m_bExclusiveRow != 0) return 1;   // !DoesAllowSiblingBars()
    }
    return 0;
}

// Symbol: ?Move@CDockingPanesRow@@UEAAXH@Z
// Retail 0x4fc90: every CPane in the row is shifted by nOffset along the row
// alignment (CPane::MoveByAlignment, 0xa1000); m_nRowOffset then absorbs the
// same nOffset.  Non-CPane entries are skipped.
extern "C" void MS_ABI impl__Move_CDockingPanesRow__UEAAXH_Z(void* pThis, int nOffset) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr) continue;
        if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                reinterpret_cast<CObject*>(pBar), impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ()))
            continue;
        impl__MoveByAlignment_CPane__QEAAHKH_Z(pBar, s->m_dwRowAlignment, nOffset);
    }
    s->m_nRowOffset += nOffset;
}

// Symbol: ?MovePane@CDockingPanesRow@@QEAAXPEAVCPane@@H_NAEAPEAX@Z
// Retail 0x505b0 (the 0x50ad0 quoted by an earlier revision of this file was
// not this function's entry), transcribed in full -- it is 40 instructions and
// does NOT go anywhere near CheckPanes:
//   if (nOffset == 0) return;
//   CRect rect; ::GetWindowRect(pBar->m_hWnd, &rect);
//   int d = bForward ? nOffset : -nOffset;
//   ::OffsetRect(&rect, horz ? d : 0, horz ? 0 : d);
//   m_pDockSite->ScreenToClient(&rect);                       // 0x2a11f0
//   pBar->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(),
//                      SWP_NOZORDER|SWP_NOACTIVATE, NULL);    // vslot 0x480
// The HDWP& parameter is accepted and never read; retail passes a literal NULL
// hdwp to SetWindowPos (`movq $0x0,0x38(%rsp)`).
extern "C" void MS_ABI impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__H_NAEAPEAX_Z(
    void* pThis, CPane* pBar, int nOffset, bool bForward, void** phdwp) {
    (void)phdwp;
    if (nOffset == 0) return;
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pBar == nullptr) return;

    CRect rect(0, 0, 0, 0);
    ::GetWindowRect(pBar->m_hWnd, RP(&rect));

    const int nDelta = bForward ? nOffset : -nOffset;
    if (RowIsHorz(s)) ::OffsetRect(RP(&rect), nDelta, 0);
    else              ::OffsetRect(RP(&rect), 0, nDelta);

    if (s->m_pParentDockBar != nullptr)
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(s->m_pParentDockBar, RP(&rect));
    impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
        pBar, nullptr, rect.left, rect.top, rect.Width(), rect.Height(),
        SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
}

// Symbol: ?MovePane@CDockingPanesRow@@QEAAXPEAVCPane@@HAEAPEAX@Z
// Retail 0x50690: place pBar at nAbsoluteOffset from the row's leading edge,
// keeping its current size, then push it there with SetWindowPos.  The HDWP&
// parameter is accepted and, exactly as in retail, never used: the SetWindowPos
// call passes a NULL hdwp.
extern "C" void MS_ABI impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__HAEAPEAX_Z(
    void* pThis, CPane* pBar, int nAbsoluteOffset, void** phdwp) {
    (void)phdwp;
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pBar == nullptr) return;

    CRect rectBar(0, 0, 0, 0);
    ::GetWindowRect(pBar->m_hWnd, RP(&rectBar));
    CRect rectRow(0, 0, 0, 0);
    impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, &rectRow);

    if (RowIsHorz(s)) {
        const int nWidth = rectBar.Width();
        rectBar.left = rectRow.left + nAbsoluteOffset;
        rectBar.right = rectBar.left + nWidth;
    } else {
        const int nHeight = rectBar.Height();
        rectBar.top = rectRow.top + nAbsoluteOffset;
        rectBar.bottom = rectBar.top + nHeight;
    }

    if (s->m_pParentDockBar != nullptr)
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(s->m_pParentDockBar, RP(&rectBar));
    impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
        pBar, nullptr, rectBar.left, rectBar.top, rectBar.Width(), rectBar.Height(),
        SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
}

// Symbol: ?MovePane@CDockingPanesRow@@QEAAXPEAVCPane@@VCPoint@@HAEAPEAX@Z
// Retail 0x50300, transcribed in full:
//   CRect rectVirtual; pBar->GetVirtualRect(rectVirtual);       // 0xa1ca0
//   CRect rectBar;     ::GetWindowRect(pBar->m_hWnd, &rectBar);
//   // (retail then offsets a COPY of rectVirtual by ptOffset and never reads
//   //  it again -- a dead local, so it is not reproduced here)
//   CPoint ptMove(0, 0);
//   int nDelta = IsHorizontal() ? ptOffset.x : ptOffset.y;
//   if (IsHorizontal()) ptMove.x = ptOffset.x; else ptMove.y = ptOffset.y;
//   BOOL bForward = (nDelta >= 0);            // shr $0x1f,%ebx / xor $0x1,%bl
//   int nAllowed = nDelta;
//   if (!IsEnoughSpaceToMove(pBar, bForward, nAllowed)) return;  // 0x50fc0
//   if (abs(nAllowed) < labs(nDelta))         // clamp on the row axis only
//       (IsHorizontal() ? ptMove.x : ptMove.y) = nAllowed;
//   ::OffsetRect(&rectBar, ptMove.x, ptMove.y);
//   if (CheckPanes(rectBar, pBar, bForward, ptMove, nExtra, hdwp)) {  // 0x507e0
//       m_pParentDockBar->ScreenToClient(rectBar);
//       pBar->SetWindowPos(NULL, rectBar.left, rectBar.top,
//                          rectBar.Width(), rectBar.Height(),
//                          SWP_NOZORDER|SWP_NOACTIVATE, NULL);   // vslot 0x480
//   }
//   ArrangePanes(pBar);                                          // vslot 0x60
// The ArrangePanes call is UNCONDITIONAL -- it runs whether or not CheckPanes
// agreed to the move (and CheckPanes in fact always returns TRUE).
extern "C" void MS_ABI impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCPoint__HAEAPEAX_Z(void* pThis, CPane* pBar, long long ptOffset, int nExtra, void** phdwp) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pBar == nullptr) return;

    CRect rectVirtual(0, 0, 0, 0);
    GetPaneVirtualRect(pBar, rectVirtual);

    CRect rectBar(0, 0, 0, 0);
    ::GetWindowRect(pBar->m_hWnd, RP(&rectBar));

    const bool bHorz = RowIsHorz(s);
    const int nDelta = bHorz ? PtX(ptOffset) : PtY(ptOffset);
    int nMoveX = bHorz ? PtX(ptOffset) : 0;
    int nMoveY = bHorz ? 0 : PtY(ptOffset);

    const bool bForward = (nDelta >= 0);
    int nAllowed = nDelta;
    if (!impl__IsEnoughSpaceToMove_CDockingPanesRow__AEAAHPEAVCPane___NAEAH_Z(
            pThis, pBar, bForward, &nAllowed))
        return;

    if (AbsInt(nAllowed) < AbsInt(nDelta)) {
        if (bHorz) nMoveX = nAllowed;
        else       nMoveY = nAllowed;
    }
    ::OffsetRect(RP(&rectBar), nMoveX, nMoveY);

    if (impl__CheckPanes_CDockingPanesRow__IEAAHAEAVCRect__PEAVCPane___NVCPoint__HAEAPEAX_Z(
            pThis, &rectBar, pBar, bForward, MakePt(nMoveX, nMoveY), nExtra, phdwp)) {
        if (s->m_pParentDockBar != nullptr)
            impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(s->m_pParentDockBar, RP(&rectBar));
        impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
            pBar, nullptr, rectBar.left, rectBar.top, rectBar.Width(), rectBar.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
    }

    impl__ArrangePanes_CDockingPanesRow__UEAAXPEAVCPane___Z(pThis, pBar);
}

// Symbol: ?MovePane@CDockingPanesRow@@QEAAXPEAVCPane@@VCRect@@AEAPEAX@Z
// Retail 0x504e0: take pBar's window rect, replace the row-axis edges with
// rectTarget's, map into the pane's parent client area and SetWindowPos there.
// The HDWP& parameter is never used, exactly as in retail.
extern "C" void MS_ABI impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCRect__AEAPEAX_Z(
    void* pThis, CPane* pBar, const CRect* pRectTarget, void** phdwp) {
    (void)phdwp;
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pBar == nullptr || pRectTarget == nullptr) return;

    CRect rect(0, 0, 0, 0);
    ::GetWindowRect(pBar->m_hWnd, RP(&rect));
    if (RowIsHorz(s)) {
        rect.left = pRectTarget->left;
        rect.right = pRectTarget->right;
    } else {
        rect.top = pRectTarget->top;
        rect.bottom = pRectTarget->bottom;
    }

    if (s->m_pParentDockBar != nullptr)
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(s->m_pParentDockBar, RP(&rect));
    impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
        pBar, nullptr, rect.left, rect.top, rect.Width(), rect.Height(),
        SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
}

// Symbol: ?MovePaneRect@CDockingPanesRow@@IEAAXPEAVCPane@@H_N@Z
// Retail 0x52d60: nOffset == 0 returns immediately.  Otherwise the pane's staged
// m_rectBar is ::OffsetRect'd along the row axis by +nOffset when bForward and
// -nOffset when not, and written back.
extern "C" void MS_ABI impl__MovePaneRect_CDockingPanesRow__IEAAXPEAVCPane__H_N_Z(
    void* pThis, CPane* pBar, int nOffset, bool bForward) {
    if (nOffset == 0) return;
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pBar == nullptr) return;

    CRect rect = pBar->m_rectBar;
    const int nDelta = bForward ? nOffset : -nOffset;
    if (RowIsHorz(s)) ::OffsetRect(RP(&rect), nDelta, 0);
    else              ::OffsetRect(RP(&rect), 0, nDelta);
    pBar->m_rectBar = rect;
}

// Symbol: ?MoveTrailingPanes@CDockingPanesRow@@AEAAXPEAU__POSITION@@VCPoint@@_NPEAVCPane@@AEAPEAX@Z
// Retail 0x50be0 (the 0x50700 an earlier revision of this file quoted is not
// this function's entry), transcribed in full.  Note the walk direction: when
// bForward is set retail steps through node->pPrev (`lea 0x8(%rdx),%rax`), and
// through node->pNext otherwise.
//   for (node = pos; node != NULL; node = bForward ? node->pPrev : node->pNext) {
//       pBar = node->data;
//       if (pBar == pBarToExclude) continue;
//       if (!pBar->IsVisible() && !m_bIgnoreBarVisibility) continue;
//       ::GetWindowRect(pBar->m_hWnd, &rectWnd);
//       pBar->GetVirtualRect(rectVirtual);                       // 0xa1ca0
//       if (::EqualRect(&rectWnd, &rectVirtual)) continue;
//       BOOL bLeftOf = pBar->IsLeftOf(rectVirtual, TRUE);        // 0xa1660
//       int nAxis = IsHorizontal() ? ptOffset.x : ptOffset.y;
//       int nBack = IsHorizontal() ? rectVirtual.left - rectWnd.left
//                                  : rectVirtual.top  - rectWnd.top;
//       int nOffset = (bLeftOf == bForward)
//                   ? labs(nAxis)
//                   : labs(labs(nAxis) < labs(nBack) ? nAxis : nBack);
//       MovePane(pBar, nOffset, bForward, hdwp);                 // 0x505b0
//       ResolveIntersection(pBar, !bForward, hdwp);              // 0x50dd0
//   }
// Retail evaluates IsLeftOf up to three times on the same arguments (the
// compiler duplicated the test across the branch); it is called once here.
// One arm of that duplication (nOffset forced to 0) is unreachable in retail
// and is not reproduced.
extern "C" void MS_ABI impl__MoveTrailingPanes_CDockingPanesRow__AEAAXPEAU__POSITION__VCPoint___NPEAVCPane__AEAPEAX_Z(void* pThis, void* pos, long long ptOffset, bool bForward, CPane* pBarToExclude, void** phdwp) {
    if (pos == nullptr) return;
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;

    const bool bHorz = RowIsHorz(s);
    const int nAxis = bHorz ? PtX(ptOffset) : PtY(ptOffset);

    for (ObNode* n = static_cast<ObNode*>(pos); n != nullptr; ) {
        CPane* pBar = PaneOf(n);
        ObNode* pNext = bForward ? n->pPrev : n->pNext;
        n = pNext;

        if (pBar == pBarToExclude || pBar == nullptr) continue;
        if (!PaneCounts(s, pBar)) continue;

        CRect rectWnd(0, 0, 0, 0);
        ::GetWindowRect(pBar->m_hWnd, RP(&rectWnd));
        CRect rectVirtual(0, 0, 0, 0);
        GetPaneVirtualRect(pBar, rectVirtual);
        if (::EqualRect(RP(&rectWnd), RP(&rectVirtual))) continue;

        CRect rectArg = rectVirtual;
        const bool bLeftOf = impl__IsLeftOf_CPane__QEBA_NVCRect___N_Z(pBar, &rectArg, true) != 0;

        int nOffset;
        if (bLeftOf == bForward) {
            nOffset = AbsInt(nAxis);
        } else {
            const int nBack = bHorz ? (rectVirtual.left - rectWnd.left)
                                    : (rectVirtual.top - rectWnd.top);
            nOffset = AbsInt(AbsInt(nAxis) < AbsInt(nBack) ? nAxis : nBack);
        }

        impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__H_NAEAPEAX_Z(
            pThis, pBar, nOffset, bForward, phdwp);
        impl__ResolveIntersection_CDockingPanesRow__AEAAXPEAVCPane___NAEAPEAX_Z(
            pThis, pBar, !bForward, phdwp);
    }
}

// Symbol: ?OffsetFromRect@CDockingPanesRow@@AEAAXAEBVCRect@@AEAVCPoint@@_N@Z
// Retail 0x51120: pure arithmetic on the row alignment.  For a horizontal row
// (TOP/BOTTOM) it writes pt.x = bForward ? (rect.left - rect.right)
// : (rect.right - rect.left); for a vertical row (LEFT/RIGHT) it writes
// pt.y = bForward ? (rect.bottom - rect.top) : (rect.top - rect.bottom).
// An alignment outside those four leaves pt untouched.
extern "C" void MS_ABI impl__OffsetFromRect_CDockingPanesRow__AEAAXAEBVCRect__AEAVCPoint___N_Z(
    void* pThis, const CRect* pRect, CPoint* pPoint, bool bForward) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pRect == nullptr || pPoint == nullptr) return;

    const DWORD dwAlign = s->m_dwRowAlignment & kAlignAny;
    if (dwAlign == kAlignTop || dwAlign == kAlignBottom) {
        pPoint->x = bForward ? (pRect->left - pRect->right) : (pRect->right - pRect->left);
    } else if (dwAlign == kAlignLeft || dwAlign == kAlignRight) {
        pPoint->y = bForward ? (pRect->bottom - pRect->top) : (pRect->top - pRect->bottom);
    }
}

// Symbol: ?OnInsertPane@CDockingPanesRow@@MEAAXPEAVCPane@@@Z
// Retail 0x4f8b0: take the new pane's window rect, find the first CPane already
// in the row that is NOT left of it (CPane::IsLeftOf with bWindowRect TRUE) and
// insert before that node, otherwise append.  Then UpdateVisibleState(TRUE)
// (vslot 0x98); if the new pane is thicker than m_nRowHeight ask the dock site
// to grow the row by m_nExtraSpace + that thickness; finally link the pane back
// to this row (CBasePane::m_pDockBarRow, +0x130) and re-arrange (vslot 0x60).
extern "C" void MS_ABI impl__OnInsertPane_CDockingPanesRow__MEAAXPEAVCPane___Z(
    void* pThis, CPane* pControlBar) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pControlBar == nullptr) return;

    CRect rectNew(0, 0, 0, 0);
    ::GetWindowRect(pControlBar->m_hWnd, RP(&rectNew));

    int nIndex = 0;
    int nInsertBefore = -1;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext, ++nIndex) {
        CPane* pBar = PaneOf(n);
        if (pBar != nullptr &&
            !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                reinterpret_cast<CObject*>(pBar), impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ())) {
            pBar = nullptr;
        }
        CRect rectArg = rectNew;
        if (impl__IsLeftOf_CPane__QEBA_NVCRect___N_Z(pBar, &rectArg, true) != 0) {
            nInsertBefore = nIndex;
            break;
        }
    }

    if (nInsertBefore < 0) {
        impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(
            &s->m_lstControlBars, reinterpret_cast<CObject*>(pControlBar));
    } else {
        CObList::POSITION pos =
            impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(&s->m_lstControlBars, nInsertBefore);
        impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(
            &s->m_lstControlBars, &pos, reinterpret_cast<CObject*>(pControlBar));
    }

    impl__UpdateVisibleState_CDockingPanesRow__UEAAXH_Z(pThis, TRUE);

    const int nSize = RowIsHorz(s) ? rectNew.Height() : rectNew.Width();
    if (nSize > s->m_nRowHeight && s->m_pParentDockBar != nullptr) {
        impl__ResizeRow_CDockSite__QEAAHPEAVCDockingPanesRow__HH_Z(
            s->m_pParentDockBar, pThis, s->m_nExtraSpace + nSize, TRUE);
    }

    pControlBar->m_pDockBarRow = reinterpret_cast<CDockBarRow*>(pThis);
    impl__ArrangePanes_CDockingPanesRow__UEAAXPEAVCPane___Z(pThis, pControlBar);
}

// Symbol: ?OnResizePane@CDockingPanesRow@@UEAAXPEAVCBasePane@@@Z
// The empty body IS the retail body; this is not an unfinished stub.
// disas.py cannot resolve the mangled name (NOT FOUND in both RVA maps), but the
// function is still reachable through the class vftable the constructor installs
// (mfc140 0x2e4148).  Matching that table against the virtual declaration order
// in afxdockingpanesrow.h accounts for every one of slots 5..27 exactly:
//   5  Create                 -> 0x3ae0  `mov $1,%eax; ret`   (return TRUE)
//   6..13  GetAvailableSpace, GetVisibleCount, CalcFixedLayout, AddPane,
//          AddPaneFromRow, RemovePane, ArrangePanes(CPane*), ArrangePanes(int,int)
//   14 ResizeByPaneDivider    -> 0x7260  `xor %eax,%eax; ret` (return 0)
//   15..17 Resize, Move, RepositionPanes
//   18 IsEmpty                -> 0x4f3d0 compares m_nCount (+0x40) with 0
//   19..22 UpdateVisibleState, ShowDockSiteRow, ReplacePane, ShowPane
//   23 OnResizePane           -> 0x2820  `ret`
//   24 IsVisible              -> 0x97e0  `mov 0xc(%rcx),%eax` -- m_bVisible at
//          +0x0C, which independently confirms the shadow layout above
//   25..27 GetAvailableLength, IsExclusiveRow, OnInsertPane
// Slot 23 (+0xb8) therefore holds a bare `ret`: identical-code folding shares that
// one address with every empty void virtual in the image, which is why the RVA map
// labels it ?UpdateModifiedFlag@CRichEditDoc@@UEAAXXZ and why the export itself has
// no RVA of its own.  Retail's CDockingPanesRow::OnResizePane does nothing, so
// neither does this.  The auto-generated placeholder parameter list was also
// corrected to the shape the mangled name pins.
extern "C" void MS_ABI impl__OnResizePane_CDockingPanesRow__UEAAXPEAVCBasePane___Z(void* pThis, CBasePane* pPane) { (void)pThis; (void)pPane; }

// Symbol: ?RedrawAll@CDockingPanesRow@@QEAAXXZ
// Retail 0x52340: ::RedrawWindow(pane, NULL, NULL, RDW_INVALIDATE|RDW_FRAME)
// for every pane in the row -- no visibility filter.
extern "C" void MS_ABI impl__RedrawAll_CDockingPanesRow__QEAAXXZ(void* pThis) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr || pBar->m_hWnd == nullptr) continue;
        ::RedrawWindow(pBar->m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_FRAME);
    }
}

// Symbol: ?RemovePane@CDockingPanesRow@@UEAAXPEAVCPane@@@Z
// Retail 0x4f9f0: find pControlBar's node (returning silently if it is not in
// this row), let the pane give its stretch back (CPane::StretchPaneDeferWndPos
// with 0xFFFF through a NULL HDWP), unlink the node, clear the pane's
// m_pDockBarRow, and then either re-settle the row (FixupVirtualRects(TRUE, pane),
// ExpandStretchedPanes, UpdateVisibleState(FALSE), and shrink the row to the new
// GetMaxPaneSize(FALSE) when that is smaller) or, if the row is now empty, ask
// the dock site to remove the row.
extern "C" void MS_ABI impl__RemovePane_CDockingPanesRow__UEAAXPEAVCPane___Z(
    void* pThis, CPane* pControlBar) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pControlBar == nullptr) return;

    int nIndex = 0;
    ObNode* n = HeadNode(s);
    while (n != nullptr && n->data != reinterpret_cast<CObject*>(pControlBar)) {
        n = n->pNext;
        ++nIndex;
    }
    if (n == nullptr) return;

    void* hdwp = nullptr;
    impl__StretchPaneDeferWndPos_CPane__UEAAHHAEAPEAX_Z(pControlBar, 0xFFFF, &hdwp);

    CObList::POSITION pos = impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(&s->m_lstControlBars, nIndex);
    impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(&s->m_lstControlBars, &pos);
    pControlBar->m_pDockBarRow = nullptr;

    if (ListCount(s) == 0) {
        if (s->m_pParentDockBar != nullptr)
            impl__RemoveRow_CDockSite__QEAAXPEAVCDockingPanesRow___Z(s->m_pParentDockBar, pThis);
        return;
    }

    impl__FixupVirtualRects_CDockingPanesRow__QEAAX_NPEAVCPane___Z(pThis, true, pControlBar);
    impl__ExpandStretchedPanes_CDockingPanesRow__QEAAXXZ(pThis);
    impl__UpdateVisibleState_CDockingPanesRow__UEAAXH_Z(pThis, FALSE);

    const int nMaxSize = impl__GetMaxPaneSize_CDockingPanesRow__QEBAHH_Z(pThis, FALSE);
    if (nMaxSize < s->m_nRowHeight) {
        if (s->m_pParentDockBar != nullptr) {
            impl__ResizeRow_CDockSite__QEAAHPEAVCDockingPanesRow__HH_Z(
                s->m_pParentDockBar, pThis, nMaxSize, TRUE);
        }
        s->m_nRowHeight = nMaxSize;
    }
}

// Symbol: ?ReplacePane@CDockingPanesRow@@UEAAHPEAVCPane@@0@Z
// Retail 0x52210: find pBarOld's node; if it is not in this row return FALSE.
// Otherwise insert pBarNew after that node and remove the old node, and return
// TRUE.  (Retail really does InsertAfter + RemoveAt rather than SetAt.)
extern "C" int MS_ABI impl__ReplacePane_CDockingPanesRow__UEAAHPEAVCPane__0_Z(
    void* pThis, CPane* pBarOld, CPane* pBarNew) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return FALSE;

    int nIndex = 0;
    ObNode* n = HeadNode(s);
    while (n != nullptr && n->data != reinterpret_cast<CObject*>(pBarOld)) {
        n = n->pNext;
        ++nIndex;
    }
    if (n == nullptr) return FALSE;

    CObList::POSITION pos = impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(&s->m_lstControlBars, nIndex);
    impl__InsertAfter_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(
        &s->m_lstControlBars, &pos, reinterpret_cast<CObject*>(pBarNew));
    pos = impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(&s->m_lstControlBars, nIndex);
    impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(&s->m_lstControlBars, &pos);
    return TRUE;
}

// Symbol: ?RepositionPanes@CDockingPanesRow@@UEAAXAEAVCRect@@IHH@Z
// Retail 0x51b50, transcribed.  rectNew arrives in the dock site's CLIENT
// coordinates and is mapped to screen immediately; everything after that works
// in screen space.
//   if (m_lstControlBars.GetCount() == 0) return;
//   if (GetVisibleCount() == 0) return;                    // vslot 0x38
//   CRect rectNew = rectNeighbourhood;
//   m_pParentDockBar->ClientToScreen(rectNew);             // 0x2a1250
//   CRect rectRow; GetWindowRect(rectRow);                 // 0x4fcf0
//   if (::IsRectEmpty(&rectRow)) return;
//   int nDelta = IsHorizontal() ? rectNew.Width()  - rectRow.Width()
//                               : rectNew.Height() - rectRow.Height();
//   HDWP hdwp = NULL;
//   if (IsExclusiveRow()) {                                // vslot 0xd0
//       CPane* p = GetHead();
//       p->SetWindowPos(NULL, rectRow.left, rectRow.top,
//                       horz ? rectNew.Width()  : rectRow.Width(),
//                       horz ? rectRow.Height() : rectNew.Height(),
//                       0x16 /*NOMOVE|NOZORDER|NOACTIVATE*/, NULL);
//       ::RedrawWindow(p->m_hWnd, NULL, NULL, 0x105);
//       return;
//   }
//   int nAvail = GetAvailableLength(TRUE);                 // vslot 0xc8
//   // three-way dispatch (0x51cdf..0x51d00):
//   //   nAvail >= 0 && bAdjust      -> straight to the "settle" block
//   //   nAvail >= 0 && !bAdjust     -> nAvail < abs(nDelta) ? BLOCK_D : settle
//   //   nAvail <  0 && bAdjust      -> BLOCK_E, then settle
//   //   nAvail <  0 && !bAdjust     -> BLOCK_D
//   BLOCK_D (0x51d06) -- redistribute and RETURN:
//     BOOL bFwd = (nSide == 1 || nSide == 3);   // test $0xfffffffd on nSide-1
//     if (nAvail >= 0) {
//         int nSign = (nDelta < 0) ? -1 : 1;
//         int nOut  = abs(GetOutOfBoundsOffset(NULL, bFwd));
//         ShiftPanes(NULL, (nAvail - nOut) * nSign, bFwd);
//         nDelta = (abs(nDelta) - nAvail) * nSign;
//     }
//     for (node = tail; node; node = node->pPrev) {
//         p = node->data;  if (!counts) continue;
//         int st = p->StretchPaneDeferWndPos(nDelta, hdwp);   // vslot 0x568
//         MovePane(p, abs(nDelta) - abs(st), bFwd, hdwp);     // 0x505b0
//         if (st == nDelta) return;
//         nDelta -= st;
//     }
//     return;
//   BLOCK_E (0x51e00) -- absorb the shortfall from the head, then fall through:
//     int nRest = nDelta;
//     for (node = head; node; node = node->pNext) {
//         p = node->data;  if (!counts) continue;
//         int st = p->StretchPaneDeferWndPos(nRest, hdwp);
//         if (st != 0) for (q = node->pNext; q; q = q->pNext)
//             if (counts(q->data)) MovePane(q->data, st, TRUE, hdwp);
//         nRest -= st;  if (nRest <= 0) break;
//     }
//   SETTLE (0x51eb5):
//     if (horz) { rectRow.left = rectNew.left; rectRow.right = rectNew.right; }
//     else      { rectRow.top  = rectNew.top;  rectRow.bottom = rectNew.bottom; }
//     CPane* a = FindFirstVisiblePane(TRUE);  AdjustPaneToRowArea(a, rectRow, hdwp);
//     CPane* b = FindFirstVisiblePane(FALSE); if (b != a) AdjustPaneToRowArea(b, ...);
//     if (nSide == (UINT)-1) return;
//     if (!bAdjust) return;
//     if (GetAvailableLength(TRUE) + nDelta <= 0) return;
//     for (node = head; node; node = node->pNext) {
//         p = node->data;  if (!counts) continue;
//         ::GetWindowRect(p->m_hWnd, &rectWnd); p->GetVirtualRect(rectV);
//         if (::EqualRect(&rectWnd, &rectV)) continue;
//         BOOL bFwd2 = horz ? !(rectWnd.left > rectV.left)
//                           : !(rectWnd.top  > rectV.top);
//         int n = 0;
//         if (horz  && (nSide == 1 || nSide == 2)) n = <min-by-abs of nSizeDelta
//                                                       and rectV.left - rectWnd.left>;
//         if (!horz && (nSide == 3 || nSide == 6)) n = <same, on top>;
//         int nAllowed = n;
//         if (IsEnoughSpaceToMove(p, bFwd2, nAllowed))          // 0x50fc0
//             MovePane(p, n, bFwd2, hdwp);                      // 0x505b0
//     }
// The nSide constants are transcribed as the literals retail compares against
// (1/2/3/6 and (UINT)-1); this tree does not model the enum they belong to, and
// the parameter NAMES below are the ones the forward declaration already in
// this file uses -- what is verified is the role each one plays above, not the
// name.
// The HDWP local is never handed to ::DeferWindowPos, exactly as in retail.
extern "C" void MS_ABI impl__RepositionPanes_CDockingPanesRow__UEAAXAEAVCRect__IHH_Z(void* pThis, CRect* pRectNew, unsigned int nSide, int bAdjust, int nSizeDelta) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pRectNew == nullptr) return;
    if (HeadNode(s) == nullptr) return;
    if (impl__GetVisibleCount_CDockingPanesRow__UEAAHXZ(pThis) == 0) return;

    CRect rectNew = *pRectNew;
    if (s->m_pParentDockBar != nullptr)
        ClientToScreenRect(s->m_pParentDockBar->m_hWnd, &rectNew);

    CRect rectRow(0, 0, 0, 0);
    impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, &rectRow);
    if (::IsRectEmpty(RP(&rectRow))) return;

    const bool bHorz = RowIsHorz(s);
    int nDelta = bHorz ? (rectNew.Width() - rectRow.Width())
                       : (rectNew.Height() - rectRow.Height());

    void* hdwp = nullptr;

    if (impl__IsExclusiveRow_CDockingPanesRow__UEBAHXZ(pThis)) {
        CPane* pBar = PaneOf(HeadNode(s));
        if (pBar == nullptr) return;
        impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
            pBar, nullptr, rectRow.left, rectRow.top,
            bHorz ? rectNew.Width() : rectRow.Width(),
            bHorz ? rectRow.Height() : rectNew.Height(),
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
        ::RedrawWindow(pBar->m_hWnd, nullptr, nullptr,
                       RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
        return;
    }

    const int nAvailable = impl__GetAvailableLength_CDockingPanesRow__UEBAHH_Z(pThis, TRUE);

    if (bAdjust == 0 && (nAvailable < 0 || nAvailable < AbsInt(nDelta))) {
        // BLOCK_D -- retail 0x51d06; this arm returns without settling.
        const int bFwd = (nSide == 1u || nSide == 3u) ? 1 : 0;
        if (nAvailable >= 0) {
            const int nSign = (nDelta < 0) ? -1 : 1;
            const int nOut = AbsInt(
                impl__GetOutOfBoundsOffset_CDockingPanesRow__AEAAHPEAVCPane__H_Z(pThis, nullptr, bFwd));
            impl__ShiftPanes_CDockingPanesRow__AEAAXPEAVCPane__HH_Z(
                pThis, nullptr, (nAvailable - nOut) * nSign, bFwd);
            nDelta = (AbsInt(nDelta) - nAvailable) * nSign;
        }
        for (ObNode* n = TailNode(s); n != nullptr; ) {
            CPane* pBar = PaneOf(n);
            ObNode* pPrev = n->pPrev;
            if (pBar != nullptr && PaneCounts(s, pBar)) {
                const int nStretched =
                    impl__StretchPaneDeferWndPos_CPane__UEAAHHAEAPEAX_Z(pBar, nDelta, &hdwp);
                impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__H_NAEAPEAX_Z(
                    pThis, pBar, AbsInt(nDelta) - AbsInt(nStretched), bFwd != 0, &hdwp);
                if (nStretched == nDelta) return;
                nDelta -= nStretched;
            }
            n = pPrev;
        }
        return;
    }

    if (nAvailable < 0 && bAdjust != 0) {
        // BLOCK_E -- retail 0x51e00; falls through into the settle block.
        int nRest = nDelta;
        for (ObNode* n = HeadNode(s); n != nullptr; ) {
            CPane* pBar = PaneOf(n);
            ObNode* pNext = n->pNext;
            if (pBar != nullptr && PaneCounts(s, pBar)) {
                const int nStretched =
                    impl__StretchPaneDeferWndPos_CPane__UEAAHHAEAPEAX_Z(pBar, nRest, &hdwp);
                if (nStretched != 0) {
                    for (ObNode* q = pNext; q != nullptr; ) {
                        CPane* p2 = PaneOf(q);
                        ObNode* qNext = q->pNext;
                        if (p2 != nullptr && PaneCounts(s, p2)) {
                            impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__H_NAEAPEAX_Z(
                                pThis, p2, nStretched, true, &hdwp);
                        }
                        q = qNext;
                    }
                }
                nRest -= nStretched;
                if (nRest <= 0) break;
            }
            n = pNext;
        }
    }

    // SETTLE -- retail 0x51eb5.
    if (bHorz) {
        rectRow.left = rectNew.left;
        rectRow.right = rectNew.right;
    } else {
        rectRow.top = rectNew.top;
        rectRow.bottom = rectNew.bottom;
    }

    CPane* pFirst = impl__FindFirstVisiblePane_CDockingPanesRow__IEAAPEAVCPane__H_Z(pThis, TRUE);
    impl__AdjustPaneToRowArea_CDockingPanesRow__IEAAXPEAVCPane__AEBVCRect__AEAPEAX_Z(
        pThis, pFirst, &rectRow, &hdwp);
    CPane* pLast = impl__FindFirstVisiblePane_CDockingPanesRow__IEAAPEAVCPane__H_Z(pThis, FALSE);
    if (pLast != pFirst) {
        impl__AdjustPaneToRowArea_CDockingPanesRow__IEAAXPEAVCPane__AEBVCRect__AEAPEAX_Z(
            pThis, pLast, &rectRow, &hdwp);
    }

    if (nSide == 0xFFFFFFFFu) return;
    if (bAdjust == 0) return;
    if (impl__GetAvailableLength_CDockingPanesRow__UEBAHH_Z(pThis, TRUE) + nDelta <= 0) return;

    for (ObNode* n = HeadNode(s); n != nullptr; ) {
        CPane* pBar = PaneOf(n);
        ObNode* pNext = n->pNext;
        n = pNext;
        if (pBar == nullptr || !PaneCounts(s, pBar)) continue;

        CRect rectWnd(0, 0, 0, 0);
        ::GetWindowRect(pBar->m_hWnd, RP(&rectWnd));
        CRect rectV(0, 0, 0, 0);
        GetPaneVirtualRect(pBar, rectV);
        if (::EqualRect(RP(&rectWnd), RP(&rectV))) continue;

        const bool bFwd2 = bHorz ? !(rectWnd.left > rectV.left)
                                 : !(rectWnd.top > rectV.top);

        int nOffset = 0;
        if (bHorz) {
            if (nSide == 1u || nSide == 2u) {
                const int d = rectV.left - rectWnd.left;
                nOffset = AbsInt(AbsInt(nSizeDelta) < AbsInt(d) ? nSizeDelta : d);
            }
        } else {
            if (nSide == 3u || nSide == 6u) {
                const int d = rectV.top - rectWnd.top;
                nOffset = AbsInt(AbsInt(nSizeDelta) < AbsInt(d) ? nSizeDelta : d);
            }
        }

        int nAllowed = nOffset;
        if (impl__IsEnoughSpaceToMove_CDockingPanesRow__AEAAHPEAVCPane___NAEAH_Z(
                pThis, pBar, bFwd2, &nAllowed)) {
            impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__H_NAEAPEAX_Z(
                pThis, pBar, nOffset, bFwd2, &hdwp);
        }
    }
}

// Symbol: ?Resize@CDockingPanesRow@@UEAAHH@Z
// Retail 0x4fc20: m_nRowHeight += nOffset, every CPane in the row gets
// RecalcLayout() (vslot 0x430), and nOffset is returned unchanged.
extern "C" int MS_ABI impl__Resize_CDockingPanesRow__UEAAHH_Z(void* pThis, int nOffset) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return nOffset;
    s->m_nRowHeight += nOffset;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr) continue;
        if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                reinterpret_cast<CObject*>(pBar), impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ()))
            continue;
        impl__RecalcLayout_CPane__UEAAXXZ(pBar);
    }
    return nOffset;
}

// Symbol: ?ResolveIntersection@CDockingPanesRow@@AEAAXPEAVCPane@@_NAEAPEAX@Z
// Retail 0x50dd0 -- the live-window twin of ResolveIntersectionRect above, with
// the same pairwise sweep.  The only differences in the retail body are that
// each pane's rect comes from ::GetWindowRect(pane->m_hWnd, ...) instead of
// m_rectBar, and that the push is issued through
// MovePane(pane, nOffset, bForward, hdwp) (0x505b0) instead of MovePaneRect.
// As in the Rect twin, retail computes the row's own window rect here and never
// reads it, so it is not computed.
extern "C" void MS_ABI impl__ResolveIntersection_CDockingPanesRow__AEAAXPEAVCPane___NAEAPEAX_Z(
    void* pThis, CPane* pBar, bool bForward, void** phdwp) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;

    ObNode* n = HeadNode(s);
    while (n != nullptr && n->data != reinterpret_cast<CObject*>(pBar)) n = n->pNext;

    const bool bHorz = RowIsHorz(s);
    CRect rectPrev(0, 0, 0, 0);
    ::SetRectEmpty(RP(&rectPrev));

    while (n != nullptr) {
        CPane* pA = PaneOf(n);
        n = bForward ? n->pNext : n->pPrev;
        if (!PaneCounts(s, pA)) continue;

        ::GetWindowRect(pA->m_hWnd, RP(&rectPrev));
        if (n == nullptr) return;

        CPane* pB = nullptr;
        ObNode* pNodeB = nullptr;
        for (;;) {
            pNodeB = n;
            n = bForward ? n->pNext : n->pPrev;
            pB = PaneOf(pNodeB);
            if (PaneCounts(s, pB)) break;
            if (n == nullptr) break;
        }
        if (pB == nullptr) return;

        CRect rectB(0, 0, 0, 0);
        ::GetWindowRect(pB->m_hWnd, RP(&rectB));

        int nOffset;
        if (bForward) {
            if (bHorz) {
                if (rectB.left > rectPrev.right) { n = pNodeB; continue; }
                nOffset = rectPrev.right - rectB.left;
            } else {
                if (rectB.top > rectPrev.bottom) { n = pNodeB; continue; }
                nOffset = rectPrev.bottom - rectB.top;
            }
        } else {
            if (bHorz) {
                if (rectB.right < rectPrev.left) { n = pNodeB; continue; }
                nOffset = rectB.right - rectPrev.left;
            } else {
                if (rectB.bottom < rectPrev.top) { n = pNodeB; continue; }
                nOffset = rectB.bottom - rectPrev.top;
            }
        }
        impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__H_NAEAPEAX_Z(pThis, pB, nOffset, bForward, phdwp);
        n = pNodeB;
    }
}

// Symbol: ?ResolveIntersectionRect@CDockingPanesRow@@IEAAXPEAVCPane@@_N@Z
// Retail 0x52920, transcribed.  It is a pairwise sweep over the pane list in
// the walk direction, pushing each pane clear of the one before it:
//   node = the node holding pBar (NULL if pBar is not in this row)
//   CRect rectPrev; ::SetRectEmpty(&rectPrev);
//   CRect rectRow;  GetWindowRect(rectRow);       // computed and never read
//   while (node != NULL) {
//       A = node->data; node = bForward ? node->pNext : node->pPrev;
//       if (!A->IsVisible() && !m_bIgnoreBarVisibility) continue;
//       rectPrev = A->m_rectBar;
//       // scan on for the next pane that counts as visible; if the list runs
//       // out first, retail still uses the LAST pane it looked at.
//       B = ...; if (B == NULL) return;
//       if (bForward) {
//           if (horz ? B.left > rectPrev.right : B.top > rectPrev.bottom) continue;
//           n = horz ? rectPrev.right - B.left : rectPrev.bottom - B.top;
//       } else {
//           if (horz ? B.right < rectPrev.left : B.bottom < rectPrev.top) continue;
//           n = horz ? B.right - rectPrev.left : B.bottom - rectPrev.top;
//       }
//       MovePaneRect(B, n, bForward);            // 0x52d60
//   }
// The `continue` re-enters at B's node, so the sweep advances one pane per pass.
// rectRow really is dead in the retail body; it is not computed here.
extern "C" void MS_ABI impl__ResolveIntersectionRect_CDockingPanesRow__IEAAXPEAVCPane___N_Z(
    void* pThis, CPane* pBar, bool bForward) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;

    ObNode* n = HeadNode(s);
    while (n != nullptr && n->data != reinterpret_cast<CObject*>(pBar)) n = n->pNext;

    const bool bHorz = RowIsHorz(s);
    CRect rectPrev(0, 0, 0, 0);
    ::SetRectEmpty(RP(&rectPrev));

    while (n != nullptr) {
        CPane* pA = PaneOf(n);
        n = bForward ? n->pNext : n->pPrev;
        if (!PaneCounts(s, pA)) continue;

        rectPrev = pA->m_rectBar;
        if (n == nullptr) return;

        // Find the next pane that counts as visible.  When the list runs out
        // first retail keeps the last candidate it examined, visible or not.
        CPane* pB = nullptr;
        ObNode* pNodeB = nullptr;
        for (;;) {
            pNodeB = n;
            n = bForward ? n->pNext : n->pPrev;
            pB = PaneOf(pNodeB);
            if (PaneCounts(s, pB)) break;
            if (n == nullptr) break;
        }
        if (pB == nullptr) return;

        int nOffset;
        if (bForward) {
            if (bHorz) {
                if (pB->m_rectBar.left > rectPrev.right) { n = pNodeB; continue; }
                nOffset = rectPrev.right - pB->m_rectBar.left;
            } else {
                if (pB->m_rectBar.top > rectPrev.bottom) { n = pNodeB; continue; }
                nOffset = rectPrev.bottom - pB->m_rectBar.top;
            }
        } else {
            if (bHorz) {
                if (pB->m_rectBar.right < rectPrev.left) { n = pNodeB; continue; }
                nOffset = pB->m_rectBar.right - rectPrev.left;
            } else {
                if (pB->m_rectBar.bottom < rectPrev.top) { n = pNodeB; continue; }
                nOffset = pB->m_rectBar.bottom - rectPrev.top;
            }
        }
        impl__MovePaneRect_CDockingPanesRow__IEAAXPEAVCPane__H_N_Z(pThis, pB, nOffset, bForward);
        n = pNodeB;
    }
}

// Symbol: ?ScreenToClient@CDockingPanesRow@@QEBAXAEAVCRect@@@Z
// Retail 0x4fda0 is a two-instruction tail jump into
// m_pParentDockBar->ScreenToClient(rect) with no NULL check; the guard below is
// the one deviation.
extern "C" void MS_ABI impl__ScreenToClient_CDockingPanesRow__QEBAXAEAVCRect___Z(
    const void* pThis, CRect* pRect) {
    const S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pRect == nullptr || s->m_pParentDockBar == nullptr) return;
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(s->m_pParentDockBar, RP(pRect));
}

// Symbol: ?ShiftPanes@CDockingPanesRow@@AEAAXPEAVCPane@@HH@Z
// Retail 0x50110, transcribed:
//   if (nOffset == 0) return;
//   node = (pBar != NULL) ? the node holding pBar
//                         : (bForward ? m_lstControlBars head : tail);
//   if (pBar == NULL) pBar = node->data;      // and retail traps on an empty row
//   CRect rectPrev; ::SetRectEmpty(&rectPrev);
//   for (; node != NULL; node = bForward ? node->pNext : node->pPrev) {
//       pane = node->data;
//       if (!pane->IsVisible() && !m_bIgnoreBarVisibility) continue;
//       CRect rectPane; ::GetWindowRect(pane->m_hWnd, &rectPane);
//       if (pane != pBar && !::IsRectEmpty(&rectPrev))
//           nOffset -= horz ? (bForward ? rectPane.left - rectPrev.right
//                                       : rectPane.right - rectPrev.left)
//                           : (bForward ? rectPane.top - rectPrev.bottom
//                                       : rectPane.bottom - rectPrev.top);
//       if (!((nOffset > 0 && bForward) || (nOffset < 0 && !bForward))) return;
//       rectPrev = rectPane;                       // the PRE-shift screen rect
//       ::OffsetRect(&rectPane, horz ? nOffset : 0, horz ? 0 : nOffset);
//       m_pDockSite->ScreenToClient(&rectPane);
//       pane->SetWindowPos(NULL, rectPane.left, rectPane.top,
//                          rectPane.Width(), rectPane.Height(),
//                          SWP_NOZORDER|SWP_NOACTIVATE, NULL);   // vslot 0x480
//   }
// Note the loop only continues down the shift path: every early-out above is a
// return, not a `continue`, except the visibility skip.
extern "C" void MS_ABI impl__ShiftPanes_CDockingPanesRow__AEAAXPEAVCPane__HH_Z(
    void* pThis, CPane* pBar, int nOffset, int bForward) {
    if (nOffset == 0) return;
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;

    ObNode* n;
    if (pBar != nullptr) {
        n = HeadNode(s);
        while (n != nullptr && n->data != reinterpret_cast<CObject*>(pBar)) n = n->pNext;
    } else {
        n = bForward ? HeadNode(s) : TailNode(s);
        if (n == nullptr) return;     // retail traps here instead
        pBar = PaneOf(n);
    }

    const bool bHorz = RowIsHorz(s);
    CRect rectPrev(0, 0, 0, 0);
    ::SetRectEmpty(RP(&rectPrev));

    while (n != nullptr) {
        CPane* pPane = PaneOf(n);
        n = bForward ? n->pNext : n->pPrev;
        if (!PaneCounts(s, pPane)) continue;

        CRect rectPane(0, 0, 0, 0);
        if (pPane != nullptr) ::GetWindowRect(pPane->m_hWnd, RP(&rectPane));

        if (pPane != pBar && !::IsRectEmpty(RP(&rectPrev))) {
            int nGap;
            if (bHorz) nGap = bForward ? (rectPane.left - rectPrev.right)
                                       : (rectPane.right - rectPrev.left);
            else       nGap = bForward ? (rectPane.top - rectPrev.bottom)
                                       : (rectPane.bottom - rectPrev.top);
            nOffset -= nGap;
        }

        const bool bApply = (nOffset > 0 && bForward != 0) || (nOffset < 0 && bForward == 0);
        if (!bApply) return;

        rectPrev = rectPane;
        if (bHorz) ::OffsetRect(RP(&rectPane), nOffset, 0);
        else       ::OffsetRect(RP(&rectPane), 0, nOffset);

        if (s->m_pParentDockBar != nullptr)
            impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(s->m_pParentDockBar, RP(&rectPane));
        impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
            pPane, nullptr, rectPane.left, rectPane.top, rectPane.Width(), rectPane.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
    }
}

// Symbol: ?ShiftPanesRect@CDockingPanesRow@@IEAAXPEAVCPane@@HH@Z
// Retail 0x52bd0 -- instruction-for-instruction the m_rectBar twin of
// ShiftPanes above: identical node walk, identical gap arithmetic and identical
// go/no-go test, except that each pane contributes its staged m_rectBar (+0x118)
// instead of a live ::GetWindowRect, and the shifted rect is written straight
// back to m_rectBar with no ScreenToClient and no SetWindowPos.
extern "C" void MS_ABI impl__ShiftPanesRect_CDockingPanesRow__IEAAXPEAVCPane__HH_Z(
    void* pThis, CPane* pBar, int nOffset, int bForward) {
    if (nOffset == 0) return;
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;

    ObNode* n;
    if (pBar != nullptr) {
        n = HeadNode(s);
        while (n != nullptr && n->data != reinterpret_cast<CObject*>(pBar)) n = n->pNext;
    } else {
        n = bForward ? HeadNode(s) : TailNode(s);
        if (n == nullptr) return;     // retail traps here instead
        pBar = PaneOf(n);
    }

    const bool bHorz = RowIsHorz(s);
    CRect rectPrev(0, 0, 0, 0);
    ::SetRectEmpty(RP(&rectPrev));

    while (n != nullptr) {
        CPane* pPane = PaneOf(n);
        n = bForward ? n->pNext : n->pPrev;
        if (!PaneCounts(s, pPane)) continue;

        CRect rectPane = pPane->m_rectBar;

        if (pPane != pBar && !::IsRectEmpty(RP(&rectPrev))) {
            int nGap;
            if (bHorz) nGap = bForward ? (rectPane.left - rectPrev.right)
                                       : (rectPane.right - rectPrev.left);
            else       nGap = bForward ? (rectPane.top - rectPrev.bottom)
                                       : (rectPane.bottom - rectPrev.top);
            nOffset -= nGap;
        }

        const bool bApply = (nOffset > 0 && bForward != 0) || (nOffset < 0 && bForward == 0);
        if (!bApply) return;

        rectPrev = rectPane;
        if (bHorz) ::OffsetRect(RP(&rectPane), nOffset, 0);
        else       ::OffsetRect(RP(&rectPane), 0, nOffset);
        pPane->m_rectBar = rectPane;
    }
}

// Symbol: ?ShowDockSiteRow@CDockingPanesRow@@UEAAXHH@Z
// Retail 0x4ffb0, eight instructions transcribed in full:
//   m_bVisible = bShow;                              // +0x0C
//   m_pDockSite->ShowRow(this, bShow, bDelay == 0);  // tail jump to 0x54300
// Note the third argument is the NEGATION of bDelay: retail does
// `test r8d,r8d` on bDelay and `sete r9b`.
// CDockSite::ShowRow is still an empty stub in this tree (CDockSite.cpp:277),
// so the call currently has no effect -- but m_bVisible is updated for real,
// which is what UpdateVisibleState below depends on.
extern "C" void MS_ABI impl__ShowDockSiteRow_CDockingPanesRow__UEAAXHH_Z(
    void* pThis, int bShow, int bDelay) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;
    s->m_bVisible = bShow;
    // Deviation: retail loads m_pDockSite and jumps without a NULL check.
    if (s->m_pParentDockBar == nullptr) return;
    impl__ShowRow_CDockSite__QEAAXPEAVCDockingPanesRow__HH_Z(
        s->m_pParentDockBar, pThis, bShow, (bDelay == 0) ? TRUE : FALSE);
}

// Symbol: ?ShowPane@CDockingPanesRow@@UEAAHPEAVCPane@@HH@Z
// Retail 0x4fdb0, transcribed:
//   if (pBar is not in m_lstControlBars) return FALSE;
//   pBar->ShowWindow(bShow ? SW_SHOW : SW_HIDE);      // 0x2a79e0, `sbb/and 5`
//   UpdateVisibleState(bDelay);                       // vslot 0x98
//   if (!bShow)     { ExpandStretchedPanes(); if (bDelay) return TRUE; }
//   else            { if (bDelay) return TRUE;
//                     pBar->vslot 0x428 (); ArrangePanes(pBar); }  // vslot 0x60
//   CRect rectClient; GetClientRect(rectClient);       // 0x4fd50
//   m_pDockSite->RepositionPanes(rectClient);          // CDockSite vslot 0x540
//   RepositionPanes(rectClient, (UINT)-1, 0, 0);       // vslot 0x88
//   return TRUE;
// CPane vslot 0x428 is the shared empty body at 0x2820 in both the CPane and
// the CDockablePane vftables and OpenMFC exports no thunk for it, so that one
// dispatch is omitted; an override in a further-derived pane would be missed.
// CDockSite::RepositionPanes is still an empty stub (CDockSite.cpp:264).
extern "C" int MS_ABI impl__ShowPane_CDockingPanesRow__UEAAHPEAVCPane__HH_Z(
    void* pThis, CPane* pBar, int bShow, int bDelay) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return FALSE;

    ObNode* n = HeadNode(s);
    while (n != nullptr && n->data != reinterpret_cast<CObject*>(pBar)) n = n->pNext;
    if (n == nullptr) return FALSE;

    impl__ShowWindow_CWnd__QEAAHH_Z(static_cast<CWnd*>(pBar), bShow ? SW_SHOW : SW_HIDE);
    impl__UpdateVisibleState_CDockingPanesRow__UEAAXH_Z(pThis, bDelay);

    if (bShow == 0) {
        impl__ExpandStretchedPanes_CDockingPanesRow__QEAAXXZ(pThis);
        if (bDelay != 0) return TRUE;
    } else {
        if (bDelay != 0) return TRUE;
        impl__ArrangePanes_CDockingPanesRow__UEAAXPEAVCPane___Z(pThis, pBar);
    }

    CRect rectClient(0, 0, 0, 0);
    impl__GetClientRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pThis, &rectClient);
    if (s->m_pParentDockBar != nullptr) {   // deviation: retail does not NULL-check
        impl__RepositionPanes_CDockSite__UEAAXAEAVCRect___Z(s->m_pParentDockBar, RP(&rectClient));
    }
    impl__RepositionPanes_CDockingPanesRow__UEAAXAEAVCRect__IHH_Z(
        pThis, &rectClient, 0xFFFFFFFFu, 0, 0);
    return TRUE;
}

// Symbol: ?StretchPaneRect@CDockingPanesRow@@IEAAHPEAVCPane@@H@Z
// Retail 0x52660, transcribed -- the staged-rect twin of
// CPane::StretchPaneDeferWndPos, and it computes the delta the same way:
//   int nExpand  = pBar->GetAvailableExpandSize();    // vslot 0x500 -> 0xa1400
//   int nStretch = pBar->GetAvailableStretchSize();   // vslot 0x508 -> 0xa14f0
//   if (nStretchSize > 0) { if (!nExpand) return 0;
//                           nDelta = min(nExpand, nStretchSize); }
//   else                  { nDelta = nStretchSize;
//                           if (nStretch < abs(nStretchSize)) nDelta = -nStretch; }
//   CRect r = pBar->m_rectBar;                        // +0x118
//   if (row is horizontal) r.right += nDelta; else r.bottom += nDelta;
//   if (abs(nDelta) > 0) pBar->m_rectBar = r;
//   return nDelta;
// Both getters are devirtualised to their CPane thunks (CPane.cpp:627 / 649).
extern "C" int MS_ABI impl__StretchPaneRect_CDockingPanesRow__IEAAHPEAVCPane__H_Z(
    void* pThis, CPane* pBar, int nStretchSize) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr || pBar == nullptr) return 0;

    const int nExpand = impl__GetAvailableExpandSize_CPane__UEBAHXZ(pBar);
    const int nStretch = impl__GetAvailableStretchSize_CPane__UEBAHXZ(pBar);

    int nDelta;
    if (nStretchSize > 0) {
        if (nExpand == 0) return 0;
        nDelta = (nExpand > nStretchSize) ? nStretchSize : nExpand;
    } else {
        nDelta = nStretchSize;
        if (nStretch < AbsInt(nStretchSize)) nDelta = -nStretch;
    }

    CRect rect = pBar->m_rectBar;
    if (RowIsHorz(s)) rect.right += nDelta;
    else              rect.bottom += nDelta;
    if (AbsInt(nDelta) > 0) pBar->m_rectBar = rect;
    return nDelta;
}

// Symbol: ?UpdateVisibleState@CDockingPanesRow@@UEAAXH@Z
// Retail 0x4fed0, transcribed:
//   BOOL bParentVisible = ::IsWindowVisible(
//        CWnd::FromHandle(::GetParent(m_pDockSite->m_hWnd))->m_hWnd);
//   BOOL bVisible = FALSE;
//   for (each pane in m_lstControlBars) {
//       if (!bParentVisible && pane->vslot 0x370 ()) {
//           bVisible = pane->vslot 0x368 ();
//           if (bVisible) break;                       // else keep scanning
//       } else if (pane->GetStyle() & WS_VISIBLE) {    // bt $0x1c,%eax
//           bVisible = TRUE; break;
//       }
//   }
//   if (m_bVisible != bVisible) ShowDockSiteRow(bVisible, bDelay);  // vslot 0xA0
//   m_bVisible = bVisible;                                          // AFTER the call
// Slots 0x368 and 0x370 are unexported one-instruction getters -- 0x87f0 is
// `return this[+0xEC]` and 0x8800 is `return this[+0xF0]`, i.e.
// CBasePane::m_bRecentVisibleState and CBasePane::m_bIsRestoredFromRegistry.
// They are read as members here because OpenMFC has no thunk for either; a
// derived override of those virtuals would be missed.
// The CWnd::FromHandle round trip only maps the HWND back to itself, so the
// visibility test is issued straight against ::GetParent's result.
extern "C" void MS_ABI impl__UpdateVisibleState_CDockingPanesRow__UEAAXH_Z(void* pThis, int bDelay) {
    S_CDockingPanesRow* s = Row(pThis);
    if (s == nullptr) return;

    // Deviation: retail dereferences m_pDockSite unchecked.
    BOOL bParentVisible = FALSE;
    if (s->m_pParentDockBar != nullptr) {
        bParentVisible = ::IsWindowVisible(::GetParent(s->m_pParentDockBar->m_hWnd));
    }

    BOOL bVisible = FALSE;
    for (ObNode* n = HeadNode(s); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr) continue;   // deviation: retail does not NULL-check
        if (!bParentVisible && pBar->m_bIsRestoredFromRegistry != 0) {
            bVisible = pBar->m_bRecentVisibleState;
            if (bVisible) break;
        } else if ((impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pBar)) & WS_VISIBLE) != 0) {
            bVisible = TRUE;
            break;
        }
    }

    if (s->m_bVisible != bVisible) {
        // retail: this->ShowDockSiteRow(bVisible, bDelay) through vslot 0xA0.
        impl__ShowDockSiteRow_CDockingPanesRow__UEAAXHH_Z(pThis, bVisible, bDelay);
    }
    s->m_bVisible = bVisible;
}
