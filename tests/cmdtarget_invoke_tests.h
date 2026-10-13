#pragma once
#include "detail/CCmdTargetDispatchSupport.h"
namespace cmdtarget_invoke_tests {
class Target : public CCmdTarget {
public:
    int calls=0;
    LONG indexed=0;
    AFX_MODULE_STATE* module=nullptr;
    double Mixed(LONG a,double b,LONG c,float d,double e,LONGLONG f) {
        ++calls;module=g_cmdTestCurrentModule;
        return a+b+c+d+e+static_cast<double>(f);
    }
    BOOL Boolean(BOOL value,BOOL* reference) { ++calls;*reference=!value;return value; }
    LONG VariantReference(const VARIANT& value) { ++calls;return value.vt==VT_I4 ? value.lVal : -1; }
    VARIANT VariantReturn(LONG value) { ++calls;VARIANT result{};result.vt=VT_I4;result.lVal=value;return result; }
    CY CurrencyReturn(LONGLONG value) { ++calls;CY result{};result.int64=value;return result; }
    BSTR StringReturn(BSTR value) { ++calls;return SysAllocStringLen(value,SysStringLen(value)); }
    void IndexedPut(LONG index,LONG value) { ++calls;indexed=index+value; }
    LONG Throw(BSTR) { ++calls;throw 17; }
    LONG Optional(const VARIANT& value) { ++calls;return value.vt==VT_ERROR && value.scode==DISP_E_PARAMNOTFOUND ? 99 : -1; }
};
template<class Member> static void* address(Member member) {
    static_assert(sizeof(member)==sizeof(void*),"single-inheritance MSVC member-function pointer");
    void* pointer=nullptr;memcpy(&pointer,&member,sizeof(pointer));return pointer;
}
static AFX_DISPMAP_ENTRY entry(void* function,const char* parameters,VARTYPE result) {
    return {L"Test",1,parameters,result,function,nullptr,0,afxDispCustom_};
}
static HRESULT call(Target& target,AFX_DISPMAP_ENTRY& method,VARIANT* values,UINT count,VARIANT* result,UINT* error=nullptr) {
    DISPPARAMS arguments{values,nullptr,count,0};
    return impl__CallMemberFunc_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__GPEAUtagVARIANT__PEAUtagDISPPARAMS__PEAI_Z(
        &target,&method,DISPATCH_METHOD,result,&arguments,error);
}
static void invoke() {
    unsigned char ownerState[8]{},callerState[8]{};
    auto* owner=reinterpret_cast<AFX_MODULE_STATE*>(ownerState);
    auto* caller=reinterpret_cast<AFX_MODULE_STATE*>(callerState);
    g_cmdTestCurrentModule=owner;
    Target target;
    g_cmdTestCurrentModule=caller;
    const char mixedParams[]={VT_I4,VT_R8,VT_I4,VT_R4,VT_R8,VT_I8,0};
    auto mixed=entry(address(&Target::Mixed),mixedParams,VT_R8);
    VARIANT inputs[6]{};
    inputs[5].vt=VT_I4;inputs[5].lVal=1;
    inputs[4].vt=VT_R8;inputs[4].dblVal=2.5;
    inputs[3].vt=VT_I4;inputs[3].lVal=3;
    inputs[2].vt=VT_R4;inputs[2].fltVal=4.5f;
    inputs[1].vt=VT_R8;inputs[1].dblVal=5.5;
    inputs[0].vt=VT_I8;inputs[0].llVal=6;
    VARIANT result{};
    check(call(target,mixed,inputs,6,&result)==S_OK && result.vt==VT_R8 && result.dblVal==22.5 &&
        target.module==owner && g_cmdTestCurrentModule==caller,
        "native method dispatch passes mixed GP/XMM and stack arguments in MSVC order and restores module context");
    UINT error=99;
    inputs[3].vt=VT_BSTR;inputs[3].bstrVal=SysAllocString(L"invalid");
    check(call(target,mixed,inputs,6,&result,&error)==DISP_E_TYPEMISMATCH && error==3 && target.calls==1,
        "method coercion reports the original reversed argument index without calling the target");
    VariantClear(&inputs[3]);
    VARIANT_BOOL changed=VARIANT_FALSE;
    VARIANT booleanArgs[2]{};
    booleanArgs[0].vt=VT_BOOL|VT_BYREF;booleanArgs[0].pboolVal=&changed;
    booleanArgs[1].vt=VT_BOOL;booleanArgs[1].boolVal=VARIANT_FALSE;
    const char boolParams[]={VT_BOOL,static_cast<char>(VT_BOOL|0x40),0};
    auto boolean=entry(address(&Target::Boolean),boolParams,VT_BOOL);
    check(call(target,boolean,booleanArgs,2,&result)==S_OK && result.vt==VT_BOOL && result.boolVal==VARIANT_FALSE && changed==VARIANT_TRUE,
        "method dispatch converts VARIANT_BOOL arguments and by-reference values to native four-byte BOOL storage");
    VARIANT value{};value.vt=VT_I4;value.lVal=123;
    const char variantParams[]={VT_VARIANT,0};
    auto variantReference=entry(address(&Target::VariantReference),variantParams,VT_I4);
    check(call(target,variantReference,&value,1,&result)==S_OK && result.vt==VT_I4 && result.lVal==123,
        "VTS_VARIANT arguments reach the C++ const VARIANT reference ABI");
    const char longParams[]={VT_I4,0};
    auto variantReturn=entry(address(&Target::VariantReturn),longParams,VT_VARIANT);
    check(call(target,variantReturn,&value,1,&result)==S_OK && result.vt==VT_I4 && result.lVal==123,
        "VARIANT method returns use the hidden result buffer after the MSVC this pointer");
    const char currencyParams[]={VT_I8,0};
    auto currencyReturn=entry(address(&Target::CurrencyReturn),currencyParams,VT_CY);
    value.vt=VT_I8;value.llVal=987654;
    check(call(target,currencyReturn,&value,1,&result)==S_OK && result.vt==VT_CY && result.cyVal.int64==987654,
        "currency method returns use the native member-function result-buffer convention");
    const char stringParams[]={VT_BSTR,0};
    auto stringReturn=entry(address(&Target::StringReturn),stringParams,VT_BSTR);
    wchar_t text[]={L'x',L'\0',L'z'};
    value.vt=VT_BSTR;value.bstrVal=SysAllocStringLen(text,3);
    check(call(target,stringReturn,&value,1,&result)==S_OK && result.vt==VT_BSTR && SysStringLen(result.bstrVal)==3 &&
        result.bstrVal[2]==L'z' && result.bstrVal!=value.bstrVal,
        "method dispatch preserves BSTR arguments and transfers ownership of returned strings");
    VariantClear(&result);VariantClear(&value);
    const char optionalParams[]={static_cast<char>(0xff),VT_VARIANT,0};
    auto optional=entry(address(&Target::Optional),optionalParams,VT_I4);
    check(call(target,optional,nullptr,0,&result)==S_OK && result.lVal==99,
        "missing optional VARIANT arguments carry DISP_E_PARAMNOTFOUND");
    check(call(target,mixed,nullptr,0,&result)==DISP_E_BADPARAMCOUNT,
        "missing required method arguments fail before entering the target");
    const char indexParams[]={VT_I4,0};
    auto indexed=entry(nullptr,indexParams,VT_I4);
    indexed.pfnSet=address(&Target::IndexedPut);
    VARIANT indexedArgs[2]{};
    indexedArgs[0].vt=VT_I4;indexedArgs[0].lVal=20;
    indexedArgs[1].vt=VT_I4;indexedArgs[1].lVal=4;
    DISPID named=DISPID_PROPERTYPUT;
    DISPPARAMS putArguments{indexedArgs,&named,2,1};
    check(impl__CallMemberFunc_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__GPEAUtagVARIANT__PEAUtagDISPPARAMS__PEAI_Z(
        &target,&indexed,DISPATCH_PROPERTYPUT,nullptr,&putArguments,nullptr)==S_OK && target.indexed==24,
        "indexed property setters append the property value after the declared index arguments");
    auto throwing=entry(address(&Target::Throw),stringParams,VT_I4);
    value.vt=VT_BSTR;value.bstrVal=SysAllocString(L"throw");
    bool caught=false;
    try { call(target,throwing,&value,1,&result); }
    catch(int code) { caught=code==17; }
    check(caught && g_cmdTestCurrentModule==caller,
        "exception unwinding through the native invoker restores module context");
    VariantClear(&value);
    g_cmdTestCurrentModule=nullptr;
}
}
static void cmdtargetInvoke() { cmdtarget_invoke_tests::invoke(); }
