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
// either left stubbed or, in InternalRelease's and SetStandardProp's case,
// implemented without the part that needs it; each one says so at its own
// definition.  Growing
// CCmdTarget to the retail 0x40 bytes is a header change (CWnd's
// _cwnd_padding1[32] would have to shrink to keep m_hWnd at 64) and is NOT
// made here.
//
// Two further points apply to the whole block and are repeated where they
// bite: (1) retail reaches its own virtuals through fixed MSVC vtable slots.
// GetInterface, GetDispEntry and GetEventSinkEntry below do not: they fall
// back to CCmdTarget's own map and skip GetInterfaceHook.  The bodies that
// were transcribed later -- OnCmdMsg, EnableTypeLib, OnFinalRelease,
// QueryAggregates -- DO dispatch by retail slot index (kVs_* below), on the
// basis that every caller of these exports passes an object with an
// MSVC-layout vtable (client objects, and the hand-authored MSVC tables in
// this tree); an object still carrying g++'s own vtable must not reach them.
// (2) retail null-checks neither `this` nor its pointer arguments; most of
// these thunks check `this` (and some their pointers) anyway, matching the
// style of the rest of this file.  Not every comment below repeats that --
// read the first lines of a body for its checks.  PushStackArgs and
// SetStandardProp check nothing, as retail.
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
extern "C" int MS_ABI impl__OnEvent_CCmdTarget__QEAAHIPEAUAFX_EVENT__PEAUAFX_CMDHANDLERINFO___Z(CCmdTarget*, unsigned int, void*, void*);

// The COccManager* AfxEnableControlContainer was handed.  Defined with
// external C linkage in featurepack/CMFC_misc_stubs.cpp (retail keeps it in
// AFX_MODULE_STATE+0xa0, which OpenMFC's AFX_MODULE_STATE has no room for).
extern "C" void* g4_g_pOccManager;

// Thunks defined in other translation units that the retail bodies below call.
// Each is declared with the parameter list its mangled name describes.
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int nLockType);            // featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int nLockType);          // featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__Lock_CTypeLibCache__QEAAXXZ(void* pCache);        // core/ole/CTypeLibCache.cpp
extern "C" void MS_ABI impl__Unlock_CTypeLibCache__QEAAXXZ(void* pCache);      // core/ole/CTypeLibCache.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                 // ??2@YAPEAX_K@Z, detail/MemcoreSupport.cpp
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* p);                            // ??3@YAXPEAX@Z, detail/MemcoreSupport.cpp
extern "C" void MS_ABI impl__AfxBSTR2CString__YAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEA_W_Z(
    CString* pStr, wchar_t* bstr);                                              // core/collections/Globals.cpp

namespace {

// --- retail (MSVC) vtable slots --------------------------------------------
// The exported CCmdTarget entry points are called with objects that carry an
// MSVC-layout vtable: a client class's own vftable, or one of the
// hand-authored MSVC tables in this tree (e.g. g_CMFCBaseAccessibleObject_vtbl
// in featurepack/controls/CMFCBaseAccessibleObject.cpp, whose slot comments
// list the same CCmdTarget order).  The slot numbers below are each read off a
// retail call site (byte offset / 8), cited where used.  They are NOT the slots
// of g++'s own C++ vtable for CCmdTarget (there GetMessageMap is slot 6, which
// is OnFinalRelease in the MSVC table), so an object that still carries the
// g++ vtable must not be handed to the bodies that use them.
constexpr size_t kVs_ScalarDeletingDtor = 1;   // OnFinalRelease: mov 0x8(%rax)
constexpr size_t kVs_GetTypeLibCache    = 10;  // EnableTypeLib / OnFinalRelease: 0x50
constexpr size_t kVs_GetMessageMap      = 12;  // OnCmdMsg: 0x60
constexpr size_t kVs_GetCommandMap      = 13;  // OnCmdMsg (CN_OLECOMMAND): 0x68
constexpr size_t kVs_GetInterfaceMap    = 16;  // QueryAggregates: 0x80

template <typename Fn>
inline Fn VSlot(const void* pObj, size_t nSlot) {
    return reinterpret_cast<Fn>((*static_cast<void* const* const*>(pObj))[nSlot]);
}

// --- retail message-map layout (afxwin.h, _AFXDLL) -------------------------
// OpenMFC's own AFX_MSGMAP_ENTRY (include/openmfc/afxwin.h) is compiled by g++,
// whose member-function pointers are 16 bytes, so its stride is not retail's.
// The export walks retail-layout maps: 0x20-byte entries (OnCmdMsg's
// `add $0x20,%rax`) with an 8-byte MSVC member-function pointer at +0x18.
// Every map OpenMFC itself exports is a lone AfxSig_end terminator, whose nSig
// word (+0x10) is 0 in either layout, so both kinds end the walk correctly.
struct S_MsgMapEntry {
    unsigned int nMessage;   // +0x00
    unsigned int nCode;      // +0x04
    unsigned int nID;        // +0x08
    unsigned int nLastID;    // +0x0c
    UINT_PTR     nSig;       // +0x10
    void*        pfn;        // +0x18
};
static_assert(sizeof(S_MsgMapEntry) == 0x20, "retail AFX_MSGMAP_ENTRY stride is 0x20");
static_assert(offsetof(S_MsgMapEntry, nSig) == 0x10, "AFX_MSGMAP_ENTRY::nSig at +0x10");
static_assert(offsetof(S_MsgMapEntry, pfn) == 0x18, "AFX_MSGMAP_ENTRY::pfn at +0x18");

struct S_MsgMap {
    const S_MsgMap* (*pfnGetBaseMap)();   // +0x00
    const S_MsgMapEntry* lpEntries;       // +0x08
};

// AFX_CMDHANDLERINFO (afxwin.h): { CCmdTarget* pTarget; AFX_PMSG pmf; }.
struct S_CmdHandlerInfo {
    void* pTarget;   // +0x00
    void* pmf;       // +0x08
};
static_assert(offsetof(S_CmdHandlerInfo, pmf) == 0x08, "AFX_CMDHANDLERINFO::pmf at +0x08");

// The CCmdUI / COleCmdUI members OnCmdMsg touches.  Retail afxwin.h declares
// vfptr, m_nID (+0x08), m_nIndex (+0x0c), m_pMenu (+0x10), m_pSubMenu (+0x18),
// m_pOther (+0x20), m_bEnableChanged (+0x28), m_bContinueRouting (+0x2c),
// m_nIndexMax (+0x30), m_pParentMenu (+0x38); afxdocob.h's COleCmdUI adds
// m_nCmdTextFlag (+0x40), m_rgCmds (+0x48), m_pguidCmdGroup (+0x50).  The
// disassembly reads exactly +0x08, +0x2c and +0x50 (core/controlbar/
// CStatusBar.cpp pins the same +0x2c).
struct S_OleCmdUI {
    void*        vfptr;              // +0x00
    unsigned int m_nID;              // +0x08
    unsigned int m_nIndex;           // +0x0c
    void*        m_pMenu;            // +0x10
    void*        m_pSubMenu;         // +0x18
    void*        m_pOther;           // +0x20
    int          m_bEnableChanged;   // +0x28
    int          m_bContinueRouting; // +0x2c
    unsigned int m_nIndexMax;        // +0x30
    void*        m_pParentMenu;      // +0x38
    unsigned long m_nCmdTextFlag;    // +0x40
    void*        m_rgCmds;           // +0x48
    const GUID*  m_pguidCmdGroup;    // +0x50
};
static_assert(offsetof(S_OleCmdUI, m_nID) == 0x08, "CCmdUI::m_nID at +0x08");
static_assert(offsetof(S_OleCmdUI, m_bContinueRouting) == 0x2c, "CCmdUI::m_bContinueRouting at +0x2c");
static_assert(offsetof(S_OleCmdUI, m_pguidCmdGroup) == 0x50, "COleCmdUI::m_pguidCmdGroup at +0x50");

// AFX_NOTIFY (afxpriv.h): { LRESULT* pResult; NMHDR* pNMHDR; }.
struct S_Notify {
    void* pResult;   // +0x00
    void* pNMHDR;    // +0x08
};

} // namespace

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
    // Simple command routing over OpenMFC's own (g++-layout) message maps.
    // This C++ method is NOT what the exported ?OnCmdMsg@CCmdTarget@@ thunk
    // runs -- see impl__OnCmdMsg_CCmdTarget_... below for the retail body.
    // The root map (CCmdTarget::messageMap, defined above) has a NULL
    // pfnGetBaseMap, so the step must test it before calling through it.
    const AFX_MSGMAP* pMap = GetMessageMap();

    for (; pMap != nullptr; pMap = pMap->pfnGetBaseMap ? (*pMap->pfnGetBaseMap)() : nullptr)
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
// CCmdTarget::GetTypeLibCache() -- retail mfc140u resolves this export (by
// ordinal) to 0x71e0, the image-wide `xor %eax,%eax ; ret` shared by every
// ICF-folded "return NULL" method.  The base class has no type-library cache;
// a class using DECLARE_/IMPLEMENT_OLETYPELIB overrides it (afxdisp.h:839:
// `return AfxGetTypeLibCache(&tlid);`).  EnableTypeLib and OnFinalRelease below both
// test the result for NULL before using it, exactly as retail does.
// (This thunk used to hand out a per-object TypeLibCacheHandle from
// g_targetTypeLibCaches; nothing in the tree consumed that handle.)
// Symbol: ?GetTypeLibCache@CCmdTarget@@UEAAPEAVCTypeLibCache@@XZ
extern "C" void* MS_ABI impl__GetTypeLibCache_CCmdTarget__UEAAPEAVCTypeLibCache__XZ(CCmdTarget* pThis) {
    (void)pThis;
    return nullptr;
}

// === Moved from ManualThunks.cpp ===
// CCmdTarget::OnCmdMsg(UINT nID, int nCode, void* pExtra,
//                      AFX_CMDHANDLERINFO* pHandlerInfo)
// -- retail mfc140u 0x1de460, transcribed (_AfxDispatchCmdMsg is inlined there):
//   if (nCode == CN_EVENT /*-2*/) {
//       ENSURE(AfxGetModuleState()->m_pOccManager);          ; +0xa0, else throw
//       return m_pOccManager->OnEvent(this, nID, pExtra, pHandlerInfo); ; vslot 1
//   }
//   if (nCode == CN_OLECOMMAND /*-3*/) {
//       ENSURE(pExtra);   COleCmdUI* pUI = pExtra;           ; else throw
//       const GUID* pguid = pUI->m_pguidCmdGroup;            ; +0x50
//       BOOL bResult = FALSE;
//       for (pMap = GetCommandMap() /*vslot 13*/; pMap->pfnGetBaseMap && !bResult;
//            pMap = pMap->pfnGetBaseMap())
//           for (e = pMap->lpEntries; e->cmdID && e->nID && !bResult; ++e)
//               if (nID == e->cmdID && IsEqualNULLGuid(pguid, e->pguid))
//                   { pUI->m_nID = e->nID; bResult = TRUE; }  ; +0x08
//       return bResult;
//   }
//   if (nCode == CN_UPDATE_COMMAND_UI /*-1*/) nMsg = WM_COMMAND;  ; nCode kept whole
//   else { nMsg = HIWORD(nCode); nCode = LOWORD(nCode); if (nMsg == 0) nMsg = WM_COMMAND; }
//   for (pMap = GetMessageMap() /*vslot 12*/; pMap->pfnGetBaseMap; pMap = pMap->pfnGetBaseMap())
//       for (e = pMap->lpEntries; e->nSig != AfxSig_end; ++e)
//           if (e->nMessage == nMsg && e->nCode == nCode &&
//               e->nID <= nID && nID <= e->nLastID) goto found;
//   return FALSE;
//  found:
//   if (pHandlerInfo) { pHandlerInfo->pTarget = this; pHandlerInfo->pmf = e->pfn; return TRUE; }
//   switch (e->nSig) { ... }   -- see the case comments below
// The CN_EVENT test runs before anything else, and the walk never scans the
// map whose pfnGetBaseMap is NULL (CCmdTarget's own, which is empty).
//
// DEVIATION, CN_EVENT only: OpenMFC's AFX_MODULE_STATE (detail/RegcoreSupport.h)
// has no m_pOccManager (+0xa0).  AfxEnableControlContainer instead stores its
// argument in the process-wide extern "C" g4_g_pOccManager
// (featurepack/CMFC_misc_stubs.cpp), which is read here:
//   * non-NULL: a caller-supplied manager; forwarded through its vslot 1
//     (`mov 0x8(%rax)` at 0x1801de4ba) exactly as retail does.
//   * NULL: OpenMFC's AfxEnableControlContainer(NULL) does not build retail's
//     default COccManager, so the default manager's OnEvent is inlined here
//     instead.  That OnEvent (mfc140u 0x2373d0, by export ordinal) only drops
//     its own `this` and tail-jumps to CCmdTarget::OnEvent (0x236e70), so the
//     call below is what the default manager would do.  The one case this
//     gets wrong is "AfxEnableControlContainer never called", where retail
//     throws AfxThrowInvalidArgException; it cannot be told apart from
//     "called with NULL" here.
//
// `this` must carry an MSVC-layout vtable: GetMessageMap / GetCommandMap are
// reached through retail slots 12 / 13 (see kVs_* above).  The callers are a
// client object's own vtable (slot 5), a hand-authored MSVC vtable
// (CMFCBaseAccessibleObject slot 5), and core/controlbar/CStatusBar.cpp's
// OnUpdateCmdUI export, which passes its own `this` -- a CStatusBar that the
// OpenMFC ctor export built with g++'s vtable unless a client class derived
// from it and installed its own.  That last caller is only safe for the
// latter kind: on a g++ vtable, slot 12 is an unrelated CWnd virtual.  The
// C++ method
// CCmdTarget::OnCmdMsg earlier in this file is deliberately NOT called: it
// dispatches GetMessageMap through g++'s vtable (slot 6), which in an MSVC
// vtable is OnFinalRelease.
// Retail does not null-check `this`; this thunk returns FALSE for a NULL
// `this`, matching the rest of this file.  Every other NULL check below is
// retail's own: a NULL pExtra where one is required throws through
// AfxThrowInvalidArgException (the `call 0x180227720` the ENSUREs share).
// Symbol: ?OnCmdMsg@CCmdTarget@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CCmdTarget* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo) {
    if (!pThis) return FALSE;

    if (nCode == -2) {                       // CN_EVENT -- see DEVIATION above
        void* const pOccManager = g4_g_pOccManager;
        if (pOccManager != nullptr) {
            using OccOnEventFn = int (MS_ABI*)(void*, void*, unsigned int, void*, void*);
            return VSlot<OccOnEventFn>(pOccManager, 1)(pOccManager, pThis, nID, pExtra, pHandlerInfo);
        }
        return impl__OnEvent_CCmdTarget__QEAAHIPEAUAFX_EVENT__PEAUAFX_CMDHANDLERINFO___Z(
            pThis, nID, pExtra, pHandlerInfo);
    }

    if (nCode == -3) {                       // CN_OLECOMMAND
        if (!pExtra) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
        S_OleCmdUI* pUI = static_cast<S_OleCmdUI*>(pExtra);
        const GUID* pguidCmdGroup = pUI->m_pguidCmdGroup;
        using GetCmdMapFn = const AFX_OLECMDMAP* (MS_ABI*)(const void*);
        int bResult = FALSE;
        for (const AFX_OLECMDMAP* pMap = VSlot<GetCmdMapFn>(pThis, kVs_GetCommandMap)(pThis);
             pMap->pfnGetBaseMap != nullptr && !bResult;
             pMap = pMap->pfnGetBaseMap()) {
            for (const AFX_OLECMDMAP_ENTRY* e = pMap->lpEntries;
                 e->cmdID != 0 && e->nID != 0 && !bResult; ++e) {
                if (nID != e->cmdID) continue;
                // IsEqualNULLGuid: NULL matches only NULL; otherwise memcmp(16).
                const bool bMatch = (pguidCmdGroup == nullptr)
                    ? (e->pguid == nullptr)
                    : (e->pguid != nullptr && ::memcmp(pguidCmdGroup, e->pguid, sizeof(GUID)) == 0);
                if (bMatch) {
                    pUI->m_nID = e->nID;
                    bResult = TRUE;
                }
            }
        }
        return bResult;
    }

    unsigned int nMsg;
    unsigned int nCodeLow;
    if (nCode == -1) {                       // CN_UPDATE_COMMAND_UI: whole value
        nMsg = WM_COMMAND;
        nCodeLow = static_cast<unsigned int>(nCode);
    } else {
        nCodeLow = static_cast<unsigned int>(nCode) & 0xffffu;
        nMsg = (static_cast<unsigned int>(nCode) >> 16) & 0xffffu;
        if (nMsg == 0) nMsg = WM_COMMAND;
    }

    using GetMsgMapFn = const S_MsgMap* (MS_ABI*)(const void*);
    const S_MsgMapEntry* pEntry = nullptr;
    const S_MsgMap* pMap = VSlot<GetMsgMapFn>(pThis, kVs_GetMessageMap)(pThis);
    while (pMap->pfnGetBaseMap != nullptr) {
        for (const S_MsgMapEntry* e = pMap->lpEntries; e->nSig != 0; ++e) {
            if (e->nMessage == nMsg && e->nCode == nCodeLow &&
                e->nID <= nID && nID <= e->nLastID) {
                pEntry = e;
                break;
            }
        }
        if (pEntry != nullptr) break;
        pMap = pMap->pfnGetBaseMap();
    }
    if (pEntry == nullptr) return FALSE;

    void* const pfn = pEntry->pfn;
    if (pHandlerInfo != nullptr) {           // query only: report, do not call
        S_CmdHandlerInfo* pInfo = static_cast<S_CmdHandlerInfo*>(pHandlerInfo);
        pInfo->pTarget = pThis;
        pInfo->pmf = pfn;
        return TRUE;
    }

    // _AfxDispatchCmdMsg, inlined.  pfn is an 8-byte MSVC member-function
    // pointer (single inheritance), i.e. a plain code address called with
    // `this` in RCX.  AfxSig values are retail afxmsg_.h's enum.
    S_Notify* pNotify = static_cast<S_Notify*>(pExtra);
    switch (pEntry->nSig) {
    case 0x3a:   // AfxSigCmd_v: void ()                                  -> TRUE
        reinterpret_cast<void (MS_ABI*)(void*)>(pfn)(pThis);
        return TRUE;
    case 0x3b:   // AfxSigCmd_b: BOOL ()
        return reinterpret_cast<int (MS_ABI*)(void*)>(pfn)(pThis);
    case 0x3c:   // AfxSigCmd_RANGE: void (UINT nID)                      -> TRUE
        reinterpret_cast<void (MS_ABI*)(void*, unsigned int)>(pfn)(pThis, nID);
        return TRUE;
    case 0x3d:   // AfxSigCmd_EX: BOOL (UINT nID)
        return reinterpret_cast<int (MS_ABI*)(void*, unsigned int)>(pfn)(pThis, nID);
    case 0x3e:   // AfxSigNotify_v: void (NMHDR*, LRESULT*)               -> TRUE
        if (!pNotify) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
        reinterpret_cast<void (MS_ABI*)(void*, void*, void*)>(pfn)(pThis, pNotify->pNMHDR, pNotify->pResult);
        return TRUE;
    case 0x3f:   // AfxSigNotify_b: BOOL (NMHDR*, LRESULT*)
        if (!pNotify) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
        return reinterpret_cast<int (MS_ABI*)(void*, void*, void*)>(pfn)(pThis, pNotify->pNMHDR, pNotify->pResult);
    case 0x40:   // AfxSigNotify_RANGE: void (UINT, NMHDR*, LRESULT*)     -> TRUE
        if (!pNotify) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
        reinterpret_cast<void (MS_ABI*)(void*, unsigned int, void*, void*)>(pfn)(pThis, nID, pNotify->pNMHDR, pNotify->pResult);
        return TRUE;
    case 0x41:   // AfxSigNotify_EX: BOOL (UINT, NMHDR*, LRESULT*)
        if (!pNotify) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
        return reinterpret_cast<int (MS_ABI*)(void*, unsigned int, void*, void*)>(pfn)(pThis, nID, pNotify->pNMHDR, pNotify->pResult);
    case 0x42:   // AfxSigCmdUI: void (CCmdUI*)
    case 0x43: { // AfxSigCmdUI_RANGE: void (CCmdUI*, UINT nID)
        if (!pExtra) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
        S_OleCmdUI* pCmdUI = static_cast<S_OleCmdUI*>(pExtra);
        if (pEntry->nSig == 0x42)
            reinterpret_cast<void (MS_ABI*)(void*, void*)>(pfn)(pThis, pCmdUI);
        else
            reinterpret_cast<void (MS_ABI*)(void*, void*, unsigned int)>(pfn)(pThis, pCmdUI, nID);
        // `cmp %ebp,0x2c(%rdi) ; movl $0,0x2c(%rdi) ; sete %bpl`
        const int bResult = (pCmdUI->m_bContinueRouting == 0) ? TRUE : FALSE;
        pCmdUI->m_bContinueRouting = FALSE;
        return bResult;
    }
    case 0x44:   // AfxSigCmd_v_pv: void (void*) -- no NULL check in retail -> TRUE
        reinterpret_cast<void (MS_ABI*)(void*, void*)>(pfn)(pThis, pExtra);
        return TRUE;
    case 0x45:   // AfxSigCmd_b_pv: BOOL (void*) -- no NULL check in retail
        return reinterpret_cast<int (MS_ABI*)(void*, void*)>(pfn)(pThis, pExtra);
    default:     // any other signature is not a command handler
        return FALSE;
    }
}

// CCmdTarget::CallMemberFunc -- STUB.  Retail (mfc140u 0x2501b0) pushes an
// AFX_MAINTAIN_STATE2 (ctor at 0x180133170) built from this->m_pModuleState
// (+0x38), sizes a raw argument block with GetStackSize (0x24fd70), marshals
// the DISPPARAMS into it with PushStackArgs (0x24fdf0) and then calls the
// AFX_DISPMAP_ENTRY's member-function pointer through hand-written thunk code.
// GetStackSize and PushStackArgs are transcribed in this file, but the rest of
// that machinery does not exist in OpenMFC (no m_pModuleState -- see the
// layout note above -- and no call thunk that replays a marshalled argument
// block), so this is left as a no-op returning S_OK-shaped 0.
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
// That offset IS addressable here, but there is no table to store.  Retail's
// CInnerUnknown methods work on the owning object: AddRef (0x26d060) is
// `lock xadd` on this-0x10 (the owner's m_dwRef) and QueryInterface
// (0x26d0b0) answers IID_IUnknown itself and otherwise calls
// InternalQueryInterface on this-0x18.  The CInnerUnknown thunks OpenMFC does
// define (core/ole/CInnerUnknown.cpp) do neither -- they keep a side-table
// refcount keyed by their `this` and answer QI for IUnknown/IClassFactory
// themselves -- so a vtable built from them would give an aggregating outer
// object an inner unknown that never reaches this CCmdTarget.  Nothing is
// stored.
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

// CCmdTarget::EnableTypeLib() -- retail mfc140u 0x26bf60, transcribed:
//     AfxLockGlobals(CRIT_TYPELIBCACHE /*13*/);        ; call 0x180033540
//     CTypeLibCache* pCache = GetTypeLibCache();        ; vslot 10 (+0x50)
//     if (pCache != NULL) pCache->Lock();               ; CTypeLibCache::Lock
//     AfxUnlockGlobals(CRIT_TYPELIBCACHE);              ; inlined
// Lock is inlined there: its body is byte-for-byte the exported
// ?Lock@CTypeLibCache@@ at mfc140u 0x26c340 (`if (m_cRef == 0) m_lcid = -1;
// lock incl m_cRef` -- m_lcid +0x08, m_cRef +0x30 per afxstat_.h:96), and the unlock is AfxUnlockGlobals(13) inlined -- a
// jmp to the LeaveCriticalSection import (0x1802c65a0) on the lock-table
// element 0x1803c3cf8 = 0x1803c3af0 + 13 * 0x28, the table
// ?AfxUnlockGlobals@@YAXH@Z (0x335e0) indexes.  The exported thunks for
// those three are called here.  Note that OpenMFC's CTypeLibCache::Lock
// (core/ole/CTypeLibCache.cpp) keeps its state in a side table keyed by the
// cache pointer rather than in a retail-layout m_cRef.
// `this` must carry an MSVC-layout vtable (see kVs_* above); the caller in
// this tree is COleControl::InitializeIIDs, run on the client's control.
// Symbol: ?EnableTypeLib@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableTypeLib_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    if (!pThis) return;
    impl__AfxLockGlobals__YAXH_Z(13);
    using GetCacheFn = void* (MS_ABI*)(void*);
    void* pCache = VSlot<GetCacheFn>(pThis, kVs_GetTypeLibCache)(pThis);
    if (pCache != nullptr)
        impl__Lock_CTypeLibCache__QEAAXXZ(pCache);
    impl__AfxUnlockGlobals__YAXH_Z(13);
}

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

// CCmdTarget::GetTypeInfoCount() -- retail mfc140u resolves this export (by
// ordinal) to the shared `xor %eax,%eax ; ret` at 0x71e0: the base class
// exposes no type information.  That is the whole retail body, so the body
// below is complete, not a placeholder; IMPLEMENT_OLETYPELIB (afxdisp.h:835)
// is what overrides it with `return 1;`.
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
// Not transcribed: OpenMFC's CTypeLibCache (core/ole/CTypeLibCache.cpp) is a
// side table keyed by the cache pointer rather than the retail record, and the
// module-state LoadTypeLib fallback is not modelled.  Retail's own failure
// value, TYPE_E_CANTLOADLIBRARY, is returned rather than a fake success.
// (Retail passes the vslot-10 result to CTypeLibCache::LookupTypeInfo at
// 0x26c400 without a NULL check, and for the base class that result is NULL.)
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
// DEVIATION: the QueryAggregates leg is not taken.  QueryAggregates (further
// down this file) walks the aggregate half of the object's own interface map,
// reached through retail vtable slot 16, whereas GetInterface above still reads
// CCmdTarget's own map.  The aggregate leg is left out until GetInterface is
// moved onto the same slot, so that both halves of this lookup see the same
// map.  For every map OpenMFC itself exports (g_ifaceEnd, the single
// { nullptr, (size_t)-1 } terminator) the leg would return NULL anyway.
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
// (see the layout note above), and the virtual is not dispatched by retail
// slot 6 because, unlike the slot-dispatching bodies in this file, this one
// is reached from other thunks in the tree (core/ole/COleLinkingDoc.cpp, and
// via ExternalRelease from core/view/CBrowserControlSite.cpp and
// core/ole/COlePropertyPage.cpp) with objects an OpenMFC ctor export may have
// built, i.e. with g++'s vtable, where slot 6 is not OnFinalRelease; and
// m_dwRef starts at 0 here (see the note above).  Only the reference
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

// CCmdTarget::OnCreateAggregates() -- retail mfc140u resolves this export (by
// ordinal) to 0x3a60, the image-wide `mov $0x1,%eax ; ret` shared by every
// ICF-folded "return TRUE" method: the base class creates no aggregates and
// reports success.
// Symbol: ?OnCreateAggregates@CCmdTarget@@UEAAHXZ
extern "C" int MS_ABI impl__OnCreateAggregates_CCmdTarget__UEAAHXZ(CCmdTarget* pThis) {
    (void)pThis;
    return TRUE;
}

// CCmdTarget::OnEvent(UINT idCtrl, AFX_EVENT* pEvent,
//                     AFX_CMDHANDLERINFO* pHandlerInfo)
// -- retail mfc140u 0x236e70 (reached by export ordinal), transcribed:
//   pEntry = GetEventSinkEntry(idCtrl, pEvent);          ; call 0x2371c0
//   if (pEntry == NULL) return FALSE;
//   if (pHandlerInfo != NULL) {                          ; query only
//       pHandlerInfo->pTarget = this;
//       switch (pEvent->m_eventKind) {
//       case event: case propRequest: pHandlerInfo->pmf = pEntry->dispEntry.pfn;    break; ; +0x20
//       case propChanged:             pHandlerInfo->pmf = pEntry->dispEntry.pfnSet; break; ; +0x28
//       }                                                ; other kinds leave pmf alone
//       return pHandlerInfo->pmf != NULL;
//   }
//   bRange = (pEntry->nCtrlIDLast != (UINT)-1);         ; +0x44
//   hResult = S_OK; bHandled = FALSE; uArgError = (UINT)-1;
//   switch (pEvent->m_eventKind) {
//   case event:        VARIANT var = {0};  (AfxVariantInit, memset 0x18)
//                      with bRange, copy *m_pDispParams, grow rgvarg by one
//                      VARIANT (??2, then memcpy of the old ones) and put
//                      VT_I4 idCtrl in the new last slot;
//                      hResult = CallMemberFunc(&pEntry->dispEntry, DISPATCH_METHOD,
//                                               &var, dispparams, &uArgError);  ; 0x2501b0
//                      bHandled = (short)V_BOOL(&var);   ; movswl
//                      with bRange, free the grown rgvarg
//   case propRequest:  bAllow = TRUE; bHandled = pfn([idCtrl,] &bAllow);
//                      hResult = bAllow ? S_OK : S_FALSE;
//   case propChanged:  bHandled = pfnSet([idCtrl]);
//   case propDSCNotify:bAllow = TRUE;
//                      bHandled = pfn([idCtrl,] m_nDSCState, m_nDSCReason, &bAllow); ; +0x28/+0x2c
//                      hResult = bAllow ? S_OK : S_FALSE;
//   }                                                    ; any other kind: FALSE, S_OK
//   if (FAILED(hResult) && pEvent->m_puArgError && uArgError != (UINT)-1)
//       *pEvent->m_puArgError = uArgError;               ; `test %r14d ; jns` skips it
//   pEvent->m_hResult = hResult;                         ; +0x24
//   return bHandled;
// where "[idCtrl,]" is passed only when bRange.  A NULL pEvent never gets
// here: GetEventSinkEntry throws AfxThrowInvalidArgException for it.
// DEVIATIONS:
//   * retail wraps the dispatch switch in TRY/CATCH_ALL (its continuation is
//     at 0x18023712a); no catch is reproduced here, so an exception thrown by
//     a handler propagates to the caller.
//   * a NULL `this` returns FALSE, and a NULL result from ??2 throws
//     AfxThrowInvalidArgException (retail's ??2 never returns NULL; OpenMFC's
//     malloc-based one can).
//   * when the ranged copy finds pDispParams->rgvarg NULL, retail zeroes the
//     new buffer and throws without freeing it (memcpy_s EINVAL path at
//     0x180237110); this frees it first.  Retail frees the grown array with
//     the CRT `free` import; this uses OpenMFC's ??3.
// Both callees are in this file: GetEventSinkEntry still consults CCmdTarget's
// own (empty) event-sink map rather than the virtual one, so today this always
// takes the first `return FALSE`; and CallMemberFunc is still a stub, so the
// `event` kind does not yet reach its handler even once an entry is found.
// Symbol: ?OnEvent@CCmdTarget@@QEAAHIPEAUAFX_EVENT@@PEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnEvent_CCmdTarget__QEAAHIPEAUAFX_EVENT__PEAUAFX_CMDHANDLERINFO___Z(CCmdTarget* pThis, unsigned int idCtrl, void* pEventV, void* pHandlerInfoV) {
    if (!pThis) return FALSE;
    const AFX_EVENTSINKMAP_ENTRY* pEntry = static_cast<const AFX_EVENTSINKMAP_ENTRY*>(
        impl__GetEventSinkEntry_CCmdTarget__IEAAPEBUAFX_EVENTSINKMAP_ENTRY__IPEAUAFX_EVENT___Z(pThis, idCtrl, pEventV));
    if (pEntry == nullptr) return FALSE;
    AFX_EVENT* pEvent = static_cast<AFX_EVENT*>(pEventV);

    if (pHandlerInfoV != nullptr) {
        S_CmdHandlerInfo* pInfo = static_cast<S_CmdHandlerInfo*>(pHandlerInfoV);
        pInfo->pTarget = pThis;
        switch (pEvent->m_eventKind) {
        case 0:     // AFX_EVENT::event
        case 1:     // AFX_EVENT::propRequest
            pInfo->pmf = pEntry->dispEntry.pfn;
            break;
        case 2:     // AFX_EVENT::propChanged
            pInfo->pmf = pEntry->dispEntry.pfnSet;
            break;
        default:
            break;
        }
        return pInfo->pmf != nullptr ? TRUE : FALSE;
    }

    const bool bRange = (pEntry->nCtrlIDLast != 0xffffffffu);
    long hResult = 0;                         // S_OK
    int bHandled = FALSE;
    unsigned int uArgError = 0xffffffffu;
    int bAllow = TRUE;
    void* const pfn = pEntry->dispEntry.pfn;

    switch (pEvent->m_eventKind) {
    case 0: {   // AFX_EVENT::event
        VARIANT var;
        std::memset(&var, 0, sizeof(var));    // AfxVariantInit
        DISPPARAMS dispparams;
        std::memset(&dispparams, 0, sizeof(dispparams));
        DISPPARAMS* pDP = static_cast<DISPPARAMS*>(pEvent->m_pDispParams);
        if (bRange) {
            if (pDP == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
            std::memcpy(&dispparams, pDP, sizeof(DISPPARAMS));
            const unsigned int cArgs = ++dispparams.cArgs;
            VARIANT* rgvarg = static_cast<VARIANT*>(
                impl___2_YAPEAX_K_Z(static_cast<std::size_t>(cArgs) * sizeof(VARIANT)));
            dispparams.rgvarg = rgvarg;
            if (rgvarg == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
            const std::size_t cbOld = static_cast<std::size_t>(cArgs - 1) * sizeof(VARIANT);
            if (cbOld != 0) {
                if (pDP->rgvarg == nullptr) {  // Checked::memcpy_s: EINVAL -> throw
                    impl___3_YAXPEAX_Z(rgvarg);
                    impl__AfxThrowInvalidArgException__YAXXZ();
                    return FALSE;
                }
                std::memcpy(rgvarg, pDP->rgvarg, cbOld);
            }
            VARIANT* pvarID = &rgvarg[cArgs - 1];
            pvarID->vt = VT_I4;
            pvarID->lVal = static_cast<LONG>(idCtrl);
            pDP = &dispparams;
        }
        hResult = impl__CallMemberFunc_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__GPEAUtagVARIANT__PEAUtagDISPPARAMS__PEAI_Z(
            pThis, &pEntry->dispEntry, DISPATCH_METHOD, &var, pDP, &uArgError);
        bHandled = static_cast<short>(var.boolVal);
        if (bRange) impl___3_YAXPEAX_Z(dispparams.rgvarg);
        break;
    }
    case 1:     // AFX_EVENT::propRequest
        if (bRange)
            bHandled = reinterpret_cast<int (MS_ABI*)(void*, unsigned int, int*)>(pfn)(pThis, idCtrl, &bAllow);
        else
            bHandled = reinterpret_cast<int (MS_ABI*)(void*, int*)>(pfn)(pThis, &bAllow);
        hResult = bAllow ? 0 : 1;             // S_OK : S_FALSE
        break;
    case 2: {   // AFX_EVENT::propChanged
        void* const pfnSet = pEntry->dispEntry.pfnSet;
        if (bRange)
            bHandled = reinterpret_cast<int (MS_ABI*)(void*, unsigned int)>(pfnSet)(pThis, idCtrl);
        else
            bHandled = reinterpret_cast<int (MS_ABI*)(void*)>(pfnSet)(pThis);
        break;
    }
    case 3:     // AFX_EVENT::propDSCNotify
        if (bRange)
            bHandled = reinterpret_cast<int (MS_ABI*)(void*, unsigned int, int, int, int*)>(pfn)(
                pThis, idCtrl, pEvent->m_nDSCState, pEvent->m_nDSCReason, &bAllow);
        else
            bHandled = reinterpret_cast<int (MS_ABI*)(void*, int, int, int*)>(pfn)(
                pThis, pEvent->m_nDSCState, pEvent->m_nDSCReason, &bAllow);
        hResult = bAllow ? 0 : 1;             // S_OK : S_FALSE
        break;
    default:
        break;
    }

    if (hResult < 0 && pEvent->m_puArgError != nullptr && uArgError != 0xffffffffu)
        *pEvent->m_puArgError = uArgError;
    pEvent->m_hResult = hResult;
    return bHandled;
}

// CCmdTarget::OnFinalRelease() -- retail mfc140u 0x1de880 (reached by export
// ordinal), transcribed:
//     AfxLockGlobals(CRIT_TYPELIBCACHE /*13*/);          ; call 0x180033540
//     CTypeLibCache* pCache = GetTypeLibCache();          ; vslot 10 (+0x50)
//     if (pCache != NULL) pCache->Unlock();               ; call 0x180133a50
//     AfxUnlockGlobals(CRIT_TYPELIBCACHE);                ; inlined LeaveCriticalSection
//     delete this;                                        ; tail jmp vslot 1 (+0x08), edx = 1
// 0x133a50 is ?Unlock@CTypeLibCache@@QEAAXXZ (by ordinal); the unlock is the
// same inlined AfxUnlockGlobals(13) EnableTypeLib uses (lock-table element
// 0x1803c3cf8).  `delete this` goes through the scalar deleting destructor in
// MSVC vtable slot 1 with flag 1, as retail's tail call does, so `this` must
// carry an MSVC-layout vtable (see kVs_* above).
// Symbol: ?OnFinalRelease@CCmdTarget@@UEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_CCmdTarget__UEAAXXZ(CCmdTarget* pThis) {
    if (!pThis) return;
    impl__AfxLockGlobals__YAXH_Z(13);
    using GetCacheFn = void* (MS_ABI*)(void*);
    void* pCache = VSlot<GetCacheFn>(pThis, kVs_GetTypeLibCache)(pThis);
    if (pCache != nullptr)
        impl__Unlock_CTypeLibCache__QEAAXXZ(pCache);
    impl__AfxUnlockGlobals__YAXH_Z(13);
    using DeletingDtorFn = void* (MS_ABI*)(void*, unsigned int);
    VSlot<DeletingDtorFn>(pThis, kVs_ScalarDeletingDtor)(pThis, 1);
}

// CCmdTarget::PushStackArgs(BYTE* pStack, const BYTE* pbParams, void* pResult,
//     VARTYPE vtResult, DISPPARAMS* pDispParams, UINT* puArgErr,
//     VARIANT* rgTempVars, CVariantBoolConverter* pTempStackArgs)
// -- retail mfc140u 0x24fdf0 (reached by export ordinal), transcribed:
//   *(void**)pStack = this; pStack += 8;
//   if (vtResult == VT_CY || vtResult == VT_VARIANT) { *(void**)pStack = pResult; pStack += 8; }
//   iArg = cArgs; iArgMin = cNamedArgs; bNamedArgStart = FALSE;   ; signed compares (jl / jle)
//   for (pb = pbParams; *pb; ++pb) {
//       --iArg;
//       vt = *pb;  if (vt != VT_MFCMARKER(0xff) && (vt & VT_MFCBYREF(0x40)))
//                      vt = (vt & ~0x40) | VT_BYREF;
//       if (iArg >= iArgMin) {
//           if (vt == VT_MFCMARKER) break;
//           pArg = &rgvarg[iArg];
//           if (vt != VT_VARIANT && vt != pArg->vt) {
//               hr = VariantChangeType(&rgTempVars[iArg], pArg, 0, vt);   ; OLEAUT32 #12
//               if (FAILED(hr)) { *puArgErr = iArg; return hr; }
//               pArg = &rgTempVars[iArg];
//           }
//       } else {
//           if (vt == VT_MFCMARKER) { iArg = cNamedArgs; iArgMin = 0; bNamedArgStart = TRUE; continue; }
//           if (bNamedArgStart || vt != VT_VARIANT) break;
//           static VARIANT vaDefault = { VT_ERROR, .scode = DISP_E_PARAMNOTFOUND };  ; 0x1803c4198
//           pArg = &vaDefault;
//       }
//       push pArg's value in one 8-byte slot (see the switch below);
//       pStack = (pStack + 7) & ~7;
//   }
//   if (iArg > 0)  { *puArgErr = iArg;  return DISP_E_BADPARAMCOUNT; }
//   if (*pb != 0)  { *puArgErr = cArgs; return DISP_E_PARAMNOTOPTIONAL; }
//   return S_OK;
// DEVIATION, VT_BOOL|VT_BYREF with a non-NULL pTempStackArgs only: retail
// allocates a BOOL (??2, 4 bytes) holding *pboolVal != 0, appends the
// {BOOL*, VARIANT_BOOL*} pair to the CVariantBoolConverter (a 0x18-byte-element
// array grown through an unexported SetSize at mfc140u 0x2518a4) so the value
// can be written back after the call, and pushes the BOOL*.  OpenMFC has no
// CVariantBoolConverter (the class is opaque in the shipping headers, and the
// only retail producer is CallMemberFunc, still a stub in this file), so that
// argument is pushed exactly as retail pushes it when pTempStackArgs is NULL:
// the VARIANT_BOOL* itself.
// Symbol: ?PushStackArgs@CCmdTarget@@IEAAJPEAEPEBEPEAXGPEAUtagDISPPARAMS@@PEAIPEAUtagVARIANT@@PEAVCVariantBoolConverter@@@Z
extern "C" long MS_ABI impl__PushStackArgs_CCmdTarget__IEAAJPEAEPEBEPEAXGPEAUtagDISPPARAMS__PEAIPEAUtagVARIANT__PEAVCVariantBoolConverter___Z(CCmdTarget* pThis, unsigned char* pStack, const unsigned char* pbParams, void* pResult, unsigned short vtResult, DISPPARAMS* pDispParams, unsigned int* puArgErr, VARIANT* rgTempVars, void* pTempStackArgs) {
    (void)pTempStackArgs;   // see DEVIATION above
    *reinterpret_cast<void**>(pStack) = pThis;
    pStack += 8;
    if (vtResult == VT_CY || vtResult == VT_VARIANT) {
        *reinterpret_cast<void**>(pStack) = pResult;
        pStack += 8;
    }

    VARIANT* const pArgs = pDispParams->rgvarg;
    int bNamedArgStart = FALSE;
    int iArg = static_cast<int>(pDispParams->cArgs);
    int iArgMin = static_cast<int>(pDispParams->cNamedArgs);
    const unsigned char* pb = pbParams;
    for (; *pb != 0; ++pb) {
        --iArg;
        unsigned short vt = *pb;
        if (vt != 0xff && (vt & 0x40) != 0)
            vt = static_cast<unsigned short>((vt & ~0x40) | VT_BYREF);

        VARIANT* pArg;
        if (iArg >= iArgMin) {
            if (vt == 0xff) break;                      // VT_MFCMARKER
            pArg = &pArgs[iArg];
            if (vt != VT_VARIANT && vt != pArg->vt) {
                VARIANT* pArgTemp = &rgTempVars[iArg];
                const HRESULT hr = ::VariantChangeType(pArgTemp, pArg, 0, vt);
                if (FAILED(hr)) {
                    *puArgErr = static_cast<unsigned int>(iArg);
                    return hr;
                }
                pArg = pArgTemp;
            }
        } else {
            if (vt == 0xff) {                           // start of named args
                iArg = static_cast<int>(pDispParams->cNamedArgs);
                iArgMin = 0;
                bNamedArgStart = TRUE;
                continue;
            }
            if (bNamedArgStart || vt != VT_VARIANT) break;
            static VARIANT s_vaDefault;
            s_vaDefault.vt = VT_ERROR;
            s_vaDefault.scode = DISP_E_PARAMNOTFOUND;
            pArg = &s_vaDefault;
        }

        // The value lives at +0x08 in every VARIANT arm.
        const unsigned char* pVal = reinterpret_cast<const unsigned char*>(pArg) + 8;
        if (vt & VT_BYREF) {                            // `bt $0xe,%si`
            *reinterpret_cast<void**>(pStack) = pArg->byref;
            pStack += 8;
        } else {
            switch (vt) {
            case VT_I2:     *reinterpret_cast<LONG64*>(pStack) = pArg->iVal;  pStack += 8; break;  // movswq
            case VT_I4:
            case VT_ERROR:  *reinterpret_cast<LONG64*>(pStack) = pArg->lVal;  pStack += 8; break;  // movslq
            case VT_R4:     *reinterpret_cast<float*>(pStack) = pArg->fltVal; pStack += 4; break;  // 4-byte store
            case VT_BOOL:   *reinterpret_cast<LONG64*>(pStack) = (pArg->boolVal != 0) ? 1 : 0; pStack += 8; break;
            case VT_I1:     *reinterpret_cast<LONG64*>(pStack) = static_cast<signed char>(pArg->cVal); pStack += 8; break;  // movsbq
            case VT_UI1:    *reinterpret_cast<ULONG64*>(pStack) = pArg->bVal;  pStack += 8; break;  // movzbl
            case VT_UI2:    *reinterpret_cast<ULONG64*>(pStack) = pArg->uiVal; pStack += 8; break;  // movzwl
            case VT_UI4:    *reinterpret_cast<ULONG64*>(pStack) = pArg->ulVal; pStack += 8; break;  // 32-bit mov
            case VT_R8: case VT_CY: case VT_DATE: case VT_BSTR:
            case VT_DISPATCH: case VT_UNKNOWN: case VT_I8: case VT_UI8:
                std::memcpy(pStack, pVal, 8); pStack += 8; break;                                  // qword copy
            case VT_VARIANT:                            // the VARIANT itself, by address
                *reinterpret_cast<VARIANT**>(pStack) = pArg; pStack += 8; break;
            default:                                    // nothing pushed
                break;
            }
        }
        pStack = reinterpret_cast<unsigned char*>(
            (reinterpret_cast<UINT_PTR>(pStack) + 7) & ~static_cast<UINT_PTR>(7));
    }

    if (iArg > 0) {
        *puArgErr = static_cast<unsigned int>(iArg);
        return DISP_E_BADPARAMCOUNT;
    }
    if (*pb != 0) {
        *puArgErr = pDispParams->cArgs;
        return DISP_E_PARAMNOTOPTIONAL;
    }
    return S_OK;
}

// CCmdTarget::QueryAggregates(const void* iid) -- retail mfc140u 0x26cf50
// (reached by export ordinal), transcribed:
//   for (pMap = GetInterfaceMap() /*vslot 16, +0x80*/; ; pMap = pMap->pfnGetBaseMap()) {
//       e = pMap->pEntry;
//       while (e->piid != NULL) ++e;                     ; skip the interface half
//       for (; e->nOffset != (size_t)-1; ++e) {          ; the aggregate half
//           LPUNKNOWN pUnk = *(LPUNKNOWN*)((BYTE*)this + e->nOffset);
//           if (pUnk != NULL) {
//               void* pv = NULL;
//               if (pUnk->QueryInterface(iid, &pv) == S_OK && pv != NULL)
//                   return (LPUNKNOWN)pv;
//           }
//       }
//       if (pMap->pfnGetBaseMap == NULL) return NULL;
//   }
// `this` must carry an MSVC-layout vtable (see kVs_* above).  Every interface
// map OpenMFC itself exports is the lone { NULL, (size_t)-1 } terminator, so
// only a client's own INTERFACE_AGGREGATE entries can be found here.
// InternalQueryInterface (below) does not yet call this; see its comment.
// Symbol: ?QueryAggregates@CCmdTarget@@QEAAPEAUIUnknown@@PEBX@Z
extern "C" IUnknown* MS_ABI impl__QueryAggregates_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(CCmdTarget* pThis, const void* iid) {
    if (!pThis || !iid) return nullptr;
    using GetIfMapFn = const AFX_INTERFACEMAP* (MS_ABI*)(const void*);
    const AFX_INTERFACEMAP* pMap = VSlot<GetIfMapFn>(pThis, kVs_GetInterfaceMap)(pThis);
    for (;;) {
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
}

// CCmdTarget::SetNotSupported() -- retail mfc140u 0x24fcd0 (reached by export
// ordinal):
//     mov $0xf18d,%edx ; or $-1,%r8d ; mov %edx,%ecx
//     call ?AfxThrowOleDispatchException@@YAXGII@Z        ; 0x180251660
// 0xF18D is AFX_IDP_SET_NOT_SUPPORTED (afxres.h:491), passed as both the
// exception code and the description resource id, with nHelpID = -1 -- the
// set-side twin of GetNotSupported above.
// Symbol: ?SetNotSupported@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__SetNotSupported_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    (void)pThis;
    impl__AfxThrowOleDispatchException__YAXGII_Z(0xf18d, 0xf18d, (UINT)-1);
}

// CCmdTarget::SetStandardProp(const AFX_DISPMAP_ENTRY* pEntry,
//     DISPPARAMS* pDispParams, UINT* puArgErr)
// -- retail mfc140u 0x24f900 (reached by export ordinal), transcribed:
//   HRESULT hr = S_OK;  VARIANT va = {0};                ; memset 0x18
//   VARIANT* pArg = &pDispParams->rgvarg[0];
//   if (pEntry->vt != VT_VARIANT && pArg->vt != pEntry->vt) {   ; vt at +0x18
//       hr = VariantChangeType(&va, pArg, 0, pEntry->vt);       ; OLEAUT32 #12
//       if (FAILED(hr)) { *puArgErr = 0; return hr; }
//       pArg = &va;
//   }
//   void* pProp = (BYTE*)this + pEntry->nPropOffset;             ; +0x30
//   switch (pEntry->vt) {
//   VT_I1, VT_UI1:                       1-byte copy
//   VT_I2, VT_UI2:                       2-byte copy
//   VT_I4, VT_R4, VT_ERROR, VT_UI4:      4-byte copy
//   VT_R8, VT_CY, VT_DATE, VT_I8, VT_UI8:8-byte copy
//   VT_BOOL:     *(BOOL*)pProp = (V_BOOL(pArg) != 0);
//   VT_BSTR:     AfxBSTR2CString((CString*)pProp, V_BSTR(pArg));    ; 0x24b2b0
//   VT_DISPATCH, VT_UNKNOWN:
//                if (pArg->punkVal) pArg->punkVal->AddRef();         ; slot 1
//                release the old pointer and NULL it                  ; 0x26ccc4
//                *(LPUNKNOWN*)pProp = pArg->punkVal;
//   VT_VARIANT:  if (VariantCopy((VARIANT*)pProp, pArg) != 0) *puArgErr = 0;  ; #10, hr kept
//   default:     hr = DISP_E_BADVARTYPE; *puArgErr = 0;
//   }
//   VariantClear(&va);                                           ; OLEAUT32 #9
//   if (SUCCEEDED(hr) && pEntry->pfnSet != NULL) {               ; +0x28
//       AFX_MAINTAIN_STATE2 _ctlState(m_pModuleState);           ; this+0x38, 0x133170
//       (this->*pfnSet)();
//   }
//   return hr;
// DEVIATION: the AFX_MAINTAIN_STATE2 push is not made.  m_pModuleState is at
// this+0x38, past OpenMFC's 32-byte CCmdTarget; OpenMFC's exported CCmdTarget
// constructor (core/runtime/CtorDtorPlacement.cpp) never writes that word, so
// even in a client object it holds whatever the allocation left there, and
// handing it to AFX_MAINTAIN_STATE2 would install a garbage module state.
// pfnSet is called without the push.
// Symbol: ?SetStandardProp@CCmdTarget@@IEAAJPEBUAFX_DISPMAP_ENTRY@@PEAUtagDISPPARAMS@@PEAI@Z
extern "C" long MS_ABI impl__SetStandardProp_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__PEAUtagDISPPARAMS__PEAI_Z(CCmdTarget* pThis, const void* pEntryV, DISPPARAMS* pDispParams, unsigned int* puArgErr) {
    const AFX_DISPMAP_ENTRY* pEntry = static_cast<const AFX_DISPMAP_ENTRY*>(pEntryV);
    HRESULT hr = S_OK;
    VARIANT va;
    std::memset(&va, 0, sizeof(va));          // AfxVariantInit
    VARIANT* pArg = &pDispParams->rgvarg[0];
    if (pEntry->vt != VT_VARIANT && pArg->vt != pEntry->vt) {
        hr = ::VariantChangeType(&va, pArg, 0, pEntry->vt);
        if (FAILED(hr)) {
            *puArgErr = 0;
            return hr;
        }
        pArg = &va;
    }

    unsigned char* pProp = reinterpret_cast<unsigned char*>(pThis) + pEntry->nPropOffset;
    const unsigned char* pVal = reinterpret_cast<const unsigned char*>(pArg) + 8;
    switch (pEntry->vt) {
    case VT_I1: case VT_UI1:
        std::memcpy(pProp, pVal, 1); break;
    case VT_I2: case VT_UI2:
        std::memcpy(pProp, pVal, 2); break;
    case VT_I4: case VT_R4: case VT_ERROR: case VT_UI4:
        std::memcpy(pProp, pVal, 4); break;
    case VT_R8: case VT_CY: case VT_DATE: case VT_I8: case VT_UI8:
        std::memcpy(pProp, pVal, 8); break;
    case VT_BOOL:
        *reinterpret_cast<BOOL*>(pProp) = (pArg->boolVal != 0) ? TRUE : FALSE; break;
    case VT_BSTR:
        impl__AfxBSTR2CString__YAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEA_W_Z(
            reinterpret_cast<CString*>(pProp), pArg->bstrVal);
        break;
    case VT_DISPATCH:
    case VT_UNKNOWN: {
        IUnknown* pNew = pArg->punkVal;
        if (pNew != nullptr) pNew->AddRef();
        IUnknown*& rOld = *reinterpret_cast<IUnknown**>(pProp);
        if (rOld != nullptr) { rOld->Release(); rOld = nullptr; }
        rOld = pNew;
        break;
    }
    case VT_VARIANT:
        if (::VariantCopy(reinterpret_cast<VARIANT*>(pProp), pArg) != 0)
            *puArgErr = 0;
        break;
    default:
        hr = DISP_E_BADVARTYPE;
        *puArgErr = 0;
        break;
    }
    ::VariantClear(&va);

    if (SUCCEEDED(hr) && pEntry->pfnSet != nullptr)
        reinterpret_cast<void (MS_ABI*)(void*)>(pEntry->pfnSet)(pThis);
    return hr;
}
