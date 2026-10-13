// Solid-color brushes: native creation, virtual lifecycle and MSVC field layout.
#include "detail/D2DResourceSupport.h"
#include <cstring>

CD2DSolidColorBrush::CD2DSolidColorBrush(CRenderTarget* parent, D2D1_COLOR_F color,
    const D2D1_BRUSH_PROPERTIES* props, BOOL autoDestroy)
    : CD2DBrush(parent, props, autoDestroy), m_pSolidColorBrush(nullptr), m_colorSolid(color) {}
CD2DSolidColorBrush::~CD2DSolidColorBrush() { CD2DSolidColorBrush::Destroy(); }
HRESULT CD2DSolidColorBrush::Create(CRenderTarget* parent) {
    if (!parent || !parent->GetRenderTarget() || m_pBrush || m_pSolidColorBrush) return E_FAIL;
    ID2D1SolidColorBrush* brush = nullptr;
    HRESULT hr = parent->GetRenderTarget()->CreateSolidColorBrush(&m_colorSolid, m_pBrushProperties, &brush);
    if (SUCCEEDED(hr)) { m_pBrush = brush; m_pSolidColorBrush = brush; }
    return hr;
}
void CD2DSolidColorBrush::Destroy() {
    CD2DBrush::Destroy();
    m_pSolidColorBrush = nullptr;
}

// Symbol: ??0CD2DSolidColorBrush@@QEAA@PEAVCRenderTarget@@KHPEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DSolidColorBrush__QEAA_PEAVCRenderTarget__KHPEAVCD2DBrushProperties__H_Z(
    void* self, void* parent, unsigned long color, int alpha, void* props, int autoDestroy) {
    D2D1_COLOR_F rgba{static_cast<float>(color & 255) / 255,
        static_cast<float>((color >> 8) & 255) / 255, static_cast<float>((color >> 16) & 255) / 255,
        static_cast<float>(alpha) / 255};
    return self ? new(self) CD2DSolidColorBrush(static_cast<CRenderTarget*>(parent), rgba,
        static_cast<const D2D1_BRUSH_PROPERTIES*>(props), autoDestroy) : nullptr;
}
// Symbol: ??0CD2DSolidColorBrush@@QEAA@PEAVCRenderTarget@@U_D3DCOLORVALUE@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DSolidColorBrush__QEAA_PEAVCRenderTarget__U_D3DCOLORVALUE__PEAVCD2DBrushProperties__H_Z(
    void* self, void* parent, const void* color, void* props, int autoDestroy) {
    return self && color ? new(self) CD2DSolidColorBrush(static_cast<CRenderTarget*>(parent),
        *static_cast<const D2D1_COLOR_F*>(color), static_cast<const D2D1_BRUSH_PROPERTIES*>(props), autoDestroy) : nullptr;
}
// Symbol: ??1CD2DSolidColorBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DSolidColorBrush__UEAA_XZ(void* self) {
    if (self) static_cast<CD2DSolidColorBrush*>(self)->CD2DSolidColorBrush::~CD2DSolidColorBrush();
}
// Symbol: ?Attach@CD2DSolidColorBrush@@QEAAXPEAUID2D1SolidColorBrush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DSolidColorBrush__QEAAXPEAUID2D1SolidColorBrush___Z(void* self, void* native) {
    if (!self) return;
    auto* brush = static_cast<CD2DSolidColorBrush*>(self);
    brush->m_pSolidColorBrush = static_cast<ID2D1SolidColorBrush*>(native);
    brush->m_pBrush = brush->m_pSolidColorBrush;
}
// Symbol: ?Create@CD2DSolidColorBrush@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DSolidColorBrush__UEAAJPEAVCRenderTarget___Z(void* self, void* parent) {
    return self ? static_cast<CD2DSolidColorBrush*>(self)->CD2DSolidColorBrush::Create(static_cast<CRenderTarget*>(parent)) : E_POINTER;
}
// Symbol: ?Destroy@CD2DSolidColorBrush@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DSolidColorBrush__UEAAXXZ(void* self) {
    if (self) static_cast<CD2DSolidColorBrush*>(self)->CD2DSolidColorBrush::Destroy();
}
// Symbol: ?Detach@CD2DSolidColorBrush@@QEAAPEAUID2D1SolidColorBrush@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DSolidColorBrush__QEAAPEAUID2D1SolidColorBrush__XZ(void* self) {
    if (!self) return nullptr;
    auto* brush = static_cast<CD2DSolidColorBrush*>(self);
    auto* result = brush->m_pSolidColorBrush;
    brush->m_pSolidColorBrush = nullptr;
    brush->m_pBrush = nullptr;
    return result;
}
// Symbol: ?GetColor@CD2DSolidColorBrush@@QEBA?AU_D3DCOLORVALUE@@XZ
extern "C" void* MS_ABI impl__GetColor_CD2DSolidColorBrush__QEBA_AU_D3DCOLORVALUE__XZ(const void* self, void* result) {
    if (!self || !result) return result;
    auto* brush = static_cast<const CD2DSolidColorBrush*>(self);
    auto color = brush->m_pSolidColorBrush ? brush->m_pSolidColorBrush->GetColor() : brush->m_colorSolid;
    std::memcpy(result, &color, sizeof(color));
    return result;
}
// Symbol: ?SetColor@CD2DSolidColorBrush@@QEAAXU_D3DCOLORVALUE@@@Z
extern "C" void MS_ABI impl__SetColor_CD2DSolidColorBrush__QEAAXU_D3DCOLORVALUE___Z(void* self, const void* color) {
    if (!self || !color) return;
    auto* brush = static_cast<CD2DSolidColorBrush*>(self);
    brush->m_colorSolid = *static_cast<const D2D1_COLOR_F*>(color);
    if (brush->m_pSolidColorBrush) brush->m_pSolidColorBrush->SetColor(&brush->m_colorSolid);
}
