// COleConnPtContainer — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CCmdTargetConnectionsSupport.h"
#include "detail/OleExceptionTranslationSupport.h"
#include "openmfc/afxole.h"
#include <cstring>
#include <new>
#include <vector>
static_assert(sizeof(COleConnPtContainer)==sizeof(void*), "native embedded container storage");

COleConnPtContainer::COleConnPtContainer() {
    memset(_oleconnptcontainer_padding, 0, sizeof(_oleconnptcontainer_padding));
}
COleConnPtContainer::~COleConnPtContainer() {
}

namespace {
CCmdTarget* Owner(void* self) {
    return self ? reinterpret_cast<CCmdTarget*>(static_cast<char*>(self)-0x30) : nullptr;
}
IConnectionPoint* MappedPoint(CCmdTarget* owner,size_t offset) {
    // CONNECTION_PART offsets address the CConnectionPoint object. Its
    // embedded IConnectionPoint table is at +0x58 in the native x64 layout.
    return reinterpret_cast<IConnectionPoint*>(reinterpret_cast<char*>(owner)+offset+0x58);
}
struct PointSnapshot {
    std::vector<IConnectionPoint*> points;
    PointSnapshot() = default;
    PointSnapshot(PointSnapshot&& other) noexcept : points(std::move(other.points)) {}
    ~PointSnapshot() { for(auto* point:points)point->Release(); }
    void Add(IConnectionPoint* point) {
        if(!point)return;
        point->AddRef();
        try { points.push_back(point); }
        catch(...) { point->Release();throw; }
    }
};
class PointEnumerator final : public IEnumConnectionPoints {
    LONG references=1;
    PointSnapshot snapshot;
    size_t position=0;
public:
    explicit PointEnumerator(PointSnapshot&& points,size_t cursor=0)
        : snapshot(std::move(points)),position(cursor) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid,void** out) override {
        if(!out)return E_POINTER;
        *out=nullptr;
        if(iid!=IID_IUnknown && iid!=IID_IEnumConnectionPoints)return E_NOINTERFACE;
        *out=static_cast<IEnumConnectionPoints*>(this);AddRef();return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&references); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG left=InterlockedDecrement(&references);
        if(!left)delete this;
        return left;
    }
    HRESULT STDMETHODCALLTYPE Next(ULONG count,IConnectionPoint** out,ULONG* fetched) override {
        if(fetched)*fetched=0;
        if(count && !out)return E_POINTER;
        if(count>1 && !fetched)return E_POINTER;
        ULONG copied=0;
        while(copied<count && position<snapshot.points.size()) {
            auto* point=snapshot.points[position++];point->AddRef();out[copied++]=point;
        }
        if(fetched)*fetched=copied;
        return copied==count ? S_OK : S_FALSE;
    }
    HRESULT STDMETHODCALLTYPE Skip(ULONG count) override {
        size_t remaining=snapshot.points.size()-position;
        size_t skipped=count<remaining ? count : remaining;
        position+=skipped;return skipped==count ? S_OK : S_FALSE;
    }
    HRESULT STDMETHODCALLTYPE Reset() override { position=0;return S_OK; }
    HRESULT STDMETHODCALLTYPE Clone(IEnumConnectionPoints** out) override {
        if(!out)return E_POINTER;
        *out=nullptr;
        try {
            PointSnapshot copy;
            for(auto* point:snapshot.points)copy.Add(point);
            *out=new(std::nothrow)PointEnumerator(std::move(copy),position);
            return *out ? S_OK : E_OUTOFMEMORY;
        } catch(CException* error) {
            openmfc::detail::FrameworkExceptionCleanup cleanup{error};
            return E_UNEXPECTED;
        } catch(const std::bad_alloc&) { return E_OUTOFMEMORY; }
    }
};
}

// Symbol: ?AddRef@COleConnPtContainer@@UEAAKXZ
extern "C" ULONG MS_ABI impl__AddRef_COleConnPtContainer__UEAAKXZ(COleConnPtContainer* self) {
    return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(Owner(self));
}
// Symbol: ?Release@COleConnPtContainer@@UEAAKXZ
extern "C" ULONG MS_ABI impl__Release_COleConnPtContainer__UEAAKXZ(COleConnPtContainer* self) {
    return impl__ExternalRelease_CCmdTarget__QEAAKXZ(Owner(self));
}
// Symbol: ?QueryInterface@COleConnPtContainer@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" HRESULT MS_ABI impl__QueryInterface_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAX_Z(COleConnPtContainer* self,REFIID iid,void** out) {
    if(!self || !out)return E_POINTER;
    return static_cast<HRESULT>(impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(Owner(self),&iid,out));
}
// Symbol: ?FindConnectionPoint@COleConnPtContainer@@UEAAJAEBU_GUID@@PEAPEAUIConnectionPoint@@@Z
extern "C" HRESULT MS_ABI impl__FindConnectionPoint_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAUIConnectionPoint___Z(
    COleConnPtContainer* self,REFIID iid,IConnectionPoint** out) {
    if(!self || !out)return E_POINTER;
    *out=nullptr;
    auto* owner=Owner(self);
    if(auto* hook=owner->GetConnectionHook(iid)) {
        hook->AddRef();*out=hook;return S_OK;
    }
    for(auto* map=owner->GetConnectionMap();map;map=map->pfnGetBaseMap ? map->pfnGetBaseMap() : nullptr)
        for(auto* entry=map->pEntry;entry->piid;++entry)
            if(*static_cast<const IID*>(entry->piid)==iid) {
                auto* point=MappedPoint(owner,entry->nOffset);
                point->AddRef();*out=point;return S_OK;
            }
    return CONNECT_E_NOCONNECTION;
}
// Symbol: ?EnumConnectionPoints@COleConnPtContainer@@UEAAJPEAPEAUIEnumConnectionPoints@@@Z
extern "C" HRESULT MS_ABI impl__EnumConnectionPoints_COleConnPtContainer__UEAAJPEAPEAUIEnumConnectionPoints___Z(
    COleConnPtContainer* self,IEnumConnectionPoints** out) {
    if(!self || !out)return E_POINTER;
    *out=nullptr;
    try {
        auto* owner=Owner(self);
        PointSnapshot points;
        CPtrArray extra;
        if(owner->GetExtraConnectionPoints(&extra))
            for(INT_PTR i=0;i<extra.GetSize();++i)points.Add(static_cast<IConnectionPoint*>(extra.GetAt(i)));
        for(auto* map=owner->GetConnectionMap();map;map=map->pfnGetBaseMap ? map->pfnGetBaseMap() : nullptr)
            for(auto* entry=map->pEntry;entry->piid;++entry)points.Add(MappedPoint(owner,entry->nOffset));
        *out=new(std::nothrow)PointEnumerator(std::move(points));
        return *out ? S_OK : E_OUTOFMEMORY;
    } catch(CException* error) {
        openmfc::detail::FrameworkExceptionCleanup cleanup{error};
        return CONNECT_E_NOCONNECTION;
    } catch(const std::bad_alloc&) { return E_OUTOFMEMORY; }
}
// Symbol: ?EnableConnections@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableConnections_CCmdTarget__QEAAXXZ(CCmdTarget* owner) {
    if(!owner)return;
    static void* table[]={
        reinterpret_cast<void*>(impl__QueryInterface_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAX_Z),
        reinterpret_cast<void*>(impl__AddRef_COleConnPtContainer__UEAAKXZ),
        reinterpret_cast<void*>(impl__Release_COleConnPtContainer__UEAAKXZ),
        reinterpret_cast<void*>(impl__EnumConnectionPoints_COleConnPtContainer__UEAAJPEAPEAUIEnumConnectionPoints___Z),
        reinterpret_cast<void*>(impl__FindConnectionPoint_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAUIConnectionPoint___Z)
    };
    owner->m_xConnPtContainer=reinterpret_cast<ULONG_PTR>(table);
}
