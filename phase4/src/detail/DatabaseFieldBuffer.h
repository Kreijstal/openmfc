#pragma once
#include "openmfc/afxstr.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <climits>
#include <sqltypes.h>
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
namespace openmfc { namespace detail {
// Native GetBufferSetLength/ReleaseBuffer over either CString representation.
// The ANSI form is needed for SDK-created CStringA objects and variant values.
template<class Char> class DatabaseFieldBuffer {
    Char** text;
    CStringData* data() const {return reinterpret_cast<CStringData*>(*text)-1;}
public:
    explicit DatabaseFieldBuffer(void* object):text(static_cast<Char**>(object)) {}
    Char* get(SQLLEN length) {
        if(length>INT_MAX)impl__AfxThrowMemoryException__YAXXZ();
        int size=int32_t(length);
        if(size<0)impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);
        auto* old=data();int required=std::max(size,old->nDataLength);
        if(old->IsShared()) {
            auto* fresh=old->pStringMgr->Clone()->Allocate(required,sizeof(Char));
            if(!fresh)impl__AfxThrowMemoryException__YAXXZ();
            std::memcpy(fresh+1,*text,size_t(old->nDataLength+1)*sizeof(Char));fresh->nDataLength=old->nDataLength;
            if(old->Release())old->pStringMgr->Free(old);*text=reinterpret_cast<Char*>(fresh+1);
        } else if(required>old->nAllocLength) {
            int extra=old->nAllocLength>1024*1024*1024?1024*1024:old->nAllocLength/2;
            int grown=int32_t(uint32_t(old->nAllocLength)+uint32_t(extra));
            auto* fresh=old->pStringMgr->Reallocate(old,std::max(required,grown),sizeof(Char));
            if(!fresh)impl__AfxThrowMemoryException__YAXXZ();
            *text=reinterpret_cast<Char*>(fresh+1);
        }
        data()->nDataLength=size;(*text)[size]=0;return *text;
    }
    void release(int length) {
        if(length==-1){length=0;while(length<data()->nAllocLength && (*text)[length])++length;}
        if(length<0 || length>data()->nAllocLength)impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);
        data()->nDataLength=length;(*text)[length]=0;
    }
    void empty() {
        auto* old=data();if(!old->nDataLength)return;
        if(old->IsLocked()){release(0);return;}
        auto* manager=old->pStringMgr;if(old->Release())manager->Free(old);
        *text=reinterpret_cast<Char*>(manager->GetNilString()+1);
    }
};
} }
