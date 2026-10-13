#pragma once
#include "detail/CCmdTargetDispatchSupport.h"

namespace cmdtarget_dispatch_tests {
class Target : public CCmdTarget {
public:
    LONG number=7;
    BOOL enabled=TRUE;
    CString text=L"initial";
    VARIANT variant{};
    IUnknown* object=nullptr;
    int notifications=0, finals=0;
    AFX_MODULE_STATE* notificationModule=nullptr;
    ~Target() { VariantClear(&variant); if(object)object->Release(); }
    void OnFinalRelease() override { ++finals; }
    static void MS_ABI Changed(void* self) {
        auto* target=static_cast<Target*>(self);
        ++target->notifications;
        target->notificationModule=g_cmdTestCurrentModule;
    }
    const AFX_DISPMAP* GetDispatchMap() const override {
        static const AFX_DISPMAP_ENTRY entries[]={
            {L"Number",11,nullptr,VT_I4,nullptr,reinterpret_cast<void*>(Changed),offsetof(Target,number),afxDispCustom_},
            {L"Enabled",12,nullptr,VT_BOOL,nullptr,nullptr,offsetof(Target,enabled),afxDispCustom_},
            {L"Text",13,nullptr,VT_BSTR,nullptr,nullptr,offsetof(Target,text),afxDispCustom_},
            {L"Variant",14,nullptr,VT_VARIANT,nullptr,nullptr,offsetof(Target,variant),afxDispCustom_},
            {L"Object",15,nullptr,VT_UNKNOWN,nullptr,nullptr,offsetof(Target,object),afxDispCustom_},
            {L"Method",16,"",VT_EMPTY,reinterpret_cast<void*>(Changed),nullptr,0,afxDispCustom_},
            {nullptr,-1,nullptr,0,nullptr,nullptr,static_cast<size_t>(-1),afxDispCustom_}
        };
        static UINT count=static_cast<UINT>(-1);
        static ULONG mask=static_cast<ULONG>(-1);
        static const AFX_DISPMAP map{nullptr,entries,&count,&mask};
        return &map;
    }
};
class CountedUnknown : public IUnknown {
public:
    ULONG references=1;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid,void** out) override {
        if(!out)return E_POINTER;
        *out=nullptr;
        if(iid!=IID_IUnknown)return E_NOINTERFACE;
        *out=this;AddRef();return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return ++references; }
    ULONG STDMETHODCALLTYPE Release() override { return --references; }
};
static HRESULT get(IDispatch* dispatch,DISPID id,VARIANT* out) {
    DISPPARAMS args{};
    return dispatch->Invoke(id,IID_NULL,0,DISPATCH_PROPERTYGET,&args,out,nullptr,nullptr);
}
static HRESULT put(IDispatch* dispatch,DISPID id,VARIANT* in,WORD flags=DISPATCH_PROPERTYPUT) {
    DISPID named=DISPID_PROPERTYPUT;
    DISPPARAMS args{in,&named,1,1};
    return dispatch->Invoke(id,IID_NULL,0,flags,&args,nullptr,nullptr,nullptr);
}
static void dispatch() {
    unsigned char ownerState[8]{},callerState[8]{};
    auto* owner=reinterpret_cast<AFX_MODULE_STATE*>(ownerState);
    auto* caller=reinterpret_cast<AFX_MODULE_STATE*>(callerState);
    g_cmdTestCurrentModule=owner;
    CountedUnknown value;
    Target target;
    target.variant.vt=VT_I4;target.variant.lVal=41;
    check(!impl__GetIDispatch_CCmdTarget__QEAAPEAUIDispatch__H_Z(&target,FALSE),
        "automation remains hidden until the native embedded interface is enabled");
    impl__EnableAutomation_CCmdTarget__QEAAXXZ(&target);
    auto* disp=static_cast<IDispatch*>(impl__GetIDispatch_CCmdTarget__QEAAPEAUIDispatch__H_Z(&target,TRUE));
    check(disp==reinterpret_cast<IDispatch*>(&target.m_xDispatch) && target.m_dwRef==2 &&
        impl__FromIDispatch_CCmdTarget__SAPEAV1_PEAUIDispatch___Z(disp)==&target &&
        !impl__FromIDispatch_CCmdTarget__SAPEAV1_PEAUIDispatch___Z(&value),
        "automation activation exposes the physical +0x20 interface and validates reverse owner lookup");
    if(!disp)return;
    IUnknown* unknown=nullptr;
    check(disp->QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&unknown))==S_OK &&
        unknown==static_cast<IUnknown*>(disp) && target.m_dwRef==3,
        "embedded dispatch QI shares the owner's COM identity and reference count");
    if(unknown)unknown->Release();
    UINT infoCount=99;ITypeInfo* typeInfo=reinterpret_cast<ITypeInfo*>(1);
    check(disp->GetTypeInfoCount(&infoCount)==S_OK && infoCount==0 &&
        disp->GetTypeInfo(0,0,&typeInfo)==DISP_E_BADINDEX && !typeInfo,
        "dispatch type information respects the owner's virtual capabilities");
    wchar_t number[]=L"nUmBeR",text[]=L"Text",missing[]=L"Missing";
    LPOLESTR names[]={number,text,missing};DISPID ids[3]{};
    check(disp->GetIDsOfNames(IID_NULL,names,3,0,ids)==DISP_E_UNKNOWNNAME && ids[0]==11 && ids[1]==13 && ids[2]==DISPID_UNKNOWN,
        "dispatch name lookup uses the client map case-insensitively and writes every output");
    g_cmdTestCurrentModule=caller;
    VARIANT result{};
    check(get(disp,11,&result)==S_OK && result.vt==VT_I4 && result.lVal==7,
        "property get returns the actual client object's LONG field");
    VARIANT input{};input.vt=VT_BSTR;input.bstrVal=SysAllocString(L"123");
    check(put(disp,11,&input)==S_OK && target.number==123 && target.notifications==1 &&
        target.notificationModule==owner && g_cmdTestCurrentModule==caller && !target.m_bResultExpected,
        "property put coerces BSTR to LONG and calls the notification in the owner's module context");
    VariantClear(&input);
    check(!impl__IsResultExpected_CCmdTarget__QEAAHXZ(&target) && target.m_bResultExpected,
        "Invoke records whether its caller supplied a result");
    check(get(disp,12,&result)==S_OK && result.vt==VT_BOOL && result.boolVal==VARIANT_TRUE,
        "BOOL properties convert their four-byte field to VARIANT_BOOL");
    input.vt=VT_BOOL;input.boolVal=VARIANT_FALSE;
    check(put(disp,12,&input)==S_OK && !target.enabled,
        "BOOL property put updates the actual native field");
    wchar_t embedded[]={L'a',L'\0',L'b'};
    input.vt=VT_BSTR;input.bstrVal=SysAllocStringLen(embedded,3);
    check(put(disp,13,&input)==S_OK && target.text.GetLength()==3 && target.text[2]==L'b' &&
        get(disp,13,&result)==S_OK && result.vt==VT_BSTR && SysStringLen(result.bstrVal)==3 && result.bstrVal[2]==L'b',
        "BSTR and CString property conversion preserves embedded NULs and allocates an owned result");
    VariantClear(&input);VariantClear(&result);
    check(get(disp,14,&result)==S_OK && result.vt==VT_I4 && result.lVal==41,
        "VARIANT properties copy their actual type and contents");
    input.vt=VT_BSTR;input.bstrVal=SysAllocString(L"variant text");
    check(put(disp,14,&input)==S_OK && target.variant.vt==VT_BSTR && target.variant.bstrVal!=input.bstrVal,
        "VARIANT property put owns an independent copy");
    VariantClear(&input);VariantClear(&result);
    input.vt=VT_UNKNOWN;input.punkVal=&value;
    check(put(disp,15,&input,DISPATCH_PROPERTYPUTREF)==S_OK && target.object==&value && value.references==2 &&
        get(disp,15,&result)==S_OK && result.punkVal==&value && value.references==3,
        "interface property get and put take the correct COM references");
    VariantClear(&result);
    input.punkVal=nullptr;
    check(put(disp,15,&input,DISPATCH_PROPERTYPUTREF)==S_OK && !target.object && value.references==1,
        "clearing an interface property releases its previous reference");
    check(get(disp,99,&result)==DISP_E_MEMBERNOTFOUND,
        "unknown DISPIDs return failure without claiming a property result");
    DISPPARAMS noArgs{};
    check(disp->Invoke(16,IID_NULL,0,DISPATCH_METHOD,&noArgs,nullptr,nullptr,nullptr)==S_OK && target.notifications==2,
        "generic no-argument method dispatch executes the mapped callback");
    check(disp->Release()==1 && !target.finals,
        "releasing the dispatch interface returns ownership to the target without destroying it");
    g_cmdTestCurrentModule=nullptr;
}
}
static void cmdtargetDispatch() { cmdtarget_dispatch_tests::dispatch(); }
