// CMFCShowAllButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
#include <cstdlib>
#include "openmfc/afxstr.h"

// CMFCShowAllButton is absent from include/openmfc. Retail CreateObject allocates
// 0x128 bytes (RVA 0x12e990, mfc140u), matching the modeled base size; pin the
// inherited field offsets used by the bodies below.
static_assert(sizeof(CMFCToolBarMenuButton) == 0x128, "CMFCShowAllButton retail size is 0x128");
static_assert(offsetof(CMFCToolBarButton, m_pWndParent) == 0x80, "m_pWndParent @0x80");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40");

extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ();
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__StartPopupMenuTimer_CMFCPopupMenuBar__QEAAXPEAVCMFCToolBarMenuButton__H_Z(
    CMFCPopupMenuBar* pThis, CMFCToolBarMenuButton* pMenuButton, int nDelayFactor);
extern "C" void MS_ABI impl__ShowAllCommands_CMFCPopupMenu__QEAAXXZ(void* pThis);
extern "C" std::int32_t impl__m_bShowAllMenusDelay_CMFCMenuBar__1HA;
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
extern "C" int MS_ABI impl__GetShowAllMenuItemsHeight_CMFCVisualManager__UEAAHPEAVCDC__AEBVCSize___Z(
    CMFCVisualManager* pThis, CDC* pDC, const CSize* pSize);
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCToolBarMenuButton__UEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(
    void* pThis, LPACCEL pAccel);
extern "C" void MS_ABI impl___1CMFCAcceleratorKey__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const void* pThis, CString* pString);
extern "C" void MS_ABI impl__Format___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXIZZ(
    void* pThis, unsigned int nID, ...);

// Retail's constructor (RVA 0x12e9d0, mfc140u) calls the menu-button constructor
// and then installs CMFCShowAllButton's vftable. OpenMFC has no compatible derived
// vftable, but initialize the modeled base instead of leaving its fields indeterminate.
// This object consequently retains the base vftable and is not a full retail instance.
// Symbol: ??0CMFCShowAllButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCShowAllButton__QEAA_XZ(void* pThis) {
    impl___0CMFCToolBarMenuButton__QEAA_XZ(pThis);
    return pThis;
}
// Retail RVA 0x12ea50 (mfc140u) stores the same class vftable used by its
// constructor (RVA 0x12e9d0), then jumps to the base destructor at 0x172bf0.
// OpenMFC cannot reproduce that derived-vftable store; delegate modeled base cleanup
// to the parent destructor thunk in toolbar/Thunks.cpp.
// Symbol: ??1CMFCShowAllButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCShowAllButton__UEAA_XZ(void* pThis) {
    impl___1CMFCToolBarMenuButton__UEAA_XZ(pThis);
}

// Retail RVA 0x12e990 (mfc140u) allocates 0x128 bytes and invokes the default
// constructor. The local constructor initializes the modeled base, but cannot
// install a CMFCShowAllButton vftable; returning it as a derived object would have
// incorrect virtual dispatch. Keep the factory null until derived-vtable support exists.
// Symbol: ?CreateObject@CMFCShowAllButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCShowAllButton__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail RVA 0x12eb80 (mfc140u): call the lazy visual-manager accessor at
// RVA 0x9774, invoke GetShowAllMenuItemsHeight(pDC, sizeDefault), then return
// SIZE(50, height). OpenMFC's GetInstance has a source definition but no callable
// impl__ thunk (a direct C++ call fails this file's link audit), so use the exported
// current-manager pointer. Deviation: without a manager, the height thunk uses its
// null-manager fallback instead of retail's lazy creation. OpenMFC's base height
// method also returns sizeDefault.cy (or 16), whereas retail returns
// CMenuImages::Size().cy + 6. The Size thunk exists, but this base method does not
// use it; themed overrides are retained while the base-height result is not retail-exact.
// Symbol: ?OnCalculateSize@CMFCShowAllButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCShowAllButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, SIZE* pResult, CDC* pDC, const CSize* pSizeDefault, int bHorz) {
    (void)pThis;
    (void)bHorz;
    auto* pManager = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    pResult->cx = 50;
    pResult->cy = impl__GetShowAllMenuItemsHeight_CMFCVisualManager__UEAAHPEAVCDC__AEBVCSize___Z(
        pManager, pDC, pSizeDefault);
    return pResult;
}

// Retail RVA 0x12ebe0 (mfc140u): read m_pWndParent at +0x80 and require a
// CMFCPopupMenuBar; delayed clicks schedule this button with delay factor 2 when
// m_bShowAllMenusDelay is enabled. Immediate clicks return FALSE unless the parent
// popup resolves to a CMFCPopupMenu, then call ShowAllCommands (RVA 0xb8150, mfc140u).
// The mfc140u IAT slot 0x1802c72d8 resolves to USER32!GetParent. Deviation: that internal call is
// routed through the existing exported thunk, whose documented body omits the
// embedded-bar SetHot and shadow-bitmap cleanup steps.
// Symbol: ?OnClick@CMFCShowAllButton@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnClick_CMFCShowAllButton__UEAAHPEAVCWnd__H_Z(
    void* pThis, CWnd* pWnd, int bDelay) {
    (void)pWnd; // Retail does not read this parameter.
    auto* pButton = static_cast<CMFCToolBarButton*>(pThis);
    CWnd* pOwner = pButton->m_pWndParent;
    if (pOwner == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pOwner, impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ())) {
        return FALSE;
    }

    if (bDelay) {
        if (impl__m_bShowAllMenusDelay_CMFCMenuBar__1HA != 0) {
            impl__StartPopupMenuTimer_CMFCPopupMenuBar__QEAAXPEAVCMFCToolBarMenuButton__H_Z(
                static_cast<CMFCPopupMenuBar*>(pOwner),
                static_cast<CMFCToolBarMenuButton*>(pThis), 2);
        }
        return TRUE;
    }

    CWnd* pMenu = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pOwner->m_hWnd));
    if (pMenu == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pMenu, impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ())) {
        return FALSE;
    }
    impl__ShowAllCommands_CMFCPopupMenu__QEAAXXZ(pMenu);
    return TRUE;
}

// Stub: retail RVA 0x12ea60 (mfc140u) insets the rectangle, calls
// CMFCToolBarButton::FillInterior, and then dispatches visual-manager drawing and
// border slots. FillInterior's current impl__ body is empty, so forwarding to it
// would still omit the retail fill; do not claim a partial drawing path as complete.
// Symbol: ?OnDraw@CMFCShowAllButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCShowAllButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, CDC* pDC, const CRect* pRect, CMFCToolBarImages* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons) {
    (void)pThis; (void)pDC; (void)pRect; (void)pImages;
    (void)bHorz; (void)bCustomizeMode; (void)bHighlight; (void)bDrawBorder; (void)bGrayDisabledButtons;
}

// Retail RVA 0x12ec90 (mfc140u; located via the Unicode ordinal/RVA map although
// disas.py cannot resolve this decorated name): format the localized resource
// 0x3e99 with the Ctrl+Down accelerator, allocate TOOLINFO text with calloc, set
// lpszText and clear uId on success. Reuse the existing accelerator and CString
// thunks. Deviation: OpenMFC's CString::Format(resourceId, ...) helper resolves
// through this tree's GetResHandle path, not retail's AfxFindStringResourceHandle path.
// Symbol: ?OnToolHitTest@CMFCShowAllButton@@UEAAHPEBVCWnd@@PEAUtagTOOLINFOW@@@Z
extern "C" int MS_ABI impl__OnToolHitTest_CMFCShowAllButton__UEAAHPEBVCWnd__PEAUtagTOOLINFOW___Z(
    void* pThis, const CWnd* pWnd, TOOLINFOW* pTI) {
    (void)pThis;
    (void)pWnd;
    if (pTI == nullptr) {
        return FALSE;
    }

    ACCEL accel{};
    accel.fVirt = FVIRTKEY | FCONTROL;
    accel.key = VK_DOWN;
    struct {
        void* vfptr;
        LPACCEL m_lpAccel;
    } accelerator;
    impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(&accelerator, &accel);
    CString strAccelerator;
    impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        &accelerator, &strAccelerator);
    impl___1CMFCAcceleratorKey__UEAA_XZ(&accelerator);

    CString strToolTip;
    impl__Format___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXIZZ(
        &strToolTip, 0x3e99, &strAccelerator);
    auto* pText = static_cast<wchar_t*>(
        std::calloc(static_cast<std::size_t>(strToolTip.GetLength()) + 1, sizeof(wchar_t)));
    pTI->lpszText = pText;
    if (pText == nullptr) {
        return FALSE;
    }

    ::lstrcpyW(pText, strToolTip.GetString());
    pTI->uId = 0;
    return TRUE;
}

// Retail RVA 0x2a4a0 (mfc140u): call vtable slot +0x50 (OnClick) with
// bDelay=FALSE. Deviation: dispatch directly to this OnClick thunk because OpenMFC
// has no compatible CMFCShowAllButton vftable for the retail virtual call.
// Symbol: ?OpenPopupMenu@CMFCShowAllButton@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OpenPopupMenu_CMFCShowAllButton__UEAAHPEAVCWnd___Z(
    void* pThis, CWnd* pWnd) {
    return impl__OnClick_CMFCShowAllButton__UEAAHPEAVCWnd__H_Z(pThis, pWnd, FALSE);
}

