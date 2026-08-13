// CMDIFrameWndEx — OpenMFC implementation.
// Sources: cbarcore.cpp, global_other-3_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// OpenMFC — CMDIFrameWndEx / CMDIChildWndEx method stubs (wave1_other-3).
//
// Every function here was decoded from the retail mfc140u export (ordinal ->
// RVA -> objdump, see /tmp/openmfc_remaining/disasm.py) rather than inferred.
//
// Retail CMDIFrameWndEx embeds a CDockingManager sub-object at this+0x228 and a
// CMDIClientAreaWnd at this+0x720; retail CMDIChildWndEx embeds a CFrameImpl at
// this+0x218 and a CDockingManager at this+0x3a8. OpenMFC's simplified headers
// (afxmfc.h) model neither class beyond a padding blob, so every retail access
// to those offsets is conservatively skipped with a `TODO(clean-room)` note per
// IMPLEMENTATION_GUIDE.md; no offsets are invented and no header is edited.
//
// Where a retail body is a bare `ret` / `xor eax,eax; ret`, the function is
// transcribed faithfully. Message handlers that end in a tail call to a base
// export are routed through the corresponding sibling impl_ function. The rest
// are type-correct conservative versions (safe defaults, no state corruption).

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

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
// Symbol: ?OnShowPopupMenu@CMDIFrameWndEx@@UEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__OnShowPopupMenu_CMDIFrameWndEx__UEAAHPEAVCMFCPopupMenu___Z(CMDIFrameWndEx* pThis, void* pPopup) {
    (void)pThis;
    (void)pPopup;
    return FALSE;
}
// Symbol: ?OnClosePopupMenu@CMDIFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_CMDIFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(CMDIFrameWndEx* pThis, void* pPopup) {
    (void)pThis;
    (void)pPopup;
}
// Symbol: ?OnShowPanes@CMDIFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_CMDIFrameWndEx__UEAAHH_Z(CMDIFrameWndEx* pThis, int bShow) {
    (void)pThis;
    (void)bShow;
    return TRUE;
}
// Symbol: ?OnToolbarCreateNew@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarCreateNew_CMDIFrameWndEx__IEAA_J_K_J_Z(CMDIFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}
// Symbol: ?OnShowMDITabContextMenu@CMDIFrameWndEx@@UEAAHVCPoint@@KH@Z
extern "C" int MS_ABI impl__OnShowMDITabContextMenu_CMDIFrameWndEx__UEAAHVCPoint__KH_Z(
    CMDIFrameWndEx* pThis, const CPoint& point, DWORD dwAllowedItems, int bDrop) {
    (void)pThis;
    (void)point;
    (void)dwAllowedItems;
    (void)bDrop;
    return FALSE;
}
// CMDIFrameWndEx::ActiveItemRecalcLayout() — retail (RVA 0x895b0) asks the
// frame for its OLE in-place active item (vtable 0x468), notifies the item's
// OLE map (item+0xa8) and ends at a layout recalc. The active-item map is not
// modeled, so the base frame relayout is kept.
// Symbol: ?ActiveItemRecalcLayout@CMDIFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__ActiveItemRecalcLayout_CMDIFrameWndEx__QEAAXXZ(
    CMDIFrameWndEx* pThis) {
    if (!pThis) return;
    pThis->CFrameWnd::RecalcLayout();
    // TODO(clean-room): transcribed partially — retail consults the OLE
    // in-place item map (vtable 0x468) before the layout recalc.
}
// CMDIFrameWndEx::AddDockSite() — retail (RVA 0x27d0) is a bare `ret` (no-op).
// Symbol: ?AddDockSite@CMDIFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_CMDIFrameWndEx__QEAAXXZ(CMDIFrameWndEx* pThis) {
    (void)pThis;
}
// CMDIFrameWndEx::AddPane(CBasePane*, BOOL) — retail (RVA 0x890e0) class-checks
// pBar (IsKindOf CMFCToolBar / CDockablePane), caches it at this+0x710/0x718
// and forwards to the embedded CDockingManager (this+0x228) AddPane. The dock
// manager is not modeled. Conservative FALSE.
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
// CMDIFrameWndEx::AdjustClientArea() — retail (RVA 0x892d0) computes the client
// rect from unmodeled frame rects (this+0x110/0x3f0) and resizes the MDI
// client-area window (this+0x720). Conservative no-op.
// Symbol: ?AdjustClientArea@CMDIFrameWndEx@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustClientArea_CMDIFrameWndEx__UEAAXXZ(CMDIFrameWndEx* pThis) {
    (void)pThis;
    // TODO(clean-room): transcribed partially — retail derives the client rect
    // from this+0x110/0x3f0 and resizes the MDI client area (this+0x720).
}
// CMDIFrameWndEx::AdjustDockingLayout(HDWP) — retail (RVA 0x89250) forwards to
// the embedded CDockingManager (this+0x228) AdjustDockingLayout, then runs the
// frame's AdjustClientArea and a conditional RecalcLayout. The dock manager is
// not modeled. Conservative no-op.
// Symbol: ?AdjustDockingLayout@CMDIFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CMDIFrameWndEx__UEAAXPEAX_Z(
    CMDIFrameWndEx* pThis, void* pParam) {
    (void)pParam;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — retail forwards to the embedded
    // CDockingManager (this+0x228) and then runs AdjustClientArea / RecalcLayout.
}
// CMDIFrameWndEx::ControlBarToTabbedDocument(CDockablePane*) — retail (RVA
// 0x8a140) allocates a CMDIChildWndEx (0x818 bytes), converts the dockable pane
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
// CMDIFrameWndEx::CreateDocumentWindow(const wchar_t*, CObject*) — retail (RVA
// 0x71e0) is `xor eax,eax; ret` — returns NULL.
// Symbol: ?CreateDocumentWindow@CMDIFrameWndEx@@UEAAPEAVCMDIChildWndEx@@PEB_WPEAVCObject@@@Z
extern "C" CMDIChildWndEx* MS_ABI impl__CreateDocumentWindow_CMDIFrameWndEx__UEAAPEAVCMDIChildWndEx__PEB_WPEAVCObject___Z(
    CMDIFrameWndEx* pThis, const wchar_t* lpcszDocName, CObject* pObj) {
    (void)pThis;
    (void)lpcszDocName;
    (void)pObj;
    return nullptr;
}
// CMDIFrameWndEx::CreateNewWindow(const wchar_t*, CObject*) — retail (RVA
// 0x89b70) allocates a CMDIChildWndEx and registers it via the MDI client area
// behind tabbed-group state (this+0x4574/0x4668). Not modeled. Conservative NULL.
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
// CMDIFrameWndEx::DockPane(CBasePane*, UINT, LPCRECT) — retail (RVA 0x891a0)
// tail-jumps to the embedded CDockingManager at this+0x228. Not modeled.
// Conservative no-op.
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
// CMDIFrameWndEx::DockPaneLeftOf(CPane*, CPane*) — retail (RVA 0x88ab0)
// tail-jumps to the embedded CDockingManager at this+0x228. Not modeled.
// Conservative FALSE.
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
// CMDIFrameWndEx::EnableAutoHidePanes(DWORD) — retail (RVA 0x89390) tail-jumps
// to the embedded CDockingManager at this+0x228. Not modeled. Conservative FALSE.
// Symbol: ?EnableAutoHidePanes@CMDIFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_CMDIFrameWndEx__QEAAHK_Z(
    CMDIFrameWndEx* pThis, unsigned long dwStyle) {
    (void)dwStyle;
    if (!pThis) return FALSE;
    // TODO(clean-room): retail tail-jumps to the embedded CDockingManager at
    // this+0x228 (EnableAutoHidePanes); not modeled.
    return FALSE;
}
// CMDIFrameWndEx::EnableMDITabbedGroups(BOOL, const CMDITabInfo&) — retail (RVA
// 0x88f40) tail-jumps to the embedded CMDIClientAreaWnd at this+0x720
// (EnableMDITabbedGroups). The client-area window is not modeled; the CMDITabInfo
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
CMDIFrameWndEx::CMDIFrameWndEx()
    : m_pVisualManager(nullptr) {
    memset(_mdiframewndex_padding, 0, sizeof(_mdiframewndex_padding));
}
CMDIFrameWndEx::~CMDIFrameWndEx() {}
CMFCVisualManager* CMDIFrameWndEx::GetVisualManager() const { return m_pVisualManager; }
// Symbol: ?EnableMDITabs@CMDIFrameWndEx@@QEAAXHHW4Location@CMFCBaseTabCtrl@@HW4Style@CMFCTabCtrl@@HH@Z
extern "C" void MS_ABI impl__EnableMDITabs_CMDIFrameWndEx__QEAAXHHW4Location_CMFCBaseTabCtrl__HW4Style_CMFCTabCtrl__HH_Z(int p0, int p1, int /*enum*/ p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10, void* p11, float p12, short p13, void* p14, void* p15, void* p16, void* p17, void* p18, void* p19, void* p20, void* p21, void* p22, void* p23, void* p24, void* p25, int p26, int /*enum*/ p27, void* p28, void* p29, void* p30, void* p31, void* p32, void* p33, float p34, short p35, void* p36, void* p37, void* p38, void* p39, void* p40, void* p41, void* p42, void* p43, int p44, int p45) {}

// Symbol: ?EnableWindowsDialog@CMDIFrameWndEx@@QEAAXIIHH@Z
extern "C" void MS_ABI impl__EnableWindowsDialog_CMDIFrameWndEx__QEAAXIIHH_Z(unsigned int p0, unsigned int p1, int p2, int p3) {}

// Symbol: ?EnableWindowsDialog@CMDIFrameWndEx@@QEAAXIPEB_WHH@Z
extern "C" void MS_ABI impl__EnableWindowsDialog_CMDIFrameWndEx__QEAAXIPEB_WHH_Z(unsigned int p0, const wchar_t* p1, int p2, int p3) {}

// Symbol: ?GetInPlaceActiveItem@CMDIFrameWndEx@@MEAAPEAVCOleClientItem@@XZ
extern "C" void* MS_ABI impl__GetInPlaceActiveItem_CMDIFrameWndEx__MEAAPEAVCOleClientItem__XZ() {
    return nullptr;
}

// Symbol: ?GetPane@CMDIFrameWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" void* MS_ABI impl__GetPane_CMDIFrameWndEx__QEAAPEAVCBasePane__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?GetRegisteredWithTaskBarMDIChildCount@CMDIFrameWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__GetRegisteredWithTaskBarMDIChildCount_CMDIFrameWndEx__QEAAHXZ() {
    return 0;
}

// Symbol: ?GetWindowMenuPopup@CMDIFrameWndEx@@MEAAPEAUHMENU__@@PEAU2@@Z
extern "C" void* MS_ABI impl__GetWindowMenuPopup_CMDIFrameWndEx__MEAAPEAUHMENU____PEAU2__Z(void* /*struct*/* p0) {
    return nullptr;
}

// Symbol: ?HtmlHelpW@CMDIFrameWndEx@@MEAAX_KI@Z
extern "C" void MS_ABI impl__HtmlHelpW_CMDIFrameWndEx__MEAAX_KI_Z(unsigned __int64 p0, unsigned int p1) {}

// Symbol: ?InsertPane@CMDIFrameWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_CMDIFrameWndEx__QEAAHPEAVCBasePane__0H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {
    return 0;
}

// Symbol: ?IsPointNearDockSite@CMDIFrameWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_CMDIFrameWndEx__QEBAHVCPoint__AEAKAEAH_Z(void* /*class*/ p0, unsigned long* p1, int* p2) {
    return 0;
}

// Symbol: ?LoadMDIState@CMDIFrameWndEx@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadMDIState_CMDIFrameWndEx__UEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?NegotiateBorderSpace@CMDIFrameWndEx@@UEAAHIPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__NegotiateBorderSpace_CMDIFrameWndEx__UEAAHIPEAUtagRECT___Z(unsigned int p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?OnActivate@CMDIFrameWndEx@@IEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_CMDIFrameWndEx__IEAAXIPEAVCWnd__H_Z(unsigned int p0, void* /*class*/* p1, int p2) {}

// Symbol: ?OnActivateApp@CMDIFrameWndEx@@IEAAXHK@Z
extern "C" void MS_ABI impl__OnActivateApp_CMDIFrameWndEx__IEAAXHK_Z(int p0, unsigned long p1) {}

// Symbol: ?OnAfterTaskbarActivate@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnAfterTaskbarActivate_CMDIFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnChangeVisualManager@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnChangeVisualManager_CMDIFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnClose@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CMDIFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnCmdMsg@CMDIFrameWndEx@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CMDIFrameWndEx__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(unsigned int p0, int p1, void* p2, void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?OnCommand@CMDIFrameWndEx@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMDIFrameWndEx__MEAAH_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnCompositionChanged@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnCompositionChanged_CMDIFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnContextHelp@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_CMDIFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnContextMenu@CMDIFrameWndEx@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CMDIFrameWndEx__IEAAXPEAVCWnd__VCPoint___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnCreate@CMDIFrameWndEx@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMDIFrameWndEx__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnCreateClient@CMDIFrameWndEx@@MEAAHPEAUtagCREATESTRUCTW@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__OnCreateClient_CMDIFrameWndEx__MEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?OnDestroy@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMDIFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnDrawMenuImage@CMDIFrameWndEx@@UEAAHPEAVCDC@@PEBVCMFCToolBarMenuButton@@AEBVCRect@@@Z
extern "C" int MS_ABI impl__OnDrawMenuImage_CMDIFrameWndEx__UEAAHPEAVCDC__PEBVCMFCToolBarMenuButton__AEBVCRect___Z(void* /*class*/* p0, const void* /*class*/* p1, const void* /*class*/* p2) {
    return 0;
}

// Symbol: ?OnExitSizeMove@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnExitSizeMove_CMDIFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnGetMinMaxInfo@CMDIFrameWndEx@@IEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CMDIFrameWndEx__IEAAXPEAUtagMINMAXINFO___Z(void* /*struct*/* p0) {}

// Symbol: ?OnIdleUpdateCmdUI@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CMDIFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnLButtonDown@CMDIFrameWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMDIFrameWndEx__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonUp@CMDIFrameWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMDIFrameWndEx__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMenuChar@CMDIFrameWndEx@@IEAA_JIIPEAVCMenu@@@Z
extern "C" __int64 MS_ABI impl__OnMenuChar_CMDIFrameWndEx__IEAA_JIIPEAVCMenu___Z(unsigned int p0, unsigned int p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?OnMouseMove@CMDIFrameWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMDIFrameWndEx__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMoveMiniFrame@CMDIFrameWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_CMDIFrameWndEx__UEAAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnNcActivate@CMDIFrameWndEx@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CMDIFrameWndEx__IEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?OnNcCalcSize@CMDIFrameWndEx@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMDIFrameWndEx__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?OnNcHitTest@CMDIFrameWndEx@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMDIFrameWndEx__IEAA_JVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?OnNcMouseMove@CMDIFrameWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CMDIFrameWndEx__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcPaint@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMDIFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnPaneCheck@CMDIFrameWndEx@@IEAAHI@Z
extern "C" int MS_ABI impl__OnPaneCheck_CMDIFrameWndEx__IEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?OnPostPreviewFrame@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnPostPreviewFrame_CMDIFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnPowerBroadcast@CMDIFrameWndEx@@IEAAII_J@Z
extern "C" unsigned int MS_ABI impl__OnPowerBroadcast_CMDIFrameWndEx__IEAAII_J_Z(unsigned int p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnSetMenu@CMDIFrameWndEx@@MEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__OnSetMenu_CMDIFrameWndEx__MEAAHPEAUHMENU_____Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnSetPreviewMode@CMDIFrameWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_CMDIFrameWndEx__UEAAXHPEAUCPrintPreviewState___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?OnSetText@CMDIFrameWndEx@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CMDIFrameWndEx__IEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?OnSize@CMDIFrameWndEx@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMDIFrameWndEx__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?OnSysColorChange@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMDIFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnToolbarContextMenu@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarContextMenu_CMDIFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnToolbarDelete@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarDelete_CMDIFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnUpdateFrameMenu@CMDIFrameWndEx@@UEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__OnUpdateFrameMenu_CMDIFrameWndEx__UEAAXPEAUHMENU_____Z(void* /*struct*/* p0) {}

// Symbol: ?OnWindowNew@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnWindowNew_CMDIFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnWindowPosChanged@CMDIFrameWndEx@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMDIFrameWndEx__IEAAXPEAUtagWINDOWPOS___Z(void* /*struct*/* p0) {}

// Symbol: ?PaneFromPoint@CMDIFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_CMDIFrameWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(void* /*class*/ p0, int p1, bool p2, void* /*struct*/* p3) {
    return nullptr;
}

// Symbol: ?PaneFromPoint@CMDIFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_CMDIFrameWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(void* /*class*/ p0, int p1, unsigned long* p2, void* /*struct*/* p3) {
    return nullptr;
}

// Symbol: ?PreCreateWindow@CMDIFrameWndEx@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMDIFrameWndEx__MEAAHAEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?PreTranslateMessage@CMDIFrameWndEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIFrameWndEx__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RecalcLayout@CMDIFrameWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CMDIFrameWndEx__UEAAXH_Z(int p0) {}

// Symbol: ?RegisterAllMDIChildrenWithTaskbar@CMDIFrameWndEx@@QEAAXH@Z
extern "C" void MS_ABI impl__RegisterAllMDIChildrenWithTaskbar_CMDIFrameWndEx__QEAAXH_Z(int p0) {}

// Symbol: ?RemovePaneFromDockManager@CMDIFrameWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CMDIFrameWndEx__QEAAXPEAVCBasePane__HHH0_Z(void* /*class*/* p0, int p1, int p2, int p3, void* /*class*/* p4) {}

// Symbol: ?SaveMDIState@CMDIFrameWndEx@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__SaveMDIState_CMDIFrameWndEx__UEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?ShowPane@CMDIFrameWndEx@@QEAAXPEAVCBasePane@@HHH@Z
extern "C" void MS_ABI impl__ShowPane_CMDIFrameWndEx__QEAAXPEAVCBasePane__HHH_Z(void* /*class*/* p0, int p1, int p2, int p3) {}

// Symbol: ?ShowPopupMenu@CMDIFrameWndEx@@IEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__ShowPopupMenu_CMDIFrameWndEx__IEAAHPEAVCMFCPopupMenu___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?ShowWindowsDialog@CMDIFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__ShowWindowsDialog_CMDIFrameWndEx__QEAAXXZ() {}

// Symbol: ?TabbedDocumentToControlBar@CMDIFrameWndEx@@UEAAHPEAVCMDIChildWndEx@@@Z
extern "C" int MS_ABI impl__TabbedDocumentToControlBar_CMDIFrameWndEx__UEAAHPEAVCMDIChildWndEx___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?UpdateMDITabbedBarsIcons@CMDIFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateMDITabbedBarsIcons_CMDIFrameWndEx__QEAAXXZ() {}

// Symbol: ?WinHelpW@CMDIFrameWndEx@@UEAAX_KI@Z
extern "C" void MS_ABI impl__WinHelpW_CMDIFrameWndEx__UEAAX_KI_Z(unsigned __int64 p0, unsigned int p1) {}
