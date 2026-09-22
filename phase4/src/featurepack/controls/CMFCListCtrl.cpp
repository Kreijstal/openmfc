// CMFCListCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// ===========================================================================
// CMFCListCtrl -- the feature-pack list control (CListCtrl with a subclassed
// CMFCHeaderCtrl, sort-column tracking and per-cell custom draw).
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp); each one
// names the entry RVA it was read from (all RVAs in this file are mfc140u).
// The class is NOT declared in OpenMFC's public headers, so this file is the
// only place its layout lives; client (MSVC) code allocates the object with
// the retail member layout, which is pinned by ListCtrlLayout below.  Note
// on the derived CMFCShellListCtrl (controls/CMFCShellListCtrl.cpp, which
// models its base as 0x248 bytes): this class's own sizeof is 0x240 (see
// below) and the retail CMFCShellListCtrl ctor (0x12a760) stores its first
// *initialised* member, m_psfCurFolder, at +0x248 -- but +0x240 is NOT a gap
// in this class.  afxshelllistctrl.h declares `enum { ... }
// AFX_ShellListColumns;` -- a data member of an unnamed enum type, 4 bytes --
// ahead of m_psfCurFolder, so the derived class's own first member sits at
// +0x240..+0x243, four bytes of padding follow (m_psfCurFolder is 8-aligned),
// and the ctor simply never writes it.  (Cross-check: that layout makes
// sizeof(CMFCShellListCtrl) 0x278 == the 632 pinned in
// featurepack/controls/RuntimeClasses.cpp:232, and it is the only reading
// under which the ctor's stores at 0x248/0x250/0x258=0x60/0x260/0x268=1/0x270
// land on m_psfCurFolder/m_pidlCurFQ/m_nTypes/m_hwndRelatedTree/
// m_bContextMenu/m_bNoNotify.)  So nothing in this class extends past 0x240 --
// consistently, no retail body of either class has a `0x240(` operand in
// 0x77900..0x781a0 or 0x12a750..0x12c400 -- and the sibling's 0x248-byte base
// view merely folds AFX_ShellListColumns into what it calls the base.
//
// Retail object layout, read from the constructor (RVA 0x77970), cross-checked
// against the shipping afxlistctrl.h on this host (atlmfc/include/
// afxlistctrl.h: m_wndHeader, m_clrSortedColumn, m_iSortedColumn,
// m_bAscending, m_bMarkSortedColumn, m_hOldFont, in that order):
//
//   +0x000  CListCtrl base, 0xe8 bytes (== CWnd; m_hWnd at +0x40; the retail
//           ctor calls ??0CWnd@@QEAA@XZ (0x28a700) directly -- CListCtrl has
//           no ctor export)
//   +0x0e8  CMFCHeaderCtrl m_wndHeader, 0x140 bytes (ctor 0x6e200 / dtor
//           0x6e2e0 are called on this+0xe8; GetHeaderCtrl, the non-exported
//           vslot 92 body at 0x77900, returns this+0xe8).  Its own retail
//           layout is pinned by HeaderCtrlLayout below.
//   +0x228  COLORREF m_clrSortedColumn   (ctor: movq $-1,0x228 -> 0xFFFFFFFF)
//   +0x22c  int      m_iSortedColumn     (same qword          -> -1)
//   +0x230  BOOL     m_bAscending        (ctor: movq $1,0x230  -> TRUE)
//   +0x234  BOOL     m_bMarkSortedColumn (same qword          -> FALSE)
//   +0x238  HFONT    m_hOldFont          (ctor: movq $0,0x238)
//   sizeof == 0x240  (the scalar deleting destructor at 0x779d0 frees with
//                     size 0x240, and the CRuntimeClass at 0x1802eca48 --
//                     returned by ?GetRuntimeClass@CMFCListCtrl, RVA 0x77960
//                     -- carries m_nObjectSize 0x240; featurepack/controls/
//                     RuntimeClasses.cpp:231 already pins 576 == 0x240)
//
// Retail vtable (0x1802ec448 in mfc140u; slots read with the workflow's
// vtable dumper: 99 slots, 0..98; the qword at +0x318 is not a slot but the
// RTTI locator (0x180356708, ".?AVCListCtrl@@") that precedes CListCtrl's own
// vftable at 0x1802ec768).  The slots the bodies below dispatch through are:
//   slot  92 (+0x2e0)  GetHeaderCtrl          (0x77900: lea 0xe8(%rcx))
//   slot  93 (+0x2e8)  Sort                   (0x77c10)
//   slot  94 (+0x2f0)  OnCompareItems         (0x71e0: xor eax,eax; ret)
//   slot  95 (+0x2f8)  OnGetCellTextColor     (0x77910: LVM_GETTEXTCOLOR)
//   slot  96 (+0x300)  OnGetCellBkColor       (0x77930: LVM_GETBKCOLOR)
//   slot  97 (+0x308)  OnGetCellFont          (0x71e0: returns NULL)
//   slot  98 (+0x310)  InitHeader             (0x77a90)
//
// Retail message map (AFX_MSGMAP at 0x1802ec328, entries at 0x1802ec340,
// base map ?GetMessageMap@CListCtrl): ON_WM_CREATE -> 0x77af0,
// ON_WM_ERASEBKGND -> 0x77f30, ON_WM_SYSCOLORCHANGE -> 0x78060, ON_WM_SIZE ->
// 0x78140, ON_WM_STYLECHANGED -> 0x780e0, ON_NOTIFY_REFLECT(NM_CUSTOMDRAW)
// -> 0x77db0, ON_NOTIFY_REFLECT(LVN_COLUMNCLICK) -> 0x77b20.  OpenMFC's map
// for this class (detail/Mfc05MsgmapSupport.cpp:14) is EMPTY, so none of the
// handlers below is reached through message dispatch until that table is
// filled in (reported as a headerRequest).
//
// Deviations from retail, applied uniformly and repeated at each site:
//
//  (1) Virtual dispatch on `this` is devirtualized.  OpenMFC models no
//      MSVC-layout vtable for this class (the constructor below does NOT
//      store the retail vftable pointer -- see its comment), so every retail
//      `this->vslot` call above is made as a direct call to this file's own
//      thunk or file-local helper.  A client class that overrides one of
//      those virtuals -- in particular OnCompareItems, which is what makes
//      Sort do anything, and the three OnGetCell* hooks -- is therefore not
//      honoured from inside the DLL.  That is the convention used throughout
//      featurepack/ (e.g. controls/CMFCShellListCtrl.cpp deviation (1)).
//
//  (2) CListCtrl / CWnd inline wrappers that retail expands to a raw
//      SendMessage / USER32 call (GetBkColor, GetTextColor, GetClientRect,
//      RedrawWindow, GetSafeHwnd, CHeaderCtrl::GetItemRect, MapWindowPoints,
//      ListView_SortItems) are written as the same Win32 call on m_hWnd,
//      exactly as the retail code does.
//
//  (3) CWaitCursor (Sort).  Retail's `CWaitCursor wait;` is AfxGetModuleState()
//      ->m_pCurrentWinApp->BeginWaitCursor() (0x133930 then 0x1de7b0) with
//      EndWaitCursor in the destructor (the local helper at 0x7687c).
//      OpenMFC's AFX_MODULE_STATE keeps m_pCurrentWinApp at a different
//      offset, so the same pointer is fetched through the AfxGetApp thunk --
//      the shape used by controls/CMFCShellListCtrl.cpp's WaitCursorScope.
//
//  (4) CMFCHeaderCtrl.  Retail calls the header's exported SetSortColumn /
//      RemoveSortColumn / EnableMultipleSort and reads m_bMultipleSort
//      (header +0x124) directly.  Those exports live in controls/
//      CMFCHeaderCtrl.cpp, which is being implemented separately; they are
//      called through forward declarations with the signatures their mangled
//      names describe.  GetColumnState is inlined by retail (the CMap lookup
//      at 0x2f610 on header +0xe8); here it is called through the exported
//      ?GetColumnState@CMFCHeaderCtrl thunk (0x6ee80 is exactly that lookup),
//      so that the map internals stay with the header's owner.
//
//  (5) The embedded m_wndHeader constructor.  OpenMFC's ??0CMFCHeaderCtrl
//      thunk (controls/CtorDtorPlacement.cpp:26) constructs the header as
//      `CWnd + char[16]` (0xf8 bytes) and leaves everything from +0xe8 to
//      +0x13f untouched, whereas the retail ctor (0x6e200) initialises all of
//      it.  After calling that thunk, this file's ctor stores every value the
//      retail ctor stores EXCEPT the CMap vftable at header +0xe8 (listed at
//      HeaderCtrlLayout), so that m_bMultipleSort, which IsMultipleSort /
//      OnColumnClick read, is defined.  That one store is omitted because no
//      MSVC-layout vtable exists for CMap<int,int,int,int> here.
// ===========================================================================

namespace {

// CWnd::m_hWnd (include/openmfc/afxwin.h places it at 64 == 0x40; every retail
// body in this file reads it as 0x40(%rcx)).
constexpr size_t kHWndOffset = 0x40;
static_assert(offsetof(CWnd, m_hWnd) == kHWndOffset, "CWnd::m_hWnd +0x40");
static_assert(sizeof(CWnd) == 0xe8, "CWnd is 0xe8 bytes (retail CListCtrl base)");

// File-local layout view of the retail CMFCHeaderCtrl, read from its ctor
// (RVA 0x6e200) and cross-checked against atlmfc/include/afxheaderctrl.h
// (m_mapColumnsStatus, m_bIsMousePressed, m_bMultipleSort, m_bAscending,
// m_bTracked, m_bIsDlgControl, m_nHighlightedItem, m_hFont).  The initial
// values in the comments are the ones the retail ctor stores.
struct HeaderCtrlLayout {
    unsigned char base[0xe8];      // CHeaderCtrl (== CWnd; m_hWnd at +0x40)
    void*   map_vfptr;             // +0x0e8  CMap<int,int,int,int> vftable (not reproduced)
    void*   map_pHashTable;        // +0x0f0  NULL
    UINT    map_nHashTableSize;    // +0x0f8  17
    UINT    pad_fc;
    INT_PTR map_nCount;            // +0x100  0
    void*   map_pFreeList;         // +0x108  NULL
    void*   map_pBlocks;           // +0x110  NULL
    INT_PTR map_nBlockSize;        // +0x118  10
    BOOL    m_bIsMousePressed;     // +0x120  FALSE
    BOOL    m_bMultipleSort;       // +0x124  FALSE
    BOOL    m_bAscending;          // +0x128  TRUE
    BOOL    m_bTracked;            // +0x12c  FALSE
    BOOL    m_bIsDlgControl;       // +0x130  FALSE
    int     m_nHighlightedItem;    // +0x134  -1
    HFONT   m_hFont;               // +0x138  NULL
};
static_assert(sizeof(HeaderCtrlLayout) == 0x140, "CMFCHeaderCtrl: retail sizeof 0x140 (0x228 - 0xe8 in the CMFCListCtrl ctor)");
static_assert(offsetof(HeaderCtrlLayout, map_vfptr) == 0xe8, "m_mapColumnsStatus +0xe8");
static_assert(offsetof(HeaderCtrlLayout, map_nHashTableSize) == 0xf8, "m_mapColumnsStatus.m_nHashTableSize +0xf8");
static_assert(offsetof(HeaderCtrlLayout, map_nBlockSize) == 0x118, "m_mapColumnsStatus.m_nBlockSize +0x118");
static_assert(offsetof(HeaderCtrlLayout, m_bMultipleSort) == 0x124, "m_bMultipleSort +0x124");
static_assert(offsetof(HeaderCtrlLayout, m_bAscending) == 0x128, "m_bAscending +0x128");
static_assert(offsetof(HeaderCtrlLayout, m_nHighlightedItem) == 0x134, "m_nHighlightedItem +0x134");
static_assert(offsetof(HeaderCtrlLayout, m_hFont) == 0x138, "m_hFont +0x138");

// File-local layout view of the retail CMFCListCtrl (offsets cited in the
// header comment).
struct ListCtrlLayout {
    unsigned char    base[0xe8];          // CListCtrl (m_hWnd at +0x40)
    HeaderCtrlLayout m_wndHeader;         // +0x0e8
    COLORREF         m_clrSortedColumn;   // +0x228
    int              m_iSortedColumn;     // +0x22c
    BOOL             m_bAscending;        // +0x230
    BOOL             m_bMarkSortedColumn; // +0x234
    HFONT            m_hOldFont;          // +0x238
};
static_assert(sizeof(ListCtrlLayout) == 0x240, "CMFCListCtrl: retail sizeof 0x240 (scalar deleting dtor 0x779d0, CRuntimeClass m_nObjectSize)");
static_assert(offsetof(ListCtrlLayout, m_wndHeader) == 0xe8, "m_wndHeader +0xe8");
static_assert(offsetof(ListCtrlLayout, m_clrSortedColumn) == 0x228, "m_clrSortedColumn +0x228");
static_assert(offsetof(ListCtrlLayout, m_iSortedColumn) == 0x22c, "m_iSortedColumn +0x22c");
static_assert(offsetof(ListCtrlLayout, m_bAscending) == 0x230, "m_bAscending +0x230");
static_assert(offsetof(ListCtrlLayout, m_bMarkSortedColumn) == 0x234, "m_bMarkSortedColumn +0x234");
static_assert(offsetof(ListCtrlLayout, m_hOldFont) == 0x238, "m_hOldFont +0x238");

// CDC::m_hDC (retail OnEraseBkgnd reads pDC+0x8).
static_assert(offsetof(CDC, m_hDC) == 0x8, "CDC::m_hDC +0x8");

// Notification / style structs, pinned at the offsets the retail bodies read.
static_assert(offsetof(NMLISTVIEW, iSubItem) == 0x1c, "NMLISTVIEW::iSubItem +0x1c (OnColumnClick)");
static_assert(offsetof(NMLVCUSTOMDRAW, nmcd.dwDrawStage) == 0x18, "NMCUSTOMDRAW::dwDrawStage +0x18");
static_assert(offsetof(NMLVCUSTOMDRAW, nmcd.hdc) == 0x20, "NMCUSTOMDRAW::hdc +0x20");
static_assert(offsetof(NMLVCUSTOMDRAW, nmcd.dwItemSpec) == 0x38, "NMCUSTOMDRAW::dwItemSpec +0x38");
static_assert(offsetof(NMLVCUSTOMDRAW, nmcd.lItemlParam) == 0x48, "NMCUSTOMDRAW::lItemlParam +0x48");
static_assert(offsetof(NMLVCUSTOMDRAW, clrText) == 0x50, "NMLVCUSTOMDRAW::clrText +0x50");
static_assert(offsetof(NMLVCUSTOMDRAW, clrTextBk) == 0x54, "NMLVCUSTOMDRAW::clrTextBk +0x54");
static_assert(offsetof(NMLVCUSTOMDRAW, iSubItem) == 0x58, "NMLVCUSTOMDRAW::iSubItem +0x58");
static_assert(offsetof(STYLESTRUCT, styleOld) == 0x0 && offsetof(STYLESTRUCT, styleNew) == 0x4, "STYLESTRUCT layout (OnStyleChanged)");
static_assert(CDRF_NOTIFYITEMDRAW == 0x20 && CDRF_NOTIFYSUBITEMDRAW == 0x20 && (CDRF_NEWFONT | CDRF_NOTIFYPOSTPAINT) == 0x12, "custom-draw result codes as retail writes them");
static_assert((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == 0x30001 && (CDDS_ITEMPOSTPAINT | CDDS_SUBITEM) == 0x30002, "custom-draw stages as retail compares them");

// Field-for-field mirror of detail/CWinAppSupport.h's `struct _AFX_THREAD_STATE`
// (int nTempMapLock; int nWndCreateLock; void* pModuleState; void* pWndInit;
// void* hHookOldCbtFilter;), used only to read pWndInit in PreSubclassWindow --
// the same mirror controls/CMFCShellListCtrl.cpp uses.  (Retail reads
// _AFX_THREAD_STATE::m_pWndInit at +0x28; OpenMFC's struct keeps it at +0x10.)
struct AfxThreadStateMirror {
    int nTempMapLock;
    int nWndCreateLock;
    void* pModuleState;
    void* pWndInit;
    void* hHookOldCbtFilter;
};
static_assert(sizeof(AfxThreadStateMirror) == 32, "AfxThreadStateMirror: five-field _AFX_THREAD_STATE");
static_assert(offsetof(AfxThreadStateMirror, pWndInit) == 16, "AfxThreadStateMirror::pWndInit");

inline ListCtrlLayout* L(void* p) { return static_cast<ListCtrlLayout*>(p); }
inline const ListCtrlLayout* L(const void* p) { return static_cast<const ListCtrlLayout*>(p); }
inline HWND HWndOf(const void* p) {
    return *reinterpret_cast<const HWND*>(static_cast<const unsigned char*>(p) + kHWndOffset);
}
// GetHeaderCtrl(): retail vslot 92 (0x77900: `lea 0xe8(%rcx),%rax`), devirtualized (1).
inline HeaderCtrlLayout* HeaderOf(void* p) { return &L(p)->m_wndHeader; }
inline const HeaderCtrlLayout* HeaderOf(const void* p) { return &L(p)->m_wndHeader; }
inline HWND HeaderHWnd(const void* p) { return HWndOf(HeaderOf(p)); }

// RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE -- CWnd::RedrawWindow's default
// flags, the 0x105 every retail RedrawWindow call in this file passes.
constexpr UINT kRedrawDefault = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE;
static_assert(kRedrawDefault == 0x105, "RedrawWindow default flags");

} // namespace

// ---- sibling impl__ exports called by the bodies in this file -------------
// (see the link rule in the campaign briefing: C++ methods of other classes
// exist in this DLL only as impl__ thunks.)  Every declaration matches a
// definition that exists today; the file that holds it is named.
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                  // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CListCtrl__UEAA_XZ(CListCtrl* pThis);                        // core/controls/RuntimeClasses.cpp
extern "C" void* MS_ABI impl___0CMFCHeaderCtrl__QEAA_XZ(void* pThis);                        // controls/CtorDtorPlacement.cpp (CWnd + 16 bytes only, see (5))
extern "C" void  MS_ABI impl___1CMFCHeaderCtrl__UEAA_XZ(void* pThis);                        // controls/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl__SetSortColumn_CMFCHeaderCtrl__QEAAXHHH_Z(void* pThis, int iColumn, int bAscending, int bAdd); // controls/CMFCHeaderCtrl.cpp (being implemented; placeholder today)
extern "C" void  MS_ABI impl__RemoveSortColumn_CMFCHeaderCtrl__QEAAXH_Z(void* pThis, int iColumn);   // controls/CMFCHeaderCtrl.cpp (being implemented; placeholder today)
extern "C" void  MS_ABI impl__EnableMultipleSort_CMFCHeaderCtrl__QEAAXH_Z(void* pThis, int bEnable); // controls/CMFCHeaderCtrl.cpp (being implemented; placeholder today)
extern "C" int   MS_ABI impl__GetColumnState_CMFCHeaderCtrl__QEBAHH_Z(const void* pThis, int iColumn); // controls/CMFCHeaderCtrl.cpp (being implemented; placeholder today)
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                         // core/window/Thunks.cpp
extern "C" void  MS_ABI impl__OnSysColorChange_CWnd__IEAAXXZ(CWnd* pThis);                   // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__SubclassWindow_CWnd__QEAAHPEAUHWND_____Z(CWnd* pThis, HWND hWnd); // core/window/Thunks.cpp
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();                             // detail/RegcoreSupport.cpp
extern "C" void  MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);        // core/runtime/CCmdTarget.cpp
extern "C" void  MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);          // core/runtime/CCmdTarget.cpp
extern "C" _AFX_THREAD_STATE* MS_ABI impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ(); // detail/CWinAppSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                          // detail/MfcExceptionsSupport.cpp
extern "C" unsigned long MS_ABI impl__PixelAlpha_CDrawingManager__SAKKNNN_Z(unsigned long srcPixel, double dR, double dG, double dB); // core/gdi/CDrawingManager.cpp

// Thunks defined further down this file that earlier bodies call.
extern "C" void  MS_ABI impl__InitColors_CMFCListCtrl__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__InitHeader_CMFCListCtrl__MEAAXXZ(void* pThis);
extern "C" int   MS_ABI impl__InitList_CMFCListCtrl__IEAAHXZ(void* pThis);
extern "C" int   MS_ABI impl__OnCompareItems_CMFCListCtrl__UEAAH_J0H_Z(void* pThis, __int64 lParam1, __int64 lParam2, int iColumn);
extern "C" void  MS_ABI impl__Sort_CMFCListCtrl__UEAAXHHH_Z(void* pThis, int iColumn, int bAscending, int bAdd);
extern "C" int   MS_ABI impl__CompareProc_CMFCListCtrl__KAH_J00_Z(__int64 lParam1, __int64 lParam2, __int64 lParamSort);

namespace {

// Deviation (3): retail's `CWaitCursor wait;` -- AfxGetApp()->BeginWaitCursor()
// with EndWaitCursor on scope exit.  Retail dereferences the app pointer
// without a NULL test; a guard is added.
struct WaitCursorScope {
    CWinApp* pApp;
    WaitCursorScope() : pApp(impl__AfxGetApp__YAPEAVCWinApp__XZ()) {
        if (pApp) impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
    ~WaitCursorScope() {
        if (pApp) impl__EndWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
};

// The three inline virtuals afxlistctrl.h declares in-class (not exported;
// retail vtable slots 95..97), devirtualized (1):
//   OnGetCellTextColor (0x77910): return ::SendMessage(m_hWnd, LVM_GETTEXTCOLOR, 0, 0);
//   OnGetCellBkColor   (0x77930): return ::SendMessage(m_hWnd, LVM_GETBKCOLOR, 0, 0);
//   OnGetCellFont      (0x71e0):  return NULL;
// (IAT 0x1802c7120 -> USER32!SendMessageW; nRow / nColumn are not read.)
inline COLORREF OnGetCellTextColor(const void* pThis, int /*nRow*/, int /*nColumn*/) {
    return static_cast<COLORREF>(::SendMessage(HWndOf(pThis), LVM_GETTEXTCOLOR, 0, 0));
}
inline COLORREF OnGetCellBkColor(const void* pThis, int /*nRow*/, int /*nColumn*/) {
    return static_cast<COLORREF>(::SendMessage(HWndOf(pThis), LVM_GETBKCOLOR, 0, 0));
}
inline HFONT OnGetCellFont(const void* /*pThis*/, int /*nRow*/, int /*nColumn*/, DWORD /*dwData*/) {
    return nullptr;
}

// CListCtrl::GetBkColor() as retail expands it (LVM_GETBKCOLOR on m_hWnd).
inline COLORREF ListBkColor(const void* pThis) {
    return static_cast<COLORREF>(::SendMessage(HWndOf(pThis), LVM_GETBKCOLOR, 0, 0));
}

} // namespace

// Symbol: ??0CMFCListCtrl@@QEAA@XZ
// Transcribed from retail entry RVA 0x77970 (mfc140u):
//     CWnd::CWnd();                                         // 0x28a700 (CListCtrl has no ctor export)
//     vfptr = 0x1802ec448;  (NOT reproduced -- see below)
//     CMFCHeaderCtrl::CMFCHeaderCtrl(&m_wndHeader);         // 0x6e200 on this+0xe8
//     m_clrSortedColumn = (COLORREF)-1;  m_iSortedColumn = -1;   // movq $-1,0x228
//     m_bAscending = TRUE;  m_bMarkSortedColumn = FALSE;         // movq $1,0x230
//     m_hOldFont = NULL;                                         // movq $0,0x238
// The vftable store is omitted: OpenMFC has no MSVC-layout vtable for this
// class, and whatever the CWnd constructor thunk installs at +0x00 is left
// alone.  Deviation (5): after the header ctor thunk, the retail header
// ctor's stores (0x6e200) are repeated here because that thunk does not
// perform them.
extern "C" void* MS_ABI impl___0CMFCListCtrl__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    impl___0CWnd__QEAA_XZ(pThis);
    ListCtrlLayout* s = L(pThis);
    impl___0CMFCHeaderCtrl__QEAA_XZ(&s->m_wndHeader);
    {   // deviation (5): what ??0CMFCHeaderCtrl (0x6e200) stores after CWnd::CWnd
        HeaderCtrlLayout* h = &s->m_wndHeader;
        h->map_pHashTable = nullptr;       // mov %rax,0xf0   (rax == 0)
        h->map_nHashTableSize = 17;        // movl $0x11,0xf8
        h->map_nCount = 0;                 // mov %rax,0x100
        h->map_pFreeList = nullptr;        // mov %rax,0x108
        h->map_pBlocks = nullptr;          // mov %rax,0x110
        h->map_nBlockSize = 10;            // movq $0xa,0x118
        h->m_bIsMousePressed = FALSE;      // mov %rax,0x120 (with m_bMultipleSort)
        h->m_bMultipleSort = FALSE;
        h->m_bAscending = TRUE;            // movq $1,0x128 (with m_bTracked)
        h->m_bTracked = FALSE;
        h->m_bIsDlgControl = FALSE;        // mov %eax,0x130
        h->m_nHighlightedItem = -1;        // movl $-1,0x134
        h->m_hFont = nullptr;              // mov %rax,0x138
    }
    s->m_clrSortedColumn = static_cast<COLORREF>(-1);
    s->m_iSortedColumn = -1;
    s->m_bAscending = TRUE;
    s->m_bMarkSortedColumn = FALSE;
    s->m_hOldFont = nullptr;
    return pThis;
}

// Symbol: ??1CMFCListCtrl@@UEAA@XZ
// Transcribed from retail entry RVA 0x77a20 (mfc140u): stores the class
// vftable (0x1802ec448; omitted here, see the constructor), destroys
// m_wndHeader (??1CMFCHeaderCtrl, 0x6e2e0, on this+0xe8) and tail-jumps to
// ??1CListCtrl@@UEAA@XZ (0x295210).  It does not touch m_hOldFont.
// (Returns pThis only because the sibling controls/CMFCShellListCtrl.cpp
// declares this thunk with a void* result; retail's rax is whatever the
// CListCtrl dtor leaves.)
extern "C" void* MS_ABI impl___1CMFCListCtrl__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    impl___1CMFCHeaderCtrl__UEAA_XZ(&L(pThis)->m_wndHeader);
    impl___1CListCtrl__UEAA_XZ(static_cast<CListCtrl*>(pThis));
    return pThis;
}

// Symbol: ?CompareProc@CMFCListCtrl@@KAH_J00@Z
// Transcribed from retail entry RVA 0x77d70 (mfc140u):
//     CMFCListCtrl* pList = (CMFCListCtrl*)lParamSort;
//     int nRes = pList->OnCompareItems(lParam1, lParam2, pList->m_iSortedColumn);  // vslot 94 (+0x2f0), devirtualized (1)
//     return pList->m_bAscending ? nRes : -nRes;             // neg / cmovne on 0x230
// Retail dereferences lParamSort without a NULL test; a guard is added.
extern "C" int MS_ABI impl__CompareProc_CMFCListCtrl__KAH_J00_Z(__int64 lParam1, __int64 lParam2, __int64 lParamSort) {
    void* pList = reinterpret_cast<void*>(lParamSort);
    if (pList == nullptr) return 0;
    const int nRes = impl__OnCompareItems_CMFCListCtrl__UEAAH_J0H_Z(pList, lParam1, lParam2, L(pList)->m_iSortedColumn);
    return L(pList)->m_bAscending ? nRes : -nRes;
}

// Symbol: ?EnableMarkSortedColumn@CMFCListCtrl@@QEAAXHH@Z
// Transcribed from retail entry RVA 0x77f00 (mfc140u):
//     m_bMarkSortedColumn = bMark;
//     if (GetSafeHwnd() != NULL && bRedraw)
//         RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);  // IAT 0x1802c7130 -> USER32!RedrawWindow
extern "C" void MS_ABI impl__EnableMarkSortedColumn_CMFCListCtrl__QEAAXHH_Z(void* pThis, int bMark, int bRedraw) {
    if (pThis == nullptr) return;
    L(pThis)->m_bMarkSortedColumn = bMark;
    HWND hWnd = HWndOf(pThis);
    if (hWnd != nullptr && bRedraw) {
        ::RedrawWindow(hWnd, nullptr, nullptr, kRedrawDefault);
    }
}

// Symbol: ?EnableMultipleSort@CMFCListCtrl@@QEAAXH@Z
// Retail entry RVA 0x77d20 (mfc140u): tail-jumps to
// ?EnableMultipleSort@CMFCHeaderCtrl (0x6ed20) on GetHeaderCtrl() (vslot 92,
// devirtualized (1)) with bEnable -- deviation (4).
extern "C" void MS_ABI impl__EnableMultipleSort_CMFCListCtrl__QEAAXH_Z(void* pThis, int bEnable) {
    if (pThis == nullptr) return;
    impl__EnableMultipleSort_CMFCHeaderCtrl__QEAAXH_Z(HeaderOf(pThis), bEnable);
}

// Symbol: ?InitColors@CMFCListCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x780a0 (mfc140u):
//     m_clrSortedColumn = CDrawingManager::PixelAlpha(GetBkColor(), .97, .97, .97);
// (LVM_GETBKCOLOR on m_hWnd, then ?PixelAlpha@CDrawingManager@@SAKKNNN@Z at
// 0x5b310 with the double at 0x18034ff50 == 0.97 in all three slots.)
extern "C" void MS_ABI impl__InitColors_CMFCListCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    L(pThis)->m_clrSortedColumn = impl__PixelAlpha_CDrawingManager__SAKKNNN_Z(ListBkColor(pThis), .97, .97, .97);
}

// Symbol: ?InitHeader@CMFCListCtrl@@MEAAXXZ
// Transcribed from retail entry RVA 0x77a90 (mfc140u):
//     GetHeaderCtrl().SubclassDlgItem(0, this);              // vslot 92 then ?SubclassDlgItem@CWnd (0x291e00)
// Retail's SubclassDlgItem(nID, pParent) (0x291e00) is
//     HWND h = ::GetDlgItem(pParent->m_hWnd, nID); if (h) return SubclassWindow(h); /* else OCC path */
// OpenMFC's ?SubclassDlgItem@CWnd thunk (core/window/Thunks.cpp:1723, which
// forwards 1:1 to CWnd::SubclassDlgItem at core/window/CWnd.cpp:3580) has the
// roles of `this` and the CWnd argument inverted and rejects nID == 0 -- the
// list view's header IS child id 0 -- so the GetDlgItem + SubclassWindow
// expansion is written here directly (the OCC fallback for a missing child
// window is not reproduced: without m_pCtrlCont there is nothing to walk).
extern "C" void MS_ABI impl__InitHeader_CMFCListCtrl__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    HWND hWndHeader = ::GetDlgItem(HWndOf(pThis), 0);
    if (hWndHeader != nullptr) {
        impl__SubclassWindow_CWnd__QEAAHPEAUHWND_____Z(reinterpret_cast<CWnd*>(HeaderOf(pThis)), hWndHeader);
    }
}

// Symbol: ?InitList@CMFCListCtrl@@IEAAHXZ
// Transcribed from retail entry RVA 0x77a60 (mfc140u):
//     InitHeader();                                          // vslot 98 (+0x310), devirtualized (1)
//     InitColors();                                          // 0x780a0
//     return TRUE;
extern "C" int MS_ABI impl__InitList_CMFCListCtrl__IEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    impl__InitHeader_CMFCListCtrl__MEAAXXZ(pThis);
    impl__InitColors_CMFCListCtrl__IEAAXXZ(pThis);
    return TRUE;
}

// Symbol: ?IsMultipleSort@CMFCListCtrl@@QEBAHXZ
// Retail entry RVA 0x77d50 (mfc140u): `return GetHeaderCtrl().m_bMultipleSort`
// (vslot 92, devirtualized (1), then header +0x124).
extern "C" int MS_ABI impl__IsMultipleSort_CMFCListCtrl__QEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return FALSE;
    return HeaderOf(pThis)->m_bMultipleSort;
}

// Symbol: ?OnColumnClick@CMFCListCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
// Transcribed from retail entry RVA 0x77b20 (mfc140u):
//     NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
//     ENSURE(pNMListView != NULL);                           // 0x227720 AfxThrowInvalidArgException
//     int iColumn = pNMListView->iSubItem;                   // +0x1c
//     BOOL bShiftIsPressed = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;   // IAT 0x1802c7170
//     int nColumnState = 0;
//     GetHeaderCtrl().m_mapColumnsStatus.Lookup(iColumn, nColumnState);      // 0x2f610 on header+0xe8 -- GetColumnState inlined, deviation (4)
//     BOOL bAscending = TRUE;
//     if (nColumnState != 0) bAscending = nColumnState <= 0;
//     BOOL bAdd = bShiftIsPressed && GetHeaderCtrl().m_bMultipleSort;        // header +0x124
//     Sort(iColumn, bAscending, bAdd);                       // vslot 93 (+0x2e8), devirtualized (1)
//     *pResult = 0;
// Deviation: retail writes *pResult unconditionally (movq $0,(%r15) at
// 0x77beb, with no NULL test on the third argument); a guard is added here.
extern "C" void MS_ABI impl__OnColumnClick_CMFCListCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(void* pThis, NMHDR* pNMHDR, __int64* pResult) {
    if (pThis == nullptr) return;
    if (pNMHDR == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    const NMLISTVIEW* pNMListView = reinterpret_cast<const NMLISTVIEW*>(pNMHDR);
    const int iColumn = pNMListView->iSubItem;
    const BOOL bShiftIsPressed = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    const int nColumnState = impl__GetColumnState_CMFCHeaderCtrl__QEBAHH_Z(HeaderOf(pThis), iColumn);
    BOOL bAscending = TRUE;
    if (nColumnState != 0) {
        bAscending = nColumnState <= 0;
    }
    const BOOL bAdd = bShiftIsPressed && HeaderOf(pThis)->m_bMultipleSort;
    impl__Sort_CMFCListCtrl__UEAAXHHH_Z(pThis, iColumn, bAscending, bAdd);
    if (pResult != nullptr) *pResult = 0;
}

// Symbol: ?OnCompareItems@CMFCListCtrl@@UEAAH_J0H@Z
// Retail entry RVA 0x71e0 (mfc140u; export-directory ordinal 8955, a
// COMDAT-folded `xor eax,eax; ret` shared with other trivial exports):
// the base implementation compares nothing and returns 0.
extern "C" int MS_ABI impl__OnCompareItems_CMFCListCtrl__UEAAH_J0H_Z(void* /*pThis*/, __int64 /*lParam1*/, __int64 /*lParam2*/, int /*iColumn*/) {
    return 0;
}

// Symbol: ?OnCreate@CMFCListCtrl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Transcribed from retail entry RVA 0x77af0 (mfc140u):
//     if ((int)Default() == -1) return -1;                   // 0x28ac80 -- CListCtrl::OnCreate collapsed to CWnd::Default
//     return InitList() ? 0 : -1;                            // 0x77a60 (neg/sbb/neg/dec)
// lpCreateStruct is not read.
extern "C" int MS_ABI impl__OnCreate_CMFCListCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* /*lpCreateStruct*/) {
    if (pThis == nullptr) return -1;
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis))) == -1) return -1;
    return impl__InitList_CMFCListCtrl__IEAAHXZ(pThis) ? 0 : -1;
}

// Symbol: ?OnCustomDraw@CMFCListCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
// Transcribed from retail entry RVA 0x77db0 (mfc140u):
//     ENSURE(pNMHDR != NULL);                                // 0x227720
//     NMLVCUSTOMDRAW* lplvcd = (NMLVCUSTOMDRAW*)pNMHDR;
//     switch (lplvcd->nmcd.dwDrawStage)                      // +0x18
//     {
//     case CDDS_PREPAINT:                *pResult = CDRF_NOTIFYITEMDRAW;    break;   // 1       -> 0x20
//     case CDDS_ITEMPREPAINT:            *pResult = CDRF_NOTIFYSUBITEMDRAW; break;   // 0x10001 -> 0x20
//     case CDDS_ITEMPREPAINT | CDDS_SUBITEM:                                         // 0x30001
//         int iColumn = lplvcd->iSubItem;                    // +0x58
//         int iRow = (int)lplvcd->nmcd.dwItemSpec;           // +0x38
//         lplvcd->clrTextBk = OnGetCellBkColor(iRow, iColumn);       // vslot 96 -> +0x54
//         lplvcd->clrText = OnGetCellTextColor(iRow, iColumn);       // vslot 95 -> +0x50
//         if (iColumn == m_iSortedColumn && m_bMarkSortedColumn && lplvcd->clrTextBk == GetBkColor())
//             lplvcd->clrTextBk = m_clrSortedColumn;
//         HFONT hFont = OnGetCellFont(iRow, iColumn, (DWORD)lplvcd->nmcd.lItemlParam);  // vslot 97; +0x48 read as 32 bits
//         if (hFont != NULL) {
//             m_hOldFont = (HFONT)::SelectObject(lplvcd->nmcd.hdc, hFont);   // +0x20; IAT 0x1802c64f8 GDI32!SelectObject
//             ENSURE(m_hOldFont != NULL);
//             *pResult = CDRF_NEWFONT | CDRF_NOTIFYPOSTPAINT;                // 0x12
//         } else *pResult = CDRF_DODEFAULT;
//         break;
//     case CDDS_ITEMPOSTPAINT | CDDS_SUBITEM:                                        // 0x30002
//         if (m_hOldFont != NULL) { ::SelectObject(lplvcd->nmcd.hdc, m_hOldFont); m_hOldFont = NULL; }
//         *pResult = CDRF_DODEFAULT;
//         break;
//     }                                                      // any other stage: *pResult untouched
// The three OnGetCell* virtuals are devirtualized (1) to the file-local
// helpers above.  Deviation: retail writes *pResult unconditionally on every
// branch that writes it at all (mov %rax,(%rsi) / movq $0x20,(%r8)), with no
// NULL test on pResult; a guard is added at each of those stores here.
extern "C" void MS_ABI impl__OnCustomDraw_CMFCListCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(void* pThis, NMHDR* pNMHDR, __int64* pResult) {
    if (pThis == nullptr) return;
    if (pNMHDR == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    ListCtrlLayout* s = L(pThis);
    NMLVCUSTOMDRAW* lplvcd = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
    switch (lplvcd->nmcd.dwDrawStage) {
    case CDDS_PREPAINT:
        if (pResult != nullptr) *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
        if (pResult != nullptr) *pResult = CDRF_NOTIFYSUBITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT | CDDS_SUBITEM: {
        const int iColumn = lplvcd->iSubItem;
        const int iRow = static_cast<int>(lplvcd->nmcd.dwItemSpec);
        lplvcd->clrTextBk = OnGetCellBkColor(pThis, iRow, iColumn);
        lplvcd->clrText = OnGetCellTextColor(pThis, iRow, iColumn);
        if (iColumn == s->m_iSortedColumn && s->m_bMarkSortedColumn && lplvcd->clrTextBk == ListBkColor(pThis)) {
            lplvcd->clrTextBk = s->m_clrSortedColumn;
        }
        HFONT hFont = OnGetCellFont(pThis, iRow, iColumn, static_cast<DWORD>(lplvcd->nmcd.lItemlParam));
        if (hFont != nullptr) {
            s->m_hOldFont = static_cast<HFONT>(::SelectObject(lplvcd->nmcd.hdc, hFont));
            if (s->m_hOldFont == nullptr) {
                impl__AfxThrowInvalidArgException__YAXXZ();
                return;
            }
            if (pResult != nullptr) *pResult = CDRF_NEWFONT | CDRF_NOTIFYPOSTPAINT;
        } else {
            if (pResult != nullptr) *pResult = CDRF_DODEFAULT;
        }
        break;
    }

    case CDDS_ITEMPOSTPAINT | CDDS_SUBITEM:
        if (s->m_hOldFont != nullptr) {
            ::SelectObject(lplvcd->nmcd.hdc, s->m_hOldFont);
            s->m_hOldFont = nullptr;
        }
        if (pResult != nullptr) *pResult = CDRF_DODEFAULT;
        break;

    default:
        break;
    }
}

// Symbol: ?OnEraseBkgnd@CMFCListCtrl@@IEAAHPEAVCDC@@@Z
// Transcribed from retail entry RVA 0x77f30 (mfc140u):
//     BOOL bRes = (BOOL)Default();                           // 0x28ac80 -- CListCtrl::OnEraseBkgnd collapsed to CWnd::Default
//     if (m_iSortedColumn >= 0 && m_bMarkSortedColumn)
//     {
//         CRect rectClient;  ::GetClientRect(m_hWnd, &rectClient);          // IAT 0x1802c7330
//         CRect rectColumn(0,0,0,0);
//         GetHeaderCtrl().GetItemRect(m_iSortedColumn, &rectColumn);        // HDM_GETITEMRECT (0x1207) on header m_hWnd
//         ::MapWindowPoints(GetHeaderCtrl().m_hWnd, m_hWnd, (LPPOINT)&rectColumn, 2);  // IAT 0x1802c7228
//         CRect rectFill = rectClient;                        // movaps -0x20 -> -0x30
//         rectFill.left = rectColumn.left;  rectFill.right = rectColumn.right;
//         CBrush br(m_clrSortedColumn);                       // ??0CBrush@@QEAA@K@Z (0x2a4060)
//         ::FillRect(pDC->m_hDC, &rectFill, (HBRUSH)br.m_hObject);          // IAT 0x1802c7208; pDC+0x8 / brush+0x8
//     }                                                      // ~CBrush: the CBrush vftable store (0x1802dde08) is inline at
//                                                            // 0x78021, then the helper at 0x1c6f0 is ~CGdiObject (stores the
//                                                            // CGdiObject vftable 0x1802dddd8, calls ?DeleteObject@CGdiObject, 0x2a3f60)
//     return bRes;
// Deviation: the CBrush is replaced by the raw ::CreateSolidBrush /
// ::DeleteObject pair -- OpenMFC's ??0CBrush@@QEAA@K@Z thunk (core/gdi/
// CBrush.cpp:12) is exactly CreateSolidBrush with no handle-map registration,
// so the GDI calls made are identical.  Retail dereferences pDC without a
// NULL test; a guard is added.
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCListCtrl__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    if (pThis == nullptr) return FALSE;
    const ListCtrlLayout* s = L(pThis);
    const BOOL bRes = static_cast<BOOL>(impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis)));
    if (s->m_iSortedColumn >= 0 && s->m_bMarkSortedColumn && pDC != nullptr) {
        RECT rectClient = { 0, 0, 0, 0 };
        ::GetClientRect(HWndOf(pThis), &rectClient);
        RECT rectColumn = { 0, 0, 0, 0 };
        HWND hWndHeader = HeaderHWnd(pThis);
        ::SendMessage(hWndHeader, HDM_GETITEMRECT, static_cast<WPARAM>(s->m_iSortedColumn), reinterpret_cast<LPARAM>(&rectColumn));
        ::MapWindowPoints(hWndHeader, HWndOf(pThis), reinterpret_cast<LPPOINT>(&rectColumn), 2);
        RECT rectFill = rectClient;
        rectFill.left = rectColumn.left;
        rectFill.right = rectColumn.right;
        HBRUSH hbr = ::CreateSolidBrush(s->m_clrSortedColumn);
        ::FillRect(pDC->m_hDC, &rectFill, hbr);
        if (hbr != nullptr) ::DeleteObject(hbr);
    }
    return bRes;
}

// Symbol: ?OnSize@CMFCListCtrl@@IEAAXIHH@Z
// Transcribed from retail entry RVA 0x78140 (mfc140u):
//     Default();                                             // 0x28ac80 -- CListCtrl::OnSize collapsed to CWnd::Default
//     if (GetHeaderCtrl().GetSafeHwnd() != NULL)             // vslot 92 (devirtualized (1)), NULL-tested, then +0x40
//         GetHeaderCtrl().RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);  // IAT 0x1802c7130
// nType / cx / cy are not read (Default() re-dispatches the current message).
extern "C" void MS_ABI impl__OnSize_CMFCListCtrl__IEAAXIHH_Z(void* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    HWND hWndHeader = HeaderHWnd(pThis);
    if (hWndHeader != nullptr) {
        ::RedrawWindow(hWndHeader, nullptr, nullptr, kRedrawDefault);
    }
}

// Symbol: ?OnStyleChanged@CMFCListCtrl@@IEAAXHPEAUtagSTYLESTRUCT@@@Z
// Transcribed from retail entry RVA 0x780e0 (mfc140u):
//     Default();                                             // 0x28ac80 -- CListCtrl::OnStyleChanged collapsed to CWnd::Default
//     if ((lpStyleStruct->styleNew & LVS_REPORT) && !(lpStyleStruct->styleOld & LVS_REPORT))  // testb $1,0x4 / testb $1,0x0
//         if (GetHeaderCtrl().GetSafeHwnd() == NULL)         // vslot 92, NULL-tested, then +0x40
//             InitHeader();                                  // vslot 98 (+0x310), devirtualized (1)
// nStyleType is not read.  Retail dereferences lpStyleStruct without a NULL
// test; a guard is added.
extern "C" void MS_ABI impl__OnStyleChanged_CMFCListCtrl__IEAAXHPEAUtagSTYLESTRUCT___Z(void* pThis, int /*nStyleType*/, STYLESTRUCT* lpStyleStruct) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    if (lpStyleStruct == nullptr) return;
    if ((lpStyleStruct->styleNew & LVS_REPORT) && !(lpStyleStruct->styleOld & LVS_REPORT)) {
        if (HeaderHWnd(pThis) == nullptr) {
            impl__InitHeader_CMFCListCtrl__MEAAXXZ(pThis);
        }
    }
}

// Symbol: ?OnSysColorChange@CMFCListCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x78060 (mfc140u):
//     CWnd::OnSysColorChange();                              // 0x28f320
//     InitColors();                                          // 0x780a0
//     RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);  // tail-jump, IAT 0x1802c7130
extern "C" void MS_ABI impl__OnSysColorChange_CMFCListCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__OnSysColorChange_CWnd__IEAAXXZ(static_cast<CWnd*>(pThis));
    impl__InitColors_CMFCListCtrl__IEAAXXZ(pThis);
    ::RedrawWindow(HWndOf(pThis), nullptr, nullptr, kRedrawDefault);
}

// Symbol: ?PreSubclassWindow@CMFCListCtrl@@MEAAXXZ
// Transcribed from retail entry RVA 0x77ac0 (mfc140u):
//     _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();        // 0x1332a0
//     if (pThreadState->m_pWndInit == NULL) InitList();             // +0x28; 0x77a60
// (No call to the CWnd/CListCtrl PreSubclassWindow survives in the retail
// body.)  OpenMFC keeps the same member as _AFX_THREAD_STATE::pWndInit (read
// through AfxThreadStateMirror above).  Retail dereferences the thread state
// unconditionally; a NULL guard is added.
extern "C" void MS_ABI impl__PreSubclassWindow_CMFCListCtrl__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    const AfxThreadStateMirror* pThreadState =
        reinterpret_cast<const AfxThreadStateMirror*>(impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ());
    if (pThreadState == nullptr || pThreadState->pWndInit == nullptr) {
        impl__InitList_CMFCListCtrl__IEAAHXZ(pThis);
    }
}

// Symbol: ?RemoveSortColumn@CMFCListCtrl@@QEAAXH@Z
// Retail entry RVA 0x77cf0 (mfc140u): tail-jumps to
// ?RemoveSortColumn@CMFCHeaderCtrl (0x6e780) on GetHeaderCtrl() (vslot 92,
// devirtualized (1)) with iColumn -- deviation (4).
extern "C" void MS_ABI impl__RemoveSortColumn_CMFCListCtrl__QEAAXH_Z(void* pThis, int iColumn) {
    if (pThis == nullptr) return;
    impl__RemoveSortColumn_CMFCHeaderCtrl__QEAAXH_Z(HeaderOf(pThis), iColumn);
}

// Symbol: ?SetSortColumn@CMFCListCtrl@@QEAAXHHH@Z
// Retail entry RVA 0x77ca0 (mfc140u): tail-jumps to
// ?SetSortColumn@CMFCHeaderCtrl (0x6e700) on GetHeaderCtrl() (vslot 92,
// devirtualized (1)) with (iColumn, bAscending, bAdd) -- deviation (4).  It
// does NOT touch m_iSortedColumn / m_bAscending; only Sort does.
extern "C" void MS_ABI impl__SetSortColumn_CMFCListCtrl__QEAAXHHH_Z(void* pThis, int iColumn, int bAscending, int bAdd) {
    if (pThis == nullptr) return;
    impl__SetSortColumn_CMFCHeaderCtrl__QEAAXHHH_Z(HeaderOf(pThis), iColumn, bAscending, bAdd);
}

// Symbol: ?Sort@CMFCListCtrl@@UEAAXHHH@Z
// Transcribed from retail entry RVA 0x77c10 (mfc140u):
//     CWaitCursor wait;                                      // deviation (3)
//     GetHeaderCtrl().SetSortColumn(iColumn, bAscending, bAdd);   // vslot 92 then 0x6e700 -- deviation (4)
//     m_iSortedColumn = iColumn;                             // 0x22c
//     m_bAscending = bAscending;                             // 0x230
//     ListView_SortItems(m_hWnd, CompareProc, (LPARAM)this); // SendMessage(m_hWnd, LVM_SORTITEMS (0x1030), (WPARAM)this, (LPARAM)0x77d70)
//                                                            // ~CWaitCursor (0x7687c)
extern "C" void MS_ABI impl__Sort_CMFCListCtrl__UEAAXHHH_Z(void* pThis, int iColumn, int bAscending, int bAdd) {
    if (pThis == nullptr) return;
    WaitCursorScope wait;
    ListCtrlLayout* s = L(pThis);
    impl__SetSortColumn_CMFCHeaderCtrl__QEAAXHHH_Z(&s->m_wndHeader, iColumn, bAscending, bAdd);
    s->m_iSortedColumn = iColumn;
    s->m_bAscending = bAscending;
    ::SendMessage(HWndOf(pThis), LVM_SORTITEMS, reinterpret_cast<WPARAM>(pThis),
                  reinterpret_cast<LPARAM>(&impl__CompareProc_CMFCListCtrl__KAH_J00_Z));
}
