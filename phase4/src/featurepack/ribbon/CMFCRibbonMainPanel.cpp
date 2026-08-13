// CMFCRibbonMainPanel — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

CMFCRibbonMainPanel::CMFCRibbonMainPanel() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonMainPanel::~CMFCRibbonMainPanel() {}

// Symbol: ?Add@CMFCRibbonMainPanel@@UEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__Add_CMFCRibbonMainPanel__UEAAXPEAVCMFCRibbonBaseElement___Z(
    void* pThis, void* pElement) {
    (void)pThis;
    (void)pElement;
}

// Symbol: ?AddRecentFilesList@CMFCRibbonMainPanel@@QEAAXPEB_WH@Z
extern "C" void MS_ABI impl__AddRecentFilesList_CMFCRibbonMainPanel__QEAAXPEB_WH_Z(
    void* pThis, const wchar_t* title) {
    (void)pThis;
    (void)title;
}

// Symbol: ?AddToBottom@CMFCRibbonMainPanel@@QEAAXPEAVCMFCRibbonMainPanelButton@@@Z
extern "C" void MS_ABI impl__AddToBottom_CMFCRibbonMainPanel__QEAAXPEAVCMFCRibbonMainPanelButton___Z(
    void* pThis, void* pButton) {
    (void)pThis;
    (void)pButton;
}

// Symbol: ?AddToRight@CMFCRibbonMainPanel@@QEAAXPEAVCMFCRibbonBaseElement@@H@Z
extern "C" void MS_ABI impl__AddToRight_CMFCRibbonMainPanel__QEAAXPEAVCMFCRibbonBaseElement__H_Z(
    void* pThis, void* pElement, int count) {
    (void)pThis;
    (void)pElement;
    (void)count;
}

// Symbol: ?CopyFrom@CMFCRibbonMainPanel@@MEAAXAEAVCMFCRibbonPanel@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonMainPanel__MEAAXAEAVCMFCRibbonPanel___Z(
    void* pThis, void* pPanel) {
    (void)pThis;
    (void)pPanel;
}

// Symbol: ?DoPaint@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCRibbonMainPanel__MEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}

// Symbol: ?DrawApplicationButton@CMFCRibbonMainPanel@@IEAAXPEAVCDC@@PEAVCWnd@@@Z
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
extern "C" int MS_ABI impl__GetMenuElements_CMFCRibbonMainPanel__IEBAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?GetPreferedMenuLocation@CMFCRibbonMainPanel@@MEAAHAEAVCRect@@@Z
extern "C" int MS_ABI impl__GetPreferedMenuLocation_CMFCRibbonMainPanel__MEAAHAEAVCRect___Z(
    void* pThis, CRect& rect) {
    (void)pThis;
    (void)rect;
    return 0;
}

// Symbol: ?MouseButtonDown@CMFCRibbonMainPanel@@MEAAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" void* MS_ABI impl__MouseButtonDown_CMFCRibbonMainPanel__MEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(
    void* pThis, CPoint point) {
    (void)pThis;
    (void)point;
    return nullptr;
}

// Symbol: ?OnDrawMenuBorder@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@PEAVCMFCRibbonPanelMenuBar@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCRibbonMainPanel__MEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar___Z(
    void* pThis, void* pDC, void* pBar) {
    (void)pThis;
    (void)pDC;
    (void)pBar;
}

// Symbol: ?RecalcWidths@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@H@Z
extern "C" void MS_ABI impl__RecalcWidths_CMFCRibbonMainPanel__MEAAXPEAVCDC__H_Z(
    void* pThis, void* pDC, int index) {
    (void)pThis;
    (void)pDC;
    (void)index;
}

// Symbol: ?Reposition@CMFCRibbonMainPanel@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__Reposition_CMFCRibbonMainPanel__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const CRect& rect) {
    (void)pThis;
    (void)pDC;
    (void)rect;
}
