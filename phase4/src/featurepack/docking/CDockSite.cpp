// CDockSite — OpenMFC implementation.
// Sources: cbarcore.cpp, manual_small_stub_implementations.cpp
//
// The bodies marked "Retail 0x....." below were decoded from the retail export
// the way phase4/src/core/ole/COleControl.cpp describes.  Every RVA quoted is a
// FUNCTION ENTRY in mfc140.dll (the ANSI twin of mfc140u.dll -- function bodies
// are byte-identical between the two images, so the control flow, member
// offsets and constants read off it are valid, but its addresses are NOT
// mfc140u addresses and are never quoted as such).
//
// Instance layout, read out of the retail constructor ??0CDockSite@@QEAA@XZ
// (mfc140 RVA 0x52e10) and cross-checked against the shipping afxdocksite.h
// member order and the sizeof 544 recorded in detail/CbarcoreSupport.cpp:
//
//   0x000 CBasePane subobject (0x1A8 bytes, include/openmfc/afxmfc.h)
//   0x1A8 CObList m_lstControlBars   (56 bytes: vfptr, head +8, tail +0x10,
//   0x1E0 CObList m_lstDockBarRows    count +0x18, ..., nBlockSize 10 at +0x30)
//   0x218 UINT    m_nDockBarID        (+4 tail pad)
//   0x220 == sizeof
//
// Retail CDockSite vftable (mfc140 RVA 0x2e4388, slot 0 = GetThisClass 0xa580,
// slot 154 = CalcFixedLayout 0x54930) -- slots the bodies below dispatch through:
//   +0x2d8 IsHorizontal      (0x88e0: GetCurrentAlignment() & 0xA000)
//   +0x300 IsResizable       +0x338 GetCurrentAlignment (0xce30)
//   +0x428 AdjustLayout      +0x478 CBasePane::MoveWindow (0xb580)
//   +0x480 CBasePane::SetWindowPos (0xb6a0)   +0x4a0 OnPaneContextMenu (0xcf50)
//   +0x4e8 IsDragMode        +0x4f0 CreateRow  +0x518 OnInsertRow
//   +0x520 OnRemoveRow       +0x528 OnResizeRow +0x530 OnShowRow
//   +0x548 FixupVirtualRects +0x560 AdjustDockingLayout +0x570 OnSetWindowPos
// Retail CDockingPanesRow vftable (mfc140 RVA 0x2e4148) slots used here:
//   +0x08 scalar deleting dtor  +0x28 Create (inline `return TRUE`, 0x3ae0)
//   +0x40 CalcFixedLayout  +0x50 AddPaneFromRow  +0x58 RemovePane
//   +0x78 Resize  +0x80 Move  +0x88 RepositionPanes  +0x90 IsEmpty (0x4f3d0:
//   m_nCount == 0)  +0xb0 ShowPane  +0xc0 IsVisible (0x97e0: m_bVisible)
//   +0xd0 IsExclusiveRow
//
// Systematic deviations, stated once:
//  * OpenMFC installs no retail-shaped vftable on these objects, so every
//    virtual retail dispatches through `this`, a row or a pane is called here
//    as the sibling impl__ thunk of the class that owns the slot (a
//    devirtualisation; overrides in derived classes are not reached).  Each
//    site names the retail slot it stands in for.
//  * OpenMFC's CObList keeps its contents in a side table keyed by `this`
//    (include/openmfc/afx.h) and never updates the in-object m_nCount word, so
//    wherever retail reads m_nCount (+0x18 of a list) the count is recomputed
//    by walking the node chain from FindIndex(0), exactly as
//    CDockingPanesRow.cpp does.
//  * An indirect `call *0x1802c5bd0` in the retail listings is the Control
//    Flow Guard dispatch through RAX, not an import.  Every USER32/CRT import
//    named below was resolved through the import table (iat.py), not guessed.

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Thunks defined elsewhere in this DLL (briefing S1: the C++ methods of these
// classes exist only as impl__ exports).  Each one was located with grep before
// being declared here; the file that defines it is named per group.
// ---------------------------------------------------------------------------
// core/collections/CObList.cpp
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(CObList* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CObList__UEAA_XZ(CObList* pThis);
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
extern "C" void MS_ABI impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(CObList* pThis, CObList::POSITION* pPos);
extern "C" CObject* MS_ABI impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(CObList* pThis);
extern "C" void MS_ABI impl__RemoveAll_CObList__QEAAXXZ(CObList* pThis);
// detail/MemcoreSupport.cpp -- the DLL's exported ??2@YAPEAX_K@Z / ??3@YAXPEAX@Z,
// which is what retail's `new`/`delete` in this file resolve to (0x2840).
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);
// detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// core/window/Thunks.cpp, core/window/CWnd.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnNcDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" void MS_ABI impl__CalcWindowRect_CWnd__UEAAXPEAUtagRECT__I_Z(CWnd* pThis, RECT* lpClientRect,
                                                                       unsigned int nAdjustType);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis,
                                                                        const CRuntimeClass* pClass);
// core/gdi/CPaintDC.cpp
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);
// featurepack/docking/Thunks.cpp, CBasePane.cpp
extern "C" void* MS_ABI impl___0CBasePane__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CBasePane__UEAA_XZ(void* pThis);
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis);
extern "C" void* MS_ABI impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(
    CBasePane* pThis, const RECT* lpRect, int bRepaint, void* hdwp);
extern "C" void* MS_ABI impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
    CBasePane* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy,
    unsigned int nFlags, void* hdwp);
extern "C" void MS_ABI impl__OnPaneContextMenu_CBasePane__UEAAXPEAVCWnd__VCPoint___Z(
    CBasePane* pThis, CWnd* pParentFrame, long long point);
// CBasePane::RemovePaneFromDockManager is still a placeholder-signature stub in
// CBasePane.cpp:1428; this is the retail export shape (this + 5 arguments), the
// same one CDockablePane.cpp:219 declares.
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CBasePane__QEAAXPEAV1_HHH0_Z(
    CBasePane* pThis, CBasePane* pPane, int bDestroy, int bAdjustLayout, int bAutoHide,
    CBasePane* pBarReplacement);
// featurepack/docking/CPane.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__GetVirtualRect_CPane__QEBAXAEAVCRect___Z(const CPane* pThis, RECT* pRect);
extern "C" void MS_ABI impl__UpdateVirtualRect_CPane__QEAAXVCPoint___Z(CPane* pThis, void* ptOffset);
extern "C" int MS_ABI impl__IsChangeState_CPane__UEBA_AW4AFX_CS_STATUS__HPEAPEAVCBasePane___Z(
    const CPane* pThis, int nOffset, CBasePane** ppTargetBar);
extern "C" int MS_ABI impl__FloatPane_CPane__UEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(
    CPane* pThis, const RECT* pRectFloat, int dockMethod, bool bShow);
// featurepack/controls/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCReBar__SAPEAUCRuntimeClass__XZ();
// featurepack/visualmanager/Thunks.cpp:1498 (dispatches C++-virtually on the
// OpenMFC manager object) and the exported mirror of CMFCVisualManager's
// m_pVisManager static (core/runtime/StaticData.cpp:56, kept in sync by
// detail/MfccoreSupport.cpp:606) -- the same pair CPaneDivider::OnPaint and
// CMFCToolBar::DrawSeparator use to reach the visual manager from a TU that
// may not call the C++ static CMFCVisualManager::GetInstance().
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CBasePane* pBar, CRect rect, CRect rectClip, int bNCArea);
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
// featurepack/docking/CDockingPanesRow.cpp (all real bodies)
extern "C" void* MS_ABI impl___0CDockingPanesRow__QEAA_PEAVCDockSite__HH_Z(
    void* pThis, void* pDockSite, int nOffset, int nRowHeight);
extern "C" void MS_ABI impl___1CDockingPanesRow__UEAA_XZ(void* pThis);
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CDockingPanesRow__UEAA_AVCSize__HH_Z(
    void* pThis, CSize* pRet, int bStretch, int bHorz);
extern "C" void MS_ABI impl__AddPaneFromRow_CDockingPanesRow__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(
    void* pThis, CPane* pBar, int dockMethod);
extern "C" void MS_ABI impl__RemovePane_CDockingPanesRow__UEAAXPEAVCPane___Z(void* pThis, CPane* pControlBar);
extern "C" int  MS_ABI impl__Resize_CDockingPanesRow__UEAAHH_Z(void* pThis, int nOffset);
extern "C" void MS_ABI impl__Move_CDockingPanesRow__UEAAXH_Z(void* pThis, int nOffset);
extern "C" void MS_ABI impl__RepositionPanes_CDockingPanesRow__UEAAXAEAVCRect__IHH_Z(
    void* pThis, CRect* pRectNew, unsigned int nSide, int bExpand, int nOffset);
extern "C" int  MS_ABI impl__ShowPane_CDockingPanesRow__UEAAHPEAVCPane__HH_Z(
    void* pThis, CPane* pBar, int bShow, int bDelay);
extern "C" int  MS_ABI impl__IsExclusiveRow_CDockingPanesRow__UEBAHXZ(const void* pThis);
extern "C" void MS_ABI impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(const void* pThis, CRect* pRect);
extern "C" void MS_ABI impl__ExpandStretchedPanesRect_CDockingPanesRow__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__FixupVirtualRects_CDockingPanesRow__QEAAX_NPEAVCPane___Z(
    void* pThis, bool bMoveBackToVirtualRect, CPane* pBarToExclude);
extern "C" void MS_ABI impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCPoint__HAEAPEAX_Z(
    void* pThis, CPane* pBar, long long ptOffset, int bSwapControlBars, void** phdwp);
// Exported statics (core/runtime/StaticData.cpp, toolbar/StaticData.cpp,
// featurepack/CMFC_misc_stubs.cpp, core/window/CWnd.cpp).
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" void* impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA;
extern "C" const unsigned char impl__wndBottom_CWnd__2V1_B[];   // ?wndBottom@CWnd@@2V1@B

// Same-file thunks used before their definition.
extern "C" void* MS_ABI impl___0CDockSite__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CDockSite__UEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__IsDragMode_CDockSite__UEBAHXZ(const void* pThis);
extern "C" void* MS_ABI impl__CreateRow_CDockSite__UEAAPEAVCDockingPanesRow__PEAV1_HH_Z(
    void* pThis, void* pParentDockBar, int nOffset, int nRowHeight);
extern "C" void* MS_ABI impl__AddRow_CDockSite__QEAAPEAVCDockingPanesRow__PEAU__POSITION__H_Z(
    void* pThis, void* pos, int nHeight);
extern "C" int MS_ABI impl__ResizeRow_CDockSite__QEAAHPEAVCDockingPanesRow__HH_Z(
    void* pThis, void* pRow, int nNewSize, int bAdjustLayout);
extern "C" void MS_ABI impl__ResizeDockSiteByOffset_CDockSite__IEAAXHH_Z(void* pThis, int nOffset, int bAdjustLayout);
extern "C" void MS_ABI impl__OnInsertRow_CDockSite__UEAAXPEAU__POSITION___Z(void* pThis, void* pos);
extern "C" void MS_ABI impl__OnRemoveRow_CDockSite__UEAAXPEAU__POSITION__H_Z(void* pThis, void* pos, int bByShow);
extern "C" int MS_ABI impl__OnResizeRow_CDockSite__UEAAHPEAVCDockingPanesRow__H_Z(void* pThis, void* pRow, int nOffset);
extern "C" void MS_ABI impl__OnShowRow_CDockSite__UEAAXPEAU__POSITION__H_Z(void* pThis, void* pos, int bShow);
extern "C" void MS_ABI impl__FixupVirtualRects_CDockSite__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockSite__UEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__OnSetWindowPos_CDockSite__UEAAHPEBVCWnd__AEBVCRect__I_Z(
    void* pThis, const CWnd* pWndInsertAfter, const CRect& rect, unsigned int nFlags);
extern "C" void* MS_ABI impl__FindNextVisibleRow_CDockSite__IEAAPEAVCDockingPanesRow__AEAPEAU__POSITION__H_Z(
    void* pThis, void** pPos, int bForward);
extern "C" void* MS_ABI impl__RowFromPoint_CDockSite__IEBAPEAVCDockingPanesRow__VCPoint__AEA_N_Z(
    const void* pThis, long long pt, bool* pbOuterRow);
extern "C" void* MS_ABI impl__RowFromPane_CDockSite__IEBAPEAVCDockingPanesRow__PEAVCBasePane___Z(
    const void* pThis, CBasePane* pBar);
extern "C" void MS_ABI impl__SwapRows_CDockSite__QEAAXPEAVCDockingPanesRow__0_Z(void* pThis, void* pFirstRow,
                                                                                void* pSecondRow);

// ---------------------------------------------------------------------------
// Shadow layouts + local helpers
// ---------------------------------------------------------------------------
namespace {

struct S_CDockSite {
    CBasePane base;                 // 0x000 (0x1A8 bytes)
    CObList   m_lstControlBars;     // 0x1A8
    CObList   m_lstDockBarRows;     // 0x1E0
    UINT      m_nDockBarID;         // 0x218
    UINT      m_pad21C;             // 0x21C (tail pad in retail)
};
static_assert(sizeof(CBasePane) == 0x1A8, "CBasePane must be 424 bytes (afxmfc.h)");
static_assert(sizeof(CObList) == 0x38, "CObList must be 56 bytes");
static_assert(offsetof(S_CDockSite, m_lstControlBars) == 0x1A8, "CDockSite layout");
static_assert(offsetof(S_CDockSite, m_lstDockBarRows) == 0x1E0, "CDockSite layout");
static_assert(offsetof(S_CDockSite, m_nDockBarID)     == 0x218, "CDockSite layout");
static_assert(sizeof(S_CDockSite) == 0x220, "CDockSite is 544 bytes in retail");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at CWnd+0x40");
static_assert(offsetof(CPane, m_pDockBarRow) == 0x130, "retail reads m_pDockBarRow at +0x130");
static_assert(offsetof(CPane, m_bDisableMove) == 0x1A8, "retail writes m_bDisableMove at +0x1A8");
static_assert(offsetof(CPane, m_bDragMode) == 0x214, "CPane::IsDragMode reads +0x214");

// Mirror of the CDockingPanesRow layout documented at the top of
// CDockingPanesRow.cpp (read out of ??0CDockingPanesRow@@QEAA@PEAVCDockSite@@HH@Z,
// mfc140 RVA 0x4f3f0).
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
static_assert(offsetof(S_CDockingPanesRow, m_bVisible)       == 0x0C, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_nRowHeight)     == 0x10, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_nRowOffset)     == 0x1C, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_lstControlBars) == 0x28, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_dwRowAlignment) == 0x60, "row layout");
static_assert(offsetof(S_CDockingPanesRow, m_pParentDockBar) == 0x68, "row layout");
static_assert(sizeof(S_CDockingPanesRow) == 0x70, "CDockingPanesRow is 112 bytes in retail");

// Mirror of CList<CObject*, CObject*>::CNode (include/openmfc/afx.h) -- what a
// CObList::POSITION points at.  GetHeadPosition / GetNext / GetCount are inline
// in retail MFC and therefore have no export to call; this read-only view plus
// the exported FindIndex thunk is how every list here is walked.
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
inline CObList::POSITION PosFromNode(const void* n) {
    CObList::POSITION pos(nullptr);
    std::memcpy(&pos, &n, sizeof(n));
    return pos;
}
inline ObNode* HeadNode(const CObList* pList) {
    return NodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0));
}
inline ObNode* TailNode(const CObList* pList) {
    ObNode* n = HeadNode(pList);
    if (n == nullptr) return nullptr;
    while (n->pNext != nullptr) n = n->pNext;
    return n;
}
// Retail reads the list's m_nCount word directly; see the file header.
inline int ListCount(const CObList* pList) {
    int n = 0;
    for (ObNode* p = HeadNode(pList); p != nullptr; p = p->pNext) ++n;
    return n;
}
// The `for (pos = GetHeadPosition(); pos; ) if (GetNext(pos) == x)` search that
// retail open-codes as a `cmp %rdx,0x10(%rax); je` loop over the node chain.
inline ObNode* FindNode(const CObList* pList, const void* data) {
    for (ObNode* p = HeadNode(pList); p != nullptr; p = p->pNext)
        if (p->data == data) return p;
    return nullptr;
}

inline S_CDockSite* Site(void* p) { return static_cast<S_CDockSite*>(p); }
inline const S_CDockSite* Site(const void* p) { return static_cast<const S_CDockSite*>(p); }
inline CBasePane* AsPane(void* p) { return static_cast<CBasePane*>(p); }
inline const CBasePane* AsPane(const void* p) { return static_cast<const CBasePane*>(p); }
inline S_CDockingPanesRow* RowOf(const void* p) {
    return static_cast<S_CDockingPanesRow*>(const_cast<void*>(p));
}
inline S_CDockingPanesRow* RowOf(ObNode* n) { return RowOf(n->data); }
inline CPane* PaneOf(ObNode* n) { return reinterpret_cast<CPane*>(n->data); }

// Retail CDockSite vslot +0x2d8 (IsHorizontal, 0x88e0) is
// `GetCurrentAlignment() & 0xA000`, GetCurrentAlignment being vslot +0x338
// (CBasePane::GetCurrentAlignment, 0xce30: m_dwStyle & 0xF000).  Devirtualised
// onto the CBasePane thunk (CBasePane.cpp:841 computes the same expression).
inline unsigned long SiteAlignment(const void* pThis) {
    return impl__GetCurrentAlignment_CBasePane__UEBAKXZ(AsPane(pThis));
}
inline bool SiteIsHorz(const void* pThis) { return (SiteAlignment(pThis) & 0xA000UL) != 0; }

// CBRS_ALIGN_* as retail compares them (afxole.h in this tree carries an
// unrelated 0x000F flavour, so the values are spelled out).
const unsigned long kAlignLeft   = 0x1000;
const unsigned long kAlignTop    = 0x2000;
const unsigned long kAlignRight  = 0x4000;
const unsigned long kAlignBottom = 0x8000;

// A CPoint passed BY VALUE arrives packed in one integer register (low dword x,
// high dword y); see CDockingPanesRow.cpp for the retail evidence.
inline int PtX(long long pt) { return static_cast<int>(static_cast<unsigned long long>(pt) & 0xFFFFFFFFull); }
inline int PtY(long long pt) { return static_cast<int>(static_cast<unsigned long long>(pt) >> 32); }
inline void* PackedPtr(long long pt) {
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(static_cast<unsigned long long>(pt)));
}

static_assert(sizeof(CRect) == sizeof(RECT), "CRect must be RECT-shaped");
static_assert(offsetof(CRect, left) == offsetof(RECT, left), "CRect must be RECT-shaped");
static_assert(offsetof(CRect, bottom) == offsetof(RECT, bottom), "CRect must be RECT-shaped");
inline RECT* RP(CRect* r) { return reinterpret_cast<RECT*>(r); }
inline RECT* RP(CRect& r) { return reinterpret_cast<RECT*>(&r); }
inline const RECT* RP(const CRect* r) { return reinterpret_cast<const RECT*>(r); }
inline const RECT* RP(const CRect& r) { return reinterpret_cast<const RECT*>(&r); }

// Retail calls the CRT `abs` import (0x1802c5910) on plain ints.
inline int AbsInt(int v) { return v < 0 ? -v : v; }

// Retail row vslot +0xc0 (IsVisible) is the afxdockingpanesrow.h inline
// `return m_bVisible;` (0x97e0 reads +0xC).
inline bool RowIsVisible(const S_CDockingPanesRow* r) { return r->m_bVisible != 0; }

// Retail row vslot +0x08 with flag 1: the scalar deleting destructor
// (~CDockingPanesRow then ??3@YAXPEAX@Z).  The row thunk in CDockingPanesRow.cpp
// is a real body (m_lstControlBars.RemoveAll()).
inline void DeleteRow(void* pRow) {
    if (pRow == nullptr) return;
    impl___1CDockingPanesRow__UEAA_XZ(pRow);
    impl___3_YAXPEAX_Z(pRow);
}

// The three-field HTLEFT/HTRIGHT/HTTOP/HTBOTTOM codes RectSideFromPoint hands
// back (0xa/0xb/0xc/0xf in the retail listing).
const int kHitLeft = 10, kHitRight = 11, kHitTop = 12, kHitBottom = 15;

} // namespace

// Symbol: ?CreateObject@CDockSite@@SAPEAVCObject@@XZ
// Retail 0x52de0: `p = ::operator new(0x220); if (p) CDockSite::CDockSite(p);
// return p;` -- the DECLARE_DYNCREATE factory.  The exported operator new
// (0x2840, ??2@YAPEAX_K@Z) is reached through the memcore thunk.  NOTE:
// g_runtimeClassDockSite in detail/CbarcoreSupport.cpp:280 still carries a NULL
// m_pfnCreateObject, so RUNTIME_CLASS(CDockSite)->CreateObject() does not reach
// this yet (see headerRequests).
extern "C" CObject* MS_ABI impl__CreateObject_CDockSite__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(S_CDockSite));
    if (p != nullptr) impl___0CDockSite__QEAA_XZ(p);
    return static_cast<CObject*>(p);
}
// Symbol: ?GetRuntimeClass@CDockSite@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDockSite__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_runtimeClassDockSite;
}
// Symbol: ?GetThisClass@CDockSite@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassDockSite;
}
// Symbol: ?Create@CDockSite@@UEAAHKAEBUtagRECT@@PEAVCWnd@@KPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CDockSite__UEAAHKAEBUtagRECT__PEAVCWnd__KPEAUCCreateContext___Z(
    void* pThis, unsigned long, const RECT& rect, CWnd* pParentWnd, unsigned long, CCreateContext*) {
    if (pThis == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    state.parent = pParentWnd;
    state.recentRect = NormalizeRect(&rect, 320, 220);
    return TRUE;
}
// Symbol: ?CreateEx@CDockSite@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@KPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CDockSite__UEAAHKKAEBUtagRECT__PEAVCWnd__KPEAUCCreateContext___Z(
    void* pThis, unsigned long, unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned long nID, CCreateContext* pContext) {
    return impl__Create_CDockSite__UEAAHKAEBUtagRECT__PEAVCWnd__KPEAUCCreateContext___Z(
        pThis, dwStyle, rect, pParentWnd, nID, pContext);
}
// Symbol: ?DockPane@CDockSite@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@PEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT___Z(
    void* pThis, CPane* pPane, int, const RECT* lpRect) {
    if (pThis == nullptr || pPane == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    g_dockSiteCoreState[pThis].recentRect = NormalizeRect(lpRect, 320, 220);
    g_paneCoreState_Cbarcore[pPane].parent = reinterpret_cast<CWnd*>(pThis);
}
// Symbol: ?DockPaneLeftOf@CDockSite@@UEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_CDockSite__UEAAHPEAVCPane__0_Z(void* pThis, CPane* pPaneToDock, CPane* pLeftOf) {
    if (pThis == nullptr || pPaneToDock == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    PaneCoreState& state = g_paneCoreState_Cbarcore[pPaneToDock];
    if (pLeftOf != nullptr) {
        auto it = g_paneCoreState_Cbarcore.find(pLeftOf);
        state.parent = (it != g_paneCoreState_Cbarcore.end()) ? it->second.parent : reinterpret_cast<CWnd*>(pThis);
    } else {
        state.parent = reinterpret_cast<CWnd*>(pThis);
    }
    return TRUE;
}
// Symbol: ?OnSetWindowPos@CDockSite@@UEAAHPEBVCWnd@@AEBVCRect@@I@Z
// Retail 0x54d80, transcribed in full:
//   return SetWindowPos(pWndInsertAfter, rectWnd.left, rectWnd.top,
//                       rectWnd.Width(), rectWnd.Height(),
//                       nFlags | SWP_NOACTIVATE, NULL) != NULL;   // vslot +0x480
// (`or $0x10,%ebx` is SWP_NOACTIVATE; the HDWP argument at [rsp+0x38] is 0.)
// Devirtualised onto the CBasePane::SetWindowPos thunk.
extern "C" int MS_ABI impl__OnSetWindowPos_CDockSite__UEAAHPEBVCWnd__AEBVCRect__I_Z(
    void* pThis, const CWnd* pWndInsertAfter, const CRect& rect, unsigned int nFlags) {
    if (pThis == nullptr) return FALSE;
    void* hdwp = impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
        AsPane(pThis), pWndInsertAfter, rect.left, rect.top, rect.right - rect.left,
        rect.bottom - rect.top, nFlags | SWP_NOACTIVATE, nullptr);
    return hdwp != nullptr ? TRUE : FALSE;
}
// Symbol: ?ResizeDockSite@CDockSite@@QEAAXHH@Z
extern "C" void MS_ABI impl__ResizeDockSite_CDockSite__QEAAXHH_Z(void* pThis, int cx, int cy) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    CRect& rect = state.recentRect;
    rect.right = rect.left + (cx > 0 ? cx : rect.Width());
    rect.bottom = rect.top + (cy > 0 ? cy : rect.Height());
    ++state.layoutRevision;
}
// Symbol: ?ResizeDockSiteByOffset@CDockSite@@IEAAXHH@Z
// Retail 0x54810, transcribed in full:
//   CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);
//   CWnd::FromHandle(::GetParent(m_hWnd))->ScreenToClient(&rectWnd);  // 0x289180 / 0x2a11f0
//   switch (GetCurrentAlignment()) {                                 // vslot +0x338
//     case CBRS_ALIGN_LEFT:   rectWnd.right  += nOffset; break;
//     case CBRS_ALIGN_TOP:    rectWnd.bottom += nOffset; break;
//     case CBRS_ALIGN_RIGHT:  rectWnd.left   -= nOffset; break;
//     case CBRS_ALIGN_BOTTOM: rectWnd.top    -= nOffset; break;
//   }
//   MoveWindow(rectWnd, TRUE, NULL);                                 // vslot +0x478
//   if (bAdjustLayout) AdjustDockingLayout();                        // vslot +0x560
// Both virtuals are devirtualised onto their sibling thunks (CBasePane::MoveWindow
// and this file's AdjustDockingLayout).  Retail does not test the FromHandle
// result; it is guarded here.
extern "C" void MS_ABI impl__ResizeDockSiteByOffset_CDockSite__IEAAXHH_Z(void* pThis, int nOffset, int bAdjustLayout) {
    if (pThis == nullptr) return;
    CBasePane* pSelf = AsPane(pThis);
    CRect rectWnd(0, 0, 0, 0);
    ::GetWindowRect(pSelf->m_hWnd, RP(&rectWnd));
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pSelf->m_hWnd));
    if (pParent != nullptr) impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, RP(&rectWnd));

    switch (SiteAlignment(pThis)) {
    case kAlignLeft:   rectWnd.right  += nOffset; break;
    case kAlignTop:    rectWnd.bottom += nOffset; break;
    case kAlignRight:  rectWnd.left   -= nOffset; break;
    case kAlignBottom: rectWnd.top    -= nOffset; break;
    default: break;
    }
    impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(pSelf, RP(&rectWnd), TRUE, nullptr);
    if (bAdjustLayout) impl__AdjustDockingLayout_CDockSite__UEAAXXZ(pThis);
}
// Symbol: ?ResizeRow@CDockSite@@QEAAHPEAVCDockingPanesRow@@HH@Z
// Retail 0x54210, transcribed in full:
//   int nOffset = nNewSize - pRow->m_nRowHeight;                      // +0x10
//   if (nOffset < 0 && !pRow->IsEmpty()) {                            // row vslot +0x90
//       CSize size = pRow->CalcFixedLayout(TRUE, IsHorizontal());     // row vslot +0x40
//       if (IsHorizontal()  && nNewSize < size.cy) return 0;
//       if (!IsHorizontal() && nNewSize < size.cx) return 0;
//   }
//   int nActualOffset = OnResizeRow(pRow, nOffset);                   // vslot +0x528
//   ResizeDockSiteByOffset(nActualOffset, bAdjustLayout);             // 0x54810
//   return nActualOffset;
// IsHorizontal (vslot +0x2d8) is re-evaluated at each of its three uses in
// retail; the value cannot change between them, so it is read once here.
extern "C" int MS_ABI impl__ResizeRow_CDockSite__QEAAHPEAVCDockingPanesRow__HH_Z(
    void* pThis, void* pRow, int nNewSize, int bAdjustLayout) {
    if (pThis == nullptr || pRow == nullptr) return 0;
    S_CDockingPanesRow* r = RowOf(pRow);
    const int nOffset = nNewSize - r->m_nRowHeight;
    const bool bHorz = SiteIsHorz(pThis);
    if (nOffset < 0 && ListCount(&r->m_lstControlBars) != 0) {
        CSize size(0, 0);
        impl__CalcFixedLayout_CDockingPanesRow__UEAA_AVCSize__HH_Z(pRow, &size, TRUE, bHorz ? TRUE : FALSE);
        if (bHorz && nNewSize < size.cy) return 0;
        if (!bHorz && nNewSize < size.cx) return 0;
    }
    const int nActualOffset = impl__OnResizeRow_CDockSite__UEAAHPEAVCDockingPanesRow__H_Z(pThis, pRow, nOffset);
    impl__ResizeDockSiteByOffset_CDockSite__IEAAXHH_Z(pThis, nActualOffset, bAdjustLayout);
    return nActualOffset;
}
// Symbol: ?ShowPane@CDockSite@@UEAAHPEAVCBasePane@@HHH@Z
// Retail 0x547b0, transcribed in full:
//   CDockingPanesRow* pRow = RowFromPane(pBar);                       // 0x54770
//   if (pRow == NULL || pBar == NULL || !pBar->IsKindOf(RUNTIME_CLASS(CPane)))
//       return FALSE;                                                 // 0x1802f3868 = CPane
//   return pRow->ShowPane((CPane*)pBar, bShow, bDelay);               // row vslot +0xb0
// bActivate is accepted and never used, exactly as in retail.
extern "C" int MS_ABI impl__ShowPane_CDockSite__UEAAHPEAVCBasePane__HHH_Z(void* pThis, CBasePane* pPane, int bShow, int bDelay, int bActivate) {
    (void)bActivate;
    if (pThis == nullptr) return FALSE;
    void* pRow = impl__RowFromPane_CDockSite__IEBAPEAVCDockingPanesRow__PEAVCBasePane___Z(pThis, pPane);
    if (pRow == nullptr || pPane == nullptr) return FALSE;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ()))
        return FALSE;
    return impl__ShowPane_CDockingPanesRow__UEAAHPEAVCPane__HH_Z(pRow, static_cast<CPane*>(pPane), bShow, bDelay);
}
// Symbol: ?AdjustDockingLayout@CDockSite@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockSite__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    state.recentRect = NormalizeRect(&state.recentRect, 320, 220);
    ++state.layoutRevision;
}
// Symbol: ?AdjustLayout@CDockSite@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CDockSite__UEAAXXZ(void* pThis) {
    impl__AdjustDockingLayout_CDockSite__UEAAXXZ(pThis);
}
// Symbol: ?FixupVirtualRects@CDockSite@@UEAAXXZ
// Retail 0x53e80, transcribed in full:
//   for (POSITION pos = m_lstDockBarRows.GetHeadPosition(); pos != NULL; )
//       ((CDockingPanesRow*)m_lstDockBarRows.GetNext(pos))
//           ->FixupVirtualRects(false, NULL);                         // 0x51170 (non-virtual)
extern "C" void MS_ABI impl__FixupVirtualRects_CDockSite__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    for (ObNode* n = HeadNode(&Site(pThis)->m_lstDockBarRows); n != nullptr; n = n->pNext)
        impl__FixupVirtualRects_CDockingPanesRow__QEAAX_NPEAVCPane___Z(n->data, false, nullptr);
}
// Symbol: ?OnInsertRow@CDockSite@@UEAAXPEAU__POSITION@@@Z
// Retail 0x54360, transcribed in full:
//   if (pos == NULL) AfxThrowInvalidArgException();                   // 0x225b80
//   CDockingPanesRow* pRow = (CDockingPanesRow*)m_lstDockBarRows.GetNext(pos);
//   int nRowHeight = pRow->m_nRowHeight;                               // +0x10
//   while (pos != NULL)
//       ((CDockingPanesRow*)m_lstDockBarRows.GetNext(pos))->Move(nRowHeight);  // row vslot +0x80
// i.e. every row AFTER the inserted one is shifted by its height.
extern "C" void MS_ABI impl__OnInsertRow_CDockSite__UEAAXPEAU__POSITION___Z(void* pThis, void* pos) {
    if (pThis == nullptr) return;
    if (pos == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    ObNode* n = static_cast<ObNode*>(pos);
    const int nRowHeight = RowOf(n)->m_nRowHeight;
    for (n = n->pNext; n != nullptr; n = n->pNext)
        impl__Move_CDockingPanesRow__UEAAXH_Z(n->data, nRowHeight);
}
// Symbol: ?OnRemoveRow@CDockSite@@UEAAXPEAU__POSITION@@H@Z
// Retail 0x543b0, transcribed in full:
//   if (pos == NULL) AfxThrowInvalidArgException();
//   CDockingPanesRow* pRow = (CDockingPanesRow*)m_lstDockBarRows.GetNext(pos);
//   if (!pRow->IsVisible() && !bByShow) return;                       // row vslot +0xc0
//   int nRowHeight = pRow->m_nRowHeight;
//   while (pos != NULL)
//       ((CDockingPanesRow*)m_lstDockBarRows.GetNext(pos))->Move(-nRowHeight);
extern "C" void MS_ABI impl__OnRemoveRow_CDockSite__UEAAXPEAU__POSITION__H_Z(void* pThis, void* pos, int bByShow) {
    if (pThis == nullptr) return;
    if (pos == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    ObNode* n = static_cast<ObNode*>(pos);
    S_CDockingPanesRow* r = RowOf(n);
    if (!RowIsVisible(r) && !bByShow) return;
    const int nRowHeight = r->m_nRowHeight;
    for (n = n->pNext; n != nullptr; n = n->pNext)
        impl__Move_CDockingPanesRow__UEAAXH_Z(n->data, -nRowHeight);
}
// Symbol: ??0CDockSite@@QEAA@XZ
// Retail 0x52e10, transcribed in full: CBasePane::CBasePane() (0xad30), install
// the CDockSite vftable, construct m_lstControlBars (+0x1A8) and m_lstDockBarRows
// (+0x1E0) as CObList(10), m_nDockBarID = 0.  OpenMFC has no retail vftable to
// install; the two lists go through the exported CObList constructor so their
// side-table storage exists (see the file header).
extern "C" void* MS_ABI impl___0CDockSite__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    S_CDockSite* s = Site(pThis);
    impl___0CBasePane__QEAA_XZ(pThis);
    impl___0CObList__QEAA__J_Z(&s->m_lstControlBars, 10);
    impl___0CObList__QEAA__J_Z(&s->m_lstDockBarRows, 10);
    s->m_nDockBarID = 0;
    return pThis;
}
// Symbol: ??1CDockSite@@UEAA@XZ
// Retail 0x52f00, transcribed in full:
//   (vftable store)
//   while (m_lstDockBarRows.GetCount() != 0) {                        // +0x1F8
//       CDockingPanesRow* pRow = (CDockingPanesRow*)m_lstDockBarRows.RemoveHead();  // 0x230560
//       if (pRow != NULL) delete pRow;                                 // row vslot +0x08, flag 1
//   }
//   m_lstDockBarRows.~CObList();    // inlined: list vftable store + CPtrList::RemoveAll (0x83d0)
//   m_lstControlBars.~CObList();    // same, at +0x1A8
//   CBasePane::~CBasePane();                                           // tail-jump 0xaf30
// Retail keys the loop on the in-object count word; here it is the head node.
// The two vftable-store + `call 0x83d0` pairs at 0x52f63-0x52f78 inside 0x52f00
// (`mov %rsi,(%rdi)` for +0x1E0, then `mov %rsi,(%rcx)` for +0x1A8; %rsi holds
// the CObList vftable 0x180328538, mfc140) are the compiler-inlined member
// destructors (~CObList is `RemoveAll()` in MFC), so
// the members ARE destroyed in retail; here that is the exported CObList
// destructor thunk, which empties the list and releases the side-table
// storage the constructor thunk registered (see the file header).
extern "C" void MS_ABI impl___1CDockSite__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_CDockSite* s = Site(pThis);
    while (HeadNode(&s->m_lstDockBarRows) != nullptr) {
        CObject* pRow = impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(&s->m_lstDockBarRows);
        DeleteRow(pRow);
    }
    impl___1CObList__UEAA_XZ(&s->m_lstDockBarRows);
    impl___1CObList__UEAA_XZ(&s->m_lstControlBars);
    impl___1CBasePane__UEAA_XZ(pThis);
}
// Symbol: ?AddRow@CDockSite@@QEAAPEAVCDockingPanesRow@@PEAU__POSITION@@H@Z
// Retail 0x54080, transcribed in full:
//   int nOffset = 0;
//   POSITION posCur = m_lstDockBarRows.GetHeadPosition();
//   while (posCur != pos) {
//       if (posCur == NULL) AfxThrowInvalidArgException();            // 0x225b80
//       CDockingPanesRow* pRow = (CDockingPanesRow*)m_lstDockBarRows.GetNext(posCur);
//       if (pRow->IsVisible()) nOffset += pRow->m_nRowHeight;         // row vslot +0xc0, +0x10
//   }
//   ResizeDockSiteByOffset(nHeight, TRUE);                             // 0x54810
//   CDockingPanesRow* pNewRow = CreateRow(this, nOffset, nHeight);     // vslot +0x4f0
//   if (pNewRow == NULL) return NULL;
//   if (pos != NULL) OnInsertRow(m_lstDockBarRows.InsertBefore(pos, pNewRow));  // 0x230630, vslot +0x518
//   else             m_lstDockBarRows.AddTail(pNewRow);                // 0x230490 -- no OnInsertRow
//   return pNewRow;
// The AddTail branch really does skip OnInsertRow: nothing follows the new
// last row, so there is nothing to shift.
extern "C" void* MS_ABI impl__AddRow_CDockSite__QEAAPEAVCDockingPanesRow__PEAU__POSITION__H_Z(
    void* pThis, void* pos, int nHeight) {
    if (pThis == nullptr) return nullptr;
    S_CDockSite* s = Site(pThis);
    int nOffset = 0;
    for (ObNode* n = HeadNode(&s->m_lstDockBarRows); n != static_cast<ObNode*>(pos); n = n->pNext) {
        if (n == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return nullptr; }
        S_CDockingPanesRow* r = RowOf(n);
        if (RowIsVisible(r)) nOffset += r->m_nRowHeight;
    }
    impl__ResizeDockSiteByOffset_CDockSite__IEAAXHH_Z(pThis, nHeight, TRUE);
    void* pNewRow = impl__CreateRow_CDockSite__UEAAPEAVCDockingPanesRow__PEAV1_HH_Z(pThis, pThis, nOffset, nHeight);
    if (pNewRow == nullptr) return nullptr;
    if (pos != nullptr) {
        CObList::POSITION posIns = PosFromNode(pos);
        CObList::POSITION posNew = impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(
            &s->m_lstDockBarRows, &posIns, static_cast<CObject*>(pNewRow));
        impl__OnInsertRow_CDockSite__UEAAXPEAU__POSITION___Z(pThis, NodeFromPos(posNew));
    } else {
        impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(&s->m_lstDockBarRows,
                                                                    static_cast<CObject*>(pNewRow));
    }
    return pNewRow;
}

// Symbol: ?AlignDockSite@CDockSite@@QEAAXAEBVCRect@@AEAV2@H@Z
// Retail 0x531d0, transcribed in full:
//   if (::IsRectEmpty(&rectResult)) ::GetWindowRect(m_hWnd, &rectResult);
//   CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);
//   int nWidth = rectResult.Width(), nHeight = rectResult.Height();
//   switch (GetCurrentAlignment()) {                                  // vslot +0x338
//   case CBRS_ALIGN_LEFT:
//       rectResult.TopLeft() = rectToAlignBy.TopLeft();
//       rectResult.bottom = rectResult.top + rectToAlignBy.Height();
//       rectResult.right  = rectResult.left + nWidth;                  break;
//   case CBRS_ALIGN_TOP:
//       rectResult.TopLeft() = rectToAlignBy.TopLeft();
//       rectResult.right  = rectResult.left + rectToAlignBy.Width();
//       rectResult.bottom = rectResult.top + nHeight;                  break;
//   case CBRS_ALIGN_RIGHT:
//       rectResult.BottomRight() = rectToAlignBy.BottomRight();
//       rectResult.top  = rectResult.bottom - rectToAlignBy.Height();
//       rectResult.left = rectResult.right - nWidth;                   break;
//   case CBRS_ALIGN_BOTTOM:
//       rectResult.BottomRight() = rectToAlignBy.BottomRight();
//       rectResult.left = rectResult.right - rectToAlignBy.Width();
//       rectResult.top  = rectResult.bottom - nHeight;                 break;
//   }
//   if (!::EqualRect(&rectResult, &rectWnd) && bMoveImmediately) {
//       CRect rectNew = rectResult;
//       CWnd::FromHandle(::GetParent(m_hWnd))->ScreenToClient(&rectNew);   // 0x289180, 0x2a11f0
//       OnSetWindowPos(&CWnd::wndBottom, rectNew, SWP_NOZORDER|SWP_NOACTIVATE);  // vslot +0x570
//   }
// (0x1803bc1a0 is ?wndBottom@CWnd@@2V1@B; the flags literal is 0x14.)  The
// FromHandle result is not tested in retail; it is guarded here.
extern "C" void MS_ABI impl__AlignDockSite_CDockSite__QEAAXAEBVCRect__AEAV2_H_Z(
    void* pThis, const CRect* pRectToAlignBy, CRect* pRectResult, int bMoveImmediately) {
    if (pThis == nullptr || pRectToAlignBy == nullptr || pRectResult == nullptr) return;
    CBasePane* pSelf = AsPane(pThis);
    const CRect& rectToAlignBy = *pRectToAlignBy;
    CRect& rectResult = *pRectResult;

    if (::IsRectEmpty(RP(rectResult))) ::GetWindowRect(pSelf->m_hWnd, RP(rectResult));
    CRect rectWnd(0, 0, 0, 0);
    ::GetWindowRect(pSelf->m_hWnd, RP(&rectWnd));

    const int nWidth  = rectResult.right - rectResult.left;
    const int nHeight = rectResult.bottom - rectResult.top;
    switch (SiteAlignment(pThis)) {
    case kAlignLeft:
        rectResult.left = rectToAlignBy.left;
        rectResult.top = rectToAlignBy.top;
        rectResult.bottom = rectResult.top + (rectToAlignBy.bottom - rectToAlignBy.top);
        rectResult.right = rectResult.left + nWidth;
        break;
    case kAlignTop:
        rectResult.left = rectToAlignBy.left;
        rectResult.top = rectToAlignBy.top;
        rectResult.right = rectResult.left + (rectToAlignBy.right - rectToAlignBy.left);
        rectResult.bottom = rectResult.top + nHeight;
        break;
    case kAlignRight:
        rectResult.right = rectToAlignBy.right;
        rectResult.bottom = rectToAlignBy.bottom;
        rectResult.top = rectResult.bottom - (rectToAlignBy.bottom - rectToAlignBy.top);
        rectResult.left = rectResult.right - nWidth;
        break;
    case kAlignBottom:
        rectResult.right = rectToAlignBy.right;
        rectResult.bottom = rectToAlignBy.bottom;
        rectResult.left = rectResult.right - (rectToAlignBy.right - rectToAlignBy.left);
        rectResult.top = rectResult.bottom - nHeight;
        break;
    default:
        break;
    }

    if (!::EqualRect(RP(rectResult), RP(&rectWnd)) && bMoveImmediately) {
        CRect rectNew = rectResult;
        CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pSelf->m_hWnd));
        if (pParent != nullptr) impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, RP(&rectNew));
        impl__OnSetWindowPos_CDockSite__UEAAHPEBVCWnd__AEBVCRect__I_Z(
            pThis, reinterpret_cast<const CWnd*>(impl__wndBottom_CWnd__2V1_B), rectNew,
            SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

// Symbol: ?CalcFixedLayout@CDockSite@@UEAA?AVCSize@@HH@Z
// Retail 0x54930, transcribed in full (RCX = this, RDX = the CSize return
// slot, R8D = bStretch, R9D = bHorz):
//   BOOL bIsHorz = IsHorizontal();                                    // vslot +0x2d8
//   for (POSITION pos = m_lstDockBarRows.GetHeadPosition(); pos != NULL; ) {
//       CDockingPanesRow* pRow = (CDockingPanesRow*)m_lstDockBarRows.GetNext(pos);
//       if (!pRow->IsVisible()) continue;                              // row vslot +0xc0
//       int nRowHeight = pRow->m_nRowHeight;
//       CSize size = pRow->CalcFixedLayout(bStretch, bHorz);           // row vslot +0x40
//       int nNewHeight = bIsHorz ? size.cy : size.cx;
//       if (nNewHeight != nRowHeight && nNewHeight > 0)
//           ResizeRow(pRow, nNewHeight, FALSE);                        // 0x54210
//   }
//   CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);
//   return CSize(rectWnd.Width(), rectWnd.Height());
// Signature corrected: the generated stub had dropped both `this` and the
// hidden return slot.
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CDockSite__UEAA_AVCSize__HH_Z(
    void* pThis, CSize* pRet, int bStretch, int bHorz) {
    if (pRet == nullptr) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr) return pRet;
    S_CDockSite* s = Site(pThis);
    const bool bIsHorz = SiteIsHorz(pThis);
    for (ObNode* n = HeadNode(&s->m_lstDockBarRows); n != nullptr; n = n->pNext) {
        S_CDockingPanesRow* r = RowOf(n);
        if (!RowIsVisible(r)) continue;
        const int nRowHeight = r->m_nRowHeight;
        CSize size(0, 0);
        impl__CalcFixedLayout_CDockingPanesRow__UEAA_AVCSize__HH_Z(r, &size, bStretch, bHorz);
        const int nNewHeight = bIsHorz ? size.cy : size.cx;
        if (nNewHeight != nRowHeight && nNewHeight > 0)
            impl__ResizeRow_CDockSite__QEAAHPEAVCDockingPanesRow__HH_Z(pThis, r, nNewHeight, FALSE);
    }
    CRect rectWnd(0, 0, 0, 0);
    ::GetWindowRect(s->base.m_hWnd, RP(&rectWnd));
    pRet->cx = rectWnd.right - rectWnd.left;
    pRet->cy = rectWnd.bottom - rectWnd.top;
    return pRet;
}

// Symbol: ?CalcWindowRect@CDockSite@@MEAAXPEAUtagRECT@@I@Z
// Retail 0x537c0 is a single `jmp 0x28d240` = CWnd::CalcWindowRect, i.e. the
// override simply forwards to the base (the CBasePane layer adds nothing here).
// Signature corrected: the generated stub had dropped `this`.
extern "C" void MS_ABI impl__CalcWindowRect_CDockSite__MEAAXPEAUtagRECT__I_Z(
    void* pThis, RECT* lpClientRect, unsigned int nAdjustType) {
    impl__CalcWindowRect_CWnd__UEAAXPEAUtagRECT__I_Z(static_cast<CWnd*>(AsPane(pThis)), lpClientRect, nAdjustType);
}

// Symbol: ?CanAcceptPane@CDockSite@@UEBAHPEBVCBasePane@@@Z
// Retail 0x54c00 (mfc140; 0x54db0 mfc140u -- identical bytes), transcribed in full:
//   if (pBar == NULL) return FALSE;
//   return IsResizable() == 0;                    // THIS's vslot +0x300, not pBar's
// Read the listing carefully before "fixing" this: the vtable is loaded from
// RCX (`this`) and RCX is still `this` at the call, so the virtual is
// CDockSite::IsResizable on the dock site -- pBar is only tested for NULL.
// In the CDockSite vftable (mfc140 0x2e4388) slot +0x300 is 0x7260, a
// linker-folded `xor eax,eax; ret` = afxdocksite.h:44
// `virtual BOOL IsResizable() const { return FALSE; }`.  Devirtualised onto
// that: the answer is simply pBar != NULL (a derived dock site's override of
// IsResizable is not reached).
extern "C" int MS_ABI impl__CanAcceptPane_CDockSite__UEBAHPEBVCBasePane___Z(const void* pThis, const CBasePane* pBar) {
    (void)pThis;
    return pBar != nullptr ? TRUE : FALSE;
}

// Symbol: ?CreateRow@CDockSite@@UEAAPEAVCDockingPanesRow@@PEAV1@HH@Z
// Retail 0x53ff0, transcribed in full:
//   void* p = ::operator new(0x70);                                   // 0x2840
//   CDockingPanesRow* pRow = p ? new (p) CDockingPanesRow(pParentDockBar, nOffset, nRowHeight) : NULL;  // 0x4f3f0
//   if (!pRow->Create()) { delete pRow; return NULL; }                // row vslot +0x28 / +0x08
//   return pRow;
// Retail dereferences pRow for the Create() call without a NULL test (operator
// new throws in MFC); the allocation is tested here instead.  Row vslot +0x28 is
// the afxdockingpanesrow.h inline `virtual BOOL Create() { return TRUE; }`
// (0x3ae0), so the failure branch is unreachable for a real CDockingPanesRow
// and is not reproduced.
// DEVIATION, and the reason it is safe to say so: the exported row constructor
// impl___0CDockingPanesRow__QEAA_PEAVCDockSite__HH_Z is still a `return pThis`
// stub (CDockingPanesRow.cpp:325 as of this writing) that initialises nothing.
// This body therefore zero-fills the block first and, when the constructor
// thunk comes back without having stored pParentDockBar at +0x68, performs the
// stores the retail constructor (0x4f3f0) makes itself:
//   m_nRowHeight = nRowHeight; m_nRowOffset = nOffset;
//   CObList m_lstControlBars(10); m_pParentDockBar = pParentDockBar;
//   m_dwRowAlignment = pParentDockBar->GetCurrentAlignment();          // vslot +0x338
//   m_nRowSavedHeight = m_nMinHeight = m_nExtraSpace = m_nExtraAlignment = 0;
//   m_bVisible = TRUE; m_bIgnoreBarVisibility = FALSE;
// Once that constructor is real the fallback is skipped automatically; it can
// then be deleted (see headerRequests).
extern "C" void* MS_ABI impl__CreateRow_CDockSite__UEAAPEAVCDockingPanesRow__PEAV1_HH_Z(
    void* pThis, void* pParentDockBar, int nOffset, int nRowHeight) {
    (void)pThis;
    void* p = impl___2_YAPEAX_K_Z(sizeof(S_CDockingPanesRow));
    if (p == nullptr) return nullptr;
    std::memset(p, 0, sizeof(S_CDockingPanesRow));
    impl___0CDockingPanesRow__QEAA_PEAVCDockSite__HH_Z(p, pParentDockBar, nOffset, nRowHeight);

    S_CDockingPanesRow* r = RowOf(p);
    if (r->m_pParentDockBar != pParentDockBar) {
        // Constructor thunk is still a stub -- apply the retail constructor's stores.
        r->m_nRowHeight = nRowHeight;
        r->m_nRowOffset = nOffset;
        impl___0CObList__QEAA__J_Z(&r->m_lstControlBars, 10);
        r->m_pParentDockBar = static_cast<CWnd*>(AsPane(pParentDockBar));
        r->m_dwRowAlignment = pParentDockBar != nullptr ? static_cast<DWORD>(SiteAlignment(pParentDockBar)) : 0;
        r->m_nRowSavedHeight = 0;
        r->m_nMinHeight = 0;
        r->m_nExtraSpace = 0;
        r->m_nExtraAlignment = 0;
        r->m_bVisible = TRUE;
        r->m_bIgnoreBarVisibility = FALSE;
    }
    return p;
}

// Symbol: ?FindNextVisibleRow@CDockSite@@IEAAPEAVCDockingPanesRow@@AEAPEAU__POSITION@@H@Z
// Retail 0x53710, transcribed in full:
//   if (m_lstDockBarRows.GetCount() == 0) { pos = NULL; return NULL; }   // +0x1F8
//   if (pos == NULL) pos = bForward ? m_lstDockBarRows.GetHeadPosition()  // +0x1E8
//                                   : m_lstDockBarRows.GetTailPosition(); // +0x1F0
//   else             pos = bForward ? pos->pNext : pos->pPrev;
//   while (pos != NULL) {
//       POSITION posRow = pos;
//       CDockingPanesRow* pRow = bForward ? (CDockingPanesRow*)m_lstDockBarRows.GetNext(pos)
//                                         : (CDockingPanesRow*)m_lstDockBarRows.GetPrev(pos);
//       if (pRow->IsVisible()) { pos = posRow; return pRow; }         // row vslot +0xc0
//   }
//   return NULL;
// So the search starts strictly AFTER (or before) the incoming position, and
// on success `pos` is left pointing AT the row returned, not past it.  (The
// `sbb/and $-8` sequence at 0x53748 selects +0x1E8 or +0x1F0 from bForward;
// the AfxThrowInvalidArgException at 0x537b1 guards GetNext on a NULL node and
// is unreachable inside the loop.)
extern "C" void* MS_ABI impl__FindNextVisibleRow_CDockSite__IEAAPEAVCDockingPanesRow__AEAPEAU__POSITION__H_Z(
    void* pThis, void** pPos, int bForward) {
    if (pThis == nullptr || pPos == nullptr) return nullptr;
    const CObList* pList = &Site(pThis)->m_lstDockBarRows;
    ObNode* head = HeadNode(pList);
    if (head == nullptr) { *pPos = nullptr; return nullptr; }

    ObNode* n = static_cast<ObNode*>(*pPos);
    if (n == nullptr) n = bForward ? head : TailNode(pList);
    else              n = bForward ? n->pNext : n->pPrev;
    *pPos = n;
    while (n != nullptr) {
        ObNode* nRow = n;
        n = bForward ? n->pNext : n->pPrev;
        *pPos = n;
        if (RowIsVisible(RowOf(nRow))) { *pPos = nRow; return nRow->data; }
    }
    return nullptr;
}

// Symbol: ?FindPaneByID@CDockSite@@QEAAPEAVCPane@@I@Z
// Retail 0x55130, transcribed in full:
//   for (POSITION pos = m_lstControlBars.GetHeadPosition(); pos != NULL; ) {
//       CPane* pBar = (CPane*)m_lstControlBars.GetNext(pos);
//       if ((UINT)pBar->GetDlgCtrlID() == nID) return pBar;           // 0x2a78b0
//       CMFCReBar* pReBar = DYNAMIC_DOWNCAST(CMFCReBar, pBar);         // 0x233310, RTTI 0x1802fb808
//       if (pReBar != NULL) {
//           CPane* pChild = DYNAMIC_DOWNCAST(CPane, pReBar->GetDlgItem(nID));  // 0x2a72a0, RTTI 0x1802f3868
//           if (pChild != NULL) return pChild;
//       }
//   }
//   return NULL;
// Signature corrected: the generated stub had dropped `this`.
extern "C" void* MS_ABI impl__FindPaneByID_CDockSite__QEAAPEAVCPane__I_Z(void* pThis, unsigned int nID) {
    if (pThis == nullptr) return nullptr;
    for (ObNode* n = HeadNode(&Site(pThis)->m_lstControlBars); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (static_cast<unsigned int>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(pBar)) == nID) return pBar;
        if (pBar != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pBar, impl__GetThisClass_CMFCReBar__SAPEAUCRuntimeClass__XZ())) {
            CWnd* pChild = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pBar, static_cast<int>(nID));
            if (pChild != nullptr &&
                impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pChild, impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ()))
                return pChild;
        }
    }
    return nullptr;
}

// Symbol: ?FindRowByRect@CDockSite@@IEAAPEAVCDockingPanesRow@@VCRect@@@Z
// Retail 0x53c10, transcribed in full (the by-value CRect arrives by address in RDX):
//   CPoint pt = rectRow.TopLeft();          // the first 8 bytes of the rect
//   ::ClientToScreen(m_hWnd, &pt);           // import 0x1802c5270
//   bool bOuterRow;
//   return RowFromPoint(pt, bOuterRow);      // 0x54670
// Signature corrected: the generated stub had dropped `this`.
extern "C" void* MS_ABI impl__FindRowByRect_CDockSite__IEAAPEAVCDockingPanesRow__VCRect___Z(void* pThis, const CRect* pRectRow) {
    if (pThis == nullptr || pRectRow == nullptr) return nullptr;
    POINT pt = { pRectRow->left, pRectRow->top };
    ::ClientToScreen(AsPane(pThis)->m_hWnd, &pt);
    bool bOuterRow = false;
    const long long packed = static_cast<long long>(
        (static_cast<unsigned long long>(static_cast<unsigned int>(pt.y)) << 32) | static_cast<unsigned int>(pt.x));
    return impl__RowFromPoint_CDockSite__IEBAPEAVCDockingPanesRow__VCPoint__AEA_N_Z(pThis, packed, &bOuterRow);
}

// Symbol: ?FindRowIndex@CDockSite@@QEAAHPEAVCDockingPanesRow@@@Z
// Retail 0x55010, transcribed in full:
//   if (pRow == NULL) return 0;
//   int nIndex = 0;
//   for (POSITION pos = m_lstDockBarRows.GetHeadPosition(); pos != NULL; nIndex++)
//       if (m_lstDockBarRows.GetNext(pos) == pRow) return nIndex;
//   return 0;            // not found answers 0, the same as "first row"
// Signature corrected: the generated stub had dropped `this`.
extern "C" int MS_ABI impl__FindRowIndex_CDockSite__QEAAHPEAVCDockingPanesRow___Z(void* pThis, void* pRow) {
    if (pThis == nullptr || pRow == nullptr) return 0;
    int nIndex = 0;
    for (ObNode* n = HeadNode(&Site(pThis)->m_lstDockBarRows); n != nullptr; n = n->pNext, ++nIndex)
        if (n->data == pRow) return nIndex;
    return 0;
}

// Symbol: ?IsDragMode@CDockSite@@UEBAHXZ
// Retail 0x550c0, transcribed in full:
//   for (POSITION pos = m_lstControlBars.GetHeadPosition(); pos != NULL; ) {
//       CPane* pBar = DYNAMIC_DOWNCAST(CPane, m_lstControlBars.GetNext(pos));  // RTTI 0x1802f3868
//       if (pBar != NULL && pBar->IsDragMode()) return TRUE;         // pBar vslot +0x4e8
//   }
//   return FALSE;
// CPane::IsDragMode is the afxpane.h:75 inline `return m_bDragMode;` (+0x214)
// and has no export; it is read directly (a derived override is not reached).
// Signature corrected: the generated stub had dropped `this`.
extern "C" int MS_ABI impl__IsDragMode_CDockSite__UEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return FALSE;
    for (ObNode* n = HeadNode(&Site(pThis)->m_lstControlBars); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr) continue;
        if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pBar, impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ()))
            continue;
        if (pBar->m_bDragMode) return TRUE;
    }
    return FALSE;
}

// Symbol: ?IsLastRow@CDockSite@@QEBA_NPEAVCDockingPanesRow@@@Z
// Retail 0x54900, transcribed in full:
//   if (m_lstDockBarRows.GetCount() == 0) return false;               // +0x1F8
//   return m_lstDockBarRows.GetHead() == pRow || m_lstDockBarRows.GetTail() == pRow;
// (Yes: the FIRST row answers true as well -- retail tests both ends.)
// Signature corrected: the generated stub had dropped `this`.
extern "C" bool MS_ABI impl__IsLastRow_CDockSite__QEBA_NPEAVCDockingPanesRow___Z(const void* pThis, void* pRow) {
    if (pThis == nullptr) return false;
    const CObList* pList = &Site(pThis)->m_lstDockBarRows;
    ObNode* head = HeadNode(pList);
    if (head == nullptr) return false;
    return head->data == pRow || TailNode(pList)->data == pRow;
}

// Symbol: ?IsRectWithinDockSite@CDockSite@@QEAAHVCRect@@AEAVCPoint@@@Z
// Retail 0x54b40, transcribed in full (by-value CRect arrives by address in RDX):
//   CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);
//   ptDelta = CPoint(0, 0);
//   if (IsHorizontal()) {                                             // vslot +0x2d8
//       if (rect.left  < rectWnd.left)  { ptDelta.x = rectWnd.left  - rect.left;  return FALSE; }
//       if (rect.right > rectWnd.right) { ptDelta.x = rectWnd.right - rect.right; return FALSE; }
//   } else {
//       if (rect.top    < rectWnd.top)    { ptDelta.y = rectWnd.top - rect.top;      return FALSE; }
//       if (rect.bottom > rectWnd.bottom) { ptDelta.y = rect.bottom - rectWnd.bottom; return FALSE; }
//   }
//   return TRUE;
// Note the asymmetry retail really has: the right-edge overflow is reported
// as a NEGATIVE x delta (rectWnd.right - rect.right), the bottom-edge overflow
// as a POSITIVE y delta (rect.bottom - rectWnd.bottom).
// Signature corrected: the generated stub had dropped `this`.
extern "C" int MS_ABI impl__IsRectWithinDockSite_CDockSite__QEAAHVCRect__AEAVCPoint___Z(
    void* pThis, const CRect* pRect, CPoint* pPtDelta) {
    if (pThis == nullptr || pRect == nullptr || pPtDelta == nullptr) return FALSE;
    CRect rectWnd(0, 0, 0, 0);
    ::GetWindowRect(AsPane(pThis)->m_hWnd, RP(&rectWnd));
    pPtDelta->x = 0;
    pPtDelta->y = 0;
    if (SiteIsHorz(pThis)) {
        if (pRect->left < rectWnd.left)   { pPtDelta->x = rectWnd.left - pRect->left;   return FALSE; }
        if (pRect->right > rectWnd.right) { pPtDelta->x = rectWnd.right - pRect->right; return FALSE; }
    } else {
        if (pRect->top < rectWnd.top)       { pPtDelta->y = rectWnd.top - pRect->top;       return FALSE; }
        if (pRect->bottom > rectWnd.bottom) { pPtDelta->y = pRect->bottom - rectWnd.bottom; return FALSE; }
    }
    return TRUE;
}

// Symbol: ?MovePane@CDockSite@@UEAAHPEAVCPane@@IVCPoint@@@Z
// Retail 0x53370, transcribed in full (by-value CPoint packed in R9):
//   CDockingPanesRow* pRow = pWnd->m_pDockBarRow;                      // +0x130
//   CRect rectVirtual; pWnd->GetVirtualRect(rectVirtual);              // 0xa1ca0
//   ::OffsetRect(&rectVirtual, ptOffset.x, ptOffset.y);                // import 0x1802c5318
//   CPoint ptMouse(0, 0); ::GetCursorPos(&ptMouse);                    // import 0x1802c5348 (unused afterwards)
//   CRect rectRow; pRow->GetWindowRect(rectRow);                       // 0x4fcf0
//   CBasePane* pTargetBar = NULL;
//   if (pWnd->IsChangeState(15, &pTargetBar) != CS_NOTHING) {          // pWnd vslot +0x578
//       pWnd->UpdateVirtualRect(ptOffset);                             // 0xa1ba0
//       pWnd->GetVirtualRect(rectVirtual);
//       pWnd->FloatPane(rectVirtual, DM_MOUSE, true);                  // pWnd vslot +0x408
//       return TRUE;
//   }
//   CPoint ptCenter = rectVirtual.CenterPoint();
//   bool bOuterRow = false;
//   CDockingPanesRow* pTargetRow = RowFromPoint(ptCenter, bOuterRow);  // 0x54670
//   int nDelta, nSensitivity;
//   if (IsHorizontal()) {                                              // vslot +0x2d8
//       nDelta = min(rectRow.top - rectVirtual.top, rectRow.bottom - rectVirtual.bottom);
//       nSensitivity = 2 * rectVirtual.Height() / 3;
//   } else {
//       nDelta = min(rectRow.left - rectVirtual.left, rectRow.right - rectVirtual.right);
//       nSensitivity = 2 * rectVirtual.Width() / 3;
//   }
//   if (abs(nDelta) > nSensitivity) {
//       if (pRow->GetPaneCount() > 1 && nDelta < pRow->m_nRowHeight) {   // +0x40, +0x10
//           POSITION pos = m_lstDockBarRows.Find(pRow);                // open-coded walk
//           ENSURE(pos != NULL);                                       // 0x225b80
//           if (nDelta < 0) FindNextVisibleRow(pos, TRUE);             // 0x53710
//           pRow->RemovePane(pWnd);                                    // row vslot +0x58
//           int nRowHeight = IsHorizontal() ? rectVirtual.Height() : rectVirtual.Width();
//           CDockingPanesRow* pNewRow = AddRow(pos, nRowHeight);       // 0x54080
//           pNewRow->AddPaneFromRow(pWnd, DM_MOUSE);                   // row vslot +0x50
//           return FALSE;
//       }
//       if (pTargetRow != pRow && pTargetRow != NULL) {
//           ::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);             // import 0x1802c5378
//           if (pRow->IsExclusiveRow())            SwapRows(pTargetRow, pRow);   // row vslot +0xd0, 0x54520
//           else if (pTargetRow->IsExclusiveRow()) SwapRows(pRow, pTargetRow);
//           else { pRow->RemovePane(pWnd); pTargetRow->AddPaneFromRow(pWnd, DM_MOUSE); }
//           pWnd->m_bDisableMove = true;                               // byte +0x1A8
//           ::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
//           ::RedrawWindow(m_hWnd, NULL, NULL, 0x185);                 // import 0x1802c5388
//           return FALSE;
//       }
//   }
//   if (abs(nDelta) < rectRow.Height()) {
//       HDWP hdwp = ::BeginDeferWindowPos(pRow->GetPaneCount());       // import 0x1802c4d28
//       pRow->MovePane(pWnd, ptOffset, TRUE, hdwp);                    // 0x50300
//       ::EndDeferWindowPos(hdwp);                                     // import 0x1802c4d30
//   }
//   return FALSE;
// Two things to know before editing: (1) the `/ 3` is the signed
// multiply-by-0x55555556 idiom at 0x534e3; (2) the HDWP& retail hands to
// CDockingPanesRow::MovePane (the `lea -0x29(%rbp),%rax; mov %rax,0x20(%rsp)`
// at 0x536c1 inside MovePane (0x53370)) points at the stack slot that last held
// nSensitivity, not at the hdwp in RDI that ::EndDeferWindowPos receives.  The
// row's MovePane (0x50300) only forwards that pointer to CheckPanes, and the
// leaf overloads never dereference it (CDockingPanesRow.cpp:1361/1387/1482 are
// `(void)phdwp`, "never used, exactly as in retail"), which is what let the
// LTCG build pass a dead address; the source-level shape above is what is
// reproduced.  0x185 is RDW_INVALIDATE|RDW_ERASE|
// RDW_ALLCHILDREN|RDW_UPDATENOW.  `dockMethod` is written as the literal 1
// (DM_MOUSE, afxbasepane.h:42) because this tree does not model the enum.
// m_bDisableMove is a `bool` in retail (afxpane.h:111); include/openmfc/afxmfc.h
// models it as a BOOL, so the store below also zeroes the three retail pad
// bytes after it.
// Both CPane virtuals are devirtualised onto the CPane thunks, which are still
// stubs (IsChangeState answers CS_NOTHING, FloatPane answers FALSE), so the
// float branch is never taken at present; it becomes live when they are.
extern "C" int MS_ABI impl__MovePane_CDockSite__UEAAHPEAVCPane__IVCPoint___Z(
    void* pThis, CPane* pWnd, unsigned int nFlags, long long ptOffset) {
    (void)nFlags;
    if (pThis == nullptr || pWnd == nullptr) return FALSE;
    CBasePane* pSelf = AsPane(pThis);
    S_CDockingPanesRow* pRow = RowOf(pWnd->m_pDockBarRow);
    if (pRow == nullptr) return FALSE;   // retail would fault on GetWindowRect; guarded

    CRect rectVirtual(0, 0, 0, 0);
    impl__GetVirtualRect_CPane__QEBAXAEAVCRect___Z(pWnd, RP(&rectVirtual));
    ::OffsetRect(RP(&rectVirtual), PtX(ptOffset), PtY(ptOffset));
    POINT ptMouse = { 0, 0 };
    ::GetCursorPos(&ptMouse);
    CRect rectRow(0, 0, 0, 0);
    impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(pRow, &rectRow);

    CBasePane* pTargetBar = nullptr;
    if (impl__IsChangeState_CPane__UEBA_AW4AFX_CS_STATUS__HPEAPEAVCBasePane___Z(pWnd, 15, &pTargetBar) != 0) {
        impl__UpdateVirtualRect_CPane__QEAAXVCPoint___Z(pWnd, PackedPtr(ptOffset));
        impl__GetVirtualRect_CPane__QEBAXAEAVCRect___Z(pWnd, RP(&rectVirtual));
        impl__FloatPane_CPane__UEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(pWnd, RP(&rectVirtual), 1 /*DM_MOUSE*/, true);
        return TRUE;
    }

    const int cx = (rectVirtual.left + rectVirtual.right) / 2;
    const int cy = (rectVirtual.top + rectVirtual.bottom) / 2;
    const long long ptCenter = static_cast<long long>(
        (static_cast<unsigned long long>(static_cast<unsigned int>(cy)) << 32) | static_cast<unsigned int>(cx));
    bool bOuterRow = false;
    S_CDockingPanesRow* pTargetRow = RowOf(
        impl__RowFromPoint_CDockSite__IEBAPEAVCDockingPanesRow__VCPoint__AEA_N_Z(pThis, ptCenter, &bOuterRow));

    const bool bHorz = SiteIsHorz(pThis);
    int nDelta, nSensitivity;
    if (bHorz) {
        const int a = rectRow.top - rectVirtual.top;
        const int b = rectRow.bottom - rectVirtual.bottom;
        nDelta = b < a ? b : a;
        nSensitivity = (2 * (rectVirtual.bottom - rectVirtual.top)) / 3;
    } else {
        const int a = rectRow.left - rectVirtual.left;
        const int b = rectRow.right - rectVirtual.right;
        nDelta = b < a ? b : a;
        nSensitivity = (2 * (rectVirtual.right - rectVirtual.left)) / 3;
    }

    if (AbsInt(nDelta) > nSensitivity) {
        if (ListCount(&pRow->m_lstControlBars) > 1 && nDelta < pRow->m_nRowHeight) {
            void* pos = FindNode(&Site(pThis)->m_lstDockBarRows, pRow);
            if (pos == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
            if (nDelta < 0)
                impl__FindNextVisibleRow_CDockSite__IEAAPEAVCDockingPanesRow__AEAPEAU__POSITION__H_Z(pThis, &pos, TRUE);
            impl__RemovePane_CDockingPanesRow__UEAAXPEAVCPane___Z(pRow, pWnd);
            const int nRowHeight = bHorz ? (rectVirtual.bottom - rectVirtual.top)
                                         : (rectVirtual.right - rectVirtual.left);
            void* pNewRow = impl__AddRow_CDockSite__QEAAPEAVCDockingPanesRow__PEAU__POSITION__H_Z(pThis, pos, nRowHeight);
            if (pNewRow != nullptr)   // retail dereferences it unconditionally
                impl__AddPaneFromRow_CDockingPanesRow__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(pNewRow, pWnd, 1 /*DM_MOUSE*/);
            return FALSE;
        }
        if (pTargetRow != pRow && pTargetRow != nullptr) {
            ::SendMessage(pSelf->m_hWnd, WM_SETREDRAW, FALSE, 0);
            if (impl__IsExclusiveRow_CDockingPanesRow__UEBAHXZ(pRow)) {
                impl__SwapRows_CDockSite__QEAAXPEAVCDockingPanesRow__0_Z(pThis, pTargetRow, pRow);
            } else if (impl__IsExclusiveRow_CDockingPanesRow__UEBAHXZ(pTargetRow)) {
                impl__SwapRows_CDockSite__QEAAXPEAVCDockingPanesRow__0_Z(pThis, pRow, pTargetRow);
            } else {
                impl__RemovePane_CDockingPanesRow__UEAAXPEAVCPane___Z(pRow, pWnd);
                impl__AddPaneFromRow_CDockingPanesRow__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(pTargetRow, pWnd, 1 /*DM_MOUSE*/);
            }
            pWnd->m_bDisableMove = TRUE;
            ::SendMessage(pSelf->m_hWnd, WM_SETREDRAW, TRUE, 0);
            ::RedrawWindow(pSelf->m_hWnd, nullptr, nullptr,
                           RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW);
            return FALSE;
        }
    }

    if (AbsInt(nDelta) < rectRow.bottom - rectRow.top) {
        HDWP hdwp = ::BeginDeferWindowPos(ListCount(&pRow->m_lstControlBars));
        void* hdwpRef = hdwp;
        impl__MovePane_CDockingPanesRow__QEAAXPEAVCPane__VCPoint__HAEAPEAX_Z(pRow, pWnd, ptOffset, TRUE, &hdwpRef);
        ::EndDeferWindowPos(hdwp);
    }
    return FALSE;
}

// Symbol: ?OnContextMenu@CDockSite@@IEAAXPEAVCWnd@@VCPoint@@@Z
// Retail 0x55040, transcribed in full (pWnd is never read; CPoint packed in R8):
//   if (CMFCPopupMenu::m_pActivePopupMenu != NULL) return;            // 0x3b6fe8
//   if (CMFCToolBar::m_bCustomizeMode) return;                        // 0x3b70bc
//   if (IsDragMode()) return;                                         // vslot +0x4e8
//   CFrameWnd* pFrame = g_pTopLevelFrame;                             // 0x3b6f18
//   if (pFrame == NULL) { pFrame = GetTopLevelFrame(); if (pFrame == NULL) return; }  // 0x28c910
//   OnPaneContextMenu(pFrame, point);                                 // vslot +0x4a0
// (The g_pTopLevelFrame-else-GetTopLevelFrame pair is MFC's AFXGetTopLevelFrame
// helper inlined.)  Both virtuals are devirtualised onto this file's IsDragMode
// and the CBasePane::OnPaneContextMenu thunk.
extern "C" void MS_ABI impl__OnContextMenu_CDockSite__IEAAXPEAVCWnd__VCPoint___Z(void* pThis, CWnd* pWnd, long long point) {
    (void)pWnd;
    if (pThis == nullptr) return;
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA != nullptr) return;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return;
    if (impl__IsDragMode_CDockSite__UEBAHXZ(pThis)) return;
    CWnd* pFrame = static_cast<CWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
    if (pFrame == nullptr) {
        pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(AsPane(pThis));
        if (pFrame == nullptr) return;
    }
    impl__OnPaneContextMenu_CBasePane__UEAAXPEAVCWnd__VCPoint___Z(AsPane(pThis), pFrame, point);
}

// Symbol: ?OnDestroy@CDockSite@@IEAAXXZ
// Retail 0x551c0, transcribed in full:
//   RemovePaneFromDockManager(this, FALSE, FALSE, FALSE, NULL);       // 0xb8a0
//   CWnd::OnDestroy();                                                // tail-jump 0x289cc0
// The CBasePane::RemovePaneFromDockManager thunk is still a stub in
// CBasePane.cpp; the call is made with the retail argument shape regardless.
// Signature corrected: the generated stub had dropped `this`.
extern "C" void MS_ABI impl__OnDestroy_CDockSite__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__RemovePaneFromDockManager_CBasePane__QEAAXPEAV1_HHH0_Z(AsPane(pThis), AsPane(pThis), FALSE, FALSE, FALSE, nullptr);
    impl__OnDestroy_CWnd__IEAAXXZ(AsPane(pThis));
}

// Symbol: ?OnEraseBkgnd@CDockSite@@IEAAHPEAVCDC@@@Z
// Retail 0x54de0 (mfc140; the export has no mfc140u RVA in the map and was
// located through the CDockSite message map's WM_ERASEBKGND entry), transcribed:
//   CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);           // import 0x1802c5358
//   CMFCVisualManager::GetInstance()->OnFillBarBackground(            // 0x97f4 (inline GetInstance), vslot +0x78
//       pDC, this, rectClient, rectClient, FALSE);                    // the same rect twice; bNCArea = 0
//   return TRUE;
// Deviation, shared with CPaneDivider::OnPaint and CMFCToolBar::DrawSeparator:
// GetInstance (0x97f4 reads m_pVisManager and otherwise creates the default
// manager from m_pRTIDefault) exists in OpenMFC only as the C++ static
// CMFCVisualManager::GetInstance() (visualmanager/CMFCVisualManager.cpp), which
// this TU may not call; the exported mirror of m_pVisManager is read instead,
// so the fill happens only when a manager already exists and is skipped (with
// TRUE still returned) otherwise.  See headerRequests for the GetInstance thunk
// that would close that gap.
// Signature corrected: the generated stub had dropped `this`.
extern "C" int MS_ABI impl__OnEraseBkgnd_CDockSite__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    if (pThis == nullptr) return TRUE;
    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(AsPane(pThis)->m_hWnd, &rectClient);
    CMFCVisualManager* pVM = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (pVM != nullptr) {   // deviation: retail's GetInstance() creates the default manager here
        impl__OnFillBarBackground_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
            pVM, pDC, AsPane(pThis), CRect(rectClient), CRect(rectClient), FALSE);
    }
    return TRUE;
}

// Symbol: ?OnNcDestroy@CDockSite@@IEAAXXZ
// Retail: the CDockSite message map (0x1802e4288, entry for WM_NCDESTROY 0x82)
// points at 0x3d590, a body the linker folded with CMFCDesktopAlertWnd::OnNcDestroy:
//   CWnd::OnNcDestroy();                                              // 0x289d80
//   if (this != NULL) delete this;                                    // vslot +0x08, flag 1
// i.e. the dock site destroys itself when its window goes away.  The scalar
// deleting destructor (~CDockSite, then ??3@YAXPEAX@Z) is spelled out below
// through this file's own destructor thunk and the memcore operator delete;
// with no retail vftable a derived class's destructor cannot be reached.
// Signature corrected: the generated stub had dropped `this`.
extern "C" void MS_ABI impl__OnNcDestroy_CDockSite__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__OnNcDestroy_CWnd__IEAAXXZ(AsPane(pThis));
    impl___1CDockSite__UEAA_XZ(pThis);
    impl___3_YAXPEAX_Z(pThis);
}

// Symbol: ?OnPaint@CDockSite@@IEAAXXZ
// Retail 0x41000, transcribed in full: `CPaintDC dc(this);` and nothing else
// (0x2a1c60 / 0x2a1d10 are the CPaintDC constructor and destructor) -- the
// paint is validated without drawing; DoPaint is not called.
// Signature corrected: the generated stub had dropped `this`.
extern "C" void MS_ABI impl__OnPaint_CDockSite__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, static_cast<CWnd*>(AsPane(pThis)));
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// Symbol: ?OnResizeRow@CDockSite@@UEAAHPEAVCDockingPanesRow@@H@Z
// Retail 0x54480, transcribed in full:
//   int nActualOffset = pRowToResize->Resize(nOffset);                // row vslot +0x78
//   if (!pRowToResize->IsVisible()) return 0;                         // row vslot +0xc0 -- returns 0, NOT nActualOffset
//   POSITION pos = m_lstDockBarRows.Find(pRowToResize);               // open-coded walk
//   ENSURE(pos != NULL);                                              // 0x225b80
//   m_lstDockBarRows.GetNext(pos);
//   while (pos != NULL)
//       ((CDockingPanesRow*)m_lstDockBarRows.GetNext(pos))->Move(nActualOffset);  // row vslot +0x80
//   return nActualOffset;
// Signature corrected: the generated stub had dropped `this`.
extern "C" int MS_ABI impl__OnResizeRow_CDockSite__UEAAHPEAVCDockingPanesRow__H_Z(void* pThis, void* pRow, int nOffset) {
    if (pThis == nullptr || pRow == nullptr) return 0;
    const int nActualOffset = impl__Resize_CDockingPanesRow__UEAAHH_Z(pRow, nOffset);
    if (!RowIsVisible(RowOf(pRow))) return 0;
    ObNode* n = FindNode(&Site(pThis)->m_lstDockBarRows, pRow);
    if (n == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return 0; }
    for (n = n->pNext; n != nullptr; n = n->pNext)
        impl__Move_CDockingPanesRow__UEAAXH_Z(n->data, nActualOffset);
    return nActualOffset;
}

// Symbol: ?OnShowRow@CDockSite@@UEAAXPEAU__POSITION@@H@Z
// Retail 0x54430, transcribed in full:
//   if (pos == NULL) AfxThrowInvalidArgException();                   // 0x225b80
//   if (bShow) OnInsertRow(pos);                                      // tail-jump vslot +0x518
//   else       OnRemoveRow(pos, TRUE);                                // tail-jump vslot +0x520
// Signature corrected: the generated stub had dropped `this`.
extern "C" void MS_ABI impl__OnShowRow_CDockSite__UEAAXPEAU__POSITION__H_Z(void* pThis, void* pos, int bShow) {
    if (pThis == nullptr) return;
    if (pos == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    if (bShow) impl__OnInsertRow_CDockSite__UEAAXPEAU__POSITION___Z(pThis, pos);
    else       impl__OnRemoveRow_CDockSite__UEAAXPEAU__POSITION__H_Z(pThis, pos, TRUE);
}

// Symbol: ?OnSize@CDockSite@@IEAAXIHH@Z
// Retail: the CDockSite message map entry for WM_SIZE points at 0xdab0, which
// is a single `jmp 0x289090` = CWnd::Default() -- the handler ignores its
// arguments and lets DefWindowProc handle WM_SIZE.  (0xdab0 is a linker-folded
// body shared with several other classes' handlers.)
// Signature corrected: the generated stub had dropped `this`.
extern "C" void MS_ABI impl__OnSize_CDockSite__IEAAXIHH_Z(void* pThis, unsigned int nType, int cx, int cy) {
    (void)nType;
    (void)cx;
    (void)cy;
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsPane(pThis));
}

// Symbol: ?PaneFromPoint@CDockSite@@UEAAPEAVCPane@@VCPoint@@@Z
// Retail 0x54c30, transcribed in full (CPoint packed in RDX):
//   for (POSITION pos = m_lstControlBars.GetHeadPosition(); pos != NULL; ) {
//       CPane* pBar = (CPane*)m_lstControlBars.GetNext(pos);
//       CRect rectBar; ::GetWindowRect(pBar->m_hWnd, &rectBar);         // import 0x1802c5370
//       if (::PtInRect(&rectBar, pt)) return pBar;                       // import 0x1802c5320
//   }
//   return NULL;
// Signature corrected: the generated stub had dropped `this`.
extern "C" void* MS_ABI impl__PaneFromPoint_CDockSite__UEAAPEAVCPane__VCPoint___Z(void* pThis, long long pt) {
    if (pThis == nullptr) return nullptr;
    const POINT point = { PtX(pt), PtY(pt) };
    for (ObNode* n = HeadNode(&Site(pThis)->m_lstControlBars); n != nullptr; n = n->pNext) {
        CPane* pBar = PaneOf(n);
        if (pBar == nullptr) continue;   // retail would fault reading m_hWnd; guarded
        RECT rectBar = { 0, 0, 0, 0 };
        ::GetWindowRect(pBar->m_hWnd, &rectBar);
        if (::PtInRect(&rectBar, point)) return pBar;
    }
    return nullptr;
}

// Symbol: ?RectSideFromPoint@CDockSite@@SAHAEBVCRect@@AEBVCPoint@@@Z
// Retail 0x54cc0, transcribed in full (a static; RCX = &rect, RDX = &point):
//   int dL = point.x - rect.left,  dR = rect.right  - point.x;
//   int dT = point.y - rect.top,   dB = rect.bottom - point.y;
//   int nH = dL <= dR ? HTLEFT : HTRIGHT;   int dH = min(dL, dR);
//   int nV = dT <= dB ? HTTOP  : HTBOTTOM;  int dV = min(dT, dB);
//   return dH <= dV ? nH : nV;
// i.e. the hit-test code of the rectangle edge nearest the point, horizontal
// edges winning ties.  The four literals in the listing are 0xa/0xb/0xc/0xf =
// HTLEFT/HTRIGHT/HTTOP/HTBOTTOM.
extern "C" int MS_ABI impl__RectSideFromPoint_CDockSite__SAHAEBVCRect__AEBVCPoint___Z(const CRect* pRect, const CPoint* pPoint) {
    if (pRect == nullptr || pPoint == nullptr) return kHitLeft;
    const int dL = pPoint->x - pRect->left;
    const int dR = pRect->right - pPoint->x;
    const int dT = pPoint->y - pRect->top;
    const int dB = pRect->bottom - pPoint->y;
    const int nH = (dL <= dR) ? kHitLeft : kHitRight;
    const int dH = (dL <= dR) ? dL : dR;
    const int nV = (dT <= dB) ? kHitTop : kHitBottom;
    const int dV = (dT <= dB) ? dT : dB;
    return (dH <= dV) ? nH : nV;
}

// Symbol: ?RemovePane@CDockSite@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@@Z
// Retail 0x53e10, transcribed in full (dockMethod is never read):
//   if (m_lstControlBars.GetCount() == 0) return;                     // +0x1C0
//   POSITION pos = m_lstControlBars.Find(pWnd);                       // open-coded walk
//   if (pos == NULL) return;
//   m_lstControlBars.RemoveAt(pos);                                   // 0x2306f0
//   CDockingPanesRow* pRow = pWnd->m_pDockBarRow;                     // +0x130
//   if (pRow != NULL) pRow->RemovePane(pWnd);                         // row vslot +0x58
// Signature corrected: the generated stub carried a fifteen-argument
// placeholder list; the export takes (this, pWnd, dockMethod).
extern "C" void MS_ABI impl__RemovePane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(void* pThis, CPane* pWnd, int dockMethod) {
    (void)dockMethod;
    if (pThis == nullptr || pWnd == nullptr) return;
    S_CDockSite* s = Site(pThis);
    ObNode* n = FindNode(&s->m_lstControlBars, pWnd);
    if (n == nullptr) return;
    CObList::POSITION pos = PosFromNode(n);
    impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(&s->m_lstControlBars, &pos);
    void* pRow = pWnd->m_pDockBarRow;
    if (pRow != nullptr) impl__RemovePane_CDockingPanesRow__UEAAXPEAVCPane___Z(pRow, pWnd);
}

// Symbol: ?RemoveRow@CDockSite@@QEAAXPEAVCDockingPanesRow@@@Z
// Retail 0x54160, transcribed in full:
//   int nRowHeight = pRow->m_nRowHeight;                              // +0x10
//   if (pRow->IsVisible()) ResizeDockSiteByOffset(-nRowHeight, TRUE); // row vslot +0xc0, 0x54810
//   POSITION pos = m_lstDockBarRows.Find(pRow);                       // open-coded walk
//   if (pos == NULL) return;
//   OnRemoveRow(pos, FALSE);                                          // vslot +0x520
//   m_lstDockBarRows.RemoveAt(pos);                                   // 0x2306f0
//   delete pRow;                                                      // row vslot +0x08, flag 1
// Note the order: the dock site is resized BEFORE the row is looked up, so a
// row that is not in the list still shrinks the site.
// Signature corrected: the generated stub had dropped `this`.
extern "C" void MS_ABI impl__RemoveRow_CDockSite__QEAAXPEAVCDockingPanesRow___Z(void* pThis, void* pRow) {
    if (pThis == nullptr || pRow == nullptr) return;
    S_CDockSite* s = Site(pThis);
    S_CDockingPanesRow* r = RowOf(pRow);
    const int nRowHeight = r->m_nRowHeight;
    if (RowIsVisible(r)) impl__ResizeDockSiteByOffset_CDockSite__IEAAXHH_Z(pThis, -nRowHeight, TRUE);
    ObNode* n = FindNode(&s->m_lstDockBarRows, pRow);
    if (n == nullptr) return;
    impl__OnRemoveRow_CDockSite__UEAAXPEAU__POSITION__H_Z(pThis, n, FALSE);
    CObList::POSITION pos = PosFromNode(n);
    impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(&s->m_lstDockBarRows, &pos);
    DeleteRow(pRow);
}

// Symbol: ?ReplacePane@CDockSite@@QEAAHPEAVCPane@@0@Z
// Retail 0x54d20, transcribed in full:
//   POSITION pos = m_lstControlBars.Find(pOldBar);                    // open-coded walk
//   if (pos == NULL) return FALSE;
//   m_lstControlBars.InsertAfter(pos, pNewBar);                       // 0x230690
//   m_lstControlBars.RemoveAt(pos);                                   // 0x2306f0
//   return TRUE;
// Signature corrected: the generated stub had dropped `this`.
extern "C" int MS_ABI impl__ReplacePane_CDockSite__QEAAHPEAVCPane__0_Z(void* pThis, CPane* pOldBar, CPane* pNewBar) {
    if (pThis == nullptr) return FALSE;
    S_CDockSite* s = Site(pThis);
    ObNode* n = FindNode(&s->m_lstControlBars, pOldBar);
    if (n == nullptr) return FALSE;
    CObList::POSITION pos = PosFromNode(n);
    impl__InsertAfter_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(&s->m_lstControlBars, &pos, pNewBar);
    pos = PosFromNode(n);
    impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(&s->m_lstControlBars, &pos);
    return TRUE;
}

// Symbol: ?RepositionPanes@CDockSite@@UEAAXAEAVCRect@@@Z
// Retail 0x53eb0, transcribed in full:
//   CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);           // import 0x1802c5358
//   int nDeltaW = rectNew.Width()  - rectClient.Width();
//   int nDeltaH = rectNew.Height() - rectClient.Height();
//   if (nDeltaW != 0 || nDeltaH != 0) {
//       for (each row in m_lstDockBarRows) {
//           if (nDeltaW != 0) pRow->RepositionPanes(rectNew, 2, nDeltaW > 0, abs(nDeltaW));  // row vslot +0x88
//           if (nDeltaH != 0) pRow->RepositionPanes(rectNew, 6, nDeltaH > 0, abs(nDeltaH));
//       }
//   } else {
//       for (each row in m_lstDockBarRows) pRow->ExpandStretchedPanesRect();  // 0x50080
//   }
// The 2 / 6 side literals are what retail passes (the tree does not model the
// enum they belong to; CDockingPanesRow.cpp compares against the same values).
// Signature corrected: the generated stub had dropped `this`.
extern "C" void MS_ABI impl__RepositionPanes_CDockSite__UEAAXAEAVCRect___Z(void* pThis, CRect* pRectNew) {
    if (pThis == nullptr || pRectNew == nullptr) return;
    S_CDockSite* s = Site(pThis);
    CRect rectClient(0, 0, 0, 0);
    ::GetClientRect(s->base.m_hWnd, RP(&rectClient));
    const int nDeltaW = (pRectNew->right - pRectNew->left) - (rectClient.right - rectClient.left);
    const int nDeltaH = (pRectNew->bottom - pRectNew->top) - (rectClient.bottom - rectClient.top);
    if (nDeltaW != 0 || nDeltaH != 0) {
        for (ObNode* n = HeadNode(&s->m_lstDockBarRows); n != nullptr; n = n->pNext) {
            if (nDeltaW != 0)
                impl__RepositionPanes_CDockingPanesRow__UEAAXAEAVCRect__IHH_Z(n->data, pRectNew, 2, nDeltaW > 0, AbsInt(nDeltaW));
            if (nDeltaH != 0)
                impl__RepositionPanes_CDockingPanesRow__UEAAXAEAVCRect__IHH_Z(n->data, pRectNew, 6, nDeltaH > 0, AbsInt(nDeltaH));
        }
    } else {
        for (ObNode* n = HeadNode(&s->m_lstDockBarRows); n != nullptr; n = n->pNext)
            impl__ExpandStretchedPanesRect_CDockingPanesRow__QEAAXXZ(n->data);
    }
}

// Symbol: ?RowFromPane@CDockSite@@IEBAPEAVCDockingPanesRow@@PEAVCBasePane@@@Z
// Retail 0x54770, transcribed in full:
//   for (each row in m_lstDockBarRows)
//       for (each pane in pRow->m_lstControlBars)                     // row +0x28, head +0x30
//           if (pane == pBar) return pRow;
//   return NULL;
// Signature corrected: the generated stub had dropped `this`.
extern "C" void* MS_ABI impl__RowFromPane_CDockSite__IEBAPEAVCDockingPanesRow__PEAVCBasePane___Z(const void* pThis, CBasePane* pBar) {
    if (pThis == nullptr) return nullptr;
    for (ObNode* n = HeadNode(&Site(pThis)->m_lstDockBarRows); n != nullptr; n = n->pNext) {
        S_CDockingPanesRow* r = RowOf(n);
        if (FindNode(&r->m_lstControlBars, pBar) != nullptr) return r;
    }
    return nullptr;
}

// Symbol: ?RowFromPoint@CDockSite@@IEBAPEAVCDockingPanesRow@@VCPoint@@AEA_N@Z
// Retail 0x54670, transcribed in full (CPoint packed in RDX, bool& in R8):
//   bOuterRow = false;
//   for (each row in m_lstDockBarRows) {
//       if (!pRow->IsVisible()) continue;                             // row vslot +0xc0
//       CRect rectRow; pRow->GetWindowRect(rectRow);                  // 0x4fcf0
//       if (::PtInRect(&rectRow, pt)) return pRow;                    // import 0x1802c5320
//   }
//   CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);                 // import 0x1802c5370
//   if ((IsHorizontal() && pt.y < rectWnd.top) ||                     // vslot +0x2d8, twice
//       (!IsHorizontal() && pt.x < rectWnd.left))
//       bOuterRow = true;
//   return NULL;
// Signature corrected: the generated stub had dropped `this`.
extern "C" void* MS_ABI impl__RowFromPoint_CDockSite__IEBAPEAVCDockingPanesRow__VCPoint__AEA_N_Z(
    const void* pThis, long long pt, bool* pbOuterRow) {
    if (pbOuterRow != nullptr) *pbOuterRow = false;
    if (pThis == nullptr) return nullptr;
    const POINT point = { PtX(pt), PtY(pt) };
    for (ObNode* n = HeadNode(&Site(pThis)->m_lstDockBarRows); n != nullptr; n = n->pNext) {
        S_CDockingPanesRow* r = RowOf(n);
        if (!RowIsVisible(r)) continue;
        CRect rectRow(0, 0, 0, 0);
        impl__GetWindowRect_CDockingPanesRow__QEBAXAEAVCRect___Z(r, &rectRow);
        if (::PtInRect(RP(&rectRow), point)) return r;
    }
    CRect rectWnd(0, 0, 0, 0);
    ::GetWindowRect(AsPane(pThis)->m_hWnd, RP(&rectWnd));
    const bool bHorz = SiteIsHorz(pThis);
    if ((bHorz && point.y < rectWnd.top) || (!bHorz && point.x < rectWnd.left)) {
        if (pbOuterRow != nullptr) *pbOuterRow = true;
    }
    return nullptr;
}

// Symbol: ?ShowRow@CDockSite@@QEAAXPEAVCDockingPanesRow@@HH@Z
// Retail 0x54300, transcribed in full:
//   POSITION pos = m_lstDockBarRows.Find(pRow);                       // open-coded walk; may be NULL
//   OnShowRow(pos, bShow);                                            // vslot +0x530 (throws on NULL)
//   ResizeDockSiteByOffset(bShow ? pRow->m_nRowHeight                 // tail-jump 0x54810
//                                : -pRow->m_nRowHeight, bAdjustLayout);
// Signature corrected: the generated stub had dropped `this`.
extern "C" void MS_ABI impl__ShowRow_CDockSite__QEAAXPEAVCDockingPanesRow__HH_Z(void* pThis, void* pRow, int bShow, int bAdjustLayout) {
    if (pThis == nullptr || pRow == nullptr) return;
    ObNode* n = FindNode(&Site(pThis)->m_lstDockBarRows, pRow);
    impl__OnShowRow_CDockSite__UEAAXPEAU__POSITION__H_Z(pThis, n, bShow);
    const int nRowHeight = RowOf(pRow)->m_nRowHeight;
    impl__ResizeDockSiteByOffset_CDockSite__IEAAXHH_Z(pThis, bShow ? nRowHeight : -nRowHeight, bAdjustLayout);
}

// Symbol: ?SwapRows@CDockSite@@QEAAXPEAVCDockingPanesRow@@0@Z
// Retail 0x54520, transcribed in full:
//   POSITION posFirst  = m_lstDockBarRows.Find(pFirstRow);            // open-coded walks
//   POSITION posSecond = m_lstDockBarRows.Find(pSecondRow);
//   ENSURE(posFirst != NULL && posSecond != NULL);                    // 0x225b80
//   POSITION pos = posFirst;
//   FindNextVisibleRow(pos, TRUE);                                    // 0x53710
//   bool bFirstBeforeSecond = (pos == posSecond);
//   if (!bFirstBeforeSecond) {
//       pos = posFirst;
//       FindNextVisibleRow(pos, FALSE);
//       if (pos != posSecond) return;        // not adjacent visible rows: nothing happens
//   }
//   m_lstDockBarRows.InsertAfter(posFirst,  pSecondRow);              // 0x230690
//   m_lstDockBarRows.InsertAfter(posSecond, pFirstRow);
//   m_lstDockBarRows.RemoveAt(posFirst);                              // 0x2306f0
//   m_lstDockBarRows.RemoveAt(posSecond);
//   pSecondRow->Move(bFirstBeforeSecond ? -pFirstRow->m_nRowHeight :  pFirstRow->m_nRowHeight);   // row vslot +0x80
//   pFirstRow ->Move(bFirstBeforeSecond ?  pSecondRow->m_nRowHeight : -pSecondRow->m_nRowHeight);
//   FixupVirtualRects();                                              // vslot +0x548
// Signature corrected: the generated stub had dropped `this`.
extern "C" void MS_ABI impl__SwapRows_CDockSite__QEAAXPEAVCDockingPanesRow__0_Z(void* pThis, void* pFirstRow, void* pSecondRow) {
    if (pThis == nullptr) return;
    S_CDockSite* s = Site(pThis);
    ObNode* nFirst  = FindNode(&s->m_lstDockBarRows, pFirstRow);
    ObNode* nSecond = FindNode(&s->m_lstDockBarRows, pSecondRow);
    if (nFirst == nullptr || nSecond == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }

    void* pos = nFirst;
    impl__FindNextVisibleRow_CDockSite__IEAAPEAVCDockingPanesRow__AEAPEAU__POSITION__H_Z(pThis, &pos, TRUE);
    const bool bFirstBeforeSecond = (pos == nSecond);
    if (!bFirstBeforeSecond) {
        pos = nFirst;
        impl__FindNextVisibleRow_CDockSite__IEAAPEAVCDockingPanesRow__AEAPEAU__POSITION__H_Z(pThis, &pos, FALSE);
        if (pos != nSecond) return;
    }

    CObList::POSITION posFirst = PosFromNode(nFirst);
    CObList::POSITION posSecond = PosFromNode(nSecond);
    impl__InsertAfter_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(&s->m_lstDockBarRows, &posFirst,
                                                                          static_cast<CObject*>(pSecondRow));
    impl__InsertAfter_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(&s->m_lstDockBarRows, &posSecond,
                                                                          static_cast<CObject*>(pFirstRow));
    posFirst = PosFromNode(nFirst);
    posSecond = PosFromNode(nSecond);
    impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(&s->m_lstDockBarRows, &posFirst);
    impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(&s->m_lstDockBarRows, &posSecond);

    const int nFirstHeight  = RowOf(pFirstRow)->m_nRowHeight;
    const int nSecondHeight = RowOf(pSecondRow)->m_nRowHeight;
    impl__Move_CDockingPanesRow__UEAAXH_Z(pSecondRow, bFirstBeforeSecond ? -nFirstHeight : nFirstHeight);
    impl__Move_CDockingPanesRow__UEAAXH_Z(pFirstRow, bFirstBeforeSecond ? nSecondHeight : -nSecondHeight);
    impl__FixupVirtualRects_CDockSite__UEAAXXZ(pThis);
}
