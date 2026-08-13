// CCmdTarget — OpenMFC implementation.
// Sources: appcore.cpp, cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"
#include "detail/CbarcoreSupport.h"

// Symbol: ?GetThisClass@CCmdTarget@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CCmdTarget__SAPEAUCRuntimeClass__XZ() {
    return &CCmdTarget::classCCmdTarget;
}
// Symbol: ?BeginWaitCursor@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    if (!pThis) return;
    int& depth = g_waitCursorDepth[pThis];
    ++depth;
    ::SetCursor(GetWaitCursorHandle());
}
// Symbol: ?EndWaitCursor@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    if (!pThis) return;
    int& depth = g_waitCursorDepth[pThis];
    if (depth > 0) --depth;
    if (depth == 0) ::SetCursor(GetArrowCursorHandle());
}
// Symbol: ?RestoreWaitCursor@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__RestoreWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    if (!pThis) return;
    const int depth = g_waitCursorDepth[pThis];
    ::SetCursor(depth > 0 ? GetWaitCursorHandle() : GetArrowCursorHandle());
}
CCmdTarget::~CCmdTarget() {
    // Base class destructor - nothing specific to clean up
}
const AFX_MSGMAP* AFXAPI CCmdTarget::GetThisMessageMap()
{
    return &CCmdTarget::messageMap;
}
const AFX_MSGMAP* CCmdTarget::GetMessageMap() const
{
    return GetThisMessageMap();
}
const AFX_MSGMAP CCmdTarget::messageMap =
{
    nullptr, // No base class with message map
    &CCmdTarget::_messageEntries[0]
};
const AFX_MSGMAP_ENTRY CCmdTarget::_messageEntries[] =
{
    {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
};
int CCmdTarget::OnCmdMsg(unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo)
{
    // Simple command routing
    const AFX_MSGMAP* pMap = GetMessageMap();
    
    for (; pMap != nullptr; pMap = (*pMap->pfnGetBaseMap)())
    {
        const AFX_MSGMAP_ENTRY* lpEntry = pMap->lpEntries;
        while (lpEntry->nSig != AfxSig_end)
        {
            if (lpEntry->nID == nID && lpEntry->nCode == (UINT)nCode)
            {
                // Found a match
                return DispatchCmdMsg(this, nID, nCode, lpEntry->pfn, pExtra, lpEntry->nSig, pHandlerInfo);
            }
            lpEntry++;
        }
    }
    
    return FALSE; // Not handled
}
int PASCAL CCmdTarget::DispatchCmdMsg(CCmdTarget* pTarget, unsigned int nID, int nCode,
                                      AFX_PMSG pfn, void* pExtra, unsigned int nSig, void* pHandlerInfo)
{
    (void)nID; (void)nCode; (void)pHandlerInfo;

    // Define member function pointer types for various signatures
    typedef void (CCmdTarget::*AFX_PMSG_v)();
    typedef int (CCmdTarget::*AFX_PMSG_b)();
    typedef void (CCmdTarget::*AFX_PMSG_vw)(UINT);
    typedef void (CCmdTarget::*AFX_PMSG_vww)(UINT, UINT);
    typedef void (CCmdTarget::*AFX_PMSG_vwww)(UINT, UINT, UINT);
    typedef void (CCmdTarget::*AFX_PMSG_vwl)(UINT, LONG);
    typedef LRESULT (CCmdTarget::*AFX_PMSG_lwl)(WPARAM, LPARAM);
    typedef void (CCmdTarget::*AFX_PMSG_vb)(BOOL);
    typedef int (CCmdTarget::*AFX_PMSG_bh)(HANDLE);
    typedef void (CCmdTarget::*AFX_PMSG_cmdui)(void*);
    typedef int (CCmdTarget::*AFX_PMSG_bwl)(UINT, LONG);
    typedef void (CCmdTarget::*AFX_PMSG_vwwh)(UINT, UINT, HANDLE);
    typedef int (CCmdTarget::*AFX_PMSG_iw)(UINT);
    typedef int (CCmdTarget::*AFX_PMSG_iww)(UINT, UINT);

    union MessageMapFunctions {
        AFX_PMSG pfn;
        AFX_PMSG_v pfn_v;
        AFX_PMSG_b pfn_b;
        AFX_PMSG_vw pfn_vw;
        AFX_PMSG_vww pfn_vww;
        AFX_PMSG_vwww pfn_vwww;
        AFX_PMSG_vwl pfn_vwl;
        AFX_PMSG_lwl pfn_lwl;
        AFX_PMSG_vb pfn_vb;
        AFX_PMSG_bh pfn_bh;
        AFX_PMSG_cmdui pfn_cmdui;
        AFX_PMSG_bwl pfn_bwl;
        AFX_PMSG_vwwh pfn_vwwh;
        AFX_PMSG_iw pfn_iw;
        AFX_PMSG_iww pfn_iww;
    };

    MessageMapFunctions mmf;
    mmf.pfn = pfn;

    switch (nSig)
    {
    case AfxSig_vv:
        (pTarget->*mmf.pfn_v)();
        return TRUE;

    case AfxSig_bv:
        return (pTarget->*mmf.pfn_b)();

    case AfxSig_vw:
        (pTarget->*mmf.pfn_vw)(nID);
        return TRUE;

    case AfxSig_vww:
        {
            UINT* pParams = static_cast<UINT*>(pExtra);
            (pTarget->*mmf.pfn_vww)(pParams ? pParams[0] : 0, pParams ? pParams[1] : 0);
        }
        return TRUE;

    case AfxSig_vwww:
        {
            UINT* pParams = static_cast<UINT*>(pExtra);
            (pTarget->*mmf.pfn_vwww)(pParams ? pParams[0] : 0, pParams ? pParams[1] : 0, pParams ? pParams[2] : 0);
        }
        return TRUE;

    case AfxSig_vwl:
        {
            LONG* pParam = static_cast<LONG*>(pExtra);
            (pTarget->*mmf.pfn_vwl)(nID, pParam ? *pParam : 0);
        }
        return TRUE;

    case AfxSig_lwl:
        {
            WPARAM wParam = static_cast<WPARAM>(nID);
            LPARAM lParam = pExtra ? *static_cast<LPARAM*>(pExtra) : 0;
            return static_cast<int>((pTarget->*mmf.pfn_lwl)(wParam, lParam));
        }

    case AfxSig_v_b:
    case AfxSig_vb:
        (pTarget->*mmf.pfn_vb)(pExtra ? *static_cast<BOOL*>(pExtra) : FALSE);
        return TRUE;

    case AfxSig_bh:
        {
            HANDLE h = pExtra ? *static_cast<HANDLE*>(pExtra) : nullptr;
            return (pTarget->*mmf.pfn_bh)(h);
        }

    case AfxSig_cmdui:
        (pTarget->*mmf.pfn_cmdui)(pExtra);
        return TRUE;

    case AfxSig_bwl:
        {
            LONG* pParam = static_cast<LONG*>(pExtra);
            return (pTarget->*mmf.pfn_bwl)(nID, pParam ? *pParam : 0);
        }

    case AfxSig_vwwh:
        {
            void** pParams = static_cast<void**>(pExtra);
            UINT u1 = pParams ? static_cast<UINT>(reinterpret_cast<UINT_PTR>(pParams[0])) : 0;
            UINT u2 = pParams ? static_cast<UINT>(reinterpret_cast<UINT_PTR>(pParams[1])) : 0;
            HANDLE h = pParams ? static_cast<HANDLE>(pParams[2]) : nullptr;
            (pTarget->*mmf.pfn_vwwh)(u1, u2, h);
        }
        return TRUE;

    case AfxSig_iw:
        return (pTarget->*mmf.pfn_iw)(nID);

    case AfxSig_iww:
        {
            UINT* pParams = static_cast<UINT*>(pExtra);
            return (pTarget->*mmf.pfn_iww)(pParams ? pParams[0] : 0, pParams ? pParams[1] : 0);
        }

    case AfxSig_vv_i:
        (pTarget->*mmf.pfn_v)();
        return TRUE;

    default:
        return FALSE;
    }
}
// Symbol: ?GetTypeLibCache@CCmdTarget@@UEAAPEAVCTypeLibCache@@XZ
extern "C" void* MS_ABI impl__GetTypeLibCache_CCmdTarget__UEAAPEAVCTypeLibCache__XZ(CCmdTarget* pThis) {
    if (!pThis) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(g_typeLibCacheMutex);
    auto& ownedCache = g_targetTypeLibCaches[pThis];
    if (!ownedCache) {
        ownedCache = std::make_unique<TypeLibCacheHandle>();
    }
    return ownedCache.get();
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?OnCmdMsg@CCmdTarget@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(void* pThis, void* p0, void* p1, void* p2, void* p3) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    return 0;
}

// Symbol: ?CallMemberFunc@CCmdTarget@@IEAAJPEBUAFX_DISPMAP_ENTRY@@GPEAUtagVARIANT@@PEAUtagDISPPARAMS@@PEAI@Z
extern "C" long MS_ABI impl__CallMemberFunc_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__GPEAUtagVARIANT__PEAUtagDISPPARAMS__PEAI_Z(const void* /*struct*/* p0, unsigned short p1, void* /*struct*/* p2, void* /*struct*/* p3, unsigned int* p4) {
    return 0;
}

// Symbol: ?DoOleVerb@CCmdTarget@@QEAAHJPEAUtagMSG@@PEAUHWND__@@PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__DoOleVerb_CCmdTarget__QEAAHJPEAUtagMSG__PEAUHWND____PEBUtagRECT___Z(long p0, void* /*struct*/* p1, void* /*struct*/* p2, const void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?EnableAggregation@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableAggregation_CCmdTarget__QEAAXXZ() {}

// Symbol: ?EnableAutomation@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableAutomation_CCmdTarget__QEAAXXZ() {}

// Symbol: ?EnableConnections@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableConnections_CCmdTarget__QEAAXXZ() {}

// Symbol: ?EnableTypeLib@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableTypeLib_CCmdTarget__QEAAXXZ() {}

// Symbol: ?EnumOleVerbs@CCmdTarget@@QEAAHPEAPEAUIEnumOLEVERB@@@Z
extern "C" int MS_ABI impl__EnumOleVerbs_CCmdTarget__QEAAHPEAPEAUIEnumOLEVERB___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?ExternalAddRef@CCmdTarget@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__ExternalAddRef_CCmdTarget__QEAAKXZ() {
    return 0;
}

// Symbol: ?ExternalDisconnect@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__ExternalDisconnect_CCmdTarget__QEAAXXZ() {}

// Symbol: ?ExternalQueryInterface@CCmdTarget@@QEAAKPEBXPEAPEAX@Z
extern "C" unsigned long MS_ABI impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(const void* p0, void** p1) {
    return 0;
}

// Symbol: ?ExternalRelease@CCmdTarget@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__ExternalRelease_CCmdTarget__QEAAKXZ() {
    return 0;
}

// Symbol: ?FromIDispatch@CCmdTarget@@SAPEAV1@PEAUIDispatch@@@Z
extern "C" void* MS_ABI impl__FromIDispatch_CCmdTarget__SAPEAV1_PEAUIDispatch___Z() {
    return nullptr;
}

// Symbol: ?GetCommandMap@CCmdTarget@@MEBAPEBUAFX_OLECMDMAP@@XZ
extern "C" void* MS_ABI impl__GetCommandMap_CCmdTarget__MEBAPEBUAFX_OLECMDMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetConnectionHook@CCmdTarget@@MEAAPEAUIConnectionPoint@@AEBU_GUID@@@Z
extern "C" void* MS_ABI impl__GetConnectionHook_CCmdTarget__MEAAPEAUIConnectionPoint__AEBU_GUID___Z(const void* /*struct*/* p0) {
    return nullptr;
}

// Symbol: ?GetConnectionMap@CCmdTarget@@MEBAPEBUAFX_CONNECTIONMAP@@XZ
extern "C" void* MS_ABI impl__GetConnectionMap_CCmdTarget__MEBAPEBUAFX_CONNECTIONMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetControllingUnknown@CCmdTarget@@QEAAPEAUIUnknown@@XZ
extern "C" void* MS_ABI impl__GetControllingUnknown_CCmdTarget__QEAAPEAUIUnknown__XZ() {
    return nullptr;
}

// Symbol: ?GetDispatchIID@CCmdTarget@@UEAAHPEAU_GUID@@@Z
extern "C" int MS_ABI impl__GetDispatchIID_CCmdTarget__UEAAHPEAU_GUID___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetDispatchMap@CCmdTarget@@MEBAPEBUAFX_DISPMAP@@XZ
extern "C" void* MS_ABI impl__GetDispatchMap_CCmdTarget__MEBAPEBUAFX_DISPMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetDispEntry@CCmdTarget@@IEAAPEBUAFX_DISPMAP_ENTRY@@J@Z
extern "C" void* MS_ABI impl__GetDispEntry_CCmdTarget__IEAAPEBUAFX_DISPMAP_ENTRY__J_Z(long p0) {
    return nullptr;
}

// Symbol: ?GetEntryCount@CCmdTarget@@KAIPEBUAFX_DISPMAP@@@Z
extern "C" unsigned int MS_ABI impl__GetEntryCount_CCmdTarget__KAIPEBUAFX_DISPMAP___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetEventSinkEntry@CCmdTarget@@IEAAPEBUAFX_EVENTSINKMAP_ENTRY@@IPEAUAFX_EVENT@@@Z
extern "C" void* MS_ABI impl__GetEventSinkEntry_CCmdTarget__IEAAPEBUAFX_EVENTSINKMAP_ENTRY__IPEAUAFX_EVENT___Z(unsigned int p0, void* /*struct*/* p1) {
    return nullptr;
}

// Symbol: ?GetEventSinkMap@CCmdTarget@@MEBAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" void* MS_ABI impl__GetEventSinkMap_CCmdTarget__MEBAPEBUAFX_EVENTSINKMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetExtraConnectionPoints@CCmdTarget@@MEAAHPEAVCPtrArray@@@Z
extern "C" int MS_ABI impl__GetExtraConnectionPoints_CCmdTarget__MEAAHPEAVCPtrArray___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetIDispatch@CCmdTarget@@QEAAPEAUIDispatch@@H@Z
extern "C" void* MS_ABI impl__GetIDispatch_CCmdTarget__QEAAPEAUIDispatch__H_Z(int p0) {
    return nullptr;
}

// Symbol: ?GetInterface@CCmdTarget@@QEAAPEAUIUnknown@@PEBX@Z
extern "C" void* MS_ABI impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(const void* p0) {
    return nullptr;
}

// Symbol: ?GetInterfaceHook@CCmdTarget@@UEAAPEAUIUnknown@@PEBX@Z
extern "C" void* MS_ABI impl__GetInterfaceHook_CCmdTarget__UEAAPEAUIUnknown__PEBX_Z(const void* p0) {
    return nullptr;
}

// Symbol: ?GetNotSupported@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__GetNotSupported_CCmdTarget__QEAAXXZ() {}

// Symbol: ?GetRoutingFrame@CCmdTarget@@IEAAPEAVCFrameWnd@@XZ
extern "C" void* MS_ABI impl__GetRoutingFrame_CCmdTarget__IEAAPEAVCFrameWnd__XZ() {
    return nullptr;
}

// Symbol: ?GetRoutingFrame_@CCmdTarget@@KAPEAVCFrameWnd@@XZ
extern "C" void* MS_ABI impl__GetRoutingFrame__CCmdTarget__KAPEAVCFrameWnd__XZ() {
    return nullptr;
}

// Symbol: ?GetRoutingView@CCmdTarget@@IEAAPEAVCView@@XZ
extern "C" void* MS_ABI impl__GetRoutingView_CCmdTarget__IEAAPEAVCView__XZ() {
    return nullptr;
}

// Symbol: ?GetRoutingView_@CCmdTarget@@KAPEAVCView@@XZ
extern "C" void* MS_ABI impl__GetRoutingView__CCmdTarget__KAPEAVCView__XZ() {
    return nullptr;
}

// Symbol: ?GetStackSize@CCmdTarget@@KAIPEBEG@Z
extern "C" unsigned int MS_ABI impl__GetStackSize_CCmdTarget__KAIPEBEG_Z(const unsigned char* p0, unsigned short p1) {
    return 0;
}

// Symbol: ?GetStandardProp@CCmdTarget@@IEAAXPEBUAFX_DISPMAP_ENTRY@@PEAUtagVARIANT@@PEAI@Z
extern "C" void MS_ABI impl__GetStandardProp_CCmdTarget__IEAAXPEBUAFX_DISPMAP_ENTRY__PEAUtagVARIANT__PEAI_Z(const void* /*struct*/* p0, void* /*struct*/* p1, unsigned int* p2) {}

// Symbol: ?GetThisCommandMap@CCmdTarget@@KAPEBUAFX_OLECMDMAP@@XZ
extern "C" void* MS_ABI impl__GetThisCommandMap_CCmdTarget__KAPEBUAFX_OLECMDMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetThisConnectionMap@CCmdTarget@@KAPEBUAFX_CONNECTIONMAP@@XZ
extern "C" void* MS_ABI impl__GetThisConnectionMap_CCmdTarget__KAPEBUAFX_CONNECTIONMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetThisDispatchMap@CCmdTarget@@KAPEBUAFX_DISPMAP@@XZ
extern "C" void* MS_ABI impl__GetThisDispatchMap_CCmdTarget__KAPEBUAFX_DISPMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetThisEventSinkMap@CCmdTarget@@KAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" void* MS_ABI impl__GetThisEventSinkMap_CCmdTarget__KAPEBUAFX_EVENTSINKMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetTypeInfoCount@CCmdTarget@@UEAAIXZ
extern "C" unsigned int MS_ABI impl__GetTypeInfoCount_CCmdTarget__UEAAIXZ() {
    return 0;
}

// Symbol: ?GetTypeInfoOfGuid@CCmdTarget@@QEAAJKAEBU_GUID@@PEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfoOfGuid_CCmdTarget__QEAAJKAEBU_GUID__PEAPEAUITypeInfo___Z(unsigned long p0, const void* /*struct*/* p1, void* /*struct*/** p2) {
    return 0;
}

// Symbol: ?GetTypeLib@CCmdTarget@@UEAAJKPEAPEAUITypeLib@@@Z
extern "C" long MS_ABI impl__GetTypeLib_CCmdTarget__UEAAJKPEAPEAUITypeLib___Z(unsigned long p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?InternalQueryInterface@CCmdTarget@@QEAAKPEBXPEAPEAX@Z
extern "C" unsigned long MS_ABI impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(const void* p0, void** p1) {
    return 0;
}

// Symbol: ?InternalRelease@CCmdTarget@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__InternalRelease_CCmdTarget__QEAAKXZ() {
    return 0;
}

// Symbol: ?IsInvokeAllowed@CCmdTarget@@UEAAHJ@Z
extern "C" int MS_ABI impl__IsInvokeAllowed_CCmdTarget__UEAAHJ_Z(long p0) {
    return 0;
}

// Symbol: ?IsResultExpected@CCmdTarget@@QEAAHXZ
extern "C" int MS_ABI impl__IsResultExpected_CCmdTarget__QEAAHXZ() {
    return 0;
}

// Symbol: ?MemberIDFromName@CCmdTarget@@KAJPEBUAFX_DISPMAP@@PEB_W@Z
extern "C" long MS_ABI impl__MemberIDFromName_CCmdTarget__KAJPEBUAFX_DISPMAP__PEB_W_Z(const void* /*struct*/* p0, const wchar_t* p1) {
    return 0;
}

// Symbol: ?OnCreateAggregates@CCmdTarget@@UEAAHXZ
extern "C" int MS_ABI impl__OnCreateAggregates_CCmdTarget__UEAAHXZ() {
    return 0;
}

// Symbol: ?OnEvent@CCmdTarget@@QEAAHIPEAUAFX_EVENT@@PEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnEvent_CCmdTarget__QEAAHIPEAUAFX_EVENT__PEAUAFX_CMDHANDLERINFO___Z(unsigned int p0, void* /*struct*/* p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?OnFinalRelease@CCmdTarget@@UEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_CCmdTarget__UEAAXXZ() {}

// Symbol: ?PushStackArgs@CCmdTarget@@IEAAJPEAEPEBEPEAXGPEAUtagDISPPARAMS@@PEAIPEAUtagVARIANT@@PEAVCVariantBoolConverter@@@Z
extern "C" long MS_ABI impl__PushStackArgs_CCmdTarget__IEAAJPEAEPEBEPEAXGPEAUtagDISPPARAMS__PEAIPEAUtagVARIANT__PEAVCVariantBoolConverter___Z(unsigned char* p0, const unsigned char* p1, void* p2, unsigned short p3, void* /*struct*/* p4, unsigned int* p5, void* /*struct*/* p6, void* /*class*/* p7) {
    return 0;
}

// Symbol: ?QueryAggregates@CCmdTarget@@QEAAPEAUIUnknown@@PEBX@Z
extern "C" void* MS_ABI impl__QueryAggregates_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(const void* p0) {
    return nullptr;
}

// Symbol: ?SetNotSupported@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__SetNotSupported_CCmdTarget__QEAAXXZ() {}

// Symbol: ?SetStandardProp@CCmdTarget@@IEAAJPEBUAFX_DISPMAP_ENTRY@@PEAUtagDISPPARAMS@@PEAI@Z
extern "C" long MS_ABI impl__SetStandardProp_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__PEAUtagDISPPARAMS__PEAI_Z(const void* /*struct*/* p0, void* /*struct*/* p1, unsigned int* p2) {
    return 0;
}
