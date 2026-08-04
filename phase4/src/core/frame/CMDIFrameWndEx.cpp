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
