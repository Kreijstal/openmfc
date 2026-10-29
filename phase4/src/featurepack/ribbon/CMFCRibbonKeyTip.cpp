// CMFCRibbonKeyTip — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

namespace {
// CMFCRibbonKeyTip is not declared in include/openmfc. Retail's constructor
// stores pElement/rectScreen/bIsMenu at +0xe8/+0xf0/+0x100 (mfc140u RVA
// 0x10e010); the CWnd base size is pinned in afxwin.h.
struct RibbonKeyTipLayout {
    alignas(8) unsigned char wnd[sizeof(CWnd)];
    void* pElement;
    RECT rectScreen;
    BOOL bIsMenu;
};
static_assert(sizeof(CWnd) == 0xe8, "CWnd base ends at CMFCRibbonKeyTip +0xe8");
static_assert(offsetof(RibbonKeyTipLayout, pElement) == 0xe8,
              "CMFCRibbonKeyTip::m_pElement at +0xe8 (mfc140u RVA 0x10e010)");
static_assert(offsetof(RibbonKeyTipLayout, rectScreen) == 0xf0,
              "CMFCRibbonKeyTip::m_rectScreen at +0xf0 (mfc140u RVA 0x10e010)");
static_assert(offsetof(RibbonKeyTipLayout, bIsMenu) == 0x100,
              "CMFCRibbonKeyTip::m_bIsMenu at +0x100 (mfc140u RVA 0x10e010)");
static_assert(sizeof(RibbonKeyTipLayout) == 0x108,
              "CMFCRibbonKeyTip fields extend through +0x104 (mfc140u RVA 0x10e010)");
static_assert(sizeof(CString) == 8, "retail CString is one data pointer");
}

extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CWnd__UEAA_XZ(CWnd* pThis);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" void MS_ABI impl__UpdateMenuShadow_CMFCRibbonKeyTip__IEAAXXZ(void* pThis);

// Symbol: ??0CMFCRibbonKeyTip@@QEAA@PEAVCMFCRibbonBaseElement@@H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonKeyTip__QEAA_PEAVCMFCRibbonBaseElement__H_Z(
    void* pThis, void* element, int flags) {
    // Retail RVA 0x10e010 (mfc140u) constructs the CWnd base, installs the
    // derived vfptr, zeroes m_rectScreen, stores the element/flag, then calls
    // SetRectEmpty on that rectangle. The derived vfptr is unavailable here;
    // keep the OpenMFC CWnd vfptr while initializing the established state.
    impl___0CWnd__QEAA_XZ(pThis);
    auto* layout = static_cast<RibbonKeyTipLayout*>(pThis);
    layout->pElement = element;
    ::SetRectEmpty(&layout->rectScreen);
    layout->bIsMenu = flags;
    return pThis;
}
// Retail RVA 0x10e0c0 (mfc140u) writes the CMFCRibbonKeyTip vfptr and then
// tail-calls CWnd::~CWnd (RVA 0x28b740, mfc140u). OpenMFC has no declared
// CMFCRibbonKeyTip vfptr to install; the CWnd destructor immediately installs
// its own vfptr before cleanup, so call that base destructor thunk directly.
// Symbol: ??1CMFCRibbonKeyTip@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonKeyTip__UEAA_XZ(void* pThis) {
    impl___1CWnd__UEAA_XZ(static_cast<CWnd*>(pThis));
}

// Retail RVA 0x10e530 (mfc140u) returns for null this/ HWND or an invisible
// window; otherwise it calls CWnd::ShowWindow(SW_HIDE) and the private shadow
// updater (RVA 0x10e570, mfc140u). The OpenMFC CWnd thunk handles the ordinary
// HWND path but does not dispatch through a control site; the shadow updater
// remains unavailable, so that part is still omitted.
// Symbol: ?Hide@CMFCRibbonKeyTip@@QEAAXXZ
extern "C" void MS_ABI impl__Hide_CMFCRibbonKeyTip__QEAAXXZ(void* pThis) {
    if (!pThis) return;
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (!pWnd->m_hWnd || !::IsWindowVisible(pWnd->m_hWnd)) return;
    impl__ShowWindow_CWnd__QEAAHH_Z(pWnd, SW_HIDE);
    impl__UpdateMenuShadow_CMFCRibbonKeyTip__IEAAXXZ(pThis);
}

// Retail RVA 0x10e5c0 (mfc140u) tests IsWindowVisible(m_hWnd), conditionally
// hides the window and updates its shadow, then unconditionally calls
// CWnd::OnDestroy. OpenMFC reproduces the visible-window/base teardown path,
// but ShowWindow does not dispatch through a control site and the shadow updater
// remains a stub. A null-this guard is an additional safety check.
// Symbol: ?OnDestroy@CMFCRibbonKeyTip@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCRibbonKeyTip__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (::IsWindowVisible(pWnd->m_hWnd)) {
        impl__ShowWindow_CWnd__QEAAHH_Z(pWnd, SW_HIDE);
        impl__UpdateMenuShadow_CMFCRibbonKeyTip__IEAAXXZ(pThis);
    }
    impl__OnDestroy_CWnd__IEAAXXZ(pWnd);
}

// No resolved mfc140u RVA; the background-erasure behavior cannot be
// established from the available key-tip layout or surrounding implementation.
// Symbol: ?OnEraseBkgnd@CMFCRibbonKeyTip@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCRibbonKeyTip__IEAAHPEAVCDC___Z(
    void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    return 0;
}

// The matching ANSI-twin entry (mfc140 RVA 0xdbf0) is exactly `mov eax, 3; ret`;
// this policy is independent of the three message arguments.
// Symbol: ?OnMouseActivate@CMFCRibbonKeyTip@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CMFCRibbonKeyTip__IEAAHPEAVCWnd__II_Z(
    void* pThis, CWnd* pDesktopWnd, unsigned int nHitTest, unsigned int message) {
    (void)pThis;
    (void)pDesktopWnd;
    (void)nHitTest;
    (void)message;
    return 3;
}

// The matching ANSI-twin body starts at mfc140 RVA 0x10ee90 and performs
// nontrivial DC acquisition, key-tip drawing, and GDI cleanup. The drawing
// state and element implementation are not modeled here, so retain this stub.
// Symbol: ?OnPaint@CMFCRibbonKeyTip@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCRibbonKeyTip__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// The matching ANSI-twin entry at mfc140 RVA 0x10efe0 builds geometry from the
// key-tip element and creates/repositions a window with several failure paths.
// That element/window infrastructure is not modeled here, so retain this stub.
// Symbol: ?Show@CMFCRibbonKeyTip@@QEAAHH@Z
extern "C" int MS_ABI impl__Show_CMFCRibbonKeyTip__QEAAHH_Z(void* pThis, int bRepos) {
    (void)pThis;
    (void)bRepos;
    return 0;
}

// This export has no resolved mfc140u RVA. The private helper reached by
// Hide/OnDestroy (entry RVA 0x10e570, mfc140u) checks a global pointer and then
// tests/updates the +0xf0 screen rectangle through unmodeled popup/geometry code.
// Symbol: ?UpdateMenuShadow@CMFCRibbonKeyTip@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateMenuShadow_CMFCRibbonKeyTip__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// This is a data export (mfc140u RVA 0x3c2498), not a function body. The
// retail initializer/value was not established, so this only provides correctly
// typed CString storage and does not claim to reproduce its class-name value.
// Symbol: ?m_strClassName@CMFCRibbonKeyTip@@1V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@A
extern "C" CString impl__m_strClassName_CMFCRibbonKeyTip__1V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A{};
