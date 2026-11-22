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
// The native window wrapper uses the retail 0x300-byte allocation. It embeds
// the pane container manager after the 0x258-byte CPaneFrameWnd base. The
// container manager owns the split tree and supplies the pane enumeration.

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

#include "detail/PaneContainerLayout.h"
#include "detail/MultiPaneFrameSupport.h"
#include "detail/MfccoreSupport.h"
#include "detail/Pane16MsgmapSupport.h"
#include "detail/CPaneFrameWndSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <new>
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
extern "C" int   MS_ABI impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(void* pPane, int bAdd);
extern "C" int   MS_ABI impl__OnCreate_CPaneFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* lpCreateStruct);
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CPaneFrameWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam);
extern "C" void  MS_ABI impl__OnCancelMode_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" int   MS_ABI impl__OnBeforeDock_CPaneFrameWnd__UEAAHXZ(void* pThis);
extern "C" int   MS_ABI impl__OnCloseMiniFrame_CPaneFrameWnd__MEAAHXZ(void* pThis);
extern "C" void  MS_ABI impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__SetRollUpTimer_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__KillRollupTimer_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__CalcMinSize_CPaneFrameWnd__MEAAXAEAVCSize__PEAUtagMINMAXINFO___Z(void* pThis, CSize* pSize, MINMAXINFO* pMMI);
extern "C" void  MS_ABI impl__SaveRecentFloatingState_CPaneFrameWnd__MEAAXXZ(void* pThis);
// docking/CPaneFrameWnd.cpp:2099 -- a documented stub (retail 0xb2d20 not reproduced).
extern "C" void  MS_ABI impl__Serialize_CPaneFrameWnd__UEAAXAEAVCArchive___Z(void* pThis, CArchive& archive);

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
extern "C" int   MS_ABI impl__InsertPane_CPaneContainerManager__UEAAHPEAVCDockablePane__0KPEBUtagRECT__W4AFX_DOCK_METHOD___Z(
                     void* pThis, void* pControlBarToInsert, void* pTargetControlBar,
                     unsigned long dwAlignment, const RECT* lpRect, int dockMethod);
extern "C" void  MS_ABI impl__ResizePaneContainers_CPaneContainerManager__UEAAXVCRect__AEAPEAX_Z(void* pThis, const RECT* pRect, void** phdwp);
extern "C" void  MS_ABI impl__GetAvailableSpace_CPaneContainerManager__UEBAXAEAVCRect___Z(const void* pThis, CRect& rect);
// docking/CPaneContainerManager.cpp:1942 -- generated placeholder `(void** p0)`
// with an empty body; the list is therefore NOT shrunk by this call (see
// RemovePane below).
extern "C" int   MS_ABI impl__RemovePaneFromPaneContainer_CPaneContainerManager__UEAAHPEAVCDockablePane___Z(void* pThis, void* pPane);
// docking/CPaneContainerManager.cpp:2086 -- generated placeholder `(void** p0) {}`.
extern "C" void  MS_ABI impl__Serialize_CPaneContainerManager__UEAAXAEAVCArchive___Z(void* pThis, CArchive& archive);

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
extern "C" void  MS_ABI impl__UndockPane_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bDelay);
extern "C" void  MS_ABI impl__ConvertToTabbedDocument_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bActiveTabOnly);
extern "C" void  MS_ABI impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ(CDockablePane* pThis);
// docking/CRecentDockSiteInfo.cpp:64 -- generated placeholder `(void** p0,
// void** p1) {}` (no `this`); docking/CPaneContainer.cpp:184 already declares
// and calls it with this three-parameter shape.
extern "C" void  MS_ABI impl__StoreDockInfo_CRecentDockSiteInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane___Z(
                     void* pThis, void* pContainer, void* pPane);
extern "C" void* MS_ABI impl__GetRecentPaneContainer_CRecentDockSiteInfo__QEAAPEAVCPaneContainer__H_Z(
                     void* pThis, int bForSlider);
extern "C" void* MS_ABI impl__GetRecentTabContainer_CRecentDockSiteInfo__QEAAPEAVCPaneContainer__H_Z(
                     void* pThis, int bForSlider);
extern "C" int   MS_ABI impl__IsRecentLeftPane_CRecentDockSiteInfo__QEAAHH_Z(
                     void* pThis, int bForSlider);
extern "C" void* MS_ABI impl__AddPaneToRecentPaneContainer_CPaneContainerManager__UEAAPEAVCDockablePane__PEAV2_PEAVCPaneContainer___Z(
                     void* pThis, void* pBarToAdd, void* pRecentContainer);

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
extern "C" int   MS_ABI impl__GetPaneAndAlignFromPoint_CGlobalUtils__QEAAHAEAVCPaneContainerManager__VCPoint__PEAPEAVCDockablePane__AEAKAEAH4_Z(
                     void* pThis, void* pBarContainerManager, long long pt, void** ppTargetControlBar,
                     unsigned long* pdwAlignment, int* pbTabArea, int* pbCaption);
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

extern "C" CWnd*  MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void*  MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(const CBasePane* pThis, int bNoAssert);
extern "C" void*  MS_ABI impl__AttachToTabWnd_CDockablePane__UEAAPEAV1_PEAV1_W4AFX_DOCK_METHOD__HPEAPEAV1__Z(
                      CDockablePane* pThis, CDockablePane* pTabControlBarAttachTo, int dockMethod, int bSetActive, CDockablePane** ppTabbedControlBar);
extern "C" void   MS_ABI impl__ShowPane_CDockablePane__UEAAXHHH_Z(CDockablePane* pThis, int bShow, int bDelay, int bActivate);
extern "C" void   MS_ABI impl__ApplyRestoredTabInfo_CBaseTabbedPane__UEAAXH_Z(void* pThis, int bApply);

// Sibling exports defined later in THIS translation unit.
extern "C" void  MS_ABI impl__RemovePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int bDestroy, int bNoDelayedDestroy);
extern "C" void  MS_ABI impl__OnSetRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnKillRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnPaneRecalcLayout_CMultiPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnShowPane_CMultiPaneFrameWnd__UEAAXPEAVCDockablePane__H_Z(void* pThis, void* pBar, int bShow);
extern "C" void  MS_ABI impl__CheckGripperVisibility_CMultiPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnDockToRecentPos_CMultiPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__CloseMiniFrame_CMultiPaneFrameWnd__UEAAXXZ(void*);
extern "C" int MS_ABI impl__OnCreate_CMultiPaneFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(void*, CREATESTRUCTW*);
extern "C" void* MS_ABI impl___0CMultiPaneFrameWnd__QEAA_XZ(void* pThis);

namespace {

static_assert(sizeof(CPaneFrameWnd) == 0x258, "embedded manager follows the retail base");
static_assert(offsetof(CMultiPaneFrameWnd, m_hWndLastFocused) == 0x2f8, "multi-frame last-focused HWND");
static_assert(sizeof(CMultiPaneFrameWnd) == 0x300, "multi-frame retail size");
// Manager-relative offset of CPaneContainerManager::m_pRootContainer, the same
// 0x78 docking/CPaneContainerManager.cpp pins; the retail bodies below read it
// as `0x2d0(%rcx)` == 0x258 + 0x78.
constexpr size_t kOffMgrRootContainer     = 0x78;
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
    return static_cast<CMultiPaneFrameWnd*>(pThis)->GetContainerManager();
}
inline HWND& LastFocused(void* pThis) {
    return static_cast<CMultiPaneFrameWnd*>(pThis)->m_hWndLastFocused;
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

// The CPaneContainer shadow in docking/CPaneContainer.cpp pins these leading
// pane pointers at +0x08 (left) and +0x10 (right).
struct PaneContainerPaneSlots {
    void* vfptr;
    CDockablePane* leftPane;
    CDockablePane* rightPane;
};
static_assert(offsetof(PaneContainerPaneSlots, leftPane) == 0x08, "CPaneContainer left pane");
static_assert(offsetof(PaneContainerPaneSlots, rightPane) == 0x10, "CPaneContainer right pane");
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
    for (const auto* pane : openmfc::detail::ContainerPanes(Mgr(pThis))) {
        if (pane && (pane->m_dwControlBarStyle & kCbrsAutoRollUp) &&
            (::GetWindowLongPtrW(pane->m_hWnd, GWL_STYLE) & WS_VISIBLE)) return true;
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
constexpr int kDockMethodUnknown = 0;

// `pMiniFrame->RemovePane(pBar, bDestroy, bNoDelayedDestroy)` -- CPaneFrameWnd
// vftable slot 0x308.  This DLL has no retail-shaped mini-frame vtable, so the
// slot is resolved by hand: a CMultiPaneFrameWnd gets this file's override,
// anything else the CPaneFrameWnd export.  A client subclass's override is not
// reached (deviation).

// `CWnd::FromHandle(::GetParent(CWnd::FromHandle(::GetParent(h))->m_hWnd))`, the
// pane->GetParent()->GetParent() retail spells out at 0x93d85 and 0x93ef8
// inside SetDockState (entry 0x93bd0): import slot 0x2c5300 = USER32!GetParent,
// direct call 0x289180 = CWnd::FromHandle.  Retail has no NULL check between
// the two steps; NULL is returned here instead of faulting.

}  // namespace

CMultiPaneFrameWnd::CMultiPaneFrameWnd() {
    impl___0CPaneContainerManager__QEAA_XZ(GetContainerManager());
}

CMultiPaneFrameWnd::~CMultiPaneFrameWnd() {
    DestroyPaneFrameWindow();
    impl___1CPaneContainerManager__UEAA_XZ(GetContainerManager());
}

namespace {
extern AFX_CLASSINIT multiPaneFrameClass;
}

CRuntimeClass* AFXAPI CMultiPaneFrameWnd::GetThisClass() {
    static CRuntimeClass type = {"CMultiPaneFrameWnd", sizeof(CMultiPaneFrameWnd), 0x80000002,
        []() -> CObject* { return new CMultiPaneFrameWnd; }, &CPaneFrameWnd::GetThisClass, nullptr, &multiPaneFrameClass};
    return &type;
}

CRuntimeClass* CMultiPaneFrameWnd::GetRuntimeClass() const { return GetThisClass(); }

const AFX_MSGMAP* CMultiPaneFrameWnd::GetMessageMap() const {
    return &openmfc::detail::pane16msgmap::classCMultiPaneFrameWnd_msgmap;
}

void CMultiPaneFrameWnd::AddPane(CBasePane* pane) {
    if (!AsDockable(pane) || !ManagerIsEmpty(this)) return;
    impl__AddPane_CPaneContainerManager__UEAAXPEAVCDockablePane___Z(GetContainerManager(), pane);
    CPaneFrameWnd::AddPane(pane);
    AdjustLayout();
}

void CMultiPaneFrameWnd::RemovePane(CBasePane* pane, BOOL destroy) {
    if (auto* dockable = AsDockable(pane)) openmfc::detail::DetachContainerPane(GetContainerManager(), *dockable);
    CPaneFrameWnd::RemovePane(pane, destroy && ManagerIsEmpty(this));
    AdjustLayout();
}

CWnd* CMultiPaneFrameWnd::GetPane() const {
    return static_cast<CWnd*>(impl__GetFirstPane_CPaneContainerManager__UEBAPEAVCBasePane__XZ(
        const_cast<void*>(GetContainerManager())));
}

CWnd* CMultiPaneFrameWnd::GetFirstVisiblePane() const {
    return static_cast<CWnd*>(impl__GetFirstVisiblePane_CPaneContainerManager__UEBAPEAVCWnd__XZ(
        const_cast<void*>(GetContainerManager())));
}

void CMultiPaneFrameWnd::AdjustLayout() {
    RECT bounds{};
    if (!::GetClientRect(m_hWnd, &bounds)) return;
    void* batch = ::BeginDeferWindowPos(8);
    impl__ResizePaneContainers_CPaneContainerManager__UEAAXVCRect__AEAPEAX_Z(GetContainerManager(), &bounds, &batch);
    if (batch) ::EndDeferWindowPos(static_cast<HDWP>(batch));
}

int CMultiPaneFrameWnd::OnCreate(CREATESTRUCTW* create) {
    return impl__OnCreate_CMultiPaneFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(this, create);
}

void CMultiPaneFrameWnd::OnClose() {
    impl__CloseMiniFrame_CMultiPaneFrameWnd__UEAAXXZ(this);
}

void CMultiPaneFrameWnd::OnSize(UINT type, int, int) {
    Default();
    if (type != SIZE_MINIMIZED) AdjustLayout();
}

LRESULT CMultiPaneFrameWnd::OnCaptionDoubleClick(WPARAM hit, LPARAM) {
    if (hit != HTCAPTION) return Default();
    impl__OnDockToRecentPos_CMultiPaneFrameWnd__UEAAXXZ(this);
    return 0;
}

namespace {
AFX_CLASSINIT multiPaneFrameClass(CMultiPaneFrameWnd::GetThisClass());
}

// Symbol: ?GetThisClass@CMultiPaneFrameWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ() {
    return CMultiPaneFrameWnd::GetThisClass();
}
// Symbol: ?GetRuntimeClass@CMultiPaneFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMultiPaneFrameWnd__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return CMultiPaneFrameWnd::GetThisClass();
}
// Symbol: ??0CMultiPaneFrameWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMultiPaneFrameWnd__QEAA_XZ(void* pThis) {
    return pThis ? ::new (pThis) CMultiPaneFrameWnd : nullptr;
}
// Symbol: ??1CMultiPaneFrameWnd@@UEAA@XZ
extern "C" void* MS_ABI impl___1CMultiPaneFrameWnd__UEAA_XZ(void* pThis) {
    if (pThis) static_cast<CMultiPaneFrameWnd*>(pThis)->CMultiPaneFrameWnd::~CMultiPaneFrameWnd();
    return pThis;
}
// Symbol: ?AddPane@CMultiPaneFrameWnd@@UEAAXPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__AddPane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane___Z(void* pThis, void* pane) {
    if (pThis) static_cast<CMultiPaneFrameWnd*>(pThis)->AddPane(static_cast<CBasePane*>(pane));
}
// Symbol: ?RemovePane@CMultiPaneFrameWnd@@UEAAXPEAVCBasePane@@HH@Z
extern "C" void MS_ABI impl__RemovePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(
    void* pThis, void* pane, int destroy, int) {
    if (pThis) static_cast<CMultiPaneFrameWnd*>(pThis)->RemovePane(static_cast<CBasePane*>(pane), destroy);
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
    return pThis ? static_cast<CMultiPaneFrameWnd*>(pThis)->GetPane() : nullptr;
}
// Symbol: ?GetCaptionText@CMultiPaneFrameWnd@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetCaptionText_CMultiPaneFrameWnd__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, void* pThis) {
    if (!pRet) return;
    pRet->Format(L"Pane group (%d)", static_cast<int>(
        openmfc::detail::ContainerPanes(Mgr(pThis)).size()));
}
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CGlobalUtils__QEAAXAEAVCPaneContainerManager__PEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    void*, void*, CWnd*, long long, RECT*, int*, void**);

// Symbol: ?CalcExpectedDockedRect@CMultiPaneFrameWnd@@UEAAXPEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CMultiPaneFrameWnd__UEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    void* pThis, CWnd* dragged, CPoint point, CRect* rect, int* drawTab, void** target) {
    if (drawTab) *drawTab = FALSE;
    if (target) *target = nullptr;
    if (!rect) return;
    rect->SetRectEmpty();
    if (!pThis || !dragged || !drawTab || !target) return;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (const auto* state = FindStatePtr(pThis); state && state->rolledUp) return;
    }
    long long packed;
    std::memcpy(&packed, &point, sizeof(packed));
    CGlobalUtils utils;
    impl__CalcExpectedDockedRect_CGlobalUtils__QEAAXAEAVCPaneContainerManager__PEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
        &utils, Mgr(pThis), dragged, packed, static_cast<RECT*>(*rect), drawTab, target);
}

// Symbol: ?AddRecentPane@CMultiPaneFrameWnd@@UEAAHPEAVCDockablePane@@@Z
// Decoded from retail entry RVA 0x92940 (mfc140u). If the pane's recent
// mini-frame info has a recent pane container, retail reparents the pane to
// this frame, registers it in the floating-pane list, and calls the manager's
// AddPaneToRecentPaneContainer. For a non-NULL result, retail resolves the
// frame's hosted-pane HWND at +0x148 and stores the result's HWND if no
// permanent CWnd is registered for the existing handle; when the manager then
// reports exactly one visible pane and the result is the input pane, it moves
// the frame to the saved +0x150 rectangle (MoveWindow entry RVA 0x2a9a10,
// mfc140u). OnShowPane is called only for a non-NULL returned pane. Otherwise,
// when recent tab-container info exists, retail selects the container's
// left/right target according to IsRecentLeftPane; the input pane receives
// AttachToTabWnd(target, DM_DBL_CLICK, TRUE), then the target receives
// ShowPane(TRUE, FALSE, TRUE) and the frame recalculates layout. If no target
// is selected, it calls AddPaneToRecentPaneContainer and passes that result to
// OnShowPane even when NULL. Both recent-container branches update the roll-up
// timer and return TRUE; absence of both containers returns FALSE. SetParent's
// IAT slot 0x1802c7288 (mfc140u) resolves to USER32!SetParent. The manager and
// AttachToTabWnd exports remain documented stubs; the calls are still made.
// Deviations: NULL inputs return FALSE rather than faulting. The +0x148
// hosted-pane handle update and conditional MoveWindow to the saved +0x150
// rectangle are omitted: PaneFrameState has no link-safe impl__ accessor in
// this TU. The manager stub currently returns NULL, so these operations are
// not reached in this build; if that sibling stub is implemented, they remain
// missing. The CPaneContainer pane slots are pinned by the shadow and
// static_asserts in docking/CPaneContainer.cpp:85-118.
extern "C" int MS_ABI impl__AddRecentPane_CMultiPaneFrameWnd__UEAAHPEAVCDockablePane___Z(void* pThis, void* pBar) {
    if (pThis == nullptr || pBar == nullptr) return FALSE;

    auto* pPane = static_cast<CPane*>(pBar);
    auto* pRecentInfo = &pPane->m_recentDockInfo;
    void* pRecentContainer = impl__GetRecentPaneContainer_CRecentDockSiteInfo__QEAAPEAVCPaneContainer__H_Z(
        pRecentInfo, FALSE);
    if (pRecentContainer != nullptr) {
        const HWND oldParent = ::SetParent(AsWnd(pBar)->m_hWnd, FrameHwnd(pThis));
        impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(oldParent);
        impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(pBar, TRUE);

        void* pAdded = impl__AddPaneToRecentPaneContainer_CPaneContainerManager__UEAAPEAVCDockablePane__PEAV2_PEAVCPaneContainer___Z(
            Mgr(pThis), pBar, pRecentContainer);
        if (pAdded != nullptr)
            impl__OnShowPane_CMultiPaneFrameWnd__UEAAXPEAVCDockablePane__H_Z(pThis, pAdded, TRUE);
    } else {
        void* pRecentTabContainer = impl__GetRecentTabContainer_CRecentDockSiteInfo__QEAAPEAVCPaneContainer__H_Z(
            pRecentInfo, FALSE);
        if (pRecentTabContainer == nullptr) return FALSE;

        const HWND oldParent = ::SetParent(AsWnd(pBar)->m_hWnd, FrameHwnd(pThis));
        impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(oldParent);
        impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(pBar, TRUE);

        const auto* slots = static_cast<const PaneContainerPaneSlots*>(pRecentTabContainer);
        void* pTabTarget = impl__IsRecentLeftPane_CRecentDockSiteInfo__QEAAHH_Z(pRecentInfo, FALSE)
            ? static_cast<void*>(slots->leftPane)
            : static_cast<void*>(slots->rightPane);
        if (pTabTarget != nullptr) {
            impl__AttachToTabWnd_CDockablePane__UEAAPEAV1_PEAV1_W4AFX_DOCK_METHOD__HPEAPEAV1__Z(
                static_cast<CDockablePane*>(pBar), static_cast<CDockablePane*>(pTabTarget),
                kDockMethodDblClick, TRUE, nullptr);
            impl__ShowPane_CDockablePane__UEAAXHHH_Z(
                static_cast<CDockablePane*>(pTabTarget), TRUE, FALSE, TRUE);
            impl__OnPaneRecalcLayout_CMultiPaneFrameWnd__UEAAXXZ(pThis);
        } else {
            void* pAdded = impl__AddPaneToRecentPaneContainer_CPaneContainerManager__UEAAPEAVCDockablePane__PEAV2_PEAVCPaneContainer___Z(
                Mgr(pThis), pBar, pRecentTabContainer);
            impl__OnShowPane_CMultiPaneFrameWnd__UEAAXPEAVCDockablePane__H_Z(pThis, pAdded, TRUE);
        }
    }
    impl__OnSetRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(pThis);
    return TRUE;
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
// Decoded from retail entry RVA 0x93f80 (mfc140u; vtable +0x320):
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
// Decoded from retail entry RVA 0x93f30 (mfc140u; vtable +0x458):
//     if (OnCloseMiniFrame()) {                 // slot 0x450 -> CPaneFrameWnd::OnCloseMiniFrame (0xb4940)
//         CWnd::ShowWindow(SW_HIDE);            // call 0x2a79e0
//         m_barContainerManager.HideAll();      // mgr slot 0xc8
//     }
// Static dispatch on both slots.
// Signature corrected: the generated list dropped `this`.
extern "C" void MS_ABI impl__CloseMiniFrame_CMultiPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (!pThis || !impl__OnCloseMiniFrame_CPaneFrameWnd__MEAAHXZ(pThis)) return;
    auto* frame = static_cast<CMultiPaneFrameWnd*>(pThis);
    CGlobalUtils utils;
    auto* manager = utils.GetDockingManager(frame);
    if (manager) {
        for (auto* pane : openmfc::detail::ContainerPanes(frame->GetContainerManager()))
            manager->HidePane(pane);
    } else {
        impl__HideAll_CPaneContainerManager__UEAAXXZ(frame->GetContainerManager());
    }
    ::ShowWindow(frame->m_hWnd, SW_HIDE);
}

// Symbol: ?ConvertToTabbedDocument@CMultiPaneFrameWnd@@UEAAXXZ
// Decoded from retail entry RVA 0x948e0 (mfc140u; vtable +0x410):
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
extern "C" void* MS_ABI impl__CreateObject_CMultiPaneFrameWnd__SAPEAVCObject__XZ() {
    return CMultiPaneFrameWnd::GetThisClass()->CreateObject();
}

extern "C" int impl__m_nDockSensitivity_CDockingManager__2HA;
extern "C" int MS_ABI impl__AddPaneContainerManager_CPaneContainerManager__UEAAHPEAVCDockablePane__KAEAV1_H_Z(
    void*, void*, unsigned long, void*, int);
extern "C" int MS_ABI impl__AddPaneContainer_CPaneDivider__UEAAHAEAVCPaneContainerManager__H_Z(void*, void*, int);
extern "C" int MS_ABI impl__DockPaneContainer_CDockablePane__UEAAHAEAVCPaneContainerManager__KW4AFX_DOCK_METHOD___Z(
    CDockablePane*, void*, unsigned long, int);
extern "C" CPaneDivider* MS_ABI impl__CreateDefaultPaneDivider_CDockablePane__SAPEAVCPaneDivider__KPEAVCWnd__PEAUCRuntimeClass___Z(
    DWORD, CWnd*, CRuntimeClass*);
extern "C" int MS_ABI impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(const CBasePane*);
extern "C" void MS_ABI impl__UpdateVirtualRect_CPane__QEAAXXZ(CPane*);
extern "C" void MS_ABI impl__EnterDragMode_CPane__MEAAXH_Z(CPane*, int);

namespace {
DWORD AllowedMultiPaneAlignment(CMultiPaneFrameWnd& source) {
    DWORD allowed = CBRS_ALIGN_ANY;
    for (auto* pane : openmfc::detail::ContainerPanes(source.GetContainerManager()))
        if (pane && (::GetWindowLongPtrW(pane->m_hWnd, GWL_STYLE) & WS_VISIBLE))
            allowed &= pane->m_dwEnabledAlignment;
    return allowed;
}

CDockablePane* DockMultiPaneDrop(CMultiPaneFrameWnd& source, CDockingManager* manager, const DockingDrop& drop,
    CMultiPaneFrameWnd* receiver, int method) {
    if (drop.change != DelayDockToTab && (drop.change != DelayDock || !drop.alignment)) return nullptr;
    auto* first = AsDockable(source.GetFirstVisiblePane());
    if (!first || !first->CanBeDocked()) return nullptr;
    openmfc::detail::ClearPaneFrameDockPreview(source);
    if (drop.pane) {
        if (!::IsWindow(drop.pane->m_hWnd) || ::IsChild(source.m_hWnd, drop.pane->m_hWnd) ||
            drop.pane->IsAutoHideMode() || !first->CanAcceptPane(drop.pane) || !drop.pane->CanAcceptPane(first))
            return nullptr;
        auto* parent = static_cast<CWnd*>(CWnd::FromHandlePermanent(::GetParent(drop.pane->m_hWnd)));
        if (receiver && receiver != parent) return nullptr;
        if (drop.change == DelayDockToTab)
            return openmfc::detail::MergeContainerTabs(*drop.pane, source.GetContainerManager());
        if (parent && parent->IsKindOf(CMultiPaneFrameWnd::GetThisClass())) {
            auto* mini = static_cast<CMultiPaneFrameWnd*>(parent);
            if (impl__AddPaneContainerManager_CPaneContainerManager__UEAAHPEAVCDockablePane__KAEAV1_H_Z(
                    mini->GetContainerManager(), drop.pane, drop.alignment, source.GetContainerManager(), TRUE)) return first;
        } else if (impl__DockPaneContainer_CDockablePane__UEAAHAEAVCPaneContainerManager__KW4AFX_DOCK_METHOD___Z(
                       drop.pane, source.GetContainerManager(), drop.alignment, method)) return first;
        return nullptr;
    }
    if (receiver || !drop.alignment) return nullptr;
    const auto* state = FindDockingState(manager);
    if (!state || !state->parentFrame) return nullptr;
    auto* divider = impl__CreateDefaultPaneDivider_CDockablePane__SAPEAVCPaneDivider__KPEAVCWnd__PEAUCRuntimeClass___Z(
        drop.alignment, state->parentFrame, nullptr);
    if (!divider) return nullptr;
    if (impl__AddPaneContainer_CPaneDivider__UEAAHAEAVCPaneContainerManager__H_Z(
            divider, source.GetContainerManager(), drop.outerEdge)) return first;
    divider->DestroyWindow();
    return nullptr;
}

CDockablePane* DropMultiPaneFrame(CMultiPaneFrameWnd& source, CPoint point, CMultiPaneFrameWnd* receiver, int method) {
    auto* first = AsDockable(source.GetFirstVisiblePane());
    if (!first) return nullptr;
    CGlobalUtils utils;
    auto* manager = utils.GetDockingManager(&source);
    const auto drop = DockingTarget(manager, point, impl__m_nDockSensitivity_CDockingManager__2HA,
        AllowedMultiPaneAlignment(source), first, first);
    return DockMultiPaneDrop(source, manager, drop, receiver, method);
}

CDockablePane* CommitMultiPanePreDock(CMultiPaneFrameWnd& source, CDockingManager* manager,
    const DockingDrop& drop, int method) {
    const HWND popup = source.m_hWnd;
    std::vector<HWND> panes;
    for (auto* pane : openmfc::detail::ContainerPanes(source.GetContainerManager())) {
        if (pane && (::GetWindowLongPtrW(pane->m_hWnd, GWL_STYLE) & WS_VISIBLE)) panes.push_back(pane->m_hWnd);
    }
    auto* docked = DockMultiPaneDrop(source, manager, drop, nullptr, method);
    if (!docked) return nullptr;

    if (CWnd::FromHandlePermanent(popup) == &source) {
        const bool wasDragging = openmfc::detail::EndPaneFrameDockDrag(source);
        if (wasDragging && method == 1 && ::GetKeyState(VK_LBUTTON) < 0 && drop.change != DelayDockToTab) {
            if (auto* receiving = static_cast<CPaneFrameWnd*>(
                    impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(docked, FALSE)))
                receiving->BeginPaneDrag();
            else
                impl__EnterDragMode_CPane__MEAAXH_Z(docked, FALSE);
        }
        if (source.GetPane()) {
            CRect previous;
            {
                std::lock_guard<std::mutex> lock(g_wave2Mutex);
                const auto entry = g_framePanes.find(&source);
                if (entry != g_framePanes.end()) previous = entry->second.recentRect;
            }
            if (!previous.IsRectEmpty()) {
                ::SetWindowPos(popup, nullptr, previous.left, previous.top, previous.Width(), previous.Height(),
                    SWP_NOZORDER | SWP_NOACTIVATE);
            }
        }
    }
    for (HWND hwnd : panes) {
        if (auto* pane = AsDockable(CWnd::FromHandlePermanent(hwnd)))
            impl__UpdateVirtualRect_CPane__QEAAXXZ(pane);
    }
    return docked;
}
}

CDockablePane* openmfc::detail::DockMultiPaneFrameAtPoint(CMultiPaneFrameWnd& frame, CPoint point, BOOL& docked,
    int method) {
    auto* pane = DropMultiPaneFrame(frame, point, nullptr, method);
    docked = pane != nullptr;
    return pane;
}

// Symbol: ?DockFrame@CMultiPaneFrameWnd@@UEAAHPEAVCPaneFrameWnd@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__DockFrame_CMultiPaneFrameWnd__UEAAHPEAVCPaneFrameWnd__W4AFX_DOCK_METHOD___Z(
    void* pThis, void* pDockedFrame, int dockMethod) {
    auto* source = static_cast<CPaneFrameWnd*>(pDockedFrame);
    POINT cursor{};
    if (!pThis || !source || !source->IsKindOf(CMultiPaneFrameWnd::GetThisClass()) ||
        pThis == pDockedFrame || (dockMethod != 1 && dockMethod != 5) || !::GetCursorPos(&cursor)) return TRUE;
    return DropMultiPaneFrame(*static_cast<CMultiPaneFrameWnd*>(source), CPoint(cursor),
        static_cast<CMultiPaneFrameWnd*>(pThis), dockMethod) == nullptr;
}

// Symbol: ?DockPane@CMultiPaneFrameWnd@@UEAAHPEAVCDockablePane@@@Z
// Decoded from retail entry RVA 0x922c0 (mfc140u): get the cursor position,
// then call CGlobalUtils::GetPaneAndAlignFromPoint (entry RVA 0x6d6c0,
// mfc140u). If it fails, or returns no target/alignment, return TRUE. Otherwise
// call CDockablePane::UndockPane(FALSE) (slot 0x490; body RVA 0x44ed0,
// mfc140u), ::SetParent(pBar, this), and CPaneContainerManager::InsertPane
// (slot 0x58) with a NULL rect and DM_UNKNOWN. On a successful insert it adds
// the pane to CPaneFrameWnd's global list, checks gripper visibility, recalculates
// the frame, and sends WM_NCPAINT. It then always sets the roll-up timer, calls
// SetFocus because pane slot 0x3c0 is the `return 1` body at RVA 0x3a60
// (mfc140u), recalculates again, and returns `!InsertPane()`.
// IAT slots were resolved with iatu.py: mfc140u VA 0x1802c7320 is USER32!GetCursorPos,
// 0x1802c7288 is USER32!SetParent, and 0x1802c7120 is USER32!SendMessageW (called as
// ::SendMessage in this UNICODE build). Deviations: NULL inputs return TRUE
// rather than faulting; virtual calls are dispatched through the available
// thunks, so further-derived overrides are not reached. UndockPane is currently
// a documented stub (CDockablePane.cpp:3294), and InsertPane eventually reaches
// the documented AddPaneAndPaneDivider stub (CPaneContainerManager.cpp:435),
// which limits successful docking in this build. The retail sequence is still
// reproduced around those unavailable sibling operations.
// Signature corrected: the generated list dropped `this`.
extern "C" int MS_ABI impl__DockPane_CMultiPaneFrameWnd__UEAAHPEAVCDockablePane___Z(void* pThis, void* pBar) {
    if (pThis == nullptr || pBar == nullptr) return TRUE;

    POINT cursor = {};
    ::GetCursorPos(&cursor);
    long long point = 0;
    static_assert(sizeof(cursor) == sizeof(point), "CPoint uses one 8-byte MS_ABI register");
    std::memcpy(&point, &cursor, sizeof(point));

    void* pTarget = nullptr;
    unsigned long dwAlignment = 0;
    int bTabArea = FALSE;
    int bCaption = FALSE;
    CGlobalUtils* pUtils = reinterpret_cast<CGlobalUtils*>(impl__afxGlobalUtils__3VCGlobalUtils__A);
    if (!impl__GetPaneAndAlignFromPoint_CGlobalUtils__QEAAHAEAVCPaneContainerManager__VCPoint__PEAPEAVCDockablePane__AEAKAEAH4_Z(
            pUtils, Mgr(pThis), point, &pTarget, &dwAlignment, &bTabArea, &bCaption) ||
        pTarget == nullptr || dwAlignment == 0) {
        return TRUE;
    }

    impl__UndockPane_CDockablePane__UEAAXH_Z(static_cast<CDockablePane*>(pBar), FALSE);
    const HWND hOldParent = ::SetParent(AsWnd(pBar)->m_hWnd, FrameHwnd(pThis));
    (void)impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hOldParent);

    const int bInserted = impl__InsertPane_CPaneContainerManager__UEAAHPEAVCDockablePane__0KPEBUtagRECT__W4AFX_DOCK_METHOD___Z(
        Mgr(pThis), pBar, pTarget, dwAlignment, nullptr, kDockMethodUnknown);
    if (bInserted) {
        impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(pBar, TRUE);
        impl__CheckGripperVisibility_CMultiPaneFrameWnd__UEAAXXZ(pThis);
        impl__OnPaneRecalcLayout_CMultiPaneFrameWnd__UEAAXXZ(pThis);
        ::SendMessage(FrameHwnd(pThis), WM_NCPAINT, 0, 0);
    }
    impl__OnSetRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(pThis);
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(AsWnd(pBar));
    impl__OnPaneRecalcLayout_CMultiPaneFrameWnd__UEAAXXZ(pThis);
    return bInserted ? FALSE : TRUE;
}

// Symbol: ?DockPaneStandard@CMultiPaneFrameWnd@@MEAAPEAVCDockablePane@@AEAH@Z
// This entry returns NULL. The result flag follows SetPreDockState for a
// docked target and DockFrame when the target belongs to another multi-frame.
extern "C" void* MS_ABI impl__DockPaneStandard_CMultiPaneFrameWnd__MEAAPEAVCDockablePane__AEAH_Z(
    void* pThis, int* wasDocked) {
    if (!wasDocked) return nullptr;
    *wasDocked = FALSE;
    if (!impl__OnBeforeDock_CPaneFrameWnd__UEAAHXZ(pThis)) return nullptr;
    auto& source = *static_cast<CMultiPaneFrameWnd*>(pThis);
    auto* first = AsDockable(source.GetFirstVisiblePane());
    POINT point{};
    if (!first || !::GetCursorPos(&point)) return nullptr;
    CGlobalUtils utils;
    auto* manager = utils.GetDockingManager(&source);
    const auto drop = DockingTarget(manager, CPoint(point), impl__m_nDockSensitivity_CDockingManager__2HA,
        AllowedMultiPaneAlignment(source), first, first);
    auto* targetFrame = drop.pane ? static_cast<CPaneFrameWnd*>(
        impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(drop.pane, FALSE)) : nullptr;
    if (targetFrame && (&source == targetFrame || !targetFrame->IsKindOf(CMultiPaneFrameWnd::GetThisClass())))
        return nullptr;

    const HWND focus = ::GetFocus();
    if (CommitMultiPanePreDock(source, manager, drop, 5 /* DM_STANDARD */))
        *wasDocked = targetFrame || drop.change != DelayDockToTab;
    if (::IsWindow(focus) && ::GetFocus() != focus) ::SetFocus(focus);
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
// Decoded from retail entry RVA 0x94430 (mfc140u; WM_APP message-map handler; wParam/lParam are never read):
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
    return impl__Create_CPaneContainerManager__UEAAHPEAVCWnd__PEAVCPaneDivider__PEAUCRuntimeClass___Z(
        Mgr(pThis), pThis, nullptr, nullptr) ? 0 : -1;
}

// Symbol: ?OnDockToRecentPos@CMultiPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__OnDockToRecentPos_CMultiPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (!pThis) return;
    for (auto* pane : openmfc::detail::ContainerPanes(Mgr(pThis))) {
        if (pane && (::GetWindowLongPtrW(pane->m_hWnd, GWL_STYLE) & WS_VISIBLE)) pane->DockToRecentPosition();
    }
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
// Decoded from retail entry RVA 0x92770 (mfc140u; vtable +0x400):
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
// Decoded from retail entry RVA 0x92ba0 (mfc140u; vtable +0x3a8):
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
    ::GetClientRect(hwnd, &rectClient);
    HDWP hdwp = ::BeginDeferWindowPos(20);
    void* pHdwp = hdwp;
    impl__ResizePaneContainers_CPaneContainerManager__UEAAXVCRect__AEAPEAX_Z(Mgr(pThis), &rectClient, &pHdwp);
    ::EndDeferWindowPos(static_cast<HDWP>(pHdwp));
    if (impl__m_bHandleMinSize_CPane__2HA != 0) {
        CRect rectAvail(0, 0, 0, 0);
        impl__GetAvailableSpace_CPaneContainerManager__UEBAXAEAVCRect___Z(Mgr(pThis), rectAvail);
        RECT rectWnd = {0, 0, 0, 0};
        ::GetWindowRect(hwnd, &rectWnd);
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
// Decoded from WM_SETFOCUS message-map handler entry RVA 0x94790 (mfc140u;
// message map RVA 0x2f06a8; pOldWnd is never read; CWnd::OnSetFocus
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
// Decoded from retail entry RVA 0x926b0 (mfc140u; vtable +0x3f8):
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
// Decoded from retail entry RVA 0x93e40 (mfc140u; vtable +0x3e0):
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
extern "C" void MS_ABI impl__OnSize_CMultiPaneFrameWnd__IEAAXIHH_Z(void* pThis, unsigned int type, int cx, int cy) {
    if (pThis) static_cast<CMultiPaneFrameWnd*>(pThis)->OnSize(type, cx, cy);
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
    if (impl__ReplacePane_CPaneContainerManager__UEAAHPEAVCDockablePane__0_Z(Mgr(pThis), pBarOrg, pBarReplaceWith) &&
        pBarOrg && pBarReplaceWith && pBarOrg != pBarReplaceWith)
        openmfc::detail::ReplaceFloatingPaneRegistration(*static_cast<CPaneFrameWnd*>(pThis),
            *static_cast<CBasePane*>(pBarOrg), *static_cast<CBasePane*>(pBarReplaceWith));
    impl__OnSetRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ(pThis);
}

// Symbol: ?SaveRecentFloatingState@CMultiPaneFrameWnd@@MEAAXXZ
// Decoded from retail entry RVA 0x94250 (mfc140u; vtable +0x448):
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
extern "C" void MS_ABI impl__Serialize_CMultiPaneFrameWnd__UEAAXAEAVCArchive___Z(
    void* self, CArchive& archive) {
    if (self) static_cast<CMultiPaneFrameWnd*>(self)->CMultiPaneFrameWnd::Serialize(archive);
}

void CMultiPaneFrameWnd::Serialize(CArchive& archive) {
    CPaneFrameWnd::Serialize(archive);
    try {
        impl__Serialize_CPaneContainerManager__UEAAXAEAVCArchive___Z(GetContainerManager(), archive);
    } catch (...) {
        if (archive.IsLoading()) {
            DestroyPaneFrameWindow();
            impl___1CPaneContainerManager__UEAA_XZ(GetContainerManager());
            impl___0CPaneContainerManager__QEAA_XZ(GetContainerManager());
        }
        throw;
    }
}

extern "C" int impl__m_bRestoringDockState_CDockingManager__2HA;
extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(CDockingManager*, UINT, int);

namespace {
void RestoreFloatingTabs(CBaseTabbedPane& group, CDockingManager& manager) {
    std::vector<HWND> priorGroups;
    if (const auto* tabState = FindTabCtrlState(group.GetTabs()); tabState && tabState->restoredTabs) {
        for (const auto& tab : *tabState->restoredTabs) {
            if (!tab.controlId || tab.controlId == -1) continue;
            auto* pane = impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(&manager, tab.controlId, TRUE);
            if (!pane || !pane->m_hWnd) continue;
            const HWND candidate = ::GetParent(::GetParent(pane->m_hWnd));
            if (candidate && candidate != group.m_hWnd &&
                std::find(priorGroups.begin(), priorGroups.end(), candidate) == priorGroups.end())
                priorGroups.push_back(candidate);
        }
    }
    impl__ApplyRestoredTabInfo_CBaseTabbedPane__UEAAXH_Z(&group, TRUE);
    for (const HWND candidate : priorGroups) {
        auto* window = static_cast<CWnd*>(CWnd::FromHandlePermanent(candidate));
        if (!window || !window->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) continue;
        auto* oldGroup = static_cast<CBaseTabbedPane*>(window);
        if (!oldGroup->GetTabs() || oldGroup->GetTabs()->GetTabsCount()) continue;
        auto* previous = static_cast<CWnd*>(CWnd::FromHandlePermanent(::GetParent(candidate)));
        if (previous && previous->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd))) {
            static_cast<CPaneFrameWnd*>(previous)->RemovePane(oldGroup);
            oldGroup->DestroyWindow();
        }
    }
}
}

// Symbol: ?SetDockState@CMultiPaneFrameWnd@@UEAAXPEAVCDockingManager@@@Z
extern "C" void MS_ABI impl__SetDockState_CMultiPaneFrameWnd__UEAAXPEAVCDockingManager___Z(
    void* self, void* dockManager) {
    using namespace openmfc::detail;
    auto* frame = static_cast<CMultiPaneFrameWnd*>(self);
    auto* manager = static_cast<CDockingManager*>(dockManager);
    if (!frame || !frame->m_hWnd || !manager) return;
    bool anyVisible = false;
    for (auto* pane : ContainerPanes(frame->GetContainerManager())) {
        if (!pane || !pane->m_hWnd) continue;
        auto* group = pane->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane)) ? static_cast<CBaseTabbedPane*>(pane) : nullptr;
        const bool hasSavedVisibility = group || impl__m_bRestoringDockState_CDockingManager__2HA;
        const BOOL visible = hasSavedVisibility
            ? pane->m_bRecentVisibleState
            : (::GetWindowLongPtrW(pane->m_hWnd, GWL_STYLE) & WS_VISIBLE) != 0;
        if (!RestorePaneInFloatingFrame(*pane, *frame, *manager, visible)) {
            DetachContainerPane(frame->GetContainerManager(), *pane);
            continue;
        }
        if (group) {
            RestoreFloatingTabs(*group, *manager);
            auto* tabs = group->GetTabs();
            if (!tabs || !tabs->GetTabsCount()) {
                frame->RemovePane(group, FALSE);
                group->DestroyWindow();
                continue;
            }
            group->RecalcLayout();
        }
        anyVisible |= visible != FALSE;
    }
    auto* pane = frame->GetPane();
    if (!pane) {
        ::PostMessageW(frame->m_hWnd, impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA, 0, 0);
        return;
    }
    frame->CPaneFrameWnd::AddPane(static_cast<CBasePane*>(pane));
    FinishFloatingFrameRestore(*frame, anyVisible);
}


// Symbol: ?SetPreDockState@CMultiPaneFrameWnd@@UEAAHW4AFX_PREDOCK_STATE@@PEAVCBasePane@@W4AFX_DOCK_METHOD@@@Z
// The tab arm returns TRUE after attaching its visible panes. Side and edge
// requests return FALSE after a successful transfer.
extern "C" int MS_ABI impl__SetPreDockState_CMultiPaneFrameWnd__UEAAHW4AFX_PREDOCK_STATE__PEAVCBasePane__W4AFX_DOCK_METHOD___Z(
    void* pThis, int request, void* bar, int method) {
    auto* source = static_cast<CMultiPaneFrameWnd*>(pThis);
    if (!source || !source->m_hWnd || (request != PreDockRegular && request != PreDockToTab)) return TRUE;
    auto* target = AsDockable(bar);
    if (bar && (!target || ::IsChild(source->m_hWnd, target->m_hWnd))) return TRUE;
    CGlobalUtils utils;
    auto* manager = utils.GetDockingManager(source);
    POINT point{};
    if (!manager || !::GetCursorPos(&point)) return TRUE;

    CDockablePane* hidden = nullptr;
    if (!source->GetFirstVisiblePane()) {
        if (request != PreDockRegular || !target ||
            openmfc::detail::ContainerPanes(source->GetContainerManager()).size() != 1) return TRUE;
        hidden = AsDockable(source->GetPane());
        if (!hidden || !(impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(hidden) & 2 /* DT_STANDARD */))
            return TRUE;
    }
    const DWORD allowed = hidden ? hidden->m_dwEnabledAlignment : AllowedMultiPaneAlignment(*source);
    const auto drop = PreDockTarget(manager, CPoint(point), allowed, target, request);
    if (drop.change == NoDockingChange) return TRUE;
    if (hidden) manager->ShowPane(hidden);
    auto* docked = CommitMultiPanePreDock(*source, manager, drop, method);
    if (!docked && hidden) manager->HidePane(hidden);
    return !docked || request == PreDockToTab;
}

// Symbol: ?StoreRecentDockSiteInfo@CMultiPaneFrameWnd@@UEAAXPEAVCPane@@@Z
// Decoded from retail entry RVA 0x94310 (mfc140u; vtable +0x3d0):
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
