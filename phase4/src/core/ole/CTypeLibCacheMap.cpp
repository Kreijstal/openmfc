// A module owns its dynamically allocated type-library cache entries.
#define OPENMFC_APPCORE_IMPL
#include "detail/TypeLibCacheSupport.h"

void CTypeLibCacheMap::RemoveAll(void* except) {
    void* savedKey=nullptr;
    void* savedValue=nullptr;
    for(auto position=GetStartPosition();position;) {
        void* key;
        void* value;
        GetNextAssoc(position,key,value);
        if(value==except) {
            savedKey=key;
            savedValue=value;
        } else {
            delete static_cast<CTypeLibCache*>(value);
        }
    }
    CMapPtrToPtr::RemoveAll();
    if(except && savedKey)SetAt(savedKey,savedValue);
}

// Symbol: ?RemoveAll@CTypeLibCacheMap@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__RemoveAll_CTypeLibCacheMap__UEAAXPEAX_Z(CTypeLibCacheMap* self,void* except) {
    self->CTypeLibCacheMap::RemoveAll(except);
}
