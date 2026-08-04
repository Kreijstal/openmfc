// CMFCVisualManagerOfficeXP — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" unsigned long MS_ABI impl__OnDrawPropertySheetListItem_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
    void* pThis, CDC* pDC, void*, const CRect& rect, int nItem, int nState);

// Symbol: ?CreateObject@CMFCVisualManagerOfficeXP@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerOfficeXP__SAPEAVCObject__XZ() { return new CMFCVisualManagerOfficeXP(); }
// Symbol: ?GetThisClass@CMFCVisualManagerOfficeXP@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOfficeXP__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerOfficeXP::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOfficeXP@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOfficeXP__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOfficeXP* pThis) { return CMFCVisualManagerOfficeXP::GetThisClass(); }
// Symbol: ??0CMFCVisualManagerOfficeXP@@IEAA@H@Z
extern "C" void* MS_ABI impl___0CMFCVisualManagerOfficeXP__IEAA_H_Z(void* pThis, int) {
    return new(pThis) CMFCVisualManagerOfficeXP();
}
// Symbol: ?OnDrawPropertySheetListItem@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCPropertySheet@@VCRect@@HH@Z
extern "C" unsigned long MS_ABI impl__OnDrawPropertySheetListItem_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
    void* pThis, CDC* pDC, void* pPropSheet, const CRect& rect, int nItem, int nState) {
    (void)pPropSheet;
    return impl__OnDrawPropertySheetListItem_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
        pThis, pDC, pPropSheet, rect, nItem, nState);
}
CMFCVisualManagerOfficeXP::CMFCVisualManagerOfficeXP() {
    memset(_pad, 0, sizeof(_pad));
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerOfficeXP::~CMFCVisualManagerOfficeXP() {}
CObject* CMFCVisualManagerOfficeXP::CreateObject() { return new CMFCVisualManagerOfficeXP(); }
