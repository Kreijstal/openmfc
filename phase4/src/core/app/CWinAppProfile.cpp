#define OPENMFC_APPCORE_IMPL
#include "detail/CWinAppSupport.h"
#include "detail/AppProfile.h"

namespace openmfc::detail::cwinapp {
std::unordered_map<const CWinApp*, AppRuntimeState> g_appRuntimeStates;
}

namespace {
std::wstring MakeProfileKey(const wchar_t* section, const wchar_t* entry) {
    std::wstring key = section ? section : L"";
    key.push_back(L'\x1f');
    key.append(entry ? entry : L"");
    return key;
}

template<class Map>
void EraseProfileSection(Map& entries, const std::wstring& prefix) {
    for (auto it = entries.begin(); it != entries.end();) {
        if (it->first.compare(0, prefix.size(), prefix) == 0)
            it = entries.erase(it);
        else
            ++it;
    }
}
}

// Symbol: ?GetProfileIntW@CWinApp@@UEAAIPEB_W0H@Z
extern "C" unsigned int MS_ABI impl__GetProfileIntW_CWinApp__UEAAIPEB_W0H_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, int defaultValue) {
    if (!pThis) return static_cast<unsigned int>(defaultValue);
    const auto& map = g_appRuntimeStates[pThis].profileInts;
    const auto it = map.find(MakeProfileKey(section, entry));
    return it == map.end() ? static_cast<unsigned int>(defaultValue) : it->second;
}
// Symbol: ?GetProfileStringW@CWinApp@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W00@Z
extern "C" void MS_ABI impl__GetProfileStringW_CWinApp__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W00_Z(
    CString* ret, CWinApp* pThis, const wchar_t* section, const wchar_t* entry, const wchar_t* defaultValue) {
    if (!ret) return;
    if (!pThis) {
        new (ret) CString(defaultValue ? defaultValue : L"");
        return;
    }
    const auto& map = g_appRuntimeStates[pThis].profileStrings;
    const auto it = map.find(MakeProfileKey(section, entry));
    new (ret) CString(it == map.end() ? (defaultValue ? defaultValue : L"") : it->second.c_str());
}
// Symbol: ?GetProfileBinary@CWinApp@@UEAAHPEB_W0PEAPEAEPEAI@Z
extern "C" int MS_ABI impl__GetProfileBinary_CWinApp__UEAAHPEB_W0PEAPEAEPEAI_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, unsigned char** ppData, unsigned int* pBytes) {
    if (ppData) *ppData = nullptr;
    if (pBytes) *pBytes = 0;
    if (!pThis || !ppData || !pBytes) return FALSE;

    const auto& map = g_appRuntimeStates[pThis].profileBinary;
    const auto it = map.find(MakeProfileKey(section, entry));
    if (it == map.end()) return FALSE;

    const std::vector<unsigned char>& data = it->second;
    if (data.empty()) return FALSE;

    unsigned char* copy = new (std::nothrow) unsigned char[data.size()];
    if (!copy) return FALSE;
    std::memcpy(copy, data.data(), data.size());
    *ppData = copy;
    *pBytes = static_cast<unsigned int>(data.size());
    return TRUE;
}
// Symbol: ?WriteProfileInt@CWinApp@@UEAAHPEB_W0H@Z
extern "C" int MS_ABI impl__WriteProfileInt_CWinApp__UEAAHPEB_W0H_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, int value) {
    if (!pThis || !section || !entry) return FALSE;
    g_appRuntimeStates[pThis].profileInts[MakeProfileKey(section, entry)] = static_cast<unsigned int>(value);
    return TRUE;
}
// Symbol: ?WriteProfileStringW@CWinApp@@UEAAHPEB_W00@Z
extern "C" int MS_ABI impl__WriteProfileStringW_CWinApp__UEAAHPEB_W00_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, const wchar_t* value) {
    if (!pThis || !section) return FALSE;
    auto& state = g_appRuntimeStates[pThis];
    const std::wstring key = MakeProfileKey(section, entry);
    if (!entry) {
        EraseProfileSection(state.profileInts, key);
        EraseProfileSection(state.profileStrings, key);
        EraseProfileSection(state.profileBinary, key);
    } else if (!value) {
        state.profileInts.erase(key);
        state.profileStrings.erase(key);
        state.profileBinary.erase(key);
    } else {
        state.profileStrings[key] = value;
    }
    return TRUE;
}
// Symbol: ?WriteProfileBinary@CWinApp@@UEAAHPEB_W0PEAEI@Z
extern "C" int MS_ABI impl__WriteProfileBinary_CWinApp__UEAAHPEB_W0PEAEI_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, unsigned char* data, unsigned int bytes) {
    if (!pThis || !section || !entry) return FALSE;
    std::vector<unsigned char> blob;
    if (data && bytes != 0) blob.assign(data, data + bytes);
    g_appRuntimeStates[pThis].profileBinary[MakeProfileKey(section, entry)] = std::move(blob);
    return TRUE;
}
