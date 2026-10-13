#pragma once
#include "detail/OleExceptionTranslationSupport.h"
namespace automation_exception_tests {
struct Lifetime { int deleted=0;AFX_MODULE_STATE* module=nullptr; };
class DispatchFailure : public COleDispatchException {
    Lifetime& lifetime;
public:
    explicit DispatchFailure(Lifetime& state,WORD code=37)
        : COleDispatchException(L"failure",0x12345,code),lifetime(state) {
        wchar_t description[]={L'a',L'\0',L'b'};
        std::memcpy(m_strDescription.GetBuffer(3),description,sizeof(description));m_strDescription.ReleaseBuffer(3);
        m_strSource=L"Automation server";m_strHelpFile=L"help.chm";
        m_scError=E_ABORT;
    }
    ~DispatchFailure() override { ++lifetime.deleted;lifetime.module=g_cmdTestCurrentModule; }
};
class OleFailure : public COleException {
    Lifetime& lifetime;
public:
    explicit OleFailure(Lifetime& state) : lifetime(state) { m_sc=E_ACCESSDENIED; }
    ~OleFailure() override { ++lifetime.deleted;lifetime.module=g_cmdTestCurrentModule; }
};
class GenericFailure : public CException {
    Lifetime& lifetime;
public:
    GenericFailure(Lifetime& state,bool owned=true) : CException(owned),lifetime(state) {}
    ~GenericFailure() override { ++lifetime.deleted;lifetime.module=g_cmdTestCurrentModule; }
};
class MemoryFailure : public GenericFailure {
public:
    using GenericFailure::GenericFailure;
    CRuntimeClass* GetRuntimeClass() const override { return &g_automationMemoryExceptionClass; }
};
class Target : public CCmdTarget {
public:
    Lifetime lifetime;
    CException* borrowed=nullptr;
    int kind=0;LONG number=0;
    LONG Fail() {
        switch(kind) {
        case 1: throw static_cast<COleDispatchException*>(new DispatchFailure(lifetime));
        case 2: throw static_cast<COleException*>(new OleFailure(lifetime));
        case 3: throw static_cast<CException*>(new GenericFailure(lifetime));
        case 4: throw static_cast<CException*>(new MemoryFailure(lifetime));
        case 5: throw borrowed;
        default: throw 23;
        }
    }
    void Changed() { Fail(); }
    const AFX_DISPMAP* GetDispatchMap() const override {
        static const AFX_DISPMAP_ENTRY entries[]={
            {L"Fail",1,"",VT_I4,cmdtarget_invoke_tests::address(&Target::Fail),nullptr,0,afxDispCustom_},
            {L"Number",2,nullptr,VT_I4,nullptr,cmdtarget_invoke_tests::address(&Target::Changed),offsetof(Target,number),afxDispCustom_},
            {nullptr,-1,nullptr,0,nullptr,nullptr,SIZE_MAX,afxDispCustom_}
        };
        static UINT count=UINT_MAX;static ULONG mask=ULONG_MAX;
        static const AFX_DISPMAP map{nullptr,entries,&count,&mask};return &map;
    }
};
static void clear(EXCEPINFO& info) {
    SysFreeString(info.bstrSource);SysFreeString(info.bstrDescription);SysFreeString(info.bstrHelpFile);info={};
}
static HRESULT invoke(IDispatch* dispatch,EXCEPINFO* info,VARIANT* result=nullptr) {
    DISPPARAMS args{};return dispatch->Invoke(1,IID_NULL,0,DISPATCH_METHOD,&args,result,info,nullptr);
}
static void exceptions() {
    alignas(COleDispatchException) unsigned char raw[sizeof(COleDispatchException)+8];std::memset(raw,0xa5,sizeof(raw));
    auto* constructed=impl___0COleDispatchException__QEAA_PEB_WIG_Z(reinterpret_cast<COleDispatchException*>(raw),L"copied",0x12345,37);
    wchar_t text[4]{};UINT help=0;
    check(sizeof(COleDispatchException)==64 && constructed->m_wCode==37 && constructed->m_dwHelpContext==0x72345 &&
        constructed->m_scError==S_OK && constructed->m_strDescription==L"copied" && constructed->m_strSource.IsEmpty() &&
        constructed->m_strHelpFile.IsEmpty() && raw[64]==0xa5,
        "dispatch exception constructor preserves native offsets, help-ID bias, error code and owned CString storage");
    check(constructed->GetRuntimeClass()==COleDispatchException::GetThisClass() &&
        !constructed->IsKindOf(COleException::GetThisClass()) && constructed->GetErrorMessage(text,4,&help) &&
        std::wcscmp(text,L"cop")==0 && help==0x72345,
        "dispatch exceptions expose their own runtime class and truncate error text with the native help context");
    impl___1COleDispatchException__UEAA_XZ(constructed);
    COleDispatchException noCode;
    check(noCode.m_scError==E_UNEXPECTED && noCode.m_dwHelpContext==0,"zero-code dispatch exceptions initialize E_UNEXPECTED");
    unsigned char ownerBytes[8]{},callerBytes[8]{};
    auto* owner=reinterpret_cast<AFX_MODULE_STATE*>(ownerBytes);
    auto* caller=reinterpret_cast<AFX_MODULE_STATE*>(callerBytes);
    g_cmdTestCurrentModule=owner;Target target;
    impl__EnableAutomation_CCmdTarget__QEAAXXZ(&target);
    auto* dispatch=reinterpret_cast<IDispatch*>(&target.m_xDispatch);
    g_cmdTestCurrentModule=caller;
    EXCEPINFO info{};VARIANT result{};result.vt=VT_I4;result.lVal=99;
    target.kind=1;
    check(invoke(dispatch,&info,&result)==DISP_E_EXCEPTION && info.wCode==37 && info.scode==E_ABORT &&
        info.dwHelpContext==0x72345 && info.bstrDescription && SysStringLen(info.bstrDescription)==3 && info.bstrDescription[2]==L'b' &&
        info.bstrSource && std::wcscmp(info.bstrSource,L"Automation server")==0 &&
        info.bstrHelpFile && std::wcscmp(info.bstrHelpFile,L"help.chm")==0 && result.vt==VT_EMPTY &&
        target.lifetime.deleted==1 && target.lifetime.module==owner && g_cmdTestCurrentModule==caller,
        "Invoke converts dispatch failures into owned EXCEPINFO, clears the result and deletes the exception in its owner module");
    clear(info);
    check(invoke(dispatch,nullptr)==DISP_E_EXCEPTION && target.lifetime.deleted==2 && g_cmdTestCurrentModule==caller,
        "Invoke consumes dispatch exceptions even when its caller omits EXCEPINFO");
    target.kind=2;info.wCode=99;
    check(invoke(dispatch,&info)==E_ACCESSDENIED && info.wCode==99 && target.lifetime.deleted==3 && g_cmdTestCurrentModule==caller,
        "COleException callbacks return their stored HRESULT without synthesizing dispatch metadata");
    info={};target.kind=3;
    check(invoke(dispatch,&info)==DISP_E_EXCEPTION && info.wCode==0xf108 && info.scode==S_OK &&
        info.bstrDescription && info.bstrSource && target.lifetime.deleted==4,
        "generic framework exceptions use the native internal-failure resource code and release their owner");
    clear(info);target.kind=4;
    check(invoke(dispatch,&info)==DISP_E_EXCEPTION && info.wCode==0xf10a && target.lifetime.deleted==5,
        "framework memory exceptions use the native allocation-failure resource code");
    clear(info);
    Lifetime borrowedLifetime;
    GenericFailure borrowed(borrowedLifetime,false);target.borrowed=&borrowed;target.kind=5;
    check(invoke(dispatch,&info)==DISP_E_EXCEPTION && borrowedLifetime.deleted==0 && g_cmdTestCurrentModule==caller,
        "exception cleanup respects CException's non-owning auto-delete policy");
    clear(info);target.kind=1;
    VARIANT input{};input.vt=VT_I4;input.lVal=17;DISPID named=DISPID_PROPERTYPUT;
    DISPPARAMS args{&input,&named,1,1};
    check(dispatch->Invoke(2,IID_NULL,0,DISPATCH_PROPERTYPUT,&args,nullptr,&info,nullptr)==DISP_E_EXCEPTION &&
        target.number==17 && info.wCode==37 && target.lifetime.deleted==6 && g_cmdTestCurrentModule==caller,
        "property-notification exceptions use the same COM translation and preserve the completed property write");
    clear(info);target.kind=0;bool caught=false;
    try { invoke(dispatch,nullptr); } catch(int code) { caught=code==23; }
    check(caught && g_cmdTestCurrentModule==caller,"non-framework C++ exceptions preserve their native propagation and restore module context");
    g_cmdTestCurrentModule=nullptr;
}
}
static void automationExceptions() { automation_exception_tests::exceptions(); }
