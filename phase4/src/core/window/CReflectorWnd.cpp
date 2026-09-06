// CReflectorWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

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

// Symbol: ?Create@CReflectorWnd@@QEAAHAEBVCRect@@PEAUHWND__@@Z
extern "C" int MS_ABI impl__Create_CReflectorWnd__QEAAHAEBVCRect__PEAUHWND____Z(
    void* pThis, const void* pRect, void* pParentHwnd) {
    if (!pThis || !pRect) {
        return FALSE;
    }

    CWnd* pWnd = static_cast<CWnd*>(pThis);
    const RECT& rect = *static_cast<const RECT*>(pRect);
    HWND parentHandle = pParentHwnd ? *static_cast<HWND*>(pParentHwnd) : nullptr;
    CWnd* pParent = parentHandle ? CWnd::FromHandle(parentHandle) : nullptr;
    const DWORD style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    return pWnd->Create(nullptr, nullptr, style, rect, pParent, 0, nullptr) ? TRUE : FALSE;
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
extern "C" int MS_ABI impl__Create_CReflectorWnd__QEAAHAEBVCRect__PEAUHWND_____Z(const void* /*class*/* p0, void* /*struct*/* p1) {
    return 0;
}

