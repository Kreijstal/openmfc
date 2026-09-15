// CMDIFrameWndEx — OpenMFC implementation.
// Sources: cbarcore.cpp, global_other-3_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// OpenMFC — CMDIFrameWndEx methods (wave1_other-3, revised; 2026-09-14
// implementation pass on the remaining stubs).
//
// Functions carrying a transcription comment were decoded from the retail
// export (ordinal -> RVA -> objdump).  Every RVA named in a comment below is a
// function ENTRY; the image it belongs to is named where the comment was
// written in the 2026-09-14 pass ("mfc140" = the ANSI twin whose bodies are
// byte-identical, "mfc140u" = the Unicode DLL OpenMFC reimplements -- the two
// images place every export at a DIFFERENT address).  Older comments that name
// a bare RVA mean mfc140_rva_symbols.json, i.e. mfc140.  Exports absent from
// both RVA maps were resolved through the mfc140u export directory (ures.py:
// ordinal -> RVA); several of those turn out to be ICF-folded trivial bodies,
// and the comment says so where it applies.
//
// Retail CMDIFrameWndEx (sizeof 0x4710) embeds a CDockingManager at this+0x228,
// a CFrameImpl at this+0x590 and a CMDIClientAreaWnd at this+0x720; OpenMFC's
// object is 0x268 bytes.  How this file bridges that -- the retail scalars kept
// in the padding at their retail offsets, m_Impl and m_dockManager built on the
// heap through their ctor thunks and reached via a per-frame record, the client
// area not constructed at all -- is documented at S_CMDIFrameWndExTail /
// FrameExtra below.  Bodies that still cannot be reproduced are documented
// stubs: a body that is only `{}` or a bare constant return is a STUB unless
// its comment shows that the retail body is exactly that, and the comment says
// what was read and what is missing.  No offsets are invented and no header is
// edited.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <shobjidl.h>

#include <cstddef>
#include <cstring>
#include <cwchar>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---- sibling impl_ exports (defined in other phase4/src/*.cpp) ----
// CWnd::Default (thunks.cpp)
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
// CFrameWnd::OnSysCommand (thunks.cpp)
extern "C" void MS_ABI impl__OnSysCommand_CFrameWnd__IEAAXI_J_Z(
    CFrameWnd* pThis, unsigned int nID, __int64 lParam);
// CMDIChildWnd::OnUpdateFrameTitle / PreCreateWindow / PreTranslateMessage
// (frame_font_exports.cpp)
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIChildWnd__MEAAXH_Z(
    CMDIChildWnd* pThis, int bAddToTitle);
extern "C" int MS_ABI impl__PreCreateWindow_CMDIChildWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CMDIChildWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIChildWnd__UEAAHPEAUtagMSG___Z(
    CMDIChildWnd* pThis, MSG* pMsg);

// ---- sibling impl_ exports used by the CMDIFrameWndEx bodies below ----
// Every callee named here is the extern "C" thunk, never the C++ method: the
// class declarations in include/openmfc/ have no linkable method symbols.
// CFrameWnd (core/frame/Thunks.cpp)
extern "C" int MS_ABI impl__OnNcActivate_CFrameWnd__IEAAHH_Z(CFrameWnd* pThis, int bActive);
extern "C" void MS_ABI impl__OnActivate_CFrameWnd__IEAAXIPEAVCWnd__H_Z(
    CFrameWnd* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized);
extern "C" void MS_ABI impl__OnClose_CFrameWnd__IEAAXXZ(CFrameWnd* pThis);
extern "C" void MS_ABI impl__OnContextHelp_CFrameWnd__QEAAXXZ(CFrameWnd* pThis);
extern "C" int MS_ABI impl__CanEnterHelpMode_CFrameWnd__QEAAHXZ(CFrameWnd* pThis);
extern "C" int MS_ABI impl__OnCreateHelper_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
    CFrameWnd* pThis, CREATESTRUCTW* pcs, CCreateContext* pContext);
extern "C" int MS_ABI impl__NegotiateBorderSpace_CFrameWnd__UEAAHIPEAUtagRECT___Z(
    CFrameWnd* pThis, unsigned int nBorderCmd, RECT* lpRectBorder);
// CMDIFrameWnd (core/frame/CMDIFrameWnd.cpp)
extern "C" int MS_ABI impl__OnCmdMsg_CMDIFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CMDIFrameWnd* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
extern "C" int MS_ABI impl__OnCommand_CMDIFrameWnd__MEAAH_K_J_Z(
    CMDIFrameWnd* pThis, WPARAM wParam, LPARAM lParam);
extern "C" int MS_ABI impl__OnCreateClient_CMDIFrameWnd__UEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
    CMDIFrameWnd* pThis, CREATESTRUCTW* pcs, CCreateContext* pContext);
extern "C" void MS_ABI impl__OnDestroy_CMDIFrameWnd__IEAAXXZ(CMDIFrameWnd* pThis);
extern "C" void* MS_ABI impl__GetWindowMenuPopup_CMDIFrameWnd__UEAAPEAUHMENU____PEAU2__Z(
    void* pThis, void* hMenuBar);
// CWnd (core/window/CWnd.cpp)
extern "C" void MS_ABI impl__HtmlHelpW_CWnd__UEAAX_KI_Z(
    CWnd* pThis, unsigned long long dwData, unsigned int nCmd);
// CFrameImpl statics (core/frame/CFrameImpl.cpp) — both are no-op stubs today,
// but retail's OnCreate/OnDestroy do call them, so the calls are kept.
extern "C" void MS_ABI impl__AddFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(void* pFrame);
extern "C" void MS_ABI impl__RemoveFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(void* pFrame);
// CMFCToolBar static (featurepack/toolbar/CMFCToolBar.cpp)
extern "C" void MS_ABI impl__SetHelpMode_CMFCToolBar__SAXH_Z(int bHelpMode);
// CMFCToolBar::m_bCustomizeMode — the protected static BOOL exported as
// ?m_bCustomizeMode@CMFCToolBar@@1HA; its storage lives in
// phase4/src/featurepack/toolbar/StaticData.cpp and sibling files
// (CMFCToolBar.cpp, CPaneFrameWnd.cpp) read it exactly this way. This is the
// global retail tests at 0x1803b70bc.
extern "C" int impl__m_bCustomizeMode_CMFCToolBar__1HA;
// COleClientItem (core/ole/Thunks.cpp)
extern "C" void MS_ABI impl__Deactivate_COleClientItem__QEAAXXZ(void* pItem);
// MFC exceptions (detail/MfcExceptionsSupport.cpp)
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

// Forward declarations of CMDIFrameWndEx thunks defined further down in this
// file but called by handlers that appear before them.
extern "C" void MS_ABI impl__AdjustDockingLayout_CMDIFrameWndEx__UEAAXPEAX_Z(
    CMDIFrameWndEx* pThis, void* pParam);
extern "C" void MS_ABI impl__AdjustClientArea_CMDIFrameWndEx__UEAAXXZ(
    CMDIFrameWndEx* pThis);
extern "C" void MS_ABI impl__RecalcLayout_CMDIFrameWndEx__UEAAXH_Z(
    CMDIFrameWndEx* pThis, int bNotify);
extern "C" void* MS_ABI impl__GetInPlaceActiveItem_CMDIFrameWndEx__MEAAPEAVCOleClientItem__XZ(
    CMDIFrameWndEx* pThis);
extern "C" void MS_ABI impl__OnContextHelp_CMDIFrameWndEx__IEAAXXZ(CMDIFrameWndEx* pThis);
extern "C" int MS_ABI impl__TabbedDocumentToControlBar_CMDIFrameWndEx__UEAAHPEAVCMDIChildWndEx___Z(
    CMDIFrameWndEx* pThis, CMDIChildWndEx* pMDIChildWnd);

// ---- more sibling thunks (all definitions verified in the tree) ----
// CFrameImpl (core/frame/CFrameImpl.cpp).  `pThis` for these is the opaque
// token that file keys its side tables on; this file constructs one per frame
// (see FrameExtra below) with the ctor thunk, exactly as retail constructs the
// embedded m_Impl(this) at this+0x590.
extern "C" void* MS_ABI impl___0CFrameImpl__QEAA_PEAVCFrameWnd___Z(void* pThis, CFrameWnd* pFrame);
extern "C" void MS_ABI impl___1CFrameImpl__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__OnCompositionChanged_CFrameImpl__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnChangeVisualManager_CFrameImpl__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnNcMouseMove_CFrameImpl__IEAAXIVCPoint___Z(void* pThis, unsigned int nHitTest, long long point);
extern "C" int MS_ABI impl__OnNcPaint_CFrameImpl__IEAAHXZ(void* pThis);
extern "C" void MS_ABI impl__OnSetText_CFrameImpl__IEAAXPEB_W_Z(void* pThis, const wchar_t* lpszText);
extern "C" void MS_ABI impl__OnWindowPosChanging_CFrameImpl__IEAAXPEAUtagWINDOWPOS___Z(void* pThis, WINDOWPOS* lpwp);
extern "C" void MS_ABI impl__UpdateCaption_CFrameImpl__IEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__DeleteToolBar_CFrameImpl__IEAAHPEAVCMFCToolBar___Z(void* pThis, void* pToolBar);
extern "C" void* MS_ABI impl__CreateNewToolBar_CFrameImpl__IEAAPEBVCMFCToolBar__PEB_W_Z(void* pThis, const wchar_t* lpszName);
extern "C" void MS_ABI impl__RestorePosition_CFrameImpl__IEAAXAEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* pcs);
extern "C" int MS_ABI impl__IsHelpKey_CFrameImpl__KAHPEAUtagMSG___Z(MSG* lpMsg);
extern "C" int MS_ABI impl__ProcessKeyboard_CFrameImpl__IEAAHHPEAH_Z(void* pThis, int nKey, int* pnAccelIndex);
extern "C" int MS_ABI impl__ProcessMouseClick_CFrameImpl__IEAAHIUtagPOINT__PEAUHWND_____Z(void* pThis, unsigned int uiMsg, long long pt, HWND hwnd);
extern "C" int MS_ABI impl__ProcessMouseMove_CFrameImpl__IEAAHUtagPOINT___Z(void* pThis, long long pt);
extern "C" int MS_ABI impl__ProcessMouseWheel_CFrameImpl__IEAAH_K_J_Z(void* pThis, unsigned long long wParam, long long lParam);
extern "C" void MS_ABI impl__OnCloseFrame_CFrameImpl__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__DeactivateMenu_CFrameImpl__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnActivateApp_CFrameImpl__IEAAXH_Z(void* pThis, int bActive);
// File-level (non-export) setters CFrameImpl.cpp provides for the members retail's
// frame classes write into m_Impl directly.
extern "C" void MS_ABI openmfc_CFrameImpl_SetDockManager(void* pFrameImpl, CDockingManager* pDockManager);
extern "C" void MS_ABI openmfc_CFrameImpl_SetRibbonBar(void* pFrameImpl, CMFCRibbonBar* pRibbonBar);
extern "C" void MS_ABI openmfc_CFrameImpl_SetRibbonStatusBar(void* pFrameImpl, CWnd* pRibbonStatusBar);
// CDockingManager (featurepack/docking/Thunks.cpp, CDockingManager.cpp)
extern "C" void* MS_ABI impl___0CDockingManager__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CDockingManager__UEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
    CDockingManager* pThis, CBasePane* pPane, int bTail, int bAutoHide, int bInsertForOuterEdge);
extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
    CDockingManager* pThis, unsigned int nID, int bSearchMiniFrames);
// (impl__PaneFromPoint_CDockingManager ...H_N... is declared in detail/MfccoreSupport.h with CPoint by value)
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
    const CDockingManager* pThis, long long point, int nSensitivity, unsigned long* pdwAlignment,
    CRuntimeClass* pRTCBarType, const CDockingManager* pDockManager);
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
    CDockingManager* pThis, CBasePane* pPane, int bDestroy, int bAdjustLayout, int bAutoHide, CBasePane* pBarReplacement);
extern "C" void MS_ABI impl__RedrawAllMiniFrames_CDockingManager__QEAAXXZ(CDockingManager* pThis);
extern "C" void MS_ABI impl__OnActivateFrame_CDockingManager__UEAAXH_Z(CDockingManager* pThis, int bActivate);
extern "C" void MS_ABI impl__RecalcLayout_CDockingManager__UEAAXH_Z(CDockingManager* pThis, int bNotify);
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(CDockingManager* pThis, void* hdwp);
extern "C" void MS_ABI impl__FixupVirtualRects_CDockingManager__UEAAXXZ(CDockingManager* pThis);
extern "C" int MS_ABI impl__ProcessPaneContextMenuCommand_CDockingManager__QEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CDockingManager* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
extern "C" void MS_ABI impl__DockPane_CDockingManager__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    CDockingManager* pThis, CBasePane* pBar, unsigned int nDockBarID, const RECT* lpRect);
extern "C" int MS_ABI impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(
    CDockingManager* pThis, CPane* pBarToDock, CPane* pBar);
extern "C" int MS_ABI impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(CDockingManager* pThis, unsigned long dwStyle);
extern "C" int MS_ABI impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
    CDockingManager* pThis, CBasePane* pControlBar, CBasePane* pTarget, int bAfter);
extern "C" int MS_ABI impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(
    const CDockingManager* pThis, long long point, unsigned long* pdwBarAlignment, int* pbOuterEdge);
extern "C" int MS_ABI impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(CDockingManager* pThis, CWnd* pFrame);
extern "C" int MS_ABI impl__EnableDocking_CDockingManager__QEAAHK_Z(CDockingManager* pThis, unsigned long dwDockStyle);
// CBasePane / CDockablePane (featurepack/docking)
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(CBasePane* pThis, int bShow, int bDelay, int bActivate);
extern "C" void MS_ABI impl__ShowPane_CDockablePane__UEAAXHHH_Z(CDockablePane* pThis, int bShow, int bDelay, int bActivate);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
// Runtime classes / RTTI (core/runtime/CObject.cpp and the per-class RuntimeClasses.cpp files)
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPreviewViewEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDocument__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonStatusBar__SAPEAUCRuntimeClass__XZ();
// COleDocument (core/ole/COleDocument.cpp).  NOTE: that file's definition still
// carries an auto-generated one-parameter placeholder list; this declaration is
// the one the mangled name describes (this, CWnd*).
extern "C" void* MS_ABI impl__GetInPlaceActiveItem_COleDocument__UEAAPEAVCOleClientItem__PEAVCWnd___Z(void* pThis, CWnd* pWnd);
// CWnd / CFrameWnd / CMDIFrameWnd / CMenu
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__OnSysColorChange_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" void MS_ABI impl__WinHelpW_CWnd__UEAAX_KI_Z(CWnd* pThis, unsigned long long dwData, unsigned int nCmd);
extern "C" CFrameWnd* MS_ABI impl__GetActiveFrame_CMDIFrameWnd__UEAAPEAVCFrameWnd__XZ(CMDIFrameWnd* pThis);
extern "C" CMDIChildWnd* MS_ABI impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(const CMDIFrameWnd* pThis, int* pbMaximized);
extern "C" void MS_ABI impl__OnWindowNew_CMDIFrameWnd__IEAAXXZ(CMDIFrameWnd* pThis);
extern "C" void MS_ABI impl__OnUpdateFrameMenu_CMDIFrameWnd__UEAAXPEAUHMENU_____Z(CMDIFrameWnd* pThis, HMENU hMenuAlt);
extern "C" int MS_ABI impl__PreCreateWindow_CMDIFrameWnd__UEAAHAEAUtagCREATESTRUCTW___Z(CMDIFrameWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIFrameWnd__UEAAHPEAUtagMSG___Z(CMDIFrameWnd* pThis, MSG* pMsg);
extern "C" int MS_ABI impl__SetMenu_CFrameWnd__UEAAHPEAVCMenu___Z(CFrameWnd* pThis, CMenu* pMenu);
extern "C" void MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int bNotify);
extern "C" void MS_ABI impl__OnSetPreviewMode_CFrameWnd__UEAAXHPEAUCPrintPreviewState___Z(CFrameWnd* pThis, int bPreview, CPrintPreviewState* pState);
extern "C" CMenu* MS_ABI impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU hMenu);
// CMFCRibbonBar (featurepack/ribbon/CMFCRibbonBar.cpp)
extern "C" void MS_ABI impl__SetActiveMDIChild_CMFCRibbonBar__QEAAXPEAVCWnd___Z(CMFCRibbonBar* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl__SetMaximizeMode_CMFCRibbonBar__QEAAXHPEAVCWnd___Z(CMFCRibbonBar* pThis, int bMax, CWnd* pWnd);
extern "C" int MS_ABI impl__OnSysKeyDown_CMFCRibbonBar__QEAAHPEAVCFrameWnd___K_J_Z(CMFCRibbonBar* pThis, CFrameWnd* pFrame, unsigned long long wParam, long long lParam);
extern "C" int MS_ABI impl__OnSysKeyUp_CMFCRibbonBar__QEAAHPEAVCFrameWnd___K_J_Z(CMFCRibbonBar* pThis, CFrameWnd* pFrame, unsigned long long wParam, long long lParam);
// CMFCPopupMenu / CMFCToolBar statics (featurepack/menu, featurepack/toolbar)
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
extern "C" void* MS_ABI impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(const CMFCPopupMenu* pThis);
extern "C" CMFCPopupMenu* MS_ABI impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
extern "C" void MS_ABI impl__RedrawUnderlines_CMFCToolBar__SAXXZ();
// CMDIChildWndEx (core/frame/CMDIChildWndEx.cpp -- implemented in parallel; only
// thunks whose definitions are already in the tree are named here)
extern "C" int MS_ABI impl__IsRegisteredWithTaskbarTabs_CMDIChildWndEx__QEAAHXZ(CMDIChildWndEx* pThis);
extern "C" void MS_ABI impl__RegisterTaskbarTab_CMDIChildWndEx__UEAAXPEAV1__Z(CMDIChildWndEx* pThis, CMDIChildWndEx* pWndNext);
extern "C" void MS_ABI impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(CMDIChildWndEx* pThis, int bCheckRegisteredTab);
extern "C" int MS_ABI impl__InvalidateIconicBitmaps_CMDIChildWndEx__QEAAHXZ(CMDIChildWndEx* pThis);
// AFX_GLOBAL_DATA (core/runtime/AFX_GLOBAL_DATA.cpp, featurepack/CMFC_misc_stubs.cpp).
// OpenMFC's afxGlobalData is a 720-byte blob read at the retail offsets, exactly as
// core/frame/CFrameImpl.cpp does; the Initialize/Resume/GetITaskbarList3 bodies take
// no `this` in this tree (retail passes &afxGlobalData in rcx, which they ignore).
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ();
extern "C" int MS_ABI impl__Resume_AFX_GLOBAL_DATA__QEAAHXZ();
extern "C" void* MS_ABI impl__GetITaskbarList3_AFX_GLOBAL_DATA__QEAAPEAUITaskbarList3__XZ();
// CMFCPopupMenu items / CMFCToolBarMenuButton (featurepack/menu, featurepack/toolbar,
// detail/CbarcoreSupport.cpp); the ctor thunk placement-constructs OpenMFC's class.
extern "C" int MS_ABI impl__InsertSeparator_CMFCPopupMenu__QEAAHH_Z(CMFCPopupMenu* pThis, int iInsertAt);
extern "C" int MS_ABI impl__InsertItem_CMFCPopupMenu__QEAAHAEBVCMFCToolBarMenuButton__H_Z(
    CMFCPopupMenu* pThis, const CMFCToolBarMenuButton* pButton, int iInsertAt);
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(
    void* pThis, unsigned int uiID, HMENU hMenu, int iImage, const wchar_t* lpszText, int bUserButton);
extern "C" void MS_ABI impl___1CMFCToolBarMenuButton__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__AddBasicCommand_CMFCToolBar__SAXI_Z(unsigned int uiCmd);
extern "C" int MS_ABI impl__OnShowPopupMenu_CFrameImpl__IEAAHPEAVCMFCPopupMenu__PEAVCFrameWnd___Z(
    void* pThis, void* pPopupMenu, CFrameWnd* pWnd);
// CDockingManager::SetPrintPreviewMode -- the definition in
// featurepack/docking/CDockingManager.cpp still carries a placeholder list; this
// is the list the mangled name describes.
extern "C" void MS_ABI impl__SetPrintPreviewMode_CDockingManager__QEAAXHPEAUCPrintPreviewState___Z(
    CDockingManager* pThis, int bPreview, CPrintPreviewState* pState);
// AfxFindStringResourceHandle (featurepack/CMFC_misc_stubs.cpp)
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);
// Registered messages (core/runtime/Globals.cpp)
extern "C" unsigned int impl__AFX_WM_POSTSETPREVIEWFRAME__3IA;
// AfxGetApp -- not an mfc140u export (inline in MFC); the file-level helper
// detail/RegcoreSupport.cpp defines, used exactly as core/frame/CFrameImpl.cpp does.
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ();
// CContextMenuManager (featurepack/menu/CContextMenuManager.cpp)
extern "C" unsigned int MS_ABI impl__TrackPopupMenu_CContextMenuManager__UEAAIPEAUHMENU____HHPEAVCWnd__H_Z(
    CContextMenuManager* pThis, HMENU hMenu, int x, int y, CWnd* pWndOwner, int bRightAlign);

namespace {

// ---------------------------------------------------------------------------
// Retail CMDIFrameWndEx layout (sizeof 0x4710: the operator-new size in
// ?CreateObject@CMDIFrameWndEx@@SAPEAVCObject@@XZ, entry RVA 0x87b40 mfc140u),
// transcribed from the constructor ??0CMDIFrameWndEx@@IEAA@XZ (entry RVA
// 0x87b90, mfc140u) and cross-checked against the destructor (0x87ce0, mfc140u)
// and the uses cited on each line.  OpenMFC's CMDIFrameWndEx is 0x268 bytes
// (CMDIFrameWnd 0x1e0 + m_pVisualManager + 128 bytes of padding), so:
//   +0x1e0  CString  (ctor stores the string manager's nil-string data pointer:
//                    IAtlStringMgr slot 3 GetNilString + 0x18; EnableWindowsDialog
//                    assigns the menu text into it) -- OpenMFC keeps
//                    m_pVisualManager at 0x1e0, so this CString is NOT modeled.
//   +0x1e8..0x227  the scalar members below; they fall inside OpenMFC's padding
//                    blob, so they are kept IN the object at their retail offsets
//                    (S_CMDIFrameWndExTail).  The retail ctor zero-fills them
//                    except +0x1fc, which it sets to 1.
//   +0x228  CDockingManager m_dockManager  (ctor 0x46ef0 mfc140u, 0x368 bytes)
//   +0x590  CFrameImpl      m_Impl(this)   (ctor 0x61ef0 mfc140u, 0x190 bytes;
//                    its +0x180/+0x188 are the ribbon bar / ribbon status bar,
//                    i.e. frame+0x710 / frame+0x718)
//   +0x720  CMDIClientAreaWnd m_wndClientArea (ctor 0x7c900 mfc140u, 0x3ff0 bytes)
// The three sub-objects do not fit in OpenMFC's object.  m_Impl and
// m_dockManager are therefore constructed on the heap through their ctor
// thunks and reached through the per-frame FrameExtra record below (the same
// side-table pattern core/frame/CFrameImpl.cpp itself uses for its members);
// m_wndClientArea is not constructed at all (see the headerRequests in
// core/frame/CMDIClientAreaWnd.cpp) and every body that needs it says so.
// ---------------------------------------------------------------------------
struct S_CMDIFrameWndExTail {
    HMENU m_hmenuWindow;                 // +0x1e8  GetWindowMenuPopup caches its result here
    UINT  m_uiWindowsDlgMenuId;          // +0x1f0  EnableWindowsDialog; ShowPopupMenu inserts this id
    int   _unwritten1f4;                 // +0x1f4  not written by the retail ctor
    BOOL  m_bContextHelp;                // +0x1f8  OnContextHelp latch; HtmlHelp/WinHelp test it
    BOOL  m_bDoSubclass;                 // +0x1fc  ctor: 1; OnCreateClient subclasses the MDI client on it
    BOOL  m_bShowWindowsDlgAlways;       // +0x200  EnableWindowsDialog; ShowPopupMenu tests it
    BOOL  m_bShowWindowsDlgHelpButton;   // +0x204  EnableWindowsDialog; ShowWindowsDialog passes it
    int   _zero208;                      // +0x208  zeroed by the ctor's 8-byte store at +0x204
    BOOL  m_bWasMaximized;               // +0x20c  OnSize: (nType == SIZE_MAXIMIZED) at exit
    BOOL  m_bIsMinimized;                // +0x210  OnSize: (nType == SIZE_MINIMIZED) on entry
    BOOL  m_bClosing;                    // +0x214  OnClose latch; GetWindowMenuPopup / OnSize test it
    UINT  m_nFrameID;                    // +0x218  Load/SaveMDIState pass it as the uiID
    int   _zero21c;                      // +0x21c  zeroed by the ctor's 8-byte store at +0x214
    CWnd* m_pWndCloseFirst;              // +0x220  OnClose: WM_CLOSE goes to this window instead (then NULL)
};
static_assert(sizeof(S_CMDIFrameWndExTail) == 0x40, "retail +0x1e8..+0x227");
static_assert(offsetof(S_CMDIFrameWndExTail, m_bDoSubclass) == 0x1fc - 0x1e8, "");
static_assert(offsetof(S_CMDIFrameWndExTail, m_bClosing) == 0x214 - 0x1e8, "");
static_assert(offsetof(S_CMDIFrameWndExTail, m_pWndCloseFirst) == 0x220 - 0x1e8, "");
// The member names above are those of the public afxmdiframewndex.h; the
// identification of each rests on the retail use cited on its line, not on the
// header text.
struct TailAccess : CMDIFrameWndEx {
    using CMDIFrameWndEx::_mdiframewndex_padding;
};
static_assert(offsetof(TailAccess, _mdiframewndex_padding) == 0x1e8, "padding must start at retail +0x1e8");
static_assert(sizeof(TailAccess::_mdiframewndex_padding) >= sizeof(S_CMDIFrameWndExTail), "tail fits in the padding");
static_assert(offsetof(CMDIFrameWnd, m_hWndMDIClient) == 0x1d8, "retail CMDIFrameWnd::m_hWndMDIClient");

S_CMDIFrameWndExTail* Tail(CMDIFrameWndEx* pThis) {
    return reinterpret_cast<S_CMDIFrameWndExTail*>(reinterpret_cast<unsigned char*>(pThis) + 0x1e8);
}
const S_CMDIFrameWndExTail* Tail(const CMDIFrameWndEx* pThis) {
    return reinterpret_cast<const S_CMDIFrameWndExTail*>(reinterpret_cast<const unsigned char*>(pThis) + 0x1e8);
}

// Heap stand-ins for the two sub-objects.  The CFrameImpl block is the retail
// size; the CFrameImpl exports never touch its bytes (CFrameImpl.cpp keys every
// member on the pointer), but the size is kept so that retail-shaped readers are
// not pointed at a short buffer.  The docking manager is OpenMFC's own class, so
// its thunks expect OpenMFC's layout and size.
struct FrameImplBlock { alignas(16) unsigned char bytes[0x190]; };
struct DockManagerBlock { alignas(16) unsigned char bytes[sizeof(CDockingManager)]; };

struct FrameExtra {
    const CMDIFrameWndEx* pFrame;
    FrameImplBlock* pImpl;            // retail m_Impl (this+0x590)
    DockManagerBlock* pDockManager;   // retail m_dockManager (this+0x228)
    CMFCRibbonBar* pRibbonBar;        // mirror of m_Impl.+0x180 (this+0x710); AddPane writes it
    CWnd* pRibbonStatusBar;           // mirror of m_Impl.+0x188 (this+0x718); AddPane writes it
    BOOL bInRecalcLayout;             // stand-in for CFrameWnd::m_bInRecalcLayout (retail this+0x1a0)
    wchar_t* pszWindowsDlgMenuText;   // stand-in for the CString at retail this+0x1e0 (EnableWindowsDialog)
    size_t cbWindowsDlgMenuText;      // its allocation size (sized operator delete)
    FrameExtra* pNext;
};

// Hand-rolled list under an SRW lock: std::unordered_map / std::mutex would add
// libstdc++ undefineds that this file's link audit rejects (CMDIClientAreaWnd.cpp
// keeps its registry the same way).
SRWLOCK g_frameExtraLock = SRWLOCK_INIT;
FrameExtra* g_frameExtraHead = nullptr;

FrameExtra* FindExtra(const CMDIFrameWndEx* pFrame) {
    AcquireSRWLockExclusive(&g_frameExtraLock);
    FrameExtra* p = g_frameExtraHead;
    while (p != nullptr && p->pFrame != pFrame) {
        p = p->pNext;
    }
    ReleaseSRWLockExclusive(&g_frameExtraLock);
    return p;
}

// Retail constructs m_dockManager with its default constructor (the ctor at
// 0x87c11 mfc140u passes only rcx) and m_Impl with (this); m_dockManager's
// parent pointer is written later by OnCreate (see there).
FrameExtra* AttachExtra(CMDIFrameWndEx* pFrame) {
    FrameExtra* p = FindExtra(pFrame);
    if (p != nullptr) {
        return p;
    }
    p = new FrameExtra();
    p->pFrame = pFrame;
    p->pImpl = new FrameImplBlock();
    memset(p->pImpl->bytes, 0, sizeof(p->pImpl->bytes));
    impl___0CFrameImpl__QEAA_PEAVCFrameWnd___Z(p->pImpl, pFrame);
    p->pDockManager = new DockManagerBlock();
    memset(p->pDockManager->bytes, 0, sizeof(p->pDockManager->bytes));
    impl___0CDockingManager__QEAA_XZ(p->pDockManager);
    p->pRibbonBar = nullptr;
    p->pRibbonStatusBar = nullptr;
    p->bInRecalcLayout = FALSE;
    p->pszWindowsDlgMenuText = nullptr;
    p->cbWindowsDlgMenuText = 0;
    AcquireSRWLockExclusive(&g_frameExtraLock);
    p->pNext = g_frameExtraHead;
    g_frameExtraHead = p;
    ReleaseSRWLockExclusive(&g_frameExtraLock);
    return p;
}

// Retail's destructor order is ~m_wndClientArea, ~m_Impl, ~m_dockManager (then the
// CString release and ~CMDIFrameWnd); the two we construct are torn down in that
// order here.
void DetachExtra(CMDIFrameWndEx* pFrame) {
    AcquireSRWLockExclusive(&g_frameExtraLock);
    FrameExtra** pp = &g_frameExtraHead;
    while (*pp != nullptr && (*pp)->pFrame != pFrame) {
        pp = &(*pp)->pNext;
    }
    FrameExtra* p = *pp;
    if (p != nullptr) {
        *pp = p->pNext;
    }
    ReleaseSRWLockExclusive(&g_frameExtraLock);
    if (p == nullptr) {
        return;
    }
    impl___1CFrameImpl__UEAA_XZ(p->pImpl);
    delete p->pImpl;
    impl___1CDockingManager__UEAA_XZ(p->pDockManager);
    delete p->pDockManager;
    if (p->pszWindowsDlgMenuText != nullptr) {   // the +0x1e0 CString release
        ::operator delete(p->pszWindowsDlgMenuText, p->cbWindowsDlgMenuText);
    }
    delete p;
}

// The m_Impl / m_dockManager of a frame, created on first use if the frame was
// built without this file's constructor (nothing in the tree does that today).
void* FrameImplOf(CMDIFrameWndEx* pThis) {
    FrameExtra* p = AttachExtra(pThis);
    return p != nullptr ? static_cast<void*>(p->pImpl) : nullptr;
}
CDockingManager* DockManagerOf(CMDIFrameWndEx* pThis) {
    FrameExtra* p = AttachExtra(pThis);
    return p != nullptr ? reinterpret_cast<CDockingManager*>(p->pDockManager) : nullptr;
}
CMFCRibbonBar* RibbonBarOf(CMDIFrameWndEx* pThis) {
    FrameExtra* p = FindExtra(pThis);
    return p != nullptr ? p->pRibbonBar : nullptr;
}
// The retail CString at this+0x1e0 (m_strWindowsDlgMenuText): OpenMFC keeps
// m_pVisualManager there, so the text lives in FrameExtra as a plain copy.
const wchar_t* WindowsDlgMenuText(CMDIFrameWndEx* pThis) {
    FrameExtra* p = FindExtra(pThis);
    return p != nullptr ? p->pszWindowsDlgMenuText : nullptr;
}
void SetWindowsDlgMenuText(CMDIFrameWndEx* pThis, const wchar_t* pszText) {
    FrameExtra* p = AttachExtra(pThis);
    if (p == nullptr) {
        return;
    }
    if (p->pszWindowsDlgMenuText != nullptr) {
        ::operator delete(p->pszWindowsDlgMenuText, p->cbWindowsDlgMenuText);
        p->pszWindowsDlgMenuText = nullptr;
        p->cbWindowsDlgMenuText = 0;
    }
    if (pszText != nullptr) {
        // plain operator new/delete only: new[]/delete[] would add libstdc++
        // undefineds this file's link audit rejects
        size_t cb = (wcslen(pszText) + 1) * sizeof(wchar_t);
        p->pszWindowsDlgMenuText = static_cast<wchar_t*>(::operator new(cb));
        p->cbWindowsDlgMenuText = cb;
        memcpy(p->pszWindowsDlgMenuText, pszText, cb);
    }
}

// OpenMFC's CDockingManager keeps its parent frame in a protected member that
// the C++ constructor CFrameWndEx.cpp uses would set; the default-ctor thunk
// (what retail's ctor calls) leaves it NULL, and retail's OnCreate then stores
// the frame into m_dockManager+0x1b0 (this+0x3d8) itself.  That store is
// reproduced through this accessor.
struct DockManagerAccess : CDockingManager {
    using CDockingManager::m_pParentWnd;
};
void SetDockManagerParent(CDockingManager* pDock, CFrameWnd* pFrame) {
    if (pDock != nullptr) {
        static_cast<DockManagerAccess*>(pDock)->m_pParentWnd = pFrame;
    }
}
CFrameWnd* DockManagerParent(CDockingManager* pDock) {
    return pDock != nullptr ? static_cast<DockManagerAccess*>(pDock)->m_pParentWnd : nullptr;
}

// Retail's `if (!afxGlobalData.<+0x00>) { afxGlobalData.Initialize(); <+0x00> = 1; }`
// prologue, emitted inline before every AFX_GLOBAL_DATA use; the same helper as
// core/frame/CFrameImpl.cpp.
void EnsureGlobalDataInitialized() {
    int* pInitialized = reinterpret_cast<int*>(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
    if (*pInitialized == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ();
        *pInitialized = 1;
    }
}
int GlobalDataInt(size_t offset) {
    int value = 0;
    memcpy(&value, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + offset, sizeof(value));
    return value;
}
void SetGlobalDataInt(size_t offset, int value) {
    memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + offset, &value, sizeof(value));
}

// CPoint / POINT by value is an 8-byte aggregate in one register under MS x64.
long long PackPoint(LONG x, LONG y) {
    return static_cast<long long>((static_cast<unsigned long long>(static_cast<unsigned long>(y)) << 32) |
                                  static_cast<unsigned long>(x));
}

// CWnd::m_hWnd of any CWnd-derived object (retail +0x40, OpenMFC the same).
HWND WndHandle(const CWnd* pWnd) {
    return pWnd != nullptr ? pWnd->m_hWnd : nullptr;
}

// Retail's afxContextMenuManager (mfc140u .data 0x3be1b0): the pointer the
// CContextMenuManager constructor (0x2f640 mfc140u) stores itself into while
// that global is still NULL.  OpenMFC's constructor publishes nothing, so the
// closest reachable equivalent is the manager the current CWinAppEx owns
// (CWinAppEx::m_pContextMenuManager, written by InitContextMenuManager).  The
// member is read directly rather than through GetContextMenuManager, which
// would create one on demand and turn retail's "none exists" gate into "always".
struct WinAppExAccess : CWinAppEx {
    using CWinAppEx::m_pContextMenuManager;
};
CContextMenuManager* CurrentContextMenuManager() {
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return static_cast<WinAppExAccess*>(static_cast<CWinAppEx*>(pApp))->m_pContextMenuManager;
}

// CBasePane::ShowPane through the class the retail vtable slot 139 (+0x458)
// would dispatch to.  OpenMFC has no MSVC vtables, so the two DLL overrides that
// exist as exports are selected by runtime class; a client-side override is not
// reached (deviation shared by every virtual call in this file).
void DispatchShowPane(CBasePane* pBar, int bShow, int bDelay, int bActivate) {
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pBar, impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ())) {
        impl__ShowPane_CDockablePane__UEAAXHHH_Z(reinterpret_cast<CDockablePane*>(pBar), bShow, bDelay, bActivate);
    } else {
        impl__ShowPane_CBasePane__UEAAXHHH_Z(pBar, bShow, bDelay, bActivate);
    }
}

}  // namespace

//=============================================================================
// CMDIChildWndEx — message handlers
//=============================================================================



















//=============================================================================
// CMDIFrameWndEx — docking / MDI-tab operations
//=============================================================================












// Symbol: ?CreateObject@CMDIFrameWndEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMDIFrameWndEx__SAPEAVCObject__XZ() {
    return new CMDIFrameWndEx();
}
// Symbol: ?GetThisClass@CMDIFrameWndEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ() {
    return &CMDIFrameWndEx::classCMDIFrameWndEx;
}
// Symbol: ?GetRuntimeClass@CMDIFrameWndEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIFrameWndEx__UEBAPEAUCRuntimeClass__XZ(CMDIFrameWndEx* pThis) {
    return pThis ? &CMDIFrameWndEx::classCMDIFrameWndEx : nullptr;
}
// Symbol: ?LoadFrame@CMDIFrameWndEx@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__LoadFrame_CMDIFrameWndEx__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CMDIFrameWndEx* pThis, UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
    return pThis ? pThis->CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext) : FALSE;
}
// Symbol: ?EnableDocking@CMDIFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_CMDIFrameWndEx__QEAAHK_Z(CMDIFrameWndEx* pThis, DWORD dwDockStyle) {
    if (!pThis) {
        return FALSE;
    }
    pThis->CFrameWnd::EnableDocking(dwDockStyle);
    return TRUE;
}
// CMDIFrameWndEx::OnShowPopupMenu(CMFCPopupMenu*) — transcribed from retail
// mfc140 (entry RVA 0x68600). The body is:
//     if (!g_afxGlobalDataInited) { AFX_GLOBAL_DATA::Initialize(&afxGlobalData);
//                                   g_afxGlobalDataInited = 1; }
//     if (*(int*)0x1803ba5f8 != 0 && pMenuPopup != NULL)
//         ::NotifyWinEvent(EVENT_SYSTEM_MENUPOPUPSTART, pMenuPopup->m_hWnd, 0, 0);
//     return TRUE;                                      // unconditional
// (the NotifyWinEvent import is the slot at 0x1802c5238, resolved with iat.py.)
// DEVIATION: the accessibility gate at 0x1803ba5f8 is a private afxGlobalData
// flag that OpenMFC does not model, so the notification is sent whenever the
// popup has a window. ::NotifyWinEvent is a no-op when no accessibility client
// is hooked, so this is observationally safe; it is still a deviation.
// Symbol: ?OnShowPopupMenu@CMDIFrameWndEx@@UEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__OnShowPopupMenu_CMDIFrameWndEx__UEAAHPEAVCMFCPopupMenu___Z(
    CMDIFrameWndEx* pThis, CMFCPopupMenu* pMenuPopup) {
    (void)pThis;
    if (pMenuPopup != nullptr) {
        HWND hWnd = pMenuPopup->GetSafeHwnd();
        if (hWnd != nullptr) {
            // EVENT_SYSTEM_MENUPOPUPSTART / OBJID_WINDOW / CHILDID_SELF
            ::NotifyWinEvent(0x0006, hWnd, 0, 0);
        }
    }
    return TRUE;
}
// CMDIFrameWndEx::OnClosePopupMenu(CMFCPopupMenu*) — retail entry RVA 0x88eb0
// (mfc140; 0x88900 mfc140u), transcribed:
//     if (!afxGlobalData.<+0x00>) { afxGlobalData.Initialize(); <+0x00> = 1; }
//     if (afxGlobalData.+0x278 /* the accessibility gate, 0x1803ba5f8 mfc140 */ != 0 && pMenuPopup != NULL) {
//         CMFCPopupMenu* pParent = pMenuPopup->GetParentPopupMenu();          // 0xb7b10 mfc140
//         DWORD dwEvent = (pMenuPopup->+0x19ac == 0 && pParent == NULL && pMenuPopup->+0x228 == NULL)
//                             ? EVENT_SYSTEM_MENUEND (5) : EVENT_SYSTEM_MENUPOPUPEND (7);
//         ::NotifyWinEvent(dwEvent, pMenuPopup->m_hWnd, 0, 0);                // import slot 0x1802c5238
//     }
//     if (CMFCPopupMenu::m_pActivePopupMenu == pMenuPopup) m_pActivePopupMenu = NULL;   // 0x1803b6fe8 mfc140
//     this->+0x570 = 0;                                                       // m_dockManager + 0x348
// (An earlier revision of this comment had the +0x228 test inverted; the
// instructions at 0x88f03..0x88f0d compare it against rax == 0 and pick 5 only
// when it IS NULL.)  +0x228 is CMFCPopupMenu::m_pParentButton in
// detail/CMFCPopupMenuSupport.h; +0x19ac is a CMFCPopupMenu int that header
// does not name.  The accessibility gate is read from OpenMFC's afxGlobalData
// blob at the retail offset, as CFrameImpl.cpp reads its neighbours.
// DEVIATION: the store into m_dockManager+0x348 has no member in OpenMFC's
// CDockingManager and is skipped.
// Symbol: ?OnClosePopupMenu@CMDIFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_CMDIFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(
    CMDIFrameWndEx* pThis, CMFCPopupMenu* pMenuPopup) {
    (void)pThis;
    EnsureGlobalDataInitialized();
    if (GlobalDataInt(0x278) != 0 && pMenuPopup != nullptr) {
        const unsigned char* pBytes = reinterpret_cast<const unsigned char*>(pMenuPopup);
        void* pParent = impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(pMenuPopup);
        int nField19ac = 0;
        void* pParentButton = nullptr;
        memcpy(&nField19ac, pBytes + 0x19ac, sizeof(nField19ac));
        memcpy(&pParentButton, pBytes + 0x228, sizeof(pParentButton));
        DWORD dwEvent = (nField19ac == 0 && pParent == nullptr && pParentButton == nullptr)
                            ? EVENT_SYSTEM_MENUEND : EVENT_SYSTEM_MENUPOPUPEND;
        ::NotifyWinEvent(dwEvent, pMenuPopup->m_hWnd, 0, 0);
    }
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA == pMenuPopup) {
        impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA = nullptr;
    }
}
// CMDIFrameWndEx::OnShowPanes(BOOL) — retail entry RVA 0x89640:
//     int bRes = m_dockManager.<vtable +0xb0>(bShow);   // dock mgr slot 22 =
//                       // ?ShowPanes@CDockingManager@@UEAAHH@Z (0x18004d490),
//                       // read out of the manager vtable at RVA 0x2e4070
//     this-><vtable +0x3a0>(NULL);                      // AdjustDockingLayout
//     return bRes;
// The frame vtable slot +0x3a0 is CMDIFrameWndEx::AdjustDockingLayout (read out
// of the retail vtable at RVA 0x2ec8f8, slot 116), so that call is reproduced
// through the sibling thunk below.
// DEVIATION: the embedded CDockingManager lives at this+0x228 in retail and is
// not modeled by OpenMFC's CMDIFrameWndEx, so its result is unavailable; TRUE
// is returned instead of the dock manager's value. The AdjustDockingLayout call
// is made through the impl__ thunk rather than the object's vtable, so an
// override in a derived client class is not dispatched to.
// Symbol: ?OnShowPanes@CMDIFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_CMDIFrameWndEx__UEAAHH_Z(CMDIFrameWndEx* pThis, int bShow) {
    (void)bShow;
    if (!pThis) return FALSE;
    impl__AdjustDockingLayout_CMDIFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
    return TRUE;
}
// CMDIFrameWndEx::OnToolbarCreateNew(WPARAM, LPARAM) — retail entry RVA
// 0x88f70, transcribed:
//     if (lParam == 0) AfxThrowInvalidArgException();   // never returns
//     return (LRESULT)m_Impl.CreateNewToolBar((LPCTSTR)lParam);
// (m_Impl is the CFrameImpl sub-object at this+0x590; the tail jump target
// 0x180062820 is ?CreateNewToolBar@CFrameImpl@@IEAAPEBVCMFCToolBar@@PEBD@Z.)
// Only the argument check is reproduced: OpenMFC's CMDIFrameWndEx does not
// embed a CFrameImpl, so the toolbar cannot be created and 0 is returned.
// Symbol: ?OnToolbarCreateNew@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarCreateNew_CMDIFrameWndEx__IEAA_J_K_J_Z(
    CMDIFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    if (lParam == 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;
    }
    // TODO(clean-room): retail returns m_Impl.CreateNewToolBar((LPCTSTR)lParam);
    // the CFrameImpl sub-object at this+0x590 is not modeled.
    return 0;
}
// CMDIFrameWndEx::OnShowMDITabContextMenu(CPoint, DWORD, BOOL) — retail entry
// RVA 0x8a5e0 (mfc140u; the ANSI twin is 0x8ab90).  The export is absent from
// the mfc140u RVA map only -- mfc140_rva_symbols.json does list the ANSI twin
// at 0x8ab90 -- and ordinal 11182 of the mfc140u export directory places the
// Unicode body at 0x8a5e0 (ures.py); the two bodies are byte-identical.
// Transcribed in full:
//     if (!(dwAllowedItems & 0x10)) return FALSE;         // test $0x10,%r8b
//     if (afxContextMenuManager /* mfc140u .data 0x3be1b0 */ == NULL) return FALSE;
//     CMenu menu;  menu.Attach(::CreatePopupMenu());      // 0x2a8100; import slot 0x1802c6cc0
//     CString str;                                        // nil-string from the string manager
//     HINSTANCE h = AfxFindStringResourceHandle(0x42c0);  // 0x2aee00
//     if (!h || !str.LoadString(h, 0x42c0))               // 0xdb70
//         AfxThrowInvalidArgException();                  // 0x227720, at 0x8a7b0
//     ::AppendMenu(menu.m_hMenu, MF_STRING, 0xffffff96, str);       // slot 0x1802c6cb8 (AppendMenuW)
//     ::CheckMenuItem(menu.m_hMenu, 0xffffff96, MF_CHECKED);        // slot 0x1802c6da0
//     HWND hwndFrame = this ? this->m_hWnd : NULL;        // test %rsi,%rsi at 0x8a6b4
//     UINT nCmd = afxContextMenuManager-><vtable +0x38>(menu.m_hMenu, point.x, point.y, this, FALSE);
//     if (::IsWindow(hwndFrame) && nCmd == 0xffffff96) {           // slot 0x1802c7138
//         CMDIChildWnd* pActive = MDIGetActive(NULL);               // 0x2a6ad0
//         if (pActive && pActive->IsKindOf(RUNTIME_CLASS(CMDIChildWndEx)))   // 0x234cf0; descriptor 0x2ed888 = "CMDIChildWndEx"
//             this-><vtable +0x3c0>(pActive);                       // slot 120 = TabbedDocumentToControlBar (0x8a2b0)
//     }
//     // ~CString (refcount release), then ~CMenu inlined: if (m_hMenu) {
//     //   CHandleMap* pMap = AfxGetModuleThreadState()->+0x30 (m_pmapHMENU);   // 0x133a20
//     //   if (pMap) pMap->m_permanentMap /* +0x28 */ .RemoveKey(m_hMenu);       // 0x232ec0 (unexported
//     //                                    // CMapPtrToPtr::RemoveKey: the inlined RemoveHandle)
//     //   m_hMenu = NULL; ::DestroyMenu(h); }                                     // slot 0x1802c6be0
//     return TRUE;
// The context-menu-manager global at 0x3be1b0 is the pointer the
// CContextMenuManager constructor (0x2f640 mfc140u) stores itself into when it
// is still NULL; +0x38 is slot 7 of that class's vftable (0x2e2f98 mfc140u) =
// ?TrackPopupMenu@CContextMenuManager@@UEAAIPEAUHMENU__@@HHPEAVCWnd@@H@Z.
// 0x42c0 is the string-table entry that reads "&Tabbed Document" in
// mfc140u.dll's own resources (RT_STRING block 0x42d, lang 1033).  bDrop is
// never read (its 0x50(%rbp) home slot is untouched); 0xffffff96 (-106) is the
// same private command id CPane::OnShowControlBarMenu's "tabbed document" arm
// dispatches on.  point.x is the low dword of the RDX aggregate (%edi) and
// point.y the high dword (0x44(%rbp) = the spilled home slot + 4).
// DEVIATIONS, each deliberate:
//  * OpenMFC publishes no afxContextMenuManager global (the ctor in
//    featurepack/menu/CContextMenuManager.cpp stores nothing); the gate reads
//    CWinAppEx::m_pContextMenuManager of the current app instead, which is
//    NULL until InitContextMenuManager has run -- the same "has a manager been
//    created" test for the common case, but a manager constructed outside the
//    app object is not seen.
//  * When the string resource is missing (OpenMFC ships no string table) the
//    text harvested from the retail resource is used instead of throwing:
//    retail's throw is its response to a state its own DLL never enters.
//  * The menu is a bare HMENU rather than a CMenu attached to the thread's
//    handle map (a C++ CMenu would pull ~CMenu into this object, which the
//    link audit rejects); ::DestroyMenu is called on the same path retail's
//    inlined ~CMenu does.
//  * TrackPopupMenu and TabbedDocumentToControlBar go to the DLL thunks, not
//    the objects' vtables (a client override is not reached).
//    TabbedDocumentToControlBar is still a stub in this file, so the chosen
//    command has no effect yet; the dispatch is in place for when it is not.
// Symbol: ?OnShowMDITabContextMenu@CMDIFrameWndEx@@UEAAHVCPoint@@KH@Z
extern "C" int MS_ABI impl__OnShowMDITabContextMenu_CMDIFrameWndEx__UEAAHVCPoint__KH_Z(
    CMDIFrameWndEx* pThis, long long point, unsigned long dwAllowedItems, int bDrop) {
    (void)bDrop;
    if ((dwAllowedItems & 0x10) == 0) {
        return FALSE;
    }
    CContextMenuManager* pManager = CurrentContextMenuManager();
    if (pManager == nullptr) {
        return FALSE;
    }
    HMENU hMenu = ::CreatePopupMenu();
    wchar_t szText[256];
    szText[0] = L'\0';
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(0x42c0));
    if (hInst == nullptr || ::LoadStringW(hInst, 0x42c0, szText, 256) == 0) {
        // retail: AfxThrowInvalidArgException(); see the DEVIATIONS note above
        static const wchar_t kTabbedDocument[] = L"&Tabbed Document";
        memcpy(szText, kTabbedDocument, sizeof(kTabbedDocument));
    }
    const UINT nTabbedDocumentCmd = 0xffffff96u;   // -106
    ::AppendMenuW(hMenu, MF_STRING, nTabbedDocumentCmd, szText);
    ::CheckMenuItem(hMenu, nTabbedDocumentCmd, MF_CHECKED);
    HWND hWndFrame = WndHandle(pThis);
    const int x = static_cast<int>(static_cast<unsigned long long>(point) & 0xffffffffu);
    const int y = static_cast<int>(static_cast<unsigned long long>(point) >> 32);
    UINT nCmd = impl__TrackPopupMenu_CContextMenuManager__UEAAIPEAUHMENU____HHPEAVCWnd__H_Z(
        pManager, hMenu, x, y, pThis, FALSE);
    if (::IsWindow(hWndFrame) && nCmd == nTabbedDocumentCmd) {
        CMDIChildWnd* pActive = impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(pThis, nullptr);
        if (pActive != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pActive, impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ())) {
            impl__TabbedDocumentToControlBar_CMDIFrameWndEx__UEAAHPEAVCMDIChildWndEx___Z(
                pThis, static_cast<CMDIChildWndEx*>(pActive));
        }
    }
    ::DestroyMenu(hMenu);
    return TRUE;
}
// CMDIFrameWndEx::ActiveItemRecalcLayout() — retail entry RVA 0x89b60 (an
// earlier revision of this comment cited 0x895b0, which is not this function's
// entry — 0x895b0 lies inside OnSetPreviewMode at 0x89570). Transcribed:
//     COleClientItem* pItem = this-><vtable +0x468>();   // GetInPlaceActiveItem
//     if (pItem) {
//         if (pItem->+0xa8) pItem->+0xa8-><vtable +0xb0>();
//         if (pItem->+0x98) {
//             CFrameWnd* pFrame = ((CWnd*)pItem->+0x98)->GetParentFrame(); // 0x28c680
//             if (pFrame && pFrame->+0x20c != 0)
//                 pItem->+0xa8-><vtable +0xb0>();
//         }
//     }
//     jmp this-><vtable +0x430>();       // AdjustClientArea (slot 134)
// The tail is AdjustClientArea, NOT a RecalcLayout: vtable byte offset +0x430
// is slot 134 of the retail CMDIFrameWndEx vtable at 0x2ec8f8, which holds
// ?AdjustClientArea@CMDIFrameWndEx@@UEAAXXZ (0x89880). An earlier revision of
// this function called CFrameWnd::RecalcLayout() here, which retail never does;
// that extra layout pass is removed.
// DEVIATION: the in-place-item half needs COleClientItem members (+0x98/+0xa8)
// that OpenMFC does not model, so only the AdjustClientArea tail is reproduced
// — and AdjustClientArea is itself a documented no-op stub here (see below),
// so this function currently has no observable effect.
// Symbol: ?ActiveItemRecalcLayout@CMDIFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__ActiveItemRecalcLayout_CMDIFrameWndEx__QEAAXXZ(
    CMDIFrameWndEx* pThis) {
    if (!pThis) return;
    impl__AdjustClientArea_CMDIFrameWndEx__UEAAXXZ(pThis);
}
// CMDIFrameWndEx::AddDockSite() — this export has no entry in either RVA map
// (the maps keep one name per address and this body is ICF-folded), but the
// export directory of mfc140u.dll places its ordinal 1839 at RVA 0x27d0, whose
// body is a bare `ret` (shared with ?AddDockSite@CFrameWndEx@@QEAAXXZ, the name
// the mfc140u map keeps for that address).  So the retail function does
// nothing, which matches the documented "retained for backward compatibility"
// note.  (An earlier revision of this file said exactly that -- "RVA 0x27d0 is
// a bare ret" -- and a later one removed it for not being in the symbol map;
// the export directory confirms the original reading.)  Re-verified
// 2026-09-15: ures.py -> ordinal 1839 -> RVA 0x27d0 (mfc140u), and the
// CContextMenuManager vftable at 0x2e2f98 also points three slots at it, the
// usual ICF home of every empty virtual.  The empty body below IS the retail
// body, not a placeholder.
// Symbol: ?AddDockSite@CMDIFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_CMDIFrameWndEx__QEAAXXZ(CMDIFrameWndEx* pThis) {
    (void)pThis;
}
// CMDIFrameWndEx::AddPane(CBasePane*, BOOL) — retail entry RVA 0x89690 (an
// earlier revision cited 0x890e0, which is an instruction inside OnActivateApp
// at 0x890c0, not this function's entry). Transcribed:
//     if (pBar != NULL) {
//         if (pBar->IsKindOf(RUNTIME_CLASS(CMFCRibbonBar)) &&
//             pBar-><vtable +0x690>())            this->+0x710 = pBar;
//         if (pBar->IsKindOf(RUNTIME_CLASS(CMFCRibbonStatusBar)))
//                                                 this->+0x718 = pBar;
//     }
//     return CDockingManager::AddPane(this + 0x228, pBar, bSelect, 0, 0);  // 0x1800476b0
// The two runtime classes have no RVA-map entry; their descriptors at
// 0x1802fe3c8 and 0x18030b0c0 decode to "CMFCRibbonBar" and
// "CMFCRibbonStatusBar" (an earlier revision named CMFCToolBar/CDockablePane,
// which is not what the descriptors say). this+0x710 is the same ribbon-bar
// cache OnDrawMenuImage reads.
// The docking manager and both cache slots are outside OpenMFC's
// CMDIFrameWndEx layout. Conservative FALSE.
// Symbol: ?AddPane@CMDIFrameWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_CMDIFrameWndEx__QEAAHPEAVCBasePane__H_Z(
    CMDIFrameWndEx* pThis, CBasePane* pBar, int bSelect) {
    (void)bSelect;
    if (!pThis || !pBar) return FALSE;
    // TODO(clean-room): transcribed partially — retail class-checks pBar
    // (IsKindOf CDockablePane), caches it at this+0x710/0x718 and forwards to
    // the embedded CDockingManager at this+0x228; not modeled.
    return FALSE;
}
// CMDIFrameWndEx::AdjustClientArea() — retail entry RVA 0x89880 in mfc140 and
// 0x892d0 in mfc140u (an earlier revision cited the latter and a later one
// called it wrong; both are this function's entry, in different images).
// Transcribed:
//     RECT rc = m_dockManager.+0x1c8;             // this+0x3f0, the client-area bounds
//     rc.left += m_rectBorder.left; rc.top += m_rectBorder.top;       // CFrameWnd +0x110..0x11c
//     rc.right -= m_rectBorder.right; rc.bottom -= m_rectBorder.bottom;
//     if (m_wndClientArea.m_hWnd /* this+0x760 */ != NULL)
//         m_wndClientArea.<vtable +0xe0>(&rc, 0);   // CWnd slot 28 = CalcWindowRect, read out of the
//                                                   // CMDIClientAreaWnd vtable (0x2eb4f8 mfc140 / 0x2ed5a8 mfc140u)
// Left a stub: the bounds rect is a CDockingManager member OpenMFC's class does
// not have, CFrameWnd::m_rectBorder is not a named member of OpenMFC's CFrameWnd
// (its layout differs from retail's), and the client-area sub-object is not
// constructed for OpenMFC frames (file header) -- so there is nothing to
// compute from and no window to resize.
// Re-checked 2026-09-15 against mfc140u 0x892d0: the body is exactly the
// 16-byte movups from this+0x3f0, the four +0x110..+0x11c adjustments, and the
// slot-28 call (0x7d900 = ?CalcWindowRect@CMDIClientAreaWnd@@MEAAXPEAUtagRECT@@I@Z,
// read out of the CMDIClientAreaWnd vftable at 0x2ed5a8 mfc140u) guarded on
// m_wndClientArea.m_hWnd (this+0x760).  Now that m_dockManager is reachable
// (DockManagerOf) the missing piece is the rect itself: neither OpenMFC's
// CDockingManager (m_pParentWnd + 128 bytes of padding) nor its side record
// (DockingManagerState in detail/MfccoreSupport.h) keeps the client-area
// bounds, OpenMFC's CFrameWnd keeps m_pViewActive at +0xe8 with no named
// border rect, and the client-area window is still not constructed.  STUB.
// Symbol: ?AdjustClientArea@CMDIFrameWndEx@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustClientArea_CMDIFrameWndEx__UEAAXXZ(CMDIFrameWndEx* pThis) {
    (void)pThis;
}
// CMDIFrameWndEx::AdjustDockingLayout(HDWP) — retail entry RVA 0x89800 (an
// earlier revision cited 0x89250, which is an instruction inside
// ShowWindowsDialog at 0x89220, not this function's entry).
// Transcribed:
//     CDockingManager* pMgr = this + 0x228;
//     if (pMgr->+0x250 != 0) return;             // relayout suppressed
//     pMgr-><vtable +0x80>(hdwp);                // CDockingManager::AdjustDockingLayout
//     this-><vtable +0x430>();                   // AdjustClientArea (slot 134)
//     void* p = this->+0x3d8;                    // the self-pointer OnCreate stores
//     if (p && p->+0x40 && p->+0x120)
//         this-><vtable +0x300>(TRUE);           // RecalcLayout(TRUE) (slot 96)
// (CDockingManager vtable +0x80 is slot 16 of the manager vtable at 0x1802e4070
// = ?AdjustDockingLayout@CDockingManager@@UEAAXPEAX@Z.)
// Everything here hangs off the CDockingManager at this+0x228 or the
// this+0x3d8 self-pointer, neither of which OpenMFC models, and the
// AdjustClientArea leg is itself a stub. Conservative no-op.
// Symbol: ?AdjustDockingLayout@CMDIFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CMDIFrameWndEx__UEAAXPEAX_Z(
    CMDIFrameWndEx* pThis, void* pParam) {
    (void)pParam;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — retail forwards to the embedded
    // CDockingManager (this+0x228) and then runs AdjustClientArea / RecalcLayout.
}
// CMDIFrameWndEx::ControlBarToTabbedDocument(CDockablePane*) — retail entry RVA
// 0x8a6f0 (an earlier revision cited 0x8a140, which is an instruction inside
// CreateNewWindow at 0x8a120). Retail allocates a CMDIChildWndEx
// (operator new(0x818) then ??0CMDIChildWndEx@@IEAA@XZ at 0x180083db0),
// converts the dockable pane
// into a tabbed document and reparents it into the MDI client area. The
// conversion pipeline is not modeled. Conservative NULL.
// Symbol: ?ControlBarToTabbedDocument@CMDIFrameWndEx@@UEAAPEAVCMDIChildWndEx@@PEAVCDockablePane@@@Z
extern "C" CMDIChildWndEx* MS_ABI impl__ControlBarToTabbedDocument_CMDIFrameWndEx__UEAAPEAVCMDIChildWndEx__PEAVCDockablePane___Z(
    CMDIFrameWndEx* pThis, CDockablePane* pBar) {
    (void)pBar;
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially — retail allocates a
    // CMDIChildWndEx, converts the dockable pane and reparents it into the MDI
    // client area; complex pipeline not modeled.
    return nullptr;
}
// CMDIFrameWndEx::CreateDocumentWindow(LPCTSTR, CObject*) — no entry in either
// RVA map (ICF-folded), but the export directory of mfc140u.dll places its
// ordinal 3232 at RVA 0x71e0, whose body is `xor eax,eax; ret` -- the folded
// `return NULL` that several trivial exports share.  So the retail default
// returns NULL (derived frames override it to create the tabbed-document
// child), which is what the documented behaviour says.  (An earlier revision
// of this comment cited exactly that reading and a later one dropped it for
// not being in the symbol map; the export directory confirms it.)
// Re-verified 2026-09-15: ures.py -> ordinal 3232 -> RVA 0x71e0 (mfc140u),
// `xor %eax,%eax; ret` (the mfc140u map labels that address with another of
// its folded tenants, ?AddRef@COleUILinkInfo@@UEAAKXZ).  The `return nullptr`
// below IS the retail body, not a placeholder.
// Symbol: ?CreateDocumentWindow@CMDIFrameWndEx@@UEAAPEAVCMDIChildWndEx@@PEB_WPEAVCObject@@@Z
extern "C" CMDIChildWndEx* MS_ABI impl__CreateDocumentWindow_CMDIFrameWndEx__UEAAPEAVCMDIChildWndEx__PEB_WPEAVCObject___Z(
    CMDIFrameWndEx* pThis, const wchar_t* lpcszDocName, CObject* pObj) {
    (void)pThis;
    (void)lpcszDocName;
    (void)pObj;
    return nullptr;
}
// CMDIFrameWndEx::CreateNewWindow(const wchar_t*, CObject*) — the wide export
// has no RVA; its ANSI twin
// ?CreateNewWindow@CMDIFrameWndEx@@UEAAPEAVCMDIChildWndEx@@PEBDPEAVCObject@@@Z
// is at entry RVA 0x8a120 (an earlier revision cited 0x89b70, which is an
// instruction inside ActiveItemRecalcLayout at 0x89b60). Retail returns NULL
// unless both tabbed-group flags this+0x4574 and this+0x4668 are clear, then
// goes through AfxGetModuleState() (0x1801345b0) to the current document
// template and creates the frame from it. Not modeled. Conservative NULL.
// Symbol: ?CreateNewWindow@CMDIFrameWndEx@@UEAAPEAVCMDIChildWndEx@@PEB_WPEAVCObject@@@Z
extern "C" CMDIChildWndEx* MS_ABI impl__CreateNewWindow_CMDIFrameWndEx__UEAAPEAVCMDIChildWndEx__PEB_WPEAVCObject___Z(
    CMDIFrameWndEx* pThis, const wchar_t* lpcszDocName, CObject* pObj) {
    (void)lpcszDocName;
    (void)pObj;
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially — retail allocates a
    // CMDIChildWndEx and registers it via the MDI client area; requires
    // unmodeled frame/tab state (this+0x4574/0x4668).
    return nullptr;
}
// CMDIFrameWndEx::DockPane(CBasePane*, UINT, LPCRECT) — retail entry RVA
// 0x89750 (an earlier revision cited 0x891a0, an instruction inside
// EnableWindowsDialog(UINT,UINT) at 0x89170) is a two-instruction tail jump:
// `rcx = this + 0x228; jmp 0x1800483c0` =
// ?DockPane@CDockingManager@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z, arguments
// unchanged. The docking manager is not modeled. Conservative no-op.
// Symbol: ?DockPane@CMDIFrameWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_CMDIFrameWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    CMDIFrameWndEx* pThis, CBasePane* pBar, unsigned int nDockBarID, const RECT* lpRect) {
    (void)pBar;
    (void)nDockBarID;
    (void)lpRect;
    if (!pThis) return;
    // TODO(clean-room): retail tail-jumps to the embedded CDockingManager at
    // this+0x228 (DockPane); not modeled.
}
// CMDIFrameWndEx::DockPaneLeftOf(CPane*, CPane*) — retail entry RVA 0x89060
// (an earlier revision cited 0x88ab0, an instruction inside LoadFrame at
// 0x88a50) is a two-instruction tail jump: `rcx = this + 0x228;
// jmp 0x180048280` = ?DockPaneLeftOf@CDockingManager@@QEAAHPEAVCPane@@0@Z,
// arguments unchanged. The docking manager is not modeled. Conservative FALSE.
// Symbol: ?DockPaneLeftOf@CMDIFrameWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_CMDIFrameWndEx__QEAAHPEAVCPane__0_Z(
    CMDIFrameWndEx* pThis, CPane* pBarToDock, CPane* pBar) {
    (void)pBarToDock;
    (void)pBar;
    if (!pThis) return FALSE;
    // TODO(clean-room): retail tail-jumps to the embedded CDockingManager at
    // this+0x228 (DockPaneLeftOf); not modeled.
    return FALSE;
}
// CMDIFrameWndEx::EnableAutoHidePanes(DWORD) — retail entry RVA 0x89940 (an
// earlier revision cited 0x89390, an instruction inside OnUpdateFrameMenu at
// 0x892f0)
// is a two-instruction tail jump: `rcx = this + 0x228; jmp 0x180047350` =
// ?EnableAutoHidePanes@CDockingManager@@QEAAHK@Z, argument unchanged. The
// docking manager is not modeled. Conservative FALSE.
// Symbol: ?EnableAutoHidePanes@CMDIFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_CMDIFrameWndEx__QEAAHK_Z(
    CMDIFrameWndEx* pThis, unsigned long dwStyle) {
    (void)dwStyle;
    if (!pThis) return FALSE;
    // TODO(clean-room): retail tail-jumps to the embedded CDockingManager at
    // this+0x228 (EnableAutoHidePanes); not modeled.
    return FALSE;
}
// CMDIFrameWndEx::EnableMDITabbedGroups(BOOL, const CMDITabInfo&) — retail entry
// RVA 0x894f0 (an earlier revision cited 0x88f40, an instruction inside
// OnClosePopupMenu at 0x88eb0) is a two-instruction tail
// jump: `rcx = this + 0x720; jmp 0x18007d3f0` =
// ?EnableMDITabbedGroups@CMDIClientAreaWnd@@QEAAXHAEBVCMDITabInfo@@@Z.
// The client-area window is not modeled; the CMDITabInfo
// value struct is also not declared in the headers, so the reference is taken
// opaquely. Conservative no-op.
// Symbol: ?EnableMDITabbedGroups@CMDIFrameWndEx@@QEAAXHAEBVCMDITabInfo@@@Z
extern "C" void MS_ABI impl__EnableMDITabbedGroups_CMDIFrameWndEx__QEAAXHAEBVCMDITabInfo___Z(
    CMDIFrameWndEx* pThis, int bEnable, const void* pParams) {
    (void)bEnable;
    (void)pParams;
    if (!pThis) return;
    // TODO(clean-room): retail tail-jumps to the embedded CMDIClientAreaWnd at
    // this+0x720 (EnableMDITabbedGroups); not modeled.
}
// Symbol: ?OnUpdatePaneMenu@CMDIFrameWndEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_CMDIFrameWndEx__IEAAXPEAVCCmdUI___Z(CMDIFrameWndEx*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// The C++ constructor behind ??0CMDIFrameWndEx@@IEAA@XZ (core/frame/Thunks.cpp
// placement-news it) and CreateObject.  Retail (entry RVA 0x87b90, mfc140u)
// runs ~CMDIFrameWnd's ctor, stores the vftable, zero-fills +0x1e8..+0x227 with
// +0x1fc = 1, constructs m_dockManager (default ctor, 0x46ef0), m_Impl(this)
// (0x61ef0) and m_wndClientArea (0x7c900), then initialises afxGlobalData if
// needed and, when a global at 0x3c1878 (mfc140u) is set, registers the two
// window messages 0x323 / 0x326 through 0x1caa8c.
// DEVIATIONS: the zero-fill covers the whole 128-byte padding (a superset of
// retail's); m_dockManager / m_Impl live in FrameExtra (see AttachExtra);
// m_wndClientArea is not constructed; the CString at +0x1e0 is not modeled;
// the tail-end global check and message registration were not identified and
// are not reproduced.
CMDIFrameWndEx::CMDIFrameWndEx()
    : m_pVisualManager(nullptr) {
    memset(_mdiframewndex_padding, 0, sizeof(_mdiframewndex_padding));
    Tail(this)->m_bDoSubclass = TRUE;   // retail: movq $0x1, 0x1fc(%rbx)
    (void)AttachExtra(this);
}
// Retail (entry RVA 0x87ce0, mfc140u): stores the vftable, then ~m_wndClientArea
// (0x7cb20), ~m_Impl (0x620b0), ~m_dockManager (0x472a0), releases the +0x1e0
// CString and tail-jumps ~CMDIFrameWnd (0x29cc60).  DetachExtra tears down
// the two sub-objects we own in that order.
CMDIFrameWndEx::~CMDIFrameWndEx() {
    DetachExtra(this);
}
CMFCVisualManager* CMDIFrameWndEx::GetVisualManager() const { return m_pVisualManager; }
// CMDIFrameWndEx::EnableMDITabs(BOOL, BOOL, CMFCBaseTabCtrl::Location, BOOL,
// CMFCTabCtrl::Style, BOOL, BOOL) — retail entry RVA 0x89500. Retail default-
// constructs a CMDITabInfo on the stack (call to ??0CMDITabInfo@@QEAA@XZ at
// RVA 0x7c9d0), fills it from the arguments — +0x00 = tabLocation,
// +0x04 = tabStyle, +0x08 = bTabIcons, +0x0c = bTabCloseButton,
// +0x10 = bTabCustomTooltips, +0x24 = bActiveTabCloseButton (the four stack
// arguments are read at rsp+0xa0/0xa8/0xb0/0xb8, i.e. declared parameters
// 4, 5, 6 and 7) — and tail-calls
// ?EnableMDITabs@CMDIClientAreaWnd@@QEAAXHAEBVCMDITabInfo@@@Z (RVA 0x7d2b0) on
// the embedded CMDIClientAreaWnd at this+0x720, passing bEnable and that info.
// Left a stub: OpenMFC's CMDIFrameWndEx has no CMDIClientAreaWnd sub-object,
// so there is no tab control to configure. Signature corrected from the
// generated stub's 46 placeholder parameters to the real seven.
// Symbol: ?EnableMDITabs@CMDIFrameWndEx@@QEAAXHHW4Location@CMFCBaseTabCtrl@@HW4Style@CMFCTabCtrl@@HH@Z
extern "C" void MS_ABI impl__EnableMDITabs_CMDIFrameWndEx__QEAAXHHW4Location_CMFCBaseTabCtrl__HW4Style_CMFCTabCtrl__HH_Z(
    CMDIFrameWndEx* pThis, int bEnable, int bTabIcons, int tabLocation,
    int bTabCloseButton, int tabStyle, int bTabCustomTooltips,
    int bActiveTabCloseButton) {
    (void)pThis; (void)bEnable; (void)bTabIcons; (void)tabLocation;
    (void)bTabCloseButton; (void)tabStyle; (void)bTabCustomTooltips;
    (void)bActiveTabCloseButton;
}

// CMDIFrameWndEx::EnableWindowsDialog(UINT, UINT, BOOL, BOOL) — retail entry
// RVA 0x89170. Retail loads the string resource uiMenuTextResId into a CString
// (?AfxFindStringResourceHandle@@YAPEAUHINSTANCE__@@I@Z at 0x1802accf0 ->
// CStringT::LoadString at 0x18000dc00) and forwards to the LPCTSTR overload at
// RVA 0x89100, then releases the string; if the string resource cannot be found
// or loaded it calls AfxThrowInvalidArgException (0x180225b80) instead. Left a stub for the same reason as that overload: the four fields it
// writes (this+0x1e0 CString, +0x1f0, +0x200, +0x204) are outside OpenMFC's
// CMDIFrameWndEx layout.
// Symbol: ?EnableWindowsDialog@CMDIFrameWndEx@@QEAAXIIHH@Z
extern "C" void MS_ABI impl__EnableWindowsDialog_CMDIFrameWndEx__QEAAXIIHH_Z(
    CMDIFrameWndEx* pThis, unsigned int uiMenuId, unsigned int uiMenuTextResId,
    int bShowAlways, int bShowHelpButton) {
    (void)pThis; (void)uiMenuId; (void)uiMenuTextResId;
    (void)bShowAlways; (void)bShowHelpButton;
}

// CMDIFrameWndEx::EnableWindowsDialog(UINT, LPCTSTR, BOOL, BOOL) — retail entry
// RVA 0x89100 (ANSI twin of this wide export). Transcribed:
//     if (lpszMenuText == NULL || uiMenuId == 0) AfxThrowInvalidArgException();
//     this->+0x1f0 = uiMenuId;
//     this->+0x1e0 (CString) = lpszMenuText;     // strlen + SetString
//     this->+0x200 = bShowAlways;
//     this->+0x204 = bShowHelpButton;
// Left a stub: none of those four fields exists in OpenMFC's CMDIFrameWndEx
// (CMDIFrameWnd + m_pVisualManager + a 128-byte padding blob), and adding them
// would need a header change this agent does not own — see headerRequests.
// The argument check alone is not reproduced because throwing without storing
// the settings would leave callers worse off than the no-op.
// Symbol: ?EnableWindowsDialog@CMDIFrameWndEx@@QEAAXIPEB_WHH@Z
extern "C" void MS_ABI impl__EnableWindowsDialog_CMDIFrameWndEx__QEAAXIPEB_WHH_Z(
    CMDIFrameWndEx* pThis, unsigned int uiMenuId, const wchar_t* lpszMenuText,
    int bShowAlways, int bShowHelpButton) {
    (void)pThis; (void)uiMenuId; (void)lpszMenuText;
    (void)bShowAlways; (void)bShowHelpButton;
}

// CMDIFrameWndEx::GetInPlaceActiveItem() — retail entry RVA 0x67880:
//     CFrameWnd* pFrame = this-><vtable +0x2f0>();   // GetActiveFrame
//     if (!pFrame) return NULL;
//     void* pView = pFrame->+0x170;  if (!pView) return NULL;
//     if (pView->IsKindOf(RUNTIME_CLASS(CPreviewViewEx))) return NULL;  // bails
//     void* pDoc = pView->+0xe8;     if (!pDoc) return NULL;   // m_pDocument
//     if (!pDoc->IsKindOf(RUNTIME_CLASS(COleDocument))) return NULL;
//     return pDoc-><vtable +0x230>(pView);   // COleDocument::GetInPlaceActiveItem
// (The two runtime classes have no RVA-map entry, but their descriptors decode:
// the CRuntimeClass at 0x1802f77f8 names "CPreviewViewEx" and the one at
// 0x18032c8c0 names "COleDocument" — the same reading recorded for the folded
// twin of this body in phase4/src/core/frame/CFrameWndEx.cpp.)
// (mfc140u places this export at 0x67a50, the address its map labels with the
// ICF-folded twin ?GetInPlaceActiveItem@CFrameWndEx@@.)  The chain is
// reproduced with named members: +0x170 is CFrameWnd::m_pViewActive and +0xe8
// is CView::m_pDocument, both public in OpenMFC's headers; the runtime classes
// come from their GetThisClass thunks; the final virtual goes to the
// COleDocument thunk (a generated stub that returns NULL today, so this cannot
// yet report an item -- the chain is in place for when it does).
// DEVIATION: retail dispatches GetActiveFrame and the COleDocument virtual
// through the objects' vtables; here the DLL classes' thunks are called
// directly, so a client override of either is not reached.
// Symbol: ?GetInPlaceActiveItem@CMDIFrameWndEx@@MEAAPEAVCOleClientItem@@XZ
extern "C" void* MS_ABI impl__GetInPlaceActiveItem_CMDIFrameWndEx__MEAAPEAVCOleClientItem__XZ(
    CMDIFrameWndEx* pThis) {
    if (!pThis) return nullptr;
    CFrameWnd* pFrame = impl__GetActiveFrame_CMDIFrameWnd__UEAAPEAVCFrameWnd__XZ(pThis);
    if (pFrame == nullptr) {
        return nullptr;
    }
    CWnd* pView = pFrame->m_pViewActive;
    if (pView == nullptr ||
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pView, impl__GetThisClass_CPreviewViewEx__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    CDocument* pDoc = static_cast<CView*>(pView)->m_pDocument;
    if (pDoc == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pDoc, impl__GetThisClass_COleDocument__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return impl__GetInPlaceActiveItem_COleDocument__UEAAPEAVCOleClientItem__PEAVCWnd___Z(pDoc, pView);
}

// CMDIFrameWndEx::GetPane(UINT) — retail entry RVA 0x89760 is a five-
// instruction tail jump:
//     rcx = this + 0x228;  r8d = 1;  jmp *(*rcx + 0x58)
// i.e. m_dockManager.<vtable +0x58>(nID, TRUE); slot 11 of the CDockingManager
// vtable at RVA 0x2e4070 holds
// ?FindPaneByID@CDockingManager@@UEAAPEAVCBasePane@@IH@Z (0x180047e20).
// Left a stub: OpenMFC's CMDIFrameWndEx embeds no CDockingManager and exposes
// no accessor for one (unlike CFrameWndEx, which has m_pDockingManager), so
// there is no manager to ask — see headerRequests.
// Symbol: ?GetPane@CMDIFrameWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" void* MS_ABI impl__GetPane_CMDIFrameWndEx__QEAAPEAVCBasePane__I_Z(
    CMDIFrameWndEx* pThis, unsigned int nID) {
    (void)pThis; (void)nID;
    return nullptr;
}

// CMDIFrameWndEx::GetRegisteredWithTaskBarMDIChildCount() — retail entry RVA
// 0x8ab10. Retail walks ::GetWindow(this->m_hWndMDIClient /* this+0x1d8 */,
// GW_CHILD) and then GW_HWNDNEXT, maps each HWND through CWnd::FromHandle, and
// counts those that IsKindOf RUNTIME_CLASS(CMDIChildWndEx) (the CRuntimeClass
// at 0x1802eb7d8 has no RVA-map entry, but its descriptor names
// "CMDIChildWndEx", object size 0x818) AND whose
// sub-object at child+0x720 has a non-null m_hWnd (+0x40) — i.e. the children
// whose taskbar-tab proxy window has actually been created.
// (0x8a560 in mfc140u.)  The child+0x760 test is exactly the body of
// ?IsRegisteredWithTaskbarTabs@CMDIChildWndEx@@QEAAHXZ (retail 0x83fe0 mfc140u:
// `return m_wndTaskbarTabProxy.GetSafeHwnd() != NULL`, per the decode in
// core/frame/CMDIChildWndEx.cpp), so the predicate is taken from that sibling
// thunk instead of a raw read of an offset OpenMFC's CMDIChildWndEx does not
// have.  In this tree that thunk answers FALSE for every child, so the count is
// 0 until the sibling models the proxy window; the walk itself is real.
// Symbol: ?GetRegisteredWithTaskBarMDIChildCount@CMDIFrameWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__GetRegisteredWithTaskBarMDIChildCount_CMDIFrameWndEx__QEAAHXZ(
    CMDIFrameWndEx* pThis) {
    if (!pThis) return 0;
    int nCount = 0;
    CRuntimeClass* pChildClass = impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ();
    for (HWND hWnd = ::GetWindow(pThis->m_hWndMDIClient, GW_CHILD); hWnd != nullptr; hWnd = ::GetWindow(hWnd, GW_HWNDNEXT)) {
        CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hWnd);
        if (pWnd != nullptr && impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWnd, pChildClass) &&
            impl__IsRegisteredWithTaskbarTabs_CMDIChildWndEx__QEAAHXZ(static_cast<CMDIChildWndEx*>(pWnd))) {
            ++nCount;
        }
    }
    return nCount;
}

// CMDIFrameWndEx::GetWindowMenuPopup(HMENU) — retail entry RVA 0x88a20,
// transcribed in full:
//     if (this->+0x214 != 0)          // the "frame is closing" flag OnClose sets
//         return NULL;
//     HMENU h = CMDIFrameWnd::GetWindowMenuPopup(hMenuBar);   // RVA 0x2a56a0
//     this->+0x1e8 = h;               // cached for later menu updates
//     return h;
// DEVIATION: OpenMFC's CMDIFrameWndEx models neither this+0x214 nor this+0x1e8,
// so the closing-frame early-out is not performed and the result is not cached;
// the base dispatch — the whole of the non-closing path — is reproduced.
// Symbol: ?GetWindowMenuPopup@CMDIFrameWndEx@@MEAAPEAUHMENU__@@PEAU2@@Z
extern "C" void* MS_ABI impl__GetWindowMenuPopup_CMDIFrameWndEx__MEAAPEAUHMENU____PEAU2__Z(
    CMDIFrameWndEx* pThis, void* hMenuBar) {
    if (!pThis) return nullptr;
    return impl__GetWindowMenuPopup_CMDIFrameWnd__UEAAPEAUHMENU____PEAU2__Z(pThis, hMenuBar);
}

// CMDIFrameWndEx::HtmlHelp(DWORD_PTR, UINT) — the wide export has no RVA, so
// this is transcribed from its ANSI twin ?HtmlHelpA@CMDIFrameWndEx@@MEAAX_KI@Z
// at entry RVA 0x88fc0 (four instructions):
//     if (dwData != 0 || this->+0x1f8 == 0)
//         jmp CWnd::HtmlHelp(dwData, nCmd);        // RVA 0x28b350
//     // else fall into CMDIFrameWndEx::OnContextHelp (RVA 0x89000)
// this+0x1f8 is the "inside OnContextHelp" latch that OnContextHelp itself sets
// to 1 on entry and clears on exit (see OnContextHelp below).
// DEVIATION: OpenMFC does not model this+0x1f8; it is treated as 0 (not inside
// context help), which is its value on every path other than a re-entrant call
// from OnContextHelp, so the generic CWnd dispatch is always taken.
// Symbol: ?HtmlHelpW@CMDIFrameWndEx@@MEAAX_KI@Z
extern "C" void MS_ABI impl__HtmlHelpW_CMDIFrameWndEx__MEAAX_KI_Z(
    CMDIFrameWndEx* pThis, unsigned __int64 dwData, unsigned int nCmd) {
    if (!pThis) return;
    impl__HtmlHelpW_CWnd__UEAAX_KI_Z(pThis, dwData, nCmd);
}

// CMDIFrameWndEx::InsertPane(CBasePane*, CBasePane*, BOOL) — retail entry RVA
// 0x89730 is a two-instruction tail jump: `rcx = this + 0x228; jmp 0x1800477a0`
// = ?InsertPane@CDockingManager@@QEAAHPEAVCBasePane@@0H@Z on the embedded
// docking manager, arguments unchanged.
// Left a stub for the same reason as GetPane: no CDockingManager is reachable
// from OpenMFC's CMDIFrameWndEx — see headerRequests.
// Symbol: ?InsertPane@CMDIFrameWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_CMDIFrameWndEx__QEAAHPEAVCBasePane__0H_Z(
    CMDIFrameWndEx* pThis, CBasePane* pControlBar, CBasePane* pTarget, int bAfter) {
    (void)pThis; (void)pControlBar; (void)pTarget; (void)bAfter;
    return 0;
}

// CMDIFrameWndEx::IsPointNearDockSite(CPoint, DWORD&, BOOL&) const — retail
// entry RVA 0x897f0 is a two-instruction tail jump: `rcx = this + 0x228;
// jmp 0x180048160` = ?IsPointNearDockSite@CDockingManager@@QEBAHVCPoint@@AEAKAEAH@Z,
// arguments unchanged.
// Left a stub for the same reason as GetPane/InsertPane. Note that returning 0
// also leaves the two out-parameters untouched, which retail would have written
// — callers must treat them as valid only when the result is TRUE.
// Symbol: ?IsPointNearDockSite@CMDIFrameWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_CMDIFrameWndEx__QEBAHVCPoint__AEAKAEAH_Z(
    const CMDIFrameWndEx* pThis, unsigned long long point,
    unsigned long* pdwBarAlignment, int* pbOuterEdge) {
    (void)pThis; (void)point; (void)pdwBarAlignment; (void)pbOuterEdge;
    return 0;
}

// CMDIFrameWndEx::LoadMDIState(LPCTSTR) — the wide export has no RVA; its ANSI
// twin ?LoadMDIState@CMDIFrameWndEx@@UEAAHPEBD@Z is at entry RVA 0x8a1f0 and is
// a three-instruction tail jump:
//     r8d = this->+0x218;  rcx = this + 0x720;  jmp 0x180082da0
// = ?LoadState@CMDIClientAreaWnd@@QEAAHPEBDI@Z(lpszProfileName, this->+0x218)
// on the embedded MDI client-area window.
// Left a stub returning FALSE ("nothing restored"): OpenMFC's CMDIFrameWndEx
// has no CMDIClientAreaWnd sub-object and no this+0x218 group id.
// Symbol: ?LoadMDIState@CMDIFrameWndEx@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadMDIState_CMDIFrameWndEx__UEAAHPEB_W_Z(
    CMDIFrameWndEx* pThis, const wchar_t* lpszProfileName) {
    (void)pThis; (void)lpszProfileName;
    return 0;
}

// CMDIFrameWndEx::NegotiateBorderSpace(UINT, LPRECT) — retail entry RVA
// 0x89c00, transcribed:
//     if (nBorderCmd == 1) {                       // borderGet
//         CFrameWnd::NegotiateBorderSpace(1, lpRectBorder);   // RVA 0x29e230
//         m_dockManager.<vtable +0x80>(0);   // dock mgr slot 16 =
//                 // ?AdjustDockingLayout@CDockingManager@@UEAAXPEAX@Z (0x180049930)
//         if (lpRectBorder == NULL) AfxThrowInvalidArgException();  // never returns
//         *lpRectBorder = *(RECT*)(this + 0x3f0);
//         return TRUE;
//     }
//     if (nBorderCmd == 3)                         // borderSet
//         return CFrameWnd::NegotiateBorderSpace(3, lpRectBorder);
//     return TRUE;                                 // nBorderCmd == 2 and all others
// DEVIATIONS, both on the nBorderCmd == 1 path only: the docking-manager
// virtual at +0x80 is skipped (no CDockingManager is reachable), and the border
// rect is left as CFrameWnd::NegotiateBorderSpace produced it instead of being
// overwritten from this+0x3f0, which OpenMFC does not model. The null-argument
// check and the return values are reproduced exactly.
// Symbol: ?NegotiateBorderSpace@CMDIFrameWndEx@@UEAAHIPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__NegotiateBorderSpace_CMDIFrameWndEx__UEAAHIPEAUtagRECT___Z(
    CMDIFrameWndEx* pThis, unsigned int nBorderCmd, RECT* lpRectBorder) {
    if (!pThis) return TRUE;
    if (nBorderCmd == 1) {
        impl__NegotiateBorderSpace_CFrameWnd__UEAAHIPEAUtagRECT___Z(pThis, 1, lpRectBorder);
        if (lpRectBorder == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return TRUE;
        }
        return TRUE;
    }
    if (nBorderCmd == 3) {
        return impl__NegotiateBorderSpace_CFrameWnd__UEAAHIPEAUtagRECT___Z(
            pThis, 3, lpRectBorder);
    }
    return TRUE;
}

// CMDIFrameWndEx::OnActivate(UINT, CWnd*, BOOL) — retail entry RVA 0x89070,
// transcribed:
//     CFrameWnd::OnActivate(nState, pWndOther, bMinimized);   // RVA 0x29c460
//     if (nState == WA_INACTIVE) {                            // tested first
//         if (!CMFCToolBar::m_bCustomizeMode)  // global 0x1803b70bc
//             m_Impl.DeactivateMenu();         // RVA 0x1800646e0
//     } else if (nState == WA_CLICKACTIVE) {                  // 2
//         ::UpdateWindow(this->m_hWnd);        // import slot 0x1802c5328
//     }
// DEVIATION: the WA_INACTIVE arm is skipped — it needs the CFrameImpl at
// this+0x590, which OpenMFC's CMDIFrameWndEx does not embed. (The global tested
// alongside it, 0x1803b70bc, is ?m_bCustomizeMode@CMFCToolBar@@1HA; it is only
// the guard on that unreachable call, so it is not consulted here.) The base
// dispatch and the WA_CLICKACTIVE arm are reproduced.
// Symbol: ?OnActivate@CMDIFrameWndEx@@IEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_CMDIFrameWndEx__IEAAXIPEAVCWnd__H_Z(
    CMDIFrameWndEx* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized) {
    if (!pThis) return;
    impl__OnActivate_CFrameWnd__IEAAXIPEAVCWnd__H_Z(pThis, nState, pWndOther, bMinimized);
    if (nState == 2 /* WA_CLICKACTIVE */ && pThis->m_hWnd != nullptr) {
        ::UpdateWindow(pThis->m_hWnd);
    }
}

// CMDIFrameWndEx::OnActivateApp(BOOL, DWORD) — retail entry RVA 0x890c0:
//     m_dockManager.<vtable +0xc0>(bActive);      // this+0x228, dock mgr slot
//        // 24 = ?OnActivateFrame@CDockingManager@@UEAAXH@Z (0x18004d750)
//     jmp CFrameImpl::OnActivateApp(&m_Impl, bActive);   // RVA 0x180065890
// Both callees hang off sub-objects (CDockingManager at this+0x228, CFrameImpl
// at this+0x590) that OpenMFC's CMDIFrameWndEx does not embed, so the whole
// body is unreachable and the function is left a no-op.
// Symbol: ?OnActivateApp@CMDIFrameWndEx@@IEAAXHK@Z
extern "C" void MS_ABI impl__OnActivateApp_CMDIFrameWndEx__IEAAXHK_Z(
    CMDIFrameWndEx* pThis, int bActive, unsigned long dwThreadID) {
    (void)pThis; (void)bActive; (void)dwThreadID;
}

// CMDIFrameWndEx::OnAfterTaskbarActivate(WPARAM, LPARAM) — retail entry RVA
// 0x8adb0, transcribed:
//     this-><vtable +0x3a0>(NULL);       // AdjustDockingLayout(NULL)
//     this-><vtable +0x300>(TRUE);       // RecalcLayout(TRUE)
//     CWnd::SetWindowPos(NULL, -1, -1, -1, -1, 0x37);
//     ::RedrawWindow(this->m_hWnd, NULL, NULL, 0x585);
//     m_dockManager.RedrawAllMiniFrames();          // RVA 0x18004ead0
//     if (lParam != 0 && ::IsWindow((HWND)lParam)) ::SetFocus((HWND)lParam);
//     return 0;
// The two vtable byte offsets are named from the retail CMDIFrameWndEx vtable
// at RVA 0x2ec8f8: slot 116 (+0x3a0) is AdjustDockingLayout, slot 96 (+0x300)
// is RecalcLayout. 0x37 = SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|
// SWP_FRAMECHANGED; 0x585 = RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|
// RDW_UPDATENOW|RDW_FRAME.
// DEVIATION: CDockingManager::RedrawAllMiniFrames is skipped — the manager at
// this+0x228 is not modeled. Everything else is reproduced, except that the
// AdjustDockingLayout call goes through the impl__ thunk rather than the
// object's vtable (so a derived client override is not dispatched to), and
// retail issues the SetWindowPos/RedrawWindow pair unconditionally where this
// body first checks m_hWnd.
// Symbol: ?OnAfterTaskbarActivate@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnAfterTaskbarActivate_CMDIFrameWndEx__IEAA_J_K_J_Z(
    CMDIFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    if (!pThis) return 0;
    impl__AdjustDockingLayout_CMDIFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
    pThis->RecalcLayout(TRUE);
    if (pThis->m_hWnd != nullptr) {
        ::SetWindowPos(pThis->m_hWnd, nullptr, -1, -1, -1, -1,
                       SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE |
                       SWP_FRAMECHANGED);
        ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr,
                       RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN |
                       RDW_UPDATENOW | RDW_FRAME);
    }
    HWND hWndFocus = reinterpret_cast<HWND>(static_cast<INT_PTR>(lParam));
    if (hWndFocus != nullptr && ::IsWindow(hWndFocus)) {
        ::SetFocus(hWndFocus);
    }
    return 0;
}

// CMDIFrameWndEx::OnChangeVisualManager(WPARAM, LPARAM) — retail entry RVA
// 0x8a610, five instructions:
//     CFrameImpl::OnChangeVisualManager(this + 0x590);   // RVA 0x180066700
//     return 0;
// Only the constant return is kept: the CFrameImpl sub-object at this+0x590 is
// not embedded in OpenMFC's CMDIFrameWndEx, so there is nothing to notify.
// Symbol: ?OnChangeVisualManager@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnChangeVisualManager_CMDIFrameWndEx__IEAA_J_K_J_Z(
    CMDIFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam; (void)lParam;
    return 0;
}

// CMDIFrameWndEx::OnClose() — retail entry RVA 0x88ad0, transcribed:
//     if (this->+0x220 != NULL) {                  // a CWnd* owner hook
//         ::SendMessage(this->+0x220->m_hWnd, WM_CLOSE, 0, 0);
//         this->+0x220 = NULL;
//         return;                                  // base OnClose NOT run
//     }
//     void* p = this->+0x6b0;
//     if (p == NULL || p->+0x208 == 0) {
//         this->+0x214 = 1;                        // "frame is closing" latch
//         COleClientItem* pItem = this-><vtable +0x468>();   // GetInPlaceActiveItem
//         if (pItem) pItem->Deactivate();          // RVA 0x1802474b0
//         m_Impl.OnCloseFrame();                   // RVA 0x180062010
//     }
//     HWND hWnd = this->m_hWnd;
//     CFrameWnd::OnClose();                        // RVA 0x29bf10
//     if (::IsWindow(hWnd)) this->+0x214 = 0;
// (vtable +0x468 is slot 141 of the retail CMDIFrameWndEx vtable at 0x2ec8f8 =
// CMDIFrameWndEx::GetInPlaceActiveItem.)
// DEVIATIONS: the this+0x220 owner-hook early-out, the this+0x6b0 guard, the
// this+0x214 latch and m_Impl.OnCloseFrame() all need offsets outside OpenMFC's
// CMDIFrameWndEx layout and are skipped. What remains — deactivate the in-place
// active item, then run CFrameWnd::OnClose — is the ordinary close path. Note
// that GetInPlaceActiveItem is itself a stub here (see above) and returns NULL,
// so the Deactivate call is inert until that is implemented.
// Symbol: ?OnClose@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CMDIFrameWndEx__IEAAXXZ(CMDIFrameWndEx* pThis) {
    if (!pThis) return;
    void* pItem = impl__GetInPlaceActiveItem_CMDIFrameWndEx__MEAAPEAVCOleClientItem__XZ(pThis);
    if (pItem != nullptr) {
        impl__Deactivate_COleClientItem__QEAAXXZ(pItem);
    }
    impl__OnClose_CFrameWnd__IEAAXXZ(pThis);
}

// CMDIFrameWndEx::OnCmdMsg(UINT, int, void*, AFX_CMDHANDLERINFO*) — retail
// entry RVA 0x8a4a0, transcribed:
//     if (CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))   // 0x2a43f0
//         return TRUE;
//     return CDockingManager::ProcessPaneContextMenuCommand(          // 0x4e870
//                this + 0x228, nID, nCode, pExtra, pHandlerInfo);
// DEVIATION: the docking-manager fallback is skipped — no CDockingManager is
// reachable from OpenMFC's CMDIFrameWndEx — so a pane show/hide command that
// only the manager would handle reports unhandled. The base dispatch, which is
// the first and normal path, is reproduced.
// Symbol: ?OnCmdMsg@CMDIFrameWndEx@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CMDIFrameWndEx__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CMDIFrameWndEx* pThis, unsigned int nID, int nCode, void* pExtra,
    AFX_CMDHANDLERINFO* pHandlerInfo) {
    if (!pThis) return FALSE;
    return impl__OnCmdMsg_CMDIFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
               pThis, nID, nCode, pExtra, pHandlerInfo) ? TRUE : FALSE;
}

// CMDIFrameWndEx::OnCommand(WPARAM, LPARAM) — retail entry RVA 0x88980,
// transcribed:
//     if (HIWORD(wParam) == 1) {                   // accelerator
//         UINT nID = LOWORD(wParam);
//         CMFCToolBar::m_UsageCount.AddCmd(nID);   // 0x1800233a0 on 0x1803aafd0
//         if (m_Impl.ProcessKeyboard(27, NULL))    // 0x180063540
//             return TRUE;
//         if (afxUserToolsManager != NULL &&       // global 0x1803b7110
//             afxUserToolsManager->InvokeTool(nID))// 0x1801824f0
//             return TRUE;
//     }
//     if (CMFCToolBar::m_bCustomizeMode) return FALSE;   // global 0x1803b70bc
//     return CMDIFrameWnd::OnCommand(wParam, lParam);   // RVA 0x2a4340
// The guard at 0x1803b70bc resolves in the symbol map to the exported static
// ?m_bCustomizeMode@CMFCToolBar@@1HA, whose storage exists in this tree, so it
// is reproduced rather than assumed away.
// DEVIATION: only the accelerator fast path is skipped (usage counting, the
// CFrameImpl at this+0x590, and the user-tools manager are not reachable here).
// What is left is the ordinary command routing, which retail also reaches
// whenever the accelerator path does not consume the command.
// Symbol: ?OnCommand@CMDIFrameWndEx@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMDIFrameWndEx__MEAAH_K_J_Z(
    CMDIFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return FALSE;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) {
        return FALSE;
    }
    return impl__OnCommand_CMDIFrameWnd__MEAAH_K_J_Z(
        pThis, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam));
}

// CMDIFrameWndEx::OnCompositionChanged() — retail entry RVA 0x8ad70 (mfc140;
// 0x8a7c0 mfc140u) is a two-instruction tail jump: `rcx = this + 0x590;
// jmp 0x180066ad0` = ?OnCompositionChanged@CFrameImpl@@IEAAXXZ.  The entire
// body is that one call, reproduced through the per-frame CFrameImpl token.
// Symbol: ?OnCompositionChanged@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnCompositionChanged_CMDIFrameWndEx__IEAAXXZ(
    CMDIFrameWndEx* pThis) {
    if (!pThis) return;
    impl__OnCompositionChanged_CFrameImpl__IEAAXXZ(FrameImplOf(pThis));
}

// CMDIFrameWndEx::OnContextHelp() — retail entry RVA 0x89000, transcribed:
//     this->+0x1f8 = 1;                            // "inside OnContextHelp"
//     if (this->+0x104 == 0 && CFrameWnd::CanEnterHelpMode())   // RVA 0x29fbb0
//         CMFCToolBar::SetHelpMode(TRUE);          // RVA 0x1801554b0
//     CFrameWnd::OnContextHelp();                  // RVA 0x29fc30
//     if (this->+0x104 == 0)
//         CMFCToolBar::SetHelpMode(FALSE);
//     this->+0x1f8 = 0;
// DEVIATION: this+0x104 (the frame's already-in-help-mode flag, re-read after
// the base call) and this+0x1f8 are not modeled by OpenMFC, so both tests are
// taken as "not already in help mode" — the state on entry from a normal
// WM_HELP. When the frame really is already in help mode retail skips both
// SetHelpMode calls and this version makes them; CMFCToolBar::SetHelpMode is
// itself a no-op stub in this tree today, so that difference is not yet
// observable.
// Symbol: ?OnContextHelp@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_CMDIFrameWndEx__IEAAXXZ(CMDIFrameWndEx* pThis) {
    if (!pThis) return;
    if (impl__CanEnterHelpMode_CFrameWnd__QEAAHXZ(pThis)) {
        impl__SetHelpMode_CMFCToolBar__SAXH_Z(TRUE);
    }
    impl__OnContextHelp_CFrameWnd__QEAAXXZ(pThis);
    impl__SetHelpMode_CMFCToolBar__SAXH_Z(FALSE);
}

// CMDIFrameWndEx::OnContextMenu(CWnd*, CPoint) — retail entry RVA 0x8a230.
// Retail falls straight through to CWnd::Default() (RVA 0x289090) unless the
// MDI tab control inside the embedded CMDIClientAreaWnd — this+0x720, its tab
// ctrl at +0xe8 — already has a window. It then returns without calling
// Default() when a popup menu is already up (global 0x1803b6fe8 =
// ?m_pActivePopupMenu@CMFCPopupMenu@@1PEAV1@EA) or when the primary mouse
// button is still down — ::GetSystemMetrics(SM_SWAPBUTTON) (slot 0x1802c4c50)
// selects VK_RBUTTON or VK_LBUTTON and ::GetAsyncKeyState (slot 0x1802c4c08)
// is tested for the down bit. Otherwise it compares pWnd->m_hWnd against the
// CMDIClientAreaWnd's own m_hWnd (this+0x720, +0x40): equal means the click is
// in the MDI client area, so ::WindowFromPoint + CWnd::FromHandle finds the
// child, CMDIClientAreaWnd::IsMemberOfMDITabGroup (0x180080df0) and an
// IsKindOf against RUNTIME_CLASS(CMFCTabCtrl) (descriptor at 0x180310b30) gate
// the MDI-tab context menu; different means the point is routed to the
// docking manager at this+0x228 instead.
// (An earlier revision of this comment compared against the tab control rather
// than the client-area window, and read the GetAsyncKeyState test as a
// "drop-alignment modifier"; both are corrected above from the instructions.)
// Left a stub: every branch condition and every callee lives on the two
// sub-objects OpenMFC does not embed (CMDIClientAreaWnd at this+0x720,
// CDockingManager at this+0x228). Emitting the bare CWnd::Default() tail would
// look like a transcription but would drop the whole point of the override.
// Symbol: ?OnContextMenu@CMDIFrameWndEx@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CMDIFrameWndEx__IEAAXPEAVCWnd__VCPoint___Z(
    CMDIFrameWndEx* pThis, CWnd* pWnd, unsigned long long point) {
    (void)pThis; (void)pWnd; (void)point;
}

// CMDIFrameWndEx::OnCreate(LPCREATESTRUCT) — the wide export has no RVA; this
// is transcribed from its ANSI twin
// ?OnCreate@CMDIFrameWndEx@@IEAAHPEAUtagCREATESTRUCTA@@@Z at entry RVA 0x89c80:
//     if (lpcs == NULL) AfxThrowInvalidArgException();      // never returns
//     int r = CFrameWnd::OnCreateHelper(lpcs, (CCreateContext*)lpcs->lpCreateParams);
//     if (r == -1) return -1;                              // RVA 0x29ba30
//     this->+0x3d8 = this;
//     m_Impl.+0x28 = (lpcs->style >> 23) & 1;              // WS_BORDER
//     CFrameImpl::AddFrame(this);                          // RVA 0x180065f40
//     m_Impl.OnChangeVisualManager();                      // RVA 0x180066700
//     return 0;
// DEVIATIONS: this+0x3d8 and the CFrameImpl sub-object at this+0x590 (both the
// WS_BORDER flag at m_Impl+0x28 and the OnChangeVisualManager call) are not
// modeled by OpenMFC's CMDIFrameWndEx and are skipped. The null check, the base
// OnCreateHelper call with lpCreateParams as the create context, the -1
// propagation, the static CFrameImpl::AddFrame registration and the 0 result
// are reproduced. (CFrameImpl::AddFrame is an empty stub in this tree today.)
// Symbol: ?OnCreate@CMDIFrameWndEx@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMDIFrameWndEx__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMDIFrameWndEx* pThis, CREATESTRUCTW* lpcs) {
    if (!pThis) return -1;
    if (lpcs == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    int result = impl__OnCreateHelper_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
        pThis, lpcs, static_cast<CCreateContext*>(lpcs->lpCreateParams));
    if (result == -1) return -1;
    impl__AddFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(pThis);
    return 0;
}

// CMDIFrameWndEx::OnCreateClient(LPCREATESTRUCT, CCreateContext*) — the wide
// export has no RVA; transcribed from its ANSI twin at entry RVA 0x88400:
//     if (!CMDIFrameWnd::OnCreateClient(lpcs, pContext)) return FALSE;  // 0x2a45c0
//     if (this->+0x1fc != 0)                       // MDI tabbed groups enabled
//         CWnd::SubclassWindow(this + 0x720, this->m_hWndMDIClient /* +0x1d8 */);
//     return TRUE;
// DEVIATION: the subclassing arm is skipped — this+0x1fc (the tabbed-groups
// flag) and the CMDIClientAreaWnd at this+0x720 are not modeled here — so the
// MDI client window is left unsubclassed, which is exactly retail's behaviour
// when MDI tabbed groups were never enabled.
// Symbol: ?OnCreateClient@CMDIFrameWndEx@@MEAAHPEAUtagCREATESTRUCTW@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__OnCreateClient_CMDIFrameWndEx__MEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
    CMDIFrameWndEx* pThis, CREATESTRUCTW* lpcs, CCreateContext* pContext) {
    if (!pThis) return FALSE;
    if (!impl__OnCreateClient_CMDIFrameWnd__UEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
            pThis, lpcs, pContext)) {
        return FALSE;
    }
    return TRUE;
}

// CMDIFrameWndEx::OnDestroy() — retail entry RVA 0x893b0. In order:
//   1. if (this->+0xf8) { ::DestroyAcceleratorTable(this->+0xf8); this->+0xf8 = NULL; }
//   2. this->+0x230 = 0;
//   3. two passes over the frame's child windows (::GetTopWindow then
//      ::GetWindow GW_HWNDNEXT, each HWND mapped through CWnd::FromHandle) that
//      collect into a local CList the children which are/are not IsKindOf
//      RUNTIME_CLASS(CPaneDivider) (the CRuntimeClass at 0x1802f4b58 — it has
//      no RVA-map entry, but its descriptor names "CPaneDivider"), skipping
//      the one whose HWND equals
//      this->+0x760; each collected window is then ::DestroyWindow'd if it is
//      still a window and ::GetParent still reports this frame;
//   4. CFrameImpl::RemoveFrame(this);                 // RVA 0x180065f90
//   5. CMDIFrameWnd::OnDestroy();                     // RVA 0x2a4950
// DEVIATIONS: steps 1-3 are skipped. this+0xf8, this+0x230 and this+0x760 are
// outside OpenMFC's CMDIFrameWndEx layout, and the two-pass CPaneDivider sweep
// destroys splitter windows that only the docking manager creates — none exist
// in this tree — so reproducing it would destroy the wrong children.
// Steps 4 and 5 — the registration teardown
// and the base destroy — are reproduced. (CFrameImpl::RemoveFrame is an empty
// stub in this tree today.)
// Symbol: ?OnDestroy@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMDIFrameWndEx__IEAAXXZ(CMDIFrameWndEx* pThis) {
    if (!pThis) return;
    impl__RemoveFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(pThis);
    impl__OnDestroy_CMDIFrameWnd__IEAAXXZ(pThis);
}

// CMDIFrameWndEx::OnDrawMenuImage(CDC*, const CMFCToolBarMenuButton*,
// const CRect&) — retail entry RVA 0x88f50, five instructions:
//     CMFCRibbonBar* pRibbon = this->+0x710;
//     if (pRibbon == NULL) return FALSE;
//     jmp CMFCRibbonBar::DrawMenuImage(pRibbon, pDC, pButton, rect);  // 0xdf500
// Left a stub returning FALSE. OpenMFC's CMDIFrameWndEx has no ribbon-bar
// member at this+0x710, so the ribbon can never be consulted; FALSE is also
// exactly what retail returns for a frame with no ribbon bar, which is every
// frame in this tree today.
// Symbol: ?OnDrawMenuImage@CMDIFrameWndEx@@UEAAHPEAVCDC@@PEBVCMFCToolBarMenuButton@@AEBVCRect@@@Z
extern "C" int MS_ABI impl__OnDrawMenuImage_CMDIFrameWndEx__UEAAHPEAVCDC__PEBVCMFCToolBarMenuButton__AEBVCRect___Z(
    CMDIFrameWndEx* pThis, CDC* pDC, const CMFCToolBarMenuButton* pButton,
    const CRect& rectImage) {
    (void)pThis; (void)pDC; (void)pButton; (void)rectImage;
    return FALSE;
}

// CMDIFrameWndEx::OnExitSizeMove() — retail entry RVA 0x89cf0, transcribed:
//     this-><vtable +0x300>(TRUE);            // RecalcLayout(TRUE)
//     jmp m_dockManager.<vtable +0x70>();     // this+0x228, dock mgr slot 14 =
//        // ?FixupVirtualRects@CDockingManager@@UEAAXXZ (0x1800480e0), read out
//        // of the manager vtable at RVA 0x2e4070
// The frame vtable offset is named from the retail CMDIFrameWndEx vtable at
// RVA 0x2ec8f8: slot 96 (+0x300) is CMDIFrameWndEx::RecalcLayout.
// DEVIATION: the trailing docking-manager virtual is skipped — no
// CDockingManager is reachable from OpenMFC's CMDIFrameWndEx.
// Symbol: ?OnExitSizeMove@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnExitSizeMove_CMDIFrameWndEx__IEAAXXZ(CMDIFrameWndEx* pThis) {
    if (!pThis) return;
    pThis->RecalcLayout(TRUE);
}

// CMDIFrameWndEx::OnGetMinMaxInfo(MINMAXINFO*) — retail entry RVA 0x8a0c0,
// transcribed:
//     if (this->+0x6d8 != 0) {                       // full-screen latch
//         LONG cx = m_Impl.+0x138 - m_Impl.+0x130;   // m_Impl = this + 0x590
//         lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x = cx;
//         LONG cy = m_Impl.+0x13c - m_Impl.+0x134;
//         lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y = cy;
//         return;                                    // CWnd::Default NOT called
//     }
//     CFrameImpl::OnGetMinMaxInfo(&m_Impl, lpMMI);   // RVA 0x180065300
//     jmp CWnd::Default();                           // RVA 0x289090
// DEVIATIONS: the full-screen arm cannot be taken (neither this+0x6d8 nor the
// CFrameImpl rect at m_Impl+0x130 exists in OpenMFC's layout) and the
// CFrameImpl::OnGetMinMaxInfo call is skipped for the same reason. The
// remaining CWnd::Default() is the default-frame path, which is what retail
// runs for any frame that is not in full-screen mode.
// Symbol: ?OnGetMinMaxInfo@CMDIFrameWndEx@@IEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CMDIFrameWndEx__IEAAXPEAUtagMINMAXINFO___Z(
    CMDIFrameWndEx* pThis, MINMAXINFO* lpMMI) {
    (void)lpMMI;
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIFrameWndEx::OnIdleUpdateCmdUI(WPARAM, LPARAM) — retail entry RVA
// 0x89df0, transcribed:
//     CDockingManager::SendMessageToMiniFrames(this + 0x228, 0x363, 0, 0);
//     return 0;                                      // callee RVA 0x18004d0f0
// (0x363 is MFC's private WM_IDLEUPDATECMDUI.)
// Only the constant return is kept: no CDockingManager is reachable from
// OpenMFC's CMDIFrameWndEx, so there are no mini-frames to broadcast to.
// Symbol: ?OnIdleUpdateCmdUI@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CMDIFrameWndEx__IEAA_J_K_J_Z(
    CMDIFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam; (void)lParam;
    return 0;
}

// CMDIFrameWndEx::OnLButtonDown(UINT, CPoint) — retail entry RVA 0x8a6c0,
// transcribed:
//     CFrameImpl::OnLButtonDown(this + 0x590, point);   // RVA 0x1800662c0
//     jmp CWnd::Default();                              // RVA 0x289090
// Note nFlags is not forwarded: retail passes only the point.
// DEVIATION: the CFrameImpl call is skipped (no sub-object at this+0x590 in
// OpenMFC's CMDIFrameWndEx); the unconditional CWnd::Default() tail is kept.
// Symbol: ?OnLButtonDown@CMDIFrameWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMDIFrameWndEx__IEAAXIVCPoint___Z(
    CMDIFrameWndEx* pThis, unsigned int nFlags, unsigned long long point) {
    (void)nFlags; (void)point;
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIFrameWndEx::OnLButtonUp(UINT, CPoint) — retail entry RVA 0x8a660,
// transcribed:
//     CFrameImpl::OnLButtonUp(this + 0x590, point);     // RVA 0x180066310
//     jmp CWnd::Default();                              // RVA 0x289090
// DEVIATION: as OnLButtonDown — the CFrameImpl call is skipped, the
// unconditional CWnd::Default() tail is kept.
// Symbol: ?OnLButtonUp@CMDIFrameWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMDIFrameWndEx__IEAAXIVCPoint___Z(
    CMDIFrameWndEx* pThis, unsigned int nFlags, unsigned long long point) {
    (void)nFlags; (void)point;
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIFrameWndEx::OnMenuChar(UINT, UINT, CMenu*) — retail entry RVA 0x88440,
// transcribed:
//     if (CFrameImpl::OnMenuChar(this + 0x590, nChar))  // RVA 0x180064490
//         return 0xffff0002;
//     if (this->+0x178 != NULL && nChar == 0x2d)        // '-'
//         return 0;
//     jmp CWnd::Default();                              // RVA 0x289090
// DEVIATIONS: the CFrameImpl menu-bar arm is skipped (no sub-object at
// this+0x590) and this+0x178 is not modeled, so the '-' special case cannot be
// evaluated either; both of those retail paths lead back to CWnd::Default()
// whenever their guards are false, which is the path taken here.
// Symbol: ?OnMenuChar@CMDIFrameWndEx@@IEAA_JIIPEAVCMenu@@@Z
extern "C" __int64 MS_ABI impl__OnMenuChar_CMDIFrameWndEx__IEAA_JIIPEAVCMenu___Z(
    CMDIFrameWndEx* pThis, unsigned int nChar, unsigned int nFlags, CMenu* pMenu) {
    (void)nChar; (void)nFlags; (void)pMenu;
    if (!pThis) return 0;
    return impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIFrameWndEx::OnMouseMove(UINT, CPoint) — retail entry RVA 0x8a690,
// transcribed:
//     CFrameImpl::OnMouseMove(this + 0x590, point);     // RVA 0x180066410
//     jmp CWnd::Default();                              // RVA 0x289090
// DEVIATION: as OnLButtonDown — the CFrameImpl call is skipped, the
// unconditional CWnd::Default() tail is kept.
// Symbol: ?OnMouseMove@CMDIFrameWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMDIFrameWndEx__IEAAXIVCPoint___Z(
    CMDIFrameWndEx* pThis, unsigned int nFlags, unsigned long long point) {
    (void)nFlags; (void)point;
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIFrameWndEx::OnMoveMiniFrame(CWnd*) — retail entry RVA 0x89910 is a
// four-instruction tail jump through the embedded docking manager:
//     rcx = this + 0x228;  jmp *(*rcx + 0x88)     // CDockingManager slot 17 =
//        // ?OnMoveMiniFrame@CDockingManager@@UEAAHPEAVCWnd@@@Z (0x1800490f0),
//        // read out of the manager vtable at RVA 0x2e4070
// Left a stub returning FALSE: no CDockingManager is reachable from OpenMFC's
// CMDIFrameWndEx, so there is no dock-site hit test to run — see headerRequests.
// Symbol: ?OnMoveMiniFrame@CMDIFrameWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_CMDIFrameWndEx__UEAAHPEAVCWnd___Z(
    CMDIFrameWndEx* pThis, CWnd* pFrame) {
    (void)pThis; (void)pFrame;
    return FALSE;
}

// CMDIFrameWndEx::OnNcActivate(BOOL) — retail entry RVA 0x8a570, transcribed:
//     if (CFrameImpl::OnNcActivate(this + 0x590, bActive))   // RVA 0x1800659d0
//         return TRUE;
//     return CFrameWnd::OnNcActivate(bActive);               // RVA 0x29c5b0
// DEVIATION: the CFrameImpl arm is skipped — OpenMFC's CMDIFrameWndEx embeds no
// CFrameImpl at this+0x590 — so the base dispatch, which is retail's path
// whenever the frame has no custom non-client painting, is always taken.
// Symbol: ?OnNcActivate@CMDIFrameWndEx@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CMDIFrameWndEx__IEAAHH_Z(
    CMDIFrameWndEx* pThis, int bActive) {
    if (!pThis) return TRUE;
    return impl__OnNcActivate_CFrameWnd__IEAAHH_Z(pThis, bActive);
}

// CMDIFrameWndEx::OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*) — retail entry RVA
// 0x8a630, transcribed:
//     if (!CFrameImpl::OnNcCalcSize(this + 0x590, bCalcValidRects, lpncsp))
//         CWnd::Default();                                   // RVA 0x289090
// (callee RVA 0x1800655f0; both arguments are forwarded unchanged.)
// DEVIATION: the CFrameImpl call is skipped — no sub-object at this+0x590 — and
// its result is treated as FALSE, which is what it returns for a frame with no
// custom non-client border, so CWnd::Default() runs.
// Symbol: ?OnNcCalcSize@CMDIFrameWndEx@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMDIFrameWndEx__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    CMDIFrameWndEx* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    (void)bCalcValidRects; (void)lpncsp;
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIFrameWndEx::OnNcHitTest(CPoint) — retail entry RVA 0x8a5e0, transcribed:
//     UINT n = CFrameImpl::OnNcHitTest(this + 0x590, point);   // RVA 0x180066040
//     if (n != 0) return (LRESULT)(unsigned)n;                 // zero-extended
//     jmp CWnd::Default();                                     // RVA 0x289090
// DEVIATION: the CFrameImpl hit test is skipped — no sub-object at this+0x590 —
// and its result is treated as 0 (no custom caption/border hit), so the
// CWnd::Default() tail is taken.
// Symbol: ?OnNcHitTest@CMDIFrameWndEx@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMDIFrameWndEx__IEAA_JVCPoint___Z(
    CMDIFrameWndEx* pThis, unsigned long long point) {
    (void)point;
    if (!pThis) return 0;
    return impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIFrameWndEx::OnNcMouseMove(UINT, CPoint) — retail entry RVA 0x8a5b0
// (mfc140; 0x8a000 mfc140u), transcribed in full (five instructions):
//     CFrameImpl::OnNcMouseMove(&m_Impl, nHitTest, point);   // 0x66270 mfc140
//     jmp CWnd::Default();                                   // 0x289090 mfc140
// Both arguments are forwarded unchanged.  Reproduced through the per-frame
// CFrameImpl token (see FrameExtra).
// Symbol: ?OnNcMouseMove@CMDIFrameWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CMDIFrameWndEx__IEAAXIVCPoint___Z(
    CMDIFrameWndEx* pThis, unsigned int nHitTest, unsigned long long point) {
    if (!pThis) return;
    impl__OnNcMouseMove_CFrameImpl__IEAAXIVCPoint___Z(FrameImplOf(pThis), nHitTest,
                                                       static_cast<long long>(point));
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIFrameWndEx::OnNcPaint() — retail entry RVA 0x8a500 (mfc140; 0x89f50
// mfc140u), transcribed in full:
//     if (!CFrameImpl::OnNcPaint(&m_Impl))    // 0x651d0 mfc140
//         CWnd::Default();                    // 0x289090 mfc140
// Reproduced through the per-frame CFrameImpl token.
// Symbol: ?OnNcPaint@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMDIFrameWndEx__IEAAXXZ(CMDIFrameWndEx* pThis) {
    if (!pThis) return;
    if (!impl__OnNcPaint_CFrameImpl__IEAAHXZ(FrameImplOf(pThis))) {
        (void)impl__Default_CWnd__IEAA_JXZ(pThis);
    }
}

// CMDIFrameWndEx::OnPaneCheck(UINT nID) — retail entry RVA 0x89d90 (mfc140;
// 0x897e0 mfc140u), transcribed:
//     CBasePane* pBar = m_dockManager.<vtable +0x58>(nID, TRUE);   // slot 11 =
//                 // ?FindPaneByID@CDockingManager@@UEAAPEAVCBasePane@@IH@Z
//     if (pBar == NULL) return FALSE;
//     BOOL bVisible = ::IsWindowVisible(pBar->m_hWnd);   // import slot 0x1802c5350
//     pBar-><vtable +0x458>(!bVisible, FALSE, FALSE);    // CBasePane slot 139 = ShowPane
//     return TRUE;
// (The message map at 0x2ec3f8 mfc140 routes ON_COMMAND for 0xe800, 0xe801 and
// 0xe804 -- ID_VIEW_STATUS_BAR / ID_VIEW_TOOLBAR / ID_VIEW_REBAR -- here.)
// The ShowPane virtual is dispatched by runtime class (DispatchShowPane).
// Symbol: ?OnPaneCheck@CMDIFrameWndEx@@IEAAHI@Z
extern "C" int MS_ABI impl__OnPaneCheck_CMDIFrameWndEx__IEAAHI_Z(CMDIFrameWndEx* pThis, unsigned int nID) {
    if (!pThis) return FALSE;
    CBasePane* pBar = impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(DockManagerOf(pThis), nID, TRUE);
    if (pBar == nullptr) {
        return FALSE;
    }
    BOOL bVisible = ::IsWindowVisible(WndHandle(pBar));
    DispatchShowPane(pBar, bVisible ? FALSE : TRUE, FALSE, FALSE);
    return TRUE;
}

// CMDIFrameWndEx::OnPostPreviewFrame(WPARAM, LPARAM) — this export has no entry
// in either RVA map, but the export directory of mfc140u.dll places ordinal
// 10799 (?OnPostPreviewFrame@CMDIFrameWndEx@@IEAA_J_K_J@Z) at RVA 0x71e0, and
// the CMDIFrameWndEx message map (0x2ee4a8 mfc140u; 0x2ec3f8 mfc140) routes the
// registered message AFX_WM_POSTSETPREVIEWFRAME (the global at 0x3c1618 mfc140u)
// to that same address.  The body at 0x71e0 (mfc140u) is `xor eax,eax; ret` --
// an ICF-folded `return 0`, shared with several other trivial exports.  So the
// handler does nothing but return 0; OnSetPreviewMode posts the message when the
// ribbon replaces the frame caption.
// Symbol: ?OnPostPreviewFrame@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnPostPreviewFrame_CMDIFrameWndEx__IEAA_J_K_J_Z(
    CMDIFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam; (void)lParam;
    return 0;
}

// CMDIFrameWndEx::OnPowerBroadcast(UINT nPowerEvent, LPARAM) — retail entry
// RVA 0x68b70 (mfc140; 0x68d40 mfc140u -- both images fold it with the
// CFrameWndEx handler of the same shape), transcribed:
//     LRESULT r = CWnd::Default();                     // 0x289090 mfc140
//     if (nPowerEvent == 7 /* PBT_APMRESUMESUSPEND */) {
//         if (!afxGlobalData.<+0x00>) { afxGlobalData.Initialize(); <+0x00> = 1; }
//         afxGlobalData.Resume();                      // 0x6c590 mfc140
//     }
//     return (UINT)r;
// Symbol: ?OnPowerBroadcast@CMDIFrameWndEx@@IEAAII_J@Z
extern "C" unsigned int MS_ABI impl__OnPowerBroadcast_CMDIFrameWndEx__IEAAII_J_Z(
    CMDIFrameWndEx* pThis, unsigned int nPowerEvent, __int64 lParam) {
    (void)lParam;
    if (!pThis) return 0;
    __int64 result = impl__Default_CWnd__IEAA_JXZ(pThis);
    if (nPowerEvent == 7 /* PBT_APMRESUMESUSPEND */) {
        EnsureGlobalDataInitialized();
        (void)impl__Resume_AFX_GLOBAL_DATA__QEAAHXZ();
    }
    return static_cast<unsigned int>(result);
}

// CMDIFrameWndEx::OnSetMenu(HMENU hMenu) — retail entry RVA 0x88310 (mfc140;
// 0x87d60 mfc140u), transcribed:
//     COleClientItem* pItem = this-><vtable +0x468>();          // GetInPlaceActiveItem (slot 141)
//     if (pItem != NULL && pItem->GetInPlaceWindow() != NULL)   // 0x247700 mfc140
//         return FALSE;
//     CMFCRibbonBar* pRibbon = m_Impl.+0x180;                   // this+0x710
//     if (pRibbon != NULL && ((pRibbon->GetStyle() & WS_VISIBLE) || m_Impl.+0x148 /* this+0x6d8 */)) {
//         this-><vtable +0xf0>(NULL);                            // CFrameWnd::SetMenu(CMenu*) (slot 30)
//         pRibbon->SetActiveMDIChild(MDIGetActive(NULL));        // 0xe0e70 / 0x2a4a00 mfc140
//         return TRUE;
//     }
//     if (m_Impl.+0x110 /* this+0x6a0, the CMFCMenuBar */ != NULL) {
//         this-><vtable +0xf0>(NULL);
//         pMenuBar-><vtable +0x878>(hMenu ? hMenu : m_Impl.+0x48 /* this+0x5d8 */, 0, 0);
//         return TRUE;
//     }
//     return FALSE;
// DEVIATIONS: (1) COleClientItem::GetInPlaceWindow has no definition in this
// tree, so a non-NULL in-place item is taken as "has an in-place window"
// (GetInPlaceActiveItem below cannot return non-NULL today, so the arm is
// unreachable); (2) m_Impl.+0x148 is the CFullScreenImpl full-screen flag,
// which CFrameImpl.cpp does not model -- read as FALSE; (3) the menu-bar arm is
// unreachable: CFrameImpl.cpp exposes no reader for m_Impl.+0x110 and nothing in
// the tree calls CFrameImpl::SetMenuBar, so the pointer is NULL here.
// Symbol: ?OnSetMenu@CMDIFrameWndEx@@MEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__OnSetMenu_CMDIFrameWndEx__MEAAHPEAUHMENU_____Z(CMDIFrameWndEx* pThis, HMENU hMenu) {
    (void)hMenu;   // only the (unreachable) menu-bar arm consumes it
    if (!pThis) return FALSE;
    void* pItem = impl__GetInPlaceActiveItem_CMDIFrameWndEx__MEAAPEAVCOleClientItem__XZ(pThis);
    if (pItem != nullptr) {
        return FALSE;
    }
    CMFCRibbonBar* pRibbon = RibbonBarOf(pThis);
    if (pRibbon != nullptr && (impl__GetStyle_CWnd__QEBAKXZ(pRibbon) & WS_VISIBLE) != 0) {
        impl__SetMenu_CFrameWnd__UEAAHPEAVCMenu___Z(pThis, nullptr);
        impl__SetActiveMDIChild_CMFCRibbonBar__QEAAXPEAVCWnd___Z(
            pRibbon, impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(pThis, nullptr));
        return TRUE;
    }
    return FALSE;
}

// CMDIFrameWndEx::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
// — retail entry RVA 0x89570 (mfc140; 0x88fc0 mfc140u), transcribed:
//     if (m_wndClientArea.m_bTabIsEnabled /* this+0x4574 */) {
//         m_wndClientArea.m_bTabIsVisible /* this+0x4570 */ = !bPreview;
//         m_wndClientArea.m_wndTab /* this+0x808 */.ShowWindow(bPreview ? SW_HIDE : SW_SHOWNOACTIVATE);
//     }
//     m_dockManager.SetPrintPreviewMode(bPreview, pState);   // 0x4ced0 mfc140
//     int saved = pState->+0x10;
//     CFrameWnd::OnSetPreviewMode(bPreview, pState);        // 0x29dca0 mfc140
//     pState->+0x10 = saved;
//     this-><vtable +0x3a0>(NULL);                          // AdjustDockingLayout (slot 116)
//     this-><vtable +0x300>(TRUE);                          // RecalcLayout (slot 96)
//     CMFCRibbonBar* pRibbon = m_Impl.+0x180;               // this+0x710
//     if (pRibbon && pRibbon->m_bReplaceFrameCaption /* +0x468 */)
//         ::PostMessage(m_hWnd, AFX_WM_POSTSETPREVIEWFRAME, bPreview, 0);   // slot 0x1802c52d8
// DEVIATIONS: the MDI-tab arm needs the CMDIClientAreaWnd sub-object, which is
// not constructed for OpenMFC frames (file header), and is skipped; the
// docking manager's SetPrintPreviewMode is a generated no-op stub in this
// tree, so its call is kept but has no effect.  pState->+0x10 is restored the
// way retail does it (the CFrameWnd body rewrites it).  The AdjustDockingLayout
// and RecalcLayout calls go through this file's thunks, not the vtable.
// Symbol: ?OnSetPreviewMode@CMDIFrameWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_CMDIFrameWndEx__UEAAXHPEAUCPrintPreviewState___Z(
    CMDIFrameWndEx* pThis, int bPreview, CPrintPreviewState* pState) {
    if (!pThis) return;
    impl__SetPrintPreviewMode_CDockingManager__QEAAXHPEAUCPrintPreviewState___Z(DockManagerOf(pThis), bPreview, pState);
    int saved = 0;
    if (pState != nullptr) {
        memcpy(&saved, reinterpret_cast<unsigned char*>(pState) + 0x10, sizeof(saved));
    }
    impl__OnSetPreviewMode_CFrameWnd__UEAAXHPEAUCPrintPreviewState___Z(pThis, bPreview, pState);
    if (pState != nullptr) {
        memcpy(reinterpret_cast<unsigned char*>(pState) + 0x10, &saved, sizeof(saved));
    }
    impl__AdjustDockingLayout_CMDIFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
    impl__RecalcLayout_CMDIFrameWndEx__UEAAXH_Z(pThis, TRUE);
    CMFCRibbonBar* pRibbon = RibbonBarOf(pThis);
    if (pRibbon != nullptr && pRibbon->m_bReplaceFrameCaption != 0) {
        ::PostMessage(pThis->m_hWnd, impl__AFX_WM_POSTSETPREVIEWFRAME__3IA,
                      static_cast<WPARAM>(bPreview), 0);
    }
}

// CMDIFrameWndEx::OnSetText(LPCTSTR) — the wide export sits at RVA 0x89f80
// (mfc140u, ordinal 11133 in the export directory); its ANSI twin
// ?OnSetText@CMDIFrameWndEx@@IEAAHPEBD@Z at 0x8a530 (mfc140) is the copy that
// was decoded:
//     LRESULT r = CWnd::Default();              // 0x289090 mfc140
//     CFrameImpl::OnSetText(&m_Impl, lpszText); // 0x65980 mfc140
//     return (BOOL)r;
// Reproduced through the per-frame CFrameImpl token.
// Symbol: ?OnSetText@CMDIFrameWndEx@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CMDIFrameWndEx__IEAAHPEB_W_Z(CMDIFrameWndEx* pThis, const wchar_t* lpszText) {
    if (!pThis) return 0;
    __int64 result = impl__Default_CWnd__IEAA_JXZ(pThis);
    impl__OnSetText_CFrameImpl__IEAAXPEB_W_Z(FrameImplOf(pThis), lpszText);
    return static_cast<int>(result);
}

// CMDIFrameWndEx::OnSize(UINT nType, int cx, int cy) — retail entry RVA
// 0x89e20 (mfc140; 0x89870 mfc140u), transcribed:
//     if (m_bClosing /* +0x214 */) {
//         if (nType != SIZE_MINIMIZED) this-><vtable +0x300>(TRUE);   // RecalcLayout
//         return;
//     }
//     m_bIsMinimized /* +0x210 */ = (nType == SIZE_MINIMIZED);
//     if (m_Impl.+0x180 /* ribbon bar */ != NULL ||
//         (CMFCVisualManager::GetInstance()-><vtable +0x3f8>() && m_Impl.+0x30 == 0)) {
//         RECT rc = {0}; ::GetWindowRect(m_hWnd, &rc);
//         WINDOWPOS wp; wp.x = rc.left; wp.y = rc.top; wp.cx = rc.right - rc.left;
//         wp.cy = rc.bottom - rc.top; wp.flags = SWP_FRAMECHANGED;   // hwnd/hwndInsertAfter left uninitialised
//         CFrameImpl::OnWindowPosChanging(&m_Impl, &wp);           // 0x650e0 mfc140
//     }
//     m_dockManager.<vtable +0xc0>(!m_bIsMinimized);               // OnActivateFrame (slot 24)
//     if (!m_bIsMinimized && nType != SIZE_MAXIMIZED && !m_bWasMaximized /* +0x20c */) {
//         m_dockManager.+0x10 = 1;
//         this-><vtable +0x3a0>(NULL);                             // AdjustDockingLayout
//         if (nType != SIZE_MINIMIZED) this-><vtable +0x300>(TRUE);
//         m_dockManager.+0x10 = 0;
//         BOOL bDragFull = 0; ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &bDragFull, 0);
//         if (!bDragFull) this-><vtable +0x300>(TRUE);
//     } else if (nType != SIZE_MINIMIZED) {      // reached when minimized, maximized or was-maximized
//         this-><vtable +0x300>(TRUE);
//         if (nType == SIZE_MAXIMIZED || (nType == SIZE_RESTORED && m_bWasMaximized))
//             this-><vtable +0x300>(TRUE);
//         m_bWasMaximized = (nType == SIZE_MAXIMIZED);
//     } else {
//         m_bWasMaximized = (nType == SIZE_MAXIMIZED);   // i.e. FALSE
//     }
//     CFrameImpl::UpdateCaption(&m_Impl);                          // 0x65d10 mfc140
// (0x210 / 0x20c / 0x214 are the S_CMDIFrameWndExTail members.)
// DEVIATIONS: the visual-manager predicate at +0x3f8 is the base `return 0`
// body that OpenMFC's CMFCVisualManager never overrides (see CFrameImpl.cpp),
// so the OnWindowPosChanging arm is entered only for a ribbon frame; the
// m_dockManager+0x10 flag set around the relayout has no member in OpenMFC's
// CDockingManager and is not written.  RecalcLayout / AdjustDockingLayout go
// through this file's thunks rather than the vtable.
// Symbol: ?OnSize@CMDIFrameWndEx@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMDIFrameWndEx__IEAAXIHH_Z(CMDIFrameWndEx* pThis, unsigned int nType, int cx, int cy) {
    (void)cx; (void)cy;
    if (!pThis) return;
    S_CMDIFrameWndExTail* tail = Tail(pThis);
    if (tail->m_bClosing != 0) {
        if (nType != SIZE_MINIMIZED) {
            impl__RecalcLayout_CMDIFrameWndEx__UEAAXH_Z(pThis, TRUE);
        }
        return;
    }
    tail->m_bIsMinimized = (nType == SIZE_MINIMIZED) ? TRUE : FALSE;
    void* pImpl = FrameImplOf(pThis);
    if (RibbonBarOf(pThis) != nullptr) {
        RECT rc = {0, 0, 0, 0};
        ::GetWindowRect(pThis->m_hWnd, &rc);
        WINDOWPOS wp;
        memset(&wp, 0, sizeof(wp));   // retail leaves hwnd/hwndInsertAfter uninitialised
        wp.x = rc.left;
        wp.y = rc.top;
        wp.cx = rc.right - rc.left;
        wp.cy = rc.bottom - rc.top;
        wp.flags = SWP_FRAMECHANGED;
        impl__OnWindowPosChanging_CFrameImpl__IEAAXPEAUtagWINDOWPOS___Z(pImpl, &wp);
    }
    impl__OnActivateFrame_CDockingManager__UEAAXH_Z(DockManagerOf(pThis), tail->m_bIsMinimized == 0 ? TRUE : FALSE);
    if (tail->m_bIsMinimized == 0 && nType != SIZE_MAXIMIZED && tail->m_bWasMaximized == 0) {
        impl__AdjustDockingLayout_CMDIFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
        if (nType != SIZE_MINIMIZED) {
            impl__RecalcLayout_CMDIFrameWndEx__UEAAXH_Z(pThis, TRUE);
        }
        BOOL bDragFullWindows = FALSE;
        ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &bDragFullWindows, 0);
        if (bDragFullWindows == 0) {
            impl__RecalcLayout_CMDIFrameWndEx__UEAAXH_Z(pThis, TRUE);
        }
    } else if (nType != SIZE_MINIMIZED) {
        impl__RecalcLayout_CMDIFrameWndEx__UEAAXH_Z(pThis, TRUE);
        if (nType == SIZE_MAXIMIZED || (nType == SIZE_RESTORED && tail->m_bWasMaximized != 0)) {
            impl__RecalcLayout_CMDIFrameWndEx__UEAAXH_Z(pThis, TRUE);
        }
        tail->m_bWasMaximized = (nType == SIZE_MAXIMIZED) ? TRUE : FALSE;
    } else {
        tail->m_bWasMaximized = FALSE;
    }
    impl__UpdateCaption_CFrameImpl__IEAAXXZ(pImpl);
}

// CMDIFrameWndEx::OnSysColorChange() — retail entry RVA 0x8ad80 (mfc140;
// 0x8a7d0 mfc140u), transcribed in full:
//     CWnd::OnSysColorChange();                        // 0x28d7a0 mfc140
//     CFrameImpl::OnChangeVisualManager(&m_Impl);      // 0x66700 mfc140
//     jmp ::SetWindowRgn(m_hWnd, NULL, TRUE);          // import slot 0x1802c4d90
// Symbol: ?OnSysColorChange@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMDIFrameWndEx__IEAAXXZ(CMDIFrameWndEx* pThis) {
    if (!pThis) return;
    impl__OnSysColorChange_CWnd__IEAAXXZ(pThis);
    impl__OnChangeVisualManager_CFrameImpl__QEAAXXZ(FrameImplOf(pThis));
    ::SetWindowRgn(pThis->m_hWnd, nullptr, TRUE);
}

// CMDIFrameWndEx::OnToolbarContextMenu(WPARAM, LPARAM) — no RVA-map entry, but
// the export directory of mfc140u.dll places ordinal 11371 at RVA 0x3a60 and the
// CMDIFrameWndEx message map (0x2ee4a8 mfc140u) routes the registered message
// AFX_WM_TOOLBARMENU to that same address; the body there is `mov eax,1; ret`
// (an ICF-folded `return 1`).  The mfc140 map (0x2ec3f8) shows the same: the
// AFX_WM_TOOLBARMENU entry (global 0x3bb21c) points at 0x3ae0 = `mov eax,1; ret`.
// Symbol: ?OnToolbarContextMenu@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarContextMenu_CMDIFrameWndEx__IEAA_J_K_J_Z(
    CMDIFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam; (void)lParam;
    return 1;
}

// CMDIFrameWndEx::OnToolbarDelete(WPARAM, LPARAM lParam) — retail entry RVA
// 0x88fa0 (mfc140; 0x889f0 mfc140u), transcribed in full:
//     return (LRESULT)(int)CFrameImpl::DeleteToolBar(&m_Impl, (CMFCToolBar*)lParam);  // 0x63350 mfc140
// (no argument check: retail passes lParam straight through, and the result is
// sign-extended with cltq).
// Symbol: ?OnToolbarDelete@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarDelete_CMDIFrameWndEx__IEAA_J_K_J_Z(
    CMDIFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    if (!pThis) return 0;
    return static_cast<__int64>(impl__DeleteToolBar_CFrameImpl__IEAAHPEAVCMFCToolBar___Z(
        FrameImplOf(pThis), reinterpret_cast<void*>(static_cast<INT_PTR>(lParam))));
}

// CMDIFrameWndEx::OnUpdateFrameMenu(HMENU hMenuAlt) — retail entry RVA 0x892f0
// (mfc140; 0x88d40 mfc140u), transcribed:
//     CMDIFrameWnd::OnUpdateFrameMenu(hMenuAlt);                    // 0x2a5720 mfc140
//     BOOL bMenuBar = m_Impl.+0x110 /* this+0x6a0 */ != NULL && (pMenuBar->GetStyle() & WS_VISIBLE);
//     BOOL bRibbon  = m_Impl.+0x180 /* this+0x710 */ != NULL && (pRibbon->GetStyle() & WS_VISIBLE);
//     if (bMenuBar || bRibbon) {
//         COleClientItem* pItem = this-><vtable +0x468>();          // GetInPlaceActiveItem
//         if (pItem != NULL && pItem->GetInPlaceWindow() != NULL)   // 0x247700 mfc140
//             this-><vtable +0xf0>(CMenu::FromHandle(hMenuAlt));    // CFrameWnd::SetMenu (slot 30)
//         else
//             this-><vtable +0xf0>(NULL);
//     }
// DEVIATIONS: as OnSetMenu -- the menu bar pointer is NULL in this tree (no
// reader, nothing sets it) and GetInPlaceWindow has no definition, so a
// non-NULL in-place item selects the FromHandle arm.
// Symbol: ?OnUpdateFrameMenu@CMDIFrameWndEx@@UEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__OnUpdateFrameMenu_CMDIFrameWndEx__UEAAXPEAUHMENU_____Z(CMDIFrameWndEx* pThis, HMENU hMenuAlt) {
    if (!pThis) return;
    impl__OnUpdateFrameMenu_CMDIFrameWnd__UEAAXPEAUHMENU_____Z(pThis, hMenuAlt);
    CMFCRibbonBar* pRibbon = RibbonBarOf(pThis);
    BOOL bRibbonVisible = (pRibbon != nullptr && (impl__GetStyle_CWnd__QEBAKXZ(pRibbon) & WS_VISIBLE) != 0) ? TRUE : FALSE;
    if (bRibbonVisible) {
        void* pItem = impl__GetInPlaceActiveItem_CMDIFrameWndEx__MEAAPEAVCOleClientItem__XZ(pThis);
        if (pItem != nullptr) {
            impl__SetMenu_CFrameWnd__UEAAHPEAVCMenu___Z(pThis, impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(hMenuAlt));
        } else {
            impl__SetMenu_CFrameWnd__UEAAHPEAVCMenu___Z(pThis, nullptr);
        }
    }
}

// CMDIFrameWndEx::OnWindowNew() — retail entry RVA 0x8a030 (mfc140; 0x89a80
// mfc140u), transcribed in full:
//     CMDIChildWnd* pActive = MDIGetActive(NULL);       // 0x2a4a00 mfc140
//     if (pActive == NULL) return;                      // base OnWindowNew NOT called
//     BOOL bMaximized = ::IsZoomed(pActive->m_hWnd);    // import slot 0x1802c4d78
//     if (bMaximized) pActive->ShowWindow(SW_RESTORE);  // 0x2a79e0 mfc140
//     CMDIFrameWnd::OnWindowNew();                      // 0x2a5df0 mfc140
//     ::RedrawWindow(pActive->m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_FRAME);   // 0x401
//     if (bMaximized) {
//         CMDIChildWnd* pNew = MDIGetActive(NULL);
//         if (pNew) pNew->ShowWindow(SW_MAXIMIZE);
//     }
// (The message map routes ON_COMMAND ID_WINDOW_NEW = 0xe130 here.)
// Symbol: ?OnWindowNew@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnWindowNew_CMDIFrameWndEx__IEAAXXZ(CMDIFrameWndEx* pThis) {
    if (!pThis) return;
    CMDIChildWnd* pActive = impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(pThis, nullptr);
    if (pActive == nullptr) {
        return;
    }
    BOOL bMaximized = ::IsZoomed(WndHandle(pActive));
    if (bMaximized) {
        impl__ShowWindow_CWnd__QEAAHH_Z(pActive, SW_RESTORE);
    }
    impl__OnWindowNew_CMDIFrameWnd__IEAAXXZ(pThis);
    ::RedrawWindow(WndHandle(pActive), nullptr, nullptr, RDW_INVALIDATE | RDW_FRAME);
    if (bMaximized) {
        CMDIChildWnd* pNew = impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(pThis, nullptr);
        if (pNew != nullptr) {
            impl__ShowWindow_CWnd__QEAAHH_Z(pNew, SW_MAXIMIZE);
        }
    }
}

// CMDIFrameWndEx::OnWindowPosChanged(WINDOWPOS* lpwp) — retail entry RVA
// 0x88490 (mfc140; 0x87ee0 mfc140u), transcribed:
//     if (lpwp->flags & SWP_FRAMECHANGED)
//         CFrameImpl::OnWindowPosChanging(&m_Impl, lpwp);           // 0x650e0 mfc140
//     CWnd::Default();                                              // 0x289090 mfc140
//     if (m_Impl.+0x110 /* menu bar, this+0x6a0 */ != NULL) {
//         int bMax = 0; CMDIChildWnd* p = MDIGetActive(&bMax);
//         if (p && bMax) pMenuBar->SetMaximizeMode(TRUE, p, TRUE);  // 0x8bca0 mfc140
//         else           pMenuBar->SetMaximizeMode(FALSE, NULL, TRUE);
//     }
//     if (m_Impl.+0x180 /* ribbon bar, this+0x710 */ != NULL) {
//         int bMax = 0; CMDIChildWnd* p = MDIGetActive(&bMax);
//         if (p && bMax) pRibbon->SetMaximizeMode(TRUE, p);         // 0xe0bc0 mfc140
//         else           pRibbon->SetMaximizeMode(FALSE, NULL);
//     }
// DEVIATION: the menu-bar arm is unreachable here (pointer NULL in this tree,
// see OnSetMenu); the ribbon arm and the rest are reproduced.
// Symbol: ?OnWindowPosChanged@CMDIFrameWndEx@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMDIFrameWndEx__IEAAXPEAUtagWINDOWPOS___Z(CMDIFrameWndEx* pThis, WINDOWPOS* lpwp) {
    if (!pThis) return;
    if (lpwp != nullptr && (lpwp->flags & SWP_FRAMECHANGED) != 0) {
        impl__OnWindowPosChanging_CFrameImpl__IEAAXPEAUtagWINDOWPOS___Z(FrameImplOf(pThis), lpwp);
    }
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
    CMFCRibbonBar* pRibbon = RibbonBarOf(pThis);
    if (pRibbon != nullptr) {
        int bMaximized = 0;
        CMDIChildWnd* pActive = impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(pThis, &bMaximized);
        if (pActive != nullptr && bMaximized != 0) {
            impl__SetMaximizeMode_CMFCRibbonBar__QEAAXHPEAVCWnd___Z(pRibbon, TRUE, pActive);
        } else {
            impl__SetMaximizeMode_CMFCRibbonBar__QEAAXHPEAVCWnd___Z(pRibbon, FALSE, nullptr);
        }
    }
}

// CMDIFrameWndEx::PaneFromPoint(CPoint, int, bool, CRuntimeClass*) const —
// retail entry RVA 0x89780 (mfc140; 0x891d0 mfc140u), transcribed in full:
//     return m_dockManager.<vtable +0x30>(point, nSensitivity, bExactBar, pRTCBarType, FALSE, NULL);
// slot 6 of the CDockingManager vtable (0x2e4070 mfc140) is
// ?PaneFromPoint@CDockingManager@@UEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@HPEBV2@@Z;
// the two trailing arguments are the constants retail stores at rsp+0x28 / +0x30.
// Symbol: ?PaneFromPoint@CMDIFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CMDIFrameWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(
    const CMDIFrameWndEx* pThis, unsigned long long point, int nSensitivity, bool bExactBar, CRuntimeClass* pRTCBarType) {
    if (!pThis) return nullptr;
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
        DockManagerOf(const_cast<CMDIFrameWndEx*>(pThis)),
        CPoint(static_cast<int>(static_cast<std::int32_t>(point & 0xffffffffULL)),
               static_cast<int>(static_cast<std::int32_t>(point >> 32))),
        nSensitivity, bExactBar, pRTCBarType, FALSE, nullptr);
}

// CMDIFrameWndEx::PaneFromPoint(CPoint, int, DWORD&, CRuntimeClass*) const —
// retail entry RVA 0x897c0 (mfc140; 0x89210 mfc140u), transcribed in full:
//     return m_dockManager.<vtable +0x28>(point, nSensitivity, dwAlignment, pRTCBarType, NULL);
// slot 5 of the CDockingManager vtable is
// ?PaneFromPoint@CDockingManager@@UEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@PEBV2@@Z.
// Symbol: ?PaneFromPoint@CMDIFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CMDIFrameWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(
    const CMDIFrameWndEx* pThis, unsigned long long point, int nSensitivity, unsigned long* pdwAlignment, CRuntimeClass* pRTCBarType) {
    if (!pThis) return nullptr;
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
        DockManagerOf(const_cast<CMDIFrameWndEx*>(pThis)), static_cast<long long>(point), nSensitivity, pdwAlignment,
        pRTCBarType, nullptr);
}

// CMDIFrameWndEx::PreCreateWindow(CREATESTRUCT&) — the wide export sits at RVA
// 0x885e0 (mfc140u, ordinal 11791 in the export directory); its ANSI twin
// ?PreCreateWindow@CMDIFrameWndEx@@MEAAHAEAUtagCREATESTRUCTA@@@Z at 0x88b90
// (mfc140) is the copy that was decoded, in full:
//     m_Impl.+0x120 = &m_dockManager;              // m_Impl.m_pDockManager (this+0x6b0) = this+0x228
//     CFrameImpl::RestorePosition(&m_Impl, cs);    // 0x621d0 mfc140
//     return CMDIFrameWnd::PreCreateWindow(cs);    // 0x2a48e0 mfc140
// The m_pDockManager store goes through CFrameImpl.cpp's setter.
// Symbol: ?PreCreateWindow@CMDIFrameWndEx@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMDIFrameWndEx__MEAAHAEAUtagCREATESTRUCTW___Z(CMDIFrameWndEx* pThis, CREATESTRUCTW& cs) {
    if (!pThis) return FALSE;
    void* pImpl = FrameImplOf(pThis);
    openmfc_CFrameImpl_SetDockManager(pImpl, DockManagerOf(pThis));
    impl__RestorePosition_CFrameImpl__IEAAXAEAUtagCREATESTRUCTW___Z(pImpl, &cs);
    return impl__PreCreateWindow_CMDIFrameWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
}

// CMDIFrameWndEx::PreTranslateMessage(MSG*) — retail entry RVA 0x88540 (mfc140;
// 0x87f90 mfc140u).  A switch on pMsg->message, transcribed:
//   WM_SYSKEYDOWN (0x104):
//     if (m_Impl.+0x180 /* ribbon */ && pRibbon->OnSysKeyDown(this, wParam, lParam))   // 0xe4190 mfc140
//         return TRUE;
//     /* fall into the WM_CONTEXTMENU block */
//   WM_CONTEXTMENU (0x7b):
//     EnsureGlobalDataInitialized();
//     if (afxGlobalData.+0x280 == 0) {             // m_bSysUnderlineKeyboardShortcuts (CFrameImpl.cpp's names)
//         if (afxGlobalData.+0x27c == 0) {         // m_bUnderlineKeyboardShortcuts
//             afxGlobalData.+0x27c = 1; CMFCToolBar::RedrawUnderlines();   // 0x159fd0 mfc140
//         }
//     }
//     CMFCPopupMenu* pActive = CMFCPopupMenu::GetSafeActivePopupMenu();   // 0xbc470 mfc140
//     if (pActive && wParam == VK_MENU) { ::SendMessage(pActive->m_hWnd, WM_CLOSE, 0, 0); return TRUE; }
//     if (m_Impl.ProcessKeyboard((int)wParam, NULL)) return TRUE;         // 0x63540 mfc140
//     break;
//   WM_KEYDOWN (0x100):
//     if (!CFrameImpl::IsHelpKey(pMsg) && m_Impl.ProcessKeyboard((int)wParam, NULL)) return TRUE;   // 0x64680 / 0x63540
//     if (wParam == VK_ESCAPE) {
//         if (m_Impl.+0x148 /* CFullScreenImpl at m_Impl+0x128, its +0x20 */) m_Impl.<fullscreen>.RestoreState(this);   // 0x69660
//         CSmartDockingManager* pSDM = m_dockManager.+0x308;   // this+0x530
//         if (pSDM && pSDM->+0xc && pSDM->+0x8) pSDM->CauseCancelMode();   // 0x133010
//         CWnd* pCapture = CWnd::FromHandle(::GetCapture());
//         if (pCapture && pCapture->IsKindOf(RUNTIME_CLASS(CPaneDivider)))   // descriptor 0x2f4b58 mfc140
//             { ::SendMessage(pCapture->m_hWnd, WM_CANCELMODE, 0, 0); return TRUE; }
//     }
//     break;
//   WM_NCLBUTTONDOWN/UP, WM_NCRBUTTONDOWN/UP, WM_NCMBUTTONDOWN/UP (0xa1,0xa2,0xa4,0xa5,0xa7,0xa8):
//     return m_Impl.ProcessMouseClick(message, POINT{(short)LOWORD(lParam), (short)HIWORD(lParam)}, hwnd) ? TRUE : base;  // 0x63840
//   WM_SYSKEYUP (0x105):
//     if (pRibbon && pRibbon->OnSysKeyUp(this, wParam, lParam)) return TRUE;   // 0xe4320 mfc140
//     bCtrl = ::GetKeyState(VK_CONTROL); bShift = ::GetKeyState(VK_SHIFT);
//     HIMC h = ::ImmGetContext(m_hWnd); bIme = h && ::ImmGetOpenStatus(h); if (h) ::ImmReleaseContext(m_hWnd, h);
//     if (m_Impl.+0x110 /* menu bar */ != NULL &&
//         (wParam == VK_MENU || (wParam == VK_F10 && !(bCtrl & 0x8000) && !(bShift & 0x8000) && !bIme))) {
//         CWnd* pFocus = CWnd::FromHandle(::GetFocus());
//         if (pMenuBar == pFocus) this->SetFocus();                                   // 0x2a7a70
//         else if (wParam == VK_MENU || !(lParam & (1 << 29))) pMenuBar->SetFocus();
//         return TRUE;
//     }
//     if (CMFCPopupMenu::GetSafeActivePopupMenu() != NULL) return TRUE;
//     break;
//   WM_MOUSEMOVE (0x200):
//     POINT pt = {(short)LOWORD(lParam), (short)HIWORD(lParam)};
//     CWnd* p = CWnd::FromHandle(hwnd); if (p) ::ClientToScreen(p->m_hWnd, &pt);
//     return m_Impl.ProcessMouseMove(pt) ? TRUE : base;                          // 0x63f40
//   WM_LBUTTONDOWN, WM_RBUTTONDOWN, WM_RBUTTONUP, WM_MBUTTONDOWN, WM_MBUTTONUP (0x201,0x204,0x205,0x207,0x208):
//     POINT pt = {(short)LOWORD(lParam), (short)HIWORD(lParam)};
//     CWnd* p = CWnd::FromHandle(hwnd); if (p && ::IsWindow(hwnd)) ::ClientToScreen(p->m_hWnd, &pt);
//     if (m_Impl.ProcessMouseClick(message, pt, hwnd)) return TRUE;
//     if (!::IsWindow(hwnd)) return TRUE;
//     break;
//   WM_MOUSEWHEEL (0x20a):
//     return m_Impl.ProcessMouseWheel(wParam, lParam) ? TRUE : base;              // 0x64040
//   base: return CMDIFrameWnd::PreTranslateMessage(pMsg);                       // 0x2a4750 mfc140
// DEVIATIONS: the full-screen restore (m_Impl+0x128 is not modeled by
// CFrameImpl.cpp), the smart-docking cancel (m_dockManager+0x308 has no
// OpenMFC member) and the CPaneDivider capture test (no CPaneDivider runtime
// class in this tree) are skipped on VK_ESCAPE; the menu-bar branch of
// WM_SYSKEYUP is unreachable (pointer NULL in this tree, see OnSetMenu), and
// the key-state / IME sampling that only feeds it is dropped with it.
// Symbol: ?PreTranslateMessage@CMDIFrameWndEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIFrameWndEx__UEAAHPEAUtagMSG___Z(CMDIFrameWndEx* pThis, MSG* pMsg) {
    if (!pThis) return FALSE;
    if (pMsg == nullptr) {
        return impl__PreTranslateMessage_CMDIFrameWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
    }
    void* pImpl = FrameImplOf(pThis);
    CMFCRibbonBar* pRibbon = RibbonBarOf(pThis);
    const UINT message = pMsg->message;
    switch (message) {
    case WM_SYSKEYDOWN:
    case WM_CONTEXTMENU: {
        if (message == WM_SYSKEYDOWN && pRibbon != nullptr &&
            impl__OnSysKeyDown_CMFCRibbonBar__QEAAHPEAVCFrameWnd___K_J_Z(
                pRibbon, pThis, static_cast<unsigned long long>(pMsg->wParam), static_cast<long long>(pMsg->lParam))) {
            return TRUE;
        }
        EnsureGlobalDataInitialized();
        if (GlobalDataInt(0x280) == 0) {        // m_bSysUnderlineKeyboardShortcuts
            if (GlobalDataInt(0x27c) == 0) {    // m_bUnderlineKeyboardShortcuts
                SetGlobalDataInt(0x27c, 1);
                impl__RedrawUnderlines_CMFCToolBar__SAXXZ();
            }
        }
        CMFCPopupMenu* pActive = impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
        if (pActive != nullptr && pMsg->wParam == VK_MENU) {
            ::SendMessage(WndHandle(pActive), WM_CLOSE, 0, 0);
            return TRUE;
        }
        if (impl__ProcessKeyboard_CFrameImpl__IEAAHHPEAH_Z(pImpl, static_cast<int>(pMsg->wParam), nullptr)) {
            return TRUE;
        }
        break;
    }
    case WM_KEYDOWN: {
        if (!impl__IsHelpKey_CFrameImpl__KAHPEAUtagMSG___Z(pMsg) &&
            impl__ProcessKeyboard_CFrameImpl__IEAAHHPEAH_Z(pImpl, static_cast<int>(pMsg->wParam), nullptr)) {
            return TRUE;
        }
        // VK_ESCAPE: retail's full-screen restore, smart-docking cancel and
        // CPaneDivider capture-cancel arms are not reproducible here (see the
        // deviations above); the base dispatch follows either way.
        break;
    }
    case WM_NCLBUTTONDOWN:
    case WM_NCLBUTTONUP:
    case WM_NCRBUTTONDOWN:
    case WM_NCRBUTTONUP:
    case WM_NCMBUTTONDOWN:
    case WM_NCMBUTTONUP: {
        long long pt = PackPoint(static_cast<short>(LOWORD(pMsg->lParam)), static_cast<short>(HIWORD(pMsg->lParam)));
        if (impl__ProcessMouseClick_CFrameImpl__IEAAHIUtagPOINT__PEAUHWND_____Z(pImpl, message, pt, pMsg->hwnd)) {
            return TRUE;
        }
        break;
    }
    case WM_SYSKEYUP: {
        if (pRibbon != nullptr &&
            impl__OnSysKeyUp_CMFCRibbonBar__QEAAHPEAVCFrameWnd___K_J_Z(
                pRibbon, pThis, static_cast<unsigned long long>(pMsg->wParam), static_cast<long long>(pMsg->lParam))) {
            return TRUE;
        }
        // The VK_CONTROL / VK_SHIFT key states and the IME open status retail
        // samples here feed only the menu-bar focus arm, which is unreachable in
        // this tree (m_Impl.+0x110 is NULL); they are dropped with it (imm32 is
        // not on the DLL's link line either).
        if (impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ() != nullptr) {
            return TRUE;
        }
        break;
    }
    case WM_MOUSEMOVE: {
        POINT pt = { static_cast<short>(LOWORD(pMsg->lParam)), static_cast<short>(HIWORD(pMsg->lParam)) };
        CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(pMsg->hwnd);
        if (pWnd != nullptr) {
            ::ClientToScreen(WndHandle(pWnd), &pt);
        }
        if (impl__ProcessMouseMove_CFrameImpl__IEAAHUtagPOINT___Z(pImpl, PackPoint(pt.x, pt.y))) {
            return TRUE;
        }
        break;
    }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP: {
        POINT pt = { static_cast<short>(LOWORD(pMsg->lParam)), static_cast<short>(HIWORD(pMsg->lParam)) };
        CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(pMsg->hwnd);
        if (pWnd != nullptr && ::IsWindow(pMsg->hwnd)) {
            ::ClientToScreen(WndHandle(pWnd), &pt);
        }
        if (impl__ProcessMouseClick_CFrameImpl__IEAAHIUtagPOINT__PEAUHWND_____Z(pImpl, message, PackPoint(pt.x, pt.y), pMsg->hwnd)) {
            return TRUE;
        }
        if (!::IsWindow(pMsg->hwnd)) {
            return TRUE;
        }
        break;
    }
    case WM_MOUSEWHEEL: {
        if (impl__ProcessMouseWheel_CFrameImpl__IEAAH_K_J_Z(
                pImpl, static_cast<unsigned long long>(pMsg->wParam), static_cast<long long>(pMsg->lParam))) {
            return TRUE;
        }
        break;
    }
    default:
        break;
    }
    return impl__PreTranslateMessage_CMDIFrameWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}

// CMDIFrameWndEx::RecalcLayout(BOOL bNotify) — retail entry RVA 0x89950
// (mfc140; 0x893a0 mfc140u), transcribed:
//     if (m_bInRecalcLayout /* CFrameWnd +0x1a0 */) return;
//     int nWasInPlace = m_Impl.+0x30;                    // this+0x5c0
//     m_bInRecalcLayout = 1;  m_Impl.+0x30 = 0;
//     COleClientItem* pItem = this-><vtable +0x468>();   // GetInPlaceActiveItem
//     if (pItem && pItem->+0xa8 && pItem->+0x90 == 4 /* activeUIState */) {
//         m_Impl.+0x30 = 1;  m_Impl.+0x34 /* this+0x5c4 */ = (GetStyle() & 0xc00000 /* WS_CAPTION */) != 0;
//     }
//     if (!m_bIsMinimized /* +0x210 */) {
//         CFrameWnd* pParent = m_dockManager.+0x1b0;     // this+0x3d8, the frame OnCreate stores
//         if (m_dockManager.+0x208 == 0 &&
//             (pParent == NULL || pParent->m_hWnd == NULL || pParent->+0x120 /* CFrameWnd::m_pNotifyHook */ == NULL)) {
//             m_dockManager.<vtable +0x78>(bNotify);     // CDockingManager::RecalcLayout (slot 15)
//         } else if (m_pViewActive /* +0x170 */ && m_pViewActive->IsKindOf(RUNTIME_CLASS(CPreviewViewEx))) {
//             m_dockManager.<vtable +0x78>(bNotify);
//             m_pViewActive->SetWindowPos(NULL, bounds.left, bounds.top, bounds.Width(), bounds.Height(),
//                                         SWP_NOZORDER | SWP_NOACTIVATE);   // bounds = m_dockManager.+0x1c8 (this+0x3f0)
//             goto done;
//         } else if (bNotify && pParent && pParent->m_hWnd && pParent->+0x120) {
//             ActiveItemRecalcLayout();                  // 0x89b60 mfc140
//             goto done;
//         } else {
//             m_bInRecalcLayout = 0;  CFrameWnd::RecalcLayout(bNotify);   // 0x29e090 mfc140
//         }
//         this-><vtable +0x430>();                       // AdjustClientArea (slot 134)
//     }
// done:
//     int nNow = m_Impl.+0x30;  m_bInRecalcLayout = 0;
//     if (nWasInPlace != nNow) {
//         if (m_Impl.+0x34 == 0) ModifyStyle(nNow ? WS_CAPTION : 0, nNow ? 0 : WS_CAPTION, 0);   // 0x2a7600
//         m_Impl.OnChangeVisualManager();                // 0x66700 mfc140
//         SetWindowPos(NULL, -1, -1, -1, -1, 0x37);       // 0x2a7970 mfc140
//     }
// DEVIATIONS, deliberate: (1) the in-place-item bookkeeping needs COleClientItem
// members OpenMFC does not model, and GetInPlaceActiveItem cannot return an
// item in this tree, so nWasInPlace == nNow always and the caption toggle is
// never taken; (2) m_dockManager.+0x208, the preview view's bounds rect at
// m_dockManager+0x1c8 and CFrameWnd::m_pNotifyHook are not modeled, so the
// branch cannot be chosen as retail does.  Following core/frame/CFrameWndEx.cpp
// (see its RecalcLayout), BOTH the docking manager's RecalcLayout and
// CFrameWnd::RecalcLayout are run: OpenMFC's CDockingManager does not lay the
// frame out by itself, so dropping the base call would leave the frame without
// a layout.  The re-entrancy guard is FrameExtra::bInRecalcLayout, a stand-in
// for the CFrameWnd member OpenMFC does not name.
// Symbol: ?RecalcLayout@CMDIFrameWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CMDIFrameWndEx__UEAAXH_Z(CMDIFrameWndEx* pThis, int bNotify) {
    if (!pThis) return;
    FrameExtra* extra = AttachExtra(pThis);
    if (extra == nullptr || extra->bInRecalcLayout) {
        return;
    }
    extra->bInRecalcLayout = TRUE;
    if (Tail(pThis)->m_bIsMinimized == 0) {
        impl__RecalcLayout_CDockingManager__UEAAXH_Z(reinterpret_cast<CDockingManager*>(extra->pDockManager), bNotify);
        extra->bInRecalcLayout = FALSE;
        impl__RecalcLayout_CFrameWnd__UEAAXH_Z(pThis, bNotify);
        impl__AdjustClientArea_CMDIFrameWndEx__UEAAXXZ(pThis);
    }
    extra->bInRecalcLayout = FALSE;
}

// CMDIFrameWndEx::RegisterAllMDIChildrenWithTaskbar(BOOL bRegister) — retail
// entry RVA 0x8aa00 (mfc140; 0x8a450 mfc140u), transcribed:
//     for (HWND h = ::GetWindow(m_hWndMDIClient, GW_CHILD); h; h = ::GetWindow(h, GW_HWNDNEXT)) {
//         CWnd* p = CWnd::FromHandle(h);
//         if (p && p->IsKindOf(RUNTIME_CLASS(CMDIChildWndEx))) {       // descriptor 0x2eb7d8 mfc140
//             if (bRegister) p-><vtable +0x3c0>(NULL);                 // CMDIChildWndEx slot 120 = RegisterTaskbarTab(NULL)
//             else           ((CMDIChildWndEx*)p)->UnregisterTaskbarTab(FALSE);   // 0x85b60 mfc140
//         }
//     }
//     if (bRegister) {
//         int bMax = 0; CMDIChildWnd* pActive = MDIGetActive(&bMax);
//         if (pActive && pActive->IsKindOf(RUNTIME_CLASS(CMDIChildWndEx)))
//             ((CMDIChildWndEx*)pActive)->InvalidateIconicBitmaps();   // 0x84640 mfc140
//     } else {
//         EnsureGlobalDataInitialized();
//         ITaskbarList3* p = afxGlobalData.GetITaskbarList3();         // 0x6b810 mfc140
//         if (p) p-><vtable +0xa0>(m_hWnd, NULL);                      // ITaskbarList3 slot 20 = SetTabActive(hwndTab=m_hWnd, hwndMDI=NULL, 0)
//     }
// (The slot-120 target is read out of the CMDIChildWndEx vtable at 0x2ebf68
// mfc140 / 0x2ee018 mfc140u.)  The child-side calls go to the sibling file's
// thunks; the ITaskbarList3 call is made through the COM vtable directly.
// Symbol: ?RegisterAllMDIChildrenWithTaskbar@CMDIFrameWndEx@@QEAAXH@Z
extern "C" void MS_ABI impl__RegisterAllMDIChildrenWithTaskbar_CMDIFrameWndEx__QEAAXH_Z(CMDIFrameWndEx* pThis, int bRegister) {
    if (!pThis) return;
    CRuntimeClass* pChildClass = impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ();
    for (HWND hWnd = ::GetWindow(pThis->m_hWndMDIClient, GW_CHILD); hWnd != nullptr; hWnd = ::GetWindow(hWnd, GW_HWNDNEXT)) {
        CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hWnd);
        if (pWnd == nullptr || !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWnd, pChildClass)) {
            continue;
        }
        CMDIChildWndEx* pChild = static_cast<CMDIChildWndEx*>(pWnd);
        if (bRegister) {
            impl__RegisterTaskbarTab_CMDIChildWndEx__UEAAXPEAV1__Z(pChild, nullptr);
        } else {
            impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(pChild, FALSE);
        }
    }
    if (bRegister) {
        int bMaximized = 0;
        CMDIChildWnd* pActive = impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(pThis, &bMaximized);
        if (pActive != nullptr && impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pActive, pChildClass)) {
            (void)impl__InvalidateIconicBitmaps_CMDIChildWndEx__QEAAHXZ(static_cast<CMDIChildWndEx*>(pActive));
        }
        return;
    }
    EnsureGlobalDataInitialized();
    ITaskbarList3* pTaskbar = static_cast<ITaskbarList3*>(impl__GetITaskbarList3_AFX_GLOBAL_DATA__QEAAPEAUITaskbarList3__XZ());
    if (pTaskbar != nullptr) {
        (void)pTaskbar->SetTabActive(pThis->m_hWnd, nullptr, 0);
    }
}

// CMDIFrameWndEx::RemovePaneFromDockManager(CBasePane*, BOOL, BOOL, BOOL,
// CBasePane*) — retail entry RVA 0x89740 (mfc140; 0x89190 mfc140u) is a
// two-instruction tail jump: `rcx = this + 0x228; jmp 0x180047810` =
// ?RemovePaneFromDockManager@CDockingManager@@QEAAXPEAVCBasePane@@HHH0@Z, all
// five arguments unchanged.  Forwarded to the per-frame docking manager.
// Symbol: ?RemovePaneFromDockManager@CMDIFrameWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CMDIFrameWndEx__QEAAXPEAVCBasePane__HHH0_Z(
    CMDIFrameWndEx* pThis, CBasePane* pControlBar, int bDestroy, int bAdjustLayout, int bAutoHide, CBasePane* pBarReplacement) {
    if (!pThis) return;
    impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
        DockManagerOf(pThis), pControlBar, bDestroy, bAdjustLayout, bAutoHide, pBarReplacement);
}

// CMDIFrameWndEx::SaveMDIState(LPCTSTR) — the wide export sits at RVA 0x89c60
// (mfc140u, ordinal 12639 in the export directory); its ANSI twin
// ?SaveMDIState@CMDIFrameWndEx@@UEAAHPEBD@Z at 0x8a210 (mfc140) is a
// three-instruction tail jump:
//     r8d = m_nFrameID /* +0x218 */;  rcx = this + 0x720;  jmp 0x180082b70
// = ?SaveState@CMDIClientAreaWnd@@QEAAHPEBDI@Z(lpszProfileName, m_nFrameID) on
// the embedded MDI client-area window.
// Left a stub returning FALSE ("nothing saved"): OpenMFC frames have no
// CMDIClientAreaWnd sub-object (file header), and that class's SaveState is
// itself a stub in this tree.  m_nFrameID is now modeled (S_CMDIFrameWndExTail).
// Symbol: ?SaveMDIState@CMDIFrameWndEx@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__SaveMDIState_CMDIFrameWndEx__UEAAHPEB_W_Z(CMDIFrameWndEx* pThis, const wchar_t* lpszProfileName) {
    (void)pThis; (void)lpszProfileName;
    return 0;
}

// CMDIFrameWndEx::ShowPane(CBasePane*, BOOL bShow, BOOL bDelay, BOOL bActivate)
// — no RVA-map entry, but the export directory of mfc140u.dll places ordinal
// 13833 at RVA 0x68050 (a body shared with the other frame classes' ShowPane),
// transcribed in full:
//     pBar-><vtable +0x458>(bShow, bDelay, bActivate);   // CBasePane slot 139 = ShowPane
// (bActivate is the stack argument at rsp+0x60 after the 0x30-byte frame.)
// The virtual is dispatched by runtime class (DispatchShowPane).
// Symbol: ?ShowPane@CMDIFrameWndEx@@QEAAXPEAVCBasePane@@HHH@Z
extern "C" void MS_ABI impl__ShowPane_CMDIFrameWndEx__QEAAXPEAVCBasePane__HHH_Z(
    CMDIFrameWndEx* pThis, CBasePane* pBar, int bShow, int bDelay, int bActivate) {
    (void)pThis;
    if (pBar == nullptr) return;   // retail dereferences it unconditionally
    DispatchShowPane(pBar, bShow, bDelay, bActivate);
}

// CMDIFrameWndEx::ShowPopupMenu(CMFCPopupMenu*) — retail entry RVA 0x88bd0
// (mfc140; 0x88620 mfc140u), transcribed:
//     if (!CFrameImpl::OnShowPopupMenu(&m_Impl, pMenuPopup, this)) return FALSE;   // 0x64130 mfc140
//     if (!CMFCToolBar::m_bCustomizeMode && m_hmenuWindow /* +0x1e8 */ && pMenuPopup) {
//         HMENU hMenu = pMenuPopup->+0x1658;                        // the popup's HMENU (PopupMenu::m_pMenu in detail/CMFCPopupMenuSupport.h)
//         if (hMenu) {
//             int n = ::GetMenuItemCount(hMenu), i = 0;
//             while (i < n && (::GetMenuItemID(hMenu, i) - 0xe130 /* ID_WINDOW_NEW */) > 0xf) ++i;
//             if (i < n) {                                          // the popup is the Window menu
//                 CMenu* pWinMenu = CMenu::FromHandle(m_hmenuWindow);    // 0x2a5fc0
//                 if (pWinMenu) {
//                     BOOL bFirst = TRUE, bMoreWindows = FALSE;
//                     int nWin = ::GetMenuItemCount(pWinMenu->m_hMenu);
//                     for (int j = 0; j < nWin; ++j) {
//                         UINT id = ::GetMenuItemID(pWinMenu->m_hMenu, j);
//                         if (id < 0xff00 || id > 0xfffe) continue;      // only AFX_IDM_FIRST_MDICHILD entries
//                         if (m_uiWindowsDlgMenuId && id == 0xff09) { bMoreWindows = TRUE; continue; }   // the "Windows..." entry
//                         if (bFirst) { pMenuPopup->InsertSeparator(-1); bFirst = FALSE;                 // 0xb7d50
//                                       ::SendMessage(m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0); }
//                         CString str; pWinMenu->GetMenuString(j, str, MF_BYPOSITION);                  // 0x2a60e0
//                         CMFCToolBarMenuButton btn(id, NULL, -1, str, FALSE);                          // 0x170ee0
//                         if (::GetMenuState(pWinMenu->m_hMenu, j, MF_BYPOSITION) & MF_CHECKED)
//                             btn.m_nStyle |= TBBS_CHECKED;   // bit 16 of +0x28
//                         pMenuPopup->InsertItem(btn, -1);                                             // 0xb7d00
//                     }
//                     if (m_uiWindowsDlgMenuId && (bMoreWindows || m_bShowWindowsDlgAlways /* +0x200 */)) {
//                         if (<global 0x3ab038> != NULL) CMFCToolBar::AddBasicCommand(m_uiWindowsDlgMenuId);   // 0x155600
//                         CMFCToolBarMenuButton btn(m_uiWindowsDlgMenuId, NULL, -1, m_strWindowsDlgMenuText /* +0x1e0 */, FALSE);
//                         pMenuPopup->InsertItem(btn, -1);
//                     }
//                 }
//             }
//         }
//     }
//     if (pMenuPopup != NULL && pMenuPopup->+0x1670 != 0) return TRUE;
//     return this-><vtable +0x3d8>(pMenuPopup);                     // OnShowPopupMenu (slot 123)
// DEVIATIONS: the CFrameImpl gate is a FALSE-returning stub in this tree, so
// this returns FALSE until CFrameImpl.cpp implements it (retail's gate returns
// FALSE only in customize mode; it is also what stores
// CMFCPopupMenu::m_pActivePopupMenu).  The menu text is read with
// ::GetMenuStringW into a local buffer instead of a CString; the button is
// built in a local buffer through the CMFCToolBarMenuButton ctor/dtor thunks;
// the AddBasicCommand call is made unconditionally (the global at 0x3ab038
// that gates it could not be identified); the Windows-dialog text comes from
// FrameExtra (the CString at +0x1e0 is not modeled, see EnableWindowsDialog).
// +0x1670 is a CMFCPopupMenu int that detail/CMFCPopupMenuSupport.h names
// m_nFadeX as a best-effort guess; it is read as the retail body reads it.
// Symbol: ?ShowPopupMenu@CMDIFrameWndEx@@IEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__ShowPopupMenu_CMDIFrameWndEx__IEAAHPEAVCMFCPopupMenu___Z(CMDIFrameWndEx* pThis, CMFCPopupMenu* pMenuPopup) {
    if (!pThis) return FALSE;
    if (!impl__OnShowPopupMenu_CFrameImpl__IEAAHPEAVCMFCPopupMenu__PEAVCFrameWnd___Z(FrameImplOf(pThis), pMenuPopup, pThis)) {
        return FALSE;
    }
    S_CMDIFrameWndExTail* tail = Tail(pThis);
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0 && tail->m_hmenuWindow != nullptr && pMenuPopup != nullptr) {
        HMENU hMenu = nullptr;
        memcpy(&hMenu, reinterpret_cast<const unsigned char*>(pMenuPopup) + 0x1658, sizeof(hMenu));
        if (hMenu != nullptr) {
            int nCount = ::GetMenuItemCount(hMenu);
            int i = 0;
            while (i < nCount && (::GetMenuItemID(hMenu, i) - 0xe130u) > 0xfu) {
                ++i;
            }
            CMenu* pWindowMenu = (i < nCount) ? impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(tail->m_hmenuWindow) : nullptr;
            if (pWindowMenu != nullptr) {
                BOOL bFirst = TRUE;
                BOOL bMoreWindows = FALSE;
                HMENU hWindowMenu = pWindowMenu->m_hMenu;
                int nWindows = ::GetMenuItemCount(hWindowMenu);
                for (int j = 0; j < nWindows; ++j) {
                    UINT nID = ::GetMenuItemID(hWindowMenu, j);
                    if ((nID - 0xff00u) > 0xffff00feu) {   // retail's unsigned range test: keep 0xff00..0xfffe
                        continue;
                    }
                    if (tail->m_uiWindowsDlgMenuId != 0 && nID == 0xff09) {
                        bMoreWindows = TRUE;
                        continue;
                    }
                    if (bFirst) {
                        impl__InsertSeparator_CMFCPopupMenu__QEAAHH_Z(pMenuPopup, -1);
                        bFirst = FALSE;
                        ::SendMessage(pThis->m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);
                    }
                    wchar_t szText[512];
                    szText[0] = L'\0';
                    ::GetMenuStringW(hWindowMenu, static_cast<UINT>(j), szText, 512, MF_BYPOSITION);
                    alignas(16) unsigned char buttonStorage[sizeof(CMFCToolBarMenuButton)];
                    CMFCToolBarMenuButton* pButton = static_cast<CMFCToolBarMenuButton*>(
                        impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(buttonStorage, nID, nullptr, -1, szText, FALSE));
                    if (pButton != nullptr) {
                        if ((::GetMenuState(hWindowMenu, static_cast<UINT>(j), MF_BYPOSITION) & MF_CHECKED) != 0) {
                            pButton->m_nStyle |= 0x10000u;   // TBBS_CHECKED = MAKELONG(0, TBSTATE_CHECKED)
                        }
                        impl__InsertItem_CMFCPopupMenu__QEAAHAEBVCMFCToolBarMenuButton__H_Z(pMenuPopup, pButton, -1);
                        impl___1CMFCToolBarMenuButton__UEAA_XZ(pButton);
                    }
                }
                if (tail->m_uiWindowsDlgMenuId != 0 && (bMoreWindows || tail->m_bShowWindowsDlgAlways != 0)) {
                    impl__AddBasicCommand_CMFCToolBar__SAXI_Z(tail->m_uiWindowsDlgMenuId);
                    const wchar_t* pszText = WindowsDlgMenuText(pThis);
                    alignas(16) unsigned char buttonStorage[sizeof(CMFCToolBarMenuButton)];
                    CMFCToolBarMenuButton* pButton = static_cast<CMFCToolBarMenuButton*>(
                        impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(
                            buttonStorage, tail->m_uiWindowsDlgMenuId, nullptr, -1, pszText != nullptr ? pszText : L"", FALSE));
                    if (pButton != nullptr) {
                        impl__InsertItem_CMFCPopupMenu__QEAAHAEBVCMFCToolBarMenuButton__H_Z(pMenuPopup, pButton, -1);
                        impl___1CMFCToolBarMenuButton__UEAA_XZ(pButton);
                    }
                }
            }
        }
    }
    if (pMenuPopup != nullptr) {
        int nShown = 0;
        memcpy(&nShown, reinterpret_cast<const unsigned char*>(pMenuPopup) + 0x1670, sizeof(nShown));
        if (nShown != 0) {
            return TRUE;
        }
    }
    return impl__OnShowPopupMenu_CMDIFrameWndEx__UEAAHPEAVCMFCPopupMenu___Z(pThis, pMenuPopup);
}

// CMDIFrameWndEx::ShowWindowsDialog() — retail entry RVA 0x89220 (mfc140;
// 0x88c70 mfc140u), transcribed:
//     CMFCWindowsManagerDialog dlg(this, m_bShowWindowsDlgHelpButton /* +0x204 */);   // 0x1c7e80 mfc140
//     dlg.DoModal();                                                                  // CDialog::DoModal 0x206a60, called directly
//     // ~CMFCWindowsManagerDialog, inlined: vftable 0x2e7198, two CPtrList::RemoveAll
//     // (dlg+0x260, dlg+0x228), ~CListBox (dlg+0x130), ~CDialog
// Left a stub: OpenMFC's CMFCWindowsManagerDialog constructor thunk
// (featurepack/customize/CMFCWindowsManagerDialog.cpp) is a placeholder that
// constructs nothing, so there is no dialog object to run modally; a DoModal on
// it would operate on unconstructed bytes.
// Re-checked 2026-09-15 against mfc140u 0x88c70: the callees are
// ??0CMFCWindowsManagerDialog@@QEAA@PEAVCMDIFrameWndEx@@H@Z (0x1c98a0, with
// r8d = this+0x204), ?DoModal@CDialog@@UEAA_JXZ (0x2088b0, called directly,
// not through the vtable), then the inlined destructor: the vftable at
// 0x2e9248 (mfc140u) is stored into the two list sub-objects at dlg+0x260
// and dlg+0x228 (dlg sits at rsp+0x20) before each is passed to the
// RemoveAll-shaped body at 0x8350 (zeroes +0x8..+0x20, frees the plex chain
// at +0x28), then ??1CListBox@@UEAA@XZ (0x293f30) on dlg+0x130 and
// ??1CDialog@@UEAA@XZ (0x207eb0) on dlg itself.  The +0x204 argument
// is available here (Tail(pThis)->m_bShowWindowsDlgHelpButton), but the
// dialog class is still the placeholder above and OpenMFC ships no dialog
// template for it, so nothing can be run.  STUB.
// Symbol: ?ShowWindowsDialog@CMDIFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__ShowWindowsDialog_CMDIFrameWndEx__QEAAXXZ(CMDIFrameWndEx* pThis) {
    (void)pThis;
}

// CMDIFrameWndEx::TabbedDocumentToControlBar(CMDIChildWndEx* pMDIChildWnd) —
// retail entry RVA 0x8a860 (mfc140; 0x8a2b0 mfc140u), transcribed:
//     CDockablePane* pBar = pMDIChildWnd->+0x710;              // the child's tabbed pane
//     if (pBar == NULL) return FALSE;
//     if (pBar->IsKindOf(RUNTIME_CLASS(CDockablePane))) {      // descriptor 0x3aa178 mfc140
//         if (pBar->+0x3f0 != 0) {                             // pane was docked before it became a document
//             CWnd::FromHandle(::SetParent(pBar->m_hWnd, this->m_hWnd));
//             CRect rc = pBar->+0x2c0;  pBar->+0xf8 = 0;
//             pBar-><vtable +0x408>(rc, 2 /* DM_SHOW? */, TRUE);   // CDockablePane slot 129
//         } else {
//             pBar->ShowWindow(SW_HIDE);                       // 0x2a79e0
//             CWnd::FromHandle(::SetParent(pBar->m_hWnd, this->m_hWnd));
//             pBar->+0xf8 = 0;
//             pBar-><vtable +0x6d8>();                         // CDockablePane slot 219
//         }
//     }
//     ::SendMessage(pMDIChildWnd->m_hWnd, WM_CLOSE, 0, 0);
//     return TRUE;
// Left a stub: the tabbed-pane pointer at CMDIChildWndEx+0x710 and the
// CDockablePane fields (+0x3f0, +0x2c0, +0xf8) and vtable slots it uses are
// not modeled in this tree (OpenMFC's CMDIChildWndEx is 0x228 bytes).  The
// slot targets were not resolved and are quoted by offset only.
// Symbol: ?TabbedDocumentToControlBar@CMDIFrameWndEx@@UEAAHPEAVCMDIChildWndEx@@@Z
extern "C" int MS_ABI impl__TabbedDocumentToControlBar_CMDIFrameWndEx__UEAAHPEAVCMDIChildWndEx___Z(
    CMDIFrameWndEx* pThis, CMDIChildWndEx* pMDIChildWnd) {
    (void)pThis; (void)pMDIChildWnd;
    return 0;
}

// CMDIFrameWndEx::UpdateMDITabbedBarsIcons() — retail entry RVA 0x8a970
// (mfc140; 0x8a3c0 mfc140u), transcribed:
//     for (HWND h = ::GetWindow(m_hWndMDIClient, GW_CHILD); h; h = ::GetWindow(h, GW_HWNDNEXT)) {
//         CWnd* p = CWnd::FromHandle(h);
//         if (!p || !p->IsKindOf(RUNTIME_CLASS(CMDIChildWndEx))) continue;   // descriptor 0x2eb7d8 mfc140
//         CWnd* pBar = p->+0x710;                                            // the child's tabbed pane
//         if (!pBar) continue;
//         HICON hIcon = (HICON)::SendMessage(pBar->m_hWnd, WM_GETICON, ICON_SMALL, 0);
//         ::SetClassLongPtr(h, GCLP_HICONSM /* -34 */, (LONG_PTR)hIcon);   // import slot 0x1802c52f8 (SetClassLongPtrA in mfc140)
//     }
// Re-checked 2026-09-15 against mfc140u 0x8a3c0: same body; the mfc140u import
// slots are 0x1802c6c10 (GetWindow), 0x1802c7120 (SendMessageW) and
// 0x1802c72d0 (SetClassLongPtrW), CWnd::FromHandle is 0x28ad70 and the
// descriptor 0x2ed888 decodes to "CMDIChildWndEx" (object size 0x818).  Note
// the icon is sign-extended (movslq %eax,%r8) from the 32-bit SendMessage
// result before SetClassLongPtr, i.e. retail passes (LONG_PTR)(int)hIcon.
// Left a stub: the per-child work hangs off CMDIChildWndEx+0x710, which is not
// modeled (see TabbedDocumentToControlBar); with it unreadable no child could
// be updated, and the loop would only walk the client's children for nothing.
// The sibling core/frame/CMDIChildWndEx.cpp still reports +0x710 as unmodeled
// and exports no accessor for it (the tabbed pane has no public getter in the
// export table), so there is nothing to forward to.  STUB.
// Symbol: ?UpdateMDITabbedBarsIcons@CMDIFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateMDITabbedBarsIcons_CMDIFrameWndEx__QEAAXXZ(CMDIFrameWndEx* pThis) {
    (void)pThis;
}

// CMDIFrameWndEx::WinHelp(DWORD_PTR dwData, UINT nCmd) — the wide export sits
// at RVA 0x88a30 (mfc140u, ordinal 14213 in the export directory); its ANSI
// twin ?WinHelpA@CMDIFrameWndEx@@UEAAX_KI@Z at 0x88fe0 (mfc140) is the copy
// that was decoded, four instructions:
//     if (dwData != 0 || m_bContextHelp /* +0x1f8 */ == 0)
//         jmp CWnd::WinHelp(dwData, nCmd);      // 0x28b2c0 mfc140
//     // else fall into CMDIFrameWndEx::OnContextHelp (0x89000 mfc140)
// The same shape as HtmlHelp above; m_bContextHelp is the latch OnContextHelp
// sets on entry and clears on exit, so the second arm is the re-entrant case
// (a WinHelp request raised while already inside context help).
// Symbol: ?WinHelpW@CMDIFrameWndEx@@UEAAX_KI@Z
extern "C" void MS_ABI impl__WinHelpW_CMDIFrameWndEx__UEAAX_KI_Z(CMDIFrameWndEx* pThis, unsigned __int64 dwData, unsigned int nCmd) {
    if (!pThis) return;
    if (dwData != 0 || Tail(pThis)->m_bContextHelp == 0) {
        impl__WinHelpW_CWnd__UEAAX_KI_Z(pThis, dwData, nCmd);
        return;
    }
    impl__OnContextHelp_CMDIFrameWndEx__IEAAXXZ(pThis);
}
