// CPaneContainer — OpenMFC implementation.
// Sources: cbarcore.cpp
//
// The bodies below marked "decoded from retail" were transcribed from the
// retail export disassembly (disas.py against
// /home/kreijstal/msvc/VC/Redist/MSVC/14.51.36231/x64/Microsoft.VC145.MFC/mfc140.dll,
// the ANSI twin of mfc140u.dll; function bodies are byte-identical between the
// two images, only the export RVAs differ, so every RVA quoted here is an
// mfc140.dll RVA and is named as such).

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

#include <cstddef>

// ---------------------------------------------------------------------------
// Retail object layout
//
// CPaneContainer is not declared in include/openmfc/ (afxmfc.h only forward
// declares it), so this translation unit models the retail object with a local
// shadow struct. Every offset below was read out of the retail constructor
// ??0CPaneContainer@@QEAA@PEAVCPaneContainerManager@@PEAVCDockablePane@@1PEAVCPaneDivider@@@Z
// (entry RVA 0xa3750, mfc140.dll) and cross-checked against the accessors that
// use it (?SetPane@ 0xa5200, ?SetPaneContainer@ 0xa5210, ?IsEmpty@ 0xa7040,
// ?GetTotalReferenceCount@ 0xa7e70, ?GetAssociatedSiblingPaneIDs@ 0xa7bd0).
//
// sizeof == 0xf8 is not a guess: ?Copy@CPaneContainer@@UEAAPEAV1@PEAV1@@Z
// (entry RVA 0xa7ca0) allocates a fresh container with `operator new(0xf8)`
// (the `mov $0xf8,%ecx; call 0x180002840` pair at 0xa7cc5) before running the
// constructor on it.
//
// Nothing inside OpenMFC allocates a CPaneContainer -- the class has no public
// declaration to allocate through -- so the only objects these entry points
// ever see are ones a client allocated with the retail layout and handed to the
// exported constructor below. Writing at these offsets is therefore correct and
// is what makes the implementations in this file meaningful.
// ---------------------------------------------------------------------------

namespace {

// CList<UINT,UINT> has the CObList field shape. The retail constructor writes
// exactly seven slots per list: the vftable at +0x00, zero at +0x08..+0x28 and
// 10 (the CList default block size) at +0x30.
struct S_CListUINT {
    void*   vfptr;         // 0x00
    void*   m_pNodeHead;   // 0x08
    void*   m_pNodeTail;   // 0x10
    void*   m_pNodeFree;   // 0x18
    void*   m_pBlocks;     // 0x20
    INT_PTR m_nCount;      // 0x28
    INT_PTR m_nBlockSize;  // 0x30
};

// One node of that list, as walked by the non-exported CList<UINT,UINT>::Find at
// RVA 0x11f3c (mfc140.dll): the head comes from list+0x08, the successor from
// node+0x00 and the stored UINT from node+0x10.  Find returns the node (the
// POSITION) or NULL; its `test %rcx,%rcx` on node+0x10 at 0x11f4a can never fire.
struct S_CListUINTNode {
    S_CListUINTNode* pNext;   // 0x00
    S_CListUINTNode* pPrev;   // 0x08
    unsigned int     value;   // 0x10
};

struct S_CPaneContainer {
    void*  vfptr;                              // 0x00
    void*  m_pLeftPane;                        // 0x08 CDockablePane*
    void*  m_pRightPane;                       // 0x10 CDockablePane*
    void*  m_pSlider;                          // 0x18 CPaneDivider*
    S_CPaneContainer* m_pLeftPaneContainer;    // 0x20
    S_CPaneContainer* m_pRightPaneContainer;   // 0x28
    S_CPaneContainer* m_pParentContainer;      // 0x30
    void*  m_pContainerManager;                // 0x38 CPaneContainerManager*
    int    m_nPercent;                         // 0x40 ctor stores 50
    int    m_nField44;                         // 0x44 ctor stores -1; ?SetUpByID@
                                               //      (0xa7860) matches it against a
                                               //      persisted LEFT-pane id
    int    m_nField48;                         // 0x48 ctor stores -1; the same, for
                                               //      the RIGHT pane
    int    m_nField4C;                         // 0x4c ctor stores -1
    int    m_nRefCount;                        // 0x50 AddRef/Release
    int    m_bDividerHorz;                     // 0x54 cached CPaneDivider orientation
    int    m_nField58;                         // 0x58
    int    m_bReleased;                        // 0x5c FreeReleasedPaneContainer sets 1
    int    m_nField60;                         // 0x60
    RECT   m_rect64;                           // 0x64 ctor ::SetRectEmpty's it
    RECT   m_rect74;                           // 0x74 ctor ::SetRectEmpty's it
    unsigned char m_pad84[4];                  // 0x84 alignment padding
    S_CListUINT m_lstLeftSiblingIDs;           // 0x88
    S_CListUINT m_lstRightSiblingIDs;          // 0xc0
};

static_assert(offsetof(S_CPaneContainer, m_pLeftPane) == 0x08, "CPaneContainer m_pLeftPane");
static_assert(offsetof(S_CPaneContainer, m_pRightPane) == 0x10, "CPaneContainer m_pRightPane");
static_assert(offsetof(S_CPaneContainer, m_pSlider) == 0x18, "CPaneContainer m_pSlider");
static_assert(offsetof(S_CPaneContainer, m_pLeftPaneContainer) == 0x20, "CPaneContainer m_pLeftPaneContainer");
static_assert(offsetof(S_CPaneContainer, m_pRightPaneContainer) == 0x28, "CPaneContainer m_pRightPaneContainer");
static_assert(offsetof(S_CPaneContainer, m_pParentContainer) == 0x30, "CPaneContainer m_pParentContainer");
static_assert(offsetof(S_CPaneContainer, m_pContainerManager) == 0x38, "CPaneContainer m_pContainerManager");
static_assert(offsetof(S_CPaneContainer, m_nPercent) == 0x40, "CPaneContainer m_nPercent");
static_assert(offsetof(S_CPaneContainer, m_nRefCount) == 0x50, "CPaneContainer m_nRefCount");
static_assert(offsetof(S_CPaneContainer, m_bDividerHorz) == 0x54, "CPaneContainer m_bDividerHorz");
static_assert(offsetof(S_CPaneContainer, m_bReleased) == 0x5c, "CPaneContainer m_bReleased");
static_assert(offsetof(S_CPaneContainer, m_rect64) == 0x64, "CPaneContainer m_rect64");
static_assert(offsetof(S_CPaneContainer, m_rect74) == 0x74, "CPaneContainer m_rect74");
static_assert(offsetof(S_CPaneContainer, m_lstLeftSiblingIDs) == 0x88, "CPaneContainer m_lstLeftSiblingIDs");
static_assert(offsetof(S_CPaneContainer, m_lstRightSiblingIDs) == 0xc0, "CPaneContainer m_lstRightSiblingIDs");
static_assert(sizeof(S_CPaneContainer) == 0xf8, "CPaneContainer size (operator new(0xf8) in ?Copy@ 0xa7ca0)");

inline S_CPaneContainer* PC(void* p) { return static_cast<S_CPaneContainer*>(p); }
inline const S_CPaneContainer* PC(const void* p) { return static_cast<const S_CPaneContainer*>(p); }

// BC_FIND_CRITERIA, as dispatched by ?FindSubPaneContainer@ (entry RVA 0xa5230):
// 0 compares m_pLeftPane, 1 compares m_pRightPane, 2 compares m_pSlider,
// 3 compares the container itself; anything else matches nothing.
enum { BC_FIND_LEFT_PANE = 0, BC_FIND_RIGHT_PANE = 1, BC_FIND_SLIDER = 2, BC_FIND_CONTAINER = 3 };

} // namespace

// Cross-class entry points. C++ methods exist in this DLL only as impl__ thunks,
// so every call below goes through the thunk rather than the class declaration.
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* p0);
extern "C" void MS_ABI impl__MoveWindow_CWnd__QEAAXHHHHH_Z(
    CWnd* pThis, int x, int y, int nWidth, int nHeight, int bRepaint);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

// Same-file entry points, forward declared so the bodies below can call each
// other the way retail does (retail recurses through the direct, non-virtual
// entry point, never through the vtable).
extern "C" void MS_ABI impl__FreeReleasedPaneContainer_CPaneContainer__IEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__IsEmpty_CPaneContainer__QEBAHXZ(const void* pThis);
extern "C" int MS_ABI impl__IsVisible_CPaneContainer__QEBAHXZ(const void* pThis);
extern "C" void MS_ABI impl__AddNode_CPaneContainer__IEAAXPEAV1__Z(void* pThis, void* pContainer);
extern "C" void* MS_ABI impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
    void* pThis, const void* pObject, int criteria);
extern "C" void MS_ABI impl__CheckPaneDividerVisibility_CPaneContainer__QEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__GetNodeCount_CPaneContainer__QEBAHXZ(const void* pThis);
extern "C" int MS_ABI impl__GetTotalReferenceCount_CPaneContainer__QEBAHXZ(const void* pThis);
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainer_CPaneContainer__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__SetPaneContainerManager_CPaneContainer__IEAAXPEAVCPaneContainerManager__H_Z(
    void* pThis, void* pManager, int bRecursive);
extern "C" void MS_ABI impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(
    const void* pThis, RECT* pRect, int bIgnoreVisibility);
extern "C" void MS_ABI impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(const void* pThis, SIZE* pSize);
extern "C" int MS_ABI impl__GetResizeStep_CPaneContainer__UEBAHXZ(const void* pThis);
extern "C" int MS_ABI impl__IsPaneDividerHorz_CPaneContainer__IEBAHXZ(const void* pThis);
extern "C" void MS_ABI impl__RemoveNonValidPanes_CPaneContainer__QEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__CalcAvailableSpace_CPaneContainer__UEAA_AVCSize__V2_H_Z(
    void* pThis, SIZE* pRet, long long sizeAdd, int bVert);
extern "C" int MS_ABI impl__IsLeftPartEmpty_CPaneContainer__QEBAHH_Z(const void* pThis, int bCheckVisibility);
extern "C" int MS_ABI impl__IsRightPartEmpty_CPaneContainer__QEBAHH_Z(const void* pThis, int bCheckVisibility);
extern "C" void MS_ABI impl__Resize_CPaneContainer__UEAAXVCRect__AEAPEAXH_Z(
    void* pThis, const RECT* pRect, void** phdwp, int bRedraw);
extern "C" int MS_ABI impl__StretchPaneContainer_CPaneContainer__UEAAHHHHHAEAPEAX_Z(
    void* pThis, int nOffset, int bStretchHorz, int bLeftBar, int bMoveSlider, void** phdwp);
extern "C" void MS_ABI impl__Move_CPaneContainer__UEAAXVCPoint___Z(void* pThis, long long point);
extern "C" void MS_ABI impl__CalculateRecentSize_CPaneContainer__QEAAXXZ(void* pThis);

// Cross-class entry points used by the layout bodies below.  Two of them are
// declared here with the retail (RCX = this) shape although the definitions in
// their own files still carry the generated stub shape that dropped `this`;
// phase4/src/featurepack/docking/CDockingPanesRow.cpp does the same thing and
// documents why -- the export ABI passes `this` in RCX regardless of what the
// generated prototype says, and the extra register is ignored by a stub body.
//   CDockablePane::GetDefaultPaneDivider   defined in CDockablePane.cpp as ()
//   CRecentDockSiteInfo::StoreDockInfo     defined in CRecentDockSiteInfo.cpp
//                                          as (void*, void*)
// The other six -- CPane::CalcAvailableSize, CPaneContainerManager::
// CheckAndRemoveNonValidPane, CBasePane::GetParentMiniFrame, CPane::MovePane,
// CBasePane::MoveWindow and CBasePane::SetWindowPos -- are defined with exactly
// these signatures.  Those sibling files are edited concurrently, so re-grep
// rather than trusting this note; line numbers are deliberately not quoted.
extern "C" void* MS_ABI impl__CalcAvailableSize_CPane__UEAA_AVCSize__VCRect___Z(
    CPane* pThis, SIZE* pRet, const RECT* pRectRequired);
extern "C" int MS_ABI impl__CheckAndRemoveNonValidPane_CPaneContainerManager__QEAAHPEAVCWnd___Z(
    void* pThis, void* pWnd);
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
    const CBasePane* pThis, int bNoAssert);
extern "C" void* MS_ABI impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(
    const void* pThis);
extern "C" void MS_ABI impl__StoreDockInfo_CRecentDockSiteInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane___Z(
    void* pThis, void* pContainer, void* pPane);
extern "C" void* MS_ABI impl__MovePane_CPane__UEAA_AVCSize__VCRect__HAEAPEAX_Z(
    CPane* pThis, SIZE* pRet, const RECT* pRectNew, int bForceMove, void** phdwp);
extern "C" void* MS_ABI impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(
    CBasePane* pThis, const RECT* lpRect, int bRepaint, void* hdwp);
extern "C" void* MS_ABI impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
    CBasePane* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy,
    unsigned int nFlags, void* hdwp);

// ---------------------------------------------------------------------------
// Helpers shared by the layout bodies below.
// ---------------------------------------------------------------------------
namespace {

// The gate that ?GetMinSize@ (0xa3ca0), ?GetMinSizeLeft@ (0xa3ec0),
// ?GetMinSizeRight@ (0xa3fa0), ?GetWindowRect@ (0xa3a70) and
// ?CalculateRecentSize@ (0xa52c0) all open with, in retail:
//
//     CPaneDivider* pDef = m_pContainerManager->m_pDefaultSlider;  // manager +0x88
//     BOOL bAll = pDef != NULL && pDef-><vftable byte 0x3c8>();
//
// When it is TRUE a pane that is not WS_VISIBLE still contributes to the layout.
// Byte 0x3c8 of the CPaneDivider vftable (0x2f4d28, mfc140.dll) holds RVA
// 0xab4a0, whose whole body is `mov 0x1c8(%rcx),%eax; ret`.  That DWORD is the
// value ?OnPaint@CPaneDivider@ (0xac570) reads at 0xac5dc and passes as the
// bAutoHideMode argument of CMFCVisualManager::OnDrawPaneDivider, and the one
// ?Init@CPaneDivider@ (0xab720) clears at 0xab797.
//
// DEVIATION: OpenMFC's CPaneDivider is not a retail-shaped object -- afxmfc.h
// declares it as a small CObject and phase4/src/featurepack/docking/
// CPaneDivider.cpp keeps its state in a side table -- so +0x1c8 does not exist
// here and cannot be read.  This returns retail's cleared-flag value, i.e. what
// Init leaves behind and therefore what every non-autohide path computes.
inline int CountsHiddenPanes(const S_CPaneContainer* p) {
    (void)p;
    return 0;
}

// Retail tests a pane's WS_VISIBLE with CWnd::GetStyle (RVA 0x2a75a0) followed
// by `bt $0x1c`.
inline int PaneIsVisible(const void* pPane) {
    return (impl__GetStyle_CWnd__QEBAKXZ(reinterpret_cast<const CWnd*>(pPane)) & WS_VISIBLE) != 0;
}

// Retail asks a pane for its minimum through byte 0x4f0 of the pane vftable.
// That slot holds the unexported two-instruction accessor at RVA 0x8910 --
// `mov 0x200(%rcx),%rax; mov %rax,(%rdx); ret`, i.e. `size = m_sizeMin` -- in
// both the CPane vftable (0x2f3a18) and the CDockablePane vftable (0x2e3068).
// OpenMFC lays CPane out at the retail offsets (m_sizeMin is pinned at 0x200 in
// include/openmfc/afxmfc.h), so the member is read directly.
//
// DEVIATION: the CTabbedPane vftable (0x30f738) overrides that slot with
// ?GetMinSize@CBaseTabbedPane@ (0x14b70), which delegates to the active tabbed
// pane when the module-global at 0x1803b6fd0 is set and otherwise falls through
// to the same `size = m_sizeMin` read at 0x14bd8.  OpenMFC's
// impl__GetMinSize_CBaseTabbedPane__UEBAXAEAVCSize___Z is an empty stub
// (CBaseTabbedPane.cpp:72), so that fallback is the only behaviour available
// here in any case.
inline void PaneMinSize(const void* pPane, SIZE* pOut) {
    const CPane* pane = reinterpret_cast<const CPane*>(pPane);
    pOut->cx = pane->m_sizeMin.cx;
    pOut->cy = pane->m_sizeMin.cy;
}

inline HWND PaneHwnd(const void* pPane) {
    return reinterpret_cast<const CPane*>(pPane)->m_hWnd;
}

inline int MaxI(int a, int b) { return a > b ? a : b; }

// MSVC passes an 8-byte POD by value in a single integer register, so a CSize
// argument and a CSize return slot are handled here as one packed 64-bit value.
inline long long PackSize(int cx, int cy) {
    return static_cast<long long>((static_cast<unsigned long long>(static_cast<unsigned int>(cy)) << 32) |
                                  static_cast<unsigned int>(cx));
}
inline int LowHalf(long long v) {
    return static_cast<int>(static_cast<unsigned int>(static_cast<unsigned long long>(v) & 0xffffffffULL));
}
inline int HighHalf(long long v) {
    return static_cast<int>(static_cast<unsigned int>(static_cast<unsigned long long>(v) >> 32));
}

// CPaneContainerManager::m_pDockSite, +0x98.  Six of the bodies below reach the
// dock-site window the way retail does -- through byte 0xf8 of the
// CPaneContainerManager vftable (0x2f41a8, mfc140.dll), slot 31, whose target is
// the unexported two-instruction accessor at RVA 0xa8060 whose whole body is
// `mov 0x98(%rcx),%rax; ret`.  The offset is the one the sibling translation
// unit phase4/src/featurepack/docking/CPaneContainerManager.cpp:67 already pins
// as kOffDockSite, so the member is read directly instead of dispatched.
//
// DEVIATION: retail dereferences m_pContainerManager without a NULL test; a NULL
// manager returns NULL here and the callers skip the coordinate mapping rather
// than faulting.
constexpr size_t kOffManagerDockSite = 0x98;
inline CWnd* ManagerDockSite(const S_CPaneContainer* p) {
    if (p->m_pContainerManager == nullptr) return nullptr;
    return *reinterpret_cast<CWnd* const*>(
        static_cast<const char*>(p->m_pContainerManager) + kOffManagerDockSite);
}

// The divider's own window handle, CWnd::m_hWnd at +0x40 (retail and OpenMFC
// agree on that offset -- include/openmfc/afxwin.h:589).
//
// DEVIATION: OpenMFC's CPaneDivider is not a CWnd at all (afxmfc.h:1747 declares
// it as a small CObject), so +0x40 only holds a handle when the object came from
// a retail-shaped allocation.  ::IsWindow rejects the other case, and the one
// caller (OnMoveInternalPaneDivider) then skips retail's screen-to-client
// mapping instead of handing a garbage handle to ::GetParent.
inline HWND PaneDividerHwnd(const void* pSlider) {
    if (pSlider == nullptr) return nullptr;
    HWND h = *reinterpret_cast<HWND const*>(static_cast<const char*>(pSlider) + 0x40);
    return ::IsWindow(h) ? h : nullptr;
}

// Byte 0x3c8 of the CPaneDivider vftable -- the same DWORD at CPaneDivider
// +0x1c8 that CountsHiddenPanes above documents, and unavailable here for the
// same reason.  Returns retail's cleared-flag value.
inline int PaneDividerAutoHideMode(const void* pSlider) {
    (void)pSlider;
    return 0;
}

// CDockablePane +0x410, the per-pane split percentage that
// ?CalculateRecentSize@ (entry RVA 0xa52c0, mfc140.dll) both writes (the stores
// at 0xa549e / 0xa54e9 / 0xa5718 / 0xa5740) and reads back (0xa5584 / 0xa55d6 /
// 0xa564c / 0xa569e).  It is a real retail member: the CDockablePane
// CRuntimeClass at 0x3aa178 records m_nObjectSize == 1248 (0x4e0), so 0x410 is
// inside the object.  In OpenMFC it falls inside the 128-byte
// _dockablepane_padding blob afxmfc.h:476 gives CDockablePane, which is why the
// static_assert below is the only thing pinning it -- there is no named member
// to use.
constexpr size_t kOffPaneRecentPercent = 0x410;
static_assert(kOffPaneRecentPercent + sizeof(int) <= sizeof(CDockablePane),
              "CDockablePane +0x410 must lie inside OpenMFC's CDockablePane");
inline int& PaneRecentPercent(void* pPane) {
    return *reinterpret_cast<int*>(static_cast<char*>(pPane) + kOffPaneRecentPercent);
}

// CList<UINT,UINT>::Find (RVA 0x11f3c, mfc140.dll), transcribed.
inline const S_CListUINTNode* FindSiblingID(const S_CListUINT* pList, unsigned int nID) {
    const S_CListUINTNode* pNode = static_cast<const S_CListUINTNode*>(pList->m_pNodeHead);
    while (pNode != nullptr) {
        if (pNode->value == nID) return pNode;
        pNode = pNode->pNext;
    }
    return nullptr;
}

} // namespace

// Symbol: ??0CPaneContainer@@QEAA@PEAVCPaneContainerManager@@PEAVCDockablePane@@1PEAVCPaneDivider@@@Z
//
// Retail constructor, entry RVA 0xa3750 (mfc140.dll). It stores the four
// arguments at +0x38/+0x08/+0x10/+0x18, nulls the three container links,
// seeds m_nPercent(+0x40) with 50 and +0x44/+0x48/+0x4c with -1, zeroes the
// int fields at +0x50..+0x60, ::SetRectEmpty's the two CRects at +0x64/+0x74,
// and constructs the two CList<UINT,UINT> members at +0x88/+0xc0.
//
// DEVIATION, deliberate: OpenMFC cannot store the retail CList<UINT,UINT>
// vftable pointer that the retail constructor writes at +0x88 and +0xc0
// (that vftable lives in mfc140u and has no OpenMFC counterpart), so the two
// lists are zero-initialised with m_nBlockSize = 10 and a NULL vfptr. Every
// other field retail writes is written here with retail's value.
//
// The side-table registration below predates this file's layout work and is
// kept because sibling translation units (CPaneContainerManager.cpp,
// CDockablePane.cpp, CPaneDivider.cpp) read g_containerPanes / g_paneToContainer.
extern "C" void* MS_ABI impl___0CPaneContainer__QEAA_PEAVCPaneContainerManager__PEAVCDockablePane__1PEAVCPaneDivider___Z(
    void* pThis, void* pManager, void* pFirstPane, void* pSecondPane, void* pDivider) {
    if (pThis) {
        S_CPaneContainer* p = PC(pThis);
        p->m_pLeftPane = pFirstPane;
        p->m_pRightPane = pSecondPane;
        p->m_pSlider = pDivider;
        p->m_pLeftPaneContainer = nullptr;
        p->m_pRightPaneContainer = nullptr;
        p->m_pParentContainer = nullptr;
        p->m_pContainerManager = pManager;
        p->m_nPercent = 50;
        p->m_nField44 = -1;
        p->m_nField48 = -1;
        p->m_nField4C = -1;
        p->m_nRefCount = 0;
        p->m_bDividerHorz = 0;
        p->m_nField58 = 0;
        p->m_bReleased = 0;
        p->m_nField60 = 0;
        ::SetRectEmpty(&p->m_rect64);
        ::SetRectEmpty(&p->m_rect74);
        p->m_lstLeftSiblingIDs = S_CListUINT();
        p->m_lstLeftSiblingIDs.m_nBlockSize = 10;
        p->m_lstRightSiblingIDs = S_CListUINT();
        p->m_lstRightSiblingIDs.m_nBlockSize = 10;
    }

    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_containerPanes[pThis];
    AddUniquePane(state, pFirstPane);
    AddUniquePane(state, pSecondPane);
    if (pManager) {
        AddUniquePane(g_containerManagerPanes[pManager], pFirstPane);
        AddUniquePane(g_containerManagerPanes[pManager], pSecondPane);
    }
    if (pDivider) {
        AddUniquePane(g_dividerPanes[pDivider], pFirstPane);
        AddUniquePane(g_dividerPanes[pDivider], pSecondPane);
    }
    if (pFirstPane) g_paneToContainer[pFirstPane] = pThis;
    if (pSecondPane) g_paneToContainer[pSecondPane] = pThis;
    return pThis;
}
// Symbol: ??1CPaneContainer@@UEAA@XZ
extern "C" void* MS_ABI impl___1CPaneContainer__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_containerPanes.erase(pThis);
    return pThis;
}
// Symbol: ?AddPane@CPaneContainer@@QEAAPEAVCDockablePane@@PEAV2@@Z
extern "C" void* MS_ABI impl__AddPane_CPaneContainer__QEAAPEAVCDockablePane__PEAV2__Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    AddUniquePane(g_containerPanes[pThis], pPane);
    if (pPane) g_paneToContainer[pPane] = pThis;
    return pPane;
}
// Symbol: ?RemovePane@CPaneContainer@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__RemovePane_CPaneContainer__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    RemovePane(g_containerPanes[pThis], pPane);
    if (pPane) g_paneToContainer.erase(pPane);
}

// Decoded from retail ?AddNode@CPaneContainer@@IEAAXPEAV1@@Z, entry RVA 0xa4c10
// (mfc140.dll):
//   if none of this->m_pLeftPane / m_pRightPane equals either of pContainer's
//   two panes -> AfxThrowInvalidArgException();
//   if m_pLeftPane is non-null and is one of pContainer's panes, the LEFT slot
//   is converted into a sub-container link, otherwise the RIGHT slot is;
//   finally pContainer->m_pParentContainer = this.
// Symbol: ?AddNode@CPaneContainer@@IEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddNode_CPaneContainer__IEAAXPEAV1__Z(void* pThis, void* pContainer) {
    S_CPaneContainer* p = PC(pThis);
    S_CPaneContainer* q = PC(pContainer);
    void* pLeft = p->m_pLeftPane;
    if (pLeft != q->m_pLeftPane && pLeft != q->m_pRightPane &&
        p->m_pRightPane != q->m_pLeftPane && p->m_pRightPane != q->m_pRightPane) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (pLeft != nullptr && (pLeft == q->m_pLeftPane || pLeft == q->m_pRightPane)) {
        p->m_pLeftPane = nullptr;
        p->m_pLeftPaneContainer = q;
    } else {
        p->m_pRightPane = nullptr;
        p->m_pRightPaneContainer = q;
    }
    q->m_pParentContainer = p;
}

// Decoded from retail ?AddRef@CPaneContainer@@QEAAXXZ, entry RVA 0xa4ce0
// (mfc140.dll): a single `incl 0x50(%rcx)`.
// Symbol: ?AddRef@CPaneContainer@@QEAAXXZ
extern "C" void MS_ABI impl__AddRef_CPaneContainer__QEAAXXZ(void* pThis) {
    ++PC(pThis)->m_nRefCount;
}

// Decoded from retail ?AddSubPaneContainer@CPaneContainer@@QEAAHPEAV1@H@Z,
// entry RVA 0xa4b20 (mfc140.dll). Picks one pane out of pContainer (its left
// pane when bLeftPane, otherwise its right one), locates the node in THIS
// sub-tree that currently owns that pane -- first searching by left pane, then
// by right pane -- and splices pContainer in there with AddNode. Returns FALSE
// when no owning node was found.
// Symbol: ?AddSubPaneContainer@CPaneContainer@@QEAAHPEAV1@H@Z
extern "C" int MS_ABI impl__AddSubPaneContainer_CPaneContainer__QEAAHPEAV1_H_Z(
    void* pThis, void* pContainer, int bLeftPane) {
    S_CPaneContainer* p = PC(pThis);
    S_CPaneContainer* q = PC(pContainer);

    if (p->m_pSlider == q->m_pSlider) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;
    }
    void* pPane = q->m_pLeftPane;
    if (pPane == nullptr && q->m_pRightPane == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;
    }
    if (!bLeftPane) pPane = q->m_pRightPane;
    if (pPane == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;
    }

    S_CPaneContainer* pFound = nullptr;
    if (p->m_pLeftPane == pPane) {
        pFound = p;
    } else {
        if (p->m_pLeftPaneContainer != nullptr) {
            pFound = PC(impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
                p->m_pLeftPaneContainer, pPane, BC_FIND_LEFT_PANE));
        }
        if (pFound == nullptr && p->m_pRightPaneContainer != nullptr) {
            pFound = PC(impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
                p->m_pRightPaneContainer, pPane, BC_FIND_LEFT_PANE));
        }
    }

    if (pFound == nullptr) {
        if (p->m_pRightPane == pPane) {
            pFound = p;
        } else {
            S_CPaneContainer* pByRight = nullptr;
            if (p->m_pLeftPaneContainer != nullptr) {
                pByRight = PC(impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
                    p->m_pLeftPaneContainer, pPane, BC_FIND_RIGHT_PANE));
            }
            if (pByRight == nullptr && p->m_pRightPaneContainer != nullptr) {
                pByRight = PC(impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
                    p->m_pRightPaneContainer, pPane, BC_FIND_RIGHT_PANE));
            }
            pFound = pByRight;
        }
    }

    if (pFound == nullptr) return 0;
    impl__AddNode_CPaneContainer__IEAAXPEAV1__Z(pFound, q);
    return 1;
}

// Decoded from retail ?CalcAvailablePaneSpace@CPaneContainer@@UEAAHHPEAVCPane@@PEAV1@H@Z,
// entry RVA 0xa6dc0 (mfc140.dll).  Four declared arguments, so the fourth lands
// in the home slot at entry_rsp+0x28 (read as `mov 0x7f(%rbp),%esi` at 0xa6de3);
// the generated stub had dropped `this`, and is corrected here.
//
//   if (pPane != NULL) {
//       CRect rect(0,0,0,0);
//       ::GetWindowRect(pPane->m_hWnd, &rect);            // IAT 0x2c5370
//       if (IsPaneDividerHorz()) {                        // 0xa7e40
//           if (bVert) rect.bottom += nOffset; else rect.top  += nOffset;
//           return pPane-><vftable byte 0x5b0>(rect).cy;  // CPane::CalcAvailableSize
//       }
//       if (bVert) rect.right += nOffset; else rect.left += nOffset;
//       return pPane-><vftable byte 0x5b0>(rect).cx;
//   }
//   if (pContainer != NULL) {
//       if (IsPaneDividerHorz())
//           return pContainer->CalcAvailableSpace(CSize(0, nOffset), bVert).cy;
//       return pContainer->CalcAvailableSpace(CSize(nOffset, 0), bVert).cx;
//   }
//   return nOffset;                                       // 0xa6ed5, unchanged
//
// Byte 0x5b0 of the pane vftable is ?CalcAvailableSize@CPane@ (RVA 0xa1590) in
// the CPane (0x2f3a18), CDockablePane (0x2e3068) and CTabbedPane (0x30f738)
// vftables alike, and that export is really implemented in OpenMFC
// (phase4/src/featurepack/docking/CPane.cpp), so the dispatch is replaced by a
// direct call to its thunk -- a client override of CalcAvailableSize would be
// missed.  Byte 0x98 of the CPaneContainer vftable (0x2f4070) is slot 19,
// CalcAvailableSpace, called directly here for the same reason.
// Symbol: ?CalcAvailablePaneSpace@CPaneContainer@@UEAAHHPEAVCPane@@PEAV1@H@Z
extern "C" int MS_ABI impl__CalcAvailablePaneSpace_CPaneContainer__UEAAHHPEAVCPane__PEAV1_H_Z(
    void* pThis, int nOffset, void* pPane, void* pContainer, int bVert) {
    if (pPane != nullptr) {
        RECT rect = {0, 0, 0, 0};
        ::GetWindowRect(PaneHwnd(pPane), &rect);
        SIZE sizeAvail = {0, 0};
        if (impl__IsPaneDividerHorz_CPaneContainer__IEBAHXZ(pThis)) {
            if (bVert) rect.bottom += nOffset; else rect.top += nOffset;
            RECT rectArg = rect;
            impl__CalcAvailableSize_CPane__UEAA_AVCSize__VCRect___Z(
                reinterpret_cast<CPane*>(pPane), &sizeAvail, &rectArg);
            return sizeAvail.cy;
        }
        if (bVert) rect.right += nOffset; else rect.left += nOffset;
        RECT rectArg = rect;
        impl__CalcAvailableSize_CPane__UEAA_AVCSize__VCRect___Z(
            reinterpret_cast<CPane*>(pPane), &sizeAvail, &rectArg);
        return sizeAvail.cx;
    }
    if (pContainer != nullptr) {
        SIZE sizeAvail = {0, 0};
        if (impl__IsPaneDividerHorz_CPaneContainer__IEBAHXZ(pThis)) {
            impl__CalcAvailableSpace_CPaneContainer__UEAA_AVCSize__V2_H_Z(
                pContainer, &sizeAvail, PackSize(0, nOffset), bVert);
            return sizeAvail.cy;
        }
        impl__CalcAvailableSpace_CPaneContainer__UEAA_AVCSize__V2_H_Z(
            pContainer, &sizeAvail, PackSize(nOffset, 0), bVert);
        return sizeAvail.cx;
    }
    return nOffset;
}

// Decoded from retail ?CalcAvailableSpace@CPaneContainer@@UEAA?AVCSize@@V2@H@Z,
// entry RVA 0xa6f00 (mfc140.dll).  Retail signature CSize(CSize, BOOL), so the
// registers are RCX = this, RDX = the return slot, R8 = the CSize by value,
// R9 = the BOOL; the generated stub had dropped `this`, and is corrected here.
//
//   CRect rect(0,0,0,0);
//   GetWindowRect(rect, FALSE);                    // vftable byte 0x28 == slot 5
//   int l = rect.left, t = rect.top, r = rect.right, b = rect.bottom;
//   if (bVert) { r += sizeAdd.cx; b += sizeAdd.cy; }   // 0xa6f69
//   else       { l += sizeAdd.cx; t += sizeAdd.cy; }   // 0xa6f70
//   CSize sizeMin(0,0);
//   GetMinSize(sizeMin);                           // vftable byte 0x30 == slot 6
//   ret = sizeAdd;
//   if (r - l < sizeMin.cx) {
//       int v = rect.Width() - sizeMin.cx;         // 0xa6fa3: the ORIGINAL width
//       ret.cx = v;
//       if (v < 0) { ret.cx = 0; v = 0; }
//       if (sizeAdd.cx < 0) ret.cx = -v;
//   }
//   if (b - t < sizeMin.cy) {
//       int w = (b - t) - sizeMin.cy;              // 0xa6fcb: the ADJUSTED height
//       ret.cy = w;
//       if (w < 0) { ret.cy = 0; w = 0; }
//       if (sizeAdd.cy < 0) ret.cy = -w;
//   }
//
// The cx/cy asymmetry on the two recompute lines is retail's, not a transcription
// slip: 0xa6fa3 reloads rect.right/rect.left from the untouched rect while
// 0xa6fcb reuses the already-adjusted height in ESI.
// Symbol: ?CalcAvailableSpace@CPaneContainer@@UEAA?AVCSize@@V2@H@Z
extern "C" void* MS_ABI impl__CalcAvailableSpace_CPaneContainer__UEAA_AVCSize__V2_H_Z(
    void* pThis, SIZE* pRet, long long sizeAdd, int bVert) {
    if (pRet == nullptr) return pRet;
    const int cxAdd = static_cast<int>(static_cast<unsigned int>(
        static_cast<unsigned long long>(sizeAdd) & 0xffffffffULL));
    const int cyAdd = static_cast<int>(static_cast<unsigned int>(
        static_cast<unsigned long long>(sizeAdd) >> 32));

    RECT rect = {0, 0, 0, 0};
    impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(pThis, &rect, FALSE);

    int left = rect.left, top = rect.top, right = rect.right, bottom = rect.bottom;
    if (bVert) {
        right += cxAdd;
        bottom += cyAdd;
    } else {
        left += cxAdd;
        top += cyAdd;
    }

    SIZE sizeMin = {0, 0};
    impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(pThis, &sizeMin);

    pRet->cx = cxAdd;
    pRet->cy = cyAdd;

    if ((right - left) < sizeMin.cx) {
        int v = (rect.right - rect.left) - sizeMin.cx;
        pRet->cx = v;
        if (v < 0) { pRet->cx = 0; v = 0; }
        if (cxAdd < 0) pRet->cx = -v;
    }
    int nHeight = bottom - top;
    if (nHeight < sizeMin.cy) {
        int w = nHeight - sizeMin.cy;
        pRet->cy = w;
        if (w < 0) { pRet->cy = 0; w = 0; }
        if (cyAdd < 0) pRet->cy = -w;
    }
    return pRet;
}

// Decoded from retail ?CalculateRecentSize@CPaneContainer@@QEAAXXZ, entry RVA
// 0xa52c0 (mfc140.dll).  Locals: rectThis at rbp-0x28, rectLeft at rbp-0x48,
// rectRight at rbp-0x38, szLeft at rbp-0x58, szRight at rbp-0x50.
//
//   CRect rectThis; ::SetRectEmpty(&rectThis);
//   BOOL bAll = <the manager/divider gate, see CountsHiddenPanes above>;
//   GetWindowRect(rectThis, FALSE);                             // slot 5
//   CRect rectLeft, rectRight; ::SetRectEmpty on both;
//   CSize szLeft(0,0), szRight(0,0);
//   // the same visible-or-bAll gather GetMinSize uses, into the four locals
//   ...
//   BOOL bCheck = bAll ^ 1;                                     // 0xa5471
//   BOOL bL = IsLeftPartEmpty(bCheck), bR = IsRightPartEmpty(bCheck);
//   if (!bL && bR) {                                            // 0xa5490
//       if (m_pLeftPane) m_pLeftPane-><+0x410> = 100;
//       if (m_pLeftPaneContainer) { left->CalculateRecentSize(); left->m_nPercent = 100; }
//       return;
//   }
//   if (bL && !bR) {                                            // 0xa54db, the mirror
//       if (m_pRightPane) m_pRightPane-><+0x410> = 100;
//       if (m_pRightPaneContainer) { right->CalculateRecentSize(); right->m_nPercent = 100; }
//       return;
//   }
//   if (bL || bR) return;                                       // both empty: 0xa5506
//   if (m_pSlider == NULL) AfxThrowInvalidArgException();       // 0xa552a -> 0x225b80
//   // pick the split axis, then work in (origin, farEdge, total, otherExtent)
//   BOOL bHorz = IsPaneDividerHorz();
//   int nOrigin, nFar, nTotal, nOther;
//   if (bHorz) { nOrigin = rectLeft.top;  nFar = rectLeft.bottom;              // 0xa554a
//                nOther  = rectRight.Height(); nTotal = rectThis.Height(); }
//   else       { nOrigin = rectLeft.left; nFar = rectLeft.right;               // 0xa5611
//                nOther  = rectRight.Width();  nTotal = rectThis.Width();  }
//   int nPct = -1;                                              // "derive from geometry"
//   if ((nFar - nOrigin) + nOther > nTotal) {                   // the parts overlap
//       if (nFar - nOrigin == nTotal) {                         // 0xa557b / 0xa5643
//           int q = m_pRightPane ? m_pRightPane-><+0x410>
//                 : m_pRightPaneContainer ? m_pRightPaneContainer->m_nPercent : 50;
//           nFar = nOrigin + nTotal - (nTotal * q) / 100;       // signed /100 at 0xa559d
//           nPct = 100 - q;
//       } else if (nOther == nTotal) {                          // 0xa55cd / 0xa5695
//           int q = m_pLeftPane ? m_pLeftPane-><+0x410>
//                 : m_pLeftPaneContainer ? m_pLeftPaneContainer->m_nPercent : 50;
//           nFar = nOrigin + (nTotal * q) / 100;
//           nPct = q;
//       } else if (bHorz) nPct = 50;                            // 0xa5572 + jne 0xa56e0
//       // in the !bHorz "neither" case retail leaves r8d == -1 (0xa5614) and
//       // falls through to the geometric percentage at 0xa56eb
//   }
//   int nPercent = (nPct == -1)
//       ? (int)((double)(nFar - nOrigin) / (double)nTotal * 100.0)   // 0xa56eb
//       : nPct;
//   if (m_pLeftPane)  m_pLeftPane-><+0x410> = nPercent;                        // 0xa5718
//   if (m_pLeftPaneContainer)  { left->CalculateRecentSize();  left->m_nPercent = nPercent; }
//   if (m_pRightPane) m_pRightPane-><+0x410> = 100 - nPercent;                 // 0xa5740
//   if (m_pRightPaneContainer) { right->CalculateRecentSize(); right->m_nPercent = 100 - nPercent; }
//
// The two "overlap" arms write the recomputed far edge back into rectLeft
// (0xa5609 / 0xa56d7) and it is that updated edge the geometric fallback would
// read -- but the fallback is only reached when nPct stayed -1, i.e. when neither
// arm ran, so nFar is the measured one in every path that uses it.
//
// The horizontal branch seeds its "neither part spans the whole container" case
// with 50 (`lea -0x32(%rdi),%r8d` at 0xa5572, edi being the literal 100) while
// the vertical branch leaves the sentinel and computes the percentage from the
// geometry.  That asymmetry is retail's, not a transcription slip.
//
// CDockablePane +0x410 is documented at PaneRecentPercent above; the gather
// inherits the CountsHiddenPanes / PaneMinSize deviations documented there.
// szLeft/szRight are collected exactly as retail collects them and, exactly as
// in retail, no later instruction reads them -- only the rects are used.
//
// DEVIATION: nTotal == 0 makes retail divide by zero in double and truncate the
// resulting infinity, which x86 turns into INT_MIN; a zero total yields 0 here.
// Symbol: ?CalculateRecentSize@CPaneContainer@@QEAAXXZ
extern "C" void MS_ABI impl__CalculateRecentSize_CPaneContainer__QEAAXXZ(void* pThis) {
    S_CPaneContainer* p = PC(pThis);

    RECT rectThis = {0, 0, 0, 0};
    ::SetRectEmpty(&rectThis);
    const int bAll = CountsHiddenPanes(p);
    impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(p, &rectThis, FALSE);

    RECT rectLeft = {0, 0, 0, 0};
    RECT rectRight = {0, 0, 0, 0};
    ::SetRectEmpty(&rectLeft);
    ::SetRectEmpty(&rectRight);
    SIZE szLeft = {0, 0};
    SIZE szRight = {0, 0};

    if (p->m_pLeftPane != nullptr && (PaneIsVisible(p->m_pLeftPane) || bAll)) {
        ::GetWindowRect(PaneHwnd(p->m_pLeftPane), &rectLeft);
        PaneMinSize(p->m_pLeftPane, &szLeft);
    }
    if (p->m_pLeftPaneContainer != nullptr &&
        (impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pLeftPaneContainer) || bAll)) {
        impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(p->m_pLeftPaneContainer, &rectLeft, FALSE);
        impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(p->m_pLeftPaneContainer, &szLeft);
    }
    if (p->m_pRightPane != nullptr && (PaneIsVisible(p->m_pRightPane) || bAll)) {
        ::GetWindowRect(PaneHwnd(p->m_pRightPane), &rectRight);
        PaneMinSize(p->m_pRightPane, &szRight);
    }
    if (p->m_pRightPaneContainer != nullptr &&
        (impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pRightPaneContainer) || bAll)) {
        impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(p->m_pRightPaneContainer, &rectRight, FALSE);
        impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(p->m_pRightPaneContainer, &szRight);
    }
    (void)szLeft;
    (void)szRight;

    const int bCheckVisibility = bAll ^ 1;
    const int bLeftEmpty = impl__IsLeftPartEmpty_CPaneContainer__QEBAHH_Z(p, bCheckVisibility);
    const int bRightEmpty = impl__IsRightPartEmpty_CPaneContainer__QEBAHH_Z(p, bCheckVisibility);

    if (!bLeftEmpty && bRightEmpty) {
        if (p->m_pLeftPane != nullptr) PaneRecentPercent(p->m_pLeftPane) = 100;
        if (p->m_pLeftPaneContainer != nullptr) {
            impl__CalculateRecentSize_CPaneContainer__QEAAXXZ(p->m_pLeftPaneContainer);
            p->m_pLeftPaneContainer->m_nPercent = 100;
        }
        return;
    }
    if (bLeftEmpty && !bRightEmpty) {
        if (p->m_pRightPane != nullptr) PaneRecentPercent(p->m_pRightPane) = 100;
        if (p->m_pRightPaneContainer != nullptr) {
            impl__CalculateRecentSize_CPaneContainer__QEAAXXZ(p->m_pRightPaneContainer);
            p->m_pRightPaneContainer->m_nPercent = 100;
        }
        return;
    }
    if (bLeftEmpty || bRightEmpty) return;      // both halves empty

    if (p->m_pSlider == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }

    const int bHorz = impl__IsPaneDividerHorz_CPaneContainer__IEBAHXZ(p);
    int nOrigin, nFar, nTotal, nOther;
    if (bHorz) {
        nOrigin = rectLeft.top;
        nFar    = rectLeft.bottom;
        nOther  = rectRight.bottom - rectRight.top;
        nTotal  = rectThis.bottom - rectThis.top;
    } else {
        nOrigin = rectLeft.left;
        nFar    = rectLeft.right;
        nOther  = rectRight.right - rectRight.left;
        nTotal  = rectThis.right - rectThis.left;
    }

    int nPct = -1;
    if ((nFar - nOrigin) + nOther > nTotal) {
        if (nFar - nOrigin == nTotal) {
            int q = 50;
            if (p->m_pRightPane != nullptr) q = PaneRecentPercent(p->m_pRightPane);
            else if (p->m_pRightPaneContainer != nullptr) q = p->m_pRightPaneContainer->m_nPercent;
            nFar = nOrigin + nTotal - (nTotal * q) / 100;
            nPct = 100 - q;
        } else if (nOther == nTotal) {
            int q = 50;
            if (p->m_pLeftPane != nullptr) q = PaneRecentPercent(p->m_pLeftPane);
            else if (p->m_pLeftPaneContainer != nullptr) q = p->m_pLeftPaneContainer->m_nPercent;
            nFar = nOrigin + (nTotal * q) / 100;
            nPct = q;
        } else if (bHorz) {
            nPct = 50;
        }
    }

    int nPercent;
    if (nPct != -1) {
        nPercent = nPct;
    } else if (nTotal != 0) {
        nPercent = static_cast<int>(static_cast<double>(nFar - nOrigin) /
                                    static_cast<double>(nTotal) * 100.0);
    } else {
        nPercent = 0;
    }

    if (p->m_pLeftPane != nullptr) PaneRecentPercent(p->m_pLeftPane) = nPercent;
    if (p->m_pLeftPaneContainer != nullptr) {
        impl__CalculateRecentSize_CPaneContainer__QEAAXXZ(p->m_pLeftPaneContainer);
        p->m_pLeftPaneContainer->m_nPercent = nPercent;
    }
    if (p->m_pRightPane != nullptr) PaneRecentPercent(p->m_pRightPane) = 100 - nPercent;
    if (p->m_pRightPaneContainer != nullptr) {
        impl__CalculateRecentSize_CPaneContainer__QEAAXXZ(p->m_pRightPaneContainer);
        p->m_pRightPaneContainer->m_nPercent = 100 - nPercent;
    }
}

// Decoded from retail ?CheckPaneDividerVisibility@CPaneContainer@@QEAAXXZ,
// entry RVA 0xa71c0 (mfc140.dll). Computes four visibility flags -- the two
// panes' WS_VISIBLE bits via CWnd::GetStyle (0x2a75a0) and the two child
// containers' IsVisible() after recursing into them -- and, only when a divider
// is present, calls CWnd::ShowWindow(SW_SHOW / SW_HIDE) on it. The
// `neg/sbb/and $5` tail at 0xa726b is what turns the flag into 5 (SW_SHOW) or
// 0 (SW_HIDE).
// Symbol: ?CheckPaneDividerVisibility@CPaneContainer@@QEAAXXZ
extern "C" void MS_ABI impl__CheckPaneDividerVisibility_CPaneContainer__QEAAXXZ(void* pThis) {
    S_CPaneContainer* p = PC(pThis);

    int bLeftPaneVisible = 0;
    if (p->m_pLeftPane != nullptr &&
        (impl__GetStyle_CWnd__QEBAKXZ(reinterpret_cast<const CWnd*>(p->m_pLeftPane)) & WS_VISIBLE) != 0) {
        bLeftPaneVisible = 1;
    }
    int bRightPaneVisible = 0;
    if (p->m_pRightPane != nullptr &&
        (impl__GetStyle_CWnd__QEBAKXZ(reinterpret_cast<const CWnd*>(p->m_pRightPane)) & WS_VISIBLE) != 0) {
        bRightPaneVisible = 1;
    }

    int bLeftContainerVisible = 0;
    if (p->m_pLeftPaneContainer != nullptr) {
        impl__CheckPaneDividerVisibility_CPaneContainer__QEAAXXZ(p->m_pLeftPaneContainer);
        bLeftContainerVisible = impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pLeftPaneContainer);
    }
    int bRightContainerVisible = 0;
    if (p->m_pRightPaneContainer != nullptr) {
        impl__CheckPaneDividerVisibility_CPaneContainer__QEAAXXZ(p->m_pRightPaneContainer);
        bRightContainerVisible = impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pRightPaneContainer);
    }

    if (p->m_pSlider == nullptr) return;

    const int bShow =
        (bLeftPaneVisible && (bRightPaneVisible || bRightContainerVisible)) ||
        (bRightPaneVisible && bLeftContainerVisible) ||
        (bLeftContainerVisible && bRightContainerVisible);

    impl__ShowWindow_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(p->m_pSlider), bShow ? SW_SHOW : SW_HIDE);
}

// STUB. Retail ?CleanUp@CPaneContainer@@IEAAXXZ, entry RVA 0xa3910 (mfc140.dll),
// fully decoded:
//
//   if (m_pLeftPaneContainer) {
//       m_pLeftPaneContainer->CleanUp();                       // 0xa3922, direct call
//       m_pLeftPaneContainer-><vftable byte 0x08>(1);          // scalar deleting dtor
//       m_pLeftPaneContainer = NULL;                           // 0xa3942
//   }
//   ... the same three steps for m_pRightPaneContainer (0xa394a) ...
//   if (m_pSlider && m_pSlider-><+0x1c4> == 0 && m_pSlider->m_hWnd != NULL) {
//       m_pSlider-><vftable byte 0xd0>();                      // CWnd::DestroyWindow
//       m_pSlider = NULL;                                      // 0xa39a4
//   }
//
// Note that m_pSlider is cleared only on the path that actually destroys the
// window; the two guards at 0xa3984 / 0xa398d skip the assignment as well.
//
// Blocked on both halves.  Byte 0x08 of the CPaneContainer vftable (0x2f4070) is
// RVA 0xa3860, the scalar deleting destructor, i.e. `~CPaneContainer(); operator
// delete(this)`.  Nothing in OpenMFC allocates a CPaneContainer (see the file
// header), so this DLL cannot know which heap a node came from, and calling
// ::operator delete on it would be a cross-allocator free; unlinking the
// children without freeing them would silently leak instead.  The divider half
// needs CPaneDivider +0x1c4 and CWnd::m_hWnd, neither of which OpenMFC's
// CObject-shaped CPaneDivider has (see PaneDividerHwnd above).
// Only the parameter list is corrected, to the (this) shape the export ABI
// passes; the body stays a stub for the reasons above.
// Symbol: ?CleanUp@CPaneContainer@@IEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CPaneContainer__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// STUB. Retail ?Copy@CPaneContainer@@UEAAPEAV1@PEAV1@@Z, entry RVA 0xa7ca0
// (mfc140.dll).  An earlier revision of this comment said the clone's
// CRuntimeClass comes from "CPaneContainerManager vtable slot 16"; it does not --
// it is the plain member m_pContainerRTC at manager +0x80 (the load at 0xa7cb9),
// and the clone is built with CRuntimeClass::CreateObject (call 0x233380):
//
//   CRuntimeClass* pRTC = m_pContainerManager->m_pContainerRTC;   // +0x80
//   CPaneContainer* pNew;
//   if (pRTC == NULL) {
//       void* p = operator new(0xf8);                             // 0xa7cc5
//       if (p == NULL) pNew = NULL;   // 0xa7cf8: retail does NOT return here,
//                                     // it falls into the code below with a
//                                     // NULL pNew and would fault at 0xa7d42
//       else pNew = new(p) CPaneContainer(m_pContainerManager, m_pLeftPane,
//                                    m_pRightPane, m_pSlider);    // call 0xa3750
//   } else {
//       pNew = (CPaneContainer*)pRTC->CreateObject();
//       pNew->m_pContainerManager = m_pContainerManager;          // 0xa7d05..0xa7d1d
//       pNew->m_pLeftPane = m_pLeftPane; pNew->m_pRightPane = m_pRightPane;
//       pNew->m_pSlider = m_pSlider;
//   }
//   // Direction check, because it is easy to get backwards: `bt $0x1c,%eax`
//   // sets CF from WS_VISIBLE and `jae` is taken when the bit is CLEAR.  So a
//   // WS_VISIBLE pane MOVES TO THE CLONE -- retail clears it from the ORIGINAL
//   // at 0xa7d38 (`movq $0,0x8(%rdi)`, rdi == this) -- while a hidden pane
//   // STAYS WITH THE ORIGINAL and is cleared from the clone at 0xa7d42
//   // (`movq $0,0x8(%rbx)`, rbx == pNew).  Same shape for m_pRightPane.
//   pNew->m_pParentContainer = pParentContainer;                  // 0xa7d70
//   if (m_pLeftPaneContainer)  { pNew->m_pLeftPaneContainer  = left->Copy(pNew);  ... }
//   if (m_pRightPaneContainer) { pNew->m_pRightPaneContainer = right->Copy(pNew); ... }
//   if (m_pSlider) {
//       if (m_pSlider->GetStyle() & WS_VISIBLE) {                 // detach it
//           this->m_nField60 = m_pSlider-><+0x1ac>;               // 0xa7de8
//           ::GetClientRect(m_pSlider->m_hWnd, &this->m_rect64);  // IAT 0x2c5358
//           this->m_bDividerHorz = m_pSlider-><vftable byte 0x2d8>();  // 0xa7e09
//           this->m_pSlider = NULL;
//       } else pNew->m_pSlider = NULL;
//   }
//   return pNew;
//
// Blocked.  Both allocation paths hand back an object whose +0x00 retail fills
// with the CPaneContainer vftable and OpenMFC's exported constructor does not
// write at all, so a client that dispatches on the clone would jump through
// whatever operator new left there; and the divider-detach arm needs
// CPaneDivider +0x1ac, m_hWnd and vftable byte 0x2d8, none of which OpenMFC's
// CObject-shaped CPaneDivider has.  Only the parameter list is corrected, to the
// (this, pParentContainer) shape the export ABI passes.
// Symbol: ?Copy@CPaneContainer@@UEAAPEAV1@PEAV1@@Z
extern "C" void* MS_ABI impl__Copy_CPaneContainer__UEAAPEAV1_PEAV1__Z(
    void* pThis, void* pParentContainer) {
    (void)pThis;
    (void)pParentContainer;
    return nullptr;
}

// Decoded from retail ?DeletePane@CPaneContainer@@UEAAXPEAVCDockablePane@@W4BC_FIND_CRITERIA@1@@Z,
// entry RVA 0xa4ee0 (mfc140.dll).  The generated argument list was pure noise
// (nineteen invented parameters); the real shape is (this, pPane, criteria).
//
//   CRect rectThis(0,0,0,0);  GetWindowRect(rectThis, FALSE);      // slot 5
//   CRect rectPane(0,0,0,0);  ::GetWindowRect(pPane->m_hWnd, &rectPane);
//   if (m_pSlider) IsPaneDividerHorz();          // 0xa4f4b -- result discarded
//   HDWP hdwp = ::BeginDeferWindowPos(10);       // IAT 0x2c4d28
//   CDockablePane*  pSurvivorPane  = NULL;
//   CPaneContainer* pSurvivorChild = NULL;
//   if (criteria == 0 && pPane == m_pLeftPane) {                   // 0xa4f63
//       pSurvivorPane = m_pRightPane;  m_pLeftPane = NULL;
//       if (!pSurvivorPane) pSurvivorChild = m_pRightPaneContainer;
//   } else if (criteria == 1 && pPane == m_pRightPane) {           // 0xa4fbf
//       pSurvivorPane = m_pLeftPane;   m_pRightPane = NULL;
//       if (!pSurvivorPane) pSurvivorChild = m_pLeftPaneContainer;
//   } else goto done;                             // any other criteria: nothing
//   if (pSurvivorPane) {
//       pSurvivorPane-><vftable byte 0x560>(rectThis, FALSE, hdwp);  // MovePane
//   } else if (pSurvivorChild && !pSurvivorChild->IsEmpty()) {       // 0xa4fe3
//       m_pContainerManager-><byte 0xf8>()->ScreenToClient(&rectThis);
//       this->Resize(rectThis, hdwp, FALSE);      // slot 20 -- on THIS, not the child
//   } else {                                      // 0xa5038: climb to a live ancestor
//       CPaneContainer* pUp = m_pParentContainer;
//       while (pUp && pUp->IsEmpty()) pUp = pUp->m_pParentContainer;
//       if (pUp && pUp->m_pSlider) {
//           CPaneDivider* pDiv = pUp->m_pSlider;
//           int nExtent = pDiv-><byte 0x2d8>() ? rectPane.Height() : rectPane.Width();
//           int nStep   = pDiv-><+0x1b0> + 2 * (nExtent + 1);        // 0xa5096
//           if (pUp->IsLeftPartEmpty(FALSE))                          // 0xa509d
//               pUp->StretchPaneContainer(-nStep, pDiv-><0x2d8>() == 0, 0, TRUE, hdwp);
//           else if (pUp->IsRightPartEmpty(FALSE))                    // 0xa50d6
//               pUp->StretchPaneContainer( nStep, pDiv-><0x2d8>() == 0, 1, TRUE, hdwp);
//       }
//   }
// done:
//   ::EndDeferWindowPos(hdwp);                    // IAT 0x2c4d30
//   if (m_pSlider == NULL) {                      // 0xa5132
//       m_pRightPane = m_pLeftPane = NULL;
//       m_pRightPaneContainer = m_pLeftPaneContainer = NULL;
//   }
//
// Note the BeginDeferWindowPos/EndDeferWindowPos pair and the final link clear
// run on EVERY path, including the "criteria did not match" one.
//
// Byte 0x2d8 of the CPaneDivider vftable is the non-exported orientation getter
// that ?IsPaneDividerHorz@ (0xa7e40) tail-jumps to when m_pSlider is non-null --
// and every use of it here is inside such a test -- so IsPaneDividerHorz on the
// owning container is the exact equivalent, and is what OpenMFC can evaluate
// (see its comment above).  CPaneDivider +0x1b0 has no counterpart here, so
// kPaneDividerThickness stands in, as it does in ?GetMinSize@.
//
// ?MovePane@CPane@ (in docking/CPane.cpp, still returning CSize(0,0) as of
// 2026-09-08), ?Resize@ and ?StretchPaneContainer@ (both below) are stubs, so
// the repositioning half of this body currently reaches no-ops; the pointer
// surgery and the deferred-window-position bracket are real.
//
// TWO DEVIATIONS: retail dereferences pPane without a NULL test at 0xa4f2b, and
// pPane is NULL-checked here; and retail's discarded ?IsPaneDividerHorz@ call at
// 0xa4f4b is not reproduced, because OpenMFC's IsPaneDividerHorz is a pure read
// of the cached +0x54 flag and so cannot have the side effect a client override
// of the retail virtual might.
// Symbol: ?DeletePane@CPaneContainer@@UEAAXPEAVCDockablePane@@W4BC_FIND_CRITERIA@1@@Z
extern "C" void MS_ABI impl__DeletePane_CPaneContainer__UEAAXPEAVCDockablePane__W4BC_FIND_CRITERIA_1__Z(
    void* pThis, void* pPane, int criteria) {
    S_CPaneContainer* p = PC(pThis);

    RECT rectThis = {0, 0, 0, 0};
    impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(p, &rectThis, FALSE);
    RECT rectPane = {0, 0, 0, 0};
    if (pPane != nullptr) {
        ::GetWindowRect(PaneHwnd(pPane), &rectPane);
    }

    void* hdwp = ::BeginDeferWindowPos(10);

    void* pSurvivorPane = nullptr;
    S_CPaneContainer* pSurvivorChild = nullptr;
    bool bMatched = false;
    if (criteria == BC_FIND_LEFT_PANE) {
        if (p->m_pLeftPane == pPane) {
            bMatched = true;
            pSurvivorPane = p->m_pRightPane;
            p->m_pLeftPane = nullptr;
            if (pSurvivorPane == nullptr) pSurvivorChild = p->m_pRightPaneContainer;
        }
    } else if (criteria == BC_FIND_RIGHT_PANE) {
        if (p->m_pRightPane == pPane) {
            bMatched = true;
            pSurvivorPane = p->m_pLeftPane;
            p->m_pRightPane = nullptr;
            if (pSurvivorPane == nullptr) pSurvivorChild = p->m_pLeftPaneContainer;
        }
    }

    if (bMatched) {
        if (pSurvivorPane != nullptr) {
            SIZE sizeMoved = {0, 0};
            RECT rectArg = rectThis;
            impl__MovePane_CPane__UEAA_AVCSize__VCRect__HAEAPEAX_Z(
                reinterpret_cast<CPane*>(pSurvivorPane), &sizeMoved, &rectArg, FALSE, &hdwp);
        } else if (pSurvivorChild != nullptr &&
                   !impl__IsEmpty_CPaneContainer__QEBAHXZ(pSurvivorChild)) {
            CWnd* pDockSite = ManagerDockSite(p);
            if (pDockSite != nullptr) {
                impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pDockSite, &rectThis);
            }
            impl__Resize_CPaneContainer__UEAAXVCRect__AEAPEAXH_Z(p, &rectThis, &hdwp, FALSE);
        } else {
            S_CPaneContainer* pUp = p->m_pParentContainer;
            while (pUp != nullptr && impl__IsEmpty_CPaneContainer__QEBAHXZ(pUp)) {
                pUp = pUp->m_pParentContainer;
            }
            if (pUp != nullptr && pUp->m_pSlider != nullptr) {
                const int bUpHorz = impl__IsPaneDividerHorz_CPaneContainer__IEBAHXZ(pUp);
                const int nExtent = bUpHorz ? (rectPane.bottom - rectPane.top)
                                            : (rectPane.right - rectPane.left);
                const int nStep = kPaneDividerThickness + 2 * (nExtent + 1);
                if (impl__IsLeftPartEmpty_CPaneContainer__QEBAHH_Z(pUp, FALSE)) {
                    impl__StretchPaneContainer_CPaneContainer__UEAAHHHHHAEAPEAX_Z(
                        pUp, -nStep, bUpHorz == 0, 0, TRUE, &hdwp);
                } else if (impl__IsRightPartEmpty_CPaneContainer__QEBAHH_Z(pUp, FALSE)) {
                    impl__StretchPaneContainer_CPaneContainer__UEAAHHHHHAEAPEAX_Z(
                        pUp, nStep, bUpHorz == 0, 1, TRUE, &hdwp);
                }
            }
        }
    }

    ::EndDeferWindowPos(static_cast<HDWP>(hdwp));

    if (p->m_pSlider == nullptr) {
        p->m_pRightPane = nullptr;
        p->m_pLeftPane = nullptr;
        p->m_pRightPaneContainer = nullptr;
        p->m_pLeftPaneContainer = nullptr;
    }
}

// Decoded from retail ?FindSubPaneContainer@CPaneContainer@@QEAAPEAV1@PEBVCObject@@W4BC_FIND_CRITERIA@1@@Z,
// entry RVA 0xa5230 (mfc140.dll). A null pObject takes the
// AfxThrowInvalidArgException path at 0xa52b9. The criteria switch compares
// m_pLeftPane / m_pRightPane / m_pSlider / this respectively; on a hit the node
// itself is returned, otherwise the left then the right sub-container is
// searched with the same criteria.
// Symbol: ?FindSubPaneContainer@CPaneContainer@@QEAAPEAV1@PEBVCObject@@W4BC_FIND_CRITERIA@1@@Z
extern "C" void* MS_ABI impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
    void* pThis, const void* pObject, int criteria) {
    S_CPaneContainer* p = PC(pThis);
    if (pObject == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }

    switch (criteria) {
    case BC_FIND_LEFT_PANE:  if (p->m_pLeftPane == pObject) return p; break;
    case BC_FIND_RIGHT_PANE: if (p->m_pRightPane == pObject) return p; break;
    case BC_FIND_SLIDER:     if (p->m_pSlider == pObject) return p; break;
    case BC_FIND_CONTAINER:  if (static_cast<const void*>(p) == pObject) return p; break;
    default: break;
    }

    void* pFound = nullptr;
    if (p->m_pLeftPaneContainer != nullptr) {
        pFound = impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
            p->m_pLeftPaneContainer, pObject, criteria);
    }
    if (pFound == nullptr && p->m_pRightPaneContainer != nullptr) {
        pFound = impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
            p->m_pRightPaneContainer, pObject, criteria);
    }
    return pFound;
}

// Decoded from retail ?FindTabbedPane@CPaneContainer@@QEAAPEAVCDockablePane@@I@Z,
// entry RVA 0xa7b50 (mfc140.dll):
//
//   CPaneContainer* p = this;
//   for (;;) {
//       if (Find(&p->m_lstLeftSiblingIDs,  nID)) return p->m_pLeftPane;   // 0xa7bac
//       if (Find(&p->m_lstRightSiblingIDs, nID)) return p->m_pRightPane;  // 0xa7bbf
//       if (p->m_pLeftPaneContainer) {
//           CDockablePane* pFound = p->m_pLeftPaneContainer->FindTabbedPane(nID);
//           if (pFound) return pFound;                                    // 0xa7b8e
//       }
//       p = p->m_pRightPaneContainer;                                     // 0xa7b90
//       if (p == NULL) return NULL;
//   }
//
// Find is the non-exported CList<UINT,UINT>::Find at 0x11f3c, transcribed as
// FindSiblingID above; the recursion is the direct entry point
// (`call 0x1800a7b50` at 0xa7b86), this method not being virtual.
//
// The two sibling-id lists this walks are only ever filled by ?Serialize@,
// ?SaveTabbedPane@ and ?LoadTabbedPane@, all of which are still stubs in this
// file, and the constructor above leaves both lists with a NULL head.  So the
// walk is complete and correct but currently finds nothing on any node.
// Symbol: ?FindTabbedPane@CPaneContainer@@QEAAPEAVCDockablePane@@I@Z
extern "C" void* MS_ABI impl__FindTabbedPane_CPaneContainer__QEAAPEAVCDockablePane__I_Z(
    void* pThis, unsigned int nID) {
    S_CPaneContainer* p = PC(pThis);
    while (p != nullptr) {
        if (FindSiblingID(&p->m_lstLeftSiblingIDs, nID) != nullptr) return p->m_pLeftPane;
        if (FindSiblingID(&p->m_lstRightSiblingIDs, nID) != nullptr) return p->m_pRightPane;
        if (p->m_pLeftPaneContainer != nullptr) {
            void* pFound = impl__FindTabbedPane_CPaneContainer__QEAAPEAVCDockablePane__I_Z(
                p->m_pLeftPaneContainer, nID);
            if (pFound != nullptr) return pFound;
        }
        p = p->m_pRightPaneContainer;
    }
    return nullptr;
}

// STUB. Retail ?FreeReleasedPaneContainer@CPaneContainer@@IEAAXXZ, entry RVA
// 0xa4d10 (mfc140.dll), decoded:
//
//   int nUsed = (m_pLeftPane != 0) + (m_pRightPane != 0)          // 0xa4d1f..0xa4d46
//             + (m_pLeftPaneContainer != 0) + (m_pRightPaneContainer != 0);
//   if (nUsed > 1) return;                                        // 0xa4d4a
//   if (m_nRefCount > 0) return;                                  // 0xa4d53
//   if (m_pSlider && m_pSlider-><+0x1c4> != 0)      goto notify;   // 0xa4d65
//   if (m_pParentContainer == NULL)                 goto notify;   // 0xa4d78
//   if (m_pParentContainer == m_pContainerManager->m_pRootContainer) goto notify;
//   // otherwise: drop this node out of the parent, promoting its single
//   // survivor into the slot the parent used to point at (0xa4d8c..0xa4e38),
//   // then destroy the divider window through CPaneDivider vftable byte 0xd0,
//   // set m_bReleased (+0x5c) = 1, and park the node on a module-global
//   // CPtrList (the ?AddTail@CPtrList@ call at 0xa4e6a onto the object at
//   // 0x1803aad58) for deferred destruction.
//   notify: m_pContainerManager->NotifyPaneDivider();             // 0xaab50
//
// Blocked on the very first decision.  CPaneDivider +0x1c4 is the DWORD
// ?Init@CPaneDivider@@QEAAXHPEAVCWnd@@@Z (entry RVA 0xab720) stores its first
// argument into at 0xab757 -- the same word ?CleanUp@ above tests before
// destroying the divider window -- and OpenMFC's CObject-shaped CPaneDivider has
// no storage for it (see PaneDividerHwnd above).  Guessing it wrong in the
// "not distinguished" direction would unlink a node retail only notifies about,
// which rewrites the parent's child and pane slots: tree corruption, not a
// no-op.  The deferred-free CPtrList has no counterpart here either.
// ?Release@ below still calls this exactly where retail does.
// Symbol: ?FreeReleasedPaneContainer@CPaneContainer@@IEAAXXZ
extern "C" void MS_ABI impl__FreeReleasedPaneContainer_CPaneContainer__IEAAXXZ(void* pThis) { (void)pThis; }

// Decoded from retail ?GetAssociatedSiblingPaneIDs@CPaneContainer@@QEAAPEAV?$CList@II@@PEAVCDockablePane@@@Z,
// entry RVA 0xa7bd0 (mfc140.dll): returns this+0x88 when pPane is the left
// pane, this+0xc0 when it is the right pane, and NULL otherwise. Pure address
// arithmetic -- no CList operation is performed.
// Symbol: ?GetAssociatedSiblingPaneIDs@CPaneContainer@@QEAAPEAV?$CList@II@@PEAVCDockablePane@@@Z
extern "C" void* MS_ABI impl__GetAssociatedSiblingPaneIDs_CPaneContainer__QEAAPEAV__CList_II__PEAVCDockablePane___Z(
    void* pThis, void* pPane) {
    S_CPaneContainer* p = PC(pThis);
    if (p->m_pLeftPane == pPane) return &p->m_lstLeftSiblingIDs;
    if (p->m_pRightPane == pPane) return &p->m_lstRightSiblingIDs;
    return nullptr;
}

// Decoded from retail ?GetMinSize@CPaneContainer@@UEBAXAEAVCSize@@@Z, entry RVA
// 0xa3ca0 (mfc140.dll).
//
//   if (m_pContainerManager == NULL) AfxThrowInvalidArgException();  // 0xa3cba
//                                                                    // -> 0x225b80
//   size = CSize(0,0);
//   bAll = <the manager/divider gate, see CountsHiddenPanes above>;
//   szLeftPane / szRightPane   <- pane-><vftable byte 0x4f0>(), each taken only
//                                 when the pane is WS_VISIBLE or bAll;
//   szLeftCont / szRightCont   <- child-><slot 6>(), each taken only when the
//                                 child IsVisible() or bAll;
//   if (m_pSlider && ((m_pSlider->GetStyle() & WS_VISIBLE) || bAll)) {
//       if (IsPaneDividerHorz()) {            // 0xa7e40
//           size.cx = max of the four cx;
//           size.cy = m_pSlider-><+0x1b0> + the four cy;   // 0xa3e10
//       } else {
//           size.cy = max of the four cy;
//           size.cx = m_pSlider-><+0x1b0> + the four cx;   // 0xa3e49
//       }
//   } else {                                                // 0xa3e5f
//       size.cx = max(szLeftPane.cx, szRightPane.cx);
//       size.cy = max(szLeftPane.cy, szRightPane.cy);
//       if (m_pLeftPaneContainer  && left->IsVisible())  size = szLeftCont;
//       if (m_pRightPaneContainer && right->IsVisible()) size = szRightCont;
//   }
//
// Note that the last two assignments overwrite the whole CSize (8-byte moves at
// 0xa3e8e / 0xa3ea7) and are gated on IsVisible() alone -- bAll is not consulted
// there.  Byte 0x30 of the CPaneContainer vftable (0x2f4070) is slot 6, GetMinSize
// itself; retail recurses through the vftable, this file calls the entry point.
//
// DEVIATION, on top of the two documented at CountsHiddenPanes / PaneMinSize:
// retail adds the divider's own extent, the int at CPaneDivider +0x1b0.  OpenMFC's
// CPaneDivider has no retail-shaped storage (see CountsHiddenPanes), so
// kPaneDividerThickness -- the divider extent the rest of OpenMFC's docking code
// uses, defined in phase4/src/detail/CbarcoreSupport.h and applied by
// ?AddPane@CPaneDivider@ -- is substituted for it.
// Symbol: ?GetMinSize@CPaneContainer@@UEBAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(
    const void* pThis, SIZE* pSize) {
    const S_CPaneContainer* p = PC(pThis);
    if (p->m_pContainerManager == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (pSize == nullptr) return;
    pSize->cx = 0;
    pSize->cy = 0;

    const int bAll = CountsHiddenPanes(p);

    SIZE szLeftPane = {0, 0}, szRightPane = {0, 0};
    SIZE szLeftCont = {0, 0}, szRightCont = {0, 0};

    if (p->m_pLeftPane != nullptr && (PaneIsVisible(p->m_pLeftPane) || bAll)) {
        PaneMinSize(p->m_pLeftPane, &szLeftPane);
    }
    if (p->m_pRightPane != nullptr && (PaneIsVisible(p->m_pRightPane) || bAll)) {
        PaneMinSize(p->m_pRightPane, &szRightPane);
    }
    if (p->m_pLeftPaneContainer != nullptr &&
        (impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pLeftPaneContainer) || bAll)) {
        impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(p->m_pLeftPaneContainer, &szLeftCont);
    }
    if (p->m_pRightPaneContainer != nullptr &&
        (impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pRightPaneContainer) || bAll)) {
        impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(p->m_pRightPaneContainer, &szRightCont);
    }

    if (p->m_pSlider != nullptr &&
        ((impl__GetStyle_CWnd__QEBAKXZ(reinterpret_cast<const CWnd*>(p->m_pSlider)) & WS_VISIBLE) || bAll)) {
        const int nDivider = kPaneDividerThickness;   // stands in for m_pSlider->+0x1b0
        if (impl__IsPaneDividerHorz_CPaneContainer__IEBAHXZ(p)) {
            pSize->cx = MaxI(MaxI(szRightPane.cx, szLeftPane.cx),
                             MaxI(szLeftCont.cx, szRightCont.cx));
            pSize->cy = nDivider + szRightCont.cy + szLeftCont.cy + szRightPane.cy + szLeftPane.cy;
        } else {
            pSize->cy = MaxI(MaxI(szRightPane.cy, szLeftPane.cy),
                             MaxI(szLeftCont.cy, szRightCont.cy));
            pSize->cx = nDivider + szRightCont.cx + szLeftCont.cx + szRightPane.cx + szLeftPane.cx;
        }
        return;
    }

    pSize->cx = MaxI(szRightPane.cx, szLeftPane.cx);
    pSize->cy = MaxI(szRightPane.cy, szLeftPane.cy);
    if (p->m_pLeftPaneContainer != nullptr &&
        impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pLeftPaneContainer)) {
        *pSize = szLeftCont;
    }
    if (p->m_pRightPaneContainer != nullptr &&
        impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pRightPaneContainer)) {
        *pSize = szRightCont;
    }
}

// Decoded from retail ?GetMinSizeLeft@CPaneContainer@@UEBAXAEAVCSize@@@Z, entry
// RVA 0xa3ec0 (mfc140.dll):
//
//   bAll = <the manager/divider gate, see CountsHiddenPanes above>;
//   CSize szPane(0,0);
//   if (m_pLeftPane && ((m_pLeftPane->GetStyle() & WS_VISIBLE) || bAll))
//       m_pLeftPane-><vftable byte 0x4f0>(szPane);
//   CSize szCont(0,0);
//   if (m_pLeftPaneContainer && (left->IsVisible() || bAll))
//       left-><slot 6 == GetMinSize>(szCont);
//   size.cx = max(szPane.cx, szCont.cx);   // cmovg pair at 0xa3f73
//   size.cy = max(szPane.cy, szCont.cy);
//
// Note that the child call is GetMinSize (byte 0x30 of the CPaneContainer vftable
// 0x2f4070), not GetMinSizeLeft.  Retail dereferences m_pContainerManager without
// a NULL check here; CountsHiddenPanes makes that read unnecessary.
// Inherits the deviations documented at CountsHiddenPanes and PaneMinSize.
// Symbol: ?GetMinSizeLeft@CPaneContainer@@UEBAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetMinSizeLeft_CPaneContainer__UEBAXAEAVCSize___Z(
    const void* pThis, SIZE* pSize) {
    const S_CPaneContainer* p = PC(pThis);
    if (pSize == nullptr) return;
    const int bAll = CountsHiddenPanes(p);

    SIZE szPane = {0, 0};
    if (p->m_pLeftPane != nullptr && (PaneIsVisible(p->m_pLeftPane) || bAll)) {
        PaneMinSize(p->m_pLeftPane, &szPane);
    }
    SIZE szCont = {0, 0};
    if (p->m_pLeftPaneContainer != nullptr &&
        (impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pLeftPaneContainer) || bAll)) {
        impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(p->m_pLeftPaneContainer, &szCont);
    }
    pSize->cx = MaxI(szPane.cx, szCont.cx);
    pSize->cy = MaxI(szPane.cy, szCont.cy);
}

// Decoded from retail ?GetMinSizeRight@CPaneContainer@@UEBAXAEAVCSize@@@Z, entry
// RVA 0xa3fa0 (mfc140.dll): instruction-for-instruction the mirror of
// GetMinSizeLeft on m_pRightPane (+0x10) and m_pRightPaneContainer (+0x28), with
// the same GetMinSize (byte 0x30) child call.  Same deviations.
// Symbol: ?GetMinSizeRight@CPaneContainer@@UEBAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetMinSizeRight_CPaneContainer__UEBAXAEAVCSize___Z(
    const void* pThis, SIZE* pSize) {
    const S_CPaneContainer* p = PC(pThis);
    if (pSize == nullptr) return;
    const int bAll = CountsHiddenPanes(p);

    SIZE szPane = {0, 0};
    if (p->m_pRightPane != nullptr && (PaneIsVisible(p->m_pRightPane) || bAll)) {
        PaneMinSize(p->m_pRightPane, &szPane);
    }
    SIZE szCont = {0, 0};
    if (p->m_pRightPaneContainer != nullptr &&
        (impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pRightPaneContainer) || bAll)) {
        impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(p->m_pRightPaneContainer, &szCont);
    }
    pSize->cx = MaxI(szPane.cx, szCont.cx);
    pSize->cy = MaxI(szPane.cy, szCont.cy);
}

// Decoded from retail ?GetNodeCount@CPaneContainer@@QEBAHXZ, entry RVA 0xa7c60
// (mfc140.dll): 1 plus the node counts of the two sub-containers.
// Symbol: ?GetNodeCount@CPaneContainer@@QEBAHXZ
extern "C" int MS_ABI impl__GetNodeCount_CPaneContainer__QEBAHXZ(const void* pThis) {
    const S_CPaneContainer* p = PC(pThis);
    int nCount = 1;
    if (p->m_pLeftPaneContainer != nullptr) {
        nCount += impl__GetNodeCount_CPaneContainer__QEBAHXZ(p->m_pLeftPaneContainer);
    }
    if (p->m_pRightPaneContainer != nullptr) {
        nCount += impl__GetNodeCount_CPaneContainer__QEBAHXZ(p->m_pRightPaneContainer);
    }
    return nCount;
}

// Decoded from retail ?GetResizeStep@CPaneContainer@@UEBAHXZ, entry RVA 0xa39c0
// (mfc140.dll):
//
//   int nStep = -1;                                             // 0xa39cd
//   if (m_pLeftPane)  nStep = pane-><vftable byte 0x4f8>();      // 0xa39d9, assigns
//   if (m_pRightPane) { v = ...; if (nStep <= v) nStep = v; }    // 0xa39f4, maximum
//   if (m_pLeftPaneContainer)  nStep = left-><slot 9>();         // 0xa3a2e, assigns
//   if (m_pRightPaneContainer) { v = right-><slot 9>();
//                                if (nStep <= v) nStep = v; }    // 0xa3a46, maximum
//   return nStep;
//
// The asymmetry is retail's: the left pane and the left sub-container overwrite
// the running value, only the right ones take a maximum.  Byte 0x48 of the
// CPaneContainer vftable (0x2f4070) is slot 9, GetResizeStep itself, so both
// sub-container calls recurse; retail's own recursion goes through the vftable,
// this file calls the entry point directly.
//
// DEVIATION: the pane call is a virtual through byte 0x4f8 of the pane vftable.
// That slot holds the same unexported function -- RVA 0x8920, whose entire body
// is `or $0xffffffff,%eax; ret` -- in the CPane (0x2f3a18), CDockablePane
// (0x2e3068) and CTabbedPane (0x30f738) vftables, the ones a CPaneContainer pane
// slot can hold, so the constant -1 is substituted for the dispatch.  A client
// override of that virtual (which is not exported, so it has no name to override
// through in OpenMFC) would be missed.
// Symbol: ?GetResizeStep@CPaneContainer@@UEBAHXZ
extern "C" int MS_ABI impl__GetResizeStep_CPaneContainer__UEBAHXZ(const void* pThis) {
    const S_CPaneContainer* p = PC(pThis);
    int nStep = -1;
    if (p->m_pLeftPane != nullptr) {
        nStep = -1;                                  // pane-><byte 0x4f8>() == -1
    }
    if (p->m_pRightPane != nullptr) {
        const int nRight = -1;                       // pane-><byte 0x4f8>() == -1
        if (nStep <= nRight) nStep = nRight;
    }
    if (p->m_pLeftPaneContainer != nullptr) {
        nStep = impl__GetResizeStep_CPaneContainer__UEBAHXZ(p->m_pLeftPaneContainer);
    }
    if (p->m_pRightPaneContainer != nullptr) {
        const int nRight = impl__GetResizeStep_CPaneContainer__UEBAHXZ(p->m_pRightPaneContainer);
        if (nStep <= nRight) nStep = nRight;
    }
    return nStep;
}

// Decoded from retail ?GetTotalReferenceCount@CPaneContainer@@QEBAHXZ, entry
// RVA 0xa7e70 (mfc140.dll): this node's m_nRefCount (+0x50) plus the totals of
// the right and then the left sub-container.
// Symbol: ?GetTotalReferenceCount@CPaneContainer@@QEBAHXZ
extern "C" int MS_ABI impl__GetTotalReferenceCount_CPaneContainer__QEBAHXZ(const void* pThis) {
    const S_CPaneContainer* p = PC(pThis);
    int nTotal = p->m_nRefCount;
    if (p->m_pRightPaneContainer != nullptr) {
        nTotal += impl__GetTotalReferenceCount_CPaneContainer__QEBAHXZ(p->m_pRightPaneContainer);
    }
    if (p->m_pLeftPaneContainer != nullptr) {
        nTotal += impl__GetTotalReferenceCount_CPaneContainer__QEBAHXZ(p->m_pLeftPaneContainer);
    }
    return nTotal;
}

// Decoded from retail ?GetWindowRect@CPaneContainer@@UEBAXAEAVCRect@@H@Z, entry
// RVA 0xa3a70 (mfc140.dll).  The IAT slots it calls were resolved with iat.py:
// 0x2c5368 ::SetRectEmpty, 0x2c5370 ::GetWindowRect, 0x2c52c8 ::IsRectEmpty,
// 0x2c4d08 ::UnionRect.
//
//   ::SetRectEmpty(&rect); ::SetRectEmpty(&rectLeft); ::SetRectEmpty(&rectRight);
//   bAll = <the manager/divider gate, see CountsHiddenPanes above>;
//   for each of m_pLeftPane -> rectLeft and m_pRightPane -> rectRight:
//       if (pane && ((pane->GetStyle() & WS_VISIBLE) || bIgnoreVisibility || bAll)) {
//           ::GetWindowRect(pane->m_hWnd, &r);
//           if (::IsRectEmpty(&r)) {                       // 0xa3b34: only when EMPTY
//               CSize szMin; pane-><vftable byte 0x4f0>(szMin);
//               if (r.right  == r.left) r.right  += szMin.cx;
//               if (r.bottom == r.top)  r.bottom += szMin.cy;
//           }
//       }
//   ::UnionRect(&rect, &rectLeft, &rectRight);             // 0xa3bf5
//   for each of m_pLeftPaneContainer, m_pRightPaneContainer:
//       if (child && (child->IsVisible() || bIgnoreVisibility || bAll)) {
//           child->GetWindowRect(rectChild, FALSE);        // byte 0x28 == slot 5,
//           ::UnionRect(&rect, &rect, &rectChild);         // note the hard FALSE
//       }
//
// The recursion passes FALSE, not the caller's flag (`xor %r8d,%r8d` at 0xa3c1d
// and 0xa3c5f).  Retail recurses through the vftable; this file calls the entry
// point directly.  See CountsHiddenPanes / PaneMinSize above for the two
// documented deviations this body inherits.
// Symbol: ?GetWindowRect@CPaneContainer@@UEBAXAEAVCRect@@H@Z
extern "C" void MS_ABI impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(
    const void* pThis, RECT* pRect, int bIgnoreVisibility) {
    const S_CPaneContainer* p = PC(pThis);
    if (pRect == nullptr) return;

    RECT rectLeft = {0, 0, 0, 0};
    RECT rectRight = {0, 0, 0, 0};
    RECT rectChild = {0, 0, 0, 0};
    ::SetRectEmpty(pRect);
    ::SetRectEmpty(&rectLeft);
    ::SetRectEmpty(&rectRight);

    const int bAll = CountsHiddenPanes(p);

    if (p->m_pLeftPane != nullptr &&
        (PaneIsVisible(p->m_pLeftPane) || bIgnoreVisibility || bAll)) {
        ::GetWindowRect(PaneHwnd(p->m_pLeftPane), &rectLeft);
        if (::IsRectEmpty(&rectLeft)) {
            SIZE szMin = {0, 0};
            PaneMinSize(p->m_pLeftPane, &szMin);
            if (rectLeft.right == rectLeft.left) rectLeft.right += szMin.cx;
            if (rectLeft.bottom == rectLeft.top) rectLeft.bottom += szMin.cy;
        }
    }
    if (p->m_pRightPane != nullptr &&
        (PaneIsVisible(p->m_pRightPane) || bIgnoreVisibility || bAll)) {
        ::GetWindowRect(PaneHwnd(p->m_pRightPane), &rectRight);
        if (::IsRectEmpty(&rectRight)) {
            SIZE szMin = {0, 0};
            PaneMinSize(p->m_pRightPane, &szMin);
            if (rectRight.right == rectRight.left) rectRight.right += szMin.cx;
            if (rectRight.bottom == rectRight.top) rectRight.bottom += szMin.cy;
        }
    }
    ::UnionRect(pRect, &rectLeft, &rectRight);

    if (p->m_pLeftPaneContainer != nullptr &&
        (impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pLeftPaneContainer) || bIgnoreVisibility || bAll)) {
        impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(p->m_pLeftPaneContainer, &rectChild, FALSE);
        ::UnionRect(pRect, pRect, &rectChild);
    }
    if (p->m_pRightPaneContainer != nullptr &&
        (impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pRightPaneContainer) || bIgnoreVisibility || bAll)) {
        impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(p->m_pRightPaneContainer, &rectChild, FALSE);
        ::UnionRect(pRect, pRect, &rectChild);
    }
}

// Decoded from retail ?IsEmpty@CPaneContainer@@QEBAHXZ, entry RVA 0xa7040
// (mfc140.dll): TRUE only when both pane slots are NULL and each present
// sub-container is itself empty.
// Symbol: ?IsEmpty@CPaneContainer@@QEBAHXZ
extern "C" int MS_ABI impl__IsEmpty_CPaneContainer__QEBAHXZ(const void* pThis) {
    const S_CPaneContainer* p = PC(pThis);
    if (p->m_pLeftPane != nullptr) return 0;
    if (p->m_pRightPane != nullptr) return 0;
    if (p->m_pLeftPaneContainer != nullptr && !impl__IsEmpty_CPaneContainer__QEBAHXZ(p->m_pLeftPaneContainer)) return 0;
    if (p->m_pRightPaneContainer != nullptr && !impl__IsEmpty_CPaneContainer__QEBAHXZ(p->m_pRightPaneContainer)) return 0;
    return 1;
}

// Decoded from retail ?IsLeftPane@CPaneContainer@@QEBAHPEAVCDockablePane@@@Z,
// entry RVA 0xa7030 (mfc140.dll): `cmp 0x8(%rcx),%rdx; sete %al`.
// Symbol: ?IsLeftPane@CPaneContainer@@QEBAHPEAVCDockablePane@@@Z
extern "C" int MS_ABI impl__IsLeftPane_CPaneContainer__QEBAHPEAVCDockablePane___Z(const void* pThis, void* pPane) {
    return PC(pThis)->m_pLeftPane == pPane ? 1 : 0;
}

// Decoded from retail ?IsLeftPaneContainer@CPaneContainer@@QEBAHXZ, entry RVA
// 0xa7010 (mfc140.dll): TRUE when there is no parent, or when the parent's
// left sub-container link points back at this node.
// Symbol: ?IsLeftPaneContainer@CPaneContainer@@QEBAHXZ
extern "C" int MS_ABI impl__IsLeftPaneContainer_CPaneContainer__QEBAHXZ(const void* pThis) {
    const S_CPaneContainer* p = PC(pThis);
    const S_CPaneContainer* pParent = p->m_pParentContainer;
    if (pParent == nullptr) return 1;
    return pParent->m_pLeftPaneContainer == p ? 1 : 0;
}

// Decoded from retail ?IsLeftPartEmpty@CPaneContainer@@QEBAHH@Z, entry RVA
// 0xa7090 (mfc140.dll). A present left pane makes the part non-empty outright
// unless bCheckVisibility is set and the pane is not WS_VISIBLE; then the same
// question is asked of the left sub-container via IsEmpty()/IsVisible().
// Symbol: ?IsLeftPartEmpty@CPaneContainer@@QEBAHH@Z
extern "C" int MS_ABI impl__IsLeftPartEmpty_CPaneContainer__QEBAHH_Z(const void* pThis, int bCheckVisibility) {
    const S_CPaneContainer* p = PC(pThis);
    if (p->m_pLeftPane != nullptr) {
        if (!bCheckVisibility) return 0;
        if ((impl__GetStyle_CWnd__QEBAKXZ(reinterpret_cast<const CWnd*>(p->m_pLeftPane)) & WS_VISIBLE) != 0) {
            return 0;
        }
    }
    const S_CPaneContainer* pChild = p->m_pLeftPaneContainer;
    if (pChild == nullptr) return 1;
    if (impl__IsEmpty_CPaneContainer__QEBAHXZ(pChild)) return 1;
    if (!bCheckVisibility) return 0;
    return impl__IsVisible_CPaneContainer__QEBAHXZ(pChild) ? 0 : 1;
}

// Decoded from retail ?IsPaneDividerHorz@CPaneContainer@@IEBAHXZ, entry RVA
// 0xa7e40 (mfc140.dll). Retail tail-jumps to CPaneDivider vtable slot 0x2d8
// (the non-exported CBasePane::IsHorizontal override) when m_pSlider is
// non-null, and returns the cached int at +0x54 otherwise.
//
// DEVIATION: OpenMFC does not emit a CPaneDivider vtable, so slot 0x2d8 cannot
// be dispatched. Both paths return the cached +0x54 flag here -- the same field
// retail's ?Copy@ (0xa7ca0, at 0xa7e09) fills in from that very virtual when a
// divider is detached, so the value is the right one whenever it has been set.
// Symbol: ?IsPaneDividerHorz@CPaneContainer@@IEBAHXZ
extern "C" int MS_ABI impl__IsPaneDividerHorz_CPaneContainer__IEBAHXZ(const void* pThis) {
    return PC(pThis)->m_bDividerHorz;
}

// Decoded from retail ?IsRightPartEmpty@CPaneContainer@@QEBAHH@Z, entry RVA
// 0xa70f0 (mfc140.dll) -- the mirror of IsLeftPartEmpty on +0x10 / +0x28.
// Symbol: ?IsRightPartEmpty@CPaneContainer@@QEBAHH@Z
extern "C" int MS_ABI impl__IsRightPartEmpty_CPaneContainer__QEBAHH_Z(const void* pThis, int bCheckVisibility) {
    const S_CPaneContainer* p = PC(pThis);
    if (p->m_pRightPane != nullptr) {
        if (!bCheckVisibility) return 0;
        if ((impl__GetStyle_CWnd__QEBAKXZ(reinterpret_cast<const CWnd*>(p->m_pRightPane)) & WS_VISIBLE) != 0) {
            return 0;
        }
    }
    const S_CPaneContainer* pChild = p->m_pRightPaneContainer;
    if (pChild == nullptr) return 1;
    if (impl__IsEmpty_CPaneContainer__QEBAHXZ(pChild)) return 1;
    if (!bCheckVisibility) return 0;
    return impl__IsVisible_CPaneContainer__QEBAHXZ(pChild) ? 0 : 1;
}

// Decoded from retail ?IsVisible@CPaneContainer@@QEBAHXZ, entry RVA 0xa7150
// (mfc140.dll): TRUE as soon as either pane has WS_VISIBLE (tested with
// `bt $0x1c` on CWnd::GetStyle, 0x2a75a0) or either sub-container is visible.
// Symbol: ?IsVisible@CPaneContainer@@QEBAHXZ
extern "C" int MS_ABI impl__IsVisible_CPaneContainer__QEBAHXZ(const void* pThis) {
    const S_CPaneContainer* p = PC(pThis);
    if (p->m_pLeftPane != nullptr &&
        (impl__GetStyle_CWnd__QEBAKXZ(reinterpret_cast<const CWnd*>(p->m_pLeftPane)) & WS_VISIBLE) != 0) {
        return 1;
    }
    if (p->m_pRightPane != nullptr &&
        (impl__GetStyle_CWnd__QEBAKXZ(reinterpret_cast<const CWnd*>(p->m_pRightPane)) & WS_VISIBLE) != 0) {
        return 1;
    }
    if (p->m_pLeftPaneContainer != nullptr && impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pLeftPaneContainer)) return 1;
    if (p->m_pRightPaneContainer != nullptr && impl__IsVisible_CPaneContainer__QEBAHXZ(p->m_pRightPaneContainer)) return 1;
    return 0;
}

// STUB. Retail ?LoadTabbedPane@CPaneContainer@@IEAAPEAVCDockablePane@@AEAVCArchive@@AEAV?$CList@II@@@Z,
// entry RVA 0xa79f0 (mfc140.dll).  It opens with the two non-exported archive
// helpers at 0x13ce0 and 0x3fc80 (0xa7a16 / 0xa7a26), then reads a raw int
// straight out of the archive buffer -- `m_lpBufCur` at CArchive +0x38 against
// `m_lpBufMax` at +0x40, refilled through 0x1cfc70, under the mode bit tested as
// `testb $0x1,0x20(%rsi)` -- and drives the recovered pane through tabbed-pane
// vftable bytes 0x398 and 0x658.
//
// Blocked: OpenMFC's CArchive does not expose those buffer members at the retail
// offsets, and the tabbed-pane reattachment path behind bytes 0x398/0x658 does
// not exist here.  Only the parameter list is corrected, to the
// (this, CArchive&, CList<UINT,UINT>&) shape the export ABI passes.
// Symbol: ?LoadTabbedPane@CPaneContainer@@IEAAPEAVCDockablePane@@AEAVCArchive@@AEAV?$CList@II@@@Z
extern "C" void* MS_ABI impl__LoadTabbedPane_CPaneContainer__IEAAPEAVCDockablePane__AEAVCArchive__AEAV__CList_II___Z(
    void* pThis, void* pArchive, void* pListIDs) {
    (void)pThis;
    (void)pArchive;
    (void)pListIDs;
    return nullptr;
}

// Decoded from retail ?Move@CPaneContainer@@UEAAXVCPoint@@@Z, entry RVA 0xa6ae0
// (mfc140.dll).  CPoint is an 8-byte aggregate, so it arrives packed in RDX;
// retail reads pt.x out of EDX and pt.y out of the high half of the home slot
// (`mov 0xbc(%rsp),%r15d` at 0xa6b3a).  The generated stub had dropped `this`,
// and is corrected here.
//
//   CRect rect; ::SetRectEmpty(&rect);            // IAT 0x2c5368
//   if (m_pLeftPane) {
//       ::GetWindowRect(m_pLeftPane->m_hWnd, &rect);              // IAT 0x2c5370
//       m_pLeftPane-><vftable byte 0x480>(NULL, pt.x, pt.y, 0, 0, 0x15, NULL);
//   }
//   if (m_pLeftPaneContainer) {
//       m_pLeftPaneContainer->GetWindowRect(rect, FALSE);         // slot 5
//       m_pLeftPaneContainer->Move(pt);                           // slot 14
//   }
//   int cx = rect.Width(), cy = rect.Height();                    // 0xa6bb9
//   if (m_pSlider) {
//       if (m_pSlider-><vftable byte 0x2d8>()) {                  // 0xa6bd7
//           m_pSlider-><0x480>(NULL, pt.x, pt.y + cy, 0,0, 0x15, NULL);
//           cx = 0; cy += m_pSlider-><+0x1b0>;                     // 0xa6c1d
//       } else {
//           m_pSlider-><0x480>(NULL, pt.x + cx, pt.y, 0,0, 0x15, NULL);
//           cy = 0; cx += m_pSlider-><+0x1b0>;                     // 0xa6c39
//       }
//   }
//   if (m_pRightPane)
//       m_pRightPane-><0x480>(NULL, pt.x + cx, pt.y + cy, 0,0, 0x15, NULL);
//   if (m_pRightPaneContainer)
//       m_pRightPaneContainer->Move(CPoint(pt.x + cx, pt.y + cy));
//
// Byte 0x480 of the pane vftable is ?SetWindowPos@CBasePane@@UEAAPEAXPEBVCWnd@@HHHHIPEAX@Z
// (RVA 0xb6a0) in the CPane (0x2f3a18), CDockablePane (0x2e3068) and
// CPaneDivider (0x2f4d28) vftables alike, and that export has a real body in
// OpenMFC (docking/Thunks.cpp:1208), so the dispatch is replaced by a direct
// call to its thunk -- a client override would be missed.  0x15 is
// SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE.
//
// Retail also ::SetRectEmpty's a second CRect at rsp+0x68 (0xa6b2e) that no
// later instruction reads; it is not reproduced.
//
// DEVIATIONS, both already documented above: byte 0x2d8 of the CPaneDivider
// vftable is the non-exported orientation getter that ?IsPaneDividerHorz@
// (0xa7e40) tail-jumps to, so the cached +0x54 flag is used instead; and
// CPaneDivider +0x1b0 has no OpenMFC counterpart, so kPaneDividerThickness
// stands in for it.
// Symbol: ?Move@CPaneContainer@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__Move_CPaneContainer__UEAAXVCPoint___Z(void* pThis, long long point) {
    S_CPaneContainer* p = PC(pThis);
    const int x = LowHalf(point);
    const int y = HighHalf(point);
    const unsigned int nFlags = SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE;   // 0x15

    RECT rect = {0, 0, 0, 0};
    ::SetRectEmpty(&rect);

    if (p->m_pLeftPane != nullptr) {
        ::GetWindowRect(PaneHwnd(p->m_pLeftPane), &rect);
        impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
            reinterpret_cast<CBasePane*>(p->m_pLeftPane), nullptr, x, y, 0, 0, nFlags, nullptr);
    }
    if (p->m_pLeftPaneContainer != nullptr) {
        impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(p->m_pLeftPaneContainer, &rect, FALSE);
        impl__Move_CPaneContainer__UEAAXVCPoint___Z(p->m_pLeftPaneContainer, point);
    }

    int cx = rect.right - rect.left;
    int cy = rect.bottom - rect.top;

    if (p->m_pSlider != nullptr) {
        if (impl__IsPaneDividerHorz_CPaneContainer__IEBAHXZ(p)) {
            impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
                reinterpret_cast<CBasePane*>(p->m_pSlider), nullptr, x, y + cy, 0, 0, nFlags, nullptr);
            cx = 0;
            cy += kPaneDividerThickness;
        } else {
            impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
                reinterpret_cast<CBasePane*>(p->m_pSlider), nullptr, x + cx, y, 0, 0, nFlags, nullptr);
            cy = 0;
            cx += kPaneDividerThickness;
        }
    }

    if (p->m_pRightPane != nullptr) {
        impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
            reinterpret_cast<CBasePane*>(p->m_pRightPane), nullptr, x + cx, y + cy, 0, 0, nFlags, nullptr);
    }
    if (p->m_pRightPaneContainer != nullptr) {
        impl__Move_CPaneContainer__UEAAXVCPoint___Z(p->m_pRightPaneContainer, PackSize(x + cx, y + cy));
    }
}

// Decoded from retail ?MoveWnd@CPaneContainer@@IEAAXPEAVCWnd@@HH@Z, entry RVA
// 0xa6cd0 (mfc140.dll). A NULL pWnd returns immediately (the test at 0xa6cd0).
// Otherwise: take the parent's client rect and the window's screen rect, map the
// latter into parent-client space, work out how far the window already sits from
// the client origin along the axis selected by bHorz, offset the rect by the
// remaining delta and finally CWnd::MoveWindow(..., TRUE).
// The IAT slots at 0x2c5300 / 0x2c5358 / 0x2c5370 / 0x2c5318 were resolved with
// iat.py to ::GetParent, ::GetClientRect, ::GetWindowRect and ::OffsetRect.
//
// DEVIATION: retail uses the CWnd::FromHandle result without checking it (it
// would fault on a parentless window); the NULL check here turns that into a
// move relative to a zero client origin instead.
// Symbol: ?MoveWnd@CPaneContainer@@IEAAXPEAVCWnd@@HH@Z
extern "C" void MS_ABI impl__MoveWnd_CPaneContainer__IEAAXPEAVCWnd__HH_Z(
    void* pThis, CWnd* pWnd, int nOffset, int bHorz) {
    (void)pThis;
    if (pWnd == nullptr) return;

    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pWnd->m_hWnd));

    RECT rectWnd = { 0, 0, 0, 0 };
    RECT rectParentClient = { 0, 0, 0, 0 };
    if (pParent != nullptr) {
        ::GetClientRect(pParent->m_hWnd, &rectParentClient);
    }
    ::GetWindowRect(pWnd->m_hWnd, &rectWnd);
    if (pParent != nullptr) {
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rectWnd);
    }

    int nCurrent = bHorz ? (rectWnd.left - rectParentClient.left)
                         : (rectWnd.top - rectParentClient.top);
    int nDelta = nOffset - nCurrent;

    ::OffsetRect(&rectWnd, bHorz ? nDelta : 0, bHorz ? 0 : nDelta);

    impl__MoveWindow_CWnd__QEAAXHHHHH_Z(pWnd, rectWnd.left, rectWnd.top,
                                        rectWnd.right - rectWnd.left,
                                        rectWnd.bottom - rectWnd.top, TRUE);
}

// Decoded from retail ?OnMoveInternalPaneDivider@CPaneContainer@@UEAAHHAEAPEAX@Z,
// entry RVA 0xa6720 (mfc140.dll).  The generated stub had dropped `this`; the
// corrected shape is the one docking/CPaneContainerManager.cpp already declares
// and calls (that file is edited concurrently, so grep it rather than trusting a
// line number here).
//
//   CRect rectLeft, rectRight; ::SetRectEmpty on both;      // IAT 0x2c5368
//   CSize szLeft(0,0), szRight(0,0);
//   if (m_pLeftPane)  { ::GetWindowRect(pane->m_hWnd, &rectLeft);
//                       pane-><vftable byte 0x4f0>(szLeft); }
//   if (m_pLeftPaneContainer)  { child->GetWindowRect(rectLeft, FALSE);   // slot 5
//                                child->GetMinSize(szLeft); }             // slot 6
//   ... the same pair for m_pRightPane / m_pRightPaneContainer ...
//   CWnd* pParent = CWnd::FromHandle(::GetParent(m_pSlider->m_hWnd));     // 0xa683e
//   pParent->ScreenToClient(&rectLeft);                                   // 0x2a11f0
//   pParent->ScreenToClient(&rectRight);                                  // re-fetched
//   if (!::IsRectEmpty(&rectLeft)) {                                      // IAT 0x2c52c8
//       if (IsPaneDividerHorz()) { rectLeft.bottom += nOffset;
//            if (rectLeft.Height() < szLeft.cy) rectLeft.bottom = rectLeft.top + szLeft.cy; }
//       else                     { rectLeft.right  += nOffset;
//            if (rectLeft.Width()  < szLeft.cx) rectLeft.right  = rectLeft.left + szLeft.cx; }
//   }
//   if (!::IsRectEmpty(&rectRight)) {
//       if (IsPaneDividerHorz()) { rectRight.top  += nOffset;
//            if (rectRight.Height() < szRight.cy) rectRight.top  = rectRight.bottom - szRight.cy; }
//       else                     { rectRight.left += nOffset;
//            if (rectRight.Width()  < szRight.cx) rectRight.left = rectRight.right - szRight.cx; }
//   }
//   if (m_pLeftPane)  hdwp = pane-><byte 0x478>(rectLeft, TRUE, hdwp);    // 0xa6942
//   if (m_pLeftPaneContainer)  child->Resize(rectLeft, hdwp, FALSE);      // slot 20
//   if (m_pRightPane) hdwp = pane-><byte 0x478>(rectRight, TRUE, hdwp);   // 0xa6997
//   if (m_pRightPaneContainer) child->Resize(rectRight, hdwp, FALSE);
//   return nOffset;                                                       // 0xa69cc
//
// Retail resolves the divider's parent twice, once per rect (0xa683e and
// 0xa6860), with identical arguments; it is resolved once here.  It likewise
// re-calls ?IsPaneDividerHorz@ inside each rect block -- that accessor is a
// plain member read, so one call is equivalent.  Byte 0x478 is
// ?MoveWindow@CBasePane@ (RVA 0xb580) and byte 0x4f0 the `size = m_sizeMin`
// accessor, both documented above.
//
// DEVIATION: retail dereferences m_pSlider unconditionally.  PaneDividerHwnd
// (above) returns NULL unless the divider really carries a window handle at
// +0x40, and the screen-to-client mapping is then skipped -- rectLeft/rectRight
// stay in screen coordinates, which is the same frame ::GetWindowRect produced
// them in, so the offset arithmetic below is unaffected; only the coordinates
// handed to MoveWindow/Resize are.  ?Resize@CPaneContainer@ (below) is still a
// stub, so the two container arms reach no-ops; ?MoveWindow@CBasePane@ has a
// real body in docking/CBasePane.cpp (verified 2026-09-08), and its return value
// is stored back into the caller's HDWP exactly as retail's `mov %rax,(%rsi)`
// does.  A second deviation: retail takes the HDWP as a reference and never
// tests it, while the two pane arms here skip the move when phdwp is NULL.
// Symbol: ?OnMoveInternalPaneDivider@CPaneContainer@@UEAAHHAEAPEAX@Z
extern "C" int MS_ABI impl__OnMoveInternalPaneDivider_CPaneContainer__UEAAHHAEAPEAX_Z(
    void* pThis, int nOffset, void** phdwp) {
    S_CPaneContainer* p = PC(pThis);

    RECT rectLeft = {0, 0, 0, 0};
    RECT rectRight = {0, 0, 0, 0};
    ::SetRectEmpty(&rectLeft);
    ::SetRectEmpty(&rectRight);
    SIZE szLeft = {0, 0};
    SIZE szRight = {0, 0};

    if (p->m_pLeftPane != nullptr) {
        ::GetWindowRect(PaneHwnd(p->m_pLeftPane), &rectLeft);
        PaneMinSize(p->m_pLeftPane, &szLeft);
    }
    if (p->m_pLeftPaneContainer != nullptr) {
        impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(p->m_pLeftPaneContainer, &rectLeft, FALSE);
        impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(p->m_pLeftPaneContainer, &szLeft);
    }
    if (p->m_pRightPane != nullptr) {
        ::GetWindowRect(PaneHwnd(p->m_pRightPane), &rectRight);
        PaneMinSize(p->m_pRightPane, &szRight);
    }
    if (p->m_pRightPaneContainer != nullptr) {
        impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(p->m_pRightPaneContainer, &rectRight, FALSE);
        impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(p->m_pRightPaneContainer, &szRight);
    }

    const HWND hwndSlider = PaneDividerHwnd(p->m_pSlider);
    if (hwndSlider != nullptr) {
        CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hwndSlider));
        if (pParent != nullptr) {
            impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rectLeft);
            impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rectRight);
        }
    }

    const int bHorz = impl__IsPaneDividerHorz_CPaneContainer__IEBAHXZ(p);

    if (!::IsRectEmpty(&rectLeft)) {
        if (bHorz) {
            rectLeft.bottom += nOffset;
            if (rectLeft.bottom - rectLeft.top < szLeft.cy) rectLeft.bottom = rectLeft.top + szLeft.cy;
        } else {
            rectLeft.right += nOffset;
            if (rectLeft.right - rectLeft.left < szLeft.cx) rectLeft.right = rectLeft.left + szLeft.cx;
        }
    }
    if (!::IsRectEmpty(&rectRight)) {
        if (bHorz) {
            rectRight.top += nOffset;
            if (rectRight.bottom - rectRight.top < szRight.cy) rectRight.top = rectRight.bottom - szRight.cy;
        } else {
            rectRight.left += nOffset;
            if (rectRight.right - rectRight.left < szRight.cx) rectRight.left = rectRight.right - szRight.cx;
        }
    }

    if (p->m_pLeftPane != nullptr && phdwp != nullptr) {
        *phdwp = impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(
            reinterpret_cast<CBasePane*>(p->m_pLeftPane), &rectLeft, TRUE, *phdwp);
    }
    if (p->m_pLeftPaneContainer != nullptr) {
        impl__Resize_CPaneContainer__UEAAXVCRect__AEAPEAXH_Z(p->m_pLeftPaneContainer, &rectLeft, phdwp, FALSE);
    }
    if (p->m_pRightPane != nullptr && phdwp != nullptr) {
        *phdwp = impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(
            reinterpret_cast<CBasePane*>(p->m_pRightPane), &rectRight, TRUE, *phdwp);
    }
    if (p->m_pRightPaneContainer != nullptr) {
        impl__Resize_CPaneContainer__UEAAXVCRect__AEAPEAXH_Z(p->m_pRightPaneContainer, &rectRight, phdwp, FALSE);
    }
    return nOffset;
}

// Decoded from retail ?OnShowPane@CPaneContainer@@UEAAXPEAVCDockablePane@@H@Z,
// entry RVA 0xa7f30 (mfc140.dll).  The generated stub had dropped `this`, and is
// corrected here.
//
//   if (bShow) return;                                            // 0xa7f30
//   CWnd* pDockSite = m_pContainerManager-><vftable byte 0xf8>();  // manager +0x98
//   CRect rect(0,0,0,0);
//   GetWindowRect(rect, TRUE);                                    // slot 5, bIgnoreVisibility = TRUE
//   pDockSite->ScreenToClient(&rect);                             // 0x2a11f0
//   CDockablePane* pOther = NULL;                                 // 0xa7fa2..0xa7fbc
//   if (m_pLeftPane  && m_pLeftPane  != pPane) pOther = m_pLeftPane;
//   else if (m_pRightPane && m_pRightPane != pPane) pOther = m_pRightPane;
//   if (pOther) {
//       pOther-><vftable byte 0x480>(NULL, rect.left, rect.top,
//                                    rect.Width(), rect.Height(), 0x14, NULL);
//       return;
//   }
//   CPaneContainer* pChild = m_pLeftPaneContainer;                 // 0xa8002
//   if (!pChild) pChild = m_pRightPaneContainer;
//   if (!pChild) return;
//   HDWP hdwp = NULL;                                              // 0xa801c
//   pChild->Resize(rect, hdwp, TRUE);                              // slot 20
//
// 0x14 is SWP_NOZORDER|SWP_NOACTIVATE -- note that unlike the Move body above
// this one does pass a size, so SWP_NOSIZE is absent.  Byte 0x480 is
// CBasePane::SetWindowPos (see the Move comment); the manager accessor and the
// dock-site offset are documented at ManagerDockSite above.
//
// DEVIATION: retail dereferences m_pContainerManager and the returned dock site
// without NULL tests; both are guarded here, which skips the screen-to-client
// mapping instead of faulting.  ?Resize@CPaneContainer@ is still a stub in this
// file, so the sub-container arm currently reaches a no-op -- the call is placed
// exactly where retail places it.
// Symbol: ?OnShowPane@CPaneContainer@@UEAAXPEAVCDockablePane@@H@Z
extern "C" void MS_ABI impl__OnShowPane_CPaneContainer__UEAAXPEAVCDockablePane__H_Z(
    void* pThis, void* pPane, int bShow) {
    if (bShow) return;
    S_CPaneContainer* p = PC(pThis);

    CWnd* pDockSite = ManagerDockSite(p);
    RECT rect = {0, 0, 0, 0};
    impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(p, &rect, TRUE);
    if (pDockSite != nullptr) {
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pDockSite, &rect);
    }

    void* pOther = nullptr;
    if (p->m_pLeftPane != nullptr && p->m_pLeftPane != pPane) {
        pOther = p->m_pLeftPane;
    } else if (p->m_pRightPane != nullptr && p->m_pRightPane != pPane) {
        pOther = p->m_pRightPane;
    }

    if (pOther != nullptr) {
        impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
            reinterpret_cast<CBasePane*>(pOther), nullptr, rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top,
            SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
        return;
    }

    S_CPaneContainer* pChild = p->m_pLeftPaneContainer;
    if (pChild == nullptr) pChild = p->m_pRightPaneContainer;
    if (pChild == nullptr) return;

    void* hdwp = nullptr;
    impl__Resize_CPaneContainer__UEAAXVCRect__AEAPEAXH_Z(pChild, &rect, &hdwp, TRUE);
}

// Decoded from retail ?Release@CPaneContainer@@QEAAKXZ, entry RVA 0xa4cf0
// (mfc140.dll): decrement m_nRefCount; if it is still > 0 return it, otherwise
// call FreeReleasedPaneContainer() and return 0.
// FreeReleasedPaneContainer is itself an honest stub in this file (see its
// comment), so the call is structurally faithful but currently a no-op.
// Symbol: ?Release@CPaneContainer@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_CPaneContainer__QEAAKXZ(void* pThis) {
    S_CPaneContainer* p = PC(pThis);
    --p->m_nRefCount;
    if (p->m_nRefCount > 0) {
        return static_cast<unsigned long>(p->m_nRefCount);
    }
    impl__FreeReleasedPaneContainer_CPaneContainer__IEAAXXZ(p);
    return 0;
}

// Decoded from retail ?ReleaseEmptyPaneContainer@CPaneContainer@@QEAAXXZ, entry
// RVA 0xa4e90 (mfc140.dll): recurse into both sub-containers, then release this
// node unless it is the manager's root container -- retail compares
// m_pParentContainer (+0x30) against the manager's root-container member at
// +0x78, the same member ?IsRootPaneContainerVisible@CPaneContainerManager@@
// (entry RVA 0xaa690) tail-jumps to CPaneContainer::IsVisible with.
//
// DEVIATION: retail dereferences m_pContainerManager unconditionally; a NULL
// manager is guarded here, which turns a crash into a no-op.
// Symbol: ?ReleaseEmptyPaneContainer@CPaneContainer@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainer_CPaneContainer__QEAAXXZ(void* pThis) {
    S_CPaneContainer* p = PC(pThis);
    if (p->m_pLeftPaneContainer != nullptr) {
        impl__ReleaseEmptyPaneContainer_CPaneContainer__QEAAXXZ(p->m_pLeftPaneContainer);
    }
    if (p->m_pRightPaneContainer != nullptr) {
        impl__ReleaseEmptyPaneContainer_CPaneContainer__QEAAXXZ(p->m_pRightPaneContainer);
    }
    if (p->m_pContainerManager == nullptr) return;
    void* pRoot = *reinterpret_cast<void**>(static_cast<char*>(p->m_pContainerManager) + 0x78);
    if (pRoot != static_cast<void*>(p->m_pParentContainer)) {
        impl__FreeReleasedPaneContainer_CPaneContainer__IEAAXXZ(p);
    }
}

// Decoded from retail ?RemoveNonValidPanes@CPaneContainer@@QEAAXXZ, entry RVA
// 0xa7ec0 (mfc140.dll).  The predicate it calls,
// ?CheckAndRemoveNonValidPane@CPaneContainerManager@ (entry RVA 0xaad00), is a
// real implementation in this repo (CPaneContainerManager.cpp:467), so the loop
// can be transcribed as it stands:
//
//   CPaneContainer* p = this;
//   if (p->m_pContainerManager == NULL) return;             // 0xa7ec6
//   do {
//       if (p->m_pLeftPane &&
//           !CheckAndRemoveNonValidPane(p->m_pContainerManager, p->m_pLeftPane))
//           p->m_pLeftPane = NULL;                          // 0xa7ee6
//       if (p->m_pLeftPaneContainer) p->m_pLeftPaneContainer->RemoveNonValidPanes();
//       if (p->m_pRightPane &&
//           !CheckAndRemoveNonValidPane(p->m_pContainerManager, p->m_pRightPane))
//           p->m_pRightPane = NULL;                         // 0xa7f12
//       p = p->m_pRightPaneContainer;                       // 0xa7f1a
//   } while (p != NULL && p->m_pContainerManager != NULL);
//
// Retail recurses only into the left sub-container and walks the right spine
// iteratively; that shape is kept here.  The recursive call is the direct entry
// point, which is what retail uses too (`call 0x1800a7ec0` at 0xa7ef7) -- this
// method is not virtual.
// Symbol: ?RemoveNonValidPanes@CPaneContainer@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveNonValidPanes_CPaneContainer__QEAAXXZ(void* pThis) {
    S_CPaneContainer* p = PC(pThis);
    if (p == nullptr || p->m_pContainerManager == nullptr) return;
    for (;;) {
        if (p->m_pLeftPane != nullptr &&
            !impl__CheckAndRemoveNonValidPane_CPaneContainerManager__QEAAHPEAVCWnd___Z(
                p->m_pContainerManager, p->m_pLeftPane)) {
            p->m_pLeftPane = nullptr;
        }
        if (p->m_pLeftPaneContainer != nullptr) {
            impl__RemoveNonValidPanes_CPaneContainer__QEAAXXZ(p->m_pLeftPaneContainer);
        }
        if (p->m_pRightPane != nullptr &&
            !impl__CheckAndRemoveNonValidPane_CPaneContainerManager__QEAAHPEAVCWnd___Z(
                p->m_pContainerManager, p->m_pRightPane)) {
            p->m_pRightPane = nullptr;
        }
        p = p->m_pRightPaneContainer;
        if (p == nullptr || p->m_pContainerManager == nullptr) return;
    }
}

// STUB. Retail ?Resize@CPaneContainer@@UEAAXVCRect@@AEAPEAXH@Z (entry RVA
// 0xa5780) is the main layout routine: it splits the incoming rect, uses the
// divider width at CPaneDivider +0x1b0 and the manager-side predicate at
// CPaneContainerManager +0x88 vtable slot 0x3c8, and applies the result through
// CDockablePane vtable slots 0x478 and 0x4f0 into the caller's HDWP. None of
// those vtables is emitted by OpenMFC. Left as the generated stub.
// Only the parameter list is corrected, to the (this, rect, hdwp, bRedraw) shape
// the export ABI passes and that CPaneContainerManager.cpp:196 already declares
// and calls; the body stays a stub for the reasons above.
// Symbol: ?Resize@CPaneContainer@@UEAAXVCRect@@AEAPEAXH@Z
extern "C" void MS_ABI impl__Resize_CPaneContainer__UEAAXVCRect__AEAPEAXH_Z(
    void* pThis, const RECT* pRect, void** phdwp, int bRedraw) {
    (void)pThis; (void)pRect; (void)phdwp; (void)bRedraw;
}

// Decoded from retail ?ResizePane@CPaneContainer@@UEAAXHPEAVCPane@@PEAV1@HHAEAPEAX@Z,
// entry RVA 0xa69f0 (mfc140.dll).  Six declared arguments, so the last three sit
// in the home area at entry_rsp+0x28/0x30/0x38 (read as `mov 0x40(%rbp)`,
// `mov 0x48(%rbp)` and `mov 0x50(%rbp)` after `lea` sets rbp = entry_rsp-0x18).
// The generated stub had dropped `this`, and is corrected here.
//
//   if (pPane) {
//       CRect rect(0,0,0,0);
//       ::GetWindowRect(pPane->m_hWnd, &rect);                    // IAT 0x2c5370
//       if (bStretchHorz) { if (bLeftBar) rect.bottom += nOffset;  // 0xa6a43
//                           else          rect.top    -= nOffset; }// 0xa6a48
//       else              { if (bLeftBar) rect.right  += nOffset;  // 0xa6a53
//                           else          rect.left   += nOffset; }// 0xa6a58
//       pPane-><vftable byte 0x560>(rect, FALSE, hdwp);            // CSize return, discarded
//       return;
//   }
//   if (pContainer)
//       pContainer-><vftable byte 0x78>(nOffset, bStretchHorz, bLeftBar, TRUE, hdwp);
//
// Note the edge asymmetry is retail's: only the bStretchHorz && !bLeftBar arm
// SUBTRACTS.  Byte 0x560 of the pane vftable is
// ?MovePane@CPane@@UEAA?AVCSize@@VCRect@@HAEAPEAX@Z (RVA 0xa1140) in the CPane
// (0x2f3a18) and CDockablePane (0x2e3068) vftables, and byte 0x78 of the
// CPaneContainer vftable (0x2f4070) is slot 15, StretchPaneContainer; both are
// called through their entry points here rather than dispatched, as the rest of
// this file does -- a client override would be missed.
//
// Both callees are still stubs in this tree as of 2026-09-08 (?MovePane@CPane@
// in docking/CPane.cpp returns CSize(0,0), ?StretchPaneContainer@ below returns
// 0), so this body currently computes the right rect and hands it to a no-op.
// Symbol: ?ResizePane@CPaneContainer@@UEAAXHPEAVCPane@@PEAV1@HHAEAPEAX@Z
extern "C" void MS_ABI impl__ResizePane_CPaneContainer__UEAAXHPEAVCPane__PEAV1_HHAEAPEAX_Z(
    void* pThis, int nOffset, void* pPane, void* pContainer,
    int bStretchHorz, int bLeftBar, void** phdwp) {
    (void)pThis;
    if (pPane != nullptr) {
        RECT rect = {0, 0, 0, 0};
        ::GetWindowRect(PaneHwnd(pPane), &rect);
        if (bStretchHorz) {
            if (bLeftBar) rect.bottom += nOffset; else rect.top -= nOffset;
        } else {
            if (bLeftBar) rect.right += nOffset; else rect.left += nOffset;
        }
        SIZE sizeMoved = {0, 0};
        RECT rectArg = rect;
        impl__MovePane_CPane__UEAA_AVCSize__VCRect__HAEAPEAX_Z(
            reinterpret_cast<CPane*>(pPane), &sizeMoved, &rectArg, FALSE, phdwp);
        return;
    }
    if (pContainer != nullptr) {
        impl__StretchPaneContainer_CPaneContainer__UEAAHHHHHAEAPEAX_Z(
            pContainer, nOffset, bStretchHorz, bLeftBar, TRUE, phdwp);
    }
}

// Decoded from retail ?ResizePartOfPaneContainer@CPaneContainer@@UEAAXHHAEAPEAX@Z,
// entry RVA 0xa48c0 (mfc140.dll).  The generated stub had dropped `this`, and is
// corrected here.  Locals: rect at rbp-0x20, CSize sizeMin at rbp-0x40.
//
//   if (m_pSlider == NULL) return;                                 // 0xa48dc
//   CRect rect; ::SetRectEmpty(&rect); CSize sizeMin(0,0);
//   // pick the half named by bLeftPart, preferring the sub-container
//   if (bLeftPart) { obj = m_pLeftPaneContainer;  fallback = m_pLeftPane;  }
//   else           { obj = m_pRightPaneContainer; fallback = m_pRightPane; }
//   if (obj) { obj->GetWindowRect(rect, FALSE);            // slot 5
//              obj->GetMinSize(sizeMin); }                 // slot 6
//   else     { if (!fallback) return;                      // 0xa4946 / 0xa4a05
//              ::GetWindowRect(fallback->m_hWnd, &rect);
//              fallback-><vftable byte 0x4f0>(sizeMin); }
//   if (bLeftPart) {                                       // grow the far edge
//       if (IsPaneDividerHorz()) { rect.bottom += nOffset;                    // 0xa497e
//            if (rect.Height() < sizeMin.cy) rect.bottom = rect.top + sizeMin.cy; }
//       else                     { rect.right += nOffset;                     // 0xa49b1
//            if (rect.Width()  < sizeMin.cx) rect.right  = rect.left + sizeMin.cx; }
//   } else {                                               // move the near edge
//       if (IsPaneDividerHorz()) { rect.top  += nOffset;                      // 0xa4a3d
//            if (rect.Height() < sizeMin.cy) rect.top  = rect.bottom - sizeMin.cy; }
//       else                     { rect.left += nOffset;                      // 0xa4a59
//            if (rect.Width()  < sizeMin.cx) rect.left = rect.right - sizeMin.cx; }
//   }
//   m_pContainerManager-><byte 0xf8>()->ScreenToClient(&rect);   // 0xa4a73
//   if (obj) obj->Resize(rect, hdwp, FALSE);                     // slot 20
//   else     hdwp = fallback-><vftable byte 0x478>(rect, FALSE, hdwp);
//
// Retail calls ?IsPaneDividerHorz@ (0xa7e40) twice on the bLeftPart path
// (0xa4972 and 0xa49a4); the second call can only be reached after the first
// returned 0 and the accessor is a plain member read, so the arm it guards
// (0xa4a59) is unreachable and a single call is equivalent.
//
// Byte 0x4f0 of the pane vftable is the `size = m_sizeMin` accessor documented at
// PaneMinSize above; byte 0x478 is
// ?MoveWindow@CBasePane@@UEAAPEAXAEBVCRect@@HPEAX@Z (RVA 0xb580); the manager
// accessor is documented at ManagerDockSite.  Retail dispatches all of these
// virtually; this file calls the entry points, as it does elsewhere.
//
// ?Resize@CPaneContainer@ (below) is still a stub here, so the sub-container arm
// currently reaches a no-op; ?MoveWindow@CBasePane@ has a real body in
// docking/CBasePane.cpp (verified 2026-09-08) and its return value is stored
// back into the caller's HDWP exactly as retail does.
//
// DEVIATION: retail takes the HDWP as a reference and never tests it; the pane
// arm here skips the move when phdwp is NULL rather than faulting.
// Symbol: ?ResizePartOfPaneContainer@CPaneContainer@@UEAAXHHAEAPEAX@Z
extern "C" void MS_ABI impl__ResizePartOfPaneContainer_CPaneContainer__UEAAXHHAEAPEAX_Z(
    void* pThis, int nOffset, int bLeftPart, void** phdwp) {
    S_CPaneContainer* p = PC(pThis);
    if (p->m_pSlider == nullptr) return;

    RECT rect = {0, 0, 0, 0};
    ::SetRectEmpty(&rect);
    SIZE sizeMin = {0, 0};

    S_CPaneContainer* pChild = bLeftPart ? p->m_pLeftPaneContainer : p->m_pRightPaneContainer;
    void* pPane = nullptr;
    if (pChild != nullptr) {
        impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(pChild, &rect, FALSE);
        impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(pChild, &sizeMin);
    } else {
        pPane = bLeftPart ? p->m_pLeftPane : p->m_pRightPane;
        if (pPane == nullptr) return;
        ::GetWindowRect(PaneHwnd(pPane), &rect);
        PaneMinSize(pPane, &sizeMin);
    }

    const int bHorz = impl__IsPaneDividerHorz_CPaneContainer__IEBAHXZ(p);
    if (bLeftPart) {
        if (bHorz) {
            rect.bottom += nOffset;
            if (rect.bottom - rect.top < sizeMin.cy) rect.bottom = rect.top + sizeMin.cy;
        } else {
            rect.right += nOffset;
            if (rect.right - rect.left < sizeMin.cx) rect.right = rect.left + sizeMin.cx;
        }
    } else {
        if (bHorz) {
            rect.top += nOffset;
            if (rect.bottom - rect.top < sizeMin.cy) rect.top = rect.bottom - sizeMin.cy;
        } else {
            rect.left += nOffset;
            if (rect.right - rect.left < sizeMin.cx) rect.left = rect.right - sizeMin.cx;
        }
    }

    CWnd* pDockSite = ManagerDockSite(p);
    if (pDockSite != nullptr) {
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pDockSite, &rect);
    }

    if (pChild != nullptr) {
        impl__Resize_CPaneContainer__UEAAXVCRect__AEAPEAXH_Z(pChild, &rect, phdwp, FALSE);
    } else if (phdwp != nullptr) {
        *phdwp = impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(
            reinterpret_cast<CBasePane*>(pPane), &rect, FALSE, *phdwp);
    }
}

// STUB. Retail ?SaveTabbedPane@CPaneContainer@@IEAAXAEAVCArchive@@PEAVCDockablePane@@@Z,
// entry RVA 0xa78d0 (mfc140.dll).  It first narrows the pane --
// `if (pPane == NULL || !pPane->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) pPane = NULL;`
// (CObject::IsKindOf at 0x233310 against the descriptor at 0x2db168, whose name
// string reads "CBaseTabbedPane") -- then asks it for a count through pane
// vftable byte 0x758, and, when that count is positive, writes ints directly
// into the archive buffer (`m_lpBufCur` at CArchive +0x38 tested against
// `m_lpBufMax` at +0x40, flushed through 0x1cfb90, the store mode read as
// `testb $0x1,0x20(%rbx)`), interleaved with pane vftable bytes 0x7c8 and 0x7c0.
//
// Blocked: OpenMFC's CArchive does not expose those buffer members at the retail
// offsets, and it dereferences the narrowed pane at 0xa78ff even when the
// narrowing produced NULL, so the shape cannot be reproduced safely either.
// Only the parameter list is corrected, to the (this, CArchive&, CDockablePane*)
// shape the export ABI passes.
// Symbol: ?SaveTabbedPane@CPaneContainer@@IEAAXAEAVCArchive@@PEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__SaveTabbedPane_CPaneContainer__IEAAXAEAVCArchive__PEAVCDockablePane___Z(
    void* pThis, void* pArchive, void* pPane) {
    (void)pThis;
    (void)pArchive;
    (void)pPane;
}

// STUB. Retail ?Serialize@CPaneContainer@@UEAAXAEAVCArchive@@@Z, entry RVA
// 0xa7290 (mfc140.dll).  It persists the whole sub-tree, branching on the
// archive's store bit (`test %r15b,0x20(%rdx)` at 0xa72a6, r15b == 1) and then
// reading and writing ints straight through the archive buffer members --
// `m_lpBufCur` at CArchive +0x38 compared against `m_lpBufMax` at +0x40, topped
// up by the non-exported helper at 0x1cfb90 -- while calling ?SaveTabbedPane@
// (0xa78d0) and ?LoadTabbedPane@ (0xa79f0) for the tabbed children.
//
// Blocked: OpenMFC's CArchive does not expose m_lpBufCur/m_lpBufMax/m_nMode at
// the retail offsets, and the two helpers this drives are themselves stubs
// above.  Only the parameter list is corrected, to the (this, CArchive&) shape
// the export ABI passes.
// Symbol: ?Serialize@CPaneContainer@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CPaneContainer__UEAAXAEAVCArchive___Z(
    void* pThis, void* pArchive) {
    (void)pThis;
    (void)pArchive;
}

// Decoded from retail ?SetPane@CPaneContainer@@QEAAXPEAVCDockablePane@@H@Z,
// entry RVA 0xa5200 (mfc140.dll): stores into +0x08 when bLeftPane is non-zero,
// into +0x10 otherwise.
// Symbol: ?SetPane@CPaneContainer@@QEAAXPEAVCDockablePane@@H@Z
extern "C" void MS_ABI impl__SetPane_CPaneContainer__QEAAXPEAVCDockablePane__H_Z(void* pThis, void* pPane, int bLeftPane) {
    S_CPaneContainer* p = PC(pThis);
    if (bLeftPane) {
        p->m_pLeftPane = pPane;
    } else {
        p->m_pRightPane = pPane;
    }
}

// Decoded from retail ?SetPaneContainer@CPaneContainer@@QEAAXPEAV1@H@Z, entry
// RVA 0xa5210 (mfc140.dll): stores into +0x20 or +0x28 according to bLeftPane
// and, when the new child is non-null, points its m_pParentContainer (+0x30)
// back at this node.
// Symbol: ?SetPaneContainer@CPaneContainer@@QEAAXPEAV1@H@Z
extern "C" void MS_ABI impl__SetPaneContainer_CPaneContainer__QEAAXPEAV1_H_Z(void* pThis, void* pContainer, int bLeftPane) {
    S_CPaneContainer* p = PC(pThis);
    S_CPaneContainer* q = PC(pContainer);
    if (bLeftPane) {
        p->m_pLeftPaneContainer = q;
    } else {
        p->m_pRightPaneContainer = q;
    }
    if (q != nullptr) {
        q->m_pParentContainer = p;
    }
}

// Decoded from retail ?SetPaneContainerManager@CPaneContainer@@IEAAXPEAVCPaneContainerManager@@H@Z,
// entry RVA 0xa7c00 (mfc140.dll). Always stores the manager at +0x38; when
// bRecursive is set it also recurses into the left sub-container and then walks
// the right-sub-container spine, storing the manager into each node it passes
// (the loop at 0xa7c21..0xa7c42 assigns +0x38 directly on the right children
// before re-entering the recursion for their left children).
// Symbol: ?SetPaneContainerManager@CPaneContainer@@IEAAXPEAVCPaneContainerManager@@H@Z
extern "C" void MS_ABI impl__SetPaneContainerManager_CPaneContainer__IEAAXPEAVCPaneContainerManager__H_Z(
    void* pThis, void* pManager, int bRecursive) {
    S_CPaneContainer* p = PC(pThis);
    p->m_pContainerManager = pManager;
    if (!bRecursive) return;

    for (;;) {
        if (p->m_pLeftPaneContainer != nullptr) {
            impl__SetPaneContainerManager_CPaneContainer__IEAAXPEAVCPaneContainerManager__H_Z(
                p->m_pLeftPaneContainer, pManager, bRecursive);
        }
        p = p->m_pRightPaneContainer;
        if (p == nullptr) return;
        p->m_pContainerManager = pManager;
    }
}

// Decoded from retail ?SetUpByID@CPaneContainer@@QEAAHIPEAVCDockablePane@@@Z,
// entry RVA 0xa7860 (mfc140.dll).  An earlier revision of this comment claimed
// the body consults the CList<UINT,UINT> sibling lists and FindTabbedPane; it
// does not -- it touches nothing but the two persisted pane ids at +0x44/+0x48
// and the two pane slots, so nothing here is blocked.  The generated stub had
// dropped `this`, and is corrected here.
//
//   CPaneContainer* p = this;
//   if (p-><+0x44> == nID) { p->m_pLeftPane = pPane; return TRUE; }   // 0xa7877
//   for (;;) {
//       if (p-><+0x48> == nID) { p->m_pRightPane = pPane; return TRUE; }  // 0xa787c
//       if (p->m_pLeftPaneContainer &&
//           p->m_pLeftPaneContainer->SetUpByID(nID, pPane)) return TRUE;  // 0xa788f
//       p = p->m_pRightPaneContainer;                                     // 0xa7898
//       if (p == NULL) return FALSE;
//       if (p-><+0x44> == nID) { p->m_pLeftPane = pPane; return TRUE; }   // 0xa78a1
//   }
//
// Retail recurses only into the left sub-container and walks the right-child
// spine iteratively -- the same shape ?RemoveNonValidPanes@ uses above -- and its
// recursive call is the direct entry point (`call 0x1800a7860` at 0xa788f), this
// method not being virtual.  The stores at 0xa78a6 / 0xa78c3 write the pane into
// the node the loop is currently standing on, not into `this`.
// Symbol: ?SetUpByID@CPaneContainer@@QEAAHIPEAVCDockablePane@@@Z
extern "C" int MS_ABI impl__SetUpByID_CPaneContainer__QEAAHIPEAVCDockablePane___Z(
    void* pThis, unsigned int nID, void* pPane) {
    S_CPaneContainer* p = PC(pThis);
    if (p == nullptr) return 0;
    if (static_cast<unsigned int>(p->m_nField44) == nID) {
        p->m_pLeftPane = pPane;
        return 1;
    }
    for (;;) {
        if (static_cast<unsigned int>(p->m_nField48) == nID) {
            p->m_pRightPane = pPane;
            return 1;
        }
        if (p->m_pLeftPaneContainer != nullptr &&
            impl__SetUpByID_CPaneContainer__QEAAHIPEAVCDockablePane___Z(
                p->m_pLeftPaneContainer, nID, pPane)) {
            return 1;
        }
        p = p->m_pRightPaneContainer;
        if (p == nullptr) return 0;
        if (static_cast<unsigned int>(p->m_nField44) == nID) {
            p->m_pLeftPane = pPane;
            return 1;
        }
    }
}

// Decoded from retail ?StoreRecentDockSiteInfo@CPaneContainer@@UEAAXPEAVCDockablePane@@@Z,
// entry RVA 0xa5180 (mfc140.dll), in full:
//
//   CPaneDivider* pSlider = pPane->GetDefaultPaneDivider();        // call 0x45cd0
//   if (pSlider == NULL ||
//       pPane-><vftable byte 0x460>(FALSE) != NULL ||              // 0xa51ad,
//                          // ?GetParentMiniFrame@CBasePane@ (RVA 0xb460)
//       pSlider-><vftable byte 0x3c8>() == 0)                      // 0xa51c5
//       pPane->m_recentDockInfo.StoreDockInfo(this, NULL);         // +0x2b8,
//                          // CRecentDockSiteInfo vftable byte 0x28 == slot 5
//
// m_recentDockInfo is a named member of OpenMFC's CPane at exactly +0x2b8
// (include/openmfc/afxmfc.h), so its address is taken directly rather than
// computed; retail dispatches StoreDockInfo through that object's vftable, this
// file calls the exported entry point.
//
// DEVIATION: the third test reads the CPaneDivider DWORD documented at
// CountsHiddenPanes / PaneDividerAutoHideMode above, which OpenMFC has no
// storage for, so it evaluates to retail's cleared-flag value here and the
// disjunct is always TRUE.  Note that the guard is only reached at all when
// GetDefaultPaneDivider returned non-NULL, and OpenMFC's
// ?GetDefaultPaneDivider@CDockablePane@ (CDockablePane.cpp:757) always returns
// NULL, so in this tree the first disjunct already short-circuits and the store
// runs unconditionally -- the same decision retail reaches.
//
// ?StoreDockInfo@CRecentDockSiteInfo@ (CRecentDockSiteInfo.cpp:64) is still an
// empty stub, so the store currently has no observable effect; the call is
// placed exactly where retail places it.
// Symbol: ?StoreRecentDockSiteInfo@CPaneContainer@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPaneContainer__UEAAXPEAVCDockablePane___Z(
    void* pThis, void* pPane) {
    if (pPane == nullptr) return;          // retail dereferences pPane unguarded

    void* pSlider = impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(pPane);
    if (pSlider != nullptr &&
        impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
            reinterpret_cast<const CBasePane*>(pPane), FALSE) == nullptr &&
        PaneDividerAutoHideMode(pSlider) != 0) {
        return;
    }

    impl__StoreDockInfo_CRecentDockSiteInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane___Z(
        &reinterpret_cast<CPane*>(pPane)->m_recentDockInfo, pThis, nullptr);
}

// STUB. Retail ?StretchPaneContainer@CPaneContainer@@UEAAHHHHHAEAPEAX@Z, entry
// RVA 0xa63b0 (mfc140.dll).  It works out a signed step from the requested
// offset, flips it for a right-to-left layout (CWnd::GetExStyle, 0x2a75d0, tested
// with `bt $0x16` = WS_EX_LAYOUTRTL, on the window reached through
// AfxGetModuleThreadState, 0x1346a0), sizes the move with ::abs (IAT 0x2c5910)
// against CalcAvailablePaneSpace / CalcAvailableSpace (CPaneContainer vftable
// bytes 0x90 and 0x98), and applies it with ResizePane (byte 0x88).
//
// The blocker is the divider: at 0xa64bb and 0xa64dc it asks m_pSlider for
// vftable byte 0x2d8 -- RVA 0xab490 in the CPaneDivider vftable (0x2f4d28), whose
// body is `mov 0x1ac(%rcx),%eax; and $1,%eax; ret` -- to decide which half of the
// container may move, and at 0xa65a2 it moves the divider itself through byte
// 0x4e8, ?Move@CPaneDivider@@UEAAXAEAVCPoint@@H@Z (0xac650).  OpenMFC's
// CPaneDivider has no member at +0x1ac (see CountsHiddenPanes above) and its
// ?Move@ is an empty stub, so neither half can be reproduced.
// Left as a stub; only the parameter list is corrected, to the shape the export
// ABI passes and that CPaneContainerManager.cpp:198 already declares.
// Symbol: ?StretchPaneContainer@CPaneContainer@@UEAAHHHHHAEAPEAX@Z
extern "C" int MS_ABI impl__StretchPaneContainer_CPaneContainer__UEAAHHHHHAEAPEAX_Z(
    void* pThis, int nOffset, int bStretchHorz, int bLeftBar, int bMoveSlider, void** phdwp) {
    (void)pThis; (void)nOffset; (void)bStretchHorz; (void)bLeftBar; (void)bMoveSlider; (void)phdwp;
    return 0;
}
