// CMFCToolBarComboBoxEdit — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Every body marked "Retail (RVA 0x..., mfc140u)" below was transcribed from the
// disassembly of that export in mfc140u.dll (14.51.36231; entry RVAs resolved
// through the export ordinal table).  The class is the edit child a flat
// CMFCToolBarComboBoxButton creates over its combo (SDK afxtoolbarcomboboxbutton.h:
// `class CMFCToolBarComboBoxEdit : public CEdit`).  It is NOT declared in
// include/openmfc, so `this` is a void* and the layout is pinned here from the
// retail constructor ??0CMFCToolBarComboBoxEdit (RVA 0x1649e0, mfc140u), which
// calls ??0CWnd@@QEAA@XZ (CEdit's constructor is inline), installs the class
// vftable (0x180316bc8, mfc140u), then stores:
//
//   CEdit / CWnd base ................................ +0x00 .. +0xe8
//   +0xe8  CMFCToolBarComboBoxButton& m_combo   (a pointer; the ctor argument)
//   +0xf0  BOOL                       m_bTracked (ctor stores 0)
//   sizeof == 0xf8 (the `new` size CreateEdit, RVA 0x1641d0 mfc140u, allocates)
//
// The owner button's fields these bodies read, at the offsets
// featurepack/toolbar/CMFCToolBarComboBoxButton.cpp documents and every body
// below uses (CMFCToolBarButton::m_nID +0x24, m_uiMenuResID +0xa0,
// m_rectCombo +0xa8, m_pWndCombo +0xd0, m_strEdit +0xd8).
//
// Button virtuals (retail vftable 0x180316ea8, mfc140u; read from the call sites
// below as offset / 8):
//   +0x080 (16) GetHwnd        -- header inline, body 0x8a960 (mfc140u): m_pWndCombo->GetSafeHwnd()
//   +0x090 (18) NotifyCommand  -- the export ?NotifyCommand@CMFCToolBarComboBoxButton@@UEAAHH@Z
//   +0x1e8 (61) GetPrompt      -- header inline, body 0x161430 (mfc140u): return CString()
// OpenMFC-built buttons carry a mingw vtable whose slot order is not retail's,
// so each virtual is dispatched with the VptrInThisModule test used by
// core/frame/CMDITabProxyWnd.cpp: an object whose vptr points into this DLL
// gets the base body (the export thunk, or the header inline); an object whose
// vptr points into a client module (a client subclass, MSVC vftable) is called
// through the retail slot.  Taking the base body for every in-DLL object is
// exact for the retail subclasses too: the CMFCToolBarFontComboBox (0x180318248)
// and CMFCToolBarFontSizeComboBox (0x180318050) vftables (mfc140u) carry the
// same three entries at slots 16, 18 and 61.
//
// DEVIATIONS shared by every body in this file:
//  * `this` and m_combo are null-checked; retail dereferences both unconditionally.
//    (PreTranslateMessage also returns FALSE for a NULL pMsg, which retail reads
//    unconditionally.)
//  * Base-class calls (CWnd::Default, CWnd::OnSetFocus, CWnd::PreTranslateMessage,
//    ...) go through their exported thunks, exactly as retail's direct calls do.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>
#include <new>

// ---------------------------------------------------------------------------
// Thunks this file calls (definitions named on each line; parameter lists are
// derived from the mangled names).
// ---------------------------------------------------------------------------
// core/window/CtorDtorPlacement.cpp
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
// core/controls/RuntimeClasses.cpp
extern "C" void MS_ABI impl___1CEdit__UEAA_XZ(CEdit* pThis);
// core/window/Thunks.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(CWnd* pThis, void* pOldWnd);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
// core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg);
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, CString* pString);
// featurepack/toolbar/CMFCToolBarComboBoxButton.cpp
extern "C" void MS_ABI impl__SetHotEdit_CMFCToolBarComboBoxButton__IEAAXH_Z(CMFCToolBarComboBoxButton* pThis, int bHot);
extern "C" int MS_ABI impl__NotifyCommand_CMFCToolBarComboBoxButton__UEAAHH_Z(CMFCToolBarComboBoxButton* pThis, int iNotifyCode);
// core/runtime/Globals.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType);
// featurepack/menu/CContextMenuManager.cpp
extern "C" CMFCPopupMenu* MS_ABI impl__ShowPopupMenu_CContextMenuManager__UEAAPEAVCMFCPopupMenu__PEAUHMENU____HHPEAVCWnd__HHH_Z(
    CContextMenuManager* pThis, HMENU hmenuPopup, int x, int y, CWnd* pWndOwner,
    int bOwnMessage, int bAutoDestroy, int bRightAlign);
// detail/RegcoreSupport.cpp, core/runtime/CObject.cpp, core/app/CWinAppEx.cpp
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ();
// core/gdi/CPaintDC.cpp
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);
// core/gdi/CDC.cpp, detail/GdicoreSupport.cpp
extern "C" int MS_ABI impl__SetBkMode_CDC__QEAAHH_Z(CDC* pThis, int nBkMode);
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor);
extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC* pThis, CFont* pFont);
extern "C" int MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat);
// core/runtime/AFX_GLOBAL_DATA.cpp, featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];   // 0x1803c1620 (mfc140u)

namespace {

// --- this class ------------------------------------------------------------
constexpr std::size_t kOffCombo   = 0xe8;   // CMFCToolBarComboBoxButton& m_combo
constexpr std::size_t kOffTracked = 0xf0;   // BOOL m_bTracked
static_assert(sizeof(CWnd) == kOffCombo, "m_combo follows the CEdit (== CWnd) base at +0xe8");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40 (every retail body reads the HWND there)");

// --- the owner button (see the file header) -------------------------------
constexpr std::size_t kBtnOffMenuResID = 0xa0;   // UINT m_uiMenuResID
constexpr std::size_t kBtnOffRectCombo = 0xa8;   // CRect m_rectCombo
constexpr std::size_t kBtnOffWndCombo  = 0xd0;   // CComboBox* m_pWndCombo
constexpr std::size_t kBtnOffStrEdit   = 0xd8;   // CString m_strEdit
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "CMFCToolBarButton::m_nID @0x24");
static_assert(sizeof(CString) == sizeof(void*), "CString is one pointer (m_strEdit occupies +0xd8..+0xe0)");
static_assert(sizeof(CMFCToolBarComboBoxButton) >= kBtnOffStrEdit + sizeof(CString),
              "the header's CMFCToolBarComboBoxButton covers the retail fields read here");

// --- CWnd::m_hWndOwner, read by the inline CWnd::GetOwner -------------------
// Retail reads it at CWnd+0xa0 (`m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd)`).
// OpenMFC's CWnd has no named member there; the slot lies in _cwnd_padding2,
// which the CWnd constructor zero-fills, so the ::GetParent fallback is taken
// unless something stored an owner (same approach as core/controlbar/CControlBar.cpp).
constexpr std::size_t kCWndOffHWndOwner = 0xa0;
static_assert(offsetof(CWnd, _cwnd_padding2) <= kCWndOffHWndOwner &&
              kCWndOffHWndOwner + sizeof(HWND) <= offsetof(CWnd, _cwnd_padding2) + sizeof(CWnd::_cwnd_padding2),
              "m_hWndOwner slot must lie inside CWnd's padding");

// --- afxGlobalData (offsets pinned in core/runtime/AFX_GLOBAL_DATA.cpp) -----
constexpr std::size_t kGlobalDataInitGate      = 0x000;   // m_bInitialized
constexpr std::size_t kGlobalDataClrGrayedText = 0x044;   // COLORREF clrGrayedText
constexpr std::size_t kGlobalDataBrWindow      = 0x108;   // CBrush brWindow (m_hObject at +0x110)
constexpr std::size_t kGlobalDataFontRegular   = 0x1a8;   // CFont fontRegular (m_hObject at +0x1b0)
static_assert(offsetof(CGdiObject, m_hObject) == 0x08, "CGdiObject::m_hObject @0x08");
static_assert(offsetof(CDC, m_hDC) == 0x08, "CDC::m_hDC @0x08");

// Win32 constants the bodies use (numeric values from the disassembly).
constexpr UINT kCbGetDroppedState = 0x157;   // CB_GETDROPPEDSTATE
constexpr UINT kCbShowDropDown    = 0x14f;   // CB_SHOWDROPDOWN
constexpr int  kCbnSetFocus       = 3;       // CBN_SETFOCUS
constexpr int  kCbnKillFocus      = 4;       // CBN_KILLFOCUS
constexpr int  kCbnEditChange     = 5;       // CBN_EDITCHANGE

inline unsigned char* Bytes(void* p) { return static_cast<unsigned char*>(p); }
inline CWnd* W(void* p) { return static_cast<CWnd*>(p); }
inline CMFCToolBarComboBoxButton* Combo(void* pThis) {
    CMFCToolBarComboBoxButton* p = nullptr;
    std::memcpy(&p, Bytes(pThis) + kOffCombo, sizeof p);
    return p;
}
inline int& Tracked(void* pThis) { return *reinterpret_cast<int*>(Bytes(pThis) + kOffTracked); }

inline unsigned char* BtnBytes(CMFCToolBarComboBoxButton* b) { return reinterpret_cast<unsigned char*>(b); }
inline UINT BtnMenuResID(CMFCToolBarComboBoxButton* b) {
    UINT v = 0;
    std::memcpy(&v, BtnBytes(b) + kBtnOffMenuResID, sizeof v);
    return v;
}
inline RECT* BtnRectCombo(CMFCToolBarComboBoxButton* b) { return reinterpret_cast<RECT*>(BtnBytes(b) + kBtnOffRectCombo); }
inline CWnd* BtnWndCombo(CMFCToolBarComboBoxButton* b) {
    CWnd* p = nullptr;
    std::memcpy(&p, BtnBytes(b) + kBtnOffWndCombo, sizeof p);
    return p;
}
inline CString* BtnStrEdit(CMFCToolBarComboBoxButton* b) { return reinterpret_cast<CString*>(BtnBytes(b) + kBtnOffStrEdit); }

// Retail's inline CWnd::GetOwner(): FromHandle(m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd)).
inline CWnd* OwnerOf(CWnd* pWnd) {
    HWND hOwner = nullptr;
    std::memcpy(&hOwner, reinterpret_cast<const unsigned char*>(pWnd) + kCWndOffHWndOwner, sizeof hOwner);
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hOwner != nullptr ? hOwner : ::GetParent(pWnd->m_hWnd));
}
// Retail's inline CWnd::GetParent(): FromHandle(::GetParent(m_hWnd)).
inline CWnd* ParentOf(CWnd* pWnd) {
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pWnd->m_hWnd));
}

// Retail's inline GetGlobalData() gate: if (!m_bInitialized) { Initialize(); m_bInitialized = TRUE; }
inline void EnsureGlobalDataInitialized() {
    int bInit = 0;
    std::memcpy(&bInit, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, sizeof bInit);
    if (bInit == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, &one, sizeof one);
    }
}
inline CGdiObject* GlobalGdiObject(std::size_t off) {
    return reinterpret_cast<CGdiObject*>(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off);
}

// ---------------------------------------------------------------------------
// Virtual dispatch on the owner button (file header).
// ---------------------------------------------------------------------------
HMODULE ModuleOfAddress(const void* p) {
    HMODULE h = nullptr;
    if (!::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                  GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                              static_cast<LPCWSTR>(p), &h)) {
        return nullptr;
    }
    return h;
}
// Does the object's vptr point into this DLL's image (an object OpenMFC built,
// mingw vtable) rather than into a client module (MSVC vftable)?
bool VptrInThisModule(const void* pObj) {
    HMODULE self = ModuleOfAddress(reinterpret_cast<const void*>(&ModuleOfAddress));
    return self != nullptr && ModuleOfAddress(*static_cast<void* const*>(pObj)) == self;
}
// Entry at vftable byte offset `off` (retail's `mov off(%rax),%rax`).
inline void* SlotAt(const void* p, std::size_t off) {
    return *reinterpret_cast<void* const*>(*static_cast<const unsigned char* const*>(p) + off);
}

using PFN_GetHwnd       = HWND (MS_ABI*)(void*);                 // +0x080
using PFN_NotifyCommand = int (MS_ABI*)(void*, int);             // +0x090
using PFN_GetPrompt     = CString* (MS_ABI*)(const void*, CString*);   // +0x1e8, sret in RDX (retail call site)

// m_combo.GetHwnd() -- vslot 0x80 (16).  Base: the header inline, m_pWndCombo->GetSafeHwnd().
HWND ButtonGetHwnd(CMFCToolBarComboBoxButton* b) {
    if (!VptrInThisModule(b)) return reinterpret_cast<PFN_GetHwnd>(SlotAt(b, 0x80))(b);
    CWnd* pCombo = BtnWndCombo(b);
    return pCombo != nullptr ? pCombo->m_hWnd : nullptr;
}
// m_combo.NotifyCommand(iNotifyCode) -- vslot 0x90 (18).  Base: the export.
int ButtonNotifyCommand(CMFCToolBarComboBoxButton* b, int iNotifyCode) {
    if (!VptrInThisModule(b)) return reinterpret_cast<PFN_NotifyCommand>(SlotAt(b, 0x90))(b, iNotifyCode);
    return impl__NotifyCommand_CMFCToolBarComboBoxButton__UEAAHH_Z(b, iNotifyCode);
}
// new (pOut) CString(m_combo.GetPrompt()) -- vslot 0x1e8 (61).  Base: the header
// inline, which returns an empty CString.  pOut is uninitialised storage; the
// caller destroys the string.
void ButtonGetPrompt(CMFCToolBarComboBoxButton* b, void* pOut) {
    if (!VptrInThisModule(b)) {
        reinterpret_cast<PFN_GetPrompt>(SlotAt(b, 0x1e8))(b, static_cast<CString*>(pOut));
        return;
    }
    new (pOut) CString();
}
// Holds the by-value CString GetPrompt returns (retail's stack temporary).
struct PromptTemp {
    alignas(CString) unsigned char storage[sizeof(CString)];
    explicit PromptTemp(CMFCToolBarComboBoxButton* b) { ButtonGetPrompt(b, storage); }
    ~PromptTemp() { Str().~CString(); }
    CString& Str() { return *reinterpret_cast<CString*>(storage); }
    PromptTemp(const PromptTemp&) = delete;
    PromptTemp& operator=(const PromptTemp&) = delete;
};

// Retail's afxContextMenuManager (mfc140u .data 0x3be1b0, not exported).  OpenMFC
// publishes no such global; as in featurepack/menu/CMFCMenuButton.cpp the closest
// reachable equivalent is the manager the current CWinAppEx owns, read directly
// (GetContextMenuManager would create one on demand and turn retail's "none
// exists" gate into "always").
struct WinAppExAccess : CWinAppEx {
    using CWinAppEx::m_pContextMenuManager;
};
CContextMenuManager* CurrentContextMenuManager() {
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return static_cast<WinAppExAccess*>(static_cast<CWinAppEx*>(pApp))->m_pContextMenuManager;
}

// The vptr ??0CWnd@@QEAA@XZ installs, recorded by the first constructor to run
// (see the constructor and destructor below).
void* g_ownVptr = nullptr;

} // namespace

// Retail (RVA 0x1649e0, mfc140u), fully transcribed:
//     CWnd::CWnd();                             // 0x28a700 (mfc140u); CEdit's ctor is inline
//     vptr      = CMFCToolBarComboBoxEdit::`vftable' (0x180316bc8, mfc140u);
//     m_combo   = &combo;                       // +0xe8
//     m_bTracked = FALSE;                       // +0xf0
//     return this;
// DEVIATION: OpenMFC has no CMFCToolBarComboBoxEdit vftable, so the object keeps
// the vptr the exported CWnd constructor installs (OpenMFC's CWnd one): its
// PreTranslateMessage override and message map are not reached by virtual
// dispatch, only through the exports below.  That vptr is recorded so the
// destructor can restore it (mirroring retail's own vptr store there).
// Symbol: ??0CMFCToolBarComboBoxEdit@@QEAA@AEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarComboBoxEdit__QEAA_AEAVCMFCToolBarComboBoxButton___Z(void* pThis, void* pButton) {
    if (pThis == nullptr) return pThis;
    impl___0CWnd__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    std::memcpy(Bytes(pThis) + kOffCombo, &pButton, sizeof pButton);
    Tracked(pThis) = FALSE;
    return pThis;
}

// Retail (RVA 0x164a70, mfc140u), fully transcribed:
//     vptr = CMFCToolBarComboBoxEdit::`vftable' (0x180316bc8, mfc140u);
//     jmp CEdit::~CEdit                         // 0x294370 (mfc140u)
// DEVIATION: the vptr stored before ~CEdit runs is the one this file's
// constructor installed (g_ownVptr, OpenMFC's CWnd vftable), not a class
// vftable OpenMFC does not have.  Here the store matters for a second reason:
// the exported ~CEdit thunk (core/controls/RuntimeClasses.cpp) destroys through
// an unqualified, hence virtual, `pThis->~CEdit()` call, which must see an
// OpenMFC (mingw) vptr rather than a client subclass's MSVC one.  The store is
// skipped if no constructor has run in this process.
// Symbol: ??1CMFCToolBarComboBoxEdit@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarComboBoxEdit__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    impl___1CEdit__UEAA_XZ(static_cast<CEdit*>(W(pThis)));
}

// Retail (RVA 0x164f00, mfc140u), fully transcribed:
//     m_combo.NotifyCommand(CBN_EDITCHANGE);    // vslot 0x90 (18), tail jump
// Symbol: ?OnChange@CMFCToolBarComboBoxEdit@@IEAAXXZ
extern "C" void MS_ABI impl__OnChange_CMFCToolBarComboBoxEdit__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CMFCToolBarComboBoxButton* pCombo = Combo(pThis);
    if (pCombo == nullptr) return;
    ButtonNotifyCommand(pCombo, kCbnEditChange);
}

// Retail (RVA 0x164fc0, mfc140u), fully transcribed:
//     if (m_combo.m_uiMenuResID != 0) {                                        // +0xa0
//         CWnd* pWndParent = pWnd->GetParent();     // FromHandle(::GetParent(pWnd->m_hWnd)); pWnd not null-checked
//         HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE((WORD)m_uiMenuResID), RT_MENU);
//         if (hInst != NULL) {
//             HMENU hMenu = ::LoadMenuW(hInst, MAKEINTRESOURCE((WORD)m_uiMenuResID));
//             if (hMenu != NULL) {
//                 HMENU hPopup = ::GetSubMenu(hMenu, 0);
//                 if (hPopup != NULL) {
//                     if (afxContextMenuManager != NULL)                          // 0x3be1b0 (mfc140u)
//                         afxContextMenuManager->ShowPopupMenu(hPopup, point.x, point.y, pWndParent,
//                                                              FALSE, TRUE, FALSE);   // its vslot 5 (+0x28)
//                     else
//                         ::TrackPopupMenu(hPopup, TPM_CENTERALIGN, point.x, point.y, 0,
//                                          pWndParent->GetSafeHwnd(), NULL);
//                     return;
//                 }
//             }
//         }
//     }
//     Default();
// (hMenu is never destroyed on any path; that is retail's behaviour and is kept.)
// DEVIATIONS: afxContextMenuManager is replaced by CurrentContextMenuManager()
// (see its comment); its ShowPopupMenu goes through the exported thunk, which
// re-dispatches on OpenMFC's own CContextMenuManager vtable, not through retail's
// MSVC slot 5; a null pWnd skips the parent lookup (pWndParent = NULL) where
// retail would fault.
// Symbol: ?OnContextMenu@CMFCToolBarComboBoxEdit@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CMFCToolBarComboBoxEdit__IEAAXPEAVCWnd__VCPoint___Z(void* pThis, CWnd* pWnd, long long point) {
    if (pThis == nullptr) return;
    CMFCToolBarComboBoxButton* pCombo = Combo(pThis);
    if (pCombo != nullptr && BtnMenuResID(pCombo) != 0) {
        CWnd* pWndParent = pWnd != nullptr ? ParentOf(pWnd) : nullptr;
        const int x = static_cast<int>(static_cast<unsigned long long>(point) & 0xffffffffu);
        const int y = static_cast<int>(static_cast<unsigned long long>(point) >> 32);
        HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
            MAKEINTRESOURCEW(static_cast<WORD>(BtnMenuResID(pCombo))), MAKEINTRESOURCEW(4) /*RT_MENU*/);
        if (hInst != nullptr) {
            HMENU hMenu = ::LoadMenuW(hInst, MAKEINTRESOURCEW(static_cast<WORD>(BtnMenuResID(pCombo))));
            if (hMenu != nullptr) {
                HMENU hPopup = ::GetSubMenu(hMenu, 0);
                if (hPopup != nullptr) {
                    if (CContextMenuManager* pManager = CurrentContextMenuManager()) {
                        impl__ShowPopupMenu_CContextMenuManager__UEAAPEAVCMFCPopupMenu__PEAUHMENU____HHPEAVCWnd__HHH_Z(
                            pManager, hPopup, x, y, pWndParent, FALSE, TRUE, FALSE);
                    } else {
                        ::TrackPopupMenu(hPopup, TPM_CENTERALIGN, x, y, 0,
                                         pWndParent != nullptr ? pWndParent->m_hWnd : nullptr, nullptr);
                    }
                    return;
                }
            }
        }
    }
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
}

// Retail (RVA 0x164e90, mfc140u), fully transcribed:
//     Default();                                     // CWnd::OnKillFocus is inline Default()
//     if (::IsWindow(m_combo.GetHwnd())) {           // vslot 0x80 (16)
//         m_combo.SetHotEdit(FALSE);
//         m_combo.NotifyCommand(CBN_KILLFOCUS);      // vslot 0x90 (18)
//     }
// (pNewWnd is not read.)
// Symbol: ?OnKillFocus@CMFCToolBarComboBoxEdit@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCToolBarComboBoxEdit__IEAAXPEAVCWnd___Z(void* pThis, CWnd* pNewWnd) {
    (void)pNewWnd;
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
    CMFCToolBarComboBoxButton* pCombo = Combo(pThis);
    if (pCombo == nullptr) return;
    if (::IsWindow(ButtonGetHwnd(pCombo))) {
        impl__SetHotEdit_CMFCToolBarComboBoxButton__IEAAXH_Z(pCombo, FALSE);
        ButtonNotifyCommand(pCombo, kCbnKillFocus);
    }
}

// Retail (RVA 0x164f80, mfc140u), fully transcribed:
//     m_bTracked = FALSE;                            // +0xf0
//     if (CWnd::FromHandle(::GetFocus()) != this)
//         m_combo.SetHotEdit(FALSE);
// (No Default() call.)
// Symbol: ?OnMouseLeave@CMFCToolBarComboBoxEdit@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCToolBarComboBoxEdit__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    Tracked(pThis) = FALSE;
    if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus()) != W(pThis)) {
        if (CMFCToolBarComboBoxButton* pCombo = Combo(pThis)) {
            impl__SetHotEdit_CMFCToolBarComboBoxButton__IEAAXH_Z(pCombo, FALSE);
        }
    }
}

// Retail (RVA 0x164f20, mfc140u), fully transcribed:
//     Default();                                     // CWnd::OnMouseMove is inline Default()
//     m_combo.SetHotEdit(TRUE);
//     if (!m_bTracked) {                             // +0xf0
//         m_bTracked = TRUE;
//         TRACKMOUSEEVENT tme;  tme.cbSize = sizeof(tme) /*0x18*/;  tme.dwFlags = TME_LEAVE;
//         tme.hwndTrack = m_hWnd;  ::TrackMouseEvent(&tme);   (dwHoverTime left unset)
//     }
// (nFlags and point are not read.)  The TRACKMOUSEEVENT is zero-initialised
// here, so dwHoverTime is 0 instead of stack garbage; TME_LEAVE ignores it.
// Symbol: ?OnMouseMove@CMFCToolBarComboBoxEdit@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCToolBarComboBoxEdit__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags; (void)point;
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
    if (CMFCToolBarComboBoxButton* pCombo = Combo(pThis)) {
        impl__SetHotEdit_CMFCToolBarComboBoxButton__IEAAXH_Z(pCombo, TRUE);
    }
    if (Tracked(pThis) == 0) {
        TRACKMOUSEEVENT tme = {};
        Tracked(pThis) = TRUE;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = W(pThis)->m_hWnd;
        ::TrackMouseEvent(&tme);
    }
}

// Retail (RVA 0x165660, mfc140u), fully transcribed:
//     CString str;  GetWindowText(str);
//     if (!str.IsEmpty() || m_combo.GetPrompt().IsEmpty()           // vslot 0x1e8 (61), temporary
//         || CWnd::FromHandle(::GetFocus()) == this) {
//         Default();            // direct call to ?Default@CWnd (0x28ac80, mfc140u) -- NOT
//         return;               // CWnd::OnPaint, which is an out-of-line export (0x290f10,
//                               // mfc140u) that also paints m_pCtrlCont (+0xc8) first
//     }
//     CRect rect(0, 0, 0, 0);  ::GetClientRect(m_hWnd, &rect);
//     CPaintDC dc(this);
//     ::FillRect(dc.m_hDC, &rect, afxGlobalData.brWindow);           // +0x110 (m_hObject)
//     dc.SetBkMode(TRANSPARENT);
//     dc.SetTextColor(afxGlobalData.clrGrayedText);                  // +0x44
//     CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular); // +0x1a8
//     rect.DeflateRect(1, 1);                                        // ::InflateRect(&rect, -1, -1)
//     dc.DrawText(m_combo.GetPrompt(), rect,                         // vslot 0x1e8 again; CDC vslot 0xe0 (28)
//                 DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX /*0x824*/);
//     dc.SelectObject(pOldFont);
// (every afxGlobalData read is behind the init gate.)
// Note: for a button OpenMFC built, GetPrompt is the base inline and returns an
// empty string, so this always takes the Default() path -- exactly what retail
// does for a button class that does not override GetPrompt.  The drawing path
// runs for a client subclass that overrides it.
// DEVIATION: CDC::DrawText goes through the in-tree helper
// impl__DrawTextW_CDC__ (CDC::DrawText is inline in the shipping headers), not
// through the CPaintDC's vslot 28.
// Symbol: ?OnPaint@CMFCToolBarComboBoxEdit@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCToolBarComboBoxEdit__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CWnd* pWnd = W(pThis);
    CMFCToolBarComboBoxButton* pCombo = Combo(pThis);

    CString str;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pWnd, &str);

    bool bDefault = true;
    if (str.GetLength() == 0 && pCombo != nullptr) {
        PromptTemp prompt(pCombo);
        if (prompt.Str().GetLength() != 0 &&
            impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus()) != pWnd) {
            bDefault = false;
        }
    }
    if (bDefault) {
        impl__Default_CWnd__IEAA_JXZ(pWnd);
        return;
    }

    RECT rect = {0, 0, 0, 0};
    ::GetClientRect(pWnd->m_hWnd, &rect);

    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, pWnd);

    EnsureGlobalDataInitialized();
    ::FillRect(pDC->m_hDC, &rect, static_cast<HBRUSH>(GlobalGdiObject(kGlobalDataBrWindow)->m_hObject));
    impl__SetBkMode_CDC__QEAAHH_Z(pDC, TRANSPARENT);
    EnsureGlobalDataInitialized();
    COLORREF clrGrayedText = 0;
    std::memcpy(&clrGrayedText, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataClrGrayedText, sizeof clrGrayedText);
    impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrGrayedText);
    EnsureGlobalDataInitialized();
    CFont* pOldFont = impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(
        pDC, static_cast<CFont*>(GlobalGdiObject(kGlobalDataFontRegular)));
    ::InflateRect(&rect, -1, -1);
    {
        PromptTemp prompt(pCombo);
        impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, static_cast<const wchar_t*>(prompt.Str()),
            prompt.Str().GetLength(), &rect, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
    }
    impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, pOldFont);
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// Retail (RVA 0x164e40, mfc140u), fully transcribed:
//     CWnd::OnSetFocus(pOldWnd);                     // direct call, pOldWnd passed through
//     m_combo.SetHotEdit(TRUE);
//     m_combo.NotifyCommand(CBN_SETFOCUS);           // vslot 0x90 (18), tail jump
// Symbol: ?OnSetFocus@CMFCToolBarComboBoxEdit@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCToolBarComboBoxEdit__IEAAXPEAVCWnd___Z(void* pThis, CWnd* pOldWnd) {
    if (pThis == nullptr) return;
    impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(W(pThis), pOldWnd);
    CMFCToolBarComboBoxButton* pCombo = Combo(pThis);
    if (pCombo == nullptr) return;
    impl__SetHotEdit_CMFCToolBarComboBoxButton__IEAAXH_Z(pCombo, TRUE);
    ButtonNotifyCommand(pCombo, kCbnSetFocus);
}

// Retail (RVA 0x164a90, mfc140u), fully transcribed (pCombo = m_combo.m_pWndCombo, +0xd0,
// re-read from the button at every use):
//     if (pMsg->message == WM_MOUSEWHEEL && pCombo != NULL && pCombo->GetDroppedState()) {
//     forward:
//         ::SendMessage(pCombo->m_hWnd, pMsg->message, pMsg->wParam, pMsg->lParam);
//         return TRUE;
//     }
//     if (pMsg->message != WM_KEYDOWN) return CWnd::PreTranslateMessage(pMsg);
//     if (::GetKeyState(VK_MENU) >= 0 && ::GetKeyState(VK_CONTROL) >= 0 && pCombo != NULL) {
//         switch (pMsg->wParam) {
//         case VK_PRIOR: case VK_NEXT: case VK_END: case VK_HOME: case VK_UP: case VK_DOWN:
//             if (!pCombo->GetDroppedState()) break;          // on to the second switch
//             /* fall through */
//         case VK_RETURN:
//             SetFocus();
//             if (pCombo->GetDroppedState()) goto forward;
//             if (pCombo->GetOwner() == NULL) return TRUE;     // inline GetOwner (+0xa0 / ::GetParent)
//             GetWindowText(m_combo.m_strEdit);                // +0xd8
//             ::PostMessage(pCombo->GetOwner()->m_hWnd, WM_COMMAND,
//                           MAKEWPARAM((WORD)m_combo.m_nID, 0), (LPARAM)pCombo->GetSafeHwnd());   // +0x24
//             return TRUE;
//         }
//     }
//     switch (pMsg->wParam) {
//     case VK_TAB:
//         if (GetParent() == NULL) break;
//         GetParent()->GetNextDlgTabItem(this)->SetFocus();    // ::GetNextDlgTabItem(parent, m_hWnd, FALSE)
//         return TRUE;
//     case VK_ESCAPE:
//         if (pCombo != NULL) ::SendMessage(pCombo->m_hWnd, CB_SHOWDROPDOWN, FALSE, 0);
//         if (GetTopLevelFrame() == NULL) break;
//         GetTopLevelFrame()->SetFocus();
//         return TRUE;
//     case VK_UP: case VK_DOWN:
//         if (::GetKeyState(VK_MENU) < 0 || ::GetKeyState(VK_CONTROL) < 0 || pCombo == NULL) break;
//         if (!pCombo->GetDroppedState()) {
//             ::SendMessage(pCombo->m_hWnd, CB_SHOWDROPDOWN, TRUE, 0);
//             if (pCombo->GetParent() != NULL)
//                 ::InvalidateRect(pCombo->GetParent()->m_hWnd, &m_combo.m_rectCombo, TRUE);   // +0xa8
//         }
//         return TRUE;
//     }
//     return CWnd::PreTranslateMessage(pMsg);                  // direct call (CEdit has no override)
// DEVIATION: on VK_TAB a NULL next tab item skips SetFocus (retail calls
// CWnd::SetFocus on the NULL pointer) and still returns TRUE.
// Symbol: ?PreTranslateMessage@CMFCToolBarComboBoxEdit@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCToolBarComboBoxEdit__UEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg) {
    if (pThis == nullptr || pMsg == nullptr) return FALSE;
    CWnd* pWnd = W(pThis);
    CMFCToolBarComboBoxButton* pButton = Combo(pThis);
    if (pButton == nullptr) return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(pWnd, pMsg);

    auto droppedState = [pButton]() -> LRESULT {
        return ::SendMessage(BtnWndCombo(pButton)->m_hWnd, kCbGetDroppedState, 0, 0);
    };
    auto forwardToCombo = [pButton, pMsg]() -> int {
        ::SendMessage(BtnWndCombo(pButton)->m_hWnd, pMsg->message, pMsg->wParam, pMsg->lParam);
        return TRUE;
    };

    if (pMsg->message == WM_MOUSEWHEEL && BtnWndCombo(pButton) != nullptr && droppedState() != 0) {
        return forwardToCombo();
    }
    if (pMsg->message != WM_KEYDOWN) {
        return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(pWnd, pMsg);
    }

    if (::GetKeyState(VK_MENU) >= 0 && ::GetKeyState(VK_CONTROL) >= 0 && BtnWndCombo(pButton) != nullptr) {
        bool bCommit = false;
        switch (pMsg->wParam) {
        case VK_RETURN:
            bCommit = true;
            break;
        case VK_PRIOR: case VK_NEXT: case VK_END: case VK_HOME: case VK_UP: case VK_DOWN:
            bCommit = droppedState() != 0;
            break;
        default:
            break;
        }
        if (bCommit) {
            impl__SetFocus_CWnd__QEAAPEAV1_XZ(pWnd);
            if (droppedState() != 0) return forwardToCombo();
            if (OwnerOf(BtnWndCombo(pButton)) == nullptr) return TRUE;
            impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                pWnd, BtnStrEdit(pButton));
            CWnd* pOwner = OwnerOf(BtnWndCombo(pButton));
            CWnd* pCombo = BtnWndCombo(pButton);
            ::PostMessage(pOwner->m_hWnd, WM_COMMAND,
                          MAKEWPARAM(static_cast<WORD>(pButton->m_nID), 0),
                          reinterpret_cast<LPARAM>(pCombo != nullptr ? pCombo->m_hWnd : nullptr));
            return TRUE;
        }
    }

    switch (pMsg->wParam) {
    case VK_TAB: {
        if (ParentOf(pWnd) == nullptr) break;
        CWnd* pParent = ParentOf(pWnd);
        CWnd* pNext = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
            ::GetNextDlgTabItem(pParent->m_hWnd, pWnd->m_hWnd, FALSE));
        if (pNext != nullptr) impl__SetFocus_CWnd__QEAAPEAV1_XZ(pNext);
        return TRUE;
    }
    case VK_ESCAPE: {
        if (CWnd* pCombo = BtnWndCombo(pButton)) {
            ::SendMessage(pCombo->m_hWnd, kCbShowDropDown, FALSE, 0);
        }
        if (impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pWnd) == nullptr) break;
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(reinterpret_cast<CWnd*>(impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pWnd)));
        return TRUE;
    }
    case VK_UP:
    case VK_DOWN: {
        if (::GetKeyState(VK_MENU) < 0 || ::GetKeyState(VK_CONTROL) < 0) break;
        if (BtnWndCombo(pButton) == nullptr) break;
        if (droppedState() != 0) return TRUE;
        ::SendMessage(BtnWndCombo(pButton)->m_hWnd, kCbShowDropDown, TRUE, 0);
        if (ParentOf(BtnWndCombo(pButton)) == nullptr) return TRUE;
        ::InvalidateRect(ParentOf(BtnWndCombo(pButton))->m_hWnd, BtnRectCombo(pButton), TRUE);
        return TRUE;
    }
    default:
        break;
    }
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(pWnd, pMsg);
}
