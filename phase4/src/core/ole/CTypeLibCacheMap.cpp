// CTypeLibCacheMap — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ?RemoveAll@CTypeLibCacheMap@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__RemoveAll_CTypeLibCacheMap__UEAAXPEAX_Z(void* pThis, void* pExcept) {
    (void)pThis;
    std::vector<std::unique_ptr<TypeLibCacheHandle>> retired;
    {
        std::lock_guard<std::mutex> lock(g_typeLibCacheMutex);
        for(auto it=g_guidTypeLibCaches.begin();it!=g_guidTypeLibCaches.end();) {
            CTypeLibCache* cache=&it->second->cache;
            if(cache==pExcept || cache->m_cRef!=0) { ++it;continue; }
            retired.push_back(std::move(it->second));it=g_guidTypeLibCaches.erase(it);
        }
    }
    // COM Release callbacks run after the registry mutex is unlocked.
}
