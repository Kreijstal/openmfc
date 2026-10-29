// CMFCRibbonMiniToolBar — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>

namespace {
// No CMFCRibbonMiniToolBar declaration or shadow struct is present in include/openmfc
// or phase4/src/detail. Retail CreateObject allocates 0x2f70 bytes (RVA 0x110e60,
// mfc140u); its constructor/OnCreate bodies (RVAs 0x110eb0/0x111410, mfc140u)
// expose the HWND and the class-tail fields below. The prefix remains opaque.
struct CMFCRibbonMiniToolBarObservedLayout {
    unsigned char opaquePrefix[0x40];
    HWND m_hWnd;
    unsigned char opaqueMiddle[0x2f60 - 0x48];
    int32_t m_bContextMenuMode;
    uint8_t m_nTransparency;
    uint8_t opaqueTailPadding[3];
    int32_t m_bWasHovered;
    int32_t m_bWasDroppedDown;
};
static_assert(sizeof(HWND) == 8, "CMFCRibbonMiniToolBar layout assumes x64 HWND");
static_assert(offsetof(CMFCRibbonMiniToolBarObservedLayout, m_hWnd) == 0x40,
              "retail OnCreate reads HWND at +0x40");
static_assert(offsetof(CMFCRibbonMiniToolBarObservedLayout, m_bContextMenuMode) == 0x2f60,
              "retail MiniToolBar context-mode offset");
static_assert(offsetof(CMFCRibbonMiniToolBarObservedLayout, m_nTransparency) == 0x2f64,
              "retail MiniToolBar transparency offset");
static_assert(offsetof(CMFCRibbonMiniToolBarObservedLayout, m_bWasHovered) == 0x2f68,
              "retail MiniToolBar hover-state offset");
static_assert(offsetof(CMFCRibbonMiniToolBarObservedLayout, m_bWasDroppedDown) == 0x2f6c,
              "retail MiniToolBar dropped-down offset");
static_assert(sizeof(CMFCRibbonMiniToolBarObservedLayout) == 0x2f70,
              "retail CreateObject allocation size");
static_assert(0x1810 + sizeof(int32_t) <= sizeof(CMFCRibbonMiniToolBarObservedLayout),
              "retail OnCreate write lies within the allocated object");
} // namespace

extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__OnCreate_CMFCPopupMenu__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* pCreateStruct);
extern "C" void* impl__m_pCurrent_CMFCRibbonMiniToolBar__1PEAV1_EA;

// Retail RVA 0x110eb0 (mfc140u) calls the non-exported panel-menu default constructor
// and initializes the toolbar fields; this pre-existing pass-through does neither. The
// panel-menu base-constructor thunk is also only a pass-through, so base initialization
// is not modeled here and this remains a documented stub.
// Symbol: ??0CMFCRibbonMiniToolBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonMiniToolBar__QEAA_XZ(void* pThis) {
    return pThis;
}
// Retail RVA 0x110f80 (mfc140u) replaces the vptr, unconditionally clears
// m_pCurrent, conditionally clears another global field when context-menu mode is off,
// then tail-jumps to the CMFCRibbonPanelMenu destructor (RVA 0x1211e0, mfc140u).
// The m_pCurrent global is modeled and cleared here. The remaining vptr/global writes
// and base destruction are omitted: this file's constructor is only a pass-through and
// does not initialize the base subobjects that the base destructor would access.
// Symbol: ??1CMFCRibbonMiniToolBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonMiniToolBar__UEAA_XZ(void* pThis) {
    (void)pThis;
    impl__m_pCurrent_CMFCRibbonMiniToolBar__1PEAV1_EA = nullptr;
}

// Retail RVA 0x110e60 (mfc140u) allocates 0x2f70 bytes and invokes the constructor at
// RVA 0x110eb0 (mfc140u). The constructor thunk in this file does not model retail
// base construction, so returning such an allocation would not produce a valid object.
// Symbol: ?CreateObject@CMFCRibbonMiniToolBar@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonMiniToolBar__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail RVA 0x111410 (mfc140u): when context-menu mode is off, tests extended-style
// bit 22 and clears the DWORD at +0x1810 if set; it then always calls the base OnCreate.
// A base failure of -1 is returned unchanged. On success it starts timer 0xec06 at
// 100 ms on the HWND at +0x40 only outside context-menu mode, then returns 0. The
// existing base thunk models the helper-failure path but not popup-bar construction.
// The corresponding mfc140 ANSI body at RVA 0x1121a0 calls through IAT slot
// 0x1802c5330, resolved by iat.py to USER32!SetTimer; the mfc140u body uses the
// corresponding slot 0x1802c7308. Call ::SetTimer.
// Symbol: ?OnCreate@CMFCRibbonMiniToolBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCRibbonMiniToolBar__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* pCreateStruct) {
    auto* layout = static_cast<CMFCRibbonMiniToolBarObservedLayout*>(pThis);
    if (layout->m_bContextMenuMode == 0 &&
        (impl__GetExStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pThis)) & (1UL << 22)) != 0) {
        *reinterpret_cast<int32_t*>(static_cast<unsigned char*>(pThis) + 0x1810) = 0;
    }

    const int baseResult = impl__OnCreate_CMFCPopupMenu__IEAAHPEAUtagCREATESTRUCTW___Z(
        pThis, pCreateStruct);
    if (baseResult == -1) return -1;

    if (layout->m_bContextMenuMode == 0) {
        ::SetTimer(layout->m_hWnd, 0xec06, 100, nullptr);
    }
    return 0;
}

// NOT FOUND by disas.py --u; retail behavior could not be established.
// Symbol: ?OnTimer@CMFCRibbonMiniToolBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCRibbonMiniToolBar__IEAAX_K_Z(
    void* pThis, unsigned __int64 nIDEvent) {
    (void)pThis;
    (void)nIDEvent;
}

// NOT FOUND by disas.py --u; command-list and ribbon-bar behavior is unverified.
// Symbol: ?SetCommands@CMFCRibbonMiniToolBar@@QEAAXPEAVCMFCRibbonBar@@AEBV?$CList@II@@@Z
extern "C" void MS_ABI impl__SetCommands_CMFCRibbonMiniToolBar__QEAAXPEAVCMFCRibbonBar__AEBV__CList_II___Z(
    void* pThis, void* pRibbonBar, const void* pCommands) {
    (void)pThis;
    (void)pRibbonBar;
    (void)pCommands;
}

// NOT FOUND by disas.py --u; popup positioning and activation behavior is unverified.
// Symbol: ?Show@CMFCRibbonMiniToolBar@@QEAAHHH@Z
extern "C" int MS_ABI impl__Show_CMFCRibbonMiniToolBar__QEAAHHH_Z(
    void* pThis, int x, int y) {
    (void)pThis;
    (void)x;
    (void)y;
    return 0;
}

// NOT FOUND by disas.py --u; context-menu setup and owner-window behavior is unverified.
// Symbol: ?ShowWithContextMenu@CMFCRibbonMiniToolBar@@QEAAHHHIPEAVCWnd@@@Z
extern "C" int MS_ABI impl__ShowWithContextMenu_CMFCRibbonMiniToolBar__QEAAHHHIPEAVCWnd___Z(
    void* pThis, int x, int y, unsigned int uiMenuResID, void* pWndOwner) {
    (void)pThis;
    (void)x;
    (void)y;
    (void)uiMenuResID;
    (void)pWndOwner;
    return 0;
}

// NOT FOUND by disas.py --u; the transparency update policy is unverified.
// Symbol: ?UpdateTransparency@CMFCRibbonMiniToolBar@@IEAAHXZ
extern "C" int MS_ABI impl__UpdateTransparency_CMFCRibbonMiniToolBar__IEAAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

