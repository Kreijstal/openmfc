// Native CTypeLibCache storage. Callers serialize updates with CRIT_TYPELIBCACHE.
#define OPENMFC_APPCORE_IMPL
#include "detail/TypeLibCacheSupport.h"
namespace {
template<class Interface> void Release(Interface*& value) {
    auto* previous=value;value=nullptr;if(previous)previous->Release();
}
}
void OpenMfcClearTypeLibCache(CTypeLibCache* cache) {
    if(!cache)return;
    Release(cache->m_ptinfo);Release(cache->m_ptlib);
}
// Symbol: ?Cache@CTypeLibCache@@QEAAXKPEAUITypeLib@@@Z
extern "C" void MS_ABI impl__Cache_CTypeLibCache__QEAAXKPEAUITypeLib___Z(void* self,unsigned long locale,ITypeLib* library) {
    if(!self || !library)return;
    auto* cache=static_cast<CTypeLibCache*>(self);
    cache->m_lcid=locale;cache->m_guidInfo=GUID_NULL;
    OpenMfcClearTypeLibCache(cache);
    cache->m_ptlib=library;library->AddRef();
}
// Symbol: ?CacheTypeInfo@CTypeLibCache@@QEAAXKAEBU_GUID@@PEAUITypeInfo@@@Z
extern "C" void MS_ABI impl__CacheTypeInfo_CTypeLibCache__QEAAXKAEBU_GUID__PEAUITypeInfo___Z(
    void* self,unsigned long locale,const GUID* iid,ITypeInfo* info) {
    if(!self || !iid || !info)return;
    auto* cache=static_cast<CTypeLibCache*>(self);
    if(cache->m_lcid!=locale)return;
    cache->m_guidInfo=*iid;Release(cache->m_ptinfo);
    cache->m_ptinfo=info;info->AddRef();
}
// Symbol: ?Lock@CTypeLibCache@@QEAAXXZ
extern "C" void MS_ABI impl__Lock_CTypeLibCache__QEAAXXZ(void* self) {
    if(!self)return;
    auto* cache=static_cast<CTypeLibCache*>(self);
    if(!cache->m_cRef)cache->m_lcid=static_cast<LCID>(-1);
    InterlockedIncrement(&cache->m_cRef);
}
// Symbol: ?Unlock@CTypeLibCache@@QEAAXXZ
extern "C" void MS_ABI impl__Unlock_CTypeLibCache__QEAAXXZ(void* self) {
    if(!self)return;
    auto* cache=static_cast<CTypeLibCache*>(self);
    if(InterlockedDecrement(&cache->m_cRef)==0)OpenMfcClearTypeLibCache(cache);
}
// Symbol: ?Lookup@CTypeLibCache@@QEAAHKPEAPEAUITypeLib@@@Z
extern "C" int MS_ABI impl__Lookup_CTypeLibCache__QEAAHKPEAPEAUITypeLib___Z(void* self,unsigned long locale,ITypeLib** out) {
    if(!out)return FALSE;*out=nullptr;
    auto* cache=static_cast<CTypeLibCache*>(self);
    if(!cache || cache->m_lcid==static_cast<LCID>(-1) || cache->m_lcid!=locale || !cache->m_ptlib)return FALSE;
    *out=cache->m_ptlib;(*out)->AddRef();return TRUE;
}
// Symbol: ?LookupTypeInfo@CTypeLibCache@@QEAAHKAEBU_GUID@@PEAPEAUITypeInfo@@@Z
extern "C" int MS_ABI impl__LookupTypeInfo_CTypeLibCache__QEAAHKAEBU_GUID__PEAPEAUITypeInfo___Z(
    void* self,unsigned long locale,const GUID* iid,ITypeInfo** out) {
    if(!out)return FALSE;*out=nullptr;
    auto* cache=static_cast<CTypeLibCache*>(self);
    if(!cache || !iid || cache->m_lcid==static_cast<LCID>(-1) || cache->m_lcid!=locale || !cache->m_ptinfo || cache->m_guidInfo!=*iid)return FALSE;
    *out=cache->m_ptinfo;(*out)->AddRef();return TRUE;
}
