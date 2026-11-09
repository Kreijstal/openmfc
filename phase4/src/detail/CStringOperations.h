#pragma once
#include "openmfc/afx.h"
#include <mbstring.h>

namespace openmfc_string_ops {
inline const unsigned char* Bytes(const char* text) {return reinterpret_cast<const unsigned char*>(text);}
inline int Compare(const wchar_t* first,const wchar_t* second,int mode) {
    switch(mode) {
    case 0:return wcscmp(first,second);
    case 1:return _wcsicmp(first,second);
    case 2:return wcscoll(first,second);
    default:return _wcsicoll(first,second);
    }
}
inline int Compare(const char* first,const char* second,int mode) {
    switch(mode) {
    case 0:return _mbscmp(Bytes(first),Bytes(second));
    case 1:return _mbsicmp(Bytes(first),Bytes(second));
    case 2:return _mbscoll(Bytes(first),Bytes(second));
    default:return _mbsicoll(Bytes(first),Bytes(second));
    }
}
template<class T> int Compare(const T& text,const typename T::XCHAR* other,int mode) {
    if(mode<2 && !other)AfxThrowOleException(static_cast<long>(0x80004005u));
    return Compare(text.GetString(),other,mode);
}
inline const wchar_t* FindChar(const wchar_t* text,wchar_t value,bool reverse) {return reverse?wcsrchr(text,value):wcschr(text,value);}
inline const char* FindChar(const char* text,char value,bool reverse) {
    auto ch=static_cast<unsigned char>(value);
    return reinterpret_cast<const char*>(reverse?_mbsrchr(Bytes(text),ch):_mbschr(Bytes(text),ch));
}
inline const wchar_t* FindText(const wchar_t* text,const wchar_t* part) {return wcsstr(text,part);}
inline const char* FindText(const char* text,const char* part) {return reinterpret_cast<const char*>(_mbsstr(Bytes(text),Bytes(part)));}
inline const wchar_t* FindSet(const wchar_t* text,const wchar_t* set) {return wcspbrk(text,set);}
inline const char* FindSet(const char* text,const char* set) {return reinterpret_cast<const char*>(_mbspbrk(Bytes(text),Bytes(set)));}
template<class T> int Index(const T& text,const typename T::XCHAR* found) {
    if(!found)return -1;
    intptr_t delta=intptr_t(uintptr_t(found)-uintptr_t(text.GetString()));
    if constexpr(sizeof(typename T::XCHAR)==2)delta>>=1;
    return int32_t(delta);
}
template<class C> const C* Offset(const C* text,int characters) {
    return reinterpret_cast<const C*>(uintptr_t(text)+size_t(int64_t(characters))*sizeof(C));
}
template<class T> int Find(const T& text,typename T::XCHAR value,int start) {
    if(start<0 || start>=text.GetLength())return -1;
    auto* found=FindChar(Offset(text.GetString(),start),value,false);
    return Index(text,found);
}
template<class T> int Find(const T& text,const typename T::XCHAR* part,int start) {
    if(!part || start<0 || start>text.GetLength())return -1;
    auto* found=FindText(Offset(text.GetString(),start),part);
    return Index(text,found);
}
template<class T> int ReverseFind(const T& text,typename T::XCHAR value) {
    auto* found=FindChar(text.GetString(),value,true);return Index(text,found);
}
template<class T> int FindOneOf(const T& text,const typename T::XCHAR* set) {
    auto* found=FindSet(text.GetString(),set);return Index(text,found);
}
inline int ChangeCase(wchar_t* text,size_t size,int mode) {
    if(mode==2){_wcsrev(text);return 0;}
    return mode?_wcsupr_s(text,size):_wcslwr_s(text,size);
}
inline int ChangeCase(char* text,size_t size,int mode) {
    if(mode==2){_mbsrev(reinterpret_cast<unsigned char*>(text));return 0;}
    auto* bytes=reinterpret_cast<unsigned char*>(text);
    return mode?_mbsupr_s(bytes,size):_mbslwr_s(bytes,size);
}
template<class T> T& ChangeCase(T& text,int mode) {
    int length=text.GetLength();
    auto* buffer=text.GetBuffer(length);
    int error=ChangeCase(buffer,size_t(int64_t(int32_t(uint32_t(length)+1u))),mode);
    if constexpr(sizeof(typename T::XCHAR)==1) {
        if(error && error!=80) {
            if(error==12)AfxThrowMemoryException();
            else AfxThrowInvalidArgException();
        }
    }
    text.SetLength(length);return text;
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
