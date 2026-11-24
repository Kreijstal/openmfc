// COleIPFrameWndEx — OpenMFC implementation.
// Sources: olecore.cpp
//
// Behaviour below was decoded from the retail MFC exports (mangled name ->
// RVA -> objdump), not inferred; every implemented body cites its source.
// Legacy unqualified address references in the transcriptions below refer to
// mfc140.dll (ANSI twin), whose function bodies match mfc140u.dll; they are not
// mfc140u RVAs/addresses unless explicitly tagged as such.
//
// Retail layout facts established from ??0COleIPFrameWndEx@@IEAA@XZ (mfc140u
// RVA 0x95f50) and ?CreateObject@COleIPFrameWndEx@@SAPEAVCObject@@XZ (mfc140u
// RVA 0x95f00):
//   * operator new is called with 0x790 == 1936 bytes, which is exactly the
//     m_nObjectSize recorded for classCOleIPFrameWndEx in
//     phase4/src/core/ole/RuntimeClasses.cpp.
//   * the base COleIPFrameWnd constructor is called with %rcx unchanged, and
//     COleIPFrameWnd's own constructor (mfc140u RVA 0x25d010) in turn calls CFrameWnd's
//     with %rcx unchanged — so CFrameWnd, COleIPFrameWnd and COleIPFrameWndEx
//     all share offset 0.  That is what makes the base-class delegations below
//     legal: `this` may be handed straight to a CFrameWnd/CWnd impl_ thunk.
//   * the constructor then initialises members OpenMFC does not model:
//         this+0x284  DWORD  (in-context-help flag; see OnContextHelp/WinHelp)
//         this+0x290  void*
//         this+0x298  CFrameImpl (constructed with `this`)
//         this+0x428  CDockingManager
//         this+0x5d8  = this
//
// OpenMFC has no COleIPFrameWndEx class in include/openmfc/ and no shadow
// struct for it in phase4/src/detail/.  This file uses a local, statically
// pinned overlay for the retail base-frame pointer at +0x210 in
// GetContainerFrameWindow.  The CDockingManager (+0x428) and the CFrameImpl
// token (+0x298) are reached through FeaturePackInPlaceFrame and the Ipfx*
// helpers below; bodies needing other unmodeled members stay partial.

#define OPENMFC_APPCORE_IMPL

#include "detail/OleFeaturePackSupport.h"
#include "detail/MfccoreSupport.h"

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

// Our own HMENU overload, called by the WPARAM/LPARAM handler (0x95260 -> 0x952a0).
extern "C" int MS_ABI impl__OnSetMenu_COleIPFrameWndEx__IEAAHPEAUHMENU_____Z(void* pThis, HMENU hMenu);

// The retail allocation for this class (0x790 bytes, see the header comment)
// must be able to hold the OpenMFC COleIPFrameWnd that the constructor below
// builds in it — impl___0COleIPFrameWndEx__IEAA_XZ forwards to
// impl___0COleIPFrameWnd__QEAA_XZ, which is a placement new at
// core/ole/Thunks.cpp:1158.
static_assert(sizeof(COleIPFrameWnd) <= 1936,
              "OpenMFC COleIPFrameWnd must fit the retail COleIPFrameWndEx allocation");

// ---- sibling impl_ exports we delegate to (defined in other phase4/src/*.cpp)
// COleIPFrameWnd (core/ole/Thunks.cpp, core/ole/COleIPFrameWnd.cpp)
extern "C" void* MS_ABI impl___0COleIPFrameWnd__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1COleIPFrameWnd__UEAA_XZ(void* pThis);
// The three below do nothing yet; they are called anyway so that the retail
// call structure is recorded and this class picks the base behaviour up
// automatically once the base is implemented.  Their current state, checked
// symbol by symbol (grep over phase4/src):
//   * impl__OnDestroy_COleIPFrameWnd__IEAAXXZ   — empty body, defined at
//     core/ole/COleIPFrameWnd.cpp:89, declared there with NO parameters.
//   * impl__RecalcLayout_COleIPFrameWnd__UEAAXH_Z — empty body, defined at
//     core/ole/COleIPFrameWnd.cpp:118 as `(int p0)`, i.e. also no `this`.
//   * impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ — has NO definition
//     anywhere in phase4/src; ?OnIdleUpdateCmdUI@COleIPFrameWnd@@IEAAXXZ
//     carries no `// Symbol:` marker, so the export is satisfied by
//     build_phase4.sh's generated stub set.  It is already declared with a
//     `void* pThis` in phase4/src/detail/COleCntrFrameWndSupport.h:59 and
//     called that way from core/ole/COleCntrFrameWnd.cpp:53, which is the
//     convention followed here.
// (None of these carry a `weak` attribute — there is no `weak` anywhere in
// COleIPFrameWnd.cpp.)  Declaring a `this` parameter that the definition does
// not have is harmless under MS x64: `this` rides in RCX and the callee simply
// ignores it.
extern "C" void MS_ABI impl__OnDestroy_COleIPFrameWnd__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RecalcLayout_COleIPFrameWnd__UEAAXH_Z(void* pThis, int bNotify);
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
extern "C" void MS_ABI impl__SetPrintPreviewMode_CDockingManager__QEAAXHPEAUCPrintPreviewState___Z(
    CDockingManager*, int, CPrintPreviewState*);
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(
    CFrameWnd* pThis, MSG* pMsg);
// CWnd (core/window/CWnd.cpp)
extern "C" void MS_ABI impl__HtmlHelpW_CWnd__UEAAX_KI_Z(
    CWnd* pThis, unsigned __int64 dwData, unsigned int nCmd);
extern "C" void MS_ABI impl__WinHelpW_CWnd__UEAAX_KI_Z(
    CWnd* pThis, unsigned __int64 dwData, unsigned int nCmd);
// MFC exception helper (phase4/src/detail/MfcExceptionsSupport.cpp:33; also
// declared in detail/MfcExceptionsSupport.h:842).
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// CBasePane (featurepack/docking/CBasePane.cpp).  CBasePane is declared in
// openmfc/afxmfc.h, which this translation unit does not include; the pane is
// therefore carried as void* — ABI-identical for a pointer argument.  The same
// thunk (defined at featurepack/docking/CBasePane.cpp:29) is also forwarded to
// from featurepack/docking/CDockSite.cpp and CDockablePane.cpp, which declare
// it with a CBasePane* first parameter because they include afxmfc.h.
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(
    void* pPane, int bShow, int bDelay, int bActivate);
// core/runtime/CObjectTypeChecks.cpp and core/ole/RuntimeClasses.cpp.
extern "C" BOOL MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleCntrFrameWndEx__SAPEAUCRuntimeClass__XZ();

namespace {
// GetContainerFrameWindow reads this slot directly (mfc140u shared-code entry
// RVA 0x95e70, disassembled as the COleDocIPFrameWndEx twin).  The matching
// m_pMainFrame offset is also pinned in COleIPFrameWnd.cpp's RetailIPFrameTail.
struct RetailCOleIPFrameWndExPrefix {
    unsigned char prefix[0x210];
    void* m_pMainFrame;
};
static_assert(offsetof(RetailCOleIPFrameWndExPrefix, m_pMainFrame) == 0x210,
              "retail COleIPFrameWnd::m_pMainFrame @+0x210");
static_assert(sizeof(RetailCOleIPFrameWndExPrefix) == 0x218,
              "retail COleIPFrameWnd prefix through m_pMainFrame");
}

extern "C" CBasePane* AFXAPI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
    CDockingManager*, UINT, int);

using openmfc::detail::olecore::FeaturePackInPlaceFrame;

namespace {
constexpr UINT viewToolbar = 0xe800;
constexpr UINT viewStatusBar = 0xe801;
constexpr UINT viewReBar = 0xe804;
}

BEGIN_MESSAGE_MAP(FeaturePackInPlaceFrame, COleIPFrameWnd)
    ON_COMMAND_EX(viewToolbar, &FeaturePackInPlaceFrame::TogglePane)
    ON_UPDATE_COMMAND_UI(viewToolbar, &FeaturePackInPlaceFrame::UpdatePaneMenu)
    ON_COMMAND_EX(viewStatusBar, &FeaturePackInPlaceFrame::TogglePane)
    ON_UPDATE_COMMAND_UI(viewStatusBar, &FeaturePackInPlaceFrame::UpdatePaneMenu)
    ON_COMMAND_EX(viewReBar, &FeaturePackInPlaceFrame::TogglePane)
    ON_UPDATE_COMMAND_UI(viewReBar, &FeaturePackInPlaceFrame::UpdatePaneMenu)
END_MESSAGE_MAP()

CBasePane* openmfc::detail::olecore::FeaturePackInPlaceFrame::FindPane(UINT id) {
    return impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(&m_dockManager, id, TRUE);
}

BOOL openmfc::detail::olecore::FeaturePackInPlaceFrame::TogglePane(UINT id) {
    auto* pane = FindPane(id);
    if (!pane) return FALSE;
    const BOOL show = (::GetWindowLongW(pane->m_hWnd, GWL_STYLE) & WS_VISIBLE) == 0;
    impl__ShowPane_CBasePane__UEAAXHHH_Z(pane, show, FALSE, FALSE);
    return TRUE;
}

void openmfc::detail::olecore::FeaturePackInPlaceFrame::UpdatePaneMenu(CCmdUI* command) {
    if (!command) return;
    if (auto* pane = FindPane(command->m_nID))
        command->SetCheck((::GetWindowLongW(pane->m_hWnd, GWL_STYLE) & WS_VISIBLE) != 0);
    else
        command->ContinueRouting();
}

extern "C" int AFXAPI impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
    CDockingManager*, CBasePane*, int, int, int);

// Symbol: ?OnUpdatePaneMenu@COleIPFrameWndEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_COleIPFrameWndEx__IEAAXPEAVCCmdUI___Z(void* frame, CCmdUI* command) {
    if (frame) static_cast<FeaturePackInPlaceFrame*>(frame)->UpdatePaneMenu(command);
}

// Symbol: ??0COleIPFrameWndEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0COleIPFrameWndEx__IEAA_XZ(void* storage) {
    using namespace openmfc::detail::olecore;
    if (!storage) return nullptr;
    ::new (storage) FeaturePackInPlaceFrame(impl__GetThisClass_COleIPFrameWndEx__SAPEAUCRuntimeClass__XZ());
    // Retail constructs the CFrameImpl at this+0x298 with `this` (see the
    // helper notes near the top of this file).
    IpfxConstructFrameImpl(storage);
    return storage;
}

// Symbol: ??1COleIPFrameWndEx@@MEAA@XZ
extern "C" void* MS_ABI impl___1COleIPFrameWndEx__MEAA_XZ(void* pThis) {
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

// COleIPFrameWndEx::AddDockSite() — ordinal 1842 resolves to mfc140u RVA
// 0x27d0 even though the name is absent from the RVA symbol map.  The ordinal
// entry disassembles to `ret $0` (the map labels the shared address as the
// similarly named CFrameWndEx entry), so retail's behavior is a no-op; keep
// this explicitly stub-shaped body rather than claiming a nontrivial method.
// Symbol: ?AddDockSite@COleIPFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_COleIPFrameWndEx__QEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?AddPane@COleIPFrameWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_COleIPFrameWndEx__QEAAHPEAVCBasePane__H_Z(void* frame, CBasePane* pane, int select) {
    return frame && pane && impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
        &static_cast<FeaturePackInPlaceFrame*>(frame)->m_dockManager, pane, select, FALSE, FALSE);
}

// Symbol: ?AdjustDockingLayout@COleIPFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_COleIPFrameWndEx__UEAAXPEAX_Z(void* frame, void*) {
    if (frame) static_cast<FeaturePackInPlaceFrame*>(frame)->m_dockManager.RecalcLayout();
}

// Symbol: ?CreateObject@COleIPFrameWndEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_COleIPFrameWndEx__SAPEAVCObject__XZ() {
    using namespace openmfc::detail::olecore;
    auto* frame = new FeaturePackInPlaceFrame(impl__GetThisClass_COleIPFrameWndEx__SAPEAUCRuntimeClass__XZ());
    IpfxConstructFrameImpl(frame);   // as the exported constructor does
    return frame;
}

// Symbol: ?DockPane@COleIPFrameWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_COleIPFrameWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(void* frame, CBasePane* pane, UINT id, LPCRECT bounds) {
    if (frame) static_cast<FeaturePackInPlaceFrame*>(frame)->m_dockManager.DockPane(pane, id, bounds);
}

// COleIPFrameWndEx::DockPaneLeftOf(CPane*, CPane*) -- mfc140u RVA 0x95be0 (one body shared
// by both in-place frame classes):
//     add $0x428,%rcx; jmp ?DockPaneLeftOf@CDockingManager@@ (0x48430)
// Symbol: ?DockPaneLeftOf@COleIPFrameWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_COleIPFrameWndEx__QEAAHPEAVCPane__0_Z(
    void* pThis, void* pBarToDock, void* pBar) {
    return impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(
        IpfxDockManager(pThis), static_cast<CPane*>(pBarToDock), static_cast<CPane*>(pBar));
}

// Symbol: ?EnableAutoHidePanes@COleIPFrameWndEx@@QEAAHK@Z
extern "C" BOOL MS_ABI impl__EnableAutoHidePanes_COleIPFrameWndEx__QEAAHK_Z(FeaturePackInPlaceFrame* frame, DWORD sides) {
    return frame && impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(&frame->m_dockManager, sides);
}

// Symbol: ?EnableDocking@COleIPFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_COleIPFrameWndEx__QEAAHK_Z(void* frame, DWORD style) {
    return frame && static_cast<FeaturePackInPlaceFrame*>(frame)->m_dockManager.EnableDocking(style);
}

// COleIPFrameWndEx::GetContainerFrameWindow() — the exact export is at mfc140u
// RVA 0x95e70 (ordinal 5094; its code-folded map label is the
// COleDocIPFrameWndEx twin).  The ANSI exact-symbol entry is RVA 0x96430.
// Disassembly: p = this->[0x210]; return (p &&
// p->IsKindOf(&classCOleCntrFrameWndEx)) ? p : nullptr.
// The local overlay above pins +0x210, and both the IsKindOf call and runtime
// class getter are invoked through their verified impl__ thunks.
// Symbol: ?GetContainerFrameWindow@COleIPFrameWndEx@@QEAAPEAVCOleCntrFrameWndEx@@XZ
extern "C" void* MS_ABI impl__GetContainerFrameWindow_COleIPFrameWndEx__QEAAPEAVCOleCntrFrameWndEx__XZ(void* pThis) {
    void* pContainer = reinterpret_cast<RetailCOleIPFrameWndExPrefix*>(pThis)->m_pMainFrame;
    if (pContainer == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pContainer),
            impl__GetThisClass_COleCntrFrameWndEx__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return pContainer;
}

// Symbol: ?GetPane@COleIPFrameWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" void* MS_ABI impl__GetPane_COleIPFrameWndEx__QEAAPEAVCBasePane__I_Z(void* frame, UINT id) {
    return frame ? static_cast<FeaturePackInPlaceFrame*>(frame)->FindPane(id) : nullptr;
}

// COleIPFrameWndEx::HtmlHelp(DWORD_PTR, UINT) — retail mfc140.dll (ANSI) RVA 0x96610 (the RVA
// map names that entry ?HtmlHelpA@COleIPFrameWndEx@@MEAAX_KI@Z, the Unicode
// DLL's exported name being the W one aliased here):
//     if (dwData != 0 || this->[0x284] == 0) jmp CWnd::HtmlHelp(dwData, nCmd);
//     else run COleIPFrameWndEx::OnContextHelp's body (0x96650).
// this+0x284 is the "already inside context help" flag the constructor zeroes
// and OnContextHelp sets for its duration.  This file does not read it at all:
// it always takes the CWnd branch.  (It does NOT "read as clear" — offset 0x284
// is simply unowned memory in whatever object the caller actually passed, and
// nothing here inspects it.)
// Symbol: ?HtmlHelpW@COleIPFrameWndEx@@MEAAX_KI@Z
extern "C" void MS_ABI impl__HtmlHelpW_COleIPFrameWndEx__MEAAX_KI_Z(
    CWnd* pThis, unsigned __int64 dwData, unsigned int nCmd) {
    if (!pThis) return;
    impl__HtmlHelpW_CWnd__UEAAX_KI_Z(pThis, dwData, nCmd);
    // TODO(clean-room): transcribed partially — the this+0x284 re-entry branch
    // into OnContextHelp is not modeled.
}

// Symbol: ?InsertPane@COleIPFrameWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_COleIPFrameWndEx__QEAAHPEAVCBasePane__0H_Z(
    FeaturePackInPlaceFrame* frame, CBasePane* pane, CBasePane* target, BOOL after) {
    return frame && openmfc::detail::olecore::impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
        &frame->m_dockManager, pane, target, after);
}

// Symbol: ?IsPointNearDockSite@COleIPFrameWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" BOOL MS_ABI impl__IsPointNearDockSite_COleIPFrameWndEx__QEBAHVCPoint__AEAKAEAH_Z(
    const FeaturePackInPlaceFrame* frame, CPoint point, DWORD* alignment, BOOL* outerEdge) {
    return impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(
        frame ? &frame->m_dockManager : nullptr, point, alignment, outerEdge);
}

// COleIPFrameWndEx::LoadFrame(UINT, DWORD, CWnd*, CCreateContext*) — retail
// (mfc140.dll ANSI RVA 0x95b80):
//     this->m_Impl.[+0x10] = nIDResource;      // CFrameImpl at this+0x298
//     CFrameImpl::LoadLargeIconsState(&this->m_Impl);
//     jmp CFrameWnd::LoadFrame(this, nIDResource, dwDefaultStyle,
//                              pParentWnd, pContext);
// The CFrameImpl sub-object is not modeled; the base LoadFrame is performed.
// Symbol: ?LoadFrame@COleIPFrameWndEx@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__LoadFrame_COleIPFrameWndEx__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CFrameWnd* pThis, unsigned int nIDResource, unsigned long dwDefaultStyle,
    CWnd* pParentWnd, CCreateContext* pContext) {
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — the CFrameImpl at this+0x298
    // (default resource id + LoadLargeIconsState) is not modeled.
    return impl__LoadFrame_CFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
        pThis, nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

// COleIPFrameWndEx::OnActivate(UINT, CWnd*, BOOL) — retail mfc140u RVA 0x960f0:
//     CFrameWnd::OnActivate(this, nState, pWndOther, bMinimized);   // 0x29c460
//     if (nState != 0) {
//         if (nState == 2) ::UpdateWindow(this->m_hWnd);
//         // remember the frame that was active BEFORE this one, then take over
//         this->[0x290] = g_pActiveFrame ? g_pActiveFrame->m_hWnd : nullptr;
//         g_pActiveFrame = this;                       // global at 0x3b6f18
//     } else {
//         if (g_moduleFlag == 0)                       // global at 0x3b70bc,
//             CFrameImpl::DeactivateMenu(this+0x298);  // the same flag OnCommand reads
//         if (g_pActivePopupMenu)                      // global at 0x3b6fe8
//             ::SendMessage(g_pActivePopupMenu->m_hWnd, WM_CLOSE, 0, 0);
//         if (g_pActiveFrame == this) {
//             p = CWnd::FromHandlePermanent(this->[0x290]);          // 0x2891d0
//             g_pActiveFrame = (p && p->IsKindOf(&classCFrameWnd)) ? p : nullptr;
//         }
//     }
// (The descriptor at 0x338990 was read out of the mfc140.dll image: name
// "CFrameWnd", m_nObjectSize 472.  UpdateWindow uses ANSI-image IAT slot
// 0x2c5328, resolved to USER32!UpdateWindow; SendMessage uses slot 0x2c5378,
// resolved to USER32!SendMessageA in mfc140.dll and SendMessageW in mfc140u.)
// Only the base activation is reproduced; all three module
// globals and this+0x290 / the CFrameImpl at this+0x298 are unmodeled state.
// Symbol: ?OnActivate@COleIPFrameWndEx@@IEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_COleIPFrameWndEx__IEAAXIPEAVCWnd__H_Z(
    CFrameWnd* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized) {
    if (!pThis) return;
    impl__OnActivate_CFrameWnd__IEAAXIPEAVCWnd__H_Z(pThis, nState, pWndOther, bMinimized);
    // TODO(clean-room): transcribed partially — this+0x290, the CFrameImpl at
    // this+0x298 and the three module globals are not modeled.
}

// COleIPFrameWndEx::OnClose() — retail mfc140.dll (ANSI) RVA 0x95d60:
//     CFrameImpl::OnCloseFrame(this+0x298);
//     jmp CFrameWnd::OnClose(this);
// The CFrameImpl sub-object is not modeled; the base close is performed.
// Symbol: ?OnClose@COleIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_COleIPFrameWndEx__IEAAXXZ(CFrameWnd* pThis) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — CFrameImpl::OnCloseFrame on the
    // sub-object at this+0x298 is not modeled.
    impl__OnClose_CFrameWnd__IEAAXXZ(pThis);
}

// COleIPFrameWndEx::OnClosePopupMenu(CMFCPopupMenu*) -- mfc140u RVA 0x95500 (shared body),
// transcribed in full:
//     if (CMFCPopupMenu::m_pActivePopupMenu == pMenuPopup)       // 0x3be288
//         CMFCPopupMenu::m_pActivePopupMenu = NULL;
//     this->[0x770] = 0;                     // dock manager +0x348, see the helper notes
// Symbol: ?OnClosePopupMenu@COleIPFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_COleIPFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(
    void* pThis, void* pMenuPopup) {
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA == pMenuPopup)
        impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA = nullptr;
    *reinterpret_cast<DWORD*>(static_cast<unsigned char*>(pThis) + kIpfxOffPanesMenuFlag) = 0;
}

// COleIPFrameWndEx::OnCmdMsg(UINT, int, void*, AFX_CMDHANDLERINFO*) — retail
// (mfc140.dll ANSI RVA 0x96460):
//     if (CFrameWnd::OnCmdMsg(...)) return TRUE;
//     return CDockingManager::ProcessPaneContextMenuCommand(this+0x428, ...);
// The docking manager at this+0x428 is not modeled; the base dispatch is done.
// Symbol: ?OnCmdMsg@COleIPFrameWndEx@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_COleIPFrameWndEx__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CFrameWnd* pThis, unsigned int nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    if (!pThis) return FALSE;
    if (impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
            pThis, nID, nCode, pExtra, pHandlerInfo))
        return TRUE;
    // TODO(clean-room): transcribed partially — the pane context-menu command
    // fallback needs the embedded CDockingManager at this+0x428.
    return FALSE;
}

// COleIPFrameWndEx::OnCommand(WPARAM, LPARAM) — retail mfc140.dll (ANSI) RVA 0x95ae0:
//     if (HIWORD(wParam) == 1) {               // accelerator
//         CMFCCmdUsageCount::AddCmd(&g_usage, LOWORD(wParam));
//         if (CFrameImpl::ProcessKeyboard(this+0x298, 27, nullptr)) return TRUE;
//         if (g_pUserToolsManager && g_pUserToolsManager->InvokeTool(LOWORD(wParam)))
//             return TRUE;
//     }
//     return g_moduleFlag ? FALSE : CFrameWnd::OnCommand(this, wParam, lParam);
// g_moduleFlag is the DWORD at 0x3b70bc; it was not resolved to a name, and it
// is the same global OnActivate consults before CFrameImpl::DeactivateMenu.
// The CFrameImpl sub-object, the usage counter and the user-tools manager are
// not modeled; the base dispatch is performed unconditionally.
// Symbol: ?OnCommand@COleIPFrameWndEx@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_COleIPFrameWndEx__MEAAH_K_J_Z(
    CFrameWnd* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — the accelerator fast path and
    // the module flag gating the base call are not modeled.
    return impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(pThis, (WPARAM)wParam, (LPARAM)lParam);
}

// COleIPFrameWndEx::OnContextHelp() — retail mfc140u RVA 0x96090:
//     this->[0x284] = 1;
//     if (this->[0x104] == 0 && CFrameWnd::CanEnterHelpMode(this))
//         CMFCToolBar::SetHelpMode(TRUE);
//     CFrameWnd::OnContextHelp(this);
//     if (this->[0x104] == 0) CMFCToolBar::SetHelpMode(FALSE);
//     this->[0x284] = 0;
// The two flags (this+0x284, this+0x104) are not modeled and CMFCToolBar's
// help mode is not reachable from here; the base help loop is performed.
// Symbol: ?OnContextHelp@COleIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_COleIPFrameWndEx__IEAAXXZ(CFrameWnd* pThis) {
    if (!pThis) return;
    impl__OnContextHelp_CFrameWnd__QEAAXXZ(pThis);
    // TODO(clean-room): transcribed partially — the this+0x284 flag and the
    // CMFCToolBar::SetHelpMode bracketing are not modeled.
}

// Server activation installs the extended container frame before invoking this callback.
// Symbol: ?OnCreateControlBars@COleIPFrameWndEx@@MEAAHPEAVCFrameWnd@@0@Z
extern "C" int MS_ABI impl__OnCreateControlBars_COleIPFrameWndEx__MEAAHPEAVCFrameWnd__0_Z(
    void* pThis, void* pWndFrame, void* pWndDoc) {
    return pThis && static_cast<COleIPFrameWnd*>(pThis)->COleIPFrameWnd::OnCreateControlBars(
        static_cast<CFrameWnd*>(pWndFrame), static_cast<CFrameWnd*>(pWndDoc));
}

// COleIPFrameWndEx::OnDestroy() — retail mfc140.dll (ANSI) RVA 0x95d90:
//     if (g_pActivePopupMenu)                       // global at 0x3b6fe8
//         ::SendMessage(g_pActivePopupMenu->m_hWnd, WM_CLOSE, 0, 0);
//     if (g_pActiveFrame == this) {                 // global at 0x3b6f18
//         p = CWnd::FromHandlePermanent(this->[0x290]);
//         g_pActiveFrame = (p && p->IsKindOf(&classCFrameWnd)) ? p : nullptr;
//     }                                             // NB: restores, not clears
//     CFrameImpl::DeactivateMenu(this+0x298);       // 0x646e0, unconditional here
//     if (this->[0xf8]) { ::DestroyAcceleratorTable(this->[0xf8]);
//                         this->[0xf8] = nullptr; }
//     this->[0x430] = 0;
//     // collect the frame's child HWNDs (::GetTopWindow then ::GetWindow with
//     // GW_HWNDNEXT off each CHILD handle) into a local list, then for each:
//     if (::IsWindow(h) && ::GetParent(h) == this->m_hWnd) ::DestroyWindow(h);
//     COleIPFrameWnd::OnDestroy(this);              // 0x25c0c0
// (Imports resolved through the PE import directory: 0x2c5378 SendMessageA,
// 0x2c4d98 DestroyAcceleratorTable, 0x2c4da0 GetTopWindow, 0x2c4c88 GetWindow,
// 0x2c5390 IsWindow, 0x2c5300 GetParent, 0x2c4da8 DestroyWindow.)
// Only that final base call is reproduced; every other step touches state this
// tree does not model — note in particular that the module "active frame"
// global is *restored* to the previously active frame, not cleared.
// (COleIPFrameWnd::OnDestroy is itself still an empty stub at
// core/ole/COleIPFrameWnd.cpp:89.)
// Symbol: ?OnDestroy@COleIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_COleIPFrameWndEx__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — popup/menu/accelerator teardown
    // on unmodeled offsets is skipped.
    impl__OnDestroy_COleIPFrameWnd__IEAAXXZ(pThis);
}

// COleIPFrameWndEx::OnIdleUpdateCmdUI(WPARAM, LPARAM) — retail mfc140.dll (ANSI) RVA 0x962d0:
//     COleIPFrameWnd::OnIdleUpdateCmdUI(this);
//     CDockingManager::SendMessageToMiniFrames(this+0x428, 0x363, 0, 0);
//     p = this->[0x210];
//     if (p && p->IsKindOf(&classCOleCntrFrameWndEx))
//         COleCntrFrameWndEx::OnIdleUpdateCmdUI(p);
//     return 0;
// The base call is made; the docking manager (this+0x428) and the container
// frame (this+0x210) are not modeled.
// Symbol: ?OnIdleUpdateCmdUI@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_COleIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam; (void)lParam;
    if (!pThis) return 0;
    impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ(static_cast<COleIPFrameWnd*>(pThis));
    // TODO(clean-room): transcribed partially — the mini-frame broadcast and the
    // container-frame forward need this+0x428 / this+0x210.
    return 0;
}

// COleIPFrameWndEx::OnMenuChar(UINT, UINT, CMenu*) — retail mfc140.dll (ANSI) RVA 0x957d0:
//     if (CFrameImpl::OnMenuChar(this+0x298, nChar)) return 0xffff0002;
//     return CFrameWnd::OnMenuChar(this, nChar, nFlags, pMenu);
// This implementation omits the CFrameImpl call, so it always falls through
// to the base handler; retail can return 0xffff0002 from the omitted branch.
// Symbol: ?OnMenuChar@COleIPFrameWndEx@@IEAA_JIIPEAVCMenu@@@Z
extern "C" __int64 MS_ABI impl__OnMenuChar_COleIPFrameWndEx__IEAA_JIIPEAVCMenu___Z(
    CFrameWnd* pThis, unsigned int nChar, unsigned int nFlags, CMenu* pMenu) {
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially — CFrameImpl::OnMenuChar on the
    // sub-object at this+0x298 is not modeled.
    return impl__OnMenuChar_CFrameWnd__IEAA_JIIPEAVCMenu___Z(pThis, nChar, nFlags, pMenu);
}

// COleIPFrameWndEx::OnMoveMiniFrame(CWnd*) -- mfc140u RVA 0x95a90 (shared body):
//     add $0x428,%rcx; jmp *vtable[+0x88]
// Slot +0x88 of the CDockingManager vftable (0x2e6120, planted by its ctor
// 0x46ef0) is ?OnMoveMiniFrame@CDockingManager@@ (0x492a0).  The manager is the
// embedded m_dockManager, whose dynamic type is always exactly CDockingManager,
// so the direct thunk call reaches the same function.
// Symbol: ?OnMoveMiniFrame@COleIPFrameWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_COleIPFrameWndEx__UEAAHPEAVCWnd___Z(void* pThis, void* pFrame) {
    return impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(
        IpfxDockManager(pThis), static_cast<CWnd*>(pFrame));
}

// Symbol: ?OnPaneCheck@COleIPFrameWndEx@@IEAAHI@Z
extern "C" int MS_ABI impl__OnPaneCheck_COleIPFrameWndEx__IEAAHI_Z(FeaturePackInPlaceFrame* frame, UINT id) {
    return frame && frame->TogglePane(id);
}

// COleIPFrameWndEx::OnSetMenu(WPARAM, LPARAM) -- mfc140u RVA 0x95260 (shared body):
//     OnSetMenu((HMENU)wParam);                              // direct call, 0x952a0
//     return this->vtable[+0x248](WM_MDISETMENU /*0x230*/, 0, lParam);   // DefWindowProc
// For an OpenMFC-built frame slot +0x248 is CWnd::DefWindowProc (no override);
// a client-derived frame's MSVC vtable is dispatched, as retail does.
// Symbol: ?OnSetMenu@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetMenu_COleIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    impl__OnSetMenu_COleIPFrameWndEx__IEAAHPEAUHMENU_____Z(
        pThis, reinterpret_cast<HMENU>(static_cast<std::uintptr_t>(wParam)));
    if (IpfxIsOwnObject(pThis))
        return static_cast<__int64>(impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
            static_cast<IpfxFrame*>(pThis), 0x0230, 0, static_cast<LPARAM>(lParam)));
    typedef LRESULT (MS_ABI* Fn_DefWindowProc)(void*, UINT, WPARAM, LPARAM);
    return static_cast<__int64>(IpfxVSlot<Fn_DefWindowProc>(pThis, 0x248)(
        pThis, 0x0230, 0, static_cast<LPARAM>(lParam)));
}

// COleIPFrameWndEx::OnSetMenu(HMENU) -- mfc140u RVA 0x952a0 (shared body):
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
// Symbol: ?OnSetMenu@COleIPFrameWndEx@@IEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__OnSetMenu_COleIPFrameWndEx__IEAAHPEAUHMENU_____Z(void* pThis, HMENU hMenu) {
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

// Symbol: ?OnSetPreviewMode@COleIPFrameWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_COleIPFrameWndEx__UEAAXHPEAUCPrintPreviewState___Z(
    FeaturePackInPlaceFrame* frame, int preview, CPrintPreviewState* state) {
    if (frame) frame->FeaturePackInPlaceFrame::OnSetPreviewMode(preview, state);
}

void openmfc::detail::olecore::FeaturePackInPlaceFrame::OnSetPreviewMode(BOOL preview, CPrintPreviewState* state) {
    impl__SetPrintPreviewMode_CDockingManager__QEAAXHPEAUCPrintPreviewState___Z(&m_dockManager, preview, state);
    CFrameWnd::OnSetPreviewMode(preview, state);
    RecalcLayout(TRUE);
}

// Symbol: ?OnShowPanes@COleIPFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_COleIPFrameWndEx__UEAAHH_Z(FeaturePackInPlaceFrame* frame, BOOL show) {
    return frame && openmfc::detail::olecore::impl__ShowPanes_CDockingManager__UEAAHH_Z(&frame->m_dockManager, show);
}

// COleIPFrameWndEx::OnToolbarContextMenu(WPARAM, LPARAM) — export ordinal
// 11373 maps to mfc140u RVA 0x3a60; the code-folded entry is `mov $1,%eax; ret`
// (disas.py --u --at 0x3a60).  The two arguments are ignored and retail returns 1.
// Symbol: ?OnToolbarContextMenu@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarContextMenu_COleIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam; (void)lParam;
    return 1;
}

// COleIPFrameWndEx::OnToolbarCreateNew(WPARAM, LPARAM) — retail mfc140.dll (ANSI) RVA 0x95c60:
//     if (lParam == 0) AfxThrowInvalidArgException();
//     return CFrameImpl::CreateNewToolBar(this+0x298, (LPCTSTR)lParam);
// The CFrameImpl sub-object is not modeled, so the toolbar cannot be created;
// the lParam==0 guard, however, has no dependencies and is reproduced, which is
// how the rest of this tree transcribes the same retail shape (see
// featurepack/docking/CMFCAutoHideBar.cpp:261).
// Symbol: ?OnToolbarCreateNew@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarCreateNew_COleIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam;
    if (lParam == 0) impl__AfxThrowInvalidArgException__YAXXZ();
    // TODO(clean-room): the CMFCToolBar* result needs the CFrameImpl at
    // this+0x298; retail returns CFrameImpl::CreateNewToolBar's pointer.
    return 0;
}

// COleIPFrameWndEx::OnToolbarDelete(WPARAM, LPARAM) -- mfc140u RVA 0x956d0 (shared body):
//     return (LRESULT)(int)CFrameImpl::DeleteToolBar(this+0x298, (CMFCToolBar*)lParam);
//                                                   // 0x63520, `cltq` sign-extends
// Unlike CFrameWndEx's handler there is no lParam==0 guard.
// Symbol: ?OnToolbarDelete@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarDelete_COleIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    return static_cast<__int64>(impl__DeleteToolBar_CFrameImpl__IEAAHPEAVCMFCToolBar___Z(
        IpfxFrameImpl(pThis), reinterpret_cast<void*>(static_cast<std::uintptr_t>(lParam))));
}

// COleIPFrameWndEx::PaneFromPoint(CPoint, int, bool, CRuntimeClass*) const -- mfc140u RVA
// 0x959a0 (shared body): this+0x428, then vtable[+0x30] with the two extra
// stack arguments (BOOL 0, const CBasePane* nullptr) appended.  Slot +0x30 of
// the CDockingManager vftable (0x2e6120) is the six-argument
// ?PaneFromPoint@CDockingManager@@...H_N... (0x47b00); called directly for the
// same exact-type reason as OnMoveMiniFrame.
// Symbol: ?PaneFromPoint@COleIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(
    const void* pThis, unsigned long long pt, int nSensitivity, bool bExactBar, void* pRTCBarType) {
    const CPoint point(static_cast<int>(static_cast<unsigned int>(pt & 0xffffffffULL)),
                       static_cast<int>(static_cast<unsigned int>(pt >> 32)));
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
        IpfxDockManager(pThis), point, nSensitivity, bExactBar,
        static_cast<CRuntimeClass*>(pRTCBarType), FALSE, nullptr);
}

// COleIPFrameWndEx::PaneFromPoint(CPoint, int, DWORD&, CRuntimeClass*) const -- mfc140u
// RVA 0x959e0 (shared body): this+0x428, then vtable[+0x28] with a trailing
// const CBasePane* nullptr.  Slot +0x28 is ?PaneFromPoint@CDockingManager@@
// ...HAEAK... (0x47cb0); called directly (exact embedded type).
// Symbol: ?PaneFromPoint@COleIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(
    const void* pThis, unsigned long long pt, int nSensitivity, unsigned long* dwAlignment, void* pRTCBarType) {
    const CPoint point(static_cast<int>(static_cast<unsigned int>(pt & 0xffffffffULL)),
                       static_cast<int>(static_cast<unsigned int>(pt >> 32)));
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
        IpfxDockManager(pThis), point, nSensitivity, dwAlignment,
        static_cast<CRuntimeClass*>(pRTCBarType), nullptr);
}

// COleIPFrameWndEx::PreCreateWindow(CREATESTRUCT&) — retail mfc140.dll (ANSI) RVA 0x96090:
//     this->[0x3b8] = this + 0x428;             // cache the docking manager
//     jmp CFrameWnd::PreCreateWindow(this, cs);
// The docking-manager back-pointer at this+0x3b8 is not modeled; the base
// PreCreateWindow is performed.  (The RVA map spells this export with the
// tagCREATESTRUCTA suffix; the Unicode DLL's exported name is the W one aliased
// here.)
// Symbol: ?PreCreateWindow@COleIPFrameWndEx@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_COleIPFrameWndEx__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW* pcs) {
    if (!pThis || !pcs) return FALSE;
    // this+0x3b8 is m_Impl.m_pDockManager (CFrameImpl +0x120).
    openmfc_CFrameImpl_SetDockManager(IpfxFrameImpl(pThis), IpfxDockManager(pThis));
    return impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(pThis, *pcs);
}

// COleIPFrameWndEx::PreTranslateMessage(MSG*) — retail mfc140.dll (ANSI) RVA 0x958b0 switches on
// pMsg->message.  The exact sets, read off the compare chain (they are NOT
// contiguous ranges):
//   0x200 WM_MOUSEMOVE only  -> point = LOWORD/HIWORD(lParam); if
//         CWnd::FromHandle(pMsg->hwnd) then ::ClientToScreen(that->m_hWnd,&pt);
//         CFrameImpl::ProcessMouseMove(this+0x298, pt)                 // 0x63f40
//         (WM_NCMOUSEMOVE 0xa0 is NOT handled)
//   0x100 WM_KEYDOWN         -> if (CFrameImpl::IsHelpKey(pMsg)) fall through;  // 0x64680
//         else CFrameImpl::ProcessKeyboard(this+0x298, pMsg->wParam, nullptr)   // 0x63540
//   0xa1,0xa2,0xa4,0xa5,0xa7,0xa8 (the six NC button down/up messages, the
//         NC double-clicks 0xa3/0xa6/0xa9 excluded) and
//   0x201 and 0x203..0x209 (WM_LBUTTONDOWN plus WM_LBUTTONDBLCLK..WM_MBUTTONDBLCLK;
//         WM_LBUTTONUP 0x202 is NOT in the set)
//                            -> CFrameImpl::ProcessMouseClick(this+0x298,
//                                 pMsg->message, pt, pMsg->hwnd)       // 0x63840
// A non-zero result from any of those returns TRUE; on the click path a zero
// result ALSO returns TRUE when ::IsWindow(pMsg->hwnd) has become false.
// Everything else falls through to CFrameWnd::PreTranslateMessage (0x29ad40).
// The ANSI-image IAT slots used here were resolved: 0x2c5270 is
// USER32!ClientToScreen and 0x2c5390 is USER32!IsWindow.
// CFrameImpl is not modeled, so only that fall-through path is performed.
// Symbol: ?PreTranslateMessage@COleIPFrameWndEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_COleIPFrameWndEx__UEAAHPEAUtagMSG___Z(
    CFrameWnd* pThis, MSG* pMsg) {
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — the CFrameImpl mouse/keyboard
    // pre-processing at this+0x298 is not modeled.
    return impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}

// COleIPFrameWndEx::RecalcLayout(BOOL) — retail mfc140.dll (ANSI) RVA 0x96770:
//     COleIPFrameWnd::RecalcLayout(this, bNotify);
//     if (this->[0x1a0]) return;                       // re-entrancy guard
//     this->[0x1a0] = 1;
//     CDockingManager vtable 0x80 (AdjustDockingLayout)(this+0x428, 0);
//     CDockingManager vtable 0x78 (RecalcLayout)(this+0x428, bNotify);
//     v = this->[0x170];
//     if (v && v->IsKindOf(&classCPreviewViewEx) && this->[0x630] != 0)
//         CWnd::SetWindowPos(v, nullptr, this->[0x5f0], this->[0x5f4],
//                            this->[0x5f8] - this->[0x5f0],
//                            this->[0x5fc] - this->[0x5f4],
//                            SWP_NOZORDER|SWP_NOACTIVATE);   // 0x14, via 0x2a7970
//     this->[0x1a0] = 0;                                     // guard released
// (this+0x5f0..0x5fc is a RECT; the descriptor at 0x2f77f8 was read out of the
// image: name "CPreviewViewEx", m_nObjectSize 5536 — this is the print-preview
// view, not a resize bar.)
// Only the base thunk is invoked; COleIPFrameWnd::RecalcLayout is itself an
// empty stub in core/ole/COleIPFrameWnd.cpp, so no base relayout currently runs.
// Symbol: ?RecalcLayout@COleIPFrameWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_COleIPFrameWndEx__UEAAXH_Z(void* pThis, int bNotify) {
    if (!pThis) return;
    impl__RecalcLayout_COleIPFrameWnd__UEAAXH_Z(pThis, bNotify);
    // TODO(clean-room): transcribed partially — the guard at this+0x1a0, the
    // docking relayout at this+0x428 and the preview-view geometry at
    // this+0x170 / this+0x5f0..0x630 are not modeled.
}

// COleIPFrameWndEx::RemovePaneFromDockManager(CBasePane*, BOOL, BOOL, BOOL, CBasePane*) --
// mfc140u RVA 0x95980 (shared body):
//     add $0x428,%rcx; jmp ?RemovePaneFromDockManager@CDockingManager@@ (0x479c0)
// Symbol: ?RemovePaneFromDockManager@COleIPFrameWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_COleIPFrameWndEx__QEAAXPEAVCBasePane__HHH0_Z(
    void* pThis, void* pPane, int bDestroy, int bAdjustLayout, int bAutoHide, void* pBarReplacement) {
    impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
        IpfxDockManager(pThis), static_cast<CBasePane*>(pPane), bDestroy, bAdjustLayout, bAutoHide,
        static_cast<CBasePane*>(pBarReplacement));
}

// COleIPFrameWndEx::ShowPane(CBasePane*, BOOL, BOOL, BOOL) — retail
// (mfc140.dll ANSI RVA 0x67e80):
//     pBar->vtable[0x458](pBar, bShow, bDelay, bActivate);
// `this` (RCX on entry) is never read: the whole body is a forward to the
// pane's own virtual ShowPane(BOOL, BOOL, BOOL) at vtable byte offset 0x458.
// Reproduced here as a direct call to CBasePane::ShowPane — the same call
// featurepack/docking/CDockSite.cpp and CDockablePane.cpp make.  That is a
// non-virtual call, so a pane class that overrides ShowPane gets the base
// behaviour instead; retail dispatches through the pane's own vtable.
// Symbol: ?ShowPane@COleIPFrameWndEx@@QEAAXPEAVCBasePane@@HHH@Z
extern "C" void MS_ABI impl__ShowPane_COleIPFrameWndEx__QEAAXPEAVCBasePane__HHH_Z(
    void* pThis, void* pPane, int bShow, int bDelay, int bActivate) {
    (void)pThis;
    if (!pPane) return;
    impl__ShowPane_CBasePane__UEAAXHHH_Z(pPane, bShow, bDelay, bActivate);
}

// COleIPFrameWndEx::ShowPopupMenu(CMFCPopupMenu*) -- mfc140u RVA 0x954a0 (shared body):
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
// Symbol: ?ShowPopupMenu@COleIPFrameWndEx@@IEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__ShowPopupMenu_COleIPFrameWndEx__IEAAHPEAVCMFCPopupMenu___Z(
    void* pThis, void* pMenuPopup) {
    if (!impl__OnShowPopupMenu_CFrameImpl__IEAAHPEAVCMFCPopupMenu__PEAVCFrameWnd___Z(
            IpfxFrameImpl(pThis), pMenuPopup, static_cast<IpfxFrame*>(pThis)))
        return FALSE;
    if (IpfxIsOwnObject(pThis))
        return TRUE;
    typedef int (MS_ABI* Fn_OnShowPopupMenu)(void*, void*);
    return IpfxVSlot<Fn_OnShowPopupMenu>(pThis, 0x3e8)(pThis, pMenuPopup);
}

// COleIPFrameWndEx::WinHelp(DWORD_PTR, UINT) — retail mfc140.dll (ANSI) RVA 0x96630 (the RVA
// map names that entry ?WinHelpA@COleIPFrameWndEx@@UEAAX_KI@Z):
//     if (dwData != 0 || this->[0x284] == 0) jmp CWnd::WinHelp(dwData, nCmd);
//     else run COleIPFrameWndEx::OnContextHelp's body (0x96650).
// Same unmodeled this+0x284 context-help flag as HtmlHelp above; this file does
// not read it and always takes the CWnd branch.
// Symbol: ?WinHelpW@COleIPFrameWndEx@@UEAAX_KI@Z
extern "C" void MS_ABI impl__WinHelpW_COleIPFrameWndEx__UEAAX_KI_Z(
    CWnd* pThis, unsigned __int64 dwData, unsigned int nCmd) {
    if (!pThis) return;
    impl__WinHelpW_CWnd__UEAAX_KI_Z(pThis, dwData, nCmd);
    // TODO(clean-room): transcribed partially — the this+0x284 re-entry branch
    // into OnContextHelp is not modeled.
}
