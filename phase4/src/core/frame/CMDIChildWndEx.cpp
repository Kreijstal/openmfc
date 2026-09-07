// CMDIChildWndEx — OpenMFC implementation.
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
#include "openmfc/afxole.h"

#include <windows.h>
#include <new>

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
// CMDIChildWnd::ActivateFrame / OnMDIActivate / OnSize
// (frame/Thunks.cpp, frame/CMDIChildWnd.cpp)
extern "C" void MS_ABI impl__ActivateFrame_CMDIChildWnd__UEAAXH_Z(
    CMDIChildWnd* pThis, int nCmdShow);
extern "C" void MS_ABI impl__OnMDIActivate_CMDIChildWnd__IEAAXHPEAVCWnd__0_Z(
    CMDIChildWnd* pThis, int bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
extern "C" void MS_ABI impl__OnSize_CMDIChildWnd__IEAAXIHH_Z(
    CMDIChildWnd* pThis, unsigned int nType, int cx, int cy);
// CWnd helpers (window/Thunks.cpp, window/CWnd.cpp)
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__GetDescendantWindow_CWnd__SAPEAV1_PEAUHWND____HH_Z(
    HWND hWnd, int nID, int bOnlyPerm);
extern "C" void MS_ABI
impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, CString* rString);
// CObject::IsKindOf (runtime/CObject.cpp)
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
// CFrameWnd::GetActiveDocument (frame/Thunks.cpp), CDocument::GetPathName
// (detail/DocviewSupport.cpp)
extern "C" CDocument* MS_ABI impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ(
    CFrameWnd* pThis);
extern "C" const wchar_t* MS_ABI impl__GetPathName_CDocument__UEBAPEB_WXZ(
    const CDocument* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(
    const CWnd* pThis);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);
extern "C" int MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(
    CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);
// RUNTIME_CLASS descriptors (frame/RuntimeClasses.cpp, frame/CMDIFrameWndEx.cpp,
// ole/RuntimeClasses.cpp)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleServerDoc__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFormView__SAPEAUCRuntimeClass__XZ();
// AFX_WM_AFTER_TASKBAR_ACTIVATE -- registered message id (runtime/Globals.cpp)
extern "C" unsigned int impl__AFX_WM_AFTER_TASKBAR_ACTIVATE__3IA;
// CMDIFrameWndEx::m_bDisableSetRedraw -- exported static BOOL (frame/StaticData.cpp).
// NOTE: retail mfc140u initialises this static to 1; OpenMFC currently
// initialises it to 0 (see the headerRequest that accompanies this file).
extern "C" int impl__m_bDisableSetRedraw_CMDIFrameWndEx__2HA;

// ---- forward declarations of exports defined further down in THIS file ----
extern "C" void MS_ABI impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(
    CMDIChildWndEx* pThis, int bCheckRegisteredTab);
extern "C" void MS_ABI impl__RegisterTaskbarTab_CMDIChildWndEx__UEAAXPEAV1__Z(
    CMDIChildWndEx* pThis, CMDIChildWndEx* pTabWnd);
extern "C" int MS_ABI impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(
    CMDIChildWndEx* pThis);
extern "C" int MS_ABI impl__InvalidateIconicBitmaps_CMDIChildWndEx__QEAAHXZ(
    CMDIChildWndEx* pThis);
extern "C" void MS_ABI impl__AdjustDockingLayout_CMDIChildWndEx__UEAAXPEAX_Z(
    CMDIChildWndEx* pThis, void* pLayout);
extern "C" void MS_ABI impl__AdjustClientArea_CMDIChildWndEx__IEAAXXZ(
    CMDIChildWndEx* pThis);
extern "C" void MS_ABI impl__RecalcLayout_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int bNotify);
extern "C" void MS_ABI impl__ActivateFrame_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int nCmdShow);

namespace {
// Models retail's process-wide re-entrancy latch for
// CMDIChildWndEx::OnMDIActivate (mfc140u data RVA 0x3c3e54, an int that the
// handler sets to 1 around the base dispatch and clears afterwards).  Retail's
// is a plain global with no synchronisation; this mirrors that.
bool g_bInMDIActivate = false;
}  // namespace

//=============================================================================
// CMDIChildWndEx — message handlers
//=============================================================================



















//=============================================================================
// CMDIFrameWndEx — docking / MDI-tab operations
//=============================================================================












// Symbol: ?CreateObject@CMDIChildWndEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMDIChildWndEx__SAPEAVCObject__XZ() {
    return new CMDIChildWndEx();
}
// Symbol: ?GetThisClass@CMDIChildWndEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ() {
    return &CMDIChildWndEx::classCMDIChildWndEx;
}
// Symbol: ?GetRuntimeClass@CMDIChildWndEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIChildWndEx__UEBAPEAUCRuntimeClass__XZ(CMDIChildWndEx* pThis) {
    return pThis ? &CMDIChildWndEx::classCMDIChildWndEx : nullptr;
}
// Symbol: ?EnableDocking@CMDIChildWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_CMDIChildWndEx__QEAAHK_Z(CMDIChildWndEx* pThis, DWORD dwDockStyle) {
    if (!pThis) {
        return FALSE;
    }
    pThis->CFrameWnd::EnableDocking(dwDockStyle);
    return TRUE;
}
// CMDIChildWndEx::RegisterTaskbarTab(CMDIChildWndEx*) -- retail mfc140u.dll
// RVA 0x83ca0.  Retail returns immediately unless IsTaskbarTabsSupportEnabled()
// (0x84000) is TRUE and the embedded tab-proxy CWnd at this+0x720 has no HWND
// yet; it then caches `this` at this+0x808, builds the off-screen rect
// {-32000,-32000,-31990,-31990}, creates the proxy window through its vtable
// slot 0xc0 (CreateEx, dwExStyle 0x08000080, style 0x80cf0000, caption =
// GetWindowText()), and drives afxGlobalData (mfc140u 0x3c1620)
// ->GetITaskbarList3() through RegisterTab (interface slot 0x58) and
// SetTabOrder (slot 0x68).  None of that is reachable here: OpenMFC's
// CMDIChildWndEx is a 64-byte padding blob with no proxy window at +0x720, no
// self-pointer slot at +0x808 and no ITaskbarList3 in AFX_GLOBAL_DATA.
// STUB -- left a no-op deliberately.
// Symbol: ?RegisterTaskbarTab@CMDIChildWndEx@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__RegisterTaskbarTab_CMDIChildWndEx__UEAAXPEAV1__Z(CMDIChildWndEx* pThis, CMDIChildWndEx* pTabWnd) {
    (void)pThis;
    (void)pTabWnd;
}
// CMDIChildWndEx::UnregisterTaskbarTab(BOOL) -- retail mfc140u.dll RVA 0x855d0.
// Retail returns immediately when the tab-proxy CWnd at this+0x720 has no HWND;
// otherwise it calls ITaskbarList3::UnregisterTab(proxyHwnd), destroys the proxy
// window (its vtable slot 0xd0, DestroyWindow), and -- when bCheckRegisteredTab
// is TRUE and the top-level frame is a CMDIFrameWndEx whose
// GetRegisteredWithTaskBarMDIChildCount() has dropped to 0 -- calls
// ITaskbarList3::SetTabActive(NULL, frameHwnd, 0).  The proxy window and the
// ITaskbarList3 pointer are not modeled by OpenMFC, so with no proxy HWND the
// faithful behaviour is the immediate return.
// STUB -- left a no-op deliberately.
// Symbol: ?UnregisterTaskbarTab@CMDIChildWndEx@@QEAAXH@Z
extern "C" void MS_ABI impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(CMDIChildWndEx* pThis, int bCheckRegisteredTab) {
    (void)pThis;
    (void)bCheckRegisteredTab;
}
// CMDIChildWndEx::SetTaskbarTabActive() -- retail mfc140u.dll RVA 0x843c0.
// Retail: if (!IsTaskbarTabsSupportEnabled()) return; then
// afxGlobalData (mfc140u 0x3c1620)->GetITaskbarList3()->SetTabActive
// (proxyHwnd at this+0x720 +0x40,
//  GetTopLevelFrame()-as-CMDIFrameWndEx ? that frame's HWND : NULL, 0).
// Neither the proxy window at this+0x720 nor an ITaskbarList3 exists in OpenMFC,
// and IsTaskbarTabsSupportEnabled() is itself a stub returning FALSE, so the
// faithful result is the early return.
// STUB -- left a no-op deliberately.
// Symbol: ?SetTaskbarTabActive@CMDIChildWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__SetTaskbarTabActive_CMDIChildWndEx__QEAAXXZ(CMDIChildWndEx* pThis) {
    (void)pThis;
}
// CMDIChildWndEx::SetTaskbarTabOrder(CMDIChildWndEx*) -- retail mfc140u.dll
// RVA 0x84220.  Retail: if (!IsTaskbarTabsSupportEnabled()) return; if this
// frame has no tab-proxy HWND (this+0x720 +0x40) return; then
// afxGlobalData (mfc140u 0x3c1620)->GetITaskbarList3()->SetTabOrder
// (my proxy HWND, pWndInsertBefore ? its proxy HWND : NULL).
// The proxy window and ITaskbarList3 are not modeled; the reachable path is the
// early return.  STUB -- left a no-op deliberately.
// Symbol: ?SetTaskbarTabOrder@CMDIChildWndEx@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__SetTaskbarTabOrder_CMDIChildWndEx__QEAAXPEAV1__Z(CMDIChildWndEx* pThis, CMDIChildWndEx* pWndInsertBefore) {
    (void)pThis;
    (void)pWndInsertBefore;
}
// CMDIChildWndEx::SetTaskbarTabProperties(DWORD) -- retail mfc140u.dll
// RVA 0x842c0.  Retail: if (!IsTaskbarTabsSupportEnabled()) return; if the
// tab-proxy HWND (this+0x720 +0x40) is NULL return; then it QueryInterface()s
// afxGlobalData (mfc140u 0x3c1620)->GetITaskbarList3() for the IID stored at
// mfc140u 0x33fbf8, calls the returned interface's slot-0xa8 method with
// (proxyHwnd, dwFlags) and then Releases it (slot 0x10).  Slot 0xa8 past
// ITaskbarList3 is ITaskbarList4::SetTabProperties; the slot numbers used by the
// sibling taskbar-tab exports (0x58 RegisterTab, 0x60 UnregisterTab,
// 0x68 SetTabOrder, 0x70 SetTabActive) line up with ITaskbarList3 exactly.
// No proxy window and no ITaskbarList3/4 in OpenMFC; the reachable path is the
// early return.  STUB -- left a no-op deliberately.
// Symbol: ?SetTaskbarTabProperties@CMDIChildWndEx@@QEAAXK@Z
extern "C" void MS_ABI impl__SetTaskbarTabProperties_CMDIChildWndEx__QEAAXK_Z(CMDIChildWndEx* pThis, DWORD dwFlags) {
    (void)pThis;
    (void)dwFlags;
}
// Symbol: ?SetTaskbarTabText@CMDIChildWndEx@@MEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetTaskbarTabText_CMDIChildWndEx__MEAAXPEB_W_Z(CMDIChildWndEx* pThis, const wchar_t* lpszTabText) {
    if (!pThis) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    g_taskbarTabText[pThis] = lpszTabText ? lpszTabText : L"";
}
// CMDIChildWndEx::SetTaskbarThumbnailClipRect(CRect) -- retail mfc140u.dll
// RVA 0x868e0.  Retail returns 0 unless every one of these holds: the
// afxGlobalData (mfc140u 0x3c1620) flag at +0x258 is set, GetTopLevelFrame() is a
// CMDIFrameWndEx, that frame's MDIGetActive() == this, and
// ITaskbarList3::SetThumbnailClip(frameHwnd, clipRect) returns S_OK -- only then
// does it return 1.  A non-empty rect is first converted to client coordinates
// with CWnd::ScreenToClient and is passed as NULL when ::IsRectEmpty() is true.
// None of the taskbar plumbing exists in OpenMFC, so the reachable retail result
// is FALSE.  The previous body returned TRUE, which claimed a clip was applied.
// STUB -- only the constant retail return of the reachable path is transcribed.
// Symbol: ?SetTaskbarThumbnailClipRect@CMDIChildWndEx@@MEAAHVCRect@@@Z
extern "C" int MS_ABI impl__SetTaskbarThumbnailClipRect_CMDIChildWndEx__MEAAHVCRect___Z(CMDIChildWndEx* pThis, const CRect& rectClip) {
    (void)pThis;
    (void)rectClip;
    return FALSE;
}
// CMDIChildWndEx::CanShowOnMDITabs() -- transcribed from retail mfc140u.dll
// RVA 0x86b00:
//     if (GetStyle() & WS_VISIBLE) return TRUE;              // bt $0x1c
//     CDocument* pDoc = GetActiveDocument();                 // vtable +0x2e8
//     if (pDoc == NULL) return FALSE;
//     if (!pDoc->IsKindOf(RUNTIME_CLASS(COleServerDoc))) return FALSE;
//     return pDoc->[+0x124] != 0;
// CDocument+0x124 is m_bEmbedded: ?SetPathName@CDocument@@UEAAXPEBDH@Z (entry
// RVA 0x2194c0 in the ANSI twin mfc140.dll) stores the path string at this+0x48
// and clears this+0x124 in the same breath, which is MFC's documented
// `m_strPathName = ...; m_bEmbedded = FALSE;` pair.  OpenMFC keeps m_bEmbedded
// on COleServerDoc (afxole.h) rather than on CDocument, so the downcast above
// reaches the same flag.
// Symbol: ?CanShowOnMDITabs@CMDIChildWndEx@@UEAAHXZ
extern "C" int MS_ABI impl__CanShowOnMDITabs_CMDIChildWndEx__UEAAHXZ(CMDIChildWndEx* pThis) {
    if (!pThis) {
        return FALSE;
    }
    if (impl__GetStyle_CWnd__QEBAKXZ(pThis) & WS_VISIBLE) {
        return TRUE;
    }
    CDocument* pDoc = impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ(pThis);
    if (!pDoc) {
        return FALSE;
    }
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pDoc, impl__GetThisClass_COleServerDoc__SAPEAUCRuntimeClass__XZ())) {
        return FALSE;
    }
    return static_cast<COleServerDoc*>(pDoc)->IsEmbedded() ? TRUE : FALSE;
}
// CMDIChildWndEx::OnSetText(const wchar_t*) — retail (RVA 0x84720) calls
// CWnd::Default() and propagates its result, then forwards the text to the
// parent MDI frame's client area (this+0x718/+0x720 UpdateTabs), runs
// CFrameImpl::OnSetText (this+0x218) and a vtable-0x458 virtual. The base
// result is preserved; the unmodeled sub-object updates are skipped.
// Symbol: ?OnSetText@CMDIChildWndEx@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CMDIChildWndEx__IEAAHPEB_W_Z(
    CMDIChildWndEx* pThis, const wchar_t* lpszText) {
    if (!pThis) return 0;
    int nResult = static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
    // TODO(clean-room): transcribed partially — retail also updates the MDI
    // frame client area (this+0x718 +0x720), CFrameImpl (this+0x218) and a
    // vtable-0x458 virtual with lpszText; unmodeled.
    return nResult;
}
// CMDIChildWndEx::OnSize(UINT, int, int) — transcribed from retail mfc140u.dll
// RVA 0x84c60.  Retail:
//     if (this->[0x1f0] != 0) return;                    // suspend-layout flag
//     InvalidateIconicBitmaps();                          // 0x840b0
//     this->[0x1f8] = (nType == SIZE_MINIMIZED);
//     if (pVM->vtbl[0x3f8]()) {   // pVM = the inlined CMFCVisualManager
//                                 // singleton accessor at mfc140.dll 0x97f4,
//                                 // which lazily news 0x108 bytes and runs
//                                 // ??0CMFCVisualManager@@QEAA@H@Z (0x182640)
//         if (this->[0x718] && ((char*)this->[0x718])[0x4668] == 0)
//              CFrameImpl(this+0x218).OnWindowPosChanging(&wp);   // 0x652b0
//         else if (this->[0x23c]) { this->[0x23c] = 0;
//              ::SetWindowRgn(m_hWnd, NULL, TRUE); }
//     }
//     if (this->[0x1f8] == 0 && nType != SIZE_MAXIMIZED && this->[0x1f4] == 0) {
//         this->[0x3b8] = 1;
//         CMDIChildWnd::OnSize(nType, cx, cy);            // 0x2a72b0
//         vtbl[0x3a8](NULL);                              // AdjustDockingLayout
//         this->[0x3b8] = 0;
//     } else {
//         CMDIChildWnd::OnSize(nType, cx, cy);
//         if (nType == SIZE_MAXIMIZED ||
//             (nType == SIZE_RESTORED && this->[0x1f4] != 0)) {
//             vtbl[0x300](TRUE);                          // RecalcLayout(TRUE)
//             if (this->[0x120] && nType == SIZE_RESTORED) { ...
//                  ModifyStyle(0, 0x00cf0000, 0); }
//         }
//         this->[0x1f4] = (nType == SIZE_MAXIMIZED);
//     }
//     CFrameImpl(this+0x218).<0x65ee0>();
// OpenMFC models none of this+0x120/0x1f0/0x1f4/0x1f8/0x23c/0x3b8/0x718 nor the
// CFrameImpl at +0x218 (CMDIChildWndEx is a 64-byte padding blob), so those
// flags are taken as zero here.  With this->[0x1f4] (the "was maximized"
// latch) read as zero the SIZE_RESTORED arm of the second branch is
// unreachable, which is the one deviation worth naming: retail relayouts on the
// restore that follows a maximize, this build does not.  RecalcLayout and
// AdjustDockingLayout are dispatched virtually by retail (slots 0x300/0x3a8);
// here they go straight to this class's own exports, so an override in a
// further-derived class is not honoured.
// Symbol: ?OnSize@CMDIChildWndEx@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMDIChildWndEx__IEAAXIHH_Z(
    CMDIChildWndEx* pThis, unsigned int nType, int cx, int cy) {
    if (!pThis) {
        return;
    }
    impl__InvalidateIconicBitmaps_CMDIChildWndEx__QEAAHXZ(pThis);
    impl__OnSize_CMDIChildWnd__IEAAXIHH_Z(pThis, nType, cx, cy);
    if (nType != SIZE_MINIMIZED && nType != SIZE_MAXIMIZED) {
        impl__AdjustDockingLayout_CMDIChildWndEx__UEAAXPEAX_Z(pThis, nullptr);
    } else if (nType == SIZE_MAXIMIZED) {
        impl__RecalcLayout_CMDIChildWndEx__UEAAXH_Z(pThis, TRUE);
    }
}
// CMDIChildWndEx::OnSizing(UINT, LPRECT) — retail (RVA 0x85520) calls
// CWnd::Default(), compares the current window rect against the proposed rect
// and calls RecalcLayout(FALSE) when the sizes differ. The comparison is
// faithful (m_hWnd at 0x40 is modeled); the virtual RecalcLayout dispatch is
// routed to the base layout instead of the derived vtable slot.
// Symbol: ?OnSizing@CMDIChildWndEx@@IEAAXIPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnSizing_CMDIChildWndEx__IEAAXIPEAUtagRECT___Z(
    CMDIChildWndEx* pThis, unsigned int fwSide, RECT* pRect) {
    (void)fwSide;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    if (pThis->GetSafeHwnd() && pRect) {
        RECT rectWnd;
        ::GetWindowRect(pThis->GetSafeHwnd(), &rectWnd);
        const int nWndW = rectWnd.right - rectWnd.left;
        const int nWndH = rectWnd.bottom - rectWnd.top;
        const int nRectW = pRect->right - pRect->left;
        const int nRectH = pRect->bottom - pRect->top;
        if (nWndW != nRectW || nWndH != nRectH) {
            // TODO(clean-room): retail dispatches RecalcLayout(FALSE) through
            // the derived vtable; routed to the base layout conservatively.
            pThis->CFrameWnd::RecalcLayout(FALSE);
        }
    }
}
// CMDIChildWndEx::OnStyleChanged(int, LPSTYLESTRUCT) — transcribed from retail
// mfc140u.dll RVA 0x859c0.  Retail, in order:
//     CWnd::Default();                                        // 0x28ac80
//     if (IsTaskbarTabsSupportEnabled())  vtbl[0x3c0](NULL);   // RegisterTaskbarTab
//     else                                UnregisterTaskbarTab(TRUE);  // 0x855d0
//     if ((lpss->styleOld & WS_SYSMENU) == (lpss->styleNew & WS_SYSMENU)) return;
//     ... MDI-tabbed-group work driven by this+0x718 / this+0x1fc..0x208 /
//         this+0x23c, ModifyStyleEx, ::GetClientRect on the group window at
//         (this->[0x718])+0x760 and a final CWnd::SetWindowPos.
// The style-change tail needs this+0x718 (the CMDIFrameWndEx tabbed group) and
// the cached client rect at this+0x1fc..0x208, none of which OpenMFC models, so
// only the head is reproduced.  Retail dispatches RegisterTaskbarTab through
// vtable slot 0x3c0 (confirmed against the retail CMDIChildWndEx vtable at
// mfc140.dll 0x2ebf68); this build calls the class's own export directly, so an
// override in a further-derived class is not honoured.
// Symbol: ?OnStyleChanged@CMDIChildWndEx@@IEAAXHPEAUtagSTYLESTRUCT@@@Z
extern "C" void MS_ABI impl__OnStyleChanged_CMDIChildWndEx__IEAAXHPEAUtagSTYLESTRUCT___Z(
    CMDIChildWndEx* pThis, int nStyleType, void* lpStyleStruct) {
    (void)nStyleType;
    (void)lpStyleStruct;
    if (!pThis) {
        return;
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
    if (impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(pThis)) {
        impl__RegisterTaskbarTab_CMDIChildWndEx__UEAAXPEAV1__Z(pThis, nullptr);
    } else {
        impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(pThis, TRUE);
    }
    // TODO(clean-room): the WS_SYSMENU-change tail needs the tabbed-group
    // pointer at this+0x718 and the cached client rect at this+0x1fc..0x208.
}
// CMDIChildWndEx::OnSysCommand(UINT, LPARAM) — retail (RVA 0x85e20): for
// SC_CLOSE (0xf100) it suppresses the base close when a tabbed control bar is
// attached (this+0x718) and non-auto-hidden; otherwise it tail-calls
// CFrameWnd::OnSysCommand. The suppression test is unmodeled, so the base
// dispatch is always performed.
// Symbol: ?OnSysCommand@CMDIChildWndEx@@IEAAXI_J@Z
extern "C" void MS_ABI impl__OnSysCommand_CMDIChildWndEx__IEAAXI_J_Z(
    CMDIChildWndEx* pThis, unsigned int nID, __int64 lParam) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — retail skips the close for
    // SC_CLOSE when the unmodeled this+0x718 tabbed-bar slot is active.
    impl__OnSysCommand_CFrameWnd__IEAAXI_J_Z(pThis, nID, lParam);
}
// CMDIChildWndEx::OnTaskbarTabThumbnailActivate(UINT, CWnd*, BOOL) — retail
// (RVA 0x866f0): when nState == 1 it activates the top-level frame through
// vtable slot 0x450 (ActivateTopLevelFrame). The vtable slot is not modeled in
// the simplified headers. Conservative no-op.
// Symbol: ?OnTaskbarTabThumbnailActivate@CMDIChildWndEx@@UEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnTaskbarTabThumbnailActivate_CMDIChildWndEx__UEAAXIPEAVCWnd__H_Z(
    CMDIChildWndEx* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized) {
    (void)pWndOther;
    (void)bMinimized;
    if (!pThis) return;
    if (nState == 1) {
        // TODO(clean-room): retail calls ActivateTopLevelFrame (vtable 0x450);
        // not modeled in the simplified headers.
    }
}
// CMDIChildWndEx::OnTaskbarTabThumbnailMouseActivate(CWnd*, UINT, UINT) —
// retail (RVA 0x86710) returns TRUE unconditionally; on WM_LBUTTONUP (0x202)
// it also activates the top-level frame (vtable 0x450). The constant result is
// kept; the vtable dispatch is unmodeled.
// Symbol: ?OnTaskbarTabThumbnailMouseActivate@CMDIChildWndEx@@UEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnTaskbarTabThumbnailMouseActivate_CMDIChildWndEx__UEAAHPEAVCWnd__II_Z(
    CMDIChildWndEx* pThis, CWnd* pDesktopWnd, unsigned int nHitTest, unsigned int message) {
    (void)pDesktopWnd;
    (void)nHitTest;
    (void)message;
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — retail also activates the
    // top-level frame (vtable 0x450) on WM_LBUTTONUP.
    return TRUE;
}
// CMDIChildWndEx::OnTaskbarTabThumbnailStretch(HBITMAP, const CRect&, HBITMAP,
// const CRect&) — retail mfc140u.dll RVA 0x86270.  Retail returns FALSE at once
// when either bitmap handle is NULL (its early-out at mfc140.dll 0x86993 is a
// plain `xor eax,eax`); otherwise it constructs an unexported bitmap-wrapper
// object (ctor at mfc140.dll 0x88040, init at 0x87dd4) that inspects the source
// HBITMAP with ::GetObject(hbm, sizeof(DIBSECTION)=0x68, &ds) under a critical
// section, builds a CClientDC over this frame (??0CClientDC@@QEAA@PEAVCWnd@@@Z,
// mfc140.dll 0x2a1a60) and runs the stretch/blend through that helper.  None of
// those helpers exist here, so the FALSE return is kept for every input.
// STUB -- only the retail null-handle result is transcribed.
// Symbol: ?OnTaskbarTabThumbnailStretch@CMDIChildWndEx@@UEAAHPEAUHBITMAP__@@AEBVCRect@@01@Z
extern "C" int MS_ABI impl__OnTaskbarTabThumbnailStretch_CMDIChildWndEx__UEAAHPEAUHBITMAP____AEBVCRect__01_Z(
    CMDIChildWndEx* pThis, HBITMAP hBmpDst, const CRect& rectDst, HBITMAP hBmpSrc, const CRect& rectSrc) {
    (void)pThis;
    (void)hBmpDst;
    (void)rectDst;
    (void)hBmpSrc;
    (void)rectSrc;
    // TODO(clean-room): transcribed partially — retail GDI/D2D stretch pipeline
    // (CreateBitmap / ColorMatrix / AlphaBlend) is not modeled.
    return FALSE;
}
// CMDIChildWndEx::OnUpdateFrameTitle(BOOL) — retail (RVA 0x84ad0) consults the
// CWinApp frame state, this+0x248 flag, GetStyle() bit 0x10000 and the
// taskbar-tab text before WM_NCACTIVATE / MDI-client UpdateTabs. The base
// CMDIChildWnd::OnUpdateFrameTitle carries the documented contract; extras are
// skipped.
// Symbol: ?OnUpdateFrameTitle@CMDIChildWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int bAddToTitle) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — retail also handles
    // WM_NCACTIVATE, MDI-client UpdateTabs and taskbar-tab text via unmodeled
    // state.
    impl__OnUpdateFrameTitle_CMDIChildWnd__MEAAXH_Z(pThis, bAddToTitle);
}
// CMDIChildWndEx::OnWindowPosChanged(WINDOWPOS*) — retail (RVA 0x85ec0): when
// lpWndPos->flags has SWP_FRAMECHANGED (0x20) it updates the tabbed-bar layout
// (this+0x218 / this+0x718) and clears the autohide flag at this+0x23c, then
// unconditionally tail-calls CWnd::Default(). The base dispatch is kept.
// Symbol: ?OnWindowPosChanged@CMDIChildWndEx@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMDIChildWndEx__IEAAXPEAUtagWINDOWPOS___Z(
    CMDIChildWndEx* pThis, WINDOWPOS* lpWndPos) {
    if (!pThis) return;
    if (lpWndPos && (lpWndPos->flags & SWP_FRAMECHANGED)) {
        // TODO(clean-room): retail updates the tabbed-bar layout (this+0x218 /
        // this+0x718) and clears the autohide flag at this+0x23c here.
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}
// CMDIChildWndEx::PaneFromPoint(CPoint, int, DWORD&, CRuntimeClass*) const —
// retail (RVA 0x850c0) delegates to the embedded CDockingManager at this+0x3a8.
// The dock manager is not modeled in the simplified headers. Conservative NULL.
// Symbol: ?PaneFromPoint@CMDIChildWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CMDIChildWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(
    const CMDIChildWndEx* pThis, unsigned long long pt, int nSensitivity,
    unsigned long* dwAlignment, CRuntimeClass* pRTCBarType) {
    (void)pt;
    (void)nSensitivity;
    (void)pRTCBarType;
    if (!pThis) return nullptr;
    // TODO(clean-room): retail forwards to the embedded CDockingManager
    // (this+0x3a8) PaneFromPoint; not modeled.
    if (dwAlignment) *dwAlignment = 0;
    return nullptr;
}
// CMDIChildWndEx::PaneFromPoint(CPoint, int, bool, CRuntimeClass*) const —
// retail (RVA 0x85080) delegates to the embedded CDockingManager at this+0x3a8.
// The dock manager is not modeled. Conservative NULL.
// Symbol: ?PaneFromPoint@CMDIChildWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CMDIChildWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(
    const CMDIChildWndEx* pThis, unsigned long long pt, int nSensitivity,
    bool bExactBar, CRuntimeClass* pRTCBarType) {
    (void)pt;
    (void)nSensitivity;
    (void)bExactBar;
    (void)pRTCBarType;
    if (!pThis) return nullptr;
    // TODO(clean-room): retail forwards to the embedded CDockingManager
    // (this+0x3a8) PaneFromPoint; not modeled.
    return nullptr;
}
// CMDIChildWndEx::PreCreateWindow(CREATESTRUCTW&) — retail (RVA 0x84e80) caches
// `this` at this+0x558 and tail-calls CMDIChildWnd::PreCreateWindow. The
// self-pointer slot is unmodeled; the base precreate is performed.
// Symbol: ?PreCreateWindow@CMDIChildWndEx@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMDIChildWndEx__MEAAHAEAUtagCREATESTRUCTW___Z(
    CMDIChildWndEx* pThis, CREATESTRUCTW& cs) {
    if (!pThis) return FALSE;
    // TODO(clean-room): retail stores `this` at this+0x558 (unmodeled) before
    // the base call.
    return impl__PreCreateWindow_CMDIChildWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
}
// CMDIChildWndEx::PreTranslateMessage(MSG*) — retail (RVA 0x83990) has keyboard
// fast paths (0x100..0x109 suppression when a tabbed bar is present, and a
// WM_KEYDOWN+VK_ESCAPE close path through unmodeled this+0x718/0x1e8 and a
// global flag) before falling back to CMDIChildWnd::PreTranslateMessage. The
// base dispatch is kept.
// Symbol: ?PreTranslateMessage@CMDIChildWndEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIChildWndEx__UEAAHPEAUtagMSG___Z(
    CMDIChildWndEx* pThis, MSG* pMsg) {
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — retail keyboard fast paths
    // rely on unmodeled this+0x718 / this+0x1e8 state and a global flag.
    return impl__PreTranslateMessage_CMDIChildWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}
// CMDIChildWndEx::RecalcLayout(BOOL) — retail (RVA 0x852b0) drives the embedded
// CDockingManager (this+0x3a8), the MDI client (this+0x558) and the internal
// AdjustClientArea (0x180085140) behind reentrancy flags (this+0x1a0/0x1f8).
// The base frame relayout is performed; the unmodeled dock/client state is
// skipped.
// Symbol: ?RecalcLayout@CMDIChildWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int bNotify) {
    if (!pThis) return;
    pThis->CFrameWnd::RecalcLayout(bNotify);
    // TODO(clean-room): transcribed partially — retail also runs the embedded
    // CDockingManager (this+0x3a8) layout and AdjustClientArea behind the
    // reentrancy flags at this+0x1a0/0x1f8.
}
// CMDIChildWndEx::RemovePaneFromDockManager(CBasePane*, BOOL, BOOL, BOOL,
// CBasePane*) — retail (RVA 0x84ec0) tail-jumps to the embedded CDockingManager
// (this+0x3a8) RemovePaneFromDockManager. Not modeled. Conservative no-op.
// Symbol: ?RemovePaneFromDockManager@CMDIChildWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CMDIChildWndEx__QEAAXPEAVCBasePane__HHH0_Z(
    CMDIChildWndEx* pThis, CBasePane* pControlBar, int bDestroy,
    int bAdjustLayout, int bAutoHide, CBasePane* pBarReplacement) {
    (void)pControlBar;
    (void)bDestroy;
    (void)bAdjustLayout;
    (void)bAutoHide;
    (void)pBarReplacement;
    if (!pThis) return;
    // TODO(clean-room): retail tail-jumps to the embedded CDockingManager
    // (this+0x3a8) RemovePaneFromDockManager; not modeled.
}
// CMDIChildWndEx::ShowPane(CBasePane*, BOOL, BOOL, BOOL) — retail (RVA 0x68050)
// dispatches a CBasePane virtual (vtable 0x458) to show/hide the pane. The
// retail pane vtable is not modeled. Conservative no-op.
// Symbol: ?ShowPane@CMDIChildWndEx@@QEAAXPEAVCBasePane@@HHH@Z
extern "C" void MS_ABI impl__ShowPane_CMDIChildWndEx__QEAAXPEAVCBasePane__HHH_Z(
    CMDIChildWndEx* pThis, CBasePane* pBar, int bShow, int bDelay, int bActivate) {
    (void)pBar;
    (void)bShow;
    (void)bDelay;
    (void)bActivate;
    if (!pThis) return;
    // TODO(clean-room): retail dispatches a CBasePane virtual (vtable 0x458);
    // the retail pane vtable is not modeled.
}
// CMDIChildWndEx::UpdateTaskbarTabIcon(HICON) — retail (RVA 0x841e0) posts
// WM_SETICON (0x80) to the taskbar-tab proxy window (this+0x720/+0x760). The
// proxy window is not modeled. Conservative no-op.
// Symbol: ?UpdateTaskbarTabIcon@CMDIChildWndEx@@UEAAXPEAUHICON__@@@Z
extern "C" void MS_ABI impl__UpdateTaskbarTabIcon_CMDIChildWndEx__UEAAXPEAUHICON_____Z(
    CMDIChildWndEx* pThis, HICON hIcon) {
    (void)hIcon;
    if (!pThis) return;
    // TODO(clean-room): retail sends WM_SETICON to the tab-proxy window
    // (this+0x720/+0x760); not modeled in the simplified headers.
}
CMDIChildWndEx::CMDIChildWndEx() {
    memset(_mdichildwndex_padding, 0, sizeof(_mdichildwndex_padding));
}
CMDIChildWndEx::~CMDIChildWndEx() {}
// CMDIChildWndEx::ActivateFrame(int) — transcribed from retail mfc140u.dll
// RVA 0x84650:
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//     if (CMDIFrameWndEx::m_bDisableSetRedraw == 0 &&
//         pParent != NULL && pParent->m_hWnd != NULL) {
//         ::SendMessage(pParent->m_hWnd, WM_SETREDRAW, FALSE, 0);
//         CMDIChildWnd::ActivateFrame(nCmdShow);
//         ::SendMessage(pParent->m_hWnd, WM_SETREDRAW, TRUE, 0);
//         ::RedrawWindow(pParent->m_hWnd, NULL, NULL, 0x185);
//     } else {
//         int nCmd = (GetStyle() & WS_SYSMENU) ? nCmdShow : SW_SHOWMAXIMIZED;
//         if (this->[0x718] && ((char*)this->[0x718])[0x4668] != 0)
//             nCmd = SW_SHOWNORMAL;
//         CMDIChildWnd::ActivateFrame(nCmd);
//     }
// 0x185 == RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_NOCHILDREN
// (0x1|0x4|0x80|0x100); the style test is `bt $0x13` on GetStyle()'s result,
// i.e. bit 19 == WS_SYSMENU (0x00080000).  The gate is the exported static
// ?m_bDisableSetRedraw@CMDIFrameWndEx@@2HA (mfc140u data RVA 0x3b1afc, which
// sits directly after m_dwExcludeStyle/m_dwDefaultTaskbarTabPropertyFlags in
// both twins); retail initialises it to 1, so the SetRedraw-bracketed arm is
// NOT the default path in a real mfc140u.  OpenMFC's copy in
// core/frame/StaticData.cpp is currently initialised to 0 -- see the
// headerRequest filed with this file.  The MDI-tabbed-group pointer at
// this+0x718 is not modeled (CMDIChildWndEx is a 64-byte padding blob), so the
// SW_SHOWNORMAL override is skipped; everything else is transcribed.
// Symbol: ?ActivateFrame@CMDIChildWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__ActivateFrame_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int nCmdShow) {
    if (!pThis) {
        return;
    }
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
        ::GetParent(pThis->m_hWnd));
    if (impl__m_bDisableSetRedraw_CMDIFrameWndEx__2HA == 0 &&
        pParent != nullptr && pParent->m_hWnd != nullptr) {
        ::SendMessage(pParent->m_hWnd, WM_SETREDRAW, FALSE, 0);
        impl__ActivateFrame_CMDIChildWnd__UEAAXH_Z(pThis, nCmdShow);
        ::SendMessage(pParent->m_hWnd, WM_SETREDRAW, TRUE, 0);
        ::RedrawWindow(pParent->m_hWnd, nullptr, nullptr,
                       RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_NOCHILDREN);
    } else {
        const int nCmd =
            (impl__GetStyle_CWnd__QEBAKXZ(pThis) & WS_SYSMENU) ? nCmdShow
                                                               : SW_SHOWMAXIMIZED;
        // TODO(clean-room): retail forces SW_SHOWNORMAL when the MDI tabbed
        // group at this+0x718 reports [0x4668] != 0; that slot is unmodeled.
        impl__ActivateFrame_CMDIChildWnd__UEAAXH_Z(pThis, nCmd);
    }
}

// CMDIChildWndEx::ActivateTopLevelFrame() — transcribed from retail mfc140u.dll
// RVA 0x86a10, whose whole body is:
//     CFrameWnd* pFrame = GetTopLevelFrame();                  // 0x28e490
//     if (pFrame == NULL) return;
//     if (!pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) return;
//     this->vtbl[0x308](-1);                                   // ActivateFrame(-1)
//     ::SetForegroundWindow(pFrame->m_hWnd);
//     BOOL bIconic = ::IsIconic(pFrame->m_hWnd);
//     pFrame->ShowWindow(bIconic ? SW_RESTORE : SW_SHOW);      // 9 : 5
//     ::PostMessageW(pFrame->m_hWnd, AFX_WM_AFTER_TASKBAR_ACTIVATE,
//                    bIconic, (LPARAM)this->m_hWnd);
//     bIconic = ::IsIconic(pFrame->m_hWnd);                    // re-read
//     pFrame->ShowWindow(bIconic ? SW_RESTORE : SW_SHOW);
// The 9/5 pair is built branchlessly as `neg; sbb edx,edx; and $4; add $5` the
// first time and as an explicit `mov $9 / mov $5` the second; the CRuntimeClass
// at mfc140u 0x2ee478 spells "CMDIFrameWndEx", and the message id is read from
// ?AFX_WM_AFTER_TASKBAR_ACTIVATE@@3IA (mfc140u data 0x3c18f4).
// Deviation: retail dispatches ActivateFrame through vtable slot 0x308, this
// build calls this class's own export directly, so an override in a
// further-derived class is not honoured.
// Symbol: ?ActivateTopLevelFrame@CMDIChildWndEx@@MEAAXXZ
extern "C" void MS_ABI impl__ActivateTopLevelFrame_CMDIChildWndEx__MEAAXXZ(
    CMDIChildWndEx* pThis) {
    if (!pThis) {
        return;
    }
    CFrameWnd* pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (pFrame == nullptr) {
        return;
    }
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pFrame, impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ())) {
        return;
    }
    impl__ActivateFrame_CMDIChildWndEx__UEAAXH_Z(pThis, -1);
    ::SetForegroundWindow(pFrame->m_hWnd);
    int bIconic = ::IsIconic(pFrame->m_hWnd) ? 1 : 0;
    impl__ShowWindow_CWnd__QEAAHH_Z(pFrame, bIconic ? SW_RESTORE : SW_SHOW);
    ::PostMessage(pFrame->m_hWnd, impl__AFX_WM_AFTER_TASKBAR_ACTIVATE__3IA,
                  static_cast<WPARAM>(bIconic),
                  reinterpret_cast<LPARAM>(pThis->m_hWnd));
    bIconic = ::IsIconic(pFrame->m_hWnd) ? 1 : 0;
    impl__ShowWindow_CWnd__QEAAHH_Z(pFrame, bIconic ? SW_RESTORE : SW_SHOW);
}

// CMDIChildWndEx::AddDockSite() — transcribed from retail mfc140u.dll
// RVA 0x27d0, whose entire body is a single `ret $0x0`.  It reads no member,
// calls nothing and returns nothing, so an empty body is the faithful
// implementation here -- not a placeholder.
// Two caveats for whoever re-checks this: (1) the export has no entry in
// mfc140_rva_symbols.json (the ANSI map disas.py reads), so only the mfc140u
// map resolves it; (2) 0x27d0 is an identical-COMDAT-folded address shared by
// 158 exports, so a disassembler that labels RVAs from a reverse map will print
// some other name (e.g. ?UpdateModifiedFlag@CRichEditDoc@@UEAAXXZ) at it.  The
// folding is itself the evidence that this body is a bare `ret`.
// Symbol: ?AddDockSite@CMDIChildWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_CMDIChildWndEx__QEAAXXZ(
    CMDIChildWndEx* pThis) {
    (void)pThis;
}

// CMDIChildWndEx::AddPane(CBasePane*, BOOL) — retail mfc140u.dll RVA 0x84e90 is
// a forwarder: `this += 0x3a8` then a (non-tail) call to
// ?AddPane@CDockingManager@@QEAAHPEAVCBasePane@@HHH@Z (mfc140u 0x47860) with
// (pControlBar, bTail, 0, 0) -- the third argument in R9D and the fourth in the
// 0x20(%rsp) home slot -- whose BOOL result is returned.  The embedded
// CDockingManager at
// this+0x3a8 is not modeled by OpenMFC (CMDIChildWndEx is a 64-byte padding
// blob), so there is nothing to forward to and no meaningful result.
// STUB -- retail's return is CDockingManager::AddPane's, not a constant.
// Symbol: ?AddPane@CMDIChildWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_CMDIChildWndEx__QEAAHPEAVCBasePane__H_Z(
    CMDIChildWndEx* pThis, CBasePane* pControlBar, int bTail) {
    (void)pThis;
    (void)pControlBar;
    (void)bTail;
    return FALSE;
}

// CMDIChildWndEx::AddTabbedPane(CDockablePane*) — retail mfc140u.dll
// RVA 0x86540.  Retail stores the pane at this+0x710, calls the pane's vtable
// slot 0x448 with (this, FALSE), then slot 0x3f0 with 0, reparents the pane's
// window to this frame with ::SetParent(pane->m_hWnd, m_hWnd),
// CWnd::ShowWindow(pane, SW_SHOW), writes 1 to pane+0xf8, calls
// CDockingManager::RemoveHiddenMDITabbedBar on (this->[0x718])+0x228 and tail
// jumps to AdjustClientArea().  The pane slot at this+0x710, the tabbed-group
// pointer at this+0x718 and the CDockablePane vtable are all unmodeled.
// STUB -- left a no-op deliberately.
// Symbol: ?AddTabbedPane@CMDIChildWndEx@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__AddTabbedPane_CMDIChildWndEx__QEAAXPEAVCDockablePane___Z(
    CMDIChildWndEx* pThis, CDockablePane* pPane) {
    (void)pThis;
    (void)pPane;
}

// CMDIChildWndEx::AdjustClientArea() — partially transcribed from retail
// mfc140u.dll RVA 0x85140, whose whole body is:
//     CWnd* pWnd = NULL;
//     if (this->[0x710] && ((CWnd*)this->[0x710])->vtbl[0x2f8]() &&
//         CWnd::FromHandle(::GetParent(this->[0x710]->m_hWnd)) == this)
//         pWnd = this->[0x710];                       // the tabbed pane
//     else
//         pWnd = CWnd::GetDlgItem(AFX_IDW_PANE_FIRST);  // 0x2a9390
//     if (pWnd == NULL) return;
//     DWORD dwAdd = (pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)) ||   // 0x332860
//                    pWnd->IsKindOf(RUNTIME_CLASS(CFormView)))        // 0x333410
//                   ? 0x04000000                       // WS_CLIPSIBLINGS
//                   : 0x06000000;                      // + WS_CLIPCHILDREN
//     pWnd->ModifyStyle(0, dwAdd, 0);                  // 0x2a96f0
//     if (this->[0x210]) return;
//     if (CDockingManager::m_bFullScreenMode) return;  // mfc140u data 0x3be204,
//                                                      // = mfc140.dll 0x3b6f64
//     pWnd->SetWindowPos(&CWnd::wndBottom,             // 0x2a9a60; the RDX
//                                                      // operand is the global
//                                                      // ?wndBottom@CWnd@@2V1@B
//                                                      // at mfc140u 0x3c3460
//         this->[0x110] + this->[0x570], this->[0x114] + this->[0x574],
//         this->[0x578] - this->[0x118] - (this->[0x110] + this->[0x570]),
//         this->[0x57c] - this->[0x11c] - (this->[0x114] + this->[0x574]),
//         SWP_NOZORDER /*0x10*/);   // the z-order arg is inert under NOZORDER
// The tabbed-pane slot at this+0x710 is not modeled by OpenMFC (CMDIChildWndEx
// is a 64-byte padding blob), so the GetDlgItem arm is the one retail takes
// here -- and it, the two IsKindOf tests and the ModifyStyle call need nothing
// this build lacks, so that whole leg is reproduced faithfully.
// Only the trailing geometry pass is skipped: it needs this+0x110..0x11c
// (the cached frame borders), this+0x210 and this+0x570..0x57c, none of which
// have slots here, and CDockingManager::m_bFullScreenMode is not modeled
// either.  The consequence is that the client pane still gets its clip styles
// but is never re-fitted to the docking borders.
// Symbol: ?AdjustClientArea@CMDIChildWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__AdjustClientArea_CMDIChildWndEx__IEAAXXZ(
    CMDIChildWndEx* pThis) {
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail prefers the tabbed pane cached at this+0x710
    // when it accepts the client area (its vtable slot 0x2f8) and this frame is
    // its parent; that slot is unmodeled, so retail's other arm is taken.
    CWnd* pWnd = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, AFX_IDW_PANE_FIRST);
    if (pWnd == nullptr) {
        return;
    }
    const unsigned long dwAdd =
        (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
             pWnd, impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ()) ||
         impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
             pWnd, impl__GetThisClass_CFormView__SAPEAUCRuntimeClass__XZ()))
            ? static_cast<unsigned long>(WS_CLIPSIBLINGS)
            : static_cast<unsigned long>(WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    impl__ModifyStyle_CWnd__QEAAHKKI_Z(pWnd, 0, dwAdd, 0);
    // TODO(clean-room): retail then re-fits the pane with CWnd::SetWindowPos
    // using this+0x110..0x11c and this+0x570..0x57c behind the this+0x210 and
    // CDockingManager::m_bFullScreenMode gates; none of those are modeled.
}

// CMDIChildWndEx::AdjustDockingLayout(HDWP) — transcribed from retail
// mfc140u.dll RVA 0x85100:
//     CDockingManager& dm = *(this + 0x3a8);
//     if (dm.[0x250] != 0) return;      // "layout in progress" latch
//     dm.vtbl[0x80]();                  // the dock manager's own relayout
//     AdjustClientArea();               // 0x85140
// The embedded CDockingManager is not modeled, so its latch reads as clear and
// its relayout call is skipped; the AdjustClientArea() leg is reproduced (see
// that function below -- it applies the client pane's clip styles but not
// retail's trailing geometry pass).
// Symbol: ?AdjustDockingLayout@CMDIChildWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CMDIChildWndEx__UEAAXPEAX_Z(
    CMDIChildWndEx* pThis, void* pLayout) {
    (void)pLayout;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail first runs the embedded CDockingManager
    // (this+0x3a8) relayout behind its this+0x3a8 +0x250 latch; unmodeled.
    impl__AdjustClientArea_CMDIChildWndEx__IEAAXXZ(pThis);
}

// CMDIChildWndEx::DockPane(CBasePane*, UINT, LPCRECT) — retail mfc140u.dll
// RVA 0x84ed0.  Retail asks the pane three virtual questions (vtable slots
// 0x3a8, 0x3b0 and 0x3e8), clears bit 0 of pane+0x108 when the first says yes
// and ?m_bEnableFloatingBars@CMDIChildWndEx@@2HA (mfc140u data 0x3be24c) is
// clear, and then routes to either CDockingManager::AddPane or
// CDockingManager::DockPane on the manager embedded at this+0x3a8.
// The dock manager and the CBasePane vtable are not modeled here.
// STUB -- left a no-op deliberately.
// Symbol: ?DockPane@CMDIChildWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_CMDIChildWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    CMDIChildWndEx* pThis, CBasePane* pBar, unsigned int dwAlignment, const RECT* lpRect) {
    (void)pThis;
    (void)pBar;
    (void)dwAlignment;
    (void)lpRect;
}

// CMDIChildWndEx::DockPaneLeftOf(CPane*, CPane*) — retail mfc140u.dll
// RVA 0x843a0 is `this += 0x3a8; CDockingManager::DockPaneLeftOf(...);
// return 1;` -- it discards the manager's result and returns TRUE
// unconditionally.  The embedded CDockingManager at this+0x3a8 is not modeled,
// so only that unconditional TRUE is reproduced; no panes are actually docked.
// STUB -- the docking work is missing, only retail's constant result is kept.
// Symbol: ?DockPaneLeftOf@CMDIChildWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_CMDIChildWndEx__QEAAHPEAVCPane__0_Z(
    CMDIChildWndEx* pThis, CPane* pBar, CPane* pLeftOf) {
    (void)pThis;
    (void)pBar;
    (void)pLeftOf;
    return TRUE;
}

// CMDIChildWndEx::EnableAutoHidePanes(DWORD) — retail mfc140u.dll RVA 0x852a0
// is a two-instruction tail jump: `this += 0x3a8` then
// CDockingManager::EnableAutoHidePanes(dwDockStyle), whose BOOL result is
// returned directly.  The embedded CDockingManager is not modeled by OpenMFC.
// STUB -- retail's result comes from the dock manager, not from a constant.
// Symbol: ?EnableAutoHidePanes@CMDIChildWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_CMDIChildWndEx__QEAAHK_Z(
    CMDIChildWndEx* pThis, unsigned long dwDockStyle) {
    (void)pThis;
    (void)dwDockStyle;
    return FALSE;
}

// CMDIChildWndEx::EnableTaskbarThumbnailClipRect(BOOL) — retail mfc140u.dll
// RVA 0x86810.  Retail stores bEnable at this+0x814, returns when m_hWnd is
// NULL, and otherwise calls vtable slot 0x448 (SetTaskbarThumbnailClipRect)
// with either vtable slot 0x438's result (GetTaskbarThumbnailClipRect) when
// bEnable is TRUE, or an all-zero CRect when it is FALSE.
// The this+0x814 flag has no slot in OpenMFC's CMDIChildWndEx, and
// SetTaskbarThumbnailClipRect is itself a stub, so nothing observable would
// come of the calls.  STUB -- left a no-op deliberately.
// Symbol: ?EnableTaskbarThumbnailClipRect@CMDIChildWndEx@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableTaskbarThumbnailClipRect_CMDIChildWndEx__QEAAXH_Z(
    CMDIChildWndEx* pThis, int bEnable) {
    (void)pThis;
    (void)bEnable;
}

// CMDIChildWndEx::GetDocumentName(CObject**) — transcribed from retail
// mfc140u.dll RVA 0x85e90, whose whole body is:
//     CDocument* pDoc = GetActiveDocument();     // vtable slot +0x2e8
//     if (pDoc == NULL) return NULL;
//     return *(LPCTSTR*)((char*)pDoc + 0x48);    // pDoc->m_strPathName
// Note that retail never writes through pObj -- RDX is untouched from entry to
// ret -- so this build does not touch it either.  CDocument+0x48 is
// m_strPathName: ?SetPathName@CDocument@@UEAAXPEBDH@Z (mfc140.dll 0x2194c0)
// stores the path there, clears m_bEmbedded at +0x124 right after, and passes
// the same +0x48 pointer to a CWinApp virtual (slot 0x160) when bAddToMRU is
// set.  Slot +0x2e8 of the retail CMDIChildWndEx vtable
// (mfc140.dll 0x2ebf68) is ?GetActiveDocument@CFrameWnd@@UEAAPEAVCDocument@@XZ,
// so calling the CFrameWnd export here matches -- but unlike retail this is a
// direct call, so an override in a further-derived class is not honoured.
// Symbol: ?GetDocumentName@CMDIChildWndEx@@UEAAPEB_WPEAPEAVCObject@@@Z
extern "C" const wchar_t* MS_ABI impl__GetDocumentName_CMDIChildWndEx__UEAAPEB_WPEAPEAVCObject___Z(
    CMDIChildWndEx* pThis, CObject** pObj) {
    (void)pObj;  // retail leaves the out-parameter alone
    if (!pThis) {
        return nullptr;
    }
    CDocument* pDoc = impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ(pThis);
    if (!pDoc) {
        return nullptr;
    }
    return impl__GetPathName_CDocument__UEBAPEB_WXZ(pDoc);
}

// CMDIChildWndEx::GetFrameIcon() const — transcribed from retail mfc140u.dll
// RVA 0x84a90, whose whole body is:
//     HICON h = (HICON)::SendMessageW(m_hWnd, WM_GETICON /*0x7f*/, 0, 0);
//     if (h == NULL) h = (HICON)::GetClassLongPtrW(m_hWnd, -34 /*GCLP_HICONSM*/);
//     return h;
// (0x7f is materialised as `lea 0x7f(%r9)` with r9 == 0, and -34 as
// `lea -0x22(%rax)` with rax == 0 on that path; the two call slots resolve to
// USER32!SendMessageW and USER32!GetClassLongPtrW.)  Retail does not null-check
// m_hWnd; both APIs simply fail on a NULL window and the function returns NULL.
// Symbol: ?GetFrameIcon@CMDIChildWndEx@@UEBAPEAUHICON__@@XZ
extern "C" HICON MS_ABI impl__GetFrameIcon_CMDIChildWndEx__UEBAPEAUHICON____XZ(
    const CMDIChildWndEx* pThis) {
    if (!pThis) {
        return nullptr;
    }
    const HWND hWnd = pThis->m_hWnd;
    HICON hIcon = reinterpret_cast<HICON>(::SendMessage(hWnd, WM_GETICON, ICON_SMALL, 0));
    if (hIcon == nullptr) {
        hIcon = reinterpret_cast<HICON>(::GetClassLongPtr(hWnd, GCLP_HICONSM));
    }
    return hIcon;
}

// CMDIChildWndEx::GetFrameText() const — transcribed from retail mfc140u.dll
// RVA 0x84a30, whose whole body is:
//     CString str;                          // nil-string data from the string
//                                           // manager at mfc140u 0x3b25e8,
//                                           // vtable slot 3 + 0x18
//     CWnd::GetWindowTextW(str);            // 0x28be00, direct (non-virtual) call
//     return str;
// The CString comes back through the hidden return slot: retail takes `this` in
// RCX and the return buffer in RDX (`mov %rdx,%rdi ... mov %rax,(%rdi)` writes
// the string data pointer into the buffer) and returns that pointer in RAX,
// which is the parameter order used below.
// Symbol: ?GetFrameText@CMDIChildWndEx@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" CString* MS_ABI impl__GetFrameText_CMDIChildWndEx__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CMDIChildWndEx* pThis, CString* pRet) {
    if (!pRet) {
        return pRet;
    }
    new (pRet) CString();
    if (pThis) {
        impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            pThis, pRet);
    }
    return pRet;
}

// CMDIChildWndEx::GetPane(UINT) — retail mfc140u.dll RVA 0x85060 is a tail
// dispatch: `this += 0x3a8` then the embedded CDockingManager's vtable slot
// 0x58 with (nID, TRUE).  The dock manager is not modeled by OpenMFC, so there
// is no pane list to search.  STUB -- left returning NULL deliberately.
// Symbol: ?GetPane@CMDIChildWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" CBasePane* MS_ABI impl__GetPane_CMDIChildWndEx__QEAAPEAVCBasePane__I_Z(
    CMDIChildWndEx* pThis, unsigned int nID) {
    (void)pThis;
    (void)nID;
    return nullptr;
}

// CMDIChildWndEx::GetTaskbarPreviewWnd() — transcribed from retail mfc140u.dll
// RVA 0x86650, whose whole body is:
//     CWnd* pWnd = CWnd::GetDescendantWindow(m_hWnd, AFX_IDW_PANE_FIRST, FALSE);
//     if (pWnd != NULL && pWnd->m_hWnd != NULL) {
//         CWnd* pParent = CWnd::FromHandle(::GetParent(pWnd->m_hWnd));
//         if (pParent != this && pParent != NULL && pParent->m_hWnd != NULL &&
//             pParent->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
//             pWnd = pParent;
//     } else {
//         pWnd = CWnd::FromHandle(::GetWindow(m_hWnd, GW_CHILD));
//     }
//     return pWnd;
// Note the shape of the two guards: the `je` at 0x86679 and the `je` at 0x86682
// (instructions inside GetTaskbarPreviewWnd, entry 0x86650) BOTH branch to the
// same GW_CHILD block at 0x866be -- a descendant that exists but carries no HWND
// takes the fallback exactly like a missing descendant does.  (An earlier
// version of this file nested the m_hWnd test inside the non-NULL arm and
// returned the handle-less CWnd instead; that was wrong.)
// 0xe900 is AFX_IDW_PANE_FIRST, the third GetDescendantWindow argument is 0
// (bOnlyPerm == FALSE), GW_CHILD is the literal 5, and the CRuntimeClass at
// mfc140u 0x332860 spells "CSplitterWnd" in its m_lpszClassName field.
// Symbol: ?GetTaskbarPreviewWnd@CMDIChildWndEx@@UEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetTaskbarPreviewWnd_CMDIChildWndEx__UEAAPEAVCWnd__XZ(
    CMDIChildWndEx* pThis) {
    if (!pThis) {
        return nullptr;
    }
    CWnd* pWnd = impl__GetDescendantWindow_CWnd__SAPEAV1_PEAUHWND____HH_Z(
        pThis->m_hWnd, AFX_IDW_PANE_FIRST, FALSE);
    if (pWnd != nullptr && pWnd->m_hWnd != nullptr) {
        CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
            ::GetParent(pWnd->m_hWnd));
        if (pParent != static_cast<CWnd*>(pThis) && pParent != nullptr &&
            pParent->m_hWnd != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pParent, impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ())) {
            pWnd = pParent;
        }
    } else {
        pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
            ::GetWindow(pThis->m_hWnd, GW_CHILD));
    }
    return pWnd;
}

// CMDIChildWndEx::GetTaskbarThumbnailClipRect() const — transcribed from retail
// mfc140u.dll RVA 0x868b0, whose whole body is:
//     CRect rect;                       // both halves zeroed first
//     ::GetWindowRect(m_hWnd, &rect);   // USER32, into the return buffer
//     return rect;
// `this` arrives in RCX and the CRect return slot in RDX (retail zeroes (%rdx)
// and 0x8(%rdx), passes RDX straight to ::GetWindowRect and returns it in RAX),
// which is the parameter order used below.  Retail does not null-check m_hWnd;
// ::GetWindowRect then fails and the caller sees the zeroed rect.
// Symbol: ?GetTaskbarThumbnailClipRect@CMDIChildWndEx@@UEBA?AVCRect@@XZ
extern "C" CRect* MS_ABI impl__GetTaskbarThumbnailClipRect_CMDIChildWndEx__UEBA_AVCRect__XZ(
    const CMDIChildWndEx* pThis, CRect* pRet) {
    if (!pRet) {
        return pRet;
    }
    pRet->left = 0;
    pRet->top = 0;
    pRet->right = 0;
    pRet->bottom = 0;
    if (pThis) {
        // OpenMFC's CRect is not derived from RECT, but is layout-compatible
        // with it (four ints, same order), which is what retail relies on.
        ::GetWindowRect(pThis->m_hWnd, reinterpret_cast<LPRECT>(pRet));
    }
    return pRet;
}

// CMDIChildWndEx::InsertPane(CBasePane*, CBasePane*, BOOL) — retail mfc140u.dll
// RVA 0x84eb0 is a two-instruction tail jump: `this += 0x3a8` then
// CDockingManager::InsertPane(pControlBar, pTarget, bAfter), whose BOOL result
// is returned directly.  The embedded CDockingManager is not modeled.
// STUB -- retail's result comes from the dock manager, not from a constant.
// Symbol: ?InsertPane@CMDIChildWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_CMDIChildWndEx__QEAAHPEAVCBasePane__0H_Z(
    CMDIChildWndEx* pThis, CBasePane* pControlBar, CBasePane* pTarget, int bAfter) {
    (void)pThis;
    (void)pControlBar;
    (void)pTarget;
    (void)bAfter;
    return FALSE;
}

// CMDIChildWndEx::InvalidateIconicBitmaps() — retail mfc140u.dll RVA 0x840b0.
// Retail returns 0 unless IsTaskbarTabsSupportEnabled() is TRUE *and* the
// tab-proxy window at this+0x720 has an HWND; only then does it refresh the
// thumbnail clip through vtable slots 0x438/0x448, resolve
// DwmInvalidateIconicBitmaps once from dwmapi.dll (cached at mfc140.dll
// 0x3bcc00) and call it on the proxy HWND, returning 1.
// The proxy window at this+0x720 is not modeled and
// IsTaskbarTabsSupportEnabled() is itself a stub returning FALSE, so the
// reachable retail result is 0.  STUB -- only that constant is transcribed.
// Symbol: ?InvalidateIconicBitmaps@CMDIChildWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__InvalidateIconicBitmaps_CMDIChildWndEx__QEAAHXZ(
    CMDIChildWndEx* pThis) {
    (void)pThis;
    return FALSE;
}

// CMDIChildWndEx::IsPointNearDockSite(CPoint, DWORD&, BOOL&) const — retail
// mfc140u.dll RVA 0x850f0 is a two-instruction tail jump: `this += 0x3a8` then
// CDockingManager::IsPointNearDockSite(point, dwBarAlignment, bOuterEdge).
// The embedded CDockingManager is not modeled, so there are no dock sites to
// test against; retail hands the two out-parameters straight to the dock
// manager and this build leaves them untouched.
// STUB -- retail's result comes from the dock manager.
// Symbol: ?IsPointNearDockSite@CMDIChildWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_CMDIChildWndEx__QEBAHVCPoint__AEAKAEAH_Z(
    const CMDIChildWndEx* pThis, unsigned long long point,
    unsigned long* dwBarAlignment, int* bOuterEdge) {
    (void)pThis;
    (void)point;
    (void)dwBarAlignment;
    (void)bOuterEdge;
    return FALSE;
}

// CMDIChildWndEx::IsRegisteredWithTaskbarTabs() — retail mfc140u.dll
// RVA 0x83fe0 is `return m_wndTaskbarTabProxy.GetSafeHwnd() != NULL;` -- it
// reads the HWND at this+0x720+0x40 (the tab-proxy CWnd's m_hWnd) and returns
// whether it is non-NULL.  OpenMFC's CMDIChildWndEx has no proxy window at
// +0x720 (the class is a 64-byte padding blob) and RegisterTaskbarTab is a
// no-op, so no tab is ever registered and FALSE is the consistent answer.
// STUB -- only the constant reachable result is transcribed.
// Symbol: ?IsRegisteredWithTaskbarTabs@CMDIChildWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__IsRegisteredWithTaskbarTabs_CMDIChildWndEx__QEAAHXZ(
    CMDIChildWndEx* pThis) {
    (void)pThis;
    return FALSE;
}

// CMDIChildWndEx::IsTabbedMDIChild() — retail mfc140u.dll RVA 0x86ad0:
//     void* p = this->[0x718];                 // the owning CMDIFrameWndEx
//     if (p == NULL) return FALSE;
//     return (((int*)p)[0x4574/4] != 0) || (((int*)p)[0x4668/4] != 0);
// Both flags live inside CMDIFrameWndEx, which OpenMFC does not model, and the
// this+0x718 back-pointer has no slot in this class either, so the reachable
// answer is FALSE.  STUB -- only that constant is transcribed.
// Symbol: ?IsTabbedMDIChild@CMDIChildWndEx@@UEAAHXZ
extern "C" int MS_ABI impl__IsTabbedMDIChild_CMDIChildWndEx__UEAAHXZ(
    CMDIChildWndEx* pThis) {
    (void)pThis;
    return FALSE;
}

// CMDIChildWndEx::IsTaskbarTabsSupportEnabled() — retail mfc140u.dll
// RVA 0x84000.  Retail returns FALSE unless ALL of: GetTopLevelFrame() is a
// CMDIFrameWndEx; AfxGetModuleState()->[+0x8] is non-NULL and its vtable slot
// 0x210 returns TRUE; this frame's own (unnamed) vtable slot 0x3f8
// returns TRUE; and the afxGlobalData flag at +0x258 is set.  Only then does it
// return !(GetStyle() >> 19 & 1), i.e. TRUE when WS_SYSMENU is clear.
// CMDIFrameWndEx's taskbar-tab support, the CWinApp slot and AFX_GLOBAL_DATA's
// Windows-7 taskbar flag are all outside OpenMFC's model, so FALSE is the
// reachable result.  STUB -- only that constant is transcribed.
// Symbol: ?IsTaskbarTabsSupportEnabled@CMDIChildWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(
    CMDIChildWndEx* pThis) {
    (void)pThis;
    return FALSE;
}

// CMDIChildWndEx::OnChangeVisualManager(WPARAM, LPARAM) — retail mfc140u.dll
// RVA 0x860b0:
//     void* p = this->[0x718];
//     if (p != NULL && ((int*)p)[0x4668/4] == 0)
//         CFrameImpl(this+0x218).OnChangeVisualManager();   // 0x66700 in the
//                                                           // ANSI twin
//     return 0;
// Both the tabbed-group back-pointer at this+0x718 and the CFrameImpl embedded
// at this+0x218 are outside OpenMFC's model of CMDIChildWndEx, so only the
// constant 0 result is reachable.  STUB -- the visual-manager refresh is missing.
// Symbol: ?OnChangeVisualManager@CMDIChildWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnChangeVisualManager_CMDIChildWndEx__IEAA_J_K_J_Z(
    CMDIChildWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// CMDIChildWndEx::OnCreate(LPCREATESTRUCT) — retail mfc140u.dll RVA 0x83a30.
// Retail caches GetMDIFrame()-as-CMDIFrameWndEx at this+0x718 (NULL when the
// MDI frame is not a CMDIFrameWndEx), sets this->[0x210] = 1 and
// this->[0x240] = (cs->style >> 23) & 1 (i.e. WS_BORDER).  Then, only when
// GetStyle() does NOT have WS_SYSMENU (bit 19) set, it brackets a geometry pass
// in ::SendMessage(parentHwnd, WM_SETREDRAW, FALSE/TRUE, 0): it stores the
// create rect {cs->x, cs->y, cs->x + cs->cx, cs->y + cs->cy} at
// this+0x1fc..0x208 and, for a tabbed group, repositions itself against that
// group's client rect with CWnd::SetWindowPos.  It then calls
// CFrameWnd::OnCreateHelper (mfc140.dll 0x29ba30) and returns -1 if that
// fails; on success it registers with CFrameImpl::AddFrame (mfc140.dll
// 0x65f40), calls vtable slot 0x3c0 (RegisterTaskbarTab(NULL)),
// ::PostMessage(m_hWnd, AFX_WM_CHANGEVISUALMANAGER, 0, 0) and returns 0.  The
// last thing it does on either path is clear this->[0x210].
// this+0x1fc..0x240 and this+0x718 have no slots in OpenMFC's CMDIChildWndEx,
// and CFrameImpl is not modeled, so the frame would be left half-initialised if
// only fragments were run.  The existing `return 0` is retail's success result.
// STUB -- only that constant is transcribed.
// Symbol: ?OnCreate@CMDIChildWndEx@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMDIChildWndEx__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMDIChildWndEx* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)pThis;
    (void)lpCreateStruct;
    return 0;
}

// CMDIChildWndEx::OnDestroy() — partially transcribed from retail mfc140u.dll
// RVA 0x856f0, which is, in order:
//     UnregisterTaskbarTab(TRUE);                       // 0x855d0
//     <notify the owning tabbed group at this+0x718>
//     <detach/hide the tabbed pane cached at this+0x710 and clear that slot>
//     CFrameImpl::RemoveFrame(this);                    // 0x65f90 in the ANSI
//                                                       // twin
//     <walk the list at this+0x528 releasing its entries>
// Only the first step is reproducible: this+0x528, this+0x710 and this+0x718
// have no slots in OpenMFC's CMDIChildWndEx and CFrameImpl is not modeled.
// Note that retail does NOT chain to CMDIChildWnd::OnDestroy, and neither does
// this build.
// Symbol: ?OnDestroy@CMDIChildWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMDIChildWndEx__IEAAXXZ(CMDIChildWndEx* pThis) {
    if (!pThis) {
        return;
    }
    impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(pThis, TRUE);
    // TODO(clean-room): the tabbed-group / tabbed-pane teardown and
    // CFrameImpl::RemoveFrame need this+0x528/0x710/0x718 and the CFrameImpl at
    // this+0x218, none of which are modeled.
}

// CMDIChildWndEx::OnEraseBkgnd(CDC*) — transcribed from retail mfc140u.dll
// RVA 0x3a60, whose entire body is `mov $1,%eax; ret`: it claims the erase
// unconditionally and never touches the DC.  The previous body returned 0,
// which told Windows to erase the background with the class brush.  The body is
// intentionally a single constant, because retail's is.
// Two caveats for whoever re-checks this: (1) the export has no entry in
// mfc140_rva_symbols.json (the ANSI map disas.py reads) -- only the mfc140u map
// resolves it; the ANSI twin's folded twin at mfc140.dll 0x3ae0 is the same
// `mov $1,%eax; ret`.  (2) 0x3a60 is an identical-COMDAT-folded address shared
// by 101 exports, so a reverse-map disassembler prints some other name at it.
// Symbol: ?OnEraseBkgnd@CMDIChildWndEx@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMDIChildWndEx__IEAAHPEAVCDC___Z(
    CMDIChildWndEx* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    return TRUE;
}

// CMDIChildWndEx::OnGetMinMaxInfo(MINMAXINFO*) — transcribed from retail
// mfc140u.dll RVA 0x85960, whose whole body is:
//     void* p = this->[0x718];
//     if (p != NULL && ((int*)p)[0x6d8/4] != 0 && ((int*)p)[0x4574/4] == 0 &&
//         ((int*)p)[0x4668/4] == 0) {
//         int cx = ((int*)p)[0x6c8/4] - ((int*)p)[0x6c0/4];
//         lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x = cx;   // +0x20, +0x8
//         int cy = ((int*)p)[0x6cc/4] - ((int*)p)[0x6c4/4];
//         lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y = cy;   // +0x24, +0xc
//         return;
//     }
//     CWnd::Default();      // tail jump
// this+0x718 (the owning CMDIFrameWndEx) has no slot in OpenMFC's
// CMDIChildWndEx, so the guarded clamp can never be taken here and the
// CWnd::Default() tail is the whole of the reachable behaviour.
// Symbol: ?OnGetMinMaxInfo@CMDIChildWndEx@@IEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CMDIChildWndEx__IEAAXPEAUtagMINMAXINFO___Z(
    CMDIChildWndEx* pThis, MINMAXINFO* lpMMI) {
    (void)lpMMI;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail clamps ptMaxSize/ptMaxTrackSize to the owning
    // tabbed group's client size when this+0x718 reports one; unmodeled.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnIdleUpdateCmdUI(WPARAM, LPARAM) — retail mfc140u.dll
// RVA 0x85e60 is:
//     CDockingManager(this+0x3a8).SendMessageToMiniFrames(0x363, 0, 0);
//     return 0;
// 0x363 is WM_IDLEUPDATECMDUI.  The embedded CDockingManager is not modeled by
// OpenMFC, so there are no mini-frames to broadcast to; the constant 0 result
// is all that is reachable.  STUB -- the broadcast is missing.
// Symbol: ?OnIdleUpdateCmdUI@CMDIChildWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CMDIChildWndEx__IEAA_J_K_J_Z(
    CMDIChildWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// CMDIChildWndEx::OnLButtonDown(UINT, CPoint) — transcribed from retail
// mfc140u.dll RVA 0x86510, whose whole body is:
//     CFrameImpl(this+0x218).OnLButtonDown(point);   // 0x66490
//     CWnd::Default();                               // tail jump to 0x28ac80
// The CFrameImpl embedded at this+0x218 is not modeled by OpenMFC (it drives
// the caption-button hit testing), so only the base dispatch is reproduced.
// Symbol: ?OnLButtonDown@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMDIChildWndEx__IEAAXIVCPoint___Z(
    CMDIChildWndEx* pThis, unsigned int nFlags, unsigned long long point) {
    (void)nFlags;
    (void)point;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail first runs CFrameImpl::OnLButtonDown on the
    // sub-object at this+0x218; unmodeled.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnLButtonUp(UINT, CPoint) — transcribed from retail
// mfc140u.dll RVA 0x864b0, whose whole body is:
//     CFrameImpl(this+0x218).OnLButtonUp(point);     // 0x664e0
//     CWnd::Default();                               // tail jump to 0x28ac80
// The CFrameImpl at this+0x218 is not modeled, so only the base dispatch is
// reproduced.
// Symbol: ?OnLButtonUp@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMDIChildWndEx__IEAAXIVCPoint___Z(
    CMDIChildWndEx* pThis, unsigned int nFlags, unsigned long long point) {
    (void)nFlags;
    (void)point;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail first runs CFrameImpl::OnLButtonUp on the
    // sub-object at this+0x218; unmodeled.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnMDIActivate(BOOL, CWnd*, CWnd*) — partially transcribed
// from retail mfc140u.dll RVA 0x84480:
//     CDockingManager(this+0x3a8).vtbl[0xc0]();
//     this->[0x20c] = bActivate;
//     if (g_bInMDIActivate == 0) {                // process-wide latch,
//                                                 // mfc140u data 0x3c3e54
//         g_bInMDIActivate = TRUE;
//         CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
//         if (bActivate && this->[0x718])
//              CMDIClientAreaWnd::SetActiveTab((this->[0x718])+0x720,
//                                              pActivateWnd ? pActivateWnd->m_hWnd
//                                                           : NULL);   // 0x7e540
//         <taskbar-tab refresh: InvalidateIconicBitmaps() on this and on
//          pDeactivateWnd when it is a CMDIChildWndEx, then SetTaskbarTabActive()>
//         g_bInMDIActivate = FALSE;
//         <a second bActivate-gated block driven by this+0x718>
//     }
//  tail:                                          // 0x845fc / 0x84600 /
//                                                 // 0x8460c / 0x8461f
//     if (!bActivate || IsTaskbarTabsSupportEnabled()) {   // 0x84600
//         if (m_wndTaskbarTabProxy(this+0x720).m_hWnd != NULL)  // 0x8460c
//             return;
//     }
//     this->vtbl[0x448](CRect(0,0,0,0));          // SetTaskbarThumbnailClipRect
// Both the latched and the unlatched path fall into that shared tail: the
// latch's `jne` at 0x844b8 targets 0x845fc, not the ret at 0x84640.  (An
// earlier version of this comment drew the re-entrant path as a plain return;
// it is not.)
// The dock manager, this+0x20c and this+0x718 are not modeled, so only the
// recursion latch and the base dispatch are reproduced.  The tail is omitted
// here because SetTaskbarThumbnailClipRect is itself a stub in this build and
// the call would have no observable effect.  The latch is kept in a file-local
// static, matching retail's single process-wide (non-thread-safe) variable.
// Symbol: ?OnMDIActivate@CMDIChildWndEx@@IEAAXHPEAVCWnd@@0@Z
extern "C" void MS_ABI impl__OnMDIActivate_CMDIChildWndEx__IEAAXHPEAVCWnd__0_Z(
    CMDIChildWndEx* pThis, int bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) {
    if (!pThis) {
        return;
    }
    if (g_bInMDIActivate) {
        return;
    }
    g_bInMDIActivate = true;
    impl__OnMDIActivate_CMDIChildWnd__IEAAXHPEAVCWnd__0_Z(
        pThis, bActivate, pActivateWnd, pDeactivateWnd);
    // TODO(clean-room): the MDI-tab activation and taskbar-tab refresh need
    // this+0x718 and the tab-proxy window; unmodeled.
    g_bInMDIActivate = false;
}

// CMDIChildWndEx::OnMouseMove(UINT, CPoint) — transcribed from retail
// mfc140u.dll RVA 0x864e0, whose whole body is:
//     CFrameImpl(this+0x218).OnMouseMove(point);     // 0x665e0
//     CWnd::Default();                               // tail jump to 0x28ac80
// The CFrameImpl at this+0x218 is not modeled, so only the base dispatch is
// reproduced.
// Symbol: ?OnMouseMove@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMDIChildWndEx__IEAAXIVCPoint___Z(
    CMDIChildWndEx* pThis, unsigned int nFlags, unsigned long long point) {
    (void)nFlags;
    (void)point;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail first runs CFrameImpl::OnMouseMove on the
    // sub-object at this+0x218; unmodeled.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnMoveMiniFrame(CWnd*) — retail mfc140u.dll RVA 0x85270 is a
// four-instruction tail dispatch: `this += 0x3a8` then the embedded
// CDockingManager's vtable slot 0x88 with the same CWnd*, returning its BOOL
// result.  The dock manager is not modeled by OpenMFC.
// STUB -- retail's result comes from the dock manager, not from a constant.
// Symbol: ?OnMoveMiniFrame@CMDIChildWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_CMDIChildWndEx__UEAAHPEAVCWnd___Z(
    CMDIChildWndEx* pThis, CWnd* pFrame) {
    (void)pThis;
    (void)pFrame;
    return FALSE;
}

// CMDIChildWndEx::OnNcActivate(BOOL) — partially transcribed from retail
// mfc140u.dll RVA 0x85f90:
//     BOOL bRepaint = CFrameImpl(this+0x218).OnNcActivate(bActive);  // 0x65ba0
//     if (bRepaint) ::SendMessageW(m_hWnd, WM_SETREDRAW, FALSE, 0);
//     LRESULT lRes = CWnd::Default();                                // 0x28ac80
//     if (bRepaint) {
//         ::SendMessageW(m_hWnd, WM_SETREDRAW, TRUE, 0);
//         ::RedrawWindow(m_hWnd, NULL, NULL, 0x585);
//     }
//     <AfxGetModuleState()->[+0x8] slot-0x208 / slot-0x70 probe; when it says
//      no, InvalidateIconicBitmaps()>
//     return (BOOL)lRes;
// 0x585 == RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_NOCHILDREN|RDW_FRAME
// (0x1|0x4|0x80|0x100|0x400).  CFrameImpl is not modeled by OpenMFC, so
// bRepaint is unobtainable and the SetRedraw bracket is skipped entirely --
// note that this means the WM_SETREDRAW pair is omitted on BOTH paths, not
// conditionally.  InvalidateIconicBitmaps() is itself a stub, so the module-
// state probe is skipped too.  The previous body returned 0 unconditionally,
// which suppressed the non-client active-state change.
// Symbol: ?OnNcActivate@CMDIChildWndEx@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CMDIChildWndEx__IEAAHH_Z(
    CMDIChildWndEx* pThis, int bActive) {
    (void)bActive;
    if (!pThis) {
        return FALSE;
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
}

// Symbol: ?OnNcCalcSize@CMDIChildWndEx@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMDIChildWndEx__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?OnNcHitTest@CMDIChildWndEx@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMDIChildWndEx__IEAA_JVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?OnNcMouseMove@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CMDIChildWndEx__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcPaint@CMDIChildWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMDIChildWndEx__IEAAXXZ() {}

// Symbol: ?OnNcRButtonUp@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcRButtonUp_CMDIChildWndEx__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnPressTaskbarThmbnailCloseButton@CMDIChildWndEx@@UEAAXXZ
extern "C" void MS_ABI impl__OnPressTaskbarThmbnailCloseButton_CMDIChildWndEx__UEAAXXZ() {}

// Symbol: ?OnSendIconicLivePreviewBitmap@CMDIChildWndEx@@IEAAX_K_J@Z
extern "C" void MS_ABI impl__OnSendIconicLivePreviewBitmap_CMDIChildWndEx__IEAAX_K_J_Z(unsigned __int64 p0, __int64 p1) {}

// Symbol: ?OnSendIconicThumbnail@CMDIChildWndEx@@IEAAX_K_J@Z
extern "C" void MS_ABI impl__OnSendIconicThumbnail_CMDIChildWndEx__IEAAX_K_J_Z(unsigned __int64 p0, __int64 p1) {}

// Symbol: ?OnSetIcon@CMDIChildWndEx@@IEAAPEAUHICON__@@HPEAU2@@Z
extern "C" void* MS_ABI impl__OnSetIcon_CMDIChildWndEx__IEAAPEAUHICON____HPEAU2__Z(int p0, void* /*struct*/* p1) {
    return nullptr;
}

// Symbol: ?OnSetPreviewMode@CMDIChildWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_CMDIChildWndEx__UEAAXHPEAUCPrintPreviewState___Z(int p0, void* /*struct*/* p1) {}
