// CPaneDialog — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// =============================================================================
// Every body below is transcribed from the retail export. RVAs are cited for
// BOTH images where they differ: the bodies are byte-identical between
// mfc140.dll (the ANSI twin disas.py reads) and mfc140u.dll, but every export
// sits at a different RVA in the two. Where only the mfc140u RVA is given, the
// symbol has no ANSI twin under that name (the _W-mangled overloads) or its
// mfc140u address was resolved by ordinal (urva.py) and re-disassembled.
//
// Retail layout (afxpanedialog.h:31, `class CPaneDialog : public CDockablePane`):
//   +0x000  CDockablePane subobject   (retail sizeof(CDockablePane) == 0x4E0)
//   +0x4E0  CSize m_sizeDefault       -- ctor RVA 0xaa9d0 (mfc140u) stores 8 zero
//                                        bytes here (`mov %rax,0x4e0(%rbx)`, rax = 0)
//   +0x4E8  _AFX_OCC_DIALOG_INFO* m_pOccDialogInfo
//                                     -- ctor zeroes it; SetOccDialogInfo
//                                        (0xaac60, mfc140u) stores it;
//                                        HandleInitDialog (0xaabf0) reads it
//   sizeof == 0x4F0                   -- CreateObject (0xaa960, mfc140u)
//                                        allocates `mov $0x4f0,%ecx`; the
//                                        CRuntimeClass in docking/RuntimeClasses.cpp
//                                        carries the same 1264.
// OpenMFC does not declare CPaneDialog at all, so the two own members are
// addressed at their retail offsets on a void* this (see PaneDialogField).
// OpenMFC's CDockablePane (include/openmfc/afxmfc.h) is a mingw class whose
// sizeof is at most 0x4E0 (static_assert below), so +0x4E0..+0x4EF lie
// past its storage but inside the 0x4F0-byte object every allocator of a
// CPaneDialog reserves -- they overlap nothing OpenMFC's base writes.
//
// The CWnd / CBasePane / CPane members these bodies touch are declared by
// OpenMFC at their retail offsets (afxmfc.h, afxwin.h) and are pinned below
// with static_asserts against the offsets the retail code dereferences. The
// one exception is CWnd::m_hWndOwner (+0xa0), which OpenMFC's CWnd does not
// name: it lies inside _cwnd_padding2 and is written by raw offset (the
// static_assert below only pins that it falls inside that padding).
//
// Virtual calls. OpenMFC has no CPaneDialog vftable -- the object carries the
// mingw CDockablePane vptr the base ctor thunk installs -- so each retail
// `call *slot(%rax)` on `this` is devirtualised to the body retail's own
// CPaneDialog vftable holds in that slot (read with vt.py / uvt.py off the
// vftable at mfc140u 0x1802f64b8, mfc140 0x1802f4408; both agree). A client
// subclass overriding one of those virtuals is therefore not consulted; each
// site names the slot and the body it calls instead.
// =============================================================================

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <atomic>
#include <cstddef>
#include <cstring>

// ---- sibling impl__ exports called below (definitions seen at the paths given) ----
// featurepack/docking/Thunks.cpp:1121 / :1146
extern "C" void* MS_ABI impl___0CDockablePane__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CDockablePane__UEAA_XZ(void* pThis);
// featurepack/docking/CDockablePane.cpp:477 (declared exactly as defined there)
extern "C" int MS_ABI impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__VCSize__HIKKK_Z(
    CDockablePane* pThis, const wchar_t* lpszCaption, CWnd* pParentWnd, CSize sizeDefault,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle,
    unsigned long dwControlBarStyle);
// featurepack/docking/CDockablePane.cpp:1530 / :2008 / :2057 / :2673. The by-value
// CPoint is one 8-byte INTEGER-class argument under the MS x64 ABI; those
// definitions spell it `void*`, which travels in the same register.
extern "C" int MS_ABI impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(
    CDockablePane* pThis, void* point, int bDetectCaption);
extern "C" void MS_ABI impl__OnLButtonDblClk_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nFlags, void* point);
extern "C" void MS_ABI impl__OnLButtonDown_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nFlags, void* point);
extern "C" void MS_ABI impl__OnUpdateCmdUI_CDockablePane__UEAAXPEAVCFrameWnd__H_Z(
    CDockablePane* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler);
// featurepack/docking/CBasePane.cpp:991 / :841
extern "C" __int64 MS_ABI impl__HandleInitDialog_CBasePane__IEAA_J_K_J_Z(
    CBasePane* pThis, unsigned __int64 wParam, __int64 lParam);
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis);
// featurepack/docking/Thunks.cpp:1208
extern "C" void* MS_ABI impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
    CBasePane* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy,
    unsigned int nFlags, void* hdwp);
// core/window/Thunks.cpp:1183 / :1378 / :1388 / :1773
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" int MS_ABI impl__LoadDynamicLayoutResource_CWnd__IEAAHPEB_W_Z(CWnd* pThis, const wchar_t* p0);
// detail/MemcoreSupport.cpp:5 -- MFC's ::operator new (??2@YAPEAX_K@Z)
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
// core/runtime/AFX_GLOBAL_DATA.cpp:656; the object is featurepack/CMFC_misc_stubs.cpp:3652
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
// featurepack/CMFC_misc_stubs.cpp:3695 -- ?g_pTopLevelFrame@@3PEAVCFrameWnd@@EA
extern "C" void* impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA;
// featurepack/CMFC_misc_stubs.cpp:519 -- the manager AfxEnableControlContainer
// recorded; OpenMFC's stand-in for AFX_MODULE_STATE::m_pOccManager (+0xa0).
extern "C" void* g4_g_pOccManager;

// ---- retail offsets these bodies dereference, pinned against OpenMFC's declarations ----
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at +0x40");
static_assert(offsetof(CDC, m_hDC) == 0x8, "retail reads pDC->m_hDC at +0x8");
static_assert(offsetof(CBasePane, m_dwControlBarStyle) == 0x108, "slot 118 body reads +0x108");
static_assert(offsetof(CBasePane, m_lpszBarTemplateName) == 0x140, "Create/HandleInitDialog use +0x140");
static_assert(offsetof(CBasePane, m_sizeDialog) == 0x148, "Create reads cx/cy at +0x148/+0x14c");
static_assert(offsetof(CPane, m_sizeMin) == 0x200, "slot 158 (GetMinSize) body reads +0x200");
static_assert(offsetof(WINDOWPOS, hwndInsertAfter) == 0x8 && offsetof(WINDOWPOS, cx) == 0x18 &&
              offsetof(WINDOWPOS, cy) == 0x1c, "OnWindowPosChanging offsets");

namespace {

constexpr std::size_t kSizeofPaneDialog      = 0x4F0;  // CreateObject: mov $0x4f0,%ecx
constexpr std::size_t kOffSizeDefault        = 0x4E0;  // CSize m_sizeDefault
constexpr std::size_t kOffOccDialogInfo      = 0x4E8;  // _AFX_OCC_DIALOG_INFO* m_pOccDialogInfo
constexpr std::size_t kOffHWndOwner          = 0xA0;   // CWnd::m_hWndOwner (inline SetOwner)
constexpr std::size_t kOffGlobalDataGate     = 0x000;  // afxGlobalData init gate
constexpr std::size_t kOffGlobalBrBtnFace    = 0x0B0;  // afxGlobalData.brBtnFace.m_hObject
static_assert(offsetof(CWnd, _cwnd_padding2) <= kOffHWndOwner &&
              kOffHWndOwner + sizeof(HWND) <= offsetof(CWnd, _cwnd_padding2) + sizeof(CWnd::_cwnd_padding2),
              "CWnd::m_hWndOwner (+0xa0) must lie inside OpenMFC's unnamed CWnd padding");
static_assert(sizeof(CDockablePane) <= kOffSizeDefault,
              "OpenMFC's CDockablePane must end before CPaneDialog's own members");

template <typename T>
T& PaneDialogField(void* pThis, std::size_t off) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(pThis) + off);
}

// The vptr the CDockablePane ctor thunk installs. Retail's dtor re-stores
// CPaneDialog's own vftable before tail-jumping to ~CDockablePane; OpenMFC has
// no CPaneDialog vftable, so the dtor re-stores this one instead (see there).
std::atomic<void*> g_dockablePaneVptr{nullptr};

// Inline retail gate in front of every afxGlobalData read (0x6a790 is
// ?Initialize@AFX_GLOBAL_DATA@@ in mfc140u; 0x6a5c0 in mfc140):
//     if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); afxGlobalData.<+0> = 1; }
void EnsureGlobalData() {
    int gate = 0;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kOffGlobalDataGate, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kOffGlobalDataGate, &one, sizeof one);
    }
}

} // namespace

// CPaneDialog::CPaneDialog -- retail entry RVA 0xaa9d0 (mfc140u; 0xaaf90 mfc140):
//     CDockablePane::CDockablePane();          // call 0x3fe60 (mfc140u)
//     vptr = CPaneDialog vftable;              // mfc140u 0x1802f64b8
//     *(QWORD*)&m_sizeDefault = 0;             // +0x4e0
//     m_pOccDialogInfo = NULL;                 // +0x4e8
//     return this;
// Deviation, forced: OpenMFC has no CPaneDialog vftable, so the object keeps
// the CDockablePane vptr the base ctor thunk installed (remembered for the dtor).
// Symbol: ??0CPaneDialog@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPaneDialog__QEAA_XZ(void* pThis) {
    impl___0CDockablePane__QEAA_XZ(pThis);
    void* vptr = nullptr;
    std::memcpy(&vptr, pThis, sizeof vptr);
    g_dockablePaneVptr.store(vptr, std::memory_order_relaxed);
    PaneDialogField<unsigned long long>(pThis, kOffSizeDefault) = 0;
    PaneDialogField<void*>(pThis, kOffOccDialogInfo) = nullptr;
    return pThis;
}

// CPaneDialog::~CPaneDialog -- retail entry RVA 0xaaa60 (mfc140u; 0xab020 mfc140),
// three instructions:
//     lea  <CPaneDialog vftable>,%rax ; mov %rax,(%rcx)   ; mfc140u 0x1802f64b8
//     jmp  ??1CDockablePane@@UEAA@XZ                      ; 0x40050 (mfc140u)
// Deviation, forced: the vptr re-stored is the mingw CDockablePane one the
// ctor recorded (there is no CPaneDialog vftable in OpenMFC). The re-store
// matters for the same reason retail does it: a client subclass's own
// destructor leaves its vftable in place, and the base dtor thunk below
// destroys through the object's vptr.
// Signature corrected: the generated stub dropped `this` and returned void*.
// Symbol: ??1CPaneDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CPaneDialog__UEAA_XZ(void* pThis) {
    void* vptr = g_dockablePaneVptr.load(std::memory_order_relaxed);
    if (vptr != nullptr) std::memcpy(pThis, &vptr, sizeof vptr);
    impl___1CDockablePane__UEAA_XZ(pThis);
}

// CPaneDialog::Create(LPCTSTR, CWnd*, BOOL, LPCTSTR, UINT, UINT, DWORD, DWORD) --
// retail entry RVA 0xaaac0 (mfc140u), transcribed:
//     m_lpszBarTemplateName = lpszTemplateName;                        // +0x140
//     if (!CDockablePane::Create(lpszWindowName, pParentWnd, CSize(0, 0), bHasGripper,
//                                nID, nStyle, dwTabbedStyle, dwControlBarStyle))   // 0x40100
//         return FALSE;
//     if (!(GetStyle() & WS_CHILD))                                    // 0x2a9690, bt $0x1e
//         return FALSE;
//     m_lpszBarTemplateName = NULL;
//     CFrameWnd* pTop = g_pTopLevelFrame != NULL ? g_pTopLevelFrame    // 0x1803be1b8
//                                                : GetTopLevelFrame(); // 0x28e490
//     m_hWndOwner = pTop != NULL ? pTop->m_hWnd : NULL;                // inline SetOwner, +0xa0
//     if (m_sizeDialog.cx != 0 || m_sizeDialog.cy != 0)                // +0x148 / +0x14c
//         SetWindowPos(NULL, -1, -1, m_sizeDialog.cx, m_sizeDialog.cy,
//                      SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE, NULL);  // slot 0x480, flags 0x16
//     return TRUE;
// (The CSize(0,0) is the 8-byte zero retail writes over the caller's stack
// slot for lpszTemplateName -- already consumed at that point -- and passes in
// %r9. Callee names: uwho.py on the mfc140u targets.)
// m_hWndOwner (CWnd +0xa0) is not NAMED by OpenMFC's CWnd -- it lies in
// _cwnd_padding2 -- so it is stored by offset, as core/controlbar/CControlBar.cpp and
// controls/CMFCReBar.cpp read it.
// SetWindowPos is vftable slot 144 (+0x480), CBasePane::SetWindowPos in
// retail's CPaneDialog vftable; dispatched statically to that export.
// Behavioural note, not a deviation in this body: OpenMFC's
// CDockablePane::Create does not build the dialog from m_lpszBarTemplateName,
// so m_sizeDialog stays zero there and the SetWindowPos arm is normally skipped.
// Signature corrected: the generated stub dropped `this`.
// Symbol: ?Create@CPaneDialog@@QEAAHPEB_WPEAVCWnd@@H0IIKK@Z
extern "C" int MS_ABI impl__Create_CPaneDialog__QEAAHPEB_WPEAVCWnd__H0IIKK_Z(
    void* pThis, const wchar_t* lpszWindowName, CWnd* pParentWnd, int bHasGripper,
    const wchar_t* lpszTemplateName, unsigned int nStyle, unsigned int nID,
    unsigned long dwTabbedStyle, unsigned long dwControlBarStyle) {
    CBasePane* pBase = static_cast<CBasePane*>(static_cast<CDockablePane*>(pThis));
    pBase->m_lpszBarTemplateName = lpszTemplateName;
    if (!impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__VCSize__HIKKK_Z(
            static_cast<CDockablePane*>(pThis), lpszWindowName, pParentWnd, CSize(0, 0),
            bHasGripper, nID, nStyle, dwTabbedStyle, dwControlBarStyle)) {
        return FALSE;
    }
    if (!(impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pBase)) & WS_CHILD)) {
        return FALSE;
    }
    pBase->m_lpszBarTemplateName = nullptr;

    CFrameWnd* pTop = static_cast<CFrameWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
    if (pTop == nullptr) {
        pTop = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(static_cast<const CWnd*>(pBase));
    }
    HWND hOwner = pTop != nullptr ? static_cast<CWnd*>(pTop)->m_hWnd : nullptr;
    std::memcpy(static_cast<unsigned char*>(pThis) + kOffHWndOwner, &hOwner, sizeof hOwner);

    if (pBase->m_sizeDialog.cx != 0 || pBase->m_sizeDialog.cy != 0) {
        impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
            pBase, nullptr, -1, -1, pBase->m_sizeDialog.cx, pBase->m_sizeDialog.cy,
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
    }
    return TRUE;
}

// CPaneDialog::Create(LPCTSTR, CWnd*, BOOL, UINT nIDTemplate, UINT, UINT) --
// retail entry RVA 0xaaa70 (mfc140u), a pure forwarder:
//     return Create(lpszWindowName, pParentWnd, bHasGripper,
//                   MAKEINTRESOURCE(nIDTemplate),          // movzwl: low 16 bits
//                   nStyle, nID,
//                   AFX_CBRS_REGULAR_TABS,                 // 0x20
//                   AFX_DEFAULT_DOCKING_PANE_STYLE);       // 0xf
// Signature corrected: the generated stub dropped `this`.
// Symbol: ?Create@CPaneDialog@@QEAAHPEB_WPEAVCWnd@@HIII@Z
extern "C" int MS_ABI impl__Create_CPaneDialog__QEAAHPEB_WPEAVCWnd__HIII_Z(
    void* pThis, const wchar_t* lpszWindowName, CWnd* pParentWnd, int bHasGripper,
    unsigned int nIDTemplate, unsigned int nStyle, unsigned int nID) {
    return impl__Create_CPaneDialog__QEAAHPEB_WPEAVCWnd__H0IIKK_Z(
        pThis, lpszWindowName, pParentWnd, bHasGripper,
        MAKEINTRESOURCEW(static_cast<WORD>(nIDTemplate)), nStyle, nID, 0x20, 0xF);
}

// CPaneDialog::CreateObject -- retail entry RVA 0xaa960 (mfc140u; 0xaaf20 mfc140):
//     void* p = ::operator new(0x4f0);        // ??2@YAPEAX_K@Z, 0x27f0 (mfc140u)
//     return p != NULL ? new(p) CPaneDialog : NULL;
// Symbol: ?CreateObject@CPaneDialog@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CPaneDialog__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kSizeofPaneDialog);
    if (p == nullptr) return nullptr;
    return impl___0CPaneDialog__QEAA_XZ(p);
}

// CPaneDialog::HandleInitDialog -- retail entry RVA 0xaabf0 (mfc140u; 0xab1b0 mfc140):
//     CBasePane::HandleInitDialog(wParam, lParam);        // 0xcc20, result discarded
//     Default();                                          // 0x28ac80, result discarded
//     COccManager* pOccManager = AfxGetModuleState()->m_pOccManager;   // 0x133930, +0xa0
//     if (pOccManager != NULL && m_pOccDialogInfo != NULL) {           // +0x4e8
//         if (!pOccManager->CreateDlgControls(this, m_lpszBarTemplateName,
//                                             m_pOccDialogInfo))      // its vftable +0x48
//             return FALSE;
//     }
//     LoadDynamicLayoutResource(m_lpszBarTemplateName);   // 0x2925b0, result discarded
//     return TRUE;
// COccManager vftable slot 9 (+0x48) is CreateDlgControls(CWnd*, LPCTSTR,
// _AFX_OCC_DIALOG_INFO*) -- see the slot table in core/ole/COccManager.cpp.
// The manager is client-built against the real header, so it is dispatched
// through its own (MSVC-layout) vftable exactly as retail does.
// Deviation, forced: OpenMFC's AFX_MODULE_STATE has no m_pOccManager slot;
// AfxEnableControlContainer records the manager in the process-wide
// g4_g_pOccManager instead, and that is what is read here (so there is no
// per-module-state distinction, and AfxGetModuleState is not called).
// Signature corrected: the generated stub dropped `this`.
// Symbol: ?HandleInitDialog@CPaneDialog@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__HandleInitDialog_CPaneDialog__QEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    CBasePane* pBase = static_cast<CBasePane*>(static_cast<CDockablePane*>(pThis));
    impl__HandleInitDialog_CBasePane__IEAA_J_K_J_Z(pBase, wParam, lParam);
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pBase));

    void* pOccManager = g4_g_pOccManager;
    void* pOccDialogInfo = PaneDialogField<void*>(pThis, kOffOccDialogInfo);
    if (pOccManager != nullptr && pOccDialogInfo != nullptr) {
        using CreateDlgControlsFn = int (MS_ABI*)(void*, CWnd*, const wchar_t*, void*);
        CreateDlgControlsFn pfn = (*reinterpret_cast<CreateDlgControlsFn* const*>(pOccManager))[9];
        if (!pfn(pOccManager, static_cast<CWnd*>(pBase), pBase->m_lpszBarTemplateName, pOccDialogInfo)) {
            return 0;
        }
    }
    impl__LoadDynamicLayoutResource_CWnd__IEAAHPEB_W_Z(static_cast<CWnd*>(pBase),
                                                        pBase->m_lpszBarTemplateName);
    return 1;
}

// CPaneDialog::OnEraseBkgnd -- retail entry RVA 0xaac70 (mfc140u, by ordinal;
// 0xab230 mfc140), transcribed:
//     CRect rect;                                        // zeroed (xorps/movups)
//     ::GetClientRect(m_hWnd, &rect);                    // IAT 0x1802c7330 (mfc140u)
//     <afxGlobalData init gate>                          // 0x6a790 (mfc140u)
//     ::FillRect(pDC->m_hDC, &rect, afxGlobalData.brBtnFace);   // +0xb0; IAT 0x1802c7208
//     return TRUE;
// The CRect goes through CRect::operator LPRECT, i.e. the address of the local.
// Retail has no NULL check on pDC; neither does this body.
// Signature corrected: the generated stub dropped `this`.
// Symbol: ?OnEraseBkgnd@CPaneDialog@@QEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CPaneDialog__QEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    RECT rect = {0, 0, 0, 0};
    ::GetClientRect(static_cast<CWnd*>(static_cast<CDockablePane*>(pThis))->m_hWnd, &rect);
    EnsureGlobalData();
    HBRUSH hbr = nullptr;
    std::memcpy(&hbr, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kOffGlobalBrBtnFace, sizeof hbr);
    ::FillRect(pDC->m_hDC, &rect, hbr);
    return TRUE;
}

// CPaneDialog::OnLButtonDblClk -- retail entry RVA 0xaacf0 (mfc140u, by ordinal;
// 0xab2b0 mfc140), transcribed:
//     CPoint ptScreen = point;
//     ::ClientToScreen(m_hWnd, &ptScreen);                 // IAT 0x1802c7248 (mfc140u)
//     if (HitTest(ptScreen, TRUE) == HTCAPTION)            // vftable +0x678, slot 207
//         CDockablePane::OnLButtonDblClk(nFlags, point);   // 0x43400 (mfc140u), CLIENT point
//     else
//         Default();                                       // 0x28ac80
// Slot 207 in retail's CPaneDialog vftable is CDockablePane::HitTest
// (0x40fd0, mfc140u); dispatched statically to that export.
// Signature corrected: the generated stub dropped `this`.
// Symbol: ?OnLButtonDblClk@CPaneDialog@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CPaneDialog__QEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point) {
    CDockablePane* pPane = static_cast<CDockablePane*>(pThis);
    POINT ptScreen;
    std::memcpy(&ptScreen, &point, sizeof ptScreen);
    ::ClientToScreen(static_cast<CWnd*>(pPane)->m_hWnd, &ptScreen);
    void* screenArg = nullptr;
    std::memcpy(&screenArg, &ptScreen, sizeof ptScreen);
    void* clientArg = nullptr;
    std::memcpy(&clientArg, &point, sizeof point);
    if (impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(pPane, screenArg, TRUE) == HTCAPTION) {
        impl__OnLButtonDblClk_CDockablePane__IEAAXIVCPoint___Z(pPane, nFlags, clientArg);
    } else {
        impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pPane));
    }
}

// CPaneDialog::OnLButtonDown -- retail entry RVA 0xaad70 (mfc140u, by ordinal;
// 0xab330 mfc140), transcribed:
//     CPoint ptScreen = point;
//     ::ClientToScreen(m_hWnd, &ptScreen);                 // IAT 0x1802c7248 (mfc140u)
//     UINT nHit = HitTest(ptScreen, TRUE);                 // vftable +0x678, slot 207
//     if (nHit <= 0x13 && ((0x80304 >> nHit) & 1))         // `cmp $0x13; ja` + `bt`
//         CDockablePane::OnLButtonDown(nFlags, point);     // 0x41300 (mfc140u), CLIENT point
//     else
//         Default();                                       // 0x28ac80
// 0x80304 selects hit codes 2 HTCAPTION, 8 HTMINBUTTON, 9 HTMAXBUTTON and
// 19 HTOBJECT (== AFX_HTCLOSE, afxcaptionbutton.h:29). The compare is unsigned,
// so HTERROR (-2) / HTTRANSPARENT (-1) take the Default() arm.
// Slot 207 is CDockablePane::HitTest (see OnLButtonDblClk); dispatched statically.
// Signature corrected: the generated stub dropped `this`.
// Symbol: ?OnLButtonDown@CPaneDialog@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CPaneDialog__QEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point) {
    CDockablePane* pPane = static_cast<CDockablePane*>(pThis);
    POINT ptScreen;
    std::memcpy(&ptScreen, &point, sizeof ptScreen);
    ::ClientToScreen(static_cast<CWnd*>(pPane)->m_hWnd, &ptScreen);
    void* screenArg = nullptr;
    std::memcpy(&screenArg, &ptScreen, sizeof ptScreen);
    void* clientArg = nullptr;
    std::memcpy(&clientArg, &point, sizeof point);
    const unsigned int nHit = static_cast<unsigned int>(
        impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(pPane, screenArg, TRUE));
    if (nHit <= 0x13u && ((0x80304u >> nHit) & 1u)) {
        impl__OnLButtonDown_CDockablePane__IEAAXIVCPoint___Z(pPane, nFlags, clientArg);
    } else {
        impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pPane));
    }
}

// CPaneDialog::OnPrintClient -- retail entry RVA 0xaaea0 (mfc140u, by ordinal;
// 0xab460 mfc140), transcribed:
//     if (nFlags & PRF_ERASEBKGND)                         // test $0x8,%r8b (PRF_ERASEBKGND == 8;
//                                                          //  PRF_CLIENT would be 4)
//         ::SendMessage(m_hWnd, WM_ERASEBKGND,             // 0x14; IAT 0x1802c7120 = SendMessageW
//                       (WPARAM)(pDC != NULL ? pDC->m_hDC : NULL), 0);
//     return 0;
// Signature corrected: the generated stub dropped `this`.
// Symbol: ?OnPrintClient@CPaneDialog@@QEAA_JPEAVCDC@@I@Z
extern "C" __int64 MS_ABI impl__OnPrintClient_CPaneDialog__QEAA_JPEAVCDC__I_Z(
    void* pThis, CDC* pDC, unsigned int nFlags) {
    if (nFlags & PRF_ERASEBKGND) {
        ::SendMessage(static_cast<CWnd*>(static_cast<CDockablePane*>(pThis))->m_hWnd, WM_ERASEBKGND,
                      reinterpret_cast<WPARAM>(pDC != nullptr ? pDC->m_hDC : nullptr), 0);
    }
    return 0;
}

// CPaneDialog::OnUpdateCmdUI -- retail entry RVA 0xaabd0 (mfc140u, by ordinal;
// 0xab190 mfc140) is a single `jmp 0x45450`, i.e. a tail call to
// CDockablePane::OnUpdateCmdUI(pTarget, bDisableIfNoHndler) (0x45450, mfc140u).
// Signature corrected: the generated stub dropped `this`.
// Symbol: ?OnUpdateCmdUI@CPaneDialog@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CPaneDialog__UEAAXPEAVCFrameWnd__H_Z(
    void* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    impl__OnUpdateCmdUI_CDockablePane__UEAAXPEAVCFrameWnd__H_Z(
        static_cast<CDockablePane*>(pThis), pTarget, bDisableIfNoHndler);
}

// CPaneDialog::OnWindowPosChanging -- retail entry RVA 0xaadf0 (mfc140u, by
// ordinal; 0xab3b0 mfc140), transcribed:
//     Default();                                           // 0x28ac80, FIRST, unconditionally
//     if (<slot 118>())                                    // +0x3b0: m_dwControlBarStyle & AFX_CBRS_RESIZE
//         return;
//     CSize sizeMin(0, 0);
//     <slot 158>(sizeMin);                                 // +0x4f0: sizeMin = m_sizeMin (+0x200)
//     if (<slot 91>() && lpwndpos->cy < sizeMin.cy) {      // +0x2d8: IsHorizontal
//         lpwndpos->cy = sizeMin.cy;
//         lpwndpos->hwndInsertAfter = (HWND)1;             // movq $0x1,0x8(%rbx)
//     } else if (!<slot 91>() && lpwndpos->cx < sizeMin.cx) {  // IsHorizontal called again
//         lpwndpos->cx = sizeMin.cx;
//         lpwndpos->hwndInsertAfter = (HWND)1;
//     }
// The compares are signed (jge). The (HWND)1 store (== HWND_BOTTOM) at
// WINDOWPOS+0x8 is what the code does; it is reproduced literally.
// The three slots, read off retail's CPaneDialog vftable (mfc140u 0x1802f64b8),
// hold unexported inline bodies (mfc140u 0x87f0 / 0x8890 / 0x8860):
//   slot 118 `mov 0x108(%rcx),%eax; and $0x4,%eax` -- CBasePane::CanBeResized
//            (afxbasepane.h:125)
//   slot 158 `mov 0x200(%rcx),%rax; mov %rax,(%rdx)` -- CPane::GetMinSize
//            (afxpane.h:76)
//   slot 91  `GetCurrentAlignment() & 0xa000` (slot 103, CBasePane::
//            GetCurrentAlignment) -- CBasePane::IsHorizontal (afxbasepane.h:313)
// Dispatched statically: the first two are evaluated inline on the OpenMFC
// members at the same offsets, the third through the GetCurrentAlignment export.
// Signature corrected: the generated stub dropped `this`.
// Symbol: ?OnWindowPosChanging@CPaneDialog@@QEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CPaneDialog__QEAAXPEAUtagWINDOWPOS___Z(
    void* pThis, WINDOWPOS* lpwndpos) {
    CPane* pPane = static_cast<CPane*>(static_cast<CDockablePane*>(pThis));
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pPane));
    if (pPane->m_dwControlBarStyle & 0x4 /*AFX_CBRS_RESIZE*/) {
        return;
    }
    CSize sizeMin(0, 0);
    sizeMin = pPane->m_sizeMin;
    auto isHorizontal = [pPane]() -> bool {
        return (impl__GetCurrentAlignment_CBasePane__UEBAKXZ(pPane) & 0xA000UL /*CBRS_ORIENT_HORZ*/) != 0;
    };
    if (isHorizontal() && lpwndpos->cy < sizeMin.cy) {
        lpwndpos->cy = sizeMin.cy;
        lpwndpos->hwndInsertAfter = reinterpret_cast<HWND>(1);
    } else if (!isHorizontal() && lpwndpos->cx < sizeMin.cx) {
        lpwndpos->cx = sizeMin.cx;
        lpwndpos->hwndInsertAfter = reinterpret_cast<HWND>(1);
    }
}

// CPaneDialog::SetOccDialogInfo -- retail entry RVA 0xaac60 (mfc140u, by
// ordinal; 0xab220 mfc140):
//     mov %rdx,0x4e8(%rcx) ; mov $0x1,%eax ; ret
// i.e. `m_pOccDialogInfo = pOccDialogInfo; return TRUE;`
// Signature corrected: the generated stub dropped `this`.
// Symbol: ?SetOccDialogInfo@CPaneDialog@@UEAAHPEAU_AFX_OCC_DIALOG_INFO@@@Z
extern "C" int MS_ABI impl__SetOccDialogInfo_CPaneDialog__UEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(
    void* pThis, void* pOccDialogInfo) {
    PaneDialogField<void*>(pThis, kOffOccDialogInfo) = pOccDialogInfo;
    return TRUE;
}
