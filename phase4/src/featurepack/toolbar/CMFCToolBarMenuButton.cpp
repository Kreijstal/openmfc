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
