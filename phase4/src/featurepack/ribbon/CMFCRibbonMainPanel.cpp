// CMFCRibbonMainPanel — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

CMFCRibbonMainPanel::CMFCRibbonMainPanel() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonMainPanel::~CMFCRibbonMainPanel() {}

// Symbol: ?Add@CMFCRibbonMainPanel@@UEAAXPEAVCMFCRibbonBaseElement@@@Z
// Retail (mfc140u RVA 0x110490) calls an element vtable entry, then inserts into the embedded CArray object at +0x6d0 (its backing pointer is at +0x6d8); the declared class has no matching layout, so this remains a stub.
extern "C" void MS_ABI impl__Add_CMFCRibbonMainPanel__UEAAXPEAVCMFCRibbonBaseElement___Z(
    void* pThis, void* pElement) {
    (void)pThis;
    (void)pElement;
}

// Symbol: ?AddRecentFilesList@CMFCRibbonMainPanel@@QEAAXPEB_WH@Z
// Retail (mfc140u RVA 0x1102d0) treats a null title as an error; otherwise it attempts to allocate a recent-files element, initializes it only on successful allocation, then forwards the pointer (possibly null) to AddToRight. The actual element and panel layouts are not modeled.
extern "C" void MS_ABI impl__AddRecentFilesList_CMFCRibbonMainPanel__QEAAXPEB_WH_Z(
    void* pThis, const wchar_t* title) {
    (void)pThis;
    (void)title;
}

// Symbol: ?AddToBottom@CMFCRibbonMainPanel@@QEAAXPEAVCMFCRibbonMainPanelButton@@@Z
// Retail (mfc140u RVA 0x110410) increments the bottom-element count at +0x6f8, grows the embedded CArray at +0x6d0, then stores the button through its backing pointer at +0x6d8; these fields are absent from the declared class layout.
extern "C" void MS_ABI impl__AddToBottom_CMFCRibbonMainPanel__QEAAXPEAVCMFCRibbonMainPanelButton___Z(
    void* pThis, void* pButton) {
    (void)pThis;
    (void)pButton;
}

// Symbol: ?AddToRight@CMFCRibbonMainPanel@@QEAAXPEAVCMFCRibbonBaseElement@@H@Z
// Retail (mfc140u RVA 0x110340) replaces the optional right-pane element at +0x720, updates the embedded CArray at +0x6d0 (backing pointer +0x6d8), and stores its width at +0x700; the class layout is unavailable.
extern "C" void MS_ABI impl__AddToRight_CMFCRibbonMainPanel__QEAAXPEAVCMFCRibbonBaseElement__H_Z(
    void* pThis, void* pElement, int count) {
    (void)pThis;
    (void)pElement;
    (void)count;
}

// Symbol: ?CopyFrom@CMFCRibbonMainPanel@@MEAAXAEAVCMFCRibbonPanel@@@Z
// Retail (mfc140u RVA 0x110520) copies the base panel and main-panel fields including +0x6f8, +0x6fc, +0x700, and +0x718; the destination/source layout is not modeled.
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonMainPanel__MEAAXAEAVCMFCRibbonPanel___Z(
    void* pThis, void* pPanel) {
    (void)pThis;
    (void)pPanel;
}

// Symbol: ?DoPaint@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@@Z
// Retail (mfc140u RVA 0x1106c0) performs extensive main-panel and element geometry/drawing work using fields through +0x720; that layout and its rendering support are unavailable.
extern "C" void MS_ABI impl__DoPaint_CMFCRibbonMainPanel__MEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}

// Symbol: ?DrawApplicationButton@CMFCRibbonMainPanel@@IEAAXPEAVCDC@@PEAVCWnd@@@Z
// Retail (mfc140u RVA 0x110a30) returns immediately when the application-button element at +0x718 is null or outside the eligible bounds; otherwise it temporarily adjusts the element rectangle, dispatches drawing calls, and restores the rectangle. The panel layout is not modeled.
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
// Retail (mfc140u RVA 0x110500) returns count(+0x6e0) - bottomCount(+0x6f8) - (rightElement(+0x720) ? 1 : 0); the current class/constructor has no such retail-sized storage.
extern "C" int MS_ABI impl__GetMenuElements_CMFCRibbonMainPanel__IEBAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?GetPreferedMenuLocation@CMFCRibbonMainPanel@@MEAAHAEAVCRect@@@Z
// Retail (mfc140u RVA 0x110630) copies the right-pane element rectangle and adjusts it for display state when +0x720 is non-null; otherwise it leaves the supplied rectangle unchanged. The element field and supporting layout are unavailable.
extern "C" int MS_ABI impl__GetPreferedMenuLocation_CMFCRibbonMainPanel__MEAAHAEAVCRect___Z(
    void* pThis, CRect& rect) {
    (void)pThis;
    (void)rect;
    return 0;
}

// Symbol: ?MouseButtonDown@CMFCRibbonMainPanel@@MEAAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
// Retail (mfc140u RVA 0x110b20) first invokes the base-panel hit test, then checks the application-button rectangle at +0x718; a hit dispatches the element handler and returns null, otherwise it returns the base hit-test result. The required object layout is absent.
extern "C" void* MS_ABI impl__MouseButtonDown_CMFCRibbonMainPanel__MEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(
    void* pThis, CPoint point) {
    (void)pThis;
    (void)point;
    return nullptr;
}

// Symbol: ?OnDrawMenuBorder@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@PEAVCMFCRibbonPanelMenuBar@@@Z
// Retail (mfc140u RVA 0x1109f0) reads the menu-bar field at +0x40, processes it through import/helper calls, then tail-calls DrawApplicationButton; the panel/menu-bar layouts are not modeled.
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCRibbonMainPanel__MEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar___Z(
    void* pThis, void* pDC, void* pBar) {
    (void)pThis;
    (void)pDC;
    (void)pBar;
}

// Symbol: ?RecalcWidths@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@H@Z
// Stub: --u did not resolve this export; the ANSI twin at mfc140 RVA 0x1109b0 uses an embedded CArray object at +0x6a8 and fields at +0xe4/+0xac, absent from the declared layout.
extern "C" void MS_ABI impl__RecalcWidths_CMFCRibbonMainPanel__MEAAXPEAVCDC__H_Z(
    void* pThis, void* pDC, int index) {
    (void)pThis;
    (void)pDC;
    (void)index;
}

// Symbol: ?Reposition@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@AEBVCRect@@@Z
// Stub: --u did not resolve this export; the ANSI twin at mfc140 RVA 0x110a70 performs extensive element/panel geometry updates using fields through +0x720, with no matching shadow layout here.
extern "C" void MS_ABI impl__Reposition_CMFCRibbonMainPanel__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const CRect& rect) {
    (void)pThis;
    (void)pDC;
    (void)rect;
}
