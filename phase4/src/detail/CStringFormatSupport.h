#pragma once
#include "openmfc/afx.h"
#include <cstdio>
#include <cwchar>

namespace openmfc_string_format {
inline int Measure(const char* format,va_list args) {
    return __stdio_common_vsprintf(*__local_stdio_printf_options()|2,nullptr,0,format,nullptr,args);
}
inline int Measure(const wchar_t* format,va_list args) {
    return __stdio_common_vswprintf(*__local_stdio_printf_options()|6,nullptr,0,format,nullptr,args);
}
inline void Write(char* output,size_t size,const char* format,va_list args) {
    __stdio_common_vsprintf_s(*__local_stdio_printf_options(),output,size,format,nullptr,args);
}
inline void Write(wchar_t* output,size_t size,const wchar_t* format,va_list args) {
    __stdio_common_vswprintf_s(*__local_stdio_printf_options()|4,output,size,format,nullptr,args);
}
template<class T>void Printf(T& text,const typename T::XCHAR* format,va_list args,bool append) {
    if(!format)AfxThrowOleException(static_cast<long>(0x80070057u));
    int oldLength=append?text.GetLength():0;
    va_list measuring;va_copy(measuring,args);
    int count=Measure(format,measuring);va_end(measuring);
    if(count<0)AfxThrowOleException(static_cast<long>(0x80004005u));
    T savedFormat(format,text.GetManager());
    int total=int32_t(uint32_t(oldLength)+uint32_t(count));
    auto* output=text.GetBuffer(total);
    va_list writing;va_copy(writing,args);
    Write(output+oldLength,size_t(int64_t(int32_t(uint32_t(count)+1u))),savedFormat.GetString(),writing);
    va_end(writing);
    text.SetLength(total);
}
template<class T>void Resource(T& text,UINT id,va_list args,bool append) {
    T format(text.GetManager());
    if(!format.LoadStringW(id))AfxThrowOleException(static_cast<long>(0x80004005u));
    if(append)text.AppendFormatV(format.GetString(),args);
    else text.FormatV(format.GetString(),args);
}
template<class C>struct LocalMessage {
    C* data=nullptr;
    ~LocalMessage(){::LocalFree(data);}
};
inline DWORD Message(const char* format,char** output,va_list* args) {
    return ::FormatMessageA(0x500,format,0,0,reinterpret_cast<char*>(output),0,args);
}
inline DWORD Message(const wchar_t* format,wchar_t** output,va_list* args) {
    return ::FormatMessageW(0x500,format,0,0,reinterpret_cast<wchar_t*>(output),0,args);
}
template<class T>void Message(T& text,const typename T::XCHAR* format,va_list* args) {
    LocalMessage<typename T::XCHAR> output;
    DWORD savedError=::GetLastError();::SetLastError(0);
    if(!Message(format,&output.data,args) && ::GetLastError())AfxThrowMemoryException();
    ::SetLastError(savedError);
    text.SetString(output.data);
}
template<class T>void MessageResource(T& text,UINT id,va_list* args) {
    T format(text.GetManager());
    if(!format.LoadStringW(id))AfxThrowOleException(static_cast<long>(0x80004005u));
    if(format.IsEmpty())AfxThrowOleException(static_cast<long>(0x80070057u));
    text.FormatMessageV(format.GetString(),args);
}
} // namespace openmfc_string_format
