#include "CMenuSupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__GetMenuStringW_CMenu__QEBAHIAEAV_CString_I_Z(const CMenu* pThis, UINT nIDItem, CString& rString, UINT nFlags);

IMPLEMENT_DYNAMIC(CMenu, CObject)
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMenu__UEBAPEAUCRuntimeClass__XZ(const CMenu* pThis) {
    return CMenu::GetThisClass();
}
namespace openmfc { namespace detail { namespace cmenu {
std::mutex g_ownedMenusMutex;
std::unordered_set<HMENU> g_ownedMenus;
std::mutex g_permanentMenusMutex;
std::unordered_map<HMENU, CMenu*> g_permanentMenus;
bool IsOwnedMenu(HMENU hMenu) {
    if (!hMenu) return false;
    std::lock_guard<std::mutex> lock(g_ownedMenusMutex);
    return g_ownedMenus.find(hMenu) != g_ownedMenus.end();
}
void MarkOwnedMenu(HMENU hMenu) {
    if (!hMenu) return;
    std::lock_guard<std::mutex> lock(g_ownedMenusMutex);
    g_ownedMenus.insert(hMenu);
}
void UnmarkOwnedMenu(HMENU hMenu) {
    if (!hMenu) return;
    std::lock_guard<std::mutex> lock(g_ownedMenusMutex);
    g_ownedMenus.erase(hMenu);
}
thread_local std::unordered_map<HMENU, CMenu*> g_tempMenuMap;
CMenu* FromHandleTemp(HMENU hMenu) {
    if (!hMenu) return nullptr;

    if (auto it = g_tempMenuMap.find(hMenu); it != g_tempMenuMap.end()) {
        return it->second;
    }

    auto* wrapper = new CMenu();
    wrapper->m_hMenu = hMenu;
    g_tempMenuMap.emplace(hMenu, wrapper);
    return wrapper;
}
void DeleteTempMenuMap() {
    for (auto& [_, menu] : g_tempMenuMap) {
        delete menu;
    }
    g_tempMenuMap.clear();
}
CMenu* FindPermanentMenu(HMENU hMenu) {
    if (!hMenu) return nullptr;
    std::lock_guard<std::mutex> lock(g_permanentMenusMutex);
    auto it = g_permanentMenus.find(hMenu);
    return (it != g_permanentMenus.end()) ? it->second : nullptr;
}
void RegisterPermanentMenu(CMenu* pMenu) {
    if (!pMenu || !pMenu->m_hMenu) return;
    std::lock_guard<std::mutex> lock(g_permanentMenusMutex);
    g_permanentMenus[pMenu->m_hMenu] = pMenu;
}
void UnregisterPermanentMenu(CMenu* pMenu) {
    if (!pMenu) return;
    std::lock_guard<std::mutex> lock(g_permanentMenusMutex);
    if (pMenu->m_hMenu) {
        auto it = g_permanentMenus.find(pMenu->m_hMenu);
        if (it != g_permanentMenus.end() && it->second == pMenu) {
            g_permanentMenus.erase(it);
        }
    }
    for (auto it = g_permanentMenus.begin(); it != g_permanentMenus.end();) {
        if (it->second == pMenu) {
            it = g_permanentMenus.erase(it);
        } else {
            ++it;
        }
    }
}
extern "C" CMenu* MS_ABI impl___0CMenu__QEAA_XZ(CMenu* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hMenu = nullptr;
    return pThis;
}
extern "C" void MS_ABI impl___1CMenu__UEAA_XZ(CMenu* pThis) {
    if (!pThis || !pThis->m_hMenu) return;

    UnregisterPermanentMenu(pThis);
    if (IsOwnedMenu(pThis->m_hMenu)) {
        ::DestroyMenu(pThis->m_hMenu);
        UnmarkOwnedMenu(pThis->m_hMenu);
    }
    pThis->m_hMenu = nullptr;
}
extern "C" int MS_ABI impl__CreateMenu_CMenu__QEAAHXZ(CMenu* pThis) {
    if (!pThis) return FALSE;
    UnregisterPermanentMenu(pThis);
    if (pThis->m_hMenu && IsOwnedMenu(pThis->m_hMenu)) {
        ::DestroyMenu(pThis->m_hMenu);
        UnmarkOwnedMenu(pThis->m_hMenu);
    }
    pThis->m_hMenu = ::CreateMenu();
    MarkOwnedMenu(pThis->m_hMenu);
    RegisterPermanentMenu(pThis);
    return pThis->m_hMenu != nullptr;
}
extern "C" int MS_ABI impl__CreatePopupMenu_CMenu__QEAAHXZ(CMenu* pThis) {
    if (!pThis) return FALSE;
    UnregisterPermanentMenu(pThis);
    if (pThis->m_hMenu && IsOwnedMenu(pThis->m_hMenu)) {
        ::DestroyMenu(pThis->m_hMenu);
        UnmarkOwnedMenu(pThis->m_hMenu);
    }
    pThis->m_hMenu = ::CreatePopupMenu();
    MarkOwnedMenu(pThis->m_hMenu);
    RegisterPermanentMenu(pThis);
    return pThis->m_hMenu != nullptr;
}
extern "C" int MS_ABI impl__LoadMenuW_CMenu__QEAAHPEB_W_Z(CMenu* pThis, const wchar_t* lpszResourceName) {
    if (!pThis) return FALSE;
    UnregisterPermanentMenu(pThis);
    if (pThis->m_hMenu && IsOwnedMenu(pThis->m_hMenu)) {
        ::DestroyMenu(pThis->m_hMenu);
        UnmarkOwnedMenu(pThis->m_hMenu);
    }
    HINSTANCE hInst = AfxGetInstanceHandle();
    pThis->m_hMenu = ::LoadMenuW(hInst, lpszResourceName);
    MarkOwnedMenu(pThis->m_hMenu);
    RegisterPermanentMenu(pThis);
    return pThis->m_hMenu != nullptr;
}
extern "C" int MS_ABI impl__LoadMenuW_CMenu__QEAAHI_Z(CMenu* pThis, UINT nIDResource) {
    if (!pThis) return FALSE;
    UnregisterPermanentMenu(pThis);
    if (pThis->m_hMenu && IsOwnedMenu(pThis->m_hMenu)) {
        ::DestroyMenu(pThis->m_hMenu);
        UnmarkOwnedMenu(pThis->m_hMenu);
    }
    HINSTANCE hInst = AfxGetInstanceHandle();
    pThis->m_hMenu = ::LoadMenuW(hInst, MAKEINTRESOURCEW(nIDResource));
    MarkOwnedMenu(pThis->m_hMenu);
    RegisterPermanentMenu(pThis);
    return pThis->m_hMenu != nullptr;
}
extern "C" HMENU MS_ABI impl__GetSafeHmenu_CMenu__QEBAPEAUHMENU____XZ(const CMenu* pThis) {
    return pThis ? pThis->m_hMenu : nullptr;
}
extern "C" CMenu* MS_ABI impl__GetSubMenu_CMenu__QEBAPEAVCMenu__H_Z(const CMenu* pThis, int nPos) {
    if (!pThis || !pThis->m_hMenu) return nullptr;
    HMENU hSub = ::GetSubMenu(pThis->m_hMenu, nPos);
    return FromHandleTemp(hSub);
}
extern "C" int MS_ABI impl__GetMenuItemCount_CMenu__QEBAHXZ(const CMenu* pThis) {
    if (!pThis || !pThis->m_hMenu) return 0;
    return ::GetMenuItemCount(pThis->m_hMenu);
}
extern "C" UINT MS_ABI impl__GetMenuItemID_CMenu__QEBAIH_Z(const CMenu* pThis, int nPos) {
    if (!pThis || !pThis->m_hMenu) return 0;
    return ::GetMenuItemID(pThis->m_hMenu, nPos);
}
extern "C" int MS_ABI impl__AppendMenuW_CMenu__QEAAHIPEB_W_Z(CMenu* pThis, UINT nFlags, const wchar_t* lpszNewItem) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::AppendMenuW(pThis->m_hMenu, nFlags, 0, lpszNewItem);
}
extern "C" int MS_ABI impl__AppendMenuW_CMenu__QEAAHI_KPEB_W_Z(CMenu* pThis, UINT nFlags, UINT_PTR nIDNewItem, const wchar_t* lpszNewItem) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::AppendMenuW(pThis->m_hMenu, nFlags, nIDNewItem, lpszNewItem);
}
extern "C" int MS_ABI impl__InsertMenuW_CMenu__QEAAHII_KPEB_W_Z(CMenu* pThis, UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const wchar_t* lpszNewItem) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::InsertMenuW(pThis->m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
}
extern "C" int MS_ABI impl__ModifyMenuW_CMenu__QEAAHII_KPEB_W_Z(CMenu* pThis, UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const wchar_t* lpszNewItem) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::ModifyMenuW(pThis->m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
}
extern "C" int MS_ABI impl__DeleteMenu_CMenu__QEAAHII_Z(CMenu* pThis, UINT nPosition, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::DeleteMenu(pThis->m_hMenu, nPosition, nFlags);
}
extern "C" int MS_ABI impl__RemoveMenu_CMenu__QEAAHII_Z(CMenu* pThis, UINT nPosition, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::RemoveMenu(pThis->m_hMenu, nPosition, nFlags);
}
extern "C" UINT MS_ABI impl__EnableMenuItem_CMenu__QEAAIIH_Z(CMenu* pThis, UINT nIDEnableItem, UINT nEnable) {
    if (!pThis || !pThis->m_hMenu) return (UINT)-1;
    return ::EnableMenuItem(pThis->m_hMenu, nIDEnableItem, nEnable);
}
extern "C" UINT MS_ABI impl__CheckMenuItem_CMenu__QEAAIIH_Z(CMenu* pThis, UINT nIDCheckItem, UINT nCheck) {
    if (!pThis || !pThis->m_hMenu) return (UINT)-1;
    return ::CheckMenuItem(pThis->m_hMenu, nIDCheckItem, nCheck);
}
extern "C" int MS_ABI impl__CheckMenuRadioItem_CMenu__QEAAHHHHH_Z(CMenu* pThis, UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::CheckMenuRadioItem(pThis->m_hMenu, nIDFirst, nIDLast, nIDItem, nFlags);
}
extern "C" int MS_ABI impl__SetMenuItemBitmaps_CMenu__QEAAHIPEAUHBITMAP____0_Z(
    CMenu* pThis, UINT nPosition, UINT nFlags, HBITMAP hBmpUnchecked, HBITMAP hBmpChecked) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::SetMenuItemBitmaps(pThis->m_hMenu, nPosition, nFlags, hBmpUnchecked, hBmpChecked);
}
extern "C" int MS_ABI impl__GetMenuStringW_CMenu__QEBAHIPEA_WHI_Z(const CMenu* pThis, UINT nIDItem, wchar_t* lpString, int nMaxCount, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) return 0;
    return ::GetMenuStringW(pThis->m_hMenu, nIDItem, lpString, nMaxCount, nFlags);
}
extern "C" int MS_ABI impl__GetMenuStringW_CMenu__QEBAHIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(
    const CMenu* pThis, UINT nIDItem, CString& rString, UINT nFlags) {
    return impl__GetMenuStringW_CMenu__QEBAHIAEAV_CString_I_Z(pThis, nIDItem, rString, nFlags);
}
extern "C" UINT MS_ABI impl__GetMenuState_CMenu__QEBAIII_Z(const CMenu* pThis, UINT nID, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) return (UINT)-1;
    return ::GetMenuState(pThis->m_hMenu, nID, nFlags);
}
extern "C" int MS_ABI impl__SetDefaultItem_CMenu__QEAAHIH_Z(CMenu* pThis, UINT uItem, UINT fByPos) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::SetMenuDefaultItem(pThis->m_hMenu, uItem, fByPos);
}
extern "C" UINT MS_ABI impl__GetDefaultItem_CMenu__QEBAIIH_Z(const CMenu* pThis, UINT gmdiFlags, UINT fByPos) {
    if (!pThis || !pThis->m_hMenu) return (UINT)-1;
    return ::GetMenuDefaultItem(pThis->m_hMenu, fByPos, gmdiFlags);
}
extern "C" void MS_ABI impl__DrawMenuBar_CWnd__QEAAXXZ(CWnd* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::DrawMenuBar(pThis->m_hWnd);
    }
}
extern "C" CMenu* MS_ABI impl__GetSystemMenu_CWnd__QEBAPEAVCMenu__H_Z(const CWnd* pThis, int bRevert) {
    if (!pThis || !pThis->m_hWnd) {
        return nullptr;
    }
    HMENU hMenu = ::GetSystemMenu(pThis->m_hWnd, bRevert);
    return FromHandleTemp(hMenu);
}
extern "C" int MS_ABI impl__HiliteMenuItem_CWnd__QEAAHPEAVCMenu__II_Z(CWnd* pThis, CMenu* pMenu, UINT nIDHiliteItem, UINT nHilite) {
    if (!pThis || !pThis->m_hWnd || !pMenu || !pMenu->m_hMenu) return FALSE;
    return ::HiliteMenuItem(pThis->m_hWnd, pMenu->m_hMenu, nIDHiliteItem, nHilite);
}
extern "C" CObject* MS_ABI impl__CreateObject_CMenu__SAPEAVCObject__XZ() {
    return CMenu::GetThisClass()->CreateObject();
}
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMenu__SAPEAUCRuntimeClass__XZ() {
    return CMenu::GetThisClass();
}
} } }  // namespace openmfc::detail::cmenu
