#pragma once

#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <cstddef>
#include <new>

#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif

// afxrendertarget.h's _AFX_D2D_STATE (CNoTrackObject contributes one vfptr).
// Client-inline getters read these slots directly. Keep synchronization outside
// the object so the 64-byte MSVC layout remains intact.
class _AFX_D2D_STATE {
public:
    _AFX_D2D_STATE() noexcept = default;
    virtual ~_AFX_D2D_STATE();
    BOOL InitD2D(D2D1_FACTORY_TYPE = D2D1_FACTORY_TYPE_SINGLE_THREADED,
                  DWRITE_FACTORY_TYPE = DWRITE_FACTORY_TYPE_SHARED);
    void ReleaseD2DRefs();
    ID2D1Factory* GetDirect2dFactory() { InitD2D(); return m_pDirect2dFactory; }
    IDWriteFactory* GetWriteFactory() { InitD2D(); return m_pWriteFactory; }
    IWICImagingFactory* GetWICFactory() { InitD2D(); return m_pWicFactory; }

    // CNoTrackObject uses LocalAlloc/LocalFree, including its deleting destructor.
    static void* operator new(size_t size) {
        if (void* memory = LocalAlloc(LPTR, size)) return memory;
        throw std::bad_alloc();
    }
    static void operator delete(void* memory) noexcept { LocalFree(memory); }
    static void* operator new(size_t, void* memory) noexcept { return memory; }
    static void operator delete(void*, void*) noexcept {}

    HINSTANCE m_hinstD2DDLL = nullptr;             // +0x08
    HINSTANCE m_hinstDWriteDLL = nullptr;          // +0x10
    ID2D1Factory* m_pDirect2dFactory = nullptr;    // +0x18
    IDWriteFactory* m_pWriteFactory = nullptr;    // +0x20
    IWICImagingFactory* m_pWicFactory = nullptr;  // +0x28
    using Rotate = void (WINAPI*)(FLOAT, D2D1_POINT_2F, D2D1_MATRIX_3X2_F*);
    Rotate m_pfD2D1MakeRotateMatrix = nullptr;     // +0x30
    BOOL m_bD2DInitialized = FALSE;               // +0x38
    BOOL m_bComInitialized = FALSE;               // +0x3c
};
static_assert(sizeof(_AFX_D2D_STATE) == 64, "retail _AFX_D2D_STATE size");
static_assert(offsetof(_AFX_D2D_STATE, m_pDirect2dFactory) == 0x18, "Direct2D factory offset");
static_assert(offsetof(_AFX_D2D_STATE, m_pWriteFactory) == 0x20, "DirectWrite factory offset");
static_assert(offsetof(_AFX_D2D_STATE, m_pWicFactory) == 0x28, "WIC factory offset");
static_assert(offsetof(_AFX_D2D_STATE, m_bD2DInitialized) == 0x38, "initialization flag offset");
static_assert(offsetof(_AFX_D2D_STATE, m_bComInitialized) == 0x3c, "COM ownership flag offset");

_AFX_D2D_STATE& OpenMfcD2DState();
ID2D1RenderTarget* OpenMfcD2DRenderTarget(const void*);

extern "C" void* MS_ABI impl__AfxGetD2DState__YAPEAV_AFX_D2D_STATE__XZ();
extern "C" void MS_ABI impl__AfxReleaseD2DRefs__YAXXZ();
