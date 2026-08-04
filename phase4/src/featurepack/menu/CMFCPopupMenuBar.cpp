// CMFCPopupMenuBar — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ?ImportFromMenu@CMFCPopupMenuBar@@UEAAHPEAUHMENU__@@H@Z
extern "C" int MS_ABI impl__ImportFromMenu_CMFCPopupMenuBar__UEAAHPEAUHMENU____H_Z(CMFCPopupMenuBar* pThis, HMENU hMenu, int bShowAllCommands) {
    return pThis ? pThis->ImportFromMenu(hMenu, bShowAllCommands) : FALSE;
}
// Symbol: ?ExportToMenu@CMFCPopupMenuBar@@UEBAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__ExportToMenu_CMFCPopupMenuBar__UEBAPEAUHMENU____XZ(const CMFCPopupMenuBar* pThis) {
    return pThis ? pThis->ExportToMenu() : nullptr;
}
// Symbol: ?BuildOrigItems@CMFCPopupMenuBar@@QEAAHI@Z
extern "C" int MS_ABI impl__BuildOrigItems_CMFCPopupMenuBar__QEAAHI_Z(CMFCPopupMenuBar* pThis, unsigned int uiMenuResID) {
    return pThis ? pThis->BuildOrigItems(uiMenuResID) : FALSE;
}
// Symbol: ?GetGutterWidth@CMFCPopupMenuBar@@QEAAHXZ
extern "C" int MS_ABI impl__GetGutterWidth_CMFCPopupMenuBar__QEAAHXZ(CMFCPopupMenuBar* pThis) {
    return pThis ? pThis->GetGutterWidth() : 0;
}
// Symbol: ?CalcSize@CMFCPopupMenuBar@@MEAA?AVCSize@@H@Z
extern "C" void MS_ABI impl__CalcSize_CMFCPopupMenuBar__MEAA_AVCSize__H_Z(CSize* pRet, CMFCPopupMenuBar* pThis, int bVertDock) {
    new (pRet) CSize(pThis ? pThis->CalcSize(bVertDock) : CSize());
}
// Symbol: ?AdjustLayout@CMFCPopupMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CMFCPopupMenuBar__MEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->AdjustLayout();
}
// Symbol: ?AdjustLocations@CMFCPopupMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCPopupMenuBar__MEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->AdjustLocations();
}
// Symbol: ?CloseDelayedSubMenu@CMFCPopupMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__CloseDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->CloseDelayedSubMenu();
}
CMFCPopupMenuBar::CMFCPopupMenuBar() {
    memset(_mfcpopupmenubar_padding, 0, sizeof(_mfcpopupmenubar_padding));
}
CMFCPopupMenuBar::~CMFCPopupMenuBar() {
    ClearPopupMenuBarState(this);
}
BOOL CMFCPopupMenuBar::ImportFromMenu(HMENU hMenu, BOOL) {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    state.menu = hMenu;
    ImportMenuIntoPopupItems(hMenu, state.items, state.ownedItems);
    return TRUE;
}
HMENU CMFCPopupMenuBar::ExportToMenu() const {
    auto it = g_popupMenuBarStates.find(this);
    if (it == g_popupMenuBarStates.end()) return nullptr;
    if (it->second.menu) return it->second.menu;

    HMENU hMenu = ::CreatePopupMenu();
    for (CMFCToolBarMenuButton* item : it->second.items) {
        if (!item) continue;
        if (item->m_hMenu) {
            ::AppendMenuW(hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(item->m_hMenu), static_cast<const wchar_t*>(item->m_strText));
        } else {
            ::AppendMenuW(hMenu, MF_STRING, static_cast<UINT_PTR>(item->m_nID), static_cast<const wchar_t*>(item->m_strText));
        }
    }
    return hMenu;
}
BOOL CMFCPopupMenuBar::BuildOrigItems(UINT) { return TRUE; }
CMFCToolBarMenuButton* CMFCPopupMenuBar::GetMenuItem(int nIndex) const {
    auto it = g_popupMenuBarStates.find(this);
    if (it == g_popupMenuBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.items.size())) return nullptr;
    return it->second.items[nIndex];
}
int CMFCPopupMenuBar::GetGutterWidth() const { return 22; }
CSize CMFCPopupMenuBar::CalcSize(BOOL) {
    auto it = g_popupMenuBarStates.find(this);
    const int count = it == g_popupMenuBarStates.end() ? 0 : static_cast<int>(it->second.items.size());
    return CSize(160, std::max(22, count * 22));
}
void CMFCPopupMenuBar::AdjustLayout() {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    if (GetSafeHwnd()) {
        ::InvalidateRect(GetSafeHwnd(), nullptr, TRUE);
        ::UpdateWindow(GetSafeHwnd());
    }
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[this].recentRect = CRect(0, 0, 160, std::max(22, static_cast<int>(state.items.size()) * 22));
}
void CMFCPopupMenuBar::AdjustLocations() {
    AdjustLayout();
}
void CMFCPopupMenuBar::CloseDelayedSubMenu() {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    state.delayedSubMenuOpen = false;
    state.selected = -1;
}
