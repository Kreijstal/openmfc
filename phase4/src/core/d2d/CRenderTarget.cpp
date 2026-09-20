// CRenderTarget — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

// Thunks defined with their own class that the D2D forwards below need.
//   VerifyResource:                phase4/src/core/d2d/CD2DResource.cpp
//   Attach (CBitmapRenderTarget):  phase4/src/core/d2d/CBitmapRenderTarget.cpp
//   Attach/Detach (CD2DBrush):     phase4/src/core/d2d/CD2DBrush.cpp
//   Attach/Detach (CD2DBitmap):    phase4/src/core/d2d/CD2DBitmap.cpp
extern "C" int MS_ABI impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(void* self, void* pResource);
extern "C" void MS_ABI impl__Attach_CBitmapRenderTarget__QEAAXPEAUID2D1BitmapRenderTarget___Z(void* pThis, void* pBitmapTarget);
extern "C" void MS_ABI impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(void* pThis, void* pBrush);
extern "C" void* MS_ABI impl__Detach_CD2DBrush__QEAAPEAUID2D1Brush__XZ(void* pThis);
extern "C" void MS_ABI impl__Attach_CD2DBitmap__QEAAXPEAUID2D1Bitmap___Z(void* pThis, void* pBitmap);
extern "C" void* MS_ABI impl__Detach_CD2DBitmap__QEAAPEAUID2D1Bitmap__XZ(void* pThis);

namespace {

// Retail CRenderTarget layout (mfc140u ctor at RVA 0xd5110): vptr, then
// ID2D1RenderTarget* m_pRenderTarget at +0x8, CObList m_lstResources at +0x10 (its
// m_nBlockSize = 10 lands at +0x40), one more zeroed 8-byte member at +0x48, so
// sizeof == 0x50 and CBitmapRenderTarget's first own member sits at +0x50.  Every
// retail body below first tests m_pRenderTarget (a `mov 0x8(%rcx),%reg; test %reg,%reg`
// pair -- %rcx in most, %r10 in CreateCompatibleRenderTarget -- or `cmpq $0x0,0x8(%rcx)`)
// and bails when it is NULL.  This tree keeps that pointer
// in g_renderTargetState[pThis].resource (the same slot Attach/Detach/Destroy/
// BeginDraw/EndDraw below use), so the read goes through here.
ID2D1RenderTarget* RenderTargetOf(const CRenderTarget* pThis) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto it = g_renderTargetState.find(pThis);
    return it == g_renderTargetState.end() ? nullptr
                                            : static_cast<ID2D1RenderTarget*>(it->second.resource);
}

// Retail CRenderTarget::VerifyResource (mfc140u RVA 0xd5540, not exported by name in the
// RVA map; resolved through the export address table):
//   if (pResource == NULL) return FALSE;
//   if (pResource->IsValid()) return TRUE;               // vslot 8: CD2DBrush's is `m_pBrush != NULL`
//                                                        // (slot 8 of the CD2DBrush vftable -> mfc140u RVA 0xd7160)
//   return SUCCEEDED(pResource->Create(this));           // vslot 6 -- Create, not ReCreate: ReCreate (vslot 5,
//                                                        // mfc140u RVA 0xd28f0) is Destroy() then Create()
// The tree's thunk (CD2DResource.cpp) is side-table based, never dispatches to the
// wrapper's virtuals, and returns TRUE for a NULL pResource, so the NULL case is
// reproduced here rather than delegated.
bool VerifyResourceOf(CRenderTarget* pThis, void* pResource) {
    return pResource != nullptr &&
           impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(pThis, pResource) != 0;
}

// Retail reads the wrapped interface at +0x18 of every CD2DResource-derived wrapper
// (`mov 0x18(%reg),...` in DrawBitmap / FillOpacityMask / DrawGlyphRun / DrawRectangle).
// Retail CD2DResource (ctor at mfc140u RVA 0xd2800) is vptr, a BOOL at +0x8 (the
// bAutoDestroy argument), CRenderTarget* m_pParentTarget at +0x10, so the first derived
// member -- m_pBrush, m_pBitmap, ... -- lands at +0x18 (the CD2DBrush ctor at RVA 0xd2b70
// zeroes exactly that slot).  This tree stores those in the CD2DBrushState / CD2DBitmapState
// side tables (detail/ManualSmallStubImplementationsSupport.h) instead.  The only way
// to reach them from here through exported entry points is the wrapper's own
// Detach/Attach pair: Detach hands back the stored interface and clears the slot,
// Attach puts the same pointer back.  Nothing is created when the wrapper has no
// state (Detach returns nullptr without inserting, and Attach is then skipped), so
// the tables are left exactly as found.  A wrapper with no state, or with no
// interface yet, yields nullptr.
ID2D1Brush* BrushInterfaceOf(void* pBrush) {
    if (!pBrush) return nullptr;
    void* brush = impl__Detach_CD2DBrush__QEAAPEAUID2D1Brush__XZ(pBrush);
    if (brush) impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(pBrush, brush);
    return static_cast<ID2D1Brush*>(brush);
}
ID2D1Bitmap* BitmapInterfaceOf(void* pBitmap) {
    if (!pBitmap) return nullptr;
    void* bitmap = impl__Detach_CD2DBitmap__QEAAPEAUID2D1Bitmap__XZ(pBitmap);
    if (bitmap) impl__Attach_CD2DBitmap__QEAAXPEAUID2D1Bitmap___Z(pBitmap, bitmap);
    return static_cast<ID2D1Bitmap*>(bitmap);
}

} // namespace


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
// Symbol: ?CreateCompatibleRenderTarget@CRenderTarget@@QEAAHAEAVCBitmapRenderTarget@@VCD2DSizeF@@VCD2DSizeU@@PEAUD2D1_PIXEL_FORMAT@@W4D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS@@@Z
// Transcribed from mfc140u RVA 0xd5fc0:
//   BOOL CreateCompatibleRenderTarget(CBitmapRenderTarget& bitmapTarget, CD2DSizeF sizeDesired,
//                                     CD2DSizeU sizePixelDesired, D2D1_PIXEL_FORMAT* desiredFormat,
//                                     D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS options)
//   if (m_pRenderTarget == NULL) return FALSE;
//   const D2D1_SIZE_F* pSize  = (sizeDesired.width == 0 && sizeDesired.height == 0) ? NULL : &sizeDesired;
//         (ucomiss + jp: a NaN component counts as "not null", exactly as here)
//   const D2D1_SIZE_U* pPixel = (sizePixelDesired.width == 0 && sizePixelDesired.height == 0) ? NULL : &sizePixelDesired;
//   HRESULT hr = m_pRenderTarget->CreateCompatibleRenderTarget(pSize, pPixel, desiredFormat, options,
//                    &bitmapTarget.m_pBitmapRenderTarget /* +0x50, first CBitmapRenderTarget member */);   // slot 12
//   if (FAILED(hr)) return FALSE;
//   bitmapTarget.m_pRenderTarget /* +0x8 */ = bitmapTarget.m_pBitmapRenderTarget;   // plain store, no Release of the old value
//   return TRUE;
// Both CD2DSizeF and CD2DSizeU are 8-byte aggregates passed in R8/R9 by value (retail
// reads them with movq %r8,%xmm0 / shr $0x20,%r9), modelled here as 64-bit integers.
// Storage: this tree keeps bitmapTarget's ID2D1BitmapRenderTarget* in the
// CBitmapRenderTarget side table (its Attach thunk) and the CRenderTarget-level pointer
// in g_renderTargetState, so the two retail stores become those two writes.  Deviation:
// retail hands D2D the address of m_pBitmapRenderTarget directly, so a failed create
// leaves whatever D2D wrote there (normally NULL) in bitmapTarget; here the FAILED(hr)
// path returns before either store, so a previously attached ID2D1BitmapRenderTarget*
// in bitmapTarget's side table is left as it was rather than cleared.
extern "C" int MS_ABI impl__CreateCompatibleRenderTarget_CRenderTarget__QEAAHAEAVCBitmapRenderTarget__VCD2DSizeF__VCD2DSizeU__PEAUD2D1_PIXEL_FORMAT__W4D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS___Z(
    CRenderTarget* pThis, void* pBitmapTarget, unsigned long long sizeDesiredBits,
    unsigned long long sizePixelDesiredBits, ::D2D1_PIXEL_FORMAT* pDesiredFormat, int options) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return FALSE;
    if (!pBitmapTarget) return FALSE;   // retail dereferences the reference unconditionally

    D2D1_SIZE_F sizeDesired;
    D2D1_SIZE_U sizePixelDesired;
    static_assert(sizeof(sizeDesired) == sizeof(sizeDesiredBits), "CD2DSizeF is 8 bytes");
    static_assert(sizeof(sizePixelDesired) == sizeof(sizePixelDesiredBits), "CD2DSizeU is 8 bytes");
    std::memcpy(&sizeDesired, &sizeDesiredBits, sizeof(sizeDesired));
    std::memcpy(&sizePixelDesired, &sizePixelDesiredBits, sizeof(sizePixelDesired));

    const D2D1_SIZE_F* pSize =
        (sizeDesired.width == 0.0f && sizeDesired.height == 0.0f) ? nullptr : &sizeDesired;
    const D2D1_SIZE_U* pPixelSize =
        (sizePixelDesired.width == 0 && sizePixelDesired.height == 0) ? nullptr : &sizePixelDesired;

    ID2D1BitmapRenderTarget* created = nullptr;
    HRESULT hr = target->CreateCompatibleRenderTarget(
        pSize, pPixelSize, pDesiredFormat,
        static_cast<D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS>(options), &created);
    if (FAILED(hr)) return FALSE;

    // bitmapTarget.m_pBitmapRenderTarget = created  (retail +0x50)
    impl__Attach_CBitmapRenderTarget__QEAAXPEAUID2D1BitmapRenderTarget___Z(pBitmapTarget, created);
    // bitmapTarget.m_pRenderTarget = created        (retail +0x8, overwrite without Release)
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        g_renderTargetState[static_cast<CRenderTarget*>(pBitmapTarget)].resource = created;
    }
    return TRUE;
}

// Symbol: ?DrawBitmap@CRenderTarget@@QEAAXPEAVCD2DBitmap@@AEBVCD2DRectF@@MW4D2D1_BITMAP_INTERPOLATION_MODE@@PEBV3@@Z
// Transcribed from mfc140u RVA 0xd5a80:
//   if (m_pRenderTarget == NULL) return;
//   if (!VerifyResource(pBitmap)) return;                       // call 0xd5540
//   m_pRenderTarget->DrawBitmap(pBitmap->m_pBitmap /* +0x18 */, &rectDest, fOpacity,
//                               interpolationMode, pRectSrc);   // slot 26
// Deviation: retail passes m_pBitmap even when it is NULL; here a wrapper with no
// interface in its side table skips the call so the frame is not put into D2D's
// error state by a NULL bitmap.
extern "C" void MS_ABI impl__DrawBitmap_CRenderTarget__QEAAXPEAVCD2DBitmap__AEBVCD2DRectF__MW4D2D1_BITMAP_INTERPOLATION_MODE__PEBV3__Z(
    CRenderTarget* pThis, void* pBitmap, const CD2DRectF* pRectDest, float fOpacity,
    int interpolationMode, const CD2DRectF* pRectSrc) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    if (!VerifyResourceOf(pThis, pBitmap)) return;
    ID2D1Bitmap* bitmap = BitmapInterfaceOf(pBitmap);
    if (!bitmap) return;
    target->DrawBitmap(bitmap,
                       reinterpret_cast<const D2D1_RECT_F*>(pRectDest),
                       fOpacity,
                       static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(interpolationMode),
                       reinterpret_cast<const D2D1_RECT_F*>(pRectSrc));
}

// Symbol: ?DrawGeometry@CRenderTarget@@QEAAXPEAVCD2DGeometry@@PEAVCD2DBrush@@MPEAUID2D1StrokeStyle@@@Z
// STUB.  Retail (mfc140u RVA 0xd5ca0) is:
//   if (m_pRenderTarget == NULL) return;
//   if (!VerifyResource(pBrush)) return;                       // only the brush is verified
//   m_pRenderTarget->DrawGeometry(pGeometry->m_pGeometry /* +0x18 */, pBrush->m_pBrush /* +0x18 */,
//                                 fStrokeWidth, pStrokeStyle);  // slot 22
// Not implemented: this tree's CD2DGeometry (core/d2d/CD2DGeometry.cpp) has no storage
// for its ID2D1Geometry* at all (Attach/Detach are empty stubs), so there is nothing
// to forward.  Signature corrected from the mangled name; body intentionally empty.
extern "C" void MS_ABI impl__DrawGeometry_CRenderTarget__QEAAXPEAVCD2DGeometry__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
    CRenderTarget* pThis, void* pGeometry, void* pBrush, float fStrokeWidth, void* pStrokeStyle) {
    (void)pThis; (void)pGeometry; (void)pBrush; (void)fStrokeWidth; (void)pStrokeStyle;
}

// Symbol: ?DrawGlyphRun@CRenderTarget@@QEAAXAEBVCD2DPointF@@AEBUDWRITE_GLYPH_RUN@@PEAVCD2DBrush@@W4DWRITE_MEASURING_MODE@@@Z
// Transcribed from mfc140u RVA 0xd5d90:
//   if (m_pRenderTarget == NULL) return;
//   if (!VerifyResource(pForegroundBrush)) return;             // call 0xd5540
//   m_pRenderTarget->DrawGlyphRun(*(D2D1_POINT_2F*)&ptBaseLineOrigin /* 8 bytes by value */,
//                                 &glyphRun, pForegroundBrush->m_pBrush /* +0x18 */,
//                                 measuringMode);              // slot 29
// Deviation: a brush wrapper with no interface in its side table skips the call
// (retail would pass NULL through to D2D).
extern "C" void MS_ABI impl__DrawGlyphRun_CRenderTarget__QEAAXAEBVCD2DPointF__AEBUDWRITE_GLYPH_RUN__PEAVCD2DBrush__W4DWRITE_MEASURING_MODE___Z(
    CRenderTarget* pThis, const ::CD2DPointF* pBaselineOrigin, const DWRITE_GLYPH_RUN* pGlyphRun,
    void* pForegroundBrush, int measuringMode) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    if (!VerifyResourceOf(pThis, pForegroundBrush)) return;
    ID2D1Brush* brush = BrushInterfaceOf(pForegroundBrush);
    if (!brush) return;
    if (!pBaselineOrigin) return;   // retail dereferences the reference unconditionally
    D2D1_POINT_2F origin;
    origin.x = pBaselineOrigin->x;
    origin.y = pBaselineOrigin->y;
    target->DrawGlyphRun(origin, pGlyphRun, brush, static_cast<DWRITE_MEASURING_MODE>(measuringMode));
}

// Symbol: ?DrawTextLayout@CRenderTarget@@QEAAXAEBVCD2DPointF@@PEAVCD2DTextLayout@@PEAVCD2DBrush@@W4D2D1_DRAW_TEXT_OPTIONS@@@Z
// STUB.  Retail (mfc140u RVA 0xd5a10) is:
//   if (m_pRenderTarget == NULL) return;
//   if (!VerifyResource(pTextLayout)) return;
//   if (!VerifyResource(pDefaultFillBrush)) return;
//   m_pRenderTarget->DrawTextLayout(*(D2D1_POINT_2F*)&ptOrigin, pTextLayout->m_pTextLayout /* +0x18 */,
//                                   pDefaultFillBrush->m_pBrush /* +0x18 */, options);   // slot 28
// Not implemented: this tree's CD2DTextLayout (core/d2d/CD2DTextLayout.cpp) keeps only
// font-family/locale strings (D2DTextState) and no IDWriteTextLayout*, so there is
// nothing to forward.  Signature corrected from the mangled name; body intentionally empty.
extern "C" void MS_ABI impl__DrawTextLayout_CRenderTarget__QEAAXAEBVCD2DPointF__PEAVCD2DTextLayout__PEAVCD2DBrush__W4D2D1_DRAW_TEXT_OPTIONS___Z(
    CRenderTarget* pThis, const ::CD2DPointF* pOrigin, void* pTextLayout, void* pDefaultFillBrush, int options) {
    (void)pThis; (void)pOrigin; (void)pTextLayout; (void)pDefaultFillBrush; (void)options;
}

// Symbol: ?FillGeometry@CRenderTarget@@QEAAXPEAVCD2DGeometry@@PEAVCD2DBrush@@1@Z
// STUB.  Retail (mfc140u RVA 0xd5d20) is:
//   if (m_pRenderTarget == NULL) return;
//   if (!VerifyResource(pBrush)) return;
//   if (pOpacityBrush != NULL) VerifyResource(pOpacityBrush);   // result NOT tested
//   m_pRenderTarget->FillGeometry(pGeometry->m_pGeometry /* +0x18 */, pBrush->m_pBrush /* +0x18 */,
//                                 pOpacityBrush ? pOpacityBrush->m_pBrush : NULL);   // slot 23
// Not implemented: same reason as DrawGeometry -- CD2DGeometry has no ID2D1Geometry*
// storage in this tree.  Signature corrected from the mangled name; body intentionally empty.
extern "C" void MS_ABI impl__FillGeometry_CRenderTarget__QEAAXPEAVCD2DGeometry__PEAVCD2DBrush__1_Z(
    CRenderTarget* pThis, void* pGeometry, void* pBrush, void* pOpacityBrush) {
    (void)pThis; (void)pGeometry; (void)pBrush; (void)pOpacityBrush;
}

// Symbol: ?FillMesh@CRenderTarget@@QEAAXPEAVCD2DMesh@@PEAVCD2DBrush@@@Z
// STUB.  Retail (mfc140u RVA 0xd5df0) is:
//   if (m_pRenderTarget == NULL) return;
//   if (!VerifyResource(pMesh)) return;
//   if (!VerifyResource(pBrush)) return;
//   m_pRenderTarget->FillMesh(pMesh->m_pMesh /* +0x18 */, pBrush->m_pBrush /* +0x18 */);   // slot 24
// Not implemented: this tree's CD2DMesh (core/d2d/CD2DMesh.cpp) has no ID2D1Mesh*
// storage (Attach/Detach/Create are empty stubs).  Signature corrected; body empty.
extern "C" void MS_ABI impl__FillMesh_CRenderTarget__QEAAXPEAVCD2DMesh__PEAVCD2DBrush___Z(
    CRenderTarget* pThis, void* pMesh, void* pBrush) {
    (void)pThis; (void)pMesh; (void)pBrush;
}

// Symbol: ?FillOpacityMask@CRenderTarget@@QEAAXPEAVCD2DBitmap@@PEAVCD2DBrush@@W4D2D1_OPACITY_MASK_CONTENT@@AEBVCD2DRectF@@3@Z
// Transcribed from mfc140u RVA 0xd5770:
//   if (m_pRenderTarget == NULL) return;
//   if (!VerifyResource(pBrush)) return;                       // brush first
//   if (!VerifyResource(pOpacityMask)) return;                 // then the mask bitmap
//   m_pRenderTarget->FillOpacityMask(pOpacityMask->m_pBitmap /* +0x18 */, pBrush->m_pBrush /* +0x18 */,
//                                    content, &rectDest, &rectSrc);   // slot 25; both rect
//                                    pointers are passed unconditionally (no IsNull test)
// Deviation: a wrapper with no interface in its side table skips the call.
extern "C" void MS_ABI impl__FillOpacityMask_CRenderTarget__QEAAXPEAVCD2DBitmap__PEAVCD2DBrush__W4D2D1_OPACITY_MASK_CONTENT__AEBVCD2DRectF__3_Z(
    CRenderTarget* pThis, void* pOpacityMask, void* pBrush, int content,
    const CD2DRectF* pRectDest, const CD2DRectF* pRectSrc) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    if (!VerifyResourceOf(pThis, pBrush)) return;
    if (!VerifyResourceOf(pThis, pOpacityMask)) return;
    ID2D1Bitmap* mask = BitmapInterfaceOf(pOpacityMask);
    ID2D1Brush* brush = BrushInterfaceOf(pBrush);
    if (!mask || !brush) return;
    target->FillOpacityMask(mask, brush, static_cast<D2D1_OPACITY_MASK_CONTENT>(content),
                            reinterpret_cast<const D2D1_RECT_F*>(pRectDest),
                            reinterpret_cast<const D2D1_RECT_F*>(pRectSrc));
}

// Symbol: ?Flush@CRenderTarget@@QEAAXPEA_K0@Z
// Transcribed from mfc140u RVA 0xd60f0:
//   if (m_pRenderTarget) m_pRenderTarget->Flush(pTag1, pTag2);   // slot 42; HRESULT discarded
extern "C" void MS_ABI impl__Flush_CRenderTarget__QEAAXPEA_K0_Z(
    CRenderTarget* pThis, unsigned __int64* pTag1, unsigned __int64* pTag2) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->Flush(pTag1, pTag2);
}

// Symbol: ?GetAntialiasMode@CRenderTarget@@QEBA?AW4D2D1_ANTIALIAS_MODE@@XZ
// Transcribed from mfc140u RVA 0xd5e90:
//   if (m_pRenderTarget == NULL) return (D2D1_ANTIALIAS_MODE)-1;   // or $0xffffffff,%eax
//   return m_pRenderTarget->GetAntialiasMode();                   // slot 33 (tail-jump)
extern "C" D2D1_ANTIALIAS_MODE MS_ABI impl__GetAntialiasMode_CRenderTarget__QEBA_AW4D2D1_ANTIALIAS_MODE__XZ(
    const CRenderTarget* pThis) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return static_cast<D2D1_ANTIALIAS_MODE>(-1);
    return target->GetAntialiasMode();
}

// Symbol: ?GetMaximumBitmapSize@CRenderTarget@@QEBAIXZ
// Transcribed from mfc140u RVA 0xd5450:
//   if (m_pRenderTarget == NULL) return 0;
//   return m_pRenderTarget->GetMaximumBitmapSize();   // slot 55 (tail-jump)
extern "C" unsigned int MS_ABI impl__GetMaximumBitmapSize_CRenderTarget__QEBAIXZ(const CRenderTarget* pThis) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return 0;
    return target->GetMaximumBitmapSize();
}

// Symbol: ?GetPixelFormat@CRenderTarget@@QEBA?AUD2D1_PIXEL_FORMAT@@XZ
// Transcribed from mfc140u RVA 0xd5360.  Non-static member functions return every UDT
// through a hidden pointer under the MSVC x64 ABI, so RDX is the 8-byte result slot and
// the D2D vcall receives that same slot in RDX (mingw's d2d1.h exposes that form as the
// GetPixelFormat(D2D1_PIXEL_FORMAT*) overload used here):
//   if (m_pRenderTarget == NULL) { *(UINT64*)pResult = 0; }          // mov %rcx,(%rdx) with rcx == 0
//   else                         m_pRenderTarget->GetPixelFormat(pResult);   // slot 50
//   return pResult;
extern "C" ::D2D1_PIXEL_FORMAT* MS_ABI impl__GetPixelFormat_CRenderTarget__QEBA_AUD2D1_PIXEL_FORMAT__XZ(
    const CRenderTarget* pThis, ::D2D1_PIXEL_FORMAT* pResult) {
    if (!pResult) return nullptr;   // retail stores through RDX unconditionally
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) {
        pResult->format = DXGI_FORMAT_UNKNOWN;
        pResult->alphaMode = D2D1_ALPHA_MODE_UNKNOWN;
        return pResult;
    }
    target->GetPixelFormat(pResult);
    return pResult;
}

// Symbol: ?GetPixelSize@CRenderTarget@@QEBA?AVCD2DSizeU@@XZ
// Transcribed from mfc140u RVA 0xd5390 (CD2DSizeU is returned through the hidden
// pointer in RDX; its layout is two UINT32, identical to D2D1_SIZE_U):
//   if (m_pRenderTarget == NULL) { *(UINT64*)pResult = 0; }   // width = height = 0
//   else { D2D1_SIZE_U tmp; m_pRenderTarget->GetPixelSize(&tmp);   // slot 54, hidden ptr in RDX
//          pResult->width = tmp.width; pResult->height = tmp.height; }
//   return pResult;
extern "C" D2D1_SIZE_U* MS_ABI impl__GetPixelSize_CRenderTarget__QEBA_AVCD2DSizeU__XZ(
    const CRenderTarget* pThis, D2D1_SIZE_U* pResult) {
    if (!pResult) return nullptr;   // retail stores through RDX unconditionally
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) {
        pResult->width = 0;
        pResult->height = 0;
        return pResult;
    }
    D2D1_SIZE_U tmp;
    target->GetPixelSize(&tmp);
    pResult->width = tmp.width;
    pResult->height = tmp.height;
    return pResult;
}

// Symbol: ?GetTextAntialiasMode@CRenderTarget@@QEBA?AW4D2D1_TEXT_ANTIALIAS_MODE@@XZ
// Transcribed from mfc140u RVA 0xd5ee0:
//   if (m_pRenderTarget == NULL) return (D2D1_TEXT_ANTIALIAS_MODE)-1;
//   return m_pRenderTarget->GetTextAntialiasMode();   // slot 35 (tail-jump)
extern "C" D2D1_TEXT_ANTIALIAS_MODE MS_ABI impl__GetTextAntialiasMode_CRenderTarget__QEBA_AW4D2D1_TEXT_ANTIALIAS_MODE__XZ(
    const CRenderTarget* pThis) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return static_cast<D2D1_TEXT_ANTIALIAS_MODE>(-1);
    return target->GetTextAntialiasMode();
}

// Symbol: ?GetTextRenderingParams@CRenderTarget@@QEAAXPEAPEAUIDWriteRenderingParams@@@Z
// Transcribed from mfc140u RVA 0xd5f30:
//   if (m_pRenderTarget) m_pRenderTarget->GetTextRenderingParams(ppParams);   // slot 37
// (*ppParams is left untouched when there is no render target.)
extern "C" void MS_ABI impl__GetTextRenderingParams_CRenderTarget__QEAAXPEAPEAUIDWriteRenderingParams___Z(
    CRenderTarget* pThis, IDWriteRenderingParams** ppParams) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->GetTextRenderingParams(ppParams);
}

// Symbol: ?GetTransform@CRenderTarget@@QEAAXPEAUD2D_MATRIX_3X2_F@@@Z
// Transcribed from mfc140u RVA 0xd5f90:
//   if (m_pRenderTarget) m_pRenderTarget->GetTransform(pTransform);   // slot 31
extern "C" void MS_ABI impl__GetTransform_CRenderTarget__QEAAXPEAUD2D_MATRIX_3X2_F___Z(
    CRenderTarget* pThis, D2D1_MATRIX_3X2_F* pTransform) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->GetTransform(pTransform);
}

// Symbol: ?IsSupported@CRenderTarget@@QEBAHAEBUD2D1_RENDER_TARGET_PROPERTIES@@@Z
// Transcribed from mfc140u RVA 0xd5430:
//   if (m_pRenderTarget == NULL) return FALSE;
//   return m_pRenderTarget->IsSupported(&renderTargetProperties);   // slot 56 (tail-jump)
extern "C" int MS_ABI impl__IsSupported_CRenderTarget__QEBAHAEBUD2D1_RENDER_TARGET_PROPERTIES___Z(
    const CRenderTarget* pThis, const D2D1_RENDER_TARGET_PROPERTIES* pProperties) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return FALSE;
    return target->IsSupported(pProperties);
}

// The eleven bodies below are not in mfc140u_rva_symbols.json (that map stops at
// ordinal ~9566); their RVAs were read straight from mfc140u.dll's export address
// table (ordinals 11747, 11748, 11935, 11937, 12550, 12634, 12920, 13677, 13684,
// 13714, 13715) and each body was disassembled at that address.

// Symbol: ?PopAxisAlignedClip@CRenderTarget@@QEAAXXZ
// Transcribed from mfc140u RVA 0xd61b0:
//   if (m_pRenderTarget) m_pRenderTarget->PopAxisAlignedClip();   // slot 46
extern "C" void MS_ABI impl__PopAxisAlignedClip_CRenderTarget__QEAAXXZ(CRenderTarget* pThis) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->PopAxisAlignedClip();
}

// Symbol: ?PopLayer@CRenderTarget@@QEAAXXZ
// Transcribed from mfc140u RVA 0xd60c0:
//   if (m_pRenderTarget) m_pRenderTarget->PopLayer();   // slot 41
extern "C" void MS_ABI impl__PopLayer_CRenderTarget__QEAAXXZ(CRenderTarget* pThis) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->PopLayer();
}

// Symbol: ?PushAxisAlignedClip@CRenderTarget@@QEAAXAEBVCD2DRectF@@W4D2D1_ANTIALIAS_MODE@@@Z
// Transcribed from mfc140u RVA 0xd6180:
//   if (m_pRenderTarget) m_pRenderTarget->PushAxisAlignedClip(&rectClip, mode);   // slot 45;
//   RDX (the CD2DRectF&) and R8D are forwarded untouched -- CD2DRectF is layout-identical
//   to D2D1_RECT_F.
extern "C" void MS_ABI impl__PushAxisAlignedClip_CRenderTarget__QEAAXAEBVCD2DRectF__W4D2D1_ANTIALIAS_MODE___Z(
    CRenderTarget* pThis, const CD2DRectF* pRectClip, int mode) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->PushAxisAlignedClip(reinterpret_cast<const D2D1_RECT_F*>(pRectClip),
                                static_cast<D2D1_ANTIALIAS_MODE>(mode));
}

// Symbol: ?PushLayer@CRenderTarget@@QEAAXAEBUD2D1_LAYER_PARAMETERS@@AEAVCD2DLayer@@@Z
// STUB.  Retail (mfc140u RVA 0xd6090) is:
//   if (m_pRenderTarget) m_pRenderTarget->PushLayer(&layerParameters, layer.m_pLayer /* +0x18 */);   // slot 40
//   (no VerifyResource on the layer)
// Not implemented: this tree's CD2DLayer (core/d2d/CD2DLayer.cpp) has no ID2D1Layer*
// storage (Attach/Detach/Create are empty stubs), so the layer argument cannot be
// produced.  Signature corrected from the mangled name; body intentionally empty.
extern "C" void MS_ABI impl__PushLayer_CRenderTarget__QEAAXAEBUD2D1_LAYER_PARAMETERS__AEAVCD2DLayer___Z(
    CRenderTarget* pThis, const D2D1_LAYER_PARAMETERS* pLayerParameters, void* pLayer) {
    (void)pThis; (void)pLayerParameters; (void)pLayer;
}

// Symbol: ?RestoreDrawingState@CRenderTarget@@QEAAXAEAUID2D1DrawingStateBlock@@@Z
// Transcribed from mfc140u RVA 0xd6150:
//   if (m_pRenderTarget) m_pRenderTarget->RestoreDrawingState(&drawingStateBlock);   // slot 44
extern "C" void MS_ABI impl__RestoreDrawingState_CRenderTarget__QEAAXAEAUID2D1DrawingStateBlock___Z(
    CRenderTarget* pThis, ID2D1DrawingStateBlock* pDrawingStateBlock) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->RestoreDrawingState(pDrawingStateBlock);
}

// Symbol: ?SaveDrawingState@CRenderTarget@@QEBAXAEAUID2D1DrawingStateBlock@@@Z
// Transcribed from mfc140u RVA 0xd6120:
//   if (m_pRenderTarget) m_pRenderTarget->SaveDrawingState(&drawingStateBlock);   // slot 43
extern "C" void MS_ABI impl__SaveDrawingState_CRenderTarget__QEBAXAEAUID2D1DrawingStateBlock___Z(
    const CRenderTarget* pThis, ID2D1DrawingStateBlock* pDrawingStateBlock) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->SaveDrawingState(pDrawingStateBlock);
}

// Symbol: ?SetAntialiasMode@CRenderTarget@@QEAAXW4D2D1_ANTIALIAS_MODE@@@Z
// Transcribed from mfc140u RVA 0xd5e60:
//   if (m_pRenderTarget) m_pRenderTarget->SetAntialiasMode(mode);   // slot 32
extern "C" void MS_ABI impl__SetAntialiasMode_CRenderTarget__QEAAXW4D2D1_ANTIALIAS_MODE___Z(
    CRenderTarget* pThis, int mode) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->SetAntialiasMode(static_cast<D2D1_ANTIALIAS_MODE>(mode));
}

// Symbol: ?SetTextAntialiasMode@CRenderTarget@@QEAAXW4D2D1_TEXT_ANTIALIAS_MODE@@@Z
// Transcribed from mfc140u RVA 0xd5eb0:
//   if (m_pRenderTarget) m_pRenderTarget->SetTextAntialiasMode(mode);   // slot 34
extern "C" void MS_ABI impl__SetTextAntialiasMode_CRenderTarget__QEAAXW4D2D1_TEXT_ANTIALIAS_MODE___Z(
    CRenderTarget* pThis, int mode) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->SetTextAntialiasMode(static_cast<D2D1_TEXT_ANTIALIAS_MODE>(mode));
}

// Symbol: ?SetTextRenderingParams@CRenderTarget@@QEAAXPEAUIDWriteRenderingParams@@@Z
// Transcribed from mfc140u RVA 0xd5f00:
//   if (m_pRenderTarget) m_pRenderTarget->SetTextRenderingParams(pParams);   // slot 36
extern "C" void MS_ABI impl__SetTextRenderingParams_CRenderTarget__QEAAXPEAUIDWriteRenderingParams___Z(
    CRenderTarget* pThis, IDWriteRenderingParams* pParams) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->SetTextRenderingParams(pParams);
}

// Symbol: ?SetTransform@CRenderTarget@@QEAAXAEBUD2D_MATRIX_3X2_F@@@Z
// Both SetTransform overloads are folded onto one retail body (export ordinals 13714 and
// 13715 both point at mfc140u RVA 0xd5f60 -- a reference and a pointer are the same
// register):
//   if (m_pRenderTarget) m_pRenderTarget->SetTransform(pTransform);   // slot 30
extern "C" void MS_ABI impl__SetTransform_CRenderTarget__QEAAXAEBUD2D_MATRIX_3X2_F___Z(
    CRenderTarget* pThis, const D2D1_MATRIX_3X2_F* pTransform) {
    ID2D1RenderTarget* target = RenderTargetOf(pThis);
    if (!target) return;
    target->SetTransform(pTransform);
}

// Symbol: ?SetTransform@CRenderTarget@@QEAAXPEBUD2D_MATRIX_3X2_F@@@Z
// Same retail body as the reference overload above (mfc140u RVA 0xd5f60).
extern "C" void MS_ABI impl__SetTransform_CRenderTarget__QEAAXPEBUD2D_MATRIX_3X2_F___Z(
    CRenderTarget* pThis, const D2D1_MATRIX_3X2_F* pTransform) {
    impl__SetTransform_CRenderTarget__QEAAXAEBUD2D_MATRIX_3X2_F___Z(pThis, pTransform);
}
