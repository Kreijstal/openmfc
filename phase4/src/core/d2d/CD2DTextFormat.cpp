// CD2DTextFormat — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

// Symbol: ??0CD2DTextFormat@@QEAA@PEAVCRenderTarget@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@MW4DWRITE_FONT_WEIGHT@@W4DWRITE_FONT_STYLE@@W4DWRITE_FONT_STRETCH@@1PEAUIDWriteFontCollection@@H@Z
extern "C" void MS_ABI impl___0CD2DTextFormat__QEAA_PEAVCRenderTarget__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_A___MW4DWRITE_FONT_WEIGHT__W4DWRITE_FONT_STYLE__W4DWRITE_FONT_STRETCH__1PEAUIDWriteFontCollection__H_Z(
    void* self, void*, const CString* fontFamilyName, float, unsigned int, unsigned int, unsigned int, const CString* localeName, void*, int) {
    D2DTextState& state = g_d2dTextFormatStates[self];
    state.fontFamilyName = CStringText(fontFamilyName);
    state.localeName = localeName && *CStringText(localeName) ? CStringText(localeName) : L"";
}
// Symbol: ??1CD2DTextFormat@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DTextFormat__UEAA_XZ(void* self) {
    g_d2dTextFormatStates.erase(self);
}
// Symbol: ?GetFontFamilyName@CD2DTextFormat@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFontFamilyName_CD2DTextFormat__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* ret, const void* self) {
    const D2DTextState* state = GetD2DFormatState(self);
    new (ret) CString(state ? state->fontFamilyName : CString());
}
// Symbol: ?GetLocaleName@CD2DTextFormat@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetLocaleName_CD2DTextFormat__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* ret, const void* self) {
    const D2DTextState* state = GetD2DFormatState(self);
    new (ret) CString(state ? state->localeName : CString());
}
