// CMFCToolBarMenuButton — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// mfccore.cpp carried a second copy of the constructor and Initialize; cbarcore.cpp
// linked first, so those were dead code. The cbarcore definitions are below.
// Symbol: ??0CMFCToolBarMenuButton@@QEAA@IPEAUHMENU__@@HPEB_WH@Z
// Symbol: ?Initialize@CMFCToolBarMenuButton@@IEAAXIPEAUHMENU__@@HPEB_WH@Z
extern "C" void MS_ABI impl__Initialize_CMFCToolBarMenuButton__IEAAXIPEAUHMENU____HPEB_WH_Z(CMFCToolBarMenuButton* pThis, unsigned int p0, HMENU p1, int p2, const wchar_t* p3, int p4) {
    pThis->Initialize(p0, p1, (BOOL)p2, p3, (BOOL)p4);
}
// Symbol: ??0CMFCToolBarMenuButton@@QEAA@AEBV0@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_AEBV0__Z(void* pThis, const CMFCToolBarMenuButton* pSrc) {
    return new (pThis) CMFCToolBarMenuButton(*pSrc);
}
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?Initialize@CMFCToolBarMenuButton@@IEAAXIPEAUHMENU__@@HPEB_WH@Z
// Symbol: ?Initialize@CMFCToolBarMenuButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarMenuButton__IEAAXXZ(CMFCToolBarMenuButton* pThis) {
    if (pThis) pThis->Initialize(0, nullptr, FALSE, nullptr, FALSE);
}
// Symbol: ?CreateFromMenu@CMFCToolBarMenuButton@@UEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__CreateFromMenu_CMFCToolBarMenuButton__UEAAXPEAUHMENU_____Z(CMFCToolBarMenuButton* pThis, HMENU hMenu) {
    if (pThis) pThis->CreateFromMenu(hMenu);
}
// Symbol: ?CreateMenu@CMFCToolBarMenuButton@@UEBAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__CreateMenu_CMFCToolBarMenuButton__UEBAPEAUHMENU____XZ(const CMFCToolBarMenuButton* pThis) {
    return pThis ? pThis->CreateMenu() : nullptr;
}
// Symbol: ?CopyFrom@CMFCToolBarMenuButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(CMFCToolBarMenuButton* pThis, const CMFCToolBarButton* pSrc) {
    if (pThis && pSrc) pThis->CopyFrom(*pSrc);
}
// Symbol: ?CompareWith@CMFCToolBarMenuButton@@UEBAHAEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__CompareWith_CMFCToolBarMenuButton__UEBAHAEBVCMFCToolBarButton___Z(const CMFCToolBarMenuButton* pThis, const CMFCToolBarButton* pOther) {
    return (pThis && pOther) ? pThis->CompareWith(*pOther) : 0;
}
CMFCToolBarMenuButton::CMFCToolBarMenuButton() : m_hMenu(nullptr) { memset(_pad, 0, sizeof(_pad)); }
CMFCToolBarMenuButton::CMFCToolBarMenuButton(UINT uiID, HMENU hMenu, BOOL bHasDropDownArrow, const wchar_t* lpszText, BOOL bUserButton)
    : CMFCToolBarMenuButton() {
    Initialize(uiID, hMenu, bHasDropDownArrow, lpszText, bUserButton);
}
CMFCToolBarMenuButton::CMFCToolBarMenuButton(const CMFCToolBarMenuButton& src)
    : CMFCToolBarButton(src.m_nID, src.m_iImage, static_cast<const wchar_t*>(src.m_strText), src.m_bUserButton, src.m_bLocked),
      m_hMenu(src.m_hMenu) {
    memset(_pad, 0, sizeof(_pad));
    CopyMenuButtonState(this, &src);
}
CMFCToolBarMenuButton::~CMFCToolBarMenuButton() {
    g_menuButtonStates.erase(this);
}
void CMFCToolBarMenuButton::Initialize(UINT uiID, HMENU hMenu, BOOL bHasDropDownArrow, const wchar_t* lpszText, BOOL bUserButton) {
    m_nID = uiID;
    m_hMenu = hMenu;
    m_bUserButton = bUserButton;
    if (lpszText) m_strText = lpszText;
    MenuButtonState& state = g_menuButtonStates[this];
    state.hasDropDownArrow = bHasDropDownArrow != FALSE;
    state.userButton = bUserButton != FALSE;
}
void CMFCToolBarMenuButton::CreateFromMenu(HMENU hMenu) {
    m_hMenu = hMenu;
}
HMENU CMFCToolBarMenuButton::CreateMenu() const {
    return m_hMenu ? m_hMenu : ::CreatePopupMenu();
}
void CMFCToolBarMenuButton::CopyFrom(const CMFCToolBarButton& src) {
    m_nID = src.m_nID;
    m_iImage = src.m_iImage;
    m_strText = src.m_strText;
    m_bUserButton = src.m_bUserButton;
    m_bLocked = src.m_bLocked;
    if (src.IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) {
        const CMFCToolBarMenuButton& menuSrc = static_cast<const CMFCToolBarMenuButton&>(src);
        m_hMenu = menuSrc.m_hMenu;
        CopyMenuButtonState(this, &menuSrc);
    }
}
int CMFCToolBarMenuButton::CompareWith(const CMFCToolBarButton& other) const {
    if (m_nID != other.m_nID) {
        return m_nID < other.m_nID ? -1 : 1;
    }
    return m_strText.Compare(other.m_strText);
}
// Symbol: ?DrawDocumentIcon@CMFCToolBarMenuButton@@IEAAXPEAVCDC@@AEBVCRect@@PEAUHICON__@@@Z
extern "C" void MS_ABI impl__DrawDocumentIcon_CMFCToolBarMenuButton__IEAAXPEAVCDC__AEBVCRect__PEAUHICON_____Z(void* /*class*/* p0, const void* /*class*/* p1, void* /*struct*/* p2) {}

// Symbol: ?DrawMenuItem@CMFCToolBarMenuButton@@IEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHH@Z
extern "C" void MS_ABI impl__DrawMenuItem_CMFCToolBarMenuButton__IEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHH_Z(void* /*class*/* p0, const void* /*class*/* p1, void* /*class*/* p2, int p3, int p4, int p5, int p6) {}

// Symbol: ?GetImageRect@CMFCToolBarMenuButton@@QEAAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetImageRect_CMFCToolBarMenuButton__QEAAXAEAVCRect___Z(void* /*class*/* p0) {}

// Symbol: ?GetTextHorzOffsets@CMFCToolBarMenuButton@@KAXAEAH0@Z
extern "C" void MS_ABI impl__GetTextHorzOffsets_CMFCToolBarMenuButton__KAXAEAH0_Z(int* p0, int* p1) {}

// Symbol: ?OnBeforeDrag@CMFCToolBarMenuButton@@UEBAHXZ
extern "C" int MS_ABI impl__OnBeforeDrag_CMFCToolBarMenuButton__UEBAHXZ() {
    return 0;
}

// Symbol: ?OnCalculateSize@CMFCToolBarMenuButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" void* MS_ABI impl__OnCalculateSize_CMFCToolBarMenuButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(void* /*struct*/* p0, void* /*class*/* p1, const void* /*class*/* p2, int p3) {
    return nullptr;
}

// Symbol: ?OnCancelMode@CMFCToolBarMenuButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCToolBarMenuButton__UEAAXXZ() {}

// Symbol: ?OnChangeParentWnd@CMFCToolBarMenuButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarMenuButton__UEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnClick@CMFCToolBarMenuButton@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnClick_CMFCToolBarMenuButton__UEAAHPEAVCWnd__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?OnDraw@CMFCToolBarMenuButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarMenuButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(void* /*class*/* p0, const void* /*class*/* p1, void* /*class*/* p2, int p3, int p4, int p5, int p6, int p7) {}

// Symbol: ?OnDrawOnCustomizeList@CMFCToolBarMenuButton@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarMenuButton__UEAAHPEAVCDC__AEBVCRect__H_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2) {
    return 0;
}

// Symbol: ?OpenPopupMenu@CMFCToolBarMenuButton@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OpenPopupMenu_CMFCToolBarMenuButton__UEAAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?ResetImageToDefault@CMFCToolBarMenuButton@@UEAAXXZ
extern "C" void MS_ABI impl__ResetImageToDefault_CMFCToolBarMenuButton__UEAAXXZ() {}

// Symbol: ?SaveBarState@CMFCToolBarMenuButton@@UEAAXXZ
extern "C" void MS_ABI impl__SaveBarState_CMFCToolBarMenuButton__UEAAXXZ() {}

// Symbol: ?Serialize@CMFCToolBarMenuButton@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarMenuButton__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetACCData@CMFCToolBarMenuButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarMenuButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?SetMenuPaletteMode@CMFCToolBarMenuButton@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetMenuPaletteMode_CMFCToolBarMenuButton__QEAAXHH_Z(int p0, int p1) {}

// Symbol: ?SetRadio@CMFCToolBarMenuButton@@UEAAXXZ
extern "C" void MS_ABI impl__SetRadio_CMFCToolBarMenuButton__UEAAXXZ() {}

// Symbol: ?SetTearOff@CMFCToolBarMenuButton@@UEAAXI@Z
extern "C" void MS_ABI impl__SetTearOff_CMFCToolBarMenuButton__UEAAXI_Z(unsigned int p0) {}
