#include "CMenuSupport.h"
#include <cstdlib>
#include <new>

// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__GetMenuStringW_CMenu__QEBAHIAEAV_CString_I_Z(const CMenu* pThis, UINT nIDItem, CString& rString, UINT nFlags);
extern "C" void* AFXAPI impl___2_YAPEAX_K_Z(size_t);
extern "C" CMenu* MS_ABI impl___0CMenu__QEAA_XZ(CMenu*);
extern "C" void MS_ABI impl___1CMenu__UEAA_XZ(CMenu*);
extern "C" HMENU MS_ABI impl__Detach_CMenu__QEAAPEAUHMENU____XZ(CMenu*);
extern "C" int MS_ABI impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(CMenu*, HMENU);
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t*, const wchar_t*);

IMPLEMENT_DYNAMIC(CMenu, CObject)
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMenu__UEBAPEAUCRuntimeClass__XZ(const CMenu* pThis) {
    return CMenu::GetThisClass();
}
namespace {
void AFXAPI ConstructMenu(CObject* object) {
    impl___0CMenu__QEAA_XZ(static_cast<CMenu*>(object));
}
void AFXAPI DestructMenu(CObject* object) {
    // Explicit destruction is statically qualified: temporary-map cleanup must
    // not dispatch through a potentially overwritten incoming vptr.
    static_cast<CMenu*>(object)->CMenu::~CMenu();
}
}
namespace openmfc { namespace detail {
CHandleMap* GetMenuHandleMap(bool create) {
    auto* state = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    if (!state->m_pmapHMENU && create) {
        auto* lease = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
        auto previous = lease->m_pfnNewHandler;
        lease->m_pfnNewHandler = &modulethreadstate::HandleAllocationFailure;
        auto* storage = static_cast<CHandleMap*>(impl___2_YAPEAX_K_Z(sizeof(CHandleMap)));
        CHandleMap* map = nullptr;
        if (storage) {
            try { map = ::new(storage) CHandleMap(&CMenu::classCMenu, ConstructMenu, DestructMenu, 8, 1); }
            catch (...) { std::free(storage); throw; }
        }
        state->m_pmapHMENU = map;
        impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pfnNewHandler = previous;
    }
    return state->m_pmapHMENU;
}
} }
namespace openmfc { namespace detail { namespace cmenu {
CMenu* FindPermanentMenu(HMENU hMenu) {
    auto* map = openmfc::detail::GetMenuHandleMap(false);
    return map ? static_cast<CMenu*>(map->m_permanentMap.GetValueAt(hMenu)) : nullptr;
}
void RegisterPermanentMenu(CMenu* pMenu) {
    if (!pMenu || !pMenu->m_hMenu) return;
    openmfc::detail::GetMenuHandleMap(true)->m_permanentMap[pMenu->m_hMenu] = pMenu;
}
void UnregisterPermanentMenu(CMenu* pMenu) {
    if (!pMenu) return;
    auto* map = openmfc::detail::GetMenuHandleMap(false);
    if (map)
        map->m_permanentMap.RemoveKey(pMenu->m_hMenu);
}
extern "C" CMenu* MS_ABI impl___0CMenu__QEAA_XZ(CMenu* pThis) {
    if (!pThis) return nullptr;
    // Match the native temporary-wrapper constructor, including installing
    // CMenu's root vptr rather than only initializing the HMENU field.
    return ::new (static_cast<void*>(pThis)) CMenu();
}
extern "C" void MS_ABI impl___1CMenu__UEAA_XZ(CMenu* pThis) {
    if (pThis) pThis->CMenu::~CMenu();
}
extern "C" int MS_ABI impl__CreateMenu_CMenu__QEAAHXZ(CMenu* pThis) {
    if (!pThis) return FALSE;
    return impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(pThis, ::CreateMenu());
}
extern "C" int MS_ABI impl__CreatePopupMenu_CMenu__QEAAHXZ(CMenu* pThis) {
    if (!pThis) return FALSE;
    return impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(pThis, ::CreatePopupMenu());
}
extern "C" int MS_ABI impl__LoadMenuW_CMenu__QEAAHPEB_W_Z(CMenu* pThis, const wchar_t* lpszResourceName) {
    if (!pThis) return FALSE;
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(lpszResourceName,
                                                                                 RT_MENU);
    return impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(pThis, ::LoadMenuW(hInst, lpszResourceName));
}
extern "C" int MS_ABI impl__LoadMenuW_CMenu__QEAAHI_Z(CMenu* pThis, UINT nIDResource) {
    if (!pThis) return FALSE;
    const wchar_t* resourceName = MAKEINTRESOURCEW(nIDResource);
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(resourceName, RT_MENU);
    return impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(pThis, ::LoadMenuW(hInst, resourceName));
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

// The alias must sit with the IMPLEMENT_DYNAMIC above that defines
// CMenu::classCMenu: .set resolves at assembly time, so lld rejects it
// when the target lives in another unit.
#ifdef __GNUC__
// MSVC symbol alias for CMenu::classCMenu
asm(".globl \"?classCMenu@CMenu@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCMenu@CMenu@@2UCRuntimeClass@@A\", _ZN5CMenu10classCMenuE\n");
#endif
