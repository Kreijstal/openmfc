// Standalone MSVC ABI test of the production drawing thunks. Only sibling
// wrapper access/verification is doubled; render-target state and drawing code
// come from the real source. A software WIC target checks actual pixel output.
#define impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z test_VerifyResource
#include "../phase4/src/core/d2d/CRenderTarget.cpp"
#undef impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z
#include "../phase4/src/core/d2d/CD2DTextFormat.cpp"
#include "../phase4/src/core/d2d/CD2DTextLayout.cpp"
#include "openmfc/afxinet.h"
#include <wincodec.h>
#include <cstdio>
#include <cstdlib>

// Unused GDI siblings in the shared support TU still need link definitions.
// Abort if a drawing path unexpectedly reaches one of these test doubles.
CImageList::CImageList() { std::abort(); }
CImageList::~CImageList() { std::abort(); }
CDC::CDC() { std::abort(); }
CDC::~CDC() { std::abort(); }
BOOL CGdiObject::DeleteObject() { std::abort(); }
HINSTANCE AFXAPI AfxGetInstanceHandle() { std::abort(); }
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CFileFind::classCFileFind{};
CRuntimeClass CDocument::classCDocument{};
CRuntimeClass CHtmlView::classCHtmlView{};
extern "C" void MS_ABI impl__Attach_CBitmapRenderTarget__QEAAXPEAUID2D1BitmapRenderTarget___Z(void*, void*) { std::abort(); }
extern "C" void MS_ABI impl__Attach_CD2DBitmap__QEAAXPEAUID2D1Bitmap___Z(void*, void*) { std::abort(); }
extern "C" void* MS_ABI impl__Detach_CD2DBitmap__QEAAPEAUID2D1Bitmap__XZ(void*) { std::abort(); }

struct TestResource {
    void* resource;
    bool valid;
};
static void* verified[8];
static unsigned verificationCount;
extern "C" int MS_ABI test_VerifyResource(void*, void* wrapper) {
    if (verificationCount < 8) verified[verificationCount++] = wrapper;
    return wrapper && static_cast<TestResource*>(wrapper)->valid;
}
extern "C" void* MS_ABI impl__Detach_CD2DBrush__QEAAPEAUID2D1Brush__XZ(void* wrapper) {
    auto* brush = static_cast<TestResource*>(wrapper);
    void* result = brush->resource;
    brush->resource = nullptr;
    return result;
}
extern "C" void MS_ABI impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(void* wrapper, void* resource) {
    static_cast<TestResource*>(wrapper)->resource = resource;
}

// Inline resources have a base resource prefix and the real pointer at +0x18.
// First byte also lets the verification double mark these wrappers valid.
struct InlineWrapper {
    void* vfptr;
    bool valid;
    void* parent;
    void* resource;
};
static_assert(offsetof(InlineWrapper, resource) == 0x18, "retail resource layout");

static int failures;
static unsigned calls;
static unsigned lastSlot;
static void* lastTarget;
static void* lastResource;
static void* lastBrush;
static void* lastStyle;
static void* lastOpacityBrush;
static float lastWidth;
static D2D1_POINT_2F lastStart, lastEnd;
static D2D1_RECT_F lastRect;
static D2D1_ELLIPSE lastEllipse;
static D2D1_ROUNDED_RECT lastRoundedRect;
static const D2D1_LAYER_PARAMETERS* lastLayerParameters;
static std::wstring lastText;
static void* lastTextFormat;
static D2D1_DRAW_TEXT_OPTIONS lastTextOptions;
static DWRITE_MEASURING_MODE lastMeasuringMode;
static void check(bool ok, const char* message) {
    std::printf("%s: %s\n", ok ? "PASS" : "FAIL", message);
    if (!ok) ++failures;
}
static void record(void* target, unsigned slot) {
    ++calls;
    lastTarget = target;
    lastSlot = slot;
    std::unique_lock<std::mutex> lock(g_wave2StateMutex, std::try_to_lock);
    check(lock.owns_lock(), "COM callback runs outside the render-target mutex");
}
static void STDMETHODCALLTYPE line(void* target, D2D1_POINT_2F start, D2D1_POINT_2F end,
                                    ID2D1Brush* brush, float width, ID2D1StrokeStyle* style) {
    record(target, 15); lastStart = start; lastEnd = end;
    lastBrush = brush; lastWidth = width; lastStyle = style;
}
static void STDMETHODCALLTYPE rectangle(void* target, const D2D1_RECT_F* rect,
                                         ID2D1Brush* brush, float width, ID2D1StrokeStyle* style) {
    record(target, 16); lastRect = *rect;
    lastBrush = brush; lastWidth = width; lastStyle = style;
}
static void STDMETHODCALLTYPE fillRectangle(void* target, const D2D1_RECT_F* rect, ID2D1Brush* brush) {
    record(target, 17); lastRect = *rect; lastBrush = brush;
}
static void STDMETHODCALLTYPE rounded(void* target, const D2D1_ROUNDED_RECT* rect,
                                     ID2D1Brush* brush, float width, ID2D1StrokeStyle* style) {
    record(target, 18); lastRoundedRect = *rect;
    lastBrush = brush; lastWidth = width; lastStyle = style;
}
static void STDMETHODCALLTYPE fillRounded(void* target, const D2D1_ROUNDED_RECT* rect, ID2D1Brush* brush) {
    record(target, 19); lastRoundedRect = *rect; lastBrush = brush;
}
static void STDMETHODCALLTYPE ellipse(void* target, const D2D1_ELLIPSE* shape,
                                     ID2D1Brush* brush, float width, ID2D1StrokeStyle* style) {
    record(target, 20); lastEllipse = *shape;
    lastBrush = brush; lastWidth = width; lastStyle = style;
}
static void STDMETHODCALLTYPE fillEllipse(void* target, const D2D1_ELLIPSE* shape, ID2D1Brush* brush) {
    record(target, 21); lastEllipse = *shape; lastBrush = brush;
}
static void STDMETHODCALLTYPE geometry(void* target, ID2D1Geometry* shape,
                                       ID2D1Brush* brush, float width, ID2D1StrokeStyle* style) {
    record(target, 22); lastResource = shape;
    lastBrush = brush; lastWidth = width; lastStyle = style;
}
static void STDMETHODCALLTYPE fillGeometry(void* target, ID2D1Geometry* shape,
                                           ID2D1Brush* brush, ID2D1Brush* opacity) {
    record(target, 23); lastResource = shape; lastBrush = brush; lastOpacityBrush = opacity;
}
static void STDMETHODCALLTYPE fillMesh(void* target, ID2D1Mesh* mesh, ID2D1Brush* brush) {
    record(target, 24); lastResource = mesh; lastBrush = brush;
}
static void STDMETHODCALLTYPE pushLayer(void* target, const D2D1_LAYER_PARAMETERS* parameters, ID2D1Layer* layer) {
    record(target, 40); lastLayerParameters = parameters; lastResource = layer;
}
static void STDMETHODCALLTYPE drawText(void* target, const wchar_t* text, UINT32 length,
    IDWriteTextFormat* format, const D2D1_RECT_F* rect, ID2D1Brush* brush,
    D2D1_DRAW_TEXT_OPTIONS options, DWRITE_MEASURING_MODE mode) {
    record(target, 27); lastText.assign(text, length); lastTextFormat = format;
    lastRect = *rect; lastBrush = brush; lastTextOptions = options; lastMeasuringMode = mode;
}
static void STDMETHODCALLTYPE drawLayout(void* target, D2D1_POINT_2F origin,
    IDWriteTextLayout* layout, ID2D1Brush* brush, D2D1_DRAW_TEXT_OPTIONS options) {
    record(target, 28); lastStart = origin; lastResource = layout;
    lastBrush = brush; lastTextOptions = options;
}
static D2D1_PIXEL_FORMAT* STDMETHODCALLTYPE pixelFormat(void* target, D2D1_PIXEL_FORMAT* result) {
    record(target, 50); *result = {DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED}; return result;
}
static D2D1_SIZE_U* STDMETHODCALLTYPE pixelSize(void* target, D2D1_SIZE_U* result) {
    record(target, 54); *result = {123, 456}; return result;
}

static void forwarding() {
    void* slots[57] = {};
    slots[15] = reinterpret_cast<void*>(line);
    slots[16] = reinterpret_cast<void*>(rectangle);
    slots[17] = reinterpret_cast<void*>(fillRectangle);
    slots[18] = reinterpret_cast<void*>(rounded);
    slots[19] = reinterpret_cast<void*>(fillRounded);
    slots[20] = reinterpret_cast<void*>(ellipse);
    slots[21] = reinterpret_cast<void*>(fillEllipse);
    slots[22] = reinterpret_cast<void*>(geometry);
    slots[23] = reinterpret_cast<void*>(fillGeometry);
    slots[24] = reinterpret_cast<void*>(fillMesh);
    slots[27] = reinterpret_cast<void*>(drawText);
    slots[28] = reinterpret_cast<void*>(drawLayout);
    slots[40] = reinterpret_cast<void*>(pushLayer);
    slots[50] = reinterpret_cast<void*>(pixelFormat);
    slots[54] = reinterpret_cast<void*>(pixelSize);
    void** target = slots;
    CRenderTarget render;
    render.Attach(&target);
    int nativeBrush, nativeStyle, nativeGeometry, nativeMesh, nativeLayer, nativeOpacity;
    TestResource brush{&nativeBrush, true}, opacity{&nativeOpacity, true};
    InlineWrapper geometryWrapper{nullptr, true, nullptr, &nativeGeometry};
    InlineWrapper meshWrapper{nullptr, true, nullptr, &nativeMesh};
    InlineWrapper layerWrapper{nullptr, true, nullptr, &nativeLayer};
    CD2DPointF start(1.25f, 2.5f), end(7.75f, 8.5f);
    CD2DRectF rect(1, 2, 11, 12);
    CD2DEllipse oval(start, CD2DSizeF(3, 4));
    CD2DRoundedRect roundedRect(rect, CD2DSizeF(5, 6));

    impl__DrawLine_CRenderTarget__QEAAXAEBVCD2DPointF__0PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        &render, &start, &end, &brush, 2.75f, &nativeStyle);
    check(lastSlot == 15 && lastTarget == &target && lastStart.x == start.x && lastStart.y == start.y &&
          lastEnd.x == end.x && lastEnd.y == end.y && lastWidth == 2.75f && lastStyle == &nativeStyle &&
          lastBrush == &nativeBrush && brush.resource == &nativeBrush, "DrawLine forwards MSVC point aggregates, brush, width and style");
    impl__DrawRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        &render, &rect, &brush, 3.25f, &nativeStyle);
    check(lastSlot == 16 && lastRect.left == 1 && lastRect.top == 2 && lastRect.right == 11 &&
          lastRect.bottom == 12 && lastWidth == 3.25f && lastStyle == &nativeStyle && lastBrush == &nativeBrush,
          "DrawRectangle forwards geometry and stroke arguments");
    impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(&render, &rect, &brush);
    check(lastSlot == 17 && lastRect.right == 11 && lastBrush == &nativeBrush, "FillRectangle dispatches to Direct2D");
    impl__DrawRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        &render, &roundedRect, &brush, 4.25f, &nativeStyle);
    check(lastSlot == 18 && lastRoundedRect.rect.bottom == 12 && lastRoundedRect.radiusX == 5 &&
          lastRoundedRect.radiusY == 6 && lastWidth == 4.25f && lastStyle == &nativeStyle,
          "DrawRoundedRectangle converts both radii and forwards stroke arguments");
    impl__FillRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush___Z(&render, &roundedRect, &brush);
    check(lastSlot == 19 && lastRoundedRect.radiusY == 6 && lastBrush == &nativeBrush, "FillRoundedRectangle dispatches to Direct2D");
    impl__DrawEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        &render, &oval, &brush, 5.25f, &nativeStyle);
    check(lastSlot == 20 && lastEllipse.point.x == 1.25f && lastEllipse.point.y == 2.5f &&
          lastEllipse.radiusX == 3 && lastEllipse.radiusY == 4 && lastWidth == 5.25f && lastStyle == &nativeStyle,
          "DrawEllipse converts center/radii and forwards stroke arguments");
    impl__FillEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush___Z(&render, &oval, &brush);
    check(lastSlot == 21 && lastEllipse.radiusY == 4 && lastBrush == &nativeBrush, "FillEllipse dispatches to Direct2D");
    impl__DrawGeometry_CRenderTarget__QEAAXPEAVCD2DGeometry__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        &render, &geometryWrapper, &brush, 6.25f, &nativeStyle);
    check(lastSlot == 22 && lastResource == &nativeGeometry && lastWidth == 6.25f && lastStyle == &nativeStyle,
          "DrawGeometry reads the existing inline resource");
    verificationCount = 0;
    opacity.valid = false;
    impl__FillGeometry_CRenderTarget__QEAAXPEAVCD2DGeometry__PEAVCD2DBrush__1_Z(
        &render, &geometryWrapper, &brush, &opacity);
    check(lastSlot == 23 && lastResource == &nativeGeometry && lastOpacityBrush == &nativeOpacity &&
          verificationCount == 2 && verified[0] == &brush && verified[1] == &opacity,
          "FillGeometry preserves retail verification order and ignores optional-brush verification failure");
    impl__FillGeometry_CRenderTarget__QEAAXPEAVCD2DGeometry__PEAVCD2DBrush__1_Z(
        &render, &geometryWrapper, &brush, nullptr);
    check(lastSlot == 23 && lastOpacityBrush == nullptr, "FillGeometry accepts no opacity brush");
    verificationCount = 0;
    impl__FillMesh_CRenderTarget__QEAAXPEAVCD2DMesh__PEAVCD2DBrush___Z(&render, &meshWrapper, &brush);
    check(lastSlot == 24 && lastResource == &nativeMesh && verificationCount == 2 &&
          verified[0] == &meshWrapper && verified[1] == &brush, "FillMesh verifies mesh then brush and reads the inline mesh");
    D2D1_LAYER_PARAMETERS parameters{};
    verificationCount = 0;
    impl__PushLayer_CRenderTarget__QEAAXAEBUD2D1_LAYER_PARAMETERS__AEAVCD2DLayer___Z(&render, &parameters, &layerWrapper);
    check(lastSlot == 40 && lastLayerParameters == &parameters && lastResource == &nativeLayer &&
          verificationCount == 0, "PushLayer forwards the parameter reference without verifying the layer");
    layerWrapper.resource = nullptr;
    impl__PushLayer_CRenderTarget__QEAAXAEBUD2D1_LAYER_PARAMETERS__AEAVCD2DLayer___Z(&render, &parameters, &layerWrapper);
    check(lastSlot == 40 && lastResource == nullptr, "PushLayer accepts an unallocated layer for automatic allocation");
    int nativeFormat, nativeLayout;
    InlineWrapper formatWrapper{nullptr, true, nullptr, &nativeFormat};
    InlineWrapper layoutWrapper{nullptr, true, nullptr, &nativeLayout};
    CString embeddedText;
    wchar_t* buffer = embeddedText.GetBuffer(3);
    buffer[0] = L'A'; buffer[1] = 0; buffer[2] = L'B';
    embeddedText.ReleaseBuffer(3);
    verificationCount = 0;
    impl__DrawTextW_CRenderTarget__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCD2DRectF__PEAVCD2DBrush__PEAVCD2DTextFormat__W4D2D1_DRAW_TEXT_OPTIONS__W4DWRITE_MEASURING_MODE___Z(
        &render, &embeddedText, &rect, &brush, &formatWrapper, D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_GDI_CLASSIC);
    check(lastSlot == 27 && lastText.size() == 3 && lastText[1] == 0 && lastText[2] == L'B' &&
          lastTextFormat == &nativeFormat && lastRect.bottom == 12 && lastBrush == &nativeBrush &&
          lastTextOptions == D2D1_DRAW_TEXT_OPTIONS_CLIP && lastMeasuringMode == DWRITE_MEASURING_MODE_GDI_CLASSIC &&
          verificationCount == 1 && verified[0] == &brush, "DrawText forwards length, embedded NUL, format, bounds, brush and options");
    impl__DrawTextW_CRenderTarget__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___VCD2DRectF__PEAVCD2DBrush__PEAVCD2DTextFormat__W4D2D1_DRAW_TEXT_OPTIONS__W4DWRITE_MEASURING_MODE___Z(
        &render, &embeddedText, rect, &brush, &formatWrapper, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
    check(lastSlot == 27 && lastText.size() == 3 && lastRect.right == 11, "by-value DrawText rectangle overload forwards the complete rectangle");
    verificationCount = 0;
    impl__DrawTextLayout_CRenderTarget__QEAAXAEBVCD2DPointF__PEAVCD2DTextLayout__PEAVCD2DBrush__W4D2D1_DRAW_TEXT_OPTIONS___Z(
        &render, &start, &layoutWrapper, &brush, D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
    check(lastSlot == 28 && lastStart.x == start.x && lastStart.y == start.y && lastResource == &nativeLayout &&
          lastBrush == &nativeBrush && lastTextOptions == D2D1_DRAW_TEXT_OPTIONS_NO_SNAP &&
          verificationCount == 2 && verified[0] == &layoutWrapper && verified[1] == &brush,
          "DrawTextLayout forwards the point aggregate and verifies layout before brush");
    D2D1_PIXEL_FORMAT format{};
    D2D1_SIZE_U size{};
    check(impl__GetPixelFormat_CRenderTarget__QEBA_AUD2D1_PIXEL_FORMAT__XZ(&render, &format) == &format &&
          format.format == DXGI_FORMAT_B8G8R8A8_UNORM && format.alphaMode == D2D1_ALPHA_MODE_PREMULTIPLIED,
          "GetPixelFormat honors the hidden MSVC return slot");
    check(impl__GetPixelSize_CRenderTarget__QEBA_AVCD2DSizeU__XZ(&render, &size) == &size && size.width == 123 && size.height == 456,
          "GetPixelSize honors the hidden MSVC return slot");
    unsigned before = calls;
    brush.valid = false;
    impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(&render, &rect, &brush);
    brush.valid = true;
    brush.resource = nullptr;
    impl__FillEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush___Z(&render, &oval, &brush);
    impl__DrawLine_CRenderTarget__QEAAXAEBVCD2DPointF__0PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        &render, &start, &end, nullptr, 1, nullptr);
    meshWrapper.valid = false;
    impl__FillMesh_CRenderTarget__QEAAXPEAVCD2DMesh__PEAVCD2DBrush___Z(&render, &meshWrapper, &brush);
    check(calls == before, "invalid or missing resources do not issue COM drawing calls");
    render.Detach(); // the fake target deliberately has no Release slot
    verificationCount = 0;
    impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(&render, &rect, &brush);
    impl__DrawGeometry_CRenderTarget__QEAAXPEAVCD2DGeometry__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
        nullptr, &geometryWrapper, &brush, 1, nullptr);
    check(calls == before && verificationCount == 0, "missing targets return before resource verification");
}

static void textResources() {
    CRenderTarget parent;
    CString family(L"Arial"), locale(L"en-us"), text(L"ABCDEF");
    CD2DSizeF size(123, 45);
    IDWriteFactory* factory = openmfc::detail::d2dtext::Factory();
    check(factory != nullptr, "load the shared DirectWrite factory");
    if (!factory) return;
    IDWriteFontCollection* collection = nullptr;
    check(SUCCEEDED(factory->GetSystemFontCollection(&collection)), "obtain a font collection for constructor forwarding");
    alignas(CD2DTextFormat) unsigned char formatStorage[sizeof(CD2DTextFormat)];
    auto* format = static_cast<CD2DTextFormat*>(
        impl___0CD2DTextFormat__QEAA_PEAVCRenderTarget__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_A___MW4DWRITE_FONT_WEIGHT__W4DWRITE_FONT_STYLE__W4DWRITE_FONT_STRETCH__1PEAUIDWriteFontCollection__H_Z(
            formatStorage, &parent, &family, 17.5f, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_OBLIQUE,
            DWRITE_FONT_STRETCH_CONDENSED, &locale, collection, FALSE));
    check(format == reinterpret_cast<CD2DTextFormat*>(formatStorage) && format->m_pTextFormat &&
          format->m_pParentTarget == &parent && format->m_bIsAutoDestroy == FALSE,
          "exported text-format constructor returns this and initializes retail inline fields");
    if (!format->m_pTextFormat) {
        impl___1CD2DTextFormat__UEAA_XZ(format);
        if (collection) collection->Release();
        return;
    }
    check(format->m_pTextFormat->GetFontSize() == 17.5f && format->m_pTextFormat->GetFontWeight() == DWRITE_FONT_WEIGHT_BOLD &&
          format->m_pTextFormat->GetFontStyle() == DWRITE_FONT_STYLE_OBLIQUE &&
          format->m_pTextFormat->GetFontStretch() == DWRITE_FONT_STRETCH_CONDENSED,
          "text-format constructor forwards font size, weight, style and stretch");
    IDWriteFontCollection* actualCollection = nullptr;
    format->m_pTextFormat->GetFontCollection(&actualCollection);
    check(actualCollection == collection, "text-format constructor forwards the supplied font collection");
    if (actualCollection) actualCollection->Release();
    if (collection) collection->Release();

    void* const* vtable = *reinterpret_cast<void* const* const*>(format);
    using GetClass = CRuntimeClass* (MS_ABI*)(const void*);
    using Valid = BOOL (MS_ABI*)(const void*);
    using Create = HRESULT (MS_ABI*)(void*, CRenderTarget*);
    using Destroy = void (MS_ABI*)(void*);
    check(reinterpret_cast<GetClass>(vtable[0])(format) == impl__GetThisClass_CD2DTextFormat__SAPEAUCRuntimeClass__XZ(),
          "text-format vtable slot zero returns its real runtime descriptor");
    IDWriteTextFormat* beforeDestroy = format->m_pTextFormat;
    reinterpret_cast<Destroy>(vtable[7])(format);
    check(reinterpret_cast<Create>(vtable[5])(format, &parent) == S_OK &&
          reinterpret_cast<Create>(vtable[6])(format, &parent) == S_OK &&
          reinterpret_cast<Valid>(vtable[8])(format) && format->m_pTextFormat == beforeDestroy,
          "text-format virtual resource slots preserve the device-independent format");

    alignas(CString) unsigned char resultStorage[sizeof(CString)];
    auto* result = reinterpret_cast<CString*>(resultStorage);
    check(impl__GetFontFamilyName_CD2DTextFormat__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(format, result) == result &&
          std::wcscmp(result->GetString(), L"Arial") == 0, "font getter honors this/result order and returns the native font family");
    result->~CString();
    check(impl__GetLocaleName_CD2DTextFormat__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(format, result) == result &&
          std::wcscmp(result->GetString(), L"en-us") == 0, "locale getter honors this/result order and returns the native locale");
    result->~CString();

    alignas(CD2DTextLayout) unsigned char layoutStorage[sizeof(CD2DTextLayout)];
    auto* layout = static_cast<CD2DTextLayout*>(
        impl___0CD2DTextLayout__QEAA_PEAVCRenderTarget__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCD2DTextFormat__AEBVCD2DSizeF__H_Z(
            layoutStorage, &parent, &text, format, &size, FALSE));
    check(layout == reinterpret_cast<CD2DTextLayout*>(layoutStorage) && layout->m_pTextLayout &&
          layout->m_pTextLayout->GetMaxWidth() == 123 && layout->m_pTextLayout->GetMaxHeight() == 45,
          "exported layout constructor creates real text with the requested bounds");
    if (layout->m_pTextLayout) {
        check(impl__SetFontFamilyName_CD2DTextLayout__QEAAHPEB_WUDWRITE_TEXT_RANGE___Z(layout, L"Times New Roman", DWRITE_TEXT_RANGE{1, 2}),
              "font setter accepts a by-value MSVC text range");
        check(impl__SetLocaleName_CD2DTextLayout__QEAAHPEB_WUDWRITE_TEXT_RANGE___Z(layout, L"de-de", DWRITE_TEXT_RANGE{2, 2}),
              "locale setter accepts a by-value MSVC text range");
        DWRITE_TEXT_RANGE range{};
        DWRITE_TEXT_RANGE nativeRange{};
        wchar_t nativeName[64] = {};
        layout->m_pTextLayout->GetFontFamilyName(1, nativeName, 64, &nativeRange);
        impl__GetFontFamilyName_CD2DTextLayout__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEAUDWRITE_TEXT_RANGE___Z(layout, result, 1, &range);
        check(std::wcscmp(result->GetString(), L"Times New Roman") == 0 &&
              range.startPosition == nativeRange.startPosition && range.length == nativeRange.length && range.length > 0,
              "font getter queries the requested position and returns the affected range");
        result->~CString();
        impl__GetFontFamilyName_CD2DTextLayout__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEAUDWRITE_TEXT_RANGE___Z(layout, result, 0, nullptr);
        check(std::wcscmp(result->GetString(), L"Arial") == 0, "font changes preserve text outside the requested range");
        result->~CString();
        layout->m_pTextLayout->GetLocaleName(2, nativeName, 64, &nativeRange);
        impl__GetLocaleName_CD2DTextLayout__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEAUDWRITE_TEXT_RANGE___Z(layout, result, 2, &range);
        check(std::wcscmp(result->GetString(), L"de-de") == 0 &&
              range.startPosition == nativeRange.startPosition && range.length == nativeRange.length && range.length > 0,
              "locale getter queries the requested position and returns the affected range");
        result->~CString();
        impl__GetLocaleName_CD2DTextLayout__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEAUDWRITE_TEXT_RANGE___Z(layout, result, 4, nullptr);
        check(std::wcscmp(result->GetString(), L"en-us") == 0, "locale changes preserve text outside the requested range");
        result->~CString();
        check(format->m_pTextFormat == beforeDestroy, "string result construction leaves the receiver intact");
    }
    impl___1CD2DTextFormat__UEAA_XZ(format);
    check(g_renderTargetState[&parent].textResources.count(format) == 0,
          "format destructor unregisters the child from its parent");
    if (layout->m_pTextLayout) {
        DWRITE_TEXT_METRICS metrics{};
        check(SUCCEEDED(layout->m_pTextLayout->GetMetrics(&metrics)) && metrics.width > 0,
              "layout stays usable after the originating format is destroyed");
    }
    impl___1CD2DTextLayout__UEAA_XZ(layout);
    check(g_renderTargetState[&parent].textResources.empty(), "layout destructor unregisters the remaining child");

    CD2DTextFormat invalid(nullptr, family, -1, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
                           DWRITE_FONT_STRETCH_NORMAL, locale, nullptr, FALSE);
    check(!invalid.IsValid(), "invalid font size leaves an initialized, invalid format");
    CD2DTextLayout invalidLayout(nullptr, text, invalid, size, FALSE);
    check(!invalidLayout.IsValid() && !impl__SetLocaleName_CD2DTextLayout__QEAAHPEB_WUDWRITE_TEXT_RANGE___Z(&invalidLayout, L"fr-fr", DWRITE_TEXT_RANGE{0, 1}),
          "missing native layout reports setter failure");
    impl__GetFontFamilyName_CD2DTextFormat__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(&invalid, result);
    check(result->GetLength() == 0, "invalid formats return an empty font name");
    result->~CString();
}

struct ReleaseProbe { void** vtable; unsigned releases = 0; };
static ULONG STDMETHODCALLTYPE probeRelease(ReleaseProbe* probe) {
    ++probe->releases;
    std::unique_lock<std::mutex> lock(g_wave2StateMutex, std::try_to_lock);
    check(lock.owns_lock(), "resource release runs outside the render-target mutex");
    return 0;
}
static void textLifetime() {
    void* slots[3] = {nullptr, nullptr, reinterpret_cast<void*>(probeRelease)};
    ReleaseProbe owned{slots}, borrowed{slots};
    CString family(L"Arial"), locale(L"en-us");
    CD2DTextFormat* survivor = nullptr;
    {
        CRenderTarget parent;
        auto* resource = new CD2DTextFormat(&parent, family, 16, DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, locale, nullptr, TRUE);
        if (resource->m_pTextFormat) resource->m_pTextFormat->Release();
        resource->m_pTextFormat = reinterpret_cast<IDWriteTextFormat*>(&owned);
        survivor = new CD2DTextFormat(&parent, family, 16, DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, locale, nullptr, FALSE);
        if (survivor->m_pTextFormat) survivor->m_pTextFormat->Release();
        survivor->m_pTextFormat = reinterpret_cast<IDWriteTextFormat*>(&borrowed);
    }
    check(owned.releases == 1 && borrowed.releases == 0 && survivor->m_pParentTarget == nullptr,
          "target destruction deletes auto-owned text resources and detaches borrowed resources");
    delete survivor;
    check(borrowed.releases == 1, "borrowed text resource releases its interface on its own destruction");
}

static void textPixels(CRenderTarget& render, ID2D1RenderTarget* target, IWICBitmap* bitmap, ID2D1Brush* nativeBrush) {
    CString family(L"Arial"), locale(L"en-us"), text(L"MFC");
    CD2DTextFormat format(&render, family, 20, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
                          DWRITE_FONT_STRETCH_NORMAL, locale, nullptr, TRUE);
    CD2DTextLayout layout(&render, text, format, CD2DSizeF(64, 64), TRUE);
    check(format.IsValid() && layout.IsValid(), "construct native resources for text rendering");
    if (!format.IsValid() || !layout.IsValid()) return;
    TestResource brush{nativeBrush, true};
    CD2DRectF bounds(2, 3, 62, 63);
    CD2DPointF origin(2, 3);
    target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
    unsigned char actual[64 * 64 * 4] = {}, reference[64 * 64 * 4] = {};
    auto capture = [&](auto draw, unsigned char* pixels) {
        target->BeginDraw();
        target->Clear(D2D1::ColorF(D2D1::ColorF::Black));
        draw();
        check(SUCCEEDED(target->EndDraw()), "finish text drawing");
        check(SUCCEEDED(bitmap->CopyPixels(nullptr, 64 * 4, sizeof(actual), pixels)), "read text pixels");
    };
    auto nonempty = [&]() {
        for (unsigned i = 2; i < sizeof(actual); i += 4) if (actual[i] != 0) return true;
        return false;
    };
    capture([&] {
        impl__DrawTextLayout_CRenderTarget__QEAAXAEBVCD2DPointF__PEAVCD2DTextLayout__PEAVCD2DBrush__W4D2D1_DRAW_TEXT_OPTIONS___Z(
            &render, &origin, &layout, &brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
    }, actual);
    capture([&] {
        target->DrawTextLayout(D2D1_POINT_2F{2, 3}, layout.m_pTextLayout, nativeBrush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
    }, reference);
    check(nonempty() && std::memcmp(actual, reference, sizeof(actual)) == 0,
          "DrawTextLayout renders glyphs identical to direct COM drawing");
    capture([&] {
        impl__DrawTextW_CRenderTarget__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCD2DRectF__PEAVCD2DBrush__PEAVCD2DTextFormat__W4D2D1_DRAW_TEXT_OPTIONS__W4DWRITE_MEASURING_MODE___Z(
            &render, &text, &bounds, &brush, &format, D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_NATURAL);
    }, actual);
    const D2D1_RECT_F nativeBounds = {2, 3, 62, 63};
    capture([&] {
        target->DrawText(text.GetString(), text.GetLength(), format.m_pTextFormat, &nativeBounds,
                         nativeBrush, D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_NATURAL);
    }, reference);
    check(nonempty() && std::memcmp(actual, reference, sizeof(actual)) == 0,
          "DrawText renders glyphs identical to direct COM drawing");
    capture([&] {
        impl__DrawTextW_CRenderTarget__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCD2DRectF__PEAVCD2DBrush__PEAVCD2DTextFormat__W4D2D1_DRAW_TEXT_OPTIONS__W4DWRITE_MEASURING_MODE___Z(
            &render, &text, &bounds, &brush, nullptr, D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_NATURAL);
    }, actual);
    auto cached = g_renderTargetState[&render].defaultTextFormat;
    check(cached && g_renderTargetState[&render].textResources.count(cached.get()) == 0,
          "missing format creates an owned default without duplicate ownership");
    if (cached) {
        auto* defaultFormat = static_cast<CD2DTextFormat*>(cached.get());
        capture([&] {
            target->DrawText(text.GetString(), text.GetLength(), defaultFormat->m_pTextFormat, &nativeBounds,
                             nativeBrush, D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_NATURAL);
        }, reference);
        check(nonempty() && std::memcmp(actual, reference, sizeof(actual)) == 0,
              "DrawText with no format renders using the system message font");
        capture([&] {
            impl__DrawTextW_CRenderTarget__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCD2DRectF__PEAVCD2DBrush__PEAVCD2DTextFormat__W4D2D1_DRAW_TEXT_OPTIONS__W4DWRITE_MEASURING_MODE___Z(
                &render, &text, &bounds, &brush, nullptr, D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_NATURAL);
        }, actual);
        check(g_renderTargetState[&render].defaultTextFormat == cached,
              "repeated default text drawing reuses the cached format");
    }
    check(g_renderTargetState[&render].size.width == 0 && g_renderTargetState[&render].size.height == 0,
          "text drawing does not change render-target dimensions");
}

static void pixels() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    check(SUCCEEDED(hr), "initialize COM for software rendering");
    if (FAILED(hr)) return;
    IWICImagingFactory* wic = nullptr;
    ID2D1Factory* factory = nullptr;
    IWICBitmap* bitmap = nullptr;
    ID2D1RenderTarget* target = nullptr;
    ID2D1SolidColorBrush* nativeBrush = nullptr;
    ID2D1RectangleGeometry* strokeGeometry = nullptr;
    ID2D1RectangleGeometry* fillGeometryResource = nullptr;
    ID2D1Mesh* mesh = nullptr;
    ID2D1Layer* layer = nullptr;
    hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wic));
    if (SUCCEEDED(hr)) hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
    if (SUCCEEDED(hr)) hr = wic->CreateBitmap(64, 64, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, &bitmap);
    D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_SOFTWARE);
    if (SUCCEEDED(hr)) hr = factory->CreateWicBitmapRenderTarget(bitmap, properties, &target);
    if (SUCCEEDED(hr)) hr = target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &nativeBrush);
    if (SUCCEEDED(hr)) hr = factory->CreateRectangleGeometry(D2D1::RectF(20, 36, 28, 44), &strokeGeometry);
    if (SUCCEEDED(hr)) hr = factory->CreateRectangleGeometry(D2D1::RectF(36, 36, 44, 44), &fillGeometryResource);
    if (SUCCEEDED(hr)) hr = target->CreateMesh(&mesh);
    ID2D1TessellationSink* sink = nullptr;
    if (SUCCEEDED(hr)) hr = mesh->Open(&sink);
    if (SUCCEEDED(hr)) {
        D2D1_TRIANGLE triangle = {{4, 52}, {12, 52}, {8, 60}};
        sink->AddTriangles(&triangle, 1);
        hr = sink->Close();
    }
    if (sink) sink->Release();
    if (SUCCEEDED(hr)) hr = target->CreateLayer(&layer);
    if (FAILED(hr)) std::printf("Software target creation HRESULT: 0x%08lx\n", static_cast<unsigned long>(hr));
    check(SUCCEEDED(hr), "create real Direct2D software target and brush");
    if (SUCCEEDED(hr)) {
        CRenderTarget render;
        render.Attach(target);
        TestResource brush{nativeBrush, true};
        CD2DRectF rect(4, 4, 12, 12);
        CD2DPointF lineStart(4, 20), lineEnd(12, 20);
        CD2DRectF outlineRect(20, 4, 28, 12);
        CD2DEllipse outlineEllipse(CD2DPointF(40, 8), CD2DSizeF(4, 4));
        CD2DEllipse filledEllipse(CD2DPointF(24, 24), CD2DSizeF(4, 4));
        CD2DRoundedRect outlineRound(CD2DRectF(4, 36, 12, 44), CD2DSizeF(2, 2));
        CD2DRoundedRect filledRound(CD2DRectF(36, 20, 44, 28), CD2DSizeF(2, 2));
        InlineWrapper strokeWrapper{nullptr, true, nullptr, strokeGeometry};
        InlineWrapper fillWrapper{nullptr, true, nullptr, fillGeometryResource};
        InlineWrapper meshWrapper{nullptr, true, nullptr, mesh};
        InlineWrapper layerWrapper{nullptr, true, nullptr, layer};
        target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        render.BeginDraw();
        target->Clear(D2D1::ColorF(D2D1::ColorF::Black));
        impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(&render, &rect, &brush);
        impl__DrawLine_CRenderTarget__QEAAXAEBVCD2DPointF__0PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
            &render, &lineStart, &lineEnd, &brush, 2, nullptr);
        impl__DrawRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
            &render, &outlineRect, &brush, 2, nullptr);
        impl__DrawEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
            &render, &outlineEllipse, &brush, 2, nullptr);
        impl__FillEllipse_CRenderTarget__QEAAXAEBVCD2DEllipse__PEAVCD2DBrush___Z(&render, &filledEllipse, &brush);
        impl__DrawRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
            &render, &outlineRound, &brush, 2, nullptr);
        impl__FillRoundedRectangle_CRenderTarget__QEAAXAEBVCD2DRoundedRect__PEAVCD2DBrush___Z(&render, &filledRound, &brush);
        impl__DrawGeometry_CRenderTarget__QEAAXPEAVCD2DGeometry__PEAVCD2DBrush__MPEAUID2D1StrokeStyle___Z(
            &render, &strokeWrapper, &brush, 2, nullptr);
        impl__FillGeometry_CRenderTarget__QEAAXPEAVCD2DGeometry__PEAVCD2DBrush__1_Z(&render, &fillWrapper, &brush, nullptr);
        impl__FillMesh_CRenderTarget__QEAAXPEAVCD2DMesh__PEAVCD2DBrush___Z(&render, &meshWrapper, &brush);
        D2D1_LAYER_PARAMETERS parameters = D2D1::LayerParameters(D2D1::RectF(20, 52, 28, 60));
        impl__PushLayer_CRenderTarget__QEAAXAEBUD2D1_LAYER_PARAMETERS__AEAVCD2DLayer___Z(&render, &parameters, &layerWrapper);
        CD2DRectF clippedRect(16, 48, 32, 64);
        impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(&render, &clippedRect, &brush);
        target->PopLayer();
        check(SUCCEEDED(render.EndDraw()), "Direct2D finishes the drawing batch");
        unsigned char data[64 * 64 * 4] = {};
        check(SUCCEEDED(bitmap->CopyPixels(nullptr, 64 * 4, sizeof(data), data)), "read rendered WIC pixels");
        auto red = [&](unsigned x, unsigned y) {
            const unsigned i = (y * 64 + x) * 4;
            return data[i] == 0 && data[i + 1] == 0 && data[i + 2] == 255 && data[i + 3] == 255;
        };
        auto black = [&](unsigned x, unsigned y) {
            const unsigned i = (y * 64 + x) * 4;
            return data[i] == 0 && data[i + 1] == 0 && data[i + 2] == 0 && data[i + 3] == 255;
        };
        check(red(8, 8) && black(1, 1), "FillRectangle renders its interior and preserves outside pixels");
        check(red(8, 20) && black(8, 22), "DrawLine renders the requested stroke");
        check(red(20, 8) && black(24, 8), "DrawRectangle renders the outline without filling the center");
        check(red(40, 4) && black(40, 8), "DrawEllipse renders the outline without filling the center");
        check(red(24, 24) && black(30, 24), "FillEllipse renders its interior and preserves outside pixels");
        check(red(8, 36) && black(8, 40), "DrawRoundedRectangle renders the outline");
        check(red(40, 24) && black(46, 24), "FillRoundedRectangle renders its interior");
        check(red(20, 40) && black(24, 40), "DrawGeometry renders the wrapped geometry outline");
        check(red(40, 40) && black(46, 40), "FillGeometry renders the wrapped geometry interior");
        const bool meshPixels = red(8, 55) && black(2, 55);
        const bool layerPixels = red(24, 56) && black(18, 56);
        const bool wine = GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "wine_get_version") != nullptr;
        if (wine && (!meshPixels || !layerPixels)) {
            // Wine 11.17's FillMesh / PushLayer / PopLayer are host-side stubs.
            // Confirm our output matches direct COM calls before reporting the
            // unavailable pixel assertions. Native Windows must pass them.
            target->BeginDraw();
            target->Clear(D2D1::ColorF(D2D1::ColorF::Black));
            target->FillMesh(mesh, nativeBrush);
            target->PushLayer(&parameters, layer);
            const D2D1_RECT_F nativeRect = {16, 48, 32, 64};
            target->FillRectangle(&nativeRect, nativeBrush);
            target->PopLayer();
            check(SUCCEEDED(target->EndDraw()), "finish direct COM reference drawing for mesh/layer");
            unsigned char reference[64 * 64 * 4] = {};
            check(SUCCEEDED(bitmap->CopyPixels(nullptr, 64 * 4, sizeof(reference), reference)), "read direct COM reference pixels");
            check(std::memcmp(data + 48 * 64 * 4, reference + 48 * 64 * 4, 16 * 64 * 4) == 0,
                  "mesh and layer output exactly matches direct COM calls on Wine");
            if (!meshPixels) std::printf("SKIP: mesh pixel expectation unavailable in Wine's Direct2D implementation\n");
            if (!layerPixels) std::printf("SKIP: layer clipping pixel expectation unavailable in Wine's Direct2D implementation\n");
        } else {
            check(meshPixels, "FillMesh renders the wrapped triangle");
            check(layerPixels, "PushLayer clips drawing to its content bounds");
        }
        check(g_renderTargetState[&render].size.width == 0 && g_renderTargetState[&render].size.height == 0,
              "drawing no longer changes the render target's size");
        textPixels(render, target, bitmap, nativeBrush);
        render.Detach();
    }
    if (layer) layer->Release();
    if (mesh) mesh->Release();
    if (fillGeometryResource) fillGeometryResource->Release();
    if (strokeGeometry) strokeGeometry->Release();
    if (nativeBrush) nativeBrush->Release();
    if (target) target->Release();
    if (bitmap) bitmap->Release();
    if (factory) factory->Release();
    if (wic) wic->Release();
    CoUninitialize();
}

int main() {
    forwarding();
    textResources();
    textLifetime();
    pixels();
    std::printf("%d failures\n", failures);
    return failures != 0;
}
