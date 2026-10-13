#pragma once
#include "detail/CCmdTargetConnectionsSupport.h"
namespace cmdtarget_connection_tests {
constexpr IID childIID={0x4401,0,0,{0,0,0,0,0,0,0,1}};
constexpr IID parentIID={0x4402,0,0,{0,0,0,0,0,0,0,2}};
constexpr IID hookIID={0x4403,0,0,{0,0,0,0,0,0,0,3}};
constexpr IID missingIID={0x4404,0,0,{0,0,0,0,0,0,0,4}};
struct NativeArray { void* table;void** data;INT_PTR size,capacity,grow; };
class Point : public IConnectionPoint {
public:
    CCmdTarget* owner=nullptr;
    ULONG references=1;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid,void** out) override {
        if(!out)return E_POINTER;*out=nullptr;
        if(iid!=IID_IUnknown && iid!=IID_IConnectionPoint)return E_NOINTERFACE;
        *out=this;AddRef();return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { ++references;return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(owner); }
    ULONG STDMETHODCALLTYPE Release() override { --references;return impl__ExternalRelease_CCmdTarget__QEAAKXZ(owner); }
    HRESULT STDMETHODCALLTYPE GetConnectionInterface(IID*) override { std::abort(); }
    HRESULT STDMETHODCALLTYPE GetConnectionPointContainer(IConnectionPointContainer**) override { std::abort(); }
    HRESULT STDMETHODCALLTYPE Advise(IUnknown*,DWORD*) override { std::abort(); }
    HRESULT STDMETHODCALLTYPE Unadvise(DWORD) override { std::abort(); }
    HRESULT STDMETHODCALLTYPE EnumConnections(IEnumConnections**) override { std::abort(); }
};
struct MappedPoint { unsigned char prefix[0x58]{};Point point; };
static_assert(offsetof(MappedPoint,point)==0x58,"native embedded connection interface offset");
class Target : public CCmdTarget {
public:
    MappedPoint child,parent;
    Point extra,hook;
    bool extras=true,mapped=true,extraLayout=false;
    int finals=0;
    Target() { child.point.owner=parent.point.owner=extra.owner=hook.owner=this; }
    void OnFinalRelease() override { ++finals; }
    const AFX_INTERFACEMAP* GetInterfaceMap() const override {
        static const AFX_INTERFACEMAP_ENTRY entries[]={{&IID_IConnectionPointContainer,0x30},{nullptr,SIZE_MAX}};
        static const AFX_INTERFACEMAP map{impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ,entries};return &map;
    }
    static const AFX_CONNECTIONMAP* AFXAPI ParentMap() {
        static const AFX_CONNECTIONMAP_ENTRY entries[]={{&parentIID,offsetof(Target,parent)},{nullptr,SIZE_MAX}};
        static const AFX_CONNECTIONMAP map{nullptr,entries};return &map;
    }
    const AFX_CONNECTIONMAP* GetConnectionMap() const override {
        if(!mapped)return static_cast<const AFX_CONNECTIONMAP*>(impl__GetThisConnectionMap_CCmdTarget__KAPEBUAFX_CONNECTIONMAP__XZ());
        static const AFX_CONNECTIONMAP_ENTRY entries[]={{&childIID,offsetof(Target,child)},{nullptr,SIZE_MAX}};
        static const AFX_CONNECTIONMAP map{ParentMap,entries};return &map;
    }
    IConnectionPoint* GetConnectionHook(const IID& iid) override { return iid==hookIID ? &hook : nullptr; }
    BOOL GetExtraConnectionPoints(CPtrArray* array) override {
        auto* native=reinterpret_cast<NativeArray*>(array);
        extraLayout=sizeof(CPtrArray)==0x28 && native->table && !native->data && !native->size;
        array->Add(&extra);
        extraLayout=extraLayout && native->size==1 && native->capacity>=1 && native->data[0]==&extra;
        return extras;
    }
};
static void arrays() {
    CPtrArray values;
    auto* native=reinterpret_cast<NativeArray*>(&values);
    int a=1,b=2,c=3;
    values.SetSize(2,3);values.SetAt(0,&a);values.SetAt(1,&b);
    check(sizeof(values)==40 && native->size==2 && native->capacity>=2 && native->grow==3 &&
        native->data[0]==&a && native->data[1]==&b,
        "CPtrArray publishes the native pointer, size, capacity and growth fields");
    native->data[1]=&c;
    check(values.GetAt(1)==&c,"native inline writes share the pointer-array backend storage");
    values.Add(&b);values.InsertAt(1,&a);values.RemoveAt(0);
    check(native->size==3 && native->data[0]==&a && native->data[1]==&c && native->data[2]==&b,
        "pointer-array native fields stay synchronized after allocation, insertion and removal");
    CPtrArray copy;copy.Copy(values);copy.Append(values);copy.FreeExtra();
    auto* copied=reinterpret_cast<NativeArray*>(&copy);
    check(copied->size==6 && copied->capacity==6 && copied->data[4]==&c,
        "pointer-array copy, append and compaction refresh native storage");
    values.RemoveAll();
    check(!native->data && native->size==0 && native->capacity==0,
        "pointer-array removal clears the native member block");
}
static void connections() {
    arrays();Target target;
    check(!target.GetInterface(&IID_IConnectionPointContainer),"connection interface remains hidden before activation");
    impl__EnableConnections_CCmdTarget__QEAAXXZ(&target);
    auto* container=reinterpret_cast<IConnectionPointContainer*>(&target.m_xConnPtContainer);
    void* queried=nullptr;
    check(target.GetInterface(&IID_IConnectionPointContainer)==container &&
        container->QueryInterface(IID_IConnectionPointContainer,&queried)==S_OK && queried==container && target.m_dwRef==2,
        "connection activation installs the physical +0x30 interface with owner-backed QueryInterface");
    IUnknown* identity=nullptr;
    check(container->QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&identity))==S_OK && identity==container &&
        target.m_dwRef==3,"connection-container identity and AddRef share the command-target lifetime");
    if(identity)identity->Release();
    check(container->AddRef()==3 && container->Release()==2,"container AddRef and Release update the owner's reference count");
    IConnectionPoint* found=nullptr;
    check(container->FindConnectionPoint(childIID,&found)==S_OK && found==&target.child.point && target.child.point.references==2,
        "connection lookup finds the derived map's native +0x58 interface and AddRefs it");
    if(found)found->Release();
    check(container->FindConnectionPoint(parentIID,&found)==S_OK && found==&target.parent.point,
        "connection lookup walks inherited connection maps");
    if(found)found->Release();
    check(container->FindConnectionPoint(hookIID,&found)==S_OK && found==&target.hook,
        "connection lookup honors the derived virtual hook");
    if(found)found->Release();
    found=reinterpret_cast<IConnectionPoint*>(1);
    check(container->FindConnectionPoint(missingIID,&found)==CONNECT_E_NOCONNECTION && !found,
        "unknown connection IIDs clear the output and return CONNECT_E_NOCONNECTION");
    IEnumConnectionPoints* enumeration=nullptr;
    check(container->EnumConnectionPoints(&enumeration)==S_OK && enumeration && target.extraLayout && target.m_dwRef==5,
        "connection enumeration snapshots extra, derived and inherited points and holds their owner alive");
    if(!enumeration) { container->Release();return; }
    ULONG fetched=99;IConnectionPoint* points[4]{};
    check(enumeration->Next(2,points,nullptr)==E_POINTER &&
        enumeration->Next(0,nullptr,&fetched)==S_OK && fetched==0,
        "connection enumeration validates outputs without consuming its cursor");
    check(enumeration->Next(2,points,&fetched)==S_OK && fetched==2 && points[0]==&target.extra && points[1]==&target.child.point,
        "enumeration returns extra points before derived map points and owns each returned reference");
    for(ULONG i=0;i<fetched;++i)points[i]->Release();
    IEnumConnectionPoints* clone=nullptr;
    check(enumeration->Clone(&clone)==S_OK && clone,
        "connection enumerator cloning preserves a retained snapshot");
    if(clone) {
        check(clone->Next(1,points,nullptr)==S_OK && points[0]==&target.parent.point,
            "cloned connection enumerators preserve the current cursor");
        points[0]->Release();clone->Reset();
        check(clone->Skip(2)==S_OK && clone->Skip(2)==S_FALSE && clone->Skip(0)==S_OK,
            "enumerator reset and skip handle exact counts, exhaustion and zero counts");
        clone->Release();
    }
    check(enumeration->Next(4,points,&fetched)==S_FALSE && fetched==1 && points[0]==&target.parent.point,
        "original enumeration advances independently and reports partial results at exhaustion");
    if(fetched)points[0]->Release();
    enumeration->Release();
    check(target.m_dwRef==2 && target.extra.references==1 && target.child.point.references==1 && target.parent.point.references==1,
        "destroying connection enumerators releases every snapshot reference");
    target.extras=false;enumeration=nullptr;
    check(container->EnumConnectionPoints(&enumeration)==S_OK && enumeration,
        "extra-point callbacks that return FALSE leave mapped points enumerable");
    if(enumeration) {
        check(enumeration->Next(4,points,&fetched)==S_FALSE && fetched==2 && points[0]==&target.child.point && points[1]==&target.parent.point,
            "FALSE extra-point callbacks contribute no points to the snapshot");
        for(ULONG i=0;i<fetched;++i)points[i]->Release();enumeration->Release();
    }
    target.mapped=false;enumeration=nullptr;
    check(container->EnumConnectionPoints(&enumeration)==S_OK && enumeration && target.m_dwRef==2,
        "empty connection maps produce a real empty enumerator without retaining unrelated owner references");
    if(enumeration) {
        fetched=99;
        check(enumeration->Next(1,points,&fetched)==S_FALSE && fetched==0 && enumeration->Skip(0)==S_OK,
            "empty connection enumerators report exhaustion and accept zero-count skips");
        enumeration->Release();
    }
    check(container->Release()==1 && target.m_dwRef==1,"releasing the queried container balances owner references");
    cmdtarget_dispatch_tests::CountedUnknown outer;
    target.m_pOuterUnknown=&outer;
    identity=nullptr;
    check(container->QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&identity))==S_OK && identity==&outer &&
        outer.references==2 && target.m_dwRef==1,
        "aggregated connection containers delegate identity and references to the controlling unknown");
    if(identity)identity->Release();
    check(container->AddRef()==2 && container->Release()==1 && outer.references==1 && target.m_dwRef==1,
        "aggregated container AddRef and Release leave the internal owner count untouched");
    target.m_pOuterUnknown=nullptr;
}
}
static void cmdtargetConnections() { cmdtarget_connection_tests::connections(); }
