#pragma once
// Shared internals of the former menucore.cpp translation unit.
// Definitions live in detail/CMenuSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmenu {} } }
using namespace openmfc::detail::cmenu;
// Menu Core Implementation
//
// Implements CMenu class for MFC menu operations.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstring>
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


namespace openmfc { namespace detail { namespace cmenu {
extern std::mutex g_ownedMenusMutex;
} } }
namespace openmfc { namespace detail { namespace cmenu {
extern std::unordered_set<HMENU> g_ownedMenus;
} } }
namespace openmfc { namespace detail { namespace cmenu {
extern std::mutex g_permanentMenusMutex;
} } }
namespace openmfc { namespace detail { namespace cmenu {
extern std::unordered_map<HMENU, CMenu*> g_permanentMenus;
} } }

namespace openmfc { namespace detail { namespace cmenu {
bool IsOwnedMenu(HMENU hMenu);
} } }

namespace openmfc { namespace detail { namespace cmenu {
void MarkOwnedMenu(HMENU hMenu);
} } }

namespace openmfc { namespace detail { namespace cmenu {
void UnmarkOwnedMenu(HMENU hMenu);
} } }

namespace openmfc { namespace detail { namespace cmenu {
extern thread_local std::unordered_map<HMENU, CMenu*> g_tempMenuMap;
} } }

namespace openmfc { namespace detail { namespace cmenu {
CMenu* FromHandleTemp(HMENU hMenu);
} } }

namespace openmfc { namespace detail { namespace cmenu {
void DeleteTempMenuMap();
} } }

namespace openmfc { namespace detail { namespace cmenu {
CMenu* FindPermanentMenu(HMENU hMenu);
} } }

namespace openmfc { namespace detail { namespace cmenu {
void RegisterPermanentMenu(CMenu* pMenu);
} } }

namespace openmfc { namespace detail { namespace cmenu {
void UnregisterPermanentMenu(CMenu* pMenu);
} } }




// CMenu default constructor
namespace openmfc { namespace detail { namespace cmenu {
extern "C" CMenu* MS_ABI impl___0CMenu__QEAA_XZ(CMenu* pThis);
} } }

// CMenu destructor
namespace openmfc { namespace detail { namespace cmenu {
extern "C" void MS_ABI impl___1CMenu__UEAA_XZ(CMenu* pThis);
} } }

// CMenu::CreateMenu
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__CreateMenu_CMenu__QEAAHXZ(CMenu* pThis);
} } }

// CMenu::CreatePopupMenu
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__CreatePopupMenu_CMenu__QEAAHXZ(CMenu* pThis);
} } }

// C++ members used by in-repo C++ code (the export impls above are the
// MSVC-ABI entry points; these satisfy internal C++ calls).


// CMenu::LoadMenuW
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__LoadMenuW_CMenu__QEAAHPEB_W_Z(CMenu* pThis, const wchar_t* lpszResourceName);
} } }

// CMenu::LoadMenuW (ID version)
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__LoadMenuW_CMenu__QEAAHI_Z(CMenu* pThis, UINT nIDResource);
} } }




// CMenu::GetSafeHmenu
namespace openmfc { namespace detail { namespace cmenu {
extern "C" HMENU MS_ABI impl__GetSafeHmenu_CMenu__QEBAPEAUHMENU____XZ(const CMenu* pThis);
} } }


// C++ method definition so other TUs (e.g. wincore) can call CMenu::FromHandle
// directly, mirroring CWnd::FromHandle.



// CMenu::GetSubMenu
namespace openmfc { namespace detail { namespace cmenu {
extern "C" CMenu* MS_ABI impl__GetSubMenu_CMenu__QEBAPEAVCMenu__H_Z(const CMenu* pThis, int nPos);
} } }

// CMenu::GetMenuItemCount
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__GetMenuItemCount_CMenu__QEBAHXZ(const CMenu* pThis);
} } }

// CMenu::GetMenuItemID
namespace openmfc { namespace detail { namespace cmenu {
extern "C" UINT MS_ABI impl__GetMenuItemID_CMenu__QEBAIH_Z(const CMenu* pThis, int nPos);
} } }

// CMenu::AppendMenuW
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__AppendMenuW_CMenu__QEAAHIPEB_W_Z(CMenu* pThis, UINT nFlags, const wchar_t* lpszNewItem);
} } }

// CMenu::AppendMenuW (with ID)
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__AppendMenuW_CMenu__QEAAHI_KPEB_W_Z(CMenu* pThis, UINT nFlags, UINT_PTR nIDNewItem, const wchar_t* lpszNewItem);
} } }

// CMenu::InsertMenuW
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__InsertMenuW_CMenu__QEAAHII_KPEB_W_Z(CMenu* pThis, UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const wchar_t* lpszNewItem);
} } }

// CMenu::ModifyMenuW
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__ModifyMenuW_CMenu__QEAAHII_KPEB_W_Z(CMenu* pThis, UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const wchar_t* lpszNewItem);
} } }

// CMenu::DeleteMenu
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__DeleteMenu_CMenu__QEAAHII_Z(CMenu* pThis, UINT nPosition, UINT nFlags);
} } }

// CMenu::RemoveMenu
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__RemoveMenu_CMenu__QEAAHII_Z(CMenu* pThis, UINT nPosition, UINT nFlags);
} } }

// CMenu::EnableMenuItem
namespace openmfc { namespace detail { namespace cmenu {
extern "C" UINT MS_ABI impl__EnableMenuItem_CMenu__QEAAIIH_Z(CMenu* pThis, UINT nIDEnableItem, UINT nEnable);
} } }

// CMenu::CheckMenuItem
namespace openmfc { namespace detail { namespace cmenu {
extern "C" UINT MS_ABI impl__CheckMenuItem_CMenu__QEAAIIH_Z(CMenu* pThis, UINT nIDCheckItem, UINT nCheck);
} } }

// CMenu::CheckMenuRadioItem
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__CheckMenuRadioItem_CMenu__QEAAHHHHH_Z(CMenu* pThis, UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags);
} } }

// CMenu::SetMenuItemBitmaps
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__SetMenuItemBitmaps_CMenu__QEAAHIPEAUHBITMAP____0_Z(
    CMenu* pThis, UINT nPosition, UINT nFlags, HBITMAP hBmpUnchecked, HBITMAP hBmpChecked);
} } }



// CMenu::GetMenuStringW
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__GetMenuStringW_CMenu__QEBAHIPEA_WHI_Z(const CMenu* pThis, UINT nIDItem, wchar_t* lpString, int nMaxCount, UINT nFlags);
} } }


// CMenu::GetMenuStringW (CString overload, full ATL CStringT mangled export name)
// The export map references this exact thunk name; forward to the shared implementation.
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__GetMenuStringW_CMenu__QEBAHIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(
    const CMenu* pThis, UINT nIDItem, CString& rString, UINT nFlags);
} } }

// CMenu::GetMenuState
namespace openmfc { namespace detail { namespace cmenu {
extern "C" UINT MS_ABI impl__GetMenuState_CMenu__QEBAIII_Z(const CMenu* pThis, UINT nID, UINT nFlags);
} } }

// CMenu::SetDefaultItem
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__SetDefaultItem_CMenu__QEAAHIH_Z(CMenu* pThis, UINT uItem, UINT fByPos);
} } }

// CMenu::GetDefaultItem
namespace openmfc { namespace detail { namespace cmenu {
extern "C" UINT MS_ABI impl__GetDefaultItem_CMenu__QEBAIIH_Z(const CMenu* pThis, UINT gmdiFlags, UINT fByPos);
} } }

// =============================================================================
// CWnd Menu Methods
// =============================================================================



// CWnd::DrawMenuBar
namespace openmfc { namespace detail { namespace cmenu {
extern "C" void MS_ABI impl__DrawMenuBar_CWnd__QEAAXXZ(CWnd* pThis);
} } }

// CWnd::GetSystemMenu
namespace openmfc { namespace detail { namespace cmenu {
extern "C" CMenu* MS_ABI impl__GetSystemMenu_CWnd__QEBAPEAVCMenu__H_Z(const CWnd* pThis, int bRevert);
} } }

// CWnd::HiliteMenuItem
namespace openmfc { namespace detail { namespace cmenu {
extern "C" int MS_ABI impl__HiliteMenuItem_CWnd__QEAAHPEAVCMenu__II_Z(CWnd* pThis, CMenu* pMenu, UINT nIDHiliteItem, UINT nHilite);
} } }

struct CCmdUIShim_Menucore {
    void* vftable;
    UINT m_nID;
    CCmdUI* m_pOther;
    int m_nIndex;
    CMenu* m_pMenu;
    CMenu* m_pSubMenu;
    char _padding[8];
};



// CMenu dynamic runtime wrappers needed by exported impl__* symbols
namespace openmfc { namespace detail { namespace cmenu {
extern "C" CObject* MS_ABI impl__CreateObject_CMenu__SAPEAVCObject__XZ();
} } }

extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMenu__UEBAPEAUCRuntimeClass__XZ(const CMenu* pThis);

namespace openmfc { namespace detail { namespace cmenu {
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMenu__SAPEAUCRuntimeClass__XZ();
} } }
