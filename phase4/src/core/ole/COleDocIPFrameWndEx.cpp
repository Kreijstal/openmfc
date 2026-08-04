// COleDocIPFrameWndEx — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/OlecoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);

// Symbol: ??0COleDocIPFrameWndEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0COleDocIPFrameWndEx__IEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ?OnUpdatePaneMenu@COleDocIPFrameWndEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_COleDocIPFrameWndEx__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
