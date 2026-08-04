// CInnerUnknown — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?AddRef@CInnerUnknown@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_CInnerUnknown__UEAAKXZ(void* pThis) {
    if (pThis == nullptr) {
        return 0;
    }

    std::lock_guard<std::mutex> lock(g_innerUnknownMutex);
    auto& count = g_innerUnknownRefCounts[pThis];
    count = (count == 0) ? 1 : count + 1;
    return count;
}
// Symbol: ?QueryInterface@CInnerUnknown@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__QueryInterface_CInnerUnknown__UEAAJAEBU_GUID__PEAPEAX_Z(
    void* pThis, const void* riid, void** ppObject) {
    if (ppObject == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    *ppObject = nullptr;
    if (pThis == nullptr || riid == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    const GUID* requested = static_cast<const GUID*>(riid);
    if (IsEqualIID(*requested, IID_IUnknown) || IsEqualIID(*requested, IID_IClassFactory)) {
        *ppObject = pThis;
        impl__AddRef_CInnerUnknown__UEAAKXZ(pThis);
        return S_OK;
    }
    return static_cast<long>(E_NOINTERFACE);
}
// Symbol: ?Release@CInnerUnknown@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_CInnerUnknown__UEAAKXZ(void* pThis) {
    if (pThis == nullptr) {
        return 0;
    }

    std::lock_guard<std::mutex> lock(g_innerUnknownMutex);
    auto it = g_innerUnknownRefCounts.find(pThis);
    if (it == g_innerUnknownRefCounts.end()) {
        return 0;
    }

    unsigned long& count = it->second;
    if (count > 0) {
        --count;
    }
    const unsigned long remaining = count;
    if (remaining == 0) {
        g_innerUnknownRefCounts.erase(it);
    }
    return remaining;
}
