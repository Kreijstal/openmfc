#pragma once
#include "detail/TypeLibCacheSupport.h"
namespace typelib_cache_tests {
constexpr GUID fixtureIID={0x872e528a,0xc2f2,0x4e8b,{0x95,0x2d,0x13,0x5b,0x91,0x0c,0x6e,0x12}};
constexpr GUID secondIID={0x872e528b,0xc2f2,0x4e8b,{0x95,0x2d,0x13,0x5b,0x91,0x0c,0x6e,0x12}};
constexpr GUID missingIID={0x872e528c,0xc2f2,0x4e8b,{0x95,0x2d,0x13,0x5b,0x91,0x0c,0x6e,0x12}};
// ABI-shaped COM probes implement only the cache's actual IUnknown calls and
// ITypeLib slot 6. Every unrelated interface method aborts if reached.
struct Info {
    void** table;
    ULONG references=1;
    static HRESULT STDMETHODCALLTYPE Query(void* self,REFIID iid,void** out) {
        if(!out)return E_POINTER;*out=nullptr;
        if(iid!=IID_IUnknown && iid!=IID_ITypeInfo)return E_NOINTERFACE;
        *out=self;Add(self);return S_OK;
    }
    static ULONG STDMETHODCALLTYPE Add(void* self) { return ++static_cast<Info*>(self)->references; }
    static ULONG STDMETHODCALLTYPE Release(void* self) { return --static_cast<Info*>(self)->references; }
    static void Forbidden() { std::abort(); }
    Info() {
        static void* slots[22];static bool initialized=false;
        if(!initialized) {
            for(auto& slot:slots)slot=reinterpret_cast<void*>(Forbidden);
            slots[0]=reinterpret_cast<void*>(Query);slots[1]=reinterpret_cast<void*>(Add);slots[2]=reinterpret_cast<void*>(Release);initialized=true;
        }
        table=slots;
    }
    ITypeInfo* Interface() { return reinterpret_cast<ITypeInfo*>(this); }
};
struct Library {
    void** table;
    ULONG references=1,queries=0;
    Info first,second;
    bool throwQuery=false,locked=false;
    static HRESULT STDMETHODCALLTYPE Query(void* self,REFIID iid,void** out) {
        if(!out)return E_POINTER;*out=nullptr;
        if(iid!=IID_IUnknown && iid!=IID_ITypeLib)return E_NOINTERFACE;
        *out=self;Add(self);return S_OK;
    }
    static ULONG STDMETHODCALLTYPE Add(void* self) { return ++static_cast<Library*>(self)->references; }
    static ULONG STDMETHODCALLTYPE Release(void* self) { return --static_cast<Library*>(self)->references; }
    static HRESULT STDMETHODCALLTYPE GetInfo(void* self,REFGUID iid,ITypeInfo** out) {
        auto* library=static_cast<Library*>(self);++library->queries;library->locked=g_typeCacheLockDepth>0;*out=nullptr;
        if(library->throwQuery)throw 47;
        Info* info=iid==fixtureIID ? &library->first : iid==secondIID ? &library->second : nullptr;
        if(!info)return TYPE_E_ELEMENTNOTFOUND;
        *out=info->Interface();(*out)->AddRef();return S_OK;
    }
    Library() {
        static void* slots[13];static bool initialized=false;
        if(!initialized) {
            for(auto& slot:slots)slot=reinterpret_cast<void*>(Info::Forbidden);
            slots[0]=reinterpret_cast<void*>(Query);slots[1]=reinterpret_cast<void*>(Add);slots[2]=reinterpret_cast<void*>(Release);
            slots[6]=reinterpret_cast<void*>(GetInfo);initialized=true;
        }
        table=slots;
    }
    ITypeLib* Interface() { return reinterpret_cast<ITypeLib*>(this); }
};
class Target : public CCmdTarget {
public:
    CTypeLibCache cache;
    Library* library=nullptr;
    int loads=0;
    LCID lastLocale=0;
    AFX_MODULE_STATE* callbackModule=nullptr;
    CTypeLibCache* GetTypeLibCache() override { return &cache; }
    BOOL GetDispatchIID(IID* out) override { *out=fixtureIID;return TRUE; }
    UINT GetTypeInfoCount() override { return 1; }
    HRESULT GetTypeLib(LCID locale,ITypeLib** out) override {
        ++loads;lastLocale=locale;callbackModule=g_cmdTestCurrentModule;
        *out=library ? library->Interface() : nullptr;
        if(*out) { (*out)->AddRef();return S_OK; }
        return TYPE_E_CANTLOADLIBRARY;
    }
};
static HRESULT lookup(Target& target,LCID locale,const GUID& iid,ITypeInfo** out) {
    return impl__GetTypeInfoOfGuid_CCmdTarget__QEAAJKAEBU_GUID__PEAPEAUITypeInfo___Z(&target,locale,&iid,reinterpret_cast<void**>(out));
}
static void cacheStorage() {
    Library library;CTypeLibCache cache;
    impl__Lock_CTypeLibCache__QEAAXXZ(&cache);impl__Lock_CTypeLibCache__QEAAXXZ(&cache);
    check(sizeof(cache)==56 && cache.m_cRef==2 && cache.m_lcid==static_cast<LCID>(-1),
        "type-library cache exposes native 56-byte storage and counts nested users");
    impl__Cache_CTypeLibCache__QEAAXKPEAUITypeLib___Z(&cache,0x409,library.Interface());
    impl__CacheTypeInfo_CTypeLibCache__QEAAXKAEBU_GUID__PEAUITypeInfo___Z(&cache,0x409,&fixtureIID,library.first.Interface());
    check(cache.m_lcid==0x409 && cache.m_ptlib==library.Interface() && cache.m_ptinfo==library.first.Interface() &&
        cache.m_guidInfo==fixtureIID && library.references==2 && library.first.references==2,
        "cache writes actual locale, library, GUID and type-info fields with retained COM references");
    ITypeLib* foundLibrary=nullptr;ITypeInfo* foundInfo=nullptr;
    check(impl__Lookup_CTypeLibCache__QEAAHKPEAPEAUITypeLib___Z(&cache,0x409,&foundLibrary) &&
        impl__LookupTypeInfo_CTypeLibCache__QEAAHKAEBU_GUID__PEAPEAUITypeInfo___Z(&cache,0x409,&fixtureIID,&foundInfo) &&
        foundLibrary==library.Interface() && foundInfo==library.first.Interface() && library.references==3 && library.first.references==3,
        "native cache hits return separately AddRef'd COM outputs");
    if(foundLibrary)foundLibrary->Release();if(foundInfo)foundInfo->Release();
    foundInfo=reinterpret_cast<ITypeInfo*>(1);
    check(!impl__LookupTypeInfo_CTypeLibCache__QEAAHKAEBU_GUID__PEAPEAUITypeInfo___Z(&cache,0x411,&fixtureIID,&foundInfo) && !foundInfo,
        "cache lookups reject a different locale and clear failed outputs");
    impl__CacheTypeInfo_CTypeLibCache__QEAAXKAEBU_GUID__PEAUITypeInfo___Z(&cache,0x411,&secondIID,library.second.Interface());
    impl__Cache_CTypeLibCache__QEAAXKPEAUITypeLib___Z(&cache,0x409,nullptr);
    check(cache.m_ptinfo==library.first.Interface() && library.second.references==1,
        "mismatched type-info inserts and null library inserts preserve the native cache");
    impl__Cache_CTypeLibCache__QEAAXKPEAUITypeLib___Z(&cache,0x411,library.Interface());
    check(!cache.m_ptinfo && cache.m_guidInfo==GUID_NULL && library.first.references==1 && library.references==2,
        "replacing a cached library invalidates prior type information and balances old references");
    impl__Unlock_CTypeLibCache__QEAAXXZ(&cache);
    check(cache.m_cRef==1 && cache.m_ptlib==library.Interface(),"nested cache unlocks preserve data until the last user leaves");
    impl__Unlock_CTypeLibCache__QEAAXXZ(&cache);
    check(cache.m_cRef==0 && !cache.m_ptlib && !cache.m_ptinfo && library.references==1,
        "last cache unlock releases the native library and type-info slots");
}
static void targetLookup(AFX_MODULE_STATE* owner,AFX_MODULE_STATE* caller) {
    Library library;g_cmdTestCurrentModule=owner;Target target;target.library=&library;
    impl__Lock_CTypeLibCache__QEAAXXZ(&target.cache);g_cmdTestCurrentModule=caller;
    ITypeInfo* info=nullptr;
    check(lookup(target,0x409,fixtureIID,&info)==S_OK && info==library.first.Interface() && target.loads==1 && library.queries==1 &&
        library.locked && target.callbackModule==caller && g_typeCacheLockDepth==0 && g_cmdTestCurrentModule==caller,
        "GetTypeInfoOfGuid loads the client's virtual library under CRIT_TYPELIBCACHE and preserves native callback context");
    if(info)info->Release();
    check(lookup(target,0x409,fixtureIID,&info)==S_OK && target.loads==1 && library.queries==1 && library.first.references==3,
        "repeat type-information requests hit the cache without invoking library callbacks");
    if(info)info->Release();
    check(lookup(target,0x409,secondIID,&info)==S_OK && target.loads==1 && library.queries==2 && library.first.references==1,
        "new GUID requests reuse the cached library and replace the single native type-info slot");
    if(info)info->Release();
    check(lookup(target,0x411,fixtureIID,&info)==S_OK && target.loads==2 && target.lastLocale==0x411 && library.second.references==1,
        "locale changes reload the virtual library and invalidate prior GUID information");
    if(info)info->Release();
    check(lookup(target,0x411,missingIID,&info)==TYPE_E_ELEMENTNOTFOUND && !info && target.cache.m_ptinfo==library.first.Interface(),
        "failed GUID queries preserve HRESULT and leave the previous type-info slot valid");
    library.throwQuery=true;bool caught=false;
    try { lookup(target,0x411,secondIID,&info); } catch(int code) { caught=code==47; }
    check(caught && library.references==2 && g_typeCacheLockDepth==0 && g_cmdTestCurrentModule==caller,
        "callback exception unwinding releases temporary libraries and the recursive global lock");
    library.throwQuery=false;impl__EnableAutomation_CCmdTarget__QEAAXXZ(&target);
    auto* dispatch=reinterpret_cast<IDispatch*>(&target.m_xDispatch);
    check(dispatch->GetTypeInfo(0,0x411,&info)==S_OK && info==library.first.Interface() && target.loads==2,
        "public IDispatch GetTypeInfo uses the same native GUID cache");
    if(info)info->Release();
    check(dispatch->GetTypeInfo(1,0x411,&info)==DISP_E_BADINDEX && !info,"dispatch type information rejects unsupported indexes");
    impl__Unlock_CTypeLibCache__QEAAXXZ(&target.cache);
    check(library.references==1 && library.first.references==1 && library.second.references==1,
        "target cache cleanup balances all provider and result references");
}
static void embeddedLookup(AFX_MODULE_STATE* owner,AFX_MODULE_STATE* caller) {
    g_cmdTestCurrentModule=owner;Target target;impl__Lock_CTypeLibCache__QEAAXXZ(&target.cache);
    g_cmdTestCurrentModule=caller;g_typeCacheInstanceModule=nullptr;
    ITypeInfo* info=nullptr;TYPEATTR* attributes=nullptr;
    HRESULT hr=lookup(target,0x409,fixtureIID,&info);
    if(info)info->GetTypeAttr(&attributes);
    check(hr==S_OK && attributes && attributes->guid==fixtureIID && target.cache.m_ptlib && target.cache.m_ptinfo==info &&
        g_typeCacheInstanceModule==owner && g_cmdTestCurrentModule==caller && g_typeCacheLockDepth==0,
        "failed virtual providers fall back to a real embedded type library in the owning module");
    if(attributes)info->ReleaseTypeAttr(attributes);if(info)info->Release();
    check(lookup(target,0x409,fixtureIID,&info)==S_OK && target.loads==1,
        "embedded library results are cached for subsequent requests");
    if(info)info->Release();impl__Unlock_CTypeLibCache__QEAAXXZ(&target.cache);
    impl__Lock_CTypeLibCache__QEAAXXZ(&target.cache);g_typeCacheForceNoInstance=true;
    check(lookup(target,0x409,fixtureIID,&info)==TYPE_E_CANTLOADLIBRARY && !info && !target.cache.m_ptlib && g_typeCacheLockDepth==0,
        "missing module handles return the native load failure without populating the cache");
    g_typeCacheForceNoInstance=false;impl__Unlock_CTypeLibCache__QEAAXXZ(&target.cache);
}
}
static void typeLibCache() {
    typelib_cache_tests::cacheStorage();
    unsigned char ownerBytes[8]{},callerBytes[8]{};
    auto* owner=reinterpret_cast<AFX_MODULE_STATE*>(ownerBytes);auto* caller=reinterpret_cast<AFX_MODULE_STATE*>(callerBytes);
    typelib_cache_tests::targetLookup(owner,caller);typelib_cache_tests::embeddedLookup(owner,caller);
    g_cmdTestCurrentModule=nullptr;
}
