// CMFCToolBarMenuButtonsButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

#include <cstddef>
#include <cstring>

// Retail derives CMFCToolBarMenuButtonsButton directly from CMFCToolBarButton,
// placing its system-command field at +0x88. The checked-in OpenMFC declaration
// still inserts CMFCToolBarMenuButton, so use the declared member rather than
// hard-coding the retail offset into the differently laid-out OpenMFC object.
static_assert(sizeof(CMFCToolBarButton) == 0x88, "retail toolbar-button base size");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == 0x28, "retail toolbar-button style offset");
static_assert(sizeof(CRect) == 0x10, "retail RECT-sized drawing argument");

extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(
    CMFCToolBarButton* pThis, const CMFCToolBarButton* pSrc);
extern "C" void MS_ABI impl__OnDrawMenuSystemButton_CMFCVisualManager__UEAAXPEAVCDC__VCRect__IIH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, unsigned int uiSystemCommand,
    unsigned int nStyle, int bHighlight);

// Symbol: ?CopyFrom@CMFCToolBarMenuButtonsButton@@UEAAXAEBVCMFCToolBarButton@@@Z
// Retail entry RVA 0x176440 (mfc140u) calls CMFCToolBarButton::CopyFrom
// (entry RVA 0x15c1a0 (mfc140u)), then copies the DWORD at +0x88 from source to destination.
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarMenuButtonsButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarMenuButtonsButton* pThis, const CMFCToolBarButton* pButton) {
    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(
        static_cast<CMFCToolBarButton*>(pThis), pButton);

    pThis->m_uiSystemCommand =
        static_cast<const CMFCToolBarMenuButtonsButton*>(pButton)->m_uiSystemCommand;
}
// Symbol: ?OnCalculateSize@CMFCToolBarMenuButtonsButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
// The assigned name is absent from the mfc140u name map. Its ANSI-twin entry is
// entry RVA 0xd89d0 (mfc140): GetSystemMetrics(0x37), then GetSystemMetrics(0x36),
// storing the second result in cx and the first in cy. The separately named
// mfc140u GetRegularSize entry at RVA 0xd7db0 (mfc140u) has the same instruction
// sequence, but is not this export. The mfc140 IAT slot 0x1802c4c50 resolves to
// USER32!GetSystemMetrics.
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCToolBarMenuButtonsButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    CMFCToolBarMenuButtonsButton* pThis, SIZE* pRet, CDC* pDC,
    const CSize* pSizeDefault, int bHorz) {
    (void)pThis;
    (void)pDC;
    (void)pSizeDefault;
    (void)bHorz;

    const int cy = ::GetSystemMetrics(SM_CYMENUSIZE);
    const int cx = ::GetSystemMetrics(SM_CXMENUSIZE);
    pRet->cx = cx;
    pRet->cy = cy;
    return pRet;
}
// Symbol: ?OnDraw@CMFCToolBarMenuButtonsButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
// Retail entry RVA 0x1763d0 (mfc140u) calls the private GetInstance helper at
// RVA 0x9774 (mfc140u), then copies the rect and calls visual-manager vtable
// slot 24 (+0xc0) with the command at +0x88, style at +0x28, and bHighlight.
// The helper lazily creates the manager, sets its +0x104 auto-destroy flag, and
// calls update slot +0x70.
// Deviation: OpenMFC has no impl__ thunk for that private helper. Although this
// tree defines C++ CMFCVisualManager::GetInstance(), referencing it here adds a new
// C++ undefined symbol rejected by checkfile's per-file audit. As in
// CMFCToolBar::DrawSeparator, draw only when the exported manager pointer is already
// non-null; default-manager creation, auto-destroy initialization, and the first-use
// update are not reproduced.
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarMenuButtonsButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    CMFCToolBarMenuButtonsButton* pThis, CDC* pDC, const CRect* pRect,
    CMFCToolBarImages* pImages, int bHorz, int bCustomizeMode,
    int bHighlight, int bDrawBorder, int bGrayDisabledButtons) {
    (void)pImages;
    (void)bHorz;
    (void)bCustomizeMode;
    (void)bDrawBorder;
    (void)bGrayDisabledButtons;

    CMFCVisualManager* pManager =
        static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (!pManager) return;

    CRect rectCopy;
    std::memcpy(&rectCopy, pRect, sizeof rectCopy);
    impl__OnDrawMenuSystemButton_CMFCVisualManager__UEAAXPEAVCDC__VCRect__IIH_Z(
        pManager, pDC, rectCopy, pThis->m_uiSystemCommand, pThis->m_nStyle, bHighlight);
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
