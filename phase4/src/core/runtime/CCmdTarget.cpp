// CCmdTarget — OpenMFC implementation.
// Sources: appcore.cpp, cbarcore.cpp

#define OPENMFC_APPCORE_IMPL
#include "detail/CommandMessageDispatch.h"

#include "detail/CWinAppSupport.h"
#include "detail/CbarcoreSupport.h"

#include <cstddef>
#include <cstring>

#include "detail/InterfaceMapsSupport.h"
#include "detail/CCmdTargetDispatchSupport.h"
#include "detail/CCmdTargetEventsSupport.h"

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
// CCmdTarget now carries all native fields through +0x38. CWnd no longer
// needs surrogate base padding, and its HWND remains at +0x40. The native
// constructor initializes the reference count, result flag, embedded interface
// slots and owning module state. Automation and connection activation below
// still need their actual interface implementations.
// The native virtual prefix supports slot dispatch for the command, interface
// and lifetime methods. Other map lookups still document their remaining gaps.
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
static_assert(sizeof(CCmdTarget) == 64, "native CCmdTarget storage");

inline S_CCmdTarget* Shadow(void* pThis) { return static_cast<S_CCmdTarget*>(pThis); }

// Map layouts are shared with the typed virtual implementation.
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
// MSVC-layout vtable: the native CCmdTarget prefix restored in afxwin.h,
// a client class's own vftable, or a hand-authored MSVC table. The indices
// below come from retail call sites (byte offset / 8). Non-MSVC vtables
// cannot be used with these ABI entry points.
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
// Native x64 MSVC entries have an 8-byte member-function pointer at +0x18
// and a 0x20-byte stride. Keep this raw view for imported client tables.
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
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CCmdTarget*,UINT,int,void*,void*);
int CCmdTarget::OnCmdMsg(unsigned int nID,int nCode,void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo) {
    return impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(this,nID,nCode,pExtra,pHandlerInfo);
}
int PASCAL CCmdTarget::DispatchCmdMsg(CCmdTarget* pTarget, unsigned int nID, int nCode,
                                      AFX_PMSG pfn, void* pExtra, unsigned int nSig, void* pHandlerInfo)
{
    (void)nCode;
    void* function=nullptr;std::memcpy(&function,&pfn,sizeof(function));
    return openmfc::detail::DispatchCommandMessage(pTarget,nID,function,pExtra,nSig,static_cast<AFX_CMDHANDLERINFO*>(pHandlerInfo));
}
// CCmdTarget::GetTypeLibCache() -- retail mfc140u resolves this export (by
// ordinal) to 0x71e0, the image-wide `xor %eax,%eax ; ret` shared by every
// ICF-folded "return NULL" method.  The base class has no type-library cache;
// a class using DECLARE_/IMPLEMENT_OLETYPELIB overrides it (afxdisp.h:839:
// `return AfxGetTypeLibCache(&tlid);`).  EnableTypeLib and OnFinalRelease below both
// test the result for NULL before using it, exactly as retail does.
// (This thunk used to hand out a per-object TypeLibCacheHandle from
// g_targetTypeLibCaches; nothing in the tree consumed that handle.)


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

    return openmfc::detail::DispatchCommandMessage(pThis,nID,pfn,pExtra,pEntry->nSig,nullptr);
}

// CallMemberFunc is implemented in CCmdTargetInvoke.cpp using native automation calls.

// OLE verb dispatch is implemented in CCmdTargetVerbs.cpp.

// --- CCmdTarget's inner (non-delegating) IUnknown ---------------------------
// Retail EnableAggregation (below) stores the vftable at 0x180331188 (mfc140u)
// in m_xInnerUnknown.  Its first three slots are the CInnerUnknown exports,
// resolved by ordinal in mfc140u: QueryInterface (ord 12006, RVA 0x26d0b0),
// AddRef (ord 1954, RVA 0x26d060), Release (ord 12230, RVA 0x26d070).  In
// every one of them `this` is &owner->m_xInnerUnknown, i.e. owner + 0x18.
//
// OpenMFC's own CInnerUnknown thunks (core/ole/CInnerUnknown.cpp, not this
// file's) do not implement those bodies -- they keep a side-table refcount
// keyed by their `this` and answer QI for IUnknown/IClassFactory themselves --
// so a table built from them would give an aggregating outer object an inner
// unknown that never reaches this CCmdTarget.  The three bodies are therefore
// transcribed here as file-local MS_ABI functions and the table below is built
// from them.  DEVIATION: the vftable pointer stored is this file's table, not
// the retail one, so the stored pointer value differs from retail's.  The only
// two references to 0x180331188 in mfc140u's code are stores into +0x18 --
// EnableAggregation itself and an inlined copy at 0x1df86e inside COleControl's
// constructor (0x1df5c0) -- so no retail code compares against it
// (FromIDispatch compares the separate COleDispatchImpl table, 0x18032e450).
namespace {

inline S_CCmdTarget* InnerUnknownOwner(void* pInner) {
    return reinterpret_cast<S_CCmdTarget*>(
        static_cast<char*>(pInner) - offsetof(S_CCmdTarget, m_xInnerUnknown));
}

// CInnerUnknown::QueryInterface -- retail mfc140u 0x26d0b0, transcribed:
//     if (memcmp(&iid, &IID_IUnknown, 16) == 0) {   ; call *memcmp (IAT slot
//                                                   ;   0x1802c73e0, VCRUNTIME140)
//         InterlockedIncrement(&owner->m_dwRef);    ; lock incl -0x10(%rsi)
//         *ppvObj = this;                           ; mov %rsi,(%rdi)
//         return 0;                                 ; eax still holds memcmp's 0
//     }
//     return owner->InternalQueryInterface(&iid, ppvObj);   ; call 0x26cfe0
// Neither iid nor ppvObj is null-checked, as retail.
long MS_ABI InnerUnknown_QueryInterface(void* pThis, const GUID* iid, void** ppvObj) {
    S_CCmdTarget* pOwner = InnerUnknownOwner(pThis);
    if (::memcmp(iid, &kIID_IUnknown, sizeof(GUID)) == 0) {
        ::InterlockedIncrement(reinterpret_cast<LONG volatile*>(&pOwner->m_dwRef));
        *ppvObj = pThis;
        return 0;                                      // S_OK
    }
    return static_cast<long>(impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(
        reinterpret_cast<CCmdTarget*>(pOwner), iid, ppvObj));
}

// CInnerUnknown::AddRef -- retail mfc140u 0x26d060:
//     mov $1,%eax ; lock xadd %eax,-0x10(%rcx) ; inc %eax ; ret
// i.e. InterlockedIncrement(&owner->m_dwRef), returning the new count.  It
// bumps m_dwRef directly -- it does NOT go through ExternalAddRef, so it never
// delegates to m_pOuterUnknown.
unsigned long MS_ABI InnerUnknown_AddRef(void* pThis) {
    return static_cast<unsigned long>(::InterlockedIncrement(
        reinterpret_cast<LONG volatile*>(&InnerUnknownOwner(pThis)->m_dwRef)));
}

// CInnerUnknown::Release -- retail mfc140u 0x26d070:
//     AFX_MAINTAIN_STATE2 _state(owner->m_pModuleState); ; owner+0x38, ctor 0x133170
//     return owner->InternalRelease();                    ; call 0x26cdb0
// (the destructor's restore leaves eax, InternalRelease's result, untouched).
// InternalRelease establishes the owning module context for its final callback.
unsigned long MS_ABI InnerUnknown_Release(void* pThis) {
    return impl__InternalRelease_CCmdTarget__QEAAKXZ(
        reinterpret_cast<CCmdTarget*>(InnerUnknownOwner(pThis)));
}

void* const kInnerUnknownVtbl[3] = {
    reinterpret_cast<void*>(&InnerUnknown_QueryInterface),   // slot 0
    reinterpret_cast<void*>(&InnerUnknown_AddRef),           // slot 1
    reinterpret_cast<void*>(&InnerUnknown_Release),          // slot 2
};

} // namespace

// CCmdTarget::EnableAggregation() -- retail mfc140u 0x26cd70 is literally
//     lea 0x180331188,%rax ; mov %rax,0x18(%rcx) ; ret
// i.e. m_xInnerUnknown = <CInnerUnknown vftable>.  Transcribed, storing the
// file-local table above (see the DEVIATION note there).  Retail does not
// null-check `this`; this thunk does, as most in this file do.
// Symbol: ?EnableAggregation@CCmdTarget@@QEAAXXZ
extern "C" void MS_ABI impl__EnableAggregation_CCmdTarget__QEAAXXZ(CCmdTarget* pThis) {
    if (!pThis) return;
    Shadow(pThis)->m_xInnerUnknown = const_cast<void**>(&kInnerUnknownVtbl[0]);
}

// EnableAutomation is implemented in CCmdTargetDispatch.cpp with a live owner-backed table.

// Connection activation is implemented in core/ole/COleConnPtContainer.cpp.

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
// those three are called here. CTypeLibCache::Lock updates the native
// m_cRef member at +0x30.
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

// OLE verb enumeration is implemented in CCmdTargetVerbs.cpp.

// CCmdTarget::ExternalAddRef() -- retail mfc140u 0x26cd80:
//     mov 0x10(%rcx),%rdx ; test ; jne -> tail-call m_pOuterUnknown->[+0x08]
//                                        (IUnknown vtable slot 1 = AddRef)
//     otherwise: mov $1,%eax ; lock xadd %eax,0x8(%rcx) ; inc %eax
// i.e. delegate to the controlling unknown when aggregated, else bump m_dwRef.


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


// CCmdTarget::ExternalQueryInterface() -- retail mfc140u 0x26d040:
//     mov 0x10(%rcx),%r9 ; test ; jne -> tail-call m_pOuterUnknown->[+0x00]
//                                       (IUnknown vtable slot 0 = QueryInterface)
//     otherwise: tail-jump to InternalQueryInterface (0x26cfe0)


// CCmdTarget::ExternalRelease() -- retail mfc140u 0x26ce20:
//     mov 0x10(%rcx),%rdx ; test ; jne -> tail-call m_pOuterUnknown->[+0x10]
//                                        (IUnknown vtable slot 2 = Release)
//     otherwise: tail-jump to InternalRelease (0x26cdb0)


// FromIDispatch uses the installed dispatch table identity in CCmdTargetDispatch.cpp.

// CCmdTarget::GetCommandMap() -- retail mfc140u 0x1de8f0, `lea 0x18033e1b0,%rax
// ; ret`.  The virtual GetCommandMap and the static GetThisCommandMap are
// ICF-folded onto the same instruction pair in the retail image, so both return
// CCmdTarget's own (empty) OLE command map.


// CCmdTarget::GetConnectionHook() -- retail mfc140u resolves this to 0x71e0,
// the image-wide `xor %eax,%eax ; ret` that identical-COMDAT-folding shares
// between every "return NULL" method.  The base class supplies no extra
// connection point for any IID.


// CCmdTarget::GetConnectionMap() -- retail mfc140u 0x1de8e0, `lea 0x18033e1c0,
// %rax ; ret`, ICF-folded with GetThisConnectionMap.  Returns CCmdTarget's own
// connection map, whose entry table is the single { NULL, (size_t)-1 }
// terminator.


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


// CCmdTarget::GetDispatchMap() -- retail mfc140u 0x1de850, `lea 0x18033e268,
// %rax ; ret`, ICF-folded with GetThisDispatchMap.


// Dispatch lookup, entry counts and names are implemented in CCmdTargetDispatch.cpp.

// Event-map lookup is implemented in CCmdTargetEvents.cpp.

// CCmdTarget::GetEventSinkMap() -- retail mfc140u 0x1de860, `lea 0x18033e200,
// %rax ; ret`, ICF-folded with GetThisEventSinkMap.


// CCmdTarget::GetExtraConnectionPoints() -- retail mfc140u resolves this to the
// shared `xor %eax,%eax ; ret` at 0x71e0: the base class adds no extra
// connection points and leaves the CPtrArray alone.


// CCmdTarget::GetIDispatch(BOOL bAddRef) -- retail mfc140u 0x24fd00:
//     if (bAddRef) ExternalAddRef();                    ; call 0x26cd80
//     tail-jump GetInterface(&IID_IDispatch)            ; rdx = 0x1802d9b48


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
// Implemented in CCmdTargetVirtuals.cpp with virtual hook and map dispatch.

// CCmdTarget::GetInterfaceHook() -- retail mfc140u resolves this to the shared
// `xor %eax,%eax ; ret` at 0x71e0.  The base class hooks nothing.


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

// Native routing getters live in CommandRouting.cpp.

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

// Standard property reads are implemented in CCmdTargetDispatch.cpp.

// CCmdTarget::GetThisCommandMap() -- retail mfc140u 0x1de8f0 (see
// GetCommandMap, with which it is ICF-folded).


// CCmdTarget::GetThisConnectionMap() -- retail mfc140u 0x1de8e0 (ICF-folded
// with GetConnectionMap).


// CCmdTarget::GetThisDispatchMap() -- retail mfc140u 0x1de850 (ICF-folded with
// GetDispatchMap).


// CCmdTarget::GetThisEventSinkMap() -- retail mfc140u 0x1de860 (ICF-folded with
// GetEventSinkMap).


// CCmdTarget::GetTypeInfoCount() -- retail mfc140u resolves this export (by
// ordinal) to the shared `xor %eax,%eax ; ret` at 0x71e0: the base class
// exposes no type information.  That is the whole retail body, so the body
// below is complete, not a placeholder; IMPLEMENT_OLETYPELIB (afxdisp.h:835)
// is what overrides it with `return 1;`.


// Cached type information and module fallback are implemented in CCmdTargetTypeInfo.cpp.

// CCmdTarget::GetTypeLib() -- retail mfc140u 0x1de7a0 is exactly
//     mov $0x80029c4a,%eax ; ret
// The base class has no type library, so it always reports
// TYPE_E_CANTLOADLIBRARY and leaves *ppTypeLib untouched.


// CCmdTarget::InternalQueryInterface() -- retail mfc140u 0x26cfe0:
//     *ppvObj = GetInterface(iid);                  ; call 0x26ce40
//     if (*ppvObj) { ExternalAddRef(); return S_OK; }
//     *ppvObj = QueryAggregates(iid);               ; call 0x26cf50
//     return *ppvObj ? S_OK : E_NOINTERFACE;        ; neg/sbb/not/and 0x80004002
// Implemented in CCmdTargetVirtuals.cpp, including aggregate lookup.

// CCmdTarget::InternalRelease() -- retail mfc140u 0x26cdb0:
//     if (m_dwRef == 0) return 0;                   ; cmpl $0,0x8(%rcx)
//     nRef = InterlockedDecrement(&m_dwRef);        ; lock xadd $-1
//     if (nRef == 0) {
//         AFX_MAINTAIN_STATE2 _state(m_pModuleState);   ; from this+0x38
//         this->vtable[+0x30]();                        ; virtual OnFinalRelease
//     }
//     return nRef;
// Implemented in CCmdTargetVirtuals.cpp. The module-state push at +0x38
// still requires the remaining CCmdTarget object layout.

// CCmdTarget::IsInvokeAllowed() -- retail mfc140u 0x3a60 is
//     mov $0x1,%eax ; ret
// The base class allows every DISPID to be invoked; only derived classes
// (COleControl, mfc140u 0x1e2a70) ever refuse.


// IsResultExpected is implemented against native storage in CCmdTargetVirtuals.cpp.

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


// CCmdTarget::OnCreateAggregates() -- retail mfc140u resolves this export (by
// ordinal) to 0x3a60, the image-wide `mov $0x1,%eax ; ret` shared by every
// ICF-folded "return TRUE" method: the base class creates no aggregates and
// reports success.


// Event invocation is implemented in CCmdTargetEvents.cpp.

// Native final release lives in CCmdTargetVirtuals.cpp.

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
// native method invoker uses its own temporary BOOL storage), so that
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
// InternalQueryInterface now calls this after the direct interface lookup.


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

// Standard property writes and module-aware callbacks live in CCmdTargetDispatch.cpp.
