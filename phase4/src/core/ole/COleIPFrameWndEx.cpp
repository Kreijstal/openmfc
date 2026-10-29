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
// GetContainerFrameWindow; the embedded CFrameImpl / CDockingManager are still
// not modeled, so bodies requiring those sub-objects are left as documented
// stubs or partial implementations rather than guessed at.

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include <cstddef>

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
extern "C" void MS_ABI impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ(void* pThis);
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

// COleIPFrameWndEx::OnUpdatePaneMenu(CCmdUI*) — retail mfc140.dll (ANSI) RVA 0x960d0:
//     pane = CDockingManager vtable slot 0x58 (FindPaneByID)(this+0x428,
//                                                pCmdUI->[+0x08], TRUE);
//     if (pane) pCmdUI->vtable[+0x08](pCmdUI, (CWnd::GetStyle(pane) >> 28) & 1);
//     else      pCmdUI->[+0x2c] = 1;
// Retail never calls CCmdUI::Enable here — the pane branch calls the CCmdUI
// virtual at vtable byte offset 8 with the pane's WS_VISIBLE bit (0x10000000,
// hence the >>28 &1), and the no-pane branch writes 1 to the DWORD at
// pCmdUI+0x2c.  What was disassembled is the byte offset; naming that slot
// SetCheck comes from documented MFC's declaration order
// (Enable/SetCheck/SetRadio/SetText), not from the binary.
// Documented MFC has CFrameWndEx::OnUpdatePaneMenu call pCmdUI->ContinueRouting()
// on the no-pane branch, which is consistent with a m_bContinueRouting flag
// living at +0x2c, but this file did not verify that naming — the write itself
// is what was disassembled.
// This thunk reproduces neither branch: the embedded CDockingManager at
// this+0x428 and a retail-layout CCmdUI are not available to this implementation.
// OpenMFC's CCmdUI (include/openmfc/afxole.h:68) is not the retail layout — it has a virtual
// destructor in vtable slot 0 and its offset 0x2c falls inside m_pSubMenu, so
// both the SetCheck dispatch and the +0x2c write would hit the wrong thing.
// Symbol: ?OnUpdatePaneMenu@COleIPFrameWndEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_COleIPFrameWndEx__IEAAXPEAVCCmdUI___Z(void* pThis, CCmdUI* pCmdUI) {
    (void)pThis; (void)pCmdUI;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428 and a
    // retail-layout CCmdUI.
}
// COleIPFrameWndEx::COleIPFrameWndEx() — retail mfc140u RVA 0x95f50:
//     COleIPFrameWnd::COleIPFrameWnd(this);   // %rcx unchanged
//     *(void**)this = &COleIPFrameWndEx_vftable;
//     this->[0x284] = 0; this->[0x290] = nullptr;
//     CFrameImpl::CFrameImpl(this+0x298, this);
//     CDockingManager::CDockingManager(this+0x428);
//     this->[0x5d8] = this;
//     return this;                            // `mov %rbx,%rax`
// Only the base construction is reproduced: OpenMFC models neither a
// COleIPFrameWndEx vtable nor the CFrameImpl / CDockingManager sub-objects, so
// the object ends up identifying as a COleIPFrameWnd.  Returning `this` (rather
// than the previous stub's nullptr) is required by the MSVC constructor ABI.
// Symbol: ??0COleIPFrameWndEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0COleIPFrameWndEx__IEAA_XZ(void* pThis) {
    if (!pThis) return nullptr;
    impl___0COleIPFrameWnd__QEAA_XZ(pThis);
    // TODO(clean-room): transcribed partially — the derived vptr and the
    // members at this+0x284/0x290/0x298/0x428/0x5d8 are not modeled.
    return pThis;
}

// COleIPFrameWndEx::~COleIPFrameWndEx() — retail mfc140u RVA 0x96000:
//     *(void**)this = &COleIPFrameWndEx_vftable;
//     CDockingManager::~CDockingManager(this+0x428);
//     CFrameImpl::~CFrameImpl(this+0x298);
//     jmp COleIPFrameWnd::~COleIPFrameWnd(this);
// Mirrors the base teardown only; the two sub-objects are not modeled, so
// nothing is destroyed for them (nothing was constructed either).
// Symbol: ??1COleIPFrameWndEx@@MEAA@XZ
extern "C" void* MS_ABI impl___1COleIPFrameWndEx__MEAA_XZ(void* pThis) {
    if (!pThis) return nullptr;
    impl___1COleIPFrameWnd__UEAA_XZ(pThis);
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

// COleIPFrameWndEx::AddPane(CBasePane*, BOOL) — retail mfc140.dll (ANSI) RVA 0x95f10:
//     %rcx += 0x428; [rsp+0x20] = 0; %r9d = 0
//     CDockingManager::AddPane(&m_dockManager, pPane, bSelect, FALSE, FALSE)
// The embedded CDockingManager at this+0x428 is not modeled — see the header
// comment — so no docking manager can be reached from here.
// Symbol: ?AddPane@COleIPFrameWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_COleIPFrameWndEx__QEAAHPEAVCBasePane__H_Z(void* pThis, void* pPane, int bSelect) {
    (void)pThis; (void)pPane; (void)bSelect;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleIPFrameWndEx::AdjustDockingLayout(void*) — retail mfc140.dll (ANSI) RVA 0x95fe0:
//     w = ::GetWindow(this->m_hWnd, GW_CHILD);             // IAT slot 0x2c4c88
//     while ((p = CWnd::FromHandle(w)) != nullptr &&       // 0x289180
//            p->IsKindOf(&classCBasePane))                 // 0x233310
//         w = ::GetWindow(this->m_hWnd, GW_HWNDNEXT);      // note: m_hWnd, not w
//     tail-call (this+0x428)->vtable[+0x80](this+0x428, pParam);
// IAT slot 0x2c4c88 was resolved through the PE import directory: it is
// USER32.dll!GetWindow; the nCmd values 5 and 2 are GW_CHILD / GW_HWNDNEXT.
// The CRuntimeClass at 0x2da490 was read straight out of the image: name
// "CBasePane", m_nObjectSize 424, m_wSchema 0xFFFF — i.e. classCBasePane.
// The loop advances off `this->m_hWnd` rather than the child handle, so after
// the first iteration it re-reads the frame's own next sibling forever; that
// is what the bytes do (retail bug preserved, not a transcription slip — the
// analogous walk in OnDestroy below does advance off the child handle).
// The docking manager at this+0x428 is not modeled.
// Symbol: ?AdjustDockingLayout@COleIPFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_COleIPFrameWndEx__UEAAXPEAX_Z(void* pThis, void* pParam) {
    (void)pThis; (void)pParam;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
}

// COleIPFrameWndEx::CreateObject() — retail mfc140u RVA 0x95f00 allocates
// 0x790 bytes and, when non-null, calls the constructor at RVA 0x95f50.  The
// constructor implemented here only constructs the COleIPFrameWnd base; it does
// not install the derived vptr or initialize the CFrameImpl / CDockingManager
// subobjects that retail initializes.  Returning that allocation would expose
// an invalid derived instance, so this remains an explicit null stub and does
// not reproduce retail's successful-allocation path.
// Symbol: ?CreateObject@COleIPFrameWndEx@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_COleIPFrameWndEx__SAPEAVCObject__XZ() {
    return nullptr;
}

// COleIPFrameWndEx::DockPane(CBasePane*, UINT, LPCRECT) — retail mfc140.dll (ANSI) RVA 0x95f50:
//     add $0x428,%rcx; jmp CDockingManager::DockPane
// The docking manager at this+0x428 is not modeled.
// Symbol: ?DockPane@COleIPFrameWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_COleIPFrameWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    void* pThis, void* pPane, unsigned int nDockBarID, const RECT* lpRect) {
    (void)pThis; (void)pPane; (void)nDockBarID; (void)lpRect;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
}

// COleIPFrameWndEx::DockPaneLeftOf(CPane*, CPane*) — retail mfc140.dll (ANSI) RVA 0x961a0:
//     add $0x428,%rcx; jmp CDockingManager::DockPaneLeftOf
// The docking manager at this+0x428 is not modeled.
// Symbol: ?DockPaneLeftOf@COleIPFrameWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_COleIPFrameWndEx__QEAAHPEAVCPane__0_Z(
    void* pThis, void* pBarToDock, void* pBar) {
    (void)pThis; (void)pBarToDock; (void)pBar;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleIPFrameWndEx::EnableAutoHidePanes(DWORD) — retail mfc140.dll (ANSI) RVA 0x96080:
//     add $0x428,%rcx; jmp CDockingManager::EnableAutoHidePanes
// The docking manager at this+0x428 is not modeled.
// Symbol: ?EnableAutoHidePanes@COleIPFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_COleIPFrameWndEx__QEAAHK_Z(void* pThis, unsigned long dwAlignment) {
    (void)pThis; (void)dwAlignment;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleIPFrameWndEx::EnableDocking(DWORD) — retail mfc140.dll (ANSI) RVA 0x96070:
//     add $0x428,%rcx; jmp CDockingManager::EnableDocking
// The docking manager at this+0x428 is not modeled.
// Symbol: ?EnableDocking@COleIPFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_COleIPFrameWndEx__QEAAHK_Z(void* pThis, unsigned long dwDockStyle) {
    (void)pThis; (void)dwDockStyle;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
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

// COleIPFrameWndEx::GetPane(UINT) — retail mfc140.dll (ANSI) RVA 0x960b0:
//     add $0x428,%rcx; %r8d = 1; tail-call CDockingManager vtable slot 0x58
//     (FindPaneByID(nID, TRUE))
// The docking manager at this+0x428 is not modeled.
// Symbol: ?GetPane@COleIPFrameWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" void* MS_ABI impl__GetPane_COleIPFrameWndEx__QEAAPEAVCBasePane__I_Z(void* pThis, unsigned int nID) {
    (void)pThis; (void)nID;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return nullptr;
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

// COleIPFrameWndEx::InsertPane(CBasePane*, CBasePane*, BOOL) — retail
// (mfc140.dll ANSI RVA 0x95f30): add $0x428,%rcx; jmp CDockingManager::InsertPane
// The docking manager at this+0x428 is not modeled.
// Symbol: ?InsertPane@COleIPFrameWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_COleIPFrameWndEx__QEAAHPEAVCBasePane__0H_Z(
    void* pThis, void* pPane, void* pTarget, int bAfter) {
    (void)pThis; (void)pPane; (void)pTarget; (void)bAfter;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleIPFrameWndEx::IsPointNearDockSite(CPoint, DWORD&, BOOL&) — retail
// (mfc140.dll ANSI RVA 0x95fd0): add $0x428,%rcx; jmp CDockingManager::IsPointNearDockSite
// The docking manager at this+0x428 is not modeled.
// Symbol: ?IsPointNearDockSite@COleIPFrameWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_COleIPFrameWndEx__QEBAHVCPoint__AEAKAEAH_Z(
    const void* pThis, unsigned long long pt, unsigned long* dwBarAlignment, int* bOuterEdge) {
    (void)pThis; (void)pt; (void)dwBarAlignment; (void)bOuterEdge;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
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

// COleIPFrameWndEx::OnClosePopupMenu(CMFCPopupMenu*) — retail mfc140.dll (ANSI) RVA 0x95ac0:
//     if (g_pActivePopupMenu == pMenuPopup) g_pActivePopupMenu = nullptr;
//     this->[0x770] = 0;
// Both the module-global active-popup pointer (0x3b6fe8) and the DWORD at
// this+0x770 are unmodeled state.
// Symbol: ?OnClosePopupMenu@COleIPFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_COleIPFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(
    void* pThis, void* pMenuPopup) {
    (void)pThis; (void)pMenuPopup;
    // TODO(clean-room): needs the active-popup global and this+0x770.
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

// COleIPFrameWndEx::OnCreateControlBars(CFrameWnd*, CFrameWnd*) — retail
// (mfc140.dll ANSI RVA 0x96330): if pWndFrame is not already a COleCntrFrameWndEx it allocates
// one (operator new(0x548), COleCntrFrameWndEx::COleCntrFrameWndEx(pWndFrame)),
// moves the HWND from the old this+0x210 frame to it via CWnd::Detach/Attach,
// releases the old one through its vtable slot 1, stores the new frame at
// this+0x210 and sets newFrame->[0x390] = newFrame. Normal completion returns
// 1; the allocation result is not checked before later uses, so allocation
// failure is not a successful TRUE-return path.
// A partial COleCntrFrameWndEx constructor thunk exists in
// core/ole/COleCntrFrameWndEx.cpp (it omits the retail derived-vftable store),
// and this file pins the +0x210 slot locally. This body remains return-only.
// Symbol: ?OnCreateControlBars@COleIPFrameWndEx@@MEAAHPEAVCFrameWnd@@0@Z
extern "C" int MS_ABI impl__OnCreateControlBars_COleIPFrameWndEx__MEAAHPEAVCFrameWnd__0_Z(
    void* pThis, void* pWndFrame, void* pWndDoc) {
    (void)pThis; (void)pWndFrame; (void)pWndDoc;
    // TODO(clean-room): needs COleCntrFrameWndEx and this+0x210.
    return TRUE;   // retail's normal return value
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
    impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ(pThis);
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

// COleIPFrameWndEx::OnMoveMiniFrame(CWnd*) — retail mfc140.dll (ANSI) RVA 0x96050:
//     add $0x428,%rcx; tail-call CDockingManager vtable slot 0x88
// The docking manager at this+0x428 is not modeled.
// Symbol: ?OnMoveMiniFrame@COleIPFrameWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_COleIPFrameWndEx__UEAAHPEAVCWnd___Z(void* pThis, void* pFrame) {
    (void)pThis; (void)pFrame;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleIPFrameWndEx::OnPaneCheck(UINT) — retail mfc140.dll (ANSI) RVA 0x96140:
//     pane = CDockingManager vtable slot 0x58 (FindPaneByID)(this+0x428, nID, TRUE);
//     if (!pane) return 0;
//     pane->ShowPane(!(pane->GetStyle() & WS_VISIBLE), FALSE, FALSE);   // slot 0x458
//     return 1;
// This stub omits the lookup; its unconditional 0 matches retail only when
// FindPaneByID returns null.
// Symbol: ?OnPaneCheck@COleIPFrameWndEx@@IEAAHI@Z
extern "C" int MS_ABI impl__OnPaneCheck_COleIPFrameWndEx__IEAAHI_Z(void* pThis, unsigned int nID) {
    (void)pThis; (void)nID;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
}

// COleIPFrameWndEx::OnSetMenu(WPARAM, LPARAM) — retail mfc140.dll (ANSI) RVA 0x95820:
//     COleIPFrameWndEx::OnSetMenu(this, (HMENU)wParam);   // the overload below
//     return this->vtable[0x248](this, 0x230, 0, lParam);
// Both halves are out of reach: the HMENU overload depends on the menu bar at
// this+0x3a8 (see below) and the trailing dispatch is through a vtable this
// tree does not build for the class.
// Symbol: ?OnSetMenu@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetMenu_COleIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam; (void)lParam;
    // TODO(clean-room): needs this+0x3a8 and a COleIPFrameWndEx vtable.
    return 0;
}

// COleIPFrameWndEx::OnSetMenu(HMENU) — retail mfc140.dll (ANSI) RVA 0x95860:
//     bar = this->[0x3a8];                       // the frame's menu bar pane
//     if (!bar) return 0;
//     bar->vtable[0x878](bar, hMenu ? hMenu : this->[0x2e0], 0, 0);
//     return 1;
// Neither this+0x3a8 nor this+0x2e0 is modeled; the stub's zero result only
// matches retail when the menu-bar pointer is null.
// Symbol: ?OnSetMenu@COleIPFrameWndEx@@IEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__OnSetMenu_COleIPFrameWndEx__IEAAHPEAUHMENU_____Z(void* pThis, HMENU hMenu) {
    (void)pThis; (void)hMenu;
    // TODO(clean-room): needs the menu-bar pane at this+0x3a8 and this+0x2e0.
    return 0;   // retail's "no menu bar" result
}

// COleIPFrameWndEx::OnSetPreviewMode(BOOL, CPrintPreviewState*) — retail
// (mfc140.dll ANSI RVA 0x961b0):
//     CDockingManager::SetPrintPreviewMode(this+0x428, bPreview, pState);
//     saved = pState->[0x10];
//     CFrameWnd::OnSetPreviewMode(this, bPreview, pState);
//     pState->[0x10] = saved;
//     tail-call this->vtable[0x300](this, TRUE);      // RecalcLayout(TRUE)
// The docking manager and the class vtable are not modeled.  Only the base
// CFrameWnd::OnSetPreviewMode is performed: the pState->[0x10] save/restore is
// NOT reproduced (CPrintPreviewState's retail layout is not modeled here, and
// writing a raw offset into a caller-owned struct would be a guess), and the
// trailing virtual RecalcLayout(TRUE) is not reachable without a class vtable.
// Symbol: ?OnSetPreviewMode@COleIPFrameWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_COleIPFrameWndEx__UEAAXHPEAUCPrintPreviewState___Z(
    CFrameWnd* pThis, int bPreview, CPrintPreviewState* pState) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — CDockingManager's print-preview
    // switch (this+0x428) and the trailing virtual RecalcLayout(TRUE) are not
    // modeled, and retail's save/restore of pState->[0x10] around the base call
    // is NOT performed (that offset is not pinned by anything in this tree).
    impl__OnSetPreviewMode_CFrameWnd__UEAAXHPEAUCPrintPreviewState___Z(pThis, bPreview, pState);
}

// COleIPFrameWndEx::OnShowPanes(BOOL) — retail mfc140.dll (ANSI) RVA 0x963e0:
//     r = CDockingManager vtable slot 0xb0 (ShowPanes)(this+0x428, bShow);
//     this->vtable[0x3d8](this, 0);              // AdjustDockingLayout(nullptr)
//     return r;
// The docking manager at this+0x428 and the class vtable are not modeled.
// Symbol: ?OnShowPanes@COleIPFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_COleIPFrameWndEx__UEAAHH_Z(void* pThis, int bShow) {
    (void)pThis; (void)bShow;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return 0;
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

// COleIPFrameWndEx::OnToolbarDelete(WPARAM, LPARAM) — retail mfc140.dll (ANSI) RVA 0x95c90:
//     return (int)CFrameImpl::DeleteToolBar(this+0x298, (CMFCToolBar*)lParam);
// The CFrameImpl sub-object is not modeled.
// Symbol: ?OnToolbarDelete@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarDelete_COleIPFrameWndEx__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam; (void)lParam;
    // TODO(clean-room): needs the CFrameImpl at this+0x298.
    return 0;
}

// COleIPFrameWndEx::PaneFromPoint(CPoint, int, bool, CRuntimeClass*) — retail
// (mfc140.dll ANSI RVA 0x95f60): add $0x428,%rcx; tail-call CDockingManager vtable slot 0x30
// with the extra arguments (..., 0, nullptr) appended.
// The docking manager at this+0x428 is not modeled.
// Symbol: ?PaneFromPoint@COleIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(
    const void* pThis, unsigned long long pt, int nSensitivity, bool bExactBar, void* pRTCBarType) {
    (void)pThis; (void)pt; (void)nSensitivity; (void)bExactBar; (void)pRTCBarType;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return nullptr;
}

// COleIPFrameWndEx::PaneFromPoint(CPoint, int, DWORD&, CRuntimeClass*) — retail
// (mfc140.dll ANSI RVA 0x95fa0): add $0x428,%rcx; tail-call CDockingManager vtable slot 0x28
// with a trailing nullptr argument.
// The docking manager at this+0x428 is not modeled.
// Symbol: ?PaneFromPoint@COleIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(
    const void* pThis, unsigned long long pt, int nSensitivity, unsigned long* dwAlignment, void* pRTCBarType) {
    (void)pThis; (void)pt; (void)nSensitivity; (void)dwAlignment; (void)pRTCBarType;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
    return nullptr;
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
    // TODO(clean-room): transcribed partially — the this+0x3b8 docking-manager
    // back-pointer is not modeled.
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

// COleIPFrameWndEx::RemovePaneFromDockManager(CBasePane*, BOOL, BOOL, BOOL,
// CBasePane*) — retail mfc140.dll (ANSI) RVA 0x95f40:
//     add $0x428,%rcx; jmp CDockingManager::RemovePaneFromDockManager
// The docking manager at this+0x428 is not modeled.
// Symbol: ?RemovePaneFromDockManager@COleIPFrameWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_COleIPFrameWndEx__QEAAXPEAVCBasePane__HHH0_Z(
    void* pThis, void* pPane, int bDestroy, int bAdjustLayout, int bAutoHide, void* pBarReplacement) {
    (void)pThis; (void)pPane; (void)bDestroy; (void)bAdjustLayout;
    (void)bAutoHide; (void)pBarReplacement;
    // TODO(clean-room): needs the embedded CDockingManager at this+0x428.
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

// COleIPFrameWndEx::ShowPopupMenu(CMFCPopupMenu*) — retail mfc140.dll (ANSI) RVA 0x95a60:
//     if (!CFrameImpl::OnShowPopupMenu(this+0x298, pMenuPopup, this)) return 0;
//     if (pMenuPopup && pMenuPopup->[0x1670]) return 1;
//     return this->vtable[0x3e8](this, pMenuPopup);     // OnShowPopupMenu
// The CFrameImpl sub-object and the class vtable are not modeled.
// Symbol: ?ShowPopupMenu@COleIPFrameWndEx@@IEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__ShowPopupMenu_COleIPFrameWndEx__IEAAHPEAVCMFCPopupMenu___Z(
    void* pThis, void* pMenuPopup) {
    (void)pThis; (void)pMenuPopup;
    // TODO(clean-room): needs the CFrameImpl at this+0x298.
    return 0;
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
