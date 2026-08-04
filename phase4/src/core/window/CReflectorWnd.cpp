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
