#pragma once
#include "openmfc/afxstr.h"
#include <mbstring.h>

namespace openmfc_string_ops {
template<class C> const C* Offset(const C* text,int characters) {
    return reinterpret_cast<const C*>(uintptr_t(text)+size_t(int64_t(characters))*sizeof(C));
}
template<class T> T Left(const T& text,int count) {
    if(count<0)count=0;
    if(count>=text.GetLength())return T(text);
    auto* manager=text.GetManager();
    return T(text.GetString(),count,manager);
}
template<class T> T Right(const T& text,int count) {
    if(count<0)count=0;
    int length=text.GetLength();
    if(count>=length)return T(text);
    auto* manager=text.GetManager();
    return T(Offset(text.GetString(),length-count),count,manager);
}
template<class T> T Mid(const T& text,int first,int count) {
    if(first<0)first=0;
    if(count<0)count=0;
    if(count>INT32_MAX-first)AfxThrowOleException(static_cast<long>(0x80070216u));
    int length=text.GetLength();
    if(first+count>length)count=length-first;
    if(first>length)count=0;
    if(first==0 && count==length)return T(text);
    auto* manager=text.GetManager();
    return T(Offset(text.GetString(),first),count,manager);
}
template<class T> T Mid(const T& text,int first) {
    return Mid(text,first,int32_t(uint32_t(text.GetLength())-uint32_t(first)));
}
inline size_t Span(const wchar_t* text,const wchar_t* set,bool including) {
    return including?wcsspn(text,set):wcscspn(text,set);
}
inline size_t Span(const char* text,const char* set,bool including) {
    auto* source=reinterpret_cast<const unsigned char*>(text);
    auto* delimiters=reinterpret_cast<const unsigned char*>(set);
    return including?_mbsspn(source,delimiters):_mbscspn(source,delimiters);
}
template<class T> T Span(const T& text,const typename T::XCHAR* set,bool including) {
    if(!set)AfxThrowOleException(static_cast<long>(0x80070057u));
    return Left(text,int(Span(text.GetString(),set,including)));
}
template<class T> T Tokenize(const T& text,const typename T::XCHAR* delimiters,int& start) {
    if(start<0)AfxThrowOleException(static_cast<long>(0x80070057u));
    if(delimiters && *delimiters) {
        auto* begin=Offset(text.GetString(),start);
        auto* end=Offset(text.GetString(),text.GetLength());
        if(uintptr_t(begin)<uintptr_t(end)) {
            int skipped=int(Span(begin,delimiters,true));
            auto* token=Offset(begin,skipped);
            if(uintptr_t(token)<uintptr_t(end)) {
                int count=int(Span(token,delimiters,false));
                int first=int32_t(uint32_t(start)+uint32_t(skipped));
                start=int32_t(uint32_t(first)+uint32_t(count)+1u);
                return Mid(text,first,count);
            }
        }
    } else if(start<text.GetLength()) {
        auto* manager=text.GetManager();
        return T(Offset(text.GetString(),start),manager);
    }
    start=-1;
    return T(text.GetManager());
}
} // namespace openmfc_string_ops
