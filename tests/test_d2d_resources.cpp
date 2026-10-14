// Exercise the production resource registry, virtual lifecycle and drawing.
// Unrelated bitmap/GDI/archive siblings abort if these paths reach them.
#include "detail/D2DTextSupport.h"
#include "detail/GdicoreSupport.h"
#include "d2d_test_support.h"
#include <wincodec.h>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" void* MS_ABI impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(void*, void*, int);
extern "C" void* MS_ABI impl___1CD2DResource__MEAA_XZ(void*);
extern "C" void* MS_ABI impl___0CD2DBrush__IEAA_PEAVCRenderTarget__PEAVCD2DBrushProperties__H_Z(void*, void*, void*, int);
extern "C" void MS_ABI impl___1CD2DBrush__MEAA_XZ(void*);
extern "C" float MS_ABI impl__GetOpacity_CD2DBrush__QEBAMXZ(const void*);
extern "C" void MS_ABI impl__GetTransform_CD2DBrush__QEBAXPEAUD2D_MATRIX_3X2_F___Z(const void*, D2D_MATRIX_3X2_F*);
extern "C" void MS_ABI impl__SetOpacity_CD2DBrush__QEAAXM_Z(void*, float);
extern "C" void MS_ABI impl__SetTransform_CD2DBrush__QEAAXPEBUD2D_MATRIX_3X2_F___Z(void*, const D2D_MATRIX_3X2_F*);
extern "C" void MS_ABI impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(void*, void*);
extern "C" void* MS_ABI impl__Detach_CD2DBrush__QEAAPEAUID2D1Brush__XZ(void*);
extern "C" void* MS_ABI impl___0CD2DSolidColorBrush__QEAA_PEAVCRenderTarget__KHPEAVCD2DBrushProperties__H_Z(void*, void*, unsigned long, int, void*, int);
extern "C" void* MS_ABI impl___0CD2DSolidColorBrush__QEAA_PEAVCRenderTarget__U_D3DCOLORVALUE__PEAVCD2DBrushProperties__H_Z(void*, void*, const void*, void*, int);
extern "C" void MS_ABI impl___1CD2DSolidColorBrush__UEAA_XZ(void*);
extern "C" void* MS_ABI impl__Detach_CD2DSolidColorBrush__QEAAPEAUID2D1SolidColorBrush__XZ(void*);
extern "C" void MS_ABI impl__Attach_CD2DSolidColorBrush__QEAAXPEAUID2D1SolidColorBrush___Z(void*, void*);
extern "C" void* MS_ABI impl__GetColor_CD2DSolidColorBrush__QEBA_AU_D3DCOLORVALUE__XZ(const void*, void*);
extern "C" void MS_ABI impl__SetColor_CD2DSolidColorBrush__QEAAXU_D3DCOLORVALUE___Z(void*, const void*);
extern "C" int MS_ABI impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(void*, void*);
extern "C" void* MS_ABI impl___0CD2DPathGeometry__QEAA_PEAVCRenderTarget__H_Z(void*, void*, int);
extern "C" ID2D1GeometrySink* MS_ABI impl__Open_CD2DPathGeometry__QEAAPEAUID2D1GeometrySink__XZ(void*);
extern "C" void* MS_ABI impl___0CD2DLayer__QEAA_PEAVCRenderTarget__H_Z(void*, void*, int);
extern "C" void* MS_ABI impl___0CD2DMesh__QEAA_PEAVCRenderTarget__H_Z(void*, void*, int);
extern "C" void MS_ABI impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(CRenderTarget*, const CD2DRectF*, void*);
extern "C" void MS_ABI impl__FillGeometry_CRenderTarget__QEAAXPEAVCD2DGeometry__PEAVCD2DBrush__1_Z(CRenderTarget*, void*, void*, void*);
extern "C" void MS_ABI impl__AfxReleaseD2DRefs__YAXXZ();

// Commands and notifications are outside the D2D paint integration tests.

static int failures;
static void check(bool ok, const char* label) {
    std::printf("%s: %s\n", ok ? "PASS" : "FAIL", label);
    if (!ok) ++failures;
}
static bool unlocked() {
    std::unique_lock<std::mutex> lock(g_wave2StateMutex, std::try_to_lock);
    return lock.owns_lock();
}
class LifecycleProbe : public CD2DResource {
public:
    std::vector<int> calls;
    HRESULT result = S_OK;
    bool valid = false;
    CRenderTarget* lastTarget = nullptr;
    LifecycleProbe(CRenderTarget* parent) : CD2DResource(parent, FALSE) {}
    HRESULT Create(CRenderTarget* parent) override {
        check(unlocked(), "Create callback runs outside the render-target mutex");
        calls.push_back(6); lastTarget = parent; valid = SUCCEEDED(result); return result;
    }
    void Destroy() override {
        check(unlocked(), "Destroy callback runs outside the render-target mutex");
        calls.push_back(7); valid = false;
    }
    BOOL IsValid() const override { return valid; }
};
static void lifecycle() {
    CRenderTarget owner, other;
    LifecycleProbe resource(&owner);
    check(owner.m_lstResources.GetCount() == 1, "base construction registers one child in the native resource list");
    check(!impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&owner, nullptr), "verification rejects a null resource");
    resource.result = E_ACCESSDENIED;
    check(!impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&owner, &resource) && resource.calls == std::vector<int>{6},
          "verification invokes Create on an invalid resource and propagates failure without destroying it");
    resource.calls.clear(); resource.result = S_FALSE;
    check(impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&owner, &resource) && resource.calls == std::vector<int>{6},
          "verification accepts successful HRESULTs other than S_OK");
    resource.calls.clear();
    check(impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&other, &resource) && resource.calls.empty() &&
          resource.m_pParentTarget == &owner, "verification preserves valid resources and their original owner");
    resource.result = E_FAIL;
    check(impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(&resource, &other) == E_FAIL &&
          resource.calls == std::vector<int>({7, 6}) && resource.lastTarget == &other && resource.m_pParentTarget == &owner,
          "base ReCreate calls virtual Destroy before Create, forwards the new target and returns its HRESULT unchanged");

    alignas(CD2DResource) unsigned char raw[sizeof(CD2DResource)];
    std::memset(raw, 0xa5, sizeof(raw));
    auto* base = static_cast<CD2DResource*>(impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(raw, &owner, FALSE));
    check(base->GetRuntimeClass() == impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ() &&
          base->m_pParentTarget == &owner && !base->m_bIsAutoDestroy && owner.m_lstResources.GetCount() == 2,
          "exported abstract-base constructor initializes its real vtable, inline owner and BOOL in poisoned storage");
    impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(raw, &owner, FALSE);
    check(owner.m_lstResources.GetCount() == 2, "base constructor avoids duplicate registry entries");
    impl___1CD2DResource__MEAA_XZ(raw);
    check(owner.m_lstResources.GetCount() == 1, "exported base destructor unregisters its child");

    alignas(CD2DBrush) unsigned char brushRaw[sizeof(CD2DBrush)];
    std::memset(brushRaw, 0xa5, sizeof(brushRaw));
    D2D1_BRUSH_PROPERTIES props = D2D1::BrushProperties(0.4f);
    auto* brush = static_cast<CD2DBrush*>(impl___0CD2DBrush__IEAA_PEAVCRenderTarget__PEAVCD2DBrushProperties__H_Z(brushRaw, &owner, &props, FALSE));
    check(brush->GetRuntimeClass() == impl__GetThisClass_CD2DBrush__SAPEAUCRuntimeClass__XZ() && !brush->m_pBrush &&
          brush->m_pBrushProperties && brush->m_pBrushProperties != &props && brush->m_pBrushProperties->opacity == 0.4f,
          "exported abstract brush constructor installs its vtable and owns an independent native properties copy");
    D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Translation(12, 13), before = matrix;
    impl__GetTransform_CD2DBrush__QEBAXPEAUD2D_MATRIX_3X2_F___Z(brush, &matrix);
    impl__SetOpacity_CD2DBrush__QEAAXM_Z(brush, 0.9f);
    check(std::memcmp(&matrix, &before, sizeof(matrix)) == 0 && impl__GetOpacity_CD2DBrush__QEBAMXZ(brush) == 0 &&
          brush->m_pBrushProperties->opacity == 0.4f, "uninitialized brush getters/setters preserve retail behavior and construction properties");
    impl___1CD2DBrush__MEAA_XZ(brush);
    check(owner.m_lstResources.GetCount() == 1, "abstract brush destruction frees properties and unregisters the base");
}

static void nativeResources() {
    auto& state = OpenMfcD2DState();
    auto* factory = state.GetDirect2dFactory();
    auto* wic = state.GetWICFactory();
    IWICBitmap* pixels = nullptr;
    ID2D1RenderTarget* native = nullptr;
    HRESULT hr = factory && wic ? wic->CreateBitmap(32, 32, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, &pixels) : E_FAIL;
    if (SUCCEEDED(hr)) hr = factory->CreateWicBitmapRenderTarget(pixels,
        D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_SOFTWARE), &native);
    check(SUCCEEDED(hr), "create a shared-factory software render target for real resource drawing");
    if (FAILED(hr)) { if (pixels) pixels->Release(); return; }
    {
        CRenderTarget render;
        render.Attach(native);
        D2D1_BRUSH_PROPERTIES props = D2D1::BrushProperties(0.35f, D2D1::Matrix3x2F::Translation(2, 3));
        alignas(CD2DSolidColorBrush) unsigned char storage[sizeof(CD2DSolidColorBrush)];
        std::memset(storage, 0xa5, sizeof(storage));
        D2D1_COLOR_F red{1, 0, 0, 1};
        auto* brush = static_cast<CD2DSolidColorBrush*>(impl___0CD2DSolidColorBrush__QEAA_PEAVCRenderTarget__U_D3DCOLORVALUE__PEAVCD2DBrushProperties__H_Z(
            storage, &render, &red, &props, FALSE));
        props.opacity = 0.9f;
        check(brush->GetRuntimeClass() == impl__GetThisClass_CD2DSolidColorBrush__SAPEAUCRuntimeClass__XZ() &&
              !brush->IsValid() && !brush->m_pSolidColorBrush && brush->m_pBrushProperties->opacity == 0.35f,
              "exported solid brush constructor initializes its vtable, aliases, color and copied properties");
        check(impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&render, brush) &&
              brush->m_pBrush == brush->m_pSolidColorBrush && brush->m_pSolidColorBrush->GetOpacity() == 0.35f,
              "real verification lazily creates a native solid brush with the copied construction properties");
        if (brush->IsValid()) {
            check(brush->Create(&render) == E_FAIL, "duplicate brush creation fails without replacing the owned interface");
            impl__SetOpacity_CD2DBrush__QEAAXM_Z(brush, 0.5f);
            auto transform = D2D1::Matrix3x2F::Scale(2, 3);
            impl__SetTransform_CD2DBrush__QEAAXPEBUD2D_MATRIX_3X2_F___Z(brush, &transform);
            D2D1_MATRIX_3X2_F result{};
            impl__GetTransform_CD2DBrush__QEBAXPEAUD2D_MATRIX_3X2_F___Z(brush, &result);
            check(impl__GetOpacity_CD2DBrush__QEBAMXZ(brush) == 0.5f && std::memcmp(&result, &transform, sizeof(result)) == 0,
                  "brush opacity and transform setters/getters operate on the actual COM interface");
            D2D1_COLOR_F blue{0, 0, 1, 1}, color{};
            impl__SetColor_CD2DSolidColorBrush__QEAAXU_D3DCOLORVALUE___Z(brush, &blue);
            check(impl__GetColor_CD2DSolidColorBrush__QEBA_AU_D3DCOLORVALUE__XZ(brush, &color) == &color && color.b == 1 &&
                  brush->m_colorSolid.b == 1, "color getter honors the MSVC hidden return slot and setter updates native and cached color");
            auto* previous = brush->m_pSolidColorBrush; previous->AddRef();
            check(impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(brush, &render) == S_OK &&
                  brush->IsValid() && brush->m_pSolidColorBrush != previous && !brush->m_pBrushProperties &&
                  brush->m_pSolidColorBrush->GetOpacity() == 1 && brush->m_pSolidColorBrush->GetColor().b == 1,
                  "recreation releases the old brush/properties and creates a replacement from the retained color");
            previous->Release();
            auto* detached = impl__Detach_CD2DSolidColorBrush__QEAAPEAUID2D1SolidColorBrush__XZ(brush);
            check(detached && !brush->m_pBrush && !brush->m_pSolidColorBrush, "solid brush Detach transfers one reference and clears both inline aliases");
            impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(brush, detached);
            check(brush->IsValid() && !brush->m_pSolidColorBrush && impl__Detach_CD2DBrush__QEAAPEAUID2D1Brush__XZ(brush) == detached,
                  "base brush Attach/Detach uses its distinct native field without inventing a derived alias");
            impl__Attach_CD2DSolidColorBrush__QEAAXPEAUID2D1SolidColorBrush___Z(brush, detached);
            impl__SetColor_CD2DSolidColorBrush__QEAAXU_D3DCOLORVALUE___Z(brush, &red);
        }
        alignas(CD2DPathGeometry) unsigned char pathStorage[sizeof(CD2DPathGeometry)];
        std::memset(pathStorage, 0xa5, sizeof(pathStorage));
        auto* path = static_cast<CD2DPathGeometry*>(impl___0CD2DPathGeometry__QEAA_PEAVCRenderTarget__H_Z(pathStorage, &render, FALSE));
        check(path->GetRuntimeClass() == impl__GetThisClass_CD2DPathGeometry__SAPEAUCRuntimeClass__XZ() &&
              impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&render, path) && path->m_pGeometry == path->m_pPathGeometry,
              "path constructor installs its runtime vtable and lazy verification creates both native geometry aliases");
        auto* sink = impl__Open_CD2DPathGeometry__QEAAPEAUID2D1GeometrySink__XZ(path);
        if (sink) {
            sink->BeginFigure(D2D1_POINT_2F{16, 16}, D2D1_FIGURE_BEGIN_FILLED);
            sink->AddLine(D2D1_POINT_2F{28, 16}); sink->AddLine(D2D1_POINT_2F{16, 28});
            sink->EndFigure(D2D1_FIGURE_END_CLOSED); hr = sink->Close(); sink->Release();
        } else hr = E_FAIL;
        check(SUCCEEDED(hr), "populate a real native path geometry");
        if (brush->IsValid() && path->IsValid()) {
            render.BeginDraw(); render.Clear(CD2DColorF{1, 1, 1, 1});
            CD2DRectF rect(2, 2, 10, 10);
            impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(&render, &rect, brush);
            impl__FillGeometry_CRenderTarget__QEAAXPEAVCD2DGeometry__PEAVCD2DBrush__1_Z(&render, path, brush, nullptr);
            check(SUCCEEDED(render.EndDraw()), "production drawing succeeds with real resource verification and native brushes/geometries");
            BYTE pixel[4]{}; WICRect sample{4, 4, 1, 1};
            check(SUCCEEDED(pixels->CopyPixels(&sample, 4, 4, pixel)) && pixel[2] == 255 && pixel[0] == 0 && pixel[1] == 0,
                  "production FillRectangle renders the native brush's red color");
            sample = {18, 18, 1, 1};
            check(SUCCEEDED(pixels->CopyPixels(&sample, 4, 4, pixel)) && pixel[2] == 255 && pixel[0] == 0 && pixel[1] == 0,
                  "production FillGeometry renders the lazily verified path with the native brush");
        }
        state.m_pDirect2dFactory = nullptr;
        check(impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(path, &render) == E_FAIL && !path->m_pGeometry && !path->m_pPathGeometry,
              "failed path recreation clears both aliases so destruction cannot release a stale pointer");
        state.m_pDirect2dFactory = factory;
        check(impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&render, path), "failed geometry can be lazily created again after factory recovery");
        alignas(CD2DLayer) unsigned char layerStorage[sizeof(CD2DLayer)];
        alignas(CD2DMesh) unsigned char meshStorage[sizeof(CD2DMesh)];
        std::memset(layerStorage, 0xa5, sizeof(layerStorage));
        std::memset(meshStorage, 0xa5, sizeof(meshStorage));
        auto& layer = *static_cast<CD2DLayer*>(impl___0CD2DLayer__QEAA_PEAVCRenderTarget__H_Z(layerStorage, &render, FALSE));
        auto& mesh = *static_cast<CD2DMesh*>(impl___0CD2DMesh__QEAA_PEAVCRenderTarget__H_Z(meshStorage, &render, FALSE));
        check(impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&render, &layer) && layer.IsValid() &&
              impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&render, &mesh) && mesh.IsValid(),
              "layer and mesh virtual Create/IsValid slots produce real native resources");
        check(render.m_lstResources.GetCount() == 4, "all resource families register in the same native list");
        check(impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(&layer, &render) == S_OK &&
              impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(&mesh, &render) == S_OK,
              "generic recreation dispatches to the native layer and mesh lifecycle");
        path->~CD2DPathGeometry(); impl___1CD2DSolidColorBrush__UEAA_XZ(brush);
        check(render.m_lstResources.GetCount() == 2, "derived resource destructors release their native interfaces and unregister");
        layer.~CD2DLayer(); mesh.~CD2DMesh();
        check(render.m_lstResources.IsEmpty(), "export-constructed layer and mesh destructors remove their native registry entries");
    }
    pixels->Release();
}

struct ReleaseCounter { void** table; int count = 0; };
static ULONG STDMETHODCALLTYPE releaseCounter(ReleaseCounter* self) {
    check(unlocked(), "owned brush release runs outside the render-target mutex");
    ++self->count; return 0;
}
static void ownershipAndRecreation() {
    void* slots[3] = {nullptr, nullptr, reinterpret_cast<void*>(releaseCounter)};
    ReleaseCounter owned{slots}, borrowed{slots};
    {
        auto* owner = new CRenderTarget;
        CD2DSolidColorBrush survivor(owner, D2D1_COLOR_F{1, 0, 0, 1}, nullptr, FALSE);
        auto* brush = new CD2DSolidColorBrush(owner, D2D1_COLOR_F{1, 0, 0, 1}, nullptr, TRUE);
        impl__Attach_CD2DSolidColorBrush__QEAAXPEAUID2D1SolidColorBrush___Z(brush, &owned);
        impl__Attach_CD2DSolidColorBrush__QEAAXPEAUID2D1SolidColorBrush___Z(&survivor, &borrowed);
        delete owner;
        check(owned.count == 1 && borrowed.count == 0 && !survivor.m_pParentTarget && survivor.IsValid(),
              "owner destruction dispatches the owned brush deleting destructor once and preserves borrowed interfaces");
    }
    check(borrowed.count == 1, "borrowed brush releases its native interface on its own destruction");

    HWND window = CreateWindowExW(0, L"STATIC", L"OpenMFC resource recreation", WS_OVERLAPPEDWINDOW,
        0, 0, 80, 80, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
    check(window != nullptr, "create a real HWND for device-resource recreation");
    if (!window) return;
    {
        CHwndRenderTarget target(window);
        CD2DSolidColorBrush brush(&target, D2D1_COLOR_F{0, 1, 0, 1}, nullptr, FALSE);
        CD2DLayer layer(&target, FALSE); CD2DMesh mesh(&target, FALSE); CD2DPathGeometry path(&target, FALSE);
        bool created = target.IsValid() &&
            impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&target, &brush) &&
            impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&target, &layer) &&
            impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&target, &mesh) &&
            impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&target, &path);
        check(created, "create all registered native resource families on the HWND target");
        if (created) {
            auto* oldTarget = target.GetHwndRenderTarget(); oldTarget->AddRef();
            auto* oldBrush = brush.m_pSolidColorBrush; oldBrush->AddRef();
            check(target.ReCreate(window) && target.GetHwndRenderTarget() != oldTarget && brush.IsValid() &&
                  layer.IsValid() && mesh.IsValid() && path.IsValid() && brush.m_pSolidColorBrush != oldBrush &&
                  target.m_lstResources.GetCount() == 4,
                  "HWND recreation walks the native registry and rebuilds brushes, layers, meshes and paths via their virtual lifecycle");
            oldBrush->Release(); oldTarget->Release();
        }
    }
    DestroyWindow(window);
}

#include "d2d_bitmap_gradient_tests.h"
#include "d2d_window_tests.h"
#include "cwnd_accessibility_tests.h"
#include "cmdtarget_vtable_tests.h"
#include "cmdtarget_dispatch_tests.h"
#include "cmdtarget_invoke_tests.h"
#include "cmdtarget_event_tests.h"
#include "cmdtarget_verb_tests.h"
#include "command_routing_tests.h"
#include "window_message_context_tests.h"
#include "window_input_tests.h"
#include "dynamic_layout_tests.h"
#include "window_message_map_tests.h"
#include "cmdui_tests.h"
#include "ribbon_cmdui_tests.h"
#include "field_exchange_tests.h"
#include "pointer_map_tests.h"
#include "recordset_storage_tests.h"
#include "rfx_binary_tests.h"
#include "rfx_scalar_tests.h"
#include "rfx_text_buffer_tests.h"
#include "database_layout_tests.h"
#include "database_strings_tests.h"
#include "rfx_text_string_tests.h"
#include "rfx_date_tests.h"
#include "data_exchange_tests.h"
#include "dlginit_tests.h"
#include "fontcombo_tests.h"
#include "cmdtarget_connection_tests.h"
#include "connection_point_sink_tests.h"
#include "automation_exception_tests.h"
#include "framework_boundary_exception_tests.h"
#include "typelib_cache_tests.h"

int main() {
    std::setvbuf(stdout,nullptr,_IONBF,0);
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    check(SUCCEEDED(hr), "initialize COM for resource integration tests");
    lifecycle();
    if (SUCCEEDED(hr)) { nativeResources(); ownershipAndRecreation(); bitmapAndGradients(); windowD2D(); windowAccessibility(); accessibilityProxy(); accessibilityProxyForwarding(); cmdtargetVtable(); cmdtargetDispatch(); cmdtargetInvoke(); cmdtargetEvents(); cmdtargetVerbs(); commandRouting(); windowMessageContext(); windowInput(); cmdtargetConnections(); connectionPointSinks(); automationExceptions(); frameworkBoundaryExceptions(); typeLibCache(); }
    if (SUCCEEDED(hr)) dynamicLayout();
    if (SUCCEEDED(hr)) windowMessageMaps();
    if (SUCCEEDED(hr)) commandUI();
    if (SUCCEEDED(hr)) ribbonCommandUI();
    if (SUCCEEDED(hr)) fieldExchange();
    if (SUCCEEDED(hr)) pointerMap();
    if (SUCCEEDED(hr)) recordsetFieldStorage();
    if (SUCCEEDED(hr)) binaryFieldExchange();
    if (SUCCEEDED(hr)) scalarFieldExchange();
    if (SUCCEEDED(hr)) textBufferFieldExchange();
    if (SUCCEEDED(hr)) databaseLayout();
    if (SUCCEEDED(hr)) databaseStrings();
    if (SUCCEEDED(hr)) textStringFieldExchange();
    if (SUCCEEDED(hr)) dateFieldExchange();
    if (SUCCEEDED(hr)) dataExchange();
    if (SUCCEEDED(hr)) dlgInit();
    if (SUCCEEDED(hr)) fontCombo();
    impl__AfxReleaseD2DRefs__YAXXZ();
    if (SUCCEEDED(hr)) CoUninitialize();
    std::printf("%d failures\n", failures);
    return failures != 0;
}
