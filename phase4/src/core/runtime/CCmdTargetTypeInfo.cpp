// Native dispatch type-information lookup and embedded module-library fallback.
#define OPENMFC_APPCORE_IMPL
#include "detail/TypeLibCacheSupport.h"
#include <vector>
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int);
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int);
namespace {
struct CacheScope {
    CacheScope() { impl__AfxLockGlobals__YAXH_Z(13); }
    ~CacheScope() { impl__AfxUnlockGlobals__YAXH_Z(13); }
};
struct LibraryReference {
    ITypeLib* value=nullptr;
    ~LibraryReference() { if(value)value->Release(); }
};
HRESULT LoadModuleLibrary(CCmdTarget* target,ITypeLib** out) {
    openmfc::detail::CmdTargetModuleScope module(target->m_pModuleState);
    HINSTANCE instance=AfxGetInstanceHandle();
    if(!instance)return TYPE_E_CANTLOADLIBRARY;
    std::vector<wchar_t> path(MAX_PATH);
    for(;;) {
        DWORD length=GetModuleFileNameW(instance,path.data(),static_cast<DWORD>(path.size()));
        if(!length)return TYPE_E_CANTLOADLIBRARY;
        if(length<path.size())return LoadTypeLib(path.data(),out);
        if(path.size()>=32768)return TYPE_E_CANTLOADLIBRARY;
        path.resize(path.size()*2);
    }
}
}
// Symbol: ?GetTypeInfoOfGuid@CCmdTarget@@QEAAJKAEBU_GUID@@PEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfoOfGuid_CCmdTarget__QEAAJKAEBU_GUID__PEAPEAUITypeInfo___Z(
    CCmdTarget* target,unsigned long locale,const void* rawIID,void** rawOut) {
    if(!target || !rawIID || !rawOut)return E_POINTER;
    auto* out=reinterpret_cast<ITypeInfo**>(rawOut);*out=nullptr;
    const GUID& iid=*static_cast<const GUID*>(rawIID);
    CacheScope locked;
    CTypeLibCache* cache=target->GetTypeLibCache();
    if(!cache)return TYPE_E_CANTLOADLIBRARY;
    if(impl__LookupTypeInfo_CTypeLibCache__QEAAHKAEBU_GUID__PEAPEAUITypeInfo___Z(cache,locale,&iid,out))return S_OK;
    LibraryReference library;
    if(!impl__Lookup_CTypeLibCache__QEAAHKPEAPEAUITypeLib___Z(cache,locale,&library.value)) {
        HRESULT hr=target->GetTypeLib(locale,&library.value);
        if(FAILED(hr)) {
            if(library.value) { library.value->Release();library.value=nullptr; }
            hr=LoadModuleLibrary(target,&library.value);
        }
        if(FAILED(hr) || !library.value)return TYPE_E_CANTLOADLIBRARY;
        impl__Cache_CTypeLibCache__QEAAXKPEAUITypeLib___Z(cache,locale,library.value);
    }
    HRESULT hr=library.value->GetTypeInfoOfGuid(iid,out);
    if(SUCCEEDED(hr) && *out)impl__CacheTypeInfo_CTypeLibCache__QEAAXKAEBU_GUID__PEAUITypeInfo___Z(cache,locale,&iid,*out);
    return hr;
}
