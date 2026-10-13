// CD2DTextLayout: native DirectWrite layout with per-range formatting.
// Retail mfc140u: constructor 0xd4c30, getters 0xd4e20 / 0xd4f90.

#define OPENMFC_APPCORE_IMPL

#include "detail/D2DTextSupport.h"

CD2DTextLayout::CD2DTextLayout(CRenderTarget* parent, const CString& text,
    CD2DTextFormat& format, const CD2DSizeF& size, BOOL autoDestroy)
    : CD2DResource(parent, autoDestroy), m_pTextLayout(nullptr) {
    if (IDWriteFactory* factory = openmfc::detail::d2dtext::Factory()) {
        factory->CreateTextLayout(text.GetString(), static_cast<UINT32>(text.GetLength()),
            format.m_pTextFormat, size.width, size.height, &m_pTextLayout);
    }
}
CD2DTextLayout::~CD2DTextLayout() {
    if (m_pTextLayout) m_pTextLayout->Release();
    m_pTextLayout = nullptr;
}

// Symbol: ??0CD2DTextLayout@@QEAA@PEAVCRenderTarget@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCD2DTextFormat@@AEBVCD2DSizeF@@H@Z
extern "C" void* MS_ABI impl___0CD2DTextLayout__QEAA_PEAVCRenderTarget__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCD2DTextFormat__AEBVCD2DSizeF__H_Z(
    void* self, CRenderTarget* parent, const CString* text, CD2DTextFormat* format, const CD2DSizeF* size, int autoDestroy) {
    if (!self || !format || !size) return nullptr;
    return new (self) CD2DTextLayout(parent, text ? *text : CString(), *format, *size, autoDestroy);
}
// Symbol: ??1CD2DTextLayout@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DTextLayout__UEAA_XZ(void* self) {
    if (self) static_cast<CD2DTextLayout*>(self)->~CD2DTextLayout();
}
// The 8-byte DWRITE_TEXT_RANGE aggregate arrives in R8 in the MSVC x64 ABI.
// Symbol: ?SetFontFamilyName@CD2DTextLayout@@QEAAHPEB_WUDWRITE_TEXT_RANGE@@@Z
extern "C" int MS_ABI impl__SetFontFamilyName_CD2DTextLayout__QEAAHPEB_WUDWRITE_TEXT_RANGE___Z(
    CD2DTextLayout* self, const wchar_t* family, DWRITE_TEXT_RANGE range) {
    return self && self->m_pTextLayout && SUCCEEDED(self->m_pTextLayout->SetFontFamilyName(family, range));
}
// Symbol: ?SetLocaleName@CD2DTextLayout@@QEAAHPEB_WUDWRITE_TEXT_RANGE@@@Z
extern "C" int MS_ABI impl__SetLocaleName_CD2DTextLayout__QEAAHPEB_WUDWRITE_TEXT_RANGE___Z(
    CD2DTextLayout* self, const wchar_t* locale, DWRITE_TEXT_RANGE range) {
    return self && self->m_pTextLayout && SUCCEEDED(self->m_pTextLayout->SetLocaleName(locale, range));
}
// CString member return ABI: RCX=this, RDX=result, R8D=position, R9=range.
// Symbol: ?GetFontFamilyName@CD2DTextLayout@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEAUDWRITE_TEXT_RANGE@@@Z
extern "C" CString* MS_ABI impl__GetFontFamilyName_CD2DTextLayout__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEAUDWRITE_TEXT_RANGE___Z(
    const CD2DTextLayout* self, CString* result, unsigned int position, DWRITE_TEXT_RANGE* range) {
    if (!result) return nullptr;
    return new (result) CString(openmfc::detail::d2dtext::LayoutName(self ? self->m_pTextLayout : nullptr, false, position, range));
}
// Symbol: ?GetLocaleName@CD2DTextLayout@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEAUDWRITE_TEXT_RANGE@@@Z
extern "C" CString* MS_ABI impl__GetLocaleName_CD2DTextLayout__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEAUDWRITE_TEXT_RANGE___Z(
    const CD2DTextLayout* self, CString* result, unsigned int position, DWRITE_TEXT_RANGE* range) {
    if (!result) return nullptr;
    return new (result) CString(openmfc::detail::d2dtext::LayoutName(self ? self->m_pTextLayout : nullptr, true, position, range));
}
