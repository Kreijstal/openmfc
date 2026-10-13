#pragma once
#include "detail/OleVerbSupport.h"
#include "openmfc/afxdisp.h"

namespace cmdtarget_verb_tests {
class Base : public CCmdTarget {
public:
    int calls=0,selected=0;
    bool accept=true,raise=false;
    MSG* message=nullptr;
    HWND parent=nullptr;
    const RECT* rectangle=nullptr;
    BOOL Handle(int verb,MSG* msg,HWND window,const RECT* rect) {
        ++calls;selected=verb;message=msg;parent=window;rectangle=rect;
        if(raise)throw 37;
        return accept;
    }
    BOOL Inherited(MSG* m,HWND h,const RECT* r) { return Handle(3,m,h,r); }
    BOOL Open(MSG* m,HWND h,const RECT* r) { return Handle(4,m,h,r); }
    DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(Base,CCmdTarget)
    ON_OLEVERB(999,Base::Inherited)
    ON_STDOLEVERB(OLEIVERB_OPEN,Base::Open)
END_MESSAGE_MAP()

class Target : public Base {
public:
    BOOL First(MSG* m,HWND h,const RECT* r) { return Handle(1,m,h,r); }
    BOOL Second(MSG* m,HWND h,const RECT* r) { return Handle(2,m,h,r); }
    BOOL Activate(MSG* m,HWND h,const RECT* r) { return Handle(5,m,h,r); }
    DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(Target,Base)
    ON_OLEVERB(101,Target::First)
    ON_OLEVERB(102,Target::Second)
    ON_STDOLEVERB(OLEIVERB_UIACTIVATE,Target::Activate)
    {0xc002,0,UINT(OLEIVERB_INPLACEACTIVATE),UINT(OLEIVERB_HIDE),UINT(-1),
        (AFX_PMSG)(BOOL (AFXAPI CCmdTarget::*)(MSG*,HWND,const RECT*))&Target::Activate},
    // These records must not count toward custom verb ordinals.
    {0xc002,1,1,1,101,(AFX_PMSG)0},
    {WM_COMMAND,0,1,1,101,(AFX_PMSG)0},
END_MESSAGE_MAP()

class Many : public CCmdTarget {
public:
    BOOL Verb(MSG*,HWND,const RECT*) { return TRUE; }
    static const AFX_MSGMAP* AFXAPI Root() { return CCmdTarget::GetThisMessageMap(); }
    const AFX_MSGMAP* GetMessageMap() const override {
        static AFX_MSGMAP_ENTRY entries[14]{};
        for(UINT i=0;i<13;++i)entries[i]={0xc002,0,1,1,101,
            (AFX_PMSG)(BOOL (AFXAPI CCmdTarget::*)(MSG*,HWND,const RECT*))&Many::Verb};
        static const AFX_MSGMAP map{Root,entries};return &map;
    }
};

static void verbs() {
    Target target;
    MSG message{};RECT rectangle{1,2,3,4};HWND parent=reinterpret_cast<HWND>(UINT_PTR(0x1234));
    check(target.DoOleVerb(0,&message,parent,&rectangle) && target.selected==1 &&
        target.message==&message && target.parent==parent && target.rectangle==&rectangle,
        "custom OLE verbs invoke real macro-generated member pointers with native arguments");
    check(target.DoOleVerb(1,nullptr,nullptr,nullptr) && target.selected==2 &&
        target.DoOleVerb(2,nullptr,nullptr,nullptr) && target.selected==3,
        "custom verb ordinals follow derived entries then inherited entries");
    check(target.DoOleVerb(OLEIVERB_OPEN,nullptr,nullptr,nullptr) && target.selected==4 &&
        target.DoOleVerb(OLEIVERB_UIACTIVATE,nullptr,nullptr,nullptr) && target.selected==5 &&
        target.DoOleVerb(OLEIVERB_HIDE,nullptr,nullptr,nullptr) &&
        target.DoOleVerb(OLEIVERB_INPLACEACTIVATE,nullptr,nullptr,nullptr),
        "standard OLE verbs match inherited handlers and unsigned negative ranges");
    int before=target.calls;
    check(!target.DoOleVerb(3,nullptr,nullptr,nullptr) && !target.DoOleVerb(-99,nullptr,nullptr,nullptr) &&
        target.calls==before,"unmapped OLE verbs return FALSE without calling a handler");
    target.accept=false;bool rejected=false;
    try { target.DoOleVerb(0,nullptr,nullptr,nullptr); }
    catch(COleException* error) { rejected=error->m_sc==S_OK;error->Delete(); }
    check(rejected && target.calls==before+1,"a matched FALSE handler throws native COleException with S_OK");
    target.accept=true;target.raise=true;bool propagated=false;
    try { target.DoOleVerb(0,nullptr,nullptr,nullptr); }catch(int error) { propagated=error==37; }
    check(propagated,"OLE verb handler exceptions propagate to their caller");target.raise=false;

    CCmdTarget empty;
    IEnumOLEVERB* enumeration=reinterpret_cast<IEnumOLEVERB*>(UINT_PTR(1));
    check(!empty.EnumOleVerbs(&enumeration) && !enumeration &&
        !empty.DoOleVerb(0,nullptr,nullptr,nullptr),"the terminal root message map has no OLE verbs");
    bool invalid=false;try { target.EnumOleVerbs(nullptr); }catch(int error) { invalid=error==19; }
    check(invalid,"OLE verb enumeration throws for a null output pointer");
    check(target.EnumOleVerbs(&enumeration) && enumeration,"custom verbs expose a native IEnumOLEVERB interface");
    if(!enumeration)return;
    auto* owner=reinterpret_cast<CEnumOleVerb*>(reinterpret_cast<char*>(enumeration)-0x68);
    check(owner->m_elementSize==sizeof(OLEVERB) && owner->m_count==3 && owner->m_dwRef==1,
        "verb enumerators use the native CCmdTarget/CEnumArray storage and interface offset");
    IEnumOLEVERB* queried=nullptr;
    HRESULT queriedResult=enumeration->QueryInterface(IID_IEnumOLEVERB,reinterpret_cast<void**>(&queried));
    check(queriedResult==S_OK && queried==enumeration && owner->m_dwRef==2,
        "IEnumOLEVERB QueryInterface returns the actual retained interface");
    if(queried)queried->Release();
    OLEVERB values[4]{};ULONG fetched=99;
    HRESULT next=enumeration->Next(4,values,&fetched);
    check(next==S_FALSE && fetched==3 && values[0].lVerb==0 && values[1].lVerb==1 && values[2].lVerb==2 &&
        !wcscmp(values[0].lpszVerbName,L"Edit verb") && !wcscmp(values[1].lpszVerbName,L"\x7de8\x96c6 verb") &&
        !wcscmp(values[2].lpszVerbName,L"<unknown verb>") &&
        values[0].fuFlags==0 && values[0].grfAttribs==OLEVERBATTRIB_ONCONTAINERMENU &&
        values[0].lpszVerbName!=owner->m_verbs[0].lpszVerbName,
        "enumeration returns custom-only ordered verbs, Unicode resources, fallback names and owned name copies");
    for(ULONG i=0;i<fetched;++i)CoTaskMemFree(values[i].lpszVerbName);
    check(enumeration->Reset()==S_OK && enumeration->Skip(2)==S_OK && enumeration->Skip(1)==S_FALSE &&
        owner->m_position==3 && enumeration->Next(1,values,&fetched)==S_FALSE && fetched==0,
        "native Skip reports S_FALSE when advancing onto the final cursor position");
    enumeration->Reset();enumeration->Skip(1);
    IEnumOLEVERB* clone=nullptr;
    check(enumeration->Clone(&clone)==S_OK && clone && owner->m_dwRef==2,
        "verb clones retain their root array and copy the current cursor");
    if(clone) {
        enumeration->Reset();
        check(clone->Next(1,values,&fetched)==S_OK && fetched==1 && values[0].lVerb==1 &&
            values[0].lpszVerbName==owner->m_verbs[1].lpszVerbName && owner->m_position==0,
            "retail generic clones preserve independent cursors and shallow-copy verb entries");
        // Retail's base clone returns borrowed names, unlike the original
        // CEnumOleVerb's allocating copier. Do not free these borrowed values.
        enumeration->Release();enumeration=nullptr;
        check(clone->Next(1,values,&fetched)==S_OK && fetched==1 && values[0].lVerb==2 &&
            !wcscmp(values[0].lpszVerbName,L"<unknown verb>"),
            "a clone keeps the root verb names alive after the original interface is released");
        clone->Release();
    }
    if(enumeration)enumeration->Release();
    Many many;
    enumeration=nullptr;
    check(many.EnumOleVerbs(&enumeration) && enumeration,"verb enumeration grows beyond the initial ten-entry allocation");
    if(enumeration) {
        OLEVERB grown[14]{};fetched=0;
        check(enumeration->Next(14,grown,&fetched)==S_FALSE && fetched==13 && grown[12].lVerb==12 &&
            !wcscmp(grown[12].lpszVerbName,L"Edit verb"),
            "grown verb arrays retain every name and sequential ordinal");
        for(ULONG i=0;i<fetched;++i)CoTaskMemFree(grown[i].lpszVerbName);
        enumeration->Release();
    }
}
}
static void cmdtargetVerbs() { cmdtarget_verb_tests::verbs(); }
