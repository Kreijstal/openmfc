// CMDIFrameWndEx — OpenMFC implementation.
// Sources: cbarcore.cpp, global_other-3_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// OpenMFC — CMDIFrameWndEx methods (wave1_other-3, revised).
//
// Functions carrying a transcription comment were decoded from the retail
// export (ordinal -> RVA -> objdump). Every RVA named in a comment below is a
// function ENTRY in mfc140_rva_symbols.json unless the comment says otherwise;
// nine comments that named an address in the middle of some other function
// have been corrected in place and the wrong value is called out so a later
// reader is not confused. The generated stubs at the end of this file
// (OnNcMouseMove onward) have NOT been decoded and carry no such comment.
//
// Retail CMDIFrameWndEx embeds a CDockingManager sub-object at this+0x228 and a
// CMDIClientAreaWnd at this+0x720; retail CMDIChildWndEx embeds a CFrameImpl at
// this+0x218 and a CDockingManager at this+0x3a8. OpenMFC's simplified headers
// (afxmfc.h) model neither class beyond a padding blob, so every retail access
// to those offsets is conservatively skipped with a `TODO(clean-room)` note per
// IMPLEMENTATION_GUIDE.md; no offsets are invented and no header is edited.
//
// Message handlers that end in a tail call to a base export are routed through
// the corresponding sibling impl_ function. The rest are type-correct
// conservative versions (safe defaults, no state corruption) — a body that is
// only `{}` or a bare constant return is a STUB, and the comment above it says
// what was read and what is missing.

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
// CMDIFrameWndEx::OnClosePopupMenu(CMFCPopupMenu*) — retail entry RVA 0x88eb0.
// Retail, in order: lazily initialises afxGlobalData; then, when the private
// accessibility flag at 0x1803ba5f8 is set and pMenuPopup != NULL, picks an
// event id — EVENT_SYSTEM_MENUEND (5) only when pMenuPopup->+0x19ac == 0 AND
// CMFCPopupMenu::GetParentPopupMenu(pMenuPopup) == NULL AND pMenuPopup->+0x228
// != NULL, otherwise EVENT_SYSTEM_MENUPOPUPEND (7) — and calls
// ::NotifyWinEvent(id, pMenuPopup->m_hWnd, 0, 0). It then clears the global
// active-popup pointer at 0x1803b6fe8 if it still points at pMenuPopup, and
// finally stores 0 into this+0x570.
// Left a stub: the event choice depends on two CMFCPopupMenu fields (+0x19ac,
// +0x228) and this+0x570 / the active-popup global, none of which OpenMFC
// models; emitting one arm of the choice would be a guess.
// Symbol: ?OnClosePopupMenu@CMDIFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_CMDIFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(
    CMDIFrameWndEx* pThis, CMFCPopupMenu* pMenuPopup) {
    (void)pThis;
    (void)pMenuPopup;
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
// RVA 0x8ab90. Retail returns immediately unless (dwAllowedItems & 0x10) is set
// and the global at 0x1803b6f10 is non-null. It then builds an EMPTY popup —
// ::CreatePopupMenu (import slot 0x1802c4d40) attached to a stack CMenu via
// ?Attach@CMenu@@QEAAHPEAUHMENU__@@@Z (0x1802a6020) — and loads resource 0x42c0
// as a *string*, not as a menu: AfxFindStringResourceHandle(0x42c0)
// (0x1802accf0) followed by CStringT::LoadString (0x18000dc00), throwing
// CInvalidArgException (0x180225b80) if that string is missing. The menu items
// are then appended by hand and run through the context-menu manager.
// (An earlier revision of this comment called 0x42c0 a menu resource; the two
// call sites above show it is a string resource.)
// Left a stub: the string resource, the module-state resource-handle walk and
// the context-menu manager plumbing are not modeled here, and a partial menu
// would show wrong commands.
// Symbol: ?OnShowMDITabContextMenu@CMDIFrameWndEx@@UEAAHVCPoint@@KH@Z
extern "C" int MS_ABI impl__OnShowMDITabContextMenu_CMDIFrameWndEx__UEAAHVCPoint__KH_Z(
    CMDIFrameWndEx* pThis, const CPoint& point, DWORD dwAllowedItems, int bDrop) {
    (void)pThis;
    (void)point;
    (void)dwAllowedItems;
    (void)bDrop;
    return FALSE;
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
// CMDIFrameWndEx::AddDockSite() — NOT VERIFIED. This export has no entry in
// mfc140_rva_symbols.json (disas.py reports NOT FOUND; the only near match is
// ?AddDockSite@CDockingManager@@QEAAHAEBUAFX_DOCKSITE_INFO@@PEAPEAVCDockSite@@@Z
// at RVA 0x474b0, a different function with a different signature), so the
// retail body could not be read. An earlier revision of this file claimed
// "retail (RVA 0x27d0) is a bare ret"; that claim is not supported by the
// symbol map and has been removed rather than repeated.
// Documented MFC behaviour: CMDIFrameWndEx::AddDockSite is retained only for
// backward compatibility and does nothing. Left a no-op on that basis.
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
// CMDIFrameWndEx::AdjustClientArea() — retail entry RVA 0x89880 (the address
// 0x892d0 named by an earlier revision of this comment is not this function's
// entry). Retail copies the 16-byte RECT at this+0x3f0 onto the stack, insets
// it by the four LONGs at this+0x110..0x11c (left/top added, right/bottom
// subtracted), and — only when the embedded CMDIClientAreaWnd at this+0x720 has
// a window handle (+0x40 non-null) — calls its vtable slot at +0xe0 with that
// rect and 0.
// Left a stub: both the source rect (this+0x3f0), the inset (this+0x110) and
// the client-area sub-object (this+0x720) are outside OpenMFC's CMDIFrameWndEx
// layout, so there is nothing to compute from and nothing to resize.
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
// CMDIFrameWndEx::CreateDocumentWindow(LPCTSTR, CObject*) — NOT VERIFIED.
// Neither the wide export nor its ANSI twin
// (?CreateDocumentWindow@CMDIFrameWndEx@@UEAAPEAVCMDIChildWndEx@@PEBDPEAVCObject@@@Z)
// has an RVA in mfc140_rva_symbols.json, so the retail body could not be read.
// An earlier revision of this comment cited "RVA 0x71e0 is xor eax,eax; ret";
// that RVA does not belong to this export in the map and the claim is dropped.
// Documented MFC behaviour: the default implementation returns NULL and derived
// frames override it to create the tabbed-document child. NULL is returned on
// that basis.
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
CMDIFrameWndEx::CMDIFrameWndEx()
    : m_pVisualManager(nullptr) {
    memset(_mdiframewndex_padding, 0, sizeof(_mdiframewndex_padding));
}
CMDIFrameWndEx::~CMDIFrameWndEx() {}
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
// Left a stub (nullptr = "no in-place active item"): the active view's OLE
// state (view+0x170, view+0xe8) is not part of OpenMFC's view layout, OpenMFC
// has no CPreviewViewEx runtime class, and its COleDocument::GetInPlaceActiveItem
// export is itself a generated stub that returns nullptr, so building the chain
// would add no behaviour. CFrameWndEx::GetInPlaceActiveItem is stubbed the same way.
// Symbol: ?GetInPlaceActiveItem@CMDIFrameWndEx@@MEAAPEAVCOleClientItem@@XZ
extern "C" void* MS_ABI impl__GetInPlaceActiveItem_CMDIFrameWndEx__MEAAPEAVCOleClientItem__XZ(
    CMDIFrameWndEx* pThis) {
    (void)pThis;
    return nullptr;
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
// Left a stub returning 0: child+0x720 is the CMDIChildWndEx taskbar sub-object,
// which OpenMFC does not model, so "registered" cannot be distinguished from
// "not registered"; counting every MDI child instead would be a wrong answer,
// not a partial one.
// Symbol: ?GetRegisteredWithTaskBarMDIChildCount@CMDIFrameWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__GetRegisteredWithTaskBarMDIChildCount_CMDIFrameWndEx__QEAAHXZ(
    CMDIFrameWndEx* pThis) {
    (void)pThis;
    return 0;
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

// CMDIFrameWndEx::OnCompositionChanged() — retail entry RVA 0x8ad70 is a
// two-instruction tail jump: `rcx = this + 0x590; jmp 0x180066ad0` =
// ?OnCompositionChanged@CFrameImpl@@IEAAXXZ. The entire body is that one call,
// and the CFrameImpl sub-object at this+0x590 is not embedded in OpenMFC's
// CMDIFrameWndEx, so nothing can be reproduced.
// Symbol: ?OnCompositionChanged@CMDIFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnCompositionChanged_CMDIFrameWndEx__IEAAXXZ(
    CMDIFrameWndEx* pThis) {
    (void)pThis;
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
