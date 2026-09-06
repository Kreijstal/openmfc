// CMFCVisualManagerVS2005 — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ?CreateObject@CMFCVisualManagerVS2005@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerVS2005__SAPEAVCObject__XZ() { return new CMFCVisualManagerVS2005(); }
// Symbol: ?GetThisClass@CMFCVisualManagerVS2005@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerVS2005__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerVS2005::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerVS2005@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerVS2005__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerVS2005* pThis) { return CMFCVisualManagerVS2005::GetThisClass(); }
CMFCVisualManagerVS2005::CMFCVisualManagerVS2005() {
    memset(_pad, 0, sizeof(_pad));
    impl__m_bRoundedAutohideButtons_CMFCVisualManagerVS2005__2HA = TRUE;
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerVS2005::~CMFCVisualManagerVS2005() {}
CObject* CMFCVisualManagerVS2005::CreateObject() { return new CMFCVisualManagerVS2005(); }
// Symbol: ?CreateAutoHideButtonRegion@CMFCVisualManagerVS2005@@IEAAHVCRect@@KAEAPEAUtagPOINT@@@Z
extern "C" int MS_ABI impl__CreateAutoHideButtonRegion_CMFCVisualManagerVS2005__IEAAHVCRect__KAEAPEAUtagPOINT___Z(void* /*class*/ p0, unsigned long p1, void* /*struct*/** p2) {
    return 0;
}

// Symbol: ?GetDockingTabsBordersSize@CMFCVisualManagerVS2005@@UEAAHXZ
extern "C" int MS_ABI impl__GetDockingTabsBordersSize_CMFCVisualManagerVS2005__UEAAHXZ() {
    return 0;
}

// Symbol: ?GetPropertyGridGroupColor@CMFCVisualManagerVS2005@@UEAAKPEAVCMFCPropertyGridCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupColor_CMFCVisualManagerVS2005__UEAAKPEAVCMFCPropertyGridCtrl___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetTabFrameColors@CMFCVisualManagerVS2005@@UEAAXPEBVCMFCBaseTabCtrl@@AEAK11111AEAPEAVCBrush@@2@Z
extern "C" void MS_ABI impl__GetTabFrameColors_CMFCVisualManagerVS2005__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(const void* /*class*/* p0, unsigned long* p1, unsigned long* p2, unsigned long* p3, unsigned long* p4, unsigned long* p5, unsigned long* p6, void* /*class*/** p7, void* /*class*/** p8) {}

// Symbol: ?OnDrawAutoHideButtonBorder@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@VCRect@@1PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__OnDrawAutoHideButtonBorder_CMFCVisualManagerVS2005__UEAAXPEAVCDC__VCRect__1PEAVCMFCAutoHideButton___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/ p2, void* /*class*/* p3) {}

// Symbol: ?OnDrawCaptionButton@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@PEAVCMFCCaptionButton@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionButton_CMFCVisualManagerVS2005__UEAAXPEAVCDC__PEAVCMFCCaptionButton__HHHHH_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, int p3, int p4, int p5, int p6) {}

// Symbol: ?OnDrawPaneCaption@CMFCVisualManagerVS2005@@UEAAKPEAVCDC@@PEAVCDockablePane@@HVCRect@@2@Z
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerVS2005__UEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, void* /*class*/ p3, void* /*class*/ p4) {
    return 0;
}

// Symbol: ?OnDrawSeparator@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerVS2005__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3) {}

// Symbol: ?OnDrawTab@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerVS2005__UEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, const void* /*class*/* p4) {}

// Symbol: ?OnDrawToolBoxFrame@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawToolBoxFrame_CMFCVisualManagerVS2005__UEAAXPEAVCDC__AEBVCRect___Z(void* /*class*/* p0, const void* /*class*/* p1) {}

// Symbol: ?OnEraseTabsArea@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerVS2005__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, const void* /*class*/* p2) {}

// Symbol: ?OnFillAutoHideButtonBackground@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__OnFillAutoHideButtonBackground_CMFCVisualManagerVS2005__UEAAXPEAVCDC__VCRect__PEAVCMFCAutoHideButton___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2) {}

// Symbol: ?OnFillHighlightedArea@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@PEAVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerVS2005__UEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, void* /*class*/* p3) {}

// Symbol: ?OnFillMiniFrameCaption@CMFCVisualManagerVS2005@@UEAAKPEAVCDC@@VCRect@@PEAVCPaneFrameWnd@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManagerVS2005__UEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int p3) {
    return 0;
}

// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerVS2005@@UEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerVS2005__UEAAXXZ() {}

