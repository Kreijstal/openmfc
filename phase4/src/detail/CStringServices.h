#pragma once
#include "openmfc/afx.h"
#include <oleauto.h>

extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
namespace openmfc_string_services {
struct Access {
    template<class T> static bool Check(T& text,const void* source) {return text.CheckImplicitLoad(source);}
};
template<class T> bool CheckResource(T& text,const void* source) {
    auto id=uintptr_t(source);
    if(id-1>0xfffe)return false;
    auto module=static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT(id)));
    if(module)text.LoadStringW(module,UINT(id));
    return true;
}
inline DWORD Environment(const char* name,char* output,DWORD capacity) {return ::GetEnvironmentVariableA(name,output,capacity);}
inline DWORD Environment(const wchar_t* name,wchar_t* output,DWORD capacity) {return ::GetEnvironmentVariableW(name,output,capacity);}
template<class T> int LoadEnvironment(T& text,const typename T::XCHAR* name) {
    DWORD capacity=Environment(name,nullptr,0);
    if(!capacity){text.Empty();return FALSE;}
    auto* output=text.GetBuffer(int32_t(capacity));
    Environment(name,output,capacity);
    text.ReleaseBuffer();return TRUE;
}
template<class T> void ConvertOEM(T& text,bool toOEM) {
    int length=text.GetLength();auto* output=text.GetBuffer(length);
    if constexpr(sizeof(typename T::XCHAR)==2)AfxThrowOleException(static_cast<long>(0x80004005u));
    else {
        size_t capacity=size_t(int64_t(int32_t(uint32_t(length)+1)));
        if(capacity>UINT32_MAX)AfxThrowOleException(static_cast<long>(0x80070057u));
        BOOL result=toOEM ? ::CharToOemBuffA(output,output,DWORD(capacity)) : ::OemToCharBuffA(output,output,DWORD(capacity));
        if(!result){long error=static_cast<long>(::GetLastError());AfxThrowOleException(error>0?long(0x80070000u|(uint32_t(error)&0xffff)):error);}
        text.SetLength(length);
    }
}
inline BSTR AllocateBSTR(const wchar_t* source,int length) {
    BSTR result=::SysAllocStringLen(source,UINT(length));if(!result)AfxThrowMemoryException();return result;
}
inline BSTR AllocateBSTR(const char* source,int length) {
    int count=::MultiByteToWideChar(CP_THREAD_ACP,0,source,length,nullptr,0);
    BSTR result=::SysAllocStringLen(nullptr,UINT(count));if(!result)AfxThrowMemoryException();
    ::MultiByteToWideChar(CP_THREAD_ACP,0,source,length,result,count);return result;
}
inline BSTR SetBSTR(const wchar_t* source,int length,BSTR* target) {
    if(!::SysReAllocStringLen(target,source,UINT(length)))AfxThrowMemoryException();return *target;
}
inline BSTR SetBSTR(const char* source,int length,BSTR* target) {
    int count=::MultiByteToWideChar(CP_THREAD_ACP,0,source,length,nullptr,0);
    if(!::SysReAllocStringLen(target,nullptr,UINT(count)))AfxThrowMemoryException();
    if(count>0 && !::MultiByteToWideChar(CP_THREAD_ACP,0,source,length,*target,count))AfxThrowMemoryException();
    return *target;
}
} // namespace openmfc_string_services
