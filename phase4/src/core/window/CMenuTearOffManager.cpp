// CMenuTearOffManager — tear-off IDs on Win32 submenu captions.

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"
#include "detail/MenuTearOffSupport.h"

#include <limits>
#include <string>

namespace {
thread_local void* activeManager = nullptr;
constexpr wchar_t kTearOffMarker = L'\x01';

void SetInUse(MenuTearOffState& state, UINT id, bool inUse) {
    if (id < state.firstId || id > state.lastId || !id) return;
    if (inUse) {
        ++state.inUse[id];
    } else if (auto it = state.inUse.find(id); it != state.inUse.end() && --it->second == 0) {
        state.inUse.erase(it);
    }
}

enum class MenuUpdate { reserve, assign, release };

void UpdateSubmenus(MenuTearOffState& state, HMENU menu, MenuUpdate update) {
    const int count = ::GetMenuItemCount(menu);
    for (int i = 0; i < count; ++i) {
        MENUITEMINFOW info{sizeof(info)};
        info.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_SUBMENU;
        if (!::GetMenuItemInfoW(menu, i, TRUE, &info) || !info.hSubMenu) continue;

        CString title;
        ++info.cch;
        info.dwTypeData = title.GetBuffer(info.cch);
        const bool gotTitle = ::GetMenuItemInfoW(menu, i, TRUE, &info) != FALSE;
        title.ReleaseBuffer(gotTitle ? info.cch : 0);
        const UINT id = openmfc::detail::DecodeTearOffTitle(title);
        if (gotTitle && id && update == MenuUpdate::reserve && id >= state.firstId && id <= state.lastId) {
            state.menuIds.insert(id);
        } else if (gotTitle && !id && update == MenuUpdate::assign && (info.fType & MFT_MENUBARBREAK)) {
            if (const UINT next = NextTearOffId(state)) {
                CString encoded = openmfc::detail::EncodeTearOffTitle(next, title);
                MENUITEMINFOW caption{sizeof(caption), MIIM_STRING};
                caption.dwTypeData = encoded.GetBuffer();
                if (::SetMenuItemInfoW(menu, i, TRUE, &caption)) state.menuIds.insert(next);
                encoded.ReleaseBuffer();
            }
        } else if (id && update == MenuUpdate::release) {
            state.menuIds.erase(id);
        }
        UpdateSubmenus(state, info.hSubMenu, update);
    }
}

void SetupMenus(MenuTearOffState& state, HMENU menu) {
    UpdateSubmenus(state, menu, MenuUpdate::reserve);
    UpdateSubmenus(state, menu, MenuUpdate::assign);
}

BOOL Initialize(void* self, const wchar_t* key, UINT first, UINT last) {
    if (!self || !first || first > last) return FALSE;
    MenuTearOffState state;
    state.registryKey = key ? key : L"";
    state.firstId = first;
    state.lastId = last;
    g_menuTearOffStates.insert_or_assign(self, std::move(state));
    return TRUE;
}
}

CString openmfc::detail::EncodeTearOffTitle(UINT id, const CString& title) {
    CString encoded;
    encoded.Format(L"%c%u%c%ls", kTearOffMarker, id, kTearOffMarker, title.GetString());
    return encoded;
}

UINT openmfc::detail::DecodeTearOffTitle(CString& title) {
    const wchar_t* prefix = title.GetString();
    if (!prefix || *prefix++ != kTearOffMarker) return 0;
    UINT id = 0;
    for (; *prefix >= L'0' && *prefix <= L'9'; ++prefix) {
        const UINT digit = *prefix - L'0';
        if (id > ((std::numeric_limits<UINT>::max)() - digit) / 10) return 0;
        id = id * 10 + digit;
    }
    if (!id || *prefix != kTearOffMarker) return 0;
    const CString label(prefix + 1);
    title = label;
    return id;
}

void openmfc::detail::SetupActiveTearOffMenus(HMENU menu) {
    if (const auto it = g_menuTearOffStates.find(activeManager); it != g_menuTearOffStates.end()) {
        SetupMenus(it->second, menu);
    }
}

BOOL openmfc::detail::EnableAppTearOffMenus(void* app, const wchar_t* key, UINT first, UINT last) {
    if (activeManager || !Initialize(app, key, first, last)) return FALSE;
    activeManager = app;
    return TRUE;
}

void openmfc::detail::DisableAppTearOffMenus(void* app) {
    if (activeManager != app) return;
    activeManager = nullptr;
    g_menuTearOffStates.erase(app);
}

void openmfc::detail::RetainTearOffID(UINT id, bool retain) {
    if (const auto it = g_menuTearOffStates.find(activeManager); it != g_menuTearOffStates.end()) {
        SetInUse(it->second, id, retain);
    }
}

// Symbol: ??0CMenuTearOffManager@@QEAA@XZ
extern "C" void MS_ABI impl___0CMenuTearOffManager__QEAA_XZ(void* self) {
    if (self) g_menuTearOffStates[self] = MenuTearOffState();
}
// Symbol: ??1CMenuTearOffManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CMenuTearOffManager__UEAA_XZ(void* self) {
    g_menuTearOffStates.erase(self);
}
// Symbol: ?Initialize@CMenuTearOffManager@@QEAAHPEB_WII@Z
extern "C" BOOL MS_ABI impl__Initialize_CMenuTearOffManager__QEAAHPEB_WII_Z(
    void* self, const wchar_t* registryKey, UINT first, UINT last) {
    return Initialize(self, registryKey, first, last);
}
// Symbol: ?SetInUse@CMenuTearOffManager@@QEAAXIH@Z
extern "C" void MS_ABI impl__SetInUse_CMenuTearOffManager__QEAAXIH_Z(void* self, UINT id, BOOL inUse) {
    if (auto it = g_menuTearOffStates.find(self); it != g_menuTearOffStates.end()) SetInUse(it->second, id, inUse != FALSE);
}
// Symbol: ?GetFreeTearOffID@CMenuTearOffManager@@IEAAIXZ
extern "C" UINT MS_ABI impl__GetFreeTearOffID_CMenuTearOffManager__IEAAIXZ(void* self) {
    const auto it = g_menuTearOffStates.find(self);
    if (it == g_menuTearOffStates.end()) return 0;
    const UINT id = NextTearOffId(it->second);
    if (id) SetInUse(it->second, id, true);
    return id;
}
// Symbol: ?Build@CMenuTearOffManager@@QEAAXIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__Build_CMenuTearOffManager__QEAAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void*, UINT id, CString* title) {
    if (title) *title = openmfc::detail::EncodeTearOffTitle(id, *title);
}
// Symbol: ?Parse@CMenuTearOffManager@@QEAAIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" UINT MS_ABI impl__Parse_CMenuTearOffManager__QEAAIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void*, CString* title) {
    return title ? openmfc::detail::DecodeTearOffTitle(*title) : 0;
}
// Symbol: ?Reset@CMenuTearOffManager@@QEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__Reset_CMenuTearOffManager__QEAAXPEAUHMENU_____Z(void* self, HMENU menu) {
    const auto it = g_menuTearOffStates.find(self);
    if (it == g_menuTearOffStates.end()) return;
    if (menu) UpdateSubmenus(it->second, menu, MenuUpdate::release);
    else {
        it->second.menuIds.clear();
        it->second.inUse.clear();
    }
}
// Symbol: ?SetupTearOffMenus@CMenuTearOffManager@@QEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__SetupTearOffMenus_CMenuTearOffManager__QEAAXPEAUHMENU_____Z(void* self, HMENU menu) {
    if (const auto it = g_menuTearOffStates.find(self); it != g_menuTearOffStates.end()) {
        SetupMenus(it->second, menu);
    }
}

namespace {
constexpr UINT maxProfileMenuItems = 65535;
constexpr UINT maxProfileMenuDepth = 64;
constexpr UINT maxProfileLabelLength = 32767;
constexpr UINT storedMenuTypes = MFT_MENUBARBREAK | MFT_MENUBREAK | MFT_RADIOCHECK |
                                 MFT_SEPARATOR | MFT_RIGHTORDER | MFT_RIGHTJUSTIFY;
constexpr UINT storedMenuStates = MFS_CHECKED | MFS_DISABLED | MFS_DEFAULT;

void WriteProfileLabel(CArchive& archive, const wchar_t* label, UINT length) {
    if (length > maxProfileLabelLength) AfxThrowArchiveException(CArchiveException::badIndex, nullptr);
    archive << length;
    if (length) archive.Write(label, length * sizeof(wchar_t));
}

std::wstring ReadProfileLabel(CArchive& archive) {
    UINT length = 0;
    archive >> length;
    if (length > maxProfileLabelLength) AfxThrowArchiveException(CArchiveException::badIndex, nullptr);
    std::wstring label(length, L'\0');
    const UINT bytes = length * sizeof(wchar_t);
    if (bytes && archive.Read(label.data(), bytes) != bytes)
        AfxThrowArchiveException(CArchiveException::endOfFile, nullptr);
    if (label.find(L'\0') != std::wstring::npos) AfxThrowArchiveException(CArchiveException::badIndex, nullptr);
    return label;
}

void WriteProfileMenu(CArchive& archive, HMENU menu, UINT depth, UINT& remaining) {
    const int count = ::GetMenuItemCount(menu);
    if (count < 0 || static_cast<UINT>(count) > remaining || depth >= maxProfileMenuDepth)
        AfxThrowArchiveException(CArchiveException::badIndex, nullptr);
    remaining -= count;
    archive << static_cast<UINT>(count);
    for (int i = 0; i < count; ++i) {
        MENUITEMINFOW item{sizeof(item), MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING | MIIM_SUBMENU};
        if (!::GetMenuItemInfoW(menu, i, TRUE, &item) || item.cch > maxProfileLabelLength)
            AfxThrowArchiveException(CArchiveException::badIndex, nullptr);
        std::wstring label(item.cch + 1, L'\0');
        item.dwTypeData = label.data();
        item.cch = static_cast<UINT>(label.size());
        if (!::GetMenuItemInfoW(menu, i, TRUE, &item))
            AfxThrowArchiveException(CArchiveException::badIndex, nullptr);
        archive << item.wID << (item.fType & storedMenuTypes) << (item.fState & storedMenuStates)
                << static_cast<BYTE>(item.hSubMenu != nullptr);
        WriteProfileLabel(archive, label.c_str(), item.cch);
        if (item.hSubMenu) WriteProfileMenu(archive, item.hSubMenu, depth + 1, remaining);
    }
}

openmfc::detail::TearOffMenuHandle ReadProfileMenu(CArchive& archive, UINT depth, UINT& remaining) {
    UINT count = 0;
    archive >> count;
    if (count > remaining || depth >= maxProfileMenuDepth)
        AfxThrowArchiveException(CArchiveException::badIndex, nullptr);
    remaining -= count;
    openmfc::detail::TearOffMenuHandle menu(::CreatePopupMenu());
    if (!menu) return {};
    for (UINT i = 0; i < count; ++i) {
        MENUITEMINFOW item{sizeof(item), MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING};
        BYTE hasChild = 0;
        archive >> item.wID >> item.fType >> item.fState >> hasChild;
        if ((item.fType & ~storedMenuTypes) || (item.fState & ~storedMenuStates) || hasChild > 1) return {};
        auto label = ReadProfileLabel(archive);
        item.dwTypeData = label.data();
        openmfc::detail::TearOffMenuHandle child;
        if (hasChild) {
            child = ReadProfileMenu(archive, depth + 1, remaining);
            if (!child) return {};
            item.fMask |= MIIM_SUBMENU;
            item.hSubMenu = child.get();
        }
        if (!::InsertMenuItemW(menu.get(), i, TRUE, &item)) return {};
        child.release();
    }
    return menu;
}
}

void openmfc::detail::WriteTearOffToolbar(CArchive& archive, const TearOffToolbarProfile& profile) {
    archive << profile.id << profile.bounds.left << profile.bounds.top
            << profile.bounds.right << profile.bounds.bottom << static_cast<BYTE>(profile.floating);
    WriteProfileLabel(archive, profile.title, profile.title.GetLength());
    UINT remaining = maxProfileMenuItems;
    WriteProfileMenu(archive, profile.menu.get(), 0, remaining);
}

bool openmfc::detail::ReadTearOffToolbar(CArchive& archive, TearOffToolbarProfile& profile) {
    BYTE floating = 0;
    archive >> profile.id >> profile.bounds.left >> profile.bounds.top
            >> profile.bounds.right >> profile.bounds.bottom >> floating;
    profile.floating = floating != 0;
    const auto width = static_cast<long long>(profile.bounds.right) - profile.bounds.left;
    const auto height = static_cast<long long>(profile.bounds.bottom) - profile.bounds.top;
    if (!profile.id || profile.id == UINT(-1) || floating > 1 || width <= 0 || height <= 0 ||
        width > maxProfileLabelLength || height > maxProfileLabelLength) return false;
    profile.title = ReadProfileLabel(archive).c_str();
    UINT remaining = maxProfileMenuItems;
    profile.menu = ReadProfileMenu(archive, 0, remaining);
    return profile.menu && ::GetMenuItemCount(profile.menu.get()) > 0;
}
