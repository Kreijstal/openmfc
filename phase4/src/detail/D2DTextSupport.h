#pragma once

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "detail/D2DResourceSupport.h"
#include <cstddef>
#include <vector>

#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DTextFormat__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DTextLayout__SAPEAUCRuntimeClass__XZ();

namespace openmfc { namespace detail { namespace d2dtext {

// Use the same process-wide factory as the exported MFC D2D state.
inline IDWriteFactory* Factory() { return OpenMfcD2DState().GetWriteFactory(); }

inline CString FormatName(IDWriteTextFormat* format, bool locale) {
    if (!format) return CString();
    UINT32 length = locale ? format->GetLocaleNameLength() : format->GetFontFamilyNameLength();
    if (!length) return CString();
    std::vector<wchar_t> buffer(static_cast<size_t>(length) + 1, L'\0');
    HRESULT hr = locale ? format->GetLocaleName(buffer.data(), length + 1)
                        : format->GetFontFamilyName(buffer.data(), length + 1);
    return SUCCEEDED(hr) ? CString(buffer.data()) : CString();
}

inline CString LayoutName(IDWriteTextLayout* layout, bool locale, UINT32 position, DWRITE_TEXT_RANGE* range) {
    if (!layout) return CString();
    UINT32 length = 0;
    HRESULT hr = locale ? layout->GetLocaleNameLength(position, &length, range)
                        : layout->GetFontFamilyNameLength(position, &length, range);
    if (FAILED(hr) || !length) return CString();
    std::vector<wchar_t> buffer(static_cast<size_t>(length) + 1, L'\0');
    hr = locale ? layout->GetLocaleName(position, buffer.data(), length + 1, range)
                : layout->GetFontFamilyName(position, buffer.data(), length + 1, range);
    return SUCCEEDED(hr) ? CString(buffer.data()) : CString();
}

} } }

class CD2DTextFormat : public CD2DResource {
public:
    CD2DTextFormat(CRenderTarget*, const CString&, FLOAT, DWRITE_FONT_WEIGHT,
                   DWRITE_FONT_STYLE, DWRITE_FONT_STRETCH, const CString&,
                   IDWriteFontCollection*, BOOL);
    ~CD2DTextFormat() override;
    CRuntimeClass* GetRuntimeClass() const override { return impl__GetThisClass_CD2DTextFormat__SAPEAUCRuntimeClass__XZ(); }
    HRESULT ReCreate(CRenderTarget*) override { return S_OK; }
    HRESULT Create(CRenderTarget*) override { return S_OK; }
    void Destroy() override {} // device-independent: retail preserves the format
    BOOL IsValid() const override { return m_pTextFormat != nullptr; }
    IDWriteTextFormat* m_pTextFormat;
};
static_assert(sizeof(CD2DTextFormat) == 32, "retail CD2DTextFormat layout");
static_assert(offsetof(CD2DTextFormat, m_pTextFormat) == 24, "retail text-format offset");

class CD2DTextLayout : public CD2DResource {
public:
    CD2DTextLayout(CRenderTarget*, const CString&, CD2DTextFormat&, const CD2DSizeF&, BOOL);
    ~CD2DTextLayout() override;
    CRuntimeClass* GetRuntimeClass() const override { return impl__GetThisClass_CD2DTextLayout__SAPEAUCRuntimeClass__XZ(); }
    HRESULT ReCreate(CRenderTarget*) override { return S_OK; }
    HRESULT Create(CRenderTarget*) override { return S_OK; }
    void Destroy() override {} // device-independent: retail preserves the layout
    BOOL IsValid() const override { return m_pTextLayout != nullptr; }
    IDWriteTextLayout* m_pTextLayout;
};
static_assert(sizeof(CD2DTextLayout) == 32, "retail CD2DTextLayout layout");
static_assert(offsetof(CD2DTextLayout, m_pTextLayout) == 24, "retail text-layout offset");
