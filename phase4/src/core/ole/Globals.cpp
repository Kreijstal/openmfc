// Globals — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??0CMFCRibbonContextCaption@@IEAA_XZ
// CMFCRibbonContextCaption::CMFCRibbonContextCaption()
extern "C" void* MS_ABI impl___0CMFCRibbonContextCaption__IEAA_XZ() {
    return new CMFCRibbonContextCaption();
}
// Symbol: ??0CMFCRibbonCaptionButton@@IEAA@IPEAUHWND__@@_Z
// CMFCRibbonCaptionButton::CMFCRibbonCaptionButton()
extern "C" void* MS_ABI impl___0CMFCRibbonCaptionButton__IEAA_IPEAUHWND_____Z(
    unsigned int p0, void* /*struct*/* p1) {
    (void)p0;
    (void)p1;
    return new CMFCRibbonCaptionButton();
}
