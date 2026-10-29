// CMFCRibbonPanelMenu — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>

namespace {
// The new member-function bodies below use defensive null-this guards; retail
// assumes valid object pointers.  The pre-existing constructor shims and explicit
// stubs do not dereference this.  CMFCRibbonPanelMenu has no declaration in include/openmfc.
// This minimal shadow pins only fields used below: CMFCPopupMenu (0x19c8), its embedded
// CMFCRibbonPanelMenuBar (0x1590), then m_bForceClose.  The 0x19c8 base size is
// pinned in detail/CMFCPopupMenuSupport.h; the menu-bar offsets/size are pinned
// in CMFCRibbonPanelMenuBar.cpp; RuntimeClasses.cpp records the retail class
// size as 12128 (0x2f60).
struct CMFCPopupMenuDestroyGateLayout {
    char beforeGate[0x19ac];
    int rawGate;
    char afterGate[0x19c8 - 0x19b0];
};
static_assert(offsetof(CMFCPopupMenuDestroyGateLayout, rawGate) == 0x19ac,
              "CMFCRibbonPanelMenu::OnDestroy raw gate");
static_assert(sizeof(CMFCPopupMenuDestroyGateLayout) == 0x19c8,
              "CMFCPopupMenu base subobject size");

struct CMFCRibbonPanelMenuLayout {
    CMFCPopupMenuDestroyGateLayout popup;
    char barBeforeOffset[0x1364];
    int menuOffset;
    char barBeforeMenuMode[0x1428 - 0x1368];
    int menuMode;
    char barBeforePanel[0x1560 - 0x142c];
    void* panel;
    char barBeforeCategory[0x1580 - 0x1568];
    void* category;
    void* pressed;
    int forceClose;
    int tail;
};
static_assert(offsetof(CMFCRibbonPanelMenuLayout, menuOffset) == 0x2d2c,
              "CMFCRibbonPanelMenu.m_wndRibbonBar.m_iOffset");
static_assert(offsetof(CMFCRibbonPanelMenuLayout, menuMode) == 0x2df0,
              "CMFCRibbonPanelMenu.m_wndRibbonBar.m_bIsMenuMode");
static_assert(offsetof(CMFCRibbonPanelMenuLayout, panel) == 0x2f28,
              "CMFCRibbonPanelMenu.m_wndRibbonBar.m_pPanel");
static_assert(offsetof(CMFCRibbonPanelMenuLayout, category) == 0x2f48,
              "CMFCRibbonPanelMenu.m_wndRibbonBar.m_pCategory");
static_assert(offsetof(CMFCRibbonPanelMenuLayout, forceClose) == 0x2f58,
              "CMFCRibbonPanelMenu.m_bForceClose");
static_assert(sizeof(CMFCRibbonPanelMenuLayout) == 0x2f60,
              "retail CMFCRibbonPanelMenu size");

// CMFCRibbonPanel::m_bScrollDnAvailable is read at +0xd8 in the retail body.
struct CMFCRibbonPanelScrollLayout {
    char beforeScrollDownAvailable[0xd8];
    int scrollDownAvailable;
};
static_assert(offsetof(CMFCRibbonPanelScrollLayout, scrollDownAvailable) == 0xd8,
              "CMFCRibbonPanel::m_bScrollDnAvailable");
static_assert(sizeof(void*) == 8, "the retail layout is x64");
} // namespace

extern "C" void MS_ABI impl___1CMFCRibbonPanelMenuBar__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCPopupMenu__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__DoPaint_CMFCPopupMenu__MEAAXPEAVCDC___Z(void* pThis, CDC* pDC);
extern "C" int MS_ABI impl__GetBorderSize_CMFCPopupMenu__MEBAHXZ(const void* pThis);
extern "C" void MS_ABI impl__OnDestroy_CMFCPopupMenu__IEAAXXZ(void* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" CWnd* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" void* impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA;
extern "C" void* MS_ABI impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(const void* pThis);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonPanelMenuBar__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCRibbonPanelMenuBar__MEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC);
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;

// Symbol: ??0CMFCRibbonPanelMenu@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenu__IEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??0CMFCRibbonPanelMenu@@QEAA@PEAVCMFCRibbonBar@@AEBV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenu__QEAA_PEAVCMFCRibbonBar__AEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___H_Z(
    void* pThis, void* pRibbonBar, const void* elements, unsigned int unknown, int flags) {
    (void)pRibbonBar;
    (void)elements;
    (void)unknown;
    (void)flags;
    return pThis;
}
// Symbol: ??0CMFCRibbonPanelMenu@@QEAA@PEAVCMFCRibbonCategory@@VCSize@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenu__QEAA_PEAVCMFCRibbonCategory__VCSize___Z(
    void* pThis, void* pCategory, const void* size) {
    (void)pCategory;
    (void)size;
    return pThis;
}
// Symbol: ??0CMFCRibbonPanelMenu@@QEAA@PEAVCMFCRibbonGallery@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenu__QEAA_PEAVCMFCRibbonGallery___Z(
    void* pThis, void* pGallery) {
    (void)pGallery;
    return pThis;
}
// Symbol: ??0CMFCRibbonPanelMenu@@QEAA@PEAVCMFCRibbonPanel@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenu__QEAA_PEAVCMFCRibbonPanel___Z(
    void* pThis, void* pPanel) {
    (void)pPanel;
    return pThis;
}
// Retail RVA 0x1211e0 (mfc140u): clear m_bForceClose, destroy the embedded
// CMFCRibbonPanelMenuBar, then tail-call the CMFCPopupMenu destructor.  OpenMFC
// does not expose the retail CMFCRibbonPanelMenu vftable address, so the retail
// vfptr reset is omitted; both subobject destructors are dispatched by thunk.
// Symbol: ??1CMFCRibbonPanelMenu@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonPanelMenu__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    auto* bytes = static_cast<char*>(pThis);
    *reinterpret_cast<int*>(bytes + 0x2f58) = 0;
    impl___1CMFCRibbonPanelMenuBar__UEAA_XZ(bytes + 0x19c8);
    impl___1CMFCPopupMenu__UEAA_XZ(pThis);
}

// Retail RVA 0x121470 (mfc140u): call CMFCPopupMenu::DoPaint(this, pDC), then
// dispatch through m_wndRibbonBar's vtable byte offset 0x8c0, slot 280
// (OnDrawMenuBorder). OpenMFC calls that concrete embedded-bar method's thunk
// directly instead of repeating the retail virtual dispatch.
// Symbol: ?DoPaint@CMFCRibbonPanelMenu@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCRibbonPanelMenu__MEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC)
{
    if (!pThis) return;
    impl__DoPaint_CMFCPopupMenu__MEAAXPEAVCDC___Z(pThis, pDC);
    impl__OnDrawMenuBorder_CMFCRibbonPanelMenuBar__MEAAXPEAVCDC___Z(
        static_cast<char*>(pThis) + 0x19c8, pDC);
}

// Retail RVA 0x121530 (mfc140u): if m_wndRibbonBar.m_bIsMenuMode (+0x2df0)
// is nonzero, tail-call the base popup-menu border-size method. Otherwise get
// the visual manager and dispatch its virtual at byte offset 0x560 with this
// menu. OpenMFC approximates the base visual-manager implementation by reading
// m_nMenuBorderSize at +0xf4 (as the popup-menu base path does at RVA 0xbb420
// (mfc140u)); this does not model derived GetRibbonPopupBorderSize overrides.
// It also uses existing manager state rather than retail's lazy GetInstance.
// Symbol: ?GetBorderSize@CMFCRibbonPanelMenu@@MEBAHXZ
extern "C" int MS_ABI impl__GetBorderSize_CMFCRibbonPanelMenu__MEBAHXZ(
    const void* pThis)
{
    if (!pThis) return 0;
    const auto* layout = static_cast<const CMFCRibbonPanelMenuLayout*>(pThis);
    if (layout->menuMode != 0)
        return impl__GetBorderSize_CMFCPopupMenu__MEBAHXZ(pThis);
    const void* pVisualManager = impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
    if (!pVisualManager) return 0;
    return *reinterpret_cast<const int*>(static_cast<const char*>(pVisualManager) + 0xf4);
}

// Retail RVA 0x121230 (mfc140u): get the parent popup menu, invoke its
// GetMenuBar virtual at vtable byte offset 0x3a0 (slot 116), and return that bar only if
// it IsKindOf(CMFCRibbonPanelMenuBar).
// Symbol: ?GetParentRibbonMenuBar@CMFCRibbonPanelMenu@@QEBAPEAVCMFCRibbonPanelMenuBar@@XZ
extern "C" void* MS_ABI impl__GetParentRibbonMenuBar_CMFCRibbonPanelMenu__QEBAPEAVCMFCRibbonPanelMenuBar__XZ(
    const void* pThis)
{
    if (!pThis) return nullptr;
    void* parentPopup = impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(pThis);
    if (!parentPopup) return nullptr;
    void** vtable = *reinterpret_cast<void***>(parentPopup);
    using GetMenuBarFn = void* (MS_ABI *)(void*);
    auto getMenuBar = reinterpret_cast<GetMenuBarFn>(vtable[0x3a0 / sizeof(void*)]);
    void* menuBar = getMenuBar(parentPopup);
    if (!menuBar || !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(menuBar),
            impl__GetThisClass_CMFCRibbonPanelMenuBar__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return menuBar;
}

// Retail RVA 0x121430 (mfc140u): return TRUE for m_bForceClose (+0x2f58);
// otherwise, if m_pPanel (+0x2f28) exists, return its IsMainPanel virtual
// (vtable byte offset 0x178, slot 47).
// Symbol: ?IsAlwaysClose@CMFCRibbonPanelMenu@@UEBAHXZ
extern "C" int MS_ABI impl__IsAlwaysClose_CMFCRibbonPanelMenu__UEBAHXZ(
    const void* pThis)
{
    if (!pThis) return 0;
    const auto* layout = static_cast<const CMFCRibbonPanelMenuLayout*>(pThis);
    if (layout->forceClose != 0) return 1;
    if (!layout->panel) return 0;
    void** vtable = *reinterpret_cast<void***>(layout->panel);
    using IsMainPanelFn = int (MS_ABI *)(const void*);
    auto isMainPanel = reinterpret_cast<IsMainPanelFn>(vtable[0x178 / sizeof(void*)]);
    return isMainPanel(layout->panel) != 0;
}

// Retail RVA 0x121580 (mfc140u): return FALSE only when m_pPanel exists and
// its m_bScrollDnAvailable field at +0xd8 is FALSE; a null panel returns TRUE.
// Symbol: ?IsScrollDnAvailable@CMFCRibbonPanelMenu@@UEAAHXZ
extern "C" int MS_ABI impl__IsScrollDnAvailable_CMFCRibbonPanelMenu__UEAAHXZ(
    void* pThis)
{
    if (!pThis) return 0;
    const auto* layout = static_cast<const CMFCRibbonPanelMenuLayout*>(pThis);
    if (!layout->panel) return 1;
    const auto* panel = static_cast<const CMFCRibbonPanelScrollLayout*>(layout->panel);
    return panel->scrollDownAvailable != 0;
}

// Retail RVA 0x121570 (mfc140u): IsScrollUpAvailable is m_wndRibbonBar's
// signed m_iOffset (+0x2d2c) > 0.
// Symbol: ?IsScrollUpAvailable@CMFCRibbonPanelMenu@@UEAAHXZ
extern "C" int MS_ABI impl__IsScrollUpAvailable_CMFCRibbonPanelMenu__UEAAHXZ(
    void* pThis)
{
    if (!pThis) return 0;
    return static_cast<CMFCRibbonPanelMenuLayout*>(pThis)->menuOffset > 0;
}

// Retail RVA 0x1215a0 (mfc140u): for a valid this, when the popup's +0x19ac
// gate and embedded ribbon bar's m_pCategory (+0x2f48) are nonzero, prefer
// g_pTopLevelFrame; if it is null, test GetTopLevelFrame(m_wndRibbonBar), reload
// the global, and, only if that first frame result was non-null and the global
// remains null, call GetTopLevelFrame a second time. Pass the selected result
// to the internal CWnd::SetFocus body at RVA 0x2a9b60 (mfc140u), then tail-call
// CMFCPopupMenu::OnDestroy. The gate remains a raw 32-bit read because its
// field name is not established by this shadow; OpenMFC's null-this guard is
// an additional deviation.
// Symbol: ?OnDestroy@CMFCRibbonPanelMenu@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCRibbonPanelMenu__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    auto* bytes = static_cast<char*>(pThis);
    const auto* layout = static_cast<const CMFCRibbonPanelMenuLayout*>(pThis);
    if (layout->popup.rawGate != 0 && layout->category != nullptr) {
        auto* pFrame = static_cast<CFrameWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
        if (pFrame != nullptr) {
            impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pFrame));
        } else {
            auto* pRibbonBar = reinterpret_cast<const CWnd*>(bytes + 0x19c8);
            if (impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pRibbonBar) != nullptr) {
                pFrame = static_cast<CFrameWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
                if (pFrame == nullptr)
                    pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pRibbonBar);
                impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pFrame));
            }
        }
    }
    impl__OnDestroy_CMFCPopupMenu__IEAAXXZ(pThis);
}

// Neither the mfc140u nor ANSI export map resolves this symbol to a body; keep
// it stubbed rather than infer message behavior from a neighboring class.
// Symbol: ?OnKeyDown@CMFCRibbonPanelMenu@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CMFCRibbonPanelMenu__IEAAXIII_Z(
    void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags)
{
    (void)pThis;
    (void)nChar;
    (void)nRepCnt;
    (void)nFlags;
}

// Neither the mfc140u nor ANSI export map resolves this symbol to a body; keep
// it stubbed rather than infer message behavior from a neighboring class.
// CPoint is an 8-byte by-value aggregate under the MS x64 ABI.
// Symbol: ?OnLButtonDown@CMFCRibbonPanelMenu@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonPanelMenu__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point)
{
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// Neither the mfc140u nor ANSI export map resolves this symbol to a body; keep
// it stubbed rather than infer message behavior from a neighboring class.
// CPoint is an 8-byte by-value aggregate under the MS x64 ABI.
// Symbol: ?OnMouseWheel@CMFCRibbonPanelMenu@@IEAAHIFVCPoint@@@Z
extern "C" int MS_ABI impl__OnMouseWheel_CMFCRibbonPanelMenu__IEAAHIFVCPoint___Z(
    void* pThis, unsigned int nFlags, short zDelta, long long point)
{
    (void)pThis;
    (void)nFlags;
    (void)zDelta;
    (void)point;
    return 0;
}

