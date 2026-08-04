// CSettingsStore — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

// Symbol: ??0CSettingsStore@@IEAA@XZ
extern "C" void* MS_ABI impl___0CSettingsStore__IEAA_XZ(void* self) {
    g_settingsStoreStates[self] = SettingsStoreState();
    return self;
}
// Symbol: ??0CSettingsStore@@QEAA@HH@Z
extern "C" void* MS_ABI impl___0CSettingsStore__QEAA_HH_Z(void* self, int, int readOnly) {
    SettingsStoreState& state = g_settingsStoreStates[self];
    state.readOnly = readOnly != FALSE;
    state.open = true;
    state.path = NormalizeSettingPath(L"Software\\OpenMFC");
    return self;
}
// Symbol: ??1CSettingsStore@@UEAA@XZ
extern "C" void MS_ABI impl___1CSettingsStore__UEAA_XZ(void* self) {
    g_settingsStoreStates.erase(self);
}
// Symbol: ?Open@CSettingsStore@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__Open_CSettingsStore__UEAAHPEB_W_Z(void* self, const wchar_t* path) {
    SettingsStoreState& state = g_settingsStoreStates[self];
    state.path = NormalizeSettingPath(path);
    state.open = true;
    return TRUE;
}
// Symbol: ?CreateKey@CSettingsStore@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(void* self, const wchar_t* key) {
    SettingsStoreState& state = g_settingsStoreStates[self];
    state.path = NormalizeSettingPath(state.path, key);
    state.open = true;
    (void)CurrentSettingsValues(self);
    return TRUE;
}
// Symbol: ?Close@CSettingsStore@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CSettingsStore__UEAAXXZ(void* self) {
    g_settingsStoreStates[self].open = false;
}
// Symbol: ?DeleteKey@CSettingsStore@@UEAAHPEB_WH@Z
extern "C" int MS_ABI impl__DeleteKey_CSettingsStore__UEAAHPEB_WH_Z(void* self, const wchar_t* key, int) {
    auto it = g_settingsStoreStates.find(self);
    std::wstring path = NormalizeSettingPath(it == g_settingsStoreStates.end() ? L"" : it->second.path, key);
    return g_settingsStoreValues.erase(path) > 0 ? TRUE : FALSE;
}
// Symbol: ?DeleteValue@CSettingsStore@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__DeleteValue_CSettingsStore__UEAAHPEB_W_Z(void* self, const wchar_t* name) {
    if (!name) return FALSE;
    auto& values = CurrentSettingsValues(self);
    return values.erase(name) > 0 ? TRUE : FALSE;
}
// Symbol: ?Write@CSettingsStore@@UEAAHPEB_W0@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_W0_Z(void* self, const wchar_t* name, const wchar_t* value) {
    if (!name) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::String;
    stored.stringValue = value ? value : L"";
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}
// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WH@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WH_Z(void* self, const wchar_t* name, int value) {
    if (!name) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::Dword;
    stored.dwordValue = static_cast<unsigned long>(value);
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}
// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WK@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WK_Z(void* self, const wchar_t* name, unsigned long value) {
    if (!name) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::Dword;
    stored.dwordValue = value;
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}
// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WPEAEI@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WPEAEI_Z(void* self, const wchar_t* name, const unsigned char* data, unsigned int size) {
    if (!name || (!data && size != 0)) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::Binary;
    stored.binaryValue.assign(data, data + size);
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}
// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WAEBVCRect@@@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WAEBVCRect___Z(void* self, const wchar_t* name, const CRect* rect) {
    if (!name || !rect) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::Rect;
    stored.rectValue = *rect;
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}
// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WPEAVCObject@@@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WPEAVCObject___Z(void* self, const wchar_t* name, CObject* object) {
    if (!name) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::ObjectPtr;
    stored.objectValue = object;
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}
// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WAEAVCObject@@@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WAEAVCObject___Z(void* self, const wchar_t* name, CObject* object) {
    return impl__Write_CSettingsStore__UEAAHPEB_WPEAVCObject___Z(self, name, object);
}
// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* self, const wchar_t* name, CString* value) {
    if (!name || !value) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end()) return FALSE;
    if (it->second.kind == SettingsValue::String) *value = it->second.stringValue;
    else {
        wchar_t buffer[64] = {};
        wsprintfW(buffer, L"%lu", it->second.dwordValue);
        *value = buffer;
    }
    return TRUE;
}
// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAH@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAH_Z(void* self, const wchar_t* name, int* value) {
    if (!name || !value) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end()) return FALSE;
    if (it->second.kind == SettingsValue::Dword) *value = static_cast<int>(it->second.dwordValue);
    else if (!ParseSingleInt(it->second.stringValue.GetString(), value)) return FALSE;
    return TRUE;
}
// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAK@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAK_Z(void* self, const wchar_t* name, unsigned long* value) {
    if (!name || !value) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end()) return FALSE;
    if (it->second.kind == SettingsValue::Dword) *value = it->second.dwordValue;
    else if (!ParseUnsignedDecimalOrHex(it->second.stringValue.GetString(), value)) return FALSE;
    return TRUE;
}
// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WPEAPEAEPEAI@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WPEAPEAEPEAI_Z(void* self, const wchar_t* name, unsigned char** data, unsigned int* size) {
    if (!name || !data || !size) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end() || it->second.kind != SettingsValue::Binary) return FALSE;
    *size = static_cast<unsigned int>(it->second.binaryValue.size());
    *data = nullptr;
    if (*size != 0) {
        *data = static_cast<unsigned char*>(::LocalAlloc(LMEM_FIXED, *size));
        if (!*data) return FALSE;
        memcpy(*data, it->second.binaryValue.data(), *size);
    }
    return TRUE;
}
// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAVCRect@@@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAVCRect___Z(void* self, const wchar_t* name, CRect* rect) {
    if (!name || !rect) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end() || it->second.kind != SettingsValue::Rect) return FALSE;
    *rect = it->second.rectValue;
    return TRUE;
}
// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAPEAVCObject@@@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAPEAVCObject___Z(void* self, const wchar_t* name, CObject** object) {
    if (!name || !object) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end() || it->second.kind != SettingsValue::ObjectPtr) return FALSE;
    *object = static_cast<CObject*>(it->second.objectValue);
    return TRUE;
}
// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAVCObject@@@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAVCObject___Z(void* self, const wchar_t* name, CObject* object) {
    CObject* stored = nullptr;
    return object && impl__Read_CSettingsStore__UEAAHPEB_WAEAPEAVCObject___Z(self, name, &stored) ? TRUE : FALSE;
}
