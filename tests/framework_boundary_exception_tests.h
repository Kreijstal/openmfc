#pragma once
#include "detail/CEnumArrayInterfacesSupport.h"
#include "detail/CCmdTargetEventsSupport.h"
namespace framework_boundary_exception_tests {
using automation_exception_tests::Lifetime;
using automation_exception_tests::GenericFailure;
class EventTarget : public CCmdTarget {
public:
    Lifetime lifetime;
    int kind=1;
    CException* borrowed=nullptr;
    AFX_MODULE_STATE* callbackModule=nullptr;
    void Fail() {
        callbackModule=g_cmdTestCurrentModule;
        if(kind==1)throw static_cast<COleDispatchException*>(new automation_exception_tests::DispatchFailure(lifetime));
        if(kind==2)throw static_cast<COleException*>(new automation_exception_tests::OleFailure(lifetime));
        if(kind==3)throw borrowed;
        throw 31;
    }
    BOOL Event(UINT,BSTR) { Fail();return FALSE; }
    BOOL Request(BOOL*) { Fail();return FALSE; }
    const AFX_EVENTSINKMAP* GetEventSinkMap() const override {
        static const char parameters[]={VT_I4,VT_BSTR,0};
        static const AFX_EVENTSINKMAP_ENTRY entries[]={
            {{nullptr,31,parameters,VT_BOOL,cmdtarget_invoke_tests::address(&EventTarget::Event),nullptr,0,afxDispCustom_},10,11},
            {{nullptr,32,nullptr,VT_BOOL,cmdtarget_invoke_tests::address(&EventTarget::Request),nullptr,1,afxDispCustom_},20,UINT_MAX},
            {{nullptr,-1,nullptr,0,nullptr,nullptr,SIZE_MAX,afxDispCustom_},0,0}
        };
        static const AFX_EVENTSINKMAP map{nullptr,entries,nullptr};return &map;
    }
};
// A client-provided CEnumArray subclass with the native prefix and virtual slots.
class ClientEnumerator : public CCmdTarget {
public:
    size_t elementSize=sizeof(CONNECTDATA);
    void* clonedFrom=nullptr;void* data=nullptr;UINT position=0,count=0;
    BOOL needFree=FALSE;DWORD padding=0;ULONG_PTR interfaceTable=0;UINT capacity=0,tail=0;
    Lifetime lifetime;
    unsigned nextCalls=0,skipCalls=0;
    bool integer=false;
    AFX_MODULE_STATE* callbackModule=nullptr;
    ClientEnumerator() {
        void* clientTable=*reinterpret_cast<void**>(this);
        impl___0CEnumConnections__QEAA_PEBXI_Z(this,nullptr,0);
        *reinterpret_cast<void**>(this)=clientTable;
    }
    ~ClientEnumerator() override { impl___1CEnumConnections__UEAA_XZ(this); }
    void OnFinalRelease() override { delete this; }
    const AFX_INTERFACEMAP* GetInterfaceMap() const override {
        return impl__GetThisInterfaceMap_CEnumConnections__KAPEBUAFX_INTERFACEMAP__XZ();
    }
    void Fail() {
        callbackModule=g_cmdTestCurrentModule;
        if(integer)throw 31;
        throw static_cast<CException*>(new GenericFailure(lifetime));
    }
    virtual BOOL OnNext(void* out) {
        if(++nextCalls==2)Fail();
        *static_cast<CONNECTDATA*>(out)={nullptr,17};++position;return TRUE;
    }
    virtual BOOL OnSkip() { if(++skipCalls==2)Fail();++position;return TRUE; }
    virtual void OnReset() { Fail(); }
    virtual void* OnClone() { Fail();return nullptr; }
    IEnumConnections* Interface() { return reinterpret_cast<IEnumConnections*>(&interfaceTable); }
};
static_assert(offsetof(ClientEnumerator,elementSize)==0x40 && offsetof(ClientEnumerator,interfaceTable)==0x68,
    "native client CEnumArray prefix");
static void events(AFX_MODULE_STATE* owner,AFX_MODULE_STATE* caller) {
    g_cmdTestCurrentModule=owner;EventTarget target;g_cmdTestCurrentModule=caller;
    VARIANT input{};input.vt=VT_BSTR;input.bstrVal=SysAllocString(L"arg");
    DISPPARAMS args{&input,nullptr,1,0};EXCEPINFO info{};UINT argError=79;
    AFX_EVENT event{};event.m_dispid=31;event.m_pDispParams=&args;event.m_pExcepInfo=&info;event.m_puArgError=&argError;
    auto invoke=[&](UINT control) { return impl__OnEvent_CCmdTarget__QEAAHIPEAUAFX_EVENT__PEAUAFX_CMDHANDLERINFO___Z(&target,control,&event,nullptr); };
    check(!invoke(10) && event.m_hResult==DISP_E_EXCEPTION && info.wCode==37 && info.scode==E_ABORT &&
        target.lifetime.deleted==1 && target.callbackModule==owner && target.lifetime.module==caller &&
        g_cmdTestCurrentModule==caller && argError==79 && args.cArgs==1 && SysStringLen(input.bstrVal)==3,
        "ranged event failures translate EXCEPINFO, preserve caller arguments and follow native cleanup module context");
    automation_exception_tests::clear(info);event.m_pExcepInfo=nullptr;
    check(!invoke(11) && event.m_hResult==DISP_E_EXCEPTION && target.lifetime.deleted==2 && argError==79,
        "events consume framework failures and preserve argument-error output when EXCEPINFO is omitted");
    event.m_pExcepInfo=&info;target.kind=2;
    check(!invoke(10) && event.m_hResult==DISP_E_EXCEPTION && info.wCode==0xf108 && target.lifetime.deleted==3,
        "event routing catches COleException through the generic framework dispatch-failure contract");
    automation_exception_tests::clear(info);target.kind=1;event.m_eventKind=1;event.m_dispid=32;
    check(!invoke(20) && event.m_hResult==DISP_E_EXCEPTION && info.wCode==37 && target.lifetime.deleted==4 &&
        target.callbackModule==caller && g_cmdTestCurrentModule==caller,
        "property-event exceptions are translated without changing native direct-callback module context");
    automation_exception_tests::clear(info);
    Lifetime borrowedLifetime;GenericFailure borrowed(borrowedLifetime,false);target.borrowed=&borrowed;target.kind=3;
    check(!invoke(20) && event.m_hResult==DISP_E_EXCEPTION && borrowedLifetime.deleted==0,
        "event cleanup respects borrowed framework exceptions");
    automation_exception_tests::clear(info);target.kind=0;bool caught=false;
    try { invoke(20); } catch(int value) { caught=value==31; }
    check(caught && g_cmdTestCurrentModule==caller,"non-framework event exceptions keep their native propagation contract");
    VariantClear(&input);
}
static void enumerators(AFX_MODULE_STATE* owner,AFX_MODULE_STATE* caller) {
    g_cmdTestCurrentModule=owner;ClientEnumerator enumeration;g_cmdTestCurrentModule=caller;
    auto* enumInterface=enumeration.Interface();CONNECTDATA output[3]{};ULONG fetched=79;
    check(enumInterface->Next(3,output,&fetched)==E_UNEXPECTED && fetched==0 && output[0].dwCookie==17 &&
        enumeration.position==1 && enumeration.lifetime.deleted==1 && enumeration.lifetime.module==owner &&
        enumeration.callbackModule==owner && g_cmdTestCurrentModule==caller,
        "shared enumerator Next catches client framework failures with native partial-progress and fetched-count behavior");
    check(enumInterface->Skip(3)==S_FALSE && enumeration.skipCalls==2 && enumeration.position==2 &&
        enumeration.lifetime.deleted==2 && g_cmdTestCurrentModule==caller,
        "shared enumerator Skip consumes framework failures and reports incomplete progress");
    IEnumConnections* clone=reinterpret_cast<IEnumConnections*>(1);
    check(enumInterface->Clone(&clone)==E_UNEXPECTED && !clone && enumeration.m_dwRef==1 && enumeration.lifetime.deleted==3 &&
        enumeration.lifetime.module==owner && g_cmdTestCurrentModule==caller,
        "shared enumerator Clone clears output and leaves owner references balanced when a client override throws");
    bool caught=false;
    try { enumInterface->Reset(); }
    catch(CException* error) { caught=true;error->Delete(); }
    check(caught && enumeration.lifetime.deleted==4 && enumeration.lifetime.module==caller && g_cmdTestCurrentModule==caller,
        "enumerator Reset preserves native framework-exception propagation while restoring module context");
    enumeration.integer=true;enumeration.nextCalls=1;caught=false;
    try { enumInterface->Next(1,output,&fetched); } catch(int value) { caught=value==31; }
    check(caught && g_cmdTestCurrentModule==caller,"shared enumerators propagate non-framework C++ exceptions");
}
class FailingPoint : public cmdtarget_connection_tests::Point {
public:
    Lifetime lifetime;bool fail=false;
    ULONG STDMETHODCALLTYPE AddRef() override {
        if(fail)throw static_cast<CException*>(new GenericFailure(lifetime));
        return cmdtarget_connection_tests::Point::AddRef();
    }
};
class ContainerTarget : public CCmdTarget {
public:
    cmdtarget_connection_tests::Point good;
    FailingPoint failing;
    ContainerTarget() { good.owner=failing.owner=this;impl__EnableConnections_CCmdTarget__QEAAXXZ(this); }
    BOOL GetExtraConnectionPoints(CPtrArray* points) override { points->Add(&good);points->Add(&failing);return TRUE; }
};
class FailingSink : public connection_point_sink_tests::Sink {
public:
    Lifetime lifetime;bool fail=false;
    ULONG STDMETHODCALLTYPE AddRef() override {
        if(fail)throw static_cast<CException*>(new GenericFailure(lifetime));
        return connection_point_sink_tests::Sink::AddRef();
    }
};
static void snapshots(AFX_MODULE_STATE* owner,AFX_MODULE_STATE* caller) {
    g_cmdTestCurrentModule=owner;ContainerTarget target;g_cmdTestCurrentModule=caller;
    auto* container=reinterpret_cast<IConnectionPointContainer*>(&target.m_xConnPtContainer);
    target.failing.fail=true;IEnumConnectionPoints* points=reinterpret_cast<IEnumConnectionPoints*>(1);
    check(container->EnumConnectionPoints(&points)==CONNECT_E_NOCONNECTION && !points && target.m_dwRef==1 &&
        target.good.references==1 && target.failing.references==1 && target.failing.lifetime.deleted==1,
        "container enumeration consumes framework failures and releases partial snapshots without releasing unacquired points");
    target.failing.fail=false;
    check(container->EnumConnectionPoints(&points)==S_OK && points && target.m_dwRef==3,
        "container enumeration remains usable after a failed snapshot");
    if(points) {
        target.failing.fail=true;IEnumConnectionPoints* clone=reinterpret_cast<IEnumConnectionPoints*>(1);
        check(points->Clone(&clone)==E_UNEXPECTED && !clone && target.m_dwRef==3 &&
            target.good.references==2 && target.failing.references==2 && target.failing.lifetime.deleted==2,
            "connection-point clone failures release only acquired references and preserve the original snapshot");
        points->Release();
    }
    check(target.m_dwRef==1 && target.good.references==1 && target.failing.references==1,
        "original connection snapshots release all retained points after a failed clone");
    connection_point_sink_tests::Sink good;FailingSink failing;
    g_cmdTestCurrentModule=owner;connection_point_sink_tests::Target sinkTarget;g_cmdTestCurrentModule=caller;
    auto* connection=sinkTarget.point.Interface();DWORD goodCookie=0,badCookie=0;
    check(connection->Advise(&good,&goodCookie)==S_OK && connection->Advise(&failing,&badCookie)==S_OK,
        "snapshot failure probes advise real native connection points");
    failing.fail=true;IEnumConnections* sinks=reinterpret_cast<IEnumConnections*>(1);
    check(connection->EnumConnections(&sinks)==E_OUTOFMEMORY && !sinks && good.references==2 && failing.references==2 &&
        failing.lifetime.deleted==1 && failing.lifetime.module==owner && g_cmdTestCurrentModule==caller,
        "sink enumeration consumes native framework failures, frees partial snapshots and restores module context");
    connection->Unadvise(goodCookie);connection->Unadvise(badCookie);
    check(good.references==1 && failing.references==1,"failed sink snapshots leave live connection references balanced");
}
}
static void frameworkBoundaryExceptions() {
    unsigned char ownerBytes[8]{},callerBytes[8]{};
    auto* owner=reinterpret_cast<AFX_MODULE_STATE*>(ownerBytes);
    auto* caller=reinterpret_cast<AFX_MODULE_STATE*>(callerBytes);
    framework_boundary_exception_tests::events(owner,caller);
    framework_boundary_exception_tests::enumerators(owner,caller);
    framework_boundary_exception_tests::snapshots(owner,caller);
    g_cmdTestCurrentModule=nullptr;
}
