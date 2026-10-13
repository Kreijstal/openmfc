// _AFX_D2D_STATE: process-wide graphics factories and balanced COM ownership.
// Retail mfc140u: ctor 0xd2240, InitD2D 0xd22e0, ReleaseD2DRefs 0xd2400.
#include "detail/D2DFactorySupport.h"
#include <mutex>

namespace {
std::recursive_mutex stateMutex;
_AFX_D2D_STATE processState;
}

_AFX_D2D_STATE& OpenMfcD2DState() { return processState; }
_AFX_D2D_STATE::~_AFX_D2D_STATE() { ReleaseD2DRefs(); }

BOOL _AFX_D2D_STATE::InitD2D(D2D1_FACTORY_TYPE d2dType, DWRITE_FACTORY_TYPE writeType) {
    std::lock_guard<std::recursive_mutex> lock(stateMutex);
    if (m_bD2DInitialized) return TRUE;
    if (!m_bComInitialized) {
        if (FAILED(CoInitialize(nullptr))) return FALSE;
        m_bComInitialized = TRUE;
    }
    if (!m_hinstD2DDLL) m_hinstD2DDLL = LoadLibraryW(L"d2d1.dll");
    if (!m_hinstD2DDLL) return FALSE;
    using CreateD2D = HRESULT (WINAPI*)(D2D1_FACTORY_TYPE, REFIID, const D2D1_FACTORY_OPTIONS*, void**);
    auto createD2D = reinterpret_cast<CreateD2D>(GetProcAddress(m_hinstD2DDLL, "D2D1CreateFactory"));
    if (createD2D && FAILED(createD2D(d2dType, __uuidof(ID2D1Factory), nullptr,
                                    reinterpret_cast<void**>(&m_pDirect2dFactory)))) {
        m_pDirect2dFactory = nullptr;
        return FALSE;
    }
    m_pfD2D1MakeRotateMatrix = reinterpret_cast<Rotate>(GetProcAddress(m_hinstD2DDLL, "D2D1MakeRotateMatrix"));
    if (!m_hinstDWriteDLL) m_hinstDWriteDLL = LoadLibraryW(L"dwrite.dll");
    if (m_hinstDWriteDLL) {
        using CreateWrite = HRESULT (WINAPI*)(DWRITE_FACTORY_TYPE, REFIID, IUnknown**);
        auto createWrite = reinterpret_cast<CreateWrite>(GetProcAddress(m_hinstDWriteDLL, "DWriteCreateFactory"));
        if (createWrite) createWrite(writeType, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pWriteFactory));
    }
    CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                     __uuidof(IWICImagingFactory), reinterpret_cast<void**>(&m_pWicFactory));
    // Retail ignores DirectWrite/WIC creation failures; callers test the pointers.
    m_bD2DInitialized = TRUE;
    return TRUE;
}

void _AFX_D2D_STATE::ReleaseD2DRefs() {
    std::lock_guard<std::recursive_mutex> lock(stateMutex);
    if (m_pDirect2dFactory) { m_pDirect2dFactory->Release(); m_pDirect2dFactory = nullptr; }
    if (m_pWriteFactory) { m_pWriteFactory->Release(); m_pWriteFactory = nullptr; }
    if (m_pWicFactory) { m_pWicFactory->Release(); m_pWicFactory = nullptr; }
    if (m_hinstD2DDLL) { FreeLibrary(m_hinstD2DDLL); m_hinstD2DDLL = nullptr; }
    if (m_hinstDWriteDLL) { FreeLibrary(m_hinstDWriteDLL); m_hinstDWriteDLL = nullptr; }
    m_pfD2D1MakeRotateMatrix = nullptr;
    m_bD2DInitialized = FALSE;
    if (m_bComInitialized) { CoUninitialize(); m_bComInitialized = FALSE; }
}

// Symbol: ??0_AFX_D2D_STATE@@QEAA@XZ
extern "C" _AFX_D2D_STATE* MS_ABI impl___0_AFX_D2D_STATE__QEAA_XZ(void* self) {
    return self ? new(self) _AFX_D2D_STATE() : nullptr;
}
// Symbol: ??1_AFX_D2D_STATE@@UEAA@XZ
extern "C" void MS_ABI impl___1_AFX_D2D_STATE__UEAA_XZ(void* self) {
    if (self) static_cast<_AFX_D2D_STATE*>(self)->~_AFX_D2D_STATE();
}
// Symbol: ?InitD2D@_AFX_D2D_STATE@@QEAAHW4D2D1_FACTORY_TYPE@@W4DWRITE_FACTORY_TYPE@@@Z
extern "C" int MS_ABI impl__InitD2D__AFX_D2D_STATE__QEAAHW4D2D1_FACTORY_TYPE__W4DWRITE_FACTORY_TYPE___Z(
    void* self, D2D1_FACTORY_TYPE d2dType, DWRITE_FACTORY_TYPE writeType) {
    return self && static_cast<_AFX_D2D_STATE*>(self)->InitD2D(d2dType, writeType);
}
// Symbol: ?ReleaseD2DRefs@_AFX_D2D_STATE@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseD2DRefs__AFX_D2D_STATE__QEAAXXZ(void* self) {
    if (self) static_cast<_AFX_D2D_STATE*>(self)->ReleaseD2DRefs();
}
// Symbol: ?AfxGetD2DState@@YAPEAV_AFX_D2D_STATE@@XZ
extern "C" void* MS_ABI impl__AfxGetD2DState__YAPEAV_AFX_D2D_STATE__XZ() { return &processState; }
// Symbol: ?AfxReleaseD2DRefs@@YAXXZ
extern "C" void MS_ABI impl__AfxReleaseD2DRefs__YAXXZ() { processState.ReleaseD2DRefs(); }
