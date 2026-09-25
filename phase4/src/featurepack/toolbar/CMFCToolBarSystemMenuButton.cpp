// CMFCToolBarSystemMenuButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Behaviour is transcribed from the retail mfc140u.dll exports (bodies read with
// the campaign disassembler; each body cites its mfc140u entry RVA).
//
// ---------------------------------------------------------------------------
// Layout.  OpenMFC's headers do not declare CMFCToolBarSystemMenuButton, so the
// object is handled as `void* pThis` and the layout is pinned here.
//
// Retail (afxtoolbarsystemmenubutton.h, on-host SDK 14.51) derives it from the
// 0x128-byte CMFCToolBarMenuButton and appends
//     HICON m_hSysMenuIcon  @0x128
//     HMENU m_hSystemMenu   @0x130
// for sizeof 0x138.  Evidence: the (HMENU, HICON) constructor (RVA 0x17f730,
// mfc140u) stores the HICON argument to +0x128 and the HMENU to +0x130;
// CreateFromMenu (RVA 0x17fa30) stores its HMENU to +0x130; CreateObject
// (RVA 0x17f630) allocates 0x138 bytes.  The runtime-class descriptor
// (featurepack/menu/RuntimeClasses.cpp) also records 312 == 0x138.
//
// Base-class members the retail bodies touch (retail CMFCToolBarMenuButton
// layout, as documented and pinned in featurepack/toolbar/CMFCToolBarMenuButton.cpp):
//     CMFCPopupMenu* m_pPopupMenu @0xc0,  BOOL m_bToBeClosed @0xdc
// Both lie inside OpenMFC's zero-initialised CMFCToolBarMenuButton::_pad
// (0x90..0x128), asserted below.
// CMFCPopupMenu::m_pMessageWnd is at +0x1f8 (pinned as kOffMessageWnd in
// featurepack/menu/CMFCPopupMenu.cpp against detail/CMFCPopupMenuSupport.h).
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

namespace {
constexpr std::size_t kSmbSysMenuIcon  = 0x128;   // HICON m_hSysMenuIcon
constexpr std::size_t kSmbSystemMenu   = 0x130;   // HMENU m_hSystemMenu
constexpr std::size_t kSmbObjectSize   = 0x138;   // sizeof(CMFCToolBarSystemMenuButton)
constexpr std::size_t kMbPopupMenu     = 0xc0;    // CMFCToolBarMenuButton::m_pPopupMenu
constexpr std::size_t kMbToBeClosed    = 0xdc;    // CMFCToolBarMenuButton::m_bToBeClosed
constexpr std::size_t kWndHWnd         = 0x40;    // CWnd::m_hWnd
constexpr std::size_t kPopupMessageWnd = 0x1f8;   // CMFCPopupMenu::m_pMessageWnd

static_assert(sizeof(CMFCToolBarMenuButton) == kSmbSysMenuIcon,
              "retail CMFCToolBarMenuButton is 0x128 bytes; the derived members follow it");
static_assert(kSmbSystemMenu == kSmbSysMenuIcon + sizeof(HICON) &&
              kSmbObjectSize == kSmbSystemMenu + sizeof(HMENU),
              "retail CMFCToolBarSystemMenuButton: HICON @0x128, HMENU @0x130, sizeof 0x138");
static_assert(offsetof(CMFCToolBarMenuButton, _pad) == 0x90 &&
              kMbToBeClosed + sizeof(BOOL) <= sizeof(CMFCToolBarMenuButton),
              "m_pPopupMenu / m_bToBeClosed lie inside OpenMFC's zeroed _pad");
static_assert(offsetof(CWnd, m_hWnd) == kWndHWnd, "CWnd::m_hWnd @0x40");
static_assert(offsetof(CDC, m_hDC) == 0x08, "CDC::m_hDC @0x08");

template <class T> inline T& SmbAt(void* p, std::size_t off) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}
template <class T> inline T SmbGet(const void* p, std::size_t off) {
    T v;
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof v);
    return v;
}
} // namespace

// ---------------------------------------------------------------------------
// Cross-file thunks (C++ methods exist only as these).  Parameter lists are the
// ones the mangled names describe; each definition was checked where noted.
// ---------------------------------------------------------------------------
//   featurepack/toolbar/Thunks.cpp
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCToolBarMenuButton__UEAA_XZ(void* pThis);
//   detail/CbarcoreSupport.cpp
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(
    void* pThis, unsigned int uiID, HMENU hMenu, int iImage, const wchar_t* lpszText, int bUserButton);
//   featurepack/toolbar/CMFCToolBarMenuButton.cpp
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarMenuButton* pThis, const CMFCToolBarButton* pSrc);
extern "C" HMENU MS_ABI impl__CreateMenu_CMFCToolBarMenuButton__UEBAPEAUHMENU____XZ(
    const CMFCToolBarMenuButton* pThis);
//   detail/MemcoreSupport.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
//   detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
//   core/runtime/CObject.cpp
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
//   core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
//   core/window/Thunks.cpp
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
//   featurepack/CMFC_misc_stubs.cpp
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const CWnd* pWnd);
//   core/frame/Thunks.cpp
extern "C" CMDIChildWnd* MS_ABI impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(
    const CMDIFrameWnd* pThis, int* pbMaximized);
//   RTTI getters: featurepack/docking/RuntimeClasses.cpp, core/frame/CMDIFrameWnd.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMiniDockFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWnd__SAPEAUCRuntimeClass__XZ();
//   Exported data: featurepack/toolbar/StaticData.cpp, featurepack/CMFC_misc_stubs.cpp
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;      // ?m_bCustomizeMode@CMFCToolBar@@1HA
extern "C" void* impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA;          // ?g_pTopLevelFrame@@3PEAVCFrameWnd@@EA

// This file's own OnCancelMode, called directly from OnDblClk (see there).
extern "C" void MS_ABI impl__OnCancelMode_CMFCToolBarSystemMenuButton__UEAAXXZ(void* pThis);

namespace {
inline bool SmbIsKindOf(const void* p, CRuntimeClass* pClass) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(p), pClass) != 0;
}

// The frame lookup shared by OnDblClk and OnAfterCreatePopupMenu, from the point
// where both retail bodies have a non-NULL frame in hand:
//     if (pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd))) {
//         pFrame = CWnd::FromHandle(::GetParent(pFrame->m_hWnd));   // import slot = USER32!GetParent
//         if (pFrame == NULL) return NULL;
//     }
//     return pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)) ? pFrame : NULL;
void* SmbMDIFrameFrom(void* pFrame) {
    if (SmbIsKindOf(pFrame, impl__GetThisClass_CMiniDockFrameWnd__SAPEAUCRuntimeClass__XZ())) {
        pFrame = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(SmbGet<HWND>(pFrame, kWndHWnd)));
        if (pFrame == nullptr) return nullptr;
    }
    return SmbIsKindOf(pFrame, impl__GetThisClass_CMDIFrameWnd__SAPEAUCRuntimeClass__XZ()) ? pFrame : nullptr;
}
} // namespace

// Retail RVA 0x17f730 (mfc140u), fully transcribed:
//     CMFCToolBarMenuButton(0, hSystemMenu, -1, NULL, FALSE);   // base ctor
//     <vptr> = CMFCToolBarSystemMenuButton::`vftable';
//     m_hSysMenuIcon = hSystemIcon;                              // +0x128
//     m_hSystemMenu  = hSystemMenu;                              // +0x130
// DEVIATION: OpenMFC has no CMFCToolBarSystemMenuButton vtable to install, so the
// object keeps the vptr the base-constructor thunk leaves (OpenMFC's own
// CMFCToolBarMenuButton).  Consequences: virtual calls made through OpenMFC reach
// the CMFCToolBarMenuButton bodies, and GetRuntimeClass() on the object reports
// CMFCToolBarMenuButton.  (The previous body did not construct the base at all.)
// Symbol: ??0CMFCToolBarSystemMenuButton@@QEAA@PEAUHMENU__@@PEAUHICON__@@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarSystemMenuButton__QEAA_PEAUHMENU____PEAUHICON_____Z(
    void* pThis, HMENU hSystemMenu, HICON hSystemIcon) {
    impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(pThis, 0, hSystemMenu, -1, nullptr, FALSE);
    SmbAt<HICON>(pThis, kSmbSysMenuIcon) = hSystemIcon;
    SmbAt<HMENU>(pThis, kSmbSystemMenu) = hSystemMenu;
    return pThis;
}
// Retail RVA 0x17f6a0 (mfc140u), fully transcribed:
//     CMFCToolBarMenuButton();                                   // base ctor
//     <vptr> = CMFCToolBarSystemMenuButton::`vftable';
//     m_hSysMenuIcon = NULL; m_hSystemMenu = NULL;               // +0x128, +0x130
// DEVIATION: the vptr, as in the (HMENU, HICON) constructor above.
// Symbol: ??0CMFCToolBarSystemMenuButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarSystemMenuButton__QEAA_XZ(void* pThis) {
    impl___0CMFCToolBarMenuButton__QEAA_XZ(pThis);
    SmbAt<HICON>(pThis, kSmbSysMenuIcon) = nullptr;
    SmbAt<HMENU>(pThis, kSmbSystemMenu) = nullptr;
    return pThis;
}
// Retail RVA 0x17f7a0 (mfc140u), fully transcribed:
//     <vptr> = CMFCToolBarSystemMenuButton::`vftable';
//     tail-jump to CMFCToolBarMenuButton::~CMFCToolBarMenuButton();
// Neither the HICON nor the HMENU is destroyed.  DEVIATION: the vptr store is
// skipped (no such vtable in OpenMFC; see the constructors).
// Symbol: ??1CMFCToolBarSystemMenuButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarSystemMenuButton__UEAA_XZ(void* pThis) {
    impl___1CMFCToolBarMenuButton__UEAA_XZ(pThis);
}

// Retail RVA 0x17f7b0 (mfc140u), fully transcribed:
//     CMFCToolBarMenuButton::CopyFrom(src);                      // non-virtual call
//     m_hSysMenuIcon = src.m_hSysMenuIcon;                       // +0x128
//     m_hSystemMenu  = src.m_hSystemMenu;                        // +0x130
// Retail does not check src's runtime class before reading +0x128/+0x130; neither
// does this body.
// Symbol: ?CopyFrom@CMFCToolBarSystemMenuButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarSystemMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
    void* pThis, const CMFCToolBarButton* pSrc) {
    impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
        static_cast<CMFCToolBarMenuButton*>(pThis), pSrc);
    SmbAt<HICON>(pThis, kSmbSysMenuIcon) = SmbGet<HICON>(pSrc, kSmbSysMenuIcon);
    SmbAt<HMENU>(pThis, kSmbSystemMenu) = SmbGet<HMENU>(pSrc, kSmbSystemMenu);
}

// Retail RVA 0x17fa30 (mfc140u), fully transcribed:  m_hSystemMenu = hMenu;   // +0x130
// Symbol: ?CreateFromMenu@CMFCToolBarSystemMenuButton@@UEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__CreateFromMenu_CMFCToolBarSystemMenuButton__UEAAXPEAUHMENU_____Z(
    void* pThis, HMENU hMenu) {
    SmbAt<HMENU>(pThis, kSmbSystemMenu) = hMenu;
}

// Retail RVA 0x17fa40 (mfc140u), fully transcribed:
//     if (m_hSystemMenu == NULL) AfxThrowInvalidArgException();          // +0x130 (ENSURE)
//     HMENU hMenu = CMFCToolBarMenuButton::CreateMenu();                 // non-virtual call
//     if (hMenu == NULL) return NULL;
//     CMenu* pMenu = CMenu::FromHandle(hMenu);
//     CMenu* pSysMenu = CMenu::FromHandle(m_hSystemMenu);
//     int iCount = ::GetMenuItemCount(pSysMenu->m_hMenu);
//     for (int i = 0; i < iCount; i++) {
//         UINT uiState = ::GetMenuState(pSysMenu->m_hMenu, i, MF_BYPOSITION);   // 0x400
//         UINT uiCmd   = ::GetMenuItemID(pSysMenu->m_hMenu, i);
//         if (uiState & MF_CHECKED)  ::CheckMenuItem(pMenu->m_hMenu, uiCmd, MF_CHECKED);    // 8
//         if (uiState & MF_DISABLED) ::EnableMenuItem(pMenu->m_hMenu, uiCmd, MF_DISABLED);  // 2
//         if (uiState & MF_GRAYED)   ::EnableMenuItem(pMenu->m_hMenu, uiCmd, MF_GRAYED);    // 1
//     }
//     return hMenu;
// (Import slots resolve to USER32!GetMenuItemCount / GetMenuState / GetMenuItemID /
// CheckMenuItem / EnableMenuItem.)  DEVIATION (behaviour-neutral): the two
// CMenu::FromHandle wrappers are not created -- retail only reads their m_hMenu,
// which is the handle passed in, so the raw HMENUs are used directly.
// CAVEAT (base-class behaviour, not this body): retail's base CreateMenu (RVA
// 0x173ef0, mfc140u) is called here non-virtually and returns a caller-owned menu.
// OpenMFC's base thunk returns CMFCToolBarMenuButton::m_hMenu itself when it is
// set (featurepack/toolbar/CMFCToolBarMenuButton.cpp), and the (HMENU, HICON)
// constructor above passes hSystemMenu to the base constructor, which stores it
// there.  For such a button, this function therefore returns the system menu
// itself, not a copy, and the state loop copies each item onto itself.  A caller
// that DestroyMenu()s the result, as retail's CMFCToolBarMenuButton::OpenPopupMenu
// (RVA 0x1759d0, mfc140u; import USER32!DestroyMenu) does, would destroy that
// system menu.  OpenMFC's OpenPopupMenu is currently a stub, so nothing inside
// OpenMFC does that today.
// Symbol: ?CreateMenu@CMFCToolBarSystemMenuButton@@UEBAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__CreateMenu_CMFCToolBarSystemMenuButton__UEBAPEAUHMENU____XZ(const void* pThis) {
    if (SmbGet<HMENU>(pThis, kSmbSystemMenu) == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    HMENU hMenu = impl__CreateMenu_CMFCToolBarMenuButton__UEBAPEAUHMENU____XZ(
        static_cast<const CMFCToolBarMenuButton*>(pThis));
    if (hMenu == nullptr) return nullptr;
    HMENU hSysMenu = SmbGet<HMENU>(pThis, kSmbSystemMenu);
    const int iCount = ::GetMenuItemCount(hSysMenu);
    for (int i = 0; i < iCount; i++) {
        const UINT uiState = ::GetMenuState(hSysMenu, static_cast<UINT>(i), MF_BYPOSITION);
        const UINT uiCmd = ::GetMenuItemID(hSysMenu, i);
        if (uiState & MF_CHECKED) ::CheckMenuItem(hMenu, uiCmd, MF_CHECKED);
        if (uiState & MF_DISABLED) ::EnableMenuItem(hMenu, uiCmd, MF_DISABLED);
        if (uiState & MF_GRAYED) ::EnableMenuItem(hMenu, uiCmd, MF_GRAYED);
    }
    return hMenu;
}

// Retail RVA 0x17f630 (mfc140u), fully transcribed:
//     void* p = operator new(0x138);
//     return p != NULL ? new (p) CMFCToolBarSystemMenuButton() : NULL;   // tail-jump to the default ctor
// The memset is OpenMFC's (as in CMFCToolBarSpinEditBoxButton.cpp): the storage is
// fully initialised before the constructor runs.
// Symbol: ?CreateObject@CMFCToolBarSystemMenuButton@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCToolBarSystemMenuButton__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kSmbObjectSize);
    if (p == nullptr) return nullptr;
    std::memset(p, 0, kSmbObjectSize);
    return impl___0CMFCToolBarSystemMenuButton__QEAA_XZ(p);
}

// Retail RVA 0x17fbc0 (mfc140u), fully transcribed:
//     if (m_pPopupMenu == NULL || !::IsWindow(m_pPopupMenu->m_hWnd)) return;   // +0xc0, +0x40
//     CFrameWnd* pFrame = g_pTopLevelFrame;
//     if (pFrame == NULL) {
//         pFrame = m_pPopupMenu->GetTopLevelFrame();
//         if (pFrame == NULL) return;
//     }
//     <SmbMDIFrameFrom: CMiniDockFrameWnd -> its parent; must be a CMDIFrameWnd>, else return;
//     m_pPopupMenu->m_pMessageWnd = pMDIFrame->MDIGetActive(NULL);            // popup +0x1f8
// (Import slot resolves to USER32!IsWindow.)
// Symbol: ?OnAfterCreatePopupMenu@CMFCToolBarSystemMenuButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnAfterCreatePopupMenu_CMFCToolBarSystemMenuButton__UEAAXXZ(void* pThis) {
    void* pPopup = SmbGet<void*>(pThis, kMbPopupMenu);
    if (pPopup == nullptr || !::IsWindow(SmbGet<HWND>(pPopup, kWndHWnd))) return;
    void* pFrame = impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA;
    if (pFrame == nullptr) {
        pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(static_cast<const CWnd*>(pPopup));
        if (pFrame == nullptr) return;
    }
    void* pMDIFrame = SmbMDIFrameFrom(pFrame);
    if (pMDIFrame == nullptr) return;
    CMDIChildWnd* pChild = impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(
        static_cast<const CMDIFrameWnd*>(pMDIFrame), nullptr);
    SmbAt<void*>(SmbGet<void*>(pThis, kMbPopupMenu), kPopupMessageWnd) = pChild;   // retail reloads m_pPopupMenu
}

// Retail RVA 0x17f7f0 (mfc140u), fully transcribed:
//     return CSize(::GetSystemMetrics(SM_CXMENUSIZE), sizeDefault.cy);   // 0x36; import = USER32!GetSystemMetrics
// The SIZE is returned through the hidden pointer in RDX (returned in RAX);
// pDC and bHorz are not read.
// Symbol: ?OnCalculateSize@CMFCToolBarSystemMenuButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCToolBarSystemMenuButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, SIZE* pRet, CDC* pDC, const CSize* pSizeDefault, int bHorz) {
    (void)pThis; (void)pDC; (void)bHorz;
    const LONG cy = pSizeDefault->cy;
    pRet->cx = ::GetSystemMetrics(SM_CXMENUSIZE);
    pRet->cy = cy;
    return pRet;
}

// Retail RVA 0x17fb30 (mfc140u):
//     if (m_pPopupMenu != NULL && ::IsWindow(m_pPopupMenu->m_hWnd)) {     // +0xc0, +0x40
//         if (m_pPopupMenu->InCommand()) return;                          // popup vslot +0x3a8
//         m_pPopupMenu->SaveState();                                      // popup vslot +0x3d0
//         m_pPopupMenu->m_bAutoDestroyParent = FALSE;                     // popup +0x1f0
//         m_pPopupMenu->CloseMenu(FALSE);                                 // non-virtual call
//     }
//     m_pPopupMenu = NULL;                                                // +0xc0
//     m_bToBeClosed = FALSE;                                              // +0xdc
// (Import slot resolves to USER32!IsWindow.)  Unlike the base
// CMFCToolBarMenuButton::OnCancelMode it neither cancels the popup's buttons nor
// invalidates the parent.
// DEVIATION (the same one, for the same reason, as the base-class body in
// featurepack/toolbar/CMFCToolBarMenuButton.cpp): the live-popup branch is not
// transcribed -- InCommand() is a non-exported CMFCPopupMenu virtual and
// OpenMFC's popup menus carry no MSVC vtable -- so when a live popup is attached
// this body returns with nothing changed, which is exactly retail's
// InCommand() == TRUE outcome.  With no popup, or a dead one, it matches retail.
// Symbol: ?OnCancelMode@CMFCToolBarSystemMenuButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCToolBarSystemMenuButton__UEAAXXZ(void* pThis) {
    void* pPopup = SmbGet<void*>(pThis, kMbPopupMenu);
    if (pPopup != nullptr && ::IsWindow(SmbGet<HWND>(pPopup, kWndHWnd))) {
        return;   // DEVIATION above
    }
    SmbAt<void*>(pThis, kMbPopupMenu) = nullptr;
    SmbAt<BOOL>(pThis, kMbToBeClosed) = FALSE;
}

// Retail RVA 0x17f900 (mfc140u), fully transcribed:
//     if (CMFCToolBar::m_bCustomizeMode) return;
//     if (pWnd == NULL) AfxThrowInvalidArgException();                   // ENSURE
//     OnCancelMode();                                                    // vslot +0xc0 (slot 24)
//     CFrameWnd* pFrame = AFXGetParentFrame(pWnd);
//     if (pFrame == NULL) return;
//     <SmbMDIFrameFrom: CMiniDockFrameWnd -> its parent; must be a CMDIFrameWnd>, else return;
//     CMDIChildWnd* pChild = pMDIFrame->MDIGetActive(NULL);
//     CMenu* pSysMenu = CMenu::FromHandle(::GetSystemMenu(pChild->m_hWnd, FALSE));
//     if (pSysMenu != NULL) {
//         MENUITEMINFO mii; memset(&mii, 0, sizeof mii);                 // 0x50 bytes
//         mii.cbSize = sizeof mii; mii.fMask = MIIM_STATE;
//         ::GetMenuItemInfo(pSysMenu->m_hMenu, SC_CLOSE, FALSE, &mii);   // result ignored
//         if (mii.fState & (MFS_GRAYED | MFS_DISABLED)) return;          // testb $3
//     }
//     ::SendMessage(pChild->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
// (Import slots resolve to USER32!GetParent / GetSystemMenu / GetMenuItemInfo /
// SendMessage; slot 24 of the retail CMFCToolBarSystemMenuButton vftable,
// mfc140u 0x18031b168, is OnCancelMode at RVA 0x17fb30.)
// DEVIATIONS: (1) OnCancelMode() is called directly rather than through slot 24:
// OpenMFC objects carry the mingw vtable, so an override in a client-derived
// class is not honoured.  (2) CMenu::FromHandle is not called; the raw HMENU is
// tested instead (FromHandle returns NULL exactly when the handle is NULL).
// (3) Retail dereferences pChild without a NULL check (a fault when the MDI frame
// has no active child); this body returns instead.
// Symbol: ?OnDblClk@CMFCToolBarSystemMenuButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnDblClk_CMFCToolBarSystemMenuButton__UEAAXPEAVCWnd___Z(void* pThis, CWnd* pWnd) {
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return;
    if (pWnd == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    impl__OnCancelMode_CMFCToolBarSystemMenuButton__UEAAXXZ(pThis);
    void* pFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(pWnd);
    if (pFrame == nullptr) return;
    void* pMDIFrame = SmbMDIFrameFrom(pFrame);
    if (pMDIFrame == nullptr) return;
    CMDIChildWnd* pChild = impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(
        static_cast<const CMDIFrameWnd*>(pMDIFrame), nullptr);
    if (pChild == nullptr) return;   // DEVIATION (3)
    const HWND hChild = SmbGet<HWND>(pChild, kWndHWnd);
    HMENU hSysMenu = ::GetSystemMenu(hChild, FALSE);
    if (hSysMenu != nullptr) {
        MENUITEMINFO mii;
        std::memset(&mii, 0, sizeof mii);
        mii.cbSize = sizeof mii;
        mii.fMask = MIIM_STATE;
        ::GetMenuItemInfo(hSysMenu, SC_CLOSE, FALSE, &mii);
        if (mii.fState & (MFS_GRAYED | MFS_DISABLED)) return;
    }
    ::SendMessage(hChild, WM_SYSCOMMAND, SC_CLOSE, 0);
}

// Retail RVA 0x17f820 (mfc140u), fully transcribed:
//     if (m_hSysMenuIcon == NULL) return;                                            // +0x128
//     int cy = min(::GetSystemMetrics(SM_CYSMICON), ::GetSystemMetrics(SM_CYMENUSIZE));   // 0x32 / 0x37
//     int cx = min(::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CXMENUSIZE));   // 0x31 / 0x36
//     ::DrawIconEx(pDC->GetSafeHdc(), rect.left,
//                  rect.top + (rect.Height() - cy) / 2,                              // signed /2
//                  m_hSysMenuIcon, cx, cy, 0, NULL, DI_NORMAL);
// min() is the macro: the winning metric is fetched a second time, as retail does
// (MENUSIZE when the two are equal).  pImages and the five BOOL flags are not read.
// (Import slots resolve to USER32!GetSystemMetrics / DrawIconEx.)
// Symbol: ?OnDraw@CMFCToolBarSystemMenuButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarSystemMenuButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, CDC* pDC, const CRect* pRect, CMFCToolBarImages* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons) {
    (void)pImages; (void)bHorz; (void)bCustomizeMode; (void)bHighlight; (void)bDrawBorder;
    (void)bGrayDisabledButtons;
    const HICON hIcon = SmbGet<HICON>(pThis, kSmbSysMenuIcon);
    if (hIcon == nullptr) return;
    const int cyMenu = ::GetSystemMetrics(SM_CYMENUSIZE);
    const int cySmIcon = ::GetSystemMetrics(SM_CYSMICON);
    const int cy = ::GetSystemMetrics(cySmIcon < cyMenu ? SM_CYSMICON : SM_CYMENUSIZE);
    const int cxMenu = ::GetSystemMetrics(SM_CXMENUSIZE);
    const int cxSmIcon = ::GetSystemMetrics(SM_CXSMICON);
    const int cx = ::GetSystemMetrics(cxSmIcon < cxMenu ? SM_CXSMICON : SM_CXMENUSIZE);
    const int y = pRect->top + (pRect->bottom - pRect->top - cy) / 2;
    ::DrawIconEx(pDC != nullptr ? pDC->m_hDC : nullptr, pRect->left, y, hIcon, cx, cy, 0, nullptr, DI_NORMAL);
}
