#pragma once
#include "detail/CCmdTargetVirtualsSupport.h"

namespace cmdtarget_vtable_tests {
constexpr IID mappedIID = {0x38c125bc,0x4fc4,0x456b,{0x94,0x38,0xe9,0x80,0x76,0x53,0x84,0xa2}};
constexpr IID hookIID = {0xafdd736b,0x83d7,0x4fdd,{0xa3,0x5a,0xa1,0x77,0x69,0x6c,0x9e,0x89}};
constexpr IID disabledIID = {0xce98f186,0x2c0a,0x47bb,{0x9b,0x3e,0xe0,0x45,0x22,0x91,0x9b,0x38}};
constexpr IID aggregateIID = {0x57f40731,0xe0d7,0x4da5,{0x94,0x2a,0x19,0xb7,0x2a,0x59,0x2c,0x22}};
class Interface : public IUnknown {
public:
    const IID* accepted=nullptr;
    int queries=0,adds=0;

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid,void** out) override {
        ++queries;
        if (!out) return E_POINTER;
        *out=nullptr;
        if (accepted && iid!=*accepted && iid!=IID_IUnknown) return E_NOINTERFACE;
        *out=this; AddRef(); return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { ++adds; return 2; }
    ULONG STDMETHODCALLTYPE Release() override { return 1; }
};
class PrefixProbe : public CCmdTarget {
public:
    Interface mapped, hooked, aggregateImpl;
    IUnknown* aggregate=&aggregateImpl;
    PrefixProbe() { aggregateImpl.accepted=&aggregateIID; }
    void* disabled=nullptr;
    int commands=0, releases=0, hooks=0;
    int OnCmdMsg(UINT id,int code,void* extra,AFX_CMDHANDLERINFO* info) override {
        ++commands;
        return id==123 && code==-7 && extra==&mapped && static_cast<void*>(info)==&hooked ? 77 : -1;
    }
    void OnFinalRelease() override { ++releases; }
    BOOL IsInvokeAllowed(DISPID id) override { return id==17; }
    BOOL GetDispatchIID(IID* out) override { if(out)*out=mappedIID; return TRUE; }
    UINT GetTypeInfoCount() override { return 3; }
    CTypeLibCache* GetTypeLibCache() override { return reinterpret_cast<CTypeLibCache*>(&hooked); }
    HRESULT GetTypeLib(LCID locale,ITypeLib** out) override {
        if(out)*out=reinterpret_cast<ITypeLib*>(&mapped);
        return locale==0x409 ? S_FALSE : E_INVALIDARG;
    }
    const AFX_MSGMAP* GetMessageMap() const override {
        static const AFX_MSGMAP map{nullptr,nullptr}; return &map;
    }
    const AFX_INTERFACEMAP* GetInterfaceMap() const override {
        static const AFX_INTERFACEMAP_ENTRY entries[]={{&disabledIID,offsetof(PrefixProbe,disabled)},
            {&mappedIID,offsetof(PrefixProbe,mapped)},
            {nullptr,offsetof(PrefixProbe,aggregate)},{nullptr,static_cast<size_t>(-1)}};
        static const AFX_INTERFACEMAP map{impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ,entries};
        return &map;
    }
    IUnknown* GetInterfaceHook(const void* iid) override {
        ++hooks;
        return iid && *static_cast<const IID*>(iid)==hookIID ? &hooked : nullptr;
    }
};
template<class Fn> static Fn slot(void* object,size_t index) { return reinterpret_cast<Fn>((*static_cast<void***>(object))[index]); }

static void nativeSlots() {
    PrefixProbe probe;
    using Command=int (*)(void*,UINT,int,void*,void*);
    using Final=void (*)(void*);
    using Allowed=BOOL (*)(void*,DISPID);
    using DispatchIID=BOOL (*)(void*,IID*);
    using Count=UINT (*)(void*);
    using Pointer=void* (*)(void*);
    using Library=HRESULT (*)(void*,LCID,ITypeLib**);
    check(slot<Command>(&probe,5)(&probe,123,-7,&probe.mapped,&probe.hooked)==77 && probe.commands==1,
        "retail CCmdTarget slot 5 reaches the command override with all arguments");
    slot<Final>(&probe,6)(&probe);
    check(probe.releases==1 && slot<Allowed>(&probe,7)(&probe,17),
        "retail final-release and invocation-policy slots reach client overrides");
    IID iid{};
    ITypeLib* library=nullptr;
    check(slot<DispatchIID>(&probe,8)(&probe,&iid) && iid==mappedIID &&
        slot<Count>(&probe,9)(&probe)==3 && slot<Pointer>(&probe,10)(&probe)==&probe.hooked &&
        slot<Library>(&probe,11)(&probe,0x409,&library)==S_FALSE && library==reinterpret_cast<ITypeLib*>(&probe.mapped),
        "retail automation slots 8 through 11 preserve return types and output arguments");
    check(slot<Pointer>(&probe,12)(&probe)==probe.GetMessageMap() &&
        slot<Pointer>(&probe,13)(&probe)==probe.GetCommandMap() &&
        slot<Pointer>(&probe,14)(&probe)==probe.GetDispatchMap() &&
        slot<Pointer>(&probe,15)(&probe)==probe.GetConnectionMap() &&
        slot<Pointer>(&probe,16)(&probe)==probe.GetInterfaceMap() &&
        slot<Pointer>(&probe,17)(&probe)==probe.GetEventSinkMap(),
        "retail metadata slots 12 through 17 select the correct map virtuals");
    using Aggregates=BOOL (*)(void*);
    using Hook=IUnknown* (*)(void*,const void*);
    using Extra=BOOL (*)(void*,CPtrArray*);
    using Connection=IConnectionPoint* (*)(void*,const IID*);
    check(slot<Aggregates>(&probe,18)(&probe) && slot<Hook>(&probe,19)(&probe,&hookIID)==&probe.hooked &&
        !slot<Extra>(&probe,20)(&probe,nullptr) && !slot<Connection>(&probe,21)(&probe,&mappedIID),
        "retail aggregate, interface hook and connection slots complete the 22-method base prefix");
    CCmdTarget base;
    long references=0; memcpy(&references,reinterpret_cast<char*>(&base)+8,sizeof(references));
    void* outer=reinterpret_cast<void*>(1); memcpy(&outer,reinterpret_cast<char*>(&base)+16,sizeof(outer));
    check(references==1 && !outer && base.IsInvokeAllowed(42) && !base.GetTypeInfoCount() && !base.GetTypeLibCache(),
        "CCmdTarget initializes its addressable COM fields and supplies actual retail defaults");
    library=reinterpret_cast<ITypeLib*>(1);
    check(base.GetTypeLib(0,&library)==TYPE_E_CANTLOADLIBRARY && library==reinterpret_cast<ITypeLib*>(1),
        "the base type-library method preserves the retail failure and untouched output");
}
static void interfaceLookup() {
    PrefixProbe probe;
    check(probe.GetInterface(&hookIID)==&probe.hooked && probe.hooks==1,
        "interface lookup consults the client hook before its map");
    check(impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(&probe,&mappedIID)==&probe.mapped && probe.hooks==2,
        "the exported interface lookup uses the client's virtual interface map");
    check(!probe.GetInterface(&disabledIID) && probe.GetInterface(&IID_IUnknown)==&probe.mapped,
        "interface lookup skips inactive embedded interfaces and selects the first live IUnknown");
    check(!probe.GetInterface(&IID_IEnumVARIANT) && !probe.GetInterface(nullptr),
        "missing interface identifiers return null without fake success");
    CCmdTarget base;
    check(!base.GetInterface(&mappedIID) && base.GetDispatchMap()->lpEntries->lDispID==-1 &&
        !base.GetCommandMap()->lpEntries->cmdID && !base.GetConnectionMap()->pEntry->piid &&
        base.GetEventSinkMap()->lpEntries->nCtrlIDFirst==static_cast<UINT>(-1),
        "base interface lookup uses the real empty map sentinels");
}
static void queryAndReferences() {
    PrefixProbe probe;
    void* queried=reinterpret_cast<void*>(1);
    check(impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(&probe,&mappedIID,&queried)==S_OK &&
        queried==&probe.mapped && probe.m_dwRef==2 && !probe.aggregateImpl.queries,
        "direct QueryInterface increments the owner reference without querying aggregates");
    queried=nullptr;
    check(impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(&probe,&aggregateIID,&queried)==S_OK &&
        queried==&probe.aggregateImpl && probe.aggregateImpl.queries==1 && probe.aggregateImpl.adds==1 && probe.m_dwRef==2,
        "QueryInterface falls back to client aggregates and preserves their own COM reference");
    queried=reinterpret_cast<void*>(1);
    check(impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(&probe,&IID_IEnumVARIANT,&queried)==static_cast<DWORD>(E_NOINTERFACE) &&
        !queried && impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(&probe,&mappedIID,nullptr)==static_cast<DWORD>(E_POINTER),
        "QueryInterface clears missing outputs and rejects null output pointers");
    check(impl__InternalRelease_CCmdTarget__QEAAKXZ(&probe)==1 && !probe.releases &&
        impl__InternalRelease_CCmdTarget__QEAAKXZ(&probe)==0 && probe.releases==1 &&
        impl__InternalRelease_CCmdTarget__QEAAKXZ(&probe)==0 && probe.releases==1,
        "the last internal release invokes the actual final-release virtual exactly once");
    PrefixProbe controlled;
    Interface outer;
    controlled.m_pOuterUnknown=&outer;
    check(impl__ExternalAddRef_CCmdTarget__QEAAKXZ(&controlled)==2 && outer.adds==1 && controlled.m_dwRef==1,
        "external AddRef delegates to a controlling unknown without changing the inner count");
}

static void storageAndContext() {
    static_assert(sizeof(CCmdTarget)==64, "full retail command-target storage");
    static_assert(sizeof(CWnd)==232, "window storage remains ABI-sized");
    static_assert(sizeof(CDocument)==384, "document storage remains ABI-sized");
    static_assert(sizeof(CDocTemplate)==240, "template storage remains ABI-sized");
    static_assert(offsetof(CWnd,m_hWnd)==64, "window handle follows the native base");
    unsigned char ownerState[8]{}, callerState[8]{};
    AFX_MODULE_STATE* owner=reinterpret_cast<AFX_MODULE_STATE*>(ownerState);
    AFX_MODULE_STATE* caller=reinterpret_cast<AFX_MODULE_STATE*>(callerState);
    g_cmdTestCurrentModule=owner;
    class ContextProbe : public PrefixProbe {
    public:
        AFX_MODULE_STATE* observed=nullptr;
        void OnFinalRelease() override { observed=g_cmdTestCurrentModule; ++releases; }
    } target;
    check(target.m_pModuleState==owner && target.m_bResultExpected && !target.m_xDispatch && !target.m_xConnPtContainer,
        "the 64-byte base constructor captures module state and initializes embedded COM storage");
    target.m_bResultExpected=FALSE;
    check(!impl__IsResultExpected_CCmdTarget__QEAAHXZ(&target) && target.m_bResultExpected &&
        impl__IsResultExpected_CCmdTarget__QEAAHXZ(&target),
        "IsResultExpected returns the actual previous flag and resets it to TRUE");
    g_cmdTestCurrentModule=caller;
    check(impl__InternalRelease_CCmdTarget__QEAAKXZ(&target)==0 && target.observed==owner &&
        g_cmdTestCurrentModule==caller && target.releases==1,
        "the final-release callback sees its owner's module state and restores the caller's state");
    CWnd window;
    check(window.m_pModuleState==caller && !window.m_hWnd && !window.m_pRenderTarget && !window.m_pStdObject,
        "growing the base preserves the physical window fields and their initialization");
    class ThrowingProbe : public PrefixProbe {
    public:
        void OnFinalRelease() override { throw 17; }
    } throwing;
    throwing.m_pModuleState=owner;
    bool caught=false;
    try { impl__InternalRelease_CCmdTarget__QEAAKXZ(&throwing); }
    catch (int value) { caught=value==17; }
    check(caught && g_cmdTestCurrentModule==caller,
        "module context is restored when the final-release callback throws");
    g_cmdTestCurrentModule=nullptr;
}

}
static void cmdtargetVtable() { cmdtarget_vtable_tests::nativeSlots(); cmdtarget_vtable_tests::interfaceLookup(); cmdtarget_vtable_tests::queryAndReferences(); cmdtarget_vtable_tests::storageAndContext(); }
