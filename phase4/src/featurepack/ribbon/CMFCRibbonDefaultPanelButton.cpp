// CMFCRibbonDefaultPanelButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

extern "C" void MS_ABI impl__OnDraw_CMFCRibbonButton__UEAAXPEAVCDC___Z(
    CMFCRibbonButton* pThis, CDC* pDC);

// The retail constructor at RVA 0x115650 (mfc140u) stores the panel at +0x278
// and clears the icon handle at +0x270; CreateObject allocates 0x280 bytes
// (RVA 0x115610). Keep only this verified derived tail; inherited state stays
// opaque and current bodies do not access it.
struct CMFCRibbonDefaultPanelButtonRetailTail {
    unsigned char inheritedAndBaseState[0x270];
    HICON m_hIcon;
    void* m_pPanel;
};
static_assert(offsetof(CMFCRibbonDefaultPanelButtonRetailTail, m_hIcon) == 0x270,
              "retail CMFCRibbonDefaultPanelButton icon offset");
static_assert(offsetof(CMFCRibbonDefaultPanelButtonRetailTail, m_pPanel) == 0x278,
              "retail CMFCRibbonDefaultPanelButton panel offset");
static_assert(sizeof(CMFCRibbonDefaultPanelButtonRetailTail) == 0x280,
              "retail CMFCRibbonDefaultPanelButton size");

// Symbol: ??0CMFCRibbonDefaultPanelButton@@QEAA@PEAVCMFCRibbonPanel@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonDefaultPanelButton__QEAA_PEAVCMFCRibbonPanel___Z(
    void* pThis, void* pPanel) {
    (void)pPanel;
    return pThis;
}
// Retail RVA 0x1156d0 (mfc140u) calls CMFCRibbonButton::CopyFrom (RVA
// 0xe7aa0), copies fields at +0x278 and +0xe0, then when +0x278 is non-null
// follows its +0x140 field and calls a helper with this+0xb8. Those layouts
// and the helper's behavior are not modeled here.
// TODO(clean-room): keep stubbed until the required ribbon-button shadow layout is available.
// Symbol: ?CopyFrom@CMFCRibbonDefaultPanelButton@@UEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonDefaultPanelButton__UEAAXAEBVCMFCRibbonBaseElement___Z(
    void* pThis, const CMFCRibbonBaseElement& source) {
    (void)pThis;
    (void)source;
}

// Retail RVA 0x115610 (mfc140u) allocates 0x280 bytes and calls this class's
// constructor at RVA 0x115650. The existing constructor thunk in this file
// only returns pThis and does not initialize the allocated object, so using it
// here would not create a valid CObject.
// TODO(clean-room): factory requires a faithful constructor/vtable implementation.
// Symbol: ?CreateObject@CMFCRibbonDefaultPanelButton@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonDefaultPanelButton__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail RVA 0x1159c0 (mfc140u) dispatches to a qualifying original element;
// otherwise it uses the visual-manager path when the icon is null, or scales
// and draws the icon. The inherited button state and rendering infrastructure
// needed by those paths are not modeled here.
// TODO(clean-room): image drawing depends on missing ribbon-button/rendering state.
// Symbol: ?DrawImage@CMFCRibbonDefaultPanelButton@@UEAAXPEAVCDC@@W4RibbonImageType@CMFCRibbonBaseElement@@VCRect@@@Z
extern "C" void MS_ABI impl__DrawImage_CMFCRibbonDefaultPanelButton__UEAAXPEAVCDC__W4RibbonImageType_CMFCRibbonBaseElement__VCRect___Z(
    void* pThis, CDC* pDC, int type, const CRect* rectImage) {
    (void)pThis;
    (void)pDC;
    (void)type;
    (void)rectImage;
}

// Retail RVA 0x115690 (mfc140u) obtains the visual manager through its private
// lazy helper and dispatches slot +0x468 with pDC and this. Reuse the existing
// CMFCRibbonButton OnDraw thunk, which obtains OpenMFC's visual manager and
// calls OnDrawRibbonDefaultPaneButton. Its null guards and the local renderer
// are OpenMFC deviations from the retail dispatch.
// Symbol: ?OnDraw@CMFCRibbonDefaultPanelButton@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonDefaultPanelButton__UEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC) {
    impl__OnDraw_CMFCRibbonButton__UEAAXPEAVCDC___Z(
        static_cast<CMFCRibbonButton*>(pThis), pDC);
}

// Retail RVA 0x1158a0 (mfc140u) saves/clears the flag at +0x148, adjusts a
// rectangle, invokes the drawing virtuals, then restores the flag and releases
// a CString-related temporary. The required ribbon/button state is not modeled.
// TODO(clean-room): list rendering requires missing state and draw dispatch.
// Symbol: ?OnDrawOnList@CMFCRibbonDefaultPanelButton@@UEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonDefaultPanelButton__UEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(
    void* pThis, CDC* pDC, CString* strText, int nTextOffset,
    const CRect* rect, int bIsSelected, int bHighlighted) {
    (void)pThis;
    (void)pDC;
    (void)strText;
    (void)nTextOffset;
    (void)rect;
    (void)bIsSelected;
    (void)bHighlighted;
}

// The mfc140.dll ANSI twin resolves this export even though the Unicode RVA
// map does not. Its byte-identical body dispatches through virtual slots
// +0x1c8 and +0x288, checks panel state at +0x278/+0x10c, and may send WM_KEYDOWN/VK_DOWN
// to the window at +0x108/+0x40. Those inherited fields and dispatch targets
// are not modeled here, so keep this as a stub. The argument is a key code.
// Symbol: ?OnKey@CMFCRibbonDefaultPanelButton@@UEAAHH@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonDefaultPanelButton__UEAAHH_Z(
    void* pThis, int nChar) {
    (void)pThis;
    (void)nChar;
    return 0;
}

// The mfc140.dll ANSI twin resolves this export even though the Unicode RVA
// map does not. Its byte-identical body calls the base-element mouse-down
// handler, then dispatches vtable slot +0x288. The base handler uses additional
// unmodeled ribbon state. CPoint is an 8-byte MS ABI aggregate and is represented
// by long long here, so this remains a stub rather than a partial interaction.
// Symbol: ?OnLButtonDown@CMFCRibbonDefaultPanelButton@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonDefaultPanelButton__UEAAXVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// The mfc140.dll ANSI twin resolves this export even though the Unicode RVA
// map does not. Its byte-identical body forwards (panel at +0x278, this) to
// CMFCRibbonPanel::ShowPopup. That implementation depends on panel/menu state;
// the existing OpenMFC ShowPopup thunk is only a null-returning stub, so keep
// this method stubbed instead of pretending the forwarding creates a popup.
// Symbol: ?OnShowPopupMenu@CMFCRibbonDefaultPanelButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnShowPopupMenu_CMFCRibbonDefaultPanelButton__UEAAXXZ(
    void* pThis) {
    (void)pThis;
}

// The mfc140.dll ANSI twin resolves this export even though the Unicode RVA
// map does not. Its byte-identical body delegates to the inherited button
// SetACCData first, then writes role/state/location data from button/panel
// geometry and virtual state. CAccessibilityData itself is modeled; the
// inherited state and available base/button thunks do not reproduce retail's
// initialization, so retain the stub rather than claim success with bad data.
// Symbol: ?SetACCData@CMFCRibbonDefaultPanelButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonDefaultPanelButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, CWnd* pParentWnd, CAccessibilityData& data) {
    (void)pThis;
    (void)pParentWnd;
    (void)data;
    return 0;
}

