// CMFCToolBarMenuButtonsButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?CopyFrom@CMFCToolBarMenuButtonsButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarMenuButtonsButton__UEAAXAEBVCMFCToolBarButton___Z(
    void* pThis, const void* pButton) {
    (void)pThis;
    (void)pButton;
}
// Symbol: ?OnCalculateSize@CMFCToolBarMenuButtonsButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" void* MS_ABI impl__OnCalculateSize_CMFCToolBarMenuButtonsButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, void* pDC, const void* size, int some) {
    (void)pThis;
    (void)pDC;
    (void)size;
    (void)some;
    return nullptr;
}
// Symbol: ?OnDraw@CMFCToolBarMenuButtonsButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarMenuButtonsButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, void* pDC, const void* rect, void* pImages,
    int n1, int n2, int n3, int n4, int n5) {
    (void)pThis;
    (void)pDC;
    (void)rect;
    (void)pImages;
    (void)n1;
    (void)n2;
    (void)n3;
    (void)n4;
    (void)n5;
}
// Symbol: ??0CMFCToolBarMenuButtonsButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButtonsButton__QEAA_XZ(void* pThis) {
    return new (pThis) CMFCToolBarMenuButtonsButton();
}
// Symbol: ??0CMFCToolBarMenuButtonsButton@@QEAA@I@Z
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButtonsButton__QEAA_I_Z(void* pThis, unsigned int uiSystemCommand) {
    return new (pThis) CMFCToolBarMenuButtonsButton(uiSystemCommand);
}
// Symbol: ??1CMFCToolBarMenuButtonsButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarMenuButtonsButton__UEAA_XZ(CMFCToolBarMenuButtonsButton* pThis) {
    if (pThis) pThis->~CMFCToolBarMenuButtonsButton();
}
CMFCToolBarMenuButtonsButton::CMFCToolBarMenuButtonsButton()
    : m_uiSystemCommand(0) {
    memset(_padButtons, 0, sizeof(_padButtons));
}
CMFCToolBarMenuButtonsButton::CMFCToolBarMenuButtonsButton(UINT uiSystemCommand)
    : CMFCToolBarMenuButtonsButton() {
    m_uiSystemCommand = uiSystemCommand;
    m_nID = uiSystemCommand;
}
CMFCToolBarMenuButtonsButton::~CMFCToolBarMenuButtonsButton() {}
