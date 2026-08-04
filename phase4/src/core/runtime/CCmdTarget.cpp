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
