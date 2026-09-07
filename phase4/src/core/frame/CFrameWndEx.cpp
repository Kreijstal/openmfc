// CFrameWndEx — OpenMFC implementation.
// Sources: cbarcore.cpp, global_core-frames-1_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// CFrameWndEx methods, wave 1 (core-frames-1). Most functions here were decoded
// from the retail mfc140u export (ordinal -> RVA -> objdump, see
// /tmp/openmfc_remaining/disasm_out). That is NOT true of every body in the
// file: the trailing generated stubs were never disassembled, and at least one
// body (OnUpdatePaneMenu) was inferred and wrong until it was decoded. Treat a
// body as decoded only where its own comment cites the RVA it came from.
//
// Retail CFrameWndEx embeds a CDockingManager sub-object at this+0x380 and
// additional frame state at this+0x1f0 / this+0x370 / this+0x530 / this+0x548.
// OpenMFC's simplified CFrameWndEx (afxmfc.h) instead holds a lazily-allocated
// CDockingManager* (m_pDockingManager), so every retail `lea 0x380(%rcx)`
// delegation is routed through GetDockingManager() and every access to an
// unmodeled byte offset is conservatively skipped with a `TODO(clean-room)`
// note, per IMPLEMENTATION_GUIDE.md.
//
// Where a retail body depends on virtual slots or internal unexported helpers
// that the OpenMFC headers do not model, a type-correct conservative version is
// emitted (safe defaults, no state corruption) with a `TODO(clean-room)` note.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---- sibling impl_ exports (defined in other phase4/src/*.cpp) ----
// CDockingManager (mfccore.cpp / thunks.cpp)
extern "C" void MS_ABI impl__DockPane_CDockingManager__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    CDockingManager* pThis, CBasePane* pBar, unsigned int nDockBarID, const RECT* lpRect);
extern "C" int MS_ABI impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(
    CDockingManager* pThis, CPane* pBarToDock, CPane* pBar);
extern "C" int MS_ABI impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(
    CDockingManager* pThis, unsigned long dwAlignment);
extern "C" int MS_ABI impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
    CDockingManager* pThis, CBasePane* pPane, CBasePane* pTarget, int nInsertAfter);
extern "C" int MS_ABI impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
    CDockingManager* pThis, CBasePane* pPane, int bCanFloat, int bSelect, int bSkipLayout);
extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
    CDockingManager* pThis, unsigned int nID, int bSearchMiniFrames);
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(
    CDockingManager* pThis, void* pParam);
extern "C" void MS_ABI impl__FixupVirtualRects_CDockingManager__UEAAXXZ(CDockingManager* pThis);
// CFrameWnd (frame_font_exports.cpp / thunks.cpp)
extern "C" int MS_ABI impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CFrameWnd* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
extern "C" int MS_ABI impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(
    CFrameWnd* pThis, WPARAM wParam, LPARAM lParam);
extern "C" void MS_ABI impl__OnContextHelp_CFrameWnd__QEAAXXZ(CFrameWnd* pThis);
extern "C" int MS_ABI impl__OnCreate_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW* pCreateStruct);
extern "C" void MS_ABI impl__OnDestroy_CFrameWnd__IEAAXXZ(CFrameWnd* pThis);
extern "C" void MS_ABI impl__OnClose_CFrameWnd__IEAAXXZ(CFrameWnd* pThis);
extern "C" void MS_ABI impl__OnActivate_CFrameWnd__IEAAXIPEAVCWnd__H_Z(
    CFrameWnd* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized);
// CWnd (global_core-windows_impl.cpp)
extern "C" void MS_ABI impl__HtmlHelpW_CWnd__UEAAX_KI_Z(
    CWnd* pThis, unsigned long long data, unsigned int command);
// CWnd (core/window/Thunks.cpp, core/window/CWnd.cpp)
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" void MS_ABI impl__OnSysColorChange_CWnd__IEAAXXZ(CWnd* pThis);
// CFrameWnd (core/frame/Thunks.cpp, core/frame/CFrameWnd.cpp)
extern "C" int MS_ABI impl__OnNcActivate_CFrameWnd__IEAAHH_Z(CFrameWnd* pThis, int bActive);
extern "C" __int64 MS_ABI impl__OnMenuChar_CFrameWnd__IEAA_JIIPEAVCMenu___Z(
    CFrameWnd* pThis, unsigned int nChar, unsigned int nFlags, CMenu* pMenu);
extern "C" void MS_ABI impl__OnSize_CFrameWnd__IEAAXIHH_Z(
    CFrameWnd* pThis, unsigned int nType, int cx, int cy);
extern "C" void MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int bNotify);
extern "C" void MS_ABI impl__OnUpdateFrameMenu_CFrameWnd__UEAAXPEAUHMENU_____Z(
    CFrameWnd* pThis, HMENU hMenuAlt);
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int bAddToTitle);
extern "C" int MS_ABI impl__NegotiateBorderSpace_CFrameWnd__UEAAHIPEAUtagRECT___Z(
    CFrameWnd* pThis, unsigned int nBorderCmd, RECT* lpRectBorder);
extern "C" void MS_ABI impl__OnSetPreviewMode_CFrameWnd__UEAAXHPEAUCPrintPreviewState___Z(
    CFrameWnd* pThis, int bPreview, CPrintPreviewState* pState);
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(
    CFrameWnd* pThis, MSG* pMsg);
// CDockingManager (featurepack/docking/CDockingManager.cpp)
extern "C" void MS_ABI impl__OnActivateFrame_CDockingManager__UEAAXH_Z(
    CDockingManager* pThis, int bActivate);
extern "C" int MS_ABI impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(
    CDockingManager* pThis, CWnd* pFrame);
extern "C" int MS_ABI impl__SendMessageToMiniFrames_CDockingManager__QEAAHI_K_J_Z(
    CDockingManager* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam);
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
    const CDockingManager* pThis, CPoint point, int nSensitivity, int bExactBar,
    CRuntimeClass* pRTCBarType, int bCheckVisibility, const CDockingManager* pDockManager);
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
    const CDockingManager* pThis, CPoint point, int nSensitivity, unsigned long* dwAlignment,
    CRuntimeClass* pRTCBarType, const CDockingManager* pDockManager);
// CBasePane (featurepack/docking/CBasePane.cpp)
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(
    CBasePane* pThis, int bShow, int bDelay, int bActivate);
// CDockingManager exports that OpenMFC still ships as GENERATED stubs whose
// parameter lists omit the `this` pointer (they ignore their arguments and
// return 0 / do nothing).  Declared here with the real MSVC signature, so the
// delegation retail performs is expressed structurally and starts working the
// moment those stubs get real bodies.
extern "C" int MS_ABI impl__ShowPanes_CDockingManager__UEAAHH_Z(
    CDockingManager* pThis, int bShow);
extern "C" int MS_ABI impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(
    const CDockingManager* pThis, CPoint point, unsigned long& dwBarAlignment, int& bOuterEdge);

// ---- forward declarations for thunks defined later in THIS file ----
extern "C" void MS_ABI impl__AdjustDockingLayout_CFrameWndEx__UEAAXPEAX_Z(
    CFrameWndEx* pThis, void* pParam);
extern "C" void MS_ABI impl__AdjustClientArea_CFrameWndEx__MEAAXXZ(CFrameWndEx* pThis);
extern "C" void MS_ABI impl__RecalcLayout_CFrameWndEx__UEAAXH_Z(CFrameWndEx* pThis, int bNotify);
extern "C" int MS_ABI impl__OnSetMenu_CFrameWndEx__IEAAHPEAUHMENU_____Z(
    CFrameWndEx* pThis, HMENU hMenu);

// EVERY RVA IN THIS FILE IS AN mfc140u.dll RVA -- the image this project
// clones -- resolved through the symbol->ordinal->RVA join described in the
// header of phase4/src/core/ole/COleControl.cpp.  (mfc140.dll, the ANSI twin,
// has the same bodies at different addresses; for the functions that live in
// the CFrameWndEx region its RVAs ran exactly 0x1d0 BELOW the mfc140u ones in
// every case compared here (that offset is a local observation, not a rule --
// the DLL-wide folded bodies such as 0x27d0 do not follow it).  A wave that
// disassembled
// mfc140 rewrote a dozen citations here and declared the original mfc140u
// addresses "wrong"; they were not, and they have been restored.)
//
// The CFrameWndEx vtable was read out of retail mfc140u .rdata at 0x2e9708;
// the byte offsets quoted below refer to it:
//   +0x248 CWnd::DefWindowProcW  +0x300 RecalcLayout(BOOL)
//   +0x390 AdjustDockingLayout   +0x3a0 AdjustClientArea
//   +0x3a8 GetInPlaceActiveItem  +0x3b0 OnShowPopupMenu  +0x3f0 OnShowPanes
// The CDockingManager vtable is at 0x2e6120:
//   +0x28/+0x30 PaneFromPoint  +0x58 FindPaneByID  +0x70 FixupVirtualRects
//   +0x78 RecalcLayout  +0x80 AdjustDockingLayout  +0x88 OnMoveMiniFrame
//   +0xb0 ShowPanes  +0xc0 OnActivateFrame
//
// Retail CFrameWndEx embeds a CFrameImpl sub-object at this+0x1f0 (all the
// `add $0x1f0,%rcx; call ...` sites below) and a CDockingManager at this+0x380.
// OpenMFC models neither as a member: there is no CFrameImpl field at all, and
// the docking manager is the lazily allocated m_pDockingManager.  Every
// CFrameImpl delegation is therefore dropped -- and note that OpenMFC's own
// CFrameImpl exports (phase4/src/core/frame/CFrameImpl.cpp) are themselves
// no-op stubs, so on this build those calls would have no effect anyway.

// CPoint is a trivially copyable 8-byte aggregate, so under MS x64 it arrives
// by value in one integer register.  Some handlers below spell that parameter
// `CPoint` (where the value is forwarded to a callee that wants a CPoint) and
// the older ones spell it `unsigned long long`; both are the same ABI.

//=============================================================================
// Simple / no-op methods
//=============================================================================




//=============================================================================
// Docking delegation (retail forwards to the embedded CDockingManager at
// this+0x380; OpenMFC routes through the lazily-allocated m_pDockingManager)
//=============================================================================










//=============================================================================
// Frame/menu lifecycle delegation
//=============================================================================













//=============================================================================
// Message-handler stubs (unmodeled state)
//=============================================================================






// Symbol: ?CreateObject@CFrameWndEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CFrameWndEx__SAPEAVCObject__XZ() {
    return new CFrameWndEx();
}
// Symbol: ?GetThisClass@CFrameWndEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWndEx__SAPEAUCRuntimeClass__XZ() {
    return &CFrameWndEx::classCFrameWndEx;
}
// Symbol: ?GetRuntimeClass@CFrameWndEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFrameWndEx__UEBAPEAUCRuntimeClass__XZ(CFrameWndEx* pThis) {
    return pThis ? &CFrameWndEx::classCFrameWndEx : nullptr;
}
// CFrameWndEx::OnShowPopupMenu(CMFCPopupMenu*) -- retail body at RVA 0x687d0
// (it is a distinct export in mfc140u; only in the ANSI twin is it ICF-folded
// onto CMDIFrameWndEx's, and the CFrameWndEx vtable at 0x2e9708 slot +0x3b0
// holds this address either way).  Transcribed:
//     if (afxGlobalData /*0x3c1620*/ not yet initialised)
//         AFX_GLOBAL_DATA::Initialize(&afxGlobalData);   // 0x6a790
//     if (afxGlobalData.<flag at +0x278, i.e. 0x3c1898> && pMenuPopup != NULL)
//         ::NotifyWinEvent(EVENT_SYSTEM_MENUPOPUPSTART /*6*/,
//                          pMenuPopup->m_hWnd /*popup+0x40*/, 0, 0);
//                          // USER32 IAT slot 0x2c7210
//     return TRUE;
// DEVIATION: the accessibility gate lives in AFX_GLOBAL_DATA, which OpenMFC
// does not model, so the notification is instead gated on the popup actually
// having a window.  Everything else matches.
// Symbol: ?OnShowPopupMenu@CFrameWndEx@@UEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__OnShowPopupMenu_CFrameWndEx__UEAAHPEAVCMFCPopupMenu___Z(
    CFrameWndEx* pThis, CMFCPopupMenu* pMenuPopup) {
    (void)pThis;
    if (pMenuPopup) {
        HWND hWnd = pMenuPopup->GetSafeHwnd();
        if (hWnd) ::NotifyWinEvent(EVENT_SYSTEM_MENUPOPUPSTART, hWnd, 0, 0);
    }
    return TRUE;
}
// CFrameWndEx::OnClosePopupMenu(CMFCPopupMenu*) -- retail (RVA 0x675d0):
//     if (afxGlobalData /*0x3c1620*/ not yet initialised)
//         AFX_GLOBAL_DATA::Initialize(&afxGlobalData);
//     if (afxGlobalData.<flag at +0x278, i.e. 0x3c1898> && pMenuPopup != NULL) {
//         CMFCPopupMenu* pParent = pMenuPopup->GetParentPopupMenu();   // 0xb7550
//         UINT ev = EVENT_SYSTEM_MENUPOPUPEND;                         // 7
//         if (pMenuPopup->[+0x19ac] == 0 && pParent == NULL &&
//             pMenuPopup->[+0x228] != NULL)
//             ev = EVENT_SYSTEM_MENUEND;                               // 5
//         ::NotifyWinEvent(ev, pMenuPopup->m_hWnd, 0, 0);
//     }
//     if (CMFCPopupMenu::m_pActivePopupMenu /*0x3be288*/ == pMenuPopup)
//         CMFCPopupMenu::m_pActivePopupMenu = NULL;
//     this->[+0x6c8] = 0;
// DEVIATION: the accessibility gate is gated on the popup having a window
// instead (see OnShowPopupMenu).  The EVENT_SYSTEM_MENUEND sub-case cannot be
// selected here: it needs pMenuPopup+0x19ac and +0x228, which the simplified
// CMFCPopupMenu does not carry, so EVENT_SYSTEM_MENUPOPUPEND is always sent.
// The active-popup global and the frame slot at this+0x6c8 are not modelled.
// Symbol: ?OnClosePopupMenu@CFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_CFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(
    CFrameWndEx* pThis, CMFCPopupMenu* pMenuPopup) {
    (void)pThis;
    if (!pMenuPopup) return;
    HWND hWnd = pMenuPopup->GetSafeHwnd();
    if (hWnd) ::NotifyWinEvent(EVENT_SYSTEM_MENUPOPUPEND, hWnd, 0, 0);
}
// CFrameWndEx::OnShowPanes(BOOL) -- retail (RVA 0x68510), transcribed exactly:
//     BOOL b = m_dockManager.ShowPanes(bShow);   // vtable +0xb0 on this+0x380
//     AdjustDockingLayout(NULL);                 // own vtable +0x390, edx = 0
//     return b;
// Symbol: ?OnShowPanes@CFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_CFrameWndEx__UEAAHH_Z(CFrameWndEx* pThis, int bShow) {
    if (!pThis) return FALSE;
    const int bResult = impl__ShowPanes_CDockingManager__UEAAHH_Z(
        pThis->GetDockingManager(), bShow);
    impl__AdjustDockingLayout_CFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
    return bResult;
}
// CFrameWndEx::OnToolbarCreateNew(WPARAM, LPARAM) -- retail (RVA 0x67910):
//     if (lParam == 0) AfxThrowInvalidArgException();               // 0x227720
//     return (LRESULT) m_Impl.CreateNewToolBar((LPCTSTR) lParam);   // 0x629f0
// The whole body is the CFrameImpl delegation, and OpenMFC has neither the
// m_Impl sub-object nor AfxThrowInvalidArgException, so this stays a stub.
// Symbol: ?OnToolbarCreateNew@CFrameWndEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarCreateNew_CFrameWndEx__QEAA_J_K_J_Z(CFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    // TODO(clean-room): not implemented -- CFrameImpl::CreateNewToolBar is the
    // entire body and the CFrameImpl sub-object is not modelled.
    return 0;
}
// CFrameWndEx::ActiveItemRecalcLayout() -- retail (RVA 0x68370), transcribed
// in full (it is fourteen instructions):
//     COleClientItem* pItem = GetInPlaceActiveItem();   // own vtable +0x3a8
//     if (pItem && pItem->[+0xa8]) pItem->[+0xa8]->vtable[+0xb0]();
//     AdjustClientArea();                               // own vtable +0x3a0
// CORRECTION: this body used to call CFrameWnd::RecalcLayout(), which retail
// never does here -- the tail call is AdjustClientArea, a far narrower
// operation (it only repositions the AFX_IDW_PANE_FIRST child).  Routed to the
// AdjustClientArea thunk instead, so it does the right thing as soon as that
// one has a body; today that thunk is a documented no-op, which is why this
// function currently does nothing.  The item+0xa8 map is not modelled.
// Symbol: ?ActiveItemRecalcLayout@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__ActiveItemRecalcLayout_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    // TODO(clean-room): retail first notifies the in-place active item's OLE
    // map (item+0xa8, vtable +0xb0); that member is not modelled.
    impl__AdjustClientArea_CFrameWndEx__MEAAXXZ(pThis);
}
// CFrameWndEx::AddDockSite() -- retail RVA 0x27d0.  That address is the
// DLL-wide empty body `ret`, shared by 158 exports through identical-code
// folding, so the retail implementation provably does nothing at all; the
// public MFC documentation ("this method is not used") agrees.  The empty body
// below is therefore a complete, evidence-backed transcription, not a stub.
// (A wave that disassembled the ANSI twin claimed this export "has no RVA" and
// that 0x27d0 "does not even decode as a function"; both claims were wrong --
// it decodes as `ret` and it is this function.)
// Symbol: ?AddDockSite@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    (void)pThis;
}
// CFrameWndEx::AdjustClientArea() -- retail entry is RVA 0x67ed0.  Transcribed:
//     CWnd* pWnd = GetDlgItem(AFX_IDW_PANE_FIRST /*0xe900*/);
//     if (!pWnd) return;
//     CRect rc = this->[+0x548 .. +0x554];          // 16 bytes, one movups
//     rc.left += this->[+0x110]; rc.top    += this->[+0x114];
//     rc.right-= this->[+0x118]; rc.bottom -= this->[+0x11c];
//     pWnd->CalcWindowRect(&rc, 0);                 // CWnd vtable +0xe0 (0x28edc0)
//     DWORD dwAdd = (pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)) ||
//                    pWnd->IsKindOf(RUNTIME_CLASS(CFormView)))
//                 ? WS_CLIPSIBLINGS
//                 : (WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
//     pWnd->ModifyStyle(0, dwAdd, 0);               // 0x2a96f0
//     pWnd->SetWindowPos(&CWnd::wndBottom /*0x3c3460*/, rc.left, rc.top,
//                        rc.Width(), rc.Height(), SWP_NOACTIVATE);  // 0x2a9a60
// Every source rectangle is an unmodelled CFrameWndEx/CFrameWnd member
// (this+0x110..0x11c and this+0x548..0x554), so nothing here can be reproduced
// without inventing offsets.  Left a no-op.
// Symbol: ?AdjustClientArea@CFrameWndEx@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustClientArea_CFrameWndEx__MEAAXXZ(CFrameWndEx* pThis) {
    (void)pThis;
    // TODO(clean-room): not implemented -- the border rect at this+0x110 and the
    // client rect at this+0x548 are not modelled in the simplified headers.
}
// CFrameWndEx::DockPane(CBasePane*, UINT, LPCRECT) — retail (RVA 0x67dc0):
//     add $0x380,%rcx; jmp CDockingManager::DockPane
// Symbol: ?DockPane@CFrameWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_CFrameWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    CFrameWndEx* pThis, CBasePane* pBar, unsigned int nDockBarID, const RECT* lpRect) {
    if (!pThis) return;
    impl__DockPane_CDockingManager__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
        pThis->GetDockingManager(), pBar, nDockBarID, lpRect);
}
// CFrameWndEx::DockPaneLeftOf(CPane*, CPane*) — retail (RVA 0x67970):
//     add $0x380,%rcx; jmp CDockingManager::DockPaneLeftOf
// Symbol: ?DockPaneLeftOf@CFrameWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_CFrameWndEx__QEAAHPEAVCPane__0_Z(
    CFrameWndEx* pThis, CPane* pBarToDock, CPane* pBar) {
    if (!pThis) return FALSE;
    return impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(
        pThis->GetDockingManager(), pBarToDock, pBar);
}
// CFrameWndEx::EnableAutoHidePanes(DWORD) — retail (RVA 0x68020):
//     add $0x380,%rcx; jmp CDockingManager::EnableAutoHidePanes
// Symbol: ?EnableAutoHidePanes@CFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_CFrameWndEx__QEAAHK_Z(
    CFrameWndEx* pThis, unsigned long dwAlignment) {
    if (!pThis) return FALSE;
    return impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(
        pThis->GetDockingManager(), dwAlignment);
}
// CFrameWndEx::InsertPane(CBasePane*, CBasePane*, int) — retail (RVA 0x67da0):
//     add $0x380,%rcx; jmp CDockingManager::InsertPane
// Symbol: ?InsertPane@CFrameWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_CFrameWndEx__QEAAHPEAVCBasePane__0H_Z(
    CFrameWndEx* pThis, CBasePane* pPane, CBasePane* pTarget, int nInsertAfter) {
    if (!pThis) return FALSE;
    return impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
        pThis->GetDockingManager(), pPane, pTarget, nInsertAfter);
}
// CFrameWndEx::AddPane(CBasePane*, BOOL bSelect) -- retail (RVA 0x67d00),
// transcribed:
//     if (pBar) {
//         if (pBar->IsKindOf(RUNTIME_CLASS(CMFCRibbonBar) /*0x2fea10*/) &&
//             pBar->vtable[+0x690]())
//             this->[+0x370] = pBar;                 // the frame's ribbon bar
//         if (pBar->IsKindOf(RUNTIME_CLASS(CMFCRibbonStatusBar) /*0x30d170*/))
//             this->[+0x378] = pBar;
//     }
//     return m_dockManager.AddPane(pBar, bSelect, FALSE, FALSE);   // 0x47860
// The RTTI checks and the unmodeled 0x370/0x378 slots are skipped; the
// docking-manager registration is forwarded.
// CORRECTION: the third argument used to be passed as bSelect; retail passes a
// literal 0 there (r9d is xor'd at 0x180067d77).  It is passed as 0 now.
// Symbol: ?AddPane@CFrameWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_CFrameWndEx__QEAAHPEAVCBasePane__H_Z(
    CFrameWndEx* pThis, CBasePane* pBar, int bSelect) {
    if (!pThis || !pBar) return FALSE;
    // TODO(clean-room): transcribed partially — retail first checks IsKindOf
    // (RuntimeClass 0x2fea10 / 0x30d170) and caches pBar at this+0x370/0x378.
    return impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
        pThis->GetDockingManager(), pBar, bSelect, 0, 0);
}
// CFrameWndEx::GetPane(UINT nID) — retail (RVA 0x68030):
//     add $0x380,%rcx; r8d=1; CDockingManager::GetPane(nID, TRUE)
// CDockingManager::GetPane(nID, TRUE) is FindPaneByID(nID, TRUE) in MFC.
// Symbol: ?GetPane@CFrameWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" CBasePane* MS_ABI impl__GetPane_CFrameWndEx__QEAAPEAVCBasePane__I_Z(
    CFrameWndEx* pThis, unsigned int nID) {
    if (!pThis) return nullptr;
    return impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
        pThis->GetDockingManager(), nID, TRUE);
}
// CFrameWndEx::IsPointNearDockSite(CPoint, DWORD&, int&) -- retail entry is
// RVA 0x67e40 and it is a two-instruction tail jump:
//     add $0x380,%rcx ; jmp 0x180048310
// 0x180048310 is ?IsPointNearDockSite@CDockingManager@@QEBAHVCPoint@@AEAKAEAH@Z,
// i.e. a plain forward to the embedded docking manager.  (An earlier comment
// here cited these same two addresses but claimed the target is not exported;
// that single claim was wrong -- the address and the RVA were right, and the
// target is an export.)  Forwarded through m_pDockingManager.
// NOTE: OpenMFC's CDockingManager::IsPointNearDockSite is still a generated
// stub that returns 0 without touching the out-parameters, so this currently
// still reports "not near a dock site".
// Symbol: ?IsPointNearDockSite@CFrameWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_CFrameWndEx__QEBAHVCPoint__AEAKAEAH_Z(
    const CFrameWndEx* pThis, CPoint pt, unsigned long& dwBarAlignment, int& bOuterEdge) {
    if (!pThis) return FALSE;
    CDockingManager* pDock = const_cast<CFrameWndEx*>(pThis)->GetDockingManager();
    if (!pDock) return FALSE;
    return impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(
        pDock, pt, dwBarAlignment, bOuterEdge);
}
// CFrameWndEx::AdjustDockingLayout(void*) — retail (RVA 0x67e50) checks a
// CDockingManager flag, calls its AdjustDockingLayout, then the frame's own
// AdjustClientArea / RecalcLayout chain. The frame-level extras are dropped;
// the docking-manager relayout is forwarded.
// Symbol: ?AdjustDockingLayout@CFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CFrameWndEx__UEAAXPEAX_Z(
    CFrameWndEx* pThis, void* pParam) {
    if (!pThis) return;
    impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(
        pThis->GetDockingManager(), pParam);
    // TODO(clean-room): transcribed partially — retail also runs the frame's
    // AdjustClientArea and a conditional RecalcLayout(1) on this+0x530 state.
}
// CFrameWndEx::OnExitSizeMove() -- retail (RVA 0x68730), transcribed in full
// (it is nine instructions):
//     RecalcLayout(TRUE);                 // own vtable +0x300, edx = 1
//     m_dockManager.FixupVirtualRects();  // tail jmp, CDM vtable +0x70
// CORRECTION: the second call used to be CDockingManager::RecalcLayout (CDM
// vtable +0x78).  Slot +0x70 of the CDockingManager vtable at .rdata 0x2e6120
// is ?FixupVirtualRects@CDockingManager@@UEAAXXZ (0x48290), not RecalcLayout;
// and the first call is the CFrameWndEx override, not CFrameWnd's.  Both are
// corrected here, and both callees have real OpenMFC bodies.
// Symbol: ?OnExitSizeMove@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnExitSizeMove_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    impl__RecalcLayout_CFrameWndEx__UEAAXH_Z(pThis, TRUE);
    CDockingManager* pDock = pThis->GetDockingManager();
    if (pDock) impl__FixupVirtualRects_CDockingManager__UEAAXXZ(pDock);
}
// CFrameWndEx::DelayUpdateFrameMenu(HMENU) — retail (RVA 0x67a10) calls the
// frame OnSize virtual, sets a flag at this+0x1d0 and stores hMenuAlt at
// this+0x190 (unmodeled). The base CFrameWnd::DelayUpdateFrameMenu carries the
// documented contract.
// Symbol: ?DelayUpdateFrameMenu@CFrameWndEx@@UEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__DelayUpdateFrameMenu_CFrameWndEx__UEAAXPEAUHMENU_____Z(
    CFrameWndEx* pThis, HMENU hMenuAlt) {
    if (!pThis) return;
    pThis->CFrameWnd::DelayUpdateFrameMenu(hMenuAlt);
    // TODO(clean-room): transcribed partially — retail toggles unmodeled
    // this+0x1d0 flag and this+0x190 menu-alt slot.
}
// CFrameWndEx::OnActivate(UINT, CWnd*, BOOL) — retail (RVA 0x67980) starts by
// calling the base CFrameWnd::OnActivate then handles state 2 (UpdateWindow)
// and state 0 (tooltip setup on this+0x1f0). The base dispatch is kept.
// Symbol: ?OnActivate@CFrameWndEx@@QEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_CFrameWndEx__QEAAXIPEAVCWnd__H_Z(
    CFrameWndEx* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized) {
    if (!pThis) return;
    impl__OnActivate_CFrameWnd__IEAAXIPEAVCWnd__H_Z(pThis, nState, pWndOther, bMinimized);
    // TODO(clean-room): transcribed partially — state==2 UpdateWindow and the
    // state==0 tooltip setup touch unmodeled state (this+0x1f0).
}
// CFrameWndEx::OnActivateApp(BOOL, DWORD) -- retail entry is RVA 0x679d0.
// Transcribed:
//     m_dockManager.OnActivateFrame(bActive);   // vtable +0xc0 on this+0x380
//     m_Impl.OnActivateApp(bActive);            // tail jmp to 0x180065a60
// The docking-manager half is forwarded for real.  The CFrameImpl half is
// dropped (no such sub-object in OpenMFC).
// Symbol: ?OnActivateApp@CFrameWndEx@@QEAAXHK@Z
extern "C" void MS_ABI impl__OnActivateApp_CFrameWndEx__QEAAXHK_Z(
    CFrameWndEx* pThis, int bActive, unsigned long dwThreadID) {
    (void)dwThreadID;
    if (!pThis) return;
    CDockingManager* pDock = pThis->GetDockingManager();
    if (pDock) impl__OnActivateFrame_CDockingManager__UEAAXH_Z(pDock, bActive);
    // TODO(clean-room): retail then tail-jumps to CFrameImpl::OnActivateApp
    // (0x65a60) on the unmodelled this+0x1f0 sub-object.
}
// CFrameWndEx::OnClose() — retail (RVA 0x677a0) has an extra owner-frame
// WM_COMMAND(0xe300) fast path (this+0x6e8, unmodeled) before delegating to
// CFrameWnd::OnClose. The base close is kept.
// Symbol: ?OnClose@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnClose_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    impl__OnClose_CFrameWnd__IEAAXXZ(pThis);
    // TODO(clean-room): transcribed partially — retail's this+0x6e8 owner
    // fast path and in-place item teardown are not modeled.
}
// CFrameWndEx::OnCmdMsg(UINT, int, void*, AFX_CMDHANDLERINFO*) — retail
// (RVA 0x68830): if the base CFrameWnd::OnCmdMsg handles the command return 1,
// else forward to the embedded CDockingManager's OnCmdMsg. The docking manager
// has no exported OnCmdMsg, so only the base dispatch is performed.
// Symbol: ?OnCmdMsg@CFrameWndEx@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CFrameWndEx__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CFrameWndEx* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    if (!pThis) return FALSE;
    int handled = impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
        pThis, nID, nCode, pExtra, pHandlerInfo);
    return handled ? TRUE : FALSE;
    // TODO(clean-room): transcribed partially — retail additionally forwards
    // unhandled commands to the embedded CDockingManager::OnCmdMsg.
}
// CFrameWndEx::OnCommand(WPARAM, LPARAM) — retail (RVA 0x67690) special-cases
// menu commands before falling back to the base CFrameWnd::OnCommand. The base
// dispatch is kept.
// Symbol: ?OnCommand@CFrameWndEx@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CFrameWndEx__MEAAH_K_J_Z(
    CFrameWndEx* pThis, WPARAM wParam, LPARAM lParam) {
    if (!pThis) return FALSE;
    return impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(pThis, wParam, lParam);
    // TODO(clean-room): transcribed partially — retail menu-command fast path
    // (wParam hi-word==1, this+0x1f0 sub-object) is not modeled.
}
// CFrameWndEx::OnCompositionChanged() -- retail entry is RVA 0x68b80.  The
// whole body is
//     add $0x1f0,%rcx ; jmp 0x180066ca0     // CFrameImpl::OnCompositionChanged
// i.e. nothing but the CFrameImpl delegation, which OpenMFC does not model.
// (OpenMFC's own CFrameImpl::OnCompositionChanged export is a no-op stub too.)
// Symbol: ?OnCompositionChanged@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnCompositionChanged_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    (void)pThis;
    // TODO(clean-room): not implemented -- the entire retail body is the
    // CFrameImpl sub-object delegation.
}
// CFrameWndEx::OnContextHelp() — retail (RVA 0x678b0) toggles an unmodeled
// this+0x1dc flag around a help-state loop (CWinApp helpers + context help
// toggles). The base CFrameWnd::OnContextHelp carries the documented contract.
// Symbol: ?OnContextHelp@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    impl__OnContextHelp_CFrameWnd__QEAAXXZ(pThis);
    // TODO(clean-room): transcribed partially.
}
// CFrameWndEx::OnCreate(CREATESTRUCTW*) — retail (RVA 0x68970): run the base
// CFrameWnd::OnCreate; if it fails return -1, otherwise stash a WS-style bit
// into this+0x218 and set up the unmodeled 0x1f0 sub-object before returning 0.
// Symbol: ?OnCreate@CFrameWndEx@@QEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CFrameWndEx__QEAAHPEAUtagCREATESTRUCTW___Z(
    CFrameWndEx* pThis, CREATESTRUCTW* pCreateStruct) {
    if (!pThis) return -1;
    int result = impl__OnCreate_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(pThis, pCreateStruct);
    if (result == -1) return -1;
    // TODO(clean-room): transcribed partially — retail records
    // (cs->dwStyle >> 23) & 1 at this+0x218 and initializes the this+0x1f0
    // sub-object; both unmodeled in the simplified headers.
    return 0;
}
// CFrameWndEx::OnDestroy() — retail (RVA 0x67ba0) is a large teardown
// (docking/tooltip state on unmodeled offsets) ending at CFrameWnd::OnDestroy.
// The base teardown is kept.
// Symbol: ?OnDestroy@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    impl__OnDestroy_CFrameWnd__IEAAXXZ(pThis);
    // TODO(clean-room): transcribed partially.
}
// CFrameWndEx::OnDrawMenuImage(CDC*, const CMFCToolBarMenuButton*, const CRect&)
// -- retail entry is RVA 0x67670.  The whole body is five instructions:
//     rcx = this->[+0x370];  if (rcx) jmp 0x1800de8e0;  else return 0;
// 0x1800de8e0 is ?DrawMenuImage@CMFCRibbonBar@@QEAAHPEAVCDC@@...@Z, so
// this+0x370 is the frame's CMFCRibbonBar* (not a generic "custom-image
// owner").  OpenMFC's CFrameWndEx carries no ribbon bar, so the null-ribbon
// arm -- return 0 -- is the only reachable one.
// Symbol: ?OnDrawMenuImage@CFrameWndEx@@UEAAHPEAVCDC@@PEBVCMFCToolBarMenuButton@@AEBVCRect@@@Z
extern "C" int MS_ABI impl__OnDrawMenuImage_CFrameWndEx__UEAAHPEAVCDC__PEBVCMFCToolBarMenuButton__AEBVCRect___Z(
    CFrameWndEx* pThis, CDC* pDC, const CMFCToolBarMenuButton* pButton, const CRect& rectImage) {
    (void)pThis;
    (void)pDC;
    (void)pButton;
    (void)rectImage;
    // TODO(clean-room): not implemented -- the ribbon bar at this+0x370 is not
    // modelled, so CMFCRibbonBar::DrawMenuImage can never be reached.
    return 0;
}
// CFrameWndEx::OnGetMinMaxInfo(MINMAXINFO*) -- retail entry is RVA 0x68770.
// Transcribed:
//     if (this->[+0x338] != 0) {                       // full-screen mode flag
//         lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x =
//             m_Impl.[+0x138] - m_Impl.[+0x130];
//         lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y =
//             m_Impl.[+0x13c] - m_Impl.[+0x134];
//         return;                                      // NOTE: no Default()
//     }
//     m_Impl.OnGetMinMaxInfo(lpMMI);                   // 0x1800654d0
//     CWnd::Default();                                 // tail jmp 0x18028ac80
// The full-screen flag and the CFrameImpl rect live at offsets OpenMFC does not
// model, so only the second arm is reproduced -- which is the arm every
// non-full-screen frame takes.
// Symbol: ?OnGetMinMaxInfo@CFrameWndEx@@QEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CFrameWndEx__QEAAXPEAUtagMINMAXINFO___Z(
    CFrameWndEx* pThis, MINMAXINFO* lpMMI) {
    (void)lpMMI;
    if (!pThis) return;
    // TODO(clean-room): the full-screen arm (this+0x338 / CFrameImpl rect) and
    // CFrameImpl::OnGetMinMaxInfo are not modelled.
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}
// CFrameWndEx::OnChangeVisualManager(WPARAM, LPARAM) -- retail entry is
// RVA 0x689e0.  The whole body is
//     m_Impl.OnChangeVisualManager();   // 0x1800668d0, on this+0x1f0
//     return 0;
// so only the constant return value survives without the CFrameImpl sub-object.
// Symbol: ?OnChangeVisualManager@CFrameWndEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnChangeVisualManager_CFrameWndEx__QEAA_J_K_J_Z(
    CFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    // TODO(clean-room): not implemented -- the only work retail does here is the
    // CFrameImpl delegation; the constant `return 0` is all that is left.
    return 0;
}
// CFrameWndEx::OnIdleUpdateCmdUI(WPARAM, LPARAM) -- retail entry is RVA 0x68560.
// Transcribed exactly:
//     m_dockManager.SendMessageToMiniFrames(WM_IDLEUPDATECMDUI /*0x363*/, 0, 0);
//     return 0;
// 0x18004d2a0 is ?SendMessageToMiniFrames@CDockingManager@@QEAAHI_K_J@Z, which
// OpenMFC implements for real, so this is a full transcription.
// Symbol: ?OnIdleUpdateCmdUI@CFrameWndEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CFrameWndEx__QEAA_J_K_J_Z(
    CFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    (void)lParam;
    if (!pThis) return 0;
    CDockingManager* pDock = pThis->GetDockingManager();
    if (pDock) {
        impl__SendMessageToMiniFrames_CDockingManager__QEAAHI_K_J_Z(
            pDock, 0x0363 /* WM_IDLEUPDATECMDUI */, 0, 0);
    }
    return 0;
}
// CFrameWndEx::OnLButtonDown(UINT, CPoint) -- retail entry is RVA 0x68b50.
// Transcribed:
//     m_Impl.OnLButtonDown(point);   // 0x180066490, on this+0x1f0
//     CWnd::Default();               // tail jmp 0x18028ac80
// The Default() half is unconditional in retail and is reproduced; the
// CFrameImpl half is dropped.
// Symbol: ?OnLButtonDown@CFrameWndEx@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CFrameWndEx__QEAAXIVCPoint___Z(
    CFrameWndEx* pThis, unsigned int nFlags, unsigned long long pt) {
    (void)nFlags;
    (void)pt;
    if (!pThis) return;
    // TODO(clean-room): retail first runs CFrameImpl::OnLButtonDown (0x66490).
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}
// CFrameWndEx::GetInPlaceActiveItem() -- retail body at RVA 0x67a50 (a
// distinct export in mfc140u; it is ICF-folded onto CMDIFrameWndEx's only in
// the ANSI twin, and slot +0x3a8 of the CFrameWndEx vtable at 0x2e9708 holds
// this address either way).  Transcribed:
//     CFrameWnd* pFrame = GetActiveFrame();          // own vtable +0x2f0,
//         // whose body there (0x10130, ICF-folded) is `mov %rcx,%rax; ret`,
//         // i.e. CFrameWnd::GetActiveFrame's `return this`
//     if (!pFrame) return NULL;
//     CView* pView = pFrame->[+0x170];               // CFrameWnd::m_pViewActive
//     if (!pView) return NULL;
//     if (pView->IsKindOf(RUNTIME_CLASS(CPreviewViewEx))) return NULL;
//     CDocument* pDoc = pView->[+0xe8];              // CView::m_pDocument
//     if (!pDoc) return NULL;
//     if (!pDoc->IsKindOf(RUNTIME_CLASS(COleDocument))) return NULL;
//     return ((COleDocument*)pDoc)->GetInPlaceActiveItem(pView);  // vtable +0x230
// (The two runtime classes were read out of .rdata at 0x2f98a8 and 0x32ea80;
// both IsKindOf calls go through 0x180234cf0 == CObject::IsKindOf.)
// Not reproduced: OpenMFC has no CPreviewViewEx runtime class, and its
// COleDocument::GetInPlaceActiveItem export is a generated stub that returns
// nullptr unconditionally, so building the chain would add no behaviour.
// Symbol: ?GetInPlaceActiveItem@CFrameWndEx@@MEAAPEAVCOleClientItem@@XZ
extern "C" COleClientItem* MS_ABI impl__GetInPlaceActiveItem_CFrameWndEx__MEAAPEAVCOleClientItem__XZ(
    CFrameWndEx* pThis) {
    (void)pThis;
    // TODO(clean-room): not implemented -- see the transcription above.
    return nullptr;
}
// CFrameWndEx::NegotiateBorderSpace(UINT, RECT*) -- retail entry is RVA 0x683d0.
// The helper it calls, 0x1802a03a0, IS an export:
// ?NegotiateBorderSpace@CFrameWnd@@UEAAHIPEAUtagRECT@@@Z (an earlier comment
// here claimed it is not; that was this file's only wrong claim about it).
// Transcribed:
//     if (nBorderCmd == 1) {
//         CFrameWnd::NegotiateBorderSpace(1, lpRectBorder);
//         if (!lpRectBorder) AfxThrowInvalidArgException();
//         *(CRect*)lpRectBorder = this->[+0x548 .. +0x554];   // one movdqu
//         return TRUE;
//     }
//     if (nBorderCmd == 3) return CFrameWnd::NegotiateBorderSpace(3, lpRectBorder);
//     return TRUE;                                    // every other command,
//                                                     // command 2 included
// The base delegation is reproduced.  DEVIATION: the cmd-1 copy of the frame's
// internal border rect (this+0x548) is dropped -- that member is not modelled --
// and OpenMFC has no AfxThrowInvalidArgException, so a null lpRectBorder simply
// returns TRUE instead of throwing.
// Symbol: ?NegotiateBorderSpace@CFrameWndEx@@UEAAHIPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__NegotiateBorderSpace_CFrameWndEx__UEAAHIPEAUtagRECT___Z(
    CFrameWndEx* pThis, unsigned int nBorderCmd, RECT* lpRectBorder) {
    if (!pThis) return TRUE;
    if (nBorderCmd == 1) {
        impl__NegotiateBorderSpace_CFrameWnd__UEAAHIPEAUtagRECT___Z(pThis, 1, lpRectBorder);
        // TODO(clean-room): retail then copies this+0x548 into *lpRectBorder.
        return TRUE;
    }
    if (nBorderCmd == 3) {
        return impl__NegotiateBorderSpace_CFrameWnd__UEAAHIPEAUtagRECT___Z(
            pThis, 3, lpRectBorder);
    }
    return TRUE;
}
// CFrameWndEx::HtmlHelpW(ULONG_PTR, UINT) — retail (RVA 0x67870): when
// (dwData != 0) or the unmodeled this+0x1dc context-help flag is clear it
// forwards to the generic CWnd::HtmlHelpW; otherwise it runs the on-window
// context-help path. The unmodeled flag defaults to inactive, so the generic
// dispatch is used.
// Symbol: ?HtmlHelpW@CFrameWndEx@@MEAAX_KI@Z
extern "C" void MS_ABI impl__HtmlHelpW_CFrameWndEx__MEAAX_KI_Z(
    CFrameWndEx* pThis, unsigned __int64 data, unsigned int command) {
    if (!pThis) return;
    impl__HtmlHelpW_CWnd__UEAAX_KI_Z(pThis, data, command);
    // TODO(clean-room): transcribed partially — retail's this+0x1dc
    // context-help branch is not modeled.
}
// CFrameWndEx::OnUpdatePaneMenu(CCmdUI*) -- retail (RVA 0x67eb0), transcribed:
//     add    $0x380,%rcx                     // the embedded CDockingManager
//     mov    0x8(%rdx),%edx                  // pCmdUI->m_nID
//     mov    $0x1,%r8d                       // bSearchMiniFrames = TRUE
//     mov    (%rcx),%rax; mov 0x58(%rax),%rax; call *__guard_dispatch_icall
//                                            // vtable slot 11 == FindPaneByID
//     test   %rax,%rax; je 0x180067f04
//     mov    (%rdi),%rcx; mov 0x8(%rcx),%rbx // pCmdUI vtable slot 1 == SetCheck
//     call   0x1802a75a0                     // ?GetStyle@CWnd@@QEBAKXZ
//     shr    $0x1c,%eax; and $0x1,%eax       // bit 28 == WS_VISIBLE
//     ... call *__guard_dispatch_icall       // pCmdUI->SetCheck(visible)
//   0x180067f04:
//     movl   $0x1,0x2c(%rdi)                 // pCmdUI->m_bContinueRouting = TRUE
// Slot 11 of the CDockingManager vtable (0x2e4070) is
// ?FindPaneByID@CDockingManager@@UEAAPEAVCBasePane@@IH@Z (0x180047e20); retail
// reaches it through the sub-object at this+0x380, which OpenMFC models as the
// lazily-allocated m_pDockingManager reached via GetDockingManager().
// This body previously called Enable(TRUE) -- retail calls Enable on neither
// path, so both the check state and the routing flag were wrong.
// TODO(clean-room): the pane-not-found path is not modeled. OpenMFC's CCmdUI
// (include/openmfc/afxole.h:68) declares no m_bContinueRouting, and its member
// order does not place anything at retail's pCmdUI+0x2c, so the routing flag
// cannot be set from here without a CCmdUI layout change.
// Symbol: ?OnUpdatePaneMenu@CFrameWndEx@@QEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_CFrameWndEx__QEAAXPEAVCCmdUI___Z(
    CFrameWndEx* pThis, CCmdUI* ui) {
    if (!ui || !pThis) return;

    CDockingManager* pManager = pThis->GetDockingManager();
    if (!pManager) return;

    CBasePane* pPane = impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
        pManager, ui->m_nID, TRUE);
    if (pPane == nullptr) return;

    const unsigned long dwStyle =
        impl__GetStyle_CWnd__QEBAKXZ(reinterpret_cast<const CWnd*>(pPane));
    impl__SetCheck_CCmdUI__UEAAXH_Z(ui, (dwStyle & WS_VISIBLE) ? 1 : 0);
}
CFrameWndEx::CFrameWndEx()
    : m_pVisualManager(nullptr), m_pDockingManager(nullptr) {
    memset(_framewndex_padding, 0, sizeof(_framewndex_padding));
}
CFrameWndEx::~CFrameWndEx() {
    if (m_pDockingManager) delete m_pDockingManager;
}
BOOL CFrameWndEx::Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName, DWORD dwStyle,
                          const RECT& rect, CWnd* pParentWnd, const wchar_t* lpszMenuName,
                          DWORD dwExStyle, CCreateContext* pContext) {
    return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect,
                             pParentWnd, lpszMenuName, dwExStyle, pContext);
}
BOOL CFrameWndEx::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
    return CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}
CMFCVisualManager* CFrameWndEx::GetVisualManager() const { return m_pVisualManager; }
CDockingManager* CFrameWndEx::GetDockingManager() {
    if (!m_pDockingManager) m_pDockingManager = new CDockingManager(this);
    return m_pDockingManager;
}
BOOL CFrameWndEx::EnableDocking(DWORD dwDockStyle) {
    if (!m_pDockingManager) m_pDockingManager = new CDockingManager(this);
    return m_pDockingManager && m_pDockingManager->EnableDocking(dwDockStyle);
}
// CFrameWndEx::OnLButtonUp(UINT, CPoint) -- retail (RVA 0x68af0):
//     m_Impl.OnLButtonUp(point);   // 0x1800664e0, on this+0x1f0
//     CWnd::Default();             // tail jmp 0x18028ac80
// The unconditional Default() is reproduced; the CFrameImpl half is dropped.
// Symbol: ?OnLButtonUp@CFrameWndEx@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CFrameWndEx__QEAAXIVCPoint___Z(
    CFrameWndEx* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags;
    (void)point;
    if (!pThis) return;
    // TODO(clean-room): retail first runs CFrameImpl::OnLButtonUp (0x664e0).
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CFrameWndEx::OnMenuChar(UINT, UINT, CMenu*) -- retail (RVA 0x67020):
//     if (m_Impl.OnMenuChar(nChar))            // 0x180064660, on this+0x1f0
//         return 0xffff0002;                   // the literal WM_MENUCHAR result
//                                              // retail returns here; I did not
//                                              // decode its MNC_* decomposition
//     return CFrameWnd::OnMenuChar(nChar, nFlags, pMenu);   // 0x1802a1010
// Without the CFrameImpl sub-object the first arm can never be taken, so the
// base delegation -- the arm every frame without a menu bar takes -- is used.
// Symbol: ?OnMenuChar@CFrameWndEx@@QEAA_JIIPEAVCMenu@@@Z
extern "C" __int64 MS_ABI impl__OnMenuChar_CFrameWndEx__QEAA_JIIPEAVCMenu___Z(
    CFrameWndEx* pThis, unsigned int nChar, unsigned int nFlags, CMenu* pMenu) {
    if (!pThis) return 0;
    // TODO(clean-room): retail first offers the character to CFrameImpl (0x64660).
    return impl__OnMenuChar_CFrameWnd__IEAA_JIIPEAVCMenu___Z(pThis, nChar, nFlags, pMenu);
}

// CFrameWndEx::OnMouseMove(UINT, CPoint) -- retail (RVA 0x68b20):
//     m_Impl.OnMouseMove(point);   // 0x1800665e0, on this+0x1f0
//     CWnd::Default();             // tail jmp 0x18028ac80
// Symbol: ?OnMouseMove@CFrameWndEx@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CFrameWndEx__QEAAXIVCPoint___Z(
    CFrameWndEx* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags;
    (void)point;
    if (!pThis) return;
    // TODO(clean-room): retail first runs CFrameImpl::OnMouseMove (0x665e0).
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CFrameWndEx::OnMoveMiniFrame(CWnd*) -- retail (RVA 0x67ff0) is a four
// instruction tail jump, transcribed exactly:
//     add $0x380,%rcx ; jmp *(vtable +0x88)
// i.e. return m_dockManager.OnMoveMiniFrame(pFrame).
// Symbol: ?OnMoveMiniFrame@CFrameWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_CFrameWndEx__UEAAHPEAVCWnd___Z(
    CFrameWndEx* pThis, CWnd* pFrame) {
    if (!pThis) return FALSE;
    CDockingManager* pDock = pThis->GetDockingManager();
    if (!pDock) return FALSE;
    return impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(pDock, pFrame);
}

// CFrameWndEx::OnNcActivate(BOOL) -- retail (RVA 0x68930):
//     if (m_Impl.OnNcActivate(bActive)) return TRUE;   // 0x180065ba0
//     return CFrameWnd::OnNcActivate(bActive);         // 0x18029e720
// Symbol: ?OnNcActivate@CFrameWndEx@@QEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CFrameWndEx__QEAAHH_Z(CFrameWndEx* pThis, int bActive) {
    if (!pThis) return TRUE;
    // TODO(clean-room): retail first offers the message to CFrameImpl (0x65ba0).
    return impl__OnNcActivate_CFrameWnd__IEAAHH_Z(pThis, bActive);
}

// CFrameWndEx::OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*) -- retail (RVA 0x68ac0):
//     if (!m_Impl.OnNcCalcSize(bCalcValidRects, lpncsp))   // 0x1800657c0
//         CWnd::Default();                                 // 0x18028ac80
// Symbol: ?OnNcCalcSize@CFrameWndEx@@QEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CFrameWndEx__QEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    CFrameWndEx* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    (void)bCalcValidRects;
    (void)lpncsp;
    if (!pThis) return;
    // TODO(clean-room): retail first offers the message to CFrameImpl (0x657c0);
    // that handler returning TRUE is what suppresses the Default() below.
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CFrameWndEx::OnNcHitTest(CPoint) -- retail (RVA 0x68a90):
//     UINT nHit = m_Impl.OnNcHitTest(point);   // 0x180066210
//     if (nHit != 0) return (LRESULT)(UINT) nHit;   // zero-extended
//     return CWnd::Default();                  // tail jmp 0x18028ac80
// Symbol: ?OnNcHitTest@CFrameWndEx@@QEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CFrameWndEx__QEAA_JVCPoint___Z(
    CFrameWndEx* pThis, CPoint point) {
    (void)point;
    if (!pThis) return 0;
    // TODO(clean-room): retail first offers the hit test to CFrameImpl (0x66210).
    return impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CFrameWndEx::OnNcMouseMove(UINT, CPoint) -- retail (RVA 0x68a00):
//     m_Impl.OnNcMouseMove(nHitTest, point);          // 0x180066440
//     if (nHitTest == HTCAPTION && (GetStyle() & WS_MAXIMIZE) &&
//         !(m_pRibbonBar /*this+0x370*/ &&
//           ::IsWindowVisible(m_pRibbonBar->m_hWnd) &&
//           m_pRibbonBar->[+0x468] != 0) &&
//         CMFCVisualManager::GetInstance()->vtable[+0x3f8]())
//         return;                                     // owner-drawn caption
//     CWnd::Default();
// (0x180009774 is the CMFCVisualManager singleton getter -- an unexported
// helper that constructs a CMFCVisualManager, or the registered derived class
// via CRuntimeClass::CreateObject (0x234d60); the +0x3f8 virtual's base body at
// 0x71e0 is the DLL-wide `xor eax,eax; ret` fold, i.e. it returns FALSE by
// default.)
// DEVIATION: neither the ribbon bar nor the visual-manager virtual is modelled
// here, so Default() is called unconditionally.  With a stock visual manager
// (whose +0x3f8 virtual returns FALSE) that is exactly retail's behaviour.
// Symbol: ?OnNcMouseMove@CFrameWndEx@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CFrameWndEx__QEAAXIVCPoint___Z(
    CFrameWndEx* pThis, unsigned int nHitTest, CPoint point) {
    (void)nHitTest;
    (void)point;
    if (!pThis) return;
    // TODO(clean-room): retail first runs CFrameImpl::OnNcMouseMove (0x66440).
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CFrameWndEx::OnNcPaint() -- retail (RVA 0x688c0):
//     if (!m_Impl.OnNcPaint())    // 0x1800653a0
//         CWnd::Default();        // 0x18028ac80
// Symbol: ?OnNcPaint@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    // TODO(clean-room): retail first offers the paint to CFrameImpl (0x653a0);
    // that handler returning TRUE is what suppresses the Default() below.
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CFrameWndEx::OnPaneCheck(UINT) -- retail (RVA 0x680f0), transcribed in full:
//     CBasePane* pBar = m_dockManager.FindPaneByID(nID, TRUE);  // vtable +0x58
//     if (!pBar) return FALSE;
//     pBar->ShowPane((pBar->GetStyle() & WS_VISIBLE) == 0, FALSE, FALSE);
//                                                    // CBasePane vtable +0x458
//     return TRUE;
// Symbol: ?OnPaneCheck@CFrameWndEx@@QEAAHI@Z
extern "C" int MS_ABI impl__OnPaneCheck_CFrameWndEx__QEAAHI_Z(
    CFrameWndEx* pThis, unsigned int nID) {
    if (!pThis) return FALSE;
    CDockingManager* pDock = pThis->GetDockingManager();
    if (!pDock) return FALSE;
    CBasePane* pBar = impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
        pDock, nID, TRUE);
    if (!pBar) return FALSE;
    const int bShow = (impl__GetStyle_CWnd__QEBAKXZ(pBar) & WS_VISIBLE) == 0;
    impl__ShowPane_CBasePane__UEAAXHHH_Z(pBar, bShow, FALSE, FALSE);
    return TRUE;
}

// CFrameWndEx::OnPostPreviewFrame(WPARAM, LPARAM) -- retail RVA 0x71e0.  That
// address is the DLL-wide `xor eax,eax; ret` body, shared by 134 exports
// through identical-code folding, so the retail handler provably does nothing
// but return 0.  The body below is a complete transcription, not a stub.
// (A wave that only had the ANSI twin's map reported this export as having "no
// RVA and no ICF-folded twin"; it resolves fine against mfc140u.)
// Symbol: ?OnPostPreviewFrame@CFrameWndEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnPostPreviewFrame_CFrameWndEx__QEAA_J_K_J_Z(
    CFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// CFrameWndEx::OnPowerBroadcast(UINT, LPARAM) -- retail RVA 0x68d40.  The
// symbol->RVA map for mfc140u lists exactly two names at that address,
// ?OnPowerBroadcast@CMDIFrameWndEx@@IEAAII_J@Z and this one, so the ICF fold is
// established by the map itself and this IS the body:
//     LRESULT lRes = CWnd::Default();               // 0x18028ac80
//     if (nPowerEvent == PBT_APMRESUMESUSPEND /*7*/) {
//         if (afxGlobalData /*0x3c1620*/ not yet initialised)
//             AFX_GLOBAL_DATA::Initialize(&afxGlobalData);   // 0x6a790
//         afxGlobalData.Resume();                   // 0x18006c7c0
//     }
//     return (UINT) lRes;
// (An earlier wave reported this export as having "no distinct RVA" and treated
// the body as inferred from the sibling; it is neither missing nor inferred.)
// The Default() dispatch is reproduced.  The resume half is NOT: OpenMFC has no
// afxGlobalData object, and its ?Resume@AFX_GLOBAL_DATA@@QEAAHXZ export is a
// generated stub that takes no `this` and returns 0, so calling it would add
// nothing.
// Symbol: ?OnPowerBroadcast@CFrameWndEx@@QEAAII_J@Z
extern "C" unsigned int MS_ABI impl__OnPowerBroadcast_CFrameWndEx__QEAAII_J_Z(
    CFrameWndEx* pThis, unsigned int nPowerEvent, __int64 nEventData) {
    (void)nPowerEvent;
    (void)nEventData;
    if (!pThis) return 0;
    return static_cast<unsigned int>(impl__Default_CWnd__IEAA_JXZ(pThis));
}

// CFrameWndEx::OnSetMenu(HMENU) [protected] -- retail (RVA 0x670b0):
//     CMFCMenuBar* pMenuBar = this->[+0x300];
//     if (!pMenuBar) return FALSE;
//     if (hMenu == NULL) hMenu = this->[+0x238];
//     pMenuBar->vtable[+0x878](hMenu, 0, 0);        // a CMFCMenuBar virtual;
//                                                   // I did not identify which
//     return TRUE;
// The menu bar at this+0x300 and the default HMENU at this+0x238 are not
// modelled members of OpenMFC's CFrameWndEx, so the no-menu-bar arm -- return
// FALSE -- is the only reachable one.
// Symbol: ?OnSetMenu@CFrameWndEx@@IEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__OnSetMenu_CFrameWndEx__IEAAHPEAUHMENU_____Z(
    CFrameWndEx* pThis, HMENU hMenu) {
    (void)pThis;
    (void)hMenu;
    // TODO(clean-room): not implemented -- the CMFCMenuBar at this+0x300 is not
    // modelled, so retail's menu-bar rebuild cannot be reproduced.
    return FALSE;
}

// CFrameWndEx::OnSetMenu(WPARAM, LPARAM) [message handler] -- retail
// (RVA 0x67070), transcribed exactly:
//     OnSetMenu((HMENU) wParam);                          // 0x1800670b0
//     return DefWindowProc(0x230 /*WM_MDISETMENU*/, 0, lParam);  // vtable +0x248
// (Slot +0x248 of the CFrameWndEx vtable at 0x2e9708 is CWnd::DefWindowProcW.)
// Symbol: ?OnSetMenu@CFrameWndEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetMenu_CFrameWndEx__QEAA_J_K_J_Z(
    CFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return 0;
    impl__OnSetMenu_CFrameWndEx__IEAAHPEAUHMENU_____Z(
        pThis, reinterpret_cast<HMENU>(static_cast<uintptr_t>(wParam)));
    return static_cast<__int64>(impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
        pThis, 0x0230 /* WM_MDISETMENU */, 0, static_cast<LPARAM>(lParam)));
}

// CFrameWndEx::OnSetPreviewMode(BOOL, CPrintPreviewState*) -- retail
// (RVA 0x68430):
//     CFrameWnd* pTop = g_pTopLevelFrame /*0x3be1b8*/ ? : GetTopLevelFrame();
//                                                               // 0x18028e490
//     if (pTop && pTop->IsKindOf(RUNTIME_CLASS(CFrameWndEx) /*0x2e9210*/))
//         pTop->[+0x6e8] = bPreview ? this : NULL;
//     m_dockManager.SetPrintPreviewMode(bPreview, pState);      // 0x18004d080
//     int nSaved = pState->[+0x10];
//     CFrameWnd::OnSetPreviewMode(bPreview, pState);            // 0x18029fe10
//     pState->[+0x10] = nSaved;
//     AdjustDockingLayout(NULL);                                // vtable +0x390
//     RecalcLayout(TRUE);                                       // vtable +0x300
//     if (m_pRibbonBar /*this+0x370*/ && m_pRibbonBar->[+0x468])
//         ::PostMessageW(m_hWnd, AFX_WM_POSTSETPREVIEWFRAME /*0x3c1618*/,
//                        bPreview, 0);            // USER32 IAT slot 0x2c72b0
// Reproduced: the base OnSetPreviewMode, AdjustDockingLayout(NULL) and
// RecalcLayout(TRUE).  NOT reproduced (all unmodelled): the top-frame back
// pointer at +0x6e8, CDockingManager::SetPrintPreviewMode (OpenMFC ships that
// export as a no-op stub), the save/restore of CPrintPreviewState+0x10, and the
// ribbon-bar notification.
// Symbol: ?OnSetPreviewMode@CFrameWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_CFrameWndEx__UEAAXHPEAUCPrintPreviewState___Z(
    CFrameWndEx* pThis, int bPreview, CPrintPreviewState* pState) {
    if (!pThis) return;
    impl__OnSetPreviewMode_CFrameWnd__UEAAXHPEAUCPrintPreviewState___Z(pThis, bPreview, pState);
    impl__AdjustDockingLayout_CFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
    impl__RecalcLayout_CFrameWndEx__UEAAXH_Z(pThis, TRUE);
}

// CFrameWndEx::OnSetText(LPCTSTR) -- retail (RVA 0x688f0):
//     LRESULT lRes = CWnd::Default();     // 0x18028ac80
//     m_Impl.OnSetText(lpszText);         // 0x180065b50, on this+0x1f0
//     return (int) lRes;
// Symbol: ?OnSetText@CFrameWndEx@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CFrameWndEx__QEAAHPEB_W_Z(
    CFrameWndEx* pThis, const wchar_t* lpszText) {
    (void)lpszText;
    if (!pThis) return 0;
    const int nResult = static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
    // TODO(clean-room): retail then runs CFrameImpl::OnSetText (0x65b50), which
    // repaints the owner-drawn caption.
    return nResult;
}

// CFrameWndEx::OnSize(UINT, int, int) -- retail (RVA 0x68590):
//     this->[+0x1e4] = (nType == SIZE_MINIMIZED);
//     if (m_Impl.[+0x180] != NULL ||
//         (CMFCVisualManager::GetInstance()->vtable[+0x3f8]() &&
//          this->[+0x220] == 0)) {
//         ::GetWindowRect(m_hWnd, &rc);
//         m_Impl.OnWindowPosChanging(<WINDOWPOS built from rc>);  // 0x1800652b0
//     }
//     m_Impl.UpdateCaption();                                   // 0x180065ee0
//     m_dockManager.OnActivateFrame(this->[+0x1e4] == 0);       // vtable +0xc0
//     if (this->[+0x1e4] == 0 && nType != SIZE_MAXIMIZED && this->[+0x1e0] == 0) {
//         this->[+0x390] = 1;
//         CFrameWnd::OnSize(nType, cx, cy);                     // 0x1802a0450
//         AdjustDockingLayout(NULL);                            // vtable +0x390
//         this->[+0x390] = 0;
//     } else {
//         CFrameWnd::OnSize(nType, cx, cy);
//         if (nType == SIZE_MAXIMIZED ||
//             (nType == SIZE_RESTORED && this->[+0x1e0] != 0))
//             RecalcLayout(TRUE);                               // vtable +0x300
//         this->[+0x1e0] = (nType == SIZE_MAXIMIZED);
//     }
// Reproduced: the docking-manager activation notice, the base OnSize, and the
// AdjustDockingLayout / RecalcLayout follow-ups.
// DEVIATION: this+0x1e0 ("the frame was maximized last time") is not a modelled
// member, so it reads as 0 here.  The visible consequence is that a
// SIZE_RESTORED that follows a maximize takes the first branch (AdjustDocking-
// Layout) instead of retail's second one (RecalcLayout(TRUE)).  The CFrameImpl
// caption work is dropped as everywhere else in this file.
// Symbol: ?OnSize@CFrameWndEx@@QEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CFrameWndEx__QEAAXIHH_Z(
    CFrameWndEx* pThis, unsigned int nType, int cx, int cy) {
    if (!pThis) return;
    const int bMinimized = (nType == SIZE_MINIMIZED);
    CDockingManager* pDock = pThis->GetDockingManager();
    if (pDock) impl__OnActivateFrame_CDockingManager__UEAAXH_Z(pDock, !bMinimized);
    impl__OnSize_CFrameWnd__IEAAXIHH_Z(pThis, nType, cx, cy);
    if (!bMinimized && nType != SIZE_MAXIMIZED) {
        impl__AdjustDockingLayout_CFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
    } else if (nType == SIZE_MAXIMIZED) {
        impl__RecalcLayout_CFrameWndEx__UEAAXH_Z(pThis, TRUE);
    }
}

// CFrameWndEx::OnSizing(UINT, LPRECT) -- retail (RVA 0x68150), transcribed in
// full (it is nine instructions):
//     CWnd::Default();             // 0x18028ac80
//     AdjustDockingLayout(NULL);   // tail call through own vtable +0x390, edx=0
// Symbol: ?OnSizing@CFrameWndEx@@QEAAXIPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnSizing_CFrameWndEx__QEAAXIPEAUtagRECT___Z(
    CFrameWndEx* pThis, unsigned int nSide, RECT* lpRect) {
    (void)nSide;
    (void)lpRect;
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
    impl__AdjustDockingLayout_CFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
}

// CFrameWndEx::OnSysColorChange() -- retail (RVA 0x68da0), transcribed in full:
//     CWnd::OnSysColorChange();            // 0x18028f320
//     m_Impl.OnChangeVisualManager();      // 0x1800668d0, on this+0x1f0
//     ::SetWindowRgn(m_hWnd, NULL, TRUE);  // tail jmp through the USER32 IAT
//                                          // slot 0x2c6d10, resolved with
//                                          // iat.py to USER32!SetWindowRgn
// Symbol: ?OnSysColorChange@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    impl__OnSysColorChange_CWnd__IEAAXXZ(pThis);
    // TODO(clean-room): retail then runs CFrameImpl::OnChangeVisualManager.
    HWND hWnd = pThis->GetSafeHwnd();
    if (hWnd) ::SetWindowRgn(hWnd, nullptr, TRUE);
}

// CFrameWndEx::OnToolbarContextMenu(WPARAM, LPARAM) -- retail RVA 0x3a60.  That
// address is the DLL-wide `mov $0x1,%eax ; ret` body, shared by 101 exports
// through identical-code folding, so the retail handler takes no action and
// returns 1 -- the "handled, do not show the default toolbar context menu"
// answer.  Complete transcription.
// CORRECTION: this body used to `return 0`, on the strength of a claim that the
// export "has no RVA in the map and no ICF-folded twin".  It resolves fine
// against mfc140u, and 0 is the wrong answer -- returning 0 tells the caller the
// message was not handled.
// Symbol: ?OnToolbarContextMenu@CFrameWndEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarContextMenu_CFrameWndEx__QEAA_J_K_J_Z(
    CFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 1;
}

// CFrameWndEx::OnToolbarDelete(WPARAM, LPARAM) -- retail (RVA 0x67940):
//     if (lParam == 0) AfxThrowInvalidArgException();      // 0x180227720
//     return (int) m_Impl.DeleteToolBar((CMFCToolBar*) lParam);   // 0x180063520
// The whole body is the CFrameImpl delegation, and OpenMFC has neither the
// m_Impl sub-object nor AfxThrowInvalidArgException, so this stays a stub.
// Symbol: ?OnToolbarDelete@CFrameWndEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarDelete_CFrameWndEx__QEAA_J_K_J_Z(
    CFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    // TODO(clean-room): not implemented -- CFrameImpl::DeleteToolBar is the
    // entire body and the CFrameImpl sub-object is not modelled.
    return 0;
}

// CFrameWndEx::OnUpdateFrameMenu(HMENU) -- retail (RVA 0x67ae0):
//     CFrameWnd::OnUpdateFrameMenu(hMenuAlt);              // 0x18029dec0
//     BOOL bMenuBar = (this->[+0x300] != NULL) &&
//                     (this->[+0x300]->GetStyle() & WS_VISIBLE);
//     BOOL bRibbon  = (this->[+0x370] != NULL) &&
//                     (this->[+0x370]->GetStyle() & WS_VISIBLE);
//     if (!bMenuBar && !bRibbon) return;
//     COleClientItem* pItem = GetInPlaceActiveItem();      // own vtable +0x3a8
//     if (pItem && pItem->GetInPlaceWindow())              // 0x1802492a0
//         SetMenu(CMenu::FromHandle(hMenuAlt) /*0x2a80a0*/);   // own vtable +0xf0
//     else
//         SetMenu(NULL);
// OpenMFC's CFrameWndEx carries neither the CMFCMenuBar at +0x300 nor the
// CMFCRibbonBar at +0x370, so bMenuBar and bRibbon are both FALSE and retail
// returns right after the base call -- which is exactly what is done here.
// Symbol: ?OnUpdateFrameMenu@CFrameWndEx@@UEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__OnUpdateFrameMenu_CFrameWndEx__UEAAXPEAUHMENU_____Z(
    CFrameWndEx* pThis, HMENU hMenuAlt) {
    if (!pThis) return;
    impl__OnUpdateFrameMenu_CFrameWnd__UEAAXPEAUHMENU_____Z(pThis, hMenuAlt);
}

// CFrameWndEx::OnUpdateFrameTitle(BOOL) -- retail (RVA 0x68b90).  Both of its
// paths run CFrameWnd::OnUpdateFrameTitle(bAddToTitle) (0x18029fbc0); the long
// path merely brackets that call with a CString snapshot of the window text
// (CWnd::GetWindowTextW, 0x28be00) taken before and after, and -- if the two
// differ (wcscmp, CRT IAT slot 0x2c7770) -- sends WM_NCPAINT (0x85) to the
// frame (::SendMessageW, USER32 IAT slot 0x2c7120) so an owner-drawn caption
// repaints.  That path is entered only when
//     CMFCVisualManager::GetInstance()->vtable[+0x3f8]() &&
//     this->[+0x220] == 0 && ::IsWindowVisible(m_hWnd) &&
//     !(m_pRibbonBar /*this+0x370*/ visible with [+0x468] != 0)
// The +0x3f8 virtual's base body (0x71e0) is `xor eax,eax; ret`, so with a
// stock visual manager the short path -- the bare base call reproduced here --
// is what runs.  The CString bracket and the ribbon-bar test are not modelled.
// Symbol: ?OnUpdateFrameTitle@CFrameWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CFrameWndEx__UEAAXH_Z(
    CFrameWndEx* pThis, int bAddToTitle) {
    if (!pThis) return;
    impl__OnUpdateFrameTitle_CFrameWnd__UEAAXH_Z(pThis, bAddToTitle);
}

// CFrameWndEx::OnWindowPosChanged(WINDOWPOS*) -- retail (RVA 0x68890):
//     if (lpwndpos->flags & SWP_FRAMECHANGED /*0x20*/)
//         m_Impl.OnWindowPosChanging(lpwndpos);   // 0x1800652b0
//     CWnd::Default();                            // tail jmp 0x18028ac80
// The Default() is unconditional in retail and is reproduced.
// Symbol: ?OnWindowPosChanged@CFrameWndEx@@QEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CFrameWndEx__QEAAXPEAUtagWINDOWPOS___Z(
    CFrameWndEx* pThis, WINDOWPOS* lpwndpos) {
    (void)lpwndpos;
    if (!pThis) return;
    // TODO(clean-room): on SWP_FRAMECHANGED retail first runs
    // CFrameImpl::OnWindowPosChanging (0x652b0).
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CFrameWndEx::PaneFromPoint(CPoint, int, bool, CRuntimeClass*) const -- retail
// (RVA 0x67dd0), transcribed exactly:
//     return m_dockManager.PaneFromPoint(point, nSensitivity, bExactBar,
//                                        pRTCBarType, FALSE, NULL);
// (CDockingManager vtable +0x30; the 6th and 7th argument slots are both
// zeroed on the stack before the call.)
// Symbol: ?PaneFromPoint@CFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CFrameWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(
    const CFrameWndEx* pThis, CPoint point, int nSensitivity, bool bExactBar,
    CRuntimeClass* pRTCBarType) {
    if (!pThis) return nullptr;
    CDockingManager* pDock = const_cast<CFrameWndEx*>(pThis)->GetDockingManager();
    if (!pDock) return nullptr;
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
        pDock, point, nSensitivity, bExactBar ? 1 : 0, pRTCBarType, FALSE, nullptr);
}

// CFrameWndEx::PaneFromPoint(CPoint, int, DWORD&, CRuntimeClass*) const -- retail
// (RVA 0x67e10), transcribed exactly:
//     return m_dockManager.PaneFromPoint(point, nSensitivity, dwAlignment,
//                                        pRTCBarType, NULL);
// (CDockingManager vtable +0x28; the 6th argument slot is zeroed on the stack.)
// Symbol: ?PaneFromPoint@CFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CFrameWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(
    const CFrameWndEx* pThis, CPoint point, int nSensitivity, unsigned long& dwAlignment,
    CRuntimeClass* pRTCBarType) {
    if (!pThis) return nullptr;
    CDockingManager* pDock = const_cast<CFrameWndEx*>(pThis)->GetDockingManager();
    if (!pDock) return nullptr;
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
        pDock, point, nSensitivity, &dwAlignment, pRTCBarType, nullptr);
}

// CFrameWndEx::PreCreateWindow(CREATESTRUCT&) -- retail (RVA 0x67820).
// Transcribed:
//     m_dockManager.[+0x1b0] = this;      // the manager's owner-frame pointer
//     m_Impl.[+0x120]        = &m_dockManager;
//     m_Impl.RestorePosition(cs);         // 0x1800623a0
//     return CFrameWnd::PreCreateWindow(cs);   // tail jmp 0x18029d8d0
// The two stores wire the embedded sub-objects together.  OpenMFC's equivalent
// of the first is constructing m_pDockingManager with this frame as its parent,
// which GetDockingManager() does, so it is called here for that side effect.
// The CFrameImpl store and RestorePosition (which reads the saved window
// placement out of the registry) are not modelled.
// Symbol: ?PreCreateWindow@CFrameWndEx@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWndEx__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWndEx* pThis, CREATESTRUCTW& cs) {
    if (!pThis) return FALSE;
    (void)pThis->GetDockingManager();
    // TODO(clean-room): retail also hands the docking manager to the CFrameImpl
    // sub-object and calls CFrameImpl::RestorePosition(cs).
    return impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
}

// CFrameWndEx::PreTranslateMessage(MSG*) -- retail (RVA 0x67100) is a large
// switch on pMsg->message.  Its arms reach the CFrameImpl handlers
// (ProcessKeyboard 0x63710, ProcessMouseClick 0x63a10, ProcessMouseMove
// 0x64110, ProcessMouseWheel 0x64210, OnShowPopupMenu 0x64300, IsHelpKey
// 0x64850), the ribbon bar at this+0x370 (CMFCRibbonBar::OnSysKeyDown 0xe3580 /
// OnSysKeyUp 0xe3710), CFullScreenImpl::RestoreState (0x69830),
// CSmartDockingManager::CauseCancelMode (0x132390),
// CMFCToolBar::RedrawUnderlines (0x15b970) and the accessibility bookkeeping
// around afxGlobalData -- and, for every message the switch does not claim and
// as the fall-through of the claimed ones, the base
// CFrameWnd::PreTranslateMessage (0x18029ceb0).  Only that base delegation is
// reproduced: CFrameImpl and the ribbon bar are not modelled, so no arm that
// depends on them can be entered.  (The callee list above was read off the
// call/jmp targets of the whole function; it is not a claim that every arm was
// individually traced.)
// Symbol: ?PreTranslateMessage@CFrameWndEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWndEx__UEAAHPEAUtagMSG___Z(
    CFrameWndEx* pThis, MSG* pMsg) {
    if (!pThis) return FALSE;
    return impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}

// CFrameWndEx::RecalcLayout(BOOL) -- retail (RVA 0x68180):
//     if (this->[+0x1a0] != 0) return;                 // re-entrancy guard
//     int nWas = this->[+0x220];   // read as 0x30(this+0x1f0): +0x220 on the
//                                  // frame IS m_Impl+0x30, same 4 bytes
//     this->[+0x1a0] = 1;  this->[+0x220] = 0;
//     COleClientItem* pItem = GetInPlaceActiveItem();  // own vtable +0x3a8
//     if (pItem && pItem->[+0xa8] && pItem->[+0x90] == 4) {
//         this->[+0x220] = 1;
//         this->[+0x224] = (GetStyle() & WS_CAPTION /*0xc00000*/) != 0;
//     }
//     if (this->[+0x1e4] == 0) {                       // not minimized
//         if (m_dockManager.[+0x208] == 0 && this->[+0x120] == 0) {
//             m_dockManager.RecalcLayout(bNotify);     // CDM vtable +0x78
//         } else if (m_pViewActive /*this+0x170*/ is a CPreviewViewEx) {
//             m_dockManager.RecalcLayout(bNotify);
//             m_pViewActive->SetWindowPos(NULL, this->[+0x548], this->[+0x54c],
//                                         w, h, SWP_NOZORDER|SWP_NOACTIVATE);
//             goto done;                               // no AdjustClientArea
//         } else if (bNotify && this->[+0x120] != 0) {
//             ActiveItemRecalcLayout();                // 0x180068370
//             goto done;
//         } else {
//             this->[+0x1a0] = 0;
//             CFrameWnd::RecalcLayout(bNotify);        // 0x1802a0200
//         }
//         AdjustClientArea();                          // own vtable +0x3a0
//     }
// done:
//     this->[+0x1a0] = 0;
//     if (nWas != this->[+0x220]) { ModifyStyle adds or removes WS_CAPTION;
//         m_Impl.OnChangeVisualManager(); SetWindowPos(...SWP_FRAMECHANGED...); }
// Almost every discriminator here (the guard at +0x1a0, the in-place item at
// +0x120, the dock-site count at CDockingManager+0x208, the preview view, the
// in-place-caption bookkeeping at +0x220/+0x224) is an unmodelled member.
// DEVIATION, deliberately: OpenMFC calls BOTH CDockingManager::RecalcLayout and
// CFrameWnd::RecalcLayout, where retail picks exactly one branch.  Retail can
// take the docking-manager-only branch because its CDockingManager lays the
// whole frame out; OpenMFC's does not -- its control bars and status bar are
// positioned by CFrameWnd::RecalcLayout -- so dropping the base call would
// leave a CFrameWndEx frame with no layout at all.
// Symbol: ?RecalcLayout@CFrameWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CFrameWndEx__UEAAXH_Z(CFrameWndEx* pThis, int bNotify) {
    if (!pThis) return;
    CDockingManager* pDock = pThis->GetDockingManager();
    if (pDock) pDock->RecalcLayout();
    impl__RecalcLayout_CFrameWnd__UEAAXH_Z(pThis, bNotify);
    impl__AdjustClientArea_CFrameWndEx__MEAAXXZ(pThis);
}

// Symbol: ?RemovePaneFromDockManager@CFrameWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CFrameWndEx__QEAAXPEAVCBasePane__HHH0_Z(void* /*class*/* p0, int p1, int p2, int p3, void* /*class*/* p4) {}

// Symbol: ?ShowPane@CFrameWndEx@@QEAAXPEAVCBasePane@@HHH@Z
extern "C" void MS_ABI impl__ShowPane_CFrameWndEx__QEAAXPEAVCBasePane__HHH_Z(void* /*class*/* p0, int p1, int p2, int p3) {}

// Symbol: ?ShowPopupMenu@CFrameWndEx@@IEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__ShowPopupMenu_CFrameWndEx__IEAAHPEAVCMFCPopupMenu___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?WinHelpW@CFrameWndEx@@UEAAX_KI@Z
extern "C" void MS_ABI impl__WinHelpW_CFrameWndEx__UEAAX_KI_Z(unsigned __int64 p0, unsigned int p1) {}
