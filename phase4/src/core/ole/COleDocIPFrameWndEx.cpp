// COleDocIPFrameWndEx — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, olecore.cpp
//
// Behaviour below was decoded from retail disassembly. Unless a comment
// explicitly says mfc140u, unqualified RVAs and image addresses are from
// mfc140.dll (the ANSI twin); do not treat them as mfc140u addresses. The two
// images have byte-identical bodies but different export RVAs. ?AddDockSite@
// has no name-map entry, but its export ordinal resolves to the same bare-ret
// body as CFrameWndEx::AddDockSite. The other exceptional symbol,
// ?OnToolbarContextMenu@, has no name in the map and was located through this
// class's message map instead.
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
// (RVA 0x956c0, mfc140.dll), ??1 (0x95770, mfc140.dll) and ?CreateObject@
// (0x95670, mfc140.dll)
// ---------------------------------------------------------------------------
//   * operator new is called with 0x790 == 1936 bytes, exactly the
//     m_nObjectSize recorded for classCOleDocIPFrameWndEx in
//     phase4/src/core/ole/RuntimeClasses.cpp:244.
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
// phase4/src/detail/.  The CDockingManager (+0x428) and the CFrameImpl token
// (+0x298) are reached through FeaturePackInPlaceFrame and the Ipfx* helpers
// below; the remaining members above are still unmodeled.

#define OPENMFC_APPCORE_IMPL

#include "detail/OleFeaturePackSupport.h"
#include "detail/MfccoreSupport.h"

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/OlecoreSupport.h"

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <new>
#include "detail/CbarcoreSupport.h"

// ---- Feature Pack sub-objects of the retail in-place frame -----------------
// Both retail frames construct a CFrameImpl at this+0x298 and a
// CDockingManager at this+0x428 (??0COleIPFrameWndEx@@ mfc140u 0x95f50:
// `lea 0x298(%rbx),%rcx; mov %rbx,%rdx; call ??0CFrameImpl (0x61ef0)` and
// `lea 0x428(%rbx),%rcx; call ??0CDockingManager (0x46ef0)`).  The dock
// manager is the real m_dockManager of FeaturePackInPlaceFrame; OpenMFC's
// CFrameImpl is an address-keyed token (core/frame/CFrameImpl.cpp), so the
// token is this+0x298, inside the frame's otherwise unused m_frameStorage.
extern "C" void* MS_ABI impl___0CFrameImpl__QEAA_PEAVCFrameWnd___Z(void* pThis, CFrameWnd* pFrame);
extern "C" void MS_ABI impl___1CFrameImpl__UEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__DeleteToolBar_CFrameImpl__IEAAHPEAVCMFCToolBar___Z(void* pThis, void* pToolBar);
extern "C" int MS_ABI impl__OnShowPopupMenu_CFrameImpl__IEAAHPEAVCMFCPopupMenu__PEAVCFrameWnd___Z(
    void* pThis, void* pPopupMenu, CFrameWnd* pWnd);
extern "C" void MS_ABI openmfc_CFrameImpl_SetDockManager(void* pFrameImpl, CDockingManager* pDockManager);
extern "C" int MS_ABI impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(
    CDockingManager* pThis, CPane* pBarToDock, CPane* pBar);
extern "C" int MS_ABI impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(CDockingManager* pThis, CWnd* pFrame);
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
    CDockingManager* pThis, CBasePane* pPane, int bDestroy, int bAdjustLayout, int bAutoHide,
    CBasePane* pBarReplacement);
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(CWnd*, UINT, WPARAM, LPARAM);
extern "C" void MS_ABI impl__CreateFromMenu_CMFCMenuBar__UEAAXPEAUHMENU____HH_Z(
    CMFCMenuBar* pThis, HMENU hMenu, int bDefaultMenu, int bForceUpdate);
// ?m_pActivePopupMenu@CMFCPopupMenu@@ (mfc140u 0x3be288), the global retail's
// OnClosePopupMenu compares against; CFrameImpl::OnShowPopupMenu writes it.
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {
using IpfxFrame = openmfc::detail::olecore::FeaturePackInPlaceFrame;
constexpr std::size_t kIpfxOffFrameImpl = 0x298;
// OnClosePopupMenu (mfc140u 0x95514): `movl $0x0,0x770(%rcx)`, i.e. the
// retail dock manager's +0x348 (CFrameImpl::OnShowPopupMenu sets it to 1 after
// building the panes menu).  OpenMFC's CDockingManager is 0x90 bytes, so the
// dword lies in FeaturePackInPlaceFrame::m_managerPadding.
constexpr std::size_t kIpfxOffPanesMenuFlag = 0x770;
static_assert(offsetof(IpfxFrame, m_frameStorage) <= kIpfxOffFrameImpl &&
              kIpfxOffFrameImpl < offsetof(IpfxFrame, m_dockManager),
              "CFrameImpl token @+0x298 lies in the unused frame storage");
static_assert(offsetof(IpfxFrame, m_managerPadding) <= kIpfxOffPanesMenuFlag &&
              kIpfxOffPanesMenuFlag + sizeof(DWORD) <= offsetof(IpfxFrame, type_),
              "+0x770 lies in the dock manager padding");

void* IpfxFrameImpl(void* pThis) { return static_cast<unsigned char*>(pThis) + kIpfxOffFrameImpl; }
CDockingManager* IpfxDockManager(const void* pThis) {
    return &static_cast<IpfxFrame*>(const_cast<void*>(pThis))->m_dockManager;
}
void IpfxConstructFrameImpl(void* pThis) {
    impl___0CFrameImpl__QEAA_PEAVCFrameWnd___Z(IpfxFrameImpl(pThis),
                                               static_cast<IpfxFrame*>(pThis));
}
// The vptr's home image tells an OpenMFC-built object (C++ vtable, retail slot
// indices do not apply) from a client-derived one (MSVC vtable laid out per the
// retail header) -- the test core/frame/CFrameImpl.cpp's IsOwnObject applies.
bool IpfxIsOwnObject(const void* pObject) {
    const void* vptr = *static_cast<const void* const*>(pObject);
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* p = static_cast<const unsigned char*>(vptr);
    return p >= base && p < base + nt->OptionalHeader.SizeOfImage;
}
template <typename Fn> Fn IpfxVSlot(const void* pObject, std::size_t byteOffset) {
    return reinterpret_cast<Fn>((*static_cast<void* const* const*>(pObject))[byteOffset / sizeof(void*)]);
}
CMFCMenuBar* IpfxMenuBar(void* pThis) {
    // CFrameImpl::m_pMenuBar (+0x110 of the impl == frame +0x3a8) lives in
    // FrameImplState::menuBar, written by CFrameImpl::SetMenuBar.
    using namespace openmfc::detail::cbarcore;
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    auto it = g_frameImplStates.find(reinterpret_cast<CFrameImpl*>(IpfxFrameImpl(pThis)));
    return it != g_frameImplStates.end() ? static_cast<CMFCMenuBar*>(it->second.menuBar) : nullptr;
}
}  // namespace

namespace {
// Minimal local view for the inherited COleIPFrameWnd::m_pMainFrame pointer.
// GetContainerFrameWindow reads this slot at +0x210 (mfc140u RVA 0x95e70).
struct S_COleDocIPFrameWndExContainerFrameView {
    unsigned char _prefix[0x210];
    void* m_pMainFrame;
};
static_assert(offsetof(S_COleDocIPFrameWndExContainerFrameView, m_pMainFrame) == 0x210,
              "retail COleIPFrameWnd::m_pMainFrame @ 0x210");
static_assert(sizeof(S_COleDocIPFrameWndExContainerFrameView) == 0x218,
              "container-frame view reaches through +0x217");

struct S_COleDocIPFrameWndExDockingView {
    unsigned char _prefix[0x428];
    unsigned char m_dockManager[0x368];
};
static_assert(offsetof(S_COleDocIPFrameWndExDockingView, m_dockManager) == 0x428,
              "retail COleDocIPFrameWndEx CDockingManager @ 0x428");
static_assert(sizeof(S_COleDocIPFrameWndExDockingView) == 0x790,
              "retail COleDocIPFrameWndEx size 0x790");

struct S_CCmdUIRoutingLayout {
    void* vfptr;
    UINT m_nID;
    UINT m_nIndex;
    void* m_pMenu;
    void* m_pSubMenu;
    void* m_pOther;
    BOOL m_bEnableChanged;
    BOOL m_bContinueRouting;
    UINT m_nIndexMax;
    DWORD _cmdUIAlignment;
    void* m_pParentMenu;
};
static_assert(offsetof(S_CCmdUIRoutingLayout, m_bContinueRouting) == 0x2c,
              "CCmdUI::m_bContinueRouting @ 0x2c");
static_assert(sizeof(S_CCmdUIRoutingLayout) == sizeof(CCmdUI),
              "local CCmdUI layout matches the public declaration");
}  // namespace

extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleCntrFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
    void* pDockManager, unsigned int nID, int bSearchMiniFrames);
extern "C" DWORD MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);

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


// Symbol: ??0COleDocIPFrameWndEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0COleDocIPFrameWndEx__IEAA_XZ(void* storage) {
    using namespace openmfc::detail::olecore;
    if (!storage) return nullptr;
    ::new (storage) FeaturePackInPlaceFrame(impl__GetThisClass_COleDocIPFrameWndEx__SAPEAUCRuntimeClass__XZ());
    // Retail constructs the CFrameImpl at this+0x298 with `this` (see the
    // helper notes near the top of this file).
    IpfxConstructFrameImpl(storage);
    return storage;
}

using openmfc::detail::olecore::FeaturePackInPlaceFrame;
extern "C" int AFXAPI impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
    CDockingManager*, CBasePane*, int, int, int);

// Symbol: ?OnUpdatePaneMenu@COleDocIPFrameWndEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_COleDocIPFrameWndEx__IEAAXPEAVCCmdUI___Z(void* frame, CCmdUI* command) {
    if (frame) static_cast<FeaturePackInPlaceFrame*>(frame)->UpdatePaneMenu(command);
}

// Symbol: ??1COleDocIPFrameWndEx@@MEAA@XZ
extern "C" void* MS_ABI impl___1COleDocIPFrameWndEx__MEAA_XZ(void* pThis) {
    using openmfc::detail::olecore::FeaturePackInPlaceFrame;
    if (pThis) {
        // Retail destroys the dock manager, then the CFrameImpl at +0x298
        // (??1CFrameImpl, 0x620b0), then the base.  DEVIATION: the CFrameImpl
        // token is released first here, because the C++ destructor below
        // tears down the dock manager and the base together.
        impl___1CFrameImpl__UEAA_XZ(IpfxFrameImpl(pThis));
        static_cast<FeaturePackInPlaceFrame*>(pThis)->~FeaturePackInPlaceFrame();
    }
    return pThis;
}

// COleDocIPFrameWndEx::AddDockSite() — the name is absent from both symbol
// maps, but export ordinal 1841 resolves to RVA 0x27d0 (mfc140u), shared with
// CFrameWndEx::AddDockSite. disas.py --u --at 0x27d0 shows a bare `ret`, so the
// empty behavior is established rather than unknown (the source body still
// counts as a stub under the campaign's empty-body metric).
// Symbol: ?AddDockSite@COleDocIPFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_COleDocIPFrameWndEx__QEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?AddPane@COleDocIPFrameWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_COleDocIPFrameWndEx__QEAAHPEAVCBasePane__H_Z(void* frame, CBasePane* pane, int select) {
    return frame && pane && impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
        &static_cast<FeaturePackInPlaceFrame*>(frame)->m_dockManager, pane, select, FALSE, FALSE);
}

// Symbol: ?AdjustDockingLayout@COleDocIPFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_COleDocIPFrameWndEx__UEAAXPEAX_Z(void* frame, void*) {
    if (frame) static_cast<FeaturePackInPlaceFrame*>(frame)->m_dockManager.RecalcLayout();
}

// Symbol: ?CreateObject@COleDocIPFrameWndEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_COleDocIPFrameWndEx__SAPEAVCObject__XZ() {
    using namespace openmfc::detail::olecore;
    auto* frame = new FeaturePackInPlaceFrame(impl__GetThisClass_COleDocIPFrameWndEx__SAPEAUCRuntimeClass__XZ());
    IpfxConstructFrameImpl(frame);   // as the exported constructor does
    return frame;
}

// Symbol: ?DockPane@COleDocIPFrameWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_COleDocIPFrameWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(void* frame, CBasePane* pane, UINT id, LPCRECT bounds) {
    if (frame) static_cast<FeaturePackInPlaceFrame*>(frame)->m_dockManager.DockPane(pane, id, bounds);
}

// COleDocIPFrameWndEx::DockPaneLeftOf(CPane*, CPane*) -- mfc140u RVA 0x95be0 (one body shared
// by both in-place frame classes):
//     add $0x428,%rcx; jmp ?DockPaneLeftOf@CDockingManager@@ (0x48430)
// Symbol: ?DockPaneLeftOf@COleDocIPFrameWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_COleDocIPFrameWndEx__QEAAHPEAVCPane__0_Z(
    void* pThis, void* pBarToDock, void* pBar) {
    return impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(
        IpfxDockManager(pThis), static_cast<CPane*>(pBarToDock), static_cast<CPane*>(pBar));
}

// Symbol: ?EnableAutoHidePanes@COleDocIPFrameWndEx@@QEAAHK@Z
extern "C" BOOL MS_ABI impl__EnableAutoHidePanes_COleDocIPFrameWndEx__QEAAHK_Z(FeaturePackInPlaceFrame* frame, DWORD sides) {
    return frame && impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(&frame->m_dockManager, sides);
}

// Symbol: ?EnableDocking@COleDocIPFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_COleDocIPFrameWndEx__QEAAHK_Z(void* frame, DWORD style) {
    return frame && static_cast<FeaturePackInPlaceFrame*>(frame)->m_dockManager.EnableDocking(style);
}

// COleDocIPFrameWndEx::GetContainerFrameWindow() — retail mfc140u RVA 0x95e70:
//     p = this->[0x210];
//     return (p && p->IsKindOf(RUNTIME_CLASS(COleCntrFrameWndEx))) ? p : nullptr;
// The +0x210 load, null test, and IsKindOf call are visible in this entry's
// disassembly. The local view above pins the pointer slot without pretending
// that include/openmfc declares COleDocIPFrameWndEx or models the inherited
// COleIPFrameWnd::m_pMainFrame member.
// Symbol: ?GetContainerFrameWindow@COleDocIPFrameWndEx@@QEAAPEAVCOleCntrFrameWndEx@@XZ
extern "C" void* MS_ABI impl__GetContainerFrameWindow_COleDocIPFrameWndEx__QEAAPEAVCOleCntrFrameWndEx__XZ(void* pThis) {
    if (!pThis) return nullptr;
    void* pFrame = reinterpret_cast<S_COleDocIPFrameWndExContainerFrameView*>(pThis)->m_pMainFrame;
    if (!pFrame) return nullptr;
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
               static_cast<const CObject*>(pFrame),
               impl__GetThisClass_COleCntrFrameWndEx__SAPEAUCRuntimeClass__XZ())
        ? pFrame
        : nullptr;
}

// Symbol: ?GetPane@COleDocIPFrameWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" void* MS_ABI impl__GetPane_COleDocIPFrameWndEx__QEAAPEAVCBasePane__I_Z(void* frame, UINT id) {
    return frame ? static_cast<FeaturePackInPlaceFrame*>(frame)->FindPane(id) : nullptr;
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

// Symbol: ?InsertPane@COleDocIPFrameWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_COleDocIPFrameWndEx__QEAAHPEAVCBasePane__0H_Z(
    FeaturePackInPlaceFrame* frame, CBasePane* pane, CBasePane* target, BOOL after) {
    return frame && openmfc::detail::olecore::impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
        &frame->m_dockManager, pane, target, after);
}

// Symbol: ?IsPointNearDockSite@COleDocIPFrameWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" BOOL MS_ABI impl__IsPointNearDockSite_COleDocIPFrameWndEx__QEBAHVCPoint__AEAKAEAH_Z(
    const FeaturePackInPlaceFrame* frame, CPoint point, DWORD* alignment, BOOL* outerEdge) {
    return impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(
        frame ? &frame->m_dockManager : nullptr, point, alignment, outerEdge);
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

// COleDocIPFrameWndEx::OnClosePopupMenu(CMFCPopupMenu*) -- mfc140u RVA 0x95500 (shared body),
// transcribed in full:
//     if (CMFCPopupMenu::m_pActivePopupMenu == pMenuPopup)       // 0x3be288
//         CMFCPopupMenu::m_pActivePopupMenu = NULL;
//     this->[0x770] = 0;                     // dock manager +0x348, see the helper notes
// Symbol: ?OnClosePopupMenu@COleDocIPFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_COleDocIPFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(
    void* pThis, void* pMenuPopup) {
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA == pMenuPopup)
        impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA = nullptr;
    *reinterpret_cast<DWORD*>(static_cast<unsigned char*>(pThis) + kIpfxOffPanesMenuFlag) = 0;
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

// Server activation installs the extended container frame before invoking this callback.
// Symbol: ?OnCreateControlBars@COleDocIPFrameWndEx@@MEAAHPEAVCFrameWnd@@0@Z
extern "C" int MS_ABI impl__OnCreateControlBars_COleDocIPFrameWndEx__MEAAHPEAVCFrameWnd__0_Z(
    void* pThis, void* pWndFrame, void* pWndDoc) {
    return pThis && static_cast<COleIPFrameWnd*>(pThis)->COleIPFrameWnd::OnCreateControlBars(
        static_cast<CFrameWnd*>(pWndFrame), static_cast<CFrameWnd*>(pWndDoc));
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
    impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ(static_cast<COleIPFrameWnd*>(pThis));
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

// COleDocIPFrameWndEx::OnMoveMiniFrame(CWnd*) -- mfc140u RVA 0x95a90 (shared body):
//     add $0x428,%rcx; jmp *vtable[+0x88]
// Slot +0x88 of the CDockingManager vftable (0x2e6120, planted by its ctor
// 0x46ef0) is ?OnMoveMiniFrame@CDockingManager@@ (0x492a0).  The manager is the
// embedded m_dockManager, whose dynamic type is always exactly CDockingManager,
// so the direct thunk call reaches the same function.
// Symbol: ?OnMoveMiniFrame@COleDocIPFrameWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_COleDocIPFrameWndEx__UEAAHPEAVCWnd___Z(void* pThis, void* pFrame) {
    return impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(
        IpfxDockManager(pThis), static_cast<CWnd*>(pFrame));
}

// Symbol: ?OnPaneCheck@COleDocIPFrameWndEx@@IEAAHI@Z
extern "C" int MS_ABI impl__OnPaneCheck_COleDocIPFrameWndEx__IEAAHI_Z(FeaturePackInPlaceFrame* frame, UINT id) {
    return frame && frame->TogglePane(id);
}

// COleDocIPFrameWndEx::OnSetMenu(WPARAM, LPARAM) -- mfc140u RVA 0x95260 (shared body):
//     OnSetMenu((HMENU)wParam);                              // direct call, 0x952a0
//     return this->vtable[+0x248](WM_MDISETMENU /*0x230*/, 0, lParam);   // DefWindowProc
// For an OpenMFC-built frame slot +0x248 is CWnd::DefWindowProc (no override);
// a client-derived frame's MSVC vtable is dispatched, as retail does.
// Symbol: ?OnSetMenu@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetMenu_COleDocIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    impl__OnSetMenu_COleDocIPFrameWndEx__IEAAHPEAUHMENU_____Z(
        pThis, reinterpret_cast<HMENU>(static_cast<std::uintptr_t>(wParam)));
    if (IpfxIsOwnObject(pThis))
        return static_cast<__int64>(impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
            static_cast<IpfxFrame*>(pThis), 0x0230, 0, static_cast<LPARAM>(lParam)));
    typedef LRESULT (MS_ABI* Fn_DefWindowProc)(void*, UINT, WPARAM, LPARAM);
    return static_cast<__int64>(IpfxVSlot<Fn_DefWindowProc>(pThis, 0x248)(
        pThis, 0x0230, 0, static_cast<LPARAM>(lParam)));
}

// COleDocIPFrameWndEx::OnSetMenu(HMENU) -- mfc140u RVA 0x952a0 (shared body):
//     CMFCMenuBar* bar = this->[0x3a8];          // m_Impl.m_pMenuBar (impl +0x110)
//     if (!bar) return FALSE;
//     bar->vtable[+0x878](hMenu ? hMenu : this->[0x2e0], FALSE, FALSE);   // CreateFromMenu
//     return TRUE;                               // this+0x2e0 = m_Impl +0x48
// The menu bar is read from the CFrameImpl token's FrameImplState (what
// CFrameImpl::SetMenuBar writes).  Slot +0x878 is CreateFromMenu (CMFCMenuBar
// vtable slot 271); it is dispatched through the vtable for a client-built bar
// and through the CMFCMenuBar thunk for an OpenMFC-built one.
// DEVIATION: OpenMFC's CFrameImpl::SetMenuBar does not cache the frame's
// default HMENU (retail impl +0x48), so with hMenu == NULL there is no menu to
// rebuild from and the CreateFromMenu call is skipped (TRUE is still returned,
// as retail does once a menu bar exists).
// Symbol: ?OnSetMenu@COleDocIPFrameWndEx@@IEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__OnSetMenu_COleDocIPFrameWndEx__IEAAHPEAUHMENU_____Z(void* pThis, HMENU hMenu) {
    CMFCMenuBar* pMenuBar = IpfxMenuBar(pThis);
    if (pMenuBar == nullptr)
        return FALSE;
    if (hMenu != nullptr) {
        if (IpfxIsOwnObject(pMenuBar)) {
            impl__CreateFromMenu_CMFCMenuBar__UEAAXPEAUHMENU____HH_Z(pMenuBar, hMenu, FALSE, FALSE);
        } else {
            typedef void (MS_ABI* Fn_CreateFromMenu)(void*, HMENU, int, int);
            IpfxVSlot<Fn_CreateFromMenu>(pMenuBar, 0x878)(pMenuBar, hMenu, FALSE, FALSE);
        }
    }
    return TRUE;
}

// Symbol: ?OnSetPreviewMode@COleDocIPFrameWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_COleDocIPFrameWndEx__UEAAXHPEAUCPrintPreviewState___Z(
    FeaturePackInPlaceFrame* frame, int preview, CPrintPreviewState* state) {
    if (frame) frame->FeaturePackInPlaceFrame::OnSetPreviewMode(preview, state);
}

// Symbol: ?OnShowPanes@COleDocIPFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_COleDocIPFrameWndEx__UEAAHH_Z(FeaturePackInPlaceFrame* frame, BOOL show) {
    return frame && openmfc::detail::olecore::impl__ShowPanes_CDockingManager__UEAAHH_Z(&frame->m_dockManager, show);
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

// COleDocIPFrameWndEx::OnToolbarDelete(WPARAM, LPARAM) -- mfc140u RVA 0x956d0 (shared body):
//     return (LRESULT)(int)CFrameImpl::DeleteToolBar(this+0x298, (CMFCToolBar*)lParam);
//                                                   // 0x63520, `cltq` sign-extends
// Unlike CFrameWndEx's handler there is no lParam==0 guard.
// Symbol: ?OnToolbarDelete@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarDelete_COleDocIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    return static_cast<__int64>(impl__DeleteToolBar_CFrameImpl__IEAAHPEAVCMFCToolBar___Z(
        IpfxFrameImpl(pThis), reinterpret_cast<void*>(static_cast<std::uintptr_t>(lParam))));
}

// COleDocIPFrameWndEx::PaneFromPoint(CPoint, int, bool, CRuntimeClass*) const -- mfc140u RVA
// 0x959a0 (shared body): this+0x428, then vtable[+0x30] with the two extra
// stack arguments (BOOL 0, const CBasePane* nullptr) appended.  Slot +0x30 of
// the CDockingManager vftable (0x2e6120) is the six-argument
// ?PaneFromPoint@CDockingManager@@...H_N... (0x47b00); called directly for the
// same exact-type reason as OnMoveMiniFrame.
// Symbol: ?PaneFromPoint@COleDocIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleDocIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(
    const void* pThis, unsigned long long pt, int nSensitivity, bool bExactBar, void* pRTCBarType) {
    const CPoint point(static_cast<int>(static_cast<unsigned int>(pt & 0xffffffffULL)),
                       static_cast<int>(static_cast<unsigned int>(pt >> 32)));
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
        IpfxDockManager(pThis), point, nSensitivity, bExactBar,
        static_cast<CRuntimeClass*>(pRTCBarType), FALSE, nullptr);
}

// COleDocIPFrameWndEx::PaneFromPoint(CPoint, int, DWORD&, CRuntimeClass*) const -- mfc140u
// RVA 0x959e0 (shared body): this+0x428, then vtable[+0x28] with a trailing
// const CBasePane* nullptr.  Slot +0x28 is ?PaneFromPoint@CDockingManager@@
// ...HAEAK... (0x47cb0); called directly (exact embedded type).
// Symbol: ?PaneFromPoint@COleDocIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleDocIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(
    const void* pThis, unsigned long long pt, int nSensitivity, unsigned long* dwAlignment, void* pRTCBarType) {
    const CPoint point(static_cast<int>(static_cast<unsigned int>(pt & 0xffffffffULL)),
                       static_cast<int>(static_cast<unsigned int>(pt >> 32)));
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
        IpfxDockManager(pThis), point, nSensitivity, dwAlignment,
        static_cast<CRuntimeClass*>(pRTCBarType), nullptr);
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
    // this+0x3b8 is m_Impl.m_pDockManager (CFrameImpl +0x120).
    openmfc_CFrameImpl_SetDockManager(IpfxFrameImpl(pThis), IpfxDockManager(pThis));
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

// COleDocIPFrameWndEx::RemovePaneFromDockManager(CBasePane*, BOOL, BOOL, BOOL, CBasePane*) --
// mfc140u RVA 0x95980 (shared body):
//     add $0x428,%rcx; jmp ?RemovePaneFromDockManager@CDockingManager@@ (0x479c0)
// Symbol: ?RemovePaneFromDockManager@COleDocIPFrameWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_COleDocIPFrameWndEx__QEAAXPEAVCBasePane__HHH0_Z(
    void* pThis, void* pPane, int bDestroy, int bAdjustLayout, int bAutoHide, void* pBarReplacement) {
    impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
        IpfxDockManager(pThis), static_cast<CBasePane*>(pPane), bDestroy, bAdjustLayout, bAutoHide,
        static_cast<CBasePane*>(pBarReplacement));
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

// COleDocIPFrameWndEx::ShowPopupMenu(CMFCPopupMenu*) -- mfc140u RVA 0x954a0 (shared body):
//     if (!CFrameImpl::OnShowPopupMenu(this+0x298, pMenuPopup, this)) return FALSE;  // 0x64300
//     if (pMenuPopup && pMenuPopup->[+0x1670]) return TRUE;
//     return this->vtable[+0x3e8](pMenuPopup);   // virtual OnShowPopupMenu
// Slot +0x3e8 is the inline virtual OnShowPopupMenu (afxoleipframewndex.h:153,
// default `return TRUE`).  A client-derived frame carries an MSVC vtable, so the
// slot is dispatched; an OpenMFC-built frame has no override, so the inline
// default's TRUE is returned.
// DEVIATION: the popup +0x1670 gate is omitted, as in CFrameWndEx::ShowPopupMenu:
// OpenMFC's own CMFCPopupMenu allocation is 0x238 bytes, so that read would be
// out of bounds and nothing in OpenMFC maintains the field.
// Symbol: ?ShowPopupMenu@COleDocIPFrameWndEx@@IEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__ShowPopupMenu_COleDocIPFrameWndEx__IEAAHPEAVCMFCPopupMenu___Z(
    void* pThis, void* pMenuPopup) {
    if (!impl__OnShowPopupMenu_CFrameImpl__IEAAHPEAVCMFCPopupMenu__PEAVCFrameWnd___Z(
            IpfxFrameImpl(pThis), pMenuPopup, static_cast<IpfxFrame*>(pThis)))
        return FALSE;
    if (IpfxIsOwnObject(pThis))
        return TRUE;
    typedef int (MS_ABI* Fn_OnShowPopupMenu)(void*, void*);
    return IpfxVSlot<Fn_OnShowPopupMenu>(pThis, 0x3e8)(pThis, pMenuPopup);
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
