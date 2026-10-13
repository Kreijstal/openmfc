#pragma once

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "detail/D2DFactorySupport.h"
#include <cstddef>
#include <vector>

#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif

extern "C" void* MS_ABI impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(void*, void*, int);
extern "C" void* MS_ABI impl___1CD2DResource__MEAA_XZ(void*);
extern "C" long MS_ABI impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(void*, void*);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DTextFormat__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DTextLayout__SAPEAUCRuntimeClass__XZ();
void OpenMfcRegisterD2DTextResource(CRenderTarget*, void*);
void OpenMfcUnregisterD2DTextResource(CRenderTarget*, void*);

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

// Private ABI models. The retail classes' Get()/IsAutoDestroy() are inline in
// client headers, so the BOOL, parent and interface must be stored in the object.
// CObject's five slots are followed by ReCreate/Create/Destroy/IsValid (5..8).
class CD2DResource : public CObject {
    friend class CRenderTarget;
protected:
    CD2DResource(CRenderTarget* parent, BOOL autoDestroy)
        : m_bIsAutoDestroy(autoDestroy), m_pParentTarget(parent) {
        impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(this, parent, autoDestroy);
        if (parent) OpenMfcRegisterD2DTextResource(parent, this);
    }
    ~CD2DResource() override {
        if (m_pParentTarget) OpenMfcUnregisterD2DTextResource(m_pParentTarget, this);
        impl___1CD2DResource__MEAA_XZ(this);
    }
    virtual HRESULT ReCreate(CRenderTarget* parent) {
        return impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(this, parent);
    }
public:
    CRuntimeClass* GetRuntimeClass() const override { return impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ(); }
    virtual HRESULT Create(CRenderTarget*) = 0;
    virtual void Destroy() = 0;
    virtual BOOL IsValid() const = 0;
    BOOL m_bIsAutoDestroy;
    CRenderTarget* m_pParentTarget;
};
static_assert(sizeof(CD2DResource) == 24, "retail CD2DResource layout");
static_assert(offsetof(CD2DResource, m_bIsAutoDestroy) == 8, "retail auto-destroy offset");
static_assert(offsetof(CD2DResource, m_pParentTarget) == 16, "retail parent offset");

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
