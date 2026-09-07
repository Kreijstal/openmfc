// CMDIClientAreaWnd — OpenMFC implementation.
// Sources: cbarcore.cpp
//
// ---------------------------------------------------------------------------
// Status of this translation unit (2026-09-07 decode pass)
// ---------------------------------------------------------------------------
// Every export below was looked up in the retail RVA map and disassembled
// (43 of the 45 assigned exports resolve; the two that appear absent under the
// Unicode names are present in the ANSI twin as ?LoadState@...@@QEAAHPEBDI@Z /
// ?SaveState@...@@QEAAHPEBDI@Z and were decoded from there).
//
// The decode is recorded per function below, but almost nothing here could be
// *implemented*: every retail body reaches into the CMDIClientAreaWnd instance
// at fixed offsets, and OpenMFC does not model this class at all —
//   * there is no `class CMDIClientAreaWnd` in include/openmfc/ (grep: the name
//     appears only in phase4/src and tools/harvest, never in a public header);
//   * there is no phase4/src/detail/CMDIClientAreaWndSupport.h shadow struct;
//   * `??0CMDIClientAreaWnd@@QEAA@XZ` below is a no-op that just returns
//     `pThis`, so no instance this DLL ever sees has its members initialised;
//   * the one object that would embed it, CMDIFrameWndEx, is declared in
//     include/openmfc/afxmfc.h:1428 as CMDIFrameWnd + `CMFCVisualManager*` +
//     `char _mdiframewndex_padding[128]`, i.e. nothing at all like the retail
//     18192-byte layout, and its ctor (phase4/src/core/frame/CMDIFrameWndEx.cpp)
//     never constructs a client-area sub-object.
// Dereferencing the retail offsets on such an instance would read uninitialised
// heap, which is strictly worse than a no-op, so the bodies stay stubs and the
// decode is recorded in comments instead. Per BRIEFING §6 these are reported as
// `stubbed`, not as implementations. The exceptions are the two bodies that
// touch no member at all: OnUpdateTabs and DrawNewGroupRect.
//
// ---------------------------------------------------------------------------
// Retail CMDIClientAreaWnd layout, transcribed from the constructor
// ??0CMDIClientAreaWnd@@QEAA@XZ (entry RVA 0x7ce90) and cross-checked against
// the destructor ??1CMDIClientAreaWnd@@UEAA@XZ (entry RVA 0x7d0b0).
// This is the information a shadow struct would need; see `headerRequests`.
// ---------------------------------------------------------------------------
//   +0x0000  CWnd base                (ctor calls ??0CWnd@@QEAA@XZ, RVA 0x288b10)
//            vftable stored = RVA 0x2eb4f8
//   +0x0040  CWnd::m_hWnd             (read as `mov 0x40(%rcx),%rcx` before
//                                      every ::SendMessage / ::GetWindowRect)
//   +0x00e8  CMFCTabCtrl m_wndTab     (ctor ??0CMFCTabCtrl@@QEAA@XZ RVA 0x137a00,
//                                      dtor RVA 0x137cb0; sizeof == 0x3d68, which
//                                      is both the `mov $0x3d68,%ecx` passed to
//                                      operator new in CreateTabGroup and the
//                                      m_nObjectSize of RUNTIME_CLASS(CMFCTabCtrl)
//                                      at RVA 0x310b30, 15720 == 0x3d68)
//   +0x3e50  int   (BOOL)             set together with +0x3e54 by EnableMDITabs
//   +0x3e54  int   (BOOL)             "flat MDI tabs enabled"; gates Serialize and
//                                      the CalcWindowRect / OnMDINext tab paths
//   +0x3e58  int   (BOOL)             last-active-tab activation; mirrored into
//                                      m_wndTab+0x334 and into every group
//   +0x3e60  CImageList (16 bytes)    dtor ??1CImageList@@UEAA@XZ RVA 0x294c80
//   +0x3e70  CMap-shaped, 0x38 bytes  vftable RVA 0x2dbb00, hash size 17,
//                                      block size 10; RemoveAll body is ICF-folded
//                                      with ?RemoveAll@CMapWordToPtr@@QEAAXXZ
//   +0x3ea8  CMDITabInfo m_mdiTabParams  (ctor ??0CMDITabInfo@@QEAA@XZ RVA 0x7c9d0;
//                                      EnableMDITabs copies exactly 0x30 bytes
//                                      from the const& argument into +0x3ea8, so
//                                      sizeof(CMDITabInfo) == 0x30)
//   +0x3ed8  CObList m_lstTabbedGroups   vftable RVA 0x328538 (slot 0 is
//                                      ?GetThisClass@CObList@@); m_pNodeHead
//                                      +0x3ee0, m_pNodeTail +0x3ee8, m_nCount
//                                      +0x3ef0, m_pNodeFree +0x3ef8, m_pBlocks
//                                      +0x3f00, m_nBlockSize +0x3f08 == 10.
//                                      List nodes are {pNext +0, pPrev +8,
//                                      data +0x10}.
//   +0x3f10  CMap-shaped, 0x38 bytes  vftable RVA 0x2eb2f8, hash size 17,
//                                      block size 10
//   +0x3f48  int   (BOOL)             "MDI tabbed groups enabled"; written by
//                                      EnableMDITabbedGroups, tested by
//                                      IsMemberOfMDITabGroup / OnEraseBkgnd /
//                                      IsKeepClientEdge / OnMDINext / OnDragComplete
//   +0x3f50  int                      re-entrancy guard tested at the top of
//                                      CalcWindowRect and UpdateTabs
//   +0x3f58  CObList (0x38 bytes)     vftable RVA 0x328538; a free-list of
//                                      detached groups — CreateTabGroup pops its
//                                      tail (?RemoveTail@CPtrList@@, ICF-folded,
//                                      RVA 0x2305d0) before allocating a new one
//   +0x3f90  int  == 40 in the ctor   group splitter/handle width
//   +0x3f94  int  == 40 in the ctor
//   +0x3f98  int  == 0
//   +0x3f9c  RECT m_rectNewGroup      zeroed by the ctor; ::SetRectEmpty'd by
//                                      OnCancelTabMove / OnMoveTabComplete
//   +0x3fb0  CStringList (0x38 bytes) vftable RVA 0x3285c8 (slot 0 is
//                                      ?GetThisClass@CStringList@@), block size 10
//   +0x3fe8  int                      group orientation; compared against 1
//                                      (== "columns") throughout
//   +0x3fec  int                      drag-in-progress flag; set to 1 by
//                                      OnDragComplete, guards
//                                      CalcWindowRectForMDITabbedGroups
//   sizeof  == 0x3ff0 (the highest member touched is the int at +0x3fec)
//
// Retail indirect calls in this class go through the CFG dispatcher at
// RVA 0x2c5bd0 (`call *0x2c5bd0(%rip)` with the callee already in %rax); that
// cell is not an import slot, so such sequences are plain virtual calls.

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Forward declaration of a sibling thunk defined later in THIS translation
// unit; CMDIClientAreaWnd methods exist only as impl__ thunks (BRIEFING §1),
// so OnUpdateTabs must reach UpdateTabs through the thunk, not through C++.
extern "C" void MS_ABI impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bSetActiveTabVisible);

// Thunks from sibling translation units used by DrawNewGroupRect below; the
// C++ methods do not exist in this DLL, only these exports (BRIEFING §1).
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC* pThis);
extern "C" void MS_ABI impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
    CDC* pThis, const RECT* lpRect, SIZE size, const RECT* lpRectLast, SIZE sizeLast,
    CBrush* pBrush, CBrush* pBrushLast);

// Symbol: ??0CMDIClientAreaWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMDIClientAreaWnd__QEAA_XZ(void* pThis) {
    return pThis;
}
// CMDIClientAreaWnd::~CMDIClientAreaWnd — retail entry RVA 0x7d0b0.
// Retail stores the vftable (RVA 0x2eb4f8), then drains m_lstTabbedGroups
// (+0x3ed8) by RemoveTail, `delete`ing each node payload through its vtable
// slot 1 (scalar deleting destructor, flag 1); drains the +0x3f58 free-list the
// same way but only for payloads that pass IsKindOf(RUNTIME_CLASS(CMFCTabCtrl))
// (descriptor at RVA 0x310b30); walks the +0x3f10 map deleting every value;
// then destroys, in order, the CStringList at +0x3fb0, the CObList at +0x3f58,
// the map at +0x3f10, the CObList at +0x3ed8, the map at +0x3e70, the
// CImageList at +0x3e60 and the CMFCTabCtrl at +0xe8, and tail-jumps to
// ??1CWnd@@UEAA@XZ (RVA 0x289bc0).
// STUB: none of those sub-objects exist in OpenMFC's model of this class, and
// this DLL never constructs one, so there is nothing to tear down.
// Symbol: ??1CMDIClientAreaWnd@@UEAA@XZ
extern "C" void MS_ABI impl___1CMDIClientAreaWnd__UEAA_XZ(void* pThis) {
    (void)pThis;
}
// Symbol: ?GetThisClass@CMDIClientAreaWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIClientAreaWnd__SAPEAUCRuntimeClass__XZ() {
    return &g_cmdiClientAreaRuntimeClass;
}
// Symbol: ?GetRuntimeClass@CMDIClientAreaWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIClientAreaWnd__UEBAPEAUCRuntimeClass__XZ(void* pThis) {
    return pThis ? &g_cmdiClientAreaRuntimeClass : nullptr;
}
// CMDIClientAreaWnd::OnUpdateTabs — retail entry RVA 0x7ec90. The whole body is
//     sub $0x28,%rsp ; xor %edx,%edx ; call 0x7efa0 ; xor %eax,%eax ; ret
// i.e. `UpdateTabs(FALSE); return 0;` — %rcx (this) is passed through
// untouched and no member is read, so this one is transcribable as-is.
// Symbol: ?OnUpdateTabs@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateTabs_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    (void)lParam;
    impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(pThis, FALSE);
    return 0;
}
// CMDIClientAreaWnd::UpdateTabs — retail entry RVA 0x7efa0.
// Returns immediately if the re-entrancy guard at +0x3f50 is non-zero. If
// +0x3f48 (tabbed groups) is set it calls the group updater at RVA 0x7f5b0 —
// ?UpdateMDITabbedGroups@CMDIClientAreaWnd@@QEAAXH@Z, also exported from this
// file — and returns. Otherwise it works on the embedded CMFCTabCtrl at
// +0xe8: bails if its m_hWnd (+0xe8+0x40) is null or if +0x3e50 is zero, reads
// the current tab count through the tab control's vtable slot at +0x368, then
// walks the MDI children with ::GetWindow(m_hWnd, GW_CHILD=5) / CWnd::FromHandle
// (RVA 0x289180), classifying each with IsKindOf against
// RUNTIME_CLASS(CMDIChildWndEx) (RVA 0x2eb7d8) and RUNTIME_CLASS(CMFCTabCtrl)
// (RVA 0x310b30) to add, remove and reorder tabs.
// STUB: every step needs the unmodeled +0xe8 CMFCTabCtrl sub-object.
// Symbol: ?UpdateTabs@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bSetActiveTabVisible) {
    (void)pThis;
    (void)bSetActiveTabVisible;
}
// CMDIClientAreaWnd::SetTaskbarTabOrder — retail entry RVA 0x7e7d0.
// Walks m_lstTabbedGroups (+0x3ee0 head, `data` at node+0x10), downcasts each
// payload with IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)) (RVA 0x310b30), and for
// each group iterates tabs [0, vtable+0x368) fetching each tab's window via
// vtable+0x370; when that window IsKindOf(RUNTIME_CLASS(CMDIChildWndEx))
// (RVA 0x2eb7d8), has a non-null m_hWnd, and
// ?IsTaskbarTabsSupportEnabled@CMDIChildWndEx@@ (RVA 0x84590) returns TRUE and
// its sub-object at +0x720 also has a non-null m_hWnd (+0x720+0x40 — the
// preceding `lea 0x720(%rsi),%rax; test %rax,%rax` at 0x7e862 is always true),
// it calls ?SetTaskbarTabOrder@CMDIChildWndEx@@QEAAXPEAV1@@Z (RVA 0x847b0) with
// a null predecessor.
// STUB: needs m_lstTabbedGroups and the CMFCTabCtrl/CMDIChildWndEx layouts.
// Symbol: ?SetTaskbarTabOrder@CMDIClientAreaWnd@@QEAAXXZ
extern "C" void MS_ABI impl__SetTaskbarTabOrder_CMDIClientAreaWnd__QEAAXXZ(void* pThis) {
    (void)pThis;
}
// CMDIClientAreaWnd::AdjustMDIChildren — retail entry RVA 0x7e5f0; repositions
// the MDI children owned by the given tab group.
// STUB: not transcribed — the body is entirely member/tab-control traffic on
// the unmodeled layout described in the header comment.
// Symbol: ?AdjustMDIChildren@CMDIClientAreaWnd@@AEAAXPEAVCMFCTabCtrl@@@Z
extern "C" void MS_ABI impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(void* pThis, void* pTabGroup) {
    (void)pThis;
    (void)pTabGroup;
}

// CMDIClientAreaWnd::ApplyParams — retail entry RVA 0x7d740. Pushes
// m_mdiTabParams (+0x3ea8) onto one tab group:
//   pTab->ModifyTabStyle(*(int*)(this+0x3eac))            (RVA 0x13d000)
//   pTab->vtable[+0x500](*(int*)(this+0x3ea8))            (SetLocation)
//   pTab->[+0x3d8]   = *(int*)(this+0x3eb4)
//   pTab->[+0x3fc]   = *(int*)(this+0x3ecc)
//   pTab->EnableTabDocumentsMenu(*(int*)(this+0x3ec0))    (RVA 0x13d430)
//   pTab->EnableAutoColor(*(int*)(this+0x3ebc))           (RVA 0x1a150)
//   pTab->[+0x1f0]   = *(int*)(this+0x3ec4)
//   pTab->vtable[+0x498](*(int*)(this+0x3ed0), TRUE)
//   pTab->EnableCustomToolTips(*(int*)(this+0x3eb8))      (RVA 0x1a990)
//   pTab->[+0x1e8] = 0; pTab->HideNoTabs(TRUE)            (RVA 0x13c6b0)
//   pTab->[+0x3e4] = 0; pTab->[+0x1e4] = 0
//   if (pTab->[+0x3e0] != *(int*)(this+0x3ec8)) { store it; if the tab control
//       has an HWND, ::InvalidateRect(hwnd, NULL, TRUE); ::UpdateWindow(hwnd); }
//   pTab->[+0x3e8] = pTab->[+0x3ec] = pTab->[+0x1f8] = 1
//   pTab->SetActiveTabBoldFont(TRUE)                      (RVA 0x13cdd0)
//   pTab->[+0xec] = pTab->[+0xf4] = pTab->[+0x200] = 1     (0x7d866..0x7d872,
//       stored after the SetActiveTabBoldFont call, in the epilogue)
// (vtable+0x500 is ?SetLocation@CMFCBaseTabCtrl@@UEAAXW4Location@1@@Z and
//  vtable+0x498 is ?SetTabBorderSize@CMFCBaseTabCtrl@@UEAAXHH@Z, read out of
//  the CMFCTabCtrl vftable at RVA 0x310520.)
// STUB: needs both CMDITabInfo at +0x3ea8 and the CMFCTabCtrl layout.
// Symbol: ?ApplyParams@CMDIClientAreaWnd@@AEAAXPEAVCMFCTabCtrl@@@Z
extern "C" void MS_ABI impl__ApplyParams_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(void* pThis, void* pTabGroup) {
    (void)pThis;
    (void)pTabGroup;
}

// CMDIClientAreaWnd::CalcWindowRect — retail entry RVA 0x7de90 (virtual).
// If the guard at +0x3f50 is set it returns without touching lpRect. Otherwise,
// when +0x3f48 (tabbed groups) is set it calls
// CalcWindowRectForMDITabbedGroups (RVA 0x7e320) and then CWnd::CalcWindowRect
// (RVA 0x28d240). In the flat-tabs case it deflates lpRect by the embedded tab
// control's window/client difference (::GetWindowRect / ::GetClientRect on
// +0xe8+0x40 and on m_hWnd), repositions the tab control with
// CWnd::SetWindowPos (RVA 0x2a7970) / CWnd::ShowWindow (RVA 0x2a79e0), and
// gives the parent CMDIFrameWndEx (GetParentFrame, RVA 0x28c680, checked with
// IsKindOf against RUNTIME_CLASS(CMDIFrameWndEx) at RVA 0x2ec3c8) a chance to
// adjust the rect through its vtable slot at +0x410.
// STUB: needs +0x3f48/+0x3f50/+0x3e50 and the embedded tab control.
// Symbol: ?CalcWindowRect@CMDIClientAreaWnd@@MEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CMDIClientAreaWnd__MEAAXPEAUtagRECT__I_Z(void* pThis, RECT* lpClientRect, unsigned int nAdjustType) {
    (void)pThis;
    (void)lpClientRect;
    (void)nAdjustType;
}

// CMDIClientAreaWnd::CalcWindowRectForMDITabbedGroups — retail entry RVA 0x7e320.
// Returns immediately if the drag flag at +0x3fec is set. Otherwise it moves
// the client-area window itself to lpRect with CWnd::SetWindowPos (RVA
// 0x2a7970, flags 0x10), and then, if m_lstTabbedGroups (+0x3ef0) is non-empty:
// with exactly one group it shows that group (ShowWindow, RVA 0x2a79e0) and
// gives it the whole rect, then calls AdjustMDIChildren (RVA 0x7e5f0); with
// several groups it sums each group's current extent (::GetWindowRect, using
// width when +0x3fe8 == 1 and height otherwise), then redistributes the space
// proportionally, inserting the splitter width from +0x3f90 between groups.
// STUB: needs m_lstTabbedGroups, +0x3fe8, +0x3fec and +0x3f90.
// Symbol: ?CalcWindowRectForMDITabbedGroups@CMDIClientAreaWnd@@QEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRectForMDITabbedGroups_CMDIClientAreaWnd__QEAAXPEAUtagRECT__I_Z(void* pThis, RECT* lpRect, unsigned int nAdjustType) {
    (void)pThis;
    (void)lpRect;
    (void)nAdjustType;
}

// CMDIClientAreaWnd::CloseAllWindows — retail entry RVA 0x811b0. Two distinct
// paths: with a tab group it walks that group's tabs from count-1 down to 0
// (vtable[+0x368] / vtable[+0x370]) and ::SendMessage(pChild->m_hWnd, WM_CLOSE,
// 0, 0) to every tab window that IsKindOf CMDIChildWndEx (RVA 0x2eb7d8). With a
// NULL argument — which is how Serialize's loading path at 0x83246 calls it —
// it instead collects the client area's own MDI children by walking
// ::GetWindow(m_hWnd, GW_CHILD) / CWnd::FromHandle into a local CObList and
// closes those.
// STUB: not transcribed in full — needs the CMFCTabCtrl tab enumeration and the
// m_lstTabbedGroups bookkeeping described in the header comment.
// Symbol: ?CloseAllWindows@CMDIClientAreaWnd@@QEAAXPEAVCMFCTabCtrl@@@Z
extern "C" void MS_ABI impl__CloseAllWindows_CMDIClientAreaWnd__QEAAXPEAVCMFCTabCtrl___Z(void* pThis, void* pTabGroup) {
    (void)pThis;
    (void)pTabGroup;
}

// CMDIClientAreaWnd::CreateNewTabGroup — retail entry RVA 0x806f0.
// Argument order verified from the call site inside MDITabNewGroup (the call at
// 0x81177 inside MDITabNewGroup, entry RVA 0x810d0): %rcx = this,
// %rdx = the tab group to split, %r8 = the CRect passed by value (16 bytes, so
// MSVC passes a pointer to a caller-owned copy), %r9d = the BOOL.
// STUB: not transcribed — it allocates/derives a sibling group and links it
// into m_lstTabbedGroups, all on the unmodeled layout.
// Symbol: ?CreateNewTabGroup@CMDIClientAreaWnd@@AEAAPEAVCMFCTabCtrl@@PEAV2@VCRect@@H@Z
extern "C" void* MS_ABI impl__CreateNewTabGroup_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__PEAV2_VCRect__H_Z(void* pThis, void* pTabGroup, const RECT* pRect, int bVert) {
    (void)pThis;
    (void)pTabGroup;
    (void)pRect;
    (void)bVert;
    return nullptr;
}

// CMDIClientAreaWnd::CreateTabGroup — retail entry RVA 0x7ecd0 (virtual).
// With a null argument it first tries to recycle: if +0x3ed4 is non-zero, the
// free-list at +0x3f58 has m_nCount > 0 and +0x3f48 is non-zero, it pops the
// tail (?RemoveTail@CPtrList@@, ICF-folded, RVA 0x2305d0) and keeps that object
// when it IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)) (RVA 0x310b30) and its m_hWnd is
// a live window. Otherwise it does `operator new(0x3d68)` (RVA 0x2840) followed
// by ??0CMFCTabCtrl@@QEAA@XZ (RVA 0x137a00). It then applies
// EnableCustomToolTips(+0x3eb8) (RVA 0x1a990), picks the parent (this when
// +0x3f48 is set, else GetParentFrame(), RVA 0x28c680) and, for a freshly
// created group, calls CMFCTabCtrl::Create (RVA 0x137d40) with the style bits
// from +0x3ea8/+0x3eac/+0x3eb4.
// STUB: needs +0x3ed4/+0x3f48/+0x3f58 and the CMDITabInfo fields.
// Symbol: ?CreateTabGroup@CMDIClientAreaWnd@@UEAAPEAVCMFCTabCtrl@@PEAV2@@Z
extern "C" void* MS_ABI impl__CreateTabGroup_CMDIClientAreaWnd__UEAAPEAVCMFCTabCtrl__PEAV2__Z(void* pThis, void* pTabGroup) {
    (void)pThis;
    (void)pTabGroup;
    return nullptr;
}

// CMDIClientAreaWnd::DrawNewGroupRect — transcribed from retail entry RVA
// 0x80f30. This is the one body in this class that is member-free: %rcx is
// overwritten at 0x80f61 without ever being read, so `this` is unused and the
// missing CMDIClientAreaWnd layout does not block it. Retail does, in order:
//     CWindowDC dc(CWnd::FromHandle(::GetDesktopWindow()));   // 0x2a1b60/0x289180
//     RECT rcNew, rcOld;
//     ::SetRectEmpty(&rcNew); ::SetRectEmpty(&rcOld);
//     if (pRectNew) ::CopyRect(&rcNew, pRectNew);
//     if (pRectOld) ::CopyRect(&rcOld, pRectOld);
//     dc.DrawDragRect(&rcNew, CSize(4,4), &rcOld, CSize(4,4), NULL, NULL);
// (RVA 0x2a3660; the 4x4 size is the qword at rbp+0x17 and it is passed BOTH
// in %r8 as `size` and at rsp+0x20 as `sizeLast` — the two stack slots that
// are zeroed, rsp+0x28 and rsp+0x30, are the two CBrush* arguments.)
// Note that a NULL argument is never forwarded: retail turns it into an empty
// rect, so DrawDragRect always receives two non-null pointers. That is
// reproduced here rather than passing the caller's pointers straight through.
// Symbol: ?DrawNewGroupRect@CMDIClientAreaWnd@@AEAAXPEBUtagRECT@@0@Z
extern "C" void MS_ABI impl__DrawNewGroupRect_CMDIClientAreaWnd__AEAAXPEBUtagRECT__0_Z(void* pThis, const RECT* pRectNew, const RECT* pRectOld) {
    (void)pThis;  // retail never reads `this` here

    CWnd* pDesktop = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetDesktopWindow());

    // CWindowDC exists in this DLL only as its impl__ thunks, so the object is
    // built in raw storage and torn down through the matching thunk.
    alignas(void*) unsigned char dcStorage[sizeof(CWindowDC)] = {};
    CWindowDC* pDC = reinterpret_cast<CWindowDC*>(dcStorage);
    impl___0CWindowDC__QEAA_PEAVCWnd___Z(pDC, pDesktop);

    RECT rectNew;
    RECT rectOld;
    ::SetRectEmpty(&rectNew);
    ::SetRectEmpty(&rectOld);
    if (pRectNew != nullptr) {
        ::CopyRect(&rectNew, pRectNew);
    }
    if (pRectOld != nullptr) {
        ::CopyRect(&rectOld, pRectOld);
    }

    SIZE size;
    size.cx = 4;
    size.cy = 4;
    impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
        pDC, &rectNew, size, &rectOld, size, nullptr, nullptr);

    impl___1CWindowDC__UEAA_XZ(pDC);
}

// CMDIClientAreaWnd::EnableMDITabbedGroups — retail entry RVA 0x7d3f0.
// If flat tabs (+0x3e54) are on it first turns them off via EnableMDITabs(FALSE)
// (RVA 0x7d2b0); hides the embedded tab control (CWnd::ShowWindow, RVA
// 0x2a79e0); records the active MDI child from
// ::SendMessage(m_hWnd, WM_MDIGETACTIVE, 0, 0); and, when bEnable differs from
// the stored +0x3f48, stores it and — on disable — walks m_lstTabbedGroups
// hiding each group and re-parenting its MDI children back to the client area
// with the WS_MAXIMIZE bit forced on.
// STUB: needs +0x3e54, +0x3f48, m_lstTabbedGroups and the CMDITabInfo copy.
// Symbol: ?EnableMDITabbedGroups@CMDIClientAreaWnd@@QEAAXHAEBVCMDITabInfo@@@Z
extern "C" void MS_ABI impl__EnableMDITabbedGroups_CMDIClientAreaWnd__QEAAXHAEBVCMDITabInfo___Z(void* pThis, int bEnable, const void* pParams) {
    (void)pThis;
    (void)bEnable;
    (void)pParams;
}

// CMDIClientAreaWnd::EnableMDITabs — retail entry RVA 0x7d2b0.
// If +0x3f48 (tabbed groups) is set it first calls EnableMDITabbedGroups(FALSE)
// (RVA 0x7d3f0). It then stores bEnable into +0x3e54 and +0x3e50, copies
// exactly 0x30 bytes of the CMDITabInfo argument into +0x3ea8 (three 16-byte
// moves), and calls ApplyParams(&m_wndTab) (RVA 0x7d740). On enable it calls
// UpdateTabs(FALSE) (RVA 0x7efa0); either way, when IsKeepClientEdge()
// (RVA 0x7fc30) is FALSE it calls CWnd::ModifyStyleEx (RVA 0x2a7650) to clear
// WS_EX_CLIENTEDGE (0x200) on enable and to set it on disable. Finally it shows
// the tab control when it has an HWND, ::BringWindowToTop(m_hWnd), and — only
// when both m_hWnd and the parent frame are live — calls the frame's virtual at
// +0x300 with TRUE (GetParentFrame, RVA 0x28c680) and then ::RedrawWindow with
// flags 0x585 (RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_UPDATENOW|RDW_FRAME)
// on the embedded tab control's m_hWnd (this+0x128 == m_wndTab+0x40, when that
// is non-null) and then on m_hWnd — the calls at 0x7d3cb and 0x7d3dd.
// STUB: needs +0x3e50/+0x3e54/+0x3ea8/+0x3f48 and the embedded tab control.
// Symbol: ?EnableMDITabs@CMDIClientAreaWnd@@QEAAXHAEBVCMDITabInfo@@@Z
extern "C" void MS_ABI impl__EnableMDITabs_CMDIClientAreaWnd__QEAAXHAEBVCMDITabInfo___Z(void* pThis, int bEnable, const void* pParams) {
    (void)pThis;
    (void)bEnable;
    (void)pParams;
}

// CMDIClientAreaWnd::EnableMDITabsLastActiveActivation — retail entry RVA 0x83930.
// Returns at once if +0x3e58 already equals bEnable. Otherwise it stores
// bEnable into +0x3e58 and into m_wndTab+0x334 (absolute this+0x41c) and walks
// m_lstTabbedGroups, storing bEnable at +0x334 of every payload that
// IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)) (RVA 0x310b30). Note the store at +0x334
// of a *null* pointer is not guarded in retail: when the downcast fails the
// group pointer is zeroed and the write still executes.
// STUB: needs +0x3e58, the embedded tab control and m_lstTabbedGroups.
// Symbol: ?EnableMDITabsLastActiveActivation@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableMDITabsLastActiveActivation_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bEnable) {
    (void)pThis;
    (void)bEnable;
}

// CMDIClientAreaWnd::FindActiveTabWnd — retail entry RVA 0x80d40.
// Walks m_lstTabbedGroups from +0x3ee0, downcasting each node payload
// (node+0x10) with IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)) (RVA 0x310b30), and
// returns the first group whose int at +0x408 is non-zero; null otherwise.
// STUB: needs m_lstTabbedGroups.
// Symbol: ?FindActiveTabWnd@CMDIClientAreaWnd@@QEAAPEAVCMFCTabCtrl@@XZ
extern "C" void* MS_ABI impl__FindActiveTabWnd_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(void* pThis) {
    (void)pThis;
    return nullptr;
}

// CMDIClientAreaWnd::FindActiveTabWndByActiveChild — retail entry RVA 0x80d00.
//     int nTab = -1;
//     return FindTabWndByChild((HWND)::SendMessage(m_hWnd, WM_MDIGETACTIVE, 0, 0),
//                              nTab);
// STUB: FindTabWndByChild below is itself blocked on m_lstTabbedGroups, and
// m_hWnd is part of the CWnd base that this DLL never initialises for this
// class, so issuing the ::SendMessage would only post to a garbage HWND.
// Symbol: ?FindActiveTabWndByActiveChild@CMDIClientAreaWnd@@QEAAPEAVCMFCTabCtrl@@XZ
extern "C" void* MS_ABI impl__FindActiveTabWndByActiveChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(void* pThis) {
    (void)pThis;
    return nullptr;
}

// CMDIClientAreaWnd::FindNextRegisteredWithTaskbarMDIChild(CMDIChildWndEx*) —
// retail entry RVA 0x7e990.
//     int nTab = -1;
//     CMFCTabCtrl* pGroup = FindTabWndByChild(pChild ? pChild->m_hWnd : NULL, nTab);
//     while (pGroup) {
//         if (CMDIChildWndEx* p = FindNextRegisteredWithTaskbarMDIChild(pGroup, nTab + 1))
//             return p;                      // nTab+1 only on the first group
//         pGroup = GetNextTabWnd(pGroup, TRUE);
//         if (!pGroup) break;
//         if (CMDIChildWndEx* p = FindNextRegisteredWithTaskbarMDIChild(pGroup, 0))
//             return p;
//     }
//     return NULL;
// STUB: both helpers below are blocked on m_lstTabbedGroups.
// Symbol: ?FindNextRegisteredWithTaskbarMDIChild@CMDIClientAreaWnd@@QEAAPEAVCMDIChildWndEx@@PEAV2@@Z
extern "C" void* MS_ABI impl__FindNextRegisteredWithTaskbarMDIChild_CMDIClientAreaWnd__QEAAPEAVCMDIChildWndEx__PEAV2__Z(void* pThis, void* pMDIChild) {
    (void)pThis;
    (void)pMDIChild;
    return nullptr;
}

// CMDIClientAreaWnd::FindNextRegisteredWithTaskbarMDIChild(CMFCTabCtrl*, int) —
// retail entry RVA 0x7ea30. Scans tabs of pTabGroup from max(nStartFrom, 0) to
// pTabGroup->vtable[+0x368]() (the tab count), fetching each tab's window with
// vtable[+0x370](i) and returning the first that IsKindOf
// RUNTIME_CLASS(CMDIChildWndEx) (RVA 0x2eb7d8), has a non-null m_hWnd, and
// whose sub-object at +0x720 also has a non-null m_hWnd. Note the +0x720 test
// is `lea 0x720(%rbx),%rax; test %rax,%rax` — always true — followed by the
// real check on (that+0x40).
// STUB: needs the CMFCTabCtrl and CMDIChildWndEx retail layouts.
// Symbol: ?FindNextRegisteredWithTaskbarMDIChild@CMDIClientAreaWnd@@QEAAPEAVCMDIChildWndEx@@PEAVCMFCTabCtrl@@H@Z
extern "C" void* MS_ABI impl__FindNextRegisteredWithTaskbarMDIChild_CMDIClientAreaWnd__QEAAPEAVCMDIChildWndEx__PEAVCMFCTabCtrl__H_Z(void* pThis, void* pTabGroup, int nStartFrom) {
    (void)pThis;
    (void)pTabGroup;
    (void)nStartFrom;
    return nullptr;
}

// CMDIClientAreaWnd::FindTabWndByChild — retail entry RVA 0x80c80.
// Returns NULL immediately when hWnd is NULL. Otherwise it walks
// m_lstTabbedGroups from +0x3ee0; for each node payload downcast to
// CMFCTabCtrl (IsKindOf, RVA 0x310b30) it calls the virtual at +0x448
// (GetTabFromHwnd-style lookup), stores the result through nTabIndex, and
// returns that group as soon as the index is >= 0. NULL if no group matches.
// STUB: needs m_lstTabbedGroups.
// Symbol: ?FindTabWndByChild@CMDIClientAreaWnd@@QEAAPEAVCMFCTabCtrl@@PEAUHWND__@@AEAH@Z
extern "C" void* MS_ABI impl__FindTabWndByChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__PEAUHWND____AEAH_Z(void* pThis, HWND hWnd, int* pnTabIndex) {
    (void)pThis;
    (void)hWnd;
    (void)pnTabIndex;
    return nullptr;
}

// CMDIClientAreaWnd::GetFirstTabWnd — retail entry RVA 0x80da0.
//     if (m_lstTabbedGroups.GetCount() == 0) return NULL;           // +0x3ef0
//     return DYNAMIC_DOWNCAST(CMFCTabCtrl, m_lstTabbedGroups.GetHead());
// (head at +0x3ee0, payload at node+0x10, IsKindOf against RVA 0x310b30).
// STUB: needs m_lstTabbedGroups.
// Symbol: ?GetFirstTabWnd@CMDIClientAreaWnd@@QEAAPEAVCMFCTabCtrl@@XZ
extern "C" void* MS_ABI impl__GetFirstTabWnd_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(void* pThis) {
    (void)pThis;
    return nullptr;
}

// CMDIClientAreaWnd::GetMDITabsContextMenuAllowedItems — retail entry RVA 0x80b70.
// Starts from FindActiveTabWndByActiveChild() (RVA 0x80d00); returns 0 when
// there is none. Otherwise, with nCount = pGroup->vtable[+0x368]():
//   flags = (nCount <= 1) ? 0
//         : (m_lstTabbedGroups.GetCount() <= 1) ? 3
//         : ((*(int*)(this+0x3fe8) != 1) + 1);
//   if (pGroup != m_lstTabbedGroups.GetHead()) flags |= 4;   // +0x3ee0
//   if (pGroup != m_lstTabbedGroups.GetTail()) flags |= 8;   // +0x3ee8
//   CWnd* pChild = pGroup->vtable[+0x430]();   // GetActiveWnd@CMFCBaseTabCtrl,
//                                              // read from the CMFCTabCtrl
//                                              // vftable at RVA 0x310520
//   downcast pChild to the class whose descriptor is at RVA 0x2eb7d8
//     (CMDIChildWndEx) and the frame from GetParentFrame() (RVA 0x28c680) to
//     the descriptor at RVA 0x2ec3c8 (CMDIFrameWndEx);
//   if (pChild && pChild->[+0x710] && pFrame && pFrame->[+0x6d8] == 0)
//       flags |= 0x10;
//   return flags;
// STUB: needs m_lstTabbedGroups, +0x3fe8 and the CMDIChildWndEx/CMDIFrameWndEx
// retail layouts.
// Symbol: ?GetMDITabsContextMenuAllowedItems@CMDIClientAreaWnd@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__GetMDITabsContextMenuAllowedItems_CMDIClientAreaWnd__QEAAKXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// CMDIClientAreaWnd::GetNextTabWnd — retail entry RVA 0x80e20.
// Finds the m_lstTabbedGroups node whose payload is pTabGroup, then returns the
// payload of node->pNext (node+0), downcast with IsKindOf(RUNTIME_CLASS
// (CMFCTabCtrl)); NULL when pTabGroup is not in the list, is last, or the
// downcast fails. The BOOL argument is not read on this path.
// STUB: needs m_lstTabbedGroups.
// Symbol: ?GetNextTabWnd@CMDIClientAreaWnd@@AEAAPEAVCMFCTabCtrl@@PEAV2@H@Z
extern "C" void* MS_ABI impl__GetNextTabWnd_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__PEAV2_H_Z(void* pThis, void* pTabGroup, int bNext) {
    (void)pThis;
    (void)pTabGroup;
    (void)bNext;
    return nullptr;
}

// CMDIClientAreaWnd::IsKeepClientEdge — retail entry RVA 0x7fc30.
//     BOOL bMaximized = TRUE;
//     HWND hActive = (HWND)::SendMessage(m_hWnd, WM_MDIGETACTIVE, 0, 0);
//     if (hActive) {
//         CWnd* p = CWnd::FromHandle(hActive);              // RVA 0x289180
//         if (p && ::IsWindow(p->m_hWnd))
//             bMaximized = (p->GetStyle() >> 19) & 1;       // WS_MAXIMIZE
//     }
//     return (*(int*)(this+0x3f48) == 0 && bMaximized) ? TRUE : FALSE;
// STUB: needs +0x3f48; and m_hWnd is never initialised for this class here.
// Symbol: ?IsKeepClientEdge@CMDIClientAreaWnd@@AEAAHXZ
extern "C" int MS_ABI impl__IsKeepClientEdge_CMDIClientAreaWnd__AEAAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// CMDIClientAreaWnd::IsMemberOfMDITabGroup — retail entry RVA 0x80df0.
//     if (*(int*)(this+0x3f48) == 0) return FALSE;
//     for (node = m_lstTabbedGroups.head; node; node = node->pNext)
//         if (node->data == pWnd) break;                    // data at node+0x10
//     return node != NULL;
// STUB: needs +0x3f48 and m_lstTabbedGroups.
// Symbol: ?IsMemberOfMDITabGroup@CMDIClientAreaWnd@@QEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__IsMemberOfMDITabGroup_CMDIClientAreaWnd__QEAAHPEAVCWnd___Z(void* pThis, void* pWnd) {
    (void)pThis;
    (void)pWnd;
    return 0;
}

// CMDIClientAreaWnd::LoadState — the Unicode export has no RVA in the map; the
// ANSI twin ?LoadState@CMDIClientAreaWnd@@QEAAHPEBDI@Z (entry RVA 0x82da0) was
// decoded instead. It builds the registry path with AFXGetRegPath (RVA
// 0xd2540) and CString::Format (RVA 0xda80) from lpszProfileName and nIndex,
// opens it through CSettingsStoreSP::Create (RVA 0x12b320), reads the blob,
// wraps it in a CMemFile/CArchive and drives Serialize.
// STUB: the value it deserialises into is this object's unmodeled state.
// Symbol: ?LoadState@CMDIClientAreaWnd@@QEAAHPEB_WI@Z
extern "C" int MS_ABI impl__LoadState_CMDIClientAreaWnd__QEAAHPEB_WI_Z(void* pThis, const wchar_t* lpszProfileName, unsigned int uiID) {
    (void)pThis;
    (void)lpszProfileName;
    (void)uiID;
    return 0;
}

// CMDIClientAreaWnd::MDITabMoveToNextGroup — retail entry RVA 0x81030.
//     CMFCTabCtrl* pGroup = FindActiveTabWndByActiveChild();     // RVA 0x80d00
//     if (!pGroup) return;
//     find pGroup's node in m_lstTabbedGroups (+0x3ee0);
//     node = bPrev ? node->pNext (node+0) : node->pPrev (node+8);
//         // a null node here reaches AfxThrowInvalidArgException (RVA 0x225b80)
//     CMFCTabCtrl* pTo = DYNAMIC_DOWNCAST(CMFCTabCtrl, node->data);
//     MoveWindowToTabGroup(pGroup, pTo, -1);                     // RVA 0x80510
// STUB: needs m_lstTabbedGroups.
// Symbol: ?MDITabMoveToNextGroup@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__MDITabMoveToNextGroup_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bPrev) {
    (void)pThis;
    (void)bPrev;
}

// CMDIClientAreaWnd::MDITabNewGroup — retail entry RVA 0x810d0.
//     int nOrientation = *(int*)(this+0x3fe8);
//     BOOL bVert = (nOrientation == 1) ? TRUE : (nOrientation == 2 ? FALSE : bVertical);
//     CMFCTabCtrl* pGroup = FindActiveTabWndByActiveChild();     // RVA 0x80d00
//     if (!pGroup) return;
//     RECT rc; ::GetWindowRect(pGroup->m_hWnd, &rc);
//     halve rc along x when bVert, along y otherwise;
//     CMFCTabCtrl* pNew = CreateNewTabGroup(pGroup, rc, bVert);  // RVA 0x806f0
//     MoveWindowToTabGroup(pGroup, pNew, -1);                    // RVA 0x80510
// STUB: needs +0x3fe8 and the tab-group machinery.
// Symbol: ?MDITabNewGroup@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__MDITabNewGroup_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bVertical) {
    (void)pThis;
    (void)bVertical;
}

// CMDIClientAreaWnd::MoveWindowToTabGroup — retail entry RVA 0x80510.
// nInsertAt is the index of the tab to move OUT OF pFrom, not a position in
// pTo: when it is -1 retail replaces it with pFrom->vtable[+0x428]() (the
// active tab) and gives up if that is still -1. It then takes that tab's window
// (vtable[+0x370]), requires it to be a CMDIChildWndEx (RVA 0x2eb7d8), reads
// its label through vtable[+0x3c8] (?GetFrameText@CMDIChildWndEx@@, returned by
// value as a CString), removes the tab from pFrom with vtable[+0x340]
// (?RemoveTab@CMFCBaseTabCtrl@@, bRecalcLayout TRUE), points the child's +0x1e8
// back-pointer at pTo and inserts it there with vtable[+0x328]
// (?AddTab@CMFCBaseTabCtrl@@) as (pWnd, label, nTabID = -1, TRUE) — that -1 is
// hard-coded, not the argument. If pFrom is then empty it is passed to
// RemoveTabGroup (RVA 0x808b0, which parks it on the free-list rather than
// deleting it), then UpdateMDITabbedGroups(TRUE) and pTo->RecalcLayout();
// otherwise AdjustMDIChildren(pFrom). Either way AdjustMDIChildren(pTo) runs,
// then SetActiveTab (RVA 0x7ead0) and, via the parent CMDIFrameWndEx, a
// ::SendMessage with the registered message id held at RVA 0x3ba650.
// STUB: not transcribed in full — the body is tab-control and
// m_lstTabbedGroups traffic on the unmodeled layout.
// Symbol: ?MoveWindowToTabGroup@CMDIClientAreaWnd@@QEAAHPEAVCMFCTabCtrl@@0H@Z
extern "C" int MS_ABI impl__MoveWindowToTabGroup_CMDIClientAreaWnd__QEAAHPEAVCMFCTabCtrl__0H_Z(void* pThis, void* pFrom, void* pTo, int nInsertAt) {
    (void)pThis;
    (void)pFrom;
    (void)pTo;
    (void)nInsertAt;
    return 0;
}

// CMDIClientAreaWnd::OnActiveTabChanged — retail entry RVA 0x7e8b0.
// Returns 0 unless: AfxGetModuleState()->[+0x8] (the module's app object,
// RVA 0x1345b0) is non-null; the global at RVA 0x3ba5d8 is set (afxGlobalData
// is lazily initialised through ?Initialize@AFX_GLOBAL_DATA@@ RVA 0x6a5c0
// guarded by the flag at RVA 0x3ba380); +0x3f48 is non-zero; the app's virtual
// at +0x210 returns TRUE; and lParam (the tab control) yields, through its
// virtual at +0x370(wParam), a window that IsKindOf RUNTIME_CLASS(CMDIChildWndEx)
// (RVA 0x2eb7d8) whose sub-object at +0x720 has a live m_hWnd. In that case it
// calls FindNextRegisteredWithTaskbarMDIChild (RVA 0x7e990) and
// ?SetTaskbarTabOrder@CMDIChildWndEx@@QEAAXPEAV1@@Z (RVA 0x847b0), returning 1.
// STUB: needs +0x3f48 and the CMDIChildWndEx layout.
// Symbol: ?OnActiveTabChanged@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnActiveTabChanged_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// CMDIClientAreaWnd::OnCancelTabMove — retail entry RVA 0x80b40.
//     DrawNewGroupRect(NULL, &m_rectNewGroup);   // +0x3f9c, RVA 0x80f30
//     ::SetRectEmpty(&m_rectNewGroup);
//     return 0;
// STUB: needs the RECT at +0x3f9c, which OpenMFC cannot address.
// Symbol: ?OnCancelTabMove@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnCancelTabMove_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// CMDIClientAreaWnd::OnDragComplete — retail entry RVA 0x7fd90.
// Returns 0 unless +0x3f48 is set, m_lstTabbedGroups is non-empty and wParam is
// non-null. Otherwise it locates the neighbouring group with GetNextTabWnd
// (RVA 0x80e20), sets the drag flag at +0x3fec to 1, converts the dragged rect
// to client coordinates (CWnd::ScreenToClient, RVA 0x2a11f0), resizes the two
// groups around the new split (CWnd::SetWindowPos, RVA 0x2a7970) using the
// orientation at +0x3fe8, and returns 1.
// STUB: needs +0x3f48/+0x3fe8/+0x3fec and m_lstTabbedGroups.
// Symbol: ?OnDragComplete@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnDragComplete_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// CMDIClientAreaWnd::OnEraseBkgnd — retail entry RVA 0x7d9d0.
// Returns TRUE (i.e. "erased") without painting when +0x3f48 is set and
// ::SendMessage(m_hWnd, WM_MDIGETACTIVE, 0, 0) names a visible window
// (::IsWindowVisible). Otherwise it asks the parent frame — GetParentFrame()
// (RVA 0x28c680) downcast to RUNTIME_CLASS(CMDIFrameWndEx) (RVA 0x2ec3c8) —
// through its virtual at +0x408, returning TRUE when that succeeds. Failing
// both, it fetches the client rect and lets the active visual manager --
// obtained from the lazy singleton getter at RVA 0x97f4, which on first call
// does operator new(0x108) + ??0CMFCVisualManager@@QEAA@H@Z (RVA 0x182640) --
// fill it through its virtual at +0x5b0, returning TRUE if it did and
// CWnd::Default() (RVA 0x289090) otherwise.
// STUB: needs +0x3f48 and the CMDIFrameWndEx layout.
// Symbol: ?OnEraseBkgnd@CMDIClientAreaWnd@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMDIClientAreaWnd__IEAAHPEAVCDC___Z(void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
    return 0;
}

// CMDIClientAreaWnd::OnGetDragBounds — retail entry RVA 0x7fcb0.
//     if (!*(int*)(this+0x3f48)) return 0;
//     if (m_lstTabbedGroups.GetCount() == 0) return 0;      // +0x3ef0
//     if (!wParam) return 0;
//     CMFCTabCtrl* pNext = GetNextTabWnd((CMFCTabCtrl*)wParam, FALSE);
//     if (!pNext) return 0;
//     RECT a, b; ::GetWindowRect(wParam->m_hWnd, &a); ::GetWindowRect(pNext->m_hWnd, &b);
//     ::UnionRect(&a, &a, &b);
//     shrink a by *(int*)(this+0x3f90) on left/right when +0x3fe8 == 1,
//     on top/bottom otherwise;
//     ::CopyRect((RECT*)lParam, &a);
//     return 1;
// STUB: needs +0x3f48/+0x3f90/+0x3fe8 and m_lstTabbedGroups.
// Symbol: ?OnGetDragBounds@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetDragBounds_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// CMDIClientAreaWnd::OnMDIDestroy — retail entry RVA 0x7dae0.
// Downcasts GetParentFrame() (RVA 0x28c680) to RUNTIME_CLASS(CMDIFrameWndEx)
// (RVA 0x2ec3c8) and pWnd to RUNTIME_CLASS(CMDIChildWndEx) (RVA 0x2eb7d8).
// When the frame's int at +0x214 is 0 and the global flag at RVA 0x3aaafc is 0
// it posts ::SendMessage(m_hWnd, 0x0B /* WM_SETREDRAW */, FALSE, 0). It then
// clears the child's tab-group back-pointer at +0x1e8, removes the child's tab
// from that group (virtuals at +0x308/+0x448/+0x340/+0x368) and, if the group
// became empty and more than one group remains, unlinks and destroys it.
// STUB: needs +0x3ee0/+0x3ef0 and the CMDIFrameWndEx/CMDIChildWndEx layouts.
// Symbol: ?OnMDIDestroy@CMDIClientAreaWnd@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnMDIDestroy_CMDIClientAreaWnd__IEAAXPEAVCWnd___Z(void* pThis, void* pWnd) {
    (void)pThis;
    (void)pWnd;
}

// CMDIClientAreaWnd::OnMDINext — retail entry RVA 0x83760.
// With neither flat tabs (+0x3e54) nor tabbed groups (+0x3f48) enabled it just
// tail-calls CWnd::Default() (RVA 0x289090). Otherwise it picks the group —
// FindActiveTabWnd() (RVA 0x80d40) for tabbed groups, else the embedded
// m_wndTab at +0xe8 — reads the active tab index through the group's virtual at
// +0x428, steps it by +1/-1 according to the BOOL, and either activates the
// neighbouring tab or, when the index runs off either end and several groups
// exist, walks m_lstTabbedGroups to the adjacent group and activates its first
// or last tab.
// STUB: needs +0x3e54/+0x3f48, m_lstTabbedGroups and the embedded tab control.
// Symbol: ?OnMDINext@CMDIClientAreaWnd@@IEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnMDINext_CMDIClientAreaWnd__IEAAXPEAVCWnd__H_Z(void* pThis, void* pWnd, int bPrev) {
    (void)pThis;
    (void)pWnd;
    (void)bPrev;
}

// CMDIClientAreaWnd::OnMDIRefreshMenu — retail entry RVA 0x7d930.
//     LRESULT lres = CWnd::Default();                       // RVA 0x289090
//     CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetParentFrame());
//     if (pFrame) {
//         if (pFrame->[+0x6a0]) pFrame->[+0x1e8] = pFrame->vtable[+0x398](pFrame->[+0x6a0]->[+0x1360]);
//         if (pFrame->[+0x710])                              // the ribbon bar
//             CMFCRibbonBar::SetActiveMDIChild(pFrame->[+0x710],
//                                              pFrame->MDIGetActive(NULL));
//                                              // RVA 0xe0e70 / 0x2a4a00
//     }
//     return (HMENU)lres;
// STUB: the useful half needs the CMDIFrameWndEx retail layout; calling
// CWnd::Default() alone would run on an uninitialised CWnd base.
// Symbol: ?OnMDIRefreshMenu@CMDIClientAreaWnd@@IEAAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__OnMDIRefreshMenu_CMDIClientAreaWnd__IEAAPEAUHMENU____XZ(void* pThis) {
    (void)pThis;
    return nullptr;
}

// CMDIClientAreaWnd::OnMDISetMenu — retail entry RVA 0x7d8a0.
//     HMENU hOk = NULL;
//     CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetParentFrame());
//     if (pFrame && ::IsWindow(pFrame->m_hWnd) && pFrame->vtable[+0x3d0](hmenuFrame))
//         hOk = hmenuFrame;
//     return (HMENU)DefWindowProc(WM_MDISETMENU /*0x230*/, (WPARAM)hOk,
//                                 (LPARAM)hmenuWindow);   // virtual at +0x248
// STUB: the guard is a CMDIFrameWndEx virtual this DLL does not model, and
// dropping it would change which menu handle retail forwards.
// Symbol: ?OnMDISetMenu@CMDIClientAreaWnd@@IEAAPEAUHMENU__@@PEAU2@0@Z
extern "C" HMENU MS_ABI impl__OnMDISetMenu_CMDIClientAreaWnd__IEAAPEAUHMENU____PEAU2_0_Z(void* pThis, HMENU hmenuFrame, HMENU hmenuWindow) {
    (void)pThis;
    (void)hmenuFrame;
    (void)hmenuWindow;
    return nullptr;
}

// CMDIClientAreaWnd::OnMoveTabComplete — retail entry RVA 0x80300.
// Copies m_rectNewGroup (+0x3f9c) to a local, calls DrawNewGroupRect(NULL,
// &m_rectNewGroup) (RVA 0x80f30) and ::SetRectEmpty's it. If wParam is non-null
// it converts the packed POINT in lParam to screen coordinates
// (::ClientToScreen on wParam->m_hWnd) and looks up the target group with
// TabWndFromPoint (RVA 0x80e80); when the saved rect was not empty
// (::IsRectEmpty) it splits off a new group, otherwise it moves the tab into
// the group under the cursor.
// STUB: needs +0x3f9c and m_lstTabbedGroups.
// Symbol: ?OnMoveTabComplete@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnMoveTabComplete_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// CMDIClientAreaWnd::OnStyleChanging — retail entry RVA 0x7fbf0.
//     if (nStyleType == GWL_EXSTYLE /* -20 */ && !IsKeepClientEdge())  // RVA 0x7fc30
//         lpss->styleNew = lpss->styleOld & ~WS_EX_CLIENTEDGE;   // btr $9
//     CWnd::Default();                                          // tail jmp RVA 0x289090
// Note the write is `styleNew = styleOld & ~WS_EX_CLIENTEDGE`, reading
// lpss->styleOld (offset 0) — it does not mask styleNew in place.
// STUB: IsKeepClientEdge reads +0x3f48, and the unconditional CWnd::Default()
// would run on an uninitialised CWnd base.
// Symbol: ?OnStyleChanging@CMDIClientAreaWnd@@IEAAXHPEAUtagSTYLESTRUCT@@@Z
extern "C" void MS_ABI impl__OnStyleChanging_CMDIClientAreaWnd__IEAAXHPEAUtagSTYLESTRUCT___Z(void* pThis, int nStyleType, STYLESTRUCT* lpss) {
    (void)pThis;
    (void)nStyleType;
    (void)lpss;
}

// CMDIClientAreaWnd::OnTabGroupMouseMove — retail entry RVA 0x7ff10.
// Resolves ::GetCapture() through CWnd::FromHandle (RVA 0x289180) and requires
// it to be a CMFCTabCtrl (RVA 0x310b30). If there is exactly one group
// (+0x3ef0 == 1) and that control reports a single tab (virtual +0x368) it
// returns 0. Otherwise it unpacks the POINT from lParam, asks the tab control's
// virtual at +0x2d8; on TRUE it sets the standard arrow cursor
// (::LoadCursor(NULL, IDC_ARROW /*0x7f00*/), ::SetCursor), and otherwise it
// computes and rubber-bands the prospective new group rectangle.
// STUB: needs m_lstTabbedGroups, +0x3f9c and the CMFCTabCtrl layout.
// Symbol: ?OnTabGroupMouseMove@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnTabGroupMouseMove_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// CMDIClientAreaWnd::PreSubclassWindow — retail entry RVA 0x7ecb0 (virtual).
// The whole body is a tail call through this object's own vtable:
//     mov (%rcx),%rax ; lea 0xe8(%rcx),%rdx ; mov 0x2d8(%rax),%rax ; jmp CFG
// i.e. `this->vtable[+0x2d8](&m_wndTab)` — the virtual that CreateTabGroup
// occupies for this class, invoked with the embedded CMFCTabCtrl at +0xe8.
// STUB: OpenMFC has no vtable for CMDIClientAreaWnd and no sub-object at +0xe8.
// Symbol: ?PreSubclassWindow@CMDIClientAreaWnd@@MEAAXXZ
extern "C" void MS_ABI impl__PreSubclassWindow_CMDIClientAreaWnd__MEAAXXZ(void* pThis) {
    (void)pThis;
}

// CMDIClientAreaWnd::RemoveTabGroup — retail entry RVA 0x808b0. Finds
// pTabGroup's node in m_lstTabbedGroups (+0x3ee0), remembers the neighbouring
// group, unlinks the node with ?RemoveAt@CPtrList@@ (RVA 0x2306f0), hides
// pTabGroup (CWnd::ShowWindow, SW_HIDE), resizes the neighbour over the freed
// space and calls AdjustMDIChildren (RVA 0x7e5f0); when only one group is left
// it resets the orientation at +0x3fe8 to 0; it looks pTabGroup up in the map
// at +0x3f10, deletes the mapped object through its vtable slot 1 and unlinks
// the association (RemoveAll'ing the map when it empties); and finally
// ?AddTail@CPtrList@@ (RVA 0x230490) parks pTabGroup on the free-list at
// +0x3f58 and hides it a second time.
// The group is NEVER deleted here (the only `delete` of a group is in the
// destructor, which drains that free-list), and the `int` second argument is
// dead: no instruction in this body reads %r8d.
// STUB: not transcribed in full — needs both lists and the map at +0x3f10.
// Symbol: ?RemoveTabGroup@CMDIClientAreaWnd@@QEAAXPEAVCMFCTabCtrl@@H@Z
extern "C" void MS_ABI impl__RemoveTabGroup_CMDIClientAreaWnd__QEAAXPEAVCMFCTabCtrl__H_Z(void* pThis, void* pTabGroup, int bDestroy) {
    (void)pThis;
    (void)pTabGroup;
    (void)bDestroy;
}

// CMDIClientAreaWnd::SaveState — the Unicode export has no RVA in the map; the
// ANSI twin ?SaveState@CMDIClientAreaWnd@@QEAAHPEBDI@Z (entry RVA 0x82b70) was
// decoded instead. It formats the registry path with AFXGetRegPath (RVA
// 0xd2540) / CString::Format (RVA 0xda80), serialises into a CMemFile through a
// storing CArchive (RVA 0x229510 / 0x1cf500) by calling this->vtable[+0x10]
// (Serialize), then writes the resulting blob to the settings store.
// STUB: the state it would serialise is this object's unmodeled state.
// Symbol: ?SaveState@CMDIClientAreaWnd@@QEAAHPEB_WI@Z
extern "C" int MS_ABI impl__SaveState_CMDIClientAreaWnd__QEAAHPEB_WI_Z(void* pThis, const wchar_t* lpszProfileName, unsigned int uiID) {
    (void)pThis;
    (void)lpszProfileName;
    (void)uiID;
    return 0;
}

// CMDIClientAreaWnd::Serialize — retail entry RVA 0x83050 (virtual).
//     m_mdiTabParams.Serialize(ar);                  // +0x3ea8, RVA 0x7ca40
//     if (ar.IsStoring()) {                          // (*(BYTE*)(ar+0x20) & 1) == 0
//         ar << (int)this->[+0x3e54] << (int)this->[+0x3f48]
//            << (int)this->[+0x3e50] << (int)this->[+0x3fe8]
//            << (int)this->[+0x3f90] << (int)this->[+0x3f94];
//         if (this->[+0x3e54])                       // flat tabs
//             SerializeTabGroup(ar, &m_wndTab, FALSE);        // RVA 0x81320
//         else if (this->[+0x3f48]) {                // tabbed groups
//             ar << (int)m_lstTabbedGroups.GetCount();
//             for (each group) SerializeTabGroup(ar, pGroup, FALSE);
//         }
//         else SerializeOpenChildren(ar);            // RVA 0x821e0, the
//                                                    // branch at 0x83246
//     } else {                                       // loading, from 0x83256
//         CloseAllWindows(NULL);                     // RVA 0x811b0
//         RemoveAll() on the CStringList at +0x3fb0;  // RVA 0x230920
//         this->[+0x3f50] = 1;                       // re-entrancy guard
//         ... then the mirrored reads of the fields listed above ...
//     }
//   (the `ar << int` sequences are the inlined CArchive fast path: bounds-check
//    m_lpBufCur (+0x38) against m_lpBufMax (+0x40), CArchive::Flush (RVA
//    0x1cfb90) when it would overflow, store, advance by 4.)
// STUB: every field it reads or writes is unmodeled here.
// Symbol: ?Serialize@CMDIClientAreaWnd@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMDIClientAreaWnd__UEAAXAEAVCArchive___Z(void* pThis, void* pArchive) {
    (void)pThis;
    (void)pArchive;
}

// Not on this pass's assignment list; signature corrected only (the generated
// stub omitted the `this` parameter that MSVC passes in %rcx).
// Symbol: ?SerializeOpenChildren@CMDIClientAreaWnd@@AEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__SerializeOpenChildren_CMDIClientAreaWnd__AEAAXAEAVCArchive___Z(void* pThis, void* pArchive) {
    (void)pThis;
    (void)pArchive;
}

// Not on this pass's assignment list; signature corrected only. Retail entry
// RVA 0x81320; argument order verified from the call sites inside Serialize
// (this, CArchive&, CMFCTabCtrl*, BOOL).
// Symbol: ?SerializeTabGroup@CMDIClientAreaWnd@@AEAAXAEAVCArchive@@PEAVCMFCTabCtrl@@H@Z
extern "C" void MS_ABI impl__SerializeTabGroup_CMDIClientAreaWnd__AEAAXAEAVCArchive__PEAVCMFCTabCtrl__H_Z(void* pThis, void* pArchive, void* pTabGroup, int bSaveOnly) {
    (void)pThis;
    (void)pArchive;
    (void)pTabGroup;
    (void)bSaveOnly;
}

// Not on this pass's assignment list; signature corrected only.
// Symbol: ?SetActiveTab@CMDIClientAreaWnd@@QEAAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__SetActiveTab_CMDIClientAreaWnd__QEAAXPEAUHWND_____Z(void* pThis, HWND hWnd) {
    (void)pThis;
    (void)hWnd;
}

// Not on this pass's assignment list; signature corrected only. CPoint is an
// 8-byte trivially-copyable POD, so MSVC passes it by value in a register.
// Symbol: ?TabWndFromPoint@CMDIClientAreaWnd@@AEAAPEAVCMFCTabCtrl@@VCPoint@@@Z
extern "C" void* MS_ABI impl__TabWndFromPoint_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__VCPoint___Z(void* pThis, POINT pt) {
    (void)pThis;
    (void)pt;
    return nullptr;
}

// Not on this pass's assignment list; signature corrected only.
// Symbol: ?UpdateMDITabbedGroups@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__UpdateMDITabbedGroups_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bForceUpdate) {
    (void)pThis;
    (void)bForceUpdate;
}
