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
