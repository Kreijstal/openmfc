#pragma once
#include "detail/CCmdTargetEventsSupport.h"

namespace cmdtarget_event_tests {
template<class Member> static void* address(Member member) {
    static_assert(sizeof(member)==sizeof(void*), "native single-inheritance member pointer");
    void* pointer=nullptr;std::memcpy(&pointer,&member,sizeof(pointer));return pointer;
}
class Target : public CCmdTarget {
public:
    UINT control=0;
    LONG value=0;
    int calls=0,state=0,reason=0;
    AFX_MODULE_STATE* module=nullptr;
    BOOL Event(LONG number) { ++calls;value=number;module=g_cmdTestCurrentModule;return TRUE; }
    BOOL Range(UINT id,LONG number) { control=id;return Event(number); }
    BOOL Request(BOOL* allow) { ++calls;*allow=FALSE;return TRUE; }
    BOOL Changed() { ++calls;return TRUE; }
    BOOL Notify(UINT id,int nextState,int nextReason,BOOL* allow) {
        ++calls;control=id;state=nextState;reason=nextReason;*allow=FALSE;return TRUE;
    }
    static const AFX_EVENTSINKMAP* AFXAPI BaseMap() {
        static const char parameters[]={VT_I4,0};
        static const AFX_EVENTSINKMAP_ENTRY entries[]={
            {{nullptr,13,parameters,VT_BOOL,address(&Target::Event),nullptr,0,afxDispCustom_},UINT_MAX,UINT_MAX},
            {{nullptr,11,parameters,VT_BOOL,address(&Target::Event),nullptr,0,afxDispCustom_},101,UINT_MAX},
            {{nullptr,-1,nullptr,0,nullptr,nullptr,SIZE_MAX,afxDispCustom_},0,0}
        };
        static const AFX_EVENTSINKMAP map{nullptr,entries,nullptr};return &map;
    }
    const AFX_EVENTSINKMAP* GetEventSinkMap() const override {
        static const char parameters[]={VT_I4,0},rangeParameters[]={VT_I4,VT_I4,0};
        static const AFX_EVENTSINKMAP_ENTRY entries[]={
            {{nullptr,11,parameters,VT_BOOL,address(&Target::Event),nullptr,0,afxDispCustom_},100,UINT_MAX},
            {{nullptr,12,rangeParameters,VT_BOOL,address(&Target::Range),nullptr,0,afxDispCustom_},200,202},
            {{nullptr,21,nullptr,VT_BOOL,address(&Target::Request),address(&Target::Changed),1,afxDispCustom_},300,UINT_MAX},
            {{nullptr,22,nullptr,VT_BOOL,address(&Target::Notify),nullptr,1,afxDispCustom_},400,402},
            {{nullptr,-1,nullptr,0,nullptr,nullptr,SIZE_MAX,afxDispCustom_},0,0}
        };
        static const AFX_EVENTSINKMAP map{BaseMap,entries,nullptr};return &map;
    }
};
static const AFX_EVENTSINKMAP_ENTRY* lookup(Target& target,UINT id,AFX_EVENT& event) {
    return static_cast<const AFX_EVENTSINKMAP_ENTRY*>(
        impl__GetEventSinkEntry_CCmdTarget__IEAAPEBUAFX_EVENTSINKMAP_ENTRY__IPEAUAFX_EVENT___Z(&target,id,&event));
}
static BOOL invoke(Target& target,UINT id,AFX_EVENT& event,void* info=nullptr) {
    return impl__OnEvent_CCmdTarget__QEAAHIPEAUAFX_EVENT__PEAUAFX_CMDHANDLERINFO___Z(&target,id,&event,info);
}
static void events() {
    unsigned char ownerBytes[8]{},callerBytes[8]{};
    auto* owner=reinterpret_cast<AFX_MODULE_STATE*>(ownerBytes);
    auto* caller=reinterpret_cast<AFX_MODULE_STATE*>(callerBytes);
    g_cmdTestCurrentModule=owner;Target target;g_cmdTestCurrentModule=caller;
    VARIANT input{};input.vt=VT_I4;input.lVal=42;
    DISPPARAMS parameters{&input,nullptr,1,0};
    AFX_EVENT event{};event.m_dispid=11;event.m_pDispParams=&parameters;
    check(lookup(target,100,event)==&target.GetEventSinkMap()->lpEntries[0] &&
        lookup(target,101,event)==&Target::BaseMap()->lpEntries[1] && !lookup(target,102,event),
        "event lookup dispatches the client's virtual map and falls back through inherited maps by control ID");
    check(invoke(target,100,event) && target.calls==1 && target.value==42 && event.m_hResult==S_OK &&
        target.module==owner && g_cmdTestCurrentModule==caller,
        "mapped control events reach native member handlers with the owner module context");
    struct HandlerInfo { CCmdTarget* target;void* member; } info{};
    check(invoke(target,100,event,&info) && info.target==&target && info.member==address(&Target::Event) && target.calls==1,
        "handler queries return the client target and actual member pointer without invoking it");
    event.m_dispid=12;
    check(lookup(target,200,event) && lookup(target,202,event) && !lookup(target,199,event) && !lookup(target,203,event),
        "event ranges include both boundaries and reject neighboring control IDs");
    input.vt=VT_BSTR;input.bstrVal=SysAllocString(L"57");
    check(invoke(target,201,event) && target.control==201 && target.value==57 && event.m_hResult==S_OK &&
        parameters.cArgs==1 && parameters.rgvarg==&input && input.vt==VT_BSTR && SysStringLen(input.bstrVal)==2,
        "ranged events prepend the control ID in C++ argument order without changing or clearing caller arguments");
    VariantClear(&input);
    input.vt=VT_I4;input.lVal=63;event.m_dispid=13;
    check(invoke(target,999,event) && target.value==63,
        "inherited wildcard event entries accept arbitrary control IDs");
    UINT error=99;event.m_puArgError=&error;event.m_dispid=12;
    input.vt=VT_BSTR;input.bstrVal=SysAllocString(L"invalid");int before=target.calls;
    check(!invoke(target,200,event) && event.m_hResult==DISP_E_TYPEMISMATCH && error==0 && target.calls==before,
        "ranged event coercion failures report the caller's reversed argument index and skip the handler");
    VariantClear(&input);
    event.m_eventKind=1;event.m_dispid=21;
    check(lookup(target,300,event) && invoke(target,300,event) && event.m_hResult==S_FALSE,
        "property-request handlers can veto a change through their native BOOL reference");
    event.m_eventKind=0;
    check(!lookup(target,300,event), "property entries do not match ordinary events with the same DISPID");
    event.m_eventKind=2;info={};before=target.calls;
    check(invoke(target,300,event,&info) && info.member==address(&Target::Changed) && target.calls==before &&
        invoke(target,300,event) && event.m_hResult==S_OK && target.calls==before+1,
        "property-change queries and notifications select the setter handler");
    event.m_eventKind=3;event.m_dispid=22;event.m_nDSCState=7;event.m_nDSCReason=9;
    check(invoke(target,402,event) && target.control==402 && target.state==7 && target.reason==9 && event.m_hResult==S_FALSE,
        "ranged data-source notifications forward control ID, state, reason, and permission result");
    event.m_dispid=99;event.m_hResult=E_PENDING;
    check(!invoke(target,402,event) && event.m_hResult==E_PENDING,
        "unmatched events remain unhandled without overwriting the existing event result");
    bool caught=false;
    try { impl__GetEventSinkEntry_CCmdTarget__IEAAPEBUAFX_EVENTSINKMAP_ENTRY__IPEAUAFX_EVENT___Z(&target,0,nullptr); }
    catch(int code) { caught=code==19; }
    check(caught, "null event lookup uses the framework invalid-argument exception path");
    g_cmdTestCurrentModule=nullptr;
}
}
static void cmdtargetEvents() { cmdtarget_event_tests::events(); }
