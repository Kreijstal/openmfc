// CMFCRibbonMainPanel — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

CMFCRibbonMainPanel::CMFCRibbonMainPanel() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonMainPanel::~CMFCRibbonMainPanel() {}

// Symbol: ?Add@CMFCRibbonMainPanel@@UEAAXPEAVCMFCRibbonBaseElement@@@Z
// Stub: retail mfc140u RVA 0x110490 calls the element's vtable slot +0x2d8 with the parent at +0x158, then inserts into the embedded array at +0x6d0 (backing pointer +0x6d8), before the optional right element. OpenMFC's declaration omits the CMFCRibbonPanel base and these fields, so using those offsets would be unsafe.
extern "C" void MS_ABI impl__Add_CMFCRibbonMainPanel__UEAAXPEAVCMFCRibbonBaseElement___Z(
    void* pThis, void* pElement) {
    (void)pThis;
    (void)pElement;
}

// Symbol: ?AddRecentFilesList@CMFCRibbonMainPanel@@QEAAXPEB_WH@Z
// Stub: retail mfc140u RVA 0x1102d0 treats a null title as an error; otherwise it allocates and initializes a 0x670-byte recent-files element when allocation succeeds, then calls AddToRight with that pointer and the requested width. The recent-files element and the panel's required retail layout are not modeled.
extern "C" void MS_ABI impl__AddRecentFilesList_CMFCRibbonMainPanel__QEAAXPEB_WH_Z(
    void* pThis, const wchar_t* title, int width) {
    (void)pThis;
    (void)title;
    (void)width;
}

// Symbol: ?AddToBottom@CMFCRibbonMainPanel@@QEAAXPEAVCMFCRibbonMainPanelButton@@@Z
// Stub: retail mfc140u RVA 0x110410 increments the bottom-element count at +0x6f8, calls the element's vtable slot +0x2d8 with the parent at +0x158, grows the embedded array at +0x6d0, and stores through its backing pointer at +0x6d8. These fields are absent from the declared class layout.
extern "C" void MS_ABI impl__AddToBottom_CMFCRibbonMainPanel__QEAAXPEAVCMFCRibbonMainPanelButton___Z(
    void* pThis, void* pButton) {
    (void)pThis;
    (void)pButton;
}

// Symbol: ?AddToRight@CMFCRibbonMainPanel@@QEAAXPEAVCMFCRibbonBaseElement@@H@Z
// Stub: retail mfc140u RVA 0x110340 removes an existing right-pane element from the array, assigns the new element to the parent at +0x158, inserts it into the array at +0x6d0, then stores it at +0x720 and its width at +0x700. The required array and field layout is absent from the OpenMFC declaration.
extern "C" void MS_ABI impl__AddToRight_CMFCRibbonMainPanel__QEAAXPEAVCMFCRibbonBaseElement__H_Z(
    void* pThis, void* pElement, int count) {
    (void)pThis;
    (void)pElement;
    (void)count;
}

// Symbol: ?CopyFrom@CMFCRibbonMainPanel@@MEAAXAEAVCMFCRibbonPanel@@@Z
// Stub: retail mfc140u RVA 0x110520 first calls the base-panel CopyFrom, copies fields at +0x6f8/+0x6fc/+0x718/+0x700, clears +0x720, then remaps the right element by its source-array index and conditionally calls an internal routine on it. The source and destination retail layouts are not modeled.
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonMainPanel__MEAAXAEAVCMFCRibbonPanel___Z(
    void* pThis, void* pPanel) {
    (void)pThis;
    (void)pPanel;
}

// Symbol: ?DoPaint@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@@Z
// Stub: retail mfc140u RVA 0x1106c0 returns if USER32!IsRectEmpty finds the panel rectangle at +0x10c empty, and also returns if a later USER32!IntersectRect finds no overlap. Otherwise it performs main-panel and element geometry/drawing work using fields through +0x720 and several virtual calls. That layout and the required rendering support are unavailable.
extern "C" void MS_ABI impl__DoPaint_CMFCRibbonMainPanel__MEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}

// Symbol: ?DrawApplicationButton@CMFCRibbonMainPanel@@IEAAXPEAVCDC@@PEAVCWnd@@@Z
// Stub: retail mfc140u RVA 0x110a30 returns if the application-button element at +0x718 is null or outside the eligible bounds; otherwise it temporarily changes that element's rectangle, calls its drawing virtuals at +0x430 and +0x308, then restores the rectangle. The panel layout is not modeled.
extern "C" void MS_ABI impl__DrawApplicationButton_CMFCRibbonMainPanel__IEAAXPEAVCDC__PEAVCWnd___Z(
    void* pThis, void* pDC, void* pWnd) {
    (void)pThis;
    (void)pDC;
    (void)pWnd;
}

// Symbol: ?GetCommandsFrame@CMFCRibbonMainPanel@@QEBA?AVCRect@@XZ
extern "C" CRect MS_ABI impl__GetCommandsFrame_CMFCRibbonMainPanel__QEBA_AVCRect__XZ(void* pThis) {
    (void)pThis;
    return CRect();
}

// Symbol: ?GetMenuElements@CMFCRibbonMainPanel@@IEBAHXZ
// Stub: retail mfc140u RVA 0x110500 returns count(+0x6e0) - bottomCount(+0x6f8) - (rightElement(+0x720) ? 1 : 0). The OpenMFC declaration and constructor do not provide this retail storage.
extern "C" int MS_ABI impl__GetMenuElements_CMFCRibbonMainPanel__IEBAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?GetPreferedMenuLocation@CMFCRibbonMainPanel@@MEAAHAEAVCRect@@@Z
// Stub: retail mfc140u RVA 0x110630 returns FALSE and leaves the input rectangle untouched when +0x720 is null; otherwise it copies that element's rectangle, inflates it by -1, and applies a display-state adjustment before returning TRUE. The element field and supporting layout are unavailable.
extern "C" int MS_ABI impl__GetPreferedMenuLocation_CMFCRibbonMainPanel__MEAAHAEAVCRect___Z(
    void* pThis, CRect& rect) {
    (void)pThis;
    (void)rect;
    return 0;
}

// Symbol: ?MouseButtonDown@CMFCRibbonMainPanel@@MEAAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
// Stub: retail mfc140u RVA 0x110b20 first calls the base-panel hit test, then tests the transformed application-button rectangle at +0x718; a hit calls the button's vtable slot +0x340 and returns null, otherwise it returns the base result. The required object layout is absent.
extern "C" void* MS_ABI impl__MouseButtonDown_CMFCRibbonMainPanel__MEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
    return nullptr;
}

// Symbol: ?OnDrawMenuBorder@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@PEAVCMFCRibbonPanelMenuBar@@@Z
// Stub: retail mfc140u RVA 0x1109f0 reads the menu-bar HWND at +0x40, calls USER32!GetParent (IAT slot 0x1802c72d8 in mfc140u), resolves the resulting window through an internal helper, then tail-calls DrawApplicationButton. The panel/menu-bar layouts are not modeled.
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCRibbonMainPanel__MEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar___Z(
    void* pThis, void* pDC, void* pBar) {
    (void)pThis;
    (void)pDC;
    (void)pBar;
}

// Symbol: ?RecalcWidths@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@H@Z
// Stub: --u did not resolve this export. The ANSI twin at mfc140 RVA 0x1109b0 initializes the array at +0x6a8, writes fields at +0xe4/+0xac, calls virtual/helper code, and appends a computed width; those fields are absent from the declared layout.
extern "C" void MS_ABI impl__RecalcWidths_CMFCRibbonMainPanel__MEAAXPEAVCDC__H_Z(
    void* pThis, void* pDC, int index) {
    (void)pThis;
    (void)pDC;
    (void)index;
}

// Symbol: ?Reposition@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@AEBVCRect@@@Z
// Stub: --u did not resolve this export. The ANSI twin at mfc140 RVA 0x110a70 performs extensive element/panel geometry updates, using panel fields through +0x720 and element geometry; no matching shadow layout is available here.
extern "C" void MS_ABI impl__Reposition_CMFCRibbonMainPanel__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const CRect& rect) {
    (void)pThis;
    (void)pDC;
    (void)rect;
}
