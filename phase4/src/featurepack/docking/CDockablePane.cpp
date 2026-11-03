// CDockablePane — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

#include <cstddef>   // offsetof (static_asserts below)
#include <cstring>   // memcpy
#include <cwchar>    // wcscmp
#include <mutex>
#include <new>
#include <unordered_map>

static_assert(sizeof(CDockablePane) == 0x478,
              "preserve the existing compact CDockablePane allocation");

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
// OpenMFC now names and asserts the retail prefix through +0x474 in the
// existing compact 0x478-byte CDockablePane allocation. alignas(8) on the first
// tail member prevents MinGW from reusing CPane's four-byte base tail padding,
// so both MinGW and clang-cl place the prefix at the retail +0x3f8. Retail
// fields beginning at +0x478 exceed this allocation and remain pThis-keyed in
// PaneCoreState / DockablePaneExtra; they are semantic side state, not raw
// offsets into a larger object. The outstanding stubs are described alongside
// their symbols below.
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
extern "C" int MS_ABI impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(
    CToolTipCtrl** ppToolTip, CWnd* pParent, unsigned int nType);
extern "C" void MS_ABI impl__DeleteToolTip_CTooltipManager__SAXAEAPEAVCToolTipCtrl___Z(
    CToolTipCtrl** ppToolTip);
extern "C" int MS_ABI impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(
    CToolTipCtrl* pThis, CWnd* pWnd, const wchar_t* lpszText, const RECT* pRect, unsigned __int64 nIDTool);
extern "C" void MS_ABI impl__SetToolRect_CToolTipCtrl__QEAAXPEAVCWnd___KPEBUtagRECT___Z(
    CToolTipCtrl* pThis, CWnd* pWnd, unsigned __int64 nIDTool, const RECT* pRect);
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
//   ReplacePane_CPaneDivider    CPaneDivider.cpp (side-state body)
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
extern "C" int impl__m_bDisableAnimation_CDockablePane__2HA;
extern "C" int impl__m_nSlideDefaultTimeOut_CDockablePane__1HA;
extern "C" int impl__m_nSlideSteps_CDockablePane__2HA;
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
    CString* pThis, HINSTANCE hInstance, unsigned int nID);
extern "C" void MS_ABI impl__HideAutoHidePanes_CDockingManager__QEAAXPEAVCDockablePane__H_Z(
    CDockingManager* pThis, CDockablePane* pPane, int bHide);
extern "C" void MS_ABI impl__AlignAutoHidePane_CDockingManager__QEAAXPEAVCPaneDivider__H_Z(
    CDockingManager* pThis, void* pDivider, int nAlignment);
extern "C" void MS_ABI impl__BringBarsToTop_CDockingManager__QEAAXKH_Z(
    CDockingManager* pThis, unsigned long dwFlags, int bActivate);
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
//   StoreRecentDockSiteInfo_CMultiPaneFrameWnd  docking/CMultiPaneFrameWnd.cpp (side-state body)
//                                      (stub whose parameter list omits `this`)
//   GetThisClass_CPaneDivider is a real descriptor getter.
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneDivider__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__RemovePane_CPaneDivider__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane);
extern "C" void MS_ABI impl__GetRootContainerRect_CPaneDivider__QEAA_AVCRect__XZ(CRect* pRet, void* pThis);
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CMultiPaneFrameWnd__UEAAXPEAVCPane___Z(void* pThis, void* pBar);
extern "C" void MS_ABI impl__StoreRecentTabRelatedInfo_CMultiPaneFrameWnd__UEAAXPEAVCDockablePane__0_Z(
    void* pThis, CDockablePane* pDockingBar, CDockablePane* pTabbedBar);
extern "C" void MS_ABI impl__StoreRecentTabRelatedInfo_CPaneDivider__QEAAXPEAVCDockablePane__0_Z(
    void* pThis, CDockablePane* pDockingBar, CDockablePane* pTabbedBar);
//   ReplacePane_CMultiPaneFrameWnd     docking/CMultiPaneFrameWnd.cpp (side-state body)
//   GetParentFrame_CWnd                core/window/Thunks.cpp:1353 (real body)
//   GetThisClass_CMFCPropertyGridToolTipCtrl  propertygrid/RuntimeClasses.cpp:75
//   InsertPane_CPaneDivider            docking/CPaneDivider.cpp (side-state body)
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
extern "C" void MS_ABI impl__SetCaptionButtons_CDockablePane__MEAAXXZ(CDockablePane* pThis);
extern "C" void MS_ABI impl__UpdateTooltips_CDockablePane__IEAAXXZ(CDockablePane* pThis);

namespace {
// --- Retail prefix fields in the compact CDockablePane allocation ------------
// afxmfc.h now names the retail members from +0x3f8 through +0x477. The first
// field is alignas(8), which ensures MinGW does not reuse CPane's tail padding
// and clang-cl/MSVC uses the same retail offsets. The 0x478-byte local object
// still does not contain fields from +0x478 onward; those remain in side state.
static_assert(sizeof(CPane) == 0x3f8, "CPane layout changed; recheck the offsets below");
static_assert(sizeof(CDockablePane) == 0x478, "CDockablePane compact allocation changed");

const size_t kOffPrepareToFloat = 0x408;   // bool   m_bPrepareToFloat
const size_t kOffLastPercent    = 0x410;   // int    m_nLastPercent
const size_t kOffEnableAutoHideAll = 0x420; // BOOL  m_bEnableAutoHideAll
const size_t kOffSlideDelta     = 0x454;   // int    m_nSlideDelta
const size_t kOffRectRestored   = 0x458;   // CRect  m_rectRestored
const size_t kOffCyGripper      = 0x40c;   // int    m_cyGripper
const size_t kOffHasGripper     = 0x418;   // BOOL   m_bHasGripper
const size_t kOffActive         = 0x41c;   // BOOL   m_bActive
const size_t kOffPinState       = 0x438;   // BOOL   m_bPinState
const size_t kOffAutoHideConditionTimerID = 0x440;  // UINT_PTR m_nAutoHideConditionTimerID
const size_t kOffAHSlideMode    = 0x474;   // UINT   m_ahSlideMode
const size_t kOffSlideTimer     = 0x448;   // UINT_PTR m_nSlideTimer
const size_t kOffSlideStep      = 0x450;   // int    m_nSlideStep
const size_t kOffIsSliding      = 0x468;   // BOOL   m_bIsSliding
const size_t kOffIsResizing     = 0x46c;   // BOOL   m_bIsResizing
const size_t kOffIsHiding       = 0x470;   // BOOL   m_bIsHiding
const size_t kOffFirstSideTable = 0x478;   // first retail member beyond local allocation

static_assert(kOffPrepareToFloat >= sizeof(CPane), "field would alias the CPane sub-object");
static_assert(kOffLastPercent >= sizeof(CPane), "field would alias the CPane sub-object");
static_assert(kOffEnableAutoHideAll >= sizeof(CPane), "field would alias the CPane sub-object");
static_assert(kOffRectRestored + sizeof(RECT) <= kOffFirstSideTable, "field runs past local storage");
static_assert(kOffCyGripper >= sizeof(CPane), "field would alias the CPane sub-object");
static_assert(kOffIsHiding + sizeof(BOOL) <= kOffFirstSideTable, "field runs past local storage");
static_assert(kOffIsResizing + sizeof(BOOL) <= kOffFirstSideTable, "field runs past local storage");
static_assert(kOffFirstSideTable == sizeof(CDockablePane), "side-table boundary changed");

template <typename T>
inline T& DockablePaneField(CDockablePane* pThis, size_t off) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(pThis) + off);
}

// AFX_GLOBAL_DATA::GetGlobalData checks m_bInitialized, invokes Initialize,
// then sets the flag. Retail's Slide and EnableGripper use an equivalent lazy
// guard; serialize this TU's calls as well.
inline void EnsureDockingGlobalDataInitialized() {
    static std::mutex initializationMutex;
    std::lock_guard<std::mutex> lock(initializationMutex);
    int& initialized = *reinterpret_cast<int*>(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
    if (!initialized) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        initialized = TRUE;
    }
}

inline int DockingGlobalDataInt(size_t off) {
    int value = 0;
    std::memcpy(&value, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof(value));
    return value;
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
inline CString& DockablePaneTipTextCache() {
    static CString text;
    return text;
}
inline std::mutex& DockablePaneTipTextMutex() {
    static std::mutex mutex;
    return mutex;
}
inline CToolTipCtrl* DockablePaneToolTipOf(const CDockablePane* pThis) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(pThis);
    return it != g_paneCoreState_Mfccore.end() ? it->second.toolTip : nullptr;
}
inline void SetDockablePaneToolTip(const CDockablePane* pThis, CToolTipCtrl* pToolTip) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[pThis].toolTip = pToolTip;
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
    static_assert(offsetof(CDockablePane, _dockablepane_padding) == 0x3f8, "CDockablePane storage view offset");
    static_assert(offsetof(CDockablePane, m_rectCloseButton) == 0x3f8, "m_rectCloseButton offset");
    static_assert(offsetof(CDockablePane, m_bPrepareToFloat) == 0x408, "m_bPrepareToFloat offset");
    static_assert(offsetof(CDockablePane, m_bReadyToFloat) == 0x409, "m_bReadyToFloat offset");
    static_assert(offsetof(CDockablePane, _dockablepane_pad40a) == 0x40a, "CDockablePane first pad offset");
    static_assert(offsetof(CDockablePane, m_cyGripper) == 0x40c, "m_cyGripper offset");
    static_assert(offsetof(CDockablePane, m_nLastPercent) == 0x410, "m_nLastPercent offset");
    static_assert(offsetof(CDockablePane, m_nBorderSize) == 0x414, "m_nBorderSize offset");
    static_assert(offsetof(CDockablePane, m_bHasGripper) == 0x418, "m_bHasGripper offset");
    static_assert(offsetof(CDockablePane, m_bActive) == 0x41c, "m_bActive offset");
    static_assert(offsetof(CDockablePane, m_bEnableAutoHideAll) == 0x420, "m_bEnableAutoHideAll offset");
    static_assert(offsetof(CDockablePane, m_dwSCBStyle) == 0x424, "m_dwSCBStyle offset");
    static_assert(offsetof(CDockablePane, m_rectRedraw) == 0x428, "m_rectRedraw offset");
    static_assert(offsetof(CDockablePane, m_bPinState) == 0x438, "CDockablePane m_bPinState offset");
    static_assert(offsetof(CDockablePane, _dockablepane_pad43c) == 0x43c, "CDockablePane second pad offset");
    static_assert(offsetof(CDockablePane, m_nAutoHideConditionTimerID) == 0x440, "m_nAutoHideConditionTimerID offset");
    static_assert(offsetof(CDockablePane, m_nSlideTimer) == 0x448, "m_nSlideTimer offset");
    static_assert(offsetof(CDockablePane, m_nSlideStep) == 0x450, "m_nSlideStep offset");
    static_assert(offsetof(CDockablePane, m_nSlideDelta) == 0x454, "m_nSlideDelta offset");
    static_assert(offsetof(CDockablePane, m_rectRestored) == 0x458, "m_rectRestored offset");
    static_assert(offsetof(CDockablePane, m_bIsSliding) == 0x468, "m_bIsSliding offset");
    static_assert(offsetof(CDockablePane, m_bIsResizing) == 0x46c, "m_bIsResizing offset");
    static_assert(offsetof(CDockablePane, m_bIsHiding) == 0x470, "m_bIsHiding offset");
    static_assert(offsetof(CDockablePane, m_ahSlideMode) == 0x474, "m_ahSlideMode offset");
    std::memset(reinterpret_cast<char*>(this) + 0x3f8, 0, 0x80);
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
// the largest body in this class; not transcribed. It uses m_nSlideStep
// (+0x450), the modeled m_pTabbedControlBarRTC side-state (+0x4b0), and the
// modeled default-slider HWND, then creates/attaches a CTabbedPane and invokes
// CPaneDivider ReplacePane/AddPane operations. The runtime-class field is now
// available, but CreateTabbedPane below still cannot reproduce CTabbedPane's
// retail Create dispatch/layout, and the divider attach/replace virtual slots
// are not modeled; calling through the path would therefore fabricate docking
// effects.
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
// Both fields are inside the modeled CDockablePane layout and are read at
// their retail offsets. The sibling CBasePane::GetCurrentAlignment body is
// implemented from its retail three-instruction body, so the alignment cases
// below are reachable when m_dwStyle carries CBRS_ALIGN_* bits.
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
// Deviations: NULL checks on `this`, the button and the bar (retail
// dereferences them unchecked; note OpenMFC's CMFCAutoHideButton::CreateObject
// still returns NULL, so today the button is NULL and this returns FALSE at
// that check); ::GetWindowRect on a NULL m_hDefaultSlider is skipped, leaving
// rectSlider empty -- the API fails and leaves the zeroed rect untouched in
// retail too, so the union is the same. The tooltip visibility test uses the
// PaneCoreState pointer rather than retail's in-object +0x4b8 member.
// Signature corrected to carry `this` (OnTimer below calls it).
// Symbol: ?CheckAutoHideCondition@CDockablePane@@MEAAHXZ
extern "C" int MS_ABI impl__CheckAutoHideCondition_CDockablePane__MEAAHXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check
    if (DockablePaneField<int>(pThis, kOffActive)) return FALSE;
    if (DockablePaneField<int>(pThis, kOffIsResizing)) return FALSE;
    if (!pThis->IsAutoHideMode()) return FALSE;
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA != nullptr) return FALSE;
    CToolTipCtrl* pToolTip = DockablePaneToolTipOf(pThis);
    if (pToolTip != nullptr && pToolTip->m_hWnd != nullptr &&
        ::IsWindowVisible(pToolTip->m_hWnd)) return FALSE;

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

// CDockablePane::CheckStopSlideCondition -- transcribed from function entry
// RVA 0x42720 (mfc140u). The body first returns TRUE when IsAutoHideMode() is
// FALSE. Otherwise it converts the pane and its auto-hide bar's parent dock-site
// rectangles to GetDockSiteFrameWnd() client coordinates, obtains WS_EX_LAYOUTRTL
// from that frame, and branches on GetCurrentAlignment() and
// m_ahSlideMode (+0x474). For slide mode 1 the four alignment arms compare the
// pane edges against the corresponding dock-site edge (left/right arms also
// branch on RTL); other slide modes test pane width/height against
// m_rectRestored (+0x458), except the top-alignment arm which returns FALSE.
// GetWindowRect is the call through IAT slot 0x1802c6c08, resolved by iatu.py
// as USER32!GetWindowRect. The dock-site pointer is the auto-hide bar's
// CBasePane::m_pParentDockBar (+0x128).
// Deviation: retail's GetDockSiteFrameWnd/GetCurrentAlignment virtual calls
// and its CWnd::ScreenToClient/GetExStyle calls are routed through stock thunks
// here, so pane/frame subclass overrides do not run. The bar pointer and frame
// are dereferenced without NULL checks, as retail does on this path. The bar
// member itself is held in PaneCoreState rather than at retail +0x480.
// Signature corrected to carry `this` (OnTimer below calls it).
// Symbol: ?CheckStopSlideCondition@CDockablePane@@MEAAHH@Z
extern "C" int MS_ABI impl__CheckStopSlideCondition_CDockablePane__MEAAHH_Z(CDockablePane* pThis, int bDirection) {
    // Retail's getter reads +0x438; this build's getter reads the corresponding
    // PaneCoreState value because SetAutoHideMode stores it there.
    if (!pThis->IsAutoHideMode()) return TRUE;

    RECT rectPane = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectPane);
    CWnd* pFrame = impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(
        static_cast<const CBasePane*>(pThis));
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pFrame, &rectPane);
    const BOOL bRTL = (impl__GetExStyle_CWnd__QEBAKXZ(pFrame) & WS_EX_LAYOUTRTL) != 0;

    CBasePane* pAutoHideBar = static_cast<CBasePane*>(AutoHideBarOf(pThis));
    CWnd* pDockSite = reinterpret_cast<CWnd*>(pAutoHideBar->m_pParentDockBar);
    RECT rectDockSite = {0, 0, 0, 0};
    ::GetWindowRect(pDockSite->m_hWnd, &rectDockSite);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pFrame, &rectDockSite);

    const unsigned long alignment =
        impl__GetCurrentAlignment_CBasePane__UEBAKXZ(static_cast<const CBasePane*>(pThis));
    if (DockablePaneField<UINT>(pThis, kOffAHSlideMode) == 1) {
        switch (alignment) {
        case kAlignLeft:
            if (bRTL) return bDirection ? rectPane.right <= rectDockSite.left
                                        : rectPane.right >= rectDockSite.left;
            return bDirection ? rectPane.left >= rectDockSite.right
                              : rectPane.right <= rectDockSite.right;
        case kAlignTop:
            return bDirection ? rectPane.top >= rectDockSite.bottom
                              : rectPane.bottom <= rectDockSite.bottom;
        case kAlignRight:
            if (bRTL) return bDirection ? rectPane.left >= rectDockSite.right
                                        : rectPane.right <= rectDockSite.right;
            return bDirection ? rectPane.right <= rectDockSite.left
                              : rectPane.left >= rectDockSite.left;
        case kAlignBottom:
            return bDirection ? rectPane.bottom <= rectDockSite.top
                              : rectPane.top >= rectDockSite.top;
        default:
            return FALSE;
        }
    }

    const int paneWidth = rectPane.right - rectPane.left;
    const int paneHeight = rectPane.bottom - rectPane.top;
    const RECT& rectRestored = DockablePaneField<RECT>(pThis, kOffRectRestored);
    switch (alignment) {
    case kAlignLeft:
    case kAlignRight:
        return bDirection ? paneWidth >= rectRestored.right - rectRestored.left
                          : paneWidth <= 0;
    case kAlignTop:
        return FALSE;
    case kAlignBottom:
        return bDirection ? paneHeight >= rectRestored.bottom - rectRestored.top
                          : paneHeight <= 0;
    default:
        return FALSE;
    }
}

// STUB. Retail mfc140.dll ANSI-twin entry RVA 0x46590 (mfc140u RVA 0x46740):
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
// PARTIAL: the retail m_ahSlideMode at +0x474 is now named and asserted in
// afxmfc.h, but this existing body has not yet copied it. m_rectRestored and the
// other copied fields are represented in the named prefix. m_rectRestored is
// not dead weight -- CalcRectForSliding and OffsetRectForSliding read it back.
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
    // m_ahSlideMode (+0x474) remains omitted by this partial body.
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
// Blocked: OpenMFC's CPaneDivider declaration (include/openmfc/afxmfc.h:1748)
// is a compact CObject, not the retail CBasePane-derived 0x218-byte window
// object. Its CreateEx thunk (docking/CPaneDivider.cpp:824) returns FALSE for
// such compact factory objects before touching CWnd storage; the +0x1c4
// default-divider write in this retail body would already overrun that object.
// The class also has no local vtable slots 0x560/0x3d0 or m_dwStyle at +0x104.
// The default arm additionally depends on ?m_pSliderRTC@CPaneDivider@@, which
// is NULL in core/runtime/StaticData.cpp:62. Neither divider creation path nor
// the alignment store can be reproduced safely.
// Symbol: ?CreateDefaultPaneDivider@CDockablePane@@SAPEAVCPaneDivider@@KPEAVCWnd@@PEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__CreateDefaultPaneDivider_CDockablePane__SAPEAVCPaneDivider__KPEAVCWnd__PEAUCRuntimeClass___Z(
    unsigned long dwAlignment, CWnd* pParent, CRuntimeClass* pSliderRTC) {
    (void)dwAlignment;
    (void)pParent;
    (void)pSliderRTC;
    return nullptr;
}

// STUB. Transcribed from function entry RVA 0x43be0 (mfc140u): retail gets
// this pane's window rectangle, converts it to the parent's client coordinates,
// creates a CTabbedPane through m_pTabbedControlBarRTC, sets the new pane's
// field at +0x4e0, calls Create with CBRS_FLOAT_MULTI and the retail style
// arguments, then copies 16-byte regions from this+0x370, +0x308 and +0x2c0.
// The +0x4b0 runtime-class pointer is now represented in PaneCoreState, but
// the retail-shaped CTabbedPane Create dispatch and its +0x4e0/+0x2c0/+0x308/
// +0x370 layout are not modeled, so the object cannot be safely constructed.
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
// Left a stub. The CObList walk itself is not the blocker: AddPanesToList is
// implemented over the docking manager's side-state graph, and this unit can
// traverse the list. The terminal CBasePane::InsertPane export is still a stub;
// CPaneDivider::AddPaneContainer has a side-state body, but its downstream
// CPaneContainerManager::AddPaneContainerManager export is still a placeholder.
// In addition, GetDefaultPaneDivider() cannot return this build's compact
// CPaneDivider object, so the final divider operation is unavailable. A partial
// list walk would still perform per-pane divider/alignment side effects without
// completing the container dock; m_hDefaultSlider itself is in DockablePaneExtra.
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
// (UndockPane is still a stub; the CPaneDivider InsertPane uses its side-state
// manager and may return 0 when that manager is unavailable; GetDefaultPaneDivider
// returns NULL for the compact divider objects in this build, so its first arm
// normally runs); NULL checks on `this` and
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
// touches m_bActive (+0x41c) and the button array (+0x490/+0x498), then paints
// through the visual manager. PaneCoreState now has semantic button records,
// but there is no raw retail m_arrButtons/CMFCCaptionButton object layout in
// this compact allocation; the public CMFCCaptionButton declaration is a
// padded CObject. The visual-manager drawing path is also not modeled.
// Signature corrected: the 16-byte CRect is passed by value in retail, i.e. by
// address under the MS x64 ABI (`mov 0x8(%r8),%rbx` reads its right/bottom).
// Symbol: ?DrawCaption@CDockablePane@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__DrawCaption_CDockablePane__MEAAXPEAVCDC__VCRect___Z(
    CDockablePane* pThis, CDC* pDC, const RECT* pRectCaption) {
    (void)pThis;
    (void)pDC;
    (void)pRectCaption;
}

// CDockablePane::EnableButton -- transcribed from entry RVA 0x45410 (mfc140u):
// it calls FindButtonByHit(nHit), and when that returns non-NULL writes bEnable
// to the button's +0x14 m_bEnabled field. The semantic PaneCoreState array now
// carries each standard button's ID/enabled state, so this body updates that
// record directly. It does not make the semantic record a raw CMFCCaptionButton
// pointer; FindButtonByHit remains blocked on that ABI-compatible representation.
// Symbol: ?EnableButton@CDockablePane@@IEAAXIH@Z
extern "C" void MS_ABI impl__EnableButton_CDockablePane__IEAAXIH_Z(CDockablePane* pThis, unsigned int nHit, int bEnable) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(pThis);
    if (it == g_paneCoreState_Mfccore.end() || it->second.captionButtons == nullptr) return;
    for (INT_PTR i = 0; i < it->second.captionButtonCount; ++i) {
        auto& button = it->second.captionButtons[i];
        if (button.id == nHit) {
            button.enabled = bEnable ? TRUE : FALSE;
            return;
        }
    }
}

// CDockablePane::EnableGripper -- transcribed from entry RVA 0x40310 (mfc140u).
// When bEnable and m_bHasGripper are both true, retail lazily initializes
// AFX_GLOBAL_DATA and stores m_nTextHeightHorz (+0x2a8) + 5 in m_cyGripper
// (+0x40c); otherwise it stores zero. It then calls SetWindowPos with flags
// 0x37 to recalculate the nonclient frame. The global-data initializer is a
// real body at core/runtime/AFX_GLOBAL_DATA.cpp:675 and UpdateTextMetrics writes
// +0x2a8; it is not an empty stub. CDockablePane::Create records hasGripper in
// PaneCoreState, so that side-state value is used here instead of the local
// member that Create does not populate. SetWindowPos dispatches statically.
// Deviation: a NULL `this` is ignored; retail dereferences it.
// Symbol: ?EnableGripper@CDockablePane@@UEAAXH@Z
extern "C" void MS_ABI impl__EnableGripper_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bEnable) {
    if (pThis == nullptr) return;

    BOOL hasGripper = DockablePaneField<BOOL>(pThis, kOffHasGripper);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        auto it = g_paneCoreState_Mfccore.find(pThis);
        if (it != g_paneCoreState_Mfccore.end()) hasGripper = it->second.hasGripper;
    }
    if (bEnable && hasGripper) {
        EnsureDockingGlobalDataInitialized();
        DockablePaneField<int>(pThis, kOffCyGripper) = DockingGlobalDataInt(0x2a8) + 5;
    } else {
        DockablePaneField<int>(pThis, kOffCyGripper) = 0;
    }
    impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
        static_cast<CBasePane*>(pThis), nullptr, 0, 0, 0, 0, 0x37, nullptr);
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

// STUB. Retail mfc140.dll ANSI-twin entry RVA 0x45110 (mfc140u RVA 0x452c0). It scans m_arrButtons (m_pData at
// this+0x490, m_nSize at this+0x498) to find the button under `point`.
// PaneCoreState now stores semantic button rectangles, but this export returns
// a CMFCCaptionButton* which consumers use as a raw retail-layout object; that
// pointer representation and its GetRect virtual are not modeled.
// Signature corrected to carry `this`; the by-value CPoint occupies one register.
// Symbol: ?FindButton@CDockablePane@@IEBAPEAVCMFCCaptionButton@@VCPoint@@@Z
extern "C" void* MS_ABI impl__FindButton_CDockablePane__IEBAPEAVCMFCCaptionButton__VCPoint___Z(
    const CDockablePane* pThis, void* point) {
    (void)pThis;
    (void)point;
    return nullptr;
}

// STUB. Retail mfc140.dll ANSI-twin entry RVA 0x45210 (mfc140u RVA 0x453c0), fully decoded:
//     for (i = 0; i < m_arrButtons.m_nSize; i++)          // +0x498
//         if (m_arrButtons.m_pData[i]->{+0x20} == nHit)   // +0x490
//             return m_arrButtons.m_pData[i];
//     return NULL;
// (out-of-range indices call AfxThrowInvalidArgException, RVA 0x225b80.)
// PaneCoreState has semantic IDs, but this export returns a pointer to a
// retail-layout button (consumers read fields at +0x14/+0x20); returning a
// CaptionButtonState* would violate that ABI. The layout-compatible pointer
// array is a headerRequest.
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
// EnableGripper now populates it from AFX_GLOBAL_DATA when a gripper is enabled;
// it remains zero until that handler has established a height.
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
// Deviations, both forced: the caption-button scan is dropped because the
// semantic side records cannot be returned as ABI-compatible CMFCCaptionButton*
// objects, and the docking-manager lookup that gates that scan is not modeled.
// So this never reports a caption-button hit code. The remaining four outcomes
// (HTNOWHERE / HTCLIENT / HTCAPTION / HTERROR) are retail's. GetCaptionHeight
// is implemented and uses m_cyGripper; EnableGripper initializes the necessary
// AFX_GLOBAL_DATA metric when it is first needed.
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

// CDockablePane::IsButtonEnabled -- transcribed from entry RVA 0x45430
// (mfc140u): it calls FindButtonByHit(nHit) and returns the button's +0x14
// m_bEnabled, or FALSE when no button matches. This reads the corresponding
// semantic state field; it does not depend on exposing a raw caption-button
// pointer.
// Symbol: ?IsButtonEnabled@CDockablePane@@IEBAHI@Z
extern "C" int MS_ABI impl__IsButtonEnabled_CDockablePane__IEBAHI_Z(const CDockablePane* pThis, unsigned int nHit) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(pThis);
    if (it == g_paneCoreState_Mfccore.end() || it->second.captionButtons == nullptr) return FALSE;
    for (INT_PTR i = 0; i < it->second.captionButtonCount; ++i) {
        const auto& button = it->second.captionButtons[i];
        if (button.id == nHit) return button.enabled;
    }
    return FALSE;
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
// m_bCaptionButtonsCaptured (+0x4d8) is read from PaneCoreState, and the
// StopCaptionButtonsTracking call is made when set. That helper updates the
// modeled pressed/hot state but omits RedrawButton because semantic button
// records are not ABI-compatible CMFCCaptionButton objects.
// Signature corrected to carry `this`; the by-value CPoint occupies one
// register and is forwarded to CPane::OnContextMenu unexamined, as in retail.
// Symbol: ?OnContextMenu@CDockablePane@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CDockablePane__IEAAXPEAVCWnd__VCPoint___Z(
    CDockablePane* pThis, CWnd* pWnd, void* point) {
    if (pThis == nullptr) return;
    if (DockablePaneField<bool>(pThis, kOffPrepareToFloat)) return;
    BOOL buttonsCaptured = FALSE;
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        auto it = g_paneCoreState_Mfccore.find(pThis);
        if (it != g_paneCoreState_Mfccore.end()) buttonsCaptured = it->second.captionButtonsCaptured;
    }
    if (buttonsCaptured) impl__StopCaptionButtonsTracking_CDockablePane__MEAAXXZ(pThis);
    impl__OnContextMenu_CPane__IEAAXPEAVCWnd__VCPoint___Z(
        static_cast<CPane*>(pThis), pWnd, point);
}

// CDockablePane::OnCreate -- transcribed from function entry RVA 0x403a0
// (mfc140u). Retail returns -1 when Default() returns -1; otherwise it calls
// SetCaptionButtons, asks CTooltipManager to create a type-16 tooltip, adds
// callback-text tools with IDs 1..3 and empty rectangles only on successful
// creation, then returns 0. The tooltip pointer is stored in PaneCoreState, not
// in the compact CDockablePane allocation. lpCreateStruct is never read.
// Deviations: a NULL `this` returns -1 instead of faulting; a successful
// CreateToolTip with a NULL/invalid tooltip is guarded before AddTool.
// Symbol: ?OnCreate@CDockablePane@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CDockablePane__IEAAHPEAUtagCREATESTRUCTW___Z(
    CDockablePane* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)lpCreateStruct;
    if (pThis == nullptr) return -1;   // deviation: retail dereferences `this`
    if (impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis)) == -1) return -1;

    impl__SetCaptionButtons_CDockablePane__MEAAXXZ(pThis);
    CToolTipCtrl* pToolTip = DockablePaneToolTipOf(pThis);
    const int bCreated = impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(
        &pToolTip, static_cast<CWnd*>(pThis), 16);
    SetDockablePaneToolTip(pThis, pToolTip);
    if (bCreated && pToolTip != nullptr && pToolTip->m_hWnd != nullptr) {
        RECT rect = {0, 0, 0, 0};
        const wchar_t* callbackText = reinterpret_cast<const wchar_t*>(static_cast<ULONG_PTR>(-1));
        for (unsigned int id = 1; id < 4; ++id) {
            impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(
                pToolTip, static_cast<CWnd*>(pThis), callbackText, &rect, id);
        }
    }
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
// Deviations: the tooltip pointer is retrieved from PaneCoreState rather than
// from retail's in-object +0x4b8 field; the five self-virtuals dispatch
// statically (RemoveCaptionButtons and UndockPane to this file's thunks); and
// OpenMFC's RemoveHiddenMDITabbedBar tolerates a NULL manager, which retail
// would fault on.
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
    CToolTipCtrl* pToolTip = DockablePaneToolTipOf(pThis);
    impl__DeleteToolTip_CTooltipManager__SAXAEAPEAVCToolTipCtrl___Z(&pToolTip);
    SetDockablePaneToolTip(pThis, nullptr);
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
// Deviation, forced: the caption-button arm (m_nHot != 0) is dropped. The
// hot/hit IDs now have semantic PaneCoreState fields, but there is no
// ABI-compatible CMFCCaptionButton object to pass through the retail lookup and
// action sequence; SetCaptionButtons creates only semantic records and
// OnTrackCaptionButtons remains a stub. This body therefore follows the no-hot
// arm. m_bPrepareToFloat is inside the
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
// (close / pin / maximize) through m_arrButtons. The hot/hit IDs are now
// represented in PaneCoreState, but its semantic button records are not
// ABI-compatible CMFCCaptionButton objects, so the release/virtual-action path
// cannot be transcribed faithfully.
// Signature corrected to carry `this`.
// Symbol: ?OnLButtonUp@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nFlags, void* point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// STUB. Retail mfc140.dll ANSI-twin entry RVA 0x41480 (~950 bytes), not transcribed. It branches on
// GetDockingMode() (vtable slot 0x380): the DT_IMMEDIATE arm drags the pane
// itself (CPane::UpdateVirtualRect RVA 0xa1ba0, then either
// CBaseTabbedPane::{slot 0x350} or FloatPane via slot 0x408 followed by
// CGlobalUtils::GetDockingManager/ForceAdjustLayout, RVA 0x6ccc0 / 0x6d800),
// the DT_STANDARD arm moves the drag frame (CMFCDragFrameImpl::MoveDragFrame,
// RVA 0x55450). Blocked: it reads and writes m_bPrepareToFloat (+0x408),
// m_bReadyToFloat (+0x409) and m_bCaptionButtonsCaptured (+0x4d8, represented
// in PaneCoreState), plus a drag anchor point at +0x240, which falls inside the
// 128-byte m_dragFrameImpl blob this repo models opaquely.
// Signature corrected to carry `this`.
// Symbol: ?OnMouseMove@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nFlags, void* point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// STUB. Retail mfc140.dll ANSI-twin entry RVA 0x40570:
//     Default();                                       // 0x180289090
//     if ({slot 0x2f0}())          // body at RVA 0x8780 is `!IsDocked()`
//         for (i = 0; i < m_arrButtons.m_nSize; i++)   // +0x498 / +0x490
//             m_arrButtons[i]->{+0x10} = i + 1;
//     else
//         // reserves the caption strip: GetCaptionHeight() (slot 0x358),
//         // ::CopyRect of lpncsp->rgrc[0] (0x1802c5240) and m_nBorderSize
//         // (+0x414); the rest of that arm was not transcribed.
// The semantic button list is modeled, but not the retail m_arrButtons pointer
// array or pointer-compatible button objects; the nonclient layout also depends
// on button virtuals and visual-manager behavior that are not reproduced here.
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

// STUB. Retail mfc140.dll ANSI-twin entry RVA 0x408f0, not transcribed. Its opening is
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

// CDockablePane::OnNeedTipText -- transcribed from the ANSI-twin function
// entry RVA 0x46120 (mfc140.dll). A NULL pNMHDR throws via
// AfxThrowInvalidArgException (entry RVA 0x225b80); otherwise the handler checks
// the tooltip HWND and pNMHDR->hwndFrom, selects resource IDs 0x3ea0 (close),
// 0x3ea1 (pin), or 0x428e (maximize), and returns FALSE for other IDs or failed
// resource lookup/loading. For the pin ID, VK_CONTROL plus the +0x420
// IsAutohideAllEnabled field adds 3 to the resource ID. `id` and `pResult` are
// unused. The ANSI-twin IAT slot 0x1802c4c08 resolves to USER32!GetAsyncKeyState.
// Successful text is loaded into a file-static CString cache and its pointer is
// written at pNMHDR+0x18, matching the lifetime of retail's module CString at
// static RVA 0x3ba308 (mfc140); cache storage differs but remains valid after
// return. AfxFindStringResourceHandle and CString::LoadString are existing
// thunks. Deviations: `this` is NULL-checked after the retail NULL-NMHDR throw.
// Symbol: ?OnNeedTipText@CDockablePane@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CDockablePane__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    CDockablePane* pThis, unsigned int id, NMHDR* pNMHDR, __int64* pResult) {
    (void)id;
    (void)pResult;
    if (pNMHDR == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    if (pThis == nullptr) return FALSE;

    CToolTipCtrl* pToolTip = DockablePaneToolTipOf(pThis);
    if (pToolTip == nullptr || pToolTip->m_hWnd == nullptr ||
        pNMHDR->hwndFrom != pToolTip->m_hWnd) return FALSE;

    unsigned int resourceID = 0;
    switch (pNMHDR->idFrom) {
    case 1:
        resourceID = 0x3ea0;
        break;
    case 2:
        resourceID = 0x3ea1;
        if (::GetAsyncKeyState(VK_CONTROL) < 0 &&
            DockablePaneField<BOOL>(pThis, kOffEnableAutoHideAll)) {
            resourceID += 3;
        }
        break;
    case 3:
        resourceID = 0x428e;
        break;
    default:
        return FALSE;
    }

    HINSTANCE hInstance = static_cast<HINSTANCE>(
        impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(resourceID));
    if (hInstance == nullptr) return FALSE;

    std::lock_guard<std::mutex> lock(DockablePaneTipTextMutex());
    CString& text = DockablePaneTipTextCache();
    if (!impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
            &text, hInstance, resourceID)) {
        return FALSE;
    }
    reinterpret_cast<NMTTDISPINFOW*>(pNMHDR)->lpszText = const_cast<LPWSTR>(text.GetString());
    return TRUE;
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
// exports, so the virtual call is statically routed to its thunk. m_cyGripper is
// inside the reachable window and is read and cleared at its retail offset;
// EnableGripper then recalculates its height from AFX_GLOBAL_DATA. The
// CBasePane::OnSettingChange sibling is itself a stub whose
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

// CDockablePane::OnSlide -- transcribed from the ANSI-twin function entry RVA
// 0x427a0 (mfc140.dll). Retail returns when IsAutoHideMode() is false and the
// default-slider HWND is not a window. Otherwise it gets the dock-site
// frame's extended style, increments m_nSlideStep (+0x450), obtains the pane
// and slider window rectangles in frame-client coordinates, then either calls
// OffsetRectForSliding on both rectangles (m_ahSlideMode == 1, +0x474) and
// applies CalcCorrectOffset when sliding out, or calls CalcRectForSliding for
// other modes. It finally positions both windows with SWP_NOZORDER |
// SWP_NOACTIVATE. ANSI-twin IAT slots resolved by iat.py are USER32!IsWindow
// (0x1802c5390), GetWindowRect (0x1802c5370), SetWindowPos (0x1802c4d10), and
// OffsetRect (0x1802c5318).
// Deviations: auto-hide mode comes from PaneCoreState via the local C++ getter;
// the default-slider HWND is read from DockablePaneExtra rather than retail
// +0x4c0. GetDockSiteFrameWnd and pane SetWindowPos dispatch use stock thunks,
// so subclass overrides do not run. As in retail, frame and HWNDs are not
// checked before the downstream calls.
// Symbol: ?OnSlide@CDockablePane@@MEAAXH@Z
extern "C" void MS_ABI impl__OnSlide_CDockablePane__MEAAXH_Z(CDockablePane* pThis, int bSlideOut) {
    const HWND hSlider = ExtraOf(pThis).hDefaultSlider;
    if (!pThis->IsAutoHideMode() && !::IsWindow(hSlider)) return;

    CWnd* pFrame = impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(
        static_cast<const CBasePane*>(pThis));
    const unsigned long dwExStyle = impl__GetExStyle_CWnd__QEBAKXZ(pFrame);
    ++DockablePaneField<int>(pThis, kOffSlideStep);

    CRect rectPane(0, 0, 0, 0);
    ::GetWindowRect(pThis->m_hWnd, rectPane);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pFrame, rectPane);
    CRect rectSlider(0, 0, 0, 0);
    ::GetWindowRect(hSlider, rectSlider);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pFrame, rectSlider);

    if (DockablePaneField<UINT>(pThis, kOffAHSlideMode) == 1) {
        const int bRTL = static_cast<int>(dwExStyle & WS_EX_LAYOUTRTL);
        impl__OffsetRectForSliding_CDockablePane__IEAAXAEAVCRect__HH_Z(
            pThis, rectPane, bSlideOut, bRTL);
        impl__OffsetRectForSliding_CDockablePane__IEAAXAEAVCRect__HH_Z(
            pThis, rectSlider, bSlideOut, bRTL);
        if (bSlideOut) {
            POINT correction = {0, 0};
            impl__CalcCorrectOffset_CDockablePane__IEAA_AVCPoint__VCRect__H_Z(
                pThis, &correction, rectPane, bRTL);
            ::OffsetRect(rectPane, correction.x, correction.y);
            ::OffsetRect(rectSlider, correction.x, correction.y);
        }
    } else {
        impl__CalcRectForSliding_CDockablePane__IEAAXAEAVCRect__0H_Z(
            pThis, rectPane, rectSlider, bSlideOut);
    }

    const unsigned int flags = SWP_NOZORDER | SWP_NOACTIVATE;
    impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
        static_cast<CBasePane*>(pThis), nullptr, rectPane.left, rectPane.top,
        rectPane.Width(), rectPane.Height(), flags, nullptr);
    ::SetWindowPos(hSlider, nullptr, rectSlider.left, rectSlider.top,
                   rectSlider.Width(), rectSlider.Height(), flags);
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
// hold CheckAutoHideCondition / CheckStopSlideCondition / OnSlide (entries
// 0x423c0 / 0x42570 / 0x427a0 in mfc140). All three handlers are implemented
// from their retail bodies; the slide path reads the default-slider HWND from
// DockablePaneExtra and statically dispatches stock helper thunks as noted above.
// The five timer/flag fields are inside the reachable padding window and are
// used at their retail offsets; m_hDefaultSlider comes from the file-private
// DockablePaneExtra table. Both the retail .data value (mfc140 RVA 0x3aaac4)
// and OpenMFC's current docking/StaticData.cpp definition initialize
// m_nTimeOutBeforeAutoHide to 700, so the timer period matches retail.
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

// STUB. Retail ANSI-twin function entry RVA 0x44f00 (mfc140.dll): it returns
// when an active popup menu exists; otherwise it calls FindButton(point),
// changes m_nHot and the button's +0x0c state, redraws changed buttons, and
// updates capture / m_bCaptionButtonsCaptured. PaneCoreState now has semantic
// button records and hot/hit IDs, but FindButton returns a retail-layout
// CMFCCaptionButton* and the tracking body accesses that object's fields. The
// semantic records are not ABI-compatible button objects, so this action path
// remains blocked.
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

// CDockablePane::OnUpdateToolTips -- transcribed from the ANSI-twin entry
// RVA 0x46480 (mfc140.dll; the Unicode export was not resolved by the RVA map).
// If wParam contains 0x10, retail creates a type-16 tooltip, adds three
// LPSTR_TEXTCALLBACK tools with empty rectangles and IDs 1..3, then calls
// UpdateTooltips; lParam is unused. The tooltip pointer is held in PaneCoreState.
// Deviations: a NULL `this` and a NULL/invalid tooltip are guarded before calls.
// Symbol: ?OnUpdateToolTips@CDockablePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CDockablePane__IEAA_J_K_J_Z(
    CDockablePane* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)lParam;
    if (pThis == nullptr) return 0;   // deviation: retail has no NULL check
    if ((wParam & 0x10u) != 0) {
        CToolTipCtrl* pToolTip = DockablePaneToolTipOf(pThis);
        (void)impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(
            &pToolTip, static_cast<CWnd*>(pThis), 0x10);
        SetDockablePaneToolTip(pThis, pToolTip);
        if (pToolTip != nullptr && pToolTip->m_hWnd != nullptr) {
            RECT rect = {0, 0, 0, 0};
            const wchar_t* callbackText = reinterpret_cast<const wchar_t*>(static_cast<ULONG_PTR>(-1));
            for (unsigned int id = 1; id < 4; ++id) {
                impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(
                    pToolTip, static_cast<CWnd*>(pThis), callbackText, &rect, id);
            }
            impl__UpdateTooltips_CDockablePane__IEAAXXZ(pThis);
        }
    }
    return 0;
}

// CDockablePane::PreTranslateMessage -- PARTIAL transcription of the ANSI-twin
// entry RVA 0x453a0 (mfc140.dll; the Unicode export was not resolved by the RVA
// map). The message-id decode is the `sub`/`je` chain at instruction addresses
// 0x453b5..0x45409 inside that entry; the rest reads:
//     switch (pMsg->message) {
//     case WM_NCLBUTTONDOWN: case WM_NCLBUTTONUP: case WM_NCRBUTTONDOWN:
//     case WM_NCRBUTTONUP: case WM_NCMBUTTONDOWN: case WM_NCMBUTTONUP:
//     case WM_KEYDOWN: case WM_SYSKEYDOWN: case WM_MOUSEMOVE: case WM_LBUTTONDOWN:
//     case WM_LBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONUP: case WM_MBUTTONDOWN:
//     case WM_MBUTTONUP:
//         if (m_pToolTip != NULL && m_pToolTip->m_hWnd != NULL)          // +0x4b8
//             ::SendMessage(m_pToolTip->m_hWnd, TTM_RELAYEVENT /*0x407*/, 0, (LPARAM)pMsg);
//                 // mfc140 IAT slot 0x1802c5378 = USER32!SendMessageA; the
//                 // mfc140u equivalent is SendMessageW.
//     }
//     if (pMsg->message == WM_KEYDOWN) {
//         if ((GetDockingMode() & DT_STANDARD /*`test $2,%al`*/) &&      // vtable slot 0x380
//             m_bPrepareToFloat && pMsg->wParam == VK_ESCAPE) {           // +0x408
//             ::PostMessage(m_hWnd, WM_CANCELMODE, 0, 0);                 // IAT 0x1802c52d8 = USER32!PostMessageA; Unicode twin: PostMessageW
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
//                 CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd)); // IAT 0x1802c5300 = USER32!GetParent
//                 if (pParent != NULL && pParent->IsKindOf(<RTC 0x3aa418 "CPaneFrameWnd">) &&
//                     CWnd::FromHandle(::GetCapture()) == pParent) {      // IAT 0x1802c4c38 = USER32!GetCapture
//                     ::PostMessage(pParent->m_hWnd, WM_CANCELMODE, 0, 0);
//                     return TRUE;
//                 }
//             }
//         }
//     }
//     return CBasePane::PreTranslateMessage(pMsg);                        // RVA 0xcbb0
// Reproduced: the tooltip relay, DT_STANDARD escape arm, tabbed-pane escape
// arm and base-class tail. Dropped: (1) the mini-frame dragging byte (+0x10d) is
// held in a side table private to docking/CPaneFrameWnd.cpp, so only the
// tabbed pane's own m_bPrepareToFloat state is tested; (2) the DT_SMART arm
// needs a CDockingManager smart-docking-manager member at +0x308 and a
// CSmartDockingManager, neither available in this build.
// Other deviations: NULL check on `this` and pMsg; the self-virtuals dispatch
// statically; CBasePane::PreTranslateMessage (docking/CBasePane.cpp:1423) is
// still a stub returning 0 whose parameter list omits `this`.
// Signature corrected to carry `this`.
// Symbol: ?PreTranslateMessage@CDockablePane@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CDockablePane__UEAAHPEAUtagMSG___Z(CDockablePane* pThis, MSG* pMsg) {
    if (pThis == nullptr || pMsg == nullptr) return FALSE;   // deviation: retail has no NULL check

    bool relayTooltip = false;
    switch (pMsg->message) {
    case WM_NCLBUTTONDOWN: case WM_NCLBUTTONUP:
    case WM_NCRBUTTONDOWN: case WM_NCRBUTTONUP:
    case WM_NCMBUTTONDOWN: case WM_NCMBUTTONUP:
    case WM_KEYDOWN: case WM_SYSKEYDOWN:
    case WM_MOUSEMOVE: case WM_LBUTTONDOWN: case WM_LBUTTONUP:
    case WM_RBUTTONDOWN: case WM_RBUTTONUP: case WM_MBUTTONDOWN: case WM_MBUTTONUP:
        relayTooltip = true;
        break;
    default:
        break;
    }
    CToolTipCtrl* pToolTip = DockablePaneToolTipOf(pThis);
    if (relayTooltip && pToolTip != nullptr && pToolTip->m_hWnd != nullptr) {
        ::SendMessage(pToolTip->m_hWnd, 0x0407 /* TTM_RELAYEVENT */, 0,
                      reinterpret_cast<LPARAM>(pMsg));
    }

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
// Blocked: the model has semantic button records but no ABI-compatible
// CMFCCaptionButton object whose enabled flag/GetRect can be read here, and the
// OnNcPaint that would consume m_rectRedraw is itself a stub.
// Signature corrected to carry `this`.
// Symbol: ?RedrawButton@CDockablePane@@IEAAXPEBVCMFCCaptionButton@@@Z
extern "C" void MS_ABI impl__RedrawButton_CDockablePane__IEAAXPEBVCMFCCaptionButton___Z(
    CDockablePane* pThis, const void* pButton) {
    (void)pThis;
    (void)pButton;
}

// CDockablePane::RemoveCaptionButtons -- transcribed from entry RVA 0x46060
// (mfc140.dll ANSI twin; the Unicode export was not resolved by the RVA map).
// Retail deletes each non-NULL array element and removes all entries. The
// modeled PaneCoreState array contains trivial semantic records rather than
// CMFCCaptionButton objects, so this clears and frees that array instead.
// Deviation: NULL `this` is ignored; retail dereferences it.
// Symbol: ?RemoveCaptionButtons@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__RemoveCaptionButtons_CDockablePane__IEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    openmfc::docking_state::CaptionButtonState* buttons = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        auto it = g_paneCoreState_Mfccore.find(pThis);
        if (it == g_paneCoreState_Mfccore.end()) return;
        auto& state = it->second;
        buttons = state.captionButtons;
        state.captionButtons = nullptr;
        state.captionButtonCount = 0;
        state.captionButtonCapacity = 0;
        state.captionButtonGrowBy = 0;
    }
    if (buttons != nullptr) ::HeapFree(::GetProcessHeap(), 0, buttons);
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
// through an IsKindOf(CMultiPaneFrameWnd) test selecting the CPaneFrameWnd or
// CMultiPaneFrameWnd side-state body; the divider one to CPaneDivider's
// side-state ReplacePane body. The latter cannot currently be reached for
// OpenMFC-created compact dividers because GetDefaultPaneDivider returns NULL.
// RemovePaneFromDockManager remains a stub, so its follow-up cleanup is omitted.
// A NULL check on `this` is added.
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
// CRecentDockSiteInfo's named POD mirror in afxmfc.h exposes the float rect at
// +0x08 and the slider docked rect at +0x50; CPane::LoadState reads those retail
// locations. The containing member remains at CPane +0x2b8. The two inline stream operators are reproduced with Read/Write of
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
    char* pRecentInfo = reinterpret_cast<char*>(&pThis->m_recentDockInfo);
    char* pRecentFloat = pRecentInfo + kOffRecentFloatRect;
    char* pRecentDocked = pRecentInfo + kOffRecentDockedRect;
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

// CDockablePane::SetCaptionButtons -- transcribed from entry RVA 0x45f30
// (mfc140.dll ANSI twin; the Unicode export was not resolved by the RVA map).
// Retail first removes old entries, then appends three enabled, initially
// unhidden CMFCCaptionButton objects with IDs 0x13, 9 and 8. This stores those
// defaults in the modeled semantic CaptionButtonState array; it does not create
// raw 0x38-byte CMFCCaptionButton objects, whose pointer-compatible layout is
// still needed by FindButton / FindButtonByHit and drawing consumers.
// Deviation: NULL `this` is ignored; retail dereferences it.
// Symbol: ?SetCaptionButtons@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__SetCaptionButtons_CDockablePane__MEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    constexpr UINT ids[] = {0x13, 0x9, 0x8};
    auto* buttons = static_cast<openmfc::docking_state::CaptionButtonState*>(
        ::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY,
                    sizeof(openmfc::docking_state::CaptionButtonState) * 3));
    if (buttons != nullptr) {
        for (std::size_t i = 0; i < 3; ++i) {
            ::new (static_cast<void*>(&buttons[i])) openmfc::docking_state::CaptionButtonState{};
            buttons[i].id = ids[i];
            buttons[i].visible = TRUE;
            buttons[i].enabled = TRUE;
        }
    }
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        auto& state = g_paneCoreState_Mfccore[pThis];
        if (state.captionButtons != nullptr) {
            ::HeapFree(::GetProcessHeap(), 0, state.captionButtons);
        }
        state.captionButtons = buttons;
        state.captionButtonCount = buttons != nullptr ? 3 : 0;
        state.captionButtonCapacity = state.captionButtonCount;
        state.captionButtonGrowBy = 0;
    }
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

// CDockablePane::Slide -- transcribed from the ANSI-twin entry RVA 0x42d30
// (mfc140.dll). It cancels old timers, derives the full-slide delta, handles the
// immediate (no-timer) path, and otherwise coordinates the auto-hide manager,
// window visibility/z-order, timer, and animated delta. m_ahSlideMode (+0x474),
// m_rectRestored (+0x458), slide fields, and the 700-ms/1-ms/12-step statics are
// available. The remote-session flag is AFX_GLOBAL_DATA +0x25c; its real
// Initialize thunk is called lazily here, and UpdateTextMetrics populates the
// horizontal text height at +0x2a8 used by EnableGripper.
// Deviations: `this` is NULL-checked; pane/global helpers and manager methods
// dispatch statically. DockingManager operations use the existing side-state
// implementations, and a missing manager/default divider is tolerated rather
// than dereferenced. If the frame is absent, coordinate conversion is skipped
// and the available screen-coordinate extent is used. Auto-hide mode is read
// from PaneCoreState.
// Symbol: ?Slide@CDockablePane@@UEAAXHH@Z
extern "C" void MS_ABI impl__Slide_CDockablePane__UEAAXHH_Z(CDockablePane* pThis, int bSlideOut, int bUseTimer) {
    if (pThis == nullptr || !pThis->IsAutoHideMode()) return;

    UINT_PTR& slideTimer = DockablePaneField<UINT_PTR>(pThis, kOffSlideTimer);
    UINT_PTR& conditionTimer = DockablePaneField<UINT_PTR>(pThis, kOffAutoHideConditionTimerID);
    if (slideTimer != 0) ::KillTimer(pThis->m_hWnd, slideTimer);
    if (conditionTimer != 0) {
        ::KillTimer(pThis->m_hWnd, conditionTimer);
        conditionTimer = 0;
    }

    RECT rectWindow = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWindow);
    const unsigned long alignment =
        impl__GetCurrentAlignment_CBasePane__UEBAKXZ(static_cast<const CBasePane*>(pThis));
    const auto fullExtent = [&]() {
        return (alignment & 0xa000UL) != 0
            ? rectWindow.bottom - rectWindow.top
            : rectWindow.right - rectWindow.left;
    };

    const int disableAnimation = impl__m_bDisableAnimation_CDockablePane__2HA;
    int remoteSession = 0;
    if (bUseTimer && !disableAnimation) {
        EnsureDockingGlobalDataInitialized();
        remoteSession = DockingGlobalDataInt(0x25c);
    }
    if (!bUseTimer || disableAnimation || remoteSession) {
        DockablePaneField<int>(pThis, kOffSlideDelta) = fullExtent();
    }

    if (!bUseTimer) {
        DockablePaneField<RECT>(pThis, kOffRectRestored) = rectWindow;
        impl__OnSlide_CDockablePane__MEAAXH_Z(pThis, FALSE);
        impl__ShowWindow_CWnd__QEAAHH_Z(static_cast<CWnd*>(pThis), SW_HIDE);
        ::ShowWindow(ExtraOf(pThis).hDefaultSlider, SW_HIDE);
        return;
    }

    CWnd* pFrame = impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(
        static_cast<const CBasePane*>(pThis));
    CDockingManager* pManager = impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(
        reinterpret_cast<CGlobalUtils*>(impl__afxGlobalUtils__3VCGlobalUtils__A), pFrame);
    const HWND hSlider = ExtraOf(pThis).hDefaultSlider;
    if (bSlideOut) {
        if (pManager != nullptr) {
            impl__HideAutoHidePanes_CDockingManager__QEAAXPEAVCDockablePane__H_Z(
                pManager, pThis, FALSE);
            impl__AlignAutoHidePane_CDockingManager__QEAAXPEAVCPaneDivider__H_Z(
                pManager, impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(pThis), FALSE);
        }
        impl__ShowWindow_CWnd__QEAAHH_Z(static_cast<CWnd*>(pThis), SW_SHOW);
        ::ShowWindow(hSlider, SW_SHOW);
    }
    ::BringWindowToTop(pThis->m_hWnd);
    ::BringWindowToTop(hSlider);
    if (DockablePaneField<UINT>(pThis, kOffAHSlideMode) == 1 && pManager != nullptr) {
        impl__BringBarsToTop_CDockingManager__QEAAXKH_Z(pManager, 0, TRUE);
    }

    slideTimer = ::SetTimer(pThis->m_hWnd,
        static_cast<UINT_PTR>(kTimerAutoHideSlideIn + (bSlideOut ? 1 : 0)),
        static_cast<UINT>(impl__m_nSlideDefaultTimeOut_CDockablePane__1HA), nullptr);

    if (!disableAnimation && !remoteSession) {
        const UINT slideMode = DockablePaneField<UINT>(pThis, kOffAHSlideMode);
        if (slideMode == 1) {
            RECT rectClient = rectWindow;
            if (pFrame != nullptr) {
                impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pFrame, &rectClient);
            }
            const int extent = (alignment & 0xa000UL) != 0
                ? rectClient.bottom - rectClient.top
                : rectClient.right - rectClient.left;
            const int steps = impl__m_nSlideSteps_CDockablePane__2HA;
            if (steps > 0) {
                const int delta = extent / steps;
                DockablePaneField<int>(pThis, kOffSlideDelta) = delta > 1 ? delta : 1;
            }
        } else if (slideMode == 2) {
            if (!bSlideOut && DockablePaneField<int>(pThis, kOffIsSliding) == FALSE) {
                DockablePaneField<RECT>(pThis, kOffRectRestored) = rectWindow;
                if (pFrame != nullptr) {
                    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(
                        pFrame, &DockablePaneField<RECT>(pThis, kOffRectRestored));
                }
            }
            const RECT& restored = DockablePaneField<RECT>(pThis, kOffRectRestored);
            const int extent = (alignment & 0xa000UL) != 0
                ? restored.bottom - restored.top
                : restored.right - restored.left;
            const int steps = impl__m_nSlideSteps_CDockablePane__2HA;
            if (steps > 0) {
                const int delta = extent / steps;
                DockablePaneField<int>(pThis, kOffSlideDelta) = delta > 1 ? delta : 1;
            }
        }
    }
    DockablePaneField<int>(pThis, kOffSlideStep) = 0;
    DockablePaneField<int>(pThis, kOffIsSliding) = TRUE;
}

// CDockablePane::StopCaptionButtonsTracking -- transcribed from the ANSI-twin
// entry RVA 0x45030 (mfc140.dll; the Unicode export was not resolved by the RVA
// map). Retail clears the hit ID and releases capture when m_nHit is nonzero,
// clears that button's pushed state, then does the same for m_nHot and its
// focused state; finally it clears m_bCaptionButtonsCaptured. The equivalent
// IDs/flags are updated in PaneCoreState. RedrawButton is omitted because these
// records are not ABI-compatible CMFCCaptionButton pointers. The side-state's
// UINT_MAX initial value is treated as an unset ID rather than releasing capture
// for a nonexistent button.
// Deviations: NULL `this` is ignored; retail dereferences it.
// Symbol: ?StopCaptionButtonsTracking@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CDockablePane__MEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    UINT hitID = 0;
    UINT hotID = 0;
    bool hadHit = false;
    bool hadHot = false;
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        auto it = g_paneCoreState_Mfccore.find(pThis);
        if (it == g_paneCoreState_Mfccore.end()) return;
        hitID = it->second.hitCaptionButtonID;
        hotID = it->second.hotCaptionButtonID;
        hadHit = hitID != 0 && hitID != static_cast<UINT>(-1);
        hadHot = hotID != 0 && hotID != static_cast<UINT>(-1);
        it->second.hitCaptionButtonID = 0;
        it->second.hotCaptionButtonID = 0;
    }
    if (hadHit) ::ReleaseCapture();
    if (hadHot) ::ReleaseCapture();

    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(pThis);
    if (it == g_paneCoreState_Mfccore.end()) return;
    if (it->second.captionButtons != nullptr) {
        for (INT_PTR i = 0; i < it->second.captionButtonCount; ++i) {
            auto& button = it->second.captionButtons[i];
            if (button.id == hitID && hadHit) button.pressed = FALSE;
            if (button.id == hotID && hadHot) button.hot = FALSE;
        }
    }
    it->second.captionButtonsCaptured = FALSE;
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
// IsKindOf(CMultiPaneFrameWnd) test -- the CPaneFrameWnd slot is an empty ret;
// the CMultiPaneFrameWnd side-state body performs the manager lookup, but its
// downstream CRecentDockSiteInfo::StoreDockInfo export is still a placeholder;
// (2) the divider arm is dropped: OpenMFC's CPaneDivider is a CObject with a
// side table and no in-object container-manager member, and GetDefaultPaneDivider()
// above cannot return a non-NULL divider for local compact objects anyway, so
// the arm is unreachable here;
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

// CDockablePane::StoreRecentTabRelatedInfo -- transcribed from the retail
// mfc140.dll ANSI-twin entry RVA 0x41350. If this pane is tabbed, retail gets
// its tab control and parent CDockablePane, then obtains this pane's mini-frame
// and the parent pane's default divider. With a mini-frame, it calls vtable
// slot +0x3d8 (CMultiPaneFrameWnd entry RVA 0x94950 (mfc140.dll); the ordinary
// CPaneFrameWnd slot is an ICF-folded `ret`). Without a mini-frame but with a
// divider, it directly calls CPaneDivider::StoreRecentTabRelatedInfo at RVA
// 0xad130 (mfc140.dll) -- this is not a vtable-slot call. This implementation
// routes the stock classes to their existing thunks; those calls are static,
// so client subclass overrides are not honored. The helpers use OpenMFC's
// docking/container state, including its limited recent-container linkage.
// The tab/parent tests match the retail early returns. Win32 GetParent calls
// implement the mfc140.dll IAT slot 0x1802c5300 (USER32!GetParent, resolved
// with iat.py); FromHandle and RTTI use the existing thunks.
// Deviation: NULL check on this; retail dereferences it immediately.
// Symbol: ?StoreRecentTabRelatedInfo@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__StoreRecentTabRelatedInfo_CDockablePane__MEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;
    if (!impl__IsTabbed_CPane__UEBAHXZ(static_cast<const CPane*>(pThis))) return;

    CWnd* pTabWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pTabWnd == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pTabWnd),
            impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ())) {
        return;
    }

    CWnd* pBarWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pTabWnd->m_hWnd));
    if (pBarWnd == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pBarWnd),
            impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ())) {
        return;
    }
    CDockablePane* pTabbedBar = reinterpret_cast<CDockablePane*>(pBarWnd);
    void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), FALSE);
    void* pDivider = impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(pTabbedBar);

    if (pMiniFrame != nullptr) {
        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(static_cast<CWnd*>(pMiniFrame)),
                impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ())) {
            impl__StoreRecentTabRelatedInfo_CMultiPaneFrameWnd__UEAAXPEAVCDockablePane__0_Z(
                pMiniFrame, pThis, pTabbedBar);
        }
        // The ordinary CPaneFrameWnd vtable's +0x3d8 slot is a no-op.
        return;
    }
    if (pDivider != nullptr) {
        impl__StoreRecentTabRelatedInfo_CPaneDivider__QEAAXPEAVCDockablePane__0_Z(
            pDivider, pThis, pTabbedBar);
    }
}

// STUB. Retail mfc140.dll ANSI-twin entry RVA 0x462c0 obtains the default
// pane divider, checks CanAutoHide(), and, when both conditions pass, calls
// SetAutoHideMode(!m_bPinState, pDivider->GetCurrentAlignment(), NULL, TRUE).
// OpenMFC declares CPaneDivider as CObject rather than its retail CBasePane
// base class, so there is no safe GetCurrentAlignment dispatch/layout for the
// divider; substituting this pane's alignment would not transcribe retail.
// Symbol: ?ToggleAutoHide@CDockablePane@@UEAAXXZ
extern "C" void MS_ABI impl__ToggleAutoHide_CDockablePane__UEAAXXZ(CDockablePane* pThis) {
    (void)pThis;
}

// STUB. Retail mfc140.dll ANSI-twin entry RVA 0x44d20; not transcribed. Signature corrected to
// carry `this` (it is called from OnBeforeChangeParent above).
// Symbol: ?UndockPane@CDockablePane@@UEAAXH@Z
extern "C" void MS_ABI impl__UndockPane_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bDelay) {
    (void)pThis;
    (void)bDelay;
}

// STUB. Retail mfc140.dll ANSI-twin entry RVA 0x42150 clears m_bPinState,
// kills the auto-hide/slide timers, removes the pane from its CMFCAutoHideBar,
// clears its default divider, and then performs docking/frame updates through
// several virtual slots. Those final operations depend on retail-shaped
// CPaneDivider/CPaneFrameWnd dispatch and the bar's embedded layout, which this
// build does not provide; keep the thunk honest rather than applying a partial
// sequence with different docking side effects.
// Signature corrected from the placeholder list using the mangled export: this
// plus the CDockablePane* pFirstBarInGroup argument.
// Symbol: ?UnSetAutoHideMode@CDockablePane@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__UnSetAutoHideMode_CDockablePane__UEAAXPEAV1__Z(
    CDockablePane* pThis, CDockablePane* pFirstBarInGroup) {
    (void)pThis;
    (void)pFirstBarInGroup;
}

// CDockablePane::UpdateTooltips -- transcribed from the ANSI-twin entry
// RVA 0x40440 (mfc140.dll; the Unicode export was not resolved by the RVA map).
// Retail returns without a tooltip window, otherwise obtains the pane rectangle,
// limits its button loop by TTM_GETTOOLCOUNT, offsets each button's GetRect()
// result by the pane client origin and calls CToolTipCtrl::SetToolRect with IDs
// 1..N. This port uses PaneCoreState's semantic button rectangles and copies at
// most the three standard records. No current caption-layout path populates those
// rectangles (SetCaptionButtons initializes them empty), so this only updates
// non-empty geometry if another owned path has supplied it; custom raw button
// objects and their layout remain unsupported.
// Symbol: ?UpdateTooltips@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltips_CDockablePane__IEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    CToolTipCtrl* pToolTip = DockablePaneToolTipOf(pThis);
    if (pToolTip == nullptr || pToolTip->m_hWnd == nullptr) return;

    openmfc::docking_state::CaptionButtonState buttons[3]{};
    std::size_t count = 0;
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        auto it = g_paneCoreState_Mfccore.find(pThis);
        if (it == g_paneCoreState_Mfccore.end() || it->second.captionButtons == nullptr) return;
        const INT_PTR modeledCount = it->second.captionButtonCount;
        count = modeledCount > 0 ? (modeledCount < 3 ? static_cast<std::size_t>(modeledCount) : 3) : 0;
        for (std::size_t i = 0; i < count; ++i) buttons[i] = it->second.captionButtons[i];
    }

    RECT rectWindow = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWindow);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(static_cast<const CWnd*>(pThis), &rectWindow);
    const LRESULT toolCount = ::SendMessage(pToolTip->m_hWnd, 0x040d /* TTM_GETTOOLCOUNT */, 0, 0);
    if (toolCount <= 0) return;
    if (count > static_cast<std::size_t>(toolCount)) count = static_cast<std::size_t>(toolCount);
    for (std::size_t i = 0; i < count; ++i) {
        RECT rect = buttons[i].rect;
        ::OffsetRect(&rect, rectWindow.left, rectWindow.top);
        impl__SetToolRect_CToolTipCtrl__QEAAXPEAVCWnd___KPEBUtagRECT___Z(
            pToolTip, static_cast<CWnd*>(pThis), static_cast<unsigned __int64>(i + 1), &rect);
    }
}
