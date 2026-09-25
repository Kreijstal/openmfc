// CRecentDockSiteInfo — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Every body below is transcribed from the retail mfc140u.dll export named in
// its comment (disassembled with the campaign's disas.py; the function bodies
// are the same instruction streams as mfc140.dll's, only the RVAs and the
// call/RIP displacements differ -- all RVAs quoted here are mfc140u unless a
// comment says otherwise).
//
// Layout.  include/openmfc/afxmfc.h models CRecentDockSiteInfo as an opaque
// 288-byte block (it is embedded by value in CPane at +0x2b8), so the layout
// is pinned here from atlmfc/include/afxrecentdocksiteinfo.h and checked
// against the offsets the retail bodies touch:
//   0x000 vfptr                        (CObject base)
//   0x008 CRect  m_rectRecentFloatingRect   Init: SetRect(this+8, ...)
//   0x018 DWORD  m_dwRecentAlignmentToFrame Init: movl $0x1000,0x18
//   0x01c int    m_nRecentTabNumber         Init: movl $-1,0x1c
//   0x020 int    m_nRecentRowIndex          Init: mov %eax,0x20
//   0x028 CDockSite*        m_pRecentDockBar
//   0x030 CDockingPanesRow* m_pRecentDockBarRow
//   0x038 HWND   m_hRecentDefaultSlider     GetRecentDefaultPaneDivider reads +0x38
//   0x040 HWND   m_hRecentMiniFrame
//   0x048 CRecentPaneContainerInfo m_recentSliderInfo     ctor: ??0CRecentPaneContainerInfo(this+0x48)
//   0x0b0 CRecentPaneContainerInfo m_recentMiniFrameInfo  ctor: ??0CRecentPaneContainerInfo(this+0xb0)
//   0x118 CPane* m_pBar                     ctor: mov %rbx,0x118(%rdi)
//   0x120 sizeof (the scalar deleting dtor passes 0x120 as the size argument
//         on its flags&4 path -- to the folded `ret` at 0x27d0, so nothing
//         is freed there; the value is evidence of sizeof, not a free)
// CRecentPaneContainerInfo's own 104-byte layout is the one declared in
// detail/CRecentPaneContainerInfoSupport.h (S_Crecentpanecontainerinfo; its
// offset static_asserts live in detail/CRecentPaneContainerInfoSupport.cpp,
// and the size is re-asserted below).
//
// Retail vftable (.rdata 0x1802fe0f8 in mfc140u; the ctor and dtor store it):
//   0 ?GetRuntimeClass@CObject@@ (0x37a0)   1 scalar deleting dtor (0xd1ae0)
//   2..4 folded `ret` (0x27d0)              5 StoreDockInfo (0xd1c10)
//   6 SetInfo (0xd1f20)
//
// Virtual calls on the two by-value CRecentPaneContainerInfo members (retail
// dispatches StoreDockInfo / SetInfo through their vftable slots 5 / 6: it
// loads the slot with `mov 0x28(%rax),%rax` / `mov 0x30(%rax),%rax` and then
// calls / tail-jumps through the indirect-call pointer at 0x1802c7b30
// (mfc140u; not an import -- the Control Flow Guard dispatch pointer)) are
// bound statically to the CRecentPaneContainerInfo exports here.  Deviation, deliberate: those members
// are always exactly CRecentPaneContainerInfo (no class in the atlmfc headers
// derives from it), and
// OpenMFC's CPane zero-fills m_recentDockInfo instead of constructing it, so
// their vfptrs are NULL in every CPane this DLL creates; the static call
// names the same function retail's dispatch resolves to, without faulting on
// those objects.  Note that OpenMFC's bodies of those two exports (docking/
// CRecentPaneContainerInfo.cpp) are not retail's: its StoreDockInfo records
// only m_pRecentBarContainer, and its SetInfo is a plain copy, whereas retail
// ?SetInfo@CRecentPaneContainerInfo@@ (RVA 0xd19c0, mfc140u) also adjusts the
// counter at +0x50 of the old/new CPaneContainer pointers (inc new, dec old,
// calling 0xa4750 when it drops to <= 0) and does not copy m_bIsRecentLeftBar.

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CRecentPaneContainerInfoSupport.h"

#include <cstdlib>

// ---- sibling thunks (all defined in the tree; signatures from their definitions)
// docking/CRecentPaneContainerInfo.cpp
extern "C" void* MS_ABI impl___0CRecentPaneContainerInfo__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__Init_CRecentPaneContainerInfo__QEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl___4CRecentPaneContainerInfo__QEAAAEAV0_AEAV0__Z(void* pThis, void* pSrc);
// (dtor, SetInfo and StoreDockInfo of CRecentPaneContainerInfo are declared by
//  detail/CRecentPaneContainerInfoSupport.h)
// core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const CObject* pThis);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
// core/window/CWnd.cpp, core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void  MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);
// docking/CDockablePane.cpp, docking/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneDivider__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(const CDockablePane* pThis);
// docking/CBasePane.cpp
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(const CBasePane* pThis, int bNoAssert);
extern "C" void* MS_ABI impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(const CBasePane* pThis);
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis);
// detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// core/runtime/Globals.cpp
extern "C" unsigned int impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA;

// ---- this class's own thunks, referenced by the vftable below
extern "C" void MS_ABI impl___1CRecentDockSiteInfo__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__Init_CRecentDockSiteInfo__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__StoreDockInfo_CRecentDockSiteInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane___Z(
    void* pThis, void* pRecentContainer, void* pTabbedBar);
extern "C" void MS_ABI impl__SetInfo_CRecentDockSiteInfo__UEAAXHAEAV1__Z(void* pThis, int bForSlider, void* pSrcInfo);

namespace {

typedef openmfc::detail::crecentpanecontainerinfo::S_Crecentpanecontainerinfo S_PaneContainerInfo;

struct S_RecentDockSiteInfo {
    void*               vfptr;                      // 0x000
    RECT                m_rectRecentFloatingRect;   // 0x008
    DWORD               m_dwRecentAlignmentToFrame; // 0x018
    int                 m_nRecentTabNumber;         // 0x01c
    int                 m_nRecentRowIndex;          // 0x020
    void*               m_pRecentDockBar;           // 0x028  CDockSite*
    void*               m_pRecentDockBarRow;        // 0x030  CDockingPanesRow*
    HWND                m_hRecentDefaultSlider;     // 0x038
    HWND                m_hRecentMiniFrame;         // 0x040
    S_PaneContainerInfo m_recentSliderInfo;         // 0x048
    S_PaneContainerInfo m_recentMiniFrameInfo;      // 0x0b0
    void*               m_pBar;                     // 0x118  CPane*
};
static_assert(sizeof(S_PaneContainerInfo) == 104, "CRecentPaneContainerInfo is 104 bytes");
static_assert(offsetof(S_RecentDockSiteInfo, m_rectRecentFloatingRect)   == 0x08,  "m_rectRecentFloatingRect");
static_assert(offsetof(S_RecentDockSiteInfo, m_dwRecentAlignmentToFrame) == 0x18,  "m_dwRecentAlignmentToFrame");
static_assert(offsetof(S_RecentDockSiteInfo, m_nRecentTabNumber)         == 0x1c,  "m_nRecentTabNumber");
static_assert(offsetof(S_RecentDockSiteInfo, m_nRecentRowIndex)          == 0x20,  "m_nRecentRowIndex");
static_assert(offsetof(S_RecentDockSiteInfo, m_pRecentDockBar)           == 0x28,  "m_pRecentDockBar");
static_assert(offsetof(S_RecentDockSiteInfo, m_pRecentDockBarRow)        == 0x30,  "m_pRecentDockBarRow");
static_assert(offsetof(S_RecentDockSiteInfo, m_hRecentDefaultSlider)     == 0x38,  "m_hRecentDefaultSlider");
static_assert(offsetof(S_RecentDockSiteInfo, m_hRecentMiniFrame)         == 0x40,  "m_hRecentMiniFrame");
static_assert(offsetof(S_RecentDockSiteInfo, m_recentSliderInfo)         == 0x48,  "m_recentSliderInfo");
static_assert(offsetof(S_RecentDockSiteInfo, m_recentMiniFrameInfo)      == 0xb0,  "m_recentMiniFrameInfo");
static_assert(offsetof(S_RecentDockSiteInfo, m_pBar)                     == 0x118, "m_pBar");
static_assert(sizeof(S_RecentDockSiteInfo) == 0x120, "retail sizeof(CRecentDockSiteInfo) == 288");
static_assert(sizeof(S_RecentDockSiteInfo) == sizeof(CRecentDockSiteInfo),
              "must match the opaque block include/openmfc/afxmfc.h embeds in CPane");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads CWnd::m_hWnd at +0x40");

inline S_RecentDockSiteInfo* Self(void* p) { return static_cast<S_RecentDockSiteInfo*>(p); }

// Every per-mode accessor in retail starts with the same three instructions
//   neg %edx ; sbb %rax,%rax ; and $-0x68,%rax
// and then either loads from, or (GetRecentDockedRect, GetRecentListOfPanes,
// via two `add`s) returns the address this + 0xb0 + field + rax.
// i.e. bForSlider selects m_recentSliderInfo (+0x48), otherwise
// m_recentMiniFrameInfo (+0xb0); 0x68 is sizeof(CRecentPaneContainerInfo).
// In the accessor comments below, "slider +X / mini-frame +Y" gives the two
// resulting offsets from `this`; the instruction's displacement is always the
// mini-frame one (Y), and X = Y - 0x68.
inline S_PaneContainerInfo* InfoFor(S_RecentDockSiteInfo* s, int bForSlider) {
    return bForSlider ? &s->m_recentSliderInfo : &s->m_recentMiniFrameInfo;
}

// m_recentXxxInfo = src.m_recentXxxInfo, with retail's self-assignment result.
// Retail ??4CRecentPaneContainerInfo (entry RVA 0xd1ff0, mfc140u) has no self
// test: it copies the scalars, RemoveAll()s the destination list, then walks
// the source list from its m_pNodeHead -- which on x = x was just zeroed -- so
// self-assignment leaves the list empty.  OpenMFC's export of that operator
// guards `s != src` and would keep the list, so the self case is routed
// through a byte view of the same object with a NULL head (the same trick
// SaveListOfRecentPanes uses below), which reproduces retail's result.
inline void AssignPaneInfo(S_PaneContainerInfo* dst, S_PaneContainerInfo* src) {
    if (dst != src) {
        impl___4CRecentPaneContainerInfo__QEAAAEAV0_AEAV0__Z(dst, src);
        return;
    }
    S_PaneContainerInfo view = *dst;
    view.m_lstRecentListOfBars.pNodeHead = nullptr;
    impl___4CRecentPaneContainerInfo__QEAAAEAV0_AEAV0__Z(dst, &view);
}

inline HWND HwndOf(const void* pWnd) {
    return static_cast<const CWnd*>(pWnd)->m_hWnd;
}

// Scalar deleting destructor, retail 0xd1ae0 (vftable slot 1):
//   ~CRecentDockSiteInfo();
//   if (flags & 1) { if (flags & 4) <folded ret 0x27d0>(this, 0x120); else free(this); }
// (free is import slot 0x1802c74e8, api-ms-win-crt-heap free.)
void* MS_ABI RecentDockSiteInfo_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CRecentDockSiteInfo__UEAA_XZ(pThis);
    if ((flags & 1) && !(flags & 4)) std::free(pThis);
    return pThis;
}
// Slots 2..4 (Serialize / AssertValid / Dump) are the folded `ret` at 0x27d0.
void MS_ABI RecentDockSiteInfo_NoOp(void* /*pThis*/, void* /*arg*/) {}

#define VT(fn) reinterpret_cast<const void*>(&fn)
const void* const g_CRecentDockSiteInfo_vtbl[7] = {
    VT(impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ),
    VT(RecentDockSiteInfo_ScalarDeletingDtor),
    VT(RecentDockSiteInfo_NoOp),
    VT(RecentDockSiteInfo_NoOp),
    VT(RecentDockSiteInfo_NoOp),
    VT(impl__StoreDockInfo_CRecentDockSiteInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane___Z),
    VT(impl__SetInfo_CRecentDockSiteInfo__UEAAXHAEAV1__Z),
};
#undef VT

} // namespace

// CRecentDockSiteInfo::CRecentDockSiteInfo(CPane* pBar) -- retail 0xd1a80:
//   vfptr = CRecentDockSiteInfo vftable;
//   m_rectRecentFloatingRect = {0,0,0,0};            // two zero qword stores at +8/+0x10
//   m_recentSliderInfo.CRecentPaneContainerInfo();    // this+0x48
//   m_recentMiniFrameInfo.CRecentPaneContainerInfo(); // this+0xb0
//   m_pBar = pBar;                                    // +0x118
//   Init();
//   return this;
// Symbol: ??0CRecentDockSiteInfo@@QEAA@PEAVCPane@@@Z
extern "C" void* MS_ABI impl___0CRecentDockSiteInfo__QEAA_PEAVCPane___Z(void* pThis, void* pPane) {
    S_RecentDockSiteInfo* s = Self(pThis);
    s->vfptr = const_cast<void*>(static_cast<const void*>(g_CRecentDockSiteInfo_vtbl));
    s->m_rectRecentFloatingRect = RECT{0, 0, 0, 0};
    impl___0CRecentPaneContainerInfo__QEAA_XZ(&s->m_recentSliderInfo);
    impl___0CRecentPaneContainerInfo__QEAA_XZ(&s->m_recentMiniFrameInfo);
    s->m_pBar = pPane;
    impl__Init_CRecentDockSiteInfo__QEAAXXZ(pThis);
    return pThis;
}

// CRecentDockSiteInfo::~CRecentDockSiteInfo -- retail 0xd1b30:
//   vfptr = CRecentDockSiteInfo vftable;
//   then the two CRecentPaneContainerInfo members are destroyed in reverse
//   declaration order, each inlined as "vfptr = CRecentPaneContainerInfo
//   vftable; list vfptr = CList vftable; CList::RemoveAll()" --
//   m_recentMiniFrameInfo (+0xb0, list +0xd0) first, then m_recentSliderInfo
//   (+0x48, list +0x68, tail jump).
// Here each member is destroyed through the CRecentPaneContainerInfo
// destructor export.  OpenMFC's body of that export only empties the list
// (freeing nodes one by one with its own allocator, see SaveListOfRecentPanes
// below); it does not re-store the member / list vfptrs as retail does.
// Signature corrected: the generated list had no `this` and returned void*.
// Symbol: ??1CRecentDockSiteInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1CRecentDockSiteInfo__UEAA_XZ(void* pThis) {
    S_RecentDockSiteInfo* s = Self(pThis);
    s->vfptr = const_cast<void*>(static_cast<const void*>(g_CRecentDockSiteInfo_vtbl));
    impl___1CRecentPaneContainerInfo__UEAA_XZ(&s->m_recentMiniFrameInfo);
    impl___1CRecentPaneContainerInfo__UEAA_XZ(&s->m_recentSliderInfo);
}

// CRecentDockSiteInfo::CleanUp -- retail 0xd1be0:
//   Init();
//   m_recentSliderInfo.Init();        // ?Init@CRecentPaneContainerInfo@@, this+0x48
//   m_recentMiniFrameInfo.Init();     // same, this+0xb0 (tail jump)
// Symbol: ?CleanUp@CRecentDockSiteInfo@@QEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CRecentDockSiteInfo__QEAAXXZ(void* pThis) {
    S_RecentDockSiteInfo* s = Self(pThis);
    impl__Init_CRecentDockSiteInfo__QEAAXXZ(pThis);
    impl__Init_CRecentPaneContainerInfo__QEAAXXZ(&s->m_recentSliderInfo);
    impl__Init_CRecentPaneContainerInfo__QEAAXXZ(&s->m_recentMiniFrameInfo);
}

// CRecentDockSiteInfo::GetRecentDefaultPaneDivider -- retail 0xd1ee0:
//   CWnd* p = CWnd::FromHandlePermanent(m_hRecentDefaultSlider);   // +0x38
//   return (p != NULL && p->IsKindOf(RUNTIME_CLASS(CPaneDivider))) ? p : NULL;
// (the CRuntimeClass the body passes is the one whose m_lpszClassName is
// "CPaneDivider").  Note OpenMFC's own CPaneDivider (include/openmfc/afxmfc.h)
// derives from CObject rather than CWnd, so a divider this DLL allocates from
// that declaration is never in the permanent map; only a CWnd whose runtime
// class chains to CPaneDivider (e.g. one built by a client compiled against
// the shipping headers) can pass.  The code path is retail's regardless.
// Symbol: ?GetRecentDefaultPaneDivider@CRecentDockSiteInfo@@QEAAPEAVCPaneDivider@@XZ
extern "C" void* MS_ABI impl__GetRecentDefaultPaneDivider_CRecentDockSiteInfo__QEAAPEAVCPaneDivider__XZ(void* pThis) {
    CWnd* p = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(Self(pThis)->m_hRecentDefaultSlider);
    if (p == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(p), impl__GetThisClass_CPaneDivider__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return p;
}

// CRecentDockSiteInfo::GetRecentDockedPercent -- retail 0xd1e00:
//   return (bForSlider ? m_recentSliderInfo : m_recentMiniFrameInfo).m_nRecentPercent;
//   // slider +0x60 / mini-frame +0xc8
// Symbol: ?GetRecentDockedPercent@CRecentDockSiteInfo@@QEAAHH@Z
extern "C" int MS_ABI impl__GetRecentDockedPercent_CRecentDockSiteInfo__QEAAHH_Z(void* pThis, int bForSlider) {
    return InfoFor(Self(pThis), bForSlider)->m_nRecentPercent;
}

// CRecentDockSiteInfo::GetRecentDockedRect -- retail 0xd1de0:
//   return &(bForSlider ? m_recentSliderInfo : m_recentMiniFrameInfo).m_rectDockedRect;
//   // slider this+0x50 / mini-frame this+0xb8
// Symbol: ?GetRecentDockedRect@CRecentDockSiteInfo@@QEAAAEAVCRect@@H@Z
extern "C" void* MS_ABI impl__GetRecentDockedRect_CRecentDockSiteInfo__QEAAAEAVCRect__H_Z(void* pThis, int bForSlider) {
    return &InfoFor(Self(pThis), bForSlider)->m_rectDockedRect;
}

// CRecentDockSiteInfo::GetRecentListOfPanes -- retail 0xd1ec0:
//   return &(bForSlider ? m_recentSliderInfo : m_recentMiniFrameInfo).m_lstRecentListOfBars;
//   // slider this+0x68 / mini-frame this+0xd0
// Symbol: ?GetRecentListOfPanes@CRecentDockSiteInfo@@QEAAAEAV?$CList@PEAUHWND__@@PEAU1@@@H@Z
extern "C" void* MS_ABI impl__GetRecentListOfPanes_CRecentDockSiteInfo__QEAAAEAV__CList_PEAUHWND____PEAU1___H_Z(void* pThis, int bForSlider) {
    return &InfoFor(Self(pThis), bForSlider)->m_lstRecentListOfBars;
}

// CRecentDockSiteInfo::GetRecentPaneContainer -- retail 0xd1da0:
//   return (bForSlider ? m_recentSliderInfo : m_recentMiniFrameInfo).m_pRecentBarContainer;
//   // slider +0xa0 / mini-frame +0x108
// Symbol: ?GetRecentPaneContainer@CRecentDockSiteInfo@@QEAAPEAVCPaneContainer@@H@Z
extern "C" void* MS_ABI impl__GetRecentPaneContainer_CRecentDockSiteInfo__QEAAPEAVCPaneContainer__H_Z(void* pThis, int bForSlider) {
    return InfoFor(Self(pThis), bForSlider)->m_pRecentBarContainer;
}

// CRecentDockSiteInfo::GetRecentTabContainer -- retail 0xd1dc0:
//   return (bForSlider ? m_recentSliderInfo : m_recentMiniFrameInfo).m_pRecentContainerOfTabWnd;
//   // slider +0xa8 / mini-frame +0x110
// Symbol: ?GetRecentTabContainer@CRecentDockSiteInfo@@QEAAPEAVCPaneContainer@@H@Z
extern "C" void* MS_ABI impl__GetRecentTabContainer_CRecentDockSiteInfo__QEAAPEAVCPaneContainer__H_Z(void* pThis, int bForSlider) {
    return InfoFor(Self(pThis), bForSlider)->m_pRecentContainerOfTabWnd;
}

// CRecentDockSiteInfo::Init -- retail 0xd1b90:
//   ::SetRect(&m_rectRecentFloatingRect, 10, 10, 110, 110);   // import slot 0x1802c7140 (SetRect)
//   m_nRecentTabNumber = -1;
//   m_nRecentRowIndex = 0;
//   m_pRecentDockBar = NULL; m_pRecentDockBarRow = NULL;
//   m_hRecentDefaultSlider = NULL; m_hRecentMiniFrame = NULL;
//   m_dwRecentAlignmentToFrame = CBRS_ALIGN_LEFT;              // 0x1000
// The two CRecentPaneContainerInfo members are not touched (CleanUp resets them).
// Symbol: ?Init@CRecentDockSiteInfo@@QEAAXXZ
extern "C" void MS_ABI impl__Init_CRecentDockSiteInfo__QEAAXXZ(void* pThis) {
    S_RecentDockSiteInfo* s = Self(pThis);
    ::SetRect(&s->m_rectRecentFloatingRect, 10, 10, 110, 110);
    s->m_nRecentTabNumber = -1;
    s->m_nRecentRowIndex = 0;
    s->m_pRecentDockBar = nullptr;
    s->m_pRecentDockBarRow = nullptr;
    s->m_hRecentDefaultSlider = nullptr;
    s->m_hRecentMiniFrame = nullptr;
    s->m_dwRecentAlignmentToFrame = 0x1000;   // CBRS_ALIGN_LEFT
}

// CRecentDockSiteInfo::IsRecentLeftPane -- retail 0xd1e20:
//   return (bForSlider ? m_recentSliderInfo : m_recentMiniFrameInfo).m_bIsRecentLeftBar;
//   // slider +0x64 / mini-frame +0xcc
// Symbol: ?IsRecentLeftPane@CRecentDockSiteInfo@@QEAAHH@Z
extern "C" int MS_ABI impl__IsRecentLeftPane_CRecentDockSiteInfo__QEAAHH_Z(void* pThis, int bForSlider) {
    return InfoFor(Self(pThis), bForSlider)->m_bIsRecentLeftBar;
}

// CRecentDockSiteInfo::SaveListOfRecentPanes -- retail 0xd1e40 (absent from
// mfc140u_rva_symbols.json, but mfc140u's own export table resolves this
// symbol to 0xd1e40; its mfc140 twin is 0xd2310, a -0x4d0 shift like its
// neighbours, and the two are the same instruction stream apart from
// call/RIP displacements).  Both arms are the same code on a different list:
//   CList<HWND,HWND>& dst = (bForSlider ? m_recentSliderInfo
//                                        : m_recentMiniFrameInfo).m_lstRecentListOfBars;  // +0x68 / +0xd0
//   dst.RemoveAll();                          // call target is the ?RemoveAll@CPtrList@@
//                                             //   export (identical code folded onto it)
//   dst.AddTail(&lstOrg);                     // inlined: ENSURE(&lstOrg != NULL)
//                                             //   -> AfxThrowInvalidArgException();
//                                             // then for each node from lstOrg.m_pNodeHead
//                                             //   (+8), via pNext (+0): dst.AddTail(node->data) (+0x10)
// OpenMFC note: the node storage of these lists is owned by the
// CRecentPaneContainerInfo implementation (detail/CRecentPaneContainerInfo
// Support.cpp allocates and frees nodes one by one, not from a CPlex), so the
// list is not rebuilt here with the CPtrList exports -- mixing the two
// allocators on one list would corrupt the heap.  Instead the replacement is
// done by that class's own ??4 export, which (like retail's, mfc140 RVA
// 0xd24c0: scalar copies, RemoveAll, then AddTail per source node) empties
// the destination list and appends a copy of every element reachable from the
// source list's m_pNodeHead.  The source handed to it is a byte copy of the
// destination member itself with only m_pNodeHead redirected to lstOrg's
// first node, so every scalar field is assigned its own value and the only
// net change is dst.RemoveAll() + dst.AddTail(&lstOrg).  The copy owns
// nothing and is not destroyed.  When lstOrg is the destination list itself,
// retail's RemoveAll empties it before the copy loop reads it, so the result
// is an empty list; the view is given a NULL head in that case to get the
// same result without reading freed nodes.
// Symbol: ?SaveListOfRecentPanes@CRecentDockSiteInfo@@QEAAXAEAV?$CList@PEAUHWND__@@PEAU1@@@H@Z
extern "C" void MS_ABI impl__SaveListOfRecentPanes_CRecentDockSiteInfo__QEAAXAEAV__CList_PEAUHWND____PEAU1___H_Z(
    void* pThis, void* pLstOrg, int bForSlider) {
    S_PaneContainerInfo* pDst = InfoFor(Self(pThis), bForSlider);
    S_PaneContainerInfo view = *pDst;
    view.m_lstRecentListOfBars.pNodeHead =
        (pLstOrg != nullptr && pLstOrg != &pDst->m_lstRecentListOfBars)
            ? static_cast<openmfc::detail::crecentpanecontainerinfo::ListS*>(pLstOrg)->pNodeHead
            : nullptr;
    impl___4CRecentPaneContainerInfo__QEAAAEAV0_AEAV0__Z(pDst, &view);
    if (pLstOrg == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
}

// CRecentDockSiteInfo::SetInfo -- retail 0xd1f20 (vftable slot 6; absent from
// mfc140u_rva_symbols.json -- the mfc140 map has it at 0xd23f0 -- so it was
// located from mfc140u vftable slot 6; mfc140u's export table also resolves
// the symbol to 0xd1f20):
//   if (bForSlider) {
//       m_dwRecentAlignmentToFrame = src.m_dwRecentAlignmentToFrame;   // +0x18
//       m_hRecentDefaultSlider     = src.m_hRecentDefaultSlider;       // +0x38
//       m_recentSliderInfo.SetInfo(src.m_recentSliderInfo);          // +0x48, vslot +0x30, tail jump
//   } else {
//       m_rectRecentFloatingRect   = src.m_rectRecentFloatingRect;     // +0x08, 16 bytes
//       m_hRecentMiniFrame         = src.m_hRecentMiniFrame;           // +0x40
//       m_recentMiniFrameInfo.SetInfo(src.m_recentMiniFrameInfo);    // +0xb0, vslot +0x30, tail jump
//   }
// The member SetInfo is bound statically (see the file header).  Signature
// corrected: the generated list had no `this`.
// Symbol: ?SetInfo@CRecentDockSiteInfo@@UEAAXHAEAV1@@Z
extern "C" void MS_ABI impl__SetInfo_CRecentDockSiteInfo__UEAAXHAEAV1__Z(void* pThis, int bForSlider, void* pSrcInfo) {
    S_RecentDockSiteInfo* s = Self(pThis);
    S_RecentDockSiteInfo* src = Self(pSrcInfo);
    if (bForSlider) {
        s->m_dwRecentAlignmentToFrame = src->m_dwRecentAlignmentToFrame;
        s->m_hRecentDefaultSlider = src->m_hRecentDefaultSlider;
        impl__SetInfo_CRecentPaneContainerInfo__UEAAXAEAV1__Z(&s->m_recentSliderInfo, &src->m_recentSliderInfo);
    } else {
        s->m_rectRecentFloatingRect = src->m_rectRecentFloatingRect;
        s->m_hRecentMiniFrame = src->m_hRecentMiniFrame;
        impl__SetInfo_CRecentPaneContainerInfo__UEAAXAEAV1__Z(&s->m_recentMiniFrameInfo, &src->m_recentMiniFrameInfo);
    }
}

// CRecentDockSiteInfo::StoreDockInfo(CPaneContainer* pRecentContainer,
// CDockablePane* pTabbedBar) -- retail 0xd1c10 (vftable slot 5; absent from
// mfc140u_rva_symbols.json -- the mfc140 map has it at 0xd20e0 -- so it was
// located from mfc140u vftable slot 5; mfc140u's export table also resolves
// the symbol to 0xd1c10).  Transcription:
//   CDockablePane* pBar = (m_pBar != NULL && m_pBar->IsKindOf(RUNTIME_CLASS(CDockablePane)))
//                         ? (CDockablePane*)m_pBar : NULL;                          // +0x118
//   CPaneDivider* pDefaultSlider =
//       (pTabbedBar != NULL ? pTabbedBar : pBar)->GetDefaultPaneDivider();          // 0x45e80
//   CPaneFrameWnd* pMiniFrame = pBar->GetParentMiniFrame(FALSE);                    // slot 140 (+0x460)
//   if (pMiniFrame != NULL) {
//       CWnd* pOld = CWnd::FromHandlePermanent(m_hRecentMiniFrame);                 // +0x40
//       CPaneFrameWnd* pOldMiniFrame =
//           (pOld != NULL && pOld->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd))) ? pOld : NULL;
//       m_hRecentMiniFrame = pMiniFrame->m_hWnd;
//       m_recentMiniFrameInfo.StoreDockInfo(pRecentContainer, pBar, pTabbedBar);    // +0xb0, vslot +0x28
//       pMiniFrame->ScreenToClient(&m_recentMiniFrameInfo.m_rectDockedRect);        // this+0xb8
//       ::GetWindowRect(pMiniFrame->m_hWnd, &m_rectRecentFloatingRect);             // import slot 0x1802c6c08 (GetWindowRect)
//       if (pOldMiniFrame != NULL)
//           ::PostMessage(pOldMiniFrame->m_hWnd, AFX_WM_CHECKEMPTYMINIFRAME, 0, 0); // import slot 0x1802c72b0 (PostMessageW)
//   } else if (pDefaultSlider != NULL) {
//       m_recentSliderInfo.StoreDockInfo(pRecentContainer, pBar, pTabbedBar);       // +0x48, vslot +0x28
//       pBar->GetDockSiteFrameWnd()->ScreenToClient(                                // slot 105 (+0x348)
//           &m_recentSliderInfo.m_rectDockedRect);                                  // this+0x50
//       m_hRecentDefaultSlider = pDefaultSlider->m_hWnd;                            // +0x38
//       m_dwRecentAlignmentToFrame = pDefaultSlider->GetCurrentAlignment();         // slot 103 (+0x338)
//   } else {
//       m_hRecentMiniFrame = NULL;
//       m_recentMiniFrameInfo.StoreDockInfo(NULL, pBar, NULL);                      // +0xb0, vslot +0x28
//   }
// Virtual calls are bound statically: CDockablePane's vftable has
// ?GetParentMiniFrame@CBasePane@@ in slot 140 and ?GetDockSiteFrameWnd@CBasePane@@
// in slot 105, and CPaneDivider's has ?GetCurrentAlignment@CBasePane@@ in slot
// 103 (read from the retail vftables); a client subclass overriding one of
// them would not be honoured.  The member StoreDockInfo is bound statically
// as explained in the file header.
// Deviations (each replaces a retail NULL dereference with a return/skip):
//   * pBar == NULL (m_pBar unset or not a CDockablePane) returns at once;
//     retail faults: inside GetDefaultPaneDivider (it reads this+0x4c0) when
//     pTabbedBar is also NULL, otherwise on pBar's vfptr.  OpenMFC's CPane zero-fills its
//     m_recentDockInfo instead of running the constructor above, so m_pBar is
//     NULL there and this call is a no-op for panes this DLL constructs.
//   * a NULL GetDockSiteFrameWnd() skips the ScreenToClient.
// Signature corrected: the generated list had no `this` (docking/
// CPaneContainer.cpp, CPaneDivider.cpp and CMultiPaneFrameWnd.cpp already call
// this export with this three-pointer shape).
// Symbol: ?StoreDockInfo@CRecentDockSiteInfo@@UEAAXPEAVCPaneContainer@@PEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__StoreDockInfo_CRecentDockSiteInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane___Z(
    void* pThis, void* pRecentContainer, void* pTabbedBar) {
    S_RecentDockSiteInfo* s = Self(pThis);

    CDockablePane* pBar = nullptr;
    if (s->m_pBar != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(static_cast<CPane*>(s->m_pBar)),
            impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ())) {
        pBar = static_cast<CDockablePane*>(static_cast<CPane*>(s->m_pBar));
    }
    if (pBar == nullptr) return;   // deviation: retail faults here (see above)

    void* pDefaultSlider = impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(
        pTabbedBar != nullptr ? static_cast<const CDockablePane*>(pTabbedBar) : pBar);
    void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pBar), FALSE);

    if (pMiniFrame != nullptr) {
        CWnd* pOld = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(s->m_hRecentMiniFrame);
        CWnd* pOldMiniFrame = nullptr;
        if (pOld != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pOld), impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ())) {
            pOldMiniFrame = pOld;
        }
        s->m_hRecentMiniFrame = HwndOf(pMiniFrame);
        impl__StoreDockInfo_CRecentPaneContainerInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane__1_Z(
            &s->m_recentMiniFrameInfo, pRecentContainer, pBar, pTabbedBar);
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(
            static_cast<const CWnd*>(pMiniFrame), &s->m_recentMiniFrameInfo.m_rectDockedRect);
        ::GetWindowRect(HwndOf(pMiniFrame), &s->m_rectRecentFloatingRect);
        if (pOldMiniFrame != nullptr)
            ::PostMessage(pOldMiniFrame->m_hWnd, impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA, 0, 0);
    } else if (pDefaultSlider != nullptr) {
        impl__StoreDockInfo_CRecentPaneContainerInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane__1_Z(
            &s->m_recentSliderInfo, pRecentContainer, pBar, pTabbedBar);
        void* pSite = impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(static_cast<const CBasePane*>(pBar));
        if (pSite != nullptr)   // deviation: retail calls ScreenToClient unguarded
            impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(
                static_cast<const CWnd*>(pSite), &s->m_recentSliderInfo.m_rectDockedRect);
        // pDefaultSlider passed IsKindOf(CPaneDivider) on a permanent-map CWnd
        // inside GetDefaultPaneDivider, so it is a CWnd (retail CPaneDivider
        // derives from CBasePane).
        s->m_hRecentDefaultSlider = HwndOf(pDefaultSlider);
        s->m_dwRecentAlignmentToFrame = impl__GetCurrentAlignment_CBasePane__UEBAKXZ(
            static_cast<const CBasePane*>(pDefaultSlider));
    } else {
        s->m_hRecentMiniFrame = nullptr;
        impl__StoreDockInfo_CRecentPaneContainerInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane__1_Z(
            &s->m_recentMiniFrameInfo, nullptr, pBar, nullptr);
    }
}

// CRecentDockSiteInfo::operator= -- retail 0xd1f70:
//   m_rectRecentFloatingRect   = src.m_rectRecentFloatingRect;     // +0x08, 16 bytes
//   m_dwRecentAlignmentToFrame = src.m_dwRecentAlignmentToFrame;   // +0x18
//   m_nRecentRowIndex          = src.m_nRecentRowIndex;            // +0x20
//   m_pRecentDockBar           = src.m_pRecentDockBar;             // +0x28
//   m_pRecentDockBarRow        = src.m_pRecentDockBarRow;          // +0x30
//   m_nRecentTabNumber         = src.m_nRecentTabNumber;           // +0x1c
//   m_hRecentDefaultSlider     = src.m_hRecentDefaultSlider;       // +0x38
//   m_hRecentMiniFrame         = src.m_hRecentMiniFrame;           // +0x40
//   m_recentSliderInfo    = src.m_recentSliderInfo;      // ??4CRecentPaneContainerInfo, +0x48
//   m_recentMiniFrameInfo = src.m_recentMiniFrameInfo;   // ??4CRecentPaneContainerInfo, +0xb0
//   return *this;
// No self-assignment test, and neither vfptr nor m_pBar is copied.  On x = x
// retail therefore empties both HWND lists (see AssignPaneInfo); the member
// assignments go through AssignPaneInfo so OpenMFC does the same.
// Signature corrected: the generated list had no `this`.
// Symbol: ??4CRecentDockSiteInfo@@QEAAAEAV0@AEAV0@@Z
extern "C" void* MS_ABI impl___4CRecentDockSiteInfo__QEAAAEAV0_AEAV0__Z(void* pThis, void* pSrc) {
    S_RecentDockSiteInfo* s = Self(pThis);
    S_RecentDockSiteInfo* src = Self(pSrc);
    s->m_rectRecentFloatingRect = src->m_rectRecentFloatingRect;
    s->m_dwRecentAlignmentToFrame = src->m_dwRecentAlignmentToFrame;
    s->m_nRecentRowIndex = src->m_nRecentRowIndex;
    s->m_pRecentDockBar = src->m_pRecentDockBar;
    s->m_pRecentDockBarRow = src->m_pRecentDockBarRow;
    s->m_nRecentTabNumber = src->m_nRecentTabNumber;
    s->m_hRecentDefaultSlider = src->m_hRecentDefaultSlider;
    s->m_hRecentMiniFrame = src->m_hRecentMiniFrame;
    AssignPaneInfo(&s->m_recentSliderInfo, &src->m_recentSliderInfo);
    AssignPaneInfo(&s->m_recentMiniFrameInfo, &src->m_recentMiniFrameInfo);
    return pThis;
}
