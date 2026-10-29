// CReflectorWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

extern "C" int MS_ABI impl__AfxEndDeferRegisterClass__YAHJ_Z(long flags);

static_assert(sizeof(CRect) == 16 && offsetof(CRect, left) == 0 && offsetof(CRect, top) == 4 &&
              offsetof(CRect, right) == 8 && offsetof(CRect, bottom) == 12,
              "CReflectorWnd::Create reads these CRect offsets (mfc140u RVA 0x1f39e0)");
static_assert(sizeof(void*) == 8 && 0xc8 / sizeof(void*) == 25,
              "CReflectorWnd::Create dispatches vtable slot 25 (mfc140u RVA 0x1f39e0)");

// Symbol: ?SetControl@CReflectorWnd@@QEAAXPEAVCOleControl@@@Z
extern "C" void MS_ABI impl__SetControl_CReflectorWnd__QEAAXPEAVCOleControl___Z(void* pThis, void* pControl) {
    if (pThis == nullptr) {
        return;
    }

    auto* newControl = static_cast<COleControl*>(pControl);
    auto* reflector = static_cast<CWnd*>(pThis);
    {
        std::lock_guard<std::mutex> lock(g_reflectorControlMutex);
        auto it = g_reflectorControls_ManualSmallStubImplementations.find(pThis);
        if (it != g_reflectorControls_ManualSmallStubImplementations.end()) {
            COleControl* priorControl = it->second;
            if (priorControl != nullptr) {
                priorControl->m_pReflect = nullptr;
            }
            if (newControl == nullptr) {
                g_reflectorControls_ManualSmallStubImplementations.erase(it);
            } else {
                it->second = newControl;
            }
        } else if (newControl != nullptr) {
            g_reflectorControls_ManualSmallStubImplementations.emplace(pThis, newControl);
        }

        for (auto& entry : g_reflectorControls_ManualSmallStubImplementations) {
            if (entry.first != pThis && entry.second == newControl) {
                entry.second = nullptr;
            }
        }
    }

    if (newControl != nullptr) {
        newControl->m_pReflect = reflector;
    }
}

// Symbol: ?PostNcDestroy@CReflectorWnd@@MEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CReflectorWnd__MEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(g_reflectorControlMutex);
        auto it = g_reflectorControls_ManualSmallStubImplementations.find(pThis);
        if (it != g_reflectorControls_ManualSmallStubImplementations.end()) {
            if (it->second) {
                it->second->m_pReflect = nullptr;
            }
            g_reflectorControls_ManualSmallStubImplementations.erase(it);
        }
    }

    static_cast<CWnd*>(pThis)->PostNcDestroy();
}

// Symbol: ?WindowProc@CReflectorWnd@@MEAA_JI_K_J@Z
extern "C" std::intptr_t MS_ABI impl__WindowProc_CReflectorWnd__MEAA_JI_K_J_Z(
    void* pThis, unsigned int message, std::uintptr_t wParam, std::intptr_t lParam) {
    if (!pThis) {
        return 0;
    }
    return static_cast<CWnd*>(pThis)->WindowProc(message, wParam, lParam);
}
// Symbol: ?Create@CReflectorWnd@@QEAAHAEBVCRect@@PEAUHWND__@@@Z
// Retail entry RVA 0x1f39e0 (mfc140u) first calls AfxEndDeferRegisterClass(0x20)
// (entry RVA 0x2918f0 (mfc140u)), ignoring its result, then calls through
// vtable byte offset 0xc8 (slot 25) with the CreateEx coordinate arguments
// (same slot/signature used by CWnd::Create at entry RVA 0x28b690 (mfc140u)),
// class name AfxOleControl140u (UTF-16 string at RVA 0x34b770 (mfc140u)),
// style 0x54000000, the rectangle edges/dimensions, parent HWND,
// and null menu/parameter. OpenMFC's AfxEndDeferRegisterClass only registers
// its generic AfxWnd class, not AfxOleControl140u, so the retail class is not
// reliably available to CreateWindowEx. Use a null class so CWnd's
// PreCreateWindow selects the registered OpenMFC class instead. This preserves
// usable window creation but intentionally differs from retail's class identity.
extern "C" int MS_ABI impl__Create_CReflectorWnd__QEAAHAEBVCRect__PEAUHWND_____Z(
    void* pThis, const CRect& rect, HWND parent) {
    impl__AfxEndDeferRegisterClass__YAHJ_Z(0x20);

    using CreateExFn = BOOL (MS_ABI *)(
        void*, DWORD, const wchar_t*, const wchar_t*, DWORD,
        int, int, int, int, HWND, HMENU, void*);
    auto** vtable = *reinterpret_cast<void***>(pThis);
    auto createEx = reinterpret_cast<CreateExFn>(vtable[0xc8 / sizeof(void*)]);
    const int width = static_cast<int>(static_cast<unsigned int>(rect.right) -
                                       static_cast<unsigned int>(rect.left));
    const int height = static_cast<int>(static_cast<unsigned int>(rect.bottom) -
                                        static_cast<unsigned int>(rect.top));
    return createEx(pThis, 0, nullptr, nullptr, 0x54000000,
                    rect.left, rect.top, width, height, parent, nullptr, nullptr);
}

