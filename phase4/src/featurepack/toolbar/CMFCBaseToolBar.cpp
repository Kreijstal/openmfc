// CMFCBaseToolBar — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, mfccore.cpp, ole_olectors_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??1CMFCBaseToolBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCBaseToolBar__UEAA_XZ(void* pThis) {
    (void)pThis;
}
// Symbol: ?OnAfterStretch@CMFCBaseToolBar@@MEAAXH@Z
extern "C" void MS_ABI impl__OnAfterStretch_CMFCBaseToolBar__MEAAXH_Z(
    void* pThis, int width) {
    (void)pThis;
    (void)width;
}
// Symbol: ?OnAfterChangeParent@CMFCBaseToolBar@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnAfterChangeParent_CMFCBaseToolBar__UEAAXPEAVCWnd___Z(
    void* pThis, void* pWnd) {
    (void)pThis;
    (void)pWnd;
}
// Symbol: ?CreateObject@CMFCBaseToolBar@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCBaseToolBar__SAPEAVCObject__XZ() {
    return new (std::nothrow) CMFCBaseToolBarObject();
}
CMFCBaseToolBar::CMFCBaseToolBar() {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];
    state.canFloat = TRUE;
    state.canAutoHide = FALSE;
    state.hasGripper = FALSE;
    state.autoHideMode = FALSE;
    state.tabbed = FALSE;
}
CMFCBaseToolBar::~CMFCBaseToolBar() {}
// Symbol: ??0CMFCBaseToolBar@@QEAA@XZ
// CMFCBaseToolBar::CMFCBaseToolBar()
extern "C" void* MS_ABI impl___0CMFCBaseToolBar__QEAA_XZ() {
    return new CMFCBaseToolBar();
}
