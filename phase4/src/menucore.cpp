// Menu Core Implementation
//
// Implements CMenu class for MFC menu operations.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <utility>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// CRuntimeClass implementation for CMenu
// =============================================================================

IMPLEMENT_DYNAMIC(CMenu, CObject)
// Symbol: ?CreateObject@CMenu@@SAPEAVCObject@@XZ
// Symbol: ?GetRuntimeClass@CMenu@@UEBAPEAUCRuntimeClass@@XZ
// Symbol: ?GetThisClass@CMenu@@SAPEAUCRuntimeClass@@XZ

#ifdef __GNUC__
// MSVC symbol alias for CMenu::classCMenu
asm(".globl \"?classCMenu@CMenu@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCMenu@CMenu@@2UCRuntimeClass@@A\", _ZN5CMenu10classCMenuE\n");
#endif

// =============================================================================
// CMenu Implementation
// =============================================================================

extern "C" void MS_ABI impl___1CMenu__UEAA_XZ(CMenu* pThis);

namespace {
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
} // namespace

CMenu::~CMenu() {
    impl___1CMenu__UEAA_XZ(this);
}

void CMenu::DrawItem(void*) {}

void CMenu::MeasureItem(void*) {}

// CMenu default constructor
extern "C" CMenu* MS_ABI impl___0CMenu__QEAA_XZ(CMenu* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hMenu = nullptr;
    return pThis;
}

// CMenu destructor
extern "C" void MS_ABI impl___1CMenu__UEAA_XZ(CMenu* pThis) {
    if (!pThis || !pThis->m_hMenu) return;

    UnregisterPermanentMenu(pThis);
    if (IsOwnedMenu(pThis->m_hMenu)) {
        ::DestroyMenu(pThis->m_hMenu);
        UnmarkOwnedMenu(pThis->m_hMenu);
    }
    pThis->m_hMenu = nullptr;
}

// CMenu::CreateMenu
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

// CMenu::CreatePopupMenu
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

// CMenu::LoadMenuW
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

// CMenu::LoadMenuW (ID version)
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

// CMenu::DestroyMenu
// Symbol: ?DestroyMenu@CMenu@@QEAAHXZ
// Ordinal: 3803
extern "C" int MS_ABI impl__DestroyMenu_CMenu__QEAAHXZ(CMenu* pThis) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    UnregisterPermanentMenu(pThis);
    int result = ::DestroyMenu(pThis->m_hMenu);
    UnmarkOwnedMenu(pThis->m_hMenu);
    pThis->m_hMenu = nullptr;
    return result;
}

// CMenu::Attach
// Symbol: ?Attach@CMenu@@QEAAHPEAUHMENU__@@@Z
// Ordinal: 2479
extern "C" int MS_ABI impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(CMenu* pThis, HMENU hMenu) {
    if (!pThis) return FALSE;
    UnregisterPermanentMenu(pThis);
    if (pThis->m_hMenu && IsOwnedMenu(pThis->m_hMenu)) {
        ::DestroyMenu(pThis->m_hMenu);
        UnmarkOwnedMenu(pThis->m_hMenu);
    }
    pThis->m_hMenu = hMenu;
    MarkOwnedMenu(pThis->m_hMenu);
    RegisterPermanentMenu(pThis);
    return TRUE;
}

// CMenu::Detach
// Symbol: ?Detach@CMenu@@QEAAPEAUHMENU__@@XZ
// Ordinal: 3832
extern "C" HMENU MS_ABI impl__Detach_CMenu__QEAAPEAUHMENU____XZ(CMenu* pThis) {
    if (!pThis) return nullptr;
    HMENU h = pThis->m_hMenu;
    UnregisterPermanentMenu(pThis);
    UnmarkOwnedMenu(h);
    pThis->m_hMenu = nullptr;
    return h;
}

// CMenu::GetSafeHmenu
extern "C" HMENU MS_ABI impl__GetSafeHmenu_CMenu__QEBAPEAUHMENU____XZ(const CMenu* pThis) {
    return pThis ? pThis->m_hMenu : nullptr;
}

// CMenu::FromHandle
// Symbol: ?FromHandle@CMenu@@SAPEAV1@PEAUHMENU__@@@Z
extern "C" CMenu* MS_ABI impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU hMenu) {
    if (CMenu* permanent = FindPermanentMenu(hMenu)) {
        return permanent;
    }
    return FromHandleTemp(hMenu);
}

// CMenu::FromHandlePermanent
// Symbol: ?FromHandlePermanent@CMenu@@SAPEAV1@PEAUHMENU__@@@Z
extern "C" CMenu* MS_ABI impl__FromHandlePermanent_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU hMenu) {
    return FindPermanentMenu(hMenu);
}

// CMenu::DeleteTempMap
// Symbol: ?DeleteTempMap@CMenu@@SAXXZ
extern "C" void MS_ABI impl__DeleteTempMap_CMenu__SAXXZ() {
    DeleteTempMenuMap();
}

// CMenu::GetSubMenu
extern "C" CMenu* MS_ABI impl__GetSubMenu_CMenu__QEBAPEAVCMenu__H_Z(const CMenu* pThis, int nPos) {
    if (!pThis || !pThis->m_hMenu) return nullptr;
    HMENU hSub = ::GetSubMenu(pThis->m_hMenu, nPos);
    return FromHandleTemp(hSub);
}

// CMenu::GetMenuItemCount
extern "C" int MS_ABI impl__GetMenuItemCount_CMenu__QEBAHXZ(const CMenu* pThis) {
    if (!pThis || !pThis->m_hMenu) return 0;
    return ::GetMenuItemCount(pThis->m_hMenu);
}

// CMenu::GetMenuItemID
extern "C" UINT MS_ABI impl__GetMenuItemID_CMenu__QEBAIH_Z(const CMenu* pThis, int nPos) {
    if (!pThis || !pThis->m_hMenu) return 0;
    return ::GetMenuItemID(pThis->m_hMenu, nPos);
}

// CMenu::AppendMenuW
extern "C" int MS_ABI impl__AppendMenuW_CMenu__QEAAHIPEB_W_Z(CMenu* pThis, UINT nFlags, const wchar_t* lpszNewItem) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::AppendMenuW(pThis->m_hMenu, nFlags, 0, lpszNewItem);
}

// CMenu::AppendMenuW (with ID)
extern "C" int MS_ABI impl__AppendMenuW_CMenu__QEAAHI_KPEB_W_Z(CMenu* pThis, UINT nFlags, UINT_PTR nIDNewItem, const wchar_t* lpszNewItem) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::AppendMenuW(pThis->m_hMenu, nFlags, nIDNewItem, lpszNewItem);
}

// CMenu::InsertMenuW
extern "C" int MS_ABI impl__InsertMenuW_CMenu__QEAAHII_KPEB_W_Z(CMenu* pThis, UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const wchar_t* lpszNewItem) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::InsertMenuW(pThis->m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
}

// CMenu::ModifyMenuW
extern "C" int MS_ABI impl__ModifyMenuW_CMenu__QEAAHII_KPEB_W_Z(CMenu* pThis, UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const wchar_t* lpszNewItem) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::ModifyMenuW(pThis->m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
}

// CMenu::DeleteMenu
extern "C" int MS_ABI impl__DeleteMenu_CMenu__QEAAHII_Z(CMenu* pThis, UINT nPosition, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::DeleteMenu(pThis->m_hMenu, nPosition, nFlags);
}

// CMenu::RemoveMenu
extern "C" int MS_ABI impl__RemoveMenu_CMenu__QEAAHII_Z(CMenu* pThis, UINT nPosition, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::RemoveMenu(pThis->m_hMenu, nPosition, nFlags);
}

// CMenu::EnableMenuItem
extern "C" UINT MS_ABI impl__EnableMenuItem_CMenu__QEAAIIH_Z(CMenu* pThis, UINT nIDEnableItem, UINT nEnable) {
    if (!pThis || !pThis->m_hMenu) return (UINT)-1;
    return ::EnableMenuItem(pThis->m_hMenu, nIDEnableItem, nEnable);
}

// CMenu::CheckMenuItem
extern "C" UINT MS_ABI impl__CheckMenuItem_CMenu__QEAAIIH_Z(CMenu* pThis, UINT nIDCheckItem, UINT nCheck) {
    if (!pThis || !pThis->m_hMenu) return (UINT)-1;
    return ::CheckMenuItem(pThis->m_hMenu, nIDCheckItem, nCheck);
}

// CMenu::CheckMenuRadioItem
extern "C" int MS_ABI impl__CheckMenuRadioItem_CMenu__QEAAHHHHH_Z(CMenu* pThis, UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::CheckMenuRadioItem(pThis->m_hMenu, nIDFirst, nIDLast, nIDItem, nFlags);
}

// CMenu::SetMenuItemBitmaps
extern "C" int MS_ABI impl__SetMenuItemBitmaps_CMenu__QEAAHIPEAUHBITMAP____0_Z(
    CMenu* pThis, UINT nPosition, UINT nFlags, HBITMAP hBmpUnchecked, HBITMAP hBmpChecked) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::SetMenuItemBitmaps(pThis->m_hMenu, nPosition, nFlags, hBmpUnchecked, hBmpChecked);
}

// CMenu::TrackPopupMenu
// Symbol: ?TrackPopupMenu@CMenu@@QEAAHIHHPEAVCWnd@@PEBUtagRECT@@@Z
// Ordinal: 13999
extern "C" int MS_ABI impl__TrackPopupMenu_CMenu__QEAAHIHHPEAVCWnd__PEBUtagRECT___Z(
    CMenu* pThis, UINT nFlags, int x, int y, CWnd* pWnd, const RECT* lpRect) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    HWND hWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    return ::TrackPopupMenu(pThis->m_hMenu, nFlags, x, y, 0, hWnd, lpRect);
}

// CMenu::TrackPopupMenuEx
// Symbol: ?TrackPopupMenuEx@CMenu@@QEAAHIHHPEAVCWnd@@PEAUtagTPMPARAMS@@@Z
// Ordinal: 14000
extern "C" int MS_ABI impl__TrackPopupMenuEx_CMenu__QEAAHIHHPEAVCWnd__PEAUtagTPMPARAMS___Z(
    CMenu* pThis, UINT nFlags, int x, int y, CWnd* pWnd, LPTPMPARAMS lpParams) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    HWND hWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    return ::TrackPopupMenuEx(pThis->m_hMenu, nFlags, x, y, hWnd, lpParams);
}

// CMenu::GetMenuStringW
extern "C" int MS_ABI impl__GetMenuStringW_CMenu__QEBAHIPEA_WHI_Z(const CMenu* pThis, UINT nIDItem, wchar_t* lpString, int nMaxCount, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) return 0;
    return ::GetMenuStringW(pThis->m_hMenu, nIDItem, lpString, nMaxCount, nFlags);
}

// CMenu::GetMenuStringW (CString overload)
// Symbol: ?GetMenuStringW@CMenu@@QEBAHIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@I@Z
extern "C" int MS_ABI impl__GetMenuStringW_CMenu__QEBAHIAEAV_CString_I_Z(const CMenu* pThis, UINT nIDItem, CString& rString, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) {
        rString.Empty();
        return 0;
    }

    int len = ::GetMenuStringW(pThis->m_hMenu, nIDItem, nullptr, 0, nFlags);
    if (len <= 0) {
        rString.Empty();
        return 0;
    }

    wchar_t* buffer = rString.GetBuffer(len);
    int copied = ::GetMenuStringW(pThis->m_hMenu, nIDItem, buffer, len + 1, nFlags);
    rString.ReleaseBuffer((copied >= 0) ? copied : 0);
    return (copied >= 0) ? copied : 0;
}

// CMenu::GetMenuState
extern "C" UINT MS_ABI impl__GetMenuState_CMenu__QEBAIII_Z(const CMenu* pThis, UINT nID, UINT nFlags) {
    if (!pThis || !pThis->m_hMenu) return (UINT)-1;
    return ::GetMenuState(pThis->m_hMenu, nID, nFlags);
}

// CMenu::SetDefaultItem
extern "C" int MS_ABI impl__SetDefaultItem_CMenu__QEAAHIH_Z(CMenu* pThis, UINT uItem, UINT fByPos) {
    if (!pThis || !pThis->m_hMenu) return FALSE;
    return ::SetMenuDefaultItem(pThis->m_hMenu, uItem, fByPos);
}

// CMenu::GetDefaultItem
extern "C" UINT MS_ABI impl__GetDefaultItem_CMenu__QEBAIIH_Z(const CMenu* pThis, UINT gmdiFlags, UINT fByPos) {
    if (!pThis || !pThis->m_hMenu) return (UINT)-1;
    return ::GetMenuDefaultItem(pThis->m_hMenu, fByPos, gmdiFlags);
}

// =============================================================================
// CWnd Menu Methods
// =============================================================================

// CWnd::GetMenu
// Symbol: ?GetMenu@CWnd@@UEBAPEAVCMenu@@XZ
// Ordinal: 5723
extern "C" CMenu* MS_ABI impl__GetMenu_CWnd__UEBAPEAVCMenu__XZ(const CWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) {
        return nullptr;
    }
    HMENU hMenu = ::GetMenu(pThis->m_hWnd);
    return FromHandleTemp(hMenu);
}

// CWnd::SetMenu
// Symbol: ?SetMenu@CWnd@@UEAAHPEAVCMenu@@@Z
// Ordinal: 13354
extern "C" int MS_ABI impl__SetMenu_CWnd__UEAAHPEAVCMenu___Z(CWnd* pThis, CMenu* pMenu) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    HMENU hMenu = pMenu ? pMenu->m_hMenu : nullptr;
    return ::SetMenu(pThis->m_hWnd, hMenu);
}

// CWnd::DrawMenuBar
extern "C" void MS_ABI impl__DrawMenuBar_CWnd__QEAAXXZ(CWnd* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::DrawMenuBar(pThis->m_hWnd);
    }
}

// CWnd::GetSystemMenu
extern "C" CMenu* MS_ABI impl__GetSystemMenu_CWnd__QEBAPEAVCMenu__H_Z(const CWnd* pThis, int bRevert) {
    if (!pThis || !pThis->m_hWnd) {
        return nullptr;
    }
    HMENU hMenu = ::GetSystemMenu(pThis->m_hWnd, bRevert);
    return FromHandleTemp(hMenu);
}

// CWnd::HiliteMenuItem
extern "C" int MS_ABI impl__HiliteMenuItem_CWnd__QEAAHPEAVCMenu__II_Z(CWnd* pThis, CMenu* pMenu, UINT nIDHiliteItem, UINT nHilite) {
    if (!pThis || !pThis->m_hWnd || !pMenu || !pMenu->m_hMenu) return FALSE;
    return ::HiliteMenuItem(pThis->m_hWnd, pMenu->m_hMenu, nIDHiliteItem, nHilite);
}

struct CCmdUIShim {
    void* vftable;
    UINT m_nID;
    CCmdUI* m_pOther;
    int m_nIndex;
    CMenu* m_pMenu;
    CMenu* m_pSubMenu;
    char _padding[8];
};

// CCmdUI::DoUpdate
// Symbol: ?DoUpdate@CCmdUI@@QEAAHPEAVCCmdTarget@@H@Z
extern "C" int MS_ABI impl__DoUpdate_CCmdUI__QEAAHPEAVCCmdTarget__H_Z(CCmdUI* pThis, CCmdTarget* pTarget, int bDisableIfNoHndler) {
    if (!pThis) return FALSE;

    auto* ui = reinterpret_cast<CCmdUIShim*>(pThis);
    CMenu* pMenu = ui->m_pSubMenu ? ui->m_pSubMenu : ui->m_pMenu;
    if (pMenu && ui->m_nID != 0U && ui->m_nID != static_cast<UINT>(-1)) {
        UINT enableFlags = MF_BYCOMMAND;
        if (!pTarget && bDisableIfNoHndler) {
            enableFlags |= MF_DISABLED | MF_GRAYED;
        } else {
            enableFlags |= MF_ENABLED;
        }
        pMenu->EnableMenuItem(ui->m_nID, enableFlags);
    }

    return TRUE;
}

// CCmdUI::SetRadio
// Symbol: ?SetRadio@CCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int bOn) {
    if (!pThis) return;

    auto* ui = reinterpret_cast<CCmdUIShim*>(pThis);
    CMenu* pMenu = ui->m_pSubMenu ? ui->m_pSubMenu : ui->m_pMenu;
    if (!pMenu || ui->m_nID == 0U || ui->m_nID == static_cast<UINT>(-1)) return;

    const UINT checkState = MF_BYCOMMAND | (bOn ? MF_CHECKED : MF_UNCHECKED);
    pMenu->CheckMenuItem(ui->m_nID, checkState);
}
