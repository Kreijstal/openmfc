// CD2DTextLayout — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

// Symbol: ??0CD2DTextLayout@@QEAA@PEAVCRenderTarget@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCD2DTextFormat@@AEBVCD2DSizeF@@H@Z
extern "C" void MS_ABI impl___0CD2DTextLayout__QEAA_PEAVCRenderTarget__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCD2DTextFormat__AEBVCD2DSizeF__H_Z(
    void* self, void*, const CString*, void* format, const CD2DSizeF*, int) {
    D2DTextState& state = g_d2dTextLayoutStates[self];
    if (const D2DTextState* formatState = GetD2DFormatState(format)) {
        state = *formatState;
    }
}
// Symbol: ??1CD2DTextLayout@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DTextLayout__UEAA_XZ(void* self) {
    g_d2dTextLayoutStates.erase(self);
}
// Symbol: ?SetFontFamilyName@CD2DTextLayout@@QEAAHPEB_WUDWRITE_TEXT_RANGE@@@Z
extern "C" int MS_ABI impl__SetFontFamilyName_CD2DTextLayout__QEAAHPEB_WUDWRITE_TEXT_RANGE___Z(
    void* self, const wchar_t* fontFamilyName, unsigned long long) {
    g_d2dTextLayoutStates[self].fontFamilyName = fontFamilyName ? fontFamilyName : L"";
    return TRUE;
}
// Symbol: ?SetLocaleName@CD2DTextLayout@@QEAAHPEB_WUDWRITE_TEXT_RANGE@@@Z
extern "C" int MS_ABI impl__SetLocaleName_CD2DTextLayout__QEAAHPEB_WUDWRITE_TEXT_RANGE___Z(
    void* self, const wchar_t* localeName, unsigned long long) {
    g_d2dTextLayoutStates[self].localeName = localeName ? localeName : L"";
    return TRUE;
}
// Symbol: ?GetFontFamilyName@CD2DTextLayout@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEAUDWRITE_TEXT_RANGE@@@Z
extern "C" void MS_ABI impl__GetFontFamilyName_CD2DTextLayout__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEAUDWRITE_TEXT_RANGE___Z(
    CString* ret, const void* self, unsigned int, void*) {
    const D2DTextState* state = GetD2DLayoutState(self);
    new (ret) CString(state ? state->fontFamilyName : CString());
}
// Symbol: ?GetLocaleName@CD2DTextLayout@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEAUDWRITE_TEXT_RANGE@@@Z
extern "C" void MS_ABI impl__GetLocaleName_CD2DTextLayout__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEAUDWRITE_TEXT_RANGE___Z(
    CString* ret, const void* self, unsigned int, void*) {
    const D2DTextState* state = GetD2DLayoutState(self);
    new (ret) CString(state ? state->localeName : CString());
}
