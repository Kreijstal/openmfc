// Typed CCmdTarget virtuals in retail x64 slot order.
#include "detail/CCmdTargetVirtualsSupport.h"
#include <cstring>
#include <cstddef>
static_assert(sizeof(CCmdTarget)==64, "native base storage");
static_assert(offsetof(CCmdTarget,m_dwRef)==8, "native reference count");
static_assert(offsetof(CCmdTarget,m_pOuterUnknown)==16, "controlling unknown");
static_assert(offsetof(CCmdTarget,m_xInnerUnknown)==24, "inner unknown");
static_assert(offsetof(CCmdTarget,m_xDispatch)==32, "embedded dispatch");
static_assert(offsetof(CCmdTarget,m_bResultExpected)==40, "invocation result flag");
static_assert(offsetof(CCmdTarget,m_xConnPtContainer)==48, "embedded connection container");
static_assert(offsetof(CCmdTarget,m_pModuleState)==56, "module state");
namespace {
constexpr IID kIID_IUnknown = {0,0,0,{0xc0,0,0,0,0,0,0,0x46}};
// --- CCmdTarget's own (empty) maps ------------------------------------------
// Reproduced byte-for-byte from retail mfc140u .rdata:
//   dispatch    map 0x18033e268 = { NULL, 0x18033e290, 0x1803b1b9c, 0x1803b1ba0 }
//               entries at 0x18033e290: one terminator record --
//               lpszName NULL, lDispID 0xffffffff, nPropOffset (size_t)-1
//               entry count at 0x1803b1b9c holds 0xffffffff (filled in lazily
//               by GetEntryCount) and the stock-property mask at 0x1803b1ba0
//               holds 0xffffffff too -- BEGIN_DISPATCH_MAP (afxdisp.h:178)
//               seeds _dwStockPropMask with (DWORD)-1 as its "not computed
//               yet" sentinel, exactly like _dispatchEntryCount
//   event sink  map 0x18033e200 = { NULL, 0x18033e220, 0x1803b1b98 };
//               the count at 0x1803b1b98 is 0xffffffff.  AFX_EVENTSINKMAP has
//               only those three fields (afxwin.h:1639), so the qword that
//               follows the map in .rdata is a different object.
//               entries at 0x18033e220: one terminator, +0x30 = -1 and the
//               qword at +0x40 = -1, i.e. nCtrlIDFirst and nCtrlIDLast both -1
//   connection  map 0x18033e1c0 = { NULL, 0x18033e1a0 }
//               entries at 0x18033e1a0 = { NULL, (size_t)-1 }
//   OLE command map 0x18033e1b0 = { NULL, 0x18033e190 }
//               entries at 0x18033e190 = 16 bytes of zero
const AFX_DISPMAP_ENTRY g_dispEntries_CCmdTarget[] = {
    { nullptr, -1, nullptr, 0, nullptr, nullptr, (size_t)-1, afxDispCustom_ }
};
unsigned int  g_dispEntryCount_CCmdTarget = 0xffffffffu;
unsigned long g_dispStockPropMask_CCmdTarget = 0xffffffffu;
const AFX_DISPMAP g_dispatchMap_CCmdTarget = {
    nullptr, g_dispEntries_CCmdTarget, &g_dispEntryCount_CCmdTarget, &g_dispStockPropMask_CCmdTarget
};

const AFX_EVENTSINKMAP_ENTRY g_evsinkEntries_CCmdTarget[] = {
    { { nullptr, -1, nullptr, 0, nullptr, nullptr, (size_t)-1, afxDispCustom_ }, 0xffffffffu, 0xffffffffu }
};
unsigned int g_evsinkEntryCount_CCmdTarget = 0xffffffffu;
const AFX_EVENTSINKMAP g_eventSinkMap_CCmdTarget = {
    nullptr, g_evsinkEntries_CCmdTarget, &g_evsinkEntryCount_CCmdTarget
};

const AFX_CONNECTIONMAP_ENTRY g_connEntries_CCmdTarget[] = { { nullptr, (size_t)-1 } };
const AFX_CONNECTIONMAP g_connectionMap_CCmdTarget = { nullptr, g_connEntries_CCmdTarget };

const AFX_OLECMDMAP_ENTRY g_cmdEntries_CCmdTarget[] = { { nullptr, 0, 0 } };
const AFX_OLECMDMAP g_commandMap_CCmdTarget = { nullptr, g_cmdEntries_CCmdTarget };

}

// Symbol: ?GetTypeLibCache@CCmdTarget@@UEAAPEAVCTypeLibCache@@XZ
extern "C" void* MS_ABI impl__GetTypeLibCache_CCmdTarget__UEAAPEAVCTypeLibCache__XZ(CCmdTarget* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetCommandMap@CCmdTarget@@MEBAPEBUAFX_OLECMDMAP@@XZ
extern "C" const void* MS_ABI impl__GetCommandMap_CCmdTarget__MEBAPEBUAFX_OLECMDMAP__XZ(const CCmdTarget* pThis) {
    (void)pThis;
    return &g_commandMap_CCmdTarget;
}

// Symbol: ?GetConnectionHook@CCmdTarget@@MEAAPEAUIConnectionPoint@@AEBU_GUID@@@Z
extern "C" void* MS_ABI impl__GetConnectionHook_CCmdTarget__MEAAPEAUIConnectionPoint__AEBU_GUID___Z(CCmdTarget* pThis, const void* iid) {
    (void)pThis; (void)iid;
    return nullptr;
}

// Symbol: ?GetConnectionMap@CCmdTarget@@MEBAPEBUAFX_CONNECTIONMAP@@XZ
extern "C" const void* MS_ABI impl__GetConnectionMap_CCmdTarget__MEBAPEBUAFX_CONNECTIONMAP__XZ(const CCmdTarget* pThis) {
    (void)pThis;
    return &g_connectionMap_CCmdTarget;
}

// Symbol: ?GetDispatchIID@CCmdTarget@@UEAAHPEAU_GUID@@@Z
extern "C" int MS_ABI impl__GetDispatchIID_CCmdTarget__UEAAHPEAU_GUID___Z(CCmdTarget* pThis, void* pIID) {
    (void)pThis; (void)pIID;
    return 0;
}

// Symbol: ?GetDispatchMap@CCmdTarget@@MEBAPEBUAFX_DISPMAP@@XZ
extern "C" const void* MS_ABI impl__GetDispatchMap_CCmdTarget__MEBAPEBUAFX_DISPMAP__XZ(const CCmdTarget* pThis) {
    (void)pThis;
    return &g_dispatchMap_CCmdTarget;
}

// Symbol: ?GetEventSinkMap@CCmdTarget@@MEBAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" const void* MS_ABI impl__GetEventSinkMap_CCmdTarget__MEBAPEBUAFX_EVENTSINKMAP__XZ(const CCmdTarget* pThis) {
    (void)pThis;
    return &g_eventSinkMap_CCmdTarget;
}

// Symbol: ?GetExtraConnectionPoints@CCmdTarget@@MEAAHPEAVCPtrArray@@@Z
extern "C" int MS_ABI impl__GetExtraConnectionPoints_CCmdTarget__MEAAHPEAVCPtrArray___Z(CCmdTarget* pThis, void* pConnPoints) {
    (void)pThis; (void)pConnPoints;
    return 0;
}

// Symbol: ?GetInterface@CCmdTarget@@QEAAPEAUIUnknown@@PEBX@Z
extern "C" IUnknown* MS_ABI impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(CCmdTarget* pThis, const void* iid) {
    if (!pThis || !iid) return nullptr;
    const GUID* pIID = static_cast<const GUID*>(iid);
    const bool bUnknown = (::memcmp(pIID, &kIID_IUnknown, sizeof(GUID)) == 0);

    if (IUnknown* hook = pThis->GetInterfaceHook(iid)) return hook;
    for (const AFX_INTERFACEMAP* pMap = pThis->GetInterfaceMap();
         pMap != nullptr;
         pMap = pMap->pfnGetBaseMap ? pMap->pfnGetBaseMap() : nullptr) {
        for (const AFX_INTERFACEMAP_ENTRY* e = pMap->pEntries; e->piid != nullptr; ++e) {
            if (!bUnknown && ::memcmp(e->piid, pIID, sizeof(GUID)) != 0) continue;
            unsigned char* pSub = reinterpret_cast<unsigned char*>(pThis) + e->nOffset;
            if (*reinterpret_cast<const unsigned int*>(pSub) != 0)
                return reinterpret_cast<IUnknown*>(pSub);
        }
    }
    return nullptr;
}

// Symbol: ?GetInterfaceHook@CCmdTarget@@UEAAPEAUIUnknown@@PEBX@Z
extern "C" IUnknown* MS_ABI impl__GetInterfaceHook_CCmdTarget__UEAAPEAUIUnknown__PEBX_Z(CCmdTarget* pThis, const void* iid) {
    (void)pThis; (void)iid;
    return nullptr;
}

// Symbol: ?GetThisCommandMap@CCmdTarget@@KAPEBUAFX_OLECMDMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisCommandMap_CCmdTarget__KAPEBUAFX_OLECMDMAP__XZ() {
    return &g_commandMap_CCmdTarget;
}

// Symbol: ?GetThisConnectionMap@CCmdTarget@@KAPEBUAFX_CONNECTIONMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisConnectionMap_CCmdTarget__KAPEBUAFX_CONNECTIONMAP__XZ() {
    return &g_connectionMap_CCmdTarget;
}

// Symbol: ?GetThisDispatchMap@CCmdTarget@@KAPEBUAFX_DISPMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisDispatchMap_CCmdTarget__KAPEBUAFX_DISPMAP__XZ() {
    return &g_dispatchMap_CCmdTarget;
}

// Symbol: ?GetThisEventSinkMap@CCmdTarget@@KAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisEventSinkMap_CCmdTarget__KAPEBUAFX_EVENTSINKMAP__XZ() {
    return &g_eventSinkMap_CCmdTarget;
}

// Symbol: ?GetTypeInfoCount@CCmdTarget@@UEAAIXZ
extern "C" unsigned int MS_ABI impl__GetTypeInfoCount_CCmdTarget__UEAAIXZ(CCmdTarget* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?GetTypeLib@CCmdTarget@@UEAAJKPEAPEAUITypeLib@@@Z
extern "C" long MS_ABI impl__GetTypeLib_CCmdTarget__UEAAJKPEAPEAUITypeLib___Z(CCmdTarget* pThis, unsigned long lcid, void** ppTypeLib) {
    (void)pThis; (void)lcid; (void)ppTypeLib;
    return static_cast<long>(0x80029c4aL); // TYPE_E_CANTLOADLIBRARY
}

// Symbol: ?IsInvokeAllowed@CCmdTarget@@UEAAHJ@Z
extern "C" int MS_ABI impl__IsInvokeAllowed_CCmdTarget__UEAAHJ_Z(CCmdTarget* pThis, long dispid) {
    (void)pThis; (void)dispid;
    return 1;
}

// Symbol: ?OnCreateAggregates@CCmdTarget@@UEAAHXZ
extern "C" int MS_ABI impl__OnCreateAggregates_CCmdTarget__UEAAHXZ(CCmdTarget* pThis) {
    (void)pThis;
    return TRUE;
}

void CCmdTarget::OnFinalRelease() { impl__OnFinalRelease_CCmdTarget__UEAAXXZ(this); }
BOOL CCmdTarget::IsInvokeAllowed(DISPID id) { return impl__IsInvokeAllowed_CCmdTarget__UEAAHJ_Z(this,id); }
BOOL CCmdTarget::GetDispatchIID(IID* iid) { return impl__GetDispatchIID_CCmdTarget__UEAAHPEAU_GUID___Z(this,iid); }
UINT CCmdTarget::GetTypeInfoCount() { return impl__GetTypeInfoCount_CCmdTarget__UEAAIXZ(this); }
CTypeLibCache* CCmdTarget::GetTypeLibCache() { return static_cast<CTypeLibCache*>(impl__GetTypeLibCache_CCmdTarget__UEAAPEAVCTypeLibCache__XZ(this)); }
HRESULT CCmdTarget::GetTypeLib(LCID lcid,ITypeLib** library) { return impl__GetTypeLib_CCmdTarget__UEAAJKPEAPEAUITypeLib___Z(this,lcid,reinterpret_cast<void**>(library)); }
const AFX_OLECMDMAP* CCmdTarget::GetCommandMap() const { return static_cast<const AFX_OLECMDMAP*>(impl__GetCommandMap_CCmdTarget__MEBAPEBUAFX_OLECMDMAP__XZ(this)); }
const AFX_DISPMAP* CCmdTarget::GetDispatchMap() const { return static_cast<const AFX_DISPMAP*>(impl__GetDispatchMap_CCmdTarget__MEBAPEBUAFX_DISPMAP__XZ(this)); }
const AFX_CONNECTIONMAP* CCmdTarget::GetConnectionMap() const { return static_cast<const AFX_CONNECTIONMAP*>(impl__GetConnectionMap_CCmdTarget__MEBAPEBUAFX_CONNECTIONMAP__XZ(this)); }
const AFX_INTERFACEMAP* CCmdTarget::GetInterfaceMap() const { return impl__GetInterfaceMap_CCmdTarget__MEBAPEBUAFX_INTERFACEMAP__XZ(this); }
const AFX_EVENTSINKMAP* CCmdTarget::GetEventSinkMap() const { return static_cast<const AFX_EVENTSINKMAP*>(impl__GetEventSinkMap_CCmdTarget__MEBAPEBUAFX_EVENTSINKMAP__XZ(this)); }
BOOL CCmdTarget::OnCreateAggregates() { return impl__OnCreateAggregates_CCmdTarget__UEAAHXZ(this); }
IUnknown* CCmdTarget::GetInterfaceHook(const void* iid) { return impl__GetInterfaceHook_CCmdTarget__UEAAPEAUIUnknown__PEBX_Z(this,iid); }
BOOL CCmdTarget::GetExtraConnectionPoints(CPtrArray* points) { return impl__GetExtraConnectionPoints_CCmdTarget__MEAAHPEAVCPtrArray___Z(this,points); }
IConnectionPoint* CCmdTarget::GetConnectionHook(const IID& iid) { return static_cast<IConnectionPoint*>(impl__GetConnectionHook_CCmdTarget__MEAAPEAUIConnectionPoint__AEBU_GUID___Z(this,&iid)); }
IUnknown* CCmdTarget::GetInterface(const void* iid) { return impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(this,iid); }

// Symbol: ?ExternalAddRef@CCmdTarget@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__ExternalAddRef_CCmdTarget__QEAAKXZ(CCmdTarget* pThis) {
    if (!pThis) return 0;
    if (pThis->m_pOuterUnknown != nullptr)
        return pThis->m_pOuterUnknown->AddRef();
    return static_cast<unsigned long>(
        ::InterlockedIncrement(reinterpret_cast<LONG volatile*>(&pThis->m_dwRef)));
}

// Symbol: ?InternalQueryInterface@CCmdTarget@@QEAAKPEBXPEAPEAX@Z
extern "C" unsigned long MS_ABI impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget* pThis, const void* iid, void** ppvObj) {
    if (!ppvObj) return 0x80004003UL;                  // E_POINTER
    *ppvObj = impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(pThis, iid);
    if (*ppvObj != nullptr) {
        impl__ExternalAddRef_CCmdTarget__QEAAKXZ(pThis);
        return 0;                                      // S_OK
    }
    *ppvObj = impl__QueryAggregates_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(pThis,iid);
    return *ppvObj ? S_OK : static_cast<unsigned long>(E_NOINTERFACE);
}

// Symbol: ?InternalRelease@CCmdTarget@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__InternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis) {
    if (!pThis || !pThis->m_dwRef) return 0;
    LONG left = ::InterlockedDecrement(&pThis->m_dwRef);
    if (!left) {
        openmfc::detail::CmdTargetModuleScope state(pThis->m_pModuleState);
        pThis->OnFinalRelease();
    }
    return static_cast<unsigned long>(left);
}

// Symbol: ?QueryAggregates@CCmdTarget@@QEAAPEAUIUnknown@@PEBX@Z
extern "C" IUnknown* MS_ABI impl__QueryAggregates_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(CCmdTarget* pThis, const void* iid) {
    if (!pThis || !iid) return nullptr;
    const AFX_INTERFACEMAP* pMap = pThis->GetInterfaceMap();
    while (pMap) {
        const AFX_INTERFACEMAP_ENTRY* e = pMap->pEntries;
        while (e->piid != nullptr) ++e;
        for (; e->nOffset != static_cast<size_t>(-1); ++e) {
            IUnknown* pUnk = *reinterpret_cast<IUnknown* const*>(
                reinterpret_cast<const unsigned char*>(pThis) + e->nOffset);
            if (pUnk != nullptr) {
                void* pv = nullptr;
                if (pUnk->QueryInterface(*static_cast<const IID*>(iid), &pv) == S_OK && pv != nullptr)
                    return static_cast<IUnknown*>(pv);
            }
        }
        if (pMap->pfnGetBaseMap == nullptr) return nullptr;
        pMap = pMap->pfnGetBaseMap();
    }
    return nullptr;
}

// Reads the previous invocation flag and resets it for the next call.
// Symbol: ?IsResultExpected@CCmdTarget@@QEAAHXZ
extern "C" int MS_ABI impl__IsResultExpected_CCmdTarget__QEAAHXZ(CCmdTarget* target) {
    if (!target) return FALSE;
    BOOL expected = target->m_bResultExpected;
    target->m_bResultExpected = TRUE;
    return expected;
}


// Symbol: ?ExternalRelease@CCmdTarget@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__ExternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis) {
    if (!pThis) return 0;
    if (pThis->m_pOuterUnknown != nullptr)
        return pThis->m_pOuterUnknown->Release();
    return impl__InternalRelease_CCmdTarget__QEAAKXZ(pThis);
}

// Symbol: ?ExternalQueryInterface@CCmdTarget@@QEAAKPEBXPEAPEAX@Z
extern "C" unsigned long MS_ABI impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget* pThis, const void* iid, void** ppvObj) {
    if (!pThis || !iid || !ppvObj) return 0x80004003UL; // E_POINTER
    if (pThis->m_pOuterUnknown != nullptr)
        return static_cast<unsigned long>(
            pThis->m_pOuterUnknown->QueryInterface(*static_cast<const IID*>(iid), ppvObj));
    return impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(pThis, iid, ppvObj);
}

// Symbol: ?ExternalDisconnect@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__ExternalDisconnect_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    if (!pThis) return;
    if (pThis->m_dwRef == 0) return;
    IUnknown* pUnk = impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(pThis, &kIID_IUnknown);
    ::InterlockedIncrement(reinterpret_cast<LONG volatile*>(&pThis->m_dwRef));
    if (pUnk != nullptr)
        ::CoDisconnectObject(pUnk, 0);
    pThis->m_dwRef = 0;
}
