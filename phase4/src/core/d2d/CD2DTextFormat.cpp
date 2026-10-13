// CD2DTextFormat: device-independent DirectWrite format with a retail layout.
// Retail mfc140u: constructor 0xd47d0, getters 0xd49a0 / 0xd4ae0.

#define OPENMFC_APPCORE_IMPL

#include "detail/D2DTextSupport.h"

CD2DTextFormat::CD2DTextFormat(CRenderTarget* parent, const CString& family, FLOAT size,
    DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch,
    const CString& locale, IDWriteFontCollection* collection, BOOL autoDestroy)
    : CD2DResource(parent, autoDestroy), m_pTextFormat(nullptr) {
    if (IDWriteFactory* factory = openmfc::detail::d2dtext::Factory()) {
        factory->CreateTextFormat(family.GetString(), collection, weight, style,
            stretch, size, locale.GetString(), &m_pTextFormat);
    }
}
CD2DTextFormat::~CD2DTextFormat() {
    if (m_pTextFormat) m_pTextFormat->Release();
    m_pTextFormat = nullptr;
}

// Symbol: ??0CD2DTextFormat@@QEAA@PEAVCRenderTarget@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@MW4DWRITE_FONT_WEIGHT@@W4DWRITE_FONT_STYLE@@W4DWRITE_FONT_STRETCH@@1PEAUIDWriteFontCollection@@H@Z
extern "C" void* MS_ABI impl___0CD2DTextFormat__QEAA_PEAVCRenderTarget__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_A___MW4DWRITE_FONT_WEIGHT__W4DWRITE_FONT_STYLE__W4DWRITE_FONT_STRETCH__1PEAUIDWriteFontCollection__H_Z(
    void* self, CRenderTarget* parent, const CString* family, float size,
    DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch,
    const CString* locale, IDWriteFontCollection* collection, int autoDestroy) {
    if (!self) return nullptr;
    return new (self) CD2DTextFormat(parent, family ? *family : CString(), size,
        weight, style, stretch, locale ? *locale : CString(), collection, autoDestroy);
}
// Symbol: ??1CD2DTextFormat@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DTextFormat__UEAA_XZ(void* self) {
    if (self) static_cast<CD2DTextFormat*>(self)->~CD2DTextFormat();
}
// CString member return ABI: RCX=this, RDX=result, RAX=result.
// Symbol: ?GetFontFamilyName@CD2DTextFormat@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" CString* MS_ABI impl__GetFontFamilyName_CD2DTextFormat__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CD2DTextFormat* self, CString* result) {
    if (!result) return nullptr;
    return new (result) CString(openmfc::detail::d2dtext::FormatName(self ? self->m_pTextFormat : nullptr, false));
}
// Symbol: ?GetLocaleName@CD2DTextFormat@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" CString* MS_ABI impl__GetLocaleName_CD2DTextFormat__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CD2DTextFormat* self, CString* result) {
    if (!result) return nullptr;
    return new (result) CString(openmfc::detail::d2dtext::FormatName(self ? self->m_pTextFormat : nullptr, true));
}
