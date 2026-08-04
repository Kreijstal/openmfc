// CTypeLibCache — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ?Cache@CTypeLibCache@@QEAAXKPEAUITypeLib@@@Z
extern "C" void MS_ABI impl__Cache_CTypeLibCache__QEAAXKPEAUITypeLib___Z(void* pThis, unsigned long lcid, ITypeLib* pTypeLib) {
    std::lock_guard<std::mutex> lock(g_typeLibCacheMutex);
    auto& slot = g_typeLibCaches[pThis].typeLibs[lcid];
    if (slot) slot->Release();
    slot = pTypeLib;
    if (slot) slot->AddRef();
}
// Symbol: ?CacheTypeInfo@CTypeLibCache@@QEAAXKAEBU_GUID@@PEAUITypeInfo@@@Z
extern "C" void MS_ABI impl__CacheTypeInfo_CTypeLibCache__QEAAXKAEBU_GUID__PEAUITypeInfo___Z(
    void* pThis, unsigned long lcid, const GUID* iid, ITypeInfo* pTypeInfo) {
    if (!iid) return;
    std::lock_guard<std::mutex> lock(g_typeLibCacheMutex);
    auto& slot = g_typeLibCaches[pThis].typeInfos[TypeInfoCacheKey(lcid, *iid)];
    if (slot) slot->Release();
    slot = pTypeInfo;
    if (slot) slot->AddRef();
}
// Symbol: ?Lock@CTypeLibCache@@QEAAXXZ
extern "C" void MS_ABI impl__Lock_CTypeLibCache__QEAAXXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_typeLibCacheMutex);
    g_typeLibCaches[pThis].locked = true;
}
// Symbol: ?Lookup@CTypeLibCache@@QEAAHKPEAPEAUITypeLib@@@Z
extern "C" int MS_ABI impl__Lookup_CTypeLibCache__QEAAHKPEAPEAUITypeLib___Z(void* pThis, unsigned long lcid, ITypeLib** ppTypeLib) {
    if (!ppTypeLib) return FALSE;
    *ppTypeLib = nullptr;
    std::lock_guard<std::mutex> lock(g_typeLibCacheMutex);
    auto cache = g_typeLibCaches.find(pThis);
    if (cache == g_typeLibCaches.end()) return FALSE;
    auto it = cache->second.typeLibs.find(lcid);
    if (it == cache->second.typeLibs.end() || !it->second) return FALSE;
    *ppTypeLib = it->second;
    (*ppTypeLib)->AddRef();
    return TRUE;
}
// Symbol: ?LookupTypeInfo@CTypeLibCache@@QEAAHKAEBU_GUID@@PEAPEAUITypeInfo@@@Z
extern "C" int MS_ABI impl__LookupTypeInfo_CTypeLibCache__QEAAHKAEBU_GUID__PEAPEAUITypeInfo___Z(
    void* pThis, unsigned long lcid, const GUID* iid, ITypeInfo** ppTypeInfo) {
    if (!iid || !ppTypeInfo) return FALSE;
    *ppTypeInfo = nullptr;
    std::lock_guard<std::mutex> lock(g_typeLibCacheMutex);
    auto cache = g_typeLibCaches.find(pThis);
    if (cache == g_typeLibCaches.end()) return FALSE;
    auto it = cache->second.typeInfos.find(TypeInfoCacheKey(lcid, *iid));
    if (it == cache->second.typeInfos.end() || !it->second) return FALSE;
    *ppTypeInfo = it->second;
    (*ppTypeInfo)->AddRef();
    return TRUE;
}
// Symbol: ?Unlock@CTypeLibCache@@QEAAXXZ
extern "C" void MS_ABI impl__Unlock_CTypeLibCache__QEAAXXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_typeLibCacheMutex);
    g_typeLibCaches[pThis].locked = false;
}
