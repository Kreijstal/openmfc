// CTagManager — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

// Symbol: ??0CTagManager@@QEAA@PEB_W@Z
extern "C" void MS_ABI impl___0CTagManager__QEAA_PEB_W_Z(void* self, const wchar_t* buffer) {
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates[self].buffer = buffer ? buffer : L"";
}
// Symbol: ??1CTagManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CTagManager__UEAA_XZ(void* self) {
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates.erase(self);
}
// Symbol: ?SetBuffer@CTagManager@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetBuffer_CTagManager__QEAAXPEB_W_Z(void* self, const wchar_t* buffer) {
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates[self].buffer = buffer ? buffer : L"";
}
// Symbol: ?LoadFromFile@CTagManager@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadFromFile_CTagManager__QEAAHPEB_W_Z(void* self, const wchar_t* path) {
    std::wstring buffer;
    if (!ReadFileToWideString(path, &buffer)) return FALSE;
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates[self].buffer = buffer;
    return TRUE;
}
// Symbol: ?LoadFromResource@CTagManager@@QEAAHIPEB_W@Z
extern "C" int MS_ABI impl__LoadFromResource_CTagManager__QEAAHIPEB_W_Z(void* self, unsigned int resourceId, const wchar_t* type) {
    HMODULE module = GetModuleHandleW(nullptr);
    HRSRC resource = FindResourceW(module, MAKEINTRESOURCEW(resourceId), type);
    std::wstring buffer;
    if (!ResourceToWideString(resource, &buffer)) return FALSE;
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates[self].buffer = buffer;
    return TRUE;
}
// Symbol: ?LoadFromResource@CTagManager@@QEAAHPEB_W0@Z
extern "C" int MS_ABI impl__LoadFromResource_CTagManager__QEAAHPEB_W0_Z(void* self, const wchar_t* resourceName, const wchar_t* type) {
    HMODULE module = GetModuleHandleW(nullptr);
    HRSRC resource = FindResourceW(module, resourceName, type);
    std::wstring buffer;
    if (!ResourceToWideString(resource, &buffer)) return FALSE;
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates[self].buffer = buffer;
    return TRUE;
}
// Symbol: ?ParseColor@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAK@Z
extern "C" int MS_ABI impl__ParseColor_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAK_Z(
    const CString* value, unsigned long* color) {
    return ParseUnsignedDecimalOrHex(CStringText(value), color) ? TRUE : FALSE;
}
// Symbol: ?ParseColorHEX@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAK@Z
extern "C" int MS_ABI impl__ParseColorHEX_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAK_Z(
    const CString* value, unsigned long* color) {
    return ParseUnsignedHex(CStringText(value), color) ? TRUE : FALSE;
}
// Symbol: ?ParsePoint@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCPoint@@@Z
extern "C" int MS_ABI impl__ParsePoint_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCPoint___Z(
    const CString* value, CPoint* point) {
    int parts[2] = {};
    if (!point || !ParseInts(CStringText(value), parts, 2)) return FALSE;
    point->SetPoint(parts[0], parts[1]);
    return TRUE;
}
// Symbol: ?ParseSize@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCSize@@@Z
extern "C" int MS_ABI impl__ParseSize_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCSize___Z(
    const CString* value, CSize* size) {
    int parts[2] = {};
    if (!size || !ParseInts(CStringText(value), parts, 2)) return FALSE;
    size->SetSize(parts[0], parts[1]);
    return TRUE;
}
// Symbol: ?ParseRect@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCRect@@@Z
extern "C" int MS_ABI impl__ParseRect_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCRect___Z(
    const CString* value, CRect* rect) {
    int parts[4] = {};
    if (!rect || !ParseInts(CStringText(value), parts, 4)) return FALSE;
    rect->SetRect(parts[0], parts[1], parts[2], parts[3]);
    return TRUE;
}
// Symbol: ?ParseFont@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAUtagLOGFONTW@@@Z
extern "C" int MS_ABI impl__ParseFont_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAUtagLOGFONTW___Z(
    const CString* value, LOGFONTW* font) {
    return ParseFontText(CStringText(value), font) ? TRUE : FALSE;
}
// Symbol: ?ParseString@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@0AEAVCStringArray@@HH@Z
extern "C" int MS_ABI impl__ParseString_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__0AEAVCStringArray__HH_Z(
    const CString* value, const CString* separators, CStringArray* output, int trim, int removeEmpty) {
    if (!value || !output) return FALSE;
    output->RemoveAll();

    const wchar_t* sep = separators ? static_cast<const wchar_t*>(*separators) : L",;";
    std::wstring text = static_cast<const wchar_t*>(*value);
    size_t start = 0;
    while (start <= text.size()) {
        size_t end = text.find_first_of(sep, start);
        std::wstring token = text.substr(start, end == std::wstring::npos ? std::wstring::npos : end - start);
        if (trim) {
            size_t first = token.find_first_not_of(L" \t\r\n");
            size_t last = token.find_last_not_of(L" \t\r\n");
            token = first == std::wstring::npos ? L"" : token.substr(first, last - first + 1);
        }
        if (!removeEmpty || !token.empty()) output->Add(CString(token.c_str()));
        if (end == std::wstring::npos) break;
        start = end + 1;
    }
    return TRUE;
}
// Symbol: ?ExcludeTag@CTagManager@@QEAAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" int MS_ABI impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    void* self, const wchar_t* tag, CString* value, int trim) {
    if (!value) return FALSE;
    std::wstring extracted;
    std::wstring buffer;
    {
        std::lock_guard<std::mutex> lock(g_tagManagerMutex);
        auto it = g_tagManagerStates.find(self);
        if (it == g_tagManagerStates.end()) return FALSE;
        buffer = it->second.buffer;
    }
    if (!ExtractNamedValue(buffer, tag, &extracted, trim != FALSE)) return FALSE;
    *value = CString(extracted.c_str());
    return TRUE;
}
// Symbol: ?ReadBool@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAH@Z
extern "C" int MS_ABI impl__ReadBool_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(
    void* self, const CString* tag, int* value) {
    std::wstring text;
    return value && GetTagManagerValue(self, tag, &text) && ParseBoolText(text.c_str(), value) ? TRUE : FALSE;
}
// Symbol: ?ReadColor@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAK@Z
extern "C" int MS_ABI impl__ReadColor_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAK_Z(
    void* self, const CString* tag, unsigned long* value) {
    std::wstring text;
    return value && GetTagManagerValue(self, tag, &text) && ParseUnsignedDecimalOrHex(text.c_str(), value) ? TRUE : FALSE;
}
// Symbol: ?ReadFont@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAUtagLOGFONTW@@@Z
extern "C" int MS_ABI impl__ReadFont_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAUtagLOGFONTW___Z(
    void* self, const CString* tag, LOGFONTW* value) {
    std::wstring text;
    return value && GetTagManagerValue(self, tag, &text) && ParseFontText(text.c_str(), value) ? TRUE : FALSE;
}
// Symbol: ?ReadInt@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAH@Z
extern "C" int MS_ABI impl__ReadInt_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(
    void* self, const CString* tag, int* value) {
    std::wstring text;
    return value && GetTagManagerValue(self, tag, &text) && ParseSingleInt(text.c_str(), value) ? TRUE : FALSE;
}
// Symbol: ?ReadPoint@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCPoint@@@Z
extern "C" int MS_ABI impl__ReadPoint_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCPoint___Z(
    void* self, const CString* tag, CPoint* value) {
    std::wstring text;
    int parts[2] = {};
    if (!value || !GetTagManagerValue(self, tag, &text) || !ParseInts(text.c_str(), parts, 2)) return FALSE;
    value->SetPoint(parts[0], parts[1]);
    return TRUE;
}
// Symbol: ?ReadRect@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCRect@@@Z
extern "C" int MS_ABI impl__ReadRect_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCRect___Z(
    void* self, const CString* tag, CRect* value) {
    std::wstring text;
    int parts[4] = {};
    if (!value || !GetTagManagerValue(self, tag, &text) || !ParseInts(text.c_str(), parts, 4)) return FALSE;
    value->SetRect(parts[0], parts[1], parts[2], parts[3]);
    return TRUE;
}
// Symbol: ?ReadSize@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCSize@@@Z
extern "C" int MS_ABI impl__ReadSize_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCSize___Z(
    void* self, const CString* tag, CSize* value) {
    std::wstring text;
    int parts[2] = {};
    if (!value || !GetTagManagerValue(self, tag, &text) || !ParseInts(text.c_str(), parts, 2)) return FALSE;
    value->SetSize(parts[0], parts[1]);
    return TRUE;
}
// Symbol: ?ParseControlRendererInfo@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRendererInfo@@@Z
extern "C" int MS_ABI impl__ParseControlRendererInfo_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRendererInfo___Z(
    const CString* value, CMFCControlRendererInfo* info) {
    if (!info) return FALSE;
    ControlRendererInfoState parsed;
    if (!ParseControlRendererInfoText(CStringText(value), &parsed)) return FALSE;
    g_controlRendererInfoStates[info] = parsed;
    return TRUE;
}
// Symbol: ?ParseControlRenderer@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRenderer@@I@Z
extern "C" int MS_ABI impl__ParseControlRenderer_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__I_Z(
    const CString* value, CMFCControlRenderer* renderer, unsigned int) {
    if (!renderer) return FALSE;
    CMFCControlRendererInfo info;
    if (!impl__ParseControlRendererInfo_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRendererInfo___Z(value, &info)) return FALSE;
    return renderer->Create(info, FALSE);
}
// Symbol: ?ParseControlRenderer@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRenderer@@PEB_W@Z
extern "C" int MS_ABI impl__ParseControlRenderer_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__PEB_W_Z(
    const CString* value, CMFCControlRenderer* renderer, const wchar_t*) {
    return impl__ParseControlRenderer_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__I_Z(value, renderer, 0);
}
// Symbol: ?ParseToolBarImages@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolBarImages@@I@Z
extern "C" int MS_ABI impl__ParseToolBarImages_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__I_Z(
    const CString* value, CMFCToolBarImages* images, unsigned int id) {
    if (!images) return FALSE;
    if (CStringText(value)[0] != L'\0') return LoadToolBarImagesFromText(CStringText(value), images, nullptr) ? TRUE : FALSE;
    return images->Load(id, nullptr, FALSE);
}
// Symbol: ?ParseToolBarImages@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolBarImages@@PEB_W@Z
extern "C" int MS_ABI impl__ParseToolBarImages_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__PEB_W_Z(
    const CString* value, CMFCToolBarImages* images, const wchar_t* resourceId) {
    if (!images) return FALSE;
    const wchar_t* source = CStringText(value)[0] != L'\0' ? CStringText(value) : resourceId;
    return LoadToolBarImagesFromText(source, images, nullptr) ? TRUE : FALSE;
}
// Symbol: ?ParseToolTipInfo@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolTipInfo@@@Z
extern "C" int MS_ABI impl__ParseToolTipInfo_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolTipInfo___Z(
    const CString* value, CMFCToolTipInfo* info) {
    if (!info) return FALSE;
    ToolTipInfoState parsed;
    if (!ParseToolTipInfoText(CStringText(value), &parsed)) return FALSE;
    g_toolTipInfoStates[info] = parsed;
    return TRUE;
}
// Symbol: ?ReadControlRendererInfo@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRendererInfo@@@Z
extern "C" int MS_ABI impl__ReadControlRendererInfo_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRendererInfo___Z(
    void* self, const CString* tag, CMFCControlRendererInfo* info) {
    std::wstring text;
    CString value;
    if (!info || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseControlRendererInfo_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRendererInfo___Z(&value, info);
}
// Symbol: ?ReadControlRenderer@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRenderer@@I@Z
extern "C" int MS_ABI impl__ReadControlRenderer_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__I_Z(
    void* self, const CString* tag, CMFCControlRenderer* renderer, unsigned int id) {
    std::wstring text;
    CString value;
    if (!renderer || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseControlRenderer_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__I_Z(&value, renderer, id);
}
// Symbol: ?ReadControlRenderer@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRenderer@@PEB_W@Z
extern "C" int MS_ABI impl__ReadControlRenderer_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__PEB_W_Z(
    void* self, const CString* tag, CMFCControlRenderer* renderer, const wchar_t* resourceId) {
    std::wstring text;
    CString value;
    if (!renderer || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseControlRenderer_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__PEB_W_Z(&value, renderer, resourceId);
}
// Symbol: ?ReadToolBarImages@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolBarImages@@I@Z
extern "C" int MS_ABI impl__ReadToolBarImages_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__I_Z(
    void* self, const CString* tag, CMFCToolBarImages* images, unsigned int id) {
    std::wstring text;
    CString value;
    if (!images || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseToolBarImages_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__I_Z(&value, images, id);
}
// Symbol: ?ReadToolBarImages@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolBarImages@@PEB_W@Z
extern "C" int MS_ABI impl__ReadToolBarImages_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__PEB_W_Z(
    void* self, const CString* tag, CMFCToolBarImages* images, const wchar_t* resourceId) {
    std::wstring text;
    CString value;
    if (!images || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseToolBarImages_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__PEB_W_Z(&value, images, resourceId);
}
// Symbol: ?ReadToolTipInfo@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolTipInfo@@@Z
extern "C" int MS_ABI impl__ReadToolTipInfo_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolTipInfo___Z(
    void* self, const CString* tag, CMFCToolTipInfo* info) {
    std::wstring text;
    CString value;
    if (!info || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseToolTipInfo_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolTipInfo___Z(&value, info);
}
