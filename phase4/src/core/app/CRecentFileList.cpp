// CRecentFileList — OpenMFC implementation.
// Sources: appcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"

// Symbol: ??0CRecentFileList@@QEAA@IPEB_W0HH@Z
extern "C" void* MS_ABI impl___0CRecentFileList__QEAA_IPEB_W0HH_Z(
    void* pThis, unsigned int start, const wchar_t* section, const wchar_t* entryFormat, int size, int maxDispLen) {
    if (pThis) {
        RecentFileListState state;
        state.start = start;
        state.maxEntries = size < 0 ? 0 : size;
        state.maxDisplayLen = maxDispLen;
        state.section = section ? section : L"";
        state.entryFormat = entryFormat ? entryFormat : L"";
        g_recentFileListStates[reinterpret_cast<CRecentFileList*>(pThis)] = std::move(state);
    }
    return pThis;
}
// Symbol: ??1CRecentFileList@@UEAA@XZ
extern "C" void MS_ABI impl___1CRecentFileList__UEAA_XZ(CRecentFileList* pThis) {
    if (pThis) g_recentFileListStates.erase(pThis);
}
// Symbol: ?Add@CRecentFileList@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__Add_CRecentFileList__UEAAXPEB_W_Z(CRecentFileList* pThis, const wchar_t* pathName) {
    if (!pThis) return;
    RecentFileListState& state = g_recentFileListStates[pThis];
    AddRecentPath(state.entries, state.maxEntries, pathName);
}
// Symbol: ?Add@CRecentFileList@@UEAAXPEB_W0@Z
extern "C" void MS_ABI impl__Add_CRecentFileList__UEAAXPEB_W0_Z(CRecentFileList* pThis, const wchar_t* pathName, const wchar_t* displayName) {
    const wchar_t* chosen = (displayName && *displayName) ? displayName : pathName;
    impl__Add_CRecentFileList__UEAAXPEB_W_Z(pThis, chosen);
}
// Symbol: ?Add@CRecentFileList@@QEAAXPEAUIShellItem@@PEB_W@Z
extern "C" void MS_ABI impl__Add_CRecentFileList__QEAAXPEAUIShellItem__PEB_W_Z(CRecentFileList* pThis, IShellItem* pItem, const wchar_t* displayName) {
    (void)pItem;
    impl__Add_CRecentFileList__UEAAXPEB_W0_Z(pThis, nullptr, displayName);
}
// Symbol: ?Add@CRecentFileList@@QEAAXPEAUIShellLinkW@@PEB_W@Z
extern "C" void MS_ABI impl__Add_CRecentFileList__QEAAXPEAUIShellLinkW__PEB_W_Z(CRecentFileList* pThis, IShellLinkW* pLink, const wchar_t* displayName) {
    (void)pLink;
    impl__Add_CRecentFileList__UEAAXPEB_W0_Z(pThis, nullptr, displayName);
}
// Symbol: ?Add@CRecentFileList@@QEAAXPEFAU_ITEMIDLIST@@PEB_W@Z
extern "C" void MS_ABI impl__Add_CRecentFileList__QEAAXPEFAU_ITEMIDLIST__PEB_W_Z(
    CRecentFileList* pThis, const ITEMIDLIST* pidl, const wchar_t* displayName) {
    (void)pidl;
    impl__Add_CRecentFileList__UEAAXPEB_W0_Z(pThis, nullptr, displayName);
}
// Symbol: ?Remove@CRecentFileList@@UEAAXH@Z
extern "C" void MS_ABI impl__Remove_CRecentFileList__UEAAXH_Z(CRecentFileList* pThis, int index) {
    if (!pThis) return;
    auto& entries = g_recentFileListStates[pThis].entries;
    if (index < 0 || static_cast<size_t>(index) >= entries.size()) return;
    entries.erase(entries.begin() + index);
}
// Symbol: ?ReadList@CRecentFileList@@UEAAXXZ
extern "C" void MS_ABI impl__ReadList_CRecentFileList__UEAAXXZ(CRecentFileList* pThis) {
    if (!pThis) return;
    auto& state = g_recentFileListStates[pThis];
    if (state.maxEntries > 0 && static_cast<int>(state.entries.size()) > state.maxEntries) {
        state.entries.resize(static_cast<size_t>(state.maxEntries));
    }
}
// Symbol: ?WriteList@CRecentFileList@@UEAAXXZ
extern "C" void MS_ABI impl__WriteList_CRecentFileList__UEAAXXZ(CRecentFileList* pThis) {
    if (!pThis) return;
    if (g_recentFileListStates.find(pThis) == g_recentFileListStates.end()) {
        g_recentFileListStates[pThis] = RecentFileListState{};
    }
}
// Symbol: ?UpdateMenu@CRecentFileList@@UEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__UpdateMenu_CRecentFileList__UEAAXPEAVCCmdUI___Z(CRecentFileList* pThis, CCmdUI* pCmdUI) {
    if (!pThis || !pCmdUI) return;
    const auto it = g_recentFileListStates.find(pThis);
    if (it == g_recentFileListStates.end()) {
        pCmdUI->Enable(FALSE);
        return;
    }

    const RecentFileListState& state = it->second;
    const int index = static_cast<int>(pCmdUI->m_nID) - static_cast<int>(state.start);
    if (index < 0 || index >= static_cast<int>(state.entries.size())) {
        pCmdUI->Enable(FALSE);
        return;
    }

    std::wstring display = state.entries[static_cast<size_t>(index)];
    if (state.maxDisplayLen > 0 && static_cast<int>(display.size()) > state.maxDisplayLen) {
        display.resize(static_cast<size_t>(state.maxDisplayLen));
    }
    pCmdUI->SetText(display.c_str());
    pCmdUI->Enable(TRUE);
}
// Symbol: ?GetDisplayName@CRecentFileList@@UEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HPEB_WHH@Z
extern "C" int MS_ABI impl__GetDisplayName_CRecentFileList__UEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HPEB_WHH_Z(
    const CRecentFileList* pThis, CString* stringOut, int index, const wchar_t* curDir, int bAtLeastName, int bFullPath) {
    (void)curDir;
    (void)bAtLeastName;
    if (!pThis || !stringOut) return FALSE;
    const auto& entries = g_recentFileListStates[pThis].entries;
    if (index < 0 || static_cast<size_t>(index) >= entries.size()) return FALSE;
    const std::wstring& value = entries[static_cast<size_t>(index)];
    *stringOut = bFullPath ? value.c_str() : FindFileNamePart(value.c_str());
    return TRUE;
}
