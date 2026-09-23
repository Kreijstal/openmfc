// CFrameImpl — OpenMFC implementation.
// Sources: cbarcore.cpp
//
// CFrameImpl is the shared implementation object that CFrameWndEx / CMDIFrameWndEx
// hang off themselves.  Behaviour below is decoded from the retail export named in
// each comment (the project's established method -- see the header of
// phase4/src/core/ole/COleControl.cpp), disassembled out of mfc140.dll.
//
// RETAIL MEMBER MAP, transcribed from the constructor ??0CFrameImpl@@QEAA@PEAVCFrameWnd@@@Z
// (RVA 0x61d20) and cross-checked against the accessors named on each line.  OpenMFC's
// CFrameImpl has no such layout: `pThis` here is an opaque token.  The fields OpenMFC
// keeps for it are the three in FrameImplState (detail/CbarcoreSupport.h) plus the
// file-local FrameImplExtra companion below, which mirrors the retail members one by
// one (each FrameImplExtra field names the retail offset it stands in for).  Retail's
// sibling classes write several of these members directly (CFrameWndEx stores its
// dock manager / ribbon bar / in-place state into m_Impl); OpenMFC's frame classes do
// not embed a CFrameImpl, so the openmfc_CFrameImpl_Set* helpers after the namespace
// are the way those writes reach this state.  Until a sibling calls them the fields
// keep their constructor values, which is exactly what a freshly constructed retail
// object holds.
//
// All RVAs below are mfc140.dll (ANSI twin) addresses unless marked (mfc140u); the
// function bodies are byte-identical between the two images.
//
//   +0x08 UINT   first user-toolbar id     ctor stores -1; written by InitUserToolbars 0x626a0
//   +0x0c UINT   last user-toolbar id      ctor stores -1; read by IsUserDefinedToolbar 0x62670
//   +0x10 UINT   default resource id       ctor stores 0; LoadDockState 0x64610 / SaveDockState
//                0x645e0 pass it as the uiID argument of the dock manager's LoadState/SaveState
//   +0x14 UINT   hot caption-button id     OnLButtonDown 0x662c0, OnTrackCaptionButtons 0x664d0
//   +0x18 UINT   pressed caption-button id OnLButtonUp 0x66310
//   +0x20 int    gate tested by OnLButtonUp/OnNcMouseMove; ctor zeroes it and no
//                CFrameImpl export writes it (purpose not established)
//   +0x24 int    written by OnWindowPosChanging 0x650e0 with the result of the visual
//                manager's vtable slot +0x400, called on the instance returned by the
//                CMFCVisualManager::GetInstance helper (0x97f4) with m_pFrame (+0x118)
//                and the packed WINDOWPOS cx/cy as ARGUMENTS -- it is NOT a virtual on
//                m_pFrame; the same shape appears in OnLButtonUp / OnMouseMove /
//                OnNcMouseMove / OnNcActivate / OnNcPaint.  The visual-manager slots
//                these bodies use, read out of the CMFCVisualManager vftable that
//                ??0CMFCVisualManager@@QEAA@H@Z (0x182640) installs (ANSI 0x319f78):
//                  +0x3f8 -> 0x7260  `xor eax,eax; ret`   a BOOL predicate taking no
//                           argument, tested before any caption-button work (the
//                           public MFC source calls it IsOwnerDrawCaption)
//                  +0x400 -> 0x18d3f0 ?OnSetWindowRegion@CMFCVisualManager@@
//                  +0x408 -> 0x2820  `ret`                 (CWnd*, BOOL): OnActivateApp
//                  +0x410 -> 0x7260  (CWnd*, const CObList&, CRect): OnNcPaint
//                  +0x418 -> 0x7260  (CWnd*, BOOL): OnNcActivate
//   +0x2c BOOL   m_pFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))  -- the descriptor at
//                0x180339bf0 carries the name "CMDIChildWnd" (read out of the image)
//   +0x30 int    a gate: non-zero makes OnActivateApp / OnNcActivate / OnNcHitTest /
//                OnMouseMove / OnNcMouseMove / OnLButtonUp / OnWindowPosChanging return
//                without doing anything.  ?RecalcLayout@CFrameWndEx@@UEAAXH@Z (0x67fb0)
//                zeroes it at 0x67fe8 and sets it to 1 at 0x6801a when the active view
//                (frame vtable +0x3a8) has a non-null object at its +0xa8 whose +0x90
//                reads 4 -- the public MFC source's "OLE item in-place active" test
//                (COleClientItem::activeUIState == 4); at 0x68030 it then stores
//                (GetStyle() & WS_CAPTION) != 0 into +0x34.
//   +0x34 int    ctor stores 1; see +0x30
//   +0x38        ctor stores 1 with an 8-byte store; LoadDockState 0x64610 tests its
//                low dword and skips the forward when it is zero
//   +0x40 int    re-entrancy flag set/cleared inside OnWindowPosChanging 0x650e0
//   +0x50..0x5f  CRect, the caption-button repaint rect (RedrawCaptionButton 0x66660)
//   +0x60 CPtrList m_lstUserToolbars    (head +0x68, count +0x78, block size 10)
//                DeleteToolBar 0x63350, GetUserToolBarByIndex 0x62640, CreateNewToolBar 0x62820
//   +0x98 CPtrList tear-off toolbars    (head +0xa0)
//                AddTearOffToolbar 0x62a40, RemoveTearOffToolbar 0x62a50, LoadTearOffMenus 0x62a90
//   +0xd0 CPtrList caption ("system") buttons (head +0xd8)   GetSysButton 0x66480
//   +0x108 CString  user-toolbar profile name; InitUserToolbars 0x626a0 fills it and
//                LoadUserToolbars 0x62490 passes it to CMFCToolBar::LoadState
//   +0x110 CMFCMenuBar*   written by SetMenuBar 0x63480
//   +0x118 CFrameWnd*     m_pFrame -- the constructor argument
//   +0x120 CDockingManager*  LoadUserToolbars 0x62490 passes it to
//                ?DockPane@CDockingManager@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z (0x483c0)
//   +0x128..0x177 CFullScreenImpl sub-object: the ctor calls ??0CFullScreenImpl (0x68cf0)
//                with (this+0x128, this).  Its +0x20 (= this+0x148) is m_bFullScreen per
//                detail/CFullScreenImplSupport.h; OnNcPaint 0x651d0 / OnNcCalcSize 0x655f0 /
//                OnGetMinMaxInfo 0x65300 test it.  OpenMFC's CFrameImpl has no such
//                sub-object, so those tests read the flag as FALSE here.
//   +0x178 CRuntimeClass* for newly created user toolbars; ctor default is the descriptor
//                at 0x1803aa5f8, whose name reads "CMFCToolBar"; SetNewUserToolBarRTC 0x62800
//   +0x180 CMFCRibbonBar* -- proven by DeactivateMenu 0x646e0 / OnActivateApp 0x65890
//                passing it as `this` to ?DeactivateKeyboardFocus@CMFCRibbonBar@@ (0xe28e0),
//                ?HideKeyTips@CMFCRibbonBar@@ (0xe4050) and ?OnCancelMode@CMFCRibbonBar@@ (0xdcf20).
//                The bodies below read its +0x468 (m_bReplaceFrameCaption, 1128 in
//                include/openmfc/afxmfc.h) and +0x448 (m_bIsTransparentCaption, 1096).
//   +0x188 CWnd*  ctor zeroes it; OnNcActivate 0x659d0 redraws its m_hWnd (+0x40) with
//                RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW.  Nothing in this class names it
//                further; the public MFC source keeps the ribbon status bar here.

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
// For the CObList side table: OpenMFC's list wrappers keep their contents in a
// per-object state map (OPENMFC_DECLARE_LIST_WRAPPER, include/openmfc/afx.h), not in
// the object's own bytes.  FindListStorage below is the header-only accessor for it.
#include "detail/FilecoreSupport.h"

// ---------------------------------------------------------------------------
// Thunks for DLL-internal calls (see the briefing's rule 1: most C++ methods in
// this DLL exist only as extern "C" impl__ thunks).
// ---------------------------------------------------------------------------
extern "C" HWND MS_ABI impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(const CWnd* pThis);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis,
                                                                        const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI
impl__GetRegSectionPath_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    CString* ret, CWinAppEx* pThis, const wchar_t* add);
extern "C" int MS_ABI impl__LoadLargeIconsState_CMFCToolBar__SAHPEB_W_Z(const wchar_t* lpszProfileName);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIChildWnd__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// CObList (core/collections/CObList.cpp)
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(CObList* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CObList__UEAA_XZ(CObList* pThis);
extern "C" CObList::POSITION MS_ABI impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CObList* pThis, CObject* value);
// CDockingManager (featurepack/docking/CDockingManager.cpp, Thunks.cpp)
extern "C" int MS_ABI impl__LoadState_CDockingManager__UEAAHPEB_WI_Z(CDockingManager* pThis, const wchar_t* lpszProfileName, unsigned int uiID);
extern "C" int MS_ABI impl__SaveState_CDockingManager__UEAAHPEB_WI_Z(CDockingManager* pThis, const wchar_t* lpszProfileName, unsigned int uiID);
extern "C" void MS_ABI impl__SetDockState_CDockingManager__UEAAXXZ(CDockingManager* pThis);
extern "C" std::int32_t impl__m_bDisableRecalcLayout_CDockingManager__2HA;
// CMFCRibbonBar (featurepack/ribbon/CMFCRibbonBar.cpp)
extern "C" void MS_ABI impl__HideKeyTips_CMFCRibbonBar__QEAAXXZ(CMFCRibbonBar* pThis);
extern "C" void MS_ABI impl__OnCancelMode_CMFCRibbonBar__IEAAXXZ(CMFCRibbonBar* pThis);
extern "C" void MS_ABI impl__DeactivateKeyboardFocus_CMFCRibbonBar__QEAAXH_Z(CMFCRibbonBar* pThis, int bSetFocus);
extern "C" void MS_ABI impl__DWMCompositionChanged_CMFCRibbonBar__UEAAXXZ(CMFCRibbonBar* pThis);
extern "C" int MS_ABI impl__TranslateChar_CMFCRibbonBar__UEAAHI_Z(CMFCRibbonBar* pThis, unsigned int nChar);
// CMFCToolBar / CMFCPopupMenu statics and thunks
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
extern "C" CMFCPopupMenu* MS_ABI impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
extern "C" void MS_ABI impl__Deactivate_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI impl__RedrawUnderlines_CMFCToolBar__SAXXZ();
extern "C" int MS_ABI impl__TranslateChar_CMFCToolBar__UEAAHI_Z(CMFCToolBar* pThis, unsigned int nChar);
extern "C" const CObList* MS_ABI impl__GetAllToolbars_CMFCToolBar__SAAEBVCObList__XZ();
// CWnd
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);
// AFX_GLOBAL_DATA / CGlobalUtils (core/runtime, featurepack/CMFC_misc_stubs.cpp)
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ();
extern "C" int MS_ABI impl__IsDwmCompositionEnabled_AFX_GLOBAL_DATA__QEAAHXZ();
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" unsigned char impl__afxGlobalUtils__3VCGlobalUtils__A[16];
extern "C" void MS_ABI impl__GetSystemBorders_CGlobalUtils__QEAA_AVCSize__PEAVCWnd___Z(CSize* ret, CGlobalUtils* pThis, CWnd* wnd);
// CMFCVisualManager base virtuals (featurepack/visualmanager/CMFCVisualManager.cpp)
extern "C" void MS_ABI impl__OnActivateApp_CMFCVisualManager__UEAAXPEAVCWnd__H_Z(CMFCVisualManager* pThis, CWnd* pWnd, int bActive);
extern "C" int MS_ABI impl__OnNcActivate_CMFCVisualManager__UEAAHPEAVCWnd__H_Z(CMFCVisualManager* pThis, CWnd* pWnd, int bActive);
extern "C" int MS_ABI impl__OnNcPaint_CMFCVisualManager__UEAAHPEAVCWnd__AEBVCObList__VCRect___Z(CMFCVisualManager* pThis, CWnd* pWnd, const CObList& lstSysButtons, CRect rectRedraw);
extern "C" int MS_ABI impl__OnSetWindowRegion_CMFCVisualManager__UEAAHPEAVCWnd__VCSize___Z(CMFCVisualManager* pThis, CWnd* pWnd, long long sizeWindow);
extern "C" void MS_ABI impl__ResetCheckCompositionFlag_AFX_GLOBAL_DATA__QEAAXXZ();
extern "C" CSize* MS_ABI impl__GetNcBtnSize_CMFCVisualManager__UEBA_AVCSize__H_Z(const CMFCVisualManager* pThis, CSize* pRet, int bSmall);
// CMFCCaptionButtonEx (featurepack/controls/CMFCCaptionButtonEx.cpp): the 0x48-byte
// object UpdateCaption allocates; its vtable slot 1 is the deleting destructor.
extern "C" void* MS_ABI impl___0CMFCCaptionButtonEx__QEAA_I_Z(void* pThis, unsigned int nHit);
// CMenu / CWnd text
extern "C" CMenu* MS_ABI impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU hMenu);
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CWnd* pThis, CString* rString);
// CWinAppEx (core/app/CWinAppEx.cpp)
extern "C" void MS_ABI impl__OnClosingMainFrame_CWinAppEx__MEAAXPEAVCFrameImpl___Z(CWinAppEx* pThis, void* pFrameImpl);
extern "C" int MS_ABI impl__LoadState_CWinAppEx__UEAAHPEB_WPEAVCFrameImpl___Z(CWinAppEx* pThis, const wchar_t* lpszSectionName, void* pFrameImpl);
// CWnd (core/window/CWnd.cpp, Thunks.cpp) and CMFCRibbonBar::RecalcLayout
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" void MS_ABI impl__RecalcLayout_CMFCRibbonBar__UEAAXXZ(CMFCRibbonBar* pThis);
extern "C" void MS_ABI impl__StoreWindowPlacement_CFrameImpl__IEAAXXZ(void* pThis);   // defined below
// CWinAppEx window placement (core/app/CWinAppEx.cpp).  Retail's parameter order is
// (rectNormalPosition, nFlags, nShowCmd) for both -- the OpenMFC definitions name
// their 3rd/4th parameters showCmd/flags, but they only store and reload the two
// ints positionally, so passing them in retail order round-trips correctly.
extern "C" int MS_ABI impl__LoadWindowPlacement_CWinAppEx__MEAAHAEAVCRect__AEAH1_Z(CWinAppEx* pThis, CRect* rect, int* nFlags, int* nShowCmd);
extern "C" int MS_ABI impl__StoreWindowPlacement_CWinAppEx__MEAAHAEBVCRect__HH_Z(CWinAppEx* pThis, const CRect* rect, int nFlags, int nShowCmd);
// CMFCToolBar::SaveState (featurepack/toolbar/CMFCToolBar.cpp, which defines it with
// this same (pThis, lpszProfileName, nIndex, uiID) list).
extern "C" int MS_ABI impl__SaveState_CMFCToolBar__UEAAHPEB_WHI_Z(CMFCToolBar* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
// Defined further down in this file.
extern "C" void MS_ABI impl__OnChangeVisualManager_CFrameImpl__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RedrawCaptionButton_CFrameImpl__IEAAXPEAVCMFCCaptionButtonEx___Z(void* pThis, void* pButton);
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CFrameImpl__IEAAXVCPoint___Z(void* pThis, long long point);
extern "C" unsigned int MS_ABI impl__OnNcHitTest_CFrameImpl__IEAAIVCPoint___Z(void* pThis, long long point);
extern "C" RECT* MS_ABI impl__GetCaptionRect_CFrameImpl__IEAA_AVCRect__XZ(void* pThis, RECT* ret);
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CFrameImpl__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__AddDefaultButtonsToCustomizePane_CFrameImpl__IEAAXPEAVCMFCPopupMenu__I_Z(
    void* pThis, void* pMenuPopup, unsigned int uiToolbarID);
// User-toolbar lifecycle (LoadUserToolbars / CreateNewToolBar / DeleteToolBar).  Each
// is used only for an object whose vtable lives in THIS image (see IsOwnObject below);
// an object built by a client carries the retail vtable and is dispatched through it.
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(CRuntimeClass* pThis);   // core/runtime/CRuntimeClass.cpp
extern "C" int MS_ABI impl__Create_CMFCToolBar__UEAAHPEAVCWnd__KI_Z(CMFCToolBar* pThis, CWnd* pParentWnd, unsigned long dwStyle, unsigned int nID);   // featurepack/toolbar/Thunks.cpp
extern "C" int MS_ABI impl__LoadState_CMFCToolBar__UEAAHPEB_WHI_Z(CMFCToolBar* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
extern "C" int MS_ABI impl__RemoveStateFromRegistry_CMFCToolBar__UEAAHPEB_WHI_Z(CMFCToolBar* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
extern "C" void MS_ABI impl__EnableDocking_CMFCToolBar__UEAAXK_Z(CMFCToolBar* pThis, unsigned long dwAlignment);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);   // core/window/CWnd.cpp
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd* pThis, const wchar_t* lpszString);   // core/window/CWnd.cpp
extern "C" int MS_ABI impl__FloatPane_CPane__UEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(CPane* pThis, const RECT* pRectFloat, int dockMethod, bool bShow);   // featurepack/docking/CPane.cpp
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(const CBasePane* pThis, int bNoAssert);   // featurepack/docking/CBasePane.cpp
extern "C" void MS_ABI impl__RemovePane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(void* pThis, CPane* pWnd, int dockMethod);
extern "C" void MS_ABI impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int bDestroy, int bNoDelayedDestroy);
extern "C" void MS_ABI impl__RemovePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int bDestroy, int bNoDelayedDestroy);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__DockPane_CDockingManager__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(CDockingManager* pThis, CBasePane* pBar, unsigned int nDockBarID, const RECT* lpRect);
// OnShowCustomizePane: the four frame classes it tests with IsKindOf.
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleIPFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDocIPFrameWndEx__SAPEAUCRuntimeClass__XZ();
// ProcessMouseWheel / ProcessMouseMove (featurepack/menu, featurepack/ribbon/RuntimeClasses.cpp)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonMiniToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonPanelMenu__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCColorPopupMenu__SAPEAUCRuntimeClass__XZ();   // featurepack/controls/RuntimeClasses.cpp
extern "C" void* MS_ABI impl__FindMenuWithConnectedFloaty_CMFCPopupMenu__KAPEAV1_XZ();
extern "C" CMFCToolBar* MS_ABI impl__FindDestintationToolBar_CMFCPopupMenuBar__QEAAPEAVCMFCToolBar__VCPoint___Z(
    CMFCPopupMenuBar* pThis, long long point);
// The linker-provided base of this DLL's own image (IsOwnObject).
extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {

// A minimal singly linked pointer list.  Retail keeps these as CPtrList members of
// CFrameImpl.  OpenMFC's list WRAPPERS (CObList / CPtrList) cannot be called from this
// translation unit -- their methods are out-of-line C++ symbols in
// detail/FilecoreSupport.cpp and this file's link audit rejects any new C++-mangled
// undefined -- and std::vector / std::list / std::wstring each drag in libstdc++
// helpers the audit rejects for the same reason.  (The header-only FindListStorage
// accessor used by GetFreeCtrlBarID below is the one thing that does get through, but
// it only READS an existing wrapper's state; it cannot stand in for a member list of
// our own.)  So the storage below is hand-rolled and uses nothing but operator
// new/delete.
struct PtrNode {
    void* pData;
    PtrNode* pNext;
};

void PtrListAddTail(PtrNode*& pHead, void* pData) {
    PtrNode* pNode = new PtrNode();
    pNode->pData = pData;
    pNode->pNext = nullptr;
    if (pHead == nullptr) {
        pHead = pNode;
        return;
    }
    PtrNode* pTail = pHead;
    while (pTail->pNext != nullptr) {
        pTail = pTail->pNext;
    }
    pTail->pNext = pNode;
}

void PtrListRemoveAll(PtrNode*& pHead) {
    while (pHead != nullptr) {
        PtrNode* pNext = pHead->pNext;
        delete pHead;
        pHead = pNext;
    }
}

// Per-CFrameImpl state that FrameImplState (detail/CbarcoreSupport.h, not owned by
// this file) has no room for.  It is file-local on purpose: every ?...@CFrameImpl@@
// export lives in this translation unit, so nothing outside can observe it except
// through the openmfc_CFrameImpl_* helpers below the namespace.  Each field is the
// stand-in for the retail member at the offset named on its line (see the map at the
// top of the file); the initial values are the constructor's (0x61d20).
struct FrameImplExtra {
    UINT uiUserToolbarFirst = static_cast<UINT>(-1);   // retail +0x08
    UINT uiUserToolbarLast = static_cast<UINT>(-1);    // retail +0x0c
    UINT nIDDefaultResource = 0;                       // retail +0x10
    UINT uiHotSysButton = 0;                           // retail +0x14
    UINT uiHitSysButton = 0;                           // retail +0x18
    int nGate20 = 0;                                   // retail +0x20 (never written by this class)
    int nWindowRegionResult = 0;                       // retail +0x24
    BOOL bOleInPlaceActive = FALSE;                    // retail +0x30
    BOOL bHadCaption = TRUE;                           // retail +0x34
    BOOL bLoadDockState = TRUE;                        // retail +0x38 (low dword)
    BOOL bWindowPosChanging = FALSE;                   // retail +0x40
    RECT rectRedraw = {};                              // retail +0x50 (SetRectEmpty in the ctor)
    PtrNode* pUserToolbars = nullptr;                  // retail +0x60 (CPtrList)
    PtrNode* pTearOffToolbars = nullptr;               // retail +0x98 (CPtrList)
    // retail +0xd0, the caption ("system") button list.  Kept as a real CObList
    // because retail hands the object itself to CMFCVisualManager::OnNcPaint; it
    // is built through the CObList ctor/dtor thunks (bSysButtonsConstructed says
    // whether that has happened) and read through FindListStorage.
    alignas(CObList) unsigned char sysButtonsStorage[sizeof(CObList)] = {};
    bool bSysButtonsConstructed = false;
    CString strUserToolbarsProfile;                    // retail +0x108
    CDockingManager* pDockManager = nullptr;           // retail +0x120
    CRuntimeClass* pNewUserToolBarRTC = nullptr;       // retail +0x178; NULL means "not set
                                                       // yet", resolved to CMFCToolBar's
                                                       // descriptor by UserToolBarRTC()
    CMFCRibbonBar* pRibbonBar = nullptr;               // retail +0x180
    CWnd* pRibbonStatusBar = nullptr;                  // retail +0x188
};

std::mutex g_frameImplExtraMutex;
std::unordered_map<const void*, FrameImplExtra> g_frameImplExtras;

// The mutex above only guards the map itself.  The bodies below copy what they need
// out of the entry and drop the lock before calling into USER32 or another thunk,
// exactly as InitUserToolbars / IsUserDefinedToolbar already do, because those
// calls re-enter the frame's window procedure and therefore this file.
FrameImplExtra* FindExtra(const void* pThis) {
    auto it = g_frameImplExtras.find(pThis);
    return it != g_frameImplExtras.end() ? &it->second : nullptr;
}

// The +0x118 member: the FrameImplState the constructor thunk fills.
CFrameWnd* OwnerFrame(const void* pThis) {
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    auto it = g_frameImplStates.find(
        reinterpret_cast<CFrameImpl*>(const_cast<void*>(pThis)));
    return it != g_frameImplStates.end() ? it->second.ownerFrame : nullptr;
}

// The +0x110 member: SetMenuBar's FrameImplState field.
CMFCMenuBar* MenuBar(const void* pThis) {
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    auto it = g_frameImplStates.find(
        reinterpret_cast<CFrameImpl*>(const_cast<void*>(pThis)));
    return it != g_frameImplStates.end()
               ? reinterpret_cast<CMFCMenuBar*>(it->second.menuBar)
               : nullptr;
}

// CWnd::m_hWnd (+0x40) of any CWnd-derived object, the way retail reads it.
HWND WndHandle(const void* pWnd) {
    return pWnd != nullptr ? static_cast<const CWnd*>(pWnd)->m_hWnd : nullptr;
}

// Retail's `if (!afxGlobalData.<+0x00>) { afxGlobalData.Initialize(); <+0x00> = 1; }`
// prologue, emitted inline before every AFX_GLOBAL_DATA use in this class (e.g. at
// 0x65b50 in OnNcActivate, 0x6522a in OnNcPaint, 0x66ad6 in OnCompositionChanged).
// OpenMFC's afxGlobalData is a zero-filled 720-byte blob (featurepack/
// CMFC_misc_stubs.cpp) and its Initialize export is a no-op, so the flag is the only
// thing that changes; reading the blob's bytes at retail offsets is what the bodies
// below do, and every such read is named where it happens.
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

// Retail's visual-manager vftable slot +0x3f8, the predicate the caption-button
// handlers test first (the public MFC source calls it IsOwnerDrawCaption).  The
// base body is `xor eax,eax; ret` (0x7260) and OpenMFC's CMFCVisualManager class
// (include/openmfc/afxmfc.h) does not declare the virtual at all, so there is no
// OpenMFC manager that can answer TRUE: every body below that retail gates on it
// takes the FALSE path.  Kept as one helper so a future manager can be wired in.
BOOL VisualManagerIsOwnerDrawCaption() {
    return FALSE;
}

// Retail 0x97f4: CMFCVisualManager::GetInstance, lazily creating the default
// manager.  OpenMFC's GetInstance is a real C++ static (featurepack/visualmanager)
// the ctor thunk above already calls.
CMFCVisualManager* VisualManager() {
    return CMFCVisualManager::GetInstance();
}

// The retail +0x148 gate is CFullScreenImpl::m_bFullScreen of the sub-object at
// +0x128 (detail/CFullScreenImplSupport.h).  OpenMFC's CFrameImpl carries no such
// sub-object, so the flag reads as "not full screen".
BOOL IsFullScreen() {
    return FALSE;
}

// Retail 0x61c2c (no export; the body AFX_GLOBAL_DATA::GetShellAutohideBars
// inlines to): when the +0x284 "recompute" flag OnSettingChange sets is non-zero,
// it clears the flag, asks SHAppBarMessage(ABM_GETAUTOHIDEBAR) once per screen edge
// and ORs 8 (bottom, edge 3) / 4 (top, edge 1) / 1 (left, edge 0) / 2 (right, edge 2)
// into the cache at +0x2a4; it then returns the +0x2a4 cache.
// DEVIATION: shell32 is not in this DLL's link line, so the recompute is not done
// here; the cached word is returned as-is (0 unless something else fills it).
int GlobalDataShellAutohideBars() {
    return GlobalDataInt(0x2a4);
}

// Retail 0x1c9288 (no export): the lazily-bound dwmapi!DwmDefWindowProc that
// OnNcHitTest hands WM_NCHITTEST to.  Retail loads L"dwmapi.dll" (the string at
// 0x341790) through its system-directory-only loader helper at 0xd8230 -- when
// kernel32 exports SetDefaultDllDirectories it calls LoadLibraryExW(name, NULL,
// LOAD_LIBRARY_SEARCH_SYSTEM32 /*0x800*/), otherwise it prefixes GetSystemDirectoryW
// and LoadLibraryW's the full path -- then GetProcAddress's "DwmDefWindowProc"
// (0x3417a8) and caches the EncodePointer'd result; on any failure it returns 0
// without touching *plResult.  The same two-way load is done here (the encoded
// cache is not, it is an in-process hardening detail with no observable effect).
typedef BOOL (WINAPI* DwmDefWindowProcFn)(HWND, UINT, WPARAM, LPARAM, LRESULT*);
DwmDefWindowProcFn g_pfnDwmDefWindowProc = nullptr;
bool g_bDwmDefWindowProcResolved = false;

HMODULE LoadSystemLibrary(const wchar_t* lpszName) {
    HMODULE hKernel = ::GetModuleHandleW(L"kernel32.dll");
    if (hKernel != nullptr && ::GetProcAddress(hKernel, "SetDefaultDllDirectories") != nullptr) {
        return ::LoadLibraryExW(lpszName, nullptr, 0x00000800 /*LOAD_LIBRARY_SEARCH_SYSTEM32*/);
    }
    // Retail: GetSystemDirectoryW into a MAX_PATH+1 buffer, append L"\\" unless the
    // directory already ends in one, append the name (wcscat_s, failure -> NULL).
    wchar_t szPath[MAX_PATH + 1] = {};
    const UINT nLen = ::GetSystemDirectoryW(szPath, MAX_PATH + 1);
    if (nLen == 0 || nLen > MAX_PATH) {
        return nullptr;
    }
    UINT nPos = nLen;
    if (szPath[nPos - 1] != L'\\') {
        if (nPos + 1 > MAX_PATH) {
            return nullptr;
        }
        szPath[nPos++] = L'\\';
    }
    const size_t nName = ::wcslen(lpszName);
    if (nPos + nName > MAX_PATH) {
        return nullptr;
    }
    ::memcpy(szPath + nPos, lpszName, (nName + 1) * sizeof(wchar_t));
    return ::LoadLibraryW(szPath);
}

BOOL CallDwmDefWindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult) {
    if (!g_bDwmDefWindowProcResolved) {
        g_bDwmDefWindowProcResolved = true;
        HMODULE hDwm = LoadSystemLibrary(L"dwmapi.dll");
        if (hDwm != nullptr) {
            g_pfnDwmDefWindowProc =
                reinterpret_cast<DwmDefWindowProcFn>(::GetProcAddress(hDwm, "DwmDefWindowProc"));
        }
    }
    if (g_pfnDwmDefWindowProc == nullptr) {
        return FALSE;
    }
    return g_pfnDwmDefWindowProc(hWnd, nMsg, wParam, lParam, plResult);
}

// The retail +0x208 BOOL of CDockingManager (the print-preview flag that
// ?IsPrintPreview@CFrameImpl@@ 0x66ab0 and OnMenuChar 0x64490 test).  OpenMFC's
// CDockingManager (include/openmfc/afxmfc.h) has no such member and its
// SetPrintPreviewMode export is a stub, so print-preview mode can never be entered
// on this side: the flag reads as FALSE.
BOOL DockManagerIsPrintPreview(const CDockingManager* pDockManager) {
    (void)pDockManager;
    return FALSE;
}

// Caption-button list access (retail +0xd0).  The CObList is constructed on first
// use through the ctor thunk; ForEachSysButton reads it through FindListStorage the
// way GetFreeCtrlBarID reads its argument list.
CObList* SysButtons(FrameImplExtra& extra) {
    return reinterpret_cast<CObList*>(extra.sysButtonsStorage);
}

// Snapshot of the caption-button list, taken under the map lock so the walkers can
// call back into USER32 / thunks without holding it.  Retail walks the node chain
// in place; the snapshot is equivalent because every caller is on the UI thread and
// the list only changes inside this file.
struct SysButtonSnapshot {
    enum { kMax = 16 };   // retail's UpdateCaption adds at most the four system buttons
    void* pButtons[kMax];
    int nCount = 0;
};

void SnapshotSysButtons(const FrameImplExtra& extra, SysButtonSnapshot& out) {
    out.nCount = 0;
    if (!extra.bSysButtonsConstructed) {
        return;
    }
    const CObList* pList = reinterpret_cast<const CObList*>(extra.sysButtonsStorage);
    typedef CList<CObject*, CObject*> ObListData;
    const ObListData* pData =
        openmfc::detail::filecore::FindListStorage<CObList, CObject*, CObject*>(pList);
    if (pData == nullptr) {
        return;
    }
    ObListData::POSITION pos = pData->GetHeadPosition();
    while (pos != ObListData::POSITION(nullptr) && out.nCount < SysButtonSnapshot::kMax) {
        out.pButtons[out.nCount++] = pData->GetNext(pos);
    }
}

// CMFCCaptionButtonEx fields the bodies below read, per detail/
// CMFCCaptionButtonExSupport.h: m_bPushed +0x08, m_bFocused +0x0c, m_nHit +0x20, and
// vtable slot 5 (GetRect, hidden 16-byte return buffer).
struct SysButtonView {
    void* vfptr;          // 0
    LONG m_bPushed;       // 8
    LONG m_bFocused;      // 12
    LONG m_bHidden;       // 16
    LONG m_bEnabled;      // 20
    LONG m_bDroppedDown;  // 24
    LONG m_bLeftAlign;    // 28
    UINT m_nHit;          // 32
};
static_assert(offsetof(SysButtonView, m_bPushed) == 0x08, "CMFCCaptionButton::m_bPushed");
static_assert(offsetof(SysButtonView, m_bFocused) == 0x0c, "CMFCCaptionButton::m_bFocused");
static_assert(offsetof(SysButtonView, m_nHit) == 0x20, "CMFCCaptionButton::m_nHit");

RECT SysButtonGetRect(void* pButton) {
    RECT rect = {};
    typedef void* (MS_ABI* GetRectFn)(void*, RECT*);
    void** vtbl = *reinterpret_cast<void***>(pButton);
    reinterpret_cast<GetRectFn>(vtbl[5])(pButton, &rect);
    return rect;
}

// Retail's `delete pButton` through the object's vtable slot 1 (the deleting
// destructor, called with flag 1) -- what ??1CFrameImpl 0x61ee0 does to every
// element of the +0xd0 list.
void SysButtonDelete(void* pButton) {
    typedef void* (MS_ABI* DeletingDtorFn)(void*, unsigned int);
    void** vtbl = *reinterpret_cast<void***>(pButton);
    reinterpret_cast<DeletingDtorFn>(vtbl[1])(pButton, 1);
}

// The +0x180 ribbon bar and the two flags of it the bodies below read, per
// include/openmfc/afxmfc.h (pinned in detail/MfccoreSupport.cpp):
// m_bReplaceFrameCaption at 1128 (retail +0x468) and m_bIsTransparentCaption at
// 1096 (retail +0x448).
static_assert(offsetof(CMFCRibbonBar, m_bReplaceFrameCaption) == 0x468, "CMFCRibbonBar::m_bReplaceFrameCaption");
static_assert(offsetof(CMFCRibbonBar, m_bIsTransparentCaption) == 0x448, "CMFCRibbonBar::m_bIsTransparentCaption");
static_assert(offsetof(CMenu, m_hMenu) == 0x8, "CMenu::m_hMenu");

CMFCRibbonBar* RibbonBar(const void* pThis) {
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    FrameImplExtra* pExtra = FindExtra(pThis);
    return pExtra != nullptr ? pExtra->pRibbonBar : nullptr;
}

CWnd* RibbonStatusBar(const void* pThis) {
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    FrameImplExtra* pExtra = FindExtra(pThis);
    return pExtra != nullptr ? pExtra->pRibbonStatusBar : nullptr;
}

BOOL OleInPlaceActive(const void* pThis) {
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    FrameImplExtra* pExtra = FindExtra(pThis);
    return pExtra != nullptr ? pExtra->bOleInPlaceActive : FALSE;
}

// The `m_pRibbonBar->GetSafeHwnd() != NULL && (m_pRibbonBar->IsWindowVisible() ||
// m_FullScreenMgr.IsFullScreen() || !m_pFrame->IsWindowVisible()) &&
// m_pRibbonBar->m_bReplaceFrameCaption` test that OnGetMinMaxInfo 0x65300,
// OnNcCalcSize 0x655f0 and OnNcPaint 0x651d0 all open with (each inlines it as:
// test +0x180 and its +0x40; IsWindowVisible on it; else test +0x148; else
// IsWindowVisible on m_pFrame->m_hWnd; then test +0x468).
BOOL RibbonReplacesCaption(CMFCRibbonBar* pRibbonBar, HWND hWndFrame) {
    if (pRibbonBar == nullptr || pRibbonBar->m_hWnd == nullptr) {
        return FALSE;
    }
    if (!::IsWindowVisible(pRibbonBar->m_hWnd) && !IsFullScreen() && ::IsWindowVisible(hWndFrame)) {
        return FALSE;
    }
    return pRibbonBar->m_bReplaceFrameCaption != 0;
}

// `afxGlobalUtils.GetSystemBorders(m_pFrame)` (0x6de90 on the object at 0x3aacf8), the
// way GetCaptionRect / OnNcHitTest / OnNcCalcSize call it.
SIZE FrameSystemBorders(CFrameWnd* pFrame) {
    alignas(CSize) unsigned char storage[sizeof(CSize)];
    CSize* pSize = reinterpret_cast<CSize*>(storage);
    impl__GetSystemBorders_CGlobalUtils__QEAA_AVCSize__PEAVCWnd___Z(
        pSize, reinterpret_cast<CGlobalUtils*>(impl__afxGlobalUtils__3VCGlobalUtils__A), pFrame);
    SIZE size;
    size.cx = pSize->cx;
    size.cy = pSize->cy;
    return size;
}

// The retail +0x178 default: the CMFCToolBar runtime-class descriptor
// (0x1803aa5f8 in the ANSI image reads "CMFCToolBar").
CRuntimeClass* UserToolBarRTC(const FrameImplExtra& extra) {
    return extra.pNewUserToolBarRTC != nullptr
               ? extra.pNewUserToolBarRTC
               : impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
}

// CPoint / CSize by value travel as one 8-byte aggregate in a register under the
// MS ABI (see the placeholder-signature note in the workflow briefing).
POINT UnpackPoint(long long packed) {
    POINT pt;
    pt.x = static_cast<LONG>(static_cast<unsigned long long>(packed) & 0xffffffffull);
    pt.y = static_cast<LONG>(static_cast<unsigned long long>(packed) >> 32);
    return pt;
}

long long PackSize(LONG cx, LONG cy) {
    return static_cast<long long>((static_cast<unsigned long long>(static_cast<unsigned long>(cy)) << 32) |
                                  static_cast<unsigned long>(cx));
}

// Retail keeps the frame list in the exported CList below.  OpenMFC's list classes
// keep their contents in a side table rather than in the object's own bytes (see
// OPENMFC_DECLARE_LIST_WRAPPER in include/openmfc/afx.h), so the working storage for
// AddFrame/RemoveFrame is here and the exported object is ABI storage only.
std::mutex g_frameListMutex;
PtrNode* g_pFrameListHead = nullptr;

// ---------------------------------------------------------------------------
// Virtual dispatch on objects this file did not create.
//
// Retail calls the toolbar / popup-menu / frame / dock-site virtuals below through
// the object's own vftable.  Two kinds of object reach these bodies:
//   * one a CLIENT built (its class compiled against the real MFC headers, e.g. a
//     CMainFrame, or a toolbar from a client runtime class): its vptr points into
//     the client's image and its vftable has retail's layout, so retail's slot
//     numbers are exact and the call is made through the slot, as retail does;
//   * one THIS DLL built (OpenMFC's own C++ classes, compiled by mingw): its vptr
//     points into this image and its vftable is Itanium-shaped, where retail's
//     slot numbers mean something else.  Such an object is handed to the sibling
//     impl__ thunk for the retail base implementation instead -- or, where a helper
//     says so, to the retail inline body or OpenMFC's own C++ virtual (a DEVIATION
//     wherever a thunk is used: an OpenMFC-side override is not reached unless named).
// The vptr's home image tells the two apart -- the same test
// featurepack/customize/CMFCToolBarsCustomizeDialog.cpp (DeleteRuntimeObject)
// applies to objects it gets back from CRuntimeClass::CreateObject.
// ---------------------------------------------------------------------------
bool IsOwnObject(const void* pObject) {
    const void* vptr = *reinterpret_cast<const void* const*>(pObject);
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* p = static_cast<const unsigned char*>(vptr);
    return p >= base && p < base + nt->OptionalHeader.SizeOfImage;
}

template <typename Fn>
Fn VSlot(const void* pObject, int nSlot) {
    void* const* vtbl = *reinterpret_cast<void* const* const*>(pObject);
    return reinterpret_cast<Fn>(vtbl[nSlot]);
}

static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at +0x40 of every window below");
static_assert(offsetof(CBasePane, m_dwStyle) == 0x104, "CBasePane::m_dwStyle (GetPaneStyle/SetPaneStyle bodies)");
static_assert(offsetof(CBasePane, m_pParentDockBar) == 0x128, "CBasePane::m_pParentDockBar (GetParentDockSite body)");
static_assert(offsetof(CPane, m_nMRUWidth) == 0x1dc, "CPane::m_nMRUWidth (CreateNewToolBar stores 0x7fff there)");

// Retail CMFCToolBar vftable slots (the table ??0CMFCToolBar@@QEAA@XZ installs at
// 0x14b93a, mfc140 RVA 0x313618; each slot read out of that table):
enum ToolBarSlot {
    kTbDeletingDtor = 1,             // +0x008  scalar deleting destructor (flags in edx)
    kTbDestroyWindow = 26,           // +0x0d0  ?DestroyWindow@CWnd@@UEAAHXZ
    kTbGetParentDockSite = 106,      // +0x350  inline CBasePane::GetParentDockSite, body 0x87e0 `mov 0x128(%rcx),%rax`
    kTbGetPaneStyle = 114,           // +0x390  inline CBasePane::GetPaneStyle, body 0x8840 `mov 0x104(%rcx),%eax`
    kTbSetPaneStyle = 123,           // +0x3d8  inline CBasePane::SetPaneStyle, body 0x88b0 `mov %edx,0x104(%rcx)`
    kTbEnableDocking = 125,          // +0x3e8  ?EnableDocking@CMFCToolBar@@UEAAXK@Z
    kTbFloatPane = 129,              // +0x408  ?FloatPane@CPane@@UEAAHVCRect@@W4AFX_DOCK_METHOD@@_N@Z
    kTbGetParentMiniFrame = 140,     // +0x460  ?GetParentMiniFrame@CBasePane@@UEBAPEAVCPaneFrameWnd@@H@Z
    kTbLoadState = 141,              // +0x468  ?LoadState@CMFCToolBar@@
    kTbCreate = 202,                 // +0x650  ?Create@CMFCToolBar@@UEAAHPEAVCWnd@@KI@Z
    kTbRemoveStateFromRegistry = 226 // +0x710  ?RemoveStateFromRegistry@CMFCToolBar@@
};

BOOL ToolBarCreate(void* pBar, CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    if (IsOwnObject(pBar)) {
        return impl__Create_CMFCToolBar__UEAAHPEAVCWnd__KI_Z(static_cast<CMFCToolBar*>(pBar), pParentWnd, dwStyle, nID);
    }
    typedef int (MS_ABI* Fn)(void*, CWnd*, DWORD, UINT);
    return VSlot<Fn>(pBar, kTbCreate)(pBar, pParentWnd, dwStyle, nID);
}

// `delete pBar`: retail calls vftable slot 1 with flags 1.  On an Itanium vftable
// slot 1 is the non-deleting destructor, so an own object is deleted through C++.
void ToolBarDelete(void* pBar) {
    if (IsOwnObject(pBar)) {
        delete static_cast<CObject*>(pBar);
        return;
    }
    typedef void* (MS_ABI* Fn)(void*, unsigned int);
    VSlot<Fn>(pBar, kTbDeletingDtor)(pBar, 1u);
}

BOOL ToolBarDestroyWindow(void* pBar) {
    if (IsOwnObject(pBar)) {
        return impl__DestroyWindow_CWnd__UEAAHXZ(static_cast<CWnd*>(pBar));
    }
    typedef int (MS_ABI* Fn)(void*);
    return VSlot<Fn>(pBar, kTbDestroyWindow)(pBar);
}

BOOL ToolBarLoadState(void* pBar, const wchar_t* lpszProfileName, int nIndex, UINT uiID) {
    if (IsOwnObject(pBar)) {
        return impl__LoadState_CMFCToolBar__UEAAHPEB_WHI_Z(static_cast<CMFCToolBar*>(pBar), lpszProfileName, nIndex, uiID);
    }
    typedef int (MS_ABI* Fn)(void*, const wchar_t*, int, UINT);
    return VSlot<Fn>(pBar, kTbLoadState)(pBar, lpszProfileName, nIndex, uiID);
}

BOOL ToolBarRemoveStateFromRegistry(void* pBar, const wchar_t* lpszProfileName, int nIndex, UINT uiID) {
    if (IsOwnObject(pBar)) {
        return impl__RemoveStateFromRegistry_CMFCToolBar__UEAAHPEB_WHI_Z(
            static_cast<CMFCToolBar*>(pBar), lpszProfileName, nIndex, uiID);
    }
    typedef int (MS_ABI* Fn)(void*, const wchar_t*, int, UINT);
    return VSlot<Fn>(pBar, kTbRemoveStateFromRegistry)(pBar, lpszProfileName, nIndex, uiID);
}

// SetPaneStyle(GetPaneStyle() | dwAdd).  Both retail slots are the inline CBasePane
// accessors of m_dwStyle (+0x104); an own object gets exactly those bodies.
void ToolBarOrPaneStyle(void* pBar, DWORD dwAdd) {
    if (IsOwnObject(pBar)) {
        CBasePane* pPane = static_cast<CBasePane*>(pBar);
        pPane->m_dwStyle = pPane->m_dwStyle | dwAdd;
        return;
    }
    typedef DWORD (MS_ABI* GetFn)(void*);
    typedef void (MS_ABI* SetFn)(void*, DWORD);
    const DWORD dwStyle = VSlot<GetFn>(pBar, kTbGetPaneStyle)(pBar);
    VSlot<SetFn>(pBar, kTbSetPaneStyle)(pBar, dwStyle | dwAdd);
}

void ToolBarEnableDocking(void* pBar, DWORD dwAlignment) {
    if (IsOwnObject(pBar)) {
        impl__EnableDocking_CMFCToolBar__UEAAXK_Z(static_cast<CMFCToolBar*>(pBar), dwAlignment);
        return;
    }
    typedef void (MS_ABI* Fn)(void*, DWORD);
    VSlot<Fn>(pBar, kTbEnableDocking)(pBar, dwAlignment);
}

// FloatPane(CRect rectFloat, AFX_DOCK_METHOD, bool): the 16-byte CRect travels by
// pointer to a caller-owned copy under the MS x64 ABI (retail passes &local in rdx).
BOOL ToolBarFloatPane(void* pBar, const RECT& rectFloat, int dockMethod, bool bShow) {
    RECT rectCopy = rectFloat;
    if (IsOwnObject(pBar)) {
        return impl__FloatPane_CPane__UEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(static_cast<CPane*>(pBar), &rectCopy, dockMethod, bShow);
    }
    typedef int (MS_ABI* Fn)(void*, RECT*, int, bool);
    return VSlot<Fn>(pBar, kTbFloatPane)(pBar, &rectCopy, dockMethod, bShow);
}

void* ToolBarGetParentDockSite(void* pBar) {
    if (IsOwnObject(pBar)) {
        return static_cast<CBasePane*>(pBar)->m_pParentDockBar;
    }
    typedef void* (MS_ABI* Fn)(void*);
    return VSlot<Fn>(pBar, kTbGetParentDockSite)(pBar);
}

void* ToolBarGetParentMiniFrame(void* pBar, BOOL bNoAssert) {
    if (IsOwnObject(pBar)) {
        return impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(static_cast<CBasePane*>(pBar), bNoAssert);
    }
    typedef void* (MS_ABI* Fn)(void*, int);
    return VSlot<Fn>(pBar, kTbGetParentMiniFrame)(pBar, bNoAssert);
}

// pDockSite->RemovePane(pBar, dockMethod): CDockSite vftable (mfc140 RVA 0x2e4388)
// slot 161, +0x508 = ?RemovePane@CDockSite@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@@Z.
void DockSiteRemovePane(void* pDockSite, void* pBar, int dockMethod) {
    if (IsOwnObject(pDockSite)) {
        impl__RemovePane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(pDockSite, static_cast<CPane*>(pBar), dockMethod);
        return;
    }
    typedef void (MS_ABI* Fn)(void*, void*, int);
    VSlot<Fn>(pDockSite, 161)(pDockSite, pBar, dockMethod);
}

// pMiniFrame->RemovePane(pBar, bDestroy, bNoDelayedDestroy): CPaneFrameWnd vftable
// (mfc140 RVA 0x2f5ad8) slot 97, +0x308 = ?RemovePane@CPaneFrameWnd@@UEAAXPEAVCBasePane@@HH@Z.
// An own mini frame that is a CMultiPaneFrameWnd goes to that class' override.
void MiniFrameRemovePane(void* pMiniFrame, void* pBar, BOOL bDestroy, BOOL bNoDelayedDestroy) {
    if (IsOwnObject(pMiniFrame)) {
        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pMiniFrame), impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ())) {
            impl__RemovePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(pMiniFrame, pBar, bDestroy, bNoDelayedDestroy);
        } else {
            impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(pMiniFrame, pBar, bDestroy, bNoDelayedDestroy);
        }
        return;
    }
    typedef void (MS_ABI* Fn)(void*, void*, int, int);
    VSlot<Fn>(pMiniFrame, 97)(pMiniFrame, pBar, bDestroy, bNoDelayedDestroy);
}

// m_pFrame->RecalcLayout(bNotify): frame vftable slot 96, +0x300
// (?RecalcLayout@CFrameWndEx@@UEAAXH@Z in the CFrameWndEx table, mfc140 RVA 0x2e7658;
// ?RecalcLayout@CMDIFrameWndEx@@UEAAXH@Z in the CMDIFrameWndEx one, 0x2ec8f8).  An own
// frame is dispatched through OpenMFC's own C++ virtual (afxwin.h CFrameWnd::RecalcLayout).
void FrameRecalcLayout(CFrameWnd* pFrame, BOOL bNotify) {
    if (IsOwnObject(pFrame)) {
        pFrame->RecalcLayout(bNotify);
        return;
    }
    typedef void (MS_ABI* Fn)(void*, int);
    VSlot<Fn>(pFrame, 96)(pFrame, bNotify);
}

// Retail CMFCPopupMenu vftable slots (the table ??0CMFCPopupMenu@@QEAA@XZ installs at
// 0xb528d, mfc140 RVA 0x2f6500), numbered against afxpopupmenu.h's declaration order
// and cross-checked on the slots whose position other bodies already pin (+0x3a8
// InCommand, +0x3d8/+0x3e0 IsScrollUp/DnAvailable, +0x3f0 DrawFade, +0x400 TearOff,
// +0x428 GetBorderSize -- see featurepack/menu/CMFCPopupMenu.cpp):
//   116 (+0x3a0) GetMenuBar -- base body `lea 0x230(%rcx),%rax`; the CMFCColorPopupMenu,
//                CMFCRibbonPanelMenu and CMFCRibbonMiniToolBar tables (mfc140 0x2df320 /
//                0x308078 / 0x3060b8) hold 0x28040 `lea 0x19c8(%rcx),%rax` there
//                (afxcolorpopupmenu.h returns &m_wndColorBar, afxribbonpanelmenu.h
//                &m_wndRibbonBar).  Those are the only CMFCPopupMenu-derived vftables in
//                the image, found by scanning .rdata for tables sharing the
//                CMFCPopupMenu slots and naming each through its RTTI locator.
//   132 (+0x420) IsRibbonMiniToolBar -- base 0x7260 `xor eax,eax`; the
//                CMFCRibbonMiniToolBar table holds 0x3ae0 `mov $1,%eax`
CMFCPopupMenuBar* PopupMenuBar(void* pPopup) {
    if (IsOwnObject(pPopup)) {
        const CObject* pObject = static_cast<const CObject*>(pPopup);
        const bool bBarAt19c8 =
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pObject, impl__GetThisClass_CMFCRibbonPanelMenu__SAPEAUCRuntimeClass__XZ()) != 0 ||
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pObject, impl__GetThisClass_CMFCColorPopupMenu__SAPEAUCRuntimeClass__XZ()) != 0;
        return reinterpret_cast<CMFCPopupMenuBar*>(static_cast<unsigned char*>(pPopup) +
                                                   (bBarAt19c8 ? 0x19c8 : 0x230));
    }
    typedef CMFCPopupMenuBar* (MS_ABI* Fn)(void*);
    return VSlot<Fn>(pPopup, 116)(pPopup);
}

BOOL PopupIsRibbonMiniToolBar(void* pPopup) {
    if (IsOwnObject(pPopup)) {
        return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pPopup), impl__GetThisClass_CMFCRibbonMiniToolBar__SAPEAUCRuntimeClass__XZ());
    }
    typedef int (MS_ABI* Fn)(void*);
    return VSlot<Fn>(pPopup, 132)(pPopup);
}

// Snapshot of the user-toolbar list (retail +0x60) and the members the toolbar bodies
// read, copied out under the companion-state lock (see the note above FindExtra).
struct UserToolbarState {
    bool bFound = false;
    UINT uiFirst = static_cast<UINT>(-1);
    UINT uiLast = static_cast<UINT>(-1);
    CRuntimeClass* pRTC = nullptr;
    CDockingManager* pDockManager = nullptr;
};

UserToolbarState ReadUserToolbarState(const void* pThis, CString* pProfile) {
    UserToolbarState state;
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    FrameImplExtra* pExtra = FindExtra(pThis);
    if (pExtra == nullptr) {
        return state;
    }
    state.bFound = true;
    state.uiFirst = pExtra->uiUserToolbarFirst;
    state.uiLast = pExtra->uiUserToolbarLast;
    state.pRTC = UserToolBarRTC(*pExtra);
    state.pDockManager = pExtra->pDockManager;
    if (pProfile != nullptr) {
        *pProfile = pExtra->strUserToolbarsProfile;
    }
    return state;
}

void AddUserToolbar(const void* pThis, void* pBar) {
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    PtrListAddTail(g_frameImplExtras[pThis].pUserToolbars, pBar);
}

}  // namespace

// ---------------------------------------------------------------------------
// File-level helpers for the sibling frame files (NOT exports: no // Symbol: marker,
// not in the .def).  Retail's CFrameWndEx / CMDIFrameWndEx / COleIPFrameWndEx write
// these CFrameImpl members directly (m_Impl.m_pDockManager = &m_dockManager in their
// constructors, m_Impl.m_pRibbonBar in AddPane, m_Impl.m_nIDDefaultResource in
// LoadFrame, the +0x30/+0x34 pair in ?RecalcLayout@CFrameWndEx@@ 0x67fb0); OpenMFC's
// frames have no embedded CFrameImpl, so this is the channel for those writes.
// ---------------------------------------------------------------------------
extern "C" void MS_ABI openmfc_CFrameImpl_SetDockManager(void* pFrameImpl, CDockingManager* pDockManager) {
    if (pFrameImpl == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    g_frameImplExtras[pFrameImpl].pDockManager = pDockManager;   // retail +0x120
}
extern "C" void MS_ABI openmfc_CFrameImpl_SetRibbonBar(void* pFrameImpl, CMFCRibbonBar* pRibbonBar) {
    if (pFrameImpl == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    g_frameImplExtras[pFrameImpl].pRibbonBar = pRibbonBar;       // retail +0x180
}
extern "C" void MS_ABI openmfc_CFrameImpl_SetRibbonStatusBar(void* pFrameImpl, CWnd* pRibbonStatusBar) {
    if (pFrameImpl == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    g_frameImplExtras[pFrameImpl].pRibbonStatusBar = pRibbonStatusBar;   // retail +0x188
}
extern "C" void MS_ABI openmfc_CFrameImpl_SetDefaultResourceID(void* pFrameImpl, UINT nIDDefaultResource) {
    if (pFrameImpl == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    g_frameImplExtras[pFrameImpl].nIDDefaultResource = nIDDefaultResource;   // retail +0x10
}
extern "C" void MS_ABI openmfc_CFrameImpl_SetOleInPlaceActive(void* pFrameImpl, BOOL bActive, BOOL bHadCaption) {
    if (pFrameImpl == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    FrameImplExtra& extra = g_frameImplExtras[pFrameImpl];
    extra.bOleInPlaceActive = bActive;     // retail +0x30
    extra.bHadCaption = bHadCaption;       // retail +0x34
}
extern "C" void MS_ABI openmfc_CFrameImpl_SetLoadDockState(void* pFrameImpl, BOOL bLoadDockState) {
    if (pFrameImpl == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    g_frameImplExtras[pFrameImpl].bLoadDockState = bLoadDockState;   // retail +0x38
}
// Snapshot of CFrameImpl::m_lstFrames for the visual manager (retail
// CMFCVisualManager::AdjustFrames / RedrawAll walk the list's nodes directly).  Copies
// up to nCapacity entries into pOut and returns the total number of frames in the
// list, so a caller can size a buffer with a first call of (NULL, 0).  A snapshot
// rather than a callback, because the callers redraw windows and a synchronous
// WM_PAINT can re-enter AddFrame/RemoveFrame.
extern "C" size_t MS_ABI openmfc_CFrameImpl_CopyFrameList(CFrameWnd** pOut, size_t nCapacity) {
    std::lock_guard<std::mutex> lock(g_frameListMutex);
    size_t nTotal = 0;
    for (PtrNode* pNode = g_pFrameListHead; pNode != nullptr; pNode = pNode->pNext, ++nTotal) {
        if (pOut != nullptr && nTotal < nCapacity) {
            pOut[nTotal] = static_cast<CFrameWnd*>(pNode->pData);
        }
    }
    return nTotal;
}

// Transcribed from retail 0x61d20 (see the member map at the top of the file): every
// scalar gets its constructor value, the three CPtrLists are constructed empty with
// block size 10, the profile string is set to the empty string, the redraw rect is
// SetRectEmpty'd, +0x178 gets CMFCToolBar's descriptor, and +0x2c gets
// m_pFrame->IsKindOf(CMDIChildWnd) (the descriptor at 0x180339bf0).
// DEVIATION: the members live in the FrameImplState / FrameImplExtra side tables
// instead of the object; the +0x2c IsKindOf result is not stored because no body in
// this file reads it, and the CFullScreenImpl sub-object at +0x128 is not modeled.
// Symbol: ??0CFrameImpl@@QEAA@PEAVCFrameWnd@@@Z
extern "C" void* MS_ABI impl___0CFrameImpl__QEAA_PEAVCFrameWnd___Z(void* pThis, CFrameWnd* pFrame) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    {
        std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
        CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
        g_frameImplStates.insert_or_assign(frameImpl, FrameImplState{pFrame, nullptr, manager != nullptr ? manager->GetRuntimeClass() : nullptr});
    }
    std::lock_guard<std::mutex> extraLock(g_frameImplExtraMutex);
    // A re-run constructor on the same address (retail would simply overwrite the
    // bytes) starts from the constructor values again.
    auto existing = g_frameImplExtras.find(pThis);
    if (existing != g_frameImplExtras.end()) {
        PtrListRemoveAll(existing->second.pUserToolbars);
        PtrListRemoveAll(existing->second.pTearOffToolbars);
        if (existing->second.bSysButtonsConstructed) {
            impl___1CObList__UEAA_XZ(SysButtons(existing->second));
        }
        g_frameImplExtras.erase(existing);
    }
    FrameImplExtra& extra = g_frameImplExtras[pThis];
    impl___0CObList__QEAA__J_Z(SysButtons(extra), 10);   // retail +0xd0, block size 10
    extra.bSysButtonsConstructed = true;
    return pThis;
}
// Retail 0x61ee0 RemoveHead's each of the three CPtrLists (+0x60, +0x98, +0xd0) and
// `delete`s every element through its vtable slot 1 (the deleting destructor, flag 1),
// then destroys the CFullScreenImpl sub-object and the profile CString.
// The user-toolbar list (+0x60, filled by CreateNewToolBar / LoadUserToolbars) is
// emptied first, as in retail, each non-NULL element deleted through ToolBarDelete
// (vftable slot 1 with flags 1 for a client object, C++ `delete` for an own one);
// the list is detached under the lock and the deletes run after it is dropped,
// because a window object's destructor can re-enter this file.
// DEVIATION: the tear-off list's elements (+0x98) are NOT deleted -- they are added
// by AddTearOffToolbar from outside this file and are left to whoever added them;
// the CFullScreenImpl sub-object is not modeled.
// Symbol: ??1CFrameImpl@@UEAA@XZ
extern "C" void MS_ABI impl___1CFrameImpl__UEAA_XZ(void* pThis) {
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    {
        std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
        (void)g_frameImplStates.erase(frameImpl);
    }
    PtrNode* pUserToolbars = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra != nullptr) {
            pUserToolbars = pExtra->pUserToolbars;
            pExtra->pUserToolbars = nullptr;
        }
    }
    for (PtrNode* pNode = pUserToolbars; pNode != nullptr; pNode = pNode->pNext) {
        if (pNode->pData != nullptr) {
            ToolBarDelete(pNode->pData);
        }
    }
    PtrListRemoveAll(pUserToolbars);
    // Drop the file-local companion state as well, or it outlives the object.
    std::lock_guard<std::mutex> extraLock(g_frameImplExtraMutex);
    auto it = g_frameImplExtras.find(pThis);
    if (it != g_frameImplExtras.end()) {
        PtrListRemoveAll(it->second.pUserToolbars);
        PtrListRemoveAll(it->second.pTearOffToolbars);
        if (it->second.bSysButtonsConstructed) {
            SysButtonSnapshot buttons;
            SnapshotSysButtons(it->second, buttons);
            for (int i = 0; i < buttons.nCount; ++i) {
                if (buttons.pButtons[i] != nullptr) {
                    SysButtonDelete(buttons.pButtons[i]);
                }
            }
            impl___1CObList__UEAA_XZ(SysButtons(it->second));
        }
        g_frameImplExtras.erase(it);
    }
}
// Retail 0x64130 reads m_pDockManager (+0x120) three times: it gates on
// m_pDockManager->[+0x308]->[+0x8]/[+0xc], then builds a panes menu into a
// ::CreatePopupMenu handle with ?BuildPanesMenu@CDockingManager@@ (0x4df60), and
// finally re-reads +0x120 again.  The rest is CMFCPopupMenu work -- RemoveAllItems
// (0xb7eb0), the popup's vtable slots +0x3a0 and +0x880, IsCustomizePane (0x64fc0)
// and ShowQuickCustomizePane (0x647d0).  It never reads m_pFrame (+0x118).
// The dock-manager pointer is now in this file's companion state, but the
// CMFCPopupMenu item model and the popup vtable slots are not, so this stays a stub.
// STUB.
// Symbol: ?OnShowPopupMenu@CFrameImpl@@IEAAHPEAVCMFCPopupMenu@@PEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__OnShowPopupMenu_CFrameImpl__IEAAHPEAVCMFCPopupMenu__PEAVCFrameWnd___Z(
    void* pThis, void* pPopupMenu, CFrameWnd* pWnd) {
    (void)pThis;
    (void)pPopupMenu;
    (void)pWnd;
    return FALSE;
}
// Transcribed from retail 0x64c60 (mfc140; RVA 0x64e30 in mfc140u):
//     if (m_pFrame && m_pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))        // descriptor 0x2ec3c8
//         return m_pFrame->OnShowCustomizePane(pMenuPopup, uiToolbarID);          // vftable +0x3e0 (slot 124)
//     if (m_pFrame && m_pFrame->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))           // 0x2e7160
//         return m_pFrame->OnShowCustomizePane(...);                             // +0x3b8 (slot 119)
//     if (m_pFrame && m_pFrame->IsKindOf(RUNTIME_CLASS(COleIPFrameWndEx)))      // 0x2ef6d0
//         return m_pFrame->OnShowCustomizePane(...);                             // +0x3f0 (slot 126)
//     if (m_pFrame && m_pFrame->IsKindOf(RUNTIME_CLASS(COleDocIPFrameWndEx)))   // 0x2ef070
//         return m_pFrame->OnShowCustomizePane(...);                             // +0x3f0 (slot 126)
//     return FALSE;
// (m_pFrame is re-read from +0x118 before every test; IsKindOf is 0x233310; each
// descriptor's m_lpszClassName was read out of the image.)  The four slots were
// identified from the classes' vftables (mfc140 RVAs 0x2ec8f8 / 0x2e7658 / 0x2ef8f8 /
// 0x2ef298): each holds an unexported copy of the header-inline
//     virtual BOOL OnShowCustomizePane(CMFCPopupMenu* p, UINT id)
//     { m_Impl.AddDefaultButtonsToCustomizePane(p, id); return TRUE; }
// (CMDIFrameWndEx 0x880d0, CFrameWndEx 0x66cd0, both OLE classes 0x95650:
// `add $0x590|$0x1f0|$0x298,%rcx; call 0x64d40 (AddDefaultButtonsToCustomizePane); mov $1,%eax`).
// A client frame is dispatched through that slot exactly as retail does.
// DEVIATION: a frame built by OpenMFC itself has no retail vftable; for it the inline
// base body is run directly -- AddDefaultButtonsToCustomizePane, then TRUE.  Retail's
// inline passes the frame's embedded m_Impl; OpenMFC's frame classes embed no
// CFrameImpl (see the file header), so THIS CFrameImpl -- the one whose m_pFrame is
// that frame -- stands in for it.  An OpenMFC-side override of the frame's
// OnShowCustomizePane is not reached.  (AddDefaultButtonsToCustomizePane is itself
// still a stub, so today this path only returns TRUE.)
// Symbol: ?OnShowCustomizePane@CFrameImpl@@IEAAHPEAVCMFCPopupMenu@@I@Z
extern "C" int MS_ABI impl__OnShowCustomizePane_CFrameImpl__IEAAHPEAVCMFCPopupMenu__I_Z(
    void* pThis, void* pMenuPopup, UINT uiToolbarID) {
    CFrameWnd* pFrame = OwnerFrame(pThis);
    if (pFrame == nullptr) {
        return FALSE;
    }
    struct Candidate {
        CRuntimeClass* (MS_ABI* pfnGetThisClass)();
        int nSlot;
    };
    const Candidate candidates[] = {
        {impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ, 124},        // +0x3e0
        {impl__GetThisClass_CFrameWndEx__SAPEAUCRuntimeClass__XZ, 119},           // +0x3b8
        {impl__GetThisClass_COleIPFrameWndEx__SAPEAUCRuntimeClass__XZ, 126},      // +0x3f0
        {impl__GetThisClass_COleDocIPFrameWndEx__SAPEAUCRuntimeClass__XZ, 126},   // +0x3f0
    };
    for (const Candidate& candidate : candidates) {
        if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pFrame, candidate.pfnGetThisClass())) {
            continue;
        }
        if (IsOwnObject(pFrame)) {
            impl__AddDefaultButtonsToCustomizePane_CFrameImpl__IEAAXPEAVCMFCPopupMenu__I_Z(pThis, pMenuPopup, uiToolbarID);
            return TRUE;
        }
        typedef int (MS_ABI* Fn)(void*, void*, UINT);
        return VSlot<Fn>(pFrame, candidate.nSlot)(pFrame, pMenuPopup, uiToolbarID);
    }
    return FALSE;
}
// Transcribed from retail 0x62010:
//     CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;                     // 0x1345b0, +0x8
//     if (!pApp || !pApp->IsKindOf(RUNTIME_CLASS(CWinAppEx))) return;             // 0x233310 against 0x31e618
//     if (m_FullScreenMgr.m_bFullScreen (+0x148) && ::IsWindow(m_pFrame ? m_pFrame->m_hWnd : NULL))
//         m_FullScreenMgr.RestoreState(m_pFrame);                                 // 0x69660 on this+0x128
//     pApp->OnClosingMainFrame(this);                                             // CWinAppEx vftable (ANSI 0x31e370) +0x268 = 0x1c6b60
//     StoreWindowPlacement();                                                     // 0x620b0
// Nothing is torn down here -- the side-table state lives until ??1CFrameImpl.
// DEVIATION: the full-screen flag reads FALSE (IsFullScreen: no CFullScreenImpl
// sub-object is modeled), so RestoreState is never reached; OnClosingMainFrame goes to
// the CWinAppEx thunk, not the app's vtable (a client override is not reached).
// Symbol: ?OnCloseFrame@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCloseFrame_CFrameImpl__IEAAXXZ(void* pThis) {
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        return;
    }
    // retail: if (+0x148 && ::IsWindow(m_pFrame->GetSafeHwnd())) m_FullScreenMgr.RestoreState(m_pFrame);
    // -- not reachable here, IsFullScreen() is the documented FALSE stand-in.
    (void)IsFullScreen();
    impl__OnClosingMainFrame_CWinAppEx__MEAAXPEAVCFrameImpl___Z(static_cast<CWinAppEx*>(pApp), pThis);
    impl__StoreWindowPlacement_CFrameImpl__IEAAXXZ(pThis);
}
// Transcribed from retail 0x623a0:
//     CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;
//     if (pApp && pApp->IsKindOf(RUNTIME_CLASS(CWinAppEx)))
//         pApp->LoadState(NULL, this);                                            // CWinAppEx vftable +0x230 = ?LoadState@CWinAppEx@@UEAAHPEBDPEAVCFrameImpl@@@Z 0x1c6140
//     if (+0x188 && +0x188->m_hWnd)
//         m_pFrame->SetWindowPos(NULL, -1, -1, -1, -1, 0x37);                     // 0x2a7970; SWP_NOSIZE|NOMOVE|NOZORDER|NOACTIVATE|FRAMECHANGED
//     else if (m_pRibbonBar (+0x180) && m_pRibbonBar->m_hWnd)
//         m_pRibbonBar->RecalcLayout();                                           // CMFCRibbonBar vftable (ANSI 0x2fccd8) +0x430 = 0xdd0a0
//     <afxGlobalData Initialize-once prologue>
//     afxGlobalData.+0x274 = (m_pFrame->GetExStyle() & WS_EX_LAYOUTRTL) != 0;    // 0x2a75d0; the public name is m_bIsRTL
// DEVIATION: LoadState / RecalcLayout go to the CWinAppEx / CMFCRibbonBar thunks, not
// the objects' vtables; a NULL frame skips the SetWindowPos and the RTL store.
// Symbol: ?OnLoadFrame@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnLoadFrame_CFrameImpl__IEAAXXZ(void* pThis) {
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        (void)impl__LoadState_CWinAppEx__UEAAHPEB_WPEAVCFrameImpl___Z(static_cast<CWinAppEx*>(pApp), nullptr, pThis);
    }
    CFrameWnd* pFrame = OwnerFrame(pThis);
    CWnd* pRibbonStatusBar = RibbonStatusBar(pThis);
    CMFCRibbonBar* pRibbonBar = RibbonBar(pThis);
    if (pRibbonStatusBar != nullptr && pRibbonStatusBar->m_hWnd != nullptr) {
        if (pFrame != nullptr) {
            (void)impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
                pFrame, nullptr, -1, -1, -1, -1,
                SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
        }
    } else if (pRibbonBar != nullptr && pRibbonBar->m_hWnd != nullptr) {
        impl__RecalcLayout_CMFCRibbonBar__UEAAXXZ(pRibbonBar);
    }
    if (pFrame != nullptr) {
        const BOOL bIsRTL = (impl__GetExStyle_CWnd__QEBAKXZ(pFrame) & WS_EX_LAYOUTRTL) != 0;
        EnsureGlobalDataInitialized();
        SetGlobalDataInt(0x274, bIsRTL);   // m_bIsRTL
    }
}
// NOT a transcription (pre-existing OpenMFC body, kept as-is).  Retail 0x66700 is
// ~230 instructions: it RecalcLayout's a visible caption-replacing ribbon (vftable
// +0x430), re-applies the frame's window region through the visual manager's
// OnSetWindowRegion (+0x400, result stored at +0x24), toggles WS_BORDER via
// CWnd::ModifyStyle (0x2a7600) around the +0x40 re-entrancy flag when +0x28/+0x2c
// allow it, and finishes with SetWindowPos/RedrawWindow of the frame.  None of
// that is reproduced here; this body only refreshes the side-table record and
// redraws the frame and menu bar.
// Symbol: ?OnChangeVisualManager@CFrameImpl@@QEAAXXZ
extern "C" void MS_ABI impl__OnChangeVisualManager_CFrameImpl__QEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }

    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    CFrameWnd* ownerFrame = nullptr;
    CWnd* menuBarWnd = nullptr;

    {
        std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
        FrameImplState& state = g_frameImplStates[frameImpl];
        ownerFrame = state.ownerFrame;
        menuBarWnd = reinterpret_cast<CWnd*>(state.menuBar);
        CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
        state.visualManagerClass = manager != nullptr ? manager->GetRuntimeClass() : nullptr;
    }

    if (ownerFrame != nullptr && ownerFrame->GetSafeHwnd() != nullptr) {
        ownerFrame->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
    }
    if (menuBarWnd != nullptr && menuBarWnd->GetSafeHwnd() != nullptr) {
        menuBarWnd->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
    }
}
// NOT a full transcription (pre-existing OpenMFC body).  Retail 0x63480 stores the
// argument at +0x110 only when +0x110 is still NULL, then caches the frame's HMENU
// (frame vftable +0xe8 = ?GetMenu@CFrameWnd@@ 0x29eee0, its +0x8 m_hMenu) at +0x48,
// calls the menu bar's vftable +0x888 (a bare `ret` in CMFCMenuBar's own table,
// i.e. an override hook) and continues with menu-bar setup.  Here the pointer is
// stored unconditionally and nothing else is done.
// Symbol: ?SetMenuBar@CFrameImpl@@IEAAXPEAVCMFCMenuBar@@@Z
extern "C" void MS_ABI impl__SetMenuBar_CFrameImpl__IEAAXPEAVCMFCMenuBar___Z(void* pThis, void* pMenuBar) {
    if (!pThis) {
        return;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    g_frameImplStates[frameImpl].menuBar = pMenuBar;
}

// Retail's private static frame list: a CList<CFrameWnd*,CFrameWnd*> at RVA 0x3aacc0,
// 56 bytes -- AddFrame/RemoveFrame reach its head at 0x3aacc8, tail at 0x3aacd0,
// count at 0x3aacd8 and free-node head at 0x3aace0, i.e. MFC's usual
// vfptr/head/tail/count/free/blocks/blocksize shape.  OpenMFC exports storage of the
// same size, so anything importing this data export sees an object of the right
// footprint; the live list is the file-local g_pFrameListHead above, exactly
// as OpenMFC's own list classes keep their contents outside the object.
// Symbol: ?m_lstFrames@CFrameImpl@@1V?$CList@PEAVCFrameWnd@@PEAV1@@@A
extern "C" alignas(8) unsigned char impl__m_lstFrames_CFrameImpl__1V__CList_PEAVCFrameWnd__PEAV1___A[56] = {};

// Retail 0x64d40 returns immediately unless ?m_pWndToolBar@CMFCCustomizeMenuButton@@
// (0x3b6f28) is set, then allocates a 0xdb8-byte object and fills the popup from it;
// none of that customization state is modeled here.  STUB.
// Symbol: ?AddDefaultButtonsToCustomizePane@CFrameImpl@@IEAAXPEAVCMFCPopupMenu@@I@Z
extern "C" void MS_ABI impl__AddDefaultButtonsToCustomizePane_CFrameImpl__IEAAXPEAVCMFCPopupMenu__I_Z(
    void* pThis, void* pMenuPopup, unsigned int uiToolbarID) {
    (void)pThis;
    (void)pMenuPopup;
    (void)uiToolbarID;
}

// Transcribed from retail 0x65f40: walk m_lstFrames comparing GetSafeHwnd() of each
// entry against GetSafeHwnd() of the argument (both sides are null-tolerant there --
// retail loads m_hWnd at +0x40 only after testing the pointer), and AddTail only when
// no entry matches.
// Symbol: ?AddFrame@CFrameImpl@@SAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__AddFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(CFrameWnd* pFrame) {
    const HWND hWndNew = impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(pFrame);
    std::lock_guard<std::mutex> lock(g_frameListMutex);
    for (PtrNode* pNode = g_pFrameListHead; pNode != nullptr; pNode = pNode->pNext) {
        if (impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(
                static_cast<const CFrameWnd*>(pNode->pData)) == hWndNew) {
            return;
        }
    }
    PtrListAddTail(g_pFrameListHead, pFrame);
}

// Transcribed from retail 0x62a40, which is a two-instruction tail jump:
//   add $0x98,%rcx ; jmp ?AddTail@CPtrList@@QEAAPEAU__POSITION@@PEAX@Z
// i.e. an unconditional AddTail of the pane onto the tear-off toolbar list, with no
// null check and no duplicate check.
// DEVIATION: the list lives in this file's companion state keyed by `this`, so the
// body adds a `pThis == NULL -> no-op` guard that retail (which would just index off
// a null pointer) does not have.  A NULL pPane is still appended, as retail does.
// Symbol: ?AddTearOffToolbar@CFrameImpl@@IEAAXPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__AddTearOffToolbar_CFrameImpl__IEAAXPEAVCBasePane___Z(void* pThis, void* pPane) {
    if (!pThis) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    PtrListAddTail(g_frameImplExtras[pThis].pTearOffToolbars, pPane);
}

// Transcribed from retail 0x62820 (mfc140; RVA 0x629f0 in mfc140u):
//     if (lpszName == NULL) AfxThrowInvalidArgException();                    // 0x225b80
//     UINT uiNewID = GetFreeCtrlBarID(+0x08, +0x0c, m_lstUserToolbars (+0x60)); // 0x62790
//     if (uiNewID == 0) {
//         CString str; str.Format(0x3f73, +0x0c - +0x08 + 1);                  // CStringT::Format(UINT, ...)
//         AfxMessageBox(str, MB_ICONINFORMATION /*0x40*/, 0);
//         return NULL;
//     }
//     CMFCToolBar* pNewToolbar = (CMFCToolBar*)(+0x178)->CreateObject();      // 0x233380, result not checked
//     if (!pNewToolbar->Create(m_pFrame, 0x50402808, uiNewID)) {             // vftable +0x650
//         delete pNewToolbar;  return NULL;                                     // vftable +0x008, flags 1
//     }
//     pNewToolbar->SetWindowText(lpszName);                                     // ?SetWindowTextW@CWnd@@ 0x2a9790 (mfc140u)
//     pNewToolbar->SetPaneStyle(pNewToolbar->GetPaneStyle() | 0x34);          // +0x390 / +0x3d8
//     pNewToolbar->EnableDocking(0xf000);                                       // +0x3e8
//     CRect rectBar; ::GetWindowRect(pNewToolbar->m_hWnd, &rectBar);           // zero-initialised first
//     int x = ::GetSystemMetrics(SM_CXFULLSCREEN) / 2, y = ::GetSystemMetrics(SM_CYFULLSCREEN) / 2;
//     CRect rectFloat(x, y, x + rectBar.Width(), y + rectBar.Height());
//     pNewToolbar->FloatPane(rectFloat, DM_UNKNOWN, true);                     // +0x408
//     pNewToolbar->m_nMRUWidth (+0x1dc) = 0x7fff;
//     m_pFrame->RecalcLayout(TRUE);                                             // frame vftable +0x300
//     m_lstUserToolbars.AddTail(pNewToolbar);  return pNewToolbar;
// (GetWindowRect / GetSystemMetrics resolved through the mfc140u import slots
// 0x1802c6c08 / 0x1802c6bd8.)  It never reads m_pDockManager (+0x120): nothing is
// docked here.  Toolbar and frame virtuals go through the helpers above the
// namespace end (retail slot for a client object, sibling thunk for an own one).
// DEVIATIONS:
//  * GetFreeCtrlBarID's walk is done inline over this file's user-toolbar list (it
//    is not a CObList here); the logic is that export's, above.
//  * no free id: the message box is NOT shown and NULL is returned.  String 0x3f73
//    lives in retail's resources and OpenMFC's image has no string table, so the
//    retail Format would throw instead of informing the user: in
//    ?Format@...@QEAAXIZZ (entry RVA 0x66db0, mfc140u) a string that cannot be
//    found or loaded branches to 0x66e8c, which loads E_FAIL (0x80004005) into ecx
//    and calls the ATL throw helper at RVA 0x333c (mfc140u); the other exit,
//    0x66e81, does the same when no string manager is available.
//  * a `this` the constructor never registered returns NULL (retail has no such
//    case); a NULL CreateObject result returns NULL; a frame-less `this` skips the
//    RecalcLayout -- in both of the last two retail would dereference NULL.
// Symbol: ?CreateNewToolBar@CFrameImpl@@IEAAPEBVCMFCToolBar@@PEB_W@Z
extern "C" void* MS_ABI impl__CreateNewToolBar_CFrameImpl__IEAAPEBVCMFCToolBar__PEB_W_Z(void* pThis, const wchar_t* lpszName) {
    if (lpszName == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    const UserToolbarState state = ReadUserToolbarState(pThis, nullptr);
    if (!state.bFound) {
        return nullptr;
    }

    // GetFreeCtrlBarID(+0x08, +0x0c, m_lstUserToolbars) over a snapshot of the list,
    // taken so GetDlgCtrlID runs without the companion-state lock held.
    void** pBars = nullptr;
    long long nCount = 0;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra != nullptr) {
            for (PtrNode* pNode = pExtra->pUserToolbars; pNode != nullptr; pNode = pNode->pNext) {
                ++nCount;
            }
            if (nCount != 0) {
                pBars = static_cast<void**>(::malloc(static_cast<size_t>(nCount) * sizeof(void*)));
                long long i = 0;
                for (PtrNode* pNode = pExtra->pUserToolbars; pNode != nullptr && pBars != nullptr; pNode = pNode->pNext) {
                    pBars[i++] = pNode->pData;
                }
            }
        }
    }
    if (nCount != 0 && pBars == nullptr) {
        return nullptr;   // out of memory for the snapshot
    }
    UINT uiNewID = 0;
    if (state.uiFirst != static_cast<UINT>(-1) && state.uiLast != static_cast<UINT>(-1) &&
        static_cast<long long>(static_cast<int>(state.uiLast - state.uiFirst + 1)) != nCount &&
        state.uiFirst <= state.uiLast) {
        for (UINT uiID = state.uiFirst; uiID <= state.uiLast; ++uiID) {
            bool bUsed = false;
            for (long long i = 0; i < nCount; ++i) {
                if (static_cast<UINT>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pBars[i]))) == uiID) {
                    bUsed = true;
                    break;
                }
            }
            if (!bUsed) {
                uiNewID = uiID;
                break;
            }
        }
    }
    ::free(pBars);
    if (uiNewID == 0) {
        return nullptr;   // DEVIATION: retail shows string 0x3f73 first (see above)
    }

    CObject* pObject = impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(state.pRTC);
    if (pObject == nullptr) {
        return nullptr;
    }
    void* pNewToolbar = pObject;
    CFrameWnd* pFrame = OwnerFrame(pThis);
    if (!ToolBarCreate(pNewToolbar, pFrame, 0x50402808, uiNewID)) {
        ToolBarDelete(pNewToolbar);
        return nullptr;
    }
    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(static_cast<CWnd*>(pNewToolbar), lpszName);
    ToolBarOrPaneStyle(pNewToolbar, 0x34);
    ToolBarEnableDocking(pNewToolbar, 0xf000);

    RECT rectBar = {};
    ::GetWindowRect(static_cast<CWnd*>(pNewToolbar)->m_hWnd, &rectBar);
    const int x = ::GetSystemMetrics(SM_CXFULLSCREEN) / 2;
    const int y = ::GetSystemMetrics(SM_CYFULLSCREEN) / 2;
    RECT rectFloat;
    rectFloat.left = x;
    rectFloat.top = y;
    rectFloat.right = x - rectBar.left + rectBar.right;
    rectFloat.bottom = y - rectBar.top + rectBar.bottom;
    (void)ToolBarFloatPane(pNewToolbar, rectFloat, 0 /*DM_UNKNOWN*/, true);

    static_cast<CPane*>(pNewToolbar)->m_nMRUWidth = 0x7fff;
    if (pFrame != nullptr) {
        FrameRecalcLayout(pFrame, TRUE);
    }
    AddUserToolbar(pThis, pNewToolbar);
    return pNewToolbar;
}

// Transcribed from retail 0x646e0:
//     if (CMFCToolBar::m_bCustomizeMode (0x3b70bc) == 0 && CMFCPopupMenu::GetSafeActivePopupMenu() (0xbc470) != NULL
//         && m_pMenuBar (+0x110) != NULL)
//         m_pMenuBar->Deactivate();                          // CMFCMenuBar vftable (ANSI 0x2ece68) +0x6d0 = ?Deactivate@CMFCToolBar@@ 0x156620
//     if (m_pRibbonBar (+0x180) && ::IsWindowVisible(m_pRibbonBar->m_hWnd))
//         m_pRibbonBar->DeactivateKeyboardFocus(FALSE);      // 0xe28e0
// DEVIATION: Deactivate goes to the CMFCToolBar thunk, not the menu bar's vtable.
// Symbol: ?DeactivateMenu@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__DeactivateMenu_CFrameImpl__IEAAXXZ(void* pThis) {
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0 &&
        impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ() != nullptr) {
        CMFCMenuBar* pMenuBar = MenuBar(pThis);
        if (pMenuBar != nullptr) {
            impl__Deactivate_CMFCToolBar__UEAAXXZ(pMenuBar);
        }
    }
    CMFCRibbonBar* pRibbonBar = RibbonBar(pThis);
    if (pRibbonBar != nullptr && ::IsWindowVisible(pRibbonBar->m_hWnd)) {
        impl__DeactivateKeyboardFocus_CMFCRibbonBar__QEAAXH_Z(pRibbonBar, FALSE);
    }
}

// Transcribed from retail 0x63350 (mfc140; RVA 0x63520 in mfc140u):
//     POSITION pos = <the +0x60 node whose data (+0x10) == pToolBar>;  none -> return FALSE;
//     m_lstUserToolbars.RemoveAt(pos);                                          // ?RemoveAt@CPtrList@@ 0x2306f0
//     pToolBar->RemoveStateFromRegistry(m_strUserToolbarsProfile, -1, -1);     // vftable +0x710
//     CDockSite* pDockSite = pToolBar->GetParentDockSite();                     // +0x350
//     CPaneFrameWnd* pMiniFrame = pToolBar->GetParentMiniFrame(FALSE);          // +0x460
//     if (pDockSite != NULL)       pDockSite->RemovePane(pToolBar, DM_UNKNOWN);  // CDockSite vftable +0x508
//     else if (pMiniFrame != NULL) pMiniFrame->RemovePane(pToolBar, FALSE, FALSE);   // CPaneFrameWnd vftable +0x308
//     pToolBar->DestroyWindow();                                                // +0x0d0
//     delete pToolBar;                                                          // +0x008, flags 1
//     m_pFrame->RecalcLayout(TRUE);                                             // frame vftable +0x300
//     return TRUE;
// Both GetParentDockSite and GetParentMiniFrame are called before either is tested.
// Every virtual goes through the helpers above the namespace end (retail slot for a
// client object, sibling thunk for an own one).
// DEVIATIONS: a frame-less `this` skips the RecalcLayout (retail would dereference
// NULL); an own dock site gets the CDockSite::RemovePane thunk (a derived class'
// override is not reached).
// Symbol: ?DeleteToolBar@CFrameImpl@@IEAAHPEAVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__DeleteToolBar_CFrameImpl__IEAAHPEAVCMFCToolBar___Z(void* pThis, void* pToolBar) {
    alignas(CString) unsigned char profileStorage[sizeof(CString)];
    CString* pProfile = new (profileStorage) CString();
    bool bRemoved = false;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra != nullptr) {
            PtrNode** ppLink = &pExtra->pUserToolbars;
            while (*ppLink != nullptr && (*ppLink)->pData != pToolBar) {
                ppLink = &(*ppLink)->pNext;
            }
            if (*ppLink != nullptr) {
                PtrNode* pNode = *ppLink;
                *ppLink = pNode->pNext;
                delete pNode;
                bRemoved = true;
                *pProfile = pExtra->strUserToolbarsProfile;
            }
        }
    }
    if (!bRemoved) {
        pProfile->~CString();
        return FALSE;
    }

    (void)ToolBarRemoveStateFromRegistry(pToolBar, pProfile->GetString(), -1, static_cast<UINT>(-1));
    pProfile->~CString();

    void* pDockSite = ToolBarGetParentDockSite(pToolBar);
    void* pMiniFrame = ToolBarGetParentMiniFrame(pToolBar, FALSE);
    if (pDockSite != nullptr) {
        DockSiteRemovePane(pDockSite, pToolBar, 0 /*DM_UNKNOWN*/);
    } else if (pMiniFrame != nullptr) {
        MiniFrameRemovePane(pMiniFrame, pToolBar, FALSE, FALSE);
    }
    (void)ToolBarDestroyWindow(pToolBar);
    ToolBarDelete(pToolBar);

    CFrameWnd* pFrame = OwnerFrame(pThis);
    if (pFrame != nullptr) {
        FrameRecalcLayout(pFrame, TRUE);
    }
    return TRUE;
}

// Transcribed from retail 0x65ba0.  MSVC x64 passes `this` in rcx and the hidden
// 16-byte CRect return buffer in rdx (0x65bc8: `mov %rdx,%rdi`), and returns that
// buffer in rax (0x65ce4) -- hence the (pThis, ret) parameter order below.
//     CSize szSystemBorder = afxGlobalUtils.GetSystemBorders(m_pFrame);        // 0x6de90
//     int cxBorder = szSystemBorder.cx, cyBorder = szSystemBorder.cy;
//     if (::IsIconic(m_pFrame->m_hWnd) || (m_pFrame->GetStyle() & WS_MAXIMIZE)) cxBorder = cyBorder = 0;
//     RECT rectWnd = {0};  ::GetWindowRect(m_pFrame->m_hWnd, &rectWnd);  m_pFrame->ScreenToClient(&rectWnd);   // 0x2a11f0
//     int cyOffset = cyBorder;  if (!::IsIconic(m_pFrame->m_hWnd)) cyOffset += ::GetSystemMetrics(SM_CYCAPTION);
//     ::OffsetRect(&rectWnd, cxBorder, cyOffset);
//     ret.left   = rectWnd.left + cxBorder;
//     ret.top    = rectWnd.top + cyBorder;
//     ret.right  = rectWnd.right - cxBorder;
//     ret.bottom = rectWnd.top + cyBorder + ::GetSystemMetrics(SM_CYCAPTION);
//     if (::IsIconic(m_pFrame->m_hWnd)) { ret.top += ::GetSystemMetrics(SM_CYSIZEFRAME /*33*/); ret.right -= ::GetSystemMetrics(SM_CXSIZEFRAME /*32*/); }
// (::GetWindowRect is slot 0x2c5370, ::IsIconic 0x2c4ca0, ::OffsetRect 0x2c5318,
// ::GetSystemMetrics 0x2c4c50.)
// DEVIATION: a `this` with no frame returns an empty rect instead of faulting.
// Symbol: ?GetCaptionRect@CFrameImpl@@IEAA?AVCRect@@XZ
extern "C" RECT* MS_ABI impl__GetCaptionRect_CFrameImpl__IEAA_AVCRect__XZ(void* pThis, RECT* ret) {
    if (ret == nullptr) {
        return nullptr;
    }
    ::SetRectEmpty(ret);
    CFrameWnd* pFrame = OwnerFrame(pThis);
    if (pFrame == nullptr) {
        return ret;
    }
    const SIZE szSystemBorder = FrameSystemBorders(pFrame);
    int cxBorder = szSystemBorder.cx;
    int cyBorder = szSystemBorder.cy;
    if (::IsIconic(pFrame->m_hWnd) || (impl__GetStyle_CWnd__QEBAKXZ(pFrame) & WS_MAXIMIZE) != 0) {
        cxBorder = 0;
        cyBorder = 0;
    }
    RECT rectWnd = {};
    ::GetWindowRect(pFrame->m_hWnd, &rectWnd);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pFrame, &rectWnd);
    int cyOffset = cyBorder;
    if (!::IsIconic(pFrame->m_hWnd)) {
        cyOffset += ::GetSystemMetrics(SM_CYCAPTION);
    }
    ::OffsetRect(&rectWnd, cxBorder, cyOffset);
    const int cyCaption = ::GetSystemMetrics(SM_CYCAPTION);
    ret->left = rectWnd.left + cxBorder;
    ret->top = rectWnd.top + cyBorder;
    ret->right = rectWnd.right - cxBorder;
    ret->bottom = rectWnd.top + cyBorder + cyCaption;
    if (::IsIconic(pFrame->m_hWnd)) {
        ret->top += ::GetSystemMetrics(SM_CYSIZEFRAME);
        ret->right -= ::GetSystemMetrics(SM_CXSIZEFRAME);
    }
    return ret;
}

// Transcribed from retail 0x62790, instruction for instruction:
//     if (uiStart == (UINT)-1 || uiEnd == (UINT)-1)                    return 0;
//     if ((INT_PTR)(int)(uiEnd - uiStart + 1) == lst.GetCount())       return 0;
//     if (uiStart > uiEnd)                                             return 0;   // unsigned
//     for (UINT id = uiStart; id <= uiEnd; ++id) {                                 // unsigned
//         BOOL bUsed = FALSE;
//         for (POSITION pos = lst.GetHeadPosition(); pos != NULL; )
//             if ((UINT)((CWnd*)lst.GetNext(pos))->GetDlgCtrlID() == id) { bUsed = TRUE; break; }
//         if (!bUsed) return id;
//     }
//     return 0;
// Retail inlines the whole walk (m_pNodeHead at +0x08, m_nCount at +0x18, node data at
// +0x10, next at +0x00) and the per-element call is ?GetDlgCtrlID@CWnd@@QEBAHXZ
// (RVA 0x2a78b0), reached here through impl__GetDlgCtrlID_CWnd__QEBAHXZ.
// DEVIATION: OpenMFC's CObList keeps its elements in the filecore side table rather
// than in the object, so the walk goes through FindListStorage<CObList,...> instead of
// the inline node chain; a list with no state yet reads as empty (GetCount() == 0).
// It is read without taking openmfc::detail::filecore::g_collectionStateMutex -- that
// symbol is defined in detail/FilecoreSupport.cpp, but naming it from this file adds a
// new C++-mangled undefined that the per-file link audit rejects.  Retail is equally
// unsynchronised here, and every caller is on the UI thread.
// Symbol: ?GetFreeCtrlBarID@CFrameImpl@@KAIIIAEBVCObList@@@Z
extern "C" unsigned int MS_ABI impl__GetFreeCtrlBarID_CFrameImpl__KAIIIAEBVCObList___Z(
    unsigned int uiStart, unsigned int uiEnd, const CObList& lst) {
    if (uiStart == static_cast<unsigned int>(-1) || uiEnd == static_cast<unsigned int>(-1)) {
        return 0;
    }

    typedef CList<CObject*, CObject*> ObListData;
    const ObListData* pData =
        openmfc::detail::filecore::FindListStorage<CObList, CObject*, CObject*>(&lst);
    const INT_PTR nCount = pData != nullptr ? pData->GetCount() : 0;

    if (static_cast<INT_PTR>(static_cast<int>(uiEnd - uiStart + 1)) == nCount) {
        return 0;
    }
    if (uiStart > uiEnd) {
        return 0;
    }

    for (unsigned int uiID = uiStart; uiID <= uiEnd; ++uiID) {
        bool bUsed = false;
        if (pData != nullptr) {
            ObListData::POSITION pos = pData->GetHeadPosition();
            while (pos != ObListData::POSITION(nullptr)) {
                CObject* pObject = pData->GetNext(pos);
                const unsigned int uiBarID = static_cast<unsigned int>(
                    impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pObject)));
                if (uiBarID == uiID) {
                    bUsed = true;
                    break;
                }
            }
        }
        if (!bUsed) {
            return uiID;
        }
    }
    return 0;
}

// Transcribed from retail 0x66480: walk the +0xd0 list from its head node (+0xd8) and
// return the first element whose m_nHit (+0x20) equals nHit; NULL when none matches.
// (The retail loop dereferences each node's element without a null test; the list
// only ever holds objects UpdateCaption created, and the snapshot here skips NULLs.)
// The list is empty in OpenMFC until UpdateCaption is implemented, so this returns
// NULL today.
// Symbol: ?GetSysButton@CFrameImpl@@IEAAPEAVCMFCCaptionButtonEx@@I@Z
extern "C" void* MS_ABI impl__GetSysButton_CFrameImpl__IEAAPEAVCMFCCaptionButtonEx__I_Z(void* pThis, unsigned int nHit) {
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    FrameImplExtra* pExtra = FindExtra(pThis);
    if (pExtra == nullptr) {
        return nullptr;
    }
    SysButtonSnapshot buttons;
    SnapshotSysButtons(*pExtra, buttons);
    for (int i = 0; i < buttons.nCount; ++i) {
        void* p = buttons.pButtons[i];
        if (p != nullptr && static_cast<SysButtonView*>(p)->m_nHit == nHit) {
            return p;
        }
    }
    return nullptr;
}

// Transcribed from retail 0x62640:
//     if (nIndex >= m_lstUserToolbars.GetCount() (+0x78) || nIndex < 0) return NULL;
//     node = head (+0x68); advance nIndex times (node = node->pNext at +0x00);
//     return node ? node->data (+0x10) : NULL;
// The count test is done first, as a signed 64-bit compare of the sign-extended index.
// Symbol: ?GetUserToolBarByIndex@CFrameImpl@@IEBAPEAVCMFCToolBar@@H@Z
extern "C" void* MS_ABI impl__GetUserToolBarByIndex_CFrameImpl__IEBAPEAVCMFCToolBar__H_Z(const void* pThis, int nIndex) {
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    FrameImplExtra* pExtra = FindExtra(pThis);
    if (pExtra == nullptr) {
        return nullptr;
    }
    long long nCount = 0;
    for (PtrNode* pNode = pExtra->pUserToolbars; pNode != nullptr; pNode = pNode->pNext) {
        ++nCount;
    }
    if (static_cast<long long>(nIndex) >= nCount || nIndex < 0) {
        return nullptr;
    }
    PtrNode* pNode = pExtra->pUserToolbars;
    for (int i = 0; i < nIndex && pNode != nullptr; ++i) {
        pNode = pNode->pNext;
    }
    return pNode != nullptr ? pNode->pData : nullptr;
}

// Transcribed from retail 0x626a0 (ANSI twin ?InitUserToolbars@CFrameImpl@@IEAAXPEBDII@Z):
//   if (uiFirst == (UINT)-1 || uiLast == (UINT)-1) return;   // nothing is stored
//   m_uiUserToolbarFirst = uiFirst;  m_uiUserToolbarLast = uiLast;
//   pApp = AfxGetModuleState()->m_pCurrentWinApp;
//   if (!pApp || !pApp->IsKindOf(RUNTIME_CLASS(CWinAppEx))) pApp = NULL;   // null-safe in retail
//   if (lpszRegEntry)   profile.SetString(lpszRegEntry, strlen(lpszRegEntry));
//   else if (pApp)      profile = pApp->GetRegSectionPath(_T(""));
//   else                profile.SetString(_T(""), 0);
// (the runtime-class descriptor at 0x18031e618 reads "CWinAppEx" in the image.)
// DEVIATION: the id pair and the profile name are kept in this file's companion state
// rather than in the object; the app lookup goes through impl__AfxGetApp (OpenMFC's
// AfxGetApp, phase4/src/detail/RegcoreSupport.cpp:59) and the CWinAppEx thunks; and a
// NULL `pThis` is a no-op here, where retail would store through a null pointer.
// Symbol: ?InitUserToolbars@CFrameImpl@@IEAAXPEB_WII@Z
extern "C" void MS_ABI impl__InitUserToolbars_CFrameImpl__IEAAXPEB_WII_Z(
    void* pThis, const wchar_t* lpszRegEntry, unsigned int uiUserToolbarFirst, unsigned int uiUserToolbarLast) {
    if (!pThis) {
        return;
    }
    if (uiUserToolbarFirst == static_cast<unsigned int>(-1) ||
        uiUserToolbarLast == static_cast<unsigned int>(-1)) {
        return;
    }

    CWinAppEx* pAppEx = nullptr;
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        pAppEx = static_cast<CWinAppEx*>(pApp);
    }

    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    FrameImplExtra& extra = g_frameImplExtras[pThis];
    extra.uiUserToolbarFirst = uiUserToolbarFirst;
    extra.uiUserToolbarLast = uiUserToolbarLast;
    if (lpszRegEntry != nullptr) {
        extra.strUserToolbarsProfile = lpszRegEntry;
    } else if (pAppEx != nullptr) {
        alignas(CString) unsigned char storage[sizeof(CString)];
        CString* pPath = reinterpret_cast<CString*>(storage);
        impl__GetRegSectionPath_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
            pPath, pAppEx, L"");
        extra.strUserToolbarsProfile = *pPath;
        pPath->~CString();
    } else {
        extra.strUserToolbarsProfile = L"";
    }
}

// Retail 0x64fc0 (mfc140), decoded:
//     CMFCPopupMenu* pParent = pMenuPopup->GetParentPopupMenu();                 // 0xb7b10
//     if (pParent == NULL) return FALSE;
//     CString strLabel;  ENSURE(strLabel.LoadString(0x427a));                    // throws via 0x225b80 if absent
//     CMFCToolBarMenuButton* pButton = pParent->m_pParentBtn (+0x228);
//     if (pButton != NULL && pButton->m_strText (+0x38).Find(strLabel) == -1) return FALSE;
//     CMFCPopupMenu* pCustomize = pParent->GetParentPopupMenu();
//     return pCustomize != NULL && pCustomize->[+0x19b0] == 1;
// NOT implemented: string 0x427a is one of retail's own resources and OpenMFC's
// image has no string table, so a faithful ENSURE would throw
// CInvalidArgException on every call that reaches it; the label comparison has no
// source of truth on this side.  STUB.
// Symbol: ?IsCustomizePane@CFrameImpl@@IEBAHPEBVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__IsCustomizePane_CFrameImpl__IEBAHPEBVCMFCPopupMenu___Z(const void* pThis, const void* pMenuPopup) {
    (void)pThis;
    (void)pMenuPopup;
    return FALSE;
}

// Transcribed from retail.  The symbol has no entry in the RVA maps because its body
// is ICF-folded: export ordinal 7887 (mfc_complete_ordinal_mapping.json) resolves
// through mfc140u.dll's export address table to RVA 0x3a60 (mfc140u), the two
// instruction `mov $0x1,%eax ; ret` that 101 exports share (the map keeps it under
// ?accDoDefaultAction@CMFCBaseAccessibleObject@@).  Neither `this` nor the CDockState
// is read: the function is `return TRUE;`, as in the public MFC source.
// Symbol: ?IsDockStateValid@CFrameImpl@@IEAAHAEBVCDockState@@@Z
extern "C" int MS_ABI impl__IsDockStateValid_CFrameImpl__IEAAHAEBVCDockState___Z(void* pThis, const void* pDockState) {
    (void)pThis;
    (void)pDockState;
    return TRUE;
}

// Transcribed from retail 0x64680: F1 with no modifier held and not an auto-repeat.
// The three ::GetKeyState calls are the import slot at 0x2c4d20 (USER32!GetKeyState),
// queried in the order VK_SHIFT, VK_CONTROL, VK_MENU; the auto-repeat test is
// `test $0x4000, 0x1a(%rcx)`, i.e. KF_REPEAT in HIWORD(lParam).
// DEVIATION: retail dereferences lpMsg unconditionally; the null guard is ours.
// Symbol: ?IsHelpKey@CFrameImpl@@KAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__IsHelpKey_CFrameImpl__KAHPEAUtagMSG___Z(MSG* lpMsg) {
    if (lpMsg == nullptr) {
        return FALSE;
    }
    if (lpMsg->message != WM_KEYDOWN) {
        return FALSE;
    }
    if (lpMsg->wParam != VK_F1) {
        return FALSE;
    }
    if ((HIWORD(lpMsg->lParam) & KF_REPEAT) != 0) {
        return FALSE;
    }
    if (::GetKeyState(VK_SHIFT) < 0) {
        return FALSE;
    }
    if (::GetKeyState(VK_CONTROL) < 0) {
        return FALSE;
    }
    if (::GetKeyState(VK_MENU) < 0) {
        return FALSE;
    }
    return TRUE;
}

// Retail 0x66ab0 is `m_pDockManager (+0x120) != NULL && m_pDockManager->[+0x208] != 0`
// -- a BOOL that sits directly in front of the print-preview pane list at +0x210 (the
// list ?HideForPrintPreview@CDockingManager@@ 0x4ce00 fills).  The dock-manager pointer
// is the companion-state field the frame files set through
// openmfc_CFrameImpl_SetDockManager; the +0x208 flag has no OpenMFC CDockingManager
// member (see DockManagerIsPrintPreview), so this STILL RETURNS FALSE for every
// caller -- only the null test is real.
// Symbol: ?IsPrintPreview@CFrameImpl@@IEAAHXZ
extern "C" int MS_ABI impl__IsPrintPreview_CFrameImpl__IEAAHXZ(void* pThis) {
    CDockingManager* pDockManager = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        pDockManager = pExtra != nullptr ? pExtra->pDockManager : nullptr;
    }
    return pDockManager != nullptr && DockManagerIsPrintPreview(pDockManager) ? TRUE : FALSE;
}

// Transcribed from retail 0x62670:
//     UINT uiID = (UINT)pToolBar->GetDlgCtrlID();          // ?GetDlgCtrlID@CWnd@@ 0x2a78b0
//     return uiID >= m_uiUserToolbarFirst && uiID <= m_uiUserToolbarLast;   // unsigned
// The two bounds are the pair InitUserToolbars stores; before that call the
// constructor leaves both at (UINT)-1, which makes the range empty for every id but
// (UINT)-1 itself -- retail behaves the same way.
// DEVIATION: retail calls GetDlgCtrlID on the argument without a null check, and it
// reads the bounds straight out of the object; a `pThis` this file has never seen
// (i.e. InitUserToolbars was not called) falls back to the constructor's -1/-1 pair.
// Both NULL `pThis` and NULL `pToolBar` return FALSE here.
// Symbol: ?IsUserDefinedToolbar@CFrameImpl@@IEBAHPEBVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__IsUserDefinedToolbar_CFrameImpl__IEBAHPEBVCMFCToolBar___Z(
    const void* pThis, const void* pToolBar) {
    if (!pThis || !pToolBar) {
        return FALSE;
    }
    const unsigned int uiID =
        static_cast<unsigned int>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(
            static_cast<const CWnd*>(reinterpret_cast<const CMFCToolBar*>(pToolBar))));

    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    auto it = g_frameImplExtras.find(pThis);
    const unsigned int uiFirst =
        it != g_frameImplExtras.end() ? it->second.uiUserToolbarFirst : static_cast<unsigned int>(-1);
    const unsigned int uiLast =
        it != g_frameImplExtras.end() ? it->second.uiUserToolbarLast : static_cast<unsigned int>(-1);
    return (uiID >= uiFirst && uiID <= uiLast) ? TRUE : FALSE;
}

// Transcribed from retail 0x64610:
//     if (m_pDockManager (+0x120) == NULL) return;
//     if (+0x38 == 0) return;
//     m_pDockManager->LoadState(lpszProfileName, m_nIDDefaultResource (+0x10));   // vftable +0xa0
// +0xa0 of the CDockingManager vftable (ANSI 0x2e4070, installed by ??0CDockingManager
// 0x46d40) holds ?LoadState@CDockingManager@@UEAAHPEBDI@Z (0x4a6b0); the BOOL it
// returns is discarded.
// DEVIATION: the call goes through the impl__LoadState_CDockingManager thunk instead
// of the vtable, because OpenMFC's CDockingManager has a different vtable layout; a
// client override of LoadState is therefore not reached.
// Symbol: ?LoadDockState@CFrameImpl@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__LoadDockState_CFrameImpl__IEAAXPEB_W_Z(void* pThis, const wchar_t* lpszProfileName) {
    CDockingManager* pDockManager = nullptr;
    UINT nIDDefaultResource = 0;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra == nullptr || pExtra->pDockManager == nullptr || pExtra->bLoadDockState == 0) {
            return;
        }
        pDockManager = pExtra->pDockManager;
        nIDDefaultResource = pExtra->nIDDefaultResource;
    }
    (void)impl__LoadState_CDockingManager__UEAAHPEB_WI_Z(pDockManager, lpszProfileName, nIDDefaultResource);
}

// Transcribed from retail 0x64750 (which ignores `this` entirely):
//     pApp = AfxGetModuleState()->m_pCurrentWinApp;
//     if (!pApp || !pApp->IsKindOf(RUNTIME_CLASS(CWinAppEx))) return FALSE;
//     return CMFCToolBar::LoadLargeIconsState(pApp->GetRegSectionPath(_T("")));
// The callee is ?LoadLargeIconsState@CMFCToolBar@@SAHPEBD@Z (RVA 0x156b80); OpenMFC's
// own CMFCToolBar::LoadLargeIconsState is itself an un-transcribed stub returning 0,
// so this currently returns FALSE for every caller -- the dispatch, not the result,
// is what is implemented here.
// Symbol: ?LoadLargeIconsState@CFrameImpl@@IEAAHXZ
extern "C" int MS_ABI impl__LoadLargeIconsState_CFrameImpl__IEAAHXZ(void* pThis) {
    (void)pThis;
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr) {
        return FALSE;
    }
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        return FALSE;
    }

    alignas(CString) unsigned char storage[sizeof(CString)];
    CString* pPath = reinterpret_cast<CString*>(storage);
    impl__GetRegSectionPath_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
        pPath, static_cast<CWinAppEx*>(pApp), L"");
    const int bResult = impl__LoadLargeIconsState_CMFCToolBar__SAHPEB_W_Z(pPath->GetString());
    pPath->~CString();
    return bResult;
}

// Retail 0x62a90 (~300 instructions, through 0x62f4d) is the registry counterpart of SaveTearOffMenus:
// it builds "<CWinAppEx::GetRegSectionPath(L"")>ControlBars-TearOff" and, per
// "%Ts-%d" section, opens a CSettingsStore (?Create@CSettingsStoreSP@@ 0x12b320),
// reads the "ID" / "Name" / "State" values (vftable dispatches), recreates the bar
// (CRuntimeClass::CreateObject, then the bar's Create / LoadState virtuals), docks it
// with ?DockPane@CDockingManager@@ (0x483c0) and CPtrList::AddTail's (0x230490) it
// onto the +0x98 list.  Not transcribed: the per-bar virtuals could now go through
// the toolbar dispatch helpers LoadUserToolbars uses, and the stack CSettingsStoreSP
// owner per section can be hand-rolled around ?Create@CSettingsStoreSP@@ (as
// featurepack/toolbar/CMFCToolBar.cpp's SettingsStoreSP does), but the "State"
// value is the bar's serialised state, which OpenMFC's toolbar does not model (its
// Serialize and LoadState are stubs).  The ~300-instruction body was not re-decoded
// end to end for this note.  STUB.
// Symbol: ?LoadTearOffMenus@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__LoadTearOffMenus_CFrameImpl__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Transcribed from retail 0x62490 (mfc140; RVA 0x62660 in mfc140u):
//     if (+0x178 == NULL) AfxThrowInvalidArgException();                        // 0x225b80
//     if (+0x08 == -1 || +0x0c == -1 || +0x08 > +0x0c) return;                   // unsigned compare
//     for (UINT uiID = +0x08; uiID <= +0x0c; ++uiID) {                          // +0x0c re-read every pass
//         CMFCToolBar* pToolBar = (CMFCToolBar*)(+0x178)->CreateObject();       // 0x233380, not checked
//         if (!pToolBar->Create(m_pFrame, 0x50402808, uiID)) { delete pToolBar; continue; }   // vftable +0x650, +0x008
//         if (!pToolBar->LoadState(+0x108, -1, -1)) {                            // +0x468
//             pToolBar->DestroyWindow();  delete pToolBar;  continue;           // +0x0d0, +0x008
//         }
//         pToolBar->SetPaneStyle(pToolBar->GetPaneStyle() | 0x34);             // +0x390 / +0x3d8
//         pToolBar->EnableDocking(0xf000);                                       // +0x3e8
//         m_pDockManager (+0x120)->DockPane(pToolBar, 0, NULL);                  // 0x483c0
//         m_lstUserToolbars.AddTail(pToolBar);                                   // ?AddTail@CPtrList@@ 0x230490
//     }
// Every toolbar virtual goes through the helpers above the namespace end (retail
// slot for a client object, sibling thunk for an own one).  NOTE: OpenMFC's own
// ?LoadState@CMFCToolBar@@ is still a stub returning FALSE, so for the default
// CMFCToolBar runtime class every bar is created and then destroyed again -- which
// is exactly retail's path for a toolbar with no saved state.
// The +0x178 NULL test cannot fire here: UserToolBarRTC() supplies the constructor's
// CMFCToolBar default, and SetNewUserToolBarRTC refuses NULL as retail does.
// DEVIATIONS: a NULL CreateObject result skips that id; with no dock manager
// registered (openmfc_CFrameImpl_SetDockManager) the DockPane call is skipped rather
// than made on NULL; a `this` the constructor never saw loads nothing; the loop
// also stops if the id wraps past 0xffffffff (only reachable if +0x0c is rewritten
// to -1 mid-loop, where retail would not terminate).
// Symbol: ?LoadUserToolbars@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__LoadUserToolbars_CFrameImpl__IEAAXXZ(void* pThis) {
    alignas(CString) unsigned char profileStorage[sizeof(CString)];
    CString* pProfile = new (profileStorage) CString();
    UserToolbarState state = ReadUserToolbarState(pThis, pProfile);
    if (!state.bFound || state.uiFirst == static_cast<UINT>(-1) || state.uiLast == static_cast<UINT>(-1) ||
        state.uiFirst > state.uiLast) {
        pProfile->~CString();
        return;
    }
    // Retail re-reads +0x0c after every pass and +0x178 / +0x118 / +0x108 / +0x120
    // inside it, so the state is refreshed after each pass (nothing runs between that
    // refresh and the next pass' reads).
    UINT uiID = state.uiFirst;
    for (;;) {
        CFrameWnd* pFrame = OwnerFrame(pThis);
        CObject* pObject = impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(state.pRTC);
        if (pObject != nullptr) {
            void* pToolBar = pObject;
            if (!ToolBarCreate(pToolBar, pFrame, 0x50402808, uiID)) {
                ToolBarDelete(pToolBar);
            } else if (!ToolBarLoadState(pToolBar, pProfile->GetString(), -1, static_cast<UINT>(-1))) {
                (void)ToolBarDestroyWindow(pToolBar);
                ToolBarDelete(pToolBar);
            } else {
                ToolBarOrPaneStyle(pToolBar, 0x34);
                ToolBarEnableDocking(pToolBar, 0xf000);
                if (state.pDockManager != nullptr) {
                    impl__DockPane_CDockingManager__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
                        state.pDockManager, static_cast<CBasePane*>(pToolBar), 0, nullptr);
                }
                AddUserToolbar(pThis, pToolBar);
            }
        }
        ++uiID;
        state = ReadUserToolbarState(pThis, pProfile);
        // (uiID == 0: the id wrapped; retail's `inc; cmp; jbe` would keep going.)
        if (!state.bFound || uiID == 0 || uiID > state.uiLast) {
            break;
        }
    }
    pProfile->~CString();
}

// Transcribed from retail 0x65890:
//     if (+0x30 != 0) return;
//     CMFCVisualManager::GetInstance()->OnActivateApp(m_pFrame, bActive);   // vftable +0x408
//     if (bActive) return;
//     if (m_pRibbonBar (+0x180) && ::IsWindowVisible(m_pRibbonBar->m_hWnd)) {
//         m_pRibbonBar->HideKeyTips();      // 0xe4050
//         m_pRibbonBar->OnCancelMode();     // 0xdcf20
//     }
//     if (afxGlobalData.+0x280 == 0 && afxGlobalData.+0x27c != 0) {
//         afxGlobalData.+0x27c = 0;  CMFCToolBar::RedrawUnderlines();     // 0x159fd0
//     }
// (each afxGlobalData read is preceded by the inline Initialize-once prologue).
// +0x280 / +0x27c are the pair ?OnSettingChange@AFX_GLOBAL_DATA@@ (0x6aeb0) fills at
// 0x6af9f..0x6afb6 with SystemParametersInfo(SPI_GETKEYBOARDCUES, 0, &+0x280, 0)
// followed by `+0x27c = +0x280` -- the public MFC names are
// m_bSysUnderlineKeyboardShortcuts and m_bUnderlineKeyboardShortcuts.
// DEVIATION: the manager call goes to the base-class thunk rather than through the
// manager's vtable, so a derived manager's override is not reached (OpenMFC's
// CMFCVisualManager declares no such virtual); the base body is a bare `ret`.
// Symbol: ?OnActivateApp@CFrameImpl@@IEAAXH@Z
extern "C" void MS_ABI impl__OnActivateApp_CFrameImpl__IEAAXH_Z(void* pThis, int bActive) {
    if (OleInPlaceActive(pThis) != 0) {
        return;
    }
    impl__OnActivateApp_CMFCVisualManager__UEAAXPEAVCWnd__H_Z(VisualManager(), OwnerFrame(pThis), bActive);
    if (bActive) {
        return;
    }
    CMFCRibbonBar* pRibbonBar = RibbonBar(pThis);
    if (pRibbonBar != nullptr && ::IsWindowVisible(pRibbonBar->m_hWnd)) {
        impl__HideKeyTips_CMFCRibbonBar__QEAAXXZ(pRibbonBar);
        impl__OnCancelMode_CMFCRibbonBar__IEAAXXZ(pRibbonBar);
    }
    EnsureGlobalDataInitialized();
    if (GlobalDataInt(0x280) != 0) {   // m_bSysUnderlineKeyboardShortcuts
        return;
    }
    if (GlobalDataInt(0x27c) == 0) {   // m_bUnderlineKeyboardShortcuts
        return;
    }
    SetGlobalDataInt(0x27c, 0);
    impl__RedrawUnderlines_CMFCToolBar__SAXXZ();
}

// Transcribed from retail 0x66ad0:
//     <afxGlobalData Initialize-once prologue>
//     <static at 0x3bb96c> = 0;            // the store ?ResetCheckCompositionFlag@AFX_GLOBAL_DATA@@ (0x6c6f0) consists of, inlined
//     if (m_pRibbonBar (+0x180) && ::IsWindowVisible(m_pRibbonBar->m_hWnd) && m_pRibbonBar->m_bReplaceFrameCaption (+0x468))
//         m_pRibbonBar->DWMCompositionChanged();   // vftable +0x6a8 of the CMFCRibbonBar vftable (ANSI 0x2fccd8) = 0xe20a0
//     OnChangeVisualManager();                     // tail jump to 0x66700
// The 0x3bb96c static is the "composition state already checked" flag that
// ?IsDwmCompositionEnabled@AFX_GLOBAL_DATA@@ (0x6c260) tests first; it lives outside
// the afxGlobalData object, so the export is the way to clear it from here.
// DEVIATION: DWMCompositionChanged is reached through the CMFCRibbonBar thunk, not
// the ribbon's vtable; and OpenMFC's ResetCheckCompositionFlag / IsDwmCompositionEnabled
// exports (core/runtime/AFX_GLOBAL_DATA.cpp) are still an empty stub and a constant
// FALSE, so the reset is a no-op on this side until that file models the flag.
// Symbol: ?OnCompositionChanged@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCompositionChanged_CFrameImpl__IEAAXXZ(void* pThis) {
    EnsureGlobalDataInitialized();
    impl__ResetCheckCompositionFlag_AFX_GLOBAL_DATA__QEAAXXZ();
    CMFCRibbonBar* pRibbonBar = RibbonBar(pThis);
    if (pRibbonBar != nullptr && ::IsWindowVisible(pRibbonBar->m_hWnd) &&
        pRibbonBar->m_bReplaceFrameCaption != 0) {
        impl__DWMCompositionChanged_CMFCRibbonBar__UEAAXXZ(pRibbonBar);
    }
    impl__OnChangeVisualManager_CFrameImpl__QEAAXXZ(pThis);
}

// Transcribed from retail 0x65300:
//     if (!lpMMI) AfxThrowInvalidArgException();                                   // 0x225b80
//     BOOL bRibbonCaption = <RibbonReplacesCaption>;                              // the +0x180/+0x148/+0x468 chain at 0x65335..0x65384
//     DWORD dwStyle = m_pFrame->GetStyle();                                        // 0x2a75a0
//     if ((dwStyle & WS_CAPTION) && (m_pFrame->GetStyle() & WS_BORDER) && !bRibbonCaption) return;
//     RECT rectWnd = {0};  ::GetWindowRect(m_pFrame->m_hWnd, &rectWnd);
//     if (::IsIconic(m_pFrame->m_hWnd)) { WINDOWPLACEMENT wp; wp.length = sizeof wp; ::GetWindowPlacement(hwnd, &wp); ::CopyRect(&rectWnd, &wp.rcNormalPosition); }
//     POINT ptCenter = { (rectWnd.left + rectWnd.right) / 2, (rectWnd.top + rectWnd.bottom) / 2 };
//     MONITORINFO mi; mi.cbSize = sizeof mi;  RECT rectWork = {0};
//     if (::GetMonitorInfo(::MonitorFromPoint(ptCenter, MONITOR_DEFAULTTONEAREST), &mi)) {
//         rectWork = mi.rcWork;  ::OffsetRect(&rectWork, -mi.rcMonitor.left, -mi.rcMonitor.top);   // computed field by field at 0x65478
//     } else ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectWork, 0);
//     if (bRibbonCaption && afxGlobalData.IsDwmCompositionEnabled()) ::OffsetRect(&rectWork, -rectWork.left, -rectWork.top);
//     int nBars = afxGlobalData.GetShellAutohideBars();                            // 0x61c2c, see GlobalDataShellAutohideBars
//     if (nBars & 8) rectWork.bottom -= 2;  if (nBars & 4) rectWork.top += 2;
//     if (nBars & 2) rectWork.right -= 2;   if (nBars & 1) rectWork.left += 2;
//     lpMMI->ptMaxPosition = rectWork.TopLeft();  lpMMI->ptMaxSize = rectWork.Size();
//     if (m_pRibbonBar && m_pRibbonBar->m_hWnd && m_pRibbonBar->+0x468 && !afxGlobalData.IsDwmCompositionEnabled()) {
//         lpMMI->ptMinTrackSize.x = ::GetSystemMetrics(SM_CXMINTRACK);  lpMMI->ptMinTrackSize.y = ::GetSystemMetrics(SM_CYMINTRACK);
//     }
// (slots: GetWindowRect 0x2c5370, IsIconic 0x2c4ca0, GetWindowPlacement 0x2c52c0,
// CopyRect 0x2c5240, MonitorFromPoint 0x2c4cc0, GetMonitorInfoA 0x2c4cb8 (-> W),
// SystemParametersInfoA 0x2c4cc8 (-> W), OffsetRect 0x2c5318, GetSystemMetrics 0x2c4c50.)
// DEVIATION: a NULL frame returns instead of faulting; the full-screen flag and the
// shell-autohide recompute are the documented stand-ins (IsFullScreen,
// GlobalDataShellAutohideBars).
// Symbol: ?OnGetMinMaxInfo@CFrameImpl@@IEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CFrameImpl__IEAAXPEAUtagMINMAXINFO___Z(void* pThis, MINMAXINFO* lpMMI) {
    if (lpMMI == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    CFrameWnd* pFrame = OwnerFrame(pThis);
    if (pFrame == nullptr) {
        return;
    }
    CMFCRibbonBar* pRibbonBar = RibbonBar(pThis);
    const BOOL bRibbonCaption = RibbonReplacesCaption(pRibbonBar, pFrame->m_hWnd);

    if ((impl__GetStyle_CWnd__QEBAKXZ(pFrame) & WS_CAPTION) != 0 &&
        (impl__GetStyle_CWnd__QEBAKXZ(pFrame) & WS_BORDER) != 0 && !bRibbonCaption) {
        return;
    }

    RECT rectWnd = {};
    ::GetWindowRect(pFrame->m_hWnd, &rectWnd);
    if (::IsIconic(pFrame->m_hWnd)) {
        WINDOWPLACEMENT wp;
        wp.length = sizeof(wp);
        ::GetWindowPlacement(pFrame->m_hWnd, &wp);
        ::CopyRect(&rectWnd, &wp.rcNormalPosition);
    }
    POINT ptCenter;
    ptCenter.x = (rectWnd.left + rectWnd.right) / 2;
    ptCenter.y = (rectWnd.top + rectWnd.bottom) / 2;

    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    RECT rectWork = {};
    if (::GetMonitorInfo(::MonitorFromPoint(ptCenter, MONITOR_DEFAULTTONEAREST), &mi)) {
        rectWork.left = mi.rcWork.left - mi.rcMonitor.left;
        rectWork.top = mi.rcWork.top - mi.rcMonitor.top;
        rectWork.right = mi.rcWork.right - mi.rcMonitor.left;
        rectWork.bottom = mi.rcWork.bottom - mi.rcMonitor.top;
    } else {
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectWork, 0);
    }

    if (bRibbonCaption) {
        EnsureGlobalDataInitialized();
        if (impl__IsDwmCompositionEnabled_AFX_GLOBAL_DATA__QEAAHXZ()) {
            ::OffsetRect(&rectWork, -rectWork.left, -rectWork.top);
        }
    }
    EnsureGlobalDataInitialized();
    const int nAutohideBars = GlobalDataShellAutohideBars();
    if (nAutohideBars & 8) {
        rectWork.bottom -= 2;
    }
    if (nAutohideBars & 4) {
        rectWork.top += 2;
    }
    if (nAutohideBars & 2) {
        rectWork.right -= 2;
    }
    if (nAutohideBars & 1) {
        rectWork.left += 2;
    }
    lpMMI->ptMaxPosition.x = rectWork.left;
    lpMMI->ptMaxPosition.y = rectWork.top;
    lpMMI->ptMaxSize.x = rectWork.right - rectWork.left;
    lpMMI->ptMaxSize.y = rectWork.bottom - rectWork.top;

    if (pRibbonBar != nullptr && pRibbonBar->m_hWnd != nullptr && pRibbonBar->m_bReplaceFrameCaption != 0) {
        EnsureGlobalDataInitialized();
        if (!impl__IsDwmCompositionEnabled_AFX_GLOBAL_DATA__QEAAHXZ()) {
            lpMMI->ptMinTrackSize.x = ::GetSystemMetrics(SM_CXMINTRACK);
            lpMMI->ptMinTrackSize.y = ::GetSystemMetrics(SM_CYMINTRACK);
        }
    }
}

// Transcribed from retail 0x662c0 (the CPoint argument is never read):
//     UINT nHot = m_nHotSysButton (+0x14);  if (nHot == 0) return;
//     walk the +0xd0 list for the button whose m_nHit (+0x20) == nHot; none -> return;
//     if (pButton == NULL) return;                       // test at 0x662ed
//     m_nHitSysButton (+0x18) = nHot;  pButton->m_bPushed (+0x08) = TRUE;
//     RedrawCaptionButton(pButton);                      // 0x66660
// Symbol: ?OnLButtonDown@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CFrameImpl__IEAAXVCPoint___Z(void* pThis, long long point) {
    (void)point;
    void* pButton = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra == nullptr || pExtra->uiHotSysButton == 0) {
            return;
        }
        const UINT nHot = pExtra->uiHotSysButton;
        SysButtonSnapshot buttons;
        SnapshotSysButtons(*pExtra, buttons);
        for (int i = 0; i < buttons.nCount; ++i) {
            void* p = buttons.pButtons[i];
            if (p != nullptr && static_cast<SysButtonView*>(p)->m_nHit == nHot) {
                pButton = p;
                break;
            }
        }
        if (pButton == nullptr) {
            return;
        }
        pExtra->uiHitSysButton = nHot;
        static_cast<SysButtonView*>(pButton)->m_bPushed = TRUE;
    }
    impl__RedrawCaptionButton_CFrameImpl__IEAAXPEAVCMFCCaptionButtonEx___Z(pThis, pButton);
}

// Transcribed from retail 0x66310 (the CPoint argument is never read):
//     if (!vm->IsOwnerDrawCaption()) return;             // vftable +0x3f8, see the map
//     if (+0x30 != 0 || +0x20 != 0) return;
//     UINT nHit = m_nHitSysButton (+0x18);
//     if (nHit != 19 && nHit != 26 && nHit != 27) return;   // the sub $0x13 / sub $7 / cmp $1 chain
//     UINT nHot = m_nHotSysButton (+0x14);               // read BEFORE the next call
//     StopCaptionButtonsTracking();                      // 0x665b0
//     if (nHot != nHit) return;
//     WPARAM sc = nHot == 19 ? SC_CLOSE (0xf060)
//               : nHot == 26 ? ((m_pFrame->GetStyle() & WS_MAXIMIZE) ? SC_RESTORE (0xf120) : SC_MAXIMIZE (0xf030))
//               : nHot == 27 ? (::IsIconic(m_pFrame->m_hWnd) ? SC_RESTORE : SC_MINIMIZE (0xf020))
//               : 0;
//     ::PostMessage(m_pFrame->m_hWnd, WM_SYSCOMMAND, sc, 0);   // slot 0x2c52d8 = PostMessageA (-> W in mfc140u)
// 19 / 26 / 27 are the m_nHit codes retail's own caption buttons carry (close /
// maximize / minimize, going by the WM_SYSCOMMAND each one maps to); they are not
// USER32 HT* values.  ?GetStyle@CWnd@@ is 0x2a75a0, ::IsIconic is slot 0x2c4ca0.
// Because OpenMFC's visual manager cannot answer TRUE to the +0x3f8 predicate (see
// VisualManagerIsOwnerDrawCaption), the first test returns here today.
// Symbol: ?OnLButtonUp@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CFrameImpl__IEAAXVCPoint___Z(void* pThis, long long point) {
    (void)point;
    if (!VisualManagerIsOwnerDrawCaption()) {
        return;
    }
    UINT nHit = 0;
    UINT nHot = 0;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra == nullptr) {
            return;
        }
        if (pExtra->bOleInPlaceActive != 0 || pExtra->nGate20 != 0) {
            return;
        }
        nHit = pExtra->uiHitSysButton;
        nHot = pExtra->uiHotSysButton;
    }
    if (nHit != 19 && nHit != 26 && nHit != 27) {
        return;
    }
    impl__StopCaptionButtonsTracking_CFrameImpl__IEAAXXZ(pThis);
    if (nHot != nHit) {
        return;
    }
    CFrameWnd* pFrame = OwnerFrame(pThis);
    if (pFrame == nullptr) {
        return;   // DEVIATION: retail would fault on m_pFrame->m_hWnd here
    }
    WPARAM wSysCommand = 0;
    if (nHot == 19) {
        wSysCommand = SC_CLOSE;
    } else if (nHot == 26) {
        wSysCommand = (impl__GetStyle_CWnd__QEBAKXZ(pFrame) & WS_MAXIMIZE) ? SC_RESTORE : SC_MAXIMIZE;
    } else if (nHot == 27) {
        wSysCommand = ::IsIconic(pFrame->m_hWnd) ? SC_RESTORE : SC_MINIMIZE;
    }
    ::PostMessage(pFrame->m_hWnd, WM_SYSCOMMAND, wSysCommand, 0);
}

// Transcribed from retail 0x64490:
//     if (m_pRibbonBar (+0x180) && (m_pRibbonBar->GetStyle() & WS_VISIBLE))
//         if (m_pRibbonBar->TranslateChar(nChar)) return TRUE;         // CMFCRibbonBar vftable +0x6b8 = 0xe2870
//     BOOL bPrintPreview = m_pDockManager (+0x120) && m_pDockManager->+0x208;
//     if (!bPrintPreview && m_pMenuBar (+0x110) && (m_pMenuBar->GetStyle() & WS_VISIBLE))
//         if (m_pMenuBar->TranslateChar(nChar)) return TRUE;           // CMFCMenuBar vftable +0x738 = ?TranslateChar@CMFCToolBar@@ 0x157bd0
//     for (node = CMFCToolBar::m_lstAllToolbars.head (0x3ab098; the list object at 0x3ab090 is what
//          ?GetAllToolbars@CMFCToolBar@@ 0x157c70 returns); node; node = node->next) {
//         pToolBar = node->data;  if (!pToolBar) AfxThrowInvalidArgException();      // 0x225b80
//         if (bPrintPreview && !pToolBar->IsKindOf(<CRuntimeClass 0x2f77c8, name "CMFCPrintPreviewToolBar">)) continue;
//         if (CWnd::FromHandlePermanent(pToolBar->m_hWnd) == NULL) continue;       // 0x2891d0
//         if (pToolBar == m_pMenuBar) continue;
//         if (!(pToolBar->GetStyle() & WS_VISIBLE)) continue;
//         if (pToolBar->GetTopLevelFrame() != m_pFrame) continue;                  // 0x28c910
//         if (pToolBar->TranslateChar(nChar)) return TRUE;                         // vftable +0x738
//     }
//     return FALSE;
// DEVIATION: the print-preview flag reads FALSE here (DockManagerIsPrintPreview, no
// such CDockingManager member in OpenMFC), so the CMFCPrintPreviewToolBar filter never
// engages; the TranslateChar calls go to the CMFCRibbonBar / CMFCToolBar thunks rather
// than the objects' vtables; the toolbar list is read through FindListStorage.
// Symbol: ?OnMenuChar@CFrameImpl@@IEAAHI@Z
extern "C" int MS_ABI impl__OnMenuChar_CFrameImpl__IEAAHI_Z(void* pThis, unsigned int nChar) {
    CMFCRibbonBar* pRibbonBar = nullptr;
    CDockingManager* pDockManager = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra != nullptr) {
            pRibbonBar = pExtra->pRibbonBar;
            pDockManager = pExtra->pDockManager;
        }
    }
    if (pRibbonBar != nullptr && (impl__GetStyle_CWnd__QEBAKXZ(pRibbonBar) & WS_VISIBLE) != 0) {
        if (impl__TranslateChar_CMFCRibbonBar__UEAAHI_Z(pRibbonBar, nChar)) {
            return TRUE;
        }
    }
    const BOOL bPrintPreview = pDockManager != nullptr && DockManagerIsPrintPreview(pDockManager);
    CMFCMenuBar* pMenuBar = MenuBar(pThis);
    if (!bPrintPreview && pMenuBar != nullptr && (impl__GetStyle_CWnd__QEBAKXZ(pMenuBar) & WS_VISIBLE) != 0) {
        if (impl__TranslateChar_CMFCToolBar__UEAAHI_Z(pMenuBar, nChar)) {
            return TRUE;
        }
    }
    CFrameWnd* pFrame = OwnerFrame(pThis);
    typedef CList<CObject*, CObject*> ObListData;
    const ObListData* pData = openmfc::detail::filecore::FindListStorage<CObList, CObject*, CObject*>(
        impl__GetAllToolbars_CMFCToolBar__SAAEBVCObList__XZ());
    if (pData == nullptr) {
        return FALSE;
    }
    ObListData::POSITION pos = pData->GetHeadPosition();
    while (pos != ObListData::POSITION(nullptr)) {
        CMFCToolBar* pToolBar = static_cast<CMFCToolBar*>(pData->GetNext(pos));
        if (pToolBar == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return FALSE;
        }
        if (bPrintPreview) {
            // Unreachable while DockManagerIsPrintPreview is FALSE; retail filters on
            // IsKindOf(RUNTIME_CLASS(CMFCPrintPreviewToolBar)) here.
            continue;
        }
        if (impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(pToolBar->m_hWnd) == nullptr) {
            continue;
        }
        if (pToolBar == pMenuBar) {
            continue;
        }
        if ((impl__GetStyle_CWnd__QEBAKXZ(pToolBar) & WS_VISIBLE) == 0) {
            continue;
        }
        if (impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pToolBar) != pFrame) {
            continue;
        }
        if (impl__TranslateChar_CMFCToolBar__UEAAHI_Z(pToolBar, nChar)) {
            return TRUE;
        }
    }
    return FALSE;
}

// Transcribed from retail 0x66410:
//     if (!vm->IsOwnerDrawCaption()) return;             // vftable +0x3f8
//     if (+0x30 != 0) return;
//     POINT pt = point;  ::ClientToScreen(m_pFrame->m_hWnd, &pt);   // slot 0x2c5270
//     OnTrackCaptionButtons(pt);                         // 0x664d0
// The +0x3f8 predicate is FALSE for every OpenMFC manager (VisualManagerIsOwnerDrawCaption).
// DEVIATION: a NULL frame (a `this` the ctor thunk never saw) returns instead of faulting.
// Symbol: ?OnMouseMove@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CFrameImpl__IEAAXVCPoint___Z(void* pThis, long long point) {
    if (!VisualManagerIsOwnerDrawCaption()) {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra != nullptr && pExtra->bOleInPlaceActive != 0) {
            return;
        }
    }
    CFrameWnd* pFrame = OwnerFrame(pThis);
    if (pFrame == nullptr) {
        return;
    }
    POINT pt = UnpackPoint(point);
    ::ClientToScreen(pFrame->m_hWnd, &pt);
    impl__OnTrackCaptionButtons_CFrameImpl__IEAAXVCPoint___Z(pThis, PackSize(pt.x, pt.y));
}

// Transcribed from retail 0x659d0:
//     if (+0x30 != 0) return FALSE;
//     if (!bActive && m_pRibbonBar (+0x180) && ::IsWindowVisible(m_pRibbonBar->m_hWnd)) {
//         m_pRibbonBar->HideKeyTips();                      // 0xe4050
//         m_pRibbonBar->DeactivateKeyboardFocus(FALSE);     // 0xe28e0
//     }
//     if (!::IsWindowVisible(m_pFrame->m_hWnd)) return FALSE;
//     BOOL bRes = CMFCVisualManager::GetInstance()->OnNcActivate(m_pFrame, bActive);   // vftable +0x418
//     BOOL bRibbonRedrawn = FALSE;
//     if (bRes && m_pRibbonBar && ::IsWindowVisible(m_pRibbonBar->m_hWnd) && m_pRibbonBar->+0x468) {
//         ::RedrawWindow(m_pRibbonBar->m_hWnd, NULL, NULL, 0x585);   // RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_UPDATENOW|RDW_FRAME
//         ::RedrawWindow(m_pFrame->m_hWnd, NULL, NULL, 0x501);       // RDW_INVALIDATE|RDW_UPDATENOW|RDW_FRAME
//         bRibbonRedrawn = TRUE;
//     }
//     if (+0x188 && +0x188->m_hWnd) ::RedrawWindow(+0x188->m_hWnd, NULL, NULL, 0x105);   // RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW
//     if (!bRibbonRedrawn && vm->IsOwnerDrawCaption() (+0x3f8) && +0x30 == 0) {
//         RECT rcEmpty = {0};  ::RedrawWindow(m_pFrame->m_hWnd, &rcEmpty, NULL, 0x541);   // RDW_INVALIDATE|RDW_NOCHILDREN|RDW_UPDATENOW|RDW_FRAME
//     }
//     if (!bRes) return FALSE;
//     return afxGlobalData.IsDwmCompositionEnabled() ? FALSE : TRUE;   // 0x6c260, after the Initialize-once prologue
// DEVIATION: the manager's OnNcActivate is the base-class thunk (returns FALSE), not
// a vtable dispatch; a NULL frame returns FALSE instead of faulting.
// Symbol: ?OnNcActivate@CFrameImpl@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CFrameImpl__IEAAHH_Z(void* pThis, int bActive) {
    if (OleInPlaceActive(pThis) != 0) {
        return FALSE;
    }
    CMFCRibbonBar* pRibbonBar = RibbonBar(pThis);
    if (!bActive && pRibbonBar != nullptr && ::IsWindowVisible(pRibbonBar->m_hWnd)) {
        impl__HideKeyTips_CMFCRibbonBar__QEAAXXZ(pRibbonBar);
        impl__DeactivateKeyboardFocus_CMFCRibbonBar__QEAAXH_Z(pRibbonBar, FALSE);
    }
    CFrameWnd* pFrame = OwnerFrame(pThis);
    if (pFrame == nullptr || !::IsWindowVisible(pFrame->m_hWnd)) {
        return FALSE;
    }
    const BOOL bRes = impl__OnNcActivate_CMFCVisualManager__UEAAHPEAVCWnd__H_Z(VisualManager(), pFrame, bActive);
    BOOL bRibbonRedrawn = FALSE;
    if (bRes && pRibbonBar != nullptr && ::IsWindowVisible(pRibbonBar->m_hWnd) &&
        pRibbonBar->m_bReplaceFrameCaption != 0) {
        ::RedrawWindow(pRibbonBar->m_hWnd, nullptr, nullptr,
                       RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_FRAME);
        ::RedrawWindow(pFrame->m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME);
        bRibbonRedrawn = TRUE;
    }
    CWnd* pRibbonStatusBar = RibbonStatusBar(pThis);
    if (pRibbonStatusBar != nullptr && pRibbonStatusBar->m_hWnd != nullptr) {
        ::RedrawWindow(pRibbonStatusBar->m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
    if (!bRibbonRedrawn && VisualManagerIsOwnerDrawCaption() && OleInPlaceActive(pThis) == 0) {
        RECT rcEmpty = {};
        ::RedrawWindow(pFrame->m_hWnd, &rcEmpty, nullptr, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_UPDATENOW | RDW_FRAME);
    }
    if (!bRes) {
        return FALSE;
    }
    EnsureGlobalDataInitialized();
    return impl__IsDwmCompositionEnabled_AFX_GLOBAL_DATA__QEAAHXZ() ? FALSE : TRUE;
}

// Transcribed from retail 0x655f0 (bCalcValidRects is never read):
//     if (!lpncsp) AfxThrowInvalidArgException();
//     CSize szSystemBorder = afxGlobalUtils.GetSystemBorders(m_pFrame);            // 0x6de90
//     BOOL bRibbonCaption = FALSE, bResult = FALSE;
//     if (<RibbonReplacesCaption>) {                                               // +0x180/+0x40/+0x148/+0x468 chain at 0x6562f..0x6568f
//         bRibbonCaption = TRUE;
//         if (afxGlobalData.IsDwmCompositionEnabled()) {
//             BOOL bSkip = FALSE;
//             if ((m_pFrame->GetStyle() & WS_MAXIMIZE) || +0x148 != 0)
//                 bSkip = afxGlobalData.GetShellAutohideBars() != 0 && ::IsWindowVisible(m_pFrame->m_hWnd);
//             if (!bSkip) { lpncsp->rgrc[0].left += szSystemBorder.cx; rgrc[0].right -= cx; rgrc[0].bottom -= cy; }
//             bResult = TRUE;
//         }
//     }
//     if (+0x188 && +0x188->m_hWnd && (::IsWindowVisible(+0x188->m_hWnd) || !::IsWindowVisible(m_pFrame->m_hWnd))) {
//         BOOL bOld = +0x188->+0x2108;
//         if (vm->IsOwnerDrawCaption() && +0x30 == 0 && !::IsZoomed(m_pFrame->m_hWnd)) { +0x188->+0x2108 = 1; rgrc[0].bottom += szSystemBorder.cy; }
//         else +0x188->+0x2108 = 0;
//         if (bOld != +0x188->+0x2108) +0x188->RecalcLayout();          // CMFCRibbonStatusBar vftable (ANSI 0x30a488) +0x430 = 0x126aa0
//     }
//     if (bResult) return TRUE;
//     if (!bRibbonCaption && vm->IsOwnerDrawCaption() && +0x30 == 0) rgrc[0].top += ::GetSystemMetrics(SM_CYCAPTION);
//     if (!(m_pFrame->GetStyle() & WS_MAXIMIZE)) return FALSE;
//     if (bRibbonCaption) return TRUE;
//     return vm->IsOwnerDrawCaption() && +0x30 == 0;
// DEVIATION: the +0x188 ribbon-status-bar block is NOT transcribed -- OpenMFC's
// CMFCRibbonStatusBar (include/openmfc/afxmfc.h) is a 64-byte-padded shell with no
// member at +0x2108 (the public source's m_bBottomFrame) and no RecalcLayout thunk
// this file can reach.  Under OpenMFC's always-FALSE IsOwnerDrawCaption that block can
// only ever write FALSE over a flag nothing else sets, so skipping it changes no
// observable state.  A NULL frame returns FALSE instead of faulting.
// Symbol: ?OnNcCalcSize@CFrameImpl@@IEAAHHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" int MS_ABI impl__OnNcCalcSize_CFrameImpl__IEAAHHPEAUtagNCCALCSIZE_PARAMS___Z(
    void* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    (void)bCalcValidRects;
    if (lpncsp == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    CFrameWnd* pFrame = OwnerFrame(pThis);
    if (pFrame == nullptr) {
        return FALSE;
    }
    const SIZE szSystemBorder = FrameSystemBorders(pFrame);
    CMFCRibbonBar* pRibbonBar = RibbonBar(pThis);
    BOOL bRibbonCaption = FALSE;
    BOOL bResult = FALSE;
    if (RibbonReplacesCaption(pRibbonBar, pFrame->m_hWnd)) {
        bRibbonCaption = TRUE;
        EnsureGlobalDataInitialized();
        if (impl__IsDwmCompositionEnabled_AFX_GLOBAL_DATA__QEAAHXZ()) {
            BOOL bSkipAdjust = FALSE;
            if ((impl__GetStyle_CWnd__QEBAKXZ(pFrame) & WS_MAXIMIZE) != 0 || IsFullScreen()) {
                EnsureGlobalDataInitialized();
                bSkipAdjust = GlobalDataShellAutohideBars() != 0 && ::IsWindowVisible(pFrame->m_hWnd);
            }
            if (!bSkipAdjust) {
                lpncsp->rgrc[0].left += szSystemBorder.cx;
                lpncsp->rgrc[0].right -= szSystemBorder.cx;
                lpncsp->rgrc[0].bottom -= szSystemBorder.cy;
            }
            bResult = TRUE;
        }
    }
    // (retail's ribbon-status-bar block goes here; see the DEVIATION note above)
    if (bResult) {
        return TRUE;
    }
    const BOOL bOwnerDrawCaption = VisualManagerIsOwnerDrawCaption() && OleInPlaceActive(pThis) == 0;
    if (!bRibbonCaption && bOwnerDrawCaption) {
        lpncsp->rgrc[0].top += ::GetSystemMetrics(SM_CYCAPTION);
    }
    if ((impl__GetStyle_CWnd__QEBAKXZ(pFrame) & WS_MAXIMIZE) == 0) {
        return FALSE;
    }
    if (bRibbonCaption) {
        return TRUE;
    }
    return bOwnerDrawCaption ? TRUE : FALSE;
}

// Transcribed from retail 0x66040 (point arrives in screen coordinates):
//     if (m_pRibbonBar (+0x180) && ::IsWindowVisible(m_pRibbonBar->m_hWnd) && m_pRibbonBar->+0x468 && m_pRibbonBar->+0x448
//         && afxGlobalData.IsDwmCompositionEnabled()) {
//         LRESULT lResult = 0;
//         DwmDefWindowProc(m_pFrame ? m_pFrame->m_hWnd : NULL, WM_NCHITTEST, 0, MAKELPARAM(point.x, point.y), &lResult);   // 0x1c9288, see CallDwmDefWindowProc
//         return (UINT)lResult;
//     }
//     if (!vm->IsOwnerDrawCaption() (+0x3f8)) return HTNOWHERE;
//     if (+0x30 != 0) return HTNOWHERE;
//     ::ScreenToClient(m_pFrame->m_hWnd, &point);                                // slot 0x2c5340
//     CSize szSystemBorder = afxGlobalUtils.GetSystemBorders(m_pFrame);
//     int cy = szSystemBorder.cy;  if (!::IsIconic(m_pFrame->m_hWnd)) cy += ::GetSystemMetrics(SM_CYCAPTION);
//     point.x += szSystemBorder.cx;  point.y += cy;
//     for each button in the +0xd0 list: if (::PtInRect(&button->GetRect() /*vtable slot 5*/, point)) return button->m_nHit (+0x20);
//     CRect rectCaption = GetCaptionRect();                                       // 0x65ba0
//     if (!::PtInRect(&rectCaption, point)) return HTNOWHERE;
//     CRect rectSysMenu = rectCaption;
//     rectSysMenu.right = rectCaption.left + ::GetSystemMetrics(SM_CYCAPTION) + 2 * szSystemBorder.cx;
//     return ::PtInRect(&rectSysMenu, point) ? HTSYSMENU : HTCAPTION;
// DEVIATION: a NULL frame returns HTNOWHERE on the owner-draw path instead of faulting.
// Symbol: ?OnNcHitTest@CFrameImpl@@IEAAIVCPoint@@@Z
extern "C" unsigned int MS_ABI impl__OnNcHitTest_CFrameImpl__IEAAIVCPoint___Z(void* pThis, long long point) {
    POINT pt = UnpackPoint(point);
    CMFCRibbonBar* pRibbonBar = RibbonBar(pThis);
    if (pRibbonBar != nullptr && ::IsWindowVisible(pRibbonBar->m_hWnd) &&
        pRibbonBar->m_bReplaceFrameCaption != 0 && pRibbonBar->m_bIsTransparentCaption != 0) {
        EnsureGlobalDataInitialized();
        if (impl__IsDwmCompositionEnabled_AFX_GLOBAL_DATA__QEAAHXZ()) {
            LRESULT lResult = 0;
            CallDwmDefWindowProc(WndHandle(OwnerFrame(pThis)), WM_NCHITTEST, 0,
                                 MAKELPARAM(static_cast<WORD>(pt.x), static_cast<WORD>(pt.y)), &lResult);
            return static_cast<unsigned int>(lResult);
        }
    }
    if (!VisualManagerIsOwnerDrawCaption()) {
        return HTNOWHERE;
    }
    if (OleInPlaceActive(pThis) != 0) {
        return HTNOWHERE;
    }
    CFrameWnd* pFrame = OwnerFrame(pThis);
    if (pFrame == nullptr) {
        return HTNOWHERE;
    }
    ::ScreenToClient(pFrame->m_hWnd, &pt);
    const SIZE szSystemBorder = FrameSystemBorders(pFrame);
    int cy = szSystemBorder.cy;
    if (!::IsIconic(pFrame->m_hWnd)) {
        cy += ::GetSystemMetrics(SM_CYCAPTION);
    }
    pt.x += szSystemBorder.cx;
    pt.y += cy;

    SysButtonSnapshot buttons;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra != nullptr) {
            SnapshotSysButtons(*pExtra, buttons);
        }
    }
    for (int i = 0; i < buttons.nCount; ++i) {
        void* pButton = buttons.pButtons[i];
        if (pButton == nullptr) {
            continue;
        }
        const RECT rectButton = SysButtonGetRect(pButton);
        if (::PtInRect(&rectButton, pt)) {
            return static_cast<SysButtonView*>(pButton)->m_nHit;
        }
    }
    RECT rectCaption;
    impl__GetCaptionRect_CFrameImpl__IEAA_AVCRect__XZ(pThis, &rectCaption);
    if (!::PtInRect(&rectCaption, pt)) {
        return HTNOWHERE;
    }
    RECT rectSysMenu = rectCaption;
    rectSysMenu.right = rectCaption.left + ::GetSystemMetrics(SM_CYCAPTION) + 2 * szSystemBorder.cx;
    return ::PtInRect(&rectSysMenu, pt) ? HTSYSMENU : HTCAPTION;
}

// Transcribed from retail 0x66270 (nHitTest is never read):
//     if (!vm->IsOwnerDrawCaption()) return;             // vftable +0x3f8
//     if (+0x30 != 0 || +0x20 != 0) return;
//     OnTrackCaptionButtons(point);                      // 0x664d0, screen coordinates as given
// The +0x3f8 predicate is FALSE for every OpenMFC manager (VisualManagerIsOwnerDrawCaption).
// Symbol: ?OnNcMouseMove@CFrameImpl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CFrameImpl__IEAAXIVCPoint___Z(void* pThis, unsigned int nHitTest, long long point) {
    (void)nHitTest;
    if (!VisualManagerIsOwnerDrawCaption()) {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra != nullptr && (pExtra->bOleInPlaceActive != 0 || pExtra->nGate20 != 0)) {
            return;
        }
    }
    impl__OnTrackCaptionButtons_CFrameImpl__IEAAXVCPoint___Z(pThis, point);
}

// Transcribed from retail 0x651d0:
//     BOOL bRibbonCaption = FALSE;
//     if (<RibbonReplacesCaption>) bRibbonCaption = !afxGlobalData.IsDwmCompositionEnabled();   // 0x6c260 (sete at 0x65256)
//     if (!(vm->IsOwnerDrawCaption() (+0x3f8) && +0x30 == 0) && !bRibbonCaption) return FALSE;
//     if (afxGlobalData.+0x0c != 0) return FALSE;                 // set to 1 on entry to ?OnSettingChange@AFX_GLOBAL_DATA@@ 0x6aeb0 and cleared at its end
//     CRect rectRedraw = +0x50;
//     return vm->OnNcPaint(m_pFrame, <CObList at +0xd0>, rectRedraw);   // vftable +0x410
// DEVIATION: the manager's OnNcPaint is the base-class thunk (returns FALSE), not a
// vtable dispatch.
// Symbol: ?OnNcPaint@CFrameImpl@@IEAAHXZ
extern "C" int MS_ABI impl__OnNcPaint_CFrameImpl__IEAAHXZ(void* pThis) {
    CFrameWnd* pFrame = OwnerFrame(pThis);
    BOOL bRibbonCaption = FALSE;
    if (RibbonReplacesCaption(RibbonBar(pThis), WndHandle(pFrame))) {
        EnsureGlobalDataInitialized();
        bRibbonCaption = impl__IsDwmCompositionEnabled_AFX_GLOBAL_DATA__QEAAHXZ() ? FALSE : TRUE;
    }
    if (!(VisualManagerIsOwnerDrawCaption() && OleInPlaceActive(pThis) == 0) && !bRibbonCaption) {
        return FALSE;
    }
    EnsureGlobalDataInitialized();
    if (GlobalDataInt(0x0c) != 0) {   // inside AFX_GLOBAL_DATA::OnSettingChange
        return FALSE;
    }
    CObList* pSysButtons = nullptr;
    RECT rectRedraw = {};
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra& extra = g_frameImplExtras[pThis];
        if (!extra.bSysButtonsConstructed) {
            impl___0CObList__QEAA__J_Z(SysButtons(extra), 10);
            extra.bSysButtonsConstructed = true;
        }
        pSysButtons = SysButtons(extra);
        rectRedraw = extra.rectRedraw;
    }
    return impl__OnNcPaint_CMFCVisualManager__UEAAHPEAVCWnd__AEBVCObList__VCRect___Z(
        VisualManager(), pFrame, *pSysButtons, CRect(rectRedraw));
}

// Transcribed from retail 0x65980 (ANSI twin ?OnSetText@CFrameImpl@@IEAAXPEBD@Z; the
// string argument is never read):
//     if (m_pRibbonBar (+0x180) && ::IsWindowVisible(m_pRibbonBar->m_hWnd) && m_pRibbonBar->+0x468)
//         ::RedrawWindow(m_pRibbonBar->m_hWnd, NULL, NULL, 0x105);   // RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW
// Symbol: ?OnSetText@CFrameImpl@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnSetText_CFrameImpl__IEAAXPEB_W_Z(void* pThis, const wchar_t* lpszText) {
    (void)lpszText;
    CMFCRibbonBar* pRibbonBar = RibbonBar(pThis);
    if (pRibbonBar != nullptr && ::IsWindowVisible(pRibbonBar->m_hWnd) &&
        pRibbonBar->m_bReplaceFrameCaption != 0) {
        ::RedrawWindow(pRibbonBar->m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
}

// Transcribed from retail 0x664d0:
//     if (CMFCPopupMenu::m_pActivePopupMenu != NULL) return;     // static at 0x3b6fe8
//     UINT nHotOld = m_nHotSysButton (+0x14);
//     UINT nHit = OnNcHitTest(point);                            // 0x66040
//     pNew = first button in the +0xd0 list whose m_nHit (+0x20) == nHit, else NULL;
//     if (pNew == NULL || pNew->m_bEnabled (+0x14) == 0) m_nHotSysButton = 0;
//     else { m_nHotSysButton = pNew->m_nHit; pNew->m_bFocused (+0x0c) = TRUE; }
//     if (m_nHotSysButton != nHotOld) {
//         RedrawCaptionButton(pNew);                             // 0x66660; NULL -> no-op
//         pOld = first button whose m_nHit == nHotOld;
//         if (pOld != NULL) { pOld->m_bFocused = FALSE; RedrawCaptionButton(pOld); }
//     }
//     if (m_nHitSysButton (+0x18) != 0) return;                  // a press is in progress
//     if (nHotOld != 0) { if (m_nHotSysButton == 0) ::ReleaseCapture(); }        // slot 0x2c5280
//     else if (m_nHotSysButton != 0) CWnd::FromHandle(::SetCapture(m_pFrame->m_hWnd));   // 0x2c5278, 0x289180
// The CWnd::FromHandle result is discarded (it only creates the temporary map entry).
// NOTE: pNew is NOT cleared on the disabled-button path -- the `je 0x6650f` at
// 0x66535 lands past the `xor %edx,%edx` at 0x6650d, so a found-but-disabled button
// is still the one RedrawCaptionButton repaints when the hot id changes.
// DEVIATION: a NULL frame skips the SetCapture instead of faulting.
// Symbol: ?OnTrackCaptionButtons@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CFrameImpl__IEAAXVCPoint___Z(void* pThis, long long point) {
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA != nullptr) {
        return;
    }
    UINT nHotOld = 0;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra == nullptr) {
            return;
        }
        nHotOld = pExtra->uiHotSysButton;
    }
    const UINT nHit = impl__OnNcHitTest_CFrameImpl__IEAAIVCPoint___Z(pThis, point);

    void* pNew = nullptr;
    void* pOld = nullptr;
    bool bChanged = false;
    UINT nHotNew = 0;
    UINT nHitSysButton = 0;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra == nullptr) {
            return;
        }
        SysButtonSnapshot buttons;
        SnapshotSysButtons(*pExtra, buttons);
        for (int i = 0; i < buttons.nCount; ++i) {
            void* p = buttons.pButtons[i];
            if (p != nullptr && static_cast<SysButtonView*>(p)->m_nHit == nHit) {
                pNew = p;
                break;
            }
        }
        if (pNew == nullptr || static_cast<SysButtonView*>(pNew)->m_bEnabled == 0) {
            pExtra->uiHotSysButton = 0;   // pNew keeps the (disabled) button, as retail does
        } else {
            pExtra->uiHotSysButton = static_cast<SysButtonView*>(pNew)->m_nHit;
            static_cast<SysButtonView*>(pNew)->m_bFocused = TRUE;
        }
        nHotNew = pExtra->uiHotSysButton;
        bChanged = nHotNew != nHotOld;
        if (bChanged) {
            for (int i = 0; i < buttons.nCount; ++i) {
                void* p = buttons.pButtons[i];
                if (p != nullptr && static_cast<SysButtonView*>(p)->m_nHit == nHotOld) {
                    pOld = p;
                    break;
                }
            }
        }
        nHitSysButton = pExtra->uiHitSysButton;
    }
    if (bChanged) {
        impl__RedrawCaptionButton_CFrameImpl__IEAAXPEAVCMFCCaptionButtonEx___Z(pThis, pNew);
        if (pOld != nullptr) {
            static_cast<SysButtonView*>(pOld)->m_bFocused = FALSE;
            impl__RedrawCaptionButton_CFrameImpl__IEAAXPEAVCMFCCaptionButtonEx___Z(pThis, pOld);
        }
    }
    if (nHitSysButton != 0) {
        return;
    }
    if (nHotOld != 0) {
        if (nHotNew == 0) {
            ::ReleaseCapture();
        }
    } else if (nHotNew != 0) {
        CFrameWnd* pFrame = OwnerFrame(pThis);
        if (pFrame != nullptr) {
            (void)impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetCapture(pFrame->m_hWnd));
        }
    }
}

// Transcribed from retail 0x650e0:
//     if (+0x40 != 0) return;                                  // re-entrancy guard
//     if (+0x30 != 0) return;
//     if ((lpwp->flags (+0x20) & (SWP_NOSIZE|SWP_FRAMECHANGED)) == SWP_NOSIZE) return;
//     if (m_pRibbonBar (+0x180) == NULL) {
//         if (!vm->IsOwnerDrawCaption() (+0x3f8)) return;
//         if (+0x30 != 0) return;                              // tested again
//     }
//     +0x40 = 1;
//     int nSaved = 0;
//     if (m_pDockManager (+0x120)) { nSaved = CDockingManager::m_bDisableRecalcLayout (0x3b6f68); m_bDisableRecalcLayout = 1; }
//     +0x24 = vm->OnSetWindowRegion(m_pFrame, CSize(lpwp->cx (+0x18), lpwp->cy (+0x1c)));   // vftable +0x400 = 0x18d3f0
//     if (m_pDockManager) m_bDisableRecalcLayout = nSaved;
//     +0x40 = 0;
// The WINDOWPOS pointer is dereferenced without a null test in retail; the guard is ours.
// DEVIATION: OnSetWindowRegion is the base-class thunk (returns FALSE), not a vtable
// dispatch.
// Symbol: ?OnWindowPosChanging@CFrameImpl@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CFrameImpl__IEAAXPEAUtagWINDOWPOS___Z(void* pThis, WINDOWPOS* lpwp) {
    if (lpwp == nullptr) {
        return;
    }
    CDockingManager* pDockManager = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra == nullptr || pExtra->bWindowPosChanging != 0 || pExtra->bOleInPlaceActive != 0) {
            return;
        }
        if ((lpwp->flags & (SWP_NOSIZE | SWP_FRAMECHANGED)) == SWP_NOSIZE) {
            return;
        }
        if (pExtra->pRibbonBar == nullptr) {
            if (!VisualManagerIsOwnerDrawCaption()) {
                return;
            }
            if (pExtra->bOleInPlaceActive != 0) {
                return;
            }
        }
        pExtra->bWindowPosChanging = TRUE;
        pDockManager = pExtra->pDockManager;
    }
    int nSavedDisableRecalcLayout = 0;
    if (pDockManager != nullptr) {
        nSavedDisableRecalcLayout = impl__m_bDisableRecalcLayout_CDockingManager__2HA;
        impl__m_bDisableRecalcLayout_CDockingManager__2HA = 1;
    }
    const int nResult = impl__OnSetWindowRegion_CMFCVisualManager__UEAAHPEAVCWnd__VCSize___Z(
        VisualManager(), OwnerFrame(pThis), PackSize(lpwp->cx, lpwp->cy));
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra& extra = g_frameImplExtras[pThis];
        extra.nWindowRegionResult = nResult;   // retail +0x24
        if (pDockManager != nullptr) {
            impl__m_bDisableRecalcLayout_CDockingManager__2HA = nSavedDisableRecalcLayout;
        }
        extra.bWindowPosChanging = FALSE;
    }
}

// Retail 0x63540: `*pnAccelIndex = 1` when the pointer is non-null; then, if
// ?GetSafeActivePopupMenu@CMFCPopupMenu@@ (0xbc470) yields a popup, the key goes to
// it (focus checks through ::GetFocus / CWnd::FromHandle / ::IsChild, the popup's
// vtable +0x420 and +0x3a0, WM_KEYDOWN / WM_CLOSE via ::SendMessage) and the
// function returns; otherwise it returns TRUE for an iconic frame, FALSE when
// CMFCToolBar::m_bCustomizeMode (0x3b70bc) is set, and else walks
// CMFCToolBar::m_lstAllToolbars (0x3ab090, head 0x3ab098) calling a no-argument BOOL
// virtual (vtable +0x158) on every button (?GetButton@CMFCToolBar@@ 0x14e470) until
// one answers TRUE, builds the Ctrl/Alt/Shift mask from ::GetAsyncKeyState, asks
// ?IsKeyHandled@CKeyboardManager@@ (0x74c40) for the frame and for the active view
// (frame vtable +0x2f0), and finally maps Ctrl+F1 with a visible ribbon whose +0xb08
// is non-zero to ?ToggleMimimizeState@CMFCRibbonBar@@ (0xe26e0) and Alt+key to OnMenuChar
// (0x64490).  The popup-menu and toolbar-button vtable slots are retail-only.  STUB.
// Symbol: ?ProcessKeyboard@CFrameImpl@@IEAAHHPEAH@Z
extern "C" int MS_ABI impl__ProcessKeyboard_CFrameImpl__IEAAHHPEAH_Z(void* pThis, int nKey, int* pnAccelIndex) {
    (void)pThis;
    (void)nKey;
    (void)pnAccelIndex;
    return FALSE;
}

// Retail 0x63840: when the ribbon bar (+0x180) is visible and the click is outside
// its window rect it calls ?DeactivateKeyboardFocus@CMFCRibbonBar@@ (0xe28e0); it
// then walks CMFCToolBar::m_lstAllToolbars (buttons via 0x14e470), the active popup
// menu chain (?GetSafeActivePopupMenu@@ 0xbc470, ?CheckArea@CMFCPopupMenu@@ 0xba140,
// ?FindMenuWithConnectedFloaty@@ 0xbbf50, the CMapPtrToPtr at the module thread
// state, ?SetFocus@CWnd@@ 0x2a7a70) and several IsKindOf tests against popup-menu
// runtime classes.  The popup-menu infrastructure it drives is not modeled.  STUB.
// Symbol: ?ProcessMouseClick@CFrameImpl@@IEAAHIUtagPOINT@@PEAUHWND__@@@Z
extern "C" int MS_ABI impl__ProcessMouseClick_CFrameImpl__IEAAHIUtagPOINT__PEAUHWND_____Z(
    void* pThis, unsigned int uiMsg, long long pt, HWND hwnd) {
    (void)pThis;
    (void)uiMsg;
    (void)pt;
    (void)hwnd;
    return FALSE;
}

// Transcribed from retail 0x63f40 (mfc140; RVA 0x64110 in mfc140u).  `this` is never
// read:
//     if (CMFCToolBar::m_bCustomizeMode (0x3b70bc) != 0) return FALSE;
//     if (CMFCPopupMenu::m_pActivePopupMenu (0x3b6fe8) == NULL) return FALSE;
//     CMFCPopupMenu* pMenuWithFloaty = CMFCPopupMenu::FindMenuWithConnectedFloaty();   // 0xbbf50
//     if (pMenuWithFloaty != NULL && ::IsWindow(pMenuWithFloaty->m_hwndConnectedFloaty (+0x19c0))) {
//         CRect rect;  ::GetWindowRect(<that HWND>, &rect);                           // zero-initialised first
//         if (::PtInRect(&rect, pt)) return FALSE;
//     }
//     CMFCPopupMenu* pPopup = CMFCPopupMenu::GetSafeActivePopupMenu();                // 0xbc470
//     if (pPopup == NULL) return FALSE;
//     CRect rect;  ::GetWindowRect(pPopup->m_hWnd, &rect);
//     if (::PtInRect(&rect, pt)) return FALSE;
//     if (pPopup->GetMenuBar()->FindDestintationToolBar(pt) != NULL) return FALSE;   // vftable +0x3a0; 0xbf1b0
//     return TRUE;
// (IsWindow / GetWindowRect / PtInRect are the import slots 0x1802c5390 / 0x1802c5370 /
// 0x1802c5320 of the mfc140 image; POINT travels by value in rdx.)  GetMenuBar goes
// through PopupMenuBar() above: the retail slot for a client popup, the retail bodies
// (this+0x230, or this+0x19c8 for a CMFCRibbonPanelMenu / CMFCColorPopupMenu) for an
// own one.  CAVEAT for an own popup: OpenMFC's ??0CMFCPopupMenu thunk does not
// construct the embedded bar (see the header of featurepack/menu/CMFCPopupMenu.cpp),
// so FindDestintationToolBar then reads the bar's m_hWnd (popup +0x270) from bytes
// that constructor never initialises.
// Symbol: ?ProcessMouseMove@CFrameImpl@@IEAAHUtagPOINT@@@Z
extern "C" int MS_ABI impl__ProcessMouseMove_CFrameImpl__IEAAHUtagPOINT___Z(void* pThis, long long pt) {
    (void)pThis;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) {
        return FALSE;
    }
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA == nullptr) {
        return FALSE;
    }
    const POINT point = UnpackPoint(pt);
    void* pMenuWithFloaty = impl__FindMenuWithConnectedFloaty_CMFCPopupMenu__KAPEAV1_XZ();
    if (pMenuWithFloaty != nullptr) {
        HWND hWndFloaty = nullptr;
        memcpy(&hWndFloaty, static_cast<const unsigned char*>(pMenuWithFloaty) + 0x19c0, sizeof(hWndFloaty));
        if (::IsWindow(hWndFloaty)) {
            RECT rect = {};
            ::GetWindowRect(hWndFloaty, &rect);
            if (::PtInRect(&rect, point)) {
                return FALSE;
            }
        }
    }
    CMFCPopupMenu* pPopup = impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
    if (pPopup == nullptr) {
        return FALSE;
    }
    RECT rect = {};
    ::GetWindowRect(WndHandle(pPopup), &rect);
    if (::PtInRect(&rect, point)) {
        return FALSE;
    }
    if (impl__FindDestintationToolBar_CMFCPopupMenuBar__QEAAPEAVCMFCToolBar__VCPoint___Z(PopupMenuBar(pPopup), pt) != nullptr) {
        return FALSE;
    }
    return TRUE;
}

// Transcribed from retail 0x64040 (mfc140; RVA 0x64210 in mfc140u):
//     CMFCPopupMenu* pPopup = CMFCPopupMenu::GetSafeActivePopupMenu();           // 0xbc470
//     if (pPopup != NULL) {
//         if (pPopup->m_bScrollable (+0x16a4))
//             ::SendMessage(pPopup->m_hWnd, WM_MOUSEWHEEL, wParam, lParam);
//         if (pPopup->IsRibbonMiniToolBar()) {                                    // vftable +0x420
//             CWnd* pFocus = CWnd::FromHandle(::GetFocus());                       // 0x289180
//             if (pFocus == NULL || pFocus->m_hWnd == NULL ||
//                 (!::IsChild(pPopup->m_hWnd, pFocus->m_hWnd) && pFocus->m_hWnd != pPopup->m_hWnd))
//                 ::SendMessage(pPopup->m_hWnd, WM_CLOSE, 0, 0);
//         }
//         return TRUE;
//     }
//     if (m_pRibbonBar (+0x180) != NULL && ::IsWindowVisible(m_pRibbonBar->m_hWnd))
//         return (int)::SendMessage(m_pRibbonBar->m_hWnd, WM_MOUSEWHEEL, wParam, lParam);
//     return FALSE;
// The import slots (mfc140 image) resolve to SendMessageA -- SendMessageW in mfc140u
// -- (0x1802c5378), GetFocus (0x1802c51d8), IsChild (0x1802c51e0) and IsWindowVisible
// (0x1802c5350).  +0x16a4 is the dword featurepack/menu/CMFCPopupMenu.cpp reads as
// m_bScrollable (kOffScrollable); detail/CMFCPopupMenuSupport.h's shadow overlays its
// 8-byte m_pAnimMgr on it, so it is read raw here.  +0x420 is slot 132 of the retail
// CMFCPopupMenu vftable, IsRibbonMiniToolBar (see PopupIsRibbonMiniToolBar above).
// Symbol: ?ProcessMouseWheel@CFrameImpl@@IEAAH_K_J@Z
extern "C" int MS_ABI impl__ProcessMouseWheel_CFrameImpl__IEAAH_K_J_Z(void* pThis, unsigned long long wParam, long long lParam) {
    CMFCPopupMenu* pPopup = impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
    if (pPopup != nullptr) {
        const HWND hWndPopup = WndHandle(pPopup);
        int bScrollable = 0;
        memcpy(&bScrollable, reinterpret_cast<const unsigned char*>(pPopup) + 0x16a4, sizeof(bScrollable));
        if (bScrollable != 0) {
            ::SendMessage(hWndPopup, WM_MOUSEWHEEL, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam));
        }
        if (PopupIsRibbonMiniToolBar(pPopup)) {
            CWnd* pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
            if (pFocus == nullptr || pFocus->m_hWnd == nullptr ||
                (!::IsChild(WndHandle(pPopup), pFocus->m_hWnd) && pFocus->m_hWnd != WndHandle(pPopup))) {
                ::SendMessage(WndHandle(pPopup), WM_CLOSE, 0, 0);
            }
        }
        return TRUE;
    }
    CMFCRibbonBar* pRibbonBar = RibbonBar(pThis);
    if (pRibbonBar != nullptr && ::IsWindowVisible(pRibbonBar->m_hWnd)) {
        return static_cast<int>(::SendMessage(pRibbonBar->m_hWnd, WM_MOUSEWHEEL, static_cast<WPARAM>(wParam),
                                              static_cast<LPARAM>(lParam)));
    }
    return FALSE;
}

// Transcribed from retail 0x66660: return at once on a NULL button; otherwise take the
// button's rect through its vtable slot 5 (CMFCCaptionButtonEx::GetRect, hidden
// return buffer), copy those 16 bytes to +0x50, ::SendMessage(m_pFrame->m_hWnd,
// WM_NCPAINT, 0, 0) -- import slot 0x2c5378 resolves to SendMessageA in the ANSI
// image, i.e. SendMessageW in mfc140u -- then ::SetRectEmpty(&this->+0x50) and
// ::UpdateWindow(m_pFrame->m_hWnd).
// DEVIATION: retail reads m_pFrame->m_hWnd with no null check on m_pFrame; a `this`
// the constructor thunk never saw has no frame here and the two USER32 calls are
// skipped (the rect bookkeeping still happens).
// Symbol: ?RedrawCaptionButton@CFrameImpl@@IEAAXPEAVCMFCCaptionButtonEx@@@Z
extern "C" void MS_ABI impl__RedrawCaptionButton_CFrameImpl__IEAAXPEAVCMFCCaptionButtonEx___Z(void* pThis, void* pButton) {
    if (pButton == nullptr) {
        return;
    }
    const RECT rectButton = SysButtonGetRect(pButton);
    const HWND hWndFrame = WndHandle(OwnerFrame(pThis));
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        g_frameImplExtras[pThis].rectRedraw = rectButton;      // retail +0x50
    }
    if (hWndFrame != nullptr) {
        ::SendMessage(hWndFrame, WM_NCPAINT, 0, 0);
    }
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        ::SetRectEmpty(&g_frameImplExtras[pThis].rectRedraw);
    }
    if (hWndFrame != nullptr) {
        ::UpdateWindow(hWndFrame);
    }
}

// Transcribed from retail 0x65f90: find the first entry whose GetSafeHwnd() matches the
// argument's and unlink it; a miss leaves the list untouched.  Retail additionally calls
// ?RemoveAll@CPtrList@@ (0x83d0) once the count reaches zero, which only releases the
// list's node blocks -- the node-per-entry storage here frees as it unlinks instead.
// Symbol: ?RemoveFrame@CFrameImpl@@SAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__RemoveFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(CFrameWnd* pFrame) {
    const HWND hWndGone = impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(pFrame);
    std::lock_guard<std::mutex> lock(g_frameListMutex);
    PtrNode* pPrev = nullptr;
    for (PtrNode* pNode = g_pFrameListHead; pNode != nullptr; pPrev = pNode, pNode = pNode->pNext) {
        if (impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(
                static_cast<const CFrameWnd*>(pNode->pData)) != hWndGone) {
            continue;
        }
        if (pPrev == nullptr) {
            g_pFrameListHead = pNode->pNext;
        } else {
            pPrev->pNext = pNode->pNext;
        }
        delete pNode;
        return;
    }
}

// Transcribed from retail 0x62a50: walk the tear-off toolbar list at +0x98 for a node
// whose element is the argument and, if one is found, RemoveAt it (first match only;
// a miss leaves the list untouched).
// DEVIATION: as for AddTearOffToolbar, the body adds a `pThis == NULL -> no-op` guard
// because the list is reached through this file's companion table rather than +0x98.
// Symbol: ?RemoveTearOffToolbar@CFrameImpl@@IEAAXPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__RemoveTearOffToolbar_CFrameImpl__IEAAXPEAVCBasePane___Z(void* pThis, void* pPane) {
    if (!pThis) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    auto it = g_frameImplExtras.find(pThis);
    if (it == g_frameImplExtras.end()) {
        return;
    }
    PtrNode* pPrev = nullptr;
    for (PtrNode* pNode = it->second.pTearOffToolbars; pNode != nullptr;
         pPrev = pNode, pNode = pNode->pNext) {
        if (pNode->pData != pPane) {
            continue;
        }
        if (pPrev == nullptr) {
            it->second.pTearOffToolbars = pNode->pNext;
        } else {
            pPrev->pNext = pNode->pNext;
        }
        delete pNode;
        return;
    }
}

// Transcribed from retail 0x621d0 (ANSI twin ?RestorePosition@CFrameImpl@@IEAAXAEAUtagCREATESTRUCTA@@@Z;
// `this` is never read):
//     CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;
//     if (!pApp || !pApp->IsKindOf(RUNTIME_CLASS(CWinAppEx)) /*0x31e618*/) return;
//     if (cs.hInstance == NULL) return;
//     CRect rectNormal(cs.x, cs.y, cs.x + cs.cx, cs.y + cs.cy);
//     int nFlags = 0, nShowCmd = SW_SHOWNORMAL;
//     if (pApp->+0x1b4 /*m_bLoadWindowPlacement, ctor stores 1*/) {
//         if (!pApp->LoadWindowPlacement(rectNormal, nFlags, nShowCmd)) return;   // vftable +0x290 = 0x1c6ea0
//         if (nShowCmd != SW_SHOWMAXIMIZED) nShowCmd = SW_SHOWNORMAL;
//     } else nShowCmd = SW_SHOWNORMAL;
//     int nCmdShow = AfxGetModuleState()->m_pCurrentWinApp->m_nCmdShow;
//     if (nCmdShow != SW_SHOWMINIMIZED && nCmdShow != SW_SHOWMAXIMIZED && nCmdShow != SW_MINIMIZE && nCmdShow != SW_SHOWMINNOACTIVE)
//         AfxGetModuleState()->m_pCurrentWinApp->m_nCmdShow = nShowCmd;
//     MONITORINFO mi; mi.cbSize = sizeof mi;  RECT rectDesktop = {0};
//     if (::GetMonitorInfo(::MonitorFromPoint(rectNormal.TopLeft(), MONITOR_DEFAULTTONEAREST), &mi)) rectDesktop = mi.rcWork;
//     else ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectDesktop, 0);
//     if (nShowCmd == SW_SHOWMAXIMIZED) { cs.x = rectDesktop.left; cs.y = rectDesktop.top; cs.cx = rectDesktop.Width(); cs.cy = rectDesktop.Height(); }
//     else {
//         RECT rectInter;
//         if (!::IntersectRect(&rectInter, &rectDesktop, &rectNormal)) return;        // slot 0x2c4d48
//         cs.x = rectInter.left; cs.y = rectInter.top; cs.cx = rectNormal.Width(); cs.cy = rectNormal.Height();
//     }
// DEVIATION: OpenMFC's CWinAppEx has no m_bLoadWindowPlacement member (retail +0x1b4,
// which ??0CWinAppEx@@QEAA@H@Z (entry 0x1c59d0) sets to 1 with the `movq $0x1,0x1b4(%rsi)`
// at 0x1c5a97), so the constructor value TRUE is assumed;
// the app comes from impl__AfxGetApp, m_nCmdShow is the OpenMFC CWinApp member, and
// LoadWindowPlacement goes to the CWinAppEx thunk, not the app's vtable.
// Symbol: ?RestorePosition@CFrameImpl@@IEAAXAEAUtagCREATESTRUCTW@@@Z
extern "C" void MS_ABI impl__RestorePosition_CFrameImpl__IEAAXAEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* pcs) {
    (void)pThis;
    if (pcs == nullptr) {
        return;   // retail dereferences the reference unconditionally
    }
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        return;
    }
    if (pcs->hInstance == nullptr) {
        return;
    }
    CRect rectNormal(pcs->x, pcs->y, pcs->x + pcs->cx, pcs->y + pcs->cy);
    int nFlags = 0;
    int nShowCmd = SW_SHOWNORMAL;
    const BOOL bLoadWindowPlacement = TRUE;   // retail: pApp->+0x1b4
    if (bLoadWindowPlacement) {
        if (!impl__LoadWindowPlacement_CWinAppEx__MEAAHAEAVCRect__AEAH1_Z(
                static_cast<CWinAppEx*>(pApp), &rectNormal, &nFlags, &nShowCmd)) {
            return;
        }
        if (nShowCmd != SW_SHOWMAXIMIZED) {
            nShowCmd = SW_SHOWNORMAL;
        }
    } else {
        nShowCmd = SW_SHOWNORMAL;
    }
    const int nCmdShow = pApp->m_nCmdShow;
    if (nCmdShow != SW_SHOWMINIMIZED && nCmdShow != SW_SHOWMAXIMIZED &&
        nCmdShow != SW_MINIMIZE && nCmdShow != SW_SHOWMINNOACTIVE) {
        pApp->m_nCmdShow = nShowCmd;
    }
    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    RECT rectDesktop = {};
    POINT ptTopLeft;
    ptTopLeft.x = rectNormal.left;
    ptTopLeft.y = rectNormal.top;
    if (::GetMonitorInfo(::MonitorFromPoint(ptTopLeft, MONITOR_DEFAULTTONEAREST), &mi)) {
        rectDesktop = mi.rcWork;
    } else {
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectDesktop, 0);
    }
    if (nShowCmd == SW_SHOWMAXIMIZED) {
        pcs->x = rectDesktop.left;
        pcs->y = rectDesktop.top;
        pcs->cx = rectDesktop.right - rectDesktop.left;
        pcs->cy = rectDesktop.bottom - rectDesktop.top;
        return;
    }
    RECT rectInter;
    RECT rcNormal;
    rcNormal.left = rectNormal.left;
    rcNormal.top = rectNormal.top;
    rcNormal.right = rectNormal.right;
    rcNormal.bottom = rectNormal.bottom;
    if (!::IntersectRect(&rectInter, &rectDesktop, &rcNormal)) {
        return;
    }
    pcs->x = rectInter.left;
    pcs->y = rectInter.top;
    pcs->cx = rectNormal.right - rectNormal.left;
    pcs->cy = rectNormal.bottom - rectNormal.top;
}

// Transcribed from retail 0x645e0 (ANSI twin ?SaveDockState@CFrameImpl@@IEAAXPEBD@Z):
//     if (m_pDockManager (+0x120) == NULL) return;
//     m_pDockManager->SaveState(lpszProfileName, m_nIDDefaultResource (+0x10));   // vftable +0x98
// There is no +0x38 gate here, unlike LoadDockState.  +0x98 of the CDockingManager
// vftable is ?SaveState@CDockingManager@@UEAAHPEBDI@Z (0x4a310); the BOOL is discarded.
// DEVIATION: dispatched through the impl__SaveState_CDockingManager thunk, not the
// vtable (see LoadDockState).
// Symbol: ?SaveDockState@CFrameImpl@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__SaveDockState_CFrameImpl__IEAAXPEB_W_Z(void* pThis, const wchar_t* lpszProfileName) {
    CDockingManager* pDockManager = nullptr;
    UINT nIDDefaultResource = 0;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra == nullptr || pExtra->pDockManager == nullptr) {
            return;
        }
        pDockManager = pExtra->pDockManager;
        nIDDefaultResource = pExtra->nIDDefaultResource;
    }
    (void)impl__SaveState_CDockingManager__UEAAHPEB_WI_Z(pDockManager, lpszProfileName, nIDDefaultResource);
}

// Retail 0x62f50:
//     pApp = AfxGetModuleState()->m_pCurrentWinApp, NULL unless IsKindOf(CWinAppEx);
//     CString strPath = pApp ? pApp->GetRegSectionPath(L"") : L"";               // 0x1c60b0 (the argument is the empty string at 0x33ac36)
//     strPath += "ControlBars-TearOff";                                          // 0x33c6e0
//     for (i = 0; ; ++i) { strKey.Format("%Ts-%d", strPath, i);                 // 0x33c6f4
//         CSettingsStore& reg = CSettingsStoreSP().Create(FALSE, FALSE);         // 0x12b320
//         if (!reg.DeleteKey(strKey)) break; }                                   // CSettingsStore vftable (ANSI 0x30c560) +0x48 = 0x12b250
//     i = 0;
//     for each pBar in the +0x98 tear-off list (head +0xa0), ++i:
//         if (bFrameBarsOnly && pBar->GetTopLevelFrame() != m_pFrame) continue;
//         if (!(pBar->GetStyle() & WS_VISIBLE)) continue;
//         CString strName; pBar->GetWindowText(strName);                         // 0x28a280
//         strKey.Format("%Ts-%d", strPath, i);
//         CSettingsStore& reg = ...Create(FALSE, FALSE);
//         reg.CreateKey(strKey);                                                 // +0x28
//         reg.Write("ID", pBar->GetDlgCtrlID());                                 // +0x80, string 0x33c6fc
//         reg.Write("Name", strName);                                            // +0x70, 0x33c700
//         reg.Write("State", (CObject*)pBar);                                    // +0x50, 0x33c708
//         pBar->SaveState(strPath, i, -1);                                       // CMFCToolBar vftable +0x470
// (Each CSettingsStoreSP above is a zeroed 16-byte stack owner; its destruction
// deletes the store Create made through the store's vftable slot 1 with flag 1 --
// the calls at 0x630b8 / 0x630f9 / 0x63297 inside this function, entry 0x62f50.)
// Not transcribed.  Everything it needs exists: the tear-off list (filled by
// AddTearOffToolbar) is in this file's companion state, the stack CSettingsStoreSP
// owner can be hand-rolled around ?Create@CSettingsStoreSP@@ the way
// featurepack/toolbar/CMFCToolBar.cpp (SettingsStoreSP) and
// featurepack/docking/CPane.cpp already do, and ?SaveState@CMFCToolBar@@ is
// implemented.  What is missing is fidelity of the payload: the "State" value goes
// through CSettingsStore::Write(LPCTSTR, CObject*), i.e. the bar's Serialize, which
// is still a stub on OpenMFC's CMFCToolBar, and the LoadTearOffMenus counterpart
// that would read these keys back is a stub too.  STUB.
// Symbol: ?SaveTearOffMenus@CFrameImpl@@IEAAXH@Z
extern "C" void MS_ABI impl__SaveTearOffMenus_CFrameImpl__IEAAXH_Z(void* pThis, int bFrameBarsOnly) {
    (void)pThis;
    (void)bFrameBarsOnly;
}

// Transcribed from retail 0x625d0:
//     for each pToolBar in the +0x60 user-toolbar list (head +0x68):
//         if (bFrameBarsOnly && pToolBar->GetTopLevelFrame() != m_pFrame) continue;   // 0x28c910
//         pToolBar->SaveState(m_strUserToolbarsProfile (+0x108), -1, (UINT)-1);        // CMFCToolBar vftable (ANSI 0x313618) +0x470 = 0x151b70
// DEVIATION: SaveState goes to the CMFCToolBar thunk, not the bar's vtable (a
// CMFCMenuBar-style override is not reached); the return value is discarded as in retail.
// Symbol: ?SaveUserToolbars@CFrameImpl@@IEAAXH@Z
extern "C" void MS_ABI impl__SaveUserToolbars_CFrameImpl__IEAAXH_Z(void* pThis, int bFrameBarsOnly) {
    enum { kMax = 64 };
    void* pToolbars[kMax];
    int nCount = 0;
    alignas(CString) unsigned char profileStorage[sizeof(CString)];
    CString* pProfile = new (profileStorage) CString();
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        if (pExtra == nullptr) {
            pProfile->~CString();
            return;
        }
        for (PtrNode* pNode = pExtra->pUserToolbars; pNode != nullptr && nCount < kMax; pNode = pNode->pNext) {
            pToolbars[nCount++] = pNode->pData;
        }
        *pProfile = pExtra->strUserToolbarsProfile;
    }
    CFrameWnd* pFrame = OwnerFrame(pThis);
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBar* pToolBar = static_cast<CMFCToolBar*>(pToolbars[i]);
        if (bFrameBarsOnly && impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pToolBar) != pFrame) {
            continue;
        }
        (void)impl__SaveState_CMFCToolBar__UEAAHPEB_WHI_Z(pToolBar, pProfile->GetString(), -1, static_cast<unsigned int>(-1));
    }
    pProfile->~CString();
}

// Transcribed from retail 0x64650 (the CDockState argument is never read):
//     if (m_pDockManager (+0x120)) m_pDockManager->SetDockState();   // CDockingManager vftable (ANSI 0x2e4070) +0xa8 = 0x4bc40
// DEVIATION: dispatched through the impl__SetDockState_CDockingManager thunk, not the
// vtable (see LoadDockState).
// Symbol: ?SetDockState@CFrameImpl@@IEAAXAEBVCDockState@@@Z
extern "C" void MS_ABI impl__SetDockState_CFrameImpl__IEAAXAEBVCDockState___Z(void* pThis, const void* pDockState) {
    (void)pDockState;
    CDockingManager* pDockManager = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra* pExtra = FindExtra(pThis);
        pDockManager = pExtra != nullptr ? pExtra->pDockManager : nullptr;
    }
    if (pDockManager != nullptr) {
        impl__SetDockState_CDockingManager__UEAAXXZ(pDockManager);
    }
}

// Transcribed from retail 0x664a0: for each button in the +0xd0 list, in order:
//     if (pButton->m_nHit (+0x20) != nHit) continue;
//     if (pButton->m_bFocused (+0x0c) != 0) return;       // already highlighted: stop
//     pButton->m_bFocused = TRUE;                          // and keep walking
// Nothing is repainted here.  The list is empty in OpenMFC until UpdateCaption is
// implemented.
// Symbol: ?SetHighlightedSysButton@CFrameImpl@@IEAAXI@Z
extern "C" void MS_ABI impl__SetHighlightedSysButton_CFrameImpl__IEAAXI_Z(void* pThis, unsigned int nHit) {
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    FrameImplExtra* pExtra = FindExtra(pThis);
    if (pExtra == nullptr) {
        return;
    }
    SysButtonSnapshot buttons;
    SnapshotSysButtons(*pExtra, buttons);
    for (int i = 0; i < buttons.nCount; ++i) {
        SysButtonView* pButton = static_cast<SysButtonView*>(buttons.pButtons[i]);
        if (pButton == nullptr || pButton->m_nHit != nHit) {
            continue;
        }
        if (pButton->m_bFocused != 0) {
            return;
        }
        pButton->m_bFocused = TRUE;
    }
}

// Transcribed from retail 0x62800:
//     if (pRTC == NULL) AfxThrowInvalidArgException();    // 0x225b80, does not return
//     m_pNewUserToolBarRTC (+0x178) = pRTC;
// Symbol: ?SetNewUserToolBarRTC@CFrameImpl@@QEAAXPEAUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__SetNewUserToolBarRTC_CFrameImpl__QEAAXPEAUCRuntimeClass___Z(void* pThis, CRuntimeClass* pRTC) {
    if (pRTC == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (pThis == nullptr) {
        return;   // DEVIATION: retail stores through the null `this`
    }
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    g_frameImplExtras[pThis].pNewUserToolBarRTC = pRTC;
}

// Transcribed from retail 0x64370:
//     for (int i = 0; i < ::GetMenuItemCount(menu.m_hMenu); ) {                     // slot 0x2c4c90, re-evaluated every iteration
//         UINT uiCmd = ::GetMenuItemID(menu.m_hMenu, i);                           // slot 0x2c4c98
//         if (uiCmd < uiViewUserToolbarCmdFirst || uiCmd > uiViewUserToolbarCmdLast) { ++i; continue; }   // unsigned compares
//         CMFCToolBar* pToolBar = GetUserToolBarByIndex(uiCmd - uiViewUserToolbarCmdFirst);   // 0x62640
//         if (pToolBar == NULL) { ::DeleteMenu(menu.m_hMenu, i, MF_BYPOSITION); continue; }   // slot 0x2c4d88; i is NOT advanced
//         CString strName;  pToolBar->GetWindowText(strName);                       // 0x28a280
//         ::ModifyMenu(menu.m_hMenu, i, MF_BYPOSITION, uiCmd, strName);             // slot 0x2c4d80 = ModifyMenuA (-> W in mfc140u)
//         ++i;
//     }
// Symbol: ?SetupToolbarMenu@CFrameImpl@@IEAAXAEAVCMenu@@II@Z
extern "C" void MS_ABI impl__SetupToolbarMenu_CFrameImpl__IEAAXAEAVCMenu__II_Z(
    void* pThis, CMenu* pMenu, unsigned int uiViewUserToolbarCmdFirst, unsigned int uiViewUserToolbarCmdLast) {
    if (pMenu == nullptr) {
        return;   // retail reads menu.m_hMenu through the reference unconditionally
    }
    const HMENU hMenu = pMenu->m_hMenu;
    for (int i = 0; i < ::GetMenuItemCount(hMenu);) {
        const unsigned int uiCmd = ::GetMenuItemID(hMenu, i);
        if (uiCmd < uiViewUserToolbarCmdFirst || uiCmd > uiViewUserToolbarCmdLast) {
            ++i;
            continue;
        }
        void* pToolBar = impl__GetUserToolBarByIndex_CFrameImpl__IEBAPEAVCMFCToolBar__H_Z(
            pThis, static_cast<int>(uiCmd - uiViewUserToolbarCmdFirst));
        if (pToolBar == nullptr) {
            ::DeleteMenu(hMenu, static_cast<UINT>(i), MF_BYPOSITION);
            continue;
        }
        alignas(CString) unsigned char storage[sizeof(CString)];
        CString* pName = new (storage) CString();
        impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            static_cast<const CWnd*>(pToolBar), pName);
        ::ModifyMenu(hMenu, static_cast<UINT>(i), MF_BYPOSITION, uiCmd, pName->GetString());
        ++i;
        pName->~CString();
    }
}

// Retail 0x647d0 (~280 instructions, through 0x64c56) climbs the popup's parents
// (?GetParentPopupMenu@CMFCPopupMenu@@ 0xb7b10), IsKindOf-tests the owning toolbar,
// enumerates its buttons (?GetButton@CMFCToolBar@@ 0x14e470, ?GetDlgCtrlID@CWnd@@
// 0x2a78b0), calls ?RemoveAllItems@CMFCPopupMenu@@ (0xb7eb0) and news the menu
// items it inserts, reading m_pFrame (+0x118) for the owner.  The popup-menu item
// model it fills is not modeled here.  STUB.
// Symbol: ?ShowQuickCustomizePane@CFrameImpl@@IEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__ShowQuickCustomizePane_CFrameImpl__IEAAXPEAVCMFCPopupMenu___Z(void* pThis, void* pMenuPopup) {
    (void)pThis;
    (void)pMenuPopup;
}

// Transcribed from retail 0x665b0, two identical passes:
//     if (m_nHitSysButton (+0x18) != 0) {
//         pBtn = first button in the +0xd0 list whose m_nHit (+0x20) == m_nHitSysButton, else NULL;
//         m_nHitSysButton = 0;  ::ReleaseCapture();                       // slot 0x2c5280
//         if (pBtn) { pBtn->m_bPushed (+0x08) = FALSE; RedrawCaptionButton(pBtn); }   // 0x66660
//     }
//     if (m_nHotSysButton (+0x14) != 0) {
//         pBtn = first button whose m_nHit == m_nHotSysButton, else NULL;
//         m_nHotSysButton = 0;  ::ReleaseCapture();
//         if (pBtn) { pBtn->m_bFocused (+0x0c) = FALSE; RedrawCaptionButton(pBtn); }
//     }
// ReleaseCapture is called whether or not a button was found, in both passes.
// Symbol: ?StopCaptionButtonsTracking@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CFrameImpl__IEAAXXZ(void* pThis) {
    for (int nPass = 0; nPass < 2; ++nPass) {
        void* pButton = nullptr;
        {
            std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
            FrameImplExtra* pExtra = FindExtra(pThis);
            if (pExtra == nullptr) {
                return;
            }
            UINT& nTracked = nPass == 0 ? pExtra->uiHitSysButton : pExtra->uiHotSysButton;
            if (nTracked == 0) {
                continue;
            }
            SysButtonSnapshot buttons;
            SnapshotSysButtons(*pExtra, buttons);
            for (int i = 0; i < buttons.nCount; ++i) {
                void* p = buttons.pButtons[i];
                if (p != nullptr && static_cast<SysButtonView*>(p)->m_nHit == nTracked) {
                    pButton = p;
                    break;
                }
            }
            nTracked = 0;
        }
        ::ReleaseCapture();
        if (pButton != nullptr) {
            if (nPass == 0) {
                static_cast<SysButtonView*>(pButton)->m_bPushed = FALSE;
            } else {
                static_cast<SysButtonView*>(pButton)->m_bFocused = FALSE;
            }
            impl__RedrawCaptionButton_CFrameImpl__IEAAXPEAVCMFCCaptionButtonEx___Z(pThis, pButton);
        }
    }
}

// Transcribed from retail 0x620b0:
//     CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;                        // 0x1345b0, +0x8
//     if (!pApp || !pApp->IsKindOf(<CRuntimeClass 0x31e618, name "CWinAppEx">)) return;   // 0x233310
//     if (!::IsWindow(m_pFrame ? m_pFrame->m_hWnd : NULL)) return;                   // slot 0x2c5390
//     WINDOWPLACEMENT wp;  wp.length = sizeof wp;
//     if (!::GetWindowPlacement(m_pFrame->m_hWnd, &wp)) return;                       // slot 0x2c52c0
//     wp.showCmd = (wp.showCmd == SW_SHOWMAXIMIZED) ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL;
//     RECT rectDesktop;  ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectDesktop, 0);
//     ::OffsetRect(&wp.rcNormalPosition, rectDesktop.left, rectDesktop.top);
//     CRect rect = wp.rcNormalPosition;
//     pApp->StoreWindowPlacement(rect, wp.flags, wp.showCmd);                        // CWinAppEx vftable (ANSI 0x31e370) +0x298 = 0x1c6fd0
// DEVIATION: the app comes from impl__AfxGetApp and the StoreWindowPlacement call
// goes to the CWinAppEx thunk, not the app's vtable (a client override is not reached).
// Symbol: ?StoreWindowPlacement@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__StoreWindowPlacement_CFrameImpl__IEAAXXZ(void* pThis) {
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        return;
    }
    CFrameWnd* pFrame = OwnerFrame(pThis);
    const HWND hWndFrame = WndHandle(pFrame);
    if (!::IsWindow(hWndFrame)) {
        return;
    }
    WINDOWPLACEMENT wp;
    wp.length = sizeof(wp);
    if (!::GetWindowPlacement(hWndFrame, &wp)) {
        return;
    }
    wp.showCmd = wp.showCmd == SW_SHOWMAXIMIZED ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL;
    RECT rectDesktop = {};
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectDesktop, 0);
    ::OffsetRect(&wp.rcNormalPosition, rectDesktop.left, rectDesktop.top);
    const CRect rectNormal(wp.rcNormalPosition);
    (void)impl__StoreWindowPlacement_CWinAppEx__MEAAHAEBVCRect__HH_Z(
        static_cast<CWinAppEx*>(pApp), &rectNormal, static_cast<int>(wp.flags), static_cast<int>(wp.showCmd));
}

// Transcribed from retail 0x65d10:
//     if (!vm->IsOwnerDrawCaption() (+0x3f8) || +0x30 != 0) return;
//     if (<+0xd0 list>.m_nCount (+0xe8) == 0) {
//         DWORD dwStyle = m_pFrame->GetStyle();
//         CMenu* pSysMenu = CMenu::FromHandle(::GetSystemMenu(m_pFrame->m_hWnd, FALSE));   // 0x2c4d68, 0x2a5fc0
//         if (pSysMenu && ::IsMenu(pSysMenu->m_hMenu) && (dwStyle & WS_SYSMENU)) {         // 0x2c4d70; IsMenu is asked twice
//             AddTail(new CMFCCaptionButtonEx(19));                                          // 0x48 bytes, ctor 0x22f30, ?AddTail@CPtrList@@ 0x230490
//             if (dwStyle & WS_MAXIMIZEBOX) AddTail(new CMFCCaptionButtonEx(26));
//             if (dwStyle & WS_MINIMIZEBOX) AddTail(new CMFCCaptionButtonEx(27));
//         }
//     }
//     CRect rectCaption = GetCaptionRect();                                                  // 0x65ba0
//     CSize sizeBtn = vm->GetNcBtnSize(FALSE);                                               // vftable +0x420 (0x1a2320 in the Office2007 vftable)
//     sizeBtn.cy = min(sizeBtn.cy, rectCaption.Height() - 2);
//     int x = rectCaption.right - sizeBtn.cx;
//     int y = max(0, (rectCaption.Height() - sizeBtn.cy) / 2) + rectCaption.top;
//     for each button: button->m_rect (+0x38) = CRect(CPoint(x, y), sizeBtn);  x -= sizeBtn.cx;
//     ::RedrawWindow(m_pFrame->m_hWnd, NULL, NULL, 0x541);   // RDW_INVALIDATE|RDW_NOCHILDREN|RDW_UPDATENOW|RDW_FRAME
// 19 / 26 / 27 are the hit codes OnLButtonUp maps to SC_CLOSE / SC_MAXIMIZE / SC_MINIMIZE.
// The buttons are the 0x48-byte CMFCCaptionButtonEx of detail/CMFCCaptionButtonExSupport.h
// (m_rect at +0x38); ??1CFrameImpl deletes them through vtable slot 1.
// DEVIATION: GetNcBtnSize is the base thunk (CSize(0,0)), not a vtable dispatch; a
// NULL frame returns instead of faulting.  Under OpenMFC's always-FALSE
// IsOwnerDrawCaption the first test returns today.
// Symbol: ?UpdateCaption@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateCaption_CFrameImpl__IEAAXXZ(void* pThis) {
    if (!VisualManagerIsOwnerDrawCaption() || OleInPlaceActive(pThis) != 0) {
        return;
    }
    CFrameWnd* pFrame = OwnerFrame(pThis);
    if (pFrame == nullptr) {
        return;
    }
    bool bEmpty = true;
    CObList* pSysButtons = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        FrameImplExtra& extra = g_frameImplExtras[pThis];
        if (!extra.bSysButtonsConstructed) {
            impl___0CObList__QEAA__J_Z(SysButtons(extra), 10);
            extra.bSysButtonsConstructed = true;
        }
        pSysButtons = SysButtons(extra);
        SysButtonSnapshot buttons;
        SnapshotSysButtons(extra, buttons);
        bEmpty = buttons.nCount == 0;
    }
    if (bEmpty) {
        const DWORD dwStyle = impl__GetStyle_CWnd__QEBAKXZ(pFrame);
        CMenu* pSysMenu = impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(::GetSystemMenu(pFrame->m_hWnd, FALSE));
        if (pSysMenu != nullptr && ::IsMenu(pSysMenu->m_hMenu) && (dwStyle & WS_SYSMENU) != 0) {
            const UINT nHits[3] = {19, 26, 27};
            const DWORD dwGate[3] = {0, WS_MAXIMIZEBOX, WS_MINIMIZEBOX};
            for (int i = 0; i < 3; ++i) {
                if (dwGate[i] != 0 && (dwStyle & dwGate[i]) == 0) {
                    continue;
                }
                void* pButton = ::operator new(0x48);
                impl___0CMFCCaptionButtonEx__QEAA_I_Z(pButton, nHits[i]);
                impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(pSysButtons, static_cast<CObject*>(pButton));
            }
        }
    }
    RECT rectCaption;
    impl__GetCaptionRect_CFrameImpl__IEAA_AVCRect__XZ(pThis, &rectCaption);
    alignas(CSize) unsigned char sizeStorage[sizeof(CSize)];
    CSize* pSizeBtn = reinterpret_cast<CSize*>(sizeStorage);
    impl__GetNcBtnSize_CMFCVisualManager__UEBA_AVCSize__H_Z(VisualManager(), pSizeBtn, FALSE);
    const int cyCaption = rectCaption.bottom - rectCaption.top;
    const int cxBtn = pSizeBtn->cx;
    int cyBtn = pSizeBtn->cy;
    if (cyBtn >= cyCaption - 2) {
        cyBtn = cyCaption - 2;
    }
    int x = rectCaption.right - cxBtn;
    int y = (cyCaption - cyBtn) / 2;
    if (y < 0) {
        y = 0;
    }
    y += rectCaption.top;

    SysButtonSnapshot buttons;
    {
        std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
        SnapshotSysButtons(g_frameImplExtras[pThis], buttons);
    }
    for (int i = 0; i < buttons.nCount; ++i) {
        RECT* pRect = reinterpret_cast<RECT*>(static_cast<unsigned char*>(buttons.pButtons[i]) + 0x38);
        pRect->left = x;
        pRect->top = y;
        pRect->right = x + cxBtn;
        pRect->bottom = y + cyBtn;
        x -= cxBtn;
    }
    ::RedrawWindow(pFrame->m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_UPDATENOW | RDW_FRAME);
}
