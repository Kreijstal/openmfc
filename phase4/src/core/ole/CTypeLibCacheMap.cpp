// CTypeLibCacheMap — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ?RemoveAll@CTypeLibCacheMap@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__RemoveAll_CTypeLibCacheMap__UEAAXPEAX_Z(void* pThis, void* pExcept) {
    std::lock_guard<std::mutex> lock(g_typeLibCacheMutex);
    for (auto it = g_typeLibCaches.begin(); it != g_typeLibCaches.end();) {
        if (it->first == pExcept || it->second.locked) {
            ++it;
        } else {
            ReleaseTypeLibCache(it->second);
            it = g_typeLibCaches.erase(it);
        }
    }
    if (pThis && !pExcept) {
        auto it = g_typeLibCaches.find(pThis);
        if (it != g_typeLibCaches.end()) {
            ReleaseTypeLibCache(it->second);
            g_typeLibCaches.erase(it);
        }
    }
}
