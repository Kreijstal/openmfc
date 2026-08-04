// CMFCRibbonButtonsGroup — OpenMFC implementation.
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

























CMFCRibbonButtonsGroup::CMFCRibbonButtonsGroup() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonButtonsGroup::~CMFCRibbonButtonsGroup() {}
// Symbol: ??0CMFCRibbonButtonsGroup@@QEAA@PEAVCMFCRibbonBaseElement@@@Z
// CMFCRibbonButtonsGroup::CMFCRibbonButtonsGroup()
extern "C" void* MS_ABI impl___0CMFCRibbonButtonsGroup__QEAA_PEAVCMFCRibbonBaseElement___Z(
    void* /*p0*/) {
    return new CMFCRibbonButtonsGroup();
}
