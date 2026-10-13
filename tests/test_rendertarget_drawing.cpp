// Standalone MSVC ABI test of the production drawing thunks. Only sibling
// wrapper access/verification is doubled; render-target state and drawing code
// come from the real source. A software WIC target checks actual pixel output.
#include "../phase4/src/core/d2d/CRenderTarget.cpp"
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
extern "C" void MS_ABI impl__Attach_CBitmapRenderTarget__QEAAXPEAUID2D1BitmapRenderTarget___Z(void*, void*) { std::abort(); }
extern "C" void MS_ABI impl__Attach_CD2DBitmap__QEAAXPEAUID2D1Bitmap___Z(void*, void*) { std::abort(); }
extern "C" void* MS_ABI impl__Detach_CD2DBitmap__QEAAPEAUID2D1Bitmap__XZ(void*) { std::abort(); }

struct TestResource {
    void* resource;
    bool valid;
};
static void* verified[8];
static unsigned verificationCount;
extern "C" int MS_ABI impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(void*, void* wrapper) {
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
    pixels();
    std::printf("%d failures\n", failures);
    return failures != 0;
}
