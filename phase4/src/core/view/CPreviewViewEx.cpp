// CPreviewViewEx — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, viewrich.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/ViewrichSupport.h"

// Symbol: ??0CPreviewViewEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0CPreviewViewEx__IEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ?OnUpdatePreviewNumPage@CPreviewViewEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePreviewNumPage_CPreviewViewEx__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
