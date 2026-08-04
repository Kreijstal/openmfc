// CRenderTarget — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

// Symbol: ?GetRuntimeClass@CRenderTarget@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRenderTarget__UEBAPEAUCRuntimeClass__XZ(const CRenderTarget* pThis) {
    (void)pThis;
    return CRenderTarget::GetThisClass();
}
// Symbol: ?GetThisClass@CRenderTarget@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRenderTarget__SAPEAUCRuntimeClass__XZ() {
    return CRenderTarget::GetThisClass();
}
// Symbol: ??0CRenderTarget@@QEAA@XZ
extern "C" CRenderTarget* MS_ABI impl___0CRenderTarget__QEAA_XZ(CRenderTarget* pThis) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_renderTargetState[pThis] = {};
    return pThis;
}
// Symbol: ??1CRenderTarget@@UEAA@XZ
extern "C" void MS_ABI impl___1CRenderTarget__UEAA_XZ(CRenderTarget* pThis) {
    if (!pThis) return;
    ID2D1RenderTarget* resource = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto it = g_renderTargetState.find(pThis);
        if (it != g_renderTargetState.end()) {
            resource = static_cast<ID2D1RenderTarget*>(it->second.resource);
            g_renderTargetState.erase(it);
        }
    }
    if (resource) resource->Release();
}
// Symbol: ?Attach@CRenderTarget@@QEAAXPEAUID2D1RenderTarget@@@Z
extern "C" void MS_ABI impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(CRenderTarget* pThis, void* pRenderTarget) {
    if (!pThis) return;
    ID2D1RenderTarget* old = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto& state = g_renderTargetState[pThis];
        old = static_cast<ID2D1RenderTarget*>(state.resource);
        state.resource = pRenderTarget;
    }
    if (old && old != pRenderTarget) old->Release();
}
// Symbol: ?Detach@CRenderTarget@@QEAAPEAUID2D1RenderTarget@@XZ
extern "C" void* MS_ABI impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(CRenderTarget* pThis) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    void* old = state.resource;
    state.resource = nullptr;
    return old;
}
// Symbol: ?BeginDraw@CRenderTarget@@QEAAXXZ
extern "C" void MS_ABI impl__BeginDraw_CRenderTarget__QEAAXXZ(CRenderTarget* pThis) {
    if (!pThis) return;
    ID2D1RenderTarget* target = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto& state = g_renderTargetState[pThis];
        state.drawing = true;
        target = static_cast<ID2D1RenderTarget*>(state.resource);
    }
    if (target) target->BeginDraw();
}
// Symbol: ?EndDraw@CRenderTarget@@QEAAJXZ
extern "C" long MS_ABI impl__EndDraw_CRenderTarget__QEAAJXZ(CRenderTarget* pThis) {
    if (!pThis) return E_POINTER;
    ID2D1RenderTarget* target = nullptr;
    unsigned __int64 tag1 = 0;
    unsigned __int64 tag2 = 0;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto& state = g_renderTargetState[pThis];
        state.drawing = false;
        target = static_cast<ID2D1RenderTarget*>(state.resource);
    }
    HRESULT hr = target ? target->EndDraw(&tag1, &tag2) : E_POINTER;
    if (target) {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto& state = g_renderTargetState[pThis];
        state.tag1 = tag1;
        state.tag2 = tag2;
    }
    return hr;
}
// Symbol: ?Destroy@CRenderTarget@@QEAAHH@Z
extern "C" int MS_ABI impl__Destroy_CRenderTarget__QEAAHH_Z(CRenderTarget* pThis, int bReleasing) {
    if (!pThis) return FALSE;
    ID2D1RenderTarget* resource = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto& state = g_renderTargetState[pThis];
        resource = static_cast<ID2D1RenderTarget*>(state.resource);
        state.resource = nullptr;
        state.drawing = false;
        state.size = CD2DSizeF(0.0f, 0.0f);
        state.tag1 = 0;
        state.tag2 = 0;
    }
    if (resource && bReleasing) resource->Release();
    return resource != nullptr;
}
// Symbol: ?Clear@CRenderTarget@@QEAAXU_D3DCOLORVALUE@@@Z
extern "C" void MS_ABI impl__Clear_CRenderTarget__QEAAXU_D3DCOLORVALUE___Z(CRenderTarget* pThis, CD2DColorF color) {
    if (!pThis) return;
    ID2D1RenderTarget* target = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto& state = g_renderTargetState[pThis];
        state.lastClearColor = color;
        state.clearWasCalled = true;
        state.drawCallCount += 1;
        target = static_cast<ID2D1RenderTarget*>(state.resource);
    }
    if (target) {
        D2D1_COLOR_F nativeColor = {color.r, color.g, color.b, color.a};
        target->Clear(&nativeColor);
    }
}
// Symbol: ?DrawLine@CRenderTarget@@QEAAXAEBVCD2DPointF@@0PEAVCD2DBrush@@MPEAUID2D1StrokeStyle@@@Z
extern "C" void MS_ABI impl__DrawLine_CRenderTarget__QEAAXAEBVCD2DPointF__0PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
    CRenderTarget* pThis, const CD2DPointF* p0, const CD2DPointF* p1, void* pBrush, float strokeWidth, void* pStrokeStyle) {
    if (!pThis) return;
    if (!p0 || !p1) return;
    (void)pBrush;
    (void)strokeWidth;
    (void)pStrokeStyle;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.drawCallCount += 1;
    state.size.width = std::max(state.size.width, std::max(p0->x, p1->x));
    state.size.height = std::max(state.size.height, std::max(p0->y, p1->y));
}
// Symbol: ?DrawRectangle@CRenderTarget@@QEAAXAEBVCD2DRectF@@PEAVCD2DBrush@@MPEAUID2D1StrokeStyle@@@Z
extern "C" void MS_ABI impl__DrawRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
    CRenderTarget* pThis, const CD2DRectF* pRect, void* pBrush, float strokeWidth, void* pStrokeStyle) {
    if (!pThis || !pRect) return;
    (void)pBrush;
    (void)strokeWidth;
    (void)pStrokeStyle;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.drawCallCount += 1;
    state.size.width = std::max(state.size.width, std::max(pRect->left, pRect->right));
    state.size.height = std::max(state.size.height, std::max(pRect->top, pRect->bottom));
}
// Symbol: ?DrawEllipse@CRenderTarget@@QEAAXAEBVCD2DEllipse@@PEAVCD2DBrush@@MPEAUID2D1StrokeStyle@@@Z
extern "C" void MS_ABI impl__DrawEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
    CRenderTarget* pThis, const CD2DEllipse* pEllipse, void* pBrush, float strokeWidth, void* pStrokeStyle) {
    if (!pThis || !pEllipse) return;
    (void)pBrush;
    (void)strokeWidth;
    (void)pStrokeStyle;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.drawCallCount += 1;
    state.size.width = std::max(state.size.width, pEllipse->point.x + pEllipse->radius.width);
    state.size.height = std::max(state.size.height, pEllipse->point.y + pEllipse->radius.height);
}
// Symbol: ?DrawRoundedRectangle@CRenderTarget@@QEAAXAEBVCD2DRoundedRect@@PEAVCD2DBrush@@MPEAUID2D1StrokeStyle@@@Z
extern "C" void MS_ABI impl__DrawRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
    CRenderTarget* pThis, const CD2DRoundedRect* pRect, void* pBrush, float strokeWidth, void* pStrokeStyle) {
    if (!pThis || !pRect) return;
    (void)pBrush;
    (void)strokeWidth;
    (void)pStrokeStyle;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.drawCallCount += 1;
    state.size.width = std::max(state.size.width, std::max(pRect->rect.left, pRect->rect.right));
    state.size.height = std::max(state.size.height, std::max(pRect->rect.top, pRect->rect.bottom));
}
// Symbol: ?FillRectangle@CRenderTarget@@QEAAXAEBVCD2DRectF@@PEAVCD2DBrush@@@Z
extern "C" void MS_ABI impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(
    CRenderTarget* pThis, const CD2DRectF* pRect, void* pBrush) {
    if (!pThis || !pRect) return;
    (void)pBrush;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.drawCallCount += 1;
    state.size.width = std::max(state.size.width, std::max(pRect->left, pRect->right));
    state.size.height = std::max(state.size.height, std::max(pRect->top, pRect->bottom));
}
// Symbol: ?FillEllipse@CRenderTarget@@QEAAXAEBVCD2DEllipse@@PEAVCD2DBrush@@@Z
extern "C" void MS_ABI impl__FillEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush___Z(
    CRenderTarget* pThis, const CD2DEllipse* pEllipse, void* pBrush) {
    if (!pThis || !pEllipse) return;
    (void)pBrush;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.drawCallCount += 1;
    state.size.width = std::max(state.size.width, pEllipse->point.x + pEllipse->radius.width);
    state.size.height = std::max(state.size.height, pEllipse->point.y + pEllipse->radius.height);
}
// Symbol: ?FillRoundedRectangle@CRenderTarget@@QEAAXAEBVCD2DRoundedRect@@PEAVCD2DBrush@@@Z
extern "C" void MS_ABI impl__FillRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush___Z(
    CRenderTarget* pThis, const CD2DRoundedRect* pRect, void* pBrush) {
    if (!pThis || !pRect) return;
    (void)pBrush;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.drawCallCount += 1;
    state.size.width = std::max(state.size.width, std::max(pRect->rect.left, pRect->rect.right));
    state.size.height = std::max(state.size.height, std::max(pRect->rect.top, pRect->rect.bottom));
}
// Symbol: ?DrawTextW@CRenderTarget@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBVCD2DRectF@@PEAVCD2DBrush@@PEAVCD2DTextFormat@@W4D2D1_DRAW_TEXT_OPTIONS@@W4DWRITE_MEASURING_MODE@@@Z
extern "C" void MS_ABI impl__DrawTextW_CRenderTarget__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCD2DRectF__PEAVCD2DBrush__PEAVCD2DTextFormat__W4D2D1_DRAW_TEXT_OPTIONS__W4DWRITE_MEASURING_MODE___Z(
    CRenderTarget* pThis, const CString* text, const CD2DRectF* rect, void* brush, void* textFormat, int drawOptions, int measuringMode) {
    (void)brush;
    (void)textFormat;
    (void)drawOptions;
    (void)measuringMode;
    if (!pThis) return;

    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_renderTargetState[pThis];
    state.lastText = text ? *text : CString();
    state.lastTextRect = rect ? *rect : CD2DRectF();
    if (rect) {
        if (rect->right > state.size.width) state.size.width = rect->right;
        if (rect->bottom > state.size.height) state.size.height = rect->bottom;
    }
}
// Symbol: ?DrawTextW@CRenderTarget@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@VCD2DRectF@@PEAVCD2DBrush@@PEAVCD2DTextFormat@@W4D2D1_DRAW_TEXT_OPTIONS@@W4DWRITE_MEASURING_MODE@@@Z
extern "C" void MS_ABI impl__DrawTextW_CRenderTarget__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___VCD2DRectF__PEAVCD2DBrush__PEAVCD2DTextFormat__W4D2D1_DRAW_TEXT_OPTIONS__W4DWRITE_MEASURING_MODE___Z(
    CRenderTarget* pThis, const CString* text, CD2DRectF rect, void* brush, void* textFormat, int drawOptions, int measuringMode) {
    impl__DrawTextW_CRenderTarget__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCD2DRectF__PEAVCD2DBrush__PEAVCD2DTextFormat__W4D2D1_DRAW_TEXT_OPTIONS__W4DWRITE_MEASURING_MODE___Z(
        pThis, text, &rect, brush, textFormat, drawOptions, measuringMode);
}
// Symbol: ?COLORREF_TO_D2DCOLOR@CRenderTarget@@SA?AU_D3DCOLORVALUE@@KH@Z
extern "C" CD2DColorF MS_ABI impl__COLORREF_TO_D2DCOLOR_CRenderTarget__SA_AU_D3DCOLORVALUE__KH_Z(
    unsigned long color, int alpha) {
    CD2DColorF result = {};
    result.r = (float)GetRValue(color) / 255.0f;
    result.g = (float)GetGValue(color) / 255.0f;
    result.b = (float)GetBValue(color) / 255.0f;
    if (alpha < 0) alpha = 0;
    if (alpha > 255) alpha = 255;
    result.a = (float)alpha / 255.0f;
    return result;
}
// Symbol: ?GetDpi@CRenderTarget@@QEBA?AVCD2DSizeF@@XZ
extern "C" CD2DSizeF MS_ABI impl__GetDpi_CRenderTarget__QEBA_AVCD2DSizeF__XZ(const CRenderTarget* pThis) {
    if (!pThis) return CD2DSizeF();
    ID2D1RenderTarget* target = nullptr;
    CD2DSizeF dpi;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto& state = g_renderTargetState[pThis];
        target = static_cast<ID2D1RenderTarget*>(state.resource);
        dpi = state.dpi;
    }
    if (target) target->GetDpi(&dpi.width, &dpi.height);
    return dpi;
}
// Symbol: ?SetDpi@CRenderTarget@@QEAAXAEBVCD2DSizeF@@@Z
extern "C" void MS_ABI impl__SetDpi_CRenderTarget__QEAAXAEBVCD2DSizeF___Z(CRenderTarget* pThis, const CD2DSizeF* pDpi) {
    if (!pThis) return;
    CD2DSizeF dpi = pDpi ? *pDpi : CD2DSizeF(96.0f, 96.0f);
    ID2D1RenderTarget* target = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto& state = g_renderTargetState[pThis];
        state.dpi = dpi;
        target = static_cast<ID2D1RenderTarget*>(state.resource);
    }
    if (target) target->SetDpi(dpi.width, dpi.height);
}
// Symbol: ?GetSize@CRenderTarget@@QEBA?AVCD2DSizeF@@XZ
extern "C" CD2DSizeF MS_ABI impl__GetSize_CRenderTarget__QEBA_AVCD2DSizeF__XZ(const CRenderTarget* pThis) {
    if (!pThis) return CD2DSizeF();
    ID2D1RenderTarget* target = nullptr;
    CD2DSizeF size;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto& state = g_renderTargetState[pThis];
        target = static_cast<ID2D1RenderTarget*>(state.resource);
        size = state.size;
    }
    if (target) {
        D2D1_SIZE_F nativeSize = target->GetSize();
        size = CD2DSizeF(nativeSize.width, nativeSize.height);
    }
    return size;
}
// Symbol: ?GetTags@CRenderTarget@@QEBAXPEA_K0@Z
extern "C" void MS_ABI impl__GetTags_CRenderTarget__QEBAXPEA_K0_Z(
    const CRenderTarget* pThis, unsigned __int64* pTag1, unsigned __int64* pTag2) {
    if (!pThis) {
        if (pTag1) *pTag1 = 0;
        if (pTag2) *pTag2 = 0;
        return;
    }
    ID2D1RenderTarget* target = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        const auto& state = g_renderTargetState[pThis];
        target = static_cast<ID2D1RenderTarget*>(state.resource);
        if (pTag1) *pTag1 = state.tag1;
        if (pTag2) *pTag2 = state.tag2;
    }
    if (target) target->GetTags(pTag1, pTag2);
}
// Symbol: ?SetTags@CRenderTarget@@QEAAX_K0@Z
extern "C" void MS_ABI impl__SetTags_CRenderTarget__QEAAX_K0_Z(CRenderTarget* pThis, unsigned __int64 tag1, unsigned __int64 tag2) {
    if (!pThis) return;
    ID2D1RenderTarget* target = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto& state = g_renderTargetState[pThis];
        state.tag1 = tag1;
        state.tag2 = tag2;
        target = static_cast<ID2D1RenderTarget*>(state.resource);
    }
    if (target) target->SetTags(tag1, tag2);
}
CRenderTarget::CRenderTarget() {
    impl___0CRenderTarget__QEAA_XZ(this);
    memset(_rendertarget_padding, 0, sizeof(_rendertarget_padding));
}
CRenderTarget::~CRenderTarget() {
    impl___1CRenderTarget__UEAA_XZ(this);
}
void CRenderTarget::Attach(void* pRenderTarget) {
    impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(this, pRenderTarget);
}
void* CRenderTarget::Detach() {
    return impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(this);
}
void CRenderTarget::BeginDraw() {
    impl__BeginDraw_CRenderTarget__QEAAXXZ(this);
}
long CRenderTarget::EndDraw() {
    return impl__EndDraw_CRenderTarget__QEAAJXZ(this);
}
int CRenderTarget::Destroy(int bReleasing) {
    return impl__Destroy_CRenderTarget__QEAAHH_Z(this, bReleasing);
}
void CRenderTarget::Clear(CD2DColorF color) {
    impl__Clear_CRenderTarget__QEAAXU_D3DCOLORVALUE___Z(this, color);
}
void CRenderTarget::DrawLine(const CD2DPointF& p0, const CD2DPointF& p1) {
    impl__DrawLine_CRenderTarget__QEAAXAEBVCD2DPointF__0PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        this, &p0, &p1, nullptr, 1.0f, nullptr);
}
void CRenderTarget::DrawRectangle(const CD2DRectF& rect) {
    impl__DrawRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        this, &rect, nullptr, 1.0f, nullptr);
}
void CRenderTarget::DrawEllipse(const CD2DEllipse& ellipse) {
    impl__DrawEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        this, &ellipse, nullptr, 1.0f, nullptr);
}
void CRenderTarget::DrawRoundedRectangle(const CD2DRoundedRect& rect) {
    impl__DrawRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        this, &rect, nullptr, 1.0f, nullptr);
}
void CRenderTarget::FillRectangle(const CD2DRectF& rect) {
    impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(this, &rect, nullptr);
}
void CRenderTarget::FillEllipse(const CD2DEllipse& ellipse) {
    impl__FillEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush___Z(this, &ellipse, nullptr);
}
void CRenderTarget::FillRoundedRectangle(const CD2DRoundedRect& rect) {
    impl__FillRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush___Z(this, &rect, nullptr);
}
CD2DColorF CRenderTarget::COLORREF_TO_D2DCOLOR(COLORREF color, int alpha) {
    return impl__COLORREF_TO_D2DCOLOR_CRenderTarget__SA_AU_D3DCOLORVALUE__KH_Z(color, alpha);
}
CD2DSizeF CRenderTarget::GetDpi() const {
    return impl__GetDpi_CRenderTarget__QEBA_AVCD2DSizeF__XZ(this);
}
void CRenderTarget::SetDpi(const CD2DSizeF& dpi) {
    impl__SetDpi_CRenderTarget__QEAAXAEBVCD2DSizeF___Z(this, &dpi);
}
CD2DSizeF CRenderTarget::GetSize() const {
    return impl__GetSize_CRenderTarget__QEBA_AVCD2DSizeF__XZ(this);
}
void CRenderTarget::GetTags(unsigned __int64* pTag1, unsigned __int64* pTag2) const {
    impl__GetTags_CRenderTarget__QEBAXPEA_K0_Z(this, pTag1, pTag2);
}
void CRenderTarget::SetTags(unsigned __int64 tag1, unsigned __int64 tag2) {
    impl__SetTags_CRenderTarget__QEAAX_K0_Z(this, tag1, tag2);
}
