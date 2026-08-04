// CContextMenuManager — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ??0CContextMenuManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CContextMenuManager__QEAA_XZ(void* pThis) { return new (pThis) CContextMenuManager(); }
// Symbol: ??1CContextMenuManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CContextMenuManager__UEAA_XZ(CContextMenuManager* pThis) { if (pThis) pThis->~CContextMenuManager(); }
// Symbol: ?AddMenu@CContextMenuManager@@QEAAHII@Z
extern "C" int MS_ABI impl__AddMenu_CContextMenuManager__QEAAHII_Z(CContextMenuManager* pThis, unsigned int nameId, unsigned int menuId) { return pThis ? pThis->AddMenu(nameId, menuId) : FALSE; }
// Symbol: ?AddMenu@CContextMenuManager@@QEAAHPEB_WI@Z
extern "C" int MS_ABI impl__AddMenu_CContextMenuManager__QEAAHPEB_WI_Z(CContextMenuManager* pThis, const wchar_t* name, unsigned int menuId) { return pThis ? pThis->AddMenu(name, menuId) : FALSE; }
// Symbol: ?GetMenuById@CContextMenuManager@@QEBAPEAUHMENU__@@I@Z
extern "C" HMENU MS_ABI impl__GetMenuById_CContextMenuManager__QEBAPEAUHMENU____I_Z(const CContextMenuManager* pThis, unsigned int id) { return pThis ? pThis->GetMenuById(id) : nullptr; }
// Symbol: ?GetMenuByName@CContextMenuManager@@QEBAPEAUHMENU__@@PEB_WPEAI@Z
extern "C" HMENU MS_ABI impl__GetMenuByName_CContextMenuManager__QEBAPEAUHMENU____PEB_WPEAI_Z(const CContextMenuManager* pThis, const wchar_t* name, unsigned int* id) { return pThis ? pThis->GetMenuByName(name, id) : nullptr; }
// Symbol: ?GetMenuNames@CContextMenuManager@@QEBAXAEAVCStringList@@@Z
extern "C" void MS_ABI impl__GetMenuNames_CContextMenuManager__QEBAXAEAVCStringList___Z(const CContextMenuManager* pThis, CStringList* list) { if (pThis && list) pThis->GetMenuNames(*list); }
// Symbol: ?ShowPopupMenu@CContextMenuManager@@UEAAHIHHPEAVCWnd@@HH@Z
extern "C" int MS_ABI impl__ShowPopupMenu_CContextMenuManager__UEAAHIHHPEAVCWnd__HH_Z(CContextMenuManager* pThis, unsigned int id, int x, int y, CWnd* owner, int own, int right) { return pThis ? pThis->ShowPopupMenu(id, x, y, owner, own, right) : FALSE; }
// Symbol: ?ShowPopupMenu@CContextMenuManager@@UEAAPEAVCMFCPopupMenu@@PEAUHMENU__@@HHPEAVCWnd@@HHH@Z
extern "C" CMFCPopupMenu* MS_ABI impl__ShowPopupMenu_CContextMenuManager__UEAAPEAVCMFCPopupMenu__PEAUHMENU____HHPEAVCWnd__HHH_Z(CContextMenuManager* pThis, HMENU menu, int x, int y, CWnd* owner, int own, int destroy, int right) { return pThis ? pThis->ShowPopupMenu(menu, x, y, owner, own, destroy, right) : nullptr; }
// Symbol: ?TrackPopupMenu@CContextMenuManager@@UEAAIPEAUHMENU__@@HHPEAVCWnd@@H@Z
extern "C" unsigned int MS_ABI impl__TrackPopupMenu_CContextMenuManager__UEAAIPEAUHMENU____HHPEAVCWnd__H_Z(CContextMenuManager* pThis, HMENU menu, int x, int y, CWnd* owner, int right) { return pThis ? pThis->TrackPopupMenu(menu, x, y, owner, right) : 0; }
// Symbol: ?ResetState@CContextMenuManager@@UEAAHXZ
extern "C" int MS_ABI impl__ResetState_CContextMenuManager__UEAAHXZ(CContextMenuManager* pThis) { return pThis ? pThis->ResetState() : FALSE; }
// Symbol: ?CopyOriginalMenuItemsFromMenu@CContextMenuManager@@IEAAXIAEAVCMFCPopupMenuBar@@@Z
extern "C" void MS_ABI impl__CopyOriginalMenuItemsFromMenu_CContextMenuManager__IEAAXIAEAVCMFCPopupMenuBar___Z(
    CContextMenuManager* pThis,
    unsigned int uiMenuResId,
    CMFCPopupMenuBar& menuBar)
{
    if (!pThis) return;
    auto stateIt = g_contextMenuStates.find(pThis);
    if (stateIt == g_contextMenuStates.end()) return;
    auto it = stateIt->second.menusById.find(uiMenuResId);
    if (it == stateIt->second.menusById.end()) return;
    if (!it->second) return;
    menuBar.ImportFromMenu(it->second, FALSE);
}
// Symbol: ?CopyOriginalMenuItemsToMenu@CContextMenuManager@@IEAAXIAEAVCMFCPopupMenuBar@@@Z
extern "C" void MS_ABI impl__CopyOriginalMenuItemsToMenu_CContextMenuManager__IEAAXIAEAVCMFCPopupMenuBar___Z(
    CContextMenuManager* pThis,
    unsigned int uiMenuResId,
    CMFCPopupMenuBar& menuBar)
{
    if (!pThis) return;
    auto stateIt = g_contextMenuStates.find(pThis);
    if (stateIt == g_contextMenuStates.end()) return;
    auto it = stateIt->second.menusById.find(uiMenuResId);
    if (it == stateIt->second.menusById.end()) return;
    if (!it->second) return;
    HMENU originalMenu = it->second;
    HMENU oldOwnedMenu = stateIt->second.ownedMenusById[uiMenuResId];
    HMENU clonedMenu = menuBar.ExportToMenu();
    if (!clonedMenu) return;
    stateIt->second.menusById[uiMenuResId] = clonedMenu;
    stateIt->second.ownedMenusById[uiMenuResId] = clonedMenu;
    if (oldOwnedMenu && oldOwnedMenu == originalMenu && oldOwnedMenu != clonedMenu) {
        ::DestroyMenu(oldOwnedMenu);
    }
}
// Symbol: ?LoadState@CContextMenuManager@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadState_CContextMenuManager__UEAAHPEB_W_Z(CContextMenuManager* pThis, const wchar_t* profile) { return pThis ? pThis->LoadState(profile) : FALSE; }
// Symbol: ?SaveState@CContextMenuManager@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__SaveState_CContextMenuManager__UEAAHPEB_W_Z(CContextMenuManager* pThis, const wchar_t* profile) { return pThis ? pThis->SaveState(profile) : FALSE; }
CContextMenuManager::CContextMenuManager() { memset(_contextmenumanager_padding, 0, sizeof(_contextmenumanager_padding)); }
CContextMenuManager::~CContextMenuManager() {
    auto it = g_contextMenuStates.find(this);
    if (it != g_contextMenuStates.end()) {
        DestroyContextMenus(it->second);
        g_contextMenuStates.erase(it);
    }
}
BOOL CContextMenuManager::AddMenu(UINT uiMenuNameResId, UINT uiMenuResId) {
    wchar_t buffer[32] = {};
    _snwprintf(buffer, 32, L"%u", uiMenuNameResId);
    return AddMenu(buffer, uiMenuResId);
}
BOOL CContextMenuManager::AddMenu(const wchar_t* lpszName, UINT uiMenuResId) {
    ContextMenuState& state = g_contextMenuStates[this];
    HMENU hOwnedMenu = nullptr;
    HMENU hMenu = LoadMenuResource(uiMenuResId, &hOwnedMenu);
    auto itOld = state.ownedMenusById.find(uiMenuResId);
    if (itOld != state.ownedMenusById.end() && itOld->second && itOld->second != hOwnedMenu) {
        ::DestroyMenu(itOld->second);
    }
    state.menusById[uiMenuResId] = hMenu;
    state.ownedMenusById[uiMenuResId] = hOwnedMenu;
    if (lpszName) state.idsByName[lpszName] = uiMenuResId;
    return hMenu != nullptr;
}
HMENU CContextMenuManager::GetMenuById(UINT uiMenuResId) const {
    auto itState = g_contextMenuStates.find(this);
    if (itState == g_contextMenuStates.end()) return nullptr;
    auto it = itState->second.menusById.find(uiMenuResId);
    return it == itState->second.menusById.end() ? nullptr : it->second;
}
HMENU CContextMenuManager::GetMenuByName(const wchar_t* lpszName, UINT* puiMenuResId) const {
    auto itState = g_contextMenuStates.find(this);
    if (itState == g_contextMenuStates.end()) return nullptr;
    auto it = itState->second.idsByName.find(WideKey(lpszName));
    if (it == itState->second.idsByName.end()) return nullptr;
    if (puiMenuResId) *puiMenuResId = it->second;
    return GetMenuById(it->second);
}
void CContextMenuManager::GetMenuNames(CStringList& listOfNames) const {
    auto itState = g_contextMenuStates.find(this);
    if (itState == g_contextMenuStates.end()) return;
    for (const auto& entry : itState->second.idsByName) {
        listOfNames.AddTail(CString(entry.first.c_str()));
    }
}
BOOL CContextMenuManager::ShowPopupMenu(UINT uiMenuResId, int x, int y, CWnd* pWndOwner, BOOL bOwnMessage, BOOL bRightAlign) {
    HMENU hMenu = GetMenuById(uiMenuResId);
    if (!hMenu) return FALSE;
    return ShowPopupMenu(hMenu, x, y, pWndOwner, bOwnMessage, TRUE, bRightAlign) != nullptr;
}
CMFCPopupMenu* CContextMenuManager::ShowPopupMenu(HMENU hmenuPopup, int x, int y, CWnd* pWndOwner, BOOL, BOOL bAutoDestroy, BOOL) {
    (void)bAutoDestroy;
    if (!hmenuPopup) return nullptr;
    CMFCPopupMenu* pPopup = new CMFCPopupMenu();
    if (!pPopup->Create(pWndOwner, x, y, hmenuPopup, FALSE, FALSE)) {
        delete pPopup;
        return nullptr;
    }
    CMFCPopupMenu::ActivatePopupMenu(nullptr, pPopup);
    return pPopup;
}
UINT CContextMenuManager::TrackPopupMenu(HMENU hmenuPopup, int x, int y, CWnd* pWndOwner, BOOL) {
    if (!hmenuPopup) return 0;
    return static_cast<UINT>(::TrackPopupMenu(hmenuPopup, TPM_RETURNCMD, x, y, 0, pWndOwner ? pWndOwner->GetSafeHwnd() : nullptr, nullptr));
}
BOOL CContextMenuManager::LoadState(const wchar_t*) { return TRUE; }
BOOL CContextMenuManager::SaveState(const wchar_t*) { return TRUE; }
BOOL CContextMenuManager::ResetState() {
    auto& state = g_contextMenuStates[this];
    DestroyContextMenus(state);
    return TRUE;
}
