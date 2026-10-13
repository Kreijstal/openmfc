#pragma once
#include "detail/CEnumConnectionsSupport.h"
extern "C" void* MS_ABI impl___0CConnectionPoint__QEAA_XZ(void*);
extern "C" void MS_ABI impl___1CConnectionPoint__UEAA_XZ(void*);
extern "C" IConnectionPointContainer* MS_ABI impl__GetContainer_CConnectionPoint__UEAAPEAUIConnectionPointContainer__XZ(void*);
extern "C" long MS_ABI impl__QuerySinkInterface_CConnectionPoint__UEAAJPEAUIUnknown__PEAPEAX_Z(void*,IUnknown*,void**);
extern "C" int MS_ABI impl__GetConnectionCount_CConnectionPoint__QEAAHXZ(void*);
extern "C" const void* MS_ABI impl__GetConnections_CConnectionPoint__QEAAPEBVCPtrArray__XZ(void*);

namespace connection_point_sink_tests {
constexpr IID eventIID={0x7701,0,0,{0,0,0,0,0,0,0,1}};
class Sink : public IUnknown {
public:
    ULONG references=1;
    AFX_MODULE_STATE* callbackModule=nullptr;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid,void** out) override {
        if(!out)return E_POINTER;*out=nullptr;
        if(iid!=IID_IUnknown && iid!=eventIID)return E_NOINTERFACE;
        *out=this;AddRef();return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { callbackModule=g_cmdTestCurrentModule;return ++references; }
    ULONG STDMETHODCALLTYPE Release() override { callbackModule=g_cmdTestCurrentModule;return --references; }
};
class Point : public CCmdTarget {
public:
    size_t offset=0;IUnknown* first=nullptr;CPtrArray* connections=nullptr;ULONG_PTR interfaceTable=0;
    int limit=-1,advised=0,unadvised=0;
    Point() { impl___0CConnectionPoint__QEAA_XZ(this); }
    ~Point() override { impl___1CConnectionPoint__UEAA_XZ(this); }
    virtual IConnectionPointContainer* GetContainer() { return impl__GetContainer_CConnectionPoint__UEAAPEAUIConnectionPointContainer__XZ(this); }
    virtual const IID& GetIID() { return eventIID; }
    virtual void OnAdvise(BOOL value) { value ? ++advised : ++unadvised; }
    virtual int GetMaxConnections() { return limit; }
    virtual HRESULT QuerySinkInterface(IUnknown* sink,void** out) {
        return impl__QuerySinkInterface_CConnectionPoint__UEAAJPEAUIUnknown__PEAPEAX_Z(this,sink,out);
    }
    IConnectionPoint* Interface() { return reinterpret_cast<IConnectionPoint*>(&interfaceTable); }
};
static_assert(offsetof(Point,offset)==0x40 && offsetof(Point,first)==0x48 &&
    offsetof(Point,connections)==0x50 && offsetof(Point,interfaceTable)==0x58,"native connection-point fields");
class Target : public CCmdTarget {
public:
    Point point;
    Target() { point.offset=offsetof(Target,point);impl__EnableConnections_CCmdTarget__QEAAXXZ(this); }
    const AFX_INTERFACEMAP* GetInterfaceMap() const override {
        static const AFX_INTERFACEMAP_ENTRY entries[]={{&IID_IConnectionPointContainer,0x30},{nullptr,SIZE_MAX}};
        static const AFX_INTERFACEMAP map{impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ,entries};return &map;
    }
    const AFX_CONNECTIONMAP* GetConnectionMap() const override {
        static const AFX_CONNECTIONMAP_ENTRY entries[]={{&eventIID,offsetof(Target,point)},{nullptr,SIZE_MAX}};
        static const AFX_CONNECTIONMAP map{nullptr,entries};return &map;
    }
};
struct EnumStorage {
    unsigned char prefix[64];size_t elementSize;void* clonedFrom;CONNECTDATA* data;
    UINT position,count;BOOL needFree;DWORD padding;void* table;UINT capacity;DWORD tail;
};
static_assert(sizeof(EnumStorage)==0x78 && offsetof(EnumStorage,table)==0x68,"native enumeration object");
static EnumStorage* Storage(IEnumConnections* enumeration) {
    return reinterpret_cast<EnumStorage*>(reinterpret_cast<char*>(enumeration)-0x68);
}
static void constructor(Sink& first,Sink& second) {
    alignas(void*) unsigned char storage[0x80];std::memset(storage,0xa5,sizeof(storage));
    auto* entries=static_cast<CONNECTDATA*>(std::malloc(2*sizeof(CONNECTDATA)));
    entries[0]={&first,41};entries[1]={&second,99};first.AddRef();second.AddRef();
    void* constructed=impl___0CEnumConnections__QEAA_PEBXI_Z(storage,entries,2);
    auto* native=reinterpret_cast<EnumStorage*>(storage);
    check(constructed==storage && native->elementSize==16 && native->data==entries && native->count==2 &&
        native->position==0 && native->needFree && native->table && native->capacity==0 && storage[0x78]==0xa5,
        "exported enumerator construction adopts CONNECTDATA storage and initializes the native prefix without overrunning it");
    CONNECTDATA result{};
    check(impl__OnNext_CEnumConnections__MEAAHPEAX_Z(storage,&result)==TRUE && result.pUnk==&first && result.dwCookie==41 &&
        first.references==3,"exported OnNext returns native TRUE and AddRefs the adopted sink");
    if(result.pUnk)result.pUnk->Release();
    check(impl__OnNext_CEnumConnections__MEAAHPEAX_Z(storage,&result)==TRUE && result.pUnk==&second && result.dwCookie==99,
        "exported OnNext advances through the caller-supplied native array");
    if(result.pUnk)result.pUnk->Release();
    check(impl__OnNext_CEnumConnections__MEAAHPEAX_Z(storage,&result)==FALSE && native->position==2,
        "exported OnNext reports native FALSE at exhaustion");
    impl___1CEnumConnections__UEAA_XZ(storage);
    check(first.references==1 && second.references==1 && storage[0x78]==0xa5,
        "exported enumerator destruction releases adopted sinks and preserves the allocation boundary");
}
static void enumeration() {
    unsigned char ownerBytes[8]{},callerBytes[8]{};
    auto* owner=reinterpret_cast<AFX_MODULE_STATE*>(ownerBytes);
    auto* caller=reinterpret_cast<AFX_MODULE_STATE*>(callerBytes);
    Sink first,second,third;
    g_cmdTestCurrentModule=owner;constructor(first,second);Target target;g_cmdTestCurrentModule=caller;
    auto* container=reinterpret_cast<IConnectionPointContainer*>(&target.m_xConnPtContainer);
    IConnectionPoint* point=nullptr;
    check(container->FindConnectionPoint(eventIID,&point)==S_OK && point==target.point.Interface() && target.m_dwRef==2,
        "real connection points expose their native +0x58 table through container lookup");
    if(!point)return;
    IID iid{};IConnectionPointContainer* parent=nullptr;
    check(point->GetConnectionInterface(&iid)==S_OK && iid==eventIID &&
        point->GetConnectionPointContainer(&parent)==S_OK && parent==container && target.m_dwRef==3,
        "connection-point virtuals return the event IID and queried owning container");
    if(parent)parent->Release();
    IEnumConnections* empty=nullptr;
    CONNECTDATA result{};ULONG fetched=99;
    check(point->EnumConnections(&empty)==S_OK && empty && empty->Next(1,&result,&fetched)==S_FALSE && fetched==0,
        "unadvised connection points return an empty native IEnumConnections interface");
    if(empty)empty->Release();
    DWORD firstCookie=0,secondCookie=0,thirdCookie=0;
    check(point->Advise(&first,&firstCookie)==S_OK && firstCookie==1 && first.references==2 && target.point.advised==1,
        "first advise stores the queried sink with cookie one and invokes the client notification");
    IEnumConnections* single=nullptr;
    check(point->EnumConnections(&single)==S_OK && single && first.references==3 &&
        Storage(single)->elementSize==sizeof(CONNECTDATA) && Storage(single)->count==1 && Storage(single)->capacity==2,
        "sink enumeration installs a native 120-byte object and retains the first-connection snapshot");
    if(single) {
        void* identity=nullptr;
        check(single->QueryInterface(IID_IEnumConnections,&identity)==S_OK && identity==single,
            "native enumerator interface maps expose IEnumConnections through owner QueryInterface");
        if(identity)static_cast<IEnumConnections*>(identity)->Release();
        check(single->Next(1,&result,&fetched)==S_OK && fetched==1 && result.pUnk==&first && result.dwCookie==1 &&
            first.references==4 && first.callbackModule==owner && g_cmdTestCurrentModule==caller,
            "enumerator Next returns CONNECTDATA, AddRefs the sink and restores the caller module context");
        if(result.pUnk)result.pUnk->Release();single->Release();
    }
    check(point->Advise(&second,&secondCookie)==S_OK && secondCookie==2 &&
        point->Advise(&third,&thirdCookie)==S_OK && thirdCookie==3 &&
        target.point.connections && target.point.connections->GetSize()==3 && target.point.connections->GetAt(0)==&first,
        "multiple advises use a real native CPtrArray that shares storage with exported collection methods");
    check(point->Unadvise(secondCookie)==S_OK && second.references==1 && target.point.connections->GetAt(1)==nullptr &&
        impl__GetConnectionCount_CConnectionPoint__QEAAHXZ(&target.point)==2,
        "unadvise leaves a cookie-preserving hole and updates the real pointer-array contents");
    IEnumConnections* original=nullptr;
    check(point->EnumConnections(&original)==S_OK && original && Storage(original)->count==2,
        "connection snapshots skip unadvised holes while retaining live sinks");
    if(original) {
        check(original->Next(1,&result,nullptr)==S_OK && result.pUnk==&first && result.dwCookie==1,
            "connection snapshots retain each sink's original cookie");
        if(result.pUnk)result.pUnk->Release();
        IEnumConnections* clone=nullptr;IEnumConnections* descendant=nullptr;
        check(original->Clone(&clone)==S_OK && clone && Storage(clone)->position==1 &&
            Storage(clone)->clonedFrom==Storage(original) && !Storage(clone)->needFree && first.references==3 && third.references==3,
            "native clones borrow the snapshot, preserve the cursor and retain the original enumerator without extra sink references");
        if(clone)check(clone->Clone(&descendant)==S_OK && descendant && Storage(descendant)->clonedFrom==Storage(original),
            "clones of clones retain the original snapshot owner");
        original->Release();
        check(point->Unadvise(firstCookie)==S_OK && point->Unadvise(thirdCookie)==S_OK && first.references==2 && third.references==2,
            "cloned snapshots keep sinks alive after all live connections are unadvised");
        if(clone) {
            check(clone->Next(1,&result,nullptr)==S_OK && result.pUnk==&third && result.dwCookie==3,
                "cloned snapshots remain usable after the original interface and live connections are released");
            if(result.pUnk)result.pUnk->Release();
            clone->Reset();
            check(clone->Skip(1)==S_OK && clone->Skip(2)==S_FALSE && clone->Skip(0)==S_OK,
                "native enumerator reset and skip honor counts and exhaustion");
            clone->Release();
        }
        if(descendant) {
            check(descendant->Next(1,&result,&fetched)==S_OK && result.pUnk==&third && result.dwCookie==3,
                "descendant enumeration advances independently of its parent clone");
            if(result.pUnk)result.pUnk->Release();descendant->Release();
        }
    }
    check(first.references==1 && second.references==1 && third.references==1 &&
        impl__GetConnectionCount_CConnectionPoint__QEAAHXZ(&target.point)==0,
        "the last snapshot release frees the original enumerator and balances all sink references");
    target.point.limit=1;
    check(point->Advise(&first,&firstCookie)==S_OK && firstCookie==1 &&
        point->Advise(&second,&secondCookie)==CONNECT_E_ADVISELIMIT && second.references==1,
        "connection points reuse free cookies and enforce client connection limits");
    point->Unadvise(firstCookie);
    check(point->Unadvise(firstCookie)==CONNECT_E_NOCONNECTION && point->Release()==1,
        "unadvise rejects stale cookies and point release balances the owner reference");
    g_cmdTestCurrentModule=nullptr;
}
}
static void connectionPointSinks() { connection_point_sink_tests::enumeration(); }
