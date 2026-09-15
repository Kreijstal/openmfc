// CMultiPaneFrameWnd — OpenMFC implementation.
// Sources: cbarcore.cpp
//
// Every body below that is marked "decoded from retail" was transcribed from
// the retail export disassembly (disas.py against mfc140.dll, the ANSI twin of
// the mfc140u.dll OpenMFC reimplements).  Function bodies are byte-identical
// between the two images; only the export RVAs differ, so every RVA quoted in
// this file is an mfc140.dll RVA unless it is explicitly labelled (mfc140u).
// Every RVA quoted is a FUNCTION ENTRY unless the text says otherwise.
//
// ---------------------------------------------------------------------------
// Retail CMultiPaneFrameWnd instance layout
//
// CMultiPaneFrameWnd has no declaration in include/openmfc/, so this
// translation unit models the retail object directly, the way
// docking/CPaneContainerManager.cpp does for its class.  The retail
// constructor ??0CMultiPaneFrameWnd@@QEAA@XZ (entry RVA 0x92450) is:
//
//     CPaneFrameWnd::CPaneFrameWnd();                       // call 0xad6d0
//     vfptr = &CMultiPaneFrameWnd::vftable;                 // .rdata 0x2ee738
//     CPaneContainerManager::CPaneContainerManager(          // call 0xa80b0
//         (CPaneContainerManager*)((char*)this + 0x258));   //   m_barContainerManager
//     m_hWndLastFocused /*+0x2f8*/ = NULL;
//     m_bHostsToolbar /*+0x104*/ = FALSE;   // CPaneFrameWnd's ctor stores 1 there
//                                           // (0xad80d); not modelled -- see below
//
// so the object is
//
//   +0x000  CPaneFrameWnd sub-object (retail sizeof 0x258 -- docking/
//           RuntimeClasses.cpp records 600 for CPaneFrameWnd)
//   +0x258  CPaneContainerManager m_barContainerManager   (sizeof 0xa0, see
//           the layout comment at the top of docking/CPaneContainerManager.cpp)
//   +0x2f8  HWND m_hWndLastFocused
//   sizeof == 0x300
//
// sizeof is not a guess: ?CreateObject@CMultiPaneFrameWnd@@SAPEAVCObject@@XZ
// (entry RVA 0x923d0) does `mov $0x300,%ecx; call operator new` before running
// the constructor, the deleting destructor at 0x924a0 frees 0x300 bytes, and
// the CRuntimeClass at .data 0x3aa2f8 ("CMultiPaneFrameWnd") records
// m_nObjectSize 0x300 (docking/RuntimeClasses.cpp records the same 768).
//
// The two derived-class fields are reached at their retail offsets.  This is
// memory-safe for every CMultiPaneFrameWnd that can exist in this build:
// nothing in the DLL allocates one (CPane::CreateDefaultMiniframe is a stub and
// CreateObject below allocates exactly 0x300 bytes), so any instance was either
// allocated by CreateObject or by a client compiled against the shipping MFC
// headers, i.e. sizeof 0x300 either way.
//
// Retail CPaneFrameWnd member names used in this file follow the shipping
// atlmfc afxpaneframewnd.h member order laid over the constructor's stores
// (0xad6d0): m_bHostsToolbar 0x104, m_bDelayShow 0x108, m_nCaptionHeight 0x114,
// m_nRestoredEmbeddedBarID 0x128, m_hParentWnd 0x130, m_hEmbeddedBar 0x148,
// m_rectRecentFloatingRect 0x150, m_dragFrameImpl 0x1c8 (its m_bDockToTab at
// +0x30 == 0x1f8 and m_pFinalTargetBar at +0x40 == 0x208), m_pDockManager 0x248.
//
// The CPaneFrameWnd sub-object is NOT retail-shaped: as the note at the top of
// docking/CPaneFrameWnd.cpp says, OpenMFC's CPaneFrameWnd is a CMiniFrameWnd
// subclass with a 128-byte padding blob (sizeof 632 == 0x278 under the phase4
// mingw flags), and that file keeps retail's CPaneFrameWnd members in side
// tables (g_framePanes for the pane list / 0x150 rectangle, the
// CPaneFrameWndSupport.h PaneFrameState for 0x140/timers).  Bodies here that
// need a CPaneFrameWnd member therefore go through that file's exports, and
// each such spot says which retail offset it stands in for.  The 632-byte
// OpenMFC base overlaps the first 0x20 bytes of m_barContainerManager; that is
// harmless because the only code that touches `_paneframewnd_padding` is the
// CPaneFrameWnd constructor's memset (docking/CPaneFrameWnd.cpp:775), which runs
// BEFORE the manager's constructor in the order below, and the static_assert
// below keeps the base inside the 0x300-byte allocation.
//
// The embedded manager has no retail vtable here (OpenMFC's
// ??0CPaneContainerManager@@ zero-fills the object and sets no vfptr), so every
// `m_barContainerManager.<virtual>()` in the retail bodies is dispatched
// statically to the CPaneContainerManager export.  Likewise this class's own
// virtuals (OnSetRollUpTimer, OnPaneRecalcLayout, CheckGripperVisibility, ...)
// are called directly.  Both are deviations: an override in a further-derived
// class is not reached.  Each body notes the slots it resolved this way.
//
// LIST REPRESENTATION.  Retail reaches m_barContainerManager.m_lstControlBars
// two ways: (1) through CPaneContainerManager::AddPanesToList(&lst, NULL)
// (0xaa960), which copies the entries that are CWnds carrying WS_VISIBLE into
// a caller-owned CObList, and (2) by walking the raw CObList nodes (head
// pointer at +0x268 == 0x258 + CObList m_pNodeHead +0x10; `node->pNext` at
// +0, `node->data` at +0x10).  OpenMFC's CPaneContainerManager keeps that list
// in a side table and never populates the raw node words (see the LIST
// REPRESENTATION note in docking/CPaneContainerManager.cpp), and that side
// table is reachable from another translation unit only through the
// CPaneContainerManager exports.  So every walk below goes through (1): a
// CObList built on the stack with the exported CObList constructor, filled by
// the AddPanesToList export, and read through the exported FindIndex thunk plus
// a node view (the pattern visualmanager/CMFCVisualManager.cpp and
// docking/CDockSite.cpp use).  For the four bodies that retail walks the raw
// list (AdjustPaneFrames, CanBeDockedToPane, LoadState, SaveState) that is a
// DEVIATION -- panes without WS_VISIBLE are not visited -- and each of those
// bodies says so.  Head-to-tail order is preserved either way.  The
// `m_lstControlBars.m_nCount == 0` tests are the exported IsEmpty (0xaa8e0 is
// exactly that compare).
//
// ---------------------------------------------------------------------------
// Retail CMultiPaneFrameWnd vftable (.rdata 0x2ee738 in mfc140.dll, the address
// the constructor stores at +0).  Slots the bodies below dispatch through:
//   0x0d0 CWnd::DestroyWindow              0x340 OnDockToRecentPos
//   0x320 CheckGripperVisibility           0x348 CPaneFrameWnd::OnBeforeDock
//   0x350 <unexported, 0x92360>            0x368 <unexported, 0x92390>
//         `return m_barContainerManager.         `return m_barContainerManager.
//          m_lstControlBars.m_nCount;`             GetFirstVisiblePane();`
//   0x3a8 OnPaneRecalcLayout               0x3e0 OnShowPane
//   0x3f8 OnSetRollUpTimer                 0x400 OnKillRollUpTimer
//   0x450 CPaneFrameWnd::OnCloseMiniFrame  0x488 CPaneFrameWnd::CalcMinSize
// CPaneContainerManager vftable (.rdata 0x2f41a8):
//   0x10 Serialize          0x28 Create             0x48 AddPane
//   0x50 AddPaneToRecentPaneContainer               0x68 GetWindowRect
//   0x70 GetAvailableSpace  0x78 RemovePaneFromPaneContainer
//   0x80 ResizePaneContainers(CRect, void*&)        0x90 ReplacePane
//   0xa0 OnShowPane         0xa8 IsRootPaneContainerVisible
//   0xb0 GetMinSize         0xb8 GetVisiblePaneCount 0xc0 EnableGrippers
//   0xc8 HideAll            0xe0 PaneFromPoint       0xf0 GetFirstPane
//   0x100 GetFirstVisiblePane                        0x128 FindPaneContainer
// CDockablePane vftable (.rdata 0x2e3068, the address ??0CDockablePane@@ stores
// at +0), slots used below:
//   0x318 <unexported, 0x12d90> CDockablePane::CanAcceptPane
//   0x398 <unexported, 0x8850>  `return m_dwControlBarStyle /*+0x108*/;`
//   0x3c0 <folded onto 0x3ae0>  `return 1;`
//   0x3f0 EnableGripper         0x400 CPane::DockPane
//   0x468 CPane::LoadState      0x470 CPane::SaveState
//   0x480 CBasePane::SetWindowPos                    0x5d0 ConvertToTabbedDocument
//   0x648 StoreRecentDockSiteInfo
// CRecentDockSiteInfo vftable (.rdata 0x2fc048): 0x28 StoreDockInfo.
// ---------------------------------------------------------------------------

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <vector>

// ---------------------------------------------------------------------------
// Sibling exports.  Every declaration was located with grep before being
// written; the file:line of the definition is given.  Where the definition is
// still a generated placeholder (wrong parameter list, or an empty body) that
// is said so -- the declaration here is the one derived from the mangled name.
// ---------------------------------------------------------------------------
// CPaneFrameWnd (docking/CPaneFrameWnd.cpp unless noted; ctor/dtor in
// docking/Thunks.cpp:1138 / :1163)
extern "C" void* MS_ABI impl___0CPaneFrameWnd__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CPaneFrameWnd__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(void* pThis, void* pPane);
extern "C" void  MS_ABI impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int bDestroy, int bNoDelayedDestroy);
extern "C" void  MS_ABI impl__ReplacePane_CPaneFrameWnd__UEAAXPEAVCBasePane__0_Z(void* pThis, void* pOldBar, void* pNewBar);
extern "C" int   MS_ABI impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(void* pPane, int bAdd);
extern "C" int   MS_ABI impl__OnCreate_CPaneFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* lpCreateStruct);
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CPaneFrameWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam);
extern "C" void  MS_ABI impl__OnCancelMode_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" int   MS_ABI impl__OnCloseMiniFrame_CPaneFrameWnd__MEAAHXZ(void* pThis);
extern "C" void  MS_ABI impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__SetRollUpTimer_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__KillRollupTimer_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__CalcMinSize_CPaneFrameWnd__MEAAXAEAVCSize__PEAUtagMINMAXINFO___Z(void* pThis, CSize* pSize, MINMAXINFO* pMMI);
extern "C" void  MS_ABI impl__SaveRecentFloatingState_CPaneFrameWnd__MEAAXXZ(void* pThis);
// docking/CPaneFrameWnd.cpp:2099 -- a documented stub (retail 0xb2d20 not reproduced).
extern "C" void  MS_ABI impl__Serialize_CPaneFrameWnd__UEAAXAEAVCArchive___Z(void* pThis, void* pArchive);

// CPaneContainerManager (docking/CPaneContainerManager.cpp)
extern "C" void* MS_ABI impl___0CPaneContainerManager__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___1CPaneContainerManager__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__AddPane_CPaneContainerManager__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane);
extern "C" int   MS_ABI impl__Create_CPaneContainerManager__UEAAHPEAVCWnd__PEAVCPaneDivider__PEAUCRuntimeClass___Z(
                     void* pThis, void* pParentWnd, void* pDefaultSlider, CRuntimeClass* pContainerRTC);
extern "C" void* MS_ABI impl__GetFirstPane_CPaneContainerManager__UEBAPEAVCBasePane__XZ(void* pThis);
extern "C" void* MS_ABI impl__GetFirstVisiblePane_CPaneContainerManager__UEBAPEAVCWnd__XZ(void* pThis);
extern "C" int   MS_ABI impl__GetVisiblePaneCount_CPaneContainerManager__UEBAHXZ(const void* pThis);
extern "C" void  MS_ABI impl__EnableGrippers_CPaneContainerManager__UEAAXH_Z(void* pThis, int bEnable);
extern "C" void  MS_ABI impl__HideAll_CPaneContainerManager__UEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__GetMinSize_CPaneContainerManager__UEAAXAEAVCSize___Z(void* pThis, CSize& size);
extern "C" int   MS_ABI impl__OnShowPane_CPaneContainerManager__UEAAHPEAVCDockablePane__H_Z(void* pThis, void* pBar, int bShow);
extern "C" int   MS_ABI impl__IsRootPaneContainerVisible_CPaneContainerManager__UEBAHXZ(const void* pThis);
extern "C" void* MS_ABI impl__PaneFromPoint_CPaneContainerManager__UEAAPEAVCDockablePane__VCPoint__HHAEAH1_Z(
                     void* pThis, void* point, int nSensitivity, int bExactBar, int* pbIsTabArea, int* pbCaption);
extern "C" void* MS_ABI impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
                     void* pThis, void* pPane, int* pbLeftPane);
extern "C" int   MS_ABI impl__ReplacePane_CPaneContainerManager__UEAAHPEAVCDockablePane__0_Z(void* pThis, void* pBarOld, void* pBarNew);
extern "C" void  MS_ABI impl__ResizePaneContainers_CPaneContainerManager__UEAAXVCRect__AEAPEAX_Z(void* pThis, const RECT* pRect, void** phdwp);
extern "C" void  MS_ABI impl__GetAvailableSpace_CPaneContainerManager__UEBAXAEAVCRect___Z(const void* pThis, CRect& rect);
// docking/CPaneContainerManager.cpp:1942 -- generated placeholder `(void** p0)`
// with an empty body; the list is therefore NOT shrunk by this call (see
// RemovePane below).
extern "C" int   MS_ABI impl__RemovePaneFromPaneContainer_CPaneContainerManager__UEAAHPEAVCDockablePane___Z(void* pThis, void* pPane);
// docking/CPaneContainerManager.cpp:2086 -- generated placeholder `(void** p0) {}`.
extern "C" void  MS_ABI impl__Serialize_CPaneContainerManager__UEAAXAEAVCArchive___Z(void* pThis, void* pArchive);

// CPaneContainer (docking/CPaneContainer.cpp)
extern "C" void  MS_ABI impl__ReleaseEmptyPaneContainer_CPaneContainer__QEAAXXZ(void* pThis);
extern "C" int   MS_ABI impl__GetNodeCount_CPaneContainer__QEBAHXZ(const void* pThis);
extern "C" int   MS_ABI impl__IsEmpty_CPaneContainer__QEBAHXZ(const void* pThis);
extern "C" void  MS_ABI impl__RemoveNonValidPanes_CPaneContainer__QEAAXXZ(void* pThis);

// Panes (docking/CBasePane.cpp, CPane.cpp, CDockablePane.cpp, Thunks.cpp:1208)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWnd__SAPEAUCRuntimeClass__XZ();
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int   MS_ABI impl__IsDocked_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" int   MS_ABI impl__IsInFloatingMultiPaneFrameWnd_CDockablePane__UEBAHXZ(const CDockablePane* pThis);
extern "C" void* MS_ABI impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
                     CBasePane* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags, void* pExtra);
extern "C" int   MS_ABI impl__DockPane_CPane__UEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
                     CPane* pThis, CBasePane* pDockBar, const RECT* lpRect, int dockMethod);
extern "C" int   MS_ABI impl__LoadState_CPane__UEAAHPEB_WHI_Z(CPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
extern "C" int   MS_ABI impl__SaveState_CPane__UEAAHPEB_WHI_Z(CPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
extern "C" void  MS_ABI impl__EnableGripper_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bEnable);
extern "C" void  MS_ABI impl__ConvertToTabbedDocument_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bActiveTabOnly);
extern "C" void  MS_ABI impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ(CDockablePane* pThis);
// docking/CRecentDockSiteInfo.cpp:64 -- generated placeholder `(void** p0,
// void** p1) {}` (no `this`); docking/CPaneContainer.cpp:184 already declares
// and calls it with this three-parameter shape.
extern "C" void  MS_ABI impl__StoreDockInfo_CRecentDockSiteInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane___Z(
                     void* pThis, void* pContainer, void* pPane);

// CWnd / CObject (core/window/CWnd.cpp, core/window/Thunks.cpp,
// core/runtime/CObject.cpp)
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" CWnd*  MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int    MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" int   MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int   MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
                     CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" int   MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);

// CGlobalUtils (core/runtime/CGlobalUtils.cpp).  GetDockingManager (:124) has a
// real body that reads no members of `this`; ForceAdjustLayout (:166) is a
// generated placeholder `(void** p0, int, int) {}`.
extern "C" CDockingManager* MS_ABI impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(CGlobalUtils* pThis, CWnd* pWnd);
extern "C" void  MS_ABI impl__ForceAdjustLayout_CGlobalUtils__QEAAXPEAVCDockingManager__HH_Z(CGlobalUtils* pThis, CDockingManager* pDockManager, int bForce, int bForceInvalidate);
// ?afxGlobalUtils@@3VCGlobalUtils@@A -- the storage other docking files pass
// as the CGlobalUtils `this` (retail .data 0x3aacf8).
extern "C" unsigned char impl__afxGlobalUtils__3VCGlobalUtils__A[];

// Globals: core/runtime/Globals.cpp:89 and docking/StaticData.cpp:76.
extern "C" unsigned int impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA;
extern "C" std::int32_t impl__m_bHandleMinSize_CPane__2HA;

// CObList (core/collections/CObList.cpp) -- the local-list plumbing described
// under LIST REPRESENTATION above.
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(CObList* pThis, long long nBlockSize);
extern "C" void  MS_ABI impl___1CObList__UEAA_XZ(CObList* pThis);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
extern "C" void  MS_ABI impl__AddPanesToList_CPaneContainerManager__QEAAXPEAVCObList__0_Z(void* pThis, void* plstControlBars, void* plstSliders);
extern "C" int   MS_ABI impl__IsEmpty_CPaneContainerManager__QEBAHXZ(const void* pThis);
// ??2@YAPEAX_K@Z (detail/MemcoreSupport.cpp) -- the operator new retail's
// CreateObject calls.
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
// docking/CPaneFrameWnd.cpp:204 -- FirstPane(g_framePanes[this]), that file's
// stand-in for retail 0x148 (see GetPane below).
extern "C" void* MS_ABI impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis);

// Exports used by SetDockState only.  Each definition was located with grep;
// where it is still a generated placeholder that is said here.
// docking/CDockingManager.cpp:311 -- a real body over that file's side table.
extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(CDockingManager* pThis, unsigned int nID, int bSearchMiniFrames);
// docking/CPaneContainer.cpp:1220 -- pure address arithmetic on the retail-shaped
// container: this+0x88 / this+0xc0 / NULL (exactly what retail inlines).
extern "C" void* MS_ABI impl__GetAssociatedSiblingPaneIDs_CPaneContainer__QEAAPEAV__CList_II__PEAVCDockablePane___Z(void* pThis, void* pPane);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CWnd*  MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int    MS_ABI impl__IsTabbed_CPane__UEBAHXZ(const CPane* pThis);
// docking/CPane.cpp:732 -- a documented stub (returns FALSE).
extern "C" int    MS_ABI impl__FloatPane_CPane__UEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(CPane* pThis, const RECT* pRectFloat, int dockMethod, bool bShow);
extern "C" void*  MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(const CBasePane* pThis, int bNoAssert);
// docking/CDockablePane.cpp:662 -- a documented stub (returns NULL).
extern "C" void*  MS_ABI impl__AttachToTabWnd_CDockablePane__UEAAPEAV1_PEAV1_W4AFX_DOCK_METHOD__HPEAPEAV1__Z(
                      CDockablePane* pThis, CDockablePane* pTabControlBarAttachTo, int dockMethod, int bSetActive, CDockablePane** ppTabbedControlBar);
extern "C" CMFCAutoHideBar* MS_ABI impl__SetAutoHideMode_CDockablePane__UEAAPEAVCMFCAutoHideBar__HKPEAV2_H_Z(
                      CDockablePane* pThis, int bAutoHideMode, unsigned long dwAlignment, void* pAutoHideBar, int bSetFocus);
extern "C" void   MS_ABI impl__ShowPane_CDockablePane__UEAAXHHH_Z(CDockablePane* pThis, int bShow, int bDelay, int bActivate);
// docking/CBaseTabbedPane.cpp:113 / :23 / :110 -- generated placeholders
// `(void** p0) { return 0; }`, `(int p0) {}` and `() {}`; none has `this`.
// Declared with the retail-correct lists; the placeholders read no argument.
extern "C" int    MS_ABI impl__RemovePane_CBaseTabbedPane__UEAAHPEAVCWnd___Z(void* pThis, void* pWnd);
extern "C" void   MS_ABI impl__ApplyRestoredTabInfo_CBaseTabbedPane__UEAAXH_Z(void* pThis, int bApply);
extern "C" void   MS_ABI impl__RecalcLayout_CBaseTabbedPane__UEAAXXZ(void* pThis);
extern "C" void   MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CWnd* pThis, CString* rString);
extern "C" void   MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd* pThis, const wchar_t* lpszString);

// Sibling exports defined later in THIS translation unit.
extern "C" void  MS_ABI impl__RemovePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int bDestroy, int bNoDelayedDestroy);
extern "C" void  MS_ABI impl__OnSetRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnKillRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnPaneRecalcLayout_CMultiPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__CheckGripperVisibility_CMultiPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnDockToRecentPos_CMultiPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl___0CMultiPaneFrameWnd__QEAA_XZ(void* pThis);

namespace {

constexpr size_t kOffContainerManager     = 0x258;   // CPaneContainerManager m_barContainerManager
constexpr size_t kOffLastFocused          = 0x2f8;   // HWND m_hWndLastFocused
constexpr size_t kSizeCMultiPaneFrameWnd  = 0x300;
constexpr size_t kSizeCPaneContainerManager = 0xa0;  // pinned in docking/CPaneContainerManager.cpp
// Manager-relative offset of CPaneContainerManager::m_pRootContainer, the same
// 0x78 docking/CPaneContainerManager.cpp pins; the retail bodies below read it
// as `0x2d0(%rcx)` == 0x258 + 0x78.
constexpr size_t kOffMgrRootContainer     = 0x78;
static_assert(kOffContainerManager + kSizeCPaneContainerManager == kOffLastFocused,
              "m_hWndLastFocused follows the embedded manager");
static_assert(kOffLastFocused + sizeof(HWND) == kSizeCMultiPaneFrameWnd, "CMultiPaneFrameWnd tail");
static_assert(sizeof(CPaneFrameWnd) <= kSizeCMultiPaneFrameWnd,
              "OpenMFC's CPaneFrameWnd must fit inside the retail-sized allocation");
// CPane::m_recentDockInfo (+0x2b8) is a CRecentDockSiteInfo whose retail layout
// is the CObject vfptr at +0 followed by CRect m_rectRecentFloatingRect at +8
// (its first data member in the shipping afxrecentdocksiteinfo.h; retail pane
// offset 0x2c0, the store SaveRecentFloatingState makes).  OpenMFC models
// CRecentDockSiteInfo as an opaque 288-byte block, so the rectangle is reached
// by offset.
static_assert(offsetof(CPane, m_recentDockInfo) == 0x2b8, "CRecentDockSiteInfo moved");
constexpr size_t kOffRecentFloatingRectInDockInfo = 0x8;
static_assert(kOffRecentFloatingRectInDockInfo + sizeof(RECT) <= sizeof(CRecentDockSiteInfo),
              "m_rectRecentFloatingRect must lie inside the opaque block");
// CBasePane::m_dwControlBarStyle, the member the unexported CBasePane virtual
// at slot 0x398 (0x8850: `mov 0x108(%rcx),%eax; ret`) returns.
static_assert(offsetof(CBasePane, m_dwControlBarStyle) == 0x108, "m_dwControlBarStyle moved");
// AFX_CBRS_AUTO_ROLLUP, the bit the roll-up timer bodies test (`test $0x10,%al`
// at 0x92cfd / 0x92dbd).  Spelled as in the shipping atlmfc afxbasepane.h.
constexpr unsigned long kCbrsAutoRollUp = 0x10UL;
// SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED,
// the immediate 0x37 several retail bodies pass to SetWindowPos.
constexpr unsigned int kSwpFrameChanged = SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED;
static_assert(kSwpFrameChanged == 0x37, "SWP immediate");
// SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE, the 0x13 OnSetFocus passes.
constexpr unsigned int kSwpTop = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
static_assert(kSwpTop == 0x13, "SWP immediate");
// SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE, the 0x16 OnPaneRecalcLayout passes.
constexpr unsigned int kSwpResize = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE;
static_assert(kSwpResize == 0x16, "SWP immediate");
// AFX_DOCK_METHOD::DM_DBL_CLICK, the immediate 2 passed to CPane::DockPane.
constexpr int kDockMethodDblClick = 2;

inline void* Mgr(void* pThis) {
    return static_cast<char*>(pThis) + kOffContainerManager;
}
inline HWND& LastFocused(void* pThis) {
    return *reinterpret_cast<HWND*>(static_cast<char*>(pThis) + kOffLastFocused);
}
inline void* MgrRootContainer(void* pThis) {
    return *reinterpret_cast<void**>(static_cast<char*>(Mgr(pThis)) + kOffMgrRootContainer);
}
inline CWnd* AsWnd(void* p) { return static_cast<CWnd*>(p); }
inline HWND FrameHwnd(void* pThis) { return pThis ? AsWnd(pThis)->m_hWnd : nullptr; }

// DYNAMIC_DOWNCAST(CDockablePane, p) / DYNAMIC_DOWNCAST(CBasePane, p): the
// retail bodies do `p && p->IsKindOf(RUNTIME_CLASS(X)) ? p : NULL` with the
// descriptors at 0x3aa178 ("CDockablePane") and 0x2da490 ("CBasePane").
inline void* DowncastTo(void* p, CRuntimeClass* pClass) {
    if (p == nullptr || pClass == nullptr) return nullptr;
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(AsWnd(p)), pClass) ? p : nullptr;
}
inline CDockablePane* AsDockable(void* p) {
    return static_cast<CDockablePane*>(DowncastTo(p, impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ()));
}
inline CBasePane* AsBasePane(void* p) {
    return static_cast<CBasePane*>(DowncastTo(p, impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ()));
}

// Mirror of CList<CObject*, CObject*>::CNode, the node a CObList::POSITION
// points at (same read-only view as visualmanager/CMFCVisualManager.cpp).
struct ObNode {
    ObNode*  pNext;
    ObNode*  pPrev;
    CObject* data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");
static_assert(sizeof(CObList) == 0x38, "CObList must be 56 bytes");
inline ObNode* NodeFromPos(CObList::POSITION pos) {
    ObNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof(p));
    return p;
}

// The `CObList lst; m_barContainerManager.AddPanesToList(&lst, NULL);` pair the
// retail bodies open with (the list's vftable is 0x328538 and it is torn down
// through CPtrList::RemoveAll, 0x83d0).  Built and destroyed through the
// exported CObList constructor/destructor so nothing here links a C++ method.
class VisiblePaneList {
public:
    explicit VisiblePaneList(void* pFrame) {
        std::memset(m_buf, 0, sizeof(m_buf));
        impl___0CObList__QEAA__J_Z(List(), 10);
        impl__AddPanesToList_CPaneContainerManager__QEAAXPEAVCObList__0_Z(Mgr(pFrame), List(), nullptr);
    }
    ~VisiblePaneList() { impl___1CObList__UEAA_XZ(List()); }
    ObNode* First() const {
        return NodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(List(), 0));
    }
    VisiblePaneList(const VisiblePaneList&) = delete;
    VisiblePaneList& operator=(const VisiblePaneList&) = delete;
private:
    CObList* List() const { return reinterpret_cast<CObList*>(const_cast<unsigned char*>(m_buf)); }
    alignas(8) unsigned char m_buf[sizeof(CObList)];
};

// `m_barContainerManager.m_lstControlBars.m_nCount == 0` (retail +0x278, also
// the unexported vftable slot 0x350 at 0x92360), through the IsEmpty export.
inline bool ManagerIsEmpty(void* pThis) {
    return impl__IsEmpty_CPaneContainerManager__QEBAHXZ(Mgr(pThis)) != 0;
}

// CDockablePane::CanAcceptPane -- the unexported CDockablePane vftable slot
// 0x318 (0x12d90), transcribed:
//     return pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(CDockablePane)) &&
//            (IsDocked() /*slot 0x2e0*/ || IsInFloatingMultiPaneFrameWnd() /*slot 0x360*/);
// Deviation: static dispatch, so a further-derived override is not reached.
BOOL DockableCanAcceptPane(const CDockablePane* pThis, const void* pBar) {
    if (pThis == nullptr) return FALSE;
    if (AsDockable(const_cast<void*>(pBar)) == nullptr) return FALSE;
    if (impl__IsDocked_CBasePane__UEBAHXZ(static_cast<const CBasePane*>(pThis))) return TRUE;
    return impl__IsInFloatingMultiPaneFrameWnd_CDockablePane__UEBAHXZ(pThis) ? TRUE : FALSE;
}

// Roll-up timer gate shared by OnSetRollUpTimer / OnKillRollUpTimer (0x92c70 /
// 0x92d30): TRUE when any visible pane in the frame is a CBasePane whose
// control-bar style has AFX_CBRS_AUTO_ROLLUP.
bool AnyVisiblePaneAutoRollsUp(void* pThis) {
    VisiblePaneList lst(pThis);
    for (ObNode* n = lst.First(); n != nullptr; n = n->pNext) {
        CBasePane* p = AsBasePane(n->data);
        if (p != nullptr && (p->m_dwControlBarStyle & kCbrsAutoRollUp)) return true;
    }
    return false;
}

// ---- helpers used by SetDockState only ------------------------------------
// CBasePane::m_bRecentVisibleState, the member the unexported CBasePane virtual
// at slot 0x368 (0x87f0: `mov 0xec(%rcx),%eax; ret`, GetRecentVisibleState in
// the shipping afxbasepane.h) returns; CPane::m_rectSavedDockedRect, the CRect
// SetDockState's final SetWindowPos reads at pane +0x3d8..+0x3e4.
static_assert(offsetof(CBasePane, m_bRecentVisibleState) == 0xec, "m_bRecentVisibleState moved");
static_assert(offsetof(CPane, m_rectSavedDockedRect) == 0x3d8, "m_rectSavedDockedRect moved");
// CBRS_ALIGN_ANY as retail passes it to SetAutoHideMode (`mov $0xf000,%r8d`);
// spelled locally because include/openmfc/afxole.h defines CBRS_ALIGN_ANY as
// 0x000F (docking/CPaneContainerManager.cpp makes the same note).
constexpr unsigned long kCbrsAlignAnyRetail = 0xF000UL;
// AFX_DOCK_METHOD::DM_UNKNOWN / DM_SHOW (shipping afxbasepane.h enum order:
// DM_UNKNOWN, DM_MOUSE, DM_DBL_CLICK, DM_SHOW, DM_RECT, DM_STANDARD).
constexpr int kDockMethodUnknown = 0;
constexpr int kDockMethodShow    = 3;
// SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED, the 0x34 SetDockState passes.
constexpr unsigned int kSwpRestoreDocked = SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED;
static_assert(kSwpRestoreDocked == 0x34, "SWP immediate");

// Read-only view of one CList<UINT,UINT> node (the same shape
// docking/CPaneContainer.cpp's S_CListUINTNode pins: pNext +0, pPrev +8, the
// UINT at +0x10) and of the list head pointer at list+0x08.
struct UintNode {
    UintNode*    pNext;
    UintNode*    pPrev;
    unsigned int value;
};
constexpr size_t kOffUintListHead = 0x08;
inline UintNode* UintListHead(void* pList) {
    return pList ? *reinterpret_cast<UintNode**>(static_cast<char*>(pList) + kOffUintListHead) : nullptr;
}

// CDockablePane::IsAutoHideMode -- the unexported CDockablePane vftable slot
// 0x3c8 (0x12e00: `mov 0x438(%rcx),%eax`, m_bPinState).  Not an export and
// +0x438 has no in-object storage in OpenMFC's CDockablePane, so this reads
// the same docking_state side table that docking/CDockablePane.cpp's C++
// CDockablePane::IsAutoHideMode reads (PaneCoreStates() is an inline accessor
// of a function-local static, so it is the identical map).
BOOL PaneIsAutoHideMode(const void* pPane) {
    std::lock_guard<std::mutex> lock(openmfc::docking_state::PaneCoreStateMutex());
    auto& states = openmfc::docking_state::PaneCoreStates();
    auto it = states.find(pPane);
    return it != states.end() ? it->second.autoHideMode : FALSE;
}

// `pMiniFrame->RemovePane(pBar, bDestroy, bNoDelayedDestroy)` -- CPaneFrameWnd
// vftable slot 0x308.  This DLL has no retail-shaped mini-frame vtable, so the
// slot is resolved by hand: a CMultiPaneFrameWnd gets this file's override,
// anything else the CPaneFrameWnd export.  A client subclass's override is not
// reached (deviation).
void MiniFrameRemovePane(void* pMini, void* pPane, int bDestroy, int bNoDelayedDestroy) {
    if (DowncastTo(pMini, impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ()) != nullptr)
        impl__RemovePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(pMini, pPane, bDestroy, bNoDelayedDestroy);
    else
        impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(pMini, pPane, bDestroy, bNoDelayedDestroy);
}

// `CWnd::FromHandle(::GetParent(CWnd::FromHandle(::GetParent(h))->m_hWnd))`, the
// pane->GetParent()->GetParent() retail spells out at 0x93d85 and 0x93ef8
// inside SetDockState (entry 0x93bd0): import slot 0x2c5300 = USER32!GetParent,
// direct call 0x289180 = CWnd::FromHandle.  Retail has no NULL check between
// the two steps; NULL is returned here instead of faulting.
CWnd* GrandParentWnd(HWND hwnd) {
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hwnd));
    if (pParent == nullptr) return nullptr;
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pParent->m_hWnd));
}

}  // namespace

// Symbol: ??0CMultiPaneFrameWnd@@QEAA@XZ
// Decoded from retail entry RVA 0x92450 -- see the layout comment at the top of
// this file for the instruction-by-instruction reading.  The base constructor
// thunk placement-news OpenMFC's CPaneFrameWnd (docking/Thunks.cpp:1138), the
// embedded manager is then constructed at +0x258 and m_hWndLastFocused zeroed.
// Deviations: (1) no vfptr is stored -- this DLL has no retail-shaped
// CMultiPaneFrameWnd vtable; the vfptr the base placement-new installed stays
// (see the family note at the top of docking/CPaneFrameWnd.cpp); (2) the store
// of FALSE to CPaneFrameWnd::m_bHostsToolbar (+0x104 by the shipping
// afxpaneframewnd.h member order; CPaneFrameWnd::AddPane, 0xaf330, reads and
// writes it) is NOT made: +0x104 of OpenMFC's CPaneFrameWnd is inside the live
// CWnd/CFrameWnd sub-object, and the flag has no side-table counterpart;
// (3) the g_multiFramePanes entry the pre-existing side-table bodies
// (GetCaptionText) read is still created.
extern "C" void* MS_ABI impl___0CMultiPaneFrameWnd__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    impl___0CPaneFrameWnd__QEAA_XZ(pThis);
    impl___0CPaneContainerManager__QEAA_XZ(Mgr(pThis));
    LastFocused(pThis) = nullptr;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_multiFramePanes[pThis] = {};
    return pThis;
}
// Symbol: ??1CMultiPaneFrameWnd@@UEAA@XZ
// Decoded from retail entry RVA 0x924f0:
//     vfptr = &CMultiPaneFrameWnd::vftable;
//     m_barContainerManager.~CPaneContainerManager();       // call 0xa8180
//     CPaneFrameWnd::~CPaneFrameWnd();                      // tail jump 0xad890
// The vfptr store is not made (see the constructor).  The side-table entry is
// dropped first so no thunk called during base destruction re-creates it.
extern "C" void* MS_ABI impl___1CMultiPaneFrameWnd__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        g_multiFramePanes.erase(pThis);
    }
    impl___1CPaneContainerManager__UEAA_XZ(Mgr(pThis));
    impl___1CPaneFrameWnd__UEAA_XZ(pThis);
    return pThis;
}
// Symbol: ?AddPane@CMultiPaneFrameWnd@@UEAAXPEAVCBasePane@@@Z
// Decoded from retail entry RVA 0x92e80:
//     CDockablePane* p = DYNAMIC_DOWNCAST(CDockablePane, pWnd);       // RTC 0x3aa178
//     if (m_barContainerManager.m_lstControlBars.m_nCount == 0) {     // cmpq $0,0x20(mgr)
//         m_barContainerManager.AddPane(p);                           // mgr slot 0x48
//         CPaneFrameWnd::AddPane(pWnd);                               // call 0xaf330
//     }
//     OnSetRollUpTimer();                                             // tail, slot 0x3f8
// The count test is the IsEmpty export (ManagerIsEmpty).  The pre-existing
// g_multiFramePanes bookkeeping is kept for GetCaptionText.
// NOTE: docking/CPaneContainerManager.cpp's AddPane only records the pane in
// its side table; retail's (0xa82c0) also stores it into
// m_pRootContainer->m_pLeftPane, so after this call the root container still
// reads as empty in this build -- see OnCheckEmptyState.
extern "C" void MS_ABI impl__AddPane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane___Z(void* pThis, void* pPane) {
    if (pThis == nullptr) return;
    void* pDockable = AsDockable(pPane);
    if (ManagerIsEmpty(pThis)) {
        impl__AddPane_CPaneContainerManager__UEAAXPEAVCDockablePane___Z(Mgr(pThis), pDockable);
        impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(pThis, pPane);
    }
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        AddUniquePane(g_multiFramePanes[pThis], pPane);
    }
    impl__OnSetRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(pThis);
}
// Symbol: ?RemovePane@CMultiPaneFrameWnd@@UEAAXPEAVCBasePane@@HH@Z
// Decoded from retail entry RVA 0x932b0 (the third argument, %r9d, is never read):
//     if (pWnd->IsKindOf(RUNTIME_CLASS(CDockablePane))) {            // 0x233310, RTC 0x3aa178
//         m_barContainerManager.RemovePaneFromPaneContainer(pWnd);  // mgr slot 0x78
//         if (m_barContainerManager.m_lstControlBars.m_nCount != 0) // cmpq $0,0x278(this)
//             CPaneFrameWnd::ReplacePane(pWnd,                      // call 0xaf880
//                 m_barContainerManager.GetFirstPane());            // mgr slot 0xf0
//         else {
//             CPaneFrameWnd::RemovePane(pWnd, FALSE, FALSE);        // call 0xaf680
//             if (CWnd::FromHandlePermanent(m_hEmbeddedBar /*0x148*/) == NULL) {
//                 CBasePane* p = m_barContainerManager.GetFirstPane();
//                 m_hEmbeddedBar = p ? p->m_hWnd : NULL;
//             }
//         }
//     }
//     if (bDestroy && m_barContainerManager.m_lstControlBars.m_nCount == 0)  // slot 0x350
//         ::PostMessage(m_hWnd, AFX_WM_CHECKEMPTYMINIFRAME, 0, 0);  // import 0x2c52d8
//     else {
//         CheckGripperVisibility();                                 // slot 0x320
//         OnPaneRecalcLayout();                                     // slot 0x3a8
//         ::SendMessage(m_hWnd, WM_NCPAINT /*0x85*/, 0, 0);         // import 0x2c5378
//     }
//     OnKillRollUpTimer();                                          // tail, slot 0x400
// 0x148 has no member here; docking/CPaneFrameWnd.cpp models it as the
// g_framePanes list, whose GetPane export (0xaf900 is exactly
// `FromHandlePermanent(m_hEmbeddedBar)`) stands in for the read and whose AddPane
// export stands in for the store (deviation: that export also clears the
// side-table roll-up flag).  Further deviations: a NULL pWnd is skipped
// (retail's IsKindOf would fault); the RemovePaneFromPaneContainer export is
// still a placeholder (see its declaration), so the manager's list does not
// shrink here and the "!= 0" branch is the one taken whenever the pane was
// ever added; the pre-existing g_multiFramePanes entry is still removed for
// GetCaptionText.
extern "C" void MS_ABI impl__RemovePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int bDestroy, int) {
    if (pThis == nullptr) return;
    if (AsDockable(pPane) != nullptr) {
        impl__RemovePaneFromPaneContainer_CPaneContainerManager__UEAAHPEAVCDockablePane___Z(Mgr(pThis), pPane);
        if (!ManagerIsEmpty(pThis)) {
            void* pFirst = impl__GetFirstPane_CPaneContainerManager__UEBAPEAVCBasePane__XZ(Mgr(pThis));
            impl__ReplacePane_CPaneFrameWnd__UEAAXPEAVCBasePane__0_Z(pThis, pPane, pFirst);
        } else {
            impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(pThis, pPane, FALSE, FALSE);
            if (impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pThis) == nullptr) {
                void* pFirst = impl__GetFirstPane_CPaneContainerManager__UEBAPEAVCBasePane__XZ(Mgr(pThis));
                if (pFirst != nullptr) impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(pThis, pFirst);
            }
        }
    }
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        RemovePane(g_multiFramePanes[pThis], pPane);
    }
    HWND hwnd = FrameHwnd(pThis);
    if (bDestroy && ManagerIsEmpty(pThis)) {
        if (hwnd) ::PostMessage(hwnd, impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA, 0, 0);
    } else {
        impl__CheckGripperVisibility_CMultiPaneFrameWnd__UEAAXXZ(pThis);
        impl__OnPaneRecalcLayout_CMultiPaneFrameWnd__UEAAXXZ(pThis);
        if (hwnd) ::SendMessage(hwnd, WM_NCPAINT, 0, 0);
    }
    impl__OnKillRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(pThis);
}
// Symbol: ?GetPane@CMultiPaneFrameWnd@@UEBAPEAVCWnd@@XZ
// Decoded from retail entry RVA 0x94ce0:
//     CWnd* p = CWnd::FromHandlePermanent(m_hEmbeddedBar /*0x148*/);   // 0x2891d0
//     if (p) return p;
//     p = GetFirstVisiblePane();          // slot 0x368 -> mgr.GetFirstVisiblePane (slot 0x100)
//     if (p) return p;
//     return m_barContainerManager.GetFirstPane();                 // mgr slot 0xf0
// The first step is exactly the CPaneFrameWnd::GetPane export (0xaf900 is
// `mov 0x148(%rcx),%rcx; jmp FromHandlePermanent`), so it is called for it.
extern "C" void* MS_ABI impl__GetPane_CMultiPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    void* p = impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pThis);
    if (p != nullptr) return p;
    p = impl__GetFirstVisiblePane_CPaneContainerManager__UEBAPEAVCWnd__XZ(Mgr(pThis));
    if (p != nullptr) return p;
    return impl__GetFirstPane_CPaneContainerManager__UEBAPEAVCBasePane__XZ(Mgr(pThis));
}
// Symbol: ?GetCaptionText@CMultiPaneFrameWnd@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetCaptionText_CMultiPaneFrameWnd__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    pRet->Format(L"Pane group (%d)", static_cast<int>(g_multiFramePanes[pThis].panes.size()));
}
// Symbol: ?CalcExpectedDockedRect@CMultiPaneFrameWnd@@UEAAXPEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CMultiPaneFrameWnd__UEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    void*, void*, CPoint, CRect* pRect, int* pnAlign, void** ppBar) {
    if (pRect) pRect->SetRectEmpty();
    if (pnAlign) *pnAlign = 0;
    if (ppBar) *ppBar = nullptr;
}

// Symbol: ?AddRecentPane@CMultiPaneFrameWnd@@UEAAHPEAVCDockablePane@@@Z
// STUB.  Retail entry RVA 0x92f00.  The body branches on two members of the
// pane's CRecentDockSiteInfo (pane +0x3c0 / +0x3c8, which by the shipping
// afxrecentdocksiteinfo.h layout are m_recentDockInfo.m_recentMiniFrameInfo.
// m_pRecentBarContainer and .m_pRecentContainerOfTabWnd).  With the first
// non-NULL it re-parents the pane (::SetParent,
// import 0x2c52b0), registers it (AddRemovePaneFromGlobalList, 0xaf740), calls
// m_barContainerManager.AddPaneToRecentPaneContainer (mgr slot 0x50), fixes
// m_hEmbeddedBar (0x148), and -- when the frame then holds exactly one visible pane
// and it is this one -- CWnd::SetWindowPos to m_rectRecentFloatingRect (0x150);
// with the second it either dispatches the tabbed pane's AttachToTabWnd (pane
// slot 0x6a8) / ShowPane (0x458) / OnPaneRecalcLayout, or falls back to
// AddPaneToRecentPaneContainer; both paths end in OnShowPane (slot 0x3e0) and
// OnSetRollUpTimer (0x3f8) and return TRUE, or FALSE when both members are
// NULL.  Not reproduced: CRecentDockSiteInfo is an opaque block in OpenMFC with
// no accessor thunk that carries `this` (docking/CRecentDockSiteInfo.cpp), so
// neither branch condition can be read, and AddPaneToRecentPaneContainer
// (docking/CPaneContainerManager.cpp) is itself a placeholder.
// Signature corrected: the generated list dropped `this`.
extern "C" int MS_ABI impl__AddRecentPane_CMultiPaneFrameWnd__UEAAHPEAVCDockablePane___Z(void* pThis, void* pBar) {
    (void)pThis;
    (void)pBar;
    return FALSE;
}

// Symbol: ?AdjustPaneFrames@CMultiPaneFrameWnd@@UEAAXXZ
// Decoded from retail entry RVA 0x92df0: walks m_barContainerManager.
// m_lstControlBars (head at +0x268) and for every entry
//     CDockablePane* p = DYNAMIC_DOWNCAST(CDockablePane, entry);    // RTC 0x3aa178
//     p->SetWindowPos(NULL, -1, -1, -1, -1,                         // pane slot 0x480
//                     SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED /*0x37*/,
//                     NULL);
// (%r8d/%r9d are both `or $-1`, the two stack ints are `movl $-1`.)
// Deviations: retail walks the raw list, this walks the AddPanesToList copy
// (panes without WS_VISIBLE are not visited -- see LIST REPRESENTATION);
// retail dereferences a NULL downcast, entries that are not CDockablePanes are
// skipped here.  Static dispatch to CBasePane::SetWindowPos.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__AdjustPaneFrames_CMultiPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    VisiblePaneList lst(pThis);
    for (ObNode* n = lst.First(); n != nullptr; n = n->pNext) {
        CDockablePane* p = AsDockable(n->data);
        if (p == nullptr) continue;
        impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
            static_cast<CBasePane*>(p), nullptr, -1, -1, -1, -1, kSwpFrameChanged, nullptr);
    }
}

// Symbol: ?CanBeDockedToPane@CMultiPaneFrameWnd@@UEBAHPEBVCDockablePane@@@Z
// Decoded from retail entry RVA 0x94be0: walks m_lstControlBars (head +0x268);
// for every entry
//     CDockablePane* p = DYNAMIC_DOWNCAST(CDockablePane, entry);    // RTC 0x3aa178
//     if (pBar->CanAcceptPane(p) && p->CanAcceptPane(pBar))         // slot 0x318 on each
//         return TRUE;
// and returns FALSE after the walk (or immediately when the list is empty).
// Both CanAcceptPane calls are the unexported CDockablePane body at 0x12d90,
// reproduced by DockableCanAcceptPane above.  Deviations: retail walks the raw
// list, this walks the AddPanesToList copy (panes without WS_VISIBLE are not
// considered -- see LIST REPRESENTATION); a NULL pBar or a NULL downcast is
// treated as "cannot accept" instead of faulting; static dispatch.
// Signature corrected: the generated list dropped `this`.
extern "C" int MS_ABI impl__CanBeDockedToPane_CMultiPaneFrameWnd__UEBAHPEBVCDockablePane___Z(const void* pThis, const void* pBar) {
    if (pThis == nullptr || pBar == nullptr) return FALSE;
    const CDockablePane* pDockBar = static_cast<const CDockablePane*>(pBar);
    VisiblePaneList lst(const_cast<void*>(pThis));
    for (ObNode* n = lst.First(); n != nullptr; n = n->pNext) {
        CDockablePane* p = AsDockable(n->data);
        if (p == nullptr) continue;
        if (DockableCanAcceptPane(pDockBar, p) && DockableCanAcceptPane(p, pBar)) return TRUE;
    }
    return FALSE;
}

// Symbol: ?CheckGripperVisibility@CMultiPaneFrameWnd@@UEAAXXZ
// Decoded from retail entry RVA 0x94540:
//     if (!::IsWindowVisible(m_hWnd)) return;                     // import 0x2c5350
//     if (m_barContainerManager.GetVisiblePaneCount() == 1) {     // mgr slot 0xb8
//         CWnd* p = m_barContainerManager.GetFirstVisiblePane();  // mgr slot 0x100
//         if (p && p->IsKindOf(RUNTIME_CLASS(CDockablePane)))     // RTC 0x3aa178
//             ((CDockablePane*)p)->EnableGripper(FALSE);          // pane slot 0x3f0
//     } else
//         m_barContainerManager.EnableGrippers(TRUE);             // mgr slot 0xc0
// Static dispatch on both slots.  (EnableGripper is a documented stub in
// docking/CDockablePane.cpp:1305; the call is still made.)
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__CheckGripperVisibility_CMultiPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    HWND hwnd = FrameHwnd(pThis);
    if (hwnd == nullptr || !::IsWindowVisible(hwnd)) return;
    if (impl__GetVisiblePaneCount_CPaneContainerManager__UEBAHXZ(Mgr(pThis)) == 1) {
        CDockablePane* p = AsDockable(impl__GetFirstVisiblePane_CPaneContainerManager__UEBAPEAVCWnd__XZ(Mgr(pThis)));
        if (p != nullptr) impl__EnableGripper_CDockablePane__UEAAXH_Z(p, FALSE);
    } else {
        impl__EnableGrippers_CPaneContainerManager__UEAAXH_Z(Mgr(pThis), TRUE);
    }
}

// Symbol: ?CloseMiniFrame@CMultiPaneFrameWnd@@UEAAXXZ
// Decoded from retail entry RVA 0x944f0:
//     if (OnCloseMiniFrame()) {                 // slot 0x450 -> CPaneFrameWnd::OnCloseMiniFrame (0xb4940)
//         CWnd::ShowWindow(SW_HIDE);            // call 0x2a79e0
//         m_barContainerManager.HideAll();      // mgr slot 0xc8
//     }
// Static dispatch on both slots.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__CloseMiniFrame_CMultiPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (impl__OnCloseMiniFrame_CPaneFrameWnd__MEAAHXZ(pThis)) {
        impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pThis), SW_HIDE);
        impl__HideAll_CPaneContainerManager__UEAAXXZ(Mgr(pThis));
    }
}

// Symbol: ?ConvertToTabbedDocument@CMultiPaneFrameWnd@@UEAAXXZ
// Decoded from retail entry RVA 0x94ea0:
//     CObList lst;                                                  // vftable 0x328538
//     m_barContainerManager.AddPanesToList(&lst, NULL);             // call 0xaa960
//     for (each entry) {
//         CDockablePane* p = DYNAMIC_DOWNCAST(CDockablePane, entry);  // RTC 0x3aa178
//         p->ConvertToTabbedDocument(FALSE);                        // pane slot 0x5d0
//     }
//     ::PostMessage(m_hWnd, AFX_WM_CHECKEMPTYMINIFRAME, 0, 0);      // import 0x2c52d8,
//                                                                   // .data 0x3bae78
// (lst is destroyed through CPtrList::RemoveAll, 0x83d0.)  Deviations: NULL
// downcasts are skipped rather than faulted on; static dispatch to the
// CDockablePane export, which is a documented stub (docking/CDockablePane.cpp:1036).
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CMultiPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    {
        VisiblePaneList lst(pThis);
        for (ObNode* n = lst.First(); n != nullptr; n = n->pNext) {
            CDockablePane* p = AsDockable(n->data);
            if (p == nullptr) continue;
            impl__ConvertToTabbedDocument_CDockablePane__UEAAXH_Z(p, FALSE);
        }
    }
    HWND hwnd = FrameHwnd(pThis);
    if (hwnd) ::PostMessage(hwnd, impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA, 0, 0);
}

// Symbol: ?CreateObject@CMultiPaneFrameWnd@@SAPEAVCObject@@XZ
// Decoded from retail entry RVA 0x923d0:
//     void* p = operator new(0x300);          // call 0x2840 (??2@YAPEAX_K@Z)
//     if (p) CMultiPaneFrameWnd::CMultiPaneFrameWnd(p);   // call 0x92450
//     return p;
// The same exported operator new is used.  The allocation is zero-filled first
// (retail leaves it uninitialised) because the OpenMFC base constructor does not
// write every retail CPaneFrameWnd offset that later bodies in this family may
// read.
extern "C" void* MS_ABI impl__CreateObject_CMultiPaneFrameWnd__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kSizeCMultiPaneFrameWnd);
    if (p == nullptr) return nullptr;
    std::memset(p, 0, kSizeCMultiPaneFrameWnd);
    impl___0CMultiPaneFrameWnd__QEAA_XZ(p);
    return p;
}

// Symbol: ?DockFrame@CMultiPaneFrameWnd@@UEAAHPEAVCPaneFrameWnd@@W4AFX_DOCK_METHOD@@@Z
// STUB.  Retail entry RVA 0x92570.  After requiring pDockedFrame to be a
// CMultiPaneFrameWnd (RTC 0x3aa2f8) whose GetFirstVisiblePane (slot 0x368) is a
// CBasePane (RTC 0x2da490) passing the unexported CBasePane virtual at slot
// 0x340 (0x87d0), it: for DM_MOUSE (1) / DM_STANDARD (5) asks
// CGlobalUtils::GetPaneAndAlignFromPoint (0x6d560) for the target pane and
// alignment at the cursor; then either merges the other frame's manager into
// this one through AddPaneContainerManagerToDockablePane (mgr slot 0x40) --
// after possibly re-targeting through a CBaseTabbedPane virtual (+0x7b0, RTC
// 0x2db168) -- or AddPaneContainerManager (mgr slot 0x30); posts
// AFX_WM_CHECKEMPTYMINIFRAME to the other frame, re-applies its 0x150
// rectangle, calls OnPaneRecalcLayout, for DM_MOUSE re-enters drag mode on the
// target pane (slot 0x638, RTC 0x2f3868) or synthesises WM_LBUTTONDOWN, and
// ends in OnSetRollUpTimer.  Not reproduced: GetPaneAndAlignFromPoint is a
// generated placeholder (core/runtime/CGlobalUtils.cpp:169), the two
// AddPaneContainerManager exports are placeholders in
// docking/CPaneContainerManager.cpp, and slots 0x340/0x7b0 are unexported.
// Signature corrected: the generated 15-parameter list was garbage.
extern "C" int MS_ABI impl__DockFrame_CMultiPaneFrameWnd__UEAAHPEAVCPaneFrameWnd__W4AFX_DOCK_METHOD___Z(
    void* pThis, void* pDockedFrame, int dockMethod) {
    (void)pThis;
    (void)pDockedFrame;
    (void)dockMethod;
    // Retail returns TRUE on every early-out and FALSE after a completed dock;
    // TRUE is the "nothing was docked" value.
    return TRUE;
}

// Symbol: ?DockPane@CMultiPaneFrameWnd@@UEAAHPEAVCDockablePane@@@Z
// STUB.  Retail entry RVA 0x92880.  ::GetCursorPos, then
// CGlobalUtils::GetPaneAndAlignFromPoint(m_barContainerManager, pt, &pTarget,
// &dwAlignment, &bTabArea, &bCaption) (0x6d560); when that yields a target
// and alignment it calls the pane's slot 0x490 with FALSE, ::SetParent(pBar,
// this), m_barContainerManager.InsertPane(pBar, pTarget, dwAlignment, NULL,
// DM_UNKNOWN) (mgr slot 0x58), and on success AddRemovePaneFromGlobalList(pBar,
// TRUE), CheckGripperVisibility, OnPaneRecalcLayout, WM_NCPAINT; then
// OnSetRollUpTimer, a conditional pBar->SetFocus (pane slot 0x3c0), a second
// OnPaneRecalcLayout, and returns `InsertPane() == 0`.  Not reproduced:
// GetPaneAndAlignFromPoint is a generated placeholder
// (core/runtime/CGlobalUtils.cpp:169) that can never yield a target, so the
// only path this build could take is the early `return TRUE`.
// Signature corrected: the generated list dropped `this`.
extern "C" int MS_ABI impl__DockPane_CMultiPaneFrameWnd__UEAAHPEAVCDockablePane___Z(void* pThis, void* pBar) {
    (void)pThis;
    (void)pBar;
    return TRUE;
}

// Symbol: ?DockPaneStandard@CMultiPaneFrameWnd@@MEAAPEAVCDockablePane@@AEAH@Z
// STUB.  Retail entry RVA 0x92a40.  Gated on OnBeforeDock() (slot 0x348); then
// builds two CObLists (AddPanesToList, plus the HWND list of every CWnd entry),
// reads m_dragFrameImpl.m_pFinalTargetBar (+0x208) and m_dragFrameImpl.
// m_bDockToTab (+0x1f8), calls the target bar's slot 0x460 with 0 to get its
// docking target, and either SetPreDockState(m_bDockToTab ? 2 : 1,
// m_pFinalTargetBar, DM_STANDARD) (slot 0x330) or -- when that target is
// another CMultiPaneFrameWnd -- that frame's DockFrame(this, DM_STANDARD)
// (slot 0x4a0); writes `bWasDocked = (result == 0)`, restores the focus, and
// when bWasDocked calls slot 0x6c8 on every recorded pane whose HWND is still
// a permanent CDockablePane.  Not reproduced: SetPreDockState and DockFrame
// are stubs in this file, and m_dragFrameImpl is not modelled in OpenMFC's
// CPaneFrameWnd.  Retail always returns NULL.
// Signature corrected: the generated list dropped `this`.
extern "C" void* MS_ABI impl__DockPaneStandard_CMultiPaneFrameWnd__MEAAPEAVCDockablePane__AEAH_Z(void* pThis, int* pbWasDocked) {
    (void)pThis;
    if (pbWasDocked) *pbWasDocked = FALSE;
    return nullptr;
}

// Symbol: ?DockRecentPaneToMainFrame@CMultiPaneFrameWnd@@UEAAXPEAVCDockablePane@@@Z
// Decoded from retail entry RVA 0x949b0:
//     CPaneFrameWnd::AddRemovePaneFromGlobalList(pBar, FALSE);      // call 0xaf740
//     pBar->DockPane(pBar, NULL, DM_DBL_CLICK /*2*/);               // pane slot 0x400
// (%rcx and %rdx are both pBar at the virtual call.)  Slot 0x400 of the
// CDockablePane vftable is ?DockPane@CPane@@ (0xa04b0); static dispatch.
// Deviation: a NULL pBar returns (retail would fault in the virtual call).
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__DockRecentPaneToMainFrame_CMultiPaneFrameWnd__UEAAXPEAVCDockablePane___Z(void* pThis, void* pBar) {
    (void)pThis;   // retail never reads `this`
    if (pBar == nullptr) return;
    impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(pBar, FALSE);
    impl__DockPane_CPane__UEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
        static_cast<CPane*>(pBar), static_cast<CBasePane*>(pBar), nullptr, kDockMethodDblClick);
}

// Symbol: ?InsertPane@CMultiPaneFrameWnd@@UEAAHPEAVCBasePane@@0H@Z
// Decoded from retail entry RVA 0x94ab0 (pTarget and bAfter are never read):
//     CPaneFrameWnd::AddRemovePaneFromGlobalList(pControlBar, TRUE);   // call 0xaf740
//     return TRUE;
// Signature corrected: the generated list dropped `this`.
extern "C" int MS_ABI impl__InsertPane_CMultiPaneFrameWnd__UEAAHPEAVCBasePane__0H_Z(void* pThis, void* pControlBar, void* pTarget, int bAfter) {
    (void)pThis;
    (void)pTarget;
    (void)bAfter;
    impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(pControlBar, TRUE);
    return TRUE;
}

// Symbol: ?LoadState@CMultiPaneFrameWnd@@UEAAHPEB_WI@Z
// Decoded from retail ?LoadState@CMultiPaneFrameWnd@@UEAAHPEBDI@Z, entry RVA
// 0x93b20 (the Unicode name above has no entry of its own in
// mfc140_rva_symbols.json; slot 0x3c0 of the vftable at 0x2ee738 points there).
// Walks m_lstControlBars (head +0x268); for every entry
//     CDockablePane* p = DYNAMIC_DOWNCAST(CDockablePane, entry);        // RTC 0x3aa178
//     p->LoadState(lpszProfileName, uiID, (UINT)-1);                   // pane slot 0x468
// and returns TRUE.  Slot 0x468 is ?LoadState@CDockablePane@@UEAAHPEBDHI@Z, a
// jmp thunk onto ?LoadState@CPane@@ (0xa2260): note retail passes this frame's
// uiID as the pane's nIndex and -1 as its uiID.  Deviations: retail walks the
// raw list, this walks the AddPanesToList copy (panes without WS_VISIBLE are
// not visited -- see LIST REPRESENTATION); NULL downcasts are skipped rather
// than faulted on; static dispatch to the CPane export (docking/CPane.cpp:943,
// a documented stub).
// Signature corrected: the generated list dropped `this`.
extern "C" int MS_ABI impl__LoadState_CMultiPaneFrameWnd__UEAAHPEB_WI_Z(void* pThis, const wchar_t* lpszProfileName, unsigned int uiID) {
    if (pThis == nullptr) return FALSE;
    VisiblePaneList lst(pThis);
    for (ObNode* n = lst.First(); n != nullptr; n = n->pNext) {
        CDockablePane* p = AsDockable(n->data);
        if (p == nullptr) continue;
        impl__LoadState_CPane__UEAAHPEB_WHI_Z(static_cast<CPane*>(p), lpszProfileName, static_cast<int>(uiID), static_cast<unsigned int>(-1));
    }
    return TRUE;
}

// Symbol: ?OnCheckEmptyState@CMultiPaneFrameWnd@@IEAA_J_K_J@Z
// Decoded from retail entry RVA 0x949f0 (wParam/lParam are never read):
//     CPaneContainer* pRoot = m_barContainerManager.m_pRootContainer;  // +0x2d0
//     if (pRoot) pRoot->ReleaseEmptyPaneContainer();                  // call 0xa4e90
//     pRoot = m_barContainerManager.m_pRootContainer;                 // re-read
//     if (pRoot == NULL || pRoot->GetNodeCount() == 0 ||              // call 0xa7c60
//         (pRoot->GetNodeCount() == 1 && pRoot->m_nRefCount /*+0x50*/ == 0 &&
//          pRoot->IsEmpty())) {                                        // call 0xa7040
//         OnCancelMode();                                             // call 0xb1e80
//         DestroyWindow();                                            // slot 0xd0
//     } else if (m_barContainerManager.GetVisiblePaneCount() == 0) {  // mgr slot 0xb8
//         CWnd::ShowWindow(SW_HIDE);                                  // call 0x2a79e0
//         OnCancelMode();
//     }
//     return 0;
// +0x50 is S_CPaneContainer::m_nRefCount as pinned in docking/CPaneContainer.cpp.
// Static dispatch on both slots.  NOTE (behavioural, inherited from a sibling):
// docking/CPaneContainerManager.cpp's AddPane records panes in its side table
// only and never stores them into m_pRootContainer->m_pLeftPane as retail's
// 0xa82c0 does, so in this build a root container that holds one pane still
// satisfies `GetNodeCount()==1 && IsEmpty()` and this handler takes the
// destroy branch.  That is the faithful reading of retail on top of the current
// sibling; the fix belongs in CPaneContainerManager::AddPane.
// Signature corrected: the generated list dropped `this`.
extern "C" __int64 MS_ABI impl__OnCheckEmptyState_CMultiPaneFrameWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64, __int64) {
    if (pThis == nullptr) return 0;
    void* pRoot = MgrRootContainer(pThis);
    if (pRoot != nullptr) impl__ReleaseEmptyPaneContainer_CPaneContainer__QEAAXXZ(pRoot);
    pRoot = MgrRootContainer(pThis);
    bool bDestroy = (pRoot == nullptr);
    if (!bDestroy) {
        const int nNodes = impl__GetNodeCount_CPaneContainer__QEBAHXZ(pRoot);
        if (nNodes == 0) {
            bDestroy = true;
        } else if (nNodes == 1 &&
                   *reinterpret_cast<const int*>(static_cast<const char*>(pRoot) + 0x50) == 0 &&
                   impl__IsEmpty_CPaneContainer__QEBAHXZ(pRoot)) {
            bDestroy = true;
        }
    }
    if (bDestroy) {
        impl__OnCancelMode_CPaneFrameWnd__IEAAXXZ(pThis);
        impl__DestroyWindow_CWnd__UEAAHXZ(AsWnd(pThis));
    } else if (impl__GetVisiblePaneCount_CPaneContainerManager__UEBAHXZ(Mgr(pThis)) == 0) {
        impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pThis), SW_HIDE);
        impl__OnCancelMode_CPaneFrameWnd__IEAAXXZ(pThis);
    }
    return 0;
}

// Symbol: ?OnCreate@CMultiPaneFrameWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Decoded from retail ?OnCreate@CMultiPaneFrameWnd@@IEAAHPEAUtagCREATESTRUCTA@@@Z,
// entry RVA 0x92530 (the Unicode twin is 0x91f70 in mfc140u, named by the
// class's message map entry for WM_CREATE):
//     if (CPaneFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;   // call 0xb3670
//     m_barContainerManager.Create(this, NULL, NULL);                 // mgr slot 0x28
//     return 0;
// Static dispatch to the CPaneContainerManager export.
// Signature corrected: the generated list dropped `this`.
extern "C" int MS_ABI impl__OnCreate_CMultiPaneFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* lpCreateStruct) {
    if (pThis == nullptr) return -1;
    if (impl__OnCreate_CPaneFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(pThis, lpCreateStruct) == -1) return -1;
    impl__Create_CPaneContainerManager__UEAAHPEAVCWnd__PEAVCPaneDivider__PEAUCRuntimeClass___Z(Mgr(pThis), pThis, nullptr, nullptr);
    return 0;
}

// Symbol: ?OnDockToRecentPos@CMultiPaneFrameWnd@@UEAAXXZ
// Decoded from retail entry RVA 0x946b0:
//     CDockingManager* pDM = m_pDockManager /*0x248*/;
//     if (!pDM) pDM = afxGlobalUtils.GetDockingManager(this);        // call 0x6ccc0
//     CObList lst;  m_barContainerManager.AddPanesToList(&lst, NULL); // call 0xaa960
//     for (each entry) DYNAMIC_DOWNCAST(CDockablePane, entry)->StoreRecentDockSiteInfo();   // pane slot 0x648
//     for (each entry) {
//         CDockablePane* p = DYNAMIC_DOWNCAST(CDockablePane, entry);
//         CPaneFrameWnd::AddRemovePaneFromGlobalList(p, FALSE);      // call 0xaf740
//         p->DockPane(p, NULL, DM_DBL_CLICK /*2*/);                  // pane slot 0x400
//     }
//     afxGlobalUtils.ForceAdjustLayout(pDM, FALSE, FALSE);           // call 0x6d800
//     ::PostMessage(m_hWnd, AFX_WM_CHECKEMPTYMINIFRAME, 0, 0);       // import 0x2c52d8
// Deviations: the 0x248 member is not modelled in OpenMFC's CPaneFrameWnd, so
// the docking manager always comes from afxGlobalUtils.GetDockingManager(this)
// (retail's fallback); NULL downcasts are skipped rather than faulted on;
// static dispatch (slot 0x400 is ?DockPane@CPane@@, 0xa04b0); ForceAdjustLayout
// is a generated placeholder (see its declaration) so that call is a no-op.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__OnDockToRecentPos_CMultiPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CGlobalUtils* pUtils = reinterpret_cast<CGlobalUtils*>(impl__afxGlobalUtils__3VCGlobalUtils__A);
    CDockingManager* pDM = impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(pUtils, AsWnd(pThis));
    {
        VisiblePaneList lst(pThis);
        for (ObNode* n = lst.First(); n != nullptr; n = n->pNext) {
            CDockablePane* p = AsDockable(n->data);
            if (p != nullptr) impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ(p);
        }
        for (ObNode* n = lst.First(); n != nullptr; n = n->pNext) {
            CDockablePane* p = AsDockable(n->data);
            if (p == nullptr) continue;
            impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(p, FALSE);
            impl__DockPane_CPane__UEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
                static_cast<CPane*>(p), static_cast<CBasePane*>(p), nullptr, kDockMethodDblClick);
        }
    }
    impl__ForceAdjustLayout_CGlobalUtils__QEAAXPEAVCDockingManager__HH_Z(pUtils, pDM, FALSE, FALSE);
    HWND hwnd = FrameHwnd(pThis);
    if (hwnd) ::PostMessage(hwnd, impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA, 0, 0);
}

// Symbol: ?OnGetMinMaxInfo@CMultiPaneFrameWnd@@IEAAXPEAUtagMINMAXINFO@@@Z
// Decoded from retail entry RVA 0x94390:
//     CSize size(0, 0);
//     m_barContainerManager.GetMinSize(size);     // mgr slot 0xb0
//     CalcMinSize(size, lpMMI);                   // slot 0x488 -> CPaneFrameWnd::CalcMinSize (0xb2480)
//     CWnd::Default();                            // call 0x289090
// Static dispatch on both slots.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CMultiPaneFrameWnd__IEAAXPEAUtagMINMAXINFO___Z(void* pThis, MINMAXINFO* lpMMI) {
    if (pThis == nullptr) return;
    CSize size(0, 0);
    impl__GetMinSize_CPaneContainerManager__UEAAXAEAVCSize___Z(Mgr(pThis), size);
    impl__CalcMinSize_CPaneFrameWnd__MEAAXAEAVCSize__PEAUtagMINMAXINFO___Z(pThis, &size, lpMMI);
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Symbol: ?OnIdleUpdateCmdUI@CMultiPaneFrameWnd@@IEAA_J_K_J@Z
// Decoded from retail entry RVA 0x94c80:
//     CPaneFrameWnd::OnIdleUpdateCmdUI(wParam, 0);   // call 0xb37d0, %r8d zeroed
//     return 0;
// Signature corrected: the generated list dropped `this`.
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CMultiPaneFrameWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64) {
    if (pThis == nullptr) return 0;
    impl__OnIdleUpdateCmdUI_CPaneFrameWnd__IEAA_J_K_J_Z(pThis, wParam, 0);
    return 0;
}

// Symbol: ?OnKillRollUpTimer@CMultiPaneFrameWnd@@UEAAXXZ
// Decoded from retail entry RVA 0x92d30:
//     CObList lst;  m_barContainerManager.AddPanesToList(&lst, NULL);   // call 0xaa960
//     for (each entry) {
//         CBasePane* p = DYNAMIC_DOWNCAST(CBasePane, entry);             // RTC 0x2da490
//         if (p && (p->GetControlBarStyle() & AFX_CBRS_AUTO_ROLLUP))     // slot 0x398, `test $0x10,%al`
//             return;                                    // (after lst's dtor)
//     }
//     KillRollupTimer();                                                 // call 0xaf630
// Slot 0x398 is the unexported `return m_dwControlBarStyle;` (0x8850); the
// member is read directly (static_assert above).
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__OnKillRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (AnyVisiblePaneAutoRollsUp(pThis)) return;
    impl__KillRollupTimer_CPaneFrameWnd__IEAAXXZ(pThis);
}

// Symbol: ?OnLButtonDblClk@CMultiPaneFrameWnd@@IEAAXIVCPoint@@@Z
// Decoded from retail entry RVA 0x94690: three instructions, a tail jump
// through vftable slot 0x340 -- OnDockToRecentPos (0x946b0).  nFlags and point
// are never read.  Static dispatch.
// Signature corrected: the generated list dropped `this` (CPoint by value is an
// 8-byte aggregate in a register under MS_ABI, modelled as `long long`).
extern "C" void MS_ABI impl__OnLButtonDblClk_CMultiPaneFrameWnd__IEAAXIVCPoint___Z(void* pThis, unsigned int, long long) {
    if (pThis == nullptr) return;
    impl__OnDockToRecentPos_CMultiPaneFrameWnd__UEAAXXZ(pThis);
}

// Symbol: ?OnPaneRecalcLayout@CMultiPaneFrameWnd@@UEAAXXZ
// Decoded from retail entry RVA 0x93160:
//     CRect rectClient;  ::GetClientRect(m_hWnd, &rectClient);            // import 0x2c5358
//     HDWP hdwp = ::BeginDeferWindowPos(20);                             // import 0x2c4d28
//     m_barContainerManager.ResizePaneContainers(rectClient, hdwp);      // mgr slot 0x80
//     ::EndDeferWindowPos(hdwp);                                         // import 0x2c4d30
//     if (CPane::m_bHandleMinSize /*.data 0x3b6fd0*/) {
//         CRect rectAvail;  m_barContainerManager.GetAvailableSpace(rectAvail);   // mgr slot 0x68
//         CRect rectWnd;    ::GetWindowRect(m_hWnd, &rectWnd);                    // import 0x2c5370
//         int cx = max(0, rectAvail.Width()  - rectClient.Width());     // sub/sub/add, cmovns
//         int cy = max(0, rectAvail.Height() - rectClient.Height());
//         if (cx != 0 || cy != 0)
//             CWnd::SetWindowPos(NULL, -1, -1,                           // call 0x2a7970
//                 cx + rectWnd.Width(), cy + rectWnd.Height(),
//                 SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE /*0x16*/);
//     }
// (the two size operands are `cx - rectWnd.left + rectWnd.right` and
// `cy - rectWnd.top + rectWnd.bottom`; with SWP_NOMOVE the -1/-1 position is
// ignored.)  Static dispatch to the CPaneContainerManager exports.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__OnPaneRecalcLayout_CMultiPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    HWND hwnd = FrameHwnd(pThis);
    RECT rectClient = {0, 0, 0, 0};
    if (hwnd) ::GetClientRect(hwnd, &rectClient);
    HDWP hdwp = ::BeginDeferWindowPos(20);
    void* pHdwp = hdwp;
    impl__ResizePaneContainers_CPaneContainerManager__UEAAXVCRect__AEAPEAX_Z(Mgr(pThis), &rectClient, &pHdwp);
    if (pHdwp) ::EndDeferWindowPos(static_cast<HDWP>(pHdwp));
    if (impl__m_bHandleMinSize_CPane__2HA != 0) {
        CRect rectAvail(0, 0, 0, 0);
        impl__GetAvailableSpace_CPaneContainerManager__UEBAXAEAVCRect___Z(Mgr(pThis), rectAvail);
        RECT rectWnd = {0, 0, 0, 0};
        if (hwnd) ::GetWindowRect(hwnd, &rectWnd);
        int cx = (rectAvail.right - rectAvail.left) - (rectClient.right - rectClient.left);
        int cy = (rectAvail.bottom - rectAvail.top) - (rectClient.bottom - rectClient.top);
        if (cx < 0) cx = 0;
        if (cy < 0) cy = 0;
        if (cx != 0 || cy != 0) {
            impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(AsWnd(pThis), nullptr, -1, -1,
                                                       cx + (rectWnd.right - rectWnd.left),
                                                       cy + (rectWnd.bottom - rectWnd.top), kSwpResize);
        }
    }
}

// Symbol: ?OnSetFocus@CMultiPaneFrameWnd@@IEAAXPEAVCWnd@@@Z
// Decoded from retail entry RVA 0x94d50 (pOldWnd is never read; CWnd::OnSetFocus
// is NOT called -- unlike CPaneFrameWnd's handler):
//     CBasePane* pFirst = DYNAMIC_DOWNCAST(CBasePane, GetFirstVisiblePane());  // slot 0x368, RTC 0x2da490
//     if (m_hWndLastFocused /*0x2f8*/ == NULL) {
//         if (pFirst && ::IsWindow(pFirst->m_hWnd) && pFirst-><slot 0x3c0>())
//             pFirst->SetFocus();                                   // call 0x2a7a70
//     } else {
//         CWnd* pLast = CWnd::FromHandlePermanent(m_hWndLastFocused);   // 0x2891d0
//         BOOL b;
//         if (pLast && pLast->IsKindOf(RUNTIME_CLASS(CDockablePane)) &&
//             m_barContainerManager.FindPaneContainer(pLast, b) &&  // mgr slot 0x128
//             ::IsWindow(pLast->m_hWnd))
//             pLast->SetFocus();
//         else if (pFirst && ::IsWindow(pFirst->m_hWnd))            // no slot-0x3c0 test on this path
//             pFirst->SetFocus();
//     }
//     if (GetParentFrame())                                          // call 0x28c680
//         GetParentFrame()->SetWindowPos(&wndTop, 0, 0, 0, 0,        // call 0x2a7970
//                                        SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE /*0x13*/);
// Slot 0x3c0 of the CDockablePane vftable is folded onto `return 1` (0x3ae0)
// and CBasePane's is the same body, so the test is elided here (deviation: a
// derived pane overriding it to FALSE would still receive the focus).  `&wndTop`
// is CWnd::wndTop whose m_hWnd is HWND_TOP == NULL, which is what a NULL
// pWndInsertAfter yields in the CWnd::SetWindowPos export.  Static dispatch.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__OnSetFocus_CMultiPaneFrameWnd__IEAAXPEAVCWnd___Z(void* pThis, CWnd*) {
    if (pThis == nullptr) return;
    CBasePane* pFirst = AsBasePane(impl__GetFirstVisiblePane_CPaneContainerManager__UEBAPEAVCWnd__XZ(Mgr(pThis)));
    HWND hLast = LastFocused(pThis);
    if (hLast == nullptr) {
        if (pFirst != nullptr && ::IsWindow(pFirst->m_hWnd)) {
            impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pFirst));
        }
    } else {
        CDockablePane* pLast = AsDockable(impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hLast));
        int bLeft = 0;
        if (pLast != nullptr &&
            impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(Mgr(pThis), pLast, &bLeft) != nullptr &&
            ::IsWindow(pLast->m_hWnd)) {
            impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pLast));
        } else if (pFirst != nullptr && ::IsWindow(pFirst->m_hWnd)) {
            impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pFirst));
        }
    }
    CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(AsWnd(pThis));
    if (pFrame != nullptr) {
        pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(AsWnd(pThis));   // retail calls it twice
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(static_cast<CWnd*>(pFrame), nullptr, 0, 0, 0, 0, kSwpTop);
    }
}

// Symbol: ?OnSetRollUpTimer@CMultiPaneFrameWnd@@UEAAXXZ
// Decoded from retail entry RVA 0x92c70:
//     CObList lst;  m_barContainerManager.AddPanesToList(&lst, NULL);   // call 0xaa960
//     for (each entry) {
//         CBasePane* p = DYNAMIC_DOWNCAST(CBasePane, entry);             // RTC 0x2da490
//         if (p && (p->GetControlBarStyle() & AFX_CBRS_AUTO_ROLLUP)) {   // slot 0x398, `test $0x10,%al`
//             SetRollUpTimer();                                          // call 0xaf5d0
//             break;
//         }
//     }
// Slot 0x398 is the unexported `return m_dwControlBarStyle;` (0x8850); the
// member is read directly.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__OnSetRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (AnyVisiblePaneAutoRollsUp(pThis)) impl__SetRollUpTimer_CPaneFrameWnd__IEAAXXZ(pThis);
}

// Symbol: ?OnShowPane@CMultiPaneFrameWnd@@UEAAXPEAVCDockablePane@@H@Z
// Decoded from retail entry RVA 0x94400:
//     m_barContainerManager.OnShowPane(pBar, bShow);                 // mgr slot 0xa0
//     if (bShow) {
//         if (!m_bDelayShow /*+0x108*/)
//             CWnd::ShowWindow(SW_SHOWNOACTIVATE /*4*/);              // call 0x2a79e0
//         OnSetRollUpTimer();                                        // slot 0x3f8
//     } else if (!m_barContainerManager.IsRootPaneContainerVisible()) {   // mgr slot 0xa8
//         CWnd::ShowWindow(SW_HIDE);
//         OnKillRollUpTimer();                                       // slot 0x400
//     }
//     CheckGripperVisibility();                                      // slot 0x320
//     OnPaneRecalcLayout();                                          // slot 0x3a8
//     CWnd::SetWindowPos(NULL, 0, 0, 0, 0, 0x37);                    // call 0x2a7970
// Deviation: the m_bDelayShow gate is not modelled.  Across the CPaneFrameWnd /
// CMultiPaneFrameWnd exports, +0x108 is zeroed by the CPaneFrameWnd
// constructor (the store at 0xad77f), read here (the cmpl at 0x94433) and
// written only by the two SetDockState bodies (the stores at 0xb3549 in
// CPaneFrameWnd's, entry 0xb31c0, and 0x941c1 in this class's, entry 0x93bd0);
// the shipping afxpaneframewnd.h also exposes an inline SetDelayShow(), so a
// client or a friend class (CPane, CDockingManager, CMFCBaseTabCtrl,
// CMFCPopupMenu) can write it without an export -- those writers were not
// scanned.  CPaneFrameWnd's SetDockState is a stub in OpenMFC and this
// class's (below) does not make the store because +0x108 has no counterpart
// here, so in this build the DLL itself never sets the flag and the
// ShowWindow(SW_SHOWNOACTIVATE) is made unconditionally.  Static dispatch.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__OnShowPane_CMultiPaneFrameWnd__UEAAXPEAVCDockablePane__H_Z(void* pThis, void* pBar, int bShow) {
    if (pThis == nullptr) return;
    impl__OnShowPane_CPaneContainerManager__UEAAHPEAVCDockablePane__H_Z(Mgr(pThis), pBar, bShow);
    if (bShow) {
        impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pThis), SW_SHOWNOACTIVATE);
        impl__OnSetRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(pThis);
    } else if (!impl__IsRootPaneContainerVisible_CPaneContainerManager__UEBAHXZ(Mgr(pThis))) {
        impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pThis), SW_HIDE);
        impl__OnKillRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(pThis);
    }
    impl__CheckGripperVisibility_CMultiPaneFrameWnd__UEAAXXZ(pThis);
    impl__OnPaneRecalcLayout_CMultiPaneFrameWnd__UEAAXXZ(pThis);
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(AsWnd(pThis), nullptr, 0, 0, 0, 0, kSwpFrameChanged);
}

// Symbol: ?OnSize@CMultiPaneFrameWnd@@IEAAXIHH@Z
// Decoded from retail entry RVA 0x93110 (nType/cx/cy are never read):
//     CWnd::Default();                                     // call 0x289090
//     OnPaneRecalcLayout();                                // slot 0x3a8
//     ArrangeCaptionButtons();                             // call 0xb1990
//     ::SendMessage(m_hWnd, WM_NCPAINT /*0x85*/, 0, 0);    // tail, import 0x2c5378
// Static dispatch on slot 0x3a8.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__OnSize_CMultiPaneFrameWnd__IEAAXIHH_Z(void* pThis, unsigned int, int, int) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
    impl__OnPaneRecalcLayout_CMultiPaneFrameWnd__UEAAXXZ(pThis);
    impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ(pThis);
    HWND hwnd = FrameHwnd(pThis);
    if (hwnd) ::SendMessage(hwnd, WM_NCPAINT, 0, 0);
}

// Symbol: ?OnSizing@CMultiPaneFrameWnd@@IEAAXIPEAUtagRECT@@@Z
// This export has no entry of its own in either RVA map.  The class's message
// map (mfc140u: GetMessageMap at 0x91f60 returns .rdata 0x2f06a8, entries at
// 0x2f06c0) routes WM_SIZING (0x214) to the body at 0xda30 (mfc140u), which is
// a single `jmp` onto ?Default@CWnd@@IEAA_JXZ (0x28ac80 in mfc140u) -- i.e.
// CWnd::OnSizing's `Default()`, ICF-folded.  nSide and lpRect are never read.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__OnSizing_CMultiPaneFrameWnd__IEAAXIPEAUtagRECT___Z(void* pThis, unsigned int, RECT*) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Symbol: ?PaneFromPoint@CMultiPaneFrameWnd@@UEAAPEAVCBasePane@@VCPoint@@HH@Z
// Decoded from retail entry RVA 0x94b50:
//     if (bCheckVisibility && !::IsWindowVisible(m_hWnd)) return NULL;   // import 0x2c5350
//     BOOL bIsTabArea = FALSE, bCaption = FALSE;
//     return m_barContainerManager.PaneFromPoint(point, nSensitivity,   // mgr slot 0xe0
//                                                TRUE /*bExactBar*/, bIsTabArea, bCaption);
// Static dispatch.  CPoint by value is an 8-byte aggregate in %rdx (modelled as
// `long long`, forwarded as the export's `void* point` register image).
// Signature corrected: the generated list dropped `this`.
extern "C" void* MS_ABI impl__PaneFromPoint_CMultiPaneFrameWnd__UEAAPEAVCBasePane__VCPoint__HH_Z(
    void* pThis, long long point, int nSensitivity, int bCheckVisibility) {
    if (pThis == nullptr) return nullptr;
    if (bCheckVisibility) {
        HWND hwnd = FrameHwnd(pThis);
        if (hwnd == nullptr || !::IsWindowVisible(hwnd)) return nullptr;
    }
    int bIsTabArea = FALSE;
    int bCaption = FALSE;
    void* pointImage = nullptr;
    std::memcpy(&pointImage, &point, sizeof(pointImage));
    return impl__PaneFromPoint_CPaneContainerManager__UEAAPEAVCDockablePane__VCPoint__HHAEAH1_Z(
        Mgr(pThis), pointImage, nSensitivity, TRUE, &bIsTabArea, &bCaption);
}

// Symbol: ?RemoveNonValidPanes@CMultiPaneFrameWnd@@UEAAXXZ
// Decoded from retail entry RVA 0x94d30:
//     if (m_barContainerManager.m_pRootContainer)          // +0x2d0
//         m_barContainerManager.m_pRootContainer->RemoveNonValidPanes();   // call 0xa7ec0
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__RemoveNonValidPanes_CMultiPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    void* pRoot = MgrRootContainer(pThis);
    if (pRoot != nullptr) impl__RemoveNonValidPanes_CPaneContainer__QEAAXXZ(pRoot);
}

// Symbol: ?ReplacePane@CMultiPaneFrameWnd@@UEAAXPEAVCBasePane@@0@Z
// Decoded from retail entry RVA 0x94ca0:
//     m_barContainerManager.ReplacePane(pBarOrg, pBarReplaceWith);   // mgr slot 0x90
//     OnSetRollUpTimer();                                            // tail, slot 0x3f8
// Static dispatch on both slots.
// Signature corrected: the generated list dropped `this` (CDockablePane.cpp
// already calls this export with (this, pOld, pNew)).
extern "C" void MS_ABI impl__ReplacePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__0_Z(void* pThis, void* pBarOrg, void* pBarReplaceWith) {
    if (pThis == nullptr) return;
    impl__ReplacePane_CPaneContainerManager__UEAAHPEAVCDockablePane__0_Z(Mgr(pThis), pBarOrg, pBarReplaceWith);
    impl__OnSetRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(pThis);
}

// Symbol: ?SaveRecentFloatingState@CMultiPaneFrameWnd@@MEAAXXZ
// Decoded from retail entry RVA 0x94810:
//     ::GetWindowRect(m_hWnd, &m_rectRecentFloatingRect /*0x150*/);   // import 0x2c5370
//     CObList lst;  m_barContainerManager.AddPanesToList(&lst, NULL);  // call 0xaa960
//     for (each entry) {
//         CDockablePane* p = DYNAMIC_DOWNCAST(CDockablePane, entry);   // RTC 0x3aa178
//         p->m_recentDockInfo.m_rectRecentFloatingRect /*pane +0x2c0*/  // 16-byte movups/movdqu
//             = m_rectRecentFloatingRect;
//     }
// 0x150 has no member here: the frame half is delegated to the
// CPaneFrameWnd::SaveRecentFloatingState export (docking/CPaneFrameWnd.cpp:2042),
// whose whole body is this same ::GetWindowRect into that file's stand-in for
// 0x150 (g_framePanes[this].recentRect); the rectangle is re-read here for the
// pane-side stores, which land at their retail offset inside
// CPane::m_recentDockInfo (static_asserts above) -- nothing in this repo reads
// them back yet.  Deviation: NULL downcasts are skipped rather than faulted on.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__SaveRecentFloatingState_CMultiPaneFrameWnd__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__SaveRecentFloatingState_CPaneFrameWnd__MEAAXXZ(pThis);
    RECT wr = {0, 0, 0, 0};
    HWND hwnd = FrameHwnd(pThis);
    if (hwnd) ::GetWindowRect(hwnd, &wr);
    VisiblePaneList lst(pThis);
    for (ObNode* n = lst.First(); n != nullptr; n = n->pNext) {
        CDockablePane* p = AsDockable(n->data);
        if (p == nullptr) continue;
        std::memcpy(reinterpret_cast<char*>(&static_cast<CPane*>(p)->m_recentDockInfo) + kOffRecentFloatingRectInDockInfo,
                    &wr, sizeof(RECT));
    }
}

// Symbol: ?SaveState@CMultiPaneFrameWnd@@UEAAHPEB_WI@Z
// Decoded from retail ?SaveState@CMultiPaneFrameWnd@@UEAAHPEBDI@Z, entry RVA
// 0x93ab0 (slot 0x3b8 of the vftable at 0x2ee738).  Same shape as LoadState:
// walks m_lstControlBars (head +0x268) and for every entry
//     DYNAMIC_DOWNCAST(CDockablePane, entry)->SaveState(lpszProfileName, uiID, (UINT)-1);   // pane slot 0x470
// then returns TRUE.  Slot 0x470 is a jmp thunk onto ?SaveState@CPane@@
// (0xa24e0).  Deviations: retail walks the raw list, this walks the
// AddPanesToList copy (panes without WS_VISIBLE are not saved -- see LIST
// REPRESENTATION); NULL downcasts are skipped; static dispatch to the CPane
// export (docking/CPane.cpp:1791, a real body).
// Signature corrected: the generated list dropped `this`.
extern "C" int MS_ABI impl__SaveState_CMultiPaneFrameWnd__UEAAHPEB_WI_Z(void* pThis, const wchar_t* lpszProfileName, unsigned int uiID) {
    if (pThis == nullptr) return FALSE;
    VisiblePaneList lst(pThis);
    for (ObNode* n = lst.First(); n != nullptr; n = n->pNext) {
        CDockablePane* p = AsDockable(n->data);
        if (p == nullptr) continue;
        impl__SaveState_CPane__UEAAHPEB_WHI_Z(static_cast<CPane*>(p), lpszProfileName, static_cast<int>(uiID), static_cast<unsigned int>(-1));
    }
    return TRUE;
}

// Symbol: ?Serialize@CMultiPaneFrameWnd@@UEAAXAEAVCArchive@@@Z
// Decoded from retail entry RVA 0x93b90:
//     CPaneFrameWnd::Serialize(ar);            // call 0xb2d20
//     m_barContainerManager.Serialize(ar);     // tail, mgr slot 0x10
// Both callees are documented stubs in OpenMFC (see their declarations), so
// this transcription currently serializes nothing; static dispatch.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__Serialize_CMultiPaneFrameWnd__UEAAXAEAVCArchive___Z(void* pThis, void* pArchive) {
    if (pThis == nullptr) return;
    impl__Serialize_CPaneFrameWnd__UEAAXAEAVCArchive___Z(pThis, pArchive);
    impl__Serialize_CPaneContainerManager__UEAAXAEAVCArchive___Z(Mgr(pThis), pArchive);
}

// Symbol: ?SetDockState@CMultiPaneFrameWnd@@UEAAXPEAVCDockingManager@@@Z
// Decoded from retail entry RVA 0x93bd0 (mfc140.dll; the body runs to the
// `ret` at 0x94388, 0x94389 is its ENSURE throw and 0x94390 is the next
// export).  Read in full:
//     CObList lstBarsToRemove;                                          // vftable 0x328538
//     if (m_barContainerManager.m_lstControlBars.m_nCount == 0) {       // cmpq 0x278
//         ::PostMessage(m_hWnd, AFX_WM_CHECKEMPTYMINIFRAME, 0, 0);       // import 0x2c52d8, .data 0x3bae78
//         return;
//     }
//     for (each entry of m_lstControlBars, head at +0x268) {
//         CDockablePane* pBar = DYNAMIC_DOWNCAST(CDockablePane, entry);  // RTC 0x3aa178
//         if (pBar->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) {          // RTC 0x2db168
//             BOOL bLeft = FALSE;
//             CPaneContainer* pC = m_barContainerManager.FindPaneContainer(pBar, bLeft);   // mgr slot 0x128
//             if (pC == NULL) AfxThrowInvalidArgException();             // 0x94389 -> 0x225b80
//             const CList<UINT,UINT>* pIDs =                             // inlined GetAssociatedSiblingPaneIDs:
//                 pBar == pC->m_pLeftPane  /*+0x08*/ ? (CList*)((char*)pC + 0x88) :
//                 pBar == pC->m_pRightPane /*+0x10*/ ? (CList*)((char*)pC + 0xc0) : NULL;
//             if (pIDs) for (node = pIDs->m_pNodeHead /*+0x08*/; node; node = node->pNext) {
//                 CBasePane* pSib = pDockManager->FindPaneByID(node->value /*+0x10*/, TRUE);   // dock-manager slot 0x58
//                 if (pSib == NULL) continue;
//                 if (pSib->IsKindOf(RUNTIME_CLASS(CDockablePane)) && pSib->IsAutoHideMode())    // slot 0x3c8
//                     pSib->SetAutoHideMode(FALSE, CBRS_ALIGN_ANY /*0xf000*/, NULL, TRUE);       // slot 0x6e0
//                 if (pSib->IsTabbed()) {                                // slot 0x2e8 (CPane::IsTabbed, 0xa2100)
//                     CWnd* pTabbed = pSib->GetParent()->GetParent();    // GetParent/FromHandle twice
//                     pSib->SetParent(this);                             // ::SetParent 0x2c52b0, then FromHandle
//                     ((CBaseTabbedPane*)pTabbed)->RemovePane(pSib);     // slot 0x790 (0x13710)
//                 } else
//                     pSib->FloatPane(CRect(0, 0, 10, 10), DM_SHOW /*3*/, false);   // slot 0x408, .rdata 0x348df0
//                 CPaneFrameWnd* pMini = pSib->GetParentMiniFrame(FALSE);            // slot 0x460
//                 if (pMini && pMini != this) pMini->RemovePane(pSib, FALSE, FALSE); // slot 0x308
//                 pSib->AttachToTabWnd(pBar, DM_UNKNOWN /*0*/, FALSE, NULL);         // slot 0x6a8
//                 if (pMini) ::PostMessage(pMini->m_hWnd, AFX_WM_CHECKEMPTYMINIFRAME, 0, 0);
//             }
//             if (((CBaseTabbedPane*)pBar)->GetTabsNum() == 0)          // slot 0x758 (0x12eb0, see below)
//                 lstBarsToRemove.AddTail(pBar);                         // 0x230490
//             else {
//                 ((CBaseTabbedPane*)pBar)->ApplyRestoredTabInfo(FALSE); // slot 0x7b8 (0x14030)
//                 pBar->RecalcLayout();                                  // slot 0x430
//             }
//         } else {
//             if (pBar->IsTabbed()) {                                    // slot 0x2e8
//                 CWnd* pTabbed = pBar->GetParent()->GetParent();
//                 CWnd* pOwner = CWnd::FromHandlePermanent(m_hParentWnd /*0x130*/);   // 0x2891d0
//                 ::SetParent(pBar->m_hWnd, pOwner ? pOwner->m_hWnd : NULL); (FromHandle of the result)
//                 ((CBaseTabbedPane*)pTabbed)->RemovePane(pBar);         // slot 0x790
//                 if (pBar->IsKindOf(RUNTIME_CLASS(CDockablePane)))
//                     pBar->EnableGripper(TRUE);                         // slot 0x3f0
//                 pBar->ShowWindow(SW_SHOW /*5*/);                       // 0x2a79e0
//             }
//             if (pBar->IsAutoHideMode())                                // slot 0x3c8
//                 pBar->SetAutoHideMode(FALSE, CBRS_ALIGN_ANY, NULL, TRUE);   // slot 0x6e0
//             CRect rectWnd; ::GetWindowRect(pBar->m_hWnd, rectWnd);      // import 0x2c5370
//             pBar->FloatPane(rectWnd, DM_SHOW, false);                   // slot 0x408
//             CPaneFrameWnd* pMini = pBar->GetParentMiniFrame(FALSE);     // slot 0x460
//             if (pMini) {
//                 pBar->SetParent(this);                                  // ::SetParent + FromHandle
//                 pMini->RemovePane(pBar, FALSE, FALSE);                  // slot 0x308
//                 pBar->SetWindowPos(NULL, m_rectSavedDockedRect.left, .top,         // slot 0x480, pane +0x3d8
//                                    .Width(), .Height(),
//                                    SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED /*0x34*/, NULL);
//             }
//         }
//     }
//     for (each pBar in lstBarsToRemove) {
//         RemovePane(DYNAMIC_DOWNCAST(CDockablePane, pBar), FALSE, TRUE); // own slot 0x308
//         pBar->DestroyWindow();                                          // slot 0xd0
//     }
//     if (m_lstControlBars.m_nCount == 0) {                               // cmpq 0x278
//         ::SendMessage(m_hWnd, AFX_WM_CHECKEMPTYMINIFRAME, 0, 0);         // import 0x2c5378
//         return;
//     }
//     for (each entry of m_lstControlBars) {
//         CDockablePane* pBar = DYNAMIC_DOWNCAST(CDockablePane, entry);
//         BOOL bShow = pBar->GetRecentVisibleState();                     // slot 0x368 (0x87f0, +0xec)
//         CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;          // 0x1345b0, +0x08
//         BOOL bExitingFullScreen = pApp && pApp->IsKindOf(RUNTIME_CLASS(CWinAppEx)) /*0x31e618*/
//                                   && ((CWinAppEx*)pApp)->m_bExitingFullScreenMode /*+0x17c*/;
//         if (bShow && !bExitingFullScreen) m_bDelayShow /*+0x108*/ = TRUE;
//         pBar->ShowPane(bShow, !bExitingFullScreen, FALSE);              // slot 0x458
//         CPaneFrameWnd::AddRemovePaneFromGlobalList(pBar, TRUE);        // 0xaf740
//     }
//     CBasePane* pFirst = (CBasePane*)m_lstControlBars.m_pNodeHead->data;
//     if (pFirst && pFirst->IsKindOf(RUNTIME_CLASS(CBasePane))) {         // RTC 0x2da490
//         if (m_lstControlBars.m_nCount > 1)
//             m_hEmbeddedBar /*0x148*/ = pFirst->m_hWnd;
//         else {
//             CString strText; pFirst->GetWindowText(strText);            // 0x28a280
//             SetWindowText(strText);                                     // 0x2a76a0
//             ::SendMessage(m_hWnd, WM_SETICON, ICON_SMALL,               // 0x80 / 0
//                 ::SendMessage(pFirst->m_hWnd, WM_GETICON, ICON_SMALL, 0));   // 0x7f
//             ::SendMessage(m_hWnd, WM_SETICON, ICON_BIG,                 // 1
//                 ::SendMessage(pFirst->m_hWnd, WM_GETICON, ICON_BIG, 0));
//         }
//     }
//     OnSetRollUpTimer();                                                 // slot 0x3f8
//     SetCaptionButtons(m_dwCaptionButtons /*0x12c*/);                    // slot 0x328 -> CPaneFrameWnd::SetCaptionButtons (0xb13f0)
//     OnPaneRecalcLayout();                                               // slot 0x3a8
// (lstBarsToRemove is torn down through CPtrList::RemoveAll, 0x83d0.)
// Slot 0x758 is the unexported CBaseTabbedPane virtual at 0x12eb0,
// `return m_pTabWnd /*+0x4e8*/ ? m_pTabWnd->slot 0x368() : 0`, and slot 0x368
// of the tab window's vftable (read off CMFCTabCtrl's, 0x310520, the address
// ??0CMFCTabCtrl@@QEAA@XZ at 0x137a00 stores at +0) is 0x15370, `return
// m_iTabsNum /*+0x150*/;` -- i.e. the shipping afxbasetabbedpane.h's inline
// GetTabsNum() over CMFCBaseTabCtrl::GetTabsNum().  The CWinAppEx member at +0x17c is
// m_bExitingFullScreenMode: the shipping afxwinappex.h member order laid over
// ??0CWinAppEx@@QEAA@H@Z (entry 0x1c59d0) puts m_bLoadUserToolbars / it at
// 0x178 / 0x17c (`movq $0x1,0x178(%rsi)` initialises the pair TRUE/FALSE) with
// CString m_strRegSection following at 0x180.
//
// Deviations, each forced by what this build can reach through exports:
//   * LIST WALKS go through the AddPanesToList copy (VisiblePaneList), so
//     panes without WS_VISIBLE are neither re-parented nor re-shown, and the
//     "m_nCount > 1" test counts that copy -- see LIST REPRESENTATION at the
//     top of this file.  pFirst is the GetFirstPane export (the raw head).
//   * GetTabsNum() cannot be read: m_pTabWnd (+0x4e8) has no storage in
//     OpenMFC's CBaseTabbedPane and neither it nor CMFCBaseTabCtrl::GetTabsNum
//     is an export.  The "tabs remain" arm is taken unconditionally -- the
//     conservative direction (the other arm destroys a client's window), and
//     the same call docking/CPaneFrameWnd.cpp's RemoveNonValidPanes makes for
//     this virtual.  Both callees on that arm are placeholders today.
//   * m_bExitingFullScreenMode has no storage in OpenMFC's CWinAppEx (a
//     padding blob) and nothing in this tree sets it, so it is taken as FALSE:
//     bDelay is always TRUE and the AfxGetApp/IsKindOf(CWinAppEx) pair, whose
//     only use is that read, is not made.
//   * the m_bDelayShow store (+0x108 is inside OpenMFC's live CWnd sub-object,
//     no side-table counterpart) is not made; OnShowPane above does not read
//     the flag either, so the two stay consistent.
//   * the m_hEmbeddedBar store follows RemovePane above: the CPaneFrameWnd
//     AddPane export stands in for the 0x148 write.  That export is
//     AddUniquePane on docking/CPaneFrameWnd.cpp's g_framePanes list plus a
//     `rollUp = FALSE` store, so unlike retail's overwrite it (a) does not make
//     pFirst the GetPane() result when that list already holds another pane
//     (it is appended, not moved to the head) and (b) clears the side-table
//     roll-up flag; retail's plain HWND store does neither.
//   * m_hParentWnd (0x130) has no counterpart reachable from here; the
//     owner is taken as ::GetParent(m_hWnd) (for a WS_POPUP frame that is the
//     window it was created with, which is what CPaneFrameWnd::CreateEx
//     stores at 0x130) and, as in retail, passed through FromHandlePermanent.
//   * SetCaptionButtons(m_dwCaptionButtons) is not re-issued: the mask lives
//     in docking/CPaneFrameWnd.cpp's side table with no reader export.
//   * the ENSURE on FindPaneContainer (0x94389 -> AfxThrowInvalidArgException)
//     is NOT reproduced.  OpenMFC's FindPaneContainer is a g_paneToContainer
//     lookup that only the CPaneContainer constructor, CPaneContainer::AddPane
//     and CPaneContainerManager::ReplacePane populate
//     (docking/CPaneContainer.cpp:393/:407, CPaneContainerManager.cpp:1998),
//     while docking/CPaneContainerManager.cpp's AddPane export (:326) only
//     records the pane in its side table and creates no container.  So every
//     CBaseTabbedPane that reached this frame through AddPane has no container
//     here, a state retail cannot produce (its AddPane, 0xa82c0, always builds
//     the root container), and throwing would abort a client's dock-state
//     restore on a healthy OpenMFC object.  A NULL container is treated as an
//     empty sibling-ID list instead (retail's own behaviour once
//     GetAssociatedSiblingPaneIDs returns NULL).
//   * the FromHandle wrappers retail creates for ::SetParent's return value
//     are not made; NULL downcasts, a NULL pDockManager and a missing parent
//     chain are skipped where retail would fault; all virtuals are dispatched
//     statically (MiniFrameRemovePane picks the mini-frame override by class).
//     CBaseTabbedPane::RemovePane, ApplyRestoredTabInfo, RecalcLayout,
//     FloatPane and AttachToTabWnd are placeholders/stubs (see their
//     declarations) but are called where retail calls them.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__SetDockState_CMultiPaneFrameWnd__UEAAXPEAVCDockingManager___Z(void* pThis, void* pDockManager) {
    if (pThis == nullptr) return;
    HWND hwndThis = FrameHwnd(pThis);
    if (ManagerIsEmpty(pThis)) {
        if (hwndThis) ::PostMessage(hwndThis, impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA, 0, 0);
        return;
    }
    CRuntimeClass* pTabbedClass = impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();
    std::vector<void*> lstBarsToRemove;
    {
        VisiblePaneList lst(pThis);
        for (ObNode* n = lst.First(); n != nullptr; n = n->pNext) {
            CDockablePane* pBar = AsDockable(n->data);
            if (pBar == nullptr) continue;
            CWnd* pBarWnd = AsWnd(pBar);
            if (DowncastTo(pBar, pTabbedClass) != nullptr) {
                int bLeft = FALSE;
                void* pContainer = impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(Mgr(pThis), pBar, &bLeft);
                // Retail: ENSURE(pContainer) -> AfxThrowInvalidArgException.  Not
                // thrown here -- see the "ENSURE" deviation above: in this build
                // a tabbed pane added through AddPane has no container, so the
                // guard would fire on a state that is healthy for OpenMFC.  A
                // missing container is treated as an empty sibling-ID list.
                void* pIDs = pContainer != nullptr
                    ? impl__GetAssociatedSiblingPaneIDs_CPaneContainer__QEAAPEAV__CList_II__PEAVCDockablePane___Z(pContainer, pBar)
                    : nullptr;
                for (UintNode* id = UintListHead(pIDs); id != nullptr; id = id->pNext) {
                    if (pDockManager == nullptr) break;
                    CBasePane* pSibBase = impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
                        static_cast<CDockingManager*>(pDockManager), id->value, TRUE);
                    if (pSibBase == nullptr) continue;
                    CDockablePane* pSib = AsDockable(pSibBase);
                    if (pSib != nullptr && PaneIsAutoHideMode(pSib))
                        impl__SetAutoHideMode_CDockablePane__UEAAPEAVCMFCAutoHideBar__HKPEAV2_H_Z(pSib, FALSE, kCbrsAlignAnyRetail, nullptr, TRUE);
                    // Retail dispatches the pane's own vtable here whatever its
                    // class; only the CPane exports exist, so a sibling that is
                    // not a CDockablePane is left where it is.
                    if (pSib == nullptr) continue;
                    CWnd* pSibWnd = AsWnd(pSib);
                    if (impl__IsTabbed_CPane__UEBAHXZ(static_cast<const CPane*>(pSib))) {
                        CWnd* pTabbed = GrandParentWnd(pSibWnd->m_hWnd);
                        ::SetParent(pSibWnd->m_hWnd, hwndThis);
                        if (pTabbed != nullptr) impl__RemovePane_CBaseTabbedPane__UEAAHPEAVCWnd___Z(pTabbed, pSib);
                    } else {
                        RECT rectDummy = {0, 0, 10, 10};
                        impl__FloatPane_CPane__UEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(static_cast<CPane*>(pSib), &rectDummy, kDockMethodShow, false);
                    }
                    void* pMini = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(static_cast<const CBasePane*>(pSib), FALSE);
                    if (pMini != nullptr && pMini != pThis) MiniFrameRemovePane(pMini, pSib, FALSE, FALSE);
                    impl__AttachToTabWnd_CDockablePane__UEAAPEAV1_PEAV1_W4AFX_DOCK_METHOD__HPEAPEAV1__Z(pSib, pBar, kDockMethodUnknown, FALSE, nullptr);
                    if (pMini != nullptr) {
                        HWND hMini = AsWnd(pMini)->m_hWnd;
                        if (hMini) ::PostMessage(hMini, impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA, 0, 0);
                    }
                }
                // GetTabsNum() == 0 is not decidable here (see the deviations
                // above); the "tabs remain" arm is taken.
                impl__ApplyRestoredTabInfo_CBaseTabbedPane__UEAAXH_Z(pBar, FALSE);
                impl__RecalcLayout_CBaseTabbedPane__UEAAXXZ(pBar);
            } else {
                if (impl__IsTabbed_CPane__UEBAHXZ(static_cast<const CPane*>(pBar))) {
                    CWnd* pTabbed = GrandParentWnd(pBarWnd->m_hWnd);
                    CWnd* pOwner = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwndThis ? ::GetParent(hwndThis) : nullptr);
                    ::SetParent(pBarWnd->m_hWnd, pOwner ? pOwner->m_hWnd : nullptr);
                    if (pTabbed != nullptr) impl__RemovePane_CBaseTabbedPane__UEAAHPEAVCWnd___Z(pTabbed, pBar);
                    impl__EnableGripper_CDockablePane__UEAAXH_Z(pBar, TRUE);   // pBar is a CDockablePane by construction
                    impl__ShowWindow_CWnd__QEAAHH_Z(pBarWnd, SW_SHOW);
                }
                if (PaneIsAutoHideMode(pBar))
                    impl__SetAutoHideMode_CDockablePane__UEAAPEAVCMFCAutoHideBar__HKPEAV2_H_Z(pBar, FALSE, kCbrsAlignAnyRetail, nullptr, TRUE);
                RECT rectWnd = {0, 0, 0, 0};
                ::GetWindowRect(pBarWnd->m_hWnd, &rectWnd);
                impl__FloatPane_CPane__UEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(static_cast<CPane*>(pBar), &rectWnd, kDockMethodShow, false);
                void* pMini = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(static_cast<const CBasePane*>(pBar), FALSE);
                if (pMini != nullptr) {
                    ::SetParent(pBarWnd->m_hWnd, hwndThis);
                    MiniFrameRemovePane(pMini, pBar, FALSE, FALSE);
                    const CRect& saved = static_cast<CPane*>(pBar)->m_rectSavedDockedRect;
                    impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
                        static_cast<CBasePane*>(pBar), nullptr, saved.left, saved.top,
                        saved.right - saved.left, saved.bottom - saved.top, kSwpRestoreDocked, nullptr);
                }
            }
        }
    }
    // Empty in this build -- only the undecidable GetTabsNum() == 0 arm above
    // appends to it -- but kept so the retail sequence stays visible.
    for (void* pBar : lstBarsToRemove) {
        CDockablePane* p = AsDockable(pBar);
        impl__RemovePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(pThis, p, FALSE, TRUE);
        if (p != nullptr) impl__DestroyWindow_CWnd__UEAAHXZ(AsWnd(p));
    }
    if (ManagerIsEmpty(pThis)) {
        if (hwndThis) ::SendMessage(hwndThis, impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA, 0, 0);
        return;
    }
    int nCount = 0;
    {
        VisiblePaneList lst(pThis);
        for (ObNode* n = lst.First(); n != nullptr; n = n->pNext) {
            ++nCount;
            CDockablePane* pBar = AsDockable(n->data);
            if (pBar == nullptr) continue;
            const BOOL bShow = static_cast<CBasePane*>(pBar)->m_bRecentVisibleState;
            // bExitingFullScreen == FALSE in this build (see above): m_bDelayShow
            // would be set whenever bShow is, and bDelay is TRUE.
            impl__ShowPane_CDockablePane__UEAAXHHH_Z(pBar, bShow, TRUE, FALSE);
            impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(pBar, TRUE);
        }
    }
    CBasePane* pFirst = AsBasePane(impl__GetFirstPane_CPaneContainerManager__UEBAPEAVCBasePane__XZ(Mgr(pThis)));
    if (pFirst != nullptr) {
        CWnd* pFirstWnd = AsWnd(pFirst);
        if (nCount > 1) {
            impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(pThis, pFirst);
        } else {
            CString strText;
            impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pFirstWnd, &strText);
            impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(AsWnd(pThis), static_cast<const wchar_t*>(strText));
            if (hwndThis && pFirstWnd->m_hWnd) {
                LRESULT hSmall = ::SendMessage(pFirstWnd->m_hWnd, WM_GETICON, ICON_SMALL, 0);
                ::SendMessage(hwndThis, WM_SETICON, ICON_SMALL, hSmall);
                LRESULT hBig = ::SendMessage(pFirstWnd->m_hWnd, WM_GETICON, ICON_BIG, 0);
                ::SendMessage(hwndThis, WM_SETICON, ICON_BIG, hBig);
            }
        }
    }
    impl__OnSetRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(pThis);
    impl__OnPaneRecalcLayout_CMultiPaneFrameWnd__UEAAXXZ(pThis);
}

// Symbol: ?SetPreDockState@CMultiPaneFrameWnd@@UEAAHW4AFX_PREDOCK_STATE@@PEAVCBasePane@@W4AFX_DOCK_METHOD@@@Z
// STUB.  Retail entry RVA 0x933f0, ~430 instructions.  Read in full:
//   * `if (preDockState == PDS_NOTHING /*0*/) return TRUE;`; for
//     PDS_DOCK_REGULAR (2) it also returns TRUE unless pBarToDock's slot 0x328
//     is non-zero; pBarToDock is then DYNAMIC_DOWNCAST to CDockablePane.
//   * needs the docking manager: m_pDockManager (0x248) or, when NULL,
//     afxGlobalUtils.GetDockingManager(FromHandlePermanent(m_hParentWnd /*0x130*/))
//     (0x6ccc0) -- NULL means return TRUE.
//   * snapshots the visible panes (AddPanesToList, 0xaa960) and, for every
//     CDockablePane among them, calls StoreRecentDockSiteInfo (slot 0x648) and
//     records its HWND in a second list.
//   * when pBarToDock is non-NULL: pFirst = m_barContainerManager.GetFirstPane()
//     (slot 0xf0, NULL -> return TRUE); CanAcceptPane both ways between
//     pBarToDock and pFirst (slot 0x318, either FALSE -> return TRUE); when
//     pFirst is hidden, is the only pane (+0x278 == 1) and its GetDockingMode
//     (slot 0x380) & 2 (DT_STANDARD), ModifyStyle (0x2a7600) adds WS_VISIBLE to
//     it; then for DM_MOUSE (1)
//     CGlobalUtils::CheckAlignment (0x6c990, with CDockingManager::
//     m_nDockSensitivity at 0x3aaacc) followed by pBarToDock->
//     DockPaneContainer(m_barContainerManager, dwAlignment, DM_MOUSE) (slot
//     0x6c0), each failure returning TRUE; for DM_DBL_CLICK (2) every visible
//     pane's AttachToTabWnd(pBarToDock, ...) (slot 0x6a8) after
//     AddRemovePaneFromGlobalList(pane, FALSE), then ShowWindow(SW_HIDE),
//     SetWindowPos to the 0x150 rectangle, OnCancelMode (0xb1e80),
//     ::SendMessage(AFX_WM_CHECKEMPTYMINIFRAME) and return TRUE; any other
//     method returns TRUE.
//   * when pBarToDock is NULL: CDockingManager::IsPointNearDockSite (0x48160)
//     must succeed; CDockablePane::CreateDefaultPaneDivider (0x43110) with the
//     owner window; the divider's slot 0x3d0 (CBasePane::SetPaneAlignment,
//     unexported 0x8890); the manager's GetWindowRect (slot 0x68); the docking
//     manager's slot 0xc8; ResizePaneContainers(rect, hdwp) (mgr slot 0x80);
//     then CDockingManager::AddPane (0x476b0) and the divider's slot 0x528 in
//     an order that depends on the alignment.
//   * a completed dock then ::SendMessage(m_hWnd, AFX_WM_CHECKEMPTYMINIFRAME),
//     SetWindowPos to the 0x150 rectangle when the frame window still exists,
//     for DM_MOUSE re-enters drag mode on the first visible pane (slot 0x638)
//     or synthesises WM_LBUTTONDOWN at the cursor, RecalcLayout (slot 0x430) on
//     the target or AdjustDockingLayout (slot 0x4e0) on the first pane,
//     OnSetRollUpTimer, slot 0x6c8 on every recorded pane that is still a
//     permanent CDockablePane, and returns FALSE.
// Not reproduced: CheckAlignment and IsPointNearDockSite are generated
// placeholders (core/runtime/CGlobalUtils.cpp:161, docking/CDockingManager.cpp:879),
// DockPaneContainer is a documented stub (docking/CDockablePane.cpp:1155), no
// CPaneDivider in this build has a retail-shaped vtable, and the 0x248/0x130
// members are not modelled -- so neither docking arm could be taken faithfully.
// TRUE is retail's "nothing was docked" value on every early-out.
// Signature corrected: the generated 29-parameter list was garbage.
extern "C" int MS_ABI impl__SetPreDockState_CMultiPaneFrameWnd__UEAAHW4AFX_PREDOCK_STATE__PEAVCBasePane__W4AFX_DOCK_METHOD___Z(
    void* pThis, int preDockState, void* pBarToDock, int dockMethod) {
    (void)pThis;
    (void)preDockState;
    (void)pBarToDock;
    (void)dockMethod;
    return TRUE;
}

// Symbol: ?StoreRecentDockSiteInfo@CMultiPaneFrameWnd@@UEAAXPEAVCPane@@@Z
// Decoded from retail entry RVA 0x948d0:
//     if (pBar == NULL) return;
//     if (pBar->IsKindOf(RUNTIME_CLASS(CDockablePane))) {                      // RTC 0x3aa178
//         BOOL bLeft = TRUE;
//         CPaneContainer* pC = m_barContainerManager.FindPaneContainer(pBar, bLeft);   // mgr slot 0x128
//         pBar->m_recentDockInfo.StoreDockInfo(pC, NULL);                       // +0x2b8, slot 0x28
//     }
// Static dispatch; the CRecentDockSiteInfo export is a placeholder (see its
// declaration), so the container is looked up but nothing is stored yet.
// Signature corrected: the generated list dropped `this` (CDockablePane.cpp
// already calls this export with (this, pBar)).
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CMultiPaneFrameWnd__UEAAXPEAVCPane___Z(void* pThis, void* pBar) {
    if (pThis == nullptr || pBar == nullptr) return;
    CDockablePane* p = AsDockable(pBar);
    if (p == nullptr) return;
    int bLeft = TRUE;
    void* pContainer = impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(Mgr(pThis), p, &bLeft);
    impl__StoreDockInfo_CRecentDockSiteInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane___Z(
        &static_cast<CPane*>(p)->m_recentDockInfo, pContainer, nullptr);
}

// Symbol: ?StoreRecentTabRelatedInfo@CMultiPaneFrameWnd@@UEAAXPEAVCDockablePane@@0@Z
// Decoded from retail entry RVA 0x94950 (no NULL checks in retail):
//     BOOL bLeft = TRUE;
//     CPaneContainer* pC = m_barContainerManager.FindPaneContainer(pTabbedBar, bLeft);   // mgr slot 0x128, %r8
//     pDockingBar->m_recentDockInfo.StoreDockInfo(pC, pTabbedBar);                       // %rdx +0x2b8, slot 0x28
// Deviation: NULL arguments return.  Static dispatch; StoreDockInfo is a
// placeholder (see its declaration).
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__StoreRecentTabRelatedInfo_CMultiPaneFrameWnd__UEAAXPEAVCDockablePane__0_Z(void* pThis, void* pDockingBar, void* pTabbedBar) {
    if (pThis == nullptr || pDockingBar == nullptr || pTabbedBar == nullptr) return;
    int bLeft = TRUE;
    void* pContainer = impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(Mgr(pThis), pTabbedBar, &bLeft);
    impl__StoreDockInfo_CRecentDockSiteInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane___Z(
        &static_cast<CPane*>(pDockingBar)->m_recentDockInfo, pContainer, pTabbedBar);
}
