// CMenuTearOffManager — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

// Symbol: ??0CMenuTearOffManager@@QEAA@XZ
extern "C" void MS_ABI impl___0CMenuTearOffManager__QEAA_XZ(void* self) {
    g_menuTearOffStates[self] = MenuTearOffState();
}
// Symbol: ??1CMenuTearOffManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CMenuTearOffManager__UEAA_XZ(void* self) {
    g_menuTearOffStates.erase(self);
}
// Symbol: ?Initialize@CMenuTearOffManager@@QEAAHPEB_WII@Z
extern "C" int MS_ABI impl__Initialize_CMenuTearOffManager__QEAAHPEB_WII_Z(
    void* self, const wchar_t* registryKey, unsigned int firstId, unsigned int lastId) {
    MenuTearOffState& state = g_menuTearOffStates[self];
    state.registryKey = registryKey ? registryKey : L"";
    state.firstId = firstId;
    state.lastId = lastId;
    state.inUse.clear();
    return firstId <= lastId ? TRUE : FALSE;
}
// Symbol: ?SetInUse@CMenuTearOffManager@@QEAAXIH@Z
extern "C" void MS_ABI impl__SetInUse_CMenuTearOffManager__QEAAXIH_Z(void* self, unsigned int id, int inUse) {
    MenuTearOffState& state = g_menuTearOffStates[self];
    if (inUse) state.inUse.insert(id);
    else state.inUse.erase(id);
}
// Symbol: ?GetFreeTearOffID@CMenuTearOffManager@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__GetFreeTearOffID_CMenuTearOffManager__IEAAIXZ(void* self) {
    return NextTearOffId(g_menuTearOffStates[self]);
}
// Symbol: ?Build@CMenuTearOffManager@@QEAAXIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__Build_CMenuTearOffManager__QEAAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* self, unsigned int id, CString* value) {
    if (!value) return;
    wchar_t buffer[32] = {};
    wsprintfW(buffer, L"%u", id);
    *value = CString(buffer);
    g_menuTearOffStates[self].inUse.insert(id);
}
// Symbol: ?Parse@CMenuTearOffManager@@QEAAIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" unsigned int MS_ABI impl__Parse_CMenuTearOffManager__QEAAIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* self, CString* value) {
    unsigned long parsed = 0;
    if (!value || !ParseUnsignedDecimalOrHex(CStringText(value), &parsed)) return 0;
    g_menuTearOffStates[self].inUse.insert(static_cast<unsigned int>(parsed));
    return static_cast<unsigned int>(parsed);
}
// Symbol: ?Reset@CMenuTearOffManager@@QEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__Reset_CMenuTearOffManager__QEAAXPEAUHMENU_____Z(void* self, HMENU hMenu) {
    if (!::IsMenu(hMenu)) return;
    auto it = g_menuTearOffStates.find(self);
    if (it == g_menuTearOffStates.end()) return;
    MenuTearOffState& state = it->second;
    for (unsigned int id : state.tearOffItems) {
        ::RemoveMenu(hMenu, id, MF_BYCOMMAND);
    }
    state.tearOffItems.clear();
}
// Symbol: ?SetupTearOffMenus@CMenuTearOffManager@@QEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__SetupTearOffMenus_CMenuTearOffManager__QEAAXPEAUHMENU_____Z(void* self, HMENU hMenu) {
    if (!::IsMenu(hMenu)) return;
    auto it = g_menuTearOffStates.find(self);
    if (it == g_menuTearOffStates.end()) return;
    MenuTearOffState& state = it->second;
    state.tearOffItems.clear();
    int pos = 0;
    for (unsigned int id : state.inUse) {
        wchar_t buffer[32] = {};
        wsprintfW(buffer, L"%u", id);
        ::InsertMenuW(hMenu, pos, MF_BYPOSITION | MF_STRING, id, buffer);
        state.tearOffItems.push_back(id);
        ++pos;
    }
}
