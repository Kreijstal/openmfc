// Native x64 MFC member calls through OleAut32's architecture-specific invoker.
#include "detail/CCmdTargetDispatchSupport.h"
#include <vector>
#include <cstring>

namespace {
struct ScopedVariant {
    VARIANT value{};
    ~ScopedVariant() { VariantClear(&value); }
};
struct TemporaryVariants {
    std::vector<VARIANT> values;
    explicit TemporaryVariants(size_t size) : values(size) {}
    ~TemporaryVariants() { for(auto& value:values) VariantClear(&value); }
};
}
// Symbol: ?CallMemberFunc@CCmdTarget@@IEAAJPEBUAFX_DISPMAP_ENTRY@@GPEAUtagVARIANT@@PEAUtagDISPPARAMS@@PEAI@Z
extern "C" long MS_ABI impl__CallMemberFunc_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__GPEAUtagVARIANT__PEAUtagDISPPARAMS__PEAI_Z(
    CCmdTarget* target,const void* rawEntry,unsigned short flags,void* rawResult,void* rawArguments,unsigned int* argError) {
    if(!target || !rawEntry || !rawArguments)return E_POINTER;
    auto* entry=static_cast<const AFX_DISPMAP_ENTRY*>(rawEntry);
    auto* args=static_cast<DISPPARAMS*>(rawArguments);
    auto* result=static_cast<VARIANT*>(rawResult);
    UINT unusedError=0;
    if(!argError)argError=&unusedError;
    bool put=(flags & (DISPATCH_PROPERTYPUT|DISPATCH_PROPERTYPUTREF))!=0;
    void* function=put ? entry->pfnSet : entry->pfn;
    if(!function)return DISP_E_MEMBERNOTFOUND;
    if(args->cArgs && !args->rgvarg)return E_POINTER;
    if(args->cNamedArgs) {
        if(!put || args->cNamedArgs!=1)return DISP_E_NONAMEDARGS;
        if(!args->rgdispidNamedArgs || args->rgdispidNamedArgs[0]!=DISPID_PROPERTYPUT)return DISP_E_PARAMNOTFOUND;
    } else if(put)return DISP_E_PARAMNOTFOUND;
    std::vector<VARTYPE> expected;
    std::vector<bool> optional;
    bool defaults=false;
    if(entry->lpszParams)for(const unsigned char* p=reinterpret_cast<const unsigned char*>(entry->lpszParams);*p;++p) {
        if(*p==0xff) { defaults=true; continue; }
        VARTYPE type=*p;
        if(type & 0x40)type=(type & ~0x40)|VT_BYREF;
        expected.push_back(type);optional.push_back(defaults && type==VT_VARIANT);
    }
    if(put) { expected.push_back(entry->vt); optional.push_back(false); }
    if(args->cArgs>expected.size())return DISP_E_BADPARAMCOUNT;
    for(size_t i=args->cArgs;i<expected.size();++i)if(!optional[i])return DISP_E_BADPARAMCOUNT;
    openmfc::detail::CmdTargetModuleScope module(target->m_pModuleState);
    const bool hiddenResult=!put && (entry->vt==VT_VARIANT || entry->vt==VT_CY);
    const size_t prefix=hiddenResult ? 2 : 1;
    TemporaryVariants prepared(expected.size()+prefix);
    std::vector<VARTYPE> types(prepared.values.size());
    std::vector<VARIANT*> parameters(prepared.values.size());
    std::vector<BOOL> boolReferences(expected.size());
    std::vector<VARIANT_BOOL*> boolOutputs(expected.size(),nullptr);
    ScopedVariant localStorage;
    VARIANT& localResult=localStorage.value;
    prepared.values[0].vt=VT_UI8;
    prepared.values[0].ullVal=reinterpret_cast<ULONG_PTR>(target);
    types[0]=VT_UI8;
    if(hiddenResult) {
        prepared.values[1].vt=VT_UI8;
        prepared.values[1].ullVal=reinterpret_cast<ULONG_PTR>(entry->vt==VT_CY ? static_cast<void*>(&localResult.cyVal) : static_cast<void*>(&localResult));
        types[1]=VT_UI8;
    }
    for(size_t i=0;i<expected.size();++i) {
        VARIANT& value=prepared.values[prefix+i];
        VARTYPE wanted=expected[i];
        VARIANT* source=i<args->cArgs ? &args->rgvarg[args->cArgs-1-static_cast<UINT>(i)] : nullptr;
        HRESULT hr=S_OK;
        if(!source) { value.vt=VT_ERROR; value.scode=DISP_E_PARAMNOTFOUND; }
        else if(wanted==VT_VARIANT)hr=VariantCopy(&value,source);
        else if(source->vt==wanted)hr=VariantCopy(&value,source);
        else if(wanted & VT_BYREF)hr=DISP_E_TYPEMISMATCH;
        else hr=VariantChangeType(&value,source,0,wanted);
        if(FAILED(hr)) {
            *argError=source ? args->cArgs-1-static_cast<UINT>(i) : 0;
            return hr;
        }
        types[prefix+i]=wanted;
        if(wanted==VT_VARIANT) {
            // VTS_VARIANT uses a const VARIANT& in the MFC dispatch ABI.
            // Keep the owned copy separate from its pointer argument.
            types[prefix+i]=VT_VARIANT|VT_BYREF;
        } else if(wanted==VT_BOOL) {
            value.vt=VT_I4;value.lVal=value.boolVal!=VARIANT_FALSE;
            types[prefix+i]=VT_I4;
        } else if(wanted==(VT_BOOL|VT_BYREF)) {
            if(!value.pboolVal)return E_POINTER;
            boolOutputs[i]=value.pboolVal;
            boolReferences[i]=*value.pboolVal!=VARIANT_FALSE;
            value.vt=VT_I4|VT_BYREF;value.plVal=reinterpret_cast<LONG*>(&boolReferences[i]);
            types[prefix+i]=VT_I4|VT_BYREF;
        }
    }
    // DispCallFunc can invoke a function address when pvInstance is NULL.
    // Supply `this` explicitly, so floating-point arguments retain their MSVC
    // register positions and stack arguments use the native shadow space.
    std::vector<VARIANT> variantPointers(expected.size());
    for(size_t i=0;i<prepared.values.size();++i)parameters[i]=&prepared.values[i];
    for(size_t i=0;i<expected.size();++i)if(expected[i]==VT_VARIANT) {
        variantPointers[i].vt=VT_VARIANT|VT_BYREF;
        variantPointers[i].pvarVal=&prepared.values[prefix+i];
        parameters[prefix+i]=&variantPointers[i];
    }
    ScopedVariant calledStorage;
    VARIANT& calledResult=calledStorage.value;
    VARTYPE returnType=put || hiddenResult ? VT_EMPTY : entry->vt==VT_BOOL ? VT_I4 : entry->vt;
    HRESULT hr=DispCallFunc(nullptr,reinterpret_cast<ULONG_PTR>(function),CC_STDCALL,returnType,
        static_cast<UINT>(types.size()),types.data(),parameters.data(),&calledResult);
    for(size_t i=0;i<boolOutputs.size();++i)if(boolOutputs[i])*boolOutputs[i]=boolReferences[i] ? VARIANT_TRUE : VARIANT_FALSE;
    if(FAILED(hr)) { VariantClear(&calledResult); VariantClear(&localResult); return hr; }
    if(hiddenResult) {
        if(entry->vt==VT_CY)localResult.vt=VT_CY;
    } else {
        localResult=calledResult;calledResult.vt=VT_EMPTY;
        if(!put && entry->vt==VT_BOOL) {
            BOOL returned=localResult.lVal;
            localResult.vt=VT_BOOL;localResult.boolVal=returned ? VARIANT_TRUE : VARIANT_FALSE;
        }
    }
    if(result) { *result=localResult;localResult.vt=VT_EMPTY; }
    VariantClear(&localResult);
    return S_OK;
}
