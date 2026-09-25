// CDialogImpl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CDialogImpl is the feature-pack dialog helper of afxdialogimpl.h (NOT the ATL
// CDialogImpl template): CDialogEx / CMFCPropertyPage / CMFCPropertySheet embed one
// and forward keyboard, mouse, WM_COMMAND and activation traffic to it so that an
// active CMFCPopupMenu behaves inside a dialog.  No OpenMFC header declares it, so
// `this` is taken as void* and the layout is pinned below from the retail ctor.
//
// Every body is transcribed from the retail export.  The disassembler reads the ANSI
// twin mfc140.dll; the bodies are byte-identical in mfc140u.dll, and every function
// RVA cited in this file is the mfc140u one (functions the mfc140u symbol map does
// not resolve were located by the ANSI body at the same +0x1b0 displacement the
// mapped ones have -- ANSI 0x3f380 ctor -> mfc140u 0x3f530 -- and checked
// instruction-for-instruction, rip-relative displacements aside).
//
// Layout (retail ctor, RVA 0x3f530 (mfc140u)):
//     lea  vftable,%rax ; mov %rdx,0x8(%rcx) ; mov %rax,(%rcx) ; mov %rcx,%rax ; ret
//   +0x00  vfptr     -- ??_7CDialogImpl@@6B@, 0x1802e4de8 (mfc140u), one slot
//   +0x08  CWnd& m_Dlg
//   sizeof == 0x10 (the scalar deleting destructor frees 0x10 bytes).

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>

// ---------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition; the parameter lists
// match those definitions and the mangled names.
//   featurepack/menu/CMFCPopupMenu.cpp    : GetSafeActivePopupMenu, CheckArea
//   featurepack/menu/CMFCPopupMenuBar.cpp : FindDestintationToolBar, CloseDelayedSubMenu
//   core/runtime/CObject.cpp              : IsKindOf
//   core/window/CWnd.cpp                  : FromHandle, IsWindowEnabled
//   core/window/Thunks.cpp                : SetFocus
//   featurepack/customize/CMFCCmdUsageCount.cpp : AddCmd
//   featurepack/customize/CUserToolsManager.cpp : InvokeTool
//   featurepack/{menu,toolbar,ribbon,controls}/RuntimeClasses.cpp : GetThisClass
// ---------------------------------------------------------------------------
extern "C" CMFCPopupMenu* MS_ABI impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
extern "C" int MS_ABI impl__CheckArea_CMFCPopupMenu__QEBA_AW4MENUAREA_TYPE_1_AEBVCPoint___Z(
    const void* pThis, const CPoint& ptScreen);
extern "C" CMFCToolBar* MS_ABI impl__FindDestintationToolBar_CMFCPopupMenuBar__QEAAPEAVCMFCToolBar__VCPoint___Z(
    CMFCPopupMenuBar* pThis, long long point);
extern "C" void MS_ABI impl__CloseDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ(CMFCPopupMenuBar* pThis);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis,
                                                                        const CRuntimeClass* pClass);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" void MS_ABI impl__AddCmd_CMFCCmdUsageCount__QEAAXI_Z(void* pThis, unsigned int uiCmd);
extern "C" int MS_ABI impl__InvokeTool_CUserToolsManager__QEAAHI_Z(void* pThis, UINT uiCmd);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonPanelMenu__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCColorPopupMenu__SAPEAUCRuntimeClass__XZ();

// Class statics with storage elsewhere (the addresses the retail bodies use, mfc140u).
extern "C" std::int32_t  impl__m_bCustomizeMode_CMFCToolBar__1HA;                  // 0x1803be35c (mfc140u)
extern "C" void*         impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;        // 0x1803be288 (mfc140u)
extern "C" void*         impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;    // 0x1803be3b0 (mfc140u)
extern "C" unsigned char impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A[80]; // 0x1803b1fd0 (mfc140u)

// This class's own statics (defined here, see the bottom of the file for the
// markers).  Both are zero in the retail image's .data.
extern "C" HHOOK impl__m_hookMouse_CDialogImpl__1PEAUHHOOK____EA;
extern "C" void* impl__m_pMenuDlgImpl_CDialogImpl__1PEAV1_EA;

// Defined below and called by sibling bodies (retail calls them directly).
extern "C" int MS_ABI impl__ProcessMouseClick_CDialogImpl__IEAAHUtagPOINT___Z(void* pThis, long long pt);
extern "C" int MS_ABI impl__ProcessMouseMove_CDialogImpl__IEAAHUtagPOINT___Z(void* pThis, long long pt);
extern "C" __int64 MS_ABI impl__DialogMouseHookProc_CDialogImpl__KA_JH_K_J_Z(int nCode, unsigned __int64 wParam,
                                                                              __int64 lParam);

// The linker-provided base of this DLL's own image (IsOwnObject).
extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {

struct S_CDialogImpl {
    const void* vfptr;   // +0x00
    CWnd* m_pDlg;        // +0x08 CWnd& m_Dlg
};
static_assert(offsetof(S_CDialogImpl, m_pDlg) == 0x08, "retail ctor: mov %rdx,0x8(%rcx)");
static_assert(sizeof(S_CDialogImpl) == 0x10, "retail deleting dtor frees 0x10 bytes");

static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at +0x40 of every window below");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "CMFCToolBarButton::m_rect (+0x68(%rbp) in ProcessMouseClick)");
static_assert(offsetof(CMFCToolBarButton, m_pWndParent) == 0x80, "CMFCToolBarButton::m_pWndParent (+0x80(%rbp))");

inline S_CDialogImpl* Self(void* pThis) { return static_cast<S_CDialogImpl*>(pThis); }
inline CWnd* Dlg(void* pThis) { return Self(pThis)->m_pDlg; }
// CWnd::GetSafeHwnd as retail inlines it on &m_Dlg (it keeps the NULL test).
inline HWND SafeHwnd(const CWnd* pWnd) { return pWnd != nullptr ? pWnd->m_hWnd : nullptr; }
inline HWND HWndOf(const void* pWnd) { return static_cast<const CWnd*>(pWnd)->m_hWnd; }

template <class T> inline T At(const void* p, int off) {
    return *reinterpret_cast<const T*>(static_cast<const unsigned char*>(p) + off);
}

// POINT by value is an 8-byte aggregate in one register under the MS x64 ABI.
inline POINT UnpackPoint(long long v) {
    POINT pt;
    pt.x = static_cast<LONG>(static_cast<std::uint32_t>(v & 0xffffffffu));
    pt.y = static_cast<LONG>(static_cast<std::uint32_t>(static_cast<std::uint64_t>(v) >> 32));
    return pt;
}
inline long long PackPoint(POINT pt) {
    return static_cast<long long>((static_cast<std::uint64_t>(static_cast<std::uint32_t>(pt.y)) << 32) |
                                  static_cast<std::uint32_t>(pt.x));
}

// Retail-private constants.
constexpr DWORD  kWF_STAYACTIVE   = 0x0020;   // afxwin.h CWnd::m_nFlags bit
constexpr UINT   kWM_FLOATSTATUS  = 0x036D;   // afxpriv.h
constexpr WPARAM kFS_SYNCACTIVE   = 0x0040;   // afxpriv.h
// CMFCPopupMenu::MENUAREA_TYPE (afxpopupmenu.h declaration order).
enum { kAreaMenu = 0, kAreaShadowRight = 1, kAreaShadowBottom = 2, kAreaLogo = 3, kAreaOutside = 4,
       kAreaTearOffCaption = 5 };
// Retail CMFCPopupMenu members read here (see featurepack/menu/CMFCPopupMenu.cpp, which
// pins the same offsets against detail/CMFCPopupMenuSupport.h).
constexpr int kOffPopupParentBtn  = 0x228;    // CMFCToolBarMenuButton* m_pParentBtn
constexpr int kOffPopupScrollable = 0x16a4;   // BOOL m_bScrollable
constexpr int kOffBarInCommand    = 0x1384;   // CMFCPopupMenuBar::m_bInCommand

// ---------------------------------------------------------------------------
// Virtual dispatch on popup menus and popup-menu bars, as core/frame/CFrameImpl.cpp
// does it.  A popup whose vptr points into a client image carries a retail-shaped
// vftable, so retail's slot is called; one whose vptr points into THIS image was
// built by OpenMFC's own C++ class (Itanium vtable), so the retail body of the slot
// is used instead (an OpenMFC-side override is then not reached -- DEVIATION).
// ---------------------------------------------------------------------------
bool IsOwnObject(const void* pObject) {
    const void* vptr = *reinterpret_cast<const void* const*>(pObject);
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* p = static_cast<const unsigned char*>(vptr);
    return p >= base && p < base + nt->OptionalHeader.SizeOfImage;
}

template <typename Fn>
Fn VSlot(const void* pObject, int nSlot) {
    void* const* vtbl = *reinterpret_cast<void* const* const*>(pObject);
    return reinterpret_cast<Fn>(vtbl[nSlot]);
}

// CMFCPopupMenu vftable slot 116 (+0x3a0), GetMenuBar.  Base body RVA 0x34cf0 (mfc140u)
// is `lea 0x230(%rcx),%rax`; the CMFCRibbonPanelMenu, CMFCRibbonMiniToolBar and
// CMFCColorPopupMenu vftables (0x18030a128 / 0x180308168 / 0x1802e13d0, mfc140u, read
// from their ctors / dtor) hold 0x27f80 there instead, which returns this+0x19c8
// (afxribbonpanelmenu.h m_wndRibbonBar / afxcolorpopupmenu.h m_wndColorBar), exactly as
// CFrameImpl.cpp's PopupMenuBar() folds it.
CMFCPopupMenuBar* PopupMenuBar(void* pPopup) {
    if (IsOwnObject(pPopup)) {
        const CObject* pObject = static_cast<const CObject*>(static_cast<CWnd*>(static_cast<CMFCPopupMenu*>(pPopup)));
        const bool bBarAt19c8 =
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pObject, impl__GetThisClass_CMFCRibbonPanelMenu__SAPEAUCRuntimeClass__XZ()) != 0 ||
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pObject, impl__GetThisClass_CMFCColorPopupMenu__SAPEAUCRuntimeClass__XZ()) != 0;
        return reinterpret_cast<CMFCPopupMenuBar*>(static_cast<unsigned char*>(pPopup) +
                                                   (bBarAt19c8 ? 0x19c8 : 0x230));
    }
    typedef CMFCPopupMenuBar* (MS_ABI* Fn)(void*);
    return VSlot<Fn>(pPopup, 116)(pPopup);
}

// CMFCPopupMenu vftable slot 117 (+0x3a8), InCommand.  RVA 0x27f30 (mfc140u), the same
// entry in the base and all three derived vftables above:
//     CMFCPopupMenuBar* pBar = GetMenuBar();          // vslot 0x3a0
//     return pBar == NULL ? FALSE : pBar->m_bInCommand; // +0x1384
// CAVEAT for an own popup: OpenMFC's ??0CMFCPopupMenu thunk builds a 0x238-byte object
// and never constructs the embedded bar (see the header of CMFCPopupMenu.cpp), so this
// reads the byte range that CMFCPopupMenuBar.cpp's SetInCommand writes for the same
// object -- consistent with the tree, but beyond OpenMFC's own allocation size.
BOOL PopupInCommand(void* pPopup) {
    if (IsOwnObject(pPopup)) {
        CMFCPopupMenuBar* pBar = PopupMenuBar(pPopup);
        return pBar == nullptr ? FALSE : At<int>(pBar, kOffBarInCommand);
    }
    typedef int (MS_ABI* Fn)(void*);
    return VSlot<Fn>(pPopup, 117)(pPopup);
}

// CMFCPopupMenuBar vftable slot 274 (+0x890), CloseDelayedSubMenu (0xbf500 in the
// vftable ??0CMFCPopupMenuBar@@QEAA@XZ installs, 0x1802f8fc8 (mfc140u)).  An own bar
// goes to the exported thunk, which dispatches through OpenMFC's C++ virtual.
void BarCloseDelayedSubMenu(void* pBar) {
    if (IsOwnObject(pBar)) {
        impl__CloseDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ(static_cast<CMFCPopupMenuBar*>(pBar));
        return;
    }
    typedef void (MS_ABI* Fn)(void*);
    VSlot<Fn>(pBar, 274)(pBar);
}

inline BOOL IsKindOfClass(const CWnd* pWnd, CRuntimeClass* pClass) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(pWnd), pClass);
}

// ---------------------------------------------------------------------------
// The one-slot vftable.  Slot 0 (RVA 0x3f550 (mfc140u)) is the scalar deleting
// destructor:
//     vfptr = &vftable;                     // inline ~CDialogImpl
//     if (flags & 1) operator delete(this, 0x10);   // 0x2b77b0 -> jmp ??_V@YAXPEAX@Z
//     return this;                                   //   -> IAT 0x1802c74e8 (mfc140u) = ucrt free
// The retail table's preceding qword is its RTTI complete-object locator; OpenMFC has
// no MSVC RTTI for this class, so that slot is NULL (typeid / dynamic_cast on a
// CDialogImpl -- a protected, non-exported-RTTI helper -- is not supported).
// ---------------------------------------------------------------------------
void* MS_ABI DialogImplScalarDeletingDtor(void* pThis, unsigned int flags);

struct DialogImplVftable {
    const void* pCompleteObjectLocator;
    void* slots[1];
};
const DialogImplVftable kDialogImplVftable = {
    nullptr,
    { reinterpret_cast<void*>(&DialogImplScalarDeletingDtor) },
};
inline const void* DialogImplVfptr() { return &kDialogImplVftable.slots[0]; }

void* MS_ABI DialogImplScalarDeletingDtor(void* pThis, unsigned int flags) {
    Self(pThis)->vfptr = DialogImplVfptr();
    if ((flags & 1) != 0) std::free(pThis);
    return pThis;
}

} // namespace

// Transcribed from RVA 0x3f530 (mfc140u), in full:
//     vfptr = &CDialogImpl::`vftable'; m_Dlg = dlg; return this;
// (The earlier placeholder returned pThis without storing m_Dlg, which every body
// below reads.)
// Symbol: ??0CDialogImpl@@IEAA@AEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CDialogImpl__IEAA_AEAVCWnd___Z(void* pThis, void* pWnd) {
    Self(pThis)->m_pDlg = static_cast<CWnd*>(pWnd);
    Self(pThis)->vfptr = DialogImplVfptr();
    return pThis;
}

// Transcribed from RVA 0x3f580 (mfc140u), in full: `vfptr = &CDialogImpl::`vftable';`
// (the class owns nothing; m_Dlg is a reference).
// Symbol: ??1CDialogImpl@@MEAA@XZ
extern "C" void MS_ABI impl___1CDialogImpl__MEAA_XZ(void* pThis) {
    Self(pThis)->vfptr = DialogImplVfptr();
}

// Transcribed from RVA 0x3fa50 (mfc140u), the WH_MOUSE hook SetActiveMenu installs:
//     if (m_pMenuDlgImpl != NULL) {
//         switch (wParam) {
//         case WM_NCLBUTTONDOWN: case WM_NCRBUTTONDOWN: case WM_NCMBUTTONDOWN:
//         case WM_LBUTTONDOWN:   case WM_RBUTTONDOWN:   case WM_MBUTTONDOWN: {
//             CPoint pt; ::GetCursorPos(&pt);                          // zero-initialised
//             CRect rect; ::GetWindowRect(m_pMenuDlgImpl->m_Dlg.m_hWnd, &rect);   // zeroed
//             if (!::PtInRect(&rect, pt)) m_pMenuDlgImpl->ProcessMouseClick(pt);  // result ignored
//         } }
//     }
//     return ::CallNextHookEx(m_hookMouse, nCode, wParam, lParam);
// (Import slots of the mfc140 image: GetCursorPos 0x1802c5348, GetWindowRect
// 0x1802c5370, PtInRect 0x1802c5320, CallNextHookEx 0x1802c4ce8.)
// Symbol: ?DialogMouseHookProc@CDialogImpl@@KA_JH_K_J@Z
extern "C" __int64 MS_ABI impl__DialogMouseHookProc_CDialogImpl__KA_JH_K_J_Z(int nCode, unsigned __int64 wParam,
                                                                              __int64 lParam) {
    if (impl__m_pMenuDlgImpl_CDialogImpl__1PEAV1_EA != nullptr) {
        switch (wParam) {
        case WM_NCLBUTTONDOWN:
        case WM_NCRBUTTONDOWN:
        case WM_NCMBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN: {
            POINT pt = {};
            ::GetCursorPos(&pt);
            RECT rect = {};
            ::GetWindowRect(Dlg(impl__m_pMenuDlgImpl_CDialogImpl__1PEAV1_EA)->m_hWnd, &rect);
            if (!::PtInRect(&rect, pt)) {
                impl__ProcessMouseClick_CDialogImpl__IEAAHUtagPOINT___Z(impl__m_pMenuDlgImpl_CDialogImpl__1PEAV1_EA,
                                                                         PackPoint(pt));
            }
            break;
        }
        default:
            break;
        }
    }
    return ::CallNextHookEx(impl__m_hookMouse_CDialogImpl__1PEAUHHOOK____EA, nCode, static_cast<WPARAM>(wParam),
                            static_cast<LPARAM>(lParam));
}

// Transcribed from RVA 0x3fcb0 (mfc140u):
//     m_Dlg.m_nFlags &= ~WF_STAYACTIVE;                                   // +0xa8, 0x20
//     CWnd* pWndActive = (nState == WA_INACTIVE) ? pWndOther : &m_Dlg;
//     if (pWndActive == NULL) {
//         ::SendMessage(m_Dlg.m_hWnd, WM_NCPAINT, 1, 0);
//         return;
//     }
//     if (pWndActive->m_hWnd == m_Dlg.GetSafeHwnd() ||
//         ::SendMessage(pWndActive->m_hWnd, WM_FLOATSTATUS, FS_SYNCACTIVE, 0) != 0)
//         m_Dlg.m_nFlags |= WF_STAYACTIVE;
// (0x36d / 0x40 are WM_FLOATSTATUS / FS_SYNCACTIVE; the import slot 0x1802c5378 of the
// mfc140 image is SendMessageA, i.e. SendMessageW in mfc140u.)
// DEVIATION: retail keeps m_nFlags at its retail CWnd offset +0xa8; OpenMFC's CWnd
// declares m_nFlags at +0x50 and its own flag bookkeeping uses that field (same choice
// as CMiniFrameWnd::OnNcActivate in core/frame/CMiniFrameWnd.cpp), so the named member
// is used.  OnNcActivate below reads the bit back from the same field.
// Symbol: ?OnActivate@CDialogImpl@@IEAAXIPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnActivate_CDialogImpl__IEAAXIPEAVCWnd___Z(void* pThis, unsigned int nState,
                                                                          CWnd* pWndOther) {
    CWnd* pDlg = Dlg(pThis);
    pDlg->m_nFlags &= ~kWF_STAYACTIVE;

    CWnd* pWndActive = (nState != WA_INACTIVE) ? pDlg : pWndOther;
    if (pWndActive == nullptr) {
        ::SendMessage(pDlg->m_hWnd, WM_NCPAINT, 1, 0);
        return;
    }
    if (pWndActive->m_hWnd == SafeHwnd(pDlg) ||
        ::SendMessage(pWndActive->m_hWnd, kWM_FLOATSTATUS, kFS_SYNCACTIVE, 0) != 0) {
        pDlg->m_nFlags |= kWF_STAYACTIVE;
    }
}

// Transcribed from RVA 0x3fbf0 (mfc140u):
//     if (HIWORD(wParam) == 1) {                          // accelerator
//         UINT uiCmd = LOWORD(wParam);
//         CMFCToolBar::m_UsageCount.AddCmd(uiCmd);
//         if (CMFCPopupMenu::GetSafeActivePopupMenu() != NULL) {
//             ::SendMessage(<that popup>->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
//             return TRUE;
//         }
//         if (afxUserToolsManager != NULL)
//             return afxUserToolsManager->InvokeTool(uiCmd) != 0;
//     }
//     return FALSE;                                      // lParam is not read
// Symbol: ?OnCommand@CDialogImpl@@IEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CDialogImpl__IEAAH_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)lParam;
    if (static_cast<std::uint16_t>(wParam >> 16) != 1) {
        return FALSE;
    }
    const unsigned int uiCmd = static_cast<std::uint16_t>(wParam);
    impl__AddCmd_CMFCCmdUsageCount__QEAAXI_Z(impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A, uiCmd);

    CMFCPopupMenu* pPopup = impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
    if (pPopup != nullptr) {
        ::SendMessage(HWndOf(pPopup), WM_KEYDOWN, VK_ESCAPE, 0);
        return TRUE;
    }
    if (impl__afxUserToolsManager__3PEAVCUserToolsManager__EA != nullptr) {
        return impl__InvokeTool_CUserToolsManager__QEAAHI_Z(impl__afxUserToolsManager__3PEAVCUserToolsManager__EA,
                                                            uiCmd) != 0 ? TRUE : FALSE;
    }
    return FALSE;
}

// Transcribed from RVA 0x3fbb0 (mfc140u), in full:
//     if (m_pMenuDlgImpl != NULL &&
//         m_pMenuDlgImpl->m_Dlg.GetSafeHwnd() == m_Dlg.GetSafeHwnd())
//         m_pMenuDlgImpl = NULL;
// (It does not unhook m_hookMouse; only SetActiveMenu(NULL) does.)
// Symbol: ?OnDestroy@CDialogImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CDialogImpl__IEAAXXZ(void* pThis) {
    void* pMenuDlgImpl = impl__m_pMenuDlgImpl_CDialogImpl__1PEAV1_EA;
    if (pMenuDlgImpl != nullptr && SafeHwnd(Dlg(pMenuDlgImpl)) == SafeHwnd(Dlg(pThis))) {
        impl__m_pMenuDlgImpl_CDialogImpl__1PEAV1_EA = nullptr;
    }
}

// Transcribed from RVA 0x3fc70 (mfc140u), in full:
//     if (m_Dlg.m_nFlags & WF_STAYACTIVE) bActive = TRUE;   // +0xa8 & 0x20
//     if (!m_Dlg.IsWindowEnabled()) bActive = FALSE;        // always called
// DEVIATION: m_nFlags is OpenMFC's named member (+0x50), as in OnActivate above.
// Symbol: ?OnNcActivate@CDialogImpl@@IEAAXAEAH@Z
extern "C" void MS_ABI impl__OnNcActivate_CDialogImpl__IEAAXAEAH_Z(void* pThis, int* pbActive) {
    CWnd* pDlg = Dlg(pThis);
    if ((pDlg->m_nFlags & kWF_STAYACTIVE) != 0) {
        *pbActive = TRUE;
    }
    if (!impl__IsWindowEnabled_CWnd__QEBAHXZ(pDlg)) {
        *pbActive = FALSE;
    }
}

// Transcribed from RVA 0x3f850 (mfc140u).  The switch on pMsg->message, case by case:
//   WM_KEYDOWN:                  pPopup = GetSafeActivePopupMenu(); if NULL -> FALSE;
//                                ::SendMessage(pPopup->m_hWnd, WM_KEYDOWN, (int)wParam, 0)
//                                (movslq: the low 32 bits, sign-extended); -> TRUE
//   WM_SYSKEYDOWN, WM_CONTEXTMENU: pPopup = GetSafeActivePopupMenu(); if NULL -> FALSE;
//                                if (wParam != VK_MENU) -> FALSE;
//                                ::SendMessage(pPopup->m_hWnd, WM_CLOSE, 0, 0); -> TRUE
//   WM_SYSKEYUP:                 -> GetSafeActivePopupMenu() != NULL
//   WM_NC{L,M,R}BUTTON{DOWN,UP}: pt = (short)LOWORD/HIWORD(lParam);
//                                -> ProcessMouseClick(pt) != 0
//   WM_LBUTTONDOWN, WM_RBUTTONDOWN, WM_RBUTTONUP, WM_MBUTTONDOWN, WM_MBUTTONUP
//   (WM_LBUTTONUP is NOT in the set):
//                                pt as above; pWnd = CWnd::FromHandle(pMsg->hwnd);
//                                if (pWnd != NULL && ::IsWindow(pMsg->hwnd))
//                                    ::ClientToScreen(pWnd->m_hWnd, &pt);
//                                if (ProcessMouseClick(pt)) -> TRUE;
//                                -> !::IsWindow(pMsg->hwnd)
//   WM_MOUSEMOVE:                pt as above; pWnd = CWnd::FromHandle(pMsg->hwnd);
//                                if (pWnd != NULL) ::ClientToScreen(pWnd->m_hWnd, &pt);
//                                -> ProcessMouseMove(pt) != 0
//   WM_MOUSEWHEEL:               pPopup = GetSafeActivePopupMenu();
//                                if (pPopup != NULL && pPopup->m_bScrollable (+0x16a4))
//                                    ::SendMessage(pPopup->m_hWnd, WM_MOUSEWHEEL, wParam, lParam);
//                                -> FALSE (also after forwarding)
//   anything else:               -> FALSE
// (Import slots of the mfc140 image: SendMessageA 0x1802c5378 = SendMessageW in
// mfc140u, IsWindow 0x1802c5390, ClientToScreen 0x1802c5270.)  CAVEAT: the +0x16a4
// read is the retail CMFCPopupMenu layout, read the same way by CFrameImpl.cpp's
// ProcessMouseWheel; OpenMFC's own 0x238-byte popup objects do not reach that far.
// Symbol: ?PreTranslateMessage@CDialogImpl@@IEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CDialogImpl__IEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg) {
    switch (pMsg->message) {
    case WM_KEYDOWN: {
        CMFCPopupMenu* pPopup = impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
        if (pPopup == nullptr) return FALSE;
        ::SendMessage(HWndOf(pPopup), WM_KEYDOWN,
                      static_cast<WPARAM>(static_cast<INT_PTR>(static_cast<int>(pMsg->wParam))), 0);
        return TRUE;
    }
    case WM_SYSKEYDOWN:
    case WM_CONTEXTMENU: {
        CMFCPopupMenu* pPopup = impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
        if (pPopup == nullptr) return FALSE;
        if (pMsg->wParam != VK_MENU) return FALSE;
        ::SendMessage(HWndOf(pPopup), WM_CLOSE, 0, 0);
        return TRUE;
    }
    case WM_SYSKEYUP:
        return impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ() != nullptr ? TRUE : FALSE;

    case WM_NCLBUTTONDOWN:
    case WM_NCLBUTTONUP:
    case WM_NCRBUTTONDOWN:
    case WM_NCRBUTTONUP:
    case WM_NCMBUTTONDOWN:
    case WM_NCMBUTTONUP: {
        POINT pt;
        pt.x = static_cast<short>(LOWORD(pMsg->lParam));
        pt.y = static_cast<short>(HIWORD(pMsg->lParam));
        return impl__ProcessMouseClick_CDialogImpl__IEAAHUtagPOINT___Z(pThis, PackPoint(pt)) != 0 ? TRUE : FALSE;
    }

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP: {
        POINT pt;
        pt.x = static_cast<short>(LOWORD(pMsg->lParam));
        pt.y = static_cast<short>(HIWORD(pMsg->lParam));
        CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(pMsg->hwnd);
        if (pWnd != nullptr && ::IsWindow(pMsg->hwnd)) {
            ::ClientToScreen(pWnd->m_hWnd, &pt);
        }
        if (impl__ProcessMouseClick_CDialogImpl__IEAAHUtagPOINT___Z(pThis, PackPoint(pt)) != 0) {
            return TRUE;
        }
        return ::IsWindow(pMsg->hwnd) ? FALSE : TRUE;
    }

    case WM_MOUSEMOVE: {
        POINT pt;
        pt.x = static_cast<short>(LOWORD(pMsg->lParam));
        pt.y = static_cast<short>(HIWORD(pMsg->lParam));
        CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(pMsg->hwnd);
        if (pWnd != nullptr) {
            ::ClientToScreen(pWnd->m_hWnd, &pt);
        }
        return impl__ProcessMouseMove_CDialogImpl__IEAAHUtagPOINT___Z(pThis, PackPoint(pt)) != 0 ? TRUE : FALSE;
    }

    case WM_MOUSEWHEEL: {
        CMFCPopupMenu* pPopup = impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
        if (pPopup != nullptr && At<int>(pPopup, kOffPopupScrollable) != 0) {
            ::SendMessage(HWndOf(pPopup), WM_MOUSEWHEEL, pMsg->wParam, pMsg->lParam);
        }
        return FALSE;
    }

    default:
        return FALSE;
    }
}

// Transcribed from RVA 0x3f590 (mfc140u):
//     CMFCPopupMenu* pActive = CMFCPopupMenu::GetSafeActivePopupMenu();   // called first
//     if (CMFCToolBar::m_bCustomizeMode || pActive == NULL) return FALSE;
//     MENUAREA_TYPE area = pActive->CheckArea(pt);
//     if (area == SHADOW_RIGHT || area == SHADOW_BOTTOM) {
//         ::SendMessage(pActive->m_hWnd, WM_CLOSE, 0, 0); m_Dlg.SetFocus(); return TRUE;
//     }
//     if (area != OUTSIDE) return FALSE;
//     CMFCToolBarMenuButton* pParentBtn = pActive->m_pParentBtn;           // +0x228
//     if (pParentBtn != NULL && pParentBtn->m_pWndParent != NULL) {        // +0x80
//         CWnd* pWndParent = pParentBtn->m_pWndParent;
//         CMFCPopupMenuBar* pParentBar =
//             pWndParent->IsKindOf(RUNTIME_CLASS(CMFCPopupMenuBar)) ? pWndParent : NULL;
//         CPoint ptClient = pt; ::ScreenToClient(pWndParent->m_hWnd, &ptClient);
//         if (::PtInRect(&pParentBtn->m_rect, ptClient)) {                // +0x68
//             if (pParentBar == NULL && !pActive->InCommand())            // vslot 0x3a8
//                 ::SendMessage(pActive->m_hWnd, WM_CLOSE, 0, 0);
//             return TRUE;
//         }
//         if (pParentBar != NULL) {
//             pParentBar->CloseDelayedSubMenu();                          // bar vslot 0x890
//             CWnd* pGrand = CWnd::FromHandle(::GetParent(pParentBar->m_hWnd));
//             if (pGrand != NULL && pGrand->IsKindOf(RUNTIME_CLASS(CMFCPopupMenu))) {
//                 switch (pGrand->CheckArea(pt)) {
//                 case MENU: case LOGO: case TEAROFF_CAPTION: return FALSE;
//                 case SHADOW_RIGHT: case SHADOW_BOTTOM:
//                     ::SendMessage(pGrand->m_hWnd, WM_CLOSE, 0, 0); m_Dlg.SetFocus(); return TRUE;
//                 }                                                       // OUTSIDE: fall through
//             }
//         }
//     }
//     CMFCPopupMenu* pMenu = CMFCPopupMenu::GetSafeActivePopupMenu();      // re-fetched
//     if (pMenu != NULL && !pMenu->InCommand()) {                         // vslot 0x3a8
//         ::SendMessage(pMenu->m_hWnd, WM_CLOSE, 0, 0);
//         CWnd* pFocus = CWnd::FromHandle(::GetFocus());
//         if (pFocus != NULL && pFocus->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) m_Dlg.SetFocus();
//     }
//     return FALSE;
// (MENUAREA_TYPE values per afxpopupmenu.h: MENU 0, SHADOW_RIGHT 1, SHADOW_BOTTOM 2,
// LOGO 3, OUTSIDE 4, TEAROFF_CAPTION 5.  Import slots of the mfc140 image:
// ScreenToClient 0x1802c5340, PtInRect 0x1802c5320, SendMessageA 0x1802c5378 (W in
// mfc140u), GetParent 0x1802c5300, GetFocus 0x1802c51d8.)
// Symbol: ?ProcessMouseClick@CDialogImpl@@IEAAHUtagPOINT@@@Z
extern "C" int MS_ABI impl__ProcessMouseClick_CDialogImpl__IEAAHUtagPOINT___Z(void* pThis, long long ptPacked) {
    const POINT pt = UnpackPoint(ptPacked);
    CMFCPopupMenu* pActive = impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 || pActive == nullptr) {
        return FALSE;
    }

    const CPoint ptScreen(pt);
    const int area = impl__CheckArea_CMFCPopupMenu__QEBA_AW4MENUAREA_TYPE_1_AEBVCPoint___Z(pActive, ptScreen);
    if (area == kAreaShadowRight || area == kAreaShadowBottom) {
        ::SendMessage(HWndOf(pActive), WM_CLOSE, 0, 0);
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(Dlg(pThis));
        return TRUE;
    }
    if (area != kAreaOutside) {
        return FALSE;
    }

    CMFCToolBarButton* pParentBtn = At<CMFCToolBarButton*>(pActive, kOffPopupParentBtn);
    if (pParentBtn != nullptr && pParentBtn->m_pWndParent != nullptr) {
        CWnd* pWndParent = pParentBtn->m_pWndParent;
        CWnd* pParentBar =
            IsKindOfClass(pWndParent, impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ()) ? pWndParent
                                                                                                        : nullptr;
        POINT ptClient = pt;
        ::ScreenToClient(pWndParent->m_hWnd, &ptClient);
        if (::PtInRect(reinterpret_cast<const RECT*>(&pParentBtn->m_rect), ptClient)) {
            if (pParentBar == nullptr && !PopupInCommand(pActive)) {
                ::SendMessage(HWndOf(pActive), WM_CLOSE, 0, 0);
            }
            return TRUE;
        }
        if (pParentBar != nullptr) {
            BarCloseDelayedSubMenu(pParentBar);
            CWnd* pGrand = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pParentBar->m_hWnd));
            if (pGrand != nullptr &&
                IsKindOfClass(pGrand, impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ())) {
                switch (impl__CheckArea_CMFCPopupMenu__QEBA_AW4MENUAREA_TYPE_1_AEBVCPoint___Z(pGrand, ptScreen)) {
                case kAreaMenu:
                case kAreaLogo:
                case kAreaTearOffCaption:
                    return FALSE;
                case kAreaShadowRight:
                case kAreaShadowBottom:
                    ::SendMessage(pGrand->m_hWnd, WM_CLOSE, 0, 0);
                    impl__SetFocus_CWnd__QEAAPEAV1_XZ(Dlg(pThis));
                    return TRUE;
                default:
                    break;
                }
            }
        }
    }

    CMFCPopupMenu* pMenu = impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
    if (pMenu != nullptr && !PopupInCommand(pMenu)) {
        ::SendMessage(HWndOf(pMenu), WM_CLOSE, 0, 0);
        CWnd* pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
        if (pFocus != nullptr &&
            IsKindOfClass(pFocus, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
            impl__SetFocus_CWnd__QEAAPEAV1_XZ(Dlg(pThis));
        }
    }
    return FALSE;
}

// Transcribed from RVA 0x3f7a0 (mfc140u); `this` is never read:
//     if (CMFCToolBar::m_bCustomizeMode) return FALSE;
//     if (CMFCPopupMenu::m_pActivePopupMenu == NULL) return FALSE;
//     CMFCPopupMenu* pPopup = CMFCPopupMenu::GetSafeActivePopupMenu();
//     if (pPopup == NULL) return FALSE;
//     CRect rect; ::GetWindowRect(pPopup->m_hWnd, &rect);                 // zeroed first
//     if (::PtInRect(&rect, pt)) return FALSE;
//     if (pPopup->GetMenuBar()->FindDestintationToolBar(pt) != NULL) return FALSE;  // vslot 0x3a0
//     return TRUE;
// CAVEAT for an own popup, as in CFrameImpl.cpp's ProcessMouseMove: OpenMFC's
// ??0CMFCPopupMenu thunk does not construct the embedded bar that GetMenuBar returns.
// Symbol: ?ProcessMouseMove@CDialogImpl@@IEAAHUtagPOINT@@@Z
extern "C" int MS_ABI impl__ProcessMouseMove_CDialogImpl__IEAAHUtagPOINT___Z(void* pThis, long long ptPacked) {
    (void)pThis;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) {
        return FALSE;
    }
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA == nullptr) {
        return FALSE;
    }
    CMFCPopupMenu* pPopup = impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ();
    if (pPopup == nullptr) {
        return FALSE;
    }
    RECT rect = {};
    ::GetWindowRect(HWndOf(pPopup), &rect);
    if (::PtInRect(&rect, UnpackPoint(ptPacked))) {
        return FALSE;
    }
    if (impl__FindDestintationToolBar_CMFCPopupMenuBar__QEAAPEAVCMFCToolBar__VCPoint___Z(PopupMenuBar(pPopup),
                                                                                       ptPacked) != nullptr) {
        return FALSE;
    }
    return TRUE;
}

// Transcribed from RVA 0x3fb30 (mfc140u), in full:
//     CMFCPopupMenu::m_pActivePopupMenu = pMenu;                  // unconditional, first
//     if (pMenu != NULL) {
//         if (m_hookMouse == NULL)
//             m_hookMouse = ::SetWindowsHookEx(WH_MOUSE, DialogMouseHookProc, NULL,
//                                              ::GetCurrentThreadId());
//         m_pMenuDlgImpl = this;
//     } else {
//         if (m_hookMouse != NULL) { ::UnhookWindowsHookEx(m_hookMouse); m_hookMouse = NULL; }
//         m_pMenuDlgImpl = NULL;
//     }
// (Import slots of the mfc140 image: GetCurrentThreadId 0x1802c4648, SetWindowsHookExA
// 0x1802c4cf0 = SetWindowsHookExW in mfc140u, UnhookWindowsHookEx 0x1802c4cf8.)
// NOTE: retail writes the exported static CMFCPopupMenu::m_pActivePopupMenu, and that is
// what is written here.  OpenMFC's CMFCPopupMenu::GetSafeActivePopupMenu (the C++ body in
// featurepack/menu/CMFCPopupMenu.cpp) currently returns a separate file-level
// g_pActivePopupMenu instead of that static, so a menu registered here is not seen by
// the GetSafeActivePopupMenu calls above until that is reconciled.
// Symbol: ?SetActiveMenu@CDialogImpl@@IEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__SetActiveMenu_CDialogImpl__IEAAXPEAVCMFCPopupMenu___Z(void* pThis, CMFCPopupMenu* pMenu) {
    impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA = pMenu;
    if (pMenu != nullptr) {
        if (impl__m_hookMouse_CDialogImpl__1PEAUHHOOK____EA == nullptr) {
            impl__m_hookMouse_CDialogImpl__1PEAUHHOOK____EA = ::SetWindowsHookEx(
                WH_MOUSE, reinterpret_cast<HOOKPROC>(&impl__DialogMouseHookProc_CDialogImpl__KA_JH_K_J_Z), nullptr,
                ::GetCurrentThreadId());
        }
        impl__m_pMenuDlgImpl_CDialogImpl__1PEAV1_EA = pThis;
    } else {
        if (impl__m_hookMouse_CDialogImpl__1PEAUHHOOK____EA != nullptr) {
            ::UnhookWindowsHookEx(impl__m_hookMouse_CDialogImpl__1PEAUHHOOK____EA);
            impl__m_hookMouse_CDialogImpl__1PEAUHHOOK____EA = nullptr;
        }
        impl__m_pMenuDlgImpl_CDialogImpl__1PEAV1_EA = nullptr;
    }
}

// static HHOOK CDialogImpl::m_hookMouse -- RVA 0x3be1e0 (mfc140u), .data, initial value 0
// (read from the image).  Set/cleared only by SetActiveMenu, read by the hook proc.
// Symbol: ?m_hookMouse@CDialogImpl@@1PEAUHHOOK__@@EA
extern "C" HHOOK impl__m_hookMouse_CDialogImpl__1PEAUHHOOK____EA = nullptr;

// static CDialogImpl* CDialogImpl::m_pMenuDlgImpl -- RVA 0x3be1d8 (mfc140u), .data,
// initial value 0 (read from the image).  Written by SetActiveMenu and OnDestroy.
// Symbol: ?m_pMenuDlgImpl@CDialogImpl@@1PEAV1@EA
extern "C" void* impl__m_pMenuDlgImpl_CDialogImpl__1PEAV1_EA = nullptr;
