#pragma once
#include "CStringOperations.h"
#include <mbctype.h>
#include <cwctype>
#include <cerrno>

namespace openmfc_string_mutation {
inline const wchar_t* Next(const wchar_t* text) {return text+1;}
inline const char* Next(const char* text) {return reinterpret_cast<const char*>(_mbsinc(openmfc_string_ops::Bytes(text)));}
template<class C> size_t Bytes(int count) {return size_t(int64_t(count))*sizeof(C);}
template<class T> int Delete(T& text,int index,int count) {
    using C=typename T::XCHAR;
    if(index<0)index=0;if(count<0)count=0;
    if(index>INT32_MAX-count)AfxThrowOleException(static_cast<long>(0x80070216u));
    int length=text.GetLength();
    if(index+count>length)count=length-index;
    if(count>0) {
        auto& base=static_cast<ATL::CSimpleStringT<C,true>&>(text);
        if(base.GetData()->IsShared())base.Fork(length);
        C* out=const_cast<C*>(text.GetString());
        memmove(out+index,out+index+count,Bytes<C>(length-index-count+1));
        text.SetLength(length-count);
    }
    return text.GetLength();
}
template<class T> int Insert(T& text,int index,typename T::XCHAR value) {
    using C=typename T::XCHAR;
    if(index<0)index=0;int length=text.GetLength();if(index>length)index=length;
    int total=int32_t(uint32_t(length)+1);
    C* out=text.GetBuffer(total);
    memmove(out+index+1,out+index,Bytes<C>(total-index));
    out[index]=value;text.SetLength(total);return total;
}
template<class T> int Insert(T& text,int index,const typename T::XCHAR* source) {
    using C=typename T::XCHAR;
    if(index<0)index=0;if(index>text.GetLength())index=text.GetLength();
    int added=T::StringLength(source),length=text.GetLength();
    if(added<=0)return length;
    int total=int32_t(uint32_t(length)+uint32_t(added));
    C* out=text.GetBuffer(total);
    memmove(out+index+added,out+index,Bytes<C>(length-index+1));
    memcpy(out+index,source,Bytes<C>(added));
    text.SetLength(total);return total;
}
template<class T> int Remove(T& text,typename T::XCHAR value) {
    using C=typename T::XCHAR;
    int length=text.GetLength();C* out=text.GetBuffer(length);
    const C* read=out;const C* end=read+length;
    while(read<end) {
        const C* next=Next(read);
        if(*read!=value)while(read<next)*out++=*read++;
        read=next;
    }
    int removed=int(read-out);*out=0;
    text.SetLength(length-removed);return removed;
}
template<class T> int Replace(T& text,typename T::XCHAR before,typename T::XCHAR after) {
    using C=typename T::XCHAR;
    if(before==after)return 0;
    int length=text.GetLength(),count=0;const C* source=text.GetString();
    for(int index=0;index<length;) {
        if(source[index]==before) {
            if(!count)source=text.GetBuffer(length);
            const_cast<C*>(source)[index]=after;++count;
        }
        index=int(Next(source+index)-source);
    }
    if(count)text.SetLength(length);return count;
}
template<class T> int Replace(T& text,const typename T::XCHAR* before,const typename T::XCHAR* after) {
    using C=typename T::XCHAR;using openmfc_string_ops::FindText;
    int oldSize=T::StringLength(before);if(!oldSize)return 0;
    int newSize=T::StringLength(after),count=0;
    const C* cursor=text.GetString();const C* end=cursor+text.GetLength();
    // Count each terminated segment of a string that can contain embedded NULs.
    while(cursor<end) {
        while(const C* found=FindText(cursor,before)){++count;cursor=found+oldSize;}
        cursor+=T::StringLength(cursor)+1;
    }
    if(count<=0)return count;
    int length=text.GetLength();
    int total=int32_t(uint32_t(length)+(uint32_t(newSize)-uint32_t(oldSize))*uint32_t(count));
    C* base=text.GetBuffer(length>total?length:total);C* write=base;
    // Native traversal keeps this original boundary while moving the tail.
    end=base+length;
    while(write<end) {
        while(const C* match=FindText(write,before)) {
            C* found=const_cast<C*>(match);write=found+newSize;
            int remaining=length-int(found-base)-oldSize;
            if(remaining)memmove(write,found+oldSize,Bytes<C>(remaining));
            if(newSize)memcpy(found,after,Bytes<C>(newSize));
            found[int32_t(uint32_t(remaining)+uint32_t(newSize))]=0;
            length=int32_t(uint32_t(length)+uint32_t(newSize)-uint32_t(oldSize));
        }
        write+=T::StringLength(write)+1;
    }
    text.SetLength(total);return count;
}
inline bool Space(wchar_t value) {return iswspace(value)!=0;}
inline bool Space(char value) {return _ismbcspace(static_cast<unsigned int>(int(value)))!=0;}
template<class T,class Predicate> T& TrimLeftIf(T& text,Predicate matches,bool stopAtNull) {
    using C=typename T::XCHAR;
    const C* cursor=text.GetString();
    while((!stopAtNull || *cursor) && matches(*cursor))cursor=Next(cursor);
    if(cursor!=text.GetString()) {
        int offset=int(cursor-text.GetString());
        C* out=text.GetBuffer(text.GetLength());
        int length=text.GetLength(),remaining=int32_t(uint32_t(length)-uint32_t(offset));
        size_t count=Bytes<C>(int32_t(uint32_t(remaining)+1));
        if(count) {
            if(count>Bytes<C>(int32_t(uint32_t(length)+1))) {
                errno=ERANGE;_invalid_parameter_noinfo();AfxThrowInvalidArgException();
            }
            memmove(out,out+offset,count);
        }
        text.SetLength(remaining);
    }
    return text;
}
template<class T,class Predicate> T& TrimRightIf(T& text,Predicate matches) {
    using C=typename T::XCHAR;
    const C* cursor=text.GetString();const C* trailing=nullptr;
    while(*cursor) {
        if(matches(*cursor)){if(!trailing)trailing=cursor;}else trailing=nullptr;
        cursor=Next(cursor);
    }
    if(trailing) {
        int length=int(trailing-text.GetString());
        text.GetBuffer(length);text.SetLength(length);
    }
    return text;
}
template<class T> T& TrimLeft(T& text) {return TrimLeftIf(text,[](typename T::XCHAR value){return Space(value);},false);}
template<class T> T& TrimRight(T& text) {return TrimRightIf(text,[](typename T::XCHAR value){return Space(value);});}
template<class T> T& TrimLeft(T& text,typename T::XCHAR target) {return TrimLeftIf(text,[target](typename T::XCHAR value){return value==target;},false);}
template<class T> T& TrimRight(T& text,typename T::XCHAR target) {return TrimRightIf(text,[target](typename T::XCHAR value){return value==target;});}
template<class T> T& TrimLeft(T& text,const typename T::XCHAR* set) {
    if(!set || !*set)return text;
    return TrimLeftIf(text,[set](typename T::XCHAR value){return openmfc_string_ops::FindChar(set,value,false)!=nullptr;},true);
}
template<class T> T& TrimRight(T& text,const typename T::XCHAR* set) {
    if(!set || !*set)return text;
    return TrimRightIf(text,[set](typename T::XCHAR value){return openmfc_string_ops::FindChar(set,value,false)!=nullptr;});
}
} // namespace openmfc_string_mutation
