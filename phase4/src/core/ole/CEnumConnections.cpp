// Native CEnumConnections: CEnumArray prefix, CONNECTDATA ownership and COM table.
#define OPENMFC_APPCORE_IMPL
#include "detail/CEnumConnectionsSupport.h"
#include "detail/CEnumArrayInterfacesSupport.h"
#include <cstdlib>
#include <cstring>
#include <limits>
#include <new>

extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
namespace {
struct NativeEnumConnections;
IEnumConnections* Interface(NativeEnumConnections*);
extern void* const interfaceTable[7];
const AFX_INTERFACEMAP* Map() {
    static const AFX_INTERFACEMAP_ENTRY entries[]={{&IID_IEnumConnections,0x68},{nullptr,SIZE_MAX}};
    static const AFX_INTERFACEMAP map{impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ,entries};
    return &map;
}
struct NativeEnumConnections : CCmdTarget {
    size_t elementSize=sizeof(CONNECTDATA);                 // +0x40
    NativeEnumConnections* clonedFrom=nullptr;              // +0x48
    CONNECTDATA* data=nullptr;                              // +0x50
    UINT position=0,count=0;                               // +0x58/+0x5c
    BOOL needFree=TRUE;DWORD padding=0;                     // +0x60/+0x64
    void* const* enumTable=interfaceTable;                  // +0x68
    UINT capacity=0;DWORD tailPadding=0;                    // +0x70
    NativeEnumConnections(const void* entries=nullptr,UINT size=0)
        : data(static_cast<CONNECTDATA*>(const_cast<void*>(entries))),count(size) {}
    ~NativeEnumConnections() override {
        if(clonedFrom)impl__InternalRelease_CCmdTarget__QEAAKXZ(clonedFrom);
        else for(UINT i=0;i<count;++i)if(data[i].pUnk)data[i].pUnk->Release();
        if(needFree)std::free(data);
    }
    void OnFinalRelease() override { delete this; }
    const AFX_INTERFACEMAP* GetInterfaceMap() const override { return Map(); }
    virtual BOOL OnNext(void* out) {
        if(position>=count)return FALSE;
        auto* result=static_cast<CONNECTDATA*>(out);
        *result=data[position++];
        if(result->pUnk)result->pUnk->AddRef();
        return TRUE;
    }
    virtual BOOL OnSkip() { if(position>=count)return FALSE;++position;return TRUE; }
    virtual void OnReset() { position=0; }
    virtual void* OnClone() {
        auto* clone=new(std::nothrow)NativeEnumConnections(data,count);
        if(clone) { clone->needFree=FALSE;clone->position=position; }
        return clone;
    }
};
static_assert(sizeof(NativeEnumConnections)==0x78,"native CEnumConnections storage");
static_assert(offsetof(NativeEnumConnections,elementSize)==0x40,"CEnumArray element size");
static_assert(offsetof(NativeEnumConnections,clonedFrom)==0x48,"CEnumArray clone owner");
static_assert(offsetof(NativeEnumConnections,data)==0x50,"CEnumArray data");
static_assert(offsetof(NativeEnumConnections,position)==0x58,"CEnumArray cursor");
static_assert(offsetof(NativeEnumConnections,count)==0x5c,"CEnumArray count");
static_assert(offsetof(NativeEnumConnections,enumTable)==0x68,"embedded enumeration interface");
static_assert(offsetof(NativeEnumConnections,capacity)==0x70,"connection-array capacity");
NativeEnumConnections* Owner(void* self) {
    return reinterpret_cast<NativeEnumConnections*>(static_cast<char*>(self)-0x68);
}
IEnumConnections* Interface(NativeEnumConnections* owner) {
    return reinterpret_cast<IEnumConnections*>(&owner->enumTable);
}
HRESULT MS_ABI Query(void* self,REFIID iid,void** out) {
    return static_cast<HRESULT>(impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(Owner(self),&iid,out));
}
ULONG MS_ABI AddRef(void* self) { return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(Owner(self)); }
ULONG MS_ABI Release(void* self) { return impl__ExternalRelease_CCmdTarget__QEAAKXZ(Owner(self)); }
void* const interfaceTable[]={reinterpret_cast<void*>(Query),reinterpret_cast<void*>(AddRef),
    reinterpret_cast<void*>(Release),reinterpret_cast<void*>(impl__Next_XEnumVOID_CEnumArray__UEAAJKPEAXPEAK_Z),
    reinterpret_cast<void*>(impl__Skip_XEnumVOID_CEnumArray__UEAAJK_Z),
    reinterpret_cast<void*>(impl__Reset_XEnumVOID_CEnumArray__UEAAJXZ),
    reinterpret_cast<void*>(impl__Clone_XEnumVOID_CEnumArray__UEAAJPEAPEAUIEnumVOID___Z)};
}
void* OpenMfcCreateConnectionEnumerator() { return new(std::nothrow)NativeEnumConnections; }
IEnumConnections* OpenMfcConnectionEnumeratorInterface(void* owner) {
    return Interface(static_cast<NativeEnumConnections*>(owner));
}
// Symbol: ??0CEnumConnections@@QEAA@PEBXI@Z
extern "C" void* MS_ABI impl___0CEnumConnections__QEAA_PEBXI_Z(void* self,const void* data,UINT count) {
    return self ? new(self)NativeEnumConnections(data,count) : nullptr;
}
// Symbol: ??1CEnumConnections@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumConnections__UEAA_XZ(void* self) {
    if(self)static_cast<NativeEnumConnections*>(self)->NativeEnumConnections::~NativeEnumConnections();
}
// Symbol: ?AddConnection@CEnumConnections@@QEAAXPEAUtagCONNECTDATA@@@Z
extern "C" void MS_ABI impl__AddConnection_CEnumConnections__QEAAXPEAUtagCONNECTDATA___Z(void* self,void* entry) {
    if(!self || !entry)return;
    auto* owner=static_cast<NativeEnumConnections*>(self);
    if(owner->count>=owner->capacity) {
        if(owner->count>std::numeric_limits<UINT>::max()-2) { impl__AfxThrowMemoryException__YAXXZ();return; }
        UINT capacity=owner->count+2;
        auto* data=static_cast<CONNECTDATA*>(std::malloc(static_cast<size_t>(capacity)*sizeof(CONNECTDATA)));
        if(!data) { impl__AfxThrowMemoryException__YAXXZ();return; }
        if(owner->count)std::memcpy(data,owner->data,static_cast<size_t>(owner->count)*sizeof(CONNECTDATA));
        if(owner->needFree)std::free(owner->data);
        owner->data=data;owner->capacity=capacity;owner->needFree=TRUE;
    }
    owner->data[owner->count]=*static_cast<CONNECTDATA*>(entry);
    if(owner->data[owner->count].pUnk)owner->data[owner->count].pUnk->AddRef();
    ++owner->count;
}
// Symbol: ?OnClone@CEnumConnections@@MEAAPEAVCEnumArray@@XZ
extern "C" void* MS_ABI impl__OnClone_CEnumConnections__MEAAPEAVCEnumArray__XZ(void* self) {
    return self ? static_cast<NativeEnumConnections*>(self)->NativeEnumConnections::OnClone() : nullptr;
}
// Symbol: ?OnNext@CEnumConnections@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumConnections__MEAAHPEAX_Z(void* self,void* out) {
    return self && out ? static_cast<NativeEnumConnections*>(self)->NativeEnumConnections::OnNext(out) : FALSE;
}
// Symbol: ?GetInterfaceMap@CEnumConnections@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_CEnumConnections__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) { return Map(); }
// Symbol: ?GetThisInterfaceMap@CEnumConnections@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_CEnumConnections__KAPEBUAFX_INTERFACEMAP__XZ() { return Map(); }
