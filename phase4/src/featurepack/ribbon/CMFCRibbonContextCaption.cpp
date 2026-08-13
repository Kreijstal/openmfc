// CMFCRibbonContextCaption — OpenMFC implementation.
// Sources: mfccore.cpp, ole_olectors_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























CMFCRibbonContextCaption::CMFCRibbonContextCaption() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonContextCaption::~CMFCRibbonContextCaption() {}
// Symbol: ??0CMFCRibbonContextCaption@@IEAA@PEB_WIW4AFX_RibbonCategoryColor@@@Z
// CMFCRibbonContextCaption::CMFCRibbonContextCaption()
// (additional parameters are kept for ABI compatibility)
extern "C" void* MS_ABI impl___0CMFCRibbonContextCaption__IEAA_PEB_WIW4AFX_RibbonCategoryColor___Z(
    const wchar_t* pName, unsigned int nCategory, int nID,
    void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6,
    void* p7, void* p8, void* p9, void* p10, void* p11, void* p12,
    void* p13, void* p14, void* p15, void* p16, void* p17, void* p18,
    void* p19, void* p20, void* p21, void* p22) {
    (void)pName;
    (void)nCategory;
    (void)nID;
    (void)p0; (void)p1; (void)p2; (void)p3; (void)p4; (void)p5; (void)p6;
    (void)p7; (void)p8; (void)p9; (void)p10; (void)p11; (void)p12;
    (void)p13; (void)p14; (void)p15; (void)p16; (void)p17; (void)p18;
    (void)p19; (void)p20; (void)p21; (void)p22;
    return new CMFCRibbonContextCaption();
}
// ?CreateObject@CMFCRibbonContextCaption@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonContextCaption__SAPEAVCObject__XZ() {
    return nullptr;
}

// ?GetContextCaptionIndex@CMFCRibbonContextCaption@@QEAAHPEAV1@@Z
extern "C" int MS_ABI impl__GetContextCaptionIndex_CMFCRibbonContextCaption__QEAAHPEAV1__Z(void* /*class*/* p0) {
    return 0;
}

// ?GetContextCategories@CMFCRibbonContextCaption@@QEAAXAEAV?$CArray@PEAVCMFCRibbonCategory@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetContextCategories_CMFCRibbonContextCaption__QEAAXAEAV__CArray_PEAVCMFCRibbonCategory__PEAV1____Z(void* /*class*/* p0, void* /*class*/* p1) {}

// ?GetContextCategoryCount@CMFCRibbonContextCaption@@QEAAHXZ
extern "C" int MS_ABI impl__GetContextCategoryCount_CMFCRibbonContextCaption__QEAAHXZ() {
    return 0;
}

// ?OnDraw@CMFCRibbonContextCaption@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonContextCaption__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// ?OnLButtonUp@CMFCRibbonContextCaption@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonContextCaption__MEAAXVCPoint___Z(void* /*class*/ p0) {}

// ?OnSetAccData@CMFCRibbonContextCaption@@MEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonContextCaption__MEAAHJ_Z(long p0) {
    return 0;
}

// ?SetACCData@CMFCRibbonContextCaption@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonContextCaption__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// ?accDoDefaultAction@CMFCRibbonContextCaption@@MEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCRibbonContextCaption__MEAAJUtagVARIANT___Z(void* /*struct*/ p0) {
    return 0;
}

// ?accNavigate@CMFCRibbonContextCaption@@MEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonContextCaption__MEAAJJUtagVARIANT__PEAU2__Z(long p0, void* /*struct*/ p1, void* /*struct*/* p2) {
    return 0;
}

// ?get_accChildCount@CMFCRibbonContextCaption@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCRibbonContextCaption__MEAAJPEAJ_Z(long* p0) {
    return 0;
}

// ?get_accParent@CMFCRibbonContextCaption@@MEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonContextCaption__MEAAJPEAPEAUIDispatch___Z(void* /*struct*/** p0) {
    return 0;
}

