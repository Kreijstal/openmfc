// CDockablePane — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

#include <cstddef>   // offsetof (static_asserts below)
#include <cstring>   // memcpy
#include <cwchar>    // wcscmp
#include <mutex>
#include <unordered_map>


// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CBasePane* pTargetBar, const RECT* lpRect, int);
extern "C" int MS_ABI impl__OnBeforeFloat_CPane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(CPane* pThis, CRect& rect, int);
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(CBasePane* pThis, int bShow, int, int);
extern "C" void MS_ABI impl__CopyState_CPane__UEAAXPEAV1__Z(CPane* pThis, CPane* pOrgBar);

// ---------------------------------------------------------------------------
// Retail CDockablePane instance layout (MSVC), derived from the shipping
// atlmfc header afxdockablepane.h and CONFIRMED against the retail
// mfc140.dll disassembly at the offsets marked (*) below. The CPane
// sub-object occupies 0..0x3F7 (sizeof(CPane) == 1016 == 0x3F8) and
// sizeof(CDockablePane) == 0x4E0 == 1248.
//
//   +0x3F8  CRect    m_rectCloseButton
//   +0x408  bool     m_bPrepareToFloat        (*) EnterDragMode 0x41e20 stores
//                                                 1; OnCancelMode 0x450d0 and
//                                                 OnContextMenu 0x46200 test it
//   +0x409  bool     m_bReadyToFloat
//   +0x40C  int      m_cyGripper              (*) EnableGripper 0x40160 writes,
//                                                 GetCaptionHeight 0x46520 reads
//   +0x410  int      m_nLastPercent           (*) CopyState 0x46410 copies
//   +0x414  int      m_nBorderSize
//   +0x418  BOOL     m_bHasGripper            (*) EnableGripper 0x40160 tests
//   +0x41C  BOOL     m_bActive
//   +0x420  BOOL     m_bEnableAutoHideAll     (*) IsAutohideAllEnabled thunk at
//                                                 RVA 0x12e10 is `mov 0x420(%rcx),%eax`
//   +0x424  DWORD    m_dwSCBStyle
//   +0x428  CRect    m_rectRedraw
//   +0x438  BOOL     m_bPinState              (*) IsAutoHideMode thunk at RVA
//                                                 0x12e00 is `mov 0x438(%rcx),%eax`
//   +0x440  UINT_PTR m_nAutoHideConditionTimerID
//   +0x448  UINT_PTR m_nSlideTimer
//   +0x450  int      m_nSlideStep
//   +0x454  int      m_nSlideDelta
//   +0x458  CRect    m_rectRestored           (*) CopyState 0x46410 copies 16 bytes
//   +0x468  BOOL     m_bIsSliding
//   +0x46C  BOOL     m_bIsResizing
//   +0x470  BOOL     m_bIsHiding
//   +0x474  UINT     m_ahSlideMode            (*) GetAHSlideMode thunk at RVA
//                                                 0x12e90 is `mov 0x474(%rcx),%eax`
//   +0x478  CMFCAutoHideButton* m_pAutoHideButton
//   +0x480  CMFCAutoHideBar*    m_pAutoHideBar (*) IsVisible 0x45330 reads it
//   +0x488  CObArray m_arrButtons             (*) FindButtonByHit 0x45210 reads
//                                                 m_pData at 0x490 and m_nSize at 0x498
//   +0x4B0  CRuntimeClass* m_pTabbedControlBarRTC
//   +0x4B8  CToolTipCtrl*  m_pToolTip
//   +0x4C0  HWND     m_hDefaultSlider         (*) GetDefaultPaneDivider 0x45cd0 reads
//   +0x4C8  HWND     m_hRestoredDefaultSlider
//   +0x4D0  UINT     m_nHot                   (*) OnLButtonDown 0x41150 writes
//   +0x4D4  UINT     m_nHit                   (*) OnLButtonDown 0x41150 writes
//   +0x4D8  BOOL     m_bCaptionButtonsCaptured (*) OnContextMenu 0x46200 tests
//
// OpenMFC's CDockablePane (include/openmfc/afxmfc.h) declares none of these by
// name: it carries a 128-byte `_dockablepane_padding` blob and sizeof() ==
// 1144. Measured with the phase4 mingw flags that blob starts at offset 1012
// (0x3F4) -- the Itanium ABI folds it into CPane's four bytes of tail padding --
// so the retail fields from +0x3F8 to +0x473 DO have storage inside the object
// and a few bodies below use them at their retail offsets through the
// DockablePaneField<> helper. Everything from +0x474 up does not: +0x474 is
// where CBaseTabbedPane's own padding blob begins (measured: offset 1140,
// sizeof(CBaseTabbedPane) == 1240) and +0x478 is one past the end of the
// object. So m_ahSlideMode, m_pAutoHideButton, m_pAutoHideBar, m_arrButtons,
// m_pTabbedControlBarRTC, m_pToolTip, m_hDefaultSlider, m_nHot, m_nHit and
// m_bCaptionButtonsCaptured have no in-object storage. Three of them are kept
// in side tables instead: m_pAutoHideBar in the shared PaneCoreState
// (AutoHideBarOf below), m_hDefaultSlider and m_pAutoHideButton in the
// file-private DockablePaneExtra table (ExtraOf below). The rest are what
// still blocks the remaining stubs in this file -- see `headerRequests`.
// ---------------------------------------------------------------------------

// Sibling thunks used below. BRIEFING S1: the C++ methods do not exist inside
// this DLL, only these exports. Several of these thunks are still generated
// stubs whose parameter list omits the implicit `this` (e.g.
// impl__OnSettingChange_CBasePane__IEAAXIPEB_W_Z is defined
// `(unsigned int p0, const wchar_t* p1) {}` at
// phase4/src/featurepack/docking/CBasePane.cpp:1412;
// impl__OnCancelMode_CPane__IEAAXXZ, once such a stub, is now the real
// `(CPane* pThis)` body at CPane.cpp:1064); the retail exports do take `this`
// in %rcx, and because the stub bodies read none of their arguments the extra
// register argument is harmless under the MS x64 ABI. The declarations here
// use the retail-correct parameter lists.
//
// One shared, deliberate deviation: the implemented bodies below start with a
// NULL check on `this` (and, in CanAcceptMiniFrame, on the mini-frame argument).
// Retail has no such check -- it would fault -- but the rest of this file
// already guards that way and a generated stub can hand these thunks anything.
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCTabCtrl__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
    const CBasePane* pThis, int bNoAssert);
extern "C" int MS_ABI impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(
    CBasePane* pBar, int bAdd);
extern "C" int MS_ABI impl__CanBeDockedToPane_CPaneFrameWnd__UEBAHPEBVCDockablePane___Z(
    const void* pMiniFrame, const CDockablePane* pPane);
extern "C" void MS_ABI impl__OnAfterChangeParent_CPane__UEAAXPEAVCWnd___Z(CPane* pThis, CWnd* pWndOldParent);
extern "C" void MS_ABI impl__OnBeforeChangeParent_CPane__UEAAXPEAVCWnd__H_Z(CPane* pThis, CWnd* pWndNewParent, int bDelay);
extern "C" void MS_ABI impl__OnCancelMode_CPane__IEAAXXZ(CPane* pThis);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC* pThis);
// CPaintDC, defined at phase4/src/core/gdi/CPaintDC.cpp:12 and :30.
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);
extern "C" void MS_ABI impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
    CDC* pThis, const RECT* lpRect, SIZE size, const RECT* lpRectLast, SIZE sizeLast,
    CBrush* pBrush, CBrush* pBrushLast);
// CDockingManager::m_bRestoringDockState, defined at
// phase4/src/featurepack/docking/StaticData.cpp:50.
extern "C" int impl__m_bRestoringDockState_CDockingManager__2HA;
// Sibling thunks defined further down in THIS file.
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CDockablePane__MEAAXXZ(CDockablePane* pThis);
extern "C" void MS_ABI impl__UndockPane_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bDelay);
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ(CDockablePane* pThis);

// --- Added for the batch of bodies transcribed below -------------------------
// Thunks in other translation units. Several of these are still generated stubs
// whose parameter list omits the implicit `this`; as noted above the retail
// exports do take it in %rcx and those bodies read no argument, so declaring the
// retail-correct list here is safe under the MS x64 ABI.
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
// CWnd::ScreenToClient(LPRECT), a real body at phase4/src/core/window/Thunks.cpp:1658.
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" void MS_ABI impl__UpdateAllShadows_CMFCPopupMenu__SAXPEAUtagRECT___Z(RECT* lpRectExclude);
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const CWnd* pWnd);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis);
extern "C" void MS_ABI impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(CBasePane* pThis, void* pRect);
extern "C" int MS_ABI impl__IsMDITabbed_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" void MS_ABI impl__EnterDragMode_CPane__MEAAXH_Z(CPane* pThis, int bChangeHotPoint);
extern "C" void MS_ABI impl__OnContextMenu_CPane__IEAAXPEAVCWnd__VCPoint___Z(
    CPane* pThis, CWnd* pWnd, void* point);
extern "C" int MS_ABI impl__LoadState_CPane__UEAAHPEB_WHI_Z(
    CPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
// CMFCPopupMenu::m_pActivePopupMenu, defined at
// phase4/src/core/runtime/StaticData.cpp:40.
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
// AFX_WM_ON_PRESS_CLOSE_BUTTON, a RegisterWindowMessage id defined at
// phase4/src/core/runtime/Globals.cpp:149.
extern "C" unsigned int impl__AFX_WM_ON_PRESS_CLOSE_BUTTON__3IA;
// More sibling thunks defined further down in THIS file.
extern "C" int MS_ABI impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(
    CDockablePane* pThis, void* point, int bDetectCaption);
extern "C" int MS_ABI impl__GetCaptionHeight_CDockablePane__UEBAHXZ(const CDockablePane* pThis);
extern "C" int MS_ABI impl__IsDocked_CDockablePane__UEBAHXZ(const CDockablePane* pThis);
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CDockablePane__MEAAXVCPoint___Z(
    CDockablePane* pThis, void* point);

// --- Added for the 2026-09-12 batch (OnUpdateCmdUI, OnSetFocus, OnLButtonDown,
// OnDestroy, OnTimer, GetCaptionHeight, GetDockingStatus, ReplacePane,
// RemoveFromDefaultPaneDivider, SaveState, OnSettingChange, OnSetText,
// IsVisible, PreTranslateMessage, CalcCorrectOffset) ---------------------------
// Thunks in other translation units. As above, several of these are still
// generated stubs whose parameter list omits the implicit `this`; the retail
// exports take it in %rcx and those stub bodies read no argument, so the
// retail-correct lists declared here are safe under the MS x64 ABI. Each was
// located with `grep -rn` before being declared:
//   UpdateDialogControls  core/window/Thunks.cpp:1743   (real body)
//   OnSetFocus_CWnd       core/window/Thunks.cpp:1588
//   SetWindowPos_CWnd     core/window/CWnd.cpp:857
//   ShowWindow_CWnd       core/window/CWnd.cpp:759
//   CanFloat_CBasePane    featurepack/docking/Thunks.cpp:1173
//   IsTabbed_CPane        featurepack/docking/CPane.cpp:893
//   IsVisible_CBasePane   featurepack/docking/CBasePane.cpp:1109
//   GetDockingMode / GetDockSiteFrameWnd / GetParentTabbedPane -- CBasePane.cpp
//   SetWindowPos_CBasePane featurepack/docking/Thunks.cpp
//   OnSettingChange_CBasePane   CBasePane.cpp:1412 (stub, no `this`)
//   PreTranslateMessage_CBasePane CBasePane.cpp:1423 (stub, no `this`)
//   RemovePaneFromDockManager_CBasePane CBasePane.cpp:1428 (stub, no `this`)
//   OnLButtonDown_CPane   generated stub (no // Symbol: marker in the tree)
//   RemoveFromMiniframe_CPane / OnDestroy_CPane / SaveState_CPane -- CPane.cpp
//   GetDockingManager_CGlobalUtils core/runtime/CGlobalUtils.cpp:124
//   RemoveHiddenMDITabbedBar_CDockingManager CDockingManager.cpp:621
//   ReplacePane_CPaneFrameWnd   CPaneFrameWnd.cpp:2003
//   ReplacePane_CPaneDivider    CPaneDivider.cpp:249 (stub)
//   GetTabFromHwnd / GetTabLabel / SetTabLabel -- tabs/CMFCBaseTabCtrl.cpp
extern "C" void MS_ABI impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(
    CWnd* pThis, CCmdTarget* pTarget, int bDisableIfNoHndler);
extern "C" void MS_ABI impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(CWnd* pThis, CWnd* pOldWnd);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__CanFloat_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" int MS_ABI impl__IsTabbed_CPane__UEBAHXZ(const CPane* pThis);
extern "C" int MS_ABI impl__IsVisible_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" int MS_ABI impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(const CBasePane* pThis);
extern "C" CWnd* MS_ABI impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(const CBasePane* pThis);
extern "C" void* MS_ABI impl__GetParentTabbedPane_CBasePane__QEBAPEAVCBaseTabbedPane__XZ(const CBasePane* pThis);
extern "C" void* MS_ABI impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
    CBasePane* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy,
    unsigned int nFlags, void* pExtra);
extern "C" void MS_ABI impl__OnSettingChange_CBasePane__IEAAXIPEB_W_Z(
    CBasePane* pThis, unsigned int uFlags, const wchar_t* lpszSection);
extern "C" int MS_ABI impl__PreTranslateMessage_CBasePane__UEAAHPEAUtagMSG___Z(CBasePane* pThis, MSG* pMsg);
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CBasePane__QEAAXPEAV1_HHH0_Z(
    CBasePane* pThis, CBasePane* pPane, int bDestroy, int bAdjustLayout, int bAutoHide,
    CBasePane* pBarReplacement);
extern "C" void MS_ABI impl__OnLButtonDown_CPane__IEAAXIVCPoint___Z(CPane* pThis, unsigned int nFlags, void* point);
extern "C" void MS_ABI impl__RemoveFromMiniframe_CPane__MEAAXPEAVCWnd__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CWnd* pNewParent, int dockMethod);
extern "C" void MS_ABI impl__OnDestroy_CPane__IEAAXXZ(CPane* pThis);
extern "C" int MS_ABI impl__SaveState_CPane__UEAAHPEB_WHI_Z(
    CPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
extern "C" CDockingManager* MS_ABI impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(
    CGlobalUtils* pThis, CWnd* pWnd);
// ?afxGlobalUtils@@3VCGlobalUtils@@A, defined at
// phase4/src/featurepack/CMFC_misc_stubs.cpp:3665 as a 16-byte blob.
extern "C" unsigned char impl__afxGlobalUtils__3VCGlobalUtils__A[16];
extern "C" void MS_ABI impl__RemoveHiddenMDITabbedBar_CDockingManager__QEAAXPEAVCDockablePane___Z(
    CDockingManager* pThis, CDockablePane* pPane);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__ReplacePane_CPaneFrameWnd__UEAAXPEAVCBasePane__0_Z(
    void* pThis, void* pOldBar, void* pNewBar);
extern "C" int MS_ABI impl__ReplacePane_CPaneDivider__UEAAHPEAVCDockablePane__0_Z(
    void* pThis, CDockablePane* pOldBar, CDockablePane* pNewBar);
extern "C" int MS_ABI impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(
    const CMFCBaseTabCtrl* pThis, HWND hWnd);
extern "C" int MS_ABI impl__GetTabLabel_CMFCBaseTabCtrl__UEBAHHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CMFCBaseTabCtrl* pThis, int iTab, CString& strLabel);
extern "C" int MS_ABI impl__SetTabLabel_CMFCBaseTabCtrl__UEAAHHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CMFCBaseTabCtrl* pThis, int iTab, const CString& strLabel);
// Exported statics, both defined in phase4/src/featurepack/docking/StaticData.cpp
// (:38 and :32).
extern "C" int impl__m_nTimeOutBeforeAutoHide_CDockablePane__2HA;
extern "C" int impl__m_bHideInAutoHideMode_CDockablePane__2HA;
// Sibling thunks defined further down in THIS file (their definitions carry the
// retail-correct parameter lists; several are still stubs -- see `stubbed`).
extern "C" int MS_ABI impl__CheckAutoHideCondition_CDockablePane__MEAAHXZ(CDockablePane* pThis);
extern "C" int MS_ABI impl__CheckStopSlideCondition_CDockablePane__MEAAHH_Z(CDockablePane* pThis, int bDirection);
extern "C" void MS_ABI impl__OnSlide_CDockablePane__MEAAXH_Z(CDockablePane* pThis, int bSlideOut);
extern "C" void* MS_ABI impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(const CDockablePane* pThis);
extern "C" void MS_ABI impl__RemoveCaptionButtons_CDockablePane__IEAAXXZ(CDockablePane* pThis);
extern "C" void MS_ABI impl__EnableGripper_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bEnable);
extern "C" void MS_ABI impl__SetDefaultPaneDivider_CDockablePane__QEAAXPEAUHWND_____Z(CDockablePane* pThis, HWND hDivider);
extern "C" int MS_ABI impl__IsVisible_CDockablePane__UEBAHXZ(const CDockablePane* pThis);

// --- Added for the 2026-09-14 batch (OnUpdateCmdUI, OnSettingChange, SaveState,
// SetResizeMode, SetAutoHideParents, Set/Get/RemoveFromDefaultPaneDivider,
// OnTimer, ReplacePane, StoreRecentDockSiteInfo, OnSetText, PreTranslateMessage,
// AdjustPaneToPaneContainer) --------------------------------------------------
// Each thunk below was located with `grep -rn` before being declared:
//   GetExStyle_CWnd                    core/window/Thunks.cpp:1312 (real body)
//   GetThisClass_CPaneDivider          docking/RuntimeClasses.cpp:268
//   RemovePane_CPaneDivider            docking/CPaneDivider.cpp:35 (side-table body)
//   GetRootContainerRect_CPaneDivider  docking/CPaneDivider.cpp:89 (side-table body;
//                                      NOTE its two pointer parameters are declared
//                                      in the order (CRect* pRet, void* pThis))
//   StoreRecentDockSiteInfo_CMultiPaneFrameWnd  docking/CMultiPaneFrameWnd.cpp:180
//                                      (stub whose parameter list omits `this`)
//   GetThisClass_CPaneDivider is a real descriptor getter.
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneDivider__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__RemovePane_CPaneDivider__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane);
extern "C" void MS_ABI impl__GetRootContainerRect_CPaneDivider__QEAA_AVCRect__XZ(CRect* pRet, void* pThis);
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CMultiPaneFrameWnd__UEAAXPEAVCPane___Z(void* pThis, void* pBar);
//   ReplacePane_CMultiPaneFrameWnd     docking/CMultiPaneFrameWnd.cpp:158 (stub, no `this`)
//   GetParentFrame_CWnd                core/window/Thunks.cpp:1353 (real body)
//   GetThisClass_CMFCPropertyGridToolTipCtrl  propertygrid/RuntimeClasses.cpp:75
//   InsertPane_CPaneDivider            docking/CPaneDivider.cpp:193 (stub, no `this`)
//   InsertPane_CBasePane               docking/CBasePane.cpp:1018 (stub, carries `this`)
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPropertyGridToolTipCtrl__SAPEAUCRuntimeClass__XZ();
extern "C" int MS_ABI impl__InsertPane_CPaneDivider__UEAAHPEAVCDockablePane__0KPEBUtagRECT___Z(
    void* pThis, CDockablePane* pPane, CDockablePane* pTarget, unsigned long dwAlignment, const RECT* lpRect);
extern "C" int MS_ABI impl__InsertPane_CBasePane__QEAAHPEAV1_0H_Z(
    CBasePane* pThis, CBasePane* pControlBar, CBasePane* pTarget, int bAfter);
//   Serialize_CBasePane                docking/CBasePane.cpp:1436 (stub, no `this`)
//   CalcRecentDockedRect_CPane         docking/CPane.cpp:416 (real body)
//   Read_CArchive / Write_CArchive     core/runtime/Thunks.cpp:1174 / :1184 (real bodies)
//   AfxThrowArchiveException           detail/MfcExceptionsSupport.cpp:618
extern "C" void MS_ABI impl__Serialize_CBasePane__UEAAXAEAVCArchive___Z(CBasePane* pThis, CArchive& ar);
extern "C" void MS_ABI impl__CalcRecentDockedRect_CPane__QEAAXXZ(CPane* pThis);
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* lpBuf, unsigned int nMax);
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* lpBuf, unsigned int nMax);
extern "C" void MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(int cause, const wchar_t* lpszArchiveName);
// Sibling thunks defined further down in THIS file.
extern "C" void MS_ABI impl__Slide_CDockablePane__UEAAXHH_Z(CDockablePane* pThis, int bSlideOut, int bUseTimer);
extern "C" void MS_ABI impl__OnAfterDock_CDockablePane__MEAAXPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, CBasePane* pBar, const RECT* lpRect, int dockMethod);
extern "C" void MS_ABI impl__ReplacePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__0_Z(
    void* pThis, void* pOldBar, void* pNewBar);
// Sibling thunks defined further down in THIS file.
extern "C" void MS_ABI impl__SetAutoHideParents_CDockablePane__QEAAXPEAVCMFCAutoHideBar__PEAVCMFCAutoHideButton___Z(
    CDockablePane* pThis, void* pAutoHideBar, void* pAutoHideButton);

namespace {
// --- CDockablePane instance fields reached through the padding blob ----------
// OpenMFC's CDockablePane declares no members; it carries
// `char _dockablepane_padding[128]` (include/openmfc/afxmfc.h:476). Measured
// with the phase4 mingw flags (see checkfile.sh) that blob starts at offset
// 1012 == 0x3F4 -- the Itanium ABI folds it into CPane's four bytes of tail
// padding -- and sizeof(CDockablePane) is 1144. So every retail field from
// +0x3F8 (the first byte past the 1016-byte CPane sub-object) up to +0x473 has
// real storage inside the object and can be used at its retail offset.
//
// +0x474 and above cannot: sizeof(CBaseTabbedPane) measures 1240 with its own
// `_basetabbedpane_padding[96]` at offset 1140 == 0x474, so a write to +0x474
// on a CTabbedPane lands in the derived class's blob, and +0x478 is one past
// the end of the object. That is why m_ahSlideMode (+0x474), m_pAutoHideBar
// (+0x480), m_arrButtons (+0x488) and everything above them stay unreachable.
static_assert(sizeof(CPane) == 1016, "CPane layout changed; recheck the offsets below");
static_assert(sizeof(CDockablePane) == 1144, "CDockablePane size changed; recheck the offsets below");

const size_t kOffPrepareToFloat = 0x408;   // bool   m_bPrepareToFloat
const size_t kOffLastPercent    = 0x410;   // int    m_nLastPercent
const size_t kOffEnableAutoHideAll = 0x420; // BOOL  m_bEnableAutoHideAll
const size_t kOffSlideDelta     = 0x454;   // int    m_nSlideDelta
const size_t kOffRectRestored   = 0x458;   // CRect  m_rectRestored
const size_t kOffCyGripper      = 0x40c;   // int    m_cyGripper
const size_t kOffActive         = 0x41c;   // BOOL   m_bActive
const size_t kOffAutoHideConditionTimerID = 0x440;  // UINT_PTR m_nAutoHideConditionTimerID
const size_t kOffSlideTimer     = 0x448;   // UINT_PTR m_nSlideTimer
const size_t kOffSlideStep      = 0x450;   // int    m_nSlideStep
const size_t kOffIsSliding      = 0x468;   // BOOL   m_bIsSliding
const size_t kOffIsResizing     = 0x46c;   // BOOL   m_bIsResizing
const size_t kOffIsHiding       = 0x470;   // BOOL   m_bIsHiding
const size_t kOffFirstUnusable  = 0x474;   // first retail offset with no storage here

static_assert(kOffPrepareToFloat >= sizeof(CPane), "field would alias the CPane sub-object");
static_assert(kOffLastPercent >= sizeof(CPane), "field would alias the CPane sub-object");
static_assert(kOffEnableAutoHideAll >= sizeof(CPane), "field would alias the CPane sub-object");
static_assert(kOffRectRestored + 16 <= kOffFirstUnusable, "field runs past the usable window");
static_assert(kOffCyGripper >= sizeof(CPane), "field would alias the CPane sub-object");
static_assert(kOffIsHiding + 4 <= kOffFirstUnusable, "field runs past the usable window");
static_assert(kOffIsResizing + 4 <= kOffFirstUnusable, "field runs past the usable window");
static_assert(kOffFirstUnusable + 4 <= sizeof(CDockablePane), "usable window computed wrongly");

template <typename T>
inline T& DockablePaneField(CDockablePane* pThis, size_t off) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(pThis) + off);
}
template <typename T>
inline const T& DockablePaneField(const CDockablePane* pThis, size_t off) {
    return *reinterpret_cast<const T*>(reinterpret_cast<const char*>(pThis) + off);
}

// CBRS_ALIGN_* from atlmfc/include/afxwin.h; the retail switches below compare
// GetCurrentAlignment() against exactly these four values.
const unsigned long kAlignLeft   = 0x1000UL;
const unsigned long kAlignTop    = 0x2000UL;
const unsigned long kAlignRight  = 0x4000UL;
const unsigned long kAlignBottom = 0x8000UL;

// AFX_CS_STATUS from atlmfc/include/afxpane.h:43.
const int kCsNothing        = 0;   // CS_NOTHING
const int kCsDelayDock      = 2;   // CS_DELAY_DOCK
const int kCsDelayDockToTab = 3;   // CS_DELAY_DOCK_TO_TAB
// AFX_DOCK_TYPE bits from atlmfc/include/afxglobals.h:38.
const int kDockTypeSmart = 0x80;   // DT_SMART
// CSmartDockingStandaloneGuide::sdCMIDDLE from atlmfc/include/afxsmartdockingguide.h:74
// (sdLEFT == 0 ... sdCBOTTOM == 7, sdCMIDDLE == 8).
const int kSdCMiddle = 8;
// Timer ids from atlmfc/include/afxres.h:157-159.
const unsigned __int64 kTimerAutoHideSlideIn  = 0xEC01;  // AFX_TIMER_ID_AUTO_HIDE_SLIDE_IN_EVENT
const unsigned __int64 kTimerAutoHideSlideOut = 0xEC02;  // AFX_TIMER_ID_AUTO_HIDE_SLIDE_OUT_EVENT
const unsigned __int64 kTimerCheckAutoHide    = 0xEC03;  // AFX_TIMER_ID_CHECK_AUTO_HIDE_CONDITION

// m_pAutoHideBar (+0x480) has no storage in the 1144-byte object (see the note
// above), but this file's C++ CDockablePane::SetAutoHideMode already records
// the bar it is handed in the shared docking side table
// (openmfc::docking_state::PaneCoreState::autoHideBar, which
// g_paneCoreState_Mfccore and g_paneCoreState_Cbarcore both alias). The bodies
// below that retail drives off m_pAutoHideBar read it from there.
inline void* AutoHideBarOf(const CDockablePane* pThis) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(pThis);
    return it != g_paneCoreState_Mfccore.end() ? it->second.autoHideBar : nullptr;
}

// --- File-private side table for retail fields with no storage here ---------
// The retail members from +0x474 up (see the offset note near the top) have no
// bytes inside OpenMFC's 1144-byte object, so the ones the bodies below need
// are kept in this table, keyed by `this`, exactly as m_pAutoHideBar already is
// in the shared PaneCoreState. The entry is erased by ~CDockablePane below
// (which the exported ??1CDockablePane@@UEAA@XZ thunk at docking/Thunks.cpp:1148
// runs), so a new pane constructed at a recycled address starts with a NULL
// slider and button exactly as retail's zero-initialised members do. This is a
// storage deviation only: the values and the code that reads them are retail's.
struct DockablePaneExtra {
    HWND  hDefaultSlider  = nullptr;   // retail +0x4C0 m_hDefaultSlider
    void* pAutoHideButton = nullptr;   // retail +0x478 m_pAutoHideButton
};
inline std::mutex& DockablePaneExtraMutex() {
    static std::mutex m;
    return m;
}
inline std::unordered_map<const void*, DockablePaneExtra>& DockablePaneExtras() {
    static std::unordered_map<const void*, DockablePaneExtra> t;
    return t;
}
inline DockablePaneExtra ExtraOf(const CDockablePane* pThis) {
    std::lock_guard<std::mutex> lock(DockablePaneExtraMutex());
    auto it = DockablePaneExtras().find(pThis);
    return it != DockablePaneExtras().end() ? it->second : DockablePaneExtra();
}
inline void SetDefaultSliderOf(const CDockablePane* pThis, HWND h) {
    std::lock_guard<std::mutex> lock(DockablePaneExtraMutex());
    DockablePaneExtras()[pThis].hDefaultSlider = h;
}
inline void SetAutoHideButtonOf(const CDockablePane* pThis, void* p) {
    std::lock_guard<std::mutex> lock(DockablePaneExtraMutex());
    DockablePaneExtras()[pThis].pAutoHideButton = p;
}
inline void EraseExtraOf(const CDockablePane* pThis) {
    std::lock_guard<std::mutex> lock(DockablePaneExtraMutex());
    DockablePaneExtras().erase(pThis);
}
}  // namespace

namespace {
// AFX_DOCK_TYPE::DT_IMMEDIATE / DT_STANDARD, from atlmfc/include/afxglobals.h.
const int kDockTypeImmediate = 1;
const int kDockTypeStandard  = 2;
}  // namespace


// Symbol: ?CreateObject@CDockablePane@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CDockablePane__SAPEAVCObject__XZ() {
    return new CDockablePane();
}
// Symbol: ?GetRuntimeClass@CDockablePane@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDockablePane__UEBAPEAUCRuntimeClass__XZ(const CDockablePane*) {
    return CDockablePane::GetThisClass();
}
// Symbol: ?GetThisClass@CDockablePane@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ() {
    return CDockablePane::GetThisClass();
}
// Symbol: ?Create@CDockablePane@@UEAAHPEB_WPEAVCWnd@@AEBUtagRECT@@HIKKKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
    CDockablePane* pThis, const wchar_t* lpszCaption, CWnd* pParentWnd, const RECT& rect,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle,
    unsigned long, CCreateContext* pContext) {
    if (pThis == nullptr) return FALSE;
    CRect useRect = NormalizeRect(&rect, 240, 140);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
        PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
        state.parent = pParentWnd;
        state.recentRect = useRect;
        state.visible = (dwStyle & WS_VISIBLE) ? TRUE : FALSE;
        state.canFloat = TRUE;
        state.canAutoHide = TRUE;
        state.hasGripper = bHasGripper ? TRUE : FALSE;
        state.tabbed = (dwTabbedStyle != 0) ? TRUE : FALSE;
        if (state.minSize.cx <= 0) state.minSize.cx = 120;
        if (state.minSize.cy <= 0) state.minSize.cy = 80;
    }
    return TryCreatePaneWindow(static_cast<CWnd*>(pThis), L"STATIC", dwStyle, useRect, pParentWnd, nID, pContext);
}
// Symbol: ?Create@CDockablePane@@UEAAHPEB_WPEAVCWnd@@VCSize@@HIKKK@Z
extern "C" int MS_ABI impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__VCSize__HIKKK_Z(
    CDockablePane* pThis, const wchar_t* lpszCaption, CWnd* pParentWnd, CSize sizeDefault,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle, unsigned long dwControlBarStyle) {
    CRect rect(0, 0, sizeDefault.cx > 0 ? sizeDefault.cx : 240, sizeDefault.cy > 0 ? sizeDefault.cy : 140);
    int created = impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
        pThis, lpszCaption, pParentWnd, *static_cast<const RECT*>(rect), bHasGripper, nID, dwStyle, dwTabbedStyle, dwControlBarStyle, nullptr);
    if (created && pThis != nullptr) {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
        g_paneCoreState_Cbarcore[pThis].minSize = sizeDefault;
    }
    return created;
}
// Symbol: ?CreateEx@CDockablePane@@UEAAHKPEB_WPEAVCWnd@@AEBUtagRECT@@HIKKKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CDockablePane__UEAAHKPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
    CDockablePane* pThis, unsigned long, const wchar_t* lpszCaption, CWnd* pParentWnd, const RECT& rect,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle,
    unsigned long dwControlBarStyle, CCreateContext* pContext) {
    return impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
        pThis, lpszCaption, pParentWnd, rect, bHasGripper, nID, dwStyle, dwTabbedStyle, dwControlBarStyle, pContext);
}
// Symbol: ?Dock@CDockablePane@@MEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__Dock_CDockablePane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, CBasePane* pTargetBar, const RECT* lpRect, int dockMethod) {
    return impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
        reinterpret_cast<CPane*>(pThis), pTargetBar, lpRect, dockMethod);
}
// Symbol: ?OnBeforeFloat@CDockablePane@@UEAAHAEAVCRect@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__OnBeforeFloat_CDockablePane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, CRect& rect, int dockMethod) {
    return impl__OnBeforeFloat_CPane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(
        reinterpret_cast<CPane*>(pThis), rect, dockMethod);
}
// Symbol: ?DockToFrameWindow@CDockablePane@@UEAAHKPEBUtagRECT@@KPEAVCBasePane@@HH@Z
extern "C" int MS_ABI impl__DockToFrameWindow_CDockablePane__UEAAHKPEBUtagRECT__KPEAVCBasePane__HH_Z(
    CDockablePane* pThis, unsigned long, const RECT* lpRect, unsigned long, CBasePane* pTargetBar, int, int) {
    if (pThis == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
    state.parent = pTargetBar != nullptr ? static_cast<CWnd*>(pTargetBar) : state.parent;
    state.recentRect = NormalizeRect(lpRect, 240, 140);
    state.tabbed = FALSE;
    return TRUE;
}
// Symbol: ?ShowPane@CDockablePane@@UEAAXHHH@Z
extern "C" void MS_ABI impl__ShowPane_CDockablePane__UEAAXHHH_Z(CDockablePane* pThis, int bShow, int bDelay, int bActivate) {
    impl__ShowPane_CBasePane__UEAAXHHH_Z(reinterpret_cast<CBasePane*>(pThis), bShow, bDelay, bActivate);
}
CDockablePane::CDockablePane() {
    memset(_dockablepane_padding, 0, sizeof(_dockablepane_padding));
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];
    state.canFloat = TRUE;
    state.canAutoHide = TRUE;
    state.hasGripper = TRUE;
}
CDockablePane::~CDockablePane() {
    EraseExtraOf(this);   // drop the file-private m_hDefaultSlider / m_pAutoHideButton mirror
}
BOOL CDockablePane::CanBeAttached() const { return TRUE; }
BOOL CDockablePane::CanAutoHide() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.canAutoHide : TRUE;
}
void CDockablePane::EnableAutohideAll() {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[this].canAutoHide = TRUE;
}
CMFCAutoHideBar* CDockablePane::SetAutoHideMode(int bAutoHideMode, unsigned long dwAlignment, void* pAutoHideBar, int) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];
    state.autoHideMode = bAutoHideMode ? TRUE : FALSE;
    state.autoHideAlignment = bAutoHideMode ? dwAlignment : 0;
    state.autoHideBar = bAutoHideMode ? pAutoHideBar : nullptr;
    state.canAutoHide = TRUE;
    return reinterpret_cast<CMFCAutoHideBar*>(state.autoHideBar);
}
BOOL CDockablePane::IsAutoHideMode() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.autoHideMode : FALSE;
}
BOOL CDockablePane::IsTabbed() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.tabbed : FALSE;
}
// CDockablePane::m_sizeDragSensitivity -- the static CSize data export. Retail
// has no static initializer for it in .data; it is filled in by the dynamic
// initializer at RVA 0x12a0, whose whole body is
//     ecx = 0x45 ; call *GetSystemMetrics ; -> ebx
//     ecx = 0x44 ; call *GetSystemMetrics ; -> eax
//     [0x3ba2f8] = eax    ; m_sizeDragSensitivity.cx
//     [0x3ba2fc] = ebx    ; m_sizeDragSensitivity.cy
// (0x1802c4c50 resolves to USER32!GetSystemMetrics; 0x44 == SM_CXDRAG and
// 0x45 == SM_CYDRAG). Reproduced here with an equivalent dynamic initializer.
// Symbol: ?m_sizeDragSensitivity@CDockablePane@@1VCSize@@A
extern "C" SIZE impl__m_sizeDragSensitivity_CDockablePane__1VCSize__A = {
    ::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG)
};

// CDockablePane::AdjustPaneToPaneContainer -- transcribed from retail entry
// RVA 0x44a30 (mfc140u; 0x44880 in mfc140), fully decoded:
//     CRect rectContainer = pSlider->GetRootContainerRect();   // RVA 0xad530; %rcx =
//                                                      // pSlider, %rdx = hidden CRect*
//     if (::IsRectEmpty(&rectContainer)) return;       // 0x1802c52c8
//     CFrameWnd* pFrame = GetParentFrame();            // RVA 0x28c680
//     if (pFrame == NULL) return;
//     pFrame->ScreenToClient(&rectContainer);          // RVA 0x2a11f0
//     CRect rectWnd(0,0,0,0); ::GetWindowRect(m_hWnd, &rectWnd);   // 0x1802c5370
//     pFrame->ScreenToClient(&rectWnd);
//     CRect rectUnion; ::UnionRect(&rectUnion, &rectWnd, &rectContainer);   // 0x1802c4d08
//     if (::EqualRect(&rectUnion, &rectContainer)) return;   // 0x1802c52e8 -- already inside
//     ::OffsetRect(&rectWnd, rectContainer.left - rectWnd.left,
//                  rectContainer.top - rectWnd.top);   // 0x1802c5318
//     if (rectWnd.Width()  > rectContainer.Width())  rectWnd.right  = rectWnd.left + rectContainer.Width();
//     if (rectWnd.Height() > rectContainer.Height()) rectWnd.bottom = rectWnd.top  + rectContainer.Height();
//     SetWindowPos(NULL, rectWnd.left, rectWnd.top, rectContainer.Width(),
//                  rectContainer.Height(), SWP_NOZORDER | SWP_NOACTIVATE /*0x14*/, NULL);  // slot 0x480
// (All five IAT slots resolved with iat.py. The clamped right/bottom are dead:
// the SetWindowPos size is the container's, not rectWnd's -- %r8d/%r9d hold
// rectWnd.left/top and [rsp+0x20]/[rsp+0x28] hold %r10d/%edx, the container's
// width and height.)
// The container rect comes from the sibling export
// impl__GetRootContainerRect_CPaneDivider__QEAA_AVCRect__XZ, which
// docking/CPaneDivider.cpp:89 defines -- and its own C++ wrapper
// CPaneDivider::GetRootContainerRect at :136 calls -- with its two pointer
// parameters in the order (CRect* pRet, void* pThis). That is the OPPOSITE of
// the MS x64 order the retail call above uses (`this` in %rcx, the hidden
// return slot in %rdx), so an MSVC-compiled client calling that export would
// have its CRect written over the divider's vfptr; that defect is reported as
// a headerRequest against CPaneDivider.cpp. This body follows the order the
// definition currently has, exactly as the C++ wrapper does; whoever corrects
// the definition must swap this call too (grep for the symbol).
// Deviations: NULL checks on `this` and pSlider (retail dereferences both);
// SetWindowPos dispatches statically to the CBasePane export.
// Signature corrected to carry `this`.
// Symbol: ?AdjustPaneToPaneContainer@CDockablePane@@MEAAXPEAVCPaneDivider@@@Z
extern "C" void MS_ABI impl__AdjustPaneToPaneContainer_CDockablePane__MEAAXPEAVCPaneDivider___Z(
    CDockablePane* pThis, void* pSlider) {
    if (pThis == nullptr || pSlider == nullptr) return;   // deviation: retail has no NULL check
    CRect rectContainer(0, 0, 0, 0);
    // NOTE: (pRet, pThis) is the order docking/CPaneDivider.cpp:89 declares -- see above.
    impl__GetRootContainerRect_CPaneDivider__QEAA_AVCRect__XZ(&rectContainer, pSlider);
    RECT rcContainer;
    rcContainer.left = rectContainer.left;
    rcContainer.top = rectContainer.top;
    rcContainer.right = rectContainer.right;
    rcContainer.bottom = rectContainer.bottom;
    if (::IsRectEmpty(&rcContainer)) return;

    CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(static_cast<const CWnd*>(pThis));
    if (pFrame == nullptr) return;
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(static_cast<const CWnd*>(pFrame), &rcContainer);

    RECT rcWnd = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rcWnd);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(static_cast<const CWnd*>(pFrame), &rcWnd);

    RECT rcUnion = {0, 0, 0, 0};
    ::UnionRect(&rcUnion, &rcWnd, &rcContainer);
    if (::EqualRect(&rcUnion, &rcContainer)) return;   // already inside the container

    ::OffsetRect(&rcWnd, rcContainer.left - rcWnd.left, rcContainer.top - rcWnd.top);
    const int cxContainer = rcContainer.right - rcContainer.left;
    const int cyContainer = rcContainer.bottom - rcContainer.top;
    if (rcWnd.right - rcWnd.left > cxContainer) rcWnd.right = rcWnd.left + cxContainer;
    if (rcWnd.bottom - rcWnd.top > cyContainer) rcWnd.bottom = rcWnd.top + cyContainer;
    impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
        static_cast<CBasePane*>(pThis), nullptr, rcWnd.left, rcWnd.top, cxContainer, cyContainer,
        SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
}

// STUB. Retail entry RVA 0x436e0 (mfc140u; 0x43530 in mfc140), ~1088 bytes,
// the largest body in this class; not transcribed. It touches the
// CDockablePane fields at +0x450 (m_nSlideStep), +0x4b0
// (m_pTabbedControlBarRTC) and +0x4c0 (m_hDefaultSlider) and drives
// CreateTabbedPane (below, itself blocked on +0x4b0), the CTabbedPane it would
// create and the CPaneDivider ReplacePane/AddPane virtuals. Blocked: +0x4b0
// has no storage (m_hDefaultSlider now lives in the file-private
// DockablePaneExtra table, but the tabbed-pane creation it hinges on cannot
// run without the runtime class held at +0x4b0).
// Signature corrected: the generated list dropped `this` and the first two
// parameters; the export returns the CDockablePane* the caller was attached to.
// Symbol: ?AttachToTabWnd@CDockablePane@@UEAAPEAV1@PEAV1@W4AFX_DOCK_METHOD@@HPEAPEAV1@@Z
extern "C" void* MS_ABI impl__AttachToTabWnd_CDockablePane__UEAAPEAV1_PEAV1_W4AFX_DOCK_METHOD__HPEAPEAV1__Z(
    CDockablePane* pThis, CDockablePane* pTabControlBarAttachTo, int dockMethod, int bSetActive,
    CDockablePane** ppTabbedControlBar) {
    (void)pThis;
    (void)pTabControlBarAttachTo;
    (void)dockMethod;
    (void)bSetActive;
    (void)ppTabbedControlBar;
    return nullptr;
}

// CDockablePane::CalcCorrectOffset -- transcribed from retail entry RVA 0x42c10
// (mfc140; 0x42dc0 in mfc140u). Register use: %rcx = this, %rdx = the hidden
// CPoint return slot (returned in %rax), %r8 = &rect (a by-value CRect is
// 16 bytes, so MSVC passes its address), %r9d = bIsRTL. The body is:
//     CRect rectBar(0,0,0,0);
//     ::GetWindowRect(m_pAutoHideBar->GetParentDockSite()->m_hWnd, &rectBar);
//                        // +0x480; slot 0x350 of the CMFCAutoHideBar vftable (mfc140
//                        // RVA 0x2d90d8) is the one-line getter at RVA 0x87e0,
//                        // `mov 0x128(%rcx),%rax` == CBasePane::m_pParentDockBar
//     GetDockSiteFrameWnd()->ScreenToClient(&rectBar);         // slot 0x348, RVA 0x2a11f0
//     CPoint pt(0,0);
//     switch (GetCurrentAlignment()) {                          // slot 0x338
//     case CBRS_ALIGN_LEFT: case CBRS_ALIGN_RIGHT:   // 0x42cfb / 0x42cca, same code
//         if (bIsRTL) { if (rect.left  > rectBar.right) pt.x = rectBar.right - rect.left; }
//         else        { if (rect.right < rectBar.left)  pt.x = rectBar.left  - rect.right; }
//         break;
//     case CBRS_ALIGN_TOP:    if (rect.top    > rectBar.bottom) pt.y = rectBar.bottom - rect.top;    break;
//     case CBRS_ALIGN_BOTTOM: if (rect.bottom < rectBar.top)    pt.y = rectBar.top    - rect.bottom; break;
//     }
//     return pt;
// (0x1802c5370 == USER32!GetWindowRect via iat.py; the `jle`/`jge` arms fall
// into the 8-byte zero store at 0x42d01.)
// m_pAutoHideBar comes from the docking side table (see AutoHideBarOf); the
// dock-site pointer on it is read as the CBasePane::m_pParentDockBar member,
// which OpenMFC's CMFCAutoHideBar (a CPane) declares at the retail offset.
// Deviations: NULL checks on `this`, the bar and its dock site (retail
// dereferences all three unchecked), and the two virtuals dispatch statically.
// Signature corrected: the generated stub dropped `this` and the hidden return
// slot.
// Symbol: ?CalcCorrectOffset@CDockablePane@@IEAA?AVCPoint@@VCRect@@H@Z
extern "C" POINT* MS_ABI impl__CalcCorrectOffset_CDockablePane__IEAA_AVCPoint__VCRect__H_Z(
    CDockablePane* pThis, POINT* pRet, const RECT* pRect, int bIsRTL) {
    if (pRet == nullptr) return pRet;
    pRet->x = 0;
    pRet->y = 0;
    if (pThis == nullptr || pRect == nullptr) return pRet;

    RECT rectBar = {0, 0, 0, 0};
    CBasePane* pBar = static_cast<CBasePane*>(AutoHideBarOf(pThis));
    // CDockBar is only forward-declared in the OpenMFC headers, so the
    // CWnd-derived dock site is reached with reinterpret_cast, the same idiom
    // CBasePane.cpp:1303 uses for this member.
    CWnd* pDockSite = pBar != nullptr ? reinterpret_cast<CWnd*>(pBar->m_pParentDockBar) : nullptr;
    if (pDockSite == nullptr) return pRet;   // deviation: retail faults here
    ::GetWindowRect(pDockSite->m_hWnd, &rectBar);
    CWnd* pFrame = impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(static_cast<const CBasePane*>(pThis));
    if (pFrame != nullptr) {   // deviation: retail calls ScreenToClient on it unchecked
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pFrame, &rectBar);
    }

    switch (impl__GetCurrentAlignment_CBasePane__UEBAKXZ(static_cast<const CBasePane*>(pThis))) {
    case kAlignLeft:
    case kAlignRight:
        if (bIsRTL) {
            if (pRect->left > rectBar.right) pRet->x = rectBar.right - pRect->left;
        } else {
            if (pRect->right < rectBar.left) pRet->x = rectBar.left - pRect->right;
        }
        break;
    case kAlignTop:
        if (pRect->top > rectBar.bottom) pRet->y = rectBar.bottom - pRect->top;
        break;
    case kAlignBottom:
        if (pRect->bottom < rectBar.top) pRet->y = rectBar.top - pRect->bottom;
        break;
    default:
        break;
    }
    return pRet;
}

// CDockablePane::CalcFixedLayout -- transcribed from retail. This export is
// absent from mfc140_rva_symbols.json only because that map is keyed by RVA and
// the linker ICF-folded this body onto another name; it is NOT missing from the
// DLL. In mfc140u.dll the export resolves to RVA 0x92b0 (shared with
// ?CalcFixedLayout@CMFCAutoHideBar@@UEAA?AVCSize@@HH@Z, whose independent
// transcription in phase4/src/featurepack/docking/CMFCAutoHideBar.cpp:222 reads
// the same way -- though note that sibling takes its two leading pointers in the
// opposite order, which the disassembly below does not support), and the whole
// body is:
//     mov 0x40(%rcx),%rcx        ; this->m_hWnd
//     xorps %xmm0,%xmm0 ; movdqu %xmm0,0x20(%rsp)   ; RECT rc = {0,0,0,0}
//     lea 0x20(%rsp),%rdx ; call *0x1802c6c08       ; ::GetWindowRect(m_hWnd,&rc)
//     mov 0x28(%rsp),%ecx ; sub 0x20(%rsp),%ecx ; mov %ecx,(%rbx)   ; cx = right-left
//     mov 0x2c(%rsp),%ecx ; sub 0x24(%rsp),%ecx ; mov %ecx,0x4(%rbx); cy = bottom-top
//     mov %rbx,%rax                                 ; return the hidden slot
// (IAT slot 0x1802c6c08 resolved with iatu.py == USER32!GetWindowRect. The
// result is NOT checked; the zeroed rect is used as-is on failure.) Both BOOL
// arguments are ignored.
// Signature corrected: the generated stub dropped `this` and the hidden CSize
// return slot. MSVC x64 passes `this` in %rcx and the hidden slot in %rdx and
// returns that same pointer in %rax -- confirmed here by `mov 0x40(%rcx),%rcx`
// (an HWND member read, so %rcx is `this`) and `mov %rbx,%rax` where %rbx was
// loaded from %rdx.
// Symbol: ?CalcFixedLayout@CDockablePane@@UEAA?AVCSize@@HH@Z
extern "C" SIZE* MS_ABI impl__CalcFixedLayout_CDockablePane__UEAA_AVCSize__HH_Z(
    CDockablePane* pThis, SIZE* pRet, int bStretch, int bHorz) {
    (void)bStretch;   // retail reads neither argument
    (void)bHorz;
    if (pRet == nullptr) return pRet;
    RECT rc = {0, 0, 0, 0};
    if (pThis != nullptr) {   // deviation: retail has no NULL check
        ::GetWindowRect(pThis->m_hWnd, &rc);
    }
    pRet->cx = rc.right - rc.left;
    pRet->cy = rc.bottom - rc.top;
    return pRet;
}

// CDockablePane::CalcRectForSliding -- transcribed from retail entry RVA
// 0x42a70. It grows or shrinks `rect` along the auto-hide edge by
// m_nSlideDelta (+0x454), clamps the result to the extent recorded in
// m_rectRestored (+0x458..+0x467), then slides `rectResult` so that its own
// (unchanged) width/height sits against the same clamped edge:
//     if (!IsAutoHideMode()) return;                  // vtable slot 0x3c8
//     int d = m_nSlideDelta;
//     switch (GetCurrentAlignment()) {                // vtable slot 0x338
//     case CBRS_ALIGN_LEFT:                           // 0x42bb6
//         if (bGrow) { rect.right += d;
//                      if (rect.Width() > m_rectRestored.Width())
//                          rect.right = rect.left + m_rectRestored.Width(); }
//         else       { rect.right -= d;
//                      if (rect.right < rect.left) rect.right = rect.left; }
//         rectResult.right = rectResult.Width() + rect.right;
//         rectResult.left  = rect.right;              break;
//     case CBRS_ALIGN_TOP:                            // 0x42b66, same on the y axis
//         ... rectResult.bottom = rectResult.Height() + rect.bottom;
//             rectResult.top    = rect.bottom;        break;
//     case CBRS_ALIGN_RIGHT:                          // 0x42b24
//         if (bGrow) { rect.left -= d;
//                      if (rect.Width() > m_rectRestored.Width())
//                          rect.left = rect.right - m_rectRestored.Width(); }
//         else       { rect.left += d;
//                      if (rect.left > rect.right) rect.left = rect.right; }
//         rectResult.right = rect.left;
//         rectResult.left  = rect.left - rectResult.Width();   break;
//     case CBRS_ALIGN_BOTTOM:                         // 0x42ad8, same on the y axis
//         ... rectResult.bottom = rect.top;
//             rectResult.top    = rect.top - rectResult.Height();  break;
//     default: return;
//     }
// (In each arm the width/height of rectResult is read BEFORE the assignment
// that overwrites one of its edges -- e.g. 0x42bf4 `mov 0x8(%rdi),%eax ; sub
// (%rdi),%eax ; add %ecx,%eax ; mov %eax,0x8(%rdi) ; mov %ecx,(%rdi)`.)
// Both fields are inside the padding blob (see the offset note near the top) and
// are read at their retail offsets. As with OffsetRectForSliding above, the
// sibling CBasePane::GetCurrentAlignment thunk is still a stub returning 0, so
// today no case matches and the caller's rects are left alone -- retail's own
// behaviour for an alignment of 0.
// Signature corrected to carry `this`.
// Symbol: ?CalcRectForSliding@CDockablePane@@IEAAXAEAVCRect@@0H@Z
extern "C" void MS_ABI impl__CalcRectForSliding_CDockablePane__IEAAXAEAVCRect__0H_Z(
    CDockablePane* pThis, CRect& rect, CRect& rectResult, int bGrow) {
    if (pThis == nullptr) return;
    if (!pThis->IsAutoHideMode()) return;

    const int nDelta = DockablePaneField<int>(pThis, kOffSlideDelta);
    const RECT& rectRestored = DockablePaneField<RECT>(pThis, kOffRectRestored);

    switch (impl__GetCurrentAlignment_CBasePane__UEBAKXZ(static_cast<const CBasePane*>(pThis))) {
    case kAlignLeft: {
        if (bGrow) {
            rect.right += nDelta;
            const int nMax = rectRestored.right - rectRestored.left;
            if (rect.right - rect.left > nMax) rect.right = rect.left + nMax;
        } else {
            rect.right -= nDelta;
            if (rect.right < rect.left) rect.right = rect.left;
        }
        const int nWidth = rectResult.right - rectResult.left;
        rectResult.right = rect.right + nWidth;
        rectResult.left = rect.right;
        break;
    }
    case kAlignTop: {
        if (bGrow) {
            rect.bottom += nDelta;
            const int nMax = rectRestored.bottom - rectRestored.top;
            if (rect.bottom - rect.top > nMax) rect.bottom = rect.top + nMax;
        } else {
            rect.bottom -= nDelta;
            if (rect.bottom < rect.top) rect.bottom = rect.top;
        }
        const int nHeight = rectResult.bottom - rectResult.top;
        rectResult.bottom = rect.bottom + nHeight;
        rectResult.top = rect.bottom;
        break;
    }
    case kAlignRight: {
        if (bGrow) {
            rect.left -= nDelta;
            const int nMax = rectRestored.right - rectRestored.left;
            if (rect.right - rect.left > nMax) rect.left = rect.right - nMax;
        } else {
            rect.left += nDelta;
            if (rect.left > rect.right) rect.left = rect.right;
        }
        const int nWidth = rectResult.right - rectResult.left;
        rectResult.right = rect.left;
        rectResult.left = rect.left - nWidth;
        break;
    }
    case kAlignBottom: {
        if (bGrow) {
            rect.top -= nDelta;
            const int nMax = rectRestored.bottom - rectRestored.top;
            if (rect.bottom - rect.top > nMax) rect.top = rect.bottom - nMax;
        } else {
            rect.top += nDelta;
            if (rect.top > rect.bottom) rect.top = rect.bottom;
        }
        const int nHeight = rectResult.bottom - rectResult.top;
        rectResult.bottom = rect.top;
        rectResult.top = rect.top - nHeight;
        break;
    }
    default:
        return;
    }
}

// CDockablePane::CanAcceptMiniFrame -- transcribed from retail entry RVA
// 0x45ed0. The whole body swaps the two arguments and tail-jumps through the
// mini frame's vtable at byte offset 0x2d8:
//     mov (%rdx),%rax ; mov %rdx,%r8 ; mov %rcx,%rdx ; mov %r8,%rcx
//     mov 0x2d8(%rax),%rax ; rex.W jmp *...
// i.e. `return pMiniFrame->{slot 0x2d8}(this);`. Slot 0x2d8 of the
// CPaneFrameWnd vftable (RVA 0x2f5ad8) holds
// ?CanBeDockedToPane@CPaneFrameWnd@@UEBAHPEBVCDockablePane@@@Z (RVA 0xb3770),
// so the call is `pMiniFrame->CanBeDockedToPane(this)`.
// Deviation: OpenMFC models no CPaneFrameWnd vtable, so this dispatches
// statically to the CPaneFrameWnd export instead of virtually; a
// CMultiPaneFrameWnd argument would run its override (RVA 0x94be0) in retail.
// Symbol: ?CanAcceptMiniFrame@CDockablePane@@UEBAHPEAVCPaneFrameWnd@@@Z
extern "C" int MS_ABI impl__CanAcceptMiniFrame_CDockablePane__UEBAHPEAVCPaneFrameWnd___Z(
    const CDockablePane* pThis, void* pMiniFrame) {
    if (pMiniFrame == nullptr) return FALSE;
    return impl__CanBeDockedToPane_CPaneFrameWnd__UEBAHPEBVCDockablePane___Z(pMiniFrame, pThis);
}

// CDockablePane::CheckAutoHideCondition -- transcribed from retail entry RVA
// 0x42570 (mfc140u; 0x423c0 in mfc140):
//     if (m_bActive) return FALSE;                          // +0x41c
//     if (m_bIsResizing) return FALSE;                      // +0x46c
//     if (!IsAutoHideMode()) return FALSE;                  // vtable slot 0x3c8
//     if (CMFCPopupMenu::m_pActivePopupMenu != NULL) return FALSE;   // RVA 0x3b6fe8
//     if (m_pToolTip != NULL && m_pToolTip->m_hWnd != NULL &&          // +0x4b8
//         ::IsWindowVisible(m_pToolTip->m_hWnd)) return FALSE;         // IAT 0x1802c5350
//     CRect rectButton = m_pAutoHideButton->m_rect;         // +0x478, 16 bytes at button+0x1c
//     m_pAutoHideBar->ClientToScreen(&rectButton);          // +0x480; RVA 0x2a1250
//     CPoint pt; ::GetCursorPos(&pt);                       // IAT 0x1802c5348
//     BOOL bIsOwn = FALSE;                                  // %esi
//     for (CWnd* p = CWnd::FromHandle(::WindowFromPoint(pt));   // IAT 0x1802c5268, RVA 0x289180
//          p != NULL; p = CWnd::FromHandle(::GetParent(p->m_hWnd))) {
//         if (p == this || p->m_hWnd == m_hDefaultSlider ||             // +0x4c0
//             p->IsKindOf(<RTC 0x2f9570 "CMFCPropertyGridToolTipCtrl">)) {
//             bIsOwn = TRUE; break;
//         }
//     }
//     CRect rectWnd;    ::GetWindowRect(m_hWnd, &rectWnd);
//     CRect rectSlider; ::GetWindowRect(m_hDefaultSlider, &rectSlider);
//     ::UnionRect(&rectWnd, &rectWnd, &rectSlider);         // IAT 0x1802c4d08
//     if (::PtInRect(&rectWnd, pt)) return FALSE;           // IAT 0x1802c5320
//     if (bIsOwn) return FALSE;
//     if (::PtInRect(&rectButton, pt)) return FALSE;
//     Slide(FALSE, TRUE);                                   // vtable slot 0x698, `xor %edx,%edx ; lea 0x1(%rsi),%r8d`
//     return TRUE;
// Slot 0x698 of the CDockablePane vftable (mfc140 RVA 0x2e3068) holds
// ?Slide@CDockablePane@@UEAAXHH@Z (RVA 0x42d30), exported by this file (still a
// stub -- see `stubbed`). m_bActive / m_bIsResizing are inside the reachable
// padding window; m_pAutoHideBar comes from the docking side table
// (AutoHideBarOf) and m_pAutoHideButton / m_hDefaultSlider from the
// file-private DockablePaneExtra table. The button rect is read through the
// CMFCAutoHideButton layout mirror in detail/CMFCAutoHideButtonSupport.h
// (RECT m_rect at +0x1c), duplicated here as a local offset because that
// header is not included by this unit.
// Deviations: (1) the tooltip test is dropped -- m_pToolTip has no storage
// here and OnCreate cannot create one, so it is never visible; (2) NULL checks
// on `this`, the button and the bar (retail dereferences both unchecked; note
// OpenMFC's CMFCAutoHideButton::CreateObject still returns NULL, so today the
// button is NULL and this returns FALSE at that check); (3) ::GetWindowRect on
// a NULL m_hDefaultSlider is skipped, leaving rectSlider empty -- the API
// fails and leaves the zeroed rect untouched in retail too, so the union is
// the same.
// Signature corrected to carry `this` (OnTimer below calls it).
// Symbol: ?CheckAutoHideCondition@CDockablePane@@MEAAHXZ
extern "C" int MS_ABI impl__CheckAutoHideCondition_CDockablePane__MEAAHXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check
    if (DockablePaneField<int>(pThis, kOffActive)) return FALSE;
    if (DockablePaneField<int>(pThis, kOffIsResizing)) return FALSE;
    if (!pThis->IsAutoHideMode()) return FALSE;
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA != nullptr) return FALSE;
    // m_pToolTip visibility test dropped -- see above.

    const DockablePaneExtra extra = ExtraOf(pThis);
    CWnd* pBar = static_cast<CWnd*>(static_cast<CBasePane*>(AutoHideBarOf(pThis)));
    if (extra.pAutoHideButton == nullptr || pBar == nullptr) return FALSE;   // deviation: retail faults
    const size_t kOffAutoHideButtonRect = 0x1c;   // CMFCAutoHideButton::m_rect (CMFCAutoHideButtonSupport.h)
    RECT rectButton;
    memcpy(&rectButton, reinterpret_cast<const char*>(extra.pAutoHideButton) + kOffAutoHideButtonRect, sizeof(rectButton));
    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(static_cast<const CWnd*>(pBar), &rectButton);

    POINT pt = {0, 0};
    ::GetCursorPos(&pt);
    int bIsOwn = FALSE;
    for (CWnd* p = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::WindowFromPoint(pt)); p != nullptr;
         p = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(p->m_hWnd))) {
        if (p == static_cast<CWnd*>(pThis) || p->m_hWnd == extra.hDefaultSlider ||
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(p),
                impl__GetThisClass_CMFCPropertyGridToolTipCtrl__SAPEAUCRuntimeClass__XZ())) {
            bIsOwn = TRUE;
            break;
        }
    }

    RECT rectWnd = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWnd);
    RECT rectSlider = {0, 0, 0, 0};
    if (extra.hDefaultSlider != nullptr) {   // retail calls it unchecked; a NULL HWND fails and leaves the zero rect
        ::GetWindowRect(extra.hDefaultSlider, &rectSlider);
    }
    ::UnionRect(&rectWnd, &rectWnd, &rectSlider);
    if (::PtInRect(&rectWnd, pt)) return FALSE;
    if (bIsOwn) return FALSE;
    if (::PtInRect(&rectButton, pt)) return FALSE;
    impl__Slide_CDockablePane__UEAAXHH_Z(pThis, FALSE, TRUE);
    return TRUE;
}

// STUB. Retail entry RVA 0x42720 (mfc140u; 0x42570 in mfc140). Its body touches
// CDockablePane fields at +0x458..+0x464 (m_rectRestored), +0x474
// (m_ahSlideMode, compared against AFX_AHSM_MOVE == 1 in every alignment arm)
// and +0x480 (m_pAutoHideBar, whose GetParentDockSite -- bar vtable slot
// 0x350 -- supplies the dock-site rect every arm compares against).
// Blocked: m_rectRestored is reachable and m_pAutoHideBar is in the side table,
// but m_ahSlideMode (+0x474) is not -- +0x474 is where a derived
// CBaseTabbedPane's own padding blob starts here -- and every arm branches on it.
// Signature corrected to carry `this` (OnTimer below calls it).
// Symbol: ?CheckStopSlideCondition@CDockablePane@@MEAAHH@Z
extern "C" int MS_ABI impl__CheckStopSlideCondition_CDockablePane__MEAAHH_Z(CDockablePane* pThis, int bDirection) {
    (void)pThis;
    (void)bDirection;
    return 0;
}

// STUB. Retail entry RVA 0x46590:
//     if (IsAutoHideMode()) return;                       // vtable slot 0x3c8
//     CWnd* pSite = GetDockSiteFrameWnd();                // vtable slot 0x348
//     if (pSite == NULL ||
//         !pSite->IsKindOf(<RTC at RVA 0x2ec3c8, "CMDIFrameWndEx">)) return;
//     if (IsTabbed()) {                                   // vtable slot 0x2e8
//         CBaseTabbedPane* p = GetParentTabbedPane();     // RVA 0xc9b0
//         if (p != NULL && p->IsKindOf(<RTC at RVA 0x3aa4a8, "CTabbedPane">))
//             p->{slot 0x5d0}(bActiveTabOnly);            // ConvertToTabbedDocument
//     } else {
//         pSite->{slot 0x3b8}(this);
//     }
// Blocked: slot 0x3b8 on the CMDIFrameWndEx dock site and slot 0x5d0 on the
// CTabbedPane are not modelled in OpenMFC (neither class has a retail-shaped
// vtable here), so neither arm can be dispatched.
// Signature corrected to carry `this`.
// Symbol: ?ConvertToTabbedDocument@CDockablePane@@UEAAXH@Z
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bActiveTabOnly) {
    (void)pThis;
    (void)bActiveTabOnly;
}

// CDockablePane::CopyState -- transcribed from retail entry RVA 0x46410, whose
// whole body is:
//     CPane::CopyState(pOrgBar);                          // RVA 0xa3610
//     m_rectRestored  = pOrgBar->m_rectRestored;          // 16 bytes at +0x458
//     m_ahSlideMode   = pOrgBar->GetAHSlideMode();        // vtable slot 0x6f0
//     m_nLastPercent  = pOrgBar->m_nLastPercent;          // +0x410
//     m_bEnableAutoHideAll = pOrgBar->IsAutohideAllEnabled();  // vtable slot 0x690
// Slots 0x6f0 and 0x690 are the one-instruction getters at RVA 0x12e90
// (`mov 0x474(%rcx),%eax`) and 0x12e10 (`mov 0x420(%rcx),%eax`), verified with
// vt.py on the CDockablePane vftable at RVA 0x2e3068, so both reduce to a plain
// field read on the source pane.
// PARTIAL, and the one deviation is deliberate: m_ahSlideMode lives at +0x474,
// which is exactly where a derived CBaseTabbedPane's own padding blob starts
// here (see the offset note near the top), so that field has no storage and is
// NOT copied. The other three are inside the reachable window and are copied at
// their retail offsets. m_rectRestored is not dead weight -- CalcRectForSliding
// and (indirectly) OffsetRectForSliding below read it back.
// The base call goes to the sibling CPane::CopyState thunk
// (phase4/src/featurepack/docking/CPane.cpp:131), which is itself still an empty
// stub, so today it contributes nothing.
// Signature corrected to carry `this`.
// Symbol: ?CopyState@CDockablePane@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__CopyState_CDockablePane__UEAAXPEAV1__Z(
    CDockablePane* pThis, CDockablePane* pOrgBar) {
    if (pThis == nullptr || pOrgBar == nullptr) return;  // deviation: retail has no NULL check
    impl__CopyState_CPane__UEAAXPEAV1__Z(static_cast<CPane*>(pThis), static_cast<CPane*>(pOrgBar));
    DockablePaneField<RECT>(pThis, kOffRectRestored) =
        DockablePaneField<RECT>(pOrgBar, kOffRectRestored);
    // m_ahSlideMode (+0x474) is skipped -- no storage here.
    DockablePaneField<int>(pThis, kOffLastPercent) =
        DockablePaneField<int>(pOrgBar, kOffLastPercent);
    DockablePaneField<int>(pThis, kOffEnableAutoHideAll) =
        DockablePaneField<int>(pOrgBar, kOffEnableAutoHideAll);
}

// STUB. Retail entry RVA 0x432c0 (mfc140u; 0x43110 in mfc140), a static (no
// `this`), fully decoded:
//     CPaneDivider* pSlider;
//     if (pSliderRTC != NULL) {
//         pSlider = DYNAMIC_DOWNCAST(CPaneDivider, pSliderRTC->CreateObject());   // RVA 0x233380,
//         pSlider->{+0x1c4} = 1;                        // RTC 0x2f4b58 "CPaneDivider"; unchecked NULL
//     } else {
//         pSlider = DYNAMIC_DOWNCAST(CPaneDivider, CPaneDivider::m_pSliderRTC->CreateObject());  // RVA 0x3aab10
//         pSlider->Init(TRUE, NULL);                    // RVA 0xab720, `lea 0x1(%r8),%edx ; xor %r8d`
//     }
//     DWORD dwStyle = ((dwAlignment & 0x5000 /*CBRS_ALIGN_LEFT|RIGHT*/) ? 2 : 1) | WS_VISIBLE;  // `bts $0x1c`
//     CRect rect(0, 0, CPaneDivider::m_nDefaultWidth, CPaneDivider::m_nDefaultWidth);   // RVA 0x3aab0c
//     if (!pSlider->CreateEx(0, dwStyle, rect, pParent, (UINT)-1, NULL)) {   // vtable slot 0x560
//         delete pSlider;                               // vtable slot 1 (deleting dtor, flag 1)
//         return NULL;
//     }
//     pSlider->SetPaneAlignment(dwAlignment);           // vtable slot 0x3d0: `andl $0xffff0fff,0x104 ; or %edx`
//     return pSlider;
// Blocked: OpenMFC's CPaneDivider (include/openmfc/afxmfc.h:1748) derives from
// CObject, not CBasePane, so it has no window, no vtable slots 0x560/0x3d0
// and no m_dwStyle at +0x104. The two exports the body needs DO exist in the
// tree but cannot carry it: ?CreateEx@CPaneDivider@@ is the generated stub at
// docking/CPaneDivider.cpp:170 (returns 0, parameter list omits `this`), so
// the creation would always take the `delete; return NULL` arm, and
// ?m_pSliderRTC@CPaneDivider@@ (core/runtime/StaticData.cpp:62) is a NULL
// pointer that nothing initialises, so the default arm would call
// CreateObject through NULL. Neither the creation nor the alignment store can
// be reproduced.
// Symbol: ?CreateDefaultPaneDivider@CDockablePane@@SAPEAVCPaneDivider@@KPEAVCWnd@@PEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__CreateDefaultPaneDivider_CDockablePane__SAPEAVCPaneDivider__KPEAVCWnd__PEAUCRuntimeClass___Z(
    unsigned long dwAlignment, CWnd* pParent, CRuntimeClass* pSliderRTC) {
    (void)dwAlignment;
    (void)pParent;
    (void)pSliderRTC;
    return nullptr;
}

// STUB. Retail entry RVA 0x43be0 (mfc140u; 0x43a30 in mfc140), decoded:
//     CRect rect; ::GetWindowRect(m_hWnd, &rect);
//     CWnd::FromHandle(::GetParent(m_hWnd))->ScreenToClient(&rect);   // RVA 0x2a11f0
//     CTabbedPane* p = (CTabbedPane*)m_pTabbedControlBarRTC->CreateObject();   // +0x4b0, RVA 0x233380
//     p->{+0x4e0} = 1;                                    // the new pane's first own field
//     DWORD dwStyle = GetStyle() | 0x40 /*CBRS_FLOAT_MULTI, afxres.h*/;   // RVA 0x2a75a0
//     if (!p->Create(L"" /*RVA 0x33ac36*/, CWnd::FromHandle(::GetParent(m_hWnd)), rect, TRUE,
//                    (UINT)-1, dwStyle, 0x20, 0xf, NULL))    // vtable slot 0x658
//         return NULL;
//     p->{+0x370} = {+0x370}; p->{+0x308} = {+0x308}; p->{+0x2c0} = {+0x2c0};   // three 16-byte copies
//     return p;
// Blocked: +0x4b0 (m_pTabbedControlBarRTC) is past the end of OpenMFC's
// 1144-byte CDockablePane, and nothing in this build writes it.
// Signature corrected to carry `this`.
// Symbol: ?CreateTabbedPane@CDockablePane@@UEAAPEAVCTabbedPane@@XZ
extern "C" void* MS_ABI impl__CreateTabbedPane_CDockablePane__UEAAPEAVCTabbedPane__XZ(CDockablePane* pThis) {
    (void)pThis;
    return nullptr;
}

// STUB. Retail entry RVA 0x44540 (mfc140u; 0x44390 in mfc140), fully decoded
// (%r9d, the AFX_DOCK_METHOD, is never read):
//     if (m_hDefaultSlider == NULL || !::IsWindow(m_hDefaultSlider)) return FALSE;   // +0x4c0
//     CObList lst;                                                    // stack, vftable 0x328538
//     barContainerManager.AddPanesToList(&lst, NULL);                 // RVA 0xaa960
//     for (POSITION pos = lst.GetHeadPosition(); pos != NULL; ) {
//         CDockablePane* pPane = DYNAMIC_DOWNCAST(CDockablePane, lst.GetNext(pos));   // RTC 0x3aa178
//         InsertPane(pPane, this, TRUE);                              // CBasePane::InsertPane, RVA 0xbc50
//         pPane->SetDefaultPaneDivider(m_hDefaultSlider);             // RVA 0x455f0, pPane unchecked
//         pPane->SetPaneAlignment(GetCurrentAlignment());             // pane slot 0x3d0 <- this slot 0x338
//     }
//     CPaneDivider* pSlider = GetDefaultPaneDivider();                // RVA 0x45cd0
//     if (pSlider == NULL) return FALSE;                              // (after lst's dtor, RVA 0x83d0)
//     return pSlider->AddPaneContainer(this, barContainerManager, dwAlignment);   // divider slot 0x520
// Left a stub. The CObList walk itself is NOT the blocker: GetHeadPosition /
// GetNext are inline in retail (no export), but OpenMFC defines them as real
// C++ methods through the list-wrapper macro in detail/FilecoreSupport.h
// (toolbar/CMFCToolBar.cpp:2089 calls them directly), so this unit could link
// them -- checkfile.sh's link audit would merely flag them as new `_Z`
// undefineds. What blocks it is the terminals: CBasePane::InsertPane
// (docking/CBasePane.cpp:1018) and
// ?AddPaneContainer@CPaneDivider@@UEAAHPEAVCDockablePane@@AEAVCPaneContainerManager@@K@Z
// (docking/CPaneDivider.cpp:152) are both generated stubs returning 0, and
// GetDefaultPaneDivider() above cannot return a divider in this build, so a
// transcription would run the per-pane SetDefaultPaneDivider / SetPaneAlignment
// side effects and then always return FALSE. m_hDefaultSlider itself is
// available (file-private DockablePaneExtra table).
// Signature corrected: the generated 16-parameter list was garbage.
// Symbol: ?DockPaneContainer@CDockablePane@@UEAAHAEAVCPaneContainerManager@@KW4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__DockPaneContainer_CDockablePane__UEAAHAEAVCPaneContainerManager__KW4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, void* pBarContainerManager, unsigned long dwAlignment, int dockMethod) {
    (void)pThis;
    (void)pBarContainerManager;
    (void)dwAlignment;
    (void)dockMethod;
    return 0;
}

// STUB. Retail entry RVA 0x45c90 (mfc140u; 0x45ae0 in mfc140). It reads
// GetDockingMode() (vtable slot 0x380), then computes `nSensitivity = (mode &
// DT_SMART) ? -1 : <global at mfc140 RVA 0x3aaacc>` (DT_SMART == 0x80) and
// calls IsChangeState (vtable slot 0x578) with it and a CBasePane*
// out-parameter; the remainder was not transcribed. Blocked: the decision
// comes from this file's IsChangeState, which is itself blocked on the retail
// CDockingManager vtable (see above), so no docking target can ever be found.
// Signature corrected to carry `this`.
// Symbol: ?DockPaneStandard@CDockablePane@@UEAAPEAVCPane@@AEAH@Z
extern "C" void* MS_ABI impl__DockPaneStandard_CDockablePane__UEAAPEAVCPane__AEAH_Z(CDockablePane* pThis, int* pbWasDocked) {
    (void)pThis;
    (void)pbWasDocked;
    return nullptr;
}

// CDockablePane::DockToWindow -- transcribed from retail entry RVA 0x443d0
// (mfc140u; 0x44220 in mfc140). %rdx (pTargetWindow) -> %rbp, %r8d
// (dwAlignment) -> %r15d, %r9 (lpRect) -> %r14:
//     CPaneDivider* pSlider = pTargetWindow->GetDefaultPaneDivider();   // `mov %rdx,%rcx ; call 0x45cd0`
//     if (pSlider == NULL) { ShowWindow(SW_SHOW /*lea 0x5(%rax)*/); return FALSE; }   // RVA 0x2a79e0
//     if (m_hDefaultSlider != NULL && ::IsWindow(m_hDefaultSlider))    // +0x4c0, IAT 0x1802c5390
//         UndockPane(FALSE);                                           // vtable slot 0x490
//     RemoveFromMiniframe(AFXGetParentFrame(this), DM_UNKNOWN);        // slot 0x620; RVA 0x6bb10; `xor %r8d`
//     if (!pSlider->InsertPane(this, pTargetWindow, dwAlignment, lpRect))   // divider slot 0x500
//         return FALSE;
//     InsertPane(this, pTargetWindow, TRUE);                           // CBasePane::InsertPane, RVA 0xbc50
//     m_hDefaultSlider = pSlider->m_hWnd;
//     EnableGripper(TRUE);                                             // slot 0x3f0
//     SetWindowPos(NULL, 0, 0, 0, 0, 0x7f, NULL);                      // slot 0x480
//     AdjustDockingLayout(NULL);                                       // slot 0x4e0
//     OnAfterDock(this, NULL, DM_UNKNOWN);                             // slot 0x608, `mov %rdi,%rdx ; xor %r8d ; xor %r9d`
//     return TRUE;
// 0x7f == SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE|
// SWP_FRAMECHANGED|SWP_SHOWWINDOW. Slots 0x490/0x620/0x3f0/0x480/0x4e0/0x608
// of the CDockablePane vftable (mfc140 RVA 0x2e3068) were read with vt.py and
// resolve to UndockPane / CPane::RemoveFromMiniframe / EnableGripper /
// CBasePane::SetWindowPos / CBasePane::AdjustDockingLayout / OnAfterDock;
// slot 0x500 of the CPaneDivider vftable (0x2f4d28) is
// ?InsertPane@CPaneDivider@@UEAAHPEAVCDockablePane@@0KPEBUtagRECT@@@Z (RVA
// 0xac8a0). m_hDefaultSlider lives in the file-private DockablePaneExtra
// table. pSlider came out of GetDefaultPaneDivider, i.e. from the permanent
// CWnd map, so it is read as a CWnd for its m_hWnd even though OpenMFC's
// CPaneDivider class is not declared as one.
// Deviations: every virtual dispatches statically to the exports named above
// (UndockPane, EnableGripper and both InsertPane bodies are still stubs -- the
// CPaneDivider one returns 0, so today this returns FALSE right after
// RemoveFromMiniframe; GetDefaultPaneDivider also returns NULL in this build,
// so in practice the first arm is what runs); NULL checks on `this` and
// pTargetWindow (retail dereferences pTargetWindow).
// Signature corrected: the generated list dropped `this` and two parameters.
// Symbol: ?DockToWindow@CDockablePane@@UEAAHPEAV1@KPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__DockToWindow_CDockablePane__UEAAHPEAV1_KPEBUtagRECT___Z(
    CDockablePane* pThis, CDockablePane* pTargetWindow, unsigned long dwAlignment, const RECT* lpRect) {
    if (pThis == nullptr || pTargetWindow == nullptr) return FALSE;   // deviation: retail has no NULL check
    void* pSlider = impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(pTargetWindow);
    if (pSlider == nullptr) {
        impl__ShowWindow_CWnd__QEAAHH_Z(static_cast<CWnd*>(pThis), SW_SHOW);
        return FALSE;
    }
    const HWND hOldSlider = ExtraOf(pThis).hDefaultSlider;
    if (hOldSlider != nullptr && ::IsWindow(hOldSlider)) {
        impl__UndockPane_CDockablePane__UEAAXH_Z(pThis, FALSE);
    }
    impl__RemoveFromMiniframe_CPane__MEAAXPEAVCWnd__W4AFX_DOCK_METHOD___Z(
        static_cast<CPane*>(pThis),
        static_cast<CWnd*>(impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(static_cast<const CWnd*>(pThis))),
        0 /*DM_UNKNOWN*/);
    if (!impl__InsertPane_CPaneDivider__UEAAHPEAVCDockablePane__0KPEBUtagRECT___Z(
            pSlider, pThis, pTargetWindow, dwAlignment, lpRect)) {
        return FALSE;
    }
    impl__InsertPane_CBasePane__QEAAHPEAV1_0H_Z(
        static_cast<CBasePane*>(pThis), static_cast<CBasePane*>(pThis), static_cast<CBasePane*>(pTargetWindow), TRUE);
    SetDefaultSliderOf(pThis, static_cast<CWnd*>(pSlider)->m_hWnd);
    impl__EnableGripper_CDockablePane__UEAAXH_Z(pThis, TRUE);
    impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
        static_cast<CBasePane*>(pThis), nullptr, 0, 0, 0, 0,
        SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW,
        nullptr);
    impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(static_cast<CBasePane*>(pThis), nullptr);
    impl__OnAfterDock_CDockablePane__MEAAXPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
        pThis, static_cast<CBasePane*>(pThis), nullptr, 0 /*DM_UNKNOWN*/);
    return TRUE;
}

// STUB. Retail entry RVA 0x446a0 (mfc140u; 0x444f0 in mfc140). Its body
// touches the CDockablePane fields at +0x41c (m_bActive) and +0x490/+0x498
// (m_arrButtons.m_pData / m_nSize) and paints through the visual manager.
// Blocked: OpenMFC's CDockablePane has no m_arrButtons (+0x488 is past the end
// of the 1144-byte object) and its CMFCCaptionButton
// (include/openmfc/afxmfc.h:1458) is a 16-byte-padded CObject with no retail
// layout, so there are no caption buttons to draw.
// Signature corrected: the 16-byte CRect is passed by value in retail, i.e. by
// address under the MS x64 ABI (`mov 0x8(%r8),%rbx` reads its right/bottom).
// Symbol: ?DrawCaption@CDockablePane@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__DrawCaption_CDockablePane__MEAAXPEAVCDC__VCRect___Z(
    CDockablePane* pThis, CDC* pDC, const RECT* pRectCaption) {
    (void)pThis;
    (void)pDC;
    (void)pRectCaption;
}

// STUB. Retail entry RVA 0x45260, fully decoded:
//     CMFCCaptionButton* p = FindButtonByHit(nHit);   // RVA 0x45210
//     if (p != NULL) p->{+0x14} = bEnable;            // CMFCCaptionButton field
// Blocked: FindButtonByHit walks m_arrButtons at this+0x488, which OpenMFC's
// CDockablePane does not have.
// Signature corrected to carry `this`.
// Symbol: ?EnableButton@CDockablePane@@IEAAXIH@Z
extern "C" void MS_ABI impl__EnableButton_CDockablePane__IEAAXIH_Z(CDockablePane* pThis, unsigned int nHit, int bEnable) {
    (void)pThis;
    (void)nHit;
    (void)bEnable;
}

// STUB. Retail entry RVA 0x40160, fully decoded:
//     m_cyGripper = (bEnable && m_bHasGripper)                 // +0x40c, +0x418
//         ? afxGlobalData<+0x2a8 at RVA 0x3ba628> + 5 : 0;     // after a lazy
//           AFX_GLOBAL_DATA::Initialize (RVA 0x6a5c0) guarded by the flag at
//           RVA 0x3ba380
//     SetWindowPos(NULL, 0,0,0,0, 0x37);   // vtable slot 0x480; 0x37 ==
//           SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED
// Left a stub. m_cyGripper (+0x40c) and m_bHasGripper (+0x418) are both inside
// the reachable window, but the value the gripper height is derived from is not:
// 0x3ba628 is afxGlobalData + 0x2a8 (the horizontal text height, the field
// toolbar/CMFCToolBar.cpp names kGlobalDataTextHeightHorz), and OpenMFC's
// afxGlobalData (phase4/src/featurepack/CMFC_misc_stubs.cpp:3652) is a
// 720-byte all-zero blob whose exported ?Initialize@AFX_GLOBAL_DATA@@QEAAXXZ
// (core/runtime/AFX_GLOBAL_DATA.cpp:97) is an empty stub that fills nothing
// in, so the transcription would store a fabricated 0 + 5 == 5. Retail
// also only ever sets m_bHasGripper from CDockablePane::Create, and this file's
// Create keeps that flag in its side table instead, so the flag would read 0
// forever and the "enable" branch could never be taken.
// Symbol: ?EnableGripper@CDockablePane@@UEAAXH@Z
extern "C" void MS_ABI impl__EnableGripper_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bEnable) {
    (void)pThis;
    (void)bEnable;
}

// CDockablePane::EnterDragMode -- transcribed from retail entry RVA 0x41e20,
// whose whole body is
//     movb $0x1,0x408(%rcx)      ; m_bPrepareToFloat = true
//     jmp  0x9ff20               ; tail call CPane::EnterDragMode(bChangeHotPoint)
// (0x9ff20 is ?EnterDragMode@CPane@@MEAAXH@Z.)
// m_bPrepareToFloat is a single byte at +0x408, inside the padding blob (see
// the offset note near the top), so it is stored at its retail offset here.
// OnNcMouseMove and OnContextMenu below read it back.
// Signature corrected to carry `this`.
// Symbol: ?EnterDragMode@CDockablePane@@MEAAXH@Z
extern "C" void MS_ABI impl__EnterDragMode_CDockablePane__MEAAXH_Z(
    CDockablePane* pThis, int bChangeHotPoint) {
    if (pThis == nullptr) return;
    DockablePaneField<bool>(pThis, kOffPrepareToFloat) = true;
    impl__EnterDragMode_CPane__MEAAXH_Z(static_cast<CPane*>(pThis), bChangeHotPoint);
}

// STUB. Retail entry RVA 0x45110. It scans m_arrButtons (m_pData at
// this+0x490, m_nSize at this+0x498) to find the button under `point`.
// Blocked: OpenMFC's CDockablePane has no m_arrButtons.
// Signature corrected to carry `this`; the by-value CPoint occupies one register.
// Symbol: ?FindButton@CDockablePane@@IEBAPEAVCMFCCaptionButton@@VCPoint@@@Z
extern "C" void* MS_ABI impl__FindButton_CDockablePane__IEBAPEAVCMFCCaptionButton__VCPoint___Z(
    const CDockablePane* pThis, void* point) {
    (void)pThis;
    (void)point;
    return nullptr;
}

// STUB. Retail entry RVA 0x45210, fully decoded:
//     for (i = 0; i < m_arrButtons.m_nSize; i++)          // +0x498
//         if (m_arrButtons.m_pData[i]->{+0x20} == nHit)   // +0x490
//             return m_arrButtons.m_pData[i];
//     return NULL;
// (out-of-range indices call AfxThrowInvalidArgException, RVA 0x225b80.)
// Blocked: OpenMFC's CDockablePane has no m_arrButtons.
// Signature corrected to carry `this`.
// Symbol: ?FindButtonByHit@CDockablePane@@IEBAPEAVCMFCCaptionButton@@I@Z
extern "C" void* MS_ABI impl__FindButtonByHit_CDockablePane__IEBAPEAVCMFCCaptionButton__I_Z(
    const CDockablePane* pThis, unsigned int nHit) {
    (void)pThis;
    (void)nHit;
    return nullptr;
}

// CDockablePane::GetCaptionHeight -- transcribed from retail entry RVA 0x46520
// (mfc140; 0x466d0 in mfc140u):
//     if (IsFloating()) return 0;        // slot 0x2f0; its body at RVA 0x8780 is
//                                        // `IsDocked() == 0` through slot 0x2e0
//     if (IsMDITabbed()) return 0;       // slot 0x2f8, RVA 0xb380
//     if (m_cyGripper == 0) return 0;    // +0x40c
//     return m_cyGripper + CMFCVisualManager::GetInstance()->{slot 0x1a0}();
// The call at 0x4655c goes to RVA 0x97f4, the non-exported inline
// CMFCVisualManager::GetInstance (it lazily news a 0x108-byte object through the
// ctor at RVA 0x182640 and caches it at 0x3b7120). Slot 0x1a0 of the resulting
// vftable is the same two-byte `xor %eax,%eax ; ret` (ICF-folded at RVA 0x7260)
// in the CMFCVisualManager vftable (mfc140 RVA 0x319f78) AND in the vftables of
// all six shipped subclasses (VS2005 0x31ba20, Office2003 0x31a5c0,
// Office2007 0x31ace8, Windows 0x31c758, OfficeXP 0x31b3d8, VS2008 0x31c0b8,
// Windows7 0x31cdd0 -- each read with vt.py), i.e. the virtual contributes 0
// for every visual manager in the DLL. OpenMFC's CMFCVisualManager declares no
// such virtual, so that term is folded to 0 here; only a user-derived visual
// manager overriding it in retail would differ.
// m_cyGripper is inside the reachable window and is read at its retail offset.
// Note nothing in this DLL writes it yet (EnableGripper above is still a stub),
// so today this returns 0 on the third test.
// Deviation: the two self-virtuals dispatch statically to this file's IsDocked
// and the CBasePane::IsMDITabbed export.
// Symbol: ?GetCaptionHeight@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__GetCaptionHeight_CDockablePane__UEBAHXZ(const CDockablePane* pThis) {
    if (pThis == nullptr) return 0;   // deviation: retail has no NULL check
    if (!impl__IsDocked_CDockablePane__UEBAHXZ(pThis)) return 0;
    if (impl__IsMDITabbed_CBasePane__UEBAHXZ(static_cast<const CBasePane*>(pThis))) return 0;
    const int cyGripper = DockablePaneField<int>(pThis, kOffCyGripper);
    if (cyGripper == 0) return 0;
    return cyGripper + 0;   // + CMFCVisualManager slot 0x1a0, which is `return 0` (see above)
}

// CDockablePane::GetDefaultPaneDivider -- transcribed from retail entry RVA
// 0x45e80 (mfc140u; 0x45cd0 in mfc140), fully decoded:
//     CWnd* p = CWnd::FromHandlePermanent(m_hDefaultSlider);   // +0x4c0, RVA 0x2891d0
//     return (p != NULL && p->IsKindOf(<RTC at mfc140 RVA 0x2f4b58, "CPaneDivider">))
//            ? (CPaneDivider*)p : NULL;
// m_hDefaultSlider has no storage in the 1144-byte object and is read from the
// file-private DockablePaneExtra table that SetDefaultPaneDivider below fills.
// The descriptor is the one docking/RuntimeClasses.cpp:268 exports for
// CPaneDivider. Note that OpenMFC's CPaneDivider (include/openmfc/afxmfc.h)
// derives from CObject, not CWnd, so no permanent-map entry can pass the
// IsKindOf test today and this returns NULL until that class is re-based; the
// code path is retail's regardless.
// Deviation: NULL check on `this`.
// Symbol: ?GetDefaultPaneDivider@CDockablePane@@QEBAPEAVCPaneDivider@@XZ
extern "C" void* MS_ABI impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(const CDockablePane* pThis) {
    if (pThis == nullptr) return nullptr;   // deviation: retail has no NULL check
    void* p = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(ExtraOf(pThis).hDefaultSlider);
    if (p == nullptr) return nullptr;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(static_cast<CWnd*>(p)),
            impl__GetThisClass_CPaneDivider__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return p;
}

// CDockablePane::GetDockingStatus -- transcribed from retail entry RVA 0x45d10
// (mfc140; 0x45ec0 in mfc140u). The enum result is returned in %eax; %rdx is
// the by-value CPoint, %r8d nSensitivity:
//     AFX_DOCK_TYPE mode = GetDockingMode();                       // slot 0x380
//     CDockingManager* pMgr = afxGlobalUtils.GetDockingManager(GetDockSiteFrameWnd());
//                                                    // RVA 0x6ccc0, %rcx = &afxGlobalUtils (0x3aacf8)
//     int nMarker = -1;                                             // %r15d
//     if ((mode & DT_SMART) && pMgr != NULL) {                      // `test %dil,%dil ; jns`
//         CSmartDockingManager* pSD = pMgr->GetSmartDockingManager();   // inline at RVA 0x3fb80
//         if (pSD != NULL && pSD->m_bCreated /*+0xc*/ && pSD->m_bStarted /*+0x8*/)   // == IsStarted()
//             nMarker = pSD->m_nHiliteSideNo;                       // +0x1b8
//     }
//     int nHit = HitTest(pt, TRUE);                                 // slot 0x678
//     CRect rectTop, rectBottom; GetTabArea(rectTop, rectBottom);   // slot 0x668
//     if (nHit == HTCAPTION || rectTop.PtInRect(pt) || rectBottom.PtInRect(pt) ||
//         nMarker == CSmartDockingStandaloneGuide::sdCMIDDLE /*8*/)
//         return CS_DELAY_DOCK_TO_TAB;                              // 3
//     CRect rectWnd(0,0,0,0); ::GetWindowRect(m_hWnd, &rectWnd);
//     rectWnd.top    += rectTop.Height() + GetCaptionHeight();      // slot 0x358
//     rectWnd.bottom -= rectBottom.Height();
//     if (nSensitivity == -1)
//         return rectWnd.PtInRect(pt) ? CS_DELAY_DOCK : CS_NOTHING;     // 2 : 0
//     ::InflateRect(&rectWnd, -nSensitivity, -nSensitivity);        // 0x1802c5310
//     return rectWnd.PtInRect(pt) ? CS_DELAY_DOCK : CS_NOTHING;     // `neg;sbb;not;and $2`
// (0x1802c5320 == USER32!PtInRect, 0x1802c5370 == GetWindowRect, both via
// iat.py. Slot 0x668 of the CDockablePane vftable (mfc140 RVA 0x2e3068) is the
// non-exported body at RVA 0x3fc20, which just ::SetRectEmpty()s both
// arguments -- CDockablePane::GetTabArea's inline default; CTabbedPane
// overrides it.)
// Deviations: (1) the DT_SMART marker probe is dropped -- OpenMFC's
// CDockingManager has no smart-docking manager pointer and CSmartDockingManager
// is not modelled -- so nMarker stays -1 and a drag over the centre guide
// reports CS_DELAY_DOCK instead of CS_DELAY_DOCK_TO_TAB; (2) GetTabArea is
// folded to its CDockablePane default (both rects empty), so a CTabbedPane's
// tab-strip areas are not honoured; (3) HitTest / GetCaptionHeight dispatch
// statically to this file's thunks. Everything else is retail's.
// Signature corrected: the generated 12-parameter list was garbage. The
// by-value CPoint occupies one register and is copied into a POINT as HitTest
// above does.
// Symbol: ?GetDockingStatus@CDockablePane@@UEAA?AW4AFX_CS_STATUS@@VCPoint@@H@Z
extern "C" int MS_ABI impl__GetDockingStatus_CDockablePane__UEAA_AW4AFX_CS_STATUS__VCPoint__H_Z(
    CDockablePane* pThis, void* point, int nSensitivity) {
    if (pThis == nullptr) return kCsNothing;   // deviation: retail has no NULL check
    POINT pt;
    memcpy(&pt, &point, sizeof(pt));

    // Retail evaluates these two even though only the DT_SMART arm (dropped
    // here, see above) consumes them; kept so the side effects match.
    (void)impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(static_cast<const CBasePane*>(pThis));
    (void)impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(
        reinterpret_cast<CGlobalUtils*>(impl__afxGlobalUtils__3VCGlobalUtils__A),
        impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(static_cast<const CBasePane*>(pThis)));
    const int nMarker = -1;

    const int nHit = impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(pThis, point, TRUE);
    RECT rectTabTop = {0, 0, 0, 0};      // GetTabArea(): CDockablePane's default empties both
    RECT rectTabBottom = {0, 0, 0, 0};
    if (nHit == HTCAPTION || ::PtInRect(&rectTabTop, pt) || ::PtInRect(&rectTabBottom, pt) ||
        nMarker == kSdCMiddle) {
        return kCsDelayDockToTab;
    }

    RECT rectWnd = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWnd);
    rectWnd.top += (rectTabTop.bottom - rectTabTop.top) + impl__GetCaptionHeight_CDockablePane__UEBAHXZ(pThis);
    rectWnd.bottom -= (rectTabBottom.bottom - rectTabBottom.top);
    if (nSensitivity == -1) {
        return ::PtInRect(&rectWnd, pt) ? kCsDelayDock : kCsNothing;
    }
    ::InflateRect(&rectWnd, -nSensitivity, -nSensitivity);
    return ::PtInRect(&rectWnd, pt) ? kCsDelayDock : kCsNothing;
}

// CDockablePane::HitTest -- transcribed from retail entry RVA 0x40e20:
//     CRect rectWindow(0,0,0,0);
//     ::GetWindowRect(m_hWnd, &rectWindow);                   // 0x1802c5370
//     if (!::PtInRect(&rectWindow, point)) return HTNOWHERE;  // 0x1802c5320
//     CDockingManager* pMgr =
//         afxGlobalUtils.GetDockingManager(GetDockSiteFrameWnd());  // slot 0x348,
//                                                             // RVA 0x6ccc0
//     if (pMgr != NULL && pMgr->{+0xc} == 0)                  // else skip the scan
//         for (i = 0; i < m_arrButtons.m_nSize; i++) {        // +0x498 / +0x490
//             CRect rc; m_arrButtons[i]->{vtable +0x28}(&rc);
//             ::OffsetRect(&rc, rectWindow.left, rectWindow.top);
//             if (::PtInRect(&rc, point)) return m_arrButtons[i]->{+0x20};
//         }
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);  // 0x1802c5358
//     ClientToScreen(&rectClient);                             // RVA 0x2a1250
//     if (::PtInRect(&rectClient, point)) return HTCLIENT;
//     if (IsDocked()) {                                        // vtable slot 0x2e0
//         CRect rectCaption;
//         ::SetRect(&rectCaption, rectWindow.left, rectWindow.top + 1,
//                   rectWindow.right, rectWindow.top + 1 + GetCaptionHeight());
//                                                              // slot 0x358, 0x1802c5398
//         if (::PtInRect(&rectCaption, point))
//             return bDetectCaption ? HTCAPTION : HTCLIENT;
//         // (`neg %r14d ; sbb %eax,%eax ; neg %eax ; inc %eax` == (bDetectCaption
//         //  != 0) + 1, and HTCLIENT == 1 / HTCAPTION == 2.)
//     }
//     return HTERROR;
// The IAT slots above were resolved with iat.py; 0x1802c5bd0 is the CFG
// indirect-call trampoline every virtual call in this DLL goes through (iat.py
// reports it is not an import slot), not a call target of its own.
//
// Deviations, both forced: the caption-button scan is dropped -- m_arrButtons
// lives at +0x488, past the usable window (see the offset note near the top) --
// and the docking-manager lookup that gates that scan goes with it. So this
// never reports a caption-button hit code. The remaining four outcomes
// (HTNOWHERE / HTCLIENT / HTCAPTION / HTERROR) are retail's. Note the caption
// strip is empty in practice today because the sibling GetCaptionHeight thunk
// is still a stub returning 0.
// Signature corrected: the generated stub dropped `this` and folded the CPoint
// into the first slot. The by-value CPoint occupies one register (retail passes
// it straight on to ::PtInRect as a POINT), so it is taken as an opaque 8-byte
// value and copied into a POINT.
// Symbol: ?HitTest@CDockablePane@@UEAAHVCPoint@@H@Z
extern "C" int MS_ABI impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(
    CDockablePane* pThis, void* point, int bDetectCaption) {
    if (pThis == nullptr) return HTERROR;  // deviation: retail has no NULL check
    POINT pt;
    memcpy(&pt, &point, sizeof(pt));

    RECT rectWindow = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWindow);
    if (!::PtInRect(&rectWindow, pt)) return HTNOWHERE;

    RECT rectClient = {0, 0, 0, 0};
    ::GetClientRect(pThis->m_hWnd, &rectClient);
    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(static_cast<const CWnd*>(pThis), &rectClient);
    if (::PtInRect(&rectClient, pt)) return HTCLIENT;

    if (impl__IsDocked_CDockablePane__UEBAHXZ(pThis)) {
        const int cyCaption = impl__GetCaptionHeight_CDockablePane__UEBAHXZ(pThis);
        RECT rectCaption = {0, 0, 0, 0};
        ::SetRect(&rectCaption, rectWindow.left, rectWindow.top + 1,
                  rectWindow.right, rectWindow.top + 1 + cyCaption);
        if (::PtInRect(&rectCaption, pt)) return bDetectCaption ? HTCAPTION : HTCLIENT;
    }
    return HTERROR;
}

// STUB. Retail entry RVA 0x45280, fully decoded:
//     CMFCCaptionButton* p = FindButtonByHit(nHit);   // RVA 0x45210
//     return (p != NULL) ? p->{+0x14} : 0;
// Blocked for the same reason as FindButtonByHit: no m_arrButtons.
// Signature corrected to carry `this`.
// Symbol: ?IsButtonEnabled@CDockablePane@@IEBAHI@Z
extern "C" int MS_ABI impl__IsButtonEnabled_CDockablePane__IEBAHI_Z(const CDockablePane* pThis, unsigned int nHit) {
    (void)pThis;
    (void)nHit;
    return 0;
}

// STUB. Retail entry RVA 0x41050 (mfc140; 0x41200 in mfc140u), fully decoded:
//     if (ppTargetBar == NULL) AfxThrowInvalidArgException();       // RVA 0x225b80
//     *ppTargetBar = NULL;
//     CPoint pt; ::GetCursorPos(&pt);                               // 0x1802c5348
//     CWnd* pWnd = CWnd::FromHandle(::GetParent(m_hWnd));           // 0x1802c5300, RVA 0x289180
//     if (pWnd->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)))             // RTC 0x3aa418, unchecked for NULL
//         pWnd = CWnd::FromHandlePermanent(((CPaneFrameWnd*)pWnd)->{+0x130});   // RVA 0x2891d0
//     CDockingManager* pMgr = afxGlobalUtils.GetDockingManager(pWnd);          // RVA 0x6ccc0
//     if (pMgr == NULL) return CS_NOTHING;                          // %eax is the NULL
//     return pMgr->{slot 0x40}(pt, nOffset, IsFloating() /*slot 0x340*/, ppTargetBar, this, this);
// Blocked: the decision is a virtual at slot 0x40 of the retail CDockingManager
// vftable; OpenMFC's CDockingManager is a CObject with a padding blob and no
// retail-shaped vtable, and the CPaneFrameWnd HWND at +0x130 is not modelled
// either.
// Signature corrected: the generated 12-parameter list was garbage; the enum is
// returned in %eax.
// Symbol: ?IsChangeState@CDockablePane@@UEBA?AW4AFX_CS_STATUS@@HPEAPEAVCBasePane@@@Z
extern "C" int MS_ABI impl__IsChangeState_CDockablePane__UEBA_AW4AFX_CS_STATUS__HPEAPEAVCBasePane___Z(
    const CDockablePane* pThis, int nOffset, CBasePane** ppTargetBar) {
    (void)pThis;
    (void)nOffset;
    if (ppTargetBar != nullptr) *ppTargetBar = nullptr;   // retail's first store; the rest is not reproduced
    return kCsNothing;
}

// CDockablePane::IsDocked -- transcribed from retail entry RVA 0xb300:
//     CPaneFrameWnd* p = GetParentMiniFrame(FALSE);   // vtable slot 0x460
//     if (p == NULL) return TRUE;
//     return p->{slot 0x350}() != 1;                  // pane count
// (`xor %ecx,%ecx ; cmp $0x1,%eax ; setne %cl` -- note it is != 1, not > 1.)
// Deviation: the pane-count virtual on CPaneFrameWnd is not modelled in this
// DLL (no GetPaneCount export exists here), so the "inside a mini frame" branch
// returns FALSE, which is what retail returns for the common single-pane mini
// frame. A mini frame holding several panes would be reported wrongly.
// Symbol: ?IsDocked@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsDocked_CDockablePane__UEBAHXZ(const CDockablePane* pThis) {
    if (pThis == nullptr) return FALSE;
    void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), FALSE);
    if (pMiniFrame == nullptr) return TRUE;
    return FALSE;
}

// CDockablePane::IsInFloatingMultiPaneFrameWnd -- transcribed from retail entry
// RVA 0x45ef0:
//     CWnd* p = GetParentMiniFrame(FALSE);            // vtable slot 0x460
//     if (p == NULL) return FALSE;
//     return p->IsKindOf(RUNTIME_CLASS(CMultiPaneFrameWnd));
// (tail-jump to ?IsKindOf@CObject@@ at RVA 0x233310 with %rdx = the descriptor
// at RVA 0x3aa2f8, whose m_lpszClassName reads "CMultiPaneFrameWnd" and whose
// m_nObjectSize is 768.)
// Symbol: ?IsInFloatingMultiPaneFrameWnd@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsInFloatingMultiPaneFrameWnd_CDockablePane__UEBAHXZ(const CDockablePane* pThis) {
    if (pThis == nullptr) return FALSE;
    void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), FALSE);
    if (pMiniFrame == nullptr) return FALSE;
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
        static_cast<const CObject*>(pMiniFrame),
        impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ());
}

// CDockablePane::IsVisible -- transcribed from retail entry RVA 0x45330 (mfc140;
// 0x454e0 in mfc140u):
//     if (!IsAutoHideMode())        return CBasePane::IsVisible();   // tail jmp RVA 0xb390
//     if (!IsHideInAutoHideMode())  return FALSE;
//     return m_pAutoHideBar->IsVisible();                            // +0x480, slot 0x308
// IsAutoHideMode is vtable slot 0x3c8 (its body at RVA 0x12e00 is
// `mov 0x438(%rcx),%eax`); IsHideInAutoHideMode is slot 0x6e8, whose body at
// RVA 0x12e80 is `mov 0x3b6f48(%rip),%eax` == the exported static
// ?m_bHideInAutoHideMode@CDockablePane@@2HA; slot 0x308 of the CMFCAutoHideBar
// vftable (mfc140 RVA 0x2d90d8) holds ?IsVisible@CBasePane@@UEBAHXZ (RVA 0xb390).
// m_pAutoHideBar comes from the docking side table (see AutoHideBarOf) and the
// auto-hide flag from this file's C++ IsAutoHideMode, which reads the same
// table. Deviations: the bar's IsVisible dispatches statically to the
// CBasePane export, and a NULL bar yields FALSE where retail would fault.
// Signature corrected to carry `this`.
// Symbol: ?IsVisible@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsVisible_CDockablePane__UEBAHXZ(const CDockablePane* pThis) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check
    if (!pThis->IsAutoHideMode()) {
        return impl__IsVisible_CBasePane__UEBAHXZ(static_cast<const CBasePane*>(pThis));
    }
    if (!impl__m_bHideInAutoHideMode_CDockablePane__2HA) return FALSE;
    const CBasePane* pBar = static_cast<const CBasePane*>(AutoHideBarOf(pThis));
    if (pBar == nullptr) return FALSE;
    return impl__IsVisible_CBasePane__UEBAHXZ(pBar);
}

// CDockablePane::LoadState -- transcribed from retail. The RVA map has no entry
// for the UNICODE mangling, but the disassembler reads mfc140.dll (the ANSI
// twin), where the same export is ?LoadState@CDockablePane@@UEAAHPEBDHI@Z at
// RVA 0x45640 and its ENTIRE body is
//     jmp 0x1800a2260            ; ?LoadState@CPane@@UEAAHPEBDHI@Z
// -- an unconditional tail jump with no argument shuffling, i.e. CDockablePane
// adds nothing to CPane::LoadState. Reproduced as a straight forward.
// Note the sibling CPane::LoadState thunk
// (phase4/src/featurepack/docking/CPane.cpp:188) is itself still a stub
// returning 0, so this returns FALSE today.
// Signature corrected to carry `this`.
// Symbol: ?LoadState@CDockablePane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CDockablePane__UEAAHPEB_WHI_Z(
    CDockablePane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    return impl__LoadState_CPane__UEAAHPEB_WHI_Z(
        static_cast<CPane*>(pThis), lpszProfileName, nIndex, uiID);
}

// CDockablePane::OffsetRectForSliding -- transcribed from retail entry RVA
// 0x429a0. The whole body is:
//     if (!IsAutoHideMode()) return;                  // vtable slot 0x3c8
//     int d = m_nSlideDelta;                          // +0x454
//     switch (GetCurrentAlignment()) {                // vtable slot 0x338
//     case CBRS_ALIGN_LEFT:   ::OffsetRect(&rect, (bParam2 != bParam3) ? d : -d, 0); break;
//     case CBRS_ALIGN_RIGHT:  ::OffsetRect(&rect, (bParam2 == bParam3) ? d : -d, 0); break;
//     case CBRS_ALIGN_TOP:    ::OffsetRect(&rect, 0, bParam2 ? d : -d); break;
//     case CBRS_ALIGN_BOTTOM: ::OffsetRect(&rect, 0, bParam2 ? -d : d); break;
//     default: return;                                // no ::OffsetRect call
//     }
// The four sign tables were read off the jump chain at 0x4299c..0x42a5c
// (0x1802c5318 resolves to USER32!OffsetRect); the two BOOL arguments arrive in
// %r8d and %r9d and MFC does not export names for them, so they are named
// positionally here. m_nSlideDelta lives inside the padding blob (see the
// offset note near the top) and is read at its retail offset.
// Note: the sibling CBasePane::GetCurrentAlignment thunk
// (phase4/src/featurepack/docking/CBasePane.cpp:262) is still a stub returning
// 0, so today no case matches and this returns without offsetting anything --
// which is exactly what retail does for an alignment of 0. Nothing in this DLL
// writes m_nSlideDelta yet either (retail writes it from Slide/OnSlide, both
// still stubs here), so it reads back as the 0 the constructor left.
// Signature corrected to carry `this`.
// Symbol: ?OffsetRectForSliding@CDockablePane@@IEAAXAEAVCRect@@HH@Z
extern "C" void MS_ABI impl__OffsetRectForSliding_CDockablePane__IEAAXAEAVCRect__HH_Z(
    CDockablePane* pThis, CRect& rect, int bParam2, int bParam3) {
    if (pThis == nullptr) return;
    if (!pThis->IsAutoHideMode()) return;

    const int nDelta = DockablePaneField<int>(pThis, kOffSlideDelta);
    int dx = 0;
    int dy = 0;
    switch (impl__GetCurrentAlignment_CBasePane__UEBAKXZ(static_cast<const CBasePane*>(pThis))) {
    case kAlignLeft:   dx = ((bParam2 != 0) != (bParam3 != 0)) ?  nDelta : -nDelta; break;
    case kAlignRight:  dx = ((bParam2 != 0) == (bParam3 != 0)) ?  nDelta : -nDelta; break;
    case kAlignTop:    dy = (bParam2 != 0) ?  nDelta : -nDelta; break;
    case kAlignBottom: dy = (bParam2 != 0) ? -nDelta :  nDelta; break;
    default: return;
    }
    ::OffsetRect(reinterpret_cast<RECT*>(&rect), dx, dy);
}

// CDockablePane::OnAfterChangeParent -- transcribed from retail entry RVA
// 0x40400, which is exactly:
//     CPane::OnAfterChangeParent(pWndOldParent);          // RVA 0xa0fb0
//     if (GetParentMiniFrame(FALSE) != NULL)              // vtable slot 0x460
//         CPaneFrameWnd::AddRemovePaneFromGlobalList(this, TRUE);  // RVA 0xaf740
// (`mov $0x1,%edx` before the static call is the bAdd argument.)
// Symbol: ?OnAfterChangeParent@CDockablePane@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnAfterChangeParent_CDockablePane__UEAAXPEAVCWnd___Z(
    CDockablePane* pThis, CWnd* pWndOldParent) {
    if (pThis == nullptr) return;
    impl__OnAfterChangeParent_CPane__UEAAXPEAVCWnd___Z(static_cast<CPane*>(pThis), pWndOldParent);
    if (impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
            static_cast<const CBasePane*>(pThis), FALSE) != nullptr) {
        impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(
            static_cast<CBasePane*>(pThis), TRUE);
    }
}

// CDockablePane::OnAfterDock -- transcribed from retail entry RVA 0x402b0. All
// three arguments are ignored; the body reads only `this`:
//     if (!CDockingManager::m_bRestoringDockState)   // DWORD at RVA 0x3b6f50
//         SetFocus();                                // RVA 0x2a7a70
//     if (GetDockingMode() == DT_IMMEDIATE) {        // vtable slot 0x380
//         ::GetCursorPos(&m_ptClientHotSpot);        // this+0x208
//         ::ScreenToClient(m_hWnd, &m_ptClientHotSpot);
//     }
//     if (GetDlgCtrlID() != -1 &&                    // RVA 0x2a78b0
//         GetParentMiniFrame(FALSE) == NULL)         // vtable slot 0x460
//         CPaneFrameWnd::AddRemovePaneFromGlobalList(this, FALSE);  // RVA 0xaf740
// (0x1802c5348 and 0x1802c5340 resolve to USER32!GetCursorPos and
// USER32!ScreenToClient; m_ptClientHotSpot is the CPane member this repo
// already models at +0x208.)
// Deviation: CBasePane::GetDockingMode (RVA 0xc820) is
// `m_dockMode != 0 ? m_dockMode : <CGlobalUtils default at RVA 0x3aaac8>`;
// OpenMFC has no such global, so m_dockMode is read directly and a zero
// m_dockMode is treated as "not DT_IMMEDIATE" -- correct whenever the
// application default is DT_STANDARD, wrong if it was set to DT_IMMEDIATE.
// Symbol: ?OnAfterDock@CDockablePane@@MEAAXPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" void MS_ABI impl__OnAfterDock_CDockablePane__MEAAXPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, CBasePane* pBar, const RECT* lpRect, int dockMethod) {
    (void)pBar;
    (void)lpRect;
    (void)dockMethod;
    if (pThis == nullptr) return;
    if (!impl__m_bRestoringDockState_CDockingManager__2HA) {
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pThis));
    }
    if (pThis->m_dockMode == kDockTypeImmediate) {
        // Retail passes &m_ptClientHotSpot straight to both APIs; OpenMFC's
        // CPoint is not derived from POINT, so a temporary is used and copied
        // back. Same net effect.
        POINT ptHotSpot = {0, 0};
        ::GetCursorPos(&ptHotSpot);
        ::ScreenToClient(pThis->m_hWnd, &ptHotSpot);
        pThis->m_ptClientHotSpot.x = ptHotSpot.x;
        pThis->m_ptClientHotSpot.y = ptHotSpot.y;
    }
    if (impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pThis)) != -1 &&
        impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
            static_cast<const CBasePane*>(pThis), FALSE) == nullptr) {
        impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(
            static_cast<CBasePane*>(pThis), FALSE);
    }
}

// CDockablePane::OnBeforeChangeParent -- transcribed from retail entry RVA
// 0x40340:
//     if (pWndNewParent == NULL) return;                 // `test %rdx,%rdx; je ret`
//     BOOL bTabbed = pWndNewParent->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane));
//     if (bTabbed) StoreRecentDockSiteInfo();            // vtable slot 0x648
//     if (pWndNewParent->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)) ||
//         pWndNewParent->IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)) || bTabbed)
//         UndockPane(bDelay);                             // vtable slot 0x490
//     CPane::OnBeforeChangeParent(pWndNewParent, FALSE);  // RVA 0xa0f80
// The three descriptors are the ones at RVA 0x2db168 ("CBaseTabbedPane"),
// 0x3aa418 ("CPaneFrameWnd") and 0x310b30 ("CMFCTabCtrl"), read out of their
// m_lpszClassName fields. Note the final call passes a hard FALSE
// (`xor %r8d,%r8d`), NOT the caller's bDelay.
// Slot 0x648 of the CDockablePane vftable (RVA 0x2e3068) holds
// ?StoreRecentDockSiteInfo@CDockablePane@@MEAAXXZ (RVA 0x41260) and slot 0x490
// holds ?UndockPane@CDockablePane@@UEAAXH@Z (RVA 0x44d20); this file has both
// exports, so the two virtual calls are reproduced as direct calls to its own
// thunks (both of which are still stubs -- see `stubbed`).
// Symbol: ?OnBeforeChangeParent@CDockablePane@@UEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnBeforeChangeParent_CDockablePane__UEAAXPEAVCWnd__H_Z(
    CDockablePane* pThis, CWnd* pWndNewParent, int bDelay) {
    if (pThis == nullptr || pWndNewParent == nullptr) return;
    CObject* pParentObj = static_cast<CObject*>(pWndNewParent);
    int bTabbed = impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
        pParentObj, impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ());
    if (bTabbed) {
        impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ(pThis);
    }
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pParentObj, impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ()) ||
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pParentObj, impl__GetThisClass_CMFCTabCtrl__SAPEAUCRuntimeClass__XZ()) ||
        bTabbed) {
        impl__UndockPane_CDockablePane__UEAAXH_Z(pThis, bDelay);
    }
    impl__OnBeforeChangeParent_CPane__UEAAXPEAVCWnd__H_Z(
        static_cast<CPane*>(pThis), pWndNewParent, FALSE);
}

// CDockablePane::OnCancelMode -- transcribed from retail entry RVA 0x450d0,
// whose whole body is:
//     StopCaptionButtonsTracking();       // vtable slot 0x728
//     if (m_bPrepareToFloat) m_bPrepareToFloat = false;   // byte at this+0x408
//     CPane::OnCancelMode();              // tail jump, RVA 0xa2930
// (`cmpb $0x0,0x408(%rbx) ; je ; movb $0x0,0x408(%rbx)` at 0x450e9.)
// All three steps are reproduced. The m_bPrepareToFloat clear MUST be here:
// +0x408 is inside the reachable padding window (see the offset note near the
// top), EnterDragMode above sets that byte, and OnNcMouseMove and OnContextMenu
// below both branch on it -- leaving it set would wedge those two handlers on
// the "in a float drag" path forever after the first drag.
// Symbol: ?OnCancelMode@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CDockablePane__IEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;
    impl__StopCaptionButtonsTracking_CDockablePane__MEAAXXZ(pThis);
    DockablePaneField<bool>(pThis, kOffPrepareToFloat) = false;
    impl__OnCancelMode_CPane__IEAAXXZ(static_cast<CPane*>(pThis));
}

// CDockablePane::OnClose -- transcribed from retail entry RVA 0x43510. The
// entire body is `mov (%rcx),%rax ; mov 0xd0(%rax),%rax ; rex.W jmp *...`, a
// tail-jump through vtable byte offset 0xd0, which in the CDockablePane vftable
// (RVA 0x2e3068) holds ?DestroyWindow@CWnd@@UEAAHXZ (RVA 0x289f70). So the
// handler is simply `DestroyWindow();`.
// Symbol: ?OnClose@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CDockablePane__IEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;
    impl__DestroyWindow_CWnd__UEAAHXZ(static_cast<CWnd*>(pThis));
}

// CDockablePane::OnContextMenu -- transcribed from retail entry RVA 0x46200,
// whose whole body is:
//     if (m_bPrepareToFloat) return;                        // byte at +0x408
//     if (m_bCaptionButtonsCaptured)                        // +0x4d8
//         StopCaptionButtonsTracking();                     // vtable slot 0x728
//     CPane::OnContextMenu(pWnd, point);                    // RVA 0xa21b0
// m_bPrepareToFloat is inside the padding blob and is honoured here;
// EnterDragMode above sets it.
// Deviation: the m_bCaptionButtonsCaptured branch is dropped -- +0x4d8 is past
// the end of the 1144-byte object (see the offset note near the top). That
// costs nothing observable today because this file's
// StopCaptionButtonsTracking is itself a stub, but a real one would not run.
// Signature corrected to carry `this`; the by-value CPoint occupies one
// register and is forwarded to CPane::OnContextMenu unexamined, as in retail.
// Symbol: ?OnContextMenu@CDockablePane@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CDockablePane__IEAAXPEAVCWnd__VCPoint___Z(
    CDockablePane* pThis, CWnd* pWnd, void* point) {
    if (pThis == nullptr) return;
    if (DockablePaneField<bool>(pThis, kOffPrepareToFloat)) return;
    impl__OnContextMenu_CPane__IEAAXPEAVCWnd__VCPoint___Z(
        static_cast<CPane*>(pThis), pWnd, point);
}

// STUB. The RVA map has no entry for the UNICODE mangling, but the ANSI twin
// the disassembler reads has ?OnCreate@CDockablePane@@IEAAHPEAUtagCREATESTRUCTA@@@Z
// at RVA 0x401f0:
//     if (Default() == -1) return -1;                      // 0x180289090
//     SetCaptionButtons();                                 // vtable slot 0x730
//     if (CTooltipManager::CreateToolTip(m_pToolTip, this, 16))   // +0x4b8,
//                                                          // RVA 0x1808e0
//         for (int i = 1; i < 4; i++) {
//             CRect rc; ::SetRectEmpty(&rc);               // 0x1802c5368
//             m_pToolTip->AddTool(this, (LPCTSTR)-1, &rc, i);
//         }                                                // RVA 0x273c00
//     return 0;
// (Argument order read off the call at 0x18004027c: %rcx = m_pToolTip,
// %rdx = this, %r8 = -1 (LPSTR_TEXTCALLBACK, the lpszText slot), %r9 = &rc and
// [%rsp+0x20] = i, i.e. AddTool(CWnd*, LPCTSTR, LPCRECT, UINT_PTR).)
// Blocked: m_pToolTip lives at +0x4b8, past the end of the 1144-byte object,
// and the two tooltip helpers above are not implemented here either.
// Signature corrected to carry `this`. lpCreateStruct is never read by retail.
// Symbol: ?OnCreate@CDockablePane@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CDockablePane__IEAAHPEAUtagCREATESTRUCTW___Z(
    CDockablePane* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)pThis;
    (void)lpCreateStruct;
    return 0;
}

// CDockablePane::OnDestroy -- transcribed from retail entry RVA 0x44e00 (mfc140;
// 0x44fb0 in mfc140u):
//     RemoveCaptionButtons();                              // RVA 0x46060
//     if (GetParentMiniFrame(FALSE) != NULL)               // slot 0x460
//         RemoveFromMiniframe(NULL, DM_UNKNOWN);           // slot 0x620 == RVA 0xa07f0;
//                                                          // `xor %r8d,%r8d ; xor %edx,%edx`
//     else
//         UndockPane(TRUE);                                // slot 0x490 == RVA 0x44d20
//     if (IsMDITabbed()) {                                 // slot 0x2f8
//         CDockingManager* pMgr =
//             afxGlobalUtils.GetDockingManager(GetDockSiteFrameWnd());   // slot 0x348, RVA 0x6ccc0
//         pMgr->RemoveHiddenMDITabbedBar(this);            // RVA 0x4ea40, called unchecked
//         CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));   // 0x1802c5300 / RVA 0x289180
//         if (pParent != NULL && pParent->IsKindOf(RUNTIME_CLASS(CMDIChildWndEx)))  // RTC 0x2eb7d8
//             ::PostMessage(pParent->m_hWnd, WM_CLOSE, 0, 0);      // 0x1802c52d8 == PostMessageA;
//                                                          // `lea 0x10(%r9),%edx` with %r9 == 0
//     }
//     CTooltipManager::DeleteToolTip(m_pToolTip);          // RVA 0x180ac0, &m_pToolTip == this+0x4b8
//     CPane::OnDestroy();                                  // tail jmp RVA 0xa1d40
// Slots 0x620 / 0x490 / 0x2f8 / 0x348 / 0x460 were read off the CDockablePane
// vftable (mfc140 RVA 0x2e3068) with vt.py; the IAT slots with iat.py.
// Deviations: (1) the DeleteToolTip call is dropped -- m_pToolTip (+0x4b8) has
// no storage here and this file's OnCreate cannot create one, so there is never
// a tooltip to delete; (2) the five self-virtuals dispatch statically
// (RemoveCaptionButtons and UndockPane to this file's own thunks, both still
// stubs); (3) OpenMFC's RemoveHiddenMDITabbedBar tolerates a NULL manager,
// which retail would fault on.
// Signature corrected to carry `this`.
// Symbol: ?OnDestroy@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CDockablePane__IEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__RemoveCaptionButtons_CDockablePane__IEAAXXZ(pThis);
    if (impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
            static_cast<const CBasePane*>(pThis), FALSE) != nullptr) {
        impl__RemoveFromMiniframe_CPane__MEAAXPEAVCWnd__W4AFX_DOCK_METHOD___Z(
            static_cast<CPane*>(pThis), nullptr, 0 /*DM_UNKNOWN*/);
    } else {
        impl__UndockPane_CDockablePane__UEAAXH_Z(pThis, TRUE);
    }
    if (impl__IsMDITabbed_CBasePane__UEBAHXZ(static_cast<const CBasePane*>(pThis))) {
        CDockingManager* pMgr = impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(
            reinterpret_cast<CGlobalUtils*>(impl__afxGlobalUtils__3VCGlobalUtils__A),
            impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(static_cast<const CBasePane*>(pThis)));
        impl__RemoveHiddenMDITabbedBar_CDockingManager__QEAAXPEAVCDockablePane___Z(pMgr, pThis);
        CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
        if (pParent != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pParent),
                impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ())) {
            ::PostMessage(pParent->m_hWnd, WM_CLOSE, 0, 0);
        }
    }
    // CTooltipManager::DeleteToolTip(m_pToolTip) skipped -- no m_pToolTip here.
    impl__OnDestroy_CPane__IEAAXXZ(static_cast<CPane*>(pThis));
}

// CDockablePane::OnDrawDragRect -- transcribed from retail entry RVA 0x40d40.
// `this` is never read (%rcx is overwritten at 0x40d61 without being saved), so
// the missing CDockablePane layout does not block this one. Retail does:
//     CWindowDC dc(CWnd::FromHandle(::GetDesktopWindow()));  // 0x2a1b60 / 0x289180,
//                                                            // slot 0x1802c4c00 =
//                                                            // USER32!GetDesktopWindow
//     dc.DrawDragRect(lprectNew, CSize(1,1), lprectOld, CSize(1,1), NULL, NULL);
// (RVA 0x2a3660. The {1,1} qword at rsp+0x98 is passed both in %r8 as `size`
// and at rsp+0x20 as `sizeLast`; rsp+0x28 and rsp+0x30 are the two zeroed
// CBrush* arguments. Unlike CMDIClientAreaWnd::DrawNewGroupRect, the caller's
// pointers are forwarded unchanged -- there is no NULL-to-empty-rect fixup.)
// Symbol: ?OnDrawDragRect@CDockablePane@@MEAAXPEBUtagRECT@@0@Z
extern "C" void MS_ABI impl__OnDrawDragRect_CDockablePane__MEAAXPEBUtagRECT__0_Z(
    CDockablePane* pThis, const RECT* lprectNew, const RECT* lprectOld) {
    (void)pThis;  // retail never reads `this` here

    CWnd* pDesktop = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetDesktopWindow());

    // CWindowDC exists in this DLL only as its impl__ thunks, so the object is
    // built in raw storage and torn down through the matching thunk.
    alignas(void*) unsigned char dcStorage[sizeof(CWindowDC)] = {};
    CWindowDC* pDC = reinterpret_cast<CWindowDC*>(dcStorage);
    impl___0CWindowDC__QEAA_PEAVCWnd___Z(pDC, pDesktop);

    SIZE size;
    size.cx = 1;
    size.cy = 1;
    impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
        pDC, lprectNew, size, lprectOld, size, nullptr, nullptr);

    impl___1CWindowDC__UEAA_XZ(pDC);
}

// STUB. Retail entry RVA 0x43400 (mfc140u; 0x43250 in mfc140), not
// transcribed. Its opening is
//     ClientToScreen(&point);                              // IAT 0x1802c5270
//     if (FindButton(point) != NULL || IsAutoHideMode()) { Default(); return; }   // RVA 0x45110; slot 0x3c8
// after which it resolves the tabbed parent (IsTabbed / GetParent chain with
// RTCs 0x2dba98 and 0x3aa178), then:
//   * inside a CMultiPaneFrameWnd (RTC 0x3aa2f8 on GetParentMiniFrame): calls
//     OnProcessDblClk (slot 0x588) and the mini frame's slot 0x4b8;
//   * else if !::IsWindow(m_hDefaultSlider): OnProcessDblClk only;
//   * else if the mini frame recorded at m_recentDockInfo+0x40 (this+0x2f8,
//     via FromHandlePermanent) is a CMultiPaneFrameWnd and one of the two
//     recorded rects at this+0x3c0/+0x3c8 is set: re-floats the pane into it
//     (slots 0x590/0x588/0x490/0x3c0, the mini frame's slot 0x4b0, 0x598 and
//     SetFocus);
//   * else CPane::OnLButtonDblClk(nFlags, point) (RVA 0xa2010).
// Blocked: the mini-frame slots 0x4b0/0x4b8 are not modelled (OpenMFC's
// CPaneFrameWnd has no retail-shaped vtable), FindButton above cannot see a
// caption button, and CPane::OnLButtonDblClk is itself a stub.
// Signature corrected to carry `this`.
// Symbol: ?OnLButtonDblClk@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nFlags, void* point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// CDockablePane::OnLButtonDown -- transcribed from retail entry RVA 0x41150
// (mfc140; 0x41300 in mfc140u):
//     if (m_nHot != 0) {                                    // +0x4d0
//         CMFCCaptionButton* pBtn = FindButtonByHit(m_nHot);   // RVA 0x45210
//         if (pBtn != NULL) {
//             SetFocus();                                   // RVA 0x2a7a70
//             m_nHit = m_nHot;                              // +0x4d4
//             pBtn->m_bPushed = TRUE;                       // button +0x8
//             RedrawButton(pBtn);                           // RVA 0x447e0
//             return;                                       // `jmp 0x41254`: skips the final SetFocus
//         }
//     } else {
//         // focus the pane's child window when there is exactly one
//         CWnd* pChild = CWnd::FromHandle(::GetWindow(m_hWnd, GW_CHILD));   // 0x1802c4c88 / RVA 0x289180
//         int nChildren = 0; CWnd* pLast = NULL;
//         for (; pChild != NULL; nChildren++) {
//             pLast = pChild;
//             pChild = CWnd::FromHandle(::GetWindow(pChild->m_hWnd, GW_HWNDNEXT));
//         }
//         if (nChildren == 1) pLast->SetFocus();
//     }
//     if (!IsAutoHideMode() && !IsTabbed()) {               // slots 0x3c8, 0x2e8
//         if (CanFloat()) m_bPrepareToFloat = true;         // slot 0x3a8 -- which in this
//                                                           // vftable is CBasePane::CanFloat
//                                                           // (RVA 0xc840); byte at +0x408
//         CPane::OnLButtonDown(nFlags, point);              // RVA 0x9fdd0
//     }
//     SetFocus();
// (GW_CHILD == 5 and GW_HWNDNEXT == 2 are the two `mov $N,%edx` before the
// GetWindow calls; slots read with vt.py off the CDockablePane vftable at
// mfc140 RVA 0x2e3068.)
// Deviation, forced: the caption-button arm (m_nHot != 0) is dropped -- m_nHot,
// m_nHit (+0x4d0/+0x4d4) and m_arrButtons (+0x488) have no storage here, and
// with no m_arrButtons no caption button can ever be hot, so the m_nHot == 0
// arm is the only one this build can reach. m_bPrepareToFloat is inside the
// padding window and is set at its retail offset (OnMouseMove / OnContextMenu /
// OnNcMouseMove / OnCancelMode read and clear it). The three self-virtuals
// dispatch statically; CPane::OnLButtonDown is still a generated stub as of
// this writing.
// Signature corrected to carry `this`; the by-value CPoint occupies one
// register and is forwarded to CPane::OnLButtonDown unexamined, as in retail.
// Symbol: ?OnLButtonDown@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nFlags, void* point) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check

    // m_nHot == 0 arm: give the focus to an only child.
    CWnd* pChild = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(pThis->m_hWnd, GW_CHILD));
    int nChildren = 0;
    CWnd* pLast = nullptr;
    for (; pChild != nullptr; nChildren++) {
        pLast = pChild;
        pChild = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(pChild->m_hWnd, GW_HWNDNEXT));
    }
    if (nChildren == 1) {
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(pLast);
    }

    if (!pThis->IsAutoHideMode() && !impl__IsTabbed_CPane__UEBAHXZ(static_cast<const CPane*>(pThis))) {
        if (impl__CanFloat_CBasePane__UEBAHXZ(static_cast<const CBasePane*>(pThis))) {
            DockablePaneField<bool>(pThis, kOffPrepareToFloat) = true;
        }
        impl__OnLButtonDown_CPane__IEAAXIVCPoint___Z(static_cast<CPane*>(pThis), nFlags, point);
    }
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pThis));
}

// STUB. Retail entry RVA 0x419f0 (mfc140u; 0x41840 in mfc140), ~1296 bytes,
// not transcribed. Its body touches the CDockablePane fields at +0x408
// (m_bPrepareToFloat), +0x438 (m_bPinState), +0x480 (m_pAutoHideBar), +0x4d0
// (m_nHot) and +0x4d4 (m_nHit) and dispatches the caption-button release
// (close / pin / maximize) through m_arrButtons. Blocked: m_nHot, m_nHit and
// m_arrButtons have no storage here and no caption button can exist in this
// build (see DrawCaption), so the release logic has nothing to act on.
// Signature corrected to carry `this`.
// Symbol: ?OnLButtonUp@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nFlags, void* point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// STUB. Retail entry RVA 0x41480 (~950 bytes), not transcribed. It branches on
// GetDockingMode() (vtable slot 0x380): the DT_IMMEDIATE arm drags the pane
// itself (CPane::UpdateVirtualRect RVA 0xa1ba0, then either
// CBaseTabbedPane::{slot 0x350} or FloatPane via slot 0x408 followed by
// CGlobalUtils::GetDockingManager/ForceAdjustLayout, RVA 0x6ccc0 / 0x6d800),
// the DT_STANDARD arm moves the drag frame (CMFCDragFrameImpl::MoveDragFrame,
// RVA 0x55450). Blocked: it reads and writes m_bPrepareToFloat (+0x408),
// m_bReadyToFloat (+0x409) and m_bCaptionButtonsCaptured (+0x4d8) -- the last
// is past the end of the object -- plus a drag anchor point at +0x240, which
// falls inside the 128-byte m_dragFrameImpl blob this repo models opaquely.
// Signature corrected to carry `this`.
// Symbol: ?OnMouseMove@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nFlags, void* point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// STUB. Retail entry RVA 0x40570:
//     Default();                                       // 0x180289090
//     if ({slot 0x2f0}())          // body at RVA 0x8780 is `!IsDocked()`
//         for (i = 0; i < m_arrButtons.m_nSize; i++)   // +0x498 / +0x490
//             m_arrButtons[i]->{+0x10} = i + 1;
//     else
//         // reserves the caption strip: GetCaptionHeight() (slot 0x358),
//         // ::CopyRect of lpncsp->rgrc[0] (0x1802c5240) and m_nBorderSize
//         // (+0x414); the rest of that arm was not transcribed.
// Blocked: m_arrButtons is at +0x488, past the end of the 1144-byte object.
// Signature corrected to carry `this`.
// Symbol: ?OnNcCalcSize@CDockablePane@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CDockablePane__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    CDockablePane* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    (void)pThis;
    (void)bCalcValidRects;
    (void)lpncsp;
}

// CDockablePane::OnNcHitTest -- transcribed from retail entry RVA 0x40de0,
// whose whole body is:
//     int nHit = HitTest(point, FALSE);      // vtable slot 0x678 == HitTest
//                                            // (`xor %r8d,%r8d` is bDetectCaption)
//     if (nHit != HTERROR) return (unsigned)nHit;   // `mov %eax,%eax`
//     return Default();                      // tail jmp 0x180289090
// Slot 0x678 of the CDockablePane vftable (RVA 0x2e3068) holds
// ?HitTest@CDockablePane@@UEAAHVCPoint@@H@Z (RVA 0x40e20), which this file
// exports, so the virtual call is reproduced as a direct call to its own thunk.
// 0x180289090 is ?Default@CWnd@@IEAA_JXZ.
// Signature corrected to carry `this`.
// Symbol: ?OnNcHitTest@CDockablePane@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CDockablePane__IEAA_JVCPoint___Z(
    CDockablePane* pThis, void* point) {
    if (pThis == nullptr) return HTERROR;
    const int nHit = impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(pThis, point, FALSE);
    if (nHit != HTERROR) return static_cast<__int64>(static_cast<unsigned int>(nHit));
    return impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// CDockablePane::OnNcLButtonDown -- transcribed from retail entry RVA 0x434e0,
// whose whole body is:
//     if (!IsDocked()) Default();     // vtable slot 0x2e0, then 0x180289090
// Neither argument is read. Slot 0x2e0 of the CDockablePane vftable holds
// ?IsDocked@CDockablePane@@UEBAHXZ (RVA 0xb300), which this file exports, so
// the virtual call is a direct call to its own thunk.
// Signature corrected to carry `this`.
// Symbol: ?OnNcLButtonDown@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcLButtonDown_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nHitTest, void* point) {
    (void)nHitTest;
    (void)point;
    if (pThis == nullptr) return;
    if (!impl__IsDocked_CDockablePane__UEBAHXZ(pThis)) {
        (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    }
}

// CDockablePane::OnNcMouseMove -- transcribed from retail entry RVA 0x44ff0,
// whose whole body is:
//     if (!m_bPrepareToFloat)                 // `cmpb $0x0,0x408(%rcx)`
//         OnTrackCaptionButtons(point);       // vtable slot 0x720, `mov %r8,%rdx`
//     Default();                              // tail jmp 0x180289090
// nHitTest is never read. Slot 0x720 holds
// ?OnTrackCaptionButtons@CDockablePane@@MEAAXVCPoint@@@Z (RVA 0x44f00), which
// this file exports (still a stub -- see `stubbed`), so the virtual call is a
// direct call to its own thunk. m_bPrepareToFloat is inside the padding blob;
// EnterDragMode above sets it.
// Signature corrected to carry `this`.
// Symbol: ?OnNcMouseMove@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nHitTest, void* point) {
    (void)nHitTest;
    if (pThis == nullptr) return;
    if (!DockablePaneField<bool>(pThis, kOffPrepareToFloat)) {
        impl__OnTrackCaptionButtons_CDockablePane__MEAAXVCPoint___Z(pThis, point);
    }
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// STUB. Retail entry RVA 0x408f0, not transcribed. Its opening is
//     if (CBasePane::m_bMultiThreaded)                  // RVA 0x3b6f00
//         ::EnterCriticalSection(<the section at RVA 0x3aabd0>);
//     CWindowDC dc(this);                               // RVA 0x2a1b60
//     ::GetUpdateRect(m_hWnd, &rectUpdate, FALSE);      // 0x1802c4d00
//     ::GetClientRect(m_hWnd, &rectClient);  ClientToScreen(&rectClient);
//     ::GetWindowRect(m_hWnd, &rectWindow);
//     ::OffsetRect(&rectClient, -rectWindow.left, -rectWindow.top);
//     ::OffsetRect(&rectWindow, -rectWindow.left, -rectWindow.top);
// after which it paints the caption and the caption buttons through the visual
// manager. Blocked: the paint itself needs m_arrButtons (+0x488) and the
// visual-manager singleton, neither of which exists here.
// Signature corrected to carry `this`.
// Symbol: ?OnNcPaint@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CDockablePane__IEAAXXZ(CDockablePane* pThis) {
    (void)pThis;
}

// STUB. Retail entry RVA 0x46120, fully decoded:
//     if (pNMHDR == NULL || m_pToolTip == NULL ||       // m_pToolTip at +0x4b8
//         m_pToolTip->m_hWnd == NULL || pNMHDR->hwndFrom != m_pToolTip->m_hWnd)
//         return FALSE;
//     switch (pNMHDR->idFrom) {
//     case 1:  nID = 0x3ea0; break;                     // close button
//     case 2:  nID = 0x3ea1;                            // pin button
//              if (::GetAsyncKeyState(VK_CONTROL) < 0)  // 0x1802c4c08, key 0x11
//                  nID += IsAutohideAllEnabled() ? 3 : 0;   // vtable slot 0x690
//              break;                                    // (`neg %eax ; sbb %ecx,%ecx
//                                                        //   ; and $0x3,%ecx`)
//     case 3:  nID = 0x428e; break;
//     default: return FALSE;
//     }
//     HINSTANCE h = AfxFindStringResourceHandle(nID);   // RVA 0x2accf0
//     if (h == NULL || !<the module-level CString at RVA 0x3ba308>.LoadString(h, nID))
//         return FALSE;                                 // RVA 0xdc00
//     ((TOOLTIPTEXT*)pNMHDR)->lpszText = that string;   // pNMHDR+0x18
//     return TRUE;
// Blocked: m_pToolTip is at +0x4b8, past the end of the 1144-byte object, so
// the guard that gates the whole body cannot be evaluated.
// Signature corrected to carry `this`.
// Symbol: ?OnNeedTipText@CDockablePane@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CDockablePane__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    CDockablePane* pThis, unsigned int id, NMHDR* pNMHDR, __int64* pResult) {
    (void)pThis;
    (void)id;
    (void)pNMHDR;
    (void)pResult;
    return 0;
}

// CDockablePane::OnPaint -- transcribed from retail. This export is absent from
// mfc140_rva_symbols.json only because that map is keyed by RVA and the linker
// ICF-folded this body onto another name; it is NOT missing from the DLL. In
// mfc140.dll it shares RVA 0x41000 with ?OnPaint@CDockSite@@IEAAXXZ and in
// mfc140u.dll RVA 0x411b0 with the same twin, and the whole body (bar the
// /GS cookie prologue and epilogue) is:
//     mov %rcx,%rdx ; lea 0x20(%rsp),%rcx ; call 0x1802a1c60  ; CPaintDC dc(this)
//     lea 0x20(%rsp),%rcx ; call 0x1802a1d10                  ; dc.~CPaintDC()
// (0x2a1c60 == ??0CPaintDC@@QEAA@PEAVCWnd@@@Z and 0x2a1d10 == ??1CPaintDC@@UEAA@XZ
// in the mfc140 RVA map; the mfc140u pair is 0x2a3d20 / 0x2a3dd0.) So the whole
// handler is a bare BeginPaint/EndPaint pair that validates the update region
// and paints nothing of its own.
// Signature corrected to carry `this` (the generated stub dropped it).
// As with OnDrawDragRect above, CPaintDC exists in this DLL only as its impl__
// thunks, so the object is built in raw storage and torn down through the
// matching thunk. Neither thunk dispatches a virtual, so the unset vfptr in
// that storage is never used.
// Symbol: ?OnPaint@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CDockablePane__IEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, static_cast<CWnd*>(pThis));
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// CDockablePane::OnPressCloseButton -- transcribed from retail entry RVA
// 0x41d50, whose whole body is:
//     CFrameWnd* pFrame = AFXGetParentFrame(this);          // RVA 0x6bb10
//     if (pFrame != NULL && pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)) &&
//         ::SendMessage(pFrame->m_hWnd, AFX_WM_ON_PRESS_CLOSE_BUTTON,
//                       0, (LPARAM)this) != 0)
//         return;                                           // the pane was handled
//     if (IsAutoHideMode())                                 // vtable slot 0x3c8
//         SetAutoHideMode(FALSE, GetCurrentAlignment(), NULL, TRUE);
//                                    // slot 0x6e0; slot 0x338; `movl $1,0x20(%rsp)`
//     ShowPane(FALSE, FALSE, FALSE);                        // slot 0x458
//     AdjustDockingLayout(NULL);                            // slot 0x4e0
// The descriptor at RVA 0x338990 reads "CFrameWnd" through its
// m_lpszClassName; the message id is the ?AFX_WM_ON_PRESS_CLOSE_BUTTON@@3IA
// export at RVA 0x3ba300. The IAT slot 0x1802c5378 resolves to
// USER32!SendMessageA in the ANSI twin the disassembler reads, i.e.
// ::SendMessage in this UNICODE build.
// Deviation: OpenMFC does not model this class's vftable, so the three
// self-virtuals dispatch statically -- IsAutoHideMode/SetAutoHideMode through
// the C++ definitions in this file and ShowPane/AdjustDockingLayout through the
// matching exports. A derived override would not run.
// Signature corrected to carry `this`.
// Symbol: ?OnPressCloseButton@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__OnPressCloseButton_CDockablePane__MEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;

    CFrameWnd* pParentFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(
        static_cast<const CWnd*>(pThis));
    if (pParentFrame != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pParentFrame),
            impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ()) &&
        ::SendMessage(pParentFrame->m_hWnd, impl__AFX_WM_ON_PRESS_CLOSE_BUTTON__3IA,
                      0, reinterpret_cast<LPARAM>(pThis)) != 0) {
        return;
    }

    if (pThis->IsAutoHideMode()) {
        pThis->SetAutoHideMode(
            FALSE,
            impl__GetCurrentAlignment_CBasePane__UEBAKXZ(static_cast<const CBasePane*>(pThis)),
            nullptr, TRUE);
    }
    impl__ShowPane_CDockablePane__UEAAXHHH_Z(pThis, FALSE, FALSE, FALSE);
    impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(static_cast<CBasePane*>(pThis), nullptr);
}

// CDockablePane::OnRButtonDown -- transcribed from retail entry RVA 0x41440,
// whose whole body is:
//     SetFocus();                                        // RVA 0x2a7a70
//     CMFCPopupMenu* p = CMFCPopupMenu::m_pActivePopupMenu;   // RVA 0x3b6fe8
//     if (p != NULL && CWnd::FromHandlePermanent(p->m_hWnd) != NULL)
//         CMFCPopupMenu::UpdateAllShadows(NULL);         // RVA 0xbb290, `xor %ecx,%ecx`
//     Default();                                         // tail jmp 0x180289090
// Neither argument is read. `mov 0x40(%rcx),%rcx` is CWnd::m_hWnd on the popup
// menu; the C++ member is used here rather than the literal offset because
// OpenMFC's CWnd is not laid out like retail's.
// Symbol: ?OnRButtonDown@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonDown_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nFlags, void* point) {
    (void)nFlags;
    (void)point;
    if (pThis == nullptr) return;
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pThis));

    CWnd* pActivePopup = reinterpret_cast<CWnd*>(impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA);
    if (pActivePopup != nullptr &&
        impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(pActivePopup->m_hWnd) != nullptr) {
        impl__UpdateAllShadows_CMFCPopupMenu__SAXPEAUtagRECT___Z(nullptr);
    }
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// CDockablePane::OnSetFocus -- transcribed from retail entry RVA 0x46260 (mfc140;
// 0x46410 in mfc140u):
//     CWnd::OnSetFocus(pOldWnd);                           // RVA 0x28d720, %rdx passed through
//     CPaneFrameWnd* pMini = GetParentMiniFrame(FALSE);    // slot 0x460
//     if (pMini != NULL && pMini->IsKindOf(RUNTIME_CLASS(CMultiPaneFrameWnd)))   // RTC 0x3aa2f8
//         ((CMultiPaneFrameWnd*)pMini)->m_hWndLastFocused = m_hWnd;   // store at +0x2f8
// (m_hWndLastFocused is the HWND member atlmfc/include/afxmultipaneframewnd.h:120
// declares; the descriptor at 0x3aa2f8 reads "CMultiPaneFrameWnd", object size
// 0x300, so +0x2f8 is its last member.)
// Deviation, forced: the store is dropped. OpenMFC declares no
// CMultiPaneFrameWnd type at all -- phase4/src/featurepack/docking/
// CMultiPaneFrameWnd.cpp handles it as a void* keyed into a side table whose
// PaneState (detail/CbarcoreSupport.h:928) has no last-focused field -- so
// there is nowhere to record the handle. The mini-frame lookup and IsKindOf
// test are therefore not performed either (they would have no effect).
// Signature corrected to carry `this`.
// Symbol: ?OnSetFocus@CDockablePane@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CDockablePane__IEAAXPEAVCWnd___Z(CDockablePane* pThis, CWnd* pOldWnd) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(static_cast<CWnd*>(pThis), pOldWnd);
    // CMultiPaneFrameWnd::m_hWndLastFocused update skipped -- see above.
}

// CDockablePane::OnSetText -- transcribed from retail entry RVA 0x459a0
// (mfc140u; the ANSI twin ?OnSetText@CDockablePane@@IEAAHPEBD@Z at mfc140 RVA
// 0x457f0 is the body read; the two differ only in the string import):
//     LRESULT lRes = Default();                                   // RVA 0x289090
//     if (lRes == 0) return 0;
//     if (IsTabbed()) {                                           // vtable slot 0x2e8
//         CWnd* pTabWnd = CWnd::FromHandle(::GetParent(m_hWnd));  // RVA 0x289180
//         if (pTabWnd != NULL && !pTabWnd->IsKindOf(<RTC 0x2dba98 "CMFCBaseTabCtrl">))
//             pTabWnd = NULL;
//         CWnd* pTabbedBar = CWnd::FromHandle(::GetParent(pTabWnd->m_hWnd));  // pTabWnd unchecked
//         if (pTabbedBar == NULL || !pTabbedBar->IsKindOf(<RTC 0x2db168 "CBaseTabbedPane">))
//             return lRes;
//         int iTab = pTabWnd->GetTabFromHwnd(m_hWnd);             // tab vtable slot 0x448
//         if (iTab < 0 || iTab >= pTabWnd->GetTabsNum())          // tab vtable slot 0x368
//             return lRes;                                        // (0x15370: `mov 0x150(%rcx),%eax`)
//         CString strLabel;        // inlined default ctor: slot 3 of the unnamed object at
//                                  // RVA 0x3ab440 (the same call the exported CString(LPCTSTR)
//                                  // ctor at 0xdd70 makes), +0x18 -> m_pszData
//         pTabWnd->GetTabLabel(iTab, strLabel);                   // tab vtable slot 0x388
//         if (lpszText != NULL && wcscmp(strLabel, lpszText) != 0) {   // mfc140u IAT 0x1802c7770
//             CString strNew(lpszText);                           // RVA 0xdd70
//             pTabWnd->SetTabLabel(iTab, strNew);                 // tab vtable slot 0x390
//         }
//         return lRes;
//     }
//     CPaneFrameWnd* pMini = GetParentMiniFrame(FALSE);           // vtable slot 0x460
//     if (pMini != NULL) {
//         pMini->SetWindowPos(NULL, 0, 0, 0, 0, 0x37);            // CWnd::SetWindowPos, RVA 0x2a7970
//     } else if (IsAutoHideMode()) {                              // vtable slot 0x3c8
//         ::RedrawWindow(m_pAutoHideBar->m_hWnd, NULL, NULL, 0x105);   // +0x480; RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW
//         SetWindowPos(NULL, 0, 0, 0, 0, 0x37, NULL);             // vtable slot 0x480 (CBasePane's)
//         AdjustDockingLayout(NULL);                              // vtable slot 0x4e0
//     } else {
//         SetWindowPos(NULL, 0, 0, 0, 0, 0x37, NULL);             // vtable slot 0x480
//     }
//     return lRes;
// 0x37 == SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED.
// The tab-control slots 0x448 / 0x388 / 0x390 were read off the CMFCBaseTabCtrl
// vftable (mfc140 RVA 0x2dbd88) with vt.py and go to the matching exports in
// tabs/CMFCBaseTabCtrl.cpp. Slot 0x368 is the unexported inline GetTabsNum;
// the upper bound is folded into the GetTabLabel call instead: OpenMFC's
// impl__GetTabLabel (tabs/CMFCBaseTabCtrl.cpp:952) applies exactly the
// `iTab < 0 || iTab >= count` test and returns FALSE for it, and retail's own
// GetTabLabel (RVA 0x16860) returns TRUE for every in-range index, so
// `!GetTabLabel(...)` is the same predicate retail evaluates separately. That
// export also leaves the label empty (the per-tab records are not modelled
// there), so today the wcscmp below sees "" and any non-empty caption is
// pushed through SetTabLabel -- the correct outcome whenever the labels differ.
// m_pAutoHideBar comes from the docking side table (AutoHideBarOf).
// Deviations: NULL checks on `this`, on pTabWnd before its parent is read
// (retail faults there when the parent is not a CMFCBaseTabCtrl) and on the
// auto-hide bar (retail dereferences it); the self-virtuals dispatch statically.
// Signature corrected to carry `this`.
// Symbol: ?OnSetText@CDockablePane@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CDockablePane__IEAAHPEB_W_Z(CDockablePane* pThis, const wchar_t* lpszText) {
    if (pThis == nullptr) return 0;   // deviation: retail has no NULL check
    const __int64 lRes = impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    if (lRes == 0) return 0;

    if (impl__IsTabbed_CPane__UEBAHXZ(static_cast<const CPane*>(pThis))) {
        CWnd* pTabWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
        if (pTabWnd != nullptr &&
            !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pTabWnd),
                impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ())) {
            pTabWnd = nullptr;
        }
        if (pTabWnd == nullptr) return static_cast<int>(lRes);   // deviation: retail faults here
        CWnd* pTabbedBar = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pTabWnd->m_hWnd));
        if (pTabbedBar == nullptr ||
            !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pTabbedBar),
                impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ())) {
            return static_cast<int>(lRes);
        }
        const CMFCBaseTabCtrl* pTab = reinterpret_cast<const CMFCBaseTabCtrl*>(pTabWnd);
        const int iTab = impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(pTab, pThis->m_hWnd);
        if (iTab < 0) return static_cast<int>(lRes);
        CString strLabel;
        if (!impl__GetTabLabel_CMFCBaseTabCtrl__UEBAHHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                pTab, iTab, strLabel)) {
            return static_cast<int>(lRes);   // == retail's `iTab >= GetTabsNum()` test, see above
        }
        if (lpszText != nullptr && wcscmp(strLabel.GetString(), lpszText) != 0) {
            CString strNew(lpszText);
            impl__SetTabLabel_CMFCBaseTabCtrl__UEAAHHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                reinterpret_cast<CMFCBaseTabCtrl*>(pTabWnd), iTab, strNew);
        }
        return static_cast<int>(lRes);
    }

    const unsigned int kSwpFrame = SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED;  // 0x37
    void* pMini = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), FALSE);
    if (pMini != nullptr) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(static_cast<CWnd*>(pMini), nullptr, 0, 0, 0, 0, kSwpFrame);
    } else if (pThis->IsAutoHideMode()) {
        CWnd* pBar = static_cast<CWnd*>(static_cast<CBasePane*>(AutoHideBarOf(pThis)));
        if (pBar != nullptr) {   // deviation: retail dereferences it unchecked
            ::RedrawWindow(pBar->m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
        }
        impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
            static_cast<CBasePane*>(pThis), nullptr, 0, 0, 0, 0, kSwpFrame, nullptr);
        impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(static_cast<CBasePane*>(pThis), nullptr);
    } else {
        impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
            static_cast<CBasePane*>(pThis), nullptr, 0, 0, 0, 0, kSwpFrame, nullptr);
    }
    return static_cast<int>(lRes);
}

// CDockablePane::OnSettingChange -- transcribed from retail entry RVA 0x46290
// (mfc140u; the ANSI twin ?OnSettingChange@CDockablePane@@IEAAXIPEBD@Z is at
// mfc140 RVA 0x460e0 and is the body read). %rdx/%r8 pass straight through:
//     CBasePane::OnSettingChange(uFlags, lpszSection);   // RVA 0xcf10 (mfc140)
//     if (m_cyGripper > 0) {                             // +0x40c, `cmpl $0 ; jle`
//         m_cyGripper = 0;
//         EnableGripper(TRUE);                           // vtable slot 0x3f0, `mov $1,%edx`
//     }
// Slot 0x3f0 of the CDockablePane vftable (mfc140 RVA 0x2e3068) holds
// ?EnableGripper@CDockablePane@@UEAAXH@Z (RVA 0x40160), which this file
// exports, so the virtual call is a direct call to its own thunk (still a stub
// -- see `stubbed`). m_cyGripper is inside the reachable window and is read and
// cleared at its retail offset. Note nothing here writes a non-zero
// m_cyGripper yet (EnableGripper is that stub), so today the branch is not
// taken. The CBasePane::OnSettingChange sibling is itself a stub whose
// parameter list omits `this`; it reads no argument, so the extra register
// argument is harmless under the MS x64 ABI.
// Deviation: NULL check on `this`.
// Signature corrected to carry `this`.
// Symbol: ?OnSettingChange@CDockablePane@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CDockablePane__IEAAXIPEB_W_Z(
    CDockablePane* pThis, unsigned int uFlags, const wchar_t* lpszSection) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__OnSettingChange_CBasePane__IEAAXIPEB_W_Z(static_cast<CBasePane*>(pThis), uFlags, lpszSection);
    if (DockablePaneField<int>(pThis, kOffCyGripper) > 0) {
        DockablePaneField<int>(pThis, kOffCyGripper) = 0;
        impl__EnableGripper_CDockablePane__UEAAXH_Z(pThis, TRUE);
    }
}

// STUB. Retail entry RVA 0x42950 (mfc140u; 0x427a0 in mfc140), decoded but
// not transcribed:
//     if (!IsAutoHideMode() && !::IsWindow(m_hDefaultSlider)) return;   // slot 0x3c8; +0x4c0
//     DWORD dwExStyle = GetDockSiteFrameWnd()->GetExStyle();            // slot 0x348; RVA 0x2a75d0
//     m_nSlideStep++;                                                   // +0x450
//     CRect rectWnd;    ::GetWindowRect(m_hWnd, &rectWnd);             GetDockSiteFrameWnd()->ScreenToClient(&rectWnd);
//     CRect rectSlider; ::GetWindowRect(m_hDefaultSlider, &rectSlider); GetDockSiteFrameWnd()->ScreenToClient(&rectSlider);
//     if (m_ahSlideMode == AFX_AHSM_MOVE /*1*/) {                       // +0x474
//         BOOL bRTL = dwExStyle & WS_EX_LAYOUTRTL /*0x400000*/;
//         OffsetRectForSliding(rectWnd, bSlideOut, bRTL);               // RVA 0x429a0
//         OffsetRectForSliding(rectSlider, bSlideOut, bRTL);
//         if (bSlideOut) {
//             CPoint pt = CalcCorrectOffset(rectWnd, bRTL);             // RVA 0x42c10
//             ::OffsetRect(&rectWnd, pt.x, pt.y); ::OffsetRect(&rectSlider, pt.x, pt.y);
//         }
//     } else {
//         CalcRectForSliding(rectWnd, rectSlider, bSlideOut);           // RVA 0x42a70
//     }
//     SetWindowPos(NULL, rectWnd.left, rectWnd.top, rectWnd.Width(), rectWnd.Height(),
//                  SWP_NOZORDER | SWP_NOACTIVATE /*0x14*/, NULL);      // slot 0x480
//     ::SetWindowPos(m_hDefaultSlider, NULL, rectSlider.left, rectSlider.top,
//                    rectSlider.Width(), rectSlider.Height(), 0x14);   // IAT 0x1802c4d10
// Blocked: every arm branches on m_ahSlideMode (+0x474), which has no storage
// in the 1144-byte object (it is where a derived CBaseTabbedPane's own padding
// blob begins), and the slide geometry it drives (Slide below) is blocked too.
// Symbol: ?OnSlide@CDockablePane@@MEAAXH@Z
extern "C" void MS_ABI impl__OnSlide_CDockablePane__MEAAXH_Z(CDockablePane* pThis, int bSlideOut) {
    (void)pThis;
    (void)bSlideOut;
}

// CDockablePane::OnTimer -- transcribed from retail entry RVA 0x42400 (mfc140u;
// 0x42250 in mfc140). The switch is `sub $0xec01,%rdx ; je ; sub $1 ; je ;
// cmp $1 ; jne default`:
//     switch (nIDEvent) {
//     case AFX_TIMER_ID_AUTO_HIDE_SLIDE_IN_EVENT:   // 0xEC01: bSlideOut = FALSE, m_bIsHiding = TRUE
//     case AFX_TIMER_ID_AUTO_HIDE_SLIDE_OUT_EVENT:  // 0xEC02: bSlideOut = TRUE,  m_bIsHiding = FALSE
//         m_bIsHiding = !bSlideOut;                 // +0x470 (`lea 0x1(%rdi),%eax` / `xor %eax,%eax`)
//         OnSlide(bSlideOut);                       // vtable slot 0x748
//         if (CheckStopSlideCondition(bSlideOut)) { // vtable slot 0x740
//             ::KillTimer(m_hWnd, m_nSlideTimer);   // +0x448, IAT 0x1802c5360
//             m_bIsSliding = FALSE;                 // +0x468
//             m_nSlideTimer = 0;
//             m_nSlideStep = 0;                     // +0x450
//             if (bSlideOut) {
//                 ::RedrawWindow(m_hWnd, NULL, NULL, 0x585);           // IAT 0x1802c5388
//                 ::RedrawWindow(m_hDefaultSlider, NULL, NULL, RDW_INVALIDATE);   // +0x4c0
//                 if (m_nAutoHideConditionTimerID != 0)                // +0x440
//                     ::KillTimer(m_hWnd, m_nAutoHideConditionTimerID);
//                 m_nAutoHideConditionTimerID = ::SetTimer(m_hWnd,
//                     AFX_TIMER_ID_CHECK_AUTO_HIDE_CONDITION /*0xEC03*/,
//                     CDockablePane::m_nTimeOutBeforeAutoHide /*RVA 0x3aaac4*/, NULL);  // IAT 0x1802c5330
//             } else {
//                 ShowWindow(SW_HIDE);                                 // RVA 0x2a79e0
//                 GetDefaultPaneDivider()->ShowWindow(SW_HIDE);        // RVA 0x45cd0, unchecked
//             }
//         }
//         Default();                                // RVA 0x289090, on every slide path
//         break;
//     case AFX_TIMER_ID_CHECK_AUTO_HIDE_CONDITION:  // 0xEC03
//         if (CheckAutoHideCondition()) {           // vtable slot 0x738
//             ::KillTimer(m_hWnd, m_nAutoHideConditionTimerID);
//             m_nAutoHideConditionTimerID = 0;
//         }
//         break;                                    // NO Default() on this case
//     default:
//         Default();
//     }
// 0x585 == RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_UPDATENOW|RDW_FRAME.
// Slots 0x738/0x740/0x748 of the CDockablePane vftable (mfc140 RVA 0x2e3068)
// hold CheckAutoHideCondition / CheckStopSlideCondition / OnSlide (RVAs
// 0x423c0 / 0x42570 / 0x427a0), all exported by this file, so the three
// virtual calls go to its own thunks -- all three of which are still stubs
// returning 0 / doing nothing (see `stubbed`), so today the slide timers never
// stop through this path and the auto-hide-condition timer is never killed.
// The five timer/flag fields are inside the reachable padding window and are
// used at their retail offsets; m_hDefaultSlider comes from the file-private
// DockablePaneExtra table. NOTE: retail's .data initialises
// ?m_nTimeOutBeforeAutoHide@CDockablePane@@2HA to 700 (read at mfc140 RVA
// 0x3aaac4), but OpenMFC's definition (docking/StaticData.cpp:38) starts at 0,
// so until that file is corrected the SetTimer below asks for a 0 ms period
// (USER32 clamps it to USER_TIMER_MINIMUM) -- reported as a headerRequest.
// Deviations: NULL check on `this`; the divider is only hidden / redrawn when
// GetDefaultPaneDivider() / the stored HWND is non-NULL (retail would call
// CWnd::ShowWindow on a NULL object and ::RedrawWindow(NULL, ...), which
// invalidates the whole desktop).
// Signature corrected to carry `this`.
// Symbol: ?OnTimer@CDockablePane@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CDockablePane__IEAAX_K_Z(CDockablePane* pThis, unsigned __int64 nIDEvent) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    if (nIDEvent == kTimerAutoHideSlideIn || nIDEvent == kTimerAutoHideSlideOut) {
        const int bSlideOut = (nIDEvent == kTimerAutoHideSlideOut) ? TRUE : FALSE;
        DockablePaneField<int>(pThis, kOffIsHiding) = bSlideOut ? FALSE : TRUE;
        impl__OnSlide_CDockablePane__MEAAXH_Z(pThis, bSlideOut);
        if (impl__CheckStopSlideCondition_CDockablePane__MEAAHH_Z(pThis, bSlideOut)) {
            ::KillTimer(pThis->m_hWnd, DockablePaneField<UINT_PTR>(pThis, kOffSlideTimer));
            DockablePaneField<int>(pThis, kOffIsSliding) = FALSE;
            DockablePaneField<UINT_PTR>(pThis, kOffSlideTimer) = 0;
            DockablePaneField<int>(pThis, kOffSlideStep) = 0;
            if (bSlideOut) {
                ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr,
                               RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_FRAME);
                const HWND hSlider = ExtraOf(pThis).hDefaultSlider;
                if (hSlider != nullptr) {   // deviation: retail passes it unchecked
                    ::RedrawWindow(hSlider, nullptr, nullptr, RDW_INVALIDATE);
                }
                UINT_PTR& nCondTimer = DockablePaneField<UINT_PTR>(pThis, kOffAutoHideConditionTimerID);
                if (nCondTimer != 0) {
                    ::KillTimer(pThis->m_hWnd, nCondTimer);
                }
                nCondTimer = ::SetTimer(pThis->m_hWnd, static_cast<UINT_PTR>(kTimerCheckAutoHide),
                                        static_cast<UINT>(impl__m_nTimeOutBeforeAutoHide_CDockablePane__2HA),
                                        nullptr);
            } else {
                impl__ShowWindow_CWnd__QEAAHH_Z(static_cast<CWnd*>(pThis), SW_HIDE);
                void* pDivider = impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(pThis);
                if (pDivider != nullptr) {   // deviation: retail calls ShowWindow on it unchecked
                    impl__ShowWindow_CWnd__QEAAHH_Z(static_cast<CWnd*>(pDivider), SW_HIDE);
                }
            }
        }
        (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
        return;
    }
    if (nIDEvent == kTimerCheckAutoHide) {
        if (impl__CheckAutoHideCondition_CDockablePane__MEAAHXZ(pThis)) {
            ::KillTimer(pThis->m_hWnd, DockablePaneField<UINT_PTR>(pThis, kOffAutoHideConditionTimerID));
            DockablePaneField<UINT_PTR>(pThis, kOffAutoHideConditionTimerID) = 0;
        }
        return;   // retail does NOT call Default() for this timer
    }
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// STUB. Retail entry RVA 0x44f00; not transcribed -- it tracks the caption
// buttons held in m_arrButtons (+0x488), which is past the usable window.
// Signature corrected to carry `this` (OnNcMouseMove above calls it).
// Symbol: ?OnTrackCaptionButtons@CDockablePane@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CDockablePane__MEAAXVCPoint___Z(
    CDockablePane* pThis, void* point) {
    (void)pThis;
    (void)point;
}

// CDockablePane::OnUpdateCmdUI -- transcribed from retail entry RVA 0x45450
// (mfc140u; 0x452a0 in mfc140). %rdx/%r8 are passed straight through to the
// first call (neither register is touched before it):
//     UpdateDialogControls(pTarget, bDisableIfNoHndler);   // RVA 0x28f8c0 (mfc140)
//     CWnd* pFocus = CWnd::FromHandle(::GetFocus());        // RVA 0x289180
//     BOOL bActiveOld = m_bActive;                          // +0x41c
//     BOOL bActive = FALSE;
//     if (pFocus != NULL && pFocus->m_hWnd != NULL &&
//         (::IsChild(m_hWnd, pFocus->m_hWnd) || pFocus->m_hWnd == m_hWnd))
//         bActive = TRUE;
//     m_bActive = bActive;
//     if (bActive != bActiveOld)
//         ::SendMessage(m_hWnd, WM_NCPAINT /*0x85*/, 0, 0);
// (IAT slots 0x1802c51d8 / 0x1802c51e0 / 0x1802c5378 resolve to USER32
// GetFocus / IsChild / SendMessageA in the ANSI twin, i.e. ::SendMessage here.)
// m_bActive is inside the reachable padding window and is stored at its retail
// offset. UpdateDialogControls is the real body at core/window/Thunks.cpp:1743.
// Deviation: NULL check on `this` (retail has none).
// Signature corrected to carry `this`.
// Symbol: ?OnUpdateCmdUI@CDockablePane@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CDockablePane__UEAAXPEAVCFrameWnd__H_Z(
    CDockablePane* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(
        static_cast<CWnd*>(pThis), static_cast<CCmdTarget*>(pTarget), bDisableIfNoHndler);
    CWnd* pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
    const int bActiveOld = DockablePaneField<int>(pThis, kOffActive);
    int bActive = FALSE;
    if (pFocus != nullptr && pFocus->m_hWnd != nullptr &&
        (::IsChild(pThis->m_hWnd, pFocus->m_hWnd) || pFocus->m_hWnd == pThis->m_hWnd)) {
        bActive = TRUE;
    }
    DockablePaneField<int>(pThis, kOffActive) = bActive;
    if (bActive != bActiveOld) {
        ::SendMessage(pThis->m_hWnd, WM_NCPAINT, 0, 0);
    }
}

// STUB. Retail entry RVA 0x46630 (mfc140u; 0x46480 in mfc140), fully decoded
// (lParam is never read):
//     if (wParam & 0x10) {                                          // `test %dl,%r8b`
//         CTooltipManager::CreateToolTip(m_pToolTip, this, 0x10);   // +0x4b8; RVA 0x1808e0
//         for (int i = 1; i < 4; i++) {
//             CRect rc; ::SetRectEmpty(&rc);                        // IAT 0x1802c5368
//             m_pToolTip->AddTool(this, LPSTR_TEXTCALLBACK, &rc, i);   // RVA 0x273c00, unchecked NULL
//         }
//         UpdateTooltips();                                         // RVA 0x40440
//     }
//     return 0;
// Blocked, as OnCreate above: m_pToolTip (+0x4b8) has no storage here.
// Signature corrected to carry `this`.
// Symbol: ?OnUpdateToolTips@CDockablePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CDockablePane__IEAA_J_K_J_Z(
    CDockablePane* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// CDockablePane::PreTranslateMessage -- PARTIAL transcription of retail entry
// RVA 0x45550 (mfc140u; 0x453a0 in mfc140). The message-id decode is the
// `sub`/`je` chain at 0x453b5..0x45409; the rest reads:
//     switch (pMsg->message) {
//     case WM_NCLBUTTONDOWN: case WM_NCLBUTTONUP: case WM_NCRBUTTONDOWN:
//     case WM_NCRBUTTONUP: case WM_NCMBUTTONDOWN: case WM_NCMBUTTONUP:
//     case WM_KEYDOWN: case WM_SYSKEYDOWN: case WM_MOUSEMOVE: case WM_LBUTTONDOWN:
//     case WM_LBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONUP: case WM_MBUTTONDOWN:
//     case WM_MBUTTONUP:
//         if (m_pToolTip != NULL && m_pToolTip->m_hWnd != NULL)          // +0x4b8
//             ::SendMessage(m_pToolTip->m_hWnd, TTM_RELAYEVENT /*0x407*/, 0, (LPARAM)pMsg);
//     }
//     if (pMsg->message == WM_KEYDOWN) {
//         if ((GetDockingMode() & DT_STANDARD /*`test $2,%al`*/) &&      // vtable slot 0x380
//             m_bPrepareToFloat && pMsg->wParam == VK_ESCAPE) {           // +0x408
//             ::PostMessage(m_hWnd, WM_CANCELMODE, 0, 0);                 // IAT 0x1802c52d8
//             return TRUE;
//         }
//         if (IsTabbed() && pMsg->wParam == VK_ESCAPE) {                  // vtable slot 0x2e8
//             CBaseTabbedPane* pTabbed = GetParentTabbedPane();          // RVA 0xc9b0
//             if (pTabbed != NULL) {
//                 CPaneFrameWnd* pMini = pTabbed->GetParentMiniFrame(FALSE);   // slot 0x460
//                 if (pTabbed->m_bPrepareToFloat ||                       // +0x408 on the tabbed pane
//                     (pMini != NULL && pMini->m_bDragging /*+0x10d*/)) {
//                     ::PostMessage((pMini != NULL ? pMini : pTabbed)->m_hWnd, WM_CANCELMODE, 0, 0);
//                     return TRUE;
//                 }
//             }
//         }
//         if (pMsg->wParam == VK_ESCAPE && (GetDockingMode() & DT_SMART)) {   // `test %al,%al ; jns`
//             CDockingManager* pMgr = afxGlobalUtils.GetDockingManager(
//                 CWnd::FromHandle(::GetParent(m_hWnd)));                 // RVA 0x6ccc0
//             CSmartDockingManager* pSD = pMgr ? pMgr->{+0x308} : NULL;
//             if (pSD != NULL && pSD->{+0xc} && pSD->{+0x8}) {           // created && started
//                 CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//                 if (pParent != NULL && pParent->IsKindOf(<RTC 0x3aa418 "CPaneFrameWnd">) &&
//                     CWnd::FromHandle(::GetCapture()) == pParent) {      // IAT 0x1802c4c38
//                     ::PostMessage(pParent->m_hWnd, WM_CANCELMODE, 0, 0);
//                     return TRUE;
//                 }
//             }
//         }
//     }
//     return CBasePane::PreTranslateMessage(pMsg);                        // RVA 0xcbb0
// Reproduced: the DT_STANDARD escape arm, the tabbed-pane escape arm and the
// base-class tail. Dropped, each because the state it reads has no storage in
// this build: (1) the tooltip relay -- m_pToolTip (+0x4b8) is past the end of
// the 1144-byte object and this file's OnCreate cannot create one, so there is
// never a tooltip to relay to; (2) the mini-frame's dragging byte (+0x10d) --
// OpenMFC keeps it in a side table private to docking/CPaneFrameWnd.cpp, so
// that half of the tabbed-pane condition reads as FALSE and only the tabbed
// pane's own m_bPrepareToFloat (inside its reachable padding window) is
// honoured; (3) the DT_SMART arm -- OpenMFC's CDockingManager has no
// smart-docking-manager member at +0x308 and no CSmartDockingManager, so the
// condition cannot be evaluated and the arm is skipped.
// Other deviations: NULL check on `this` and pMsg; the self-virtuals dispatch
// statically; CBasePane::PreTranslateMessage (docking/CBasePane.cpp:1423) is
// still a stub returning 0 whose parameter list omits `this`.
// Signature corrected to carry `this`.
// Symbol: ?PreTranslateMessage@CDockablePane@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CDockablePane__UEAAHPEAUtagMSG___Z(CDockablePane* pThis, MSG* pMsg) {
    if (pThis == nullptr || pMsg == nullptr) return FALSE;   // deviation: retail has no NULL check

    // Tooltip relay (TTM_RELAYEVENT) dropped -- no m_pToolTip here, see above.

    if (pMsg->message == WM_KEYDOWN) {
        if ((impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(static_cast<const CBasePane*>(pThis)) & kDockTypeStandard) &&
            DockablePaneField<bool>(pThis, kOffPrepareToFloat) && pMsg->wParam == VK_ESCAPE) {
            ::PostMessage(pThis->m_hWnd, WM_CANCELMODE, 0, 0);
            return TRUE;
        }
        if (impl__IsTabbed_CPane__UEBAHXZ(static_cast<const CPane*>(pThis)) && pMsg->wParam == VK_ESCAPE) {
            CDockablePane* pTabbed = static_cast<CDockablePane*>(
                impl__GetParentTabbedPane_CBasePane__QEBAPEAVCBaseTabbedPane__XZ(static_cast<const CBasePane*>(pThis)));
            if (pTabbed != nullptr) {
                void* pMini = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
                    static_cast<const CBasePane*>(pTabbed), FALSE);
                // pMini->m_bDragging (+0x10d) is not reachable here -- see above.
                if (DockablePaneField<bool>(pTabbed, kOffPrepareToFloat)) {
                    CWnd* pTarget = pMini != nullptr ? static_cast<CWnd*>(pMini) : static_cast<CWnd*>(pTabbed);
                    ::PostMessage(pTarget->m_hWnd, WM_CANCELMODE, 0, 0);
                    return TRUE;
                }
            }
        }
        // DT_SMART escape arm dropped -- no smart-docking manager here, see above.
    }
    return impl__PreTranslateMessage_CBasePane__UEAAHPEAUtagMSG___Z(static_cast<CBasePane*>(pThis), pMsg);
}

// STUB. Retail entry RVA 0x44990 (mfc140u; 0x447e0 in mfc140), fully decoded:
//     if (pButton == NULL || !pButton->{+0x14 m_bEnabled}) return;
//     m_rectRedraw = *pButton->{slot 0x28}();              // +0x428 <- the button's rect
//     ::SendMessage(m_hWnd, WM_NCPAINT /*0x85*/, 0, 0);    // IAT 0x1802c5378
//     ::SetRectEmpty(&m_rectRedraw);                       // IAT 0x1802c5368
//     ::UpdateWindow(m_hWnd);                              // IAT 0x1802c5328
// Blocked: no caption button can exist in this build (see DrawCaption above)
// and the OnNcPaint that would consume m_rectRedraw is itself a stub.
// Signature corrected to carry `this`.
// Symbol: ?RedrawButton@CDockablePane@@IEAAXPEBVCMFCCaptionButton@@@Z
extern "C" void MS_ABI impl__RedrawButton_CDockablePane__IEAAXPEBVCMFCCaptionButton___Z(
    CDockablePane* pThis, const void* pButton) {
    (void)pThis;
    (void)pButton;
}

// STUB. Retail entry RVA 0x46210 (mfc140u; 0x46060 in mfc140), fully decoded:
//     for (i = 0; i < m_arrButtons.m_nSize; i++)          // +0x498
//         if (m_arrButtons.m_pData[i] != NULL)            // +0x490
//             delete m_arrButtons.m_pData[i];             // vtable slot 1, flag 1
//     m_arrButtons.RemoveAll();                           // +0x488
// Blocked: OpenMFC's CDockablePane has no m_arrButtons (+0x488 is past the end
// of the 1144-byte object). Harmless as a no-op: nothing here can populate the
// array either (SetCaptionButtons below).
// Signature corrected to carry `this` (OnDestroy above calls it).
// Symbol: ?RemoveCaptionButtons@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__RemoveCaptionButtons_CDockablePane__IEAAXXZ(CDockablePane* pThis) {
    (void)pThis;
}

// CDockablePane::RemoveFromDefaultPaneDivider -- transcribed from retail entry
// RVA 0x40590 (mfc140u; 0x403e0 in mfc140), whose whole body is
//     if (m_hDefaultSlider != NULL)           // `cmpq $0,0x4c0(%rcx) ; je`
//         SetDefaultPaneDivider(NULL);        // `xor %edx,%edx ; call 0x455f0`
// m_hDefaultSlider is read from the file-private DockablePaneExtra table.
// Deviation: NULL check on `this`.
// Signature corrected to carry `this`.
// Symbol: ?RemoveFromDefaultPaneDivider@CDockablePane@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveFromDefaultPaneDivider_CDockablePane__QEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    if (ExtraOf(pThis).hDefaultSlider != nullptr) {
        impl__SetDefaultPaneDivider_CDockablePane__QEAAXPEAUHWND_____Z(pThis, nullptr);
    }
}

// CDockablePane::ReplacePane -- transcribed from retail entry RVA 0x43b20
// (mfc140u; 0x43970 in mfc140). %rdx (pBarToReplaceWith) is kept in %rdi and
// %r9d (bRegisterWithFrame) in %esi; %r8d (dockMethod) is never read:
//     CPaneFrameWnd* pMini = GetParentMiniFrame(FALSE);       // vtable slot 0x460
//     if (pMini != NULL) {
//         pMini->ReplacePane(this, pBarToReplaceWith);         // mini-frame vtable slot 0x310
//         return TRUE;
//     }
//     if (m_hDefaultSlider == NULL) return FALSE;              // +0x4c0
//     CPaneDivider* pSlider = GetDefaultPaneDivider();         // RVA 0x45cd0 (mfc140)
//     if (pSlider == NULL) return FALSE;
//     if (!pSlider->ReplacePane(this, pBarToReplaceWith))      // divider vtable slot 0x508
//         return FALSE;
//     RemovePaneFromDockManager(this, FALSE, FALSE, FALSE,     // RVA 0xb8a0 (mfc140)
//         bRegisterWithFrame ? pBarToReplaceWith : NULL);      // `neg;sbb;and %rdi`
//     return TRUE;
// Slot 0x310 holds ?ReplacePane@CPaneFrameWnd@@UEAAXPEAVCBasePane@@0@Z (RVA
// 0xaf880) in the CPaneFrameWnd vftable (mfc140 RVA 0x2f5ad8) and
// ?ReplacePane@CMultiPaneFrameWnd@@... (RVA 0x94ca0) in the CMultiPaneFrameWnd
// vftable (0x2ee738); slot 0x508 of the CPaneDivider vftable (0x2f4d28) holds
// ?ReplacePane@CPaneDivider@@UEAAHPEAVCDockablePane@@0@Z (RVA 0xaca00).
// Deviations: the two virtuals dispatch statically -- the mini-frame one
// through an IsKindOf(CMultiPaneFrameWnd) test that selects between the two
// exports named above (the CMultiPaneFrameWnd export is still a generated stub
// whose parameter list omits `this`; it reads nothing), the divider one to the
// CPaneDivider export (also still a stub returning 0, so the divider path
// returns FALSE today). RemovePaneFromDockManager is likewise a stub. A NULL
// check on `this` is added.
// Signature corrected: the generated list dropped `this` and two parameters.
// Symbol: ?ReplacePane@CDockablePane@@QEAAHPEAV1@W4AFX_DOCK_METHOD@@H@Z
extern "C" int MS_ABI impl__ReplacePane_CDockablePane__QEAAHPEAV1_W4AFX_DOCK_METHOD__H_Z(
    CDockablePane* pThis, CDockablePane* pBarToReplaceWith, int dockMethod, int bRegisterWithFrame) {
    (void)dockMethod;   // retail never reads it
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check
    void* pMini = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), FALSE);
    if (pMini != nullptr) {
        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(static_cast<CWnd*>(pMini)),
                impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ())) {
            impl__ReplacePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__0_Z(pMini, pThis, pBarToReplaceWith);
        } else {
            impl__ReplacePane_CPaneFrameWnd__UEAAXPEAVCBasePane__0_Z(pMini, pThis, pBarToReplaceWith);
        }
        return TRUE;
    }
    if (ExtraOf(pThis).hDefaultSlider == nullptr) return FALSE;
    void* pSlider = impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(pThis);
    if (pSlider == nullptr) return FALSE;
    if (!impl__ReplacePane_CPaneDivider__UEAAHPEAVCDockablePane__0_Z(pSlider, pThis, pBarToReplaceWith)) {
        return FALSE;
    }
    impl__RemovePaneFromDockManager_CBasePane__QEAAXPEAV1_HHH0_Z(
        static_cast<CBasePane*>(pThis), static_cast<CBasePane*>(pThis), FALSE, FALSE, FALSE,
        bRegisterWithFrame ? static_cast<CBasePane*>(pBarToReplaceWith) : nullptr);
    return TRUE;
}

// CDockablePane::SaveState -- transcribed from retail entry RVA 0x45800
// (mfc140u; the ANSI twin ?SaveState@CDockablePane@@UEAAHPEBDHI@Z at mfc140 RVA
// 0x45650 is the body read). Its ENTIRE body is
//     jmp 0x1800a24e0            ; ?SaveState@CPane@@UEAAHPEBDHI@Z
// -- an unconditional tail jump with no argument shuffling, i.e. CDockablePane
// adds nothing to CPane::SaveState. Reproduced as a straight forward to the
// sibling thunk (phase4/src/featurepack/docking/CPane.cpp:1791), the same way
// LoadState above forwards.
// Signature corrected to carry `this`.
// Symbol: ?SaveState@CDockablePane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CDockablePane__UEAAHPEB_WHI_Z(
    CDockablePane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    return impl__SaveState_CPane__UEAAHPEB_WHI_Z(
        static_cast<CPane*>(pThis), lpszProfileName, nIndex, uiID);
}

// CDockablePane::Serialize -- transcribed from retail entry RVA 0x45810
// (mfc140u; 0x45660 in mfc140). %rcx/%rdx pass straight through to the first
// call; %rdi = this, %rbx = &ar afterwards:
//     CBasePane::Serialize(ar);                                     // RVA 0xc360 (mfc140)
//     if (ar.IsLoading()) {                                         // `testb $1,0x20(%rbx)` == m_nMode & load
//         if (ar.Read(&m_recentDockInfo.<+8>, 16) != 16)            // this+0x2c0; RVA 0x1cf7f0
//             AfxThrowArchiveException(CArchiveException::endOfFile /*3*/, NULL);   // RVA 0x1d15a0
//         if (ar.Read(&m_rectSavedDockedRect, 16) != 16)            // this+0x3d8
//             AfxThrowArchiveException(endOfFile, NULL);
//         m_recentDockInfo.<+0x50> = m_rectSavedDockedRect;         // 16 bytes to this+0x308
//         ar >> m_bRecentFloatingState;                             // this+0x3e8, the inline
//                                    // operator>>: FillBuffer(4) (RVA 0x1cfc70) when fewer than
//                                    // 4 bytes are buffered, then a 4-byte read; it throws
//                                    // readOnly (4) if the archive were not loading
//     } else {
//         if (IsFloating()) {                                       // vtable slot 0x2f0 (== !IsDocked())
//             CPaneFrameWnd* pMini = GetParentMiniFrame(FALSE);     // slot 0x460
//             if (pMini != NULL) ::GetWindowRect(pMini->m_hWnd, &m_recentDockInfo.<+8>);
//         } else {
//             CalcRecentDockedRect();                               // RVA 0xa27e0
//         }
//         ar.Write(&m_recentDockInfo.<+8>, 16);                     // RVA 0x1cfa20
//         ar.Write(&m_recentDockInfo.<+0x50>, 16);
//         ar << m_bRecentFloatingState;                             // inline operator<<: Flush()
//                                    // (RVA 0x1cfb90) when fewer than 4 bytes fit, 4-byte store;
//                                    // throws writeOnly (2) if the archive were loading
//     }
// The two rects inside CRecentDockSiteInfo are the ones CPane::LoadState (see
// docking/CPane.cpp:925) reads from the registry as "RectRecentFloat"
// (m_recentDockInfo + 8, i.e. this+0x2c0) and "RectRecentDocked"
// (m_recentDockInfo + 0x50, this+0x308). OpenMFC's CRecentDockSiteInfo is an
// opaque 288-byte block, but it is a real member at the retail offset +0x2b8
// (include/openmfc/afxmfc.h:453), so the two rects are addressed through its
// storage; m_rectSavedDockedRect and m_bRecentFloatingState are named CPane
// members. The two inline stream operators are reproduced with Read/Write of
// 4 bytes plus the same endOfFile throw the inline path raises through
// FillBuffer; the readOnly/writeOnly checks are unreachable on their branches.
// Deviations: NULL check on `this`; IsFloating dispatches statically to this
// file's IsDocked; CBasePane::Serialize is still a stub whose parameter list
// omits `this` (it reads nothing).
// Signature corrected to carry `this`.
// Symbol: ?Serialize@CDockablePane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CDockablePane__UEAAXAEAVCArchive___Z(CDockablePane* pThis, CArchive& ar) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__Serialize_CBasePane__UEAAXAEAVCArchive___Z(static_cast<CBasePane*>(pThis), ar);
    const size_t kOffRecentFloatRect  = 0x08;   // this+0x2c0, "RectRecentFloat"
    const size_t kOffRecentDockedRect = 0x50;   // this+0x308, "RectRecentDocked"
    static_assert(offsetof(CPane, m_recentDockInfo) == 0x2b8, "CRecentDockSiteInfo moved");
    static_assert(offsetof(CPane, m_rectSavedDockedRect) == 0x3d8, "m_rectSavedDockedRect moved");
    static_assert(offsetof(CPane, m_bRecentFloatingState) == 0x3e8, "m_bRecentFloatingState moved");
    char* pRecentFloat  = pThis->m_recentDockInfo._storage + kOffRecentFloatRect;
    char* pRecentDocked = pThis->m_recentDockInfo._storage + kOffRecentDockedRect;
    if (ar.IsLoading()) {
        if (impl__Read_CArchive__QEAAIPEAXI_Z(&ar, pRecentFloat, sizeof(RECT)) != sizeof(RECT)) {
            impl__AfxThrowArchiveException__YAXHPEB_W_Z(3 /*CArchiveException::endOfFile*/, nullptr);
        }
        if (impl__Read_CArchive__QEAAIPEAXI_Z(&ar, &pThis->m_rectSavedDockedRect, sizeof(RECT)) != sizeof(RECT)) {
            impl__AfxThrowArchiveException__YAXHPEB_W_Z(3 /*CArchiveException::endOfFile*/, nullptr);
        }
        memcpy(pRecentDocked, &pThis->m_rectSavedDockedRect, sizeof(RECT));
        int bRecentFloating = 0;
        if (impl__Read_CArchive__QEAAIPEAXI_Z(&ar, &bRecentFloating, sizeof(bRecentFloating)) != sizeof(bRecentFloating)) {
            impl__AfxThrowArchiveException__YAXHPEB_W_Z(3 /*CArchiveException::endOfFile*/, nullptr);
        }
        pThis->m_bRecentFloatingState = bRecentFloating;
    } else {
        if (!impl__IsDocked_CDockablePane__UEBAHXZ(pThis)) {   // IsFloating()
            void* pMini = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
                static_cast<const CBasePane*>(pThis), FALSE);
            if (pMini != nullptr) {
                ::GetWindowRect(static_cast<CWnd*>(pMini)->m_hWnd, reinterpret_cast<RECT*>(pRecentFloat));
            }
        } else {
            impl__CalcRecentDockedRect_CPane__QEAAXXZ(static_cast<CPane*>(pThis));
        }
        impl__Write_CArchive__QEAAXPEBXI_Z(&ar, pRecentFloat, sizeof(RECT));
        impl__Write_CArchive__QEAAXPEBXI_Z(&ar, pRecentDocked, sizeof(RECT));
        const int bRecentFloating = pThis->m_bRecentFloatingState;
        impl__Write_CArchive__QEAAXPEBXI_Z(&ar, &bRecentFloating, sizeof(bRecentFloating));
    }
}

// CDockablePane::SetAutoHideParents -- transcribed from retail entry RVA
// 0x432a0 (mfc140u; 0x430f0 in mfc140), whose whole body is
//     mov %rdx,0x480(%rcx)      ; m_pAutoHideBar    = pAutoHideBar
//     mov %r8,0x478(%rcx)       ; m_pAutoHideButton = pAutoHideButton
//     ret
// Neither field has storage in the 1144-byte object (see the offset note near
// the top). m_pAutoHideBar goes to the shared PaneCoreState::autoHideBar slot
// that this file's C++ SetAutoHideMode already fills and that AutoHideBarOf()
// reads back (IsVisible, CalcCorrectOffset); m_pAutoHideButton goes to the
// file-private DockablePaneExtra table. Storage deviation only.
// Deviation: NULL check on `this`.
// Signature corrected to carry `this`.
// Symbol: ?SetAutoHideParents@CDockablePane@@QEAAXPEAVCMFCAutoHideBar@@PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__SetAutoHideParents_CDockablePane__QEAAXPEAVCMFCAutoHideBar__PEAVCMFCAutoHideButton___Z(
    CDockablePane* pThis, void* pAutoHideBar, void* pAutoHideButton) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        g_paneCoreState_Mfccore[pThis].autoHideBar = pAutoHideBar;
    }
    SetAutoHideButtonOf(pThis, pAutoHideButton);
}

// STUB. Retail entry RVA 0x460e0 (mfc140u; 0x45f30 in mfc140): it calls
// RemoveCaptionButtons (RVA 0x46060), then `operator new`s (RVA 0x2840)
// 0x38-byte CMFCCaptionButton objects with the constructor inlined (vftable
// mfc140 RVA 0x2ddd78 at +0, hit id 0x13 / 9 / ... at +0x20, +0x14 = 1,
// +0x24 = -1) and appends each with CPtrArray::SetAtGrow(m_arrButtons.m_nSize,
// p) (RVA 0x1d28a0 on this+0x488). Blocked: OpenMFC's CMFCCaptionButton is a
// 16-byte-padded CObject with no retail layout or vtable and this class has
// no m_arrButtons.
// Signature corrected to carry `this`.
// Symbol: ?SetCaptionButtons@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__SetCaptionButtons_CDockablePane__MEAAXXZ(CDockablePane* pThis) {
    (void)pThis;
}

// CDockablePane::SetDefaultPaneDivider -- transcribed from retail entry RVA
// 0x457a0 (mfc140u; 0x455f0 in mfc140), fully decoded:
//     if (m_hDefaultSlider != hDivider) {                  // +0x4c0
//         CPaneDivider* pOld = GetDefaultPaneDivider();    // RVA 0x45cd0 (mfc140)
//         if (pOld != NULL) pOld->RemovePane(this);        // vtable slot 0x510
//     }
//     m_hDefaultSlider = hDivider;
// Slot 0x510 of the CPaneDivider vftable (mfc140 RVA 0x2f4d28) holds
// ?RemovePane@CPaneDivider@@UEAAXPEAVCDockablePane@@@Z (RVA 0xac920), so the
// virtual call dispatches statically to that export
// (docking/CPaneDivider.cpp:35). m_hDefaultSlider lives in the file-private
// DockablePaneExtra table (no storage in the object -- see the note near the
// top). Deviation: NULL check on `this`.
// Symbol: ?SetDefaultPaneDivider@CDockablePane@@QEAAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__SetDefaultPaneDivider_CDockablePane__QEAAXPEAUHWND_____Z(CDockablePane* pThis, HWND hDivider) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    if (ExtraOf(pThis).hDefaultSlider != hDivider) {
        void* pOld = impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(pThis);
        if (pOld != nullptr) {
            impl__RemovePane_CPaneDivider__UEAAXPEAVCDockablePane___Z(pOld, pThis);
        }
    }
    SetDefaultSliderOf(pThis, hDivider);
}

// CDockablePane::SetResizeMode -- transcribed from retail entry RVA 0x432b0
// (mfc140u; 0x43100 in mfc140), whose whole body is
//     mov %edx,0x46c(%rcx) ; ret        ; m_bIsResizing = bResize
// +0x46c is inside the reachable padding window (see the offset note near the
// top), so the flag is stored at its retail offset. (CPaneContainerManager's
// SetResizeMode sibling writes the same +0x46c on every pane it walks.)
// Deviation: NULL check on `this`.
// Signature corrected to carry `this`.
// Symbol: ?SetResizeMode@CDockablePane@@QEAAXH@Z
extern "C" void MS_ABI impl__SetResizeMode_CDockablePane__QEAAXH_Z(CDockablePane* pThis, int bResize) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    DockablePaneField<int>(pThis, kOffIsResizing) = bResize;
}

// STUB. Retail entry RVA 0x42ee0 (mfc140u; 0x42d30 in mfc140), ~950 bytes,
// not transcribed. Its control flow was read (all RVAs below mfc140):
//     if (!IsAutoHideMode()) return;                              // slot 0x3c8
//     if (m_nSlideTimer) ::KillTimer(m_hWnd, m_nSlideTimer);      // +0x448
//     if (m_nAutoHideConditionTimerID) { ::KillTimer(..); m_nAutoHideConditionTimerID = 0; }  // +0x440
//     CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);
//     if (!bUseTimer || CDockablePane::m_bDisableAnimation /*RVA 0x3b6f4c*/ ||
//         <afxGlobalData+0x25c> != 0)   // read after the lazy AFX_GLOBAL_DATA::Initialize
//                                       // (RVA 0x6a5c0) gated by the flag at 0x3ba380
//         m_nSlideDelta = IsHorizontal() ? rectWnd.Height() : rectWnd.Width();   // +0x454; slot 0x2d8
//                       // (slot 0x2d8, RVA 0x88e0, is `GetCurrentAlignment() & 0xa000`, i.e. TOP|BOTTOM)
//     if (!bUseTimer) {                                            // immediate arm
//         m_rectRestored = rectWnd;                                // +0x458
//         OnSlide(FALSE);                                          // slot 0x748
//         ShowWindow(SW_HIDE); ::ShowWindow(m_hDefaultSlider, SW_HIDE);   // +0x4c0
//         return;
//     }
//     CDockingManager* pMgr = afxGlobalUtils.GetDockingManager(GetDockSiteFrameWnd());   // RVA 0x6ccc0
//     if (bSlideOut) {
//         pMgr->HideAutoHidePanes(this, FALSE);                    // RVA 0x487c0
//         pMgr->AlignAutoHidePane(GetDefaultPaneDivider(), FALSE); // RVA 0x48840
//         ShowWindow(SW_SHOW); ::ShowWindow(m_hDefaultSlider, SW_SHOW);
//     }
//     ::BringWindowToTop(m_hWnd); ::BringWindowToTop(m_hDefaultSlider);   // IAT 0x1802c5290
//     if (m_ahSlideMode == 1) pMgr->BringBarsToTop(0, TRUE);      // +0x474; RVA 0x495b0
//     m_nSlideTimer = ::SetTimer(m_hWnd, 0xEC01 + (bSlideOut ? 1 : 0),
//                                CDockablePane::m_nSlideDefaultTimeOut /*0x3aaac0*/, NULL);
//     if (!m_bDisableAnimation && <afxGlobalData+0x25c> == 0) {   // animated: split the travel
//         if (m_ahSlideMode == 1)      m_nSlideDelta = max(1, <extent of rectWnd in the dock-site's
//                                          client space> / CDockablePane::m_nSlideSteps /*0x3aaabc*/);
//         else if (m_ahSlideMode == 2) { if (!bSlideOut && !m_bIsSliding) m_rectRestored = rectWnd
//                                          (dock-site client space);
//                                        m_nSlideDelta = max(1, <extent of m_rectRestored> / m_nSlideSteps); }
//     }
//     m_nSlideStep = 0; m_bIsSliding = TRUE;                       // +0x450, +0x468
// (Extent == height when GetCurrentAlignment() & 0xa000 (TOP|BOTTOM), else width.)
// So the afxGlobalData flag at +0x25c does not choose between a timer and an
// immediate show/hide -- only bUseTimer does that -- it is a no-animation
// flag: when it is NONZERO (or m_bDisableAnimation is set) the timer-driven
// slide moves in one step (m_nSlideDelta = the full extent), when ZERO it
// moves in m_nSlideSteps steps.
// Blocked: OpenMFC's afxGlobalData is an all-zero blob whose exported
// Initialize (core/runtime/AFX_GLOBAL_DATA.cpp:97) is an empty stub (see
// EnableGripper above), so the +0x25c flag always reads 0 and every timer
// slide would be animated even in a session where retail suppresses it;
// worse, OpenMFC's ?m_nSlideSteps@CDockablePane@@2HA (docking/StaticData.cpp:36)
// is initialised to 0 where retail's .data holds 12, so the `idiv` that splits
// the travel would fault. m_ahSlideMode (+0x474) has no storage, and the three
// CDockingManager bodies it drives (docking/CDockingManager.cpp:401, :89,
// :130) are side-table approximations, not retail transcriptions.
// Signature corrected to carry `this` (CheckAutoHideCondition above calls it).
// Symbol: ?Slide@CDockablePane@@UEAAXHH@Z
extern "C" void MS_ABI impl__Slide_CDockablePane__UEAAXHH_Z(CDockablePane* pThis, int bSlideOut, int bUseTimer) {
    (void)pThis;
    (void)bSlideOut;
    (void)bUseTimer;
}

// STUB. Retail entry RVA 0x45030. It walks m_arrButtons (this+0x488, m_pData
// at +0x490 / m_nSize at +0x498) clearing each caption button's pressed/hot
// state and repainting. OpenMFC's CDockablePane has no m_arrButtons -- offsets
// from +0x478 up are past the end of the 1144-byte object -- so there is
// nothing to walk. Signature corrected to carry `this` (it is called above).
// Symbol: ?StopCaptionButtonsTracking@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CDockablePane__MEAAXXZ(CDockablePane* pThis) {
    (void)pThis;
}

// CDockablePane::StoreRecentDockSiteInfo -- transcribed from retail entry RVA
// 0x41410 (mfc140u; 0x41260 in mfc140):
//     CPaneFrameWnd* pMini = GetParentMiniFrame(FALSE);        // vtable slot 0x460, -> %rsi
//     CDockablePane* pBar = this;                              // %rbx
//     if (IsTabbed()) {                                        // vtable slot 0x2e8
//         CWnd* pTabWnd = CWnd::FromHandle(::GetParent(m_hWnd));          // RVA 0x289180
//         if (pTabWnd != NULL && pTabWnd->IsKindOf(<RTC 0x2dba98 "CMFCBaseTabCtrl">)) {
//             pBar = CWnd::FromHandle(::GetParent(pTabWnd->m_hWnd));
//             if (pBar != NULL && !pBar->IsKindOf(<RTC 0x3aa178 "CDockablePane">))
//                 pBar = NULL;
//         }
//     }
//     CPaneDivider* pSlider = pBar->GetDefaultPaneDivider();  // RVA 0x45cd0, pBar unchecked
//     if (pMini != NULL)
//         pMini->{slot 0x3d0}(pBar);
//     else if (pSlider != NULL && pSlider->{+0x208} != NULL)
//         pSlider->{+0x208}->{slot 0x120}(pBar);
// Slot 0x3d0 is `ret` (ICF-folded onto ?UpdateModifiedFlag@CRichEditDoc@@, RVA
// 0x2820) in the CPaneFrameWnd vftable (mfc140 RVA 0x2f5ad8) and
// ?StoreRecentDockSiteInfo@CMultiPaneFrameWnd@@UEAAXPEAVCPane@@@Z (RVA 0x948d0)
// in the CMultiPaneFrameWnd vftable (0x2ee738). +0x208 of the 0x218-byte
// CPaneDivider is its CPaneContainerManager pointer, and slot 0x120 of that
// class's vftable (0x2f41a8) is
// ?StoreRecentDockSiteInfo@CPaneContainerManager@@UEAAXPEAVCDockablePane@@@Z
// (RVA 0xa8bc0).
// Deviations, all forced: (1) the mini-frame virtual is dispatched through an
// IsKindOf(CMultiPaneFrameWnd) test -- the CPaneFrameWnd body is empty, so
// only the CMultiPaneFrameWnd export (docking/CMultiPaneFrameWnd.cpp:180, still
// a stub whose parameter list omits `this`) is called; (2) the divider arm is
// dropped: OpenMFC's CPaneDivider is a CObject with a side table and no
// container-manager member, and GetDefaultPaneDivider() above cannot return a
// non-NULL divider in this build anyway, so the arm is unreachable here;
// (3) NULL checks on `this` and on pBar before GetDefaultPaneDivider (retail
// would fault on a tabbed pane whose tab control's parent is not a
// CDockablePane).
// Signature corrected to carry `this` (it is called from OnBeforeChangeParent above).
// Symbol: ?StoreRecentDockSiteInfo@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    void* pMini = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), FALSE);
    CWnd* pBar = static_cast<CWnd*>(pThis);
    if (impl__IsTabbed_CPane__UEBAHXZ(static_cast<const CPane*>(pThis))) {
        CWnd* pTabWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
        if (pTabWnd != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pTabWnd),
                impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ())) {
            pBar = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pTabWnd->m_hWnd));
            if (pBar != nullptr &&
                !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                    static_cast<const CObject*>(pBar),
                    impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ())) {
                pBar = nullptr;
            }
        }
    }
    if (pBar == nullptr) return;   // deviation: retail dereferences it
    void* pSlider = impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(
        static_cast<const CDockablePane*>(pBar));
    if (pMini != nullptr) {
        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(static_cast<CWnd*>(pMini)),
                impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ())) {
            impl__StoreRecentDockSiteInfo_CMultiPaneFrameWnd__UEAAXPEAVCPane___Z(pMini, pBar);
        }
        // CPaneFrameWnd's own slot 0x3d0 body is an empty `ret`.
        return;
    }
    (void)pSlider;   // divider arm dropped -- see the note above
}

// Symbol: ?StoreRecentTabRelatedInfo@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__StoreRecentTabRelatedInfo_CDockablePane__MEAAXXZ() {}

// Symbol: ?ToggleAutoHide@CDockablePane@@UEAAXXZ
extern "C" void MS_ABI impl__ToggleAutoHide_CDockablePane__UEAAXXZ() {}

// STUB. Retail entry RVA 0x44d20; not transcribed. Signature corrected to
// carry `this` (it is called from OnBeforeChangeParent above).
// Symbol: ?UndockPane@CDockablePane@@UEAAXH@Z
extern "C" void MS_ABI impl__UndockPane_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bDelay) {
    (void)pThis;
    (void)bDelay;
}

// Symbol: ?UnSetAutoHideMode@CDockablePane@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__UnSetAutoHideMode_CDockablePane__UEAAXPEAV1__Z(void* /*class*/* p0) {}

// Symbol: ?UpdateTooltips@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltips_CDockablePane__IEAAXXZ() {}
