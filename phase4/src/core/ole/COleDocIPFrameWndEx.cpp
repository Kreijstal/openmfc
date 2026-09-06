// COleDocIPFrameWndEx — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, olecore.cpp
//
// Behaviour below was decoded from the retail mfc140 image (mangled name -> RVA
// -> objdump).  Two symbols could not be reached that way and say so at their
// own call sites: ?AddDockSite@ has no body anywhere in the image, and
// ?OnToolbarContextMenu@ has no name in the map and was located through this
// class's message map instead (it still cites the RVA it landed on).
//
// The `if (!pThis)` / `if (!pPane)` guards in the bodies below are OpenMFC's
// own defensiveness: retail dereferences those pointers unconditionally.
//
// ---------------------------------------------------------------------------
// Where the retail bodies live, and why most of them carry another class's name
// ---------------------------------------------------------------------------
// mfc140_rva_symbols.json names only ten RVAs for this class.  The reason is
// identical-COMDAT folding: most of COleDocIPFrameWndEx's methods compile to
// byte-identical code to COleIPFrameWndEx's, so the linker keeps a single copy
// and the map records just one of the two names for it.  (Beyond the three
// per-class functions that cannot fold by construction — CreateObject,
// GetThisClass, GetThisMessageMap — five methods plus the ctor and dtor also
// kept their own bodies; the reasons are set out under "Retail layout facts"
// below, and they are NOT all the same reason.)  Two independent pieces of
// evidence in the image tie the shared bodies to THIS class:
//
//   * COleDocIPFrameWndEx's own vftable (0x2ef298, the pointer ??0 stores at
//     RVA 0x956c0) holds them.  Read out of the image:
//         +0x028 -> 0x96460  named ?OnCmdMsg@COleIPFrameWndEx@@...
//         +0x0d8 -> 0x96090  named ?PreCreateWindow@COleIPFrameWndEx@@...
//         +0x108 -> 0x95be0  ?WinHelpA@COleDocIPFrameWndEx@@...      (own copy)
//         +0x110 -> 0x95bc0  ?HtmlHelpA@COleDocIPFrameWndEx@@...     (own copy)
//         +0x1f8 -> 0x95ae0  named ?OnCommand@COleIPFrameWndEx@@...
//         +0x228 -> 0x958b0  named ?PreTranslateMessage@COleIPFrameWndEx@@...
//         +0x2e0 -> 0x95b80  named ?LoadFrame@COleIPFrameWndEx@@...
//         +0x300 -> 0x96210  ?RecalcLayout@COleDocIPFrameWndEx@@...  (own copy)
//         +0x338 -> 0x961b0  named ?OnSetPreviewMode@COleIPFrameWndEx@@...
//         +0x390 -> 0x96330  named ?OnCreateControlBars@COleIPFrameWndEx@@...
//         +0x3d0 -> 0x963e0  named ?OnShowPanes@COleIPFrameWndEx@@...
//         +0x3d8 -> 0x95fe0  named ?AdjustDockingLayout@COleIPFrameWndEx@@...
//         +0x3e0 -> 0x96050  named ?OnMoveMiniFrame@COleIPFrameWndEx@@...
//         +0x3f8 -> 0x95ac0  named ?OnClosePopupMenu@COleIPFrameWndEx@@...
//   * COleDocIPFrameWndEx's own message map (0x2ef0a0, returned by
//     ?GetThisMessageMap@COleDocIPFrameWndEx@@, entries at 0x2ef0b0) dispatches
//     to 0x957d0 (OnMenuChar), 0x95cb0 (OnActivate, own copy), 0x95d60
//     (OnClose), 0x95d90 (OnDestroy), 0x962d0 (OnIdleUpdateCmdUI), 0x960d0
//     (OnUpdatePaneMenu), 0x96140 (OnPaneCheck), 0x3ae0, 0x95c60
//     (OnToolbarCreateNew) and 0x95c90 (OnToolbarDelete).  Of those, 0x95cb0
//     is named for THIS class in the map and the rest are named for
//     COleIPFrameWndEx.
// The non-virtual, non-message-map methods that are pure docking-manager
// forwarders (`add $0x428,%rcx; jmp CDockingManager::X`) reference nothing
// class-specific at all, so their single body necessarily serves both classes.
// Each function below cites the RVA it was read from and says plainly when the
// name attached to that RVA is the COleIPFrameWndEx one.
//
// ---------------------------------------------------------------------------
// Retail layout facts, established from ??0COleDocIPFrameWndEx@@IEAA@XZ
// (RVA 0x956c0), ??1 (0x95770) and ?CreateObject@ (0x95670)
// ---------------------------------------------------------------------------
//   * operator new is called with 0x790 == 1936 bytes, exactly the
//     m_nObjectSize recorded for classCOleDocIPFrameWndEx in
//     phase4/src/core/ole/RuntimeClasses.cpp:242.
//   * the base COleDocIPFrameWnd constructor (0x255950) is called with %rcx
//     unchanged, so COleDocIPFrameWnd and COleDocIPFrameWndEx share offset 0.
//   * the constructor then initialises members OpenMFC does not model:
//         this+0x288  DWORD      (in-context-help flag; see OnContextHelp,
//                                 WinHelp and HtmlHelp below)
//         this+0x290  void*      (HWND of the previously active frame)
//         this+0x298  CFrameImpl (constructed with `this`)
//         this+0x428  CDockingManager
//     NOTE the help flag is at 0x288 here; the corresponding member of
//     COleIPFrameWndEx is at 0x284 (read from ??0COleIPFrameWndEx@@, 0x96510),
//     and COleIPFrameWndEx additionally stores `this` at this+0x5d8, which this
//     class's constructor does not do.  That layout difference accounts for the
//     ctor, the dtor and the WinHelp/HtmlHelp/OnContextHelp trio having their
//     own bodies.  It does NOT account for the other two unfolded methods, and
//     the two classes are not one source modulo one member offset — both of the
//     following were read out of the image and differ in logic, not offsets:
//       - OnActivate: this class (0x95cb0) calls CFrameImpl::DeactivateMenu
//         unconditionally on deactivation; COleIPFrameWndEx's own copy
//         (0x966b0) first gates it on the module DWORD at 0x3b70bc
//         (CMFCToolBar's customize-mode flag — see OnCommand below).
//       - RecalcLayout: this class (0x96210) calls
//         COleDocIPFrameWnd::RecalcLayout (0x2559d0) and then only the docking
//         manager's vtable +0x78; COleIPFrameWndEx's copy (0x96770) calls
//         COleIPFrameWnd::RecalcLayout (0x25c500) and ALSO the docking
//         manager's vtable +0x80 before +0x78.
//   * both retail CRuntimeClass descriptors read out of the image agree with
//     this tree: COleDocIPFrameWnd 640 bytes, COleDocIPFrameWndEx 1936 bytes.
//
// OpenMFC has no COleDocIPFrameWndEx and no COleDocIPFrameWnd class — grep over
// include/openmfc/ finds only a forward declaration of COleDocIPFrameWnd
// (afxole.h:528) — and there is no shadow struct for either in
// phase4/src/detail/.  None of the members above are therefore reachable here.
// Every retail body that only touches the embedded CFrameImpl / CDockingManager
// (or a module global) is left a documented stub rather than guessed at; the
// bodies that delegate to an exported base-class method are implemented.

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/OlecoreSupport.h"


// ---- sibling impl_ exports we delegate to (all verified to exist in the tree)
// COleDocIPFrameWnd / COleIPFrameWnd (core/ole/COleDocIPFrameWnd.cpp,
// core/ole/COleIPFrameWnd.cpp, core/ole/Thunks.cpp).  Checked symbol by symbol:
//   * impl___1COleDocIPFrameWnd__UEAA_XZ    — returns nullptr, does nothing;
//     defined at core/ole/COleDocIPFrameWnd.cpp:11 with NO parameters.
//   * impl__RecalcLayout_COleDocIPFrameWnd__MEAAXH_Z — empty body, defined at
//     core/ole/COleDocIPFrameWnd.cpp:32 as `(int p0)`, i.e. also no `this`.
//   * impl__OnDestroy_COleIPFrameWnd__IEAAXXZ — empty body, defined at
//     core/ole/COleIPFrameWnd.cpp:89, declared there with NO parameters.
//   * impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ — has NO definition
//     anywhere in phase4/src; the export is satisfied by build_phase4.sh's
//     generated stub set.  It is already declared with a `void* pThis` in
//     phase4/src/detail/COleCntrFrameWndSupport.h:59, which is the convention
//     followed here.
// Declaring a `this` parameter that the definition does not have is harmless
// under MS x64: `this` rides in RCX and the callee simply ignores it.
extern "C" void* MS_ABI impl___1COleDocIPFrameWnd__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__RecalcLayout_COleDocIPFrameWnd__MEAAXH_Z(void* pThis, int bNotify);
extern "C" void MS_ABI impl__OnDestroy_COleIPFrameWnd__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ(void* pThis);
// CFrameWnd (core/frame/CFrameWnd.cpp, core/frame/Thunks.cpp)
extern "C" int MS_ABI impl__LoadFrame_CFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CFrameWnd* pThis, UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext);
extern "C" void MS_ABI impl__OnActivate_CFrameWnd__IEAAXIPEAVCWnd__H_Z(
    CFrameWnd* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized);
extern "C" void MS_ABI impl__OnClose_CFrameWnd__IEAAXXZ(CFrameWnd* pThis);
extern "C" int MS_ABI impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CFrameWnd* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
extern "C" int MS_ABI impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(
    CFrameWnd* pThis, WPARAM wParam, LPARAM lParam);
extern "C" void MS_ABI impl__OnContextHelp_CFrameWnd__QEAAXXZ(CFrameWnd* pThis);
extern "C" __int64 MS_ABI impl__OnMenuChar_CFrameWnd__IEAA_JIIPEAVCMenu___Z(
    CFrameWnd* pThis, unsigned int nChar, unsigned int nFlags, CMenu* pMenu);
extern "C" void MS_ABI impl__OnSetPreviewMode_CFrameWnd__UEAAXHPEAUCPrintPreviewState___Z(
    CFrameWnd* pThis, int bPreview, CPrintPreviewState* pState);
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(
    CFrameWnd* pThis, MSG* pMsg);
// CWnd (core/window/CWnd.cpp)
extern "C" void MS_ABI impl__HtmlHelpW_CWnd__UEAAX_KI_Z(
    CWnd* pThis, unsigned __int64 dwData, unsigned int nCmd);
extern "C" void MS_ABI impl__WinHelpW_CWnd__UEAAX_KI_Z(
    CWnd* pThis, unsigned __int64 dwData, unsigned int nCmd);
// MFC exception helper (phase4/src/detail/MfcExceptionsSupport.cpp:35).
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// CBasePane::ShowPane (featurepack/docking/CBasePane.cpp:29).  The pane is
// carried as void* here — ABI-identical for a pointer argument, and the same
// thunk is forwarded to from featurepack/docking/CDockSite.cpp and
// CDockablePane.cpp, which spell the first parameter CBasePane*.
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(
    void* pPane, int bShow, int bDelay, int bActivate);

// CMFCPopupMenu's module-global "currently active popup" pointer.  Retail keeps
// it at 0x3b6fe8; OpenMFC models it as openmfc::detail::mfccore::g_pActivePopupMenu
// (declared phase4/src/detail/MfccoreSupport.h:781, defined and maintained in
// featurepack/menu/CMFCPopupMenu.cpp).  Reached here through the two exported
// thunks that read and write it — ?GetSafeActivePopupMenu@ (CMFCPopupMenu.cpp:395)
// and ?ActivatePopupMenu@ (the thunk at CMFCPopupMenu.cpp:391, forwarding to
// CMFCPopupMenu::ActivatePopupMenu at CMFCPopupMenu.cpp:450, whose entire body
// is `g_pActivePopupMenu = pPopupMenu; return pPopupMenu != nullptr;`).  Both
// are spelled with void* pointers here, which is ABI-identical.
extern "C" void* MS_ABI impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
extern "C" int MS_ABI impl__ActivatePopupMenu_CMFCPopupMenu__SAHPEAVCFrameWnd__PEAV1__Z(
    void* pTopFrame, void* pPopupMenu);

// Forward declaration of our own HMENU overload; the WPARAM/LPARAM overload
// calls it, exactly as retail does (RVA 0x95820 -> 0x95860).
extern "C" int MS_ABI impl__OnSetMenu_COleDocIPFrameWndEx__IEAAHPEAUHMENU_____Z(void* pThis, HMENU hMenu);


// COleDocIPFrameWndEx::COleDocIPFrameWndEx() — retail (RVA 0x956c0):
//     COleDocIPFrameWnd::COleDocIPFrameWnd(this);   // 0x255950, %rcx unchanged
//     *(void**)this = &COleDocIPFrameWndEx_vftable; // 0x2ef298
//     this->[0x288] = 0; this->[0x290] = nullptr;
//     CFrameImpl::CFrameImpl(this+0x298, this);     // 0x61d20
//     CDockingManager::CDockingManager(this+0x428); // 0x46d40
//     return this;                                  // `mov %rbx,%rax`
// Left exactly as it was: this thunk already forwards nothing but `this`, and
// OpenMFC models neither a COleDocIPFrameWndEx vtable nor the CFrameImpl /
// CDockingManager sub-objects.  impl___0COleDocIPFrameWnd__QEAA_XZ
// (core/ole/COleDocIPFrameWnd.cpp:7) is itself a body that only returns pThis,
// so calling it would add nothing; it is deliberately not called.
// Symbol: ??0COleDocIPFrameWndEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0COleDocIPFrameWndEx__IEAA_XZ(void* pThis) {
    return pThis;
}

// COleDocIPFrameWndEx::OnUpdatePaneMenu(CCmdUI*) — retail (RVA 0x960d0, named
// ?OnUpdatePaneMenu@COleIPFrameWndEx@@ in the map, reached from THIS class's
// message-map entries 5/7/9), transcribed in full:
//     pane = CDockingManager::FindPaneByID(this+0x428, pCmdUI->m_nID /*+0x8*/,
//                                          TRUE);          // vtable +0x58
//     if (!pane) { pCmdUI->[0x2c] = 1; return; }            // continue routing
//     pCmdUI->vtable[+0x08](pCmdUI, (CWnd::GetStyle(pane) >> 28) & 1);
//                                    // GetStyle 0x2a75a0; bit 28 == WS_VISIBLE.
// Slot +0x08 was resolved in the image: ??0CCmdUI@@ (0x1dc830) stores vftable
// 0x3210e8, whose entries are Enable / SetCheck / SetRadio / SetText, so the
// call is SetCheck(bVisible).
// Retail NEVER calls CCmdUI::Enable here.  This file used to do exactly that
// (`if (ui) Enable(TRUE)`), which is a behaviour retail does not have on any
// path, so it has been reverted to a documented no-op.  The pane lookup needs
// the embedded CDockingManager at this+0x428, and the pCmdUI+0x2c write has no
// counterpart in OpenMFC's CCmdUI: that class (include/openmfc/afxole.h:68) is
// vptr / m_nID@8 / m_pOther@16 / m_nIndex@24 / m_pMenu@32 / m_pSubMenu@40,
// sizeof 56 (offsets confirmed by compiling offsetof against the real header),
// so 0x2c==44 lands in the middle of its m_pSubMenu pointer — writing there
// would corrupt the caller's object, not set a flag.  With no docking manager
// the pane is never found, so retail's reachable path here is exactly that one
// write, and it is the part this tree cannot express.
// Symbol: ?OnUpdatePaneMenu@COleDocIPFrameWndEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_COleDocIPFrameWndEx__IEAAXPEAVCCmdUI___Z(void* pThis, CCmdUI* ui) {
    (void)pThis; (void)ui;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428 and a
    // CCmdUI::m_bContinueRouting member (pCmdUI+0x2c).
}

// COleDocIPFrameWndEx::~COleDocIPFrameWndEx() — retail (RVA 0x95770):
//     *(void**)this = &COleDocIPFrameWndEx_vftable;   // 0x2ef298
//     CDockingManager::~CDockingManager(this+0x428);  // 0x470f0
//     CFrameImpl::~CFrameImpl(this+0x298);            // 0x61ee0
//     *(void**)this = &COleDocIPFrameWnd_vftable;     // 0x32cb60
//     jmp COleIPFrameWnd::~COleIPFrameWnd(this);      // 0x25bfd0
// DEVIATION, deliberate: the retail tail-jump goes straight to COleIPFrameWnd's
// destructor because COleDocIPFrameWnd's trivial destructor was inlined (that
// is what the second vptr store is).  Routing through the immediate base
// instead keeps the chain correct if COleDocIPFrameWnd ever grows a real
// destructor; both are no-ops today (COleDocIPFrameWnd.cpp:11).
// The two sub-objects are not modeled, so nothing is destroyed for them —
// nothing was constructed either.
// Symbol: ??1COleDocIPFrameWndEx@@MEAA@XZ
extern "C" void* MS_ABI impl___1COleDocIPFrameWndEx__MEAA_XZ(void* pThis) {
    if (!pThis) return nullptr;
    impl___1COleDocIPFrameWnd__UEAA_XZ(pThis);
    return pThis;
}

// COleDocIPFrameWndEx::AddDockSite() — this export has no resolved RVA in
// mfc140_rva_symbols.json (disas.py reports no match for the name, and no match
// for ?AddDockSite@COleIPFrameWndEx@@ either), which is what an empty body looks
// like after folding: there is nothing left to name.  Documented MFC says the
// same ("this method is not used").  Left the no-op the generated stub was.
// Symbol: ?AddDockSite@COleDocIPFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_COleDocIPFrameWndEx__QEAAXXZ(void* pThis) {
    (void)pThis;
}

// COleDocIPFrameWndEx::AddPane(CBasePane*, BOOL) — retail (RVA 0x95f10, the map
// names it ?AddPane@COleIPFrameWndEx@@; the body is a pure docking-manager
// forward and so is shared):
//     %rcx += 0x428; %r9d = 0; [rsp+0x20] = 0
//     CDockingManager::AddPane(&m_dockManager, pPane, bTail, FALSE, FALSE)  // 0x476b0
// The embedded CDockingManager at this+0x428 is not modeled — see the header
// comment — so no docking manager can be reached from here.
// Symbol: ?AddPane@COleDocIPFrameWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_COleDocIPFrameWndEx__QEAAHPEAVCBasePane__H_Z(
    void* pThis, void* pPane, int bTail) {
    (void)pThis; (void)pPane; (void)bTail;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleDocIPFrameWndEx::AdjustDockingLayout(HDWP) — retail (RVA 0x95fe0; the map
// names it ?AdjustDockingLayout@COleIPFrameWndEx@@, and slot +0x3d8 of THIS
// class's vftable points at it):
//     %rcx = this->m_hWnd (this+0x40); %edx = 5 (GW_CHILD)
//     do {
//         w = ::GetWindow(%rcx, %edx);          // IAT slot 0x2c4c88
//         p = CWnd::FromHandle(w);              // 0x289180
//         if (!p) break;
//         if (!p->IsKindOf(&classCBasePane)) break;   // 0x233310, descriptor 0x2da490
//         %rcx = this->m_hWnd; %edx = 2 (GW_HWNDNEXT);
//     } while (true);
//     tail-call (this+0x428)->vtable[+0x80](this+0x428, pParam);
//         // CDockingManager::AdjustDockingLayout, 0x49930
// The CRuntimeClass at 0x2da490 was read straight out of the image: name
// "CBasePane", m_nObjectSize 424, m_wSchema 0xFFFF.  The walk re-reads
// `this->m_hWnd` on every iteration rather than advancing off the child handle,
// so GW_HWNDNEXT keeps returning the frame's own next sibling; that is what the
// bytes do (`mov 0x40(%rbx),%rcx` at 0x9600e, with %rbx == this).
// The docking manager at this+0x428 is not modeled.
// Symbol: ?AdjustDockingLayout@COleDocIPFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_COleDocIPFrameWndEx__UEAAXPEAX_Z(
    void* pThis, void* pParam) {
    (void)pThis; (void)pParam;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
}

// COleDocIPFrameWndEx::CreateObject() — retail (RVA 0x95670), transcribed in
// full:
//     rax = ??2@YAPEAX_K@Z(0x790);              // MFC operator new, 0x2840
//     if (rax) { rcx = rax; ??0COleDocIPFrameWndEx@@IEAA@XZ(rcx); }  // 0x956c0
//     return rax;                                // the ctor leaves `this` in rax
// 0x790 == 1936 == classCOleDocIPFrameWndEx's m_nObjectSize in
// core/ole/RuntimeClasses.cpp:242, so the size is pinned from both sides.
//
// Left an explicit null stub rather than transcribed.  The whole job of this
// export is to hand back an object whose *dynamic type* is COleDocIPFrameWndEx,
// and this tree has no such class (grep over include/openmfc/ finds no
// declaration), so new+ctor could only produce the untyped 1936-byte block that
// impl___0COleDocIPFrameWndEx__IEAA_XZ above leaves untouched: no vptr, and
// bytes 0..1936 uninitialized.  Handing that to a caller as a CObject is a
// silent identity error; returning null fails at the caller's first
// dereference instead.  It also matches the base class —
// ?CreateObject@COleDocIPFrameWnd@@SAPEAVCObject@@XZ
// (core/ole/COleDocIPFrameWnd.cpp:21) is a null stub too — and
// classCOleDocIPFrameWndEx.m_pfnCreateObject is already nullptr
// (RuntimeClasses.cpp:228 OF_DESC), so no DYNCREATE path in this tree reaches
// here regardless.
// Symbol: ?CreateObject@COleDocIPFrameWndEx@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_COleDocIPFrameWndEx__SAPEAVCObject__XZ() {
    return nullptr;
}

// COleDocIPFrameWndEx::DockPane(CBasePane*, UINT, LPCRECT) — retail
// (RVA 0x95f50, named ?DockPane@COleIPFrameWndEx@@ in the map):
//     add $0x428,%rcx; jmp CDockingManager::DockPane   // 0x483c0
// The docking manager at this+0x428 is not modeled.
// Symbol: ?DockPane@COleDocIPFrameWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_COleDocIPFrameWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    void* pThis, void* pPane, unsigned int nDockBarID, const RECT* lpRect) {
    (void)pThis; (void)pPane; (void)nDockBarID; (void)lpRect;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
}

// COleDocIPFrameWndEx::DockPaneLeftOf(CPane*, CPane*) — retail (RVA 0x961a0,
// named ?DockPaneLeftOf@COleIPFrameWndEx@@ in the map):
//     add $0x428,%rcx; jmp CDockingManager::DockPaneLeftOf   // 0x48280
// The docking manager at this+0x428 is not modeled.
// Symbol: ?DockPaneLeftOf@COleDocIPFrameWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_COleDocIPFrameWndEx__QEAAHPEAVCPane__0_Z(
    void* pThis, void* pBarToDock, void* pBar) {
    (void)pThis; (void)pBarToDock; (void)pBar;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleDocIPFrameWndEx::EnableAutoHidePanes(DWORD) — retail (RVA 0x96080, named
// ?EnableAutoHidePanes@COleIPFrameWndEx@@ in the map):
//     add $0x428,%rcx; jmp CDockingManager::EnableAutoHidePanes   // 0x47350
// The docking manager at this+0x428 is not modeled.
// Symbol: ?EnableAutoHidePanes@COleDocIPFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_COleDocIPFrameWndEx__QEAAHK_Z(
    void* pThis, unsigned long dwAlignment) {
    (void)pThis; (void)dwAlignment;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleDocIPFrameWndEx::EnableDocking(DWORD) — retail (RVA 0x96070, named
// ?EnableDocking@COleIPFrameWndEx@@ in the map):
//     add $0x428,%rcx; jmp CDockingManager::EnableDocking   // 0x47240
// The docking manager at this+0x428 is not modeled.
// Symbol: ?EnableDocking@COleDocIPFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_COleDocIPFrameWndEx__QEAAHK_Z(
    void* pThis, unsigned long dwDockStyle) {
    (void)pThis; (void)dwDockStyle;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleDocIPFrameWndEx::GetContainerFrameWindow() — retail (RVA 0x96430, named
// ?GetContainerFrameWindow@COleIPFrameWndEx@@ in the map):
//     p = this->[0x210];                       // the COleIPFrameWnd base's frame
//     return (p && p->IsKindOf(&classCOleCntrFrameWndEx)) ? p : nullptr;
// The descriptor at 0x2eec00 was read out of the image: name
// "COleCntrFrameWndEx", m_nObjectSize 1352, m_wSchema 0xFFFF.
// this+0x210 is a member of the COleIPFrameWnd base that OpenMFC does not model
// (there is no COleIPFrameWnd/COleDocIPFrameWnd definition in include/openmfc/),
// so the slot cannot be read.
// Symbol: ?GetContainerFrameWindow@COleDocIPFrameWndEx@@QEAAPEAVCOleCntrFrameWndEx@@XZ
extern "C" void* MS_ABI impl__GetContainerFrameWindow_COleDocIPFrameWndEx__QEAAPEAVCOleCntrFrameWndEx__XZ(void* pThis) {
    (void)pThis;
    // TODO(clean-room): needs the container-frame member at this+0x210.
    return nullptr;
}

// COleDocIPFrameWndEx::GetPane(UINT) — retail (RVA 0x960b0, named
// ?GetPane@COleIPFrameWndEx@@ in the map):
//     add $0x428,%rcx; %r8d = 1;
//     tail-call CDockingManager vtable +0x58 -> CDockingManager::FindPaneByID
//     (0x47e20), i.e. FindPaneByID(nID, TRUE)
// The docking manager at this+0x428 is not modeled.
// Symbol: ?GetPane@COleDocIPFrameWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" void* MS_ABI impl__GetPane_COleDocIPFrameWndEx__QEAAPEAVCBasePane__I_Z(
    void* pThis, unsigned int nID) {
    (void)pThis; (void)nID;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return nullptr;
}

// COleDocIPFrameWndEx::HtmlHelp(DWORD_PTR, UINT) — retail (RVA 0x95bc0; the map
// names that entry ?HtmlHelpA@COleDocIPFrameWndEx@@, the Unicode DLL's exported
// name being the W one aliased here; slot +0x110 of this class's vftable points
// at it), transcribed in full:
//     if (dwData != 0) jmp CWnd::HtmlHelp(dwData, nCmd);      // 0x28b350
//     if (this->[0x288] != 0) jmp COleDocIPFrameWndEx::OnContextHelp (0x95c00);
//     jmp CWnd::HtmlHelp(dwData, nCmd);
// this+0x288 is the "already inside context help" flag the constructor zeroes
// and OnContextHelp sets for its duration.  This file does not read it at all:
// it always takes the CWnd branch.  (It does NOT "read as clear" — offset 0x288
// is simply unowned memory in whatever object the caller passed, and nothing
// here inspects it.)
// Symbol: ?HtmlHelpW@COleDocIPFrameWndEx@@MEAAX_KI@Z
extern "C" void MS_ABI impl__HtmlHelpW_COleDocIPFrameWndEx__MEAAX_KI_Z(
    CWnd* pThis, unsigned __int64 dwData, unsigned int nCmd) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — the this+0x288 re-entry branch
    // into OnContextHelp is not modeled.
    impl__HtmlHelpW_CWnd__UEAAX_KI_Z(pThis, dwData, nCmd);
}

// COleDocIPFrameWndEx::InsertPane(CBasePane*, CBasePane*, BOOL) — retail
// (RVA 0x95f30, named ?InsertPane@COleIPFrameWndEx@@ in the map):
//     add $0x428,%rcx; jmp CDockingManager::InsertPane   // 0x477a0
// The docking manager at this+0x428 is not modeled.
// Symbol: ?InsertPane@COleDocIPFrameWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_COleDocIPFrameWndEx__QEAAHPEAVCBasePane__0H_Z(
    void* pThis, void* pPane, void* pTarget, int bAfter) {
    (void)pThis; (void)pPane; (void)pTarget; (void)bAfter;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleDocIPFrameWndEx::IsPointNearDockSite(CPoint, DWORD&, BOOL&) const —
// retail (RVA 0x95fd0, named ?IsPointNearDockSite@COleIPFrameWndEx@@ in the
// map):
//     add $0x428,%rcx; jmp CDockingManager::IsPointNearDockSite   // 0x48160
// The docking manager at this+0x428 is not modeled.
// Symbol: ?IsPointNearDockSite@COleDocIPFrameWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_COleDocIPFrameWndEx__QEBAHVCPoint__AEAKAEAH_Z(
    const void* pThis, unsigned long long pt, unsigned long* dwBarAlignment, int* bOuterEdge) {
    (void)pThis; (void)pt; (void)dwBarAlignment; (void)bOuterEdge;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleDocIPFrameWndEx::LoadFrame(UINT, DWORD, CWnd*, CCreateContext*) — retail
// (RVA 0x95b80, named ?LoadFrame@COleIPFrameWndEx@@ in the map; slot +0x2e0 of
// this class's vftable points at it), transcribed in full:
//     this->m_Impl.[+0x10] = nIDResource;              // CFrameImpl at this+0x298
//     CFrameImpl::LoadLargeIconsState(&this->m_Impl);  // 0x64750
//     jmp CFrameWnd::LoadFrame(this, nIDResource, dwDefaultStyle,
//                              pParentWnd, pContext);  // 0x29bb90
// The CFrameImpl sub-object is not modeled; the base LoadFrame is performed.
// Symbol: ?LoadFrame@COleDocIPFrameWndEx@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__LoadFrame_COleDocIPFrameWndEx__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CFrameWnd* pThis, unsigned int nIDResource, unsigned long dwDefaultStyle,
    CWnd* pParentWnd, CCreateContext* pContext) {
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — the CFrameImpl at this+0x298
    // (default resource id + LoadLargeIconsState) is not modeled.
    return impl__LoadFrame_CFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
        pThis, nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

// COleDocIPFrameWndEx::OnActivate(UINT, CWnd*, BOOL) — retail (RVA 0x95cb0, one
// of this class's OWN bodies; reached from message-map entry 1, WM_ACTIVATE):
//     CFrameWnd::OnActivate(this, nState, pWndOther, bMinimized);   // 0x29c460
//     if (nState != 0) {
//         if (nState == 2) ::UpdateWindow(this->m_hWnd);   // IAT slot 0x2c5328
//         // remember the frame that was active BEFORE this one, then take over
//         this->[0x290] = g_pActiveFrame ? g_pActiveFrame->m_hWnd : nullptr;
//         g_pActiveFrame = this;                           // global at 0x3b6f18
//     } else {
//         CFrameImpl::DeactivateMenu(this+0x298);          // 0x646e0, unconditional
//         if (g_pActivePopupMenu)                          // global at 0x3b6fe8
//             ::SendMessage(g_pActivePopupMenu->m_hWnd, 0x10 /*WM_CLOSE*/, 0, 0);
//         if (g_pActiveFrame == this) {
//             p = CWnd::FromHandlePermanent(this->[0x290]);          // 0x2891d0
//             g_pActiveFrame = (p && p->IsKindOf(&classCFrameWnd)) ? p : nullptr;
//         }
//     }
// (The descriptor at 0x338990 was read out of the image: name "CFrameWnd",
// m_nObjectSize 472.  The SendMessage above is IAT slot 0x2c5378, which the
// import directory of this image names USER32!SendMessageA.)
// Only the base activation is reproduced.  Of the state the rest touches:
// this+0x290 and the CFrameImpl at this+0x298 are unmodeled; the module
// "active frame" pointer at 0x3b6f18 has no counterpart anywhere in this tree
// (grep over phase4/src finds only per-window CFrameWnd::GetActiveFrame); and
// while the active-popup pointer at 0x3b6fe8 IS modeled here, retail's use of
// it is ::SendMessage(popup->m_hWnd, WM_CLOSE, ...) and OpenMFC's
// CMFCPopupMenu::Create (featurepack/menu/CMFCPopupMenu.cpp) only records
// state in a side table — it never creates a window — so there is no HWND to
// post to.
// Symbol: ?OnActivate@COleDocIPFrameWndEx@@IEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_COleDocIPFrameWndEx__IEAAXIPEAVCWnd__H_Z(
    CFrameWnd* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized) {
    if (!pThis) return;
    impl__OnActivate_CFrameWnd__IEAAXIPEAVCWnd__H_Z(pThis, nState, pWndOther, bMinimized);
    // TODO(clean-room): transcribed partially — this+0x290, the CFrameImpl at
    // this+0x298 and the two module globals are not modeled.
}

// COleDocIPFrameWndEx::OnClose() — retail (RVA 0x95d60, named
// ?OnClose@COleIPFrameWndEx@@ in the map, reached from THIS class's message-map
// entry 2), transcribed in full:
//     CFrameImpl::OnCloseFrame(this+0x298);   // 0x62010
//     jmp CFrameWnd::OnClose(this);           // 0x29bf10
// The CFrameImpl sub-object is not modeled; the base close is performed.
// Symbol: ?OnClose@COleDocIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_COleDocIPFrameWndEx__IEAAXXZ(CFrameWnd* pThis) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — CFrameImpl::OnCloseFrame on the
    // sub-object at this+0x298 is not modeled.
    impl__OnClose_CFrameWnd__IEAAXXZ(pThis);
}

// COleDocIPFrameWndEx::OnClosePopupMenu(CMFCPopupMenu*) — retail (RVA 0x95ac0,
// named ?OnClosePopupMenu@COleIPFrameWndEx@@ in the map; slot +0x3f8 of this
// class's vftable points at it), transcribed in full:
//     if (g_pActivePopupMenu == pMenuPopup) g_pActivePopupMenu = nullptr;
//     this->[0x770] = 0;
// The module-global active-popup pointer at 0x3b6fe8 IS modeled in this tree
// (openmfc::detail::mfccore::g_pActivePopupMenu — see the thunk declarations at
// the top of this file), so retail's first statement is reproduced exactly.
// The DWORD at this+0x770 belongs to the unmodeled part of the frame object and
// is left alone.
// Symbol: ?OnClosePopupMenu@COleDocIPFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_COleDocIPFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(
    void* pThis, void* pMenuPopup) {
    (void)pThis;
    if (impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ() == pMenuPopup)
        impl__ActivatePopupMenu_CMFCPopupMenu__SAHPEAVCFrameWnd__PEAV1__Z(nullptr, nullptr);
    // TODO(clean-room): the `this->[0x770] = 0` half needs the frame layout.
}

// COleDocIPFrameWndEx::OnCmdMsg(UINT, int, void*, AFX_CMDHANDLERINFO*) — retail
// (RVA 0x96460, named ?OnCmdMsg@COleIPFrameWndEx@@ in the map; slot +0x028 of
// this class's vftable points at it), transcribed in full:
//     if (CFrameWnd::OnCmdMsg(this, nID, nCode, pExtra, pHandlerInfo)) // 0x29c1f0
//         return TRUE;
//     return CDockingManager::ProcessPaneContextMenuCommand(              // 0x4e870
//                this+0x428, nID, nCode, pExtra, pHandlerInfo);
// The docking manager at this+0x428 is not modeled; the base dispatch is done.
// Symbol: ?OnCmdMsg@COleDocIPFrameWndEx@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_COleDocIPFrameWndEx__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CFrameWnd* pThis, unsigned int nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    if (!pThis) return FALSE;
    if (impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
            pThis, nID, nCode, pExtra, pHandlerInfo))
        return TRUE;
    // TODO(clean-room): transcribed partially — the pane context-menu command
    // fallback needs the embedded CDockingManager at this+0x428.
    return FALSE;
}

// COleDocIPFrameWndEx::OnCommand(WPARAM, LPARAM) — retail (RVA 0x95ae0, named
// ?OnCommand@COleIPFrameWndEx@@ in the map; slot +0x1f8 of this class's vftable
// points at it), transcribed in full:
//     if (HIWORD(wParam) == 1) {                        // accelerator
//         CMFCCmdUsageCount::AddCmd(&g_usage, LOWORD(wParam));   // 0x233a0,
//                                                       // object at 0x3aafd0
//         if (CFrameImpl::ProcessKeyboard(this+0x298, 27, nullptr)) return TRUE;
//                                                       // 0x63540
//         if (g_pUserToolsManager &&                    // global at 0x3b7110
//             CUserToolsManager::InvokeTool(g_pUserToolsManager, LOWORD(wParam)))
//             return TRUE;                              // 0x1824f0
//     }
//     if (g_moduleFlag != 0) return FALSE;              // DWORD at 0x3b70bc
//     return CFrameWnd::OnCommand(this, wParam, lParam);            // 0x29b180
// The DWORD at 0x3b70bc is not named in the RVA map, but it is written only by
// ?SetCustomizeMode@CMFCToolBar@@SAHH@Z (0x150e40, `mov %esi,0x3b70bc` at
// 0x150ed4) and by CMFCToolBar::OnLButtonDown (0x14eec6/0x14eefe): it is
// CMFCToolBar's customize-mode flag, i.e. the gate is documented MFC's
// `if (CMFCToolBar::IsCustomizeMode()) return FALSE;`.  This tree has no
// implementation of ?IsCustomizeMode@CMFCToolBar@@SAHXZ (grep over phase4/src
// finds none), so the gate is left unreproduced rather than routed through a
// generated stub.  The CFrameImpl sub-object, the usage counter and the
// user-tools manager are likewise not modeled, so the accelerator fast path is
// skipped entirely and the base dispatch is performed unconditionally.
// Symbol: ?OnCommand@COleDocIPFrameWndEx@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_COleDocIPFrameWndEx__MEAAH_K_J_Z(
    CFrameWnd* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — the accelerator fast path and
    // the module flag gating the base call are not modeled.
    return impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(pThis, (WPARAM)wParam, (LPARAM)lParam);
}

// COleDocIPFrameWndEx::OnContextHelp() — retail (RVA 0x95c00, one of this
// class's OWN bodies), transcribed in full:
//     this->[0x288] = 1;
//     if (this->[0x104] == 0 && CFrameWnd::CanEnterHelpMode(this))   // 0x29fbb0
//         CMFCToolBar::SetHelpMode(TRUE);                            // 0x1554b0
//     CFrameWnd::OnContextHelp(this);                                // 0x29fc30
//     if (this->[0x104] == 0) CMFCToolBar::SetHelpMode(FALSE);
//     this->[0x288] = 0;
// The two flags (this+0x288, this+0x104) are not modeled, so neither the guard
// nor the bracketing can be evaluated; only the base help loop is performed.
// (?SetHelpMode@CMFCToolBar@@SAXH@Z IS reachable — the thunk is defined at
// featurepack/toolbar/CMFCToolBar.cpp:2773 — but its body there is empty, so
// calling it would reproduce no observable effect while implying a gate this
// file cannot actually evaluate.)
// Symbol: ?OnContextHelp@COleDocIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_COleDocIPFrameWndEx__IEAAXXZ(CFrameWnd* pThis) {
    if (!pThis) return;
    impl__OnContextHelp_CFrameWnd__QEAAXXZ(pThis);
    // TODO(clean-room): transcribed partially — the this+0x288 flag and the
    // CMFCToolBar::SetHelpMode bracketing are not modeled.
}

// COleDocIPFrameWndEx::OnCreateControlBars(CFrameWnd*, CFrameWnd*) — retail
// (RVA 0x96330, named ?OnCreateControlBars@COleIPFrameWndEx@@ in the map; slot
// +0x390 of this class's vftable points at it), transcribed in full:
//     if (pWndFrame && pWndFrame->IsKindOf(&classCOleCntrFrameWndEx))
//         return TRUE;                       // descriptor 0x2eec00
//     p = ??2@YAPEAX_K@Z(0x548);             // 0x2840
//     newFrame = p ? COleCntrFrameWndEx::COleCntrFrameWndEx(p, this) : nullptr;
//                                            // 0x94f80, takes a COleIPFrameWnd*
//     h = CWnd::Detach(this->[0x210]);       // 0x289260
//     if (this->[0x210])                     // release the old container frame
//         this->[0x210]->vtable[+0x08](this->[0x210], 1);  // deleting dtor
//     this->[0x210] = newFrame;
//     CWnd::Attach(newFrame, h);             // 0x289200
//     newFrame->[0x390] = newFrame;
//     return TRUE;
// (pWndDoc, the second parameter, is never read.)
// COleCntrFrameWndEx is an unimplemented stub class in this tree
// (core/ole/COleCntrFrameWndEx.cpp) and this+0x210 is not modeled, so only the
// return value is reproduced — retail returns 1 on every path.
// Symbol: ?OnCreateControlBars@COleDocIPFrameWndEx@@MEAAHPEAVCFrameWnd@@0@Z
extern "C" int MS_ABI impl__OnCreateControlBars_COleDocIPFrameWndEx__MEAAHPEAVCFrameWnd__0_Z(
    void* pThis, void* pWndFrame, void* pWndDoc) {
    (void)pThis; (void)pWndFrame; (void)pWndDoc;
    // TODO(clean-room): needs COleCntrFrameWndEx and this+0x210.
    return TRUE;
}

// COleDocIPFrameWndEx::OnDestroy() — retail (RVA 0x95d90, named
// ?OnDestroy@COleIPFrameWndEx@@ in the map, reached from THIS class's
// message-map entry 3):
//     if (g_pActivePopupMenu)                       // global at 0x3b6fe8
//         ::SendMessage(g_pActivePopupMenu->m_hWnd, 0x10 /*WM_CLOSE*/, 0, 0);
//     if (g_pActiveFrame == this) {                 // global at 0x3b6f18
//         p = CWnd::FromHandlePermanent(this->[0x290]);
//         g_pActiveFrame = (p && p->IsKindOf(&classCFrameWnd)) ? p : nullptr;
//     }                                             // NB: restores, not clears
//     CFrameImpl::DeactivateMenu(this+0x298);       // 0x646e0, unconditional
//     if (this->[0xf8]) { ::DestroyAcceleratorTable(this->[0xf8]);
//                         this->[0xf8] = nullptr; }
//     this->[0x430] = 0;
//     // collect the frame's child HWNDs (::GetTopWindow, then ::GetWindow with
//     // GW_HWNDNEXT off each CHILD handle) into a local CPtrList, then for each:
//     if (::IsWindow(h) && ::GetParent(h) == this->m_hWnd) ::DestroyWindow(h);
//     COleIPFrameWnd::OnDestroy(this);              // 0x25c0c0
// (Imports resolved through the PE import directory: 0x2c5378 SendMessageA,
// 0x2c4d98 DestroyAcceleratorTable, 0x2c4da0 GetTopWindow, 0x2c4c88 GetWindow,
// 0x2c5390 IsWindow, 0x2c5300 GetParent, 0x2c4da8 DestroyWindow.)
// Only that final base call is reproduced; every other step touches state this
// tree does not model — note in particular that the module "active frame"
// global is *restored* to the previously active frame, not cleared.  (The
// active-popup global at 0x3b6fe8 is the one piece that does exist here, but as
// in OnActivate above retail only uses it to ::SendMessage the popup's HWND,
// and this tree's CMFCPopupMenu never creates one.)
// COleIPFrameWnd::OnDestroy really is the base retail calls (COleDocIPFrameWnd
// does not override it); it is itself an empty stub at
// core/ole/COleIPFrameWnd.cpp:89.
// Symbol: ?OnDestroy@COleDocIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_COleDocIPFrameWndEx__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — popup/menu/accelerator teardown
    // and the child-window sweep run on unmodeled offsets and are skipped.
    impl__OnDestroy_COleIPFrameWnd__IEAAXXZ(pThis);
}

// COleDocIPFrameWndEx::OnIdleUpdateCmdUI(WPARAM, LPARAM) — retail (RVA 0x962d0,
// named ?OnIdleUpdateCmdUI@COleIPFrameWndEx@@ in the map, reached from THIS
// class's message-map entry 4), transcribed in full:
//     COleIPFrameWnd::OnIdleUpdateCmdUI(this);                    // 0x25c160
//     CDockingManager::SendMessageToMiniFrames(this+0x428, 0x363, 0, 0); // 0x4d0f0
//     p = this->[0x210];
//     if (p && p->IsKindOf(&classCOleCntrFrameWndEx))             // 0x2eec00
//         COleCntrFrameWndEx::OnIdleUpdateCmdUI(p);               // 0x95530
//     return 0;
// Neither wParam nor lParam is read.  The base call is made; the docking manager
// (this+0x428) and the container frame (this+0x210) are not modeled.
// Symbol: ?OnIdleUpdateCmdUI@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_COleDocIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam; (void)lParam;
    if (!pThis) return 0;
    impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ(pThis);
    // TODO(clean-room): transcribed partially — the mini-frame broadcast and the
    // container-frame forward need this+0x428 / this+0x210.
    return 0;
}

// COleDocIPFrameWndEx::OnMenuChar(UINT, UINT, CMenu*) — retail (RVA 0x957d0,
// named ?OnMenuChar@COleIPFrameWndEx@@ in the map, reached from THIS class's
// message-map entry 0, WM_MENUCHAR), transcribed in full:
//     if (CFrameImpl::OnMenuChar(this+0x298, nChar))    // 0x64490
//         return 0xffff0002;
//     return CFrameWnd::OnMenuChar(this, nChar, nFlags, pMenu);   // 0x29ee90
// The CFrameImpl sub-object is not modeled, so its branch never fires and the
// base handler runs.
// Symbol: ?OnMenuChar@COleDocIPFrameWndEx@@IEAA_JIIPEAVCMenu@@@Z
extern "C" __int64 MS_ABI impl__OnMenuChar_COleDocIPFrameWndEx__IEAA_JIIPEAVCMenu___Z(
    CFrameWnd* pThis, unsigned int nChar, unsigned int nFlags, CMenu* pMenu) {
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially — CFrameImpl::OnMenuChar on the
    // sub-object at this+0x298 is not modeled.
    return impl__OnMenuChar_CFrameWnd__IEAA_JIIPEAVCMenu___Z(pThis, nChar, nFlags, pMenu);
}

// COleDocIPFrameWndEx::OnMoveMiniFrame(CWnd*) — retail (RVA 0x96050, named
// ?OnMoveMiniFrame@COleIPFrameWndEx@@ in the map; slot +0x3e0 of this class's
// vftable points at it):
//     add $0x428,%rcx; tail-call CDockingManager vtable +0x88
//                      -> CDockingManager::OnMoveMiniFrame (0x490f0)
// The docking manager at this+0x428 is not modeled.
// Symbol: ?OnMoveMiniFrame@COleDocIPFrameWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_COleDocIPFrameWndEx__UEAAHPEAVCWnd___Z(
    void* pThis, void* pFrame) {
    (void)pThis; (void)pFrame;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleDocIPFrameWndEx::OnPaneCheck(UINT) — retail (RVA 0x96140, named
// ?OnPaneCheck@COleIPFrameWndEx@@ in the map, reached from THIS class's
// message-map entries 6/8/10), transcribed in full:
//     pane = CDockingManager::FindPaneByID(this+0x428, nID, TRUE);  // vtable +0x58
//     if (!pane) return 0;
//     bShow = ((CWnd::GetStyle(pane) & WS_VISIBLE) == 0);           // 0x2a75a0,
//                                       // encoded as `bt $0x1c,%eax; setae %dl`
//     pane->vtable[+0x458](pane, bShow, 0, 0);       // CBasePane::ShowPane
//     return 1;
// The lookup needs the embedded CDockingManager at this+0x428, which is not
// modeled, so the pane can never be found and retail's !pane path is what runs.
// Symbol: ?OnPaneCheck@COleDocIPFrameWndEx@@IEAAHI@Z
extern "C" int MS_ABI impl__OnPaneCheck_COleDocIPFrameWndEx__IEAAHI_Z(void* pThis, unsigned int nID) {
    (void)pThis; (void)nID;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleDocIPFrameWndEx::OnSetMenu(WPARAM, LPARAM) — retail (RVA 0x95820, named
// ?OnSetMenu@COleIPFrameWndEx@@ in the map), transcribed in full:
//     COleDocIPFrameWndEx::OnSetMenu(this, (HMENU)wParam);   // direct call to
//                                                            // 0x95860, below
//     return this->vtable[+0x248](this, 0x230 /*WM_MDISETMENU*/, 0, lParam);
// Slot +0x248 of this class's vftable was read out of the image: it holds
// ?DefWindowProcA@CWnd@@MEAA_JI_K_J@Z (0x28a000) — the Unicode build binds the W
// entry — so for a COleDocIPFrameWndEx the virtual call resolves to CWnd's own
// DefWindowProc and nothing else.  The HMENU overload IS called here, exactly
// as retail does; the trailing dispatch is not.  The reason is NOT that a
// direct call would pick a different override (for this class it would not; it
// would only differ for an application subclass that overrides DefWindowProc):
// it is that OpenMFC's ??0COleDocIPFrameWndEx@@ runs no CWnd constructor, so
// nothing here guarantees that this+0x40 holds a window handle to hand to
// ::DefWindowProc.  Retail's return value is DefWindowProc's; 0 is returned
// instead (WM_MDISETMENU is a DefFrameProc message, not one DefWindowProc
// acts on).
// Symbol: ?OnSetMenu@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetMenu_COleDocIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)lParam;
    if (!pThis) return 0;
    impl__OnSetMenu_COleDocIPFrameWndEx__IEAAHPEAUHMENU_____Z(pThis, (HMENU)(ULONG_PTR)wParam);
    // TODO(clean-room): the trailing virtual DefWindowProc(WM_MDISETMENU, 0,
    // lParam) needs a COleDocIPFrameWndEx vtable.
    return 0;
}

// COleDocIPFrameWndEx::OnSetMenu(HMENU) — retail (RVA 0x95860, named
// ?OnSetMenu@COleIPFrameWndEx@@ in the map), transcribed in full:
//     bar = this->[0x3a8];                       // the frame's menu bar pane
//     if (!bar) return 0;
//     bar->vtable[+0x878](bar, hMenu ? hMenu : this->[0x2e0], 0, 0);
//     return 1;
// Neither this+0x3a8 nor this+0x2e0 is modeled, so retail's "no menu bar"
// result is what is returned.
// Symbol: ?OnSetMenu@COleDocIPFrameWndEx@@IEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__OnSetMenu_COleDocIPFrameWndEx__IEAAHPEAUHMENU_____Z(void* pThis, HMENU hMenu) {
    (void)pThis; (void)hMenu;
    // TODO(clean-room): needs the menu-bar pane at this+0x3a8 and this+0x2e0.
    return 0;
}

// COleDocIPFrameWndEx::OnSetPreviewMode(BOOL, CPrintPreviewState*) — retail
// (RVA 0x961b0, named ?OnSetPreviewMode@COleIPFrameWndEx@@ in the map; slot
// +0x338 of this class's vftable points at it), transcribed in full:
//     CDockingManager::SetPrintPreviewMode(this+0x428, bPreview, pState); // 0x4ced0
//     saved = pState->[0x10];
//     CFrameWnd::OnSetPreviewMode(this, bPreview, pState);                // 0x29dca0
//     pState->[0x10] = saved;
//     tail-call this->vtable[+0x300](this, TRUE);   // RecalcLayout(TRUE)
// The docking manager and the class vtable are not modeled.  Only the base
// CFrameWnd::OnSetPreviewMode is performed: the pState->[0x10] save/restore is
// NOT reproduced (CPrintPreviewState's retail layout is not modeled here, and
// writing a raw offset into a caller-owned struct would be a guess), and the
// trailing virtual RecalcLayout(TRUE) is not reachable without a class vtable.
// Symbol: ?OnSetPreviewMode@COleDocIPFrameWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_COleDocIPFrameWndEx__UEAAXHPEAUCPrintPreviewState___Z(
    CFrameWnd* pThis, int bPreview, CPrintPreviewState* pState) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — CDockingManager's print-preview
    // switch (this+0x428) and the trailing virtual RecalcLayout(TRUE) are not
    // modeled, and retail's save/restore of pState->[0x10] around the base call
    // is NOT performed (that offset is not pinned by anything in this tree).
    impl__OnSetPreviewMode_CFrameWnd__UEAAXHPEAUCPrintPreviewState___Z(pThis, bPreview, pState);
}

// COleDocIPFrameWndEx::OnShowPanes(BOOL) — retail (RVA 0x963e0, named
// ?OnShowPanes@COleIPFrameWndEx@@ in the map; slot +0x3d0 of this class's
// vftable points at it), transcribed in full:
//     r = CDockingManager::ShowPanes(this+0x428, bShow);   // vtable +0xb0, 0x4d490
//     this->vtable[+0x3d8](this, nullptr);                 // AdjustDockingLayout
//     return r;
// The docking manager at this+0x428 and the class vtable are not modeled.
// Symbol: ?OnShowPanes@COleDocIPFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_COleDocIPFrameWndEx__UEAAHH_Z(void* pThis, int bShow) {
    (void)pThis; (void)bShow;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleDocIPFrameWndEx::OnToolbarContextMenu(WPARAM, LPARAM) — the export name
// has no entry in mfc140_rva_symbols.json, but THIS class's own message map
// (entries at 0x2ef0b0) resolves it: its three ON_REGISTERED_MESSAGE rows point
// at 0x3ae0, 0x95c60 (OnToolbarCreateNew) and 0x95c90 (OnToolbarDelete), in that
// order, and the body at 0x3ae0 is exactly
//     mov $0x1,%eax ; ret
// The registered-message id globals (0x3bb21c / 0x3bb234 / 0x3bb238) are not
// named in the map, but they do not have to be inferred: each is written once,
// by a one-line initialiser that calls RegisterWindowMessage (IAT 0x2c5298) on
// a literal string, and those strings were read out of the image —
//     0x1c40: 0x3bb21c = RegisterWindowMessage("TOOLBAR_POPUPMENU")  @0x33e9c8
//     0x1c80: 0x3bb234 = RegisterWindowMessage("TOOLBAR_CREATE")     @0x33e9f8
//     0x1ca0: 0x3bb238 = RegisterWindowMessage("TOOLBAR_DELETE")     @0x33ea08
// so row 11 is AFX_WM_TOOLBARMENU and rows 12/13 are AFX_WM_CREATETOOLBAR /
// AFX_WM_DELETETOOLBAR, matching the handlers named at 0x95c60 and 0x95c90.
// The `return 1` itself is read off the bytes and also matches documented
// MFC's CFrameWndEx::OnToolbarContextMenu.
// (0x3ae0 is a folded one-instruction body, which is why the map labels it
// ?OnEraseBkgnd@CPaneTrackingWnd@@ — an unrelated function with the same code.)
// Symbol: ?OnToolbarContextMenu@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarContextMenu_COleDocIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam; (void)lParam;
    return 1;
}

// COleDocIPFrameWndEx::OnToolbarCreateNew(WPARAM, LPARAM) — retail (RVA 0x95c60,
// named ?OnToolbarCreateNew@COleIPFrameWndEx@@ in the map, reached from THIS
// class's message-map entry 12), transcribed in full:
//     if (lParam == 0) AfxThrowInvalidArgException();          // 0x225b80
//     return CFrameImpl::CreateNewToolBar(this+0x298, (LPCTSTR)lParam);  // 0x62820
// The CFrameImpl sub-object is not modeled, so the toolbar cannot be created;
// the lParam==0 guard has no dependencies and is reproduced, which is how the
// rest of this tree transcribes the same retail shape (see
// featurepack/docking/CMFCAutoHideBar.cpp:261).
// Symbol: ?OnToolbarCreateNew@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarCreateNew_COleDocIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam;
    if (lParam == 0) impl__AfxThrowInvalidArgException__YAXXZ();
    // TODO(clean-room): the CMFCToolBar* result needs the CFrameImpl at
    // this+0x298; retail returns CFrameImpl::CreateNewToolBar's pointer.
    return 0;
}

// COleDocIPFrameWndEx::OnToolbarDelete(WPARAM, LPARAM) — retail (RVA 0x95c90,
// named ?OnToolbarDelete@COleIPFrameWndEx@@ in the map, reached from THIS
// class's message-map entry 13), transcribed in full:
//     return (__int64)(int)CFrameImpl::DeleteToolBar(this+0x298,
//                                                    (CMFCToolBar*)lParam);
//                                            // 0x63350, result sign-extended
// The CFrameImpl sub-object is not modeled.
// Symbol: ?OnToolbarDelete@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarDelete_COleDocIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam; (void)lParam;
    // TODO(clean-room): needs the CFrameImpl at this+0x298.
    return 0;
}

// COleDocIPFrameWndEx::PaneFromPoint(CPoint, int, bool, CRuntimeClass*) const —
// retail (RVA 0x95f60, named ?PaneFromPoint@COleIPFrameWndEx@@...H_N... in the
// map):
//     add $0x428,%rcx; tail-call CDockingManager vtable +0x30
//         -> CDockingManager::PaneFromPoint(pt, nSensitivity, bExactBar,
//                                           pRTCBarType, 0, nullptr)   // 0x47950
// (the two trailing arguments the six-parameter retail overload takes are
// written as 0 / nullptr on the stack).
// The docking manager at this+0x428 is not modeled.
// Symbol: ?PaneFromPoint@COleDocIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleDocIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(
    const void* pThis, unsigned long long pt, int nSensitivity, bool bExactBar, void* pRTCBarType) {
    (void)pThis; (void)pt; (void)nSensitivity; (void)bExactBar; (void)pRTCBarType;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return nullptr;
}

// COleDocIPFrameWndEx::PaneFromPoint(CPoint, int, DWORD&, CRuntimeClass*) const
// — retail (RVA 0x95fa0, named ?PaneFromPoint@COleIPFrameWndEx@@...HAEAK... in
// the map):
//     add $0x428,%rcx; tail-call CDockingManager vtable +0x28
//         -> CDockingManager::PaneFromPoint(pt, nSensitivity, dwAlignment,
//                                           pRTCBarType, nullptr)      // 0x47b00
// The docking manager at this+0x428 is not modeled.
// Symbol: ?PaneFromPoint@COleDocIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleDocIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(
    const void* pThis, unsigned long long pt, int nSensitivity, unsigned long* dwAlignment, void* pRTCBarType) {
    (void)pThis; (void)pt; (void)nSensitivity; (void)dwAlignment; (void)pRTCBarType;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return nullptr;
}

// COleDocIPFrameWndEx::PreCreateWindow(CREATESTRUCT&) — retail (RVA 0x96090; the
// map spells that entry ?PreCreateWindow@COleIPFrameWndEx@@...tagCREATESTRUCTA,
// the Unicode DLL's exported name being the W one aliased here, and slot +0x0d8
// of this class's vftable points at it), transcribed in full:
//     this->[0x3b8] = this + 0x428;             // cache the docking manager
//     jmp CFrameWnd::PreCreateWindow(this, cs); // 0x29b760
// The docking-manager back-pointer at this+0x3b8 is not modeled; the base
// PreCreateWindow is performed.
// Symbol: ?PreCreateWindow@COleDocIPFrameWndEx@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_COleDocIPFrameWndEx__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW* pcs) {
    if (!pThis || !pcs) return FALSE;
    // TODO(clean-room): transcribed partially — the this+0x3b8 docking-manager
    // back-pointer is not modeled.
    return impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(pThis, *pcs);
}

// COleDocIPFrameWndEx::PreTranslateMessage(MSG*) — retail (RVA 0x958b0, named
// ?PreTranslateMessage@COleIPFrameWndEx@@ in the map; slot +0x228 of this
// class's vftable points at it).  It switches on pMsg->message; the exact sets
// were read off the compare chain and are NOT contiguous ranges:
//   0x200 WM_MOUSEMOVE only -> pt = LOWORD/HIWORD(pMsg->lParam);
//         p = CWnd::FromHandle(pMsg->hwnd); if (p) ::ClientToScreen(p->m_hWnd,&pt);
//         CFrameImpl::ProcessMouseMove(this+0x298, pt)                  // 0x63f40
//         (WM_NCMOUSEMOVE 0xa0 is NOT handled)
//   0x100 WM_KEYDOWN        -> if (CFrameImpl::IsHelpKey(pMsg))         // 0x64680
//             fall through to the base; else
//             CFrameImpl::ProcessKeyboard(this+0x298, pMsg->wParam, nullptr)
//                                                                       // 0x63540
//   0xa1,0xa2,0xa4,0xa5,0xa7,0xa8 (the six NC button down/up messages; the NC
//         double-clicks 0xa3/0xa6/0xa9 are excluded) ->
//             CFrameImpl::ProcessMouseClick(this+0x298, pMsg->message,
//                 LOWORD/HIWORD(pMsg->lParam), pMsg->hwnd)              // 0x63840
//   0x201 and 0x203..0x209 (WM_LBUTTONDOWN plus WM_LBUTTONDBLCLK..WM_MBUTTONDBLCLK;
//         WM_LBUTTONUP 0x202 is NOT in the set) -> same ProcessMouseClick, but
//         preceded by CWnd::FromHandle + ::IsWindow + ::ClientToScreen on the
//         point.
// A non-zero result from any of those returns TRUE; on that last click path a
// zero result ALSO returns TRUE when ::IsWindow(pMsg->hwnd) has become false.
// Everything else falls through to CFrameWnd::PreTranslateMessage (0x29ad40).
// CFrameImpl is not modeled, so only that fall-through path is performed.
// Symbol: ?PreTranslateMessage@COleDocIPFrameWndEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_COleDocIPFrameWndEx__UEAAHPEAUtagMSG___Z(
    CFrameWnd* pThis, MSG* pMsg) {
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — the CFrameImpl mouse/keyboard
    // pre-processing at this+0x298 is not modeled.
    return impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}

// COleDocIPFrameWndEx::RecalcLayout(BOOL) — retail (RVA 0x96210, one of this
// class's OWN bodies; slot +0x300 of its vftable points at it), transcribed in
// full:
//     COleDocIPFrameWnd::RecalcLayout(this, bNotify);   // 0x2559d0
//     if (this->[0x1a0]) return;                        // re-entrancy guard
//     this->[0x1a0] = 1;
//     CDockingManager::RecalcLayout(this+0x428, bNotify);   // vtable +0x78, 0x49760
//     v = this->[0x170];
//     if (v && v->IsKindOf(&classCPreviewViewEx) && this->[0x630] != 0)
//         CWnd::SetWindowPos(v, nullptr, this->[0x5f0], this->[0x5f4],
//                            this->[0x5f8] - this->[0x5f0],
//                            this->[0x5fc] - this->[0x5f4],
//                            0x14 /*SWP_NOZORDER|SWP_NOACTIVATE*/);  // 0x2a7970
//     this->[0x1a0] = 0;                                // guard released
// (this+0x5f0..0x5fc is a RECT; the descriptor at 0x2f77f8 was read out of the
// image: name "CPreviewViewEx", m_nObjectSize 5536 — the print-preview view.)
// Unlike COleIPFrameWndEx::RecalcLayout (0x96770) this body does NOT call the
// docking manager's AdjustDockingLayout (vtable +0x80) first; the only docking
// call here is vtable +0x78.
// Only the base call is reproduced.  impl__RecalcLayout_COleDocIPFrameWnd__MEAAXH_Z
// is itself an empty stub (core/ole/COleDocIPFrameWnd.cpp:32).
// Symbol: ?RecalcLayout@COleDocIPFrameWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_COleDocIPFrameWndEx__UEAAXH_Z(void* pThis, int bNotify) {
    if (!pThis) return;
    impl__RecalcLayout_COleDocIPFrameWnd__MEAAXH_Z(pThis, bNotify);
    // TODO(clean-room): transcribed partially — the guard at this+0x1a0, the
    // docking relayout at this+0x428 and the preview-view geometry at
    // this+0x170 / this+0x5f0..0x630 are not modeled.
}

// COleDocIPFrameWndEx::RemovePaneFromDockManager(CBasePane*, BOOL, BOOL, BOOL,
// CBasePane*) — retail (RVA 0x95f40, named
// ?RemovePaneFromDockManager@COleIPFrameWndEx@@ in the map):
//     add $0x428,%rcx; jmp CDockingManager::RemovePaneFromDockManager  // 0x47810
// The docking manager at this+0x428 is not modeled.
// Symbol: ?RemovePaneFromDockManager@COleDocIPFrameWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_COleDocIPFrameWndEx__QEAAXPEAVCBasePane__HHH0_Z(
    void* pThis, void* pPane, int bDestroy, int bAdjustLayout, int bAutoHide, void* pBarReplacement) {
    (void)pThis; (void)pPane; (void)bDestroy; (void)bAdjustLayout;
    (void)bAutoHide; (void)pBarReplacement;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
}

// COleDocIPFrameWndEx::ShowPane(CBasePane*, BOOL, BOOL, BOOL) — retail
// (RVA 0x67e80, which the map names ?ShowPane@COleIPFrameWndEx@@ even though it
// sits in CFrameWndEx's code block; the body reads no member at all, so a single
// copy serves every frame class), transcribed in full:
//     pBar->vtable[+0x458](pBar, bShow, bDelay, bActivate);
// `this` (RCX on entry) is never read: the whole body is a forward to the pane's
// own virtual ShowPane(BOOL, BOOL, BOOL) at vtable byte offset 0x458.
// DEVIATION: reproduced here as a direct call to CBasePane::ShowPane — the same
// call featurepack/docking/CDockSite.cpp and CDockablePane.cpp make — which is
// non-virtual, so a pane class that overrides ShowPane gets the base behaviour
// instead; retail dispatches through the pane's own vtable.
// Symbol: ?ShowPane@COleDocIPFrameWndEx@@QEAAXPEAVCBasePane@@HHH@Z
extern "C" void MS_ABI impl__ShowPane_COleDocIPFrameWndEx__QEAAXPEAVCBasePane__HHH_Z(
    void* pThis, void* pPane, int bShow, int bDelay, int bActivate) {
    (void)pThis;
    if (!pPane) return;
    impl__ShowPane_CBasePane__UEAAXHHH_Z(pPane, bShow, bDelay, bActivate);
}

// COleDocIPFrameWndEx::ShowPopupMenu(CMFCPopupMenu*) — retail (RVA 0x95a60,
// named ?ShowPopupMenu@COleIPFrameWndEx@@ in the map), transcribed in full:
//     if (!CFrameImpl::OnShowPopupMenu(this+0x298, pMenuPopup, this)) return 0;
//                                                                   // 0x64130
//     if (pMenuPopup && pMenuPopup->[0x1670]) return 1;
//     return this->vtable[+0x3e8](this, pMenuPopup);   // the class's own
//                                                      // OnShowPopupMenu
// The CFrameImpl sub-object and the class vtable are not modeled, so retail's
// "OnShowPopupMenu said no" result is what is returned.
// Symbol: ?ShowPopupMenu@COleDocIPFrameWndEx@@IEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__ShowPopupMenu_COleDocIPFrameWndEx__IEAAHPEAVCMFCPopupMenu___Z(
    void* pThis, void* pMenuPopup) {
    (void)pThis; (void)pMenuPopup;
    // TODO(clean-room): needs the CFrameImpl at this+0x298.
    return 0;
}

// COleDocIPFrameWndEx::WinHelp(DWORD_PTR, UINT) — retail (RVA 0x95be0; the map
// names that entry ?WinHelpA@COleDocIPFrameWndEx@@, and slot +0x108 of this
// class's vftable points at it), transcribed in full:
//     if (dwData != 0) jmp CWnd::WinHelp(dwData, nCmd);       // 0x28b2c0
//     if (this->[0x288] != 0) jmp COleDocIPFrameWndEx::OnContextHelp (0x95c00);
//     jmp CWnd::WinHelp(dwData, nCmd);
// Same unmodeled this+0x288 context-help flag as HtmlHelp above; this file does
// not read it and always takes the CWnd branch.
// Symbol: ?WinHelpW@COleDocIPFrameWndEx@@UEAAX_KI@Z
extern "C" void MS_ABI impl__WinHelpW_COleDocIPFrameWndEx__UEAAX_KI_Z(
    CWnd* pThis, unsigned __int64 dwData, unsigned int nCmd) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — the this+0x288 re-entry branch
    // into OnContextHelp is not modeled.
    impl__WinHelpW_CWnd__UEAAX_KI_Z(pThis, dwData, nCmd);
}
