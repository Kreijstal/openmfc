#pragma once
#include "openmfc/afxstr.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <cwchar>
// Database connection writes must preserve ATL manager and locked-buffer
// ownership. CString's convenience operators still use older simplified rules.
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
namespace openmfc { namespace detail {
class DatabaseStringBuffer {
    wchar_t** text;
    CStringData* data() const {return reinterpret_cast<CStringData*>(*text)-1;}
    static wchar_t* chars(CStringData* value) {return reinterpret_cast<wchar_t*>(value+1);}
    void prepare(int size) {
        auto* old=data();size=std::max(size,old->nDataLength);
        if(old->IsShared()) {
            auto* replacement=old->pStringMgr->Clone()->Allocate(size,sizeof(wchar_t));
            if(!replacement) {impl__AfxThrowMemoryException__YAXXZ();return;}
            std::memcpy(chars(replacement),*text,size_t(old->nDataLength+1)*sizeof(wchar_t));replacement->nDataLength=old->nDataLength;
            if(old->Release())old->pStringMgr->Free(old);*text=chars(replacement);
        } else if(size>old->nAllocLength) {
            int capacity=old->nAllocLength;
            int extra=capacity>1024*1024*1024?1024*1024:capacity/2;
            int grown=int32_t(uint32_t(capacity)+uint32_t(extra));
            auto* replacement=old->pStringMgr->Reallocate(old,std::max(size,grown),sizeof(wchar_t));
            if(!replacement) {impl__AfxThrowMemoryException__YAXXZ();return;}
            *text=chars(replacement);
        }
    }
public:
    explicit DatabaseStringBuffer(CString& value):text(reinterpret_cast<wchar_t**>(&value)) {}
    wchar_t* get(int capacity) {prepare(capacity);return *text;}
    void release(int length) {
        if(length==-1) {length=0;while(length<data()->nAllocLength && (*text)[length])++length;}
        if(length<0 || length>data()->nAllocLength) {impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);return;}
        data()->nDataLength=length;(*text)[length]=0;
    }
    void empty() {
        auto* old=data();if(!old->nDataLength)return;
        if(old->IsLocked()) {old->nDataLength=0;(*text)[0]=0;return;}
        auto* manager=old->pStringMgr;if(old->Release())manager->Free(old);*text=chars(manager->GetNilString());
    }
    void set(const wchar_t* source) {
        int length=source?int(std::wcslen(source)):0;if(!length){empty();return;}
        bool internal=UINT_PTR(source)>=UINT_PTR(*text) && UINT_PTR(source)<=UINT_PTR(*text)+size_t(data()->nDataLength)*sizeof(wchar_t);
        size_t offset=internal?size_t(source-*text):0;prepare(length);if(internal)source=*text+offset;
        std::memmove(*text,source,size_t(length)*sizeof(wchar_t));data()->nDataLength=length;(*text)[length]=0;
    }
    void append(const wchar_t* source) {
        int extra=source?int(std::wcslen(source)):0;if(!extra)return;
        int old=data()->nDataLength;int total=int32_t(uint32_t(old)+uint32_t(extra));
        if(total<old){impl__AfxThrowMemoryException__YAXXZ();return;}
        bool internal=UINT_PTR(source)>=UINT_PTR(*text) && UINT_PTR(source)<=UINT_PTR(*text)+size_t(old)*sizeof(wchar_t);
        size_t offset=internal?size_t(source-*text):0;prepare(total);if(internal)source=*text+offset;
        std::memmove(*text+old,source,size_t(extra)*sizeof(wchar_t));data()->nDataLength=total;(*text)[total]=0;
    }
};
} }
