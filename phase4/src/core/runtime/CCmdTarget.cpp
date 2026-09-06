// CCmdTarget — OpenMFC implementation.
// Sources: appcore.cpp, cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"
#include "detail/CbarcoreSupport.h"

#include <cstddef>
#include <cstring>

#include "detail/InterfaceMapsSupport.h"

// Declared rather than pulled in via detail/MfcExceptionsSupport.h: that header
// drags in CException::classCException, which this translation unit must not
// reference.  Both thunks are defined in detail/MfcExceptionsSupport.cpp.
extern "C" void MS_ABI impl__AfxThrowOleDispatchException__YAXGII_Z(WORD wCode, UINT nDescriptionID, UINT nHelpID);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

// ===========================================================================
// CCmdTarget OLE / automation entry points
// ---------------------------------------------------------------------------
// Everything in the "Moved from ManualThunks.cpp" block below was decoded from
// the retail mfc140u.dll export (RVA cited per function) rather than inferred,
// following the method described in the header of
// phase4/src/core/ole/COleControl.cpp.  Where a struct layout was needed it was
// cross-checked against the shipping MFC headers on this host
// (VC/Tools/MSVC/14.51.36231/atlmfc/include/afxwin.h, afxdisp.h, afxpriv2.h).
//
// Retail CCmdTarget member layout (afxwin.h + the disassembly agree on every
// offset below):
//   +0x00  vfptr
//   +0x08  long           m_dwRef            ExternalAddRef   0x26cd80
//   +0x10  LPUNKNOWN      m_pOuterUnknown    ExternalAddRef   0x26cd80
//   +0x18  DWORD_PTR      m_xInnerUnknown    EnableAggregation 0x26cd70
//   +0x20  XDispatch      m_xDispatch        EnableAutomation  0x24fcf0
//   +0x28  BOOL           m_bResultExpected  IsResultExpected  0x24fd50
//   +0x30  XConnPtContainer m_xConnPtContainer EnableConnections 0x24c230
//   +0x38  AFX_MODULE_STATE* m_pModuleState   InternalRelease   0x26cdb0
//   sizeof == 0x40 (m_pModuleState is the last data member afxwin.h declares)
//
// OpenMFC's CCmdTarget (include/openmfc/afxwin.h) is only 32 bytes -- CObject's
// vfptr plus `char _padding[24]` -- so ONLY m_dwRef, m_pOuterUnknown and
// m_xInnerUnknown are addressable.  Entry points that need +0x20 or beyond are
// either left stubbed or, in InternalRelease's case, implemented without the
// part that needs it; each one says so at its own definition.  Growing
// CCmdTarget to the retail 0x40 bytes is a header change (CWnd's
// _cwnd_padding1[32] would have to shrink to keep m_hWnd at 64) and is NOT
// made here.
//
// Two further deviations apply to the whole block and are repeated where they
// bite: (1) retail reaches GetInterfaceMap / GetInterfaceHook / GetDispatchMap
// / GetEventSinkMap through fixed vtable slots, which is impossible here
// because OpenMFC's C++ vtable is unrelated to retail's, so the three map
// lookups fall back to CCmdTarget's own map and the GetInterfaceHook call is
// skipped entirely; (2) retail null-checks neither `this` nor its pointer
// arguments, whereas these thunks do, matching the style of the rest of this
// file.
//
// One initialisation gap the reference-counting entry points below inherit and
// cannot fix from this file: the retail CCmdTarget constructor (mfc140u
// 0x1de3f0) is
//     m_pModuleState = AfxGetModuleState();   ; call 0x180133930, store +0x38
//     m_dwRef = 1;                            ; mov $1,%ecx ; mov %ecx,0x8
//     m_pOuterUnknown = m_xInnerUnknown = m_xDispatch = m_xConnPtContainer = 0;
//     m_bResultExpected = 1;                  ; mov %ecx,0x28
// so a freshly constructed retail CCmdTarget starts at ONE reference.  OpenMFC
// builds the object with `new (pThis) CCmdTarget()` (CtorDtorPlacement.cpp),
// which value-initialises `char _padding[24]` to zero, so m_dwRef starts at 0
// here.  ExternalAddRef / InternalRelease / ExternalDisconnect below are each
// faithful to their retail body, but they start counting from 0 rather than 1
// until that constructor stores the initial reference.
// ===========================================================================

namespace {

// --- the addressable prefix of retail CCmdTarget -----------------------------
struct S_CCmdTarget {
    void*     vfptr;             // +0x00
    long      m_dwRef;           // +0x08
    long      _pad_0c;
    IUnknown* m_pOuterUnknown;   // +0x10
    void*     m_xInnerUnknown;   // +0x18
};
static_assert(offsetof(S_CCmdTarget, m_dwRef) == 0x08, "CCmdTarget::m_dwRef is at +0x08");
static_assert(offsetof(S_CCmdTarget, m_pOuterUnknown) == 0x10, "CCmdTarget::m_pOuterUnknown is at +0x10");
static_assert(offsetof(S_CCmdTarget, m_xInnerUnknown) == 0x18, "CCmdTarget::m_xInnerUnknown is at +0x18");
static_assert(sizeof(S_CCmdTarget) == 32, "shadow must fit inside OpenMFC's CCmdTarget");
static_assert(sizeof(CCmdTarget) == 32, "OpenMFC CCmdTarget is 32 bytes (afxwin.h)");

inline S_CCmdTarget* Shadow(void* pThis) { return static_cast<S_CCmdTarget*>(pThis); }

// --- map structures ----------------------------------------------------------
// Transcribed from the shipping afxwin.h (_AFXDLL flavour, which is what
// mfc140u is built as: every pfnGetBaseMap slot below is a function pointer,
// and the disassembly indirect-calls through it).  AFX_INTERFACEMAP itself is
// already declared by detail/InterfaceMapsSupport.h and is reused as-is.
enum AFX_DISPMAP_FLAGS_ { afxDispCustom_ = 0, afxDispStock_ = 1 };

struct AFX_DISPMAP_ENTRY {
    const wchar_t*      lpszName;     // +0x00
    long                lDispID;      // +0x08
    const char*         lpszParams;   // +0x10
    unsigned short      vt;           // +0x18
    void*               pfn;          // +0x20
    void*               pfnSet;       // +0x28
    size_t              nPropOffset;  // +0x30
    AFX_DISPMAP_FLAGS_  flags;        // +0x38
};
static_assert(offsetof(AFX_DISPMAP_ENTRY, lDispID) == 0x08, "AFX_DISPMAP_ENTRY::lDispID at +0x08");
static_assert(offsetof(AFX_DISPMAP_ENTRY, vt) == 0x18, "AFX_DISPMAP_ENTRY::vt at +0x18");
static_assert(offsetof(AFX_DISPMAP_ENTRY, nPropOffset) == 0x30, "AFX_DISPMAP_ENTRY::nPropOffset at +0x30");
static_assert(sizeof(AFX_DISPMAP_ENTRY) == 0x40, "AFX_DISPMAP_ENTRY stride is 0x40 (GetEntryCount: add $0x40 / sar $0x6)");

struct AFX_DISPMAP {
    const AFX_DISPMAP* (AFXAPI* pfnGetBaseMap)();  // +0x00
    const AFX_DISPMAP_ENTRY*    lpEntries;         // +0x08
    unsigned int*               lpEntryCount;      // +0x10
    unsigned long*              lpStockPropMask;   // +0x18 (not read by anything here)
};

struct AFX_EVENTSINKMAP_ENTRY {
    AFX_DISPMAP_ENTRY dispEntry;     // +0x00
    unsigned int      nCtrlIDFirst;  // +0x40
    unsigned int      nCtrlIDLast;   // +0x44
};
static_assert(sizeof(AFX_EVENTSINKMAP_ENTRY) == 0x48, "AFX_EVENTSINKMAP_ENTRY stride is 0x48 (GetEventSinkEntry: add $0x48)");
static_assert(offsetof(AFX_EVENTSINKMAP_ENTRY, nCtrlIDFirst) == 0x40, "nCtrlIDFirst at +0x40");
static_assert(offsetof(AFX_EVENTSINKMAP_ENTRY, nCtrlIDLast) == 0x44, "nCtrlIDLast at +0x44");

struct AFX_EVENTSINKMAP {
    const AFX_EVENTSINKMAP* (AFXAPI* pfnGetBaseMap)();
    const AFX_EVENTSINKMAP_ENTRY*    lpEntries;
    unsigned int*                    lpEntryCount;
};

struct AFX_CONNECTIONMAP_ENTRY { const void* piid; size_t nOffset; };
struct AFX_CONNECTIONMAP {
    const AFX_CONNECTIONMAP* (AFXAPI* pfnGetBaseMap)();
    const AFX_CONNECTIONMAP_ENTRY*    pEntry;
};

struct AFX_OLECMDMAP_ENTRY { const GUID* pguid; unsigned long cmdID; unsigned int nID; };
struct AFX_OLECMDMAP {
    const AFX_OLECMDMAP* (AFXAPI* pfnGetBaseMap)();
    const AFX_OLECMDMAP_ENTRY*    lpEntries;
};

// AFX_EVENT (afxpriv2.h).  Only the first two members are read by
// GetEventSinkEntry; the rest are carried so the offsets stay honest.
struct AFX_EVENT {
    int   m_eventKind;      // +0x00   event / propRequest / propChanged / propDSCNotify
    long  m_dispid;         // +0x04
    void* m_pDispParams;    // +0x08
    void* m_pExcepInfo;
    unsigned int* m_puArgError;
    int   m_bPropChanged;
    long  m_hResult;
    int   m_nDSCState;
    int   m_nDSCReason;
};
static_assert(offsetof(AFX_EVENT, m_eventKind) == 0x00, "AFX_EVENT::m_eventKind at +0x00");
static_assert(offsetof(AFX_EVENT, m_dispid) == 0x04, "AFX_EVENT::m_dispid at +0x04");

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

// --- IIDs the retail bodies reference by address -----------------------------
// mfc140u 0x1802d9a58 = {00000000-0000-0000-C000-000000000046} (IID_IUnknown)
// mfc140u 0x1802d9b48 = {00020400-0000-0000-C000-000000000046} (IID_IDispatch)
const GUID kIID_IUnknown  = { 0x00000000, 0x0000, 0x0000, { 0xC0,0,0,0,0,0,0,0x46 } };
const GUID kIID_IDispatch = { 0x00020400, 0x0000, 0x0000, { 0xC0,0,0,0,0,0,0,0x46 } };

// GetStackSize's three lookup tables, copied verbatim out of mfc140u .rdata.
//   0x18034de80  result size,     indexed by VARTYPE          (0x16 entries)
//   0x18034de20  argument size,   byte & VT_MFCBYREF(0x40) set
//   0x18034dee0  argument size,   byte & VT_MFCBYREF(0x40) clear
const unsigned int kStackRetSize[0x16] =
    { 0,0,0,0,0,0,8,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0 };
const unsigned int kStackArgSizeByRef[0x16] =
    { 0,0,8,8,8,8,8,8,8,8,8,8,8,8,8,0,8,8,8,8,8,8 };
const unsigned int kStackArgSizeByVal[0x16] =
    { 0,0,8,8,4,8,8,8,8,8,4,4,8,8,8,0,8,8,8,8,8,8 };

} // namespace

// Forward declarations for the thunks this file calls from other thunks in it.
extern "C" unsigned long MS_ABI impl__ExternalAddRef_CCmdTarget__QEAAKXZ(CCmdTarget*);
extern "C" unsigned long MS_ABI impl__InternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget*);
extern "C" unsigned long MS_ABI impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget*, const void*, void**);
extern "C" IUnknown* MS_ABI impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(CCmdTarget*, const void*);
extern "C" unsigned int MS_ABI impl__GetEntryCount_CCmdTarget__KAIPEBUAFX_DISPMAP___Z(const void*);
extern "C" long MS_ABI impl__MemberIDFromName_CCmdTarget__KAJPEBUAFX_DISPMAP__PEB_W_Z(const void*, const wchar_t*);
extern "C" const void* MS_ABI impl__GetDispatchMap_CCmdTarget__MEBAPEBUAFX_DISPMAP__XZ(const CCmdTarget*);
extern "C" const void* MS_ABI impl__GetEventSinkMap_CCmdTarget__MEBAPEBUAFX_EVENTSINKMAP__XZ(const CCmdTarget*);

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

// CCmdTarget::CallMemberFunc -- STUB.  Retail (mfc140u 0x2501b0) pushes an
// AFX_MAINTAIN_STATE2 (ctor at 0x180133170) built from this->m_pModuleState
// (+0x38), sizes a raw argument block with GetStackSize (0x24fd70), marshals
// the DISPPARAMS into it with PushStackArgs (0x24fdf0) and then calls the
// AFX_DISPMAP_ENTRY's member-function pointer through hand-written thunk code.  None of that machinery exists in OpenMFC
// (no m_pModuleState -- see the layout note above -- and no PushStackArgs
// implementation), so this is left as a no-op returning S_OK-shaped 0.
// Symbol: ?CallMemberFunc@CCmdTarget@@IEAAJPEBUAFX_DISPMAP_ENTRY@@GPEAUtagVARIANT@@PEAUtagDISPPARAMS@@PEAI@Z
extern "C" long MS_ABI impl__CallMemberFunc_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__GPEAUtagVARIANT__PEAUtagDISPPARAMS__PEAI_Z(CCmdTarget* pThis, const void* pEntry, unsigned short wFlags, void* pvarResult, void* pDispParams, unsigned int* puArgErr) {
    (void)pThis; (void)pEntry; (void)wFlags; (void)pvarResult; (void)pDispParams; (void)puArgErr;
    return 0;
}

// CCmdTarget::DoOleVerb -- STUB.  Retail (mfc140u 0x2711e0) walks the MESSAGE
// map chain -- the virtual at vtable+0x60 is GetMessageMap -- looking for the
// records ON_OLEVERB / ON_STDOLEVERB plant there: afxole.h emits them as
// AFX_MSGMAP_ENTRYs with nMessage == 0xC002 and nCode == 0, and the retail code
// matches exactly that (cmpl $0xc002,(%rbx) / cmpl $0x0,0x4(%rbx)), walks by
// the 0x20-byte entry stride and finally calls the entry's pfn at +0x18 as
// (this->*pfn)(lpMsg, hWndParent, lpRect).
// It has TWO walks, selected on the sign of iVerb (test %edi,%edi / jns):
//   * iVerb < 0 (standard verbs, 0x180271222): the entry matches when
//     nID <= iVerb <= nLastID -- the +0x08/+0x0c pair ON_STDOLEVERB fills with
//     the verb number.
//   * iVerb >= 0 (custom verbs, 0x18027127c): the entry matches when
//     nID <= 1 and nLastID >= 1 -- the constants ON_OLEVERB plants -- and a
//     counter (esi) then skips the first iVerb such records.
// Both walks return FALSE (xor %eax,%eax at 0x1802712b2) when nothing matches;
// only a matching pfn that itself returns FALSE reaches 0x1802712c1.
// OpenMFC has no ON_OLEVERB macro, so no OpenMFC class can put such an entry in
// its message map, and CCmdTarget's own map (defined earlier in this file) is a
// single AfxSig_end terminator: the walk would find nothing for every object.
// Returning FALSE unconditionally is therefore the same answer the transcribed
// walk would give, so the walk is not written out.
// Symbol: ?DoOleVerb@CCmdTarget@@QEAAHJPEAUtagMSG@@PEAUHWND__@@PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__DoOleVerb_CCmdTarget__QEAAHJPEAUtagMSG__PEAUHWND____PEBUtagRECT___Z(CCmdTarget* pThis, long iVerb, void* lpMsg, void* hWndParent, const void* lpRect) {
    (void)pThis; (void)iVerb; (void)lpMsg; (void)hWndParent; (void)lpRect;
    return 0;
}

// CCmdTarget::EnableAggregation -- STUB.  Retail (mfc140u 0x26cd70) is
// literally `lea 0x180331188,%rax ; mov %rax,0x18(%rcx) ; ret`.  The table at
// 0x180331188 holds ?QueryInterface@CInnerUnknown@@, ?AddRef@CInnerUnknown@@,
// ?Release@CInnerUnknown@@, so this parks CInnerUnknown's IUnknown vtable in
// m_xInnerUnknown (+0x18).
// That offset IS addressable here, but OpenMFC implements no such nested
// IUnknown, and GetInterface treats a non-zero word at an interface-map offset
// as "this sub-object is live".  Storing a pointer to something that cannot be
// called would be worse than doing nothing, so nothing is stored.
// Symbol: ?EnableAggregation@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableAggregation_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) { (void)pThis; }

// CCmdTarget::EnableAutomation -- STUB.  Retail (mfc140u 0x24fcf0) stores the
// vtable at 0x18032e450 -- whose first slots are ?QueryInterface@
// COleDispatchImpl@@, ?AddRef@COleDispatchImpl@@, ?Release@COleDispatchImpl@@ --
// into m_xDispatch at this+0x20.
// +0x20 is past the end of OpenMFC's 32-byte CCmdTarget (see the layout note
// above), so the store cannot be made without corrupting whatever the derived
// class put there.
// Symbol: ?EnableAutomation@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableAutomation_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) { (void)pThis; }

// CCmdTarget::EnableConnections -- STUB.  Retail (mfc140u 0x24c230) stores the
// vtable at 0x18032c2e0 -- whose first slots are ?QueryInterface@
// COleConnPtContainer@@, ?AddRef@, ?Release@ -- into m_xConnPtContainer at
// this+0x30, which is past the end of OpenMFC's 32-byte CCmdTarget.
// Symbol: ?EnableConnections@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableConnections_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) { (void)pThis; }

// CCmdTarget::EnableTypeLib -- STUB.  Retail (mfc140u 0x26bf60):
//     ?AfxLockGlobals@@YAXH@Z(13)                     ; call 0x180033540
//     pCache = this->vtable[+0x50]()                  ; virtual GetTypeLibCache
//     if (pCache) { if (pCache->[0x30] == 0) pCache->[0x08] = -1;
//                   lock incl pCache->[0x30]; }       ; a refcount
//     LeaveCriticalSection(&<globals lock>)           ; AfxUnlockGlobals, inlined
// The +0x50 virtual really is GetTypeLibCache: GetTypeInfoOfGuid calls the same
// slot and feeds the result straight to CTypeLibCache::Lookup (0x26c360).
// OpenMFC's GetTypeLibCache (defined earlier in this file) hands back an opaque
// TypeLibCacheHandle out of a side map, not a retail-layout CTypeLibCache, so
// the +0x08/+0x30 fields this needs do not exist.
// Symbol: ?EnableTypeLib@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableTypeLib_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) { (void)pThis; }

// CCmdTarget::EnumOleVerbs -- STUB.  Retail (mfc140u 0x270f70) collects the
// same ON_OLEVERB message-map records DoOleVerb matches (nMessage == 0xC002,
// nCode == 0) into a grown array and hands it to a heap-allocated CEnumOleVerb,
// returning that object's IEnumOLEVERB.  OpenMFC has neither the ON_OLEVERB
// macro (so no such record can exist) nor CEnumOleVerb, so the collection half
// cannot be written; *ppEnumOleVerb is cleared and FALSE reported instead.
// The one branch that IS reproducible is retail's argument check: at
// 0x180270f8a, before it touches the message map, 0x270f70 does
// `test %rdx,%rdx ; je 0x1802711da`, and 0x1802711da is `call 0x180227720` =
// ?AfxThrowInvalidArgException@@YAXXZ -- a NULL out-parameter throws rather
// than returning FALSE.  That branch is reproduced here.
// Symbol: ?EnumOleVerbs@CCmdTarget@@QEAAHPEAPEAUIEnumOLEVERB@@@Z
extern "C" int MS_ABI impl__EnumOleVerbs_CCmdTarget__QEAAHPEAPEAUIEnumOLEVERB___Z(CCmdTarget* pThis, void** ppEnumOleVerb) {
    (void)pThis;
    if (!ppEnumOleVerb) { impl__AfxThrowInvalidArgException__YAXXZ(); return 0; }
    *ppEnumOleVerb = nullptr;
    return 0;
}

// CCmdTarget::ExternalAddRef() -- retail mfc140u 0x26cd80:
//     mov 0x10(%rcx),%rdx ; test ; jne -> tail-call m_pOuterUnknown->[+0x08]
//                                        (IUnknown vtable slot 1 = AddRef)
//     otherwise: mov $1,%eax ; lock xadd %eax,0x8(%rcx) ; inc %eax
// i.e. delegate to the controlling unknown when aggregated, else bump m_dwRef.
// Symbol: ?ExternalAddRef@CCmdTarget@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__ExternalAddRef_CCmdTarget__QEAAKXZ(CCmdTarget* pThis) {
    if (!pThis) return 0;
    S_CCmdTarget* p = Shadow(pThis);
    if (p->m_pOuterUnknown != nullptr)
        return p->m_pOuterUnknown->AddRef();
    return static_cast<unsigned long>(
        ::InterlockedIncrement(reinterpret_cast<LONG volatile*>(&p->m_dwRef)));
}

// CCmdTarget::ExternalDisconnect() -- retail mfc140u 0x26d100:
//     if (m_dwRef != 0) {                       ; cmpl $0,0x8(%rcx)
//         LPUNKNOWN pUnk = GetInterface(&IID_IUnknown);
//         lock incl 0x8(%rbx);                  ; hold a reference over the call
//         CoDisconnectObject(pUnk, 0);
//         m_dwRef = 0;
//     }
// DEVIATION: retail passes GetInterface's result to CoDisconnectObject
// unconditionally.  OpenMFC's GetInterface returns NULL for every IID today
// (all g_imap_* tables in detail/InterfaceMapsSupport.cpp hold only the shared
// g_ifaceEnd terminator), and CoDisconnectObject(NULL, 0) would fault, so the
// call is skipped when the lookup comes back NULL.  The refcount bookkeeping is
// unchanged.
// Symbol: ?ExternalDisconnect@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__ExternalDisconnect_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    if (!pThis) return;
    S_CCmdTarget* p = Shadow(pThis);
    if (p->m_dwRef == 0) return;
    IUnknown* pUnk = impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(pThis, &kIID_IUnknown);
    ::InterlockedIncrement(reinterpret_cast<LONG volatile*>(&p->m_dwRef));
    if (pUnk != nullptr)
        ::CoDisconnectObject(pUnk, 0);
    p->m_dwRef = 0;
}

// CCmdTarget::ExternalQueryInterface() -- retail mfc140u 0x26d040:
//     mov 0x10(%rcx),%r9 ; test ; jne -> tail-call m_pOuterUnknown->[+0x00]
//                                       (IUnknown vtable slot 0 = QueryInterface)
//     otherwise: tail-jump to InternalQueryInterface (0x26cfe0)
// Symbol: ?ExternalQueryInterface@CCmdTarget@@QEAAKPEBXPEAPEAX@Z
extern "C" unsigned long MS_ABI impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget* pThis, const void* iid, void** ppvObj) {
    if (!pThis || !iid || !ppvObj) return 0x80004003UL; // E_POINTER
    S_CCmdTarget* p = Shadow(pThis);
    if (p->m_pOuterUnknown != nullptr)
        return static_cast<unsigned long>(
            p->m_pOuterUnknown->QueryInterface(*static_cast<const IID*>(iid), ppvObj));
    return impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(pThis, iid, ppvObj);
}

// CCmdTarget::ExternalRelease() -- retail mfc140u 0x26ce20:
//     mov 0x10(%rcx),%rdx ; test ; jne -> tail-call m_pOuterUnknown->[+0x10]
//                                        (IUnknown vtable slot 2 = Release)
//     otherwise: tail-jump to InternalRelease (0x26cdb0)
// Symbol: ?ExternalRelease@CCmdTarget@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__ExternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis) {
    if (!pThis) return 0;
    S_CCmdTarget* p = Shadow(pThis);
    if (p->m_pOuterUnknown != nullptr)
        return p->m_pOuterUnknown->Release();
    return impl__InternalRelease_CCmdTarget__QEAAKXZ(pThis);
}

// CCmdTarget::FromIDispatch -- STUB.  Retail (mfc140u 0x24fd30) is
//     xor %eax,%eax
//     cmp <low dword of COleDispatchImpl's vtable, 0x18032e450>,(%rcx)
//     lea -0x20(%rcx),%rdx ; cmove %rdx,%rax
// i.e. "if this IDispatch is one of ours (its vptr is the COleDispatchImpl
// vtable EnableAutomation installs),
// back up 0x20 bytes to the owning CCmdTarget".  That identity test needs the
// embedded m_xDispatch sub-object at this+0x20, which OpenMFC's 32-byte
// CCmdTarget does not have (see the layout note above), so there is nothing to
// recognise and NULL is the only honest answer.
// Symbol: ?FromIDispatch@CCmdTarget@@SAPEAV1@PEAUIDispatch@@@Z
extern "C" void* MS_ABI impl__FromIDispatch_CCmdTarget__SAPEAV1_PEAUIDispatch___Z(void* pDispatch) {
    (void)pDispatch;
    return nullptr;
}

// CCmdTarget::GetCommandMap() -- retail mfc140u 0x1de8f0, `lea 0x18033e1b0,%rax
// ; ret`.  The virtual GetCommandMap and the static GetThisCommandMap are
// ICF-folded onto the same instruction pair in the retail image, so both return
// CCmdTarget's own (empty) OLE command map.
// Symbol: ?GetCommandMap@CCmdTarget@@MEBAPEBUAFX_OLECMDMAP@@XZ
extern "C" const void* MS_ABI impl__GetCommandMap_CCmdTarget__MEBAPEBUAFX_OLECMDMAP__XZ(const CCmdTarget* pThis) {
    (void)pThis;
    return &g_commandMap_CCmdTarget;
}

// CCmdTarget::GetConnectionHook() -- retail mfc140u resolves this to 0x71e0,
// the image-wide `xor %eax,%eax ; ret` that identical-COMDAT-folding shares
// between every "return NULL" method.  The base class supplies no extra
// connection point for any IID.
// Symbol: ?GetConnectionHook@CCmdTarget@@MEAAPEAUIConnectionPoint@@AEBU_GUID@@@Z
extern "C" void* MS_ABI impl__GetConnectionHook_CCmdTarget__MEAAPEAUIConnectionPoint__AEBU_GUID___Z(CCmdTarget* pThis, const void* iid) {
    (void)pThis; (void)iid;
    return nullptr;
}

// CCmdTarget::GetConnectionMap() -- retail mfc140u 0x1de8e0, `lea 0x18033e1c0,
// %rax ; ret`, ICF-folded with GetThisConnectionMap.  Returns CCmdTarget's own
// connection map, whose entry table is the single { NULL, (size_t)-1 }
// terminator.
// Symbol: ?GetConnectionMap@CCmdTarget@@MEBAPEBUAFX_CONNECTIONMAP@@XZ
extern "C" const void* MS_ABI impl__GetConnectionMap_CCmdTarget__MEBAPEBUAFX_CONNECTIONMAP__XZ(const CCmdTarget* pThis) {
    (void)pThis;
    return &g_connectionMap_CCmdTarget;
}

// CCmdTarget::GetControllingUnknown() -- retail mfc140u 0x26d140:
//     mov 0x10(%rcx),%rax ; test ; jne -> return m_pOuterUnknown
//     otherwise: lea IID_IUnknown,%rdx ; call GetInterface (0x26ce40)
// Symbol: ?GetControllingUnknown@CCmdTarget@@QEAAPEAUIUnknown@@XZ
extern "C" IUnknown* MS_ABI impl__GetControllingUnknown_CCmdTarget__QEAAPEAUIUnknown__XZ(CCmdTarget* pThis) {
    if (!pThis) return nullptr;
    S_CCmdTarget* p = Shadow(pThis);
    if (p->m_pOuterUnknown != nullptr)
        return p->m_pOuterUnknown;
    return impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(pThis, &kIID_IUnknown);
}

// CCmdTarget::GetDispatchIID() -- retail mfc140u resolves this to the shared
// `xor %eax,%eax ; ret` at 0x71e0: the base class reports FALSE (it has no
// dispatch IID) and leaves the caller's GUID untouched.
// Symbol: ?GetDispatchIID@CCmdTarget@@UEAAHPEAU_GUID@@@Z
extern "C" int MS_ABI impl__GetDispatchIID_CCmdTarget__UEAAHPEAU_GUID___Z(CCmdTarget* pThis, void* pIID) {
    (void)pThis; (void)pIID;
    return 0;
}

// CCmdTarget::GetDispatchMap() -- retail mfc140u 0x1de850, `lea 0x18033e268,
// %rax ; ret`, ICF-folded with GetThisDispatchMap.
// Symbol: ?GetDispatchMap@CCmdTarget@@MEBAPEBUAFX_DISPMAP@@XZ
extern "C" const void* MS_ABI impl__GetDispatchMap_CCmdTarget__MEBAPEBUAFX_DISPMAP__XZ(const CCmdTarget* pThis) {
    (void)pThis;
    return &g_dispatchMap_CCmdTarget;
}

// CCmdTarget::GetDispEntry(LONG memid) -- retail mfc140u 0x24fba0, transcribed:
//     pMap = this->vtable[+0x70]()                  ; virtual GetDispatchMap
//     if (memid == 0) {                             ; DISPID_VALUE
//         walk the whole chain for the entry whose vt == 0xFFF (VT_MFCVALUE,
//         afxdisp.h's "special value for DISPID_VALUE");
//         if none is found fall through to the linear scan below;
//         memid = thatEntry->lDispID;
//         if (memid == DISPID_UNKNOWN)
//             memid = MemberIDFromName(pMap, thatEntry->lpszName);
//         if (memid == DISPID_UNKNOWN) return NULL;
//     }
//     if (memid > 0) {                              ; jle -> linear scan
//         // memid may encode (inheritance depth << 16) | (1-based index)
//         pM = pMap; for (i = 0; i < HIWORD(memid) && pM->pfnGetBaseMap; ++i)
//                        pM = pM->pfnGetBaseMap();
//         if (pM && LOWORD(memid) <= GetEntryCount(pM)) {
//             e = &pM->lpEntries[LOWORD(memid) - 1];
//             if (e->lDispID == DISPID_UNKNOWN || e->lDispID == memid) return e;
//         }
//     }
//     // linear scan of the whole chain by lDispID
//     for (pM = pMap; pM; pM = pM->pfnGetBaseMap ? pM->pfnGetBaseMap() : NULL)
//         for (e = pM->lpEntries; e->nPropOffset != (size_t)-1; ++e)
//             if (e->lDispID == memid) return e;
//     return NULL;
// DEVIATION: the virtual GetDispatchMap (retail vtable slot 14) is replaced by
// CCmdTarget's own map -- OpenMFC's C++ vtable has no relation to retail's.
// CCmdTarget's map is empty, so this currently returns NULL for every memid.
// Symbol: ?GetDispEntry@CCmdTarget@@IEAAPEBUAFX_DISPMAP_ENTRY@@J@Z
extern "C" const void* MS_ABI impl__GetDispEntry_CCmdTarget__IEAAPEBUAFX_DISPMAP_ENTRY__J_Z(CCmdTarget* pThis, long memid) {
    if (!pThis) return nullptr;
    const AFX_DISPMAP* pMap = static_cast<const AFX_DISPMAP*>(
        impl__GetDispatchMap_CCmdTarget__MEBAPEBUAFX_DISPMAP__XZ(pThis));
    if (!pMap) return nullptr;

    if (memid == 0) {
        const AFX_DISPMAP_ENTRY* pValue = nullptr;
        for (const AFX_DISPMAP* pM = pMap; pM != nullptr && pValue == nullptr;
             pM = pM->pfnGetBaseMap ? pM->pfnGetBaseMap() : nullptr) {
            for (const AFX_DISPMAP_ENTRY* e = pM->lpEntries; e->nPropOffset != (size_t)-1; ++e) {
                if (e->vt == 0xfff) { pValue = e; break; }   // VT_MFCVALUE
            }
        }
        if (pValue != nullptr) {
            memid = pValue->lDispID;
            if (memid == -1) {
                memid = impl__MemberIDFromName_CCmdTarget__KAJPEBUAFX_DISPMAP__PEB_W_Z(pMap, pValue->lpszName);
                if (memid == -1) return nullptr;
            }
        }
    }

    if (memid > 0) {
        const unsigned int nInherit = static_cast<unsigned int>(memid >> 16) & 0xffffu;
        const AFX_DISPMAP* pM = pMap;
        for (unsigned int i = 0; i < nInherit; ++i) {
            if (!pM->pfnGetBaseMap) break;
            pM = pM->pfnGetBaseMap();
            if (!pM) break;
        }
        if (pM != nullptr) {
            const unsigned int nIndex = static_cast<unsigned int>(memid) & 0xffffu;
            // The `nIndex != 0` half of this test is an OpenMFC addition: retail
            // computes lpEntries - 0x40 + nIndex * 0x40 unguarded and would read
            // one record before the table when LOWORD(memid) is 0.
            if (nIndex != 0 && nIndex <= impl__GetEntryCount_CCmdTarget__KAIPEBUAFX_DISPMAP___Z(pM)) {
                const AFX_DISPMAP_ENTRY* e = pM->lpEntries + (nIndex - 1);
                if (e->lDispID == -1 || e->lDispID == memid) return e;
            }
        }
    }

    for (const AFX_DISPMAP* pM = pMap; pM != nullptr;
         pM = pM->pfnGetBaseMap ? pM->pfnGetBaseMap() : nullptr) {
        for (const AFX_DISPMAP_ENTRY* e = pM->lpEntries; e->nPropOffset != (size_t)-1; ++e) {
            if (e->lDispID == memid) return e;
        }
    }
    return nullptr;
}

// CCmdTarget::GetEntryCount(const AFX_DISPMAP*) -- retail mfc140u 0x24fac0:
//     pCount = pMap->lpEntryCount                       ; [pMap+0x10]
//     if (*pCount == (UINT)-1) {                        ; not counted yet
//         e = pMap->lpEntries;                          ; [pMap+0x08]
//         while (e->nPropOffset != (size_t)-1) e += 1;   ; add $0x40
//         *pCount = (e - pMap->lpEntries);              ; sub / sar $0x6
//     }
//     return *pCount;
// Symbol: ?GetEntryCount@CCmdTarget@@KAIPEBUAFX_DISPMAP@@@Z
extern "C" unsigned int MS_ABI impl__GetEntryCount_CCmdTarget__KAIPEBUAFX_DISPMAP___Z(const void* pDispMap) {
    const AFX_DISPMAP* pMap = static_cast<const AFX_DISPMAP*>(pDispMap);
    if (!pMap || !pMap->lpEntryCount || !pMap->lpEntries) return 0;
    unsigned int* pCount = pMap->lpEntryCount;
    if (*pCount == 0xffffffffu) {
        const AFX_DISPMAP_ENTRY* e = pMap->lpEntries;
        while (e->nPropOffset != (size_t)-1) ++e;
        *pCount = static_cast<unsigned int>(e - pMap->lpEntries);
    }
    return *pCount;
}

// CCmdTarget::GetEventSinkEntry(UINT idCtrl, AFX_EVENT* pEvent)
// -- retail mfc140u 0x2371c0, transcribed:
//     if (pEvent == NULL) AfxThrowInvalidArgException();
//     pMap = this->vtable[+0x88]()                  ; virtual GetEventSinkMap
//     bProp = (pEvent->m_eventKind != 0) ? 1 : 0    ; setne on [pEvent+0x00]
//     for each map in the chain:
//         for (e = pMap->lpEntries; e->dispEntry.nPropOffset != (size_t)-1; ++e) {
//             if (e->dispEntry.lDispID != pEvent->m_dispid) continue;   ; [pEvent+0x04]
//             if (e->dispEntry.nPropOffset != (size_t)bProp) continue;
//             if (e->nCtrlIDLast == (UINT)-1) {
//                 if (e->nCtrlIDFirst == (UINT)-1 || e->nCtrlIDFirst == idCtrl) return e;
//             } else if (e->nCtrlIDFirst <= idCtrl && idCtrl <= e->nCtrlIDLast) {
//                 return e;
//             }
//         }
//     return NULL;
// DEVIATION: the virtual GetEventSinkMap (retail vtable slot 17) is replaced by
// CCmdTarget's own map, which is empty, so this currently returns NULL.
// Symbol: ?GetEventSinkEntry@CCmdTarget@@IEAAPEBUAFX_EVENTSINKMAP_ENTRY@@IPEAUAFX_EVENT@@@Z
extern "C" const void* MS_ABI impl__GetEventSinkEntry_CCmdTarget__IEAAPEBUAFX_EVENTSINKMAP_ENTRY__IPEAUAFX_EVENT___Z(CCmdTarget* pThis, unsigned int idCtrl, void* pEventV) {
    if (!pThis) return nullptr;
    if (!pEventV) { impl__AfxThrowInvalidArgException__YAXXZ(); return nullptr; }
    const AFX_EVENT* pEvent = static_cast<const AFX_EVENT*>(pEventV);
    const size_t bProp = (pEvent->m_eventKind != 0) ? 1u : 0u;

    for (const AFX_EVENTSINKMAP* pMap = static_cast<const AFX_EVENTSINKMAP*>(
             impl__GetEventSinkMap_CCmdTarget__MEBAPEBUAFX_EVENTSINKMAP__XZ(pThis));
         pMap != nullptr;
         pMap = pMap->pfnGetBaseMap ? pMap->pfnGetBaseMap() : nullptr) {
        for (const AFX_EVENTSINKMAP_ENTRY* e = pMap->lpEntries;
             e->dispEntry.nPropOffset != (size_t)-1; ++e) {
            if (e->dispEntry.lDispID != pEvent->m_dispid) continue;
            if (e->dispEntry.nPropOffset != bProp) continue;
            if (e->nCtrlIDLast == 0xffffffffu) {
                if (e->nCtrlIDFirst == 0xffffffffu || e->nCtrlIDFirst == idCtrl) return e;
            } else if (e->nCtrlIDFirst <= idCtrl && idCtrl <= e->nCtrlIDLast) {
                return e;
            }
        }
    }
    return nullptr;
}

// CCmdTarget::GetEventSinkMap() -- retail mfc140u 0x1de860, `lea 0x18033e200,
// %rax ; ret`, ICF-folded with GetThisEventSinkMap.
// Symbol: ?GetEventSinkMap@CCmdTarget@@MEBAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" const void* MS_ABI impl__GetEventSinkMap_CCmdTarget__MEBAPEBUAFX_EVENTSINKMAP__XZ(const CCmdTarget* pThis) {
    (void)pThis;
    return &g_eventSinkMap_CCmdTarget;
}

// CCmdTarget::GetExtraConnectionPoints() -- retail mfc140u resolves this to the
// shared `xor %eax,%eax ; ret` at 0x71e0: the base class adds no extra
// connection points and leaves the CPtrArray alone.
// Symbol: ?GetExtraConnectionPoints@CCmdTarget@@MEAAHPEAVCPtrArray@@@Z
extern "C" int MS_ABI impl__GetExtraConnectionPoints_CCmdTarget__MEAAHPEAVCPtrArray___Z(CCmdTarget* pThis, void* pConnPoints) {
    (void)pThis; (void)pConnPoints;
    return 0;
}

// CCmdTarget::GetIDispatch(BOOL bAddRef) -- retail mfc140u 0x24fd00:
//     if (bAddRef) ExternalAddRef();                    ; call 0x26cd80
//     tail-jump GetInterface(&IID_IDispatch)            ; rdx = 0x1802d9b48
// Symbol: ?GetIDispatch@CCmdTarget@@QEAAPEAUIDispatch@@H@Z
extern "C" void* MS_ABI impl__GetIDispatch_CCmdTarget__QEAAPEAUIDispatch__H_Z(CCmdTarget* pThis, int bAddRef) {
    if (!pThis) return nullptr;
    if (bAddRef) impl__ExternalAddRef_CCmdTarget__QEAAKXZ(pThis);
    return impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(pThis, &kIID_IDispatch);
}

// CCmdTarget::GetInterface(const void* iid) -- retail mfc140u 0x26ce40:
//     p = this->vtable[+0x98](iid)          ; virtual GetInterfaceHook
//     if (p) return p;
//     pMap = this->vtable[+0x80]()          ; virtual GetInterfaceMap
//     if (*iid == IID_IUnknown)             ; four dword compares vs 0x1802d9a58
//         for each map in the chain, for (e = pMap->pEntry; e->piid; ++e)
//             if (*(DWORD*)((BYTE*)this + e->nOffset) != 0)
//                 return (LPUNKNOWN)((BYTE*)this + e->nOffset);   ; first live one
//     else
//         same walk, but additionally requiring *e->piid == *iid
//     return NULL;
// The "!= 0" test is the live-vtable check: a nested interface sub-object only
// counts once Enable{Automation,Connections,Aggregation} has parked its vtable
// pointer there.
// DEVIATIONS: neither virtual can be dispatched by slot here (OpenMFC's C++
// vtable is unrelated to retail's), so
//   * GetInterfaceHook is not consulted.  CCmdTarget's own GetInterfaceHook is
//     the shared "return NULL" thunk at mfc140u 0x71e0, so skipping it matches
//     the base class exactly; a derived override would be missed.
//   * the map used is always CCmdTarget's own.
// Both are moot today: every g_imap_* in detail/InterfaceMapsSupport.cpp holds
// only the shared g_ifaceEnd = { { nullptr, (size_t)-1 } } terminator, so this
// returns NULL for every IID.  Whoever populates those tables must use OpenMFC
// offsets, not the retail ones -- OpenMFC's objects do not embed the nested COM
// sub-objects at the retail offsets.
// Symbol: ?GetInterface@CCmdTarget@@QEAAPEAUIUnknown@@PEBX@Z
extern "C" IUnknown* MS_ABI impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(CCmdTarget* pThis, const void* iid) {
    if (!pThis || !iid) return nullptr;
    const GUID* pIID = static_cast<const GUID*>(iid);
    const bool bUnknown = (::memcmp(pIID, &kIID_IUnknown, sizeof(GUID)) == 0);

    for (const AFX_INTERFACEMAP* pMap = impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ();
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

// CCmdTarget::GetInterfaceHook() -- retail mfc140u resolves this to the shared
// `xor %eax,%eax ; ret` at 0x71e0.  The base class hooks nothing.
// Symbol: ?GetInterfaceHook@CCmdTarget@@UEAAPEAUIUnknown@@PEBX@Z
extern "C" IUnknown* MS_ABI impl__GetInterfaceHook_CCmdTarget__UEAAPEAUIUnknown__PEBX_Z(CCmdTarget* pThis, const void* iid) {
    (void)pThis; (void)iid;
    return nullptr;
}

// CCmdTarget::GetNotSupported() -- retail mfc140u 0x24fcb0:
//     mov $0xf18c,%edx ; or $-1,%r8d ; mov %edx,%ecx
//     call ?AfxThrowOleDispatchException@@YAXGII@Z
// 0xF18C is AFX_IDP_GET_NOT_SUPPORTED (afxres.h); it is passed as BOTH the
// COleDispatchException code and the description resource id, with nHelpID = -1.
// Symbol: ?GetNotSupported@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__GetNotSupported_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    (void)pThis;
    impl__AfxThrowOleDispatchException__YAXGII_Z(0xf18c, 0xf18c, (UINT)-1);
}

// CCmdTarget::GetRoutingFrame() -- STUB.  Retail (mfc140u 0x1cee30, which the
// static GetRoutingFrame_ is ICF-folded onto) is
//     call ?AfxGetThreadState@@YAPEAV_AFX_THREAD_STATE@@XZ
//     mov 0x1b8(%rax),%rax ; ret
// i.e. it just reads _AFX_THREAD_STATE::m_pRoutingFrame.  OpenMFC's
// _AFX_THREAD_STATE (phase4/src/detail/CWinAppSupport.h) has no such member,
// and nothing in the tree sets one, so there is nothing to return.  See the
// header request that goes with this file.
// Symbol: ?GetRoutingFrame@CCmdTarget@@IEAAPEAVCFrameWnd@@XZ
extern "C" void* MS_ABI impl__GetRoutingFrame_CCmdTarget__IEAAPEAVCFrameWnd__XZ(CCmdTarget* pThis) {
    (void)pThis;
    return nullptr;
}

// CCmdTarget::GetRoutingFrame_() -- STUB.  ICF-folded with GetRoutingFrame in
// the retail image (both resolve to mfc140u 0x1cee30); blocked for the same
// reason -- _AFX_THREAD_STATE has no routing-frame member here.
// Symbol: ?GetRoutingFrame_@CCmdTarget@@KAPEAVCFrameWnd@@XZ
extern "C" void* MS_ABI impl__GetRoutingFrame__CCmdTarget__KAPEAVCFrameWnd__XZ() {
    return nullptr;
}

// CCmdTarget::GetRoutingView() -- STUB.  Retail (mfc140u 0x1cee10, shared with
// the static GetRoutingView_) reads _AFX_THREAD_STATE::m_pRoutingView at
// thread-state offset 0x1a8.  OpenMFC's _AFX_THREAD_STATE has no such member.
// Symbol: ?GetRoutingView@CCmdTarget@@IEAAPEAVCView@@XZ
extern "C" void* MS_ABI impl__GetRoutingView_CCmdTarget__IEAAPEAVCView__XZ(CCmdTarget* pThis) {
    (void)pThis;
    return nullptr;
}

// CCmdTarget::GetRoutingView_() -- STUB.  ICF-folded with GetRoutingView
// (mfc140u 0x1cee10); blocked for the same reason.
// Symbol: ?GetRoutingView_@CCmdTarget@@KAPEAVCView@@XZ
extern "C" void* MS_ABI impl__GetRoutingView__CCmdTarget__KAPEAVCView__XZ() {
    return nullptr;
}

// CCmdTarget::GetStackSize(const BYTE* pbParams, VARTYPE vtResult)
// -- retail mfc140u 0x24fd70, transcribed instruction for instruction:
//     if (vtResult >= 0x16) AfxThrowInvalidArgException();
//     nSize = (kStackRetSize[vtResult] + 0x0f) & ~7;
//     for (b = *pbParams; b != 0; b = *++pbParams) {
//         if (b == 0xff) continue;                  ; VT_MFCMARKER, no stack slot
//         tbl = (b & 0x40) ? kStackArgSizeByRef      ; VT_MFCBYREF
//                          : kStackArgSizeByVal;
//         if ((b & 0xbf) >= 0x16) AfxThrowInvalidArgException();
//         nSize = (nSize + 7 + tbl[b & 0xbf]) & ~7;
//     }
//     return nSize;
// The three tables are copied verbatim from .rdata; see their definition above.
// Symbol: ?GetStackSize@CCmdTarget@@KAIPEBEG@Z
extern "C" unsigned int MS_ABI impl__GetStackSize_CCmdTarget__KAIPEBEG_Z(const unsigned char* pbParams, unsigned short vtResult) {
    if (vtResult >= 0x16) { impl__AfxThrowInvalidArgException__YAXXZ(); return 0; }
    unsigned int nSize = (kStackRetSize[vtResult] + 0x0fu) & ~7u;
    if (!pbParams) return nSize;
    for (const unsigned char* p = pbParams; *p != 0; ++p) {
        const unsigned char b = *p;
        if (b == 0xff) continue;                       // VT_MFCMARKER
        const unsigned int* tbl = (b & 0x40) ? kStackArgSizeByRef : kStackArgSizeByVal;
        const unsigned char idx = static_cast<unsigned char>(b & 0xbf);
        if (idx >= 0x16) { impl__AfxThrowInvalidArgException__YAXXZ(); return nSize; }
        nSize = (nSize + 7u + tbl[idx]) & ~7u;
    }
    return nSize;
}

// CCmdTarget::GetStandardProp -- STUB.  Retail (mfc140u 0x24f7e0) computes
// `(BYTE*)this + pEntry->nPropOffset` (the +0x30 field) and copies that member
// into the VARIANT, switching on pEntry->vt (the WORD at +0x18); pvarResult->vt
// is set from the entry except when the entry's vt is 12 (VT_VARIANT), which is
// passed through untouched.  It is blocked here on two
// counts: OpenMFC declares no dispatch maps at all, so no AFX_DISPMAP_ENTRY
// with a meaningful nPropOffset ever reaches this, and the offsets a retail map
// carries index a retail object layout, not OpenMFC's.
// Symbol: ?GetStandardProp@CCmdTarget@@IEAAXPEBUAFX_DISPMAP_ENTRY@@PEAUtagVARIANT@@PEAI@Z
extern "C" void MS_ABI impl__GetStandardProp_CCmdTarget__IEAAXPEBUAFX_DISPMAP_ENTRY__PEAUtagVARIANT__PEAI_Z(CCmdTarget* pThis, const void* pEntry, void* pvarResult, unsigned int* puArgErr) {
    (void)pThis; (void)pEntry; (void)pvarResult; (void)puArgErr;
}

// CCmdTarget::GetThisCommandMap() -- retail mfc140u 0x1de8f0 (see
// GetCommandMap, with which it is ICF-folded).
// Symbol: ?GetThisCommandMap@CCmdTarget@@KAPEBUAFX_OLECMDMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisCommandMap_CCmdTarget__KAPEBUAFX_OLECMDMAP__XZ() {
    return &g_commandMap_CCmdTarget;
}

// CCmdTarget::GetThisConnectionMap() -- retail mfc140u 0x1de8e0 (ICF-folded
// with GetConnectionMap).
// Symbol: ?GetThisConnectionMap@CCmdTarget@@KAPEBUAFX_CONNECTIONMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisConnectionMap_CCmdTarget__KAPEBUAFX_CONNECTIONMAP__XZ() {
    return &g_connectionMap_CCmdTarget;
}

// CCmdTarget::GetThisDispatchMap() -- retail mfc140u 0x1de850 (ICF-folded with
// GetDispatchMap).
// Symbol: ?GetThisDispatchMap@CCmdTarget@@KAPEBUAFX_DISPMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisDispatchMap_CCmdTarget__KAPEBUAFX_DISPMAP__XZ() {
    return &g_dispatchMap_CCmdTarget;
}

// CCmdTarget::GetThisEventSinkMap() -- retail mfc140u 0x1de860 (ICF-folded with
// GetEventSinkMap).
// Symbol: ?GetThisEventSinkMap@CCmdTarget@@KAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisEventSinkMap_CCmdTarget__KAPEBUAFX_EVENTSINKMAP__XZ() {
    return &g_eventSinkMap_CCmdTarget;
}

// CCmdTarget::GetTypeInfoCount() -- retail mfc140u resolves this to the shared
// `xor %eax,%eax ; ret` at 0x71e0: the base class exposes no type information.
// Symbol: ?GetTypeInfoCount@CCmdTarget@@UEAAIXZ
extern "C" unsigned int MS_ABI impl__GetTypeInfoCount_CCmdTarget__UEAAIXZ(CCmdTarget* pThis) {
    (void)pThis;
    return 0;
}

// CCmdTarget::GetTypeInfoOfGuid -- STUB.  Retail (mfc140u 0x26bfb0):
//     AfxLockGlobals(13); pCache = this->vtable[+0x50]()   ; GetTypeLibCache
//     if (pCache->LookupTypeInfo(lcid, guid, ppTypeInfo))  ; 0x26c400
//         return S_OK;
//     hr = TYPE_E_CANTLOADLIBRARY;
//     if (!pCache->Lookup(lcid, &pTypeLib))                ; 0x26c360
//         if (this->vtable[+0x58](lcid, &pTypeLib) < 0)    ; virtual GetTypeLib
//             load it from GetModuleFileNameW(module state's hCurrentInstance)
//             via the OLEAUT32 ordinal-161 import (LoadTypeLib), caching the
//             ITypeLib back into the CTypeLibCache record at +0x08/+0x10/+0x18;
//     if (pTypeLib) { hr = pTypeLib->GetTypeInfoOfGuid(guid, ppTypeInfo);
//                     pTypeLib->Release();
//                     pCache->CacheTypeInfo(lcid, guid, *ppTypeInfo); }  ; 0x26c470
//     LeaveCriticalSection(...); return hr;
// OpenMFC's GetTypeLibCache returns an opaque TypeLibCacheHandle with none of
// those fields and there is no CTypeLibCache, so the whole path is
// unavailable.  Retail's own failure value, TYPE_E_CANTLOADLIBRARY, is
// returned rather than a fake success.
// Symbol: ?GetTypeInfoOfGuid@CCmdTarget@@QEAAJKAEBU_GUID@@PEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfoOfGuid_CCmdTarget__QEAAJKAEBU_GUID__PEAPEAUITypeInfo___Z(CCmdTarget* pThis, unsigned long lcid, const void* guid, void** ppTypeInfo) {
    (void)pThis; (void)lcid; (void)guid;
    if (ppTypeInfo) *ppTypeInfo = nullptr;
    return static_cast<long>(0x80029c4aL); // TYPE_E_CANTLOADLIBRARY
}

// CCmdTarget::GetTypeLib() -- retail mfc140u 0x1de7a0 is exactly
//     mov $0x80029c4a,%eax ; ret
// The base class has no type library, so it always reports
// TYPE_E_CANTLOADLIBRARY and leaves *ppTypeLib untouched.
// Symbol: ?GetTypeLib@CCmdTarget@@UEAAJKPEAPEAUITypeLib@@@Z
extern "C" long MS_ABI impl__GetTypeLib_CCmdTarget__UEAAJKPEAPEAUITypeLib___Z(CCmdTarget* pThis, unsigned long lcid, void** ppTypeLib) {
    (void)pThis; (void)lcid; (void)ppTypeLib;
    return static_cast<long>(0x80029c4aL); // TYPE_E_CANTLOADLIBRARY
}

// CCmdTarget::InternalQueryInterface() -- retail mfc140u 0x26cfe0:
//     *ppvObj = GetInterface(iid);                  ; call 0x26ce40
//     if (*ppvObj) { ExternalAddRef(); return S_OK; }
//     *ppvObj = QueryAggregates(iid);               ; call 0x26cf50
//     return *ppvObj ? S_OK : E_NOINTERFACE;        ; neg/sbb/not/and 0x80004002
// DEVIATION: the QueryAggregates leg is not taken.  QueryAggregates walks the
// aggregate half of the interface map -- the records that follow the
// piid == NULL terminator, up to nOffset == (size_t)-1 -- and OpenMFC's maps
// have no such records: g_ifaceEnd is the single { nullptr, (size_t)-1 } entry
// that terminates both halves at once, so QueryAggregates would return NULL.
// (The exported impl__QueryAggregates_... thunk further down this file is also
// still a generated stub whose signature carries no `this`.)
// Symbol: ?InternalQueryInterface@CCmdTarget@@QEAAKPEBXPEAPEAX@Z
extern "C" unsigned long MS_ABI impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget* pThis, const void* iid, void** ppvObj) {
    if (!ppvObj) return 0x80004003UL;                  // E_POINTER
    *ppvObj = impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(pThis, iid);
    if (*ppvObj != nullptr) {
        impl__ExternalAddRef_CCmdTarget__QEAAKXZ(pThis);
        return 0;                                      // S_OK
    }
    return 0x80004002UL;                               // E_NOINTERFACE
}

// CCmdTarget::InternalRelease() -- retail mfc140u 0x26cdb0:
//     if (m_dwRef == 0) return 0;                   ; cmpl $0,0x8(%rcx)
//     nRef = InterlockedDecrement(&m_dwRef);        ; lock xadd $-1
//     if (nRef == 0) {
//         AFX_MAINTAIN_STATE2 _state(m_pModuleState);   ; from this+0x38
//         this->vtable[+0x30]();                        ; virtual OnFinalRelease
//     }
//     return nRef;
// DEVIATION: neither the module-state push nor the OnFinalRelease call is made.
// m_pModuleState lives at +0x38, past the end of OpenMFC's 32-byte CCmdTarget
// (see the layout note above), and the virtual cannot be reached by slot
// because OpenMFC's C++ vtable is unrelated to retail's.  Only the reference
// count is maintained, so an object that would have deleted itself on the last
// Release simply stays alive.
// Symbol: ?InternalRelease@CCmdTarget@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__InternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis) {
    if (!pThis) return 0;
    S_CCmdTarget* p = Shadow(pThis);
    if (p->m_dwRef == 0) return 0;
    return static_cast<unsigned long>(
        ::InterlockedDecrement(reinterpret_cast<LONG volatile*>(&p->m_dwRef)));
}

// CCmdTarget::IsInvokeAllowed() -- retail mfc140u 0x3a60 is
//     mov $0x1,%eax ; ret
// The base class allows every DISPID to be invoked; only derived classes
// (COleControl, mfc140u 0x1e2a70) ever refuse.
// Symbol: ?IsInvokeAllowed@CCmdTarget@@UEAAHJ@Z
extern "C" int MS_ABI impl__IsInvokeAllowed_CCmdTarget__UEAAHJ_Z(CCmdTarget* pThis, long dispid) {
    (void)pThis; (void)dispid;
    return 1;
}

// CCmdTarget::IsResultExpected() -- retail mfc140u 0x24fd50 is
//     mov 0x28(%rcx),%eax ; movl $0x1,0x28(%rcx) ; ret
// i.e. it returns m_bResultExpected and resets it to TRUE.  m_bResultExpected
// lives at +0x28, past the end of OpenMFC's 32-byte CCmdTarget, so the flag
// itself cannot be stored here -- but its value in this build is not unknown,
// it is pinned:
//   * the retail CCmdTarget constructor (mfc140u 0x1de3f0) does
//     `mov $0x1,%ecx ... mov %ecx,0x28(%rbx)`, i.e. m_bResultExpected starts
//     TRUE;
//   * this very function resets it to TRUE on every read, so TRUE is also the
//     value every call after the first one sees;
//   * the only code that ever stores FALSE is COleDispatchImpl::Invoke, which
//     sets it from `pvarResult != NULL` before dispatching -- and OpenMFC has
//     no such store anywhere in the tree.
// So TRUE is the value retail would report for every object this build can
// construct; returning the generated 0 would be reporting a state that cannot
// occur here.  Only the reset half of the retail body is unimplementable.
// Symbol: ?IsResultExpected@CCmdTarget@@QEAAHXZ
extern "C" int MS_ABI impl__IsResultExpected_CCmdTarget__QEAAHXZ(CCmdTarget* pThis) {
    (void)pThis;
    return 1;
}

// CCmdTarget::MemberIDFromName(const AFX_DISPMAP* pMap, LPCOLESTR lpszName)
// -- retail mfc140u 0x24faf0, transcribed:
//     for (nInherit = 0; pMap != NULL; ++nInherit) {
//         nCount = GetEntryCount(pMap);
//         for (i = 0; i < nCount; ++i) {
//             e = &pMap->lpEntries[i];
//             if (e->vt == VT_MFCVALUE /*0xFFF*/) continue;
//             if (CompareStringW(0x7f /*LOCALE_INVARIANT*/, NORM_IGNORECASE,
//                                e->lpszName, -1, lpszName, -1) != CSTR_EQUAL)
//                 continue;
//             if (e->lDispID != DISPID_UNKNOWN) return e->lDispID;
//             return (LONG)((WORD)nInherit << 16 | (WORD)(i + 1));
//         }
//         pMap = pMap->pfnGetBaseMap ? pMap->pfnGetBaseMap() : NULL;
//     }
//     return DISPID_UNKNOWN;
// The synthesised DISPID is exactly the (depth << 16 | 1-based index) encoding
// GetDispEntry decodes on its fast path.
// Symbol: ?MemberIDFromName@CCmdTarget@@KAJPEBUAFX_DISPMAP@@PEB_W@Z
extern "C" long MS_ABI impl__MemberIDFromName_CCmdTarget__KAJPEBUAFX_DISPMAP__PEB_W_Z(const void* pDispMap, const wchar_t* lpszName) {
    const AFX_DISPMAP* pMap = static_cast<const AFX_DISPMAP*>(pDispMap);
    if (!lpszName) return -1;
    for (unsigned int nInherit = 0; pMap != nullptr; ++nInherit) {
        const unsigned int nCount = impl__GetEntryCount_CCmdTarget__KAIPEBUAFX_DISPMAP___Z(pMap);
        for (unsigned int i = 0; i < nCount; ++i) {
            const AFX_DISPMAP_ENTRY* e = pMap->lpEntries + i;
            if (e->vt == 0xfff) continue;              // VT_MFCVALUE
            if (::CompareStringW(0x7f /*LOCALE_INVARIANT*/, NORM_IGNORECASE,
                                 e->lpszName, -1, lpszName, -1) != CSTR_EQUAL)
                continue;
            if (e->lDispID != -1) return e->lDispID;
            return static_cast<long>((static_cast<unsigned long>(nInherit & 0xffffu) << 16) |
                                     static_cast<unsigned short>(i + 1));
        }
        pMap = pMap->pfnGetBaseMap ? pMap->pfnGetBaseMap() : nullptr;
    }
    return -1;  // DISPID_UNKNOWN
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
