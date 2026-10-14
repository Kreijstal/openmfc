// Compile exclusively against the Microsoft ATL SDK. The test executable uses
// OpenMFC headers and passes ABI-compatible string managers across this boundary.
#include <atlbase.h>
#include <atlsimpstr.h>
#include <new>
using NativeString=ATL::CSimpleStringT<wchar_t,false>;
template<class Operation> HRESULT invoke(Operation operation) {
    try {operation();return S_OK;}
    catch(const ATL::CAtlException& error) {return error.m_hr;}
}
extern "C" __declspec(dllexport) HRESULT native_string_copy(void* destination,const void* source) {
    return invoke([&]{new(destination) NativeString(*static_cast<const NativeString*>(source));});
}
extern "C" __declspec(dllexport) HRESULT native_string_assign(void* destination,const void* source) {
    return invoke([&]{*static_cast<NativeString*>(destination)=*static_cast<const NativeString*>(source);});
}
extern "C" __declspec(dllexport) HRESULT native_string_set(void* destination,const wchar_t* source) {
    return invoke([&]{*static_cast<NativeString*>(destination)=source;});
}
extern "C" __declspec(dllexport) HRESULT native_string_empty(void* destination) {
    return invoke([&]{static_cast<NativeString*>(destination)->Empty();});
}
extern "C" __declspec(dllexport) void native_string_destroy(void* value) {static_cast<NativeString*>(value)->~NativeString();}
