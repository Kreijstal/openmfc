// CMFCBaseAccessibleObject — OpenMFC implementation.
// Sources: global_cmfcbaseaccessibleobject.cpp, manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// OpenMFC: CMFCBaseAccessibleObject exports, transcribed from the retail
// mfc140u.dll (14.51.36231, x64) by disassembling each export's body (the
// method described in the header of core/ole/COleControl.cpp).  Every RVA
// quoted below is an mfc140u RVA unless it says otherwise.
//
// The class is NOT declared in the OpenMFC public headers, so this file is the
// only place its layout lives.  Every export takes `this` as void*, and every
// object that reaches these thunks was allocated by CLIENT code with the retail
// member layout: nothing inside this DLL calls ??0CMFCBaseAccessibleObject
// (checked: grep of phase4/src finds no caller), and OpenMFC's own
// CMFCRibbonBaseElement derives from CObject, not from this class.  The
// retail layout, read from the constructor (RVA 0xe2c0), the destructor
// (0xe360), the scalar deleting destructor (0xe310, `mov $0xa0,%edx` sized
// delete) and the shipping declaration in atlmfc/include/afxbaseribbonelement.h
// (class CMFCBaseAccessibleObject : public CCmdTarget { DECLARE_DYNAMIC;
// CAccessibilityData m_AccData; DECLARE_INTERFACE_MAP;
// BEGIN_INTERFACE_PART(Accessible, IAccessible) ... }):
//
//   +0x00  vfptr                       CMFCBaseAccessibleObject vftable (.rdata 0x2dd068)
//   +0x08  long   m_dwRef              CCmdTarget (ctor 0x1de3f0 stores 1)
//   +0x10  LPUNKNOWN m_pOuterUnknown   CCmdTarget (ctor stores 0)
//   +0x18  DWORD_PTR m_xInnerUnknown   CCmdTarget (ctor stores 0)
//   +0x20  XDispatch m_xDispatch       CCmdTarget (ctor stores 0; EnableAutomation
//                                      0x24fcf0 later parks .rdata 0x32e450 there)
//   +0x28  BOOL   m_bResultExpected    CCmdTarget (ctor stores 1)
//   +0x30  XConnPtContainer m_xConnPtContainer   CCmdTarget (ctor stores 0)
//   +0x38  AFX_MODULE_STATE* m_pModuleState      CCmdTarget (ctor stores AfxGetModuleState())
//   +0x40  CAccessibilityData m_AccData          0x58 bytes: six CStrings +0x40..+0x68,
//                                      m_nAccRole +0x70, m_bAccState +0x74, m_nAccHit
//                                      +0x78, m_rectAccLocation +0x7c, m_ptAccHit +0x8c
//                                      (ctor helper 0xab00 / dtor helper 0xadb4)
//   +0x98  XAccessible m_xAccessible   vptr-only part, vftable .rdata 0x2dcf80;
//                                      every XAccessible::X opens with
//                                      `lea -0x98(%rcx),...`, and the class's
//                                      AFX_INTERFACEMAP (.rdata 0x2dd1d8, right
//                                      after the vtable) has the single entry
//                                      { IID_IAccessible, 0x98 }
//   sizeof == 0xa0 (160) -- the deleting destructor's sized delete, and the
//   160 that controls/RuntimeClasses.cpp records in classCMFCBaseAccessibleObject.
//
// Vtables.  Retail reaches GetParentWnd / OnSetAccData / SetACCData from the
// outer get_acc* bodies through vtable slots 0x158/0x160/0x168 (43/44/45), and
// every XAccessible::X forwards to the OUTER virtual through slots 0xb0..0x150
// (22..42); a client-derived class (CMFCRibbonBaseElement and friends in retail)
// overrides those, so they must be dispatched, never called directly.  Both
// vtables were dumped slot by slot from mfc140u's .rdata (0x2dd068: 46 slots,
// 0x2dcf80: 28 slots) and every slot was matched back to a body -- an export,
// or an unexported body disassembled at its RVA; the slot numbers used below
// come from those dumps.  Because a client-derived
// object carries the CLIENT's MSVC-layout vtable, and a bare instance built by
// the exported constructor must carry an equivalent one, this file authors both
// vtables in the MSVC layout (precedent: featurepack/ribbon/CMFCRibbonCmdUI.cpp,
// featurepack/docking/CSmartDockingInfo.cpp) and the constructor installs them
// exactly where retail installs its own.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * a NULL `this` (or a NULL owner vptr) is a deviation guard: retail
//     dereferences it and would fault; the guard returns E_UNEXPECTED (the
//     convention of core/ole/CDocObjectServer.cpp) unless the function says
//     otherwise;
//   * VARIANT by value is a 24-byte aggregate, passed by hidden reference under
//     the MS x64 ABI; GCC's ms_abi does the same, so it is spelled `VARIANT`.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <oleacc.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

#ifndef MS_ABI
  #ifdef __GNUC__
    #define MS_ABI __attribute__((ms_abi))
  #else
    #define MS_ABI
  #endif
#endif

//=============================================================================
// Thunks from other translation units (definitions checked, see each comment).
//=============================================================================
// core/runtime/AFX_MAINTAIN_STATE2.cpp -- the METHOD_PROLOGUE_EX state push
// (retail ctor 0x133170) and its pop.
extern "C" void* MS_ABI impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(void* pThis, void* pState);
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(void* pThis);
// core/runtime/CCmdTarget.cpp
extern "C" void MS_ABI impl__EnableAutomation_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" void MS_ABI impl__ExternalDisconnect_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" IUnknown* MS_ABI impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(CCmdTarget* pThis, const void* iid);
extern "C" unsigned long MS_ABI impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget* pThis, const void* iid, void** ppvObj);
extern "C" unsigned long MS_ABI impl__ExternalAddRef_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);
extern "C" unsigned long MS_ABI impl__ExternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);
// The CCmdTarget virtuals that fill vtable slots 5..21 (retail order from
// atlmfc/include/afxwin.h; slot numbers confirmed against the 0x2dd068 dump).
// Only their addresses are taken here.  OnCmdMsg's definition
// (core/runtime/CCmdTarget.cpp:448) carries an auto-generated
// `(void*, void*, void*, void*, void*)` parameter list; the declaration below
// is the one the mangled name describes.
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
extern "C" void MS_ABI impl__OnFinalRelease_CCmdTarget__UEAAXXZ();
extern "C" int MS_ABI impl__IsInvokeAllowed_CCmdTarget__UEAAHJ_Z(CCmdTarget* pThis, long dispid);
extern "C" int MS_ABI impl__GetDispatchIID_CCmdTarget__UEAAHPEAU_GUID___Z(CCmdTarget* pThis, void* pIID);
extern "C" unsigned int MS_ABI impl__GetTypeInfoCount_CCmdTarget__UEAAIXZ(CCmdTarget* pThis);
extern "C" void* MS_ABI impl__GetTypeLibCache_CCmdTarget__UEAAPEAVCTypeLibCache__XZ(CCmdTarget* pThis);
extern "C" long MS_ABI impl__GetTypeLib_CCmdTarget__UEAAJKPEAPEAUITypeLib___Z(CCmdTarget* pThis, unsigned long lcid, void** ppTypeLib);
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CCmdTarget__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis);
extern "C" const void* MS_ABI impl__GetCommandMap_CCmdTarget__MEBAPEBUAFX_OLECMDMAP__XZ(const CCmdTarget* pThis);
extern "C" const void* MS_ABI impl__GetDispatchMap_CCmdTarget__MEBAPEBUAFX_DISPMAP__XZ(const CCmdTarget* pThis);
extern "C" const void* MS_ABI impl__GetConnectionMap_CCmdTarget__MEBAPEBUAFX_CONNECTIONMAP__XZ(const CCmdTarget* pThis);
extern "C" const void* MS_ABI impl__GetEventSinkMap_CCmdTarget__MEBAPEBUAFX_EVENTSINKMAP__XZ(const CCmdTarget* pThis);
extern "C" int MS_ABI impl__OnCreateAggregates_CCmdTarget__UEAAHXZ();
extern "C" IUnknown* MS_ABI impl__GetInterfaceHook_CCmdTarget__UEAAPEAUIUnknown__PEBX_Z(CCmdTarget* pThis, const void* iid);
extern "C" int MS_ABI impl__GetExtraConnectionPoints_CCmdTarget__MEAAHPEAVCPtrArray___Z(CCmdTarget* pThis, void* pConnPoints);
extern "C" void* MS_ABI impl__GetConnectionHook_CCmdTarget__MEAAPEAUIConnectionPoint__AEBU_GUID___Z(CCmdTarget* pThis, const void* iid);
// featurepack/controls/RuntimeClasses.cpp and InterfaceMaps.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCBaseAccessibleObject__UEBAPEAUCRuntimeClass__XZ(const void* pThis);
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_CMFCBaseAccessibleObject__MEBAPEBUAFX_INTERFACEMAP__XZ(const void* pThis);
// core/collections/Thunks.cpp -- CStringW::AllocSysString (SysAllocStringLen of
// the buffer; returns NULL on failure, it does not throw).
extern "C" OLECHAR* MS_ABI impl__AllocSysString___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAPEA_WXZ(void* pThis);
// detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();

// Thunks defined further down in THIS file that earlier code refers to.
extern "C" void MS_ABI impl___1CMFCBaseAccessibleObject__UEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__OnSetAccData_CMFCBaseAccessibleObject__UEAAHJ_Z(void* pThis, long lVal);
extern "C" int MS_ABI impl__SetACCData_CMFCBaseAccessibleObject__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(void* pThis, CWnd* pParentWnd, CAccessibilityData& accData);

namespace {

//=============================================================================
// Retail layout (see the top note for the evidence behind each line).
//=============================================================================
constexpr size_t kOff_vfptr              = 0x00;
constexpr size_t kOff_m_dwRef            = 0x08;
constexpr size_t kOff_m_pOuterUnknown    = 0x10;
constexpr size_t kOff_m_xInnerUnknown    = 0x18;
constexpr size_t kOff_m_xDispatch        = 0x20;
constexpr size_t kOff_m_bResultExpected  = 0x28;
constexpr size_t kOff_m_xConnPtContainer = 0x30;
constexpr size_t kOff_m_pModuleState     = 0x38;
constexpr size_t kOff_m_AccData          = 0x40;
constexpr size_t kOff_m_xAccessible      = 0x98;
constexpr size_t kSizeof                 = 0xa0;

// OpenMFC's CAccessibilityData (include/openmfc/afxmfc.h) is the retail one:
// the ctor helper 0xab00 initialises six CStrings at +0x00..+0x28 and zeroes
// +0x30..+0x54, the outer get_acc* bodies read the strings at object +0x40..
// +0x68, the role at +0x70 and the state at +0x74.
static_assert(sizeof(CAccessibilityData) == 0x58, "retail m_AccData spans +0x40..+0x98");
static_assert(offsetof(CAccessibilityData, m_strAccName)      == 0x00, "+0x40");
static_assert(offsetof(CAccessibilityData, m_strAccValue)     == 0x08, "+0x48");
static_assert(offsetof(CAccessibilityData, m_strDescription)  == 0x10, "+0x50");
static_assert(offsetof(CAccessibilityData, m_strAccKeys)      == 0x18, "+0x58");
static_assert(offsetof(CAccessibilityData, m_strAccHelp)      == 0x20, "+0x60");
static_assert(offsetof(CAccessibilityData, m_strAccDefAction) == 0x28, "+0x68");
static_assert(offsetof(CAccessibilityData, m_nAccRole)        == 0x30, "+0x70");
static_assert(offsetof(CAccessibilityData, m_bAccState)       == 0x34, "+0x74");
static_assert(kOff_m_AccData + sizeof(CAccessibilityData) == kOff_m_xAccessible, "m_xAccessible follows m_AccData");
static_assert(kOff_m_xAccessible + sizeof(void*) == kSizeof, "sizeof(CMFCBaseAccessibleObject) == 0xa0");
static_assert(sizeof(CString) == sizeof(void*), "CString is one CStringT pointer");
// The outer bodies test the parent window's HWND at CWnd + 0x40 (`cmpq $0x0,0x40(%rax)`).
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40");

// CMFCBaseAccessibleObject vftable (.rdata 0x2dd068), 46 slots.  0..4 are
// CObject's, 5..21 CCmdTarget's (afxwin.h declaration order), 22..45 this
// class's own, in the afxbaseribbonelement.h declaration order.
enum : int {
    kSlot_GetRuntimeClass = 0, kSlot_DeletingDtor, kSlot_Serialize, kSlot_AssertValid, kSlot_Dump,
    kSlot_OnCmdMsg = 5, kSlot_OnFinalRelease, kSlot_IsInvokeAllowed, kSlot_GetDispatchIID,
    kSlot_GetTypeInfoCount, kSlot_GetTypeLibCache, kSlot_GetTypeLib, kSlot_GetMessageMap,
    kSlot_GetCommandMap, kSlot_GetDispatchMap, kSlot_GetConnectionMap, kSlot_GetInterfaceMap,
    kSlot_GetEventSinkMap, kSlot_OnCreateAggregates, kSlot_GetInterfaceHook,
    kSlot_GetExtraConnectionPoints, kSlot_GetConnectionHook,
    kSlot_get_accParent = 22, kSlot_get_accChildCount, kSlot_get_accChild, kSlot_get_accName,
    kSlot_get_accValue, kSlot_get_accDescription, kSlot_get_accRole, kSlot_get_accState,
    kSlot_get_accHelp, kSlot_get_accHelpTopic, kSlot_get_accKeyboardShortcut, kSlot_get_accFocus,
    kSlot_get_accSelection, kSlot_get_accDefaultAction, kSlot_accSelect, kSlot_accLocation,
    kSlot_accNavigate, kSlot_accHitTest, kSlot_accDoDefaultAction, kSlot_put_accName,
    kSlot_put_accValue,
    kSlot_GetParentWnd = 43, kSlot_OnSetAccData, kSlot_SetACCData,
    kClassVtblSlots = 46
};
static_assert(kSlot_GetConnectionHook == 21, "CCmdTarget's last virtual");
static_assert(kSlot_get_accParent * 8 == 0xb0,  "XAccessible::get_accParent reads vtbl+0xb0");
static_assert(kSlot_get_accName * 8 == 0xc8,    "XAccessible::get_accName reads vtbl+0xc8");
static_assert(kSlot_put_accValue * 8 == 0x150,  "XAccessible::put_accValue reads vtbl+0x150");
static_assert(kSlot_GetParentWnd * 8 == 0x158,  "get_accName reads vtbl+0x158");
static_assert(kSlot_OnSetAccData * 8 == 0x160,  "get_accName reads vtbl+0x160");
static_assert(kSlot_SetACCData * 8 == 0x168,    "get_accName reads vtbl+0x168");

// XAccessible vftable (.rdata 0x2dcf80), 28 slots: IUnknown, IDispatch,
// then IAccessible in oleacc.h order.
enum : int {
    kXSlot_QueryInterface = 0, kXSlot_AddRef, kXSlot_Release,
    kXSlot_GetTypeInfoCount = 3, kXSlot_GetTypeInfo, kXSlot_GetIDsOfNames, kXSlot_Invoke,
    kXSlot_get_accParent = 7, kXSlot_get_accChildCount, kXSlot_get_accChild, kXSlot_get_accName,
    kXSlot_get_accValue, kXSlot_get_accDescription, kXSlot_get_accRole, kXSlot_get_accState,
    kXSlot_get_accHelp, kXSlot_get_accHelpTopic, kXSlot_get_accKeyboardShortcut, kXSlot_get_accFocus,
    kXSlot_get_accSelection, kXSlot_get_accDefaultAction, kXSlot_accSelect, kXSlot_accLocation,
    kXSlot_accNavigate, kXSlot_accHitTest, kXSlot_accDoDefaultAction, kXSlot_put_accName,
    kXSlot_put_accValue,
    kXAccVtblSlots = 28
};
static_assert(kXSlot_put_accValue == 27, "IAccessible has 21 methods after IDispatch's 7");

// The two hand-authored MSVC-layout vtables; defined at the end of this file,
// after every body they point at.
extern void* const g_CMFCBaseAccessibleObject_vtbl[kClassVtblSlots];
extern void* const g_XAccessible_vtbl[kXAccVtblSlots];

// IID_IDispatch, read out of mfc140u .rdata 0x2d9b48 (the constant every
// XAccessible IDispatch method passes to GetInterface): {00020400-0000-0000-
// C000-000000000046}.  Spelled locally so the file adds no import-library symbol.
const GUID kIID_IDispatch = { 0x00020400, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

template <class T>
inline T& Member(void* pObj, size_t off) {
    return *reinterpret_cast<T*>(static_cast<char*>(pObj) + off);
}
inline CAccessibilityData& AccData(void* pThis) {
    return Member<CAccessibilityData>(pThis, kOff_m_AccData);
}
// `lea -0x98(%rcx),%rbx`: the owning object of an XAccessible part.
inline void* OwnerFromPart(void* pItf) {
    return static_cast<char*>(pItf) - kOff_m_xAccessible;
}
inline void** VtblOf(void* pObj) {
    return *static_cast<void***>(pObj);
}
// `mov (%rcx),%rax ; call *0xNN(%rax)`: slot NN/8 of pObj's own vtable.
template <class Fn>
inline Fn SlotFn(void* pObj, int slot) {
    return reinterpret_cast<Fn>(VtblOf(pObj)[slot]);
}

// METHOD_PROLOGUE_EX: AFX_MAINTAIN_STATE2 built from owner->m_pModuleState
// (`mov 0x38(%rbx),%rdx ; call 0x133170`) and popped on exit.  Reproduced
// through the exported AFX_MAINTAIN_STATE2 thunks; the local is only the key
// those thunks track the push under.
// A NULL owner (the deviation guard's case) pushes nothing.
struct MaintainState {
    void* m_slot[2];
    bool  m_pushed;
    explicit MaintainState(void* pOwner) : m_pushed(pOwner != nullptr) {
        if (m_pushed) {
            impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(
                m_slot, Member<void*>(pOwner, kOff_m_pModuleState));
        }
    }
    ~MaintainState() {
        if (m_pushed) impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(m_slot);
    }
    MaintainState(const MaintainState&) = delete;
    MaintainState& operator=(const MaintainState&) = delete;
};

// The three per-object virtuals the outer get_acc* bodies dispatch.
using GetParentWndFn = void* (MS_ABI*)(const void* pThis);                       // slot 43
using OnSetAccDataFn = int (MS_ABI*)(void* pThis, long lVal);                    // slot 44
using SetACCDataFn   = int (MS_ABI*)(void* pThis, void* pParentWnd, void* pData); // slot 45

inline void* CallGetParentWnd(void* pThis) {
    return SlotFn<GetParentWndFn>(pThis, kSlot_GetParentWnd)(pThis);
}
inline void CallOnSetAccData(void* pThis, long lVal) {
    SlotFn<OnSetAccDataFn>(pThis, kSlot_OnSetAccData)(pThis, lVal);
}
inline void CallSetACCData(void* pThis, void* pParentWnd) {
    SlotFn<SetACCDataFn>(pThis, kSlot_SetACCData)(pThis, pParentWnd, &AccData(pThis));
}
// The `GetParentWnd(); if (p == NULL || p->m_hWnd == NULL)` test every
// CHILDID_SELF path makes (`test %rax,%rax ; je` then `cmpq $0x0,0x40(%rax) ; je`).
// Returns the parent when it exists and has a window, else NULL.
inline void* ParentWithWindow(void* pThis) {
    void* pParent = CallGetParentWnd(pThis);
    if (pParent == nullptr) return nullptr;
    if (static_cast<CWnd*>(pParent)->m_hWnd == nullptr) return nullptr;
    return pParent;
}
// `*ppOut = str.AllocSysString()` as retail inlines it: SysAllocStringLen(psz,
// len) through OLEAUT32 #4, and AfxThrowMemoryException() (helper 0x3160 ->
// 0x333c, which dispatches E_OUTOFMEMORY to ?AfxThrowMemoryException@@YAXXZ)
// when that returns NULL.  Deviation: a NULL ppOut returns E_INVALIDARG where
// retail would fault on the store.
inline long StoreBstr(wchar_t** ppOut, CString& str) {
    if (ppOut == nullptr) return E_INVALIDARG;
    OLECHAR* bstr = impl__AllocSysString___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAPEA_WXZ(&str);
    if (bstr == nullptr) {
        impl__AfxThrowMemoryException__YAXXZ();
        return E_OUTOFMEMORY;   // not reached when the thunk throws
    }
    *ppOut = bstr;
    return S_OK;
}
inline bool StrIsEmpty(const CString& str) { return str.GetLength() == 0; }

//=============================================================================
// Vtable slots that retail fills with unexported or folded bodies.
//=============================================================================
// Slot 1: the scalar deleting destructor, mfc140u 0xe310 (unexported; found
// as slot 1 of the 0x2dd068 vftable):
//     ~CMFCBaseAccessibleObject()                  ; call 0xe360
//     if (flags & 1) {
//         if (flags & 4) (0x27d0)(this, 0xa0);     ; edx = sizeof: 0x27d0 is the
//                                                  ;   bare `ret $0` (c2 00 00)
//                                                  ;   in mfc140u, so this branch
//                                                  ;   frees nothing
//         else           free(this);               ; IAT 0x2c74e8 = api-ms-win-
//     }                                            ;   crt-heap free
//     return this;
// Every deleting destructor in mfc140u has the same two-branch shape and the
// same 0x27d0 callee (541 call sites); what the flags&4 branch was meant to be
// (a sized operator delete, by its size argument) is an inference, and no
// in-image caller passes flags with bit 4 set.  The flags&1 path calls the
// CRT's free directly, which is exactly retail's callee (the precedent
// detail/CSmartDockingInfoSupport.cpp spells it ::operator delete, which
// under mingw is the same free).
void* MS_ABI Slot_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CMFCBaseAccessibleObject__UEAA_XZ(pThis);
    if (flags & 1) {
        if ((flags & 4) == 0) {
            ::free(pThis);
        }
        // flags & 4: retail's callee at 0x27d0 returns without freeing.
    }
    return pThis;
}
// Slots 2, 3, 4 (CObject::Serialize / AssertValid / Dump): all three retail
// slots hold the folded one-instruction body at 0x27d0 (`ret`).
void MS_ABI Slot_NoOp(void* /*pThis*/, void* /*arg*/) {}
// Slot 43, GetParentWnd: inline `{ return NULL; }` in afxbaseribbonelement.h,
// so no export; the retail slot holds the folded `xor %eax,%eax ; ret` at 0x71e0.
void* MS_ABI Slot_GetParentWnd(const void* /*pThis*/) { return nullptr; }

// XAccessible slots 0..2 -- BEGIN_INTERFACE_PART's IUnknown trio, unexported.
// Each opens with METHOD_PROLOGUE_EX (owner = this - 0x98, state push from
// owner+0x38).  QueryInterface (0xe430) and Release (0xe3d0) then inline
// CCmdTarget::ExternalQueryInterface / ExternalRelease: test m_pOuterUnknown
// at owner+0x10 and call its IUnknown slot 0 / 2 when set, else
// InternalQueryInterface 0x26cfe0 / InternalRelease 0x26cdb0.  AddRef (0xe390)
// calls ExternalAddRef 0x26cd80 outright.  The exported External* thunks in
// core/runtime/CCmdTarget.cpp are those same bodies.
//   0xe430: QueryInterface   0xe390: AddRef   0xe3d0: Release
// Deviation: a NULL part returns E_UNEXPECTED / 0 (retail would fault).
long MS_ABI XAcc_QueryInterface(void* pItf, const IID* riid, void** ppvObj) {
    if (pItf == nullptr) return E_UNEXPECTED;
    void* pOwner = OwnerFromPart(pItf);
    MaintainState state(pOwner);
    return static_cast<long>(impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(
        static_cast<CCmdTarget*>(pOwner), riid, ppvObj));
}
unsigned long MS_ABI XAcc_AddRef(void* pItf) {
    if (pItf == nullptr) return 0;
    void* pOwner = OwnerFromPart(pItf);
    MaintainState state(pOwner);
    return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(static_cast<CCmdTarget*>(pOwner));
}
unsigned long MS_ABI XAcc_Release(void* pItf) {
    if (pItf == nullptr) return 0;
    void* pOwner = OwnerFromPart(pItf);
    MaintainState state(pOwner);
    return impl__ExternalRelease_CCmdTarget__QEAAKXZ(static_cast<CCmdTarget*>(pOwner));
}

// Shared prologue of the XAccessible forwarders: the owner behind the part and
// the METHOD_PROLOGUE_EX state push.  `ok` is the deviation guard (NULL part or
// an owner with no vptr; retail dereferences both).
struct AccForward {
    void* pOwner;
    bool  ok;
    MaintainState state;
    explicit AccForward(void* pItf)
        : pOwner(pItf != nullptr ? OwnerFromPart(pItf) : nullptr),
          ok(pOwner != nullptr && VtblOf(pOwner) != nullptr),
          state(pOwner) {}
};

}  // namespace

//=============================================================================
// Construction / destruction
//=============================================================================
// CMFCBaseAccessibleObject::CMFCBaseAccessibleObject -- mfc140u 0xe2c0:
//     CCmdTarget::CCmdTarget(this)                        ; call 0x1de3f0
//     *(void**)this = &vftable                             ; .rdata 0x2dd068
//     CAccessibilityData::CAccessibilityData(this + 0x40) ; call 0xab00
//     *(void**)(this + 0x98) = &XAccessible vftable       ; .rdata 0x2dcf80
//     *(void**)(this + 0x20) = &CCmdTarget::XDispatch vftable   ; .rdata 0x32e450,
//                                                         ; i.e. EnableAutomation()
//                                                         ; (0x24fcf0) inlined
//     return this
// CCmdTarget::CCmdTarget (0x1de3f0) is seven stores -- m_pModuleState =
// AfxGetModuleState() (call 0x133930), m_dwRef = 1, m_pOuterUnknown =
// m_xInnerUnknown = m_xDispatch = m_xConnPtContainer = 0, m_bResultExpected = 1
// -- and they are made HERE rather than through ??0CCmdTarget's thunk: that
// thunk placement-news OpenMFC's 32-byte CCmdTarget (core/runtime/
// CtorDtorPlacement.cpp:22), which can neither reach +0x20..+0x38 nor install a
// retail-shaped vptr, whereas this object is retail-sized (see the top note).
// The two vptr stores install this file's MSVC-layout vtables.  m_AccData is
// built with OpenMFC's CAccessibilityData constructor (six empty CStrings, the
// scalars zeroed -- what helper 0xab00 does with afxStringManager's nil string).
// EnableAutomation is called through its thunk; that thunk is a documented
// no-op today (core/runtime/CCmdTarget.cpp:517), so m_xDispatch stays the NULL
// the base constructor stored, and this object answers no IDispatch query
// until it is implemented.  Deviation: the pThis guard.
// Symbol: ??0CMFCBaseAccessibleObject@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCBaseAccessibleObject__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    // CCmdTarget::CCmdTarget, 0x1de3f0
    Member<void*>(pThis, kOff_m_pModuleState)    = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    Member<long>(pThis, kOff_m_dwRef)            = 1;
    Member<void*>(pThis, kOff_m_pOuterUnknown)   = nullptr;
    Member<void*>(pThis, kOff_m_xInnerUnknown)   = nullptr;
    Member<void*>(pThis, kOff_m_xDispatch)       = nullptr;
    Member<void*>(pThis, kOff_m_xConnPtContainer)= nullptr;
    Member<int>(pThis, kOff_m_bResultExpected)   = 1;
    // this class, 0xe2c0
    Member<void*>(pThis, kOff_vfptr) = const_cast<void**>(&g_CMFCBaseAccessibleObject_vtbl[0]);
    new (static_cast<char*>(pThis) + kOff_m_AccData) CAccessibilityData();
    Member<void*>(pThis, kOff_m_xAccessible) = const_cast<void**>(&g_XAccessible_vtbl[0]);
    impl__EnableAutomation_CCmdTarget__QEAAXXZ(static_cast<CCmdTarget*>(pThis));
    return pThis;
}

// CMFCBaseAccessibleObject::~CMFCBaseAccessibleObject -- mfc140u 0xe360:
//     *(void**)this = &vftable                              ; .rdata 0x2dd068
//     ExternalDisconnect()                                  ; call 0x26d100
//     CAccessibilityData::~CAccessibilityData(this + 0x40)  ; call 0xadb4: the six
//                                                           ;   CStrings released
//                                                           ;   in reverse order
//     tail-jump CCmdTarget::~CCmdTarget                     ; 0x1de430
// CCmdTarget::~CCmdTarget (0x1de430) is
//     if (m_xDispatch.vptr != NULL) m_xDispatch->vtbl[0x38/8 = 7]()   ; Disconnect
//     m_pModuleState = NULL
// and is reproduced inline rather than through ??1CCmdTarget's thunk: that
// thunk (detail/CWinAppSupport.cpp:70) runs `pThis->~CCmdTarget()`, a VIRTUAL
// call through the object's vptr, and this object's vptr is an MSVC-layout
// vtable whose slot 1 is the scalar deleting destructor -- it would recurse
// into the most-derived deleting destructor with a garbage flags argument.
// The vptr store re-installs this file's vtable, as retail re-installs its own.
// Deviation: the pThis guard.
// Symbol: ??1CMFCBaseAccessibleObject@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCBaseAccessibleObject__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    Member<void*>(pThis, kOff_vfptr) = const_cast<void**>(&g_CMFCBaseAccessibleObject_vtbl[0]);
    impl__ExternalDisconnect_CCmdTarget__QEAAXXZ(static_cast<CCmdTarget*>(pThis));
    AccData(pThis).~CAccessibilityData();
    // CCmdTarget::~CCmdTarget, 0x1de430
    void* pDispatchPart = static_cast<char*>(pThis) + kOff_m_xDispatch;
    if (Member<void*>(pThis, kOff_m_xDispatch) != nullptr) {
        using DisconnectFn = void (MS_ABI*)(void* pDispatch);
        SlotFn<DisconnectFn>(pDispatchPart, 7)(pDispatchPart);
    }
    Member<void*>(pThis, kOff_m_pModuleState) = nullptr;
}

//=============================================================================
// Outer IAccessible-shaped virtuals (retail vtable slots 22..45)
//=============================================================================
// Retail's CMFCBaseAccessibleObject::OnSetAccData and ::SetACCData have no RVA
// in the symbol map, but vtable slots 44 and 45 of 0x2dd068 both hold the
// folded `xor %eax,%eax ; ret` at 0x71e0, and mfc140u's export address table
// resolves both ordinals to that same 0x71e0: always FALSE.
// Symbol: ?OnSetAccData@CMFCBaseAccessibleObject@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCBaseAccessibleObject__UEAAHJ_Z(
    void* /*pThis*/, long /*lVal*/)
{
    return FALSE;
}
// Symbol: ?SetACCData@CMFCBaseAccessibleObject@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCBaseAccessibleObject__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* /*pThis*/, CWnd* /*pParentWnd*/, CAccessibilityData& /*accData*/)
{
    return FALSE;
}
// The seven S_FALSE stubs: mfc140u 0x3a60 is `mov $0x1,%eax ; ret`, and vtable
// slots 36..42 of 0x2dd068 all hold it.  No argument is read.
// Symbol: ?accDoDefaultAction@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCBaseAccessibleObject__UEAAJUtagVARIANT___Z(
    void* /*pThis*/, VARIANT /*varChild*/)
{
    return S_FALSE;
}
// Symbol: ?accHitTest@CMFCBaseAccessibleObject@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCBaseAccessibleObject__UEAAJJJPEAUtagVARIANT___Z(
    void* /*pThis*/, long /*xLeft*/, long /*yTop*/, VARIANT* /*pvarChild*/)
{
    return S_FALSE;
}
// Symbol: ?accLocation@CMFCBaseAccessibleObject@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCBaseAccessibleObject__UEAAJPEAJ000UtagVARIANT___Z(
    void* /*pThis*/, long* /*pxLeft*/, long* /*pyTop*/, long* /*pcxWidth*/,
    long* /*pcyHeight*/, VARIANT /*varChild*/)
{
    return S_FALSE;
}
// Symbol: ?accNavigate@CMFCBaseAccessibleObject@@UEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCBaseAccessibleObject__UEAAJJUtagVARIANT__PEAU2__Z(
    void* /*pThis*/, long /*navDir*/, VARIANT /*varStart*/, VARIANT* /*pvarEndUpAt*/)
{
    return S_FALSE;
}
// Symbol: ?accSelect@CMFCBaseAccessibleObject@@UEAAJJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accSelect_CMFCBaseAccessibleObject__UEAAJJUtagVARIANT___Z(
    void* /*pThis*/, long /*flagsSelect*/, VARIANT /*varChild*/)
{
    return S_FALSE;
}
// Symbol: ?put_accName@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI impl__put_accName_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEA_W_Z(
    void* /*pThis*/, VARIANT /*varChild*/, wchar_t* /*szName*/)
{
    return S_FALSE;
}
// Symbol: ?put_accValue@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI impl__put_accValue_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEA_W_Z(
    void* /*pThis*/, VARIANT /*varChild*/, wchar_t* /*szValue*/)
{
    return S_FALSE;
}
// mfc140u 0xf0e0: if (ppdispChild == NULL) return E_INVALIDARG;
// *ppdispChild = NULL; return varChild.vt == VT_I4 ? S_FALSE : E_INVALIDARG.
// Symbol: ?get_accChild@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    void* /*pThis*/, VARIANT varChild, IDispatch** ppdispChild)
{
    if (ppdispChild == nullptr) return E_INVALIDARG;
    *ppdispChild = nullptr;
    return (varChild.vt != VT_I4) ? E_INVALIDARG : S_FALSE;
}
// Vtable slot 23 of 0x2dd068 holds 0xcf50 (the symbol map names that RVA
// ?get_accChildCount@CBasePane@@, a folded twin): if (pcountChildren == NULL)
// return E_INVALIDARG; *pcountChildren = 0; return S_OK.
// Symbol: ?get_accChildCount@CMFCBaseAccessibleObject@@UEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCBaseAccessibleObject__UEAAJPEAJ_Z(
    void* /*pThis*/, long* pcountChildren)
{
    if (pcountChildren == nullptr) return E_INVALIDARG;
    *pcountChildren = 0;
    return S_OK;
}
// mfc140u 0xf5a0.  lVal is loaded before vt is tested (`mov 0x8(%rdx),%ebx`):
//     if (vt != VT_I4) { if (lVal != 0) return E_INVALIDARG; goto generic; }
//     if (lVal == 0) {                                   // CHILDID_SELF
//         p = GetParentWnd();                            // slot 43
//         if (p == NULL || p->m_hWnd == NULL) goto generic;
//         SetACCData(p, m_AccData);                      // slot 45
//         *pszDefaultAction = m_strAccDefAction.AllocSysString();   // +0x68
//         return S_OK;
//     }
// generic:                       // vt != VT_I4 && lVal == 0; vt == VT_I4 &&
//                                // lVal != 0 (either sign); self without a window
//     OnSetAccData(lVal);                                // slot 44
//     if (m_strAccDefAction is empty) return S_FALSE;    // `lea 0x1(%rdx),%eax`
//     *pszDefaultAction = m_strAccDefAction.AllocSysString(); return S_OK;
// Symbol: ?get_accDefaultAction@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDefaultAction_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszDefaultAction)
{
    const long lVal = varChild.lVal;
    if (varChild.vt != VT_I4 && lVal != 0) return E_INVALIDARG;
    if (pThis == nullptr || VtblOf(pThis) == nullptr) return E_UNEXPECTED;
    if (varChild.vt == VT_I4 && lVal == 0) {
        void* pParent = ParentWithWindow(pThis);
        if (pParent != nullptr) {
            CallSetACCData(pThis, pParent);
            return StoreBstr(pszDefaultAction, AccData(pThis).m_strAccDefAction);
        }
    }
    CallOnSetAccData(pThis, lVal);
    if (StrIsEmpty(AccData(pThis).m_strAccDefAction)) return S_FALSE;
    return StoreBstr(pszDefaultAction, AccData(pThis).m_strAccDefAction);
}
// mfc140u 0xf270:
//     if (vt != VT_I4) return S_FALSE;
//     if (lVal == 0) {                                   // CHILDID_SELF
//         p = GetParentWnd();                            // slot 43
//         if (p == NULL || p->m_hWnd == NULL) return S_FALSE;
//         SetACCData(p, m_AccData);                      // slot 45
//         *pszDescription = m_strDescription.AllocSysString();   // +0x50, no
//         return S_OK;                                   //   empty test here
//     }
//     if (lVal < 0) return S_FALSE;
//     OnSetAccData(lVal);                                // slot 44
//     if (m_strDescription is empty) return S_FALSE;
//     *pszDescription = m_strDescription.AllocSysString(); return S_OK;
// Symbol: ?get_accDescription@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDescription_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszDescription)
{
    if (varChild.vt != VT_I4) return S_FALSE;
    if (pThis == nullptr || VtblOf(pThis) == nullptr) return E_UNEXPECTED;
    const long lVal = varChild.lVal;
    if (lVal == 0) {
        void* pParent = ParentWithWindow(pThis);
        if (pParent == nullptr) return S_FALSE;
        CallSetACCData(pThis, pParent);
        return StoreBstr(pszDescription, AccData(pThis).m_strDescription);
    }
    if (lVal < 0) return S_FALSE;
    CallOnSetAccData(pThis, lVal);
    if (StrIsEmpty(AccData(pThis).m_strDescription)) return S_FALSE;
    return StoreBstr(pszDescription, AccData(pThis).m_strDescription);
}
// mfc140u 0xf590 (shared by get_accFocus and get_accSelection):
//     neg %rdx ; sbb %eax,%eax ; and $0xfff93faa,%eax ; add $0x80070057,%eax
// i.e. E_INVALIDARG when the pointer is NULL, E_NOTIMPL otherwise.
// Symbol: ?get_accFocus@CMFCBaseAccessibleObject@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accFocus_CMFCBaseAccessibleObject__UEAAJPEAUtagVARIANT___Z(
    void* /*pThis*/, VARIANT* pvarChild)
{
    if (pvarChild == nullptr) return E_INVALIDARG;
    return E_NOTIMPL;
}
// mfc140u 0xf460:
//     if (vt != VT_I4) return E_INVALIDARG;
//     if (lVal == 0) return S_FALSE;                     // `lea 0x1(%rdx),%eax`
//     if (pszHelp == NULL) return E_INVALIDARG;
//     if (lVal < 0) return E_INVALIDARG;
//     OnSetAccData(lVal);                                // slot 44
//     *pszHelp = m_strAccHelp.AllocSysString();          // +0x60, no empty test
//     return S_OK;
// Symbol: ?get_accHelp@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accHelp_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszHelp)
{
    if (varChild.vt != VT_I4) return E_INVALIDARG;
    const long lVal = varChild.lVal;
    if (lVal == 0) return S_FALSE;
    if (pszHelp == nullptr) return E_INVALIDARG;
    if (lVal < 0) return E_INVALIDARG;
    if (pThis == nullptr || VtblOf(pThis) == nullptr) return E_UNEXPECTED;
    CallOnSetAccData(pThis, lVal);
    return StoreBstr(pszHelp, AccData(pThis).m_strAccHelp);
}
// mfc140u 0xf4d0: `mov $0x80004001,%eax ; ret` -- always E_NOTIMPL.
// Symbol: ?get_accHelpTopic@CMFCBaseAccessibleObject@@UEAAJPEAPEA_WUtagVARIANT@@PEAJ@Z
extern "C" long MS_ABI impl__get_accHelpTopic_CMFCBaseAccessibleObject__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(
    void* /*pThis*/, wchar_t** /*pszHelpFile*/, VARIANT /*varChild*/, long* /*pidTopic*/)
{
    return E_NOTIMPL;
}
// mfc140u 0xf4e0:
//     if (vt != VT_I4) return lVal != 0 ? E_INVALIDARG : S_FALSE;   // neg/sbb/and/inc
//     if (lVal == 0) {                                   // CHILDID_SELF
//         p = GetParentWnd();                            // slot 43
//         if (p == NULL || p->m_hWnd == NULL) return S_FALSE;
//         SetACCData(p, m_AccData);                      // slot 45
//     } else {
//         if (lVal < 0) return S_FALSE;
//         OnSetAccData(lVal);                            // slot 44
//     }
//     *pszKeyboardShortcut = m_strAccKeys.AllocSysString();   // +0x58, no empty test
//     return S_OK;
// Symbol: ?get_accKeyboardShortcut@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accKeyboardShortcut_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszKeyboardShortcut)
{
    const long lVal = varChild.lVal;
    if (varChild.vt != VT_I4) return (lVal != 0) ? E_INVALIDARG : S_FALSE;
    if (pThis == nullptr || VtblOf(pThis) == nullptr) return E_UNEXPECTED;
    if (lVal == 0) {
        void* pParent = ParentWithWindow(pThis);
        if (pParent == nullptr) return S_FALSE;
        CallSetACCData(pThis, pParent);
    } else {
        if (lVal < 0) return S_FALSE;
        CallOnSetAccData(pThis, lVal);
    }
    return StoreBstr(pszKeyboardShortcut, AccData(pThis).m_strAccKeys);
}
// mfc140u 0xf110:
//     if (vt != VT_I4) return S_OK;
//     if (lVal == 0) {                                   // CHILDID_SELF
//         p = GetParentWnd();                            // slot 43
//         if (p == NULL || p->m_hWnd == NULL) return S_OK;
//         SetACCData(p, m_AccData);                      // slot 45
//         *pszName = m_strAccName.AllocSysString();      // +0x40, no empty test
//         return S_OK;
//     }
//     if (lVal < 0) return S_OK;
//     OnSetAccData(lVal);                                // slot 44
//     if (m_strAccName is empty) return S_FALSE;         // `lea 0x1(%rdx),%eax`
//     *pszName = m_strAccName.AllocSysString(); return S_OK;
// Symbol: ?get_accName@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszName)
{
    if (varChild.vt != VT_I4) return S_OK;
    if (pThis == nullptr || VtblOf(pThis) == nullptr) return E_UNEXPECTED;
    const long lVal = varChild.lVal;
    if (lVal == 0) {
        void* pParent = ParentWithWindow(pThis);
        if (pParent == nullptr) return S_OK;
        CallSetACCData(pThis, pParent);
        return StoreBstr(pszName, AccData(pThis).m_strAccName);
    }
    if (lVal < 0) return S_OK;
    CallOnSetAccData(pThis, lVal);
    if (StrIsEmpty(AccData(pThis).m_strAccName)) return S_FALSE;
    return StoreBstr(pszName, AccData(pThis).m_strAccName);
}
// mfc140u 0xf0c0: if (ppdispParent == NULL) return E_INVALIDARG;
// *ppdispParent = NULL; return S_OK.
// Symbol: ?get_accParent@CMFCBaseAccessibleObject@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCBaseAccessibleObject__UEAAJPEAPEAUIDispatch___Z(
    void* /*pThis*/, IDispatch** ppdispParent)
{
    if (ppdispParent == nullptr) return E_INVALIDARG;
    *ppdispParent = nullptr;
    return S_OK;
}
// mfc140u 0xf320:
//     if (vt == VT_I4 && lVal == 0) {                    // CHILDID_SELF
//         p = GetParentWnd();                            // slot 43
//         if (p != NULL && p->m_hWnd != NULL) {
//             SetACCData(p, m_AccData);                  // slot 45
//             pvarRole->vt = VT_I4; pvarRole->lVal = m_nAccRole;   // +0x70, pvarRole
//             return S_OK;                               //   not NULL-tested here
//         }
//     }
//     if (pvarRole == NULL) return E_INVALIDARG;
//     if (vt != VT_I4) return lVal == 0 ? S_OK : E_INVALIDARG;
//     if (lVal <= 0) return S_OK;                        // also: self without a window
//     pvarRole->vt = VT_I4;
//     OnSetAccData(lVal);                                // slot 44
//     pvarRole->lVal = m_nAccRole; return S_OK;
// Deviation: a NULL pvarRole on the CHILDID_SELF path returns E_INVALIDARG
// (retail would fault on the store).
// Symbol: ?get_accRole@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accRole_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAU2__Z(
    void* pThis, VARIANT varChild, VARIANT* pvarRole)
{
    if (pThis == nullptr || VtblOf(pThis) == nullptr) return E_UNEXPECTED;
    const long lVal = varChild.lVal;
    if (varChild.vt == VT_I4 && lVal == 0) {
        void* pParent = ParentWithWindow(pThis);
        if (pParent != nullptr) {
            CallSetACCData(pThis, pParent);
            if (pvarRole == nullptr) return E_INVALIDARG;
            pvarRole->vt = VT_I4;
            pvarRole->lVal = static_cast<long>(AccData(pThis).m_nAccRole);
            return S_OK;
        }
    }
    if (pvarRole == nullptr) return E_INVALIDARG;
    if (varChild.vt != VT_I4) return (lVal == 0) ? S_OK : E_INVALIDARG;
    if (lVal <= 0) return S_OK;
    pvarRole->vt = VT_I4;
    CallOnSetAccData(pThis, lVal);
    pvarRole->lVal = static_cast<long>(AccData(pThis).m_nAccRole);
    return S_OK;
}
// mfc140u 0xf590 -- the same folded body as get_accFocus above.
// Symbol: ?get_accSelection@CMFCBaseAccessibleObject@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accSelection_CMFCBaseAccessibleObject__UEAAJPEAUtagVARIANT___Z(
    void* /*pThis*/, VARIANT* pvarChildren)
{
    if (pvarChildren == nullptr) return E_INVALIDARG;
    return E_NOTIMPL;
}
// mfc140u 0xf3c0:
//     if (vt == VT_I4 && lVal == 0) {                    // CHILDID_SELF
//         p = GetParentWnd();                            // slot 43
//         if (p != NULL && p->m_hWnd != NULL) {
//             SetACCData(p, m_AccData);                  // slot 45
//             pvarState->vt = VT_I4; pvarState->lVal = m_bAccState;   // +0x74,
//             return S_OK;                               //   pvarState not NULL-tested
//         }
//     }
//     if (pvarState == NULL) return E_INVALIDARG;
//     if (vt != VT_I4) return E_INVALIDARG;
//     if (lVal <= 0) return E_INVALIDARG;                // also: self without a window
//     OnSetAccData(lVal);                                // slot 44
//     pvarState->vt = VT_I4; pvarState->lVal = m_bAccState; return S_OK;
// Deviation: a NULL pvarState on the CHILDID_SELF path returns E_INVALIDARG
// (retail would fault on the store).
// Symbol: ?get_accState@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAU2__Z(
    void* pThis, VARIANT varChild, VARIANT* pvarState)
{
    if (pThis == nullptr || VtblOf(pThis) == nullptr) return E_UNEXPECTED;
    const long lVal = varChild.lVal;
    if (varChild.vt == VT_I4 && lVal == 0) {
        void* pParent = ParentWithWindow(pThis);
        if (pParent != nullptr) {
            CallSetACCData(pThis, pParent);
            if (pvarState == nullptr) return E_INVALIDARG;
            pvarState->vt = VT_I4;
            pvarState->lVal = static_cast<long>(AccData(pThis).m_bAccState);
            return S_OK;
        }
    }
    if (pvarState == nullptr) return E_INVALIDARG;
    if (varChild.vt != VT_I4) return E_INVALIDARG;
    if (lVal <= 0) return E_INVALIDARG;
    CallOnSetAccData(pThis, lVal);
    pvarState->vt = VT_I4;
    pvarState->lVal = static_cast<long>(AccData(pThis).m_bAccState);
    return S_OK;
}
// mfc140u 0xf1c0 -- the get_accDescription shape with m_strAccValue (+0x48):
//     if (vt != VT_I4) return S_FALSE;
//     if (lVal == 0) { p = GetParentWnd(); if (p == NULL || p->m_hWnd == NULL)
//         return S_FALSE; SetACCData(p, m_AccData);
//         *pszValue = m_strAccValue.AllocSysString(); return S_OK; }   // no empty test
//     if (lVal < 0) return S_FALSE;
//     OnSetAccData(lVal);
//     if (m_strAccValue is empty) return S_FALSE;
//     *pszValue = m_strAccValue.AllocSysString(); return S_OK;
// Symbol: ?get_accValue@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accValue_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszValue)
{
    if (varChild.vt != VT_I4) return S_FALSE;
    if (pThis == nullptr || VtblOf(pThis) == nullptr) return E_UNEXPECTED;
    const long lVal = varChild.lVal;
    if (lVal == 0) {
        void* pParent = ParentWithWindow(pThis);
        if (pParent == nullptr) return S_FALSE;
        CallSetACCData(pThis, pParent);
        return StoreBstr(pszValue, AccData(pThis).m_strAccValue);
    }
    if (lVal < 0) return S_FALSE;
    CallOnSetAccData(pThis, lVal);
    if (StrIsEmpty(AccData(pThis).m_strAccValue)) return S_FALSE;
    return StoreBstr(pszValue, AccData(pThis).m_strAccValue);
}

//=============================================================================
// XAccessible -- the IAccessible interface part at +0x98
//=============================================================================
// The IDispatch quartet.  Each is, in mfc140u:
//     owner = this - 0x98                                  ; lea -0x98(%rcx)
//     AFX_MAINTAIN_STATE2 _state(owner->m_pModuleState)    ; +0x38, call 0x133170
//     p = owner->GetInterface(&IID_IDispatch)              ; call 0x26ce40, rdx =
//                                                          ;   .rdata 0x2d9b48
//     return p->vtbl[slot](args...)                        ; IDispatch slot 3/4/5/6,
//                                                          ;   via the CFG dispatch
//                                                          ;   pointer; p not tested
// In retail GetInterface resolves IID_IDispatch to the owner's own m_xDispatch
// part (+0x20), live because the constructor ran EnableAutomation.  In OpenMFC
// EnableAutomation is a no-op and every interface map is empty (see
// core/runtime/CCmdTarget.cpp GetInterface), so the lookup returns NULL today;
// deviation: that NULL yields E_UNEXPECTED instead of the fault retail would
// take.  The forward starts working the moment GetInterface can answer.
namespace {
using DispGetTypeInfoCountFn = long (MS_ABI*)(void*, unsigned int*);
using DispGetTypeInfoFn      = long (MS_ABI*)(void*, unsigned int, unsigned long, ITypeInfo**);
using DispGetIDsOfNamesFn    = long (MS_ABI*)(void*, const GUID*, wchar_t**, unsigned int, unsigned long, long*);
using DispInvokeFn           = long (MS_ABI*)(void*, long, const GUID*, unsigned long, unsigned short,
                                              DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);
inline void* OwnerDispatch(const AccForward& fwd) {
    if (!fwd.ok) return nullptr;
    return impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(
        static_cast<CCmdTarget*>(fwd.pOwner), &kIID_IDispatch);
}
}  // namespace

// mfc140u 0xe4b0, IDispatch slot 0x18/8 = 3.
// Symbol: ?GetTypeInfoCount@XAccessible@CMFCBaseAccessibleObject@@UEAAJPEAI@Z
extern "C" long MS_ABI impl__GetTypeInfoCount_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAI_Z(
    void* pThis, unsigned int* pctinfo)
{
    AccForward fwd(pThis);
    void* pDisp = OwnerDispatch(fwd);
    if (pDisp == nullptr) return E_UNEXPECTED;
    return SlotFn<DispGetTypeInfoCountFn>(pDisp, 3)(pDisp, pctinfo);
}
// mfc140u 0xe520, IDispatch slot 0x20/8 = 4.
// Symbol: ?GetTypeInfo@XAccessible@CMFCBaseAccessibleObject@@UEAAJIKPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfo_XAccessible_CMFCBaseAccessibleObject__UEAAJIKPEAPEAUITypeInfo___Z(
    void* pThis, unsigned int itinfo, unsigned long lcid, ITypeInfo** pptinfo)
{
    AccForward fwd(pThis);
    void* pDisp = OwnerDispatch(fwd);
    if (pDisp == nullptr) return E_UNEXPECTED;
    return SlotFn<DispGetTypeInfoFn>(pDisp, 4)(pDisp, itinfo, lcid, pptinfo);
}
// mfc140u 0xe590, IDispatch slot 0x28/8 = 5 (the 5th and 6th arguments are
// re-pushed from the caller's home slots at 0xa0/0xa8(%rsp)).
// Symbol: ?GetIDsOfNames@XAccessible@CMFCBaseAccessibleObject@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
extern "C" long MS_ABI impl__GetIDsOfNames_XAccessible_CMFCBaseAccessibleObject__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(
    void* pThis, const GUID* riid, wchar_t** rgszNames, unsigned int cNames,
    unsigned long lcid, long* rgdispid)
{
    AccForward fwd(pThis);
    void* pDisp = OwnerDispatch(fwd);
    if (pDisp == nullptr) return E_UNEXPECTED;
    return SlotFn<DispGetIDsOfNamesFn>(pDisp, 5)(pDisp, riid, rgszNames, cNames, lcid, rgdispid);
}
// mfc140u 0xe620, IDispatch slot 0x30/8 = 6 (arguments 5..9 re-pushed from
// 0xb0..0xd0(%rsp)).
// Symbol: ?Invoke@XAccessible@CMFCBaseAccessibleObject@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" long MS_ABI impl__Invoke_XAccessible_CMFCBaseAccessibleObject__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
    void* pThis, long dispidMember, const GUID* riid, unsigned long lcid, unsigned short wFlags,
    DISPPARAMS* pdispparams, VARIANT* pvarResult, EXCEPINFO* pexcepinfo, unsigned int* puArgErr)
{
    AccForward fwd(pThis);
    void* pDisp = OwnerDispatch(fwd);
    if (pDisp == nullptr) return E_UNEXPECTED;
    return SlotFn<DispInvokeFn>(pDisp, 6)(pDisp, dispidMember, riid, lcid, wFlags,
                                           pdispparams, pvarResult, pexcepinfo, puArgErr);
}

// The 21 IAccessible forwarders.  Each is, in mfc140u:
//     owner = this - 0x98                                  ; lea -0x98(%rcx)
//     AFX_MAINTAIN_STATE2 _state(owner->m_pModuleState)    ; +0x38, call 0x133170
//     return owner->vtbl[slot](args...)                    ; the OUTER virtual of
//                                                          ;   the same name, via the
//                                                          ;   CFG dispatch pointer
// A by-value VARIANT is copied to the stack (`movups (%rbx),%xmm0 ; movsd
// 0x10(%rbx),%xmm1` -> 0x30(%rsp)) and passed by reference to the copy, which
// is what the by-value VARIANT parameters below do under the MS ABI.  Every
// argument and the HRESULT pass through unchanged.  The slot is dispatched
// through the owner's vtable because a client-derived class overrides these
// virtuals; nothing here calls the outer impl__ thunk directly.  Deviation
// (shared): a NULL part or a vptr-less owner returns E_UNEXPECTED.
namespace {
using AccParentFn      = long (MS_ABI*)(void*, IDispatch**);
using AccCountFn       = long (MS_ABI*)(void*, long*);
using AccChildFn       = long (MS_ABI*)(void*, VARIANT, IDispatch**);
using AccVarBstrFn     = long (MS_ABI*)(void*, VARIANT, wchar_t**);
using AccVarVarFn      = long (MS_ABI*)(void*, VARIANT, VARIANT*);
using AccHelpTopicFn   = long (MS_ABI*)(void*, wchar_t**, VARIANT, long*);
using AccVarPtrFn      = long (MS_ABI*)(void*, VARIANT*);
using AccSelectFn      = long (MS_ABI*)(void*, long, VARIANT);
using AccLocationFn    = long (MS_ABI*)(void*, long*, long*, long*, long*, VARIANT);
using AccNavigateFn    = long (MS_ABI*)(void*, long, VARIANT, VARIANT*);
using AccHitTestFn     = long (MS_ABI*)(void*, long, long, VARIANT*);
using AccVarFn         = long (MS_ABI*)(void*, VARIANT);
using AccPutBstrFn     = long (MS_ABI*)(void*, VARIANT, wchar_t*);
}  // namespace

// mfc140u 0xe6d0, slot 0xb0/8 = 22.
// Symbol: ?get_accParent@XAccessible@CMFCBaseAccessibleObject@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAPEAUIDispatch___Z(
    void* pThis, IDispatch** ppdispParent)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccParentFn>(fwd.pOwner, kSlot_get_accParent)(fwd.pOwner, ppdispParent);
}
// mfc140u 0xe730, slot 0xb8/8 = 23.
// Symbol: ?get_accChildCount@XAccessible@CMFCBaseAccessibleObject@@UEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAJ_Z(
    void* pThis, long* pChildCount)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccCountFn>(fwd.pOwner, kSlot_get_accChildCount)(fwd.pOwner, pChildCount);
}
// mfc140u 0xe790, slot 0xc0/8 = 24.
// Symbol: ?get_accChild@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    void* pThis, VARIANT varChildIndex, IDispatch** ppdispChild)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccChildFn>(fwd.pOwner, kSlot_get_accChild)(fwd.pOwner, varChildIndex, ppdispChild);
}
// mfc140u 0xe810, slot 0xc8/8 = 25.
// Symbol: ?get_accName@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszName)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccVarBstrFn>(fwd.pOwner, kSlot_get_accName)(fwd.pOwner, varChild, pszName);
}
// mfc140u 0xe890, slot 0xd0/8 = 26.
// Symbol: ?get_accValue@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accValue_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszValue)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccVarBstrFn>(fwd.pOwner, kSlot_get_accValue)(fwd.pOwner, varChild, pszValue);
}
// mfc140u 0xe910, slot 0xd8/8 = 27.
// Symbol: ?get_accDescription@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDescription_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszDescription)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccVarBstrFn>(fwd.pOwner, kSlot_get_accDescription)(fwd.pOwner, varChild, pszDescription);
}
// mfc140u 0xe990, slot 0xe0/8 = 28.
// Symbol: ?get_accRole@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAU3@@Z
extern "C" long MS_ABI impl__get_accRole_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAU3__Z(
    void* pThis, VARIANT varChild, VARIANT* pvarRole)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccVarVarFn>(fwd.pOwner, kSlot_get_accRole)(fwd.pOwner, varChild, pvarRole);
}
// mfc140u 0xea10, slot 0xe8/8 = 29.
// Symbol: ?get_accState@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAU3@@Z
extern "C" long MS_ABI impl__get_accState_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAU3__Z(
    void* pThis, VARIANT varChild, VARIANT* pvarState)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccVarVarFn>(fwd.pOwner, kSlot_get_accState)(fwd.pOwner, varChild, pvarState);
}
// mfc140u 0xea90, slot 0xf0/8 = 30.
// Symbol: ?get_accHelp@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accHelp_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszHelp)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccVarBstrFn>(fwd.pOwner, kSlot_get_accHelp)(fwd.pOwner, varChild, pszHelp);
}
// mfc140u 0xeb10, slot 0xf8/8 = 31 (the VARIANT is the 3rd argument; its copy
// is passed in r8, pszHelpFile in rdx, pidTopic in r9).
// Symbol: ?get_accHelpTopic@XAccessible@CMFCBaseAccessibleObject@@UEAAJPEAPEA_WUtagVARIANT@@PEAJ@Z
extern "C" long MS_ABI impl__get_accHelpTopic_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(
    void* pThis, wchar_t** pszHelpFile, VARIANT varChild, long* pidTopic)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccHelpTopicFn>(fwd.pOwner, kSlot_get_accHelpTopic)(fwd.pOwner, pszHelpFile, varChild, pidTopic);
}
// mfc140u 0xeb90, slot 0x100/8 = 32.
// Symbol: ?get_accKeyboardShortcut@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accKeyboardShortcut_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszKeyboardShortcut)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccVarBstrFn>(fwd.pOwner, kSlot_get_accKeyboardShortcut)(fwd.pOwner, varChild, pszKeyboardShortcut);
}
// mfc140u 0xec10, slot 0x108/8 = 33.
// Symbol: ?get_accFocus@XAccessible@CMFCBaseAccessibleObject@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accFocus_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAUtagVARIANT___Z(
    void* pThis, VARIANT* pvarFocusChild)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccVarPtrFn>(fwd.pOwner, kSlot_get_accFocus)(fwd.pOwner, pvarFocusChild);
}
// mfc140u 0xec70, slot 0x110/8 = 34.
// Symbol: ?get_accSelection@XAccessible@CMFCBaseAccessibleObject@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accSelection_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAUtagVARIANT___Z(
    void* pThis, VARIANT* pvarSelectedChildren)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccVarPtrFn>(fwd.pOwner, kSlot_get_accSelection)(fwd.pOwner, pvarSelectedChildren);
}
// mfc140u 0xecd0, slot 0x118/8 = 35.
// Symbol: ?get_accDefaultAction@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDefaultAction_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszDefaultAction)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccVarBstrFn>(fwd.pOwner, kSlot_get_accDefaultAction)(fwd.pOwner, varChild, pszDefaultAction);
}
// mfc140u 0xed50, slot 0x120/8 = 36.
// Symbol: ?accSelect@XAccessible@CMFCBaseAccessibleObject@@UEAAJJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accSelect_XAccessible_CMFCBaseAccessibleObject__UEAAJJUtagVARIANT___Z(
    void* pThis, long flagsSelect, VARIANT varChild)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccSelectFn>(fwd.pOwner, kSlot_accSelect)(fwd.pOwner, flagsSelect, varChild);
}
// mfc140u 0xedd0, slot 0x128/8 = 37 (pcyHeight and the VARIANT copy are
// re-pushed from the caller's 0xc0/0xc8(%rsp) home slots).
// Symbol: ?accLocation@XAccessible@CMFCBaseAccessibleObject@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAJ000UtagVARIANT___Z(
    void* pThis, long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccLocationFn>(fwd.pOwner, kSlot_accLocation)(fwd.pOwner, pxLeft, pyTop, pcxWidth, pcyHeight, varChild);
}
// mfc140u 0xee70, slot 0x130/8 = 38.
// Symbol: ?accNavigate@XAccessible@CMFCBaseAccessibleObject@@UEAAJJUtagVARIANT@@PEAU3@@Z
extern "C" long MS_ABI impl__accNavigate_XAccessible_CMFCBaseAccessibleObject__UEAAJJUtagVARIANT__PEAU3__Z(
    void* pThis, long navDir, VARIANT varStart, VARIANT* pvarEndUpAt)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccNavigateFn>(fwd.pOwner, kSlot_accNavigate)(fwd.pOwner, navDir, varStart, pvarEndUpAt);
}
// mfc140u 0xeef0, slot 0x138/8 = 39.
// Symbol: ?accHitTest@XAccessible@CMFCBaseAccessibleObject@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_XAccessible_CMFCBaseAccessibleObject__UEAAJJJPEAUtagVARIANT___Z(
    void* pThis, long xLeft, long yTop, VARIANT* pvarChildAtPoint)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccHitTestFn>(fwd.pOwner, kSlot_accHitTest)(fwd.pOwner, xLeft, yTop, pvarChildAtPoint);
}
// mfc140u 0xef50, slot 0x140/8 = 40.
// Symbol: ?accDoDefaultAction@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT___Z(
    void* pThis, VARIANT varChild)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccVarFn>(fwd.pOwner, kSlot_accDoDefaultAction)(fwd.pOwner, varChild);
}
// mfc140u 0xefc0, slot 0x148/8 = 41.  This export has no entry in the RVA map;
// the body was found as slot 26 of the XAccessible vftable (0x2dcf80), the
// put_accName position of IAccessible, and read there; mfc140u's export
// address table independently resolves the ordinal to 0xefc0.
// Symbol: ?put_accName@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI impl__put_accName_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t* szName)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccPutBstrFn>(fwd.pOwner, kSlot_put_accName)(fwd.pOwner, varChild, szName);
}
// mfc140u 0xf040, slot 0x150/8 = 42.  Found the same way, as slot 27 of 0x2dcf80
// (export address table: 0xf040).
// Symbol: ?put_accValue@XAccessible@CMFCBaseAccessibleObject@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI impl__put_accValue_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t* szValue)
{
    AccForward fwd(pThis);
    if (!fwd.ok) return E_UNEXPECTED;
    return SlotFn<AccPutBstrFn>(fwd.pOwner, kSlot_put_accValue)(fwd.pOwner, varChild, szValue);
}

//=============================================================================
// The MSVC-layout vtables the constructor installs (see the top note).
//=============================================================================
namespace {
#define VT(fn) reinterpret_cast<void*>(&fn)
// .rdata 0x2dd068, 46 slots.  Retail's entries: 2/3/4 = 0x27d0 (`ret`, the
// CObject no-ops, unexported); 1 = 0xe310 (this class's deleting destructor,
// unexported); 5..21 = CCmdTarget's exports, of which 7 and 18 are the folded
// 0x3a60 (TRUE) and 8/9/10/19/20/21 the folded 0x71e0 (0); 0, 16, 22..42,
// 44, 45 = this class's exports, of which 44/45 are 0x71e0 and 36..42 are
// 0x3a60 (S_FALSE); 43 = GetParentWnd, inline in the header and unexported,
// also folded onto 0x71e0.  Two CCmdTarget thunks OpenMFC supplies for those slots do not
// yet match retail's folded bodies: OnCreateAggregates (slot 18) returns 0
// where 0x3a60 returns TRUE, and GetTypeLibCache (slot 10) returns a live
// cache handle where 0x71e0 returns NULL (both in core/runtime/CCmdTarget.cpp,
// not this file's to change).
void* const g_CMFCBaseAccessibleObject_vtbl[kClassVtblSlots] = {
    VT(impl__GetRuntimeClass_CMFCBaseAccessibleObject__UEBAPEAUCRuntimeClass__XZ),     // 0
    VT(Slot_ScalarDeletingDtor),                                                       // 1
    VT(Slot_NoOp),                                                                     // 2  Serialize
    VT(Slot_NoOp),                                                                     // 3  AssertValid
    VT(Slot_NoOp),                                                                     // 4  Dump
    VT(impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z),             // 5
    VT(impl__OnFinalRelease_CCmdTarget__UEAAXXZ),                                      // 6
    VT(impl__IsInvokeAllowed_CCmdTarget__UEAAHJ_Z),                                    // 7
    VT(impl__GetDispatchIID_CCmdTarget__UEAAHPEAU_GUID___Z),                           // 8
    VT(impl__GetTypeInfoCount_CCmdTarget__UEAAIXZ),                                    // 9
    VT(impl__GetTypeLibCache_CCmdTarget__UEAAPEAVCTypeLibCache__XZ),                   // 10
    VT(impl__GetTypeLib_CCmdTarget__UEAAJKPEAPEAUITypeLib___Z),                        // 11
    VT(impl__GetMessageMap_CCmdTarget__MEBAPEBUAFX_MSGMAP__XZ),                        // 12
    VT(impl__GetCommandMap_CCmdTarget__MEBAPEBUAFX_OLECMDMAP__XZ),                     // 13
    VT(impl__GetDispatchMap_CCmdTarget__MEBAPEBUAFX_DISPMAP__XZ),                      // 14
    VT(impl__GetConnectionMap_CCmdTarget__MEBAPEBUAFX_CONNECTIONMAP__XZ),              // 15
    VT(impl__GetInterfaceMap_CMFCBaseAccessibleObject__MEBAPEBUAFX_INTERFACEMAP__XZ), // 16
    VT(impl__GetEventSinkMap_CCmdTarget__MEBAPEBUAFX_EVENTSINKMAP__XZ),                // 17
    VT(impl__OnCreateAggregates_CCmdTarget__UEAAHXZ),                                  // 18
    VT(impl__GetInterfaceHook_CCmdTarget__UEAAPEAUIUnknown__PEBX_Z),                   // 19
    VT(impl__GetExtraConnectionPoints_CCmdTarget__MEAAHPEAVCPtrArray___Z),             // 20
    VT(impl__GetConnectionHook_CCmdTarget__MEAAPEAUIConnectionPoint__AEBU_GUID___Z),  // 21
    VT(impl__get_accParent_CMFCBaseAccessibleObject__UEAAJPEAPEAUIDispatch___Z),      // 22
    VT(impl__get_accChildCount_CMFCBaseAccessibleObject__UEAAJPEAJ_Z),                 // 23
    VT(impl__get_accChild_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z), // 24
    VT(impl__get_accName_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z),      // 25
    VT(impl__get_accValue_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z),     // 26
    VT(impl__get_accDescription_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z), // 27
    VT(impl__get_accRole_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAU2__Z),        // 28
    VT(impl__get_accState_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAU2__Z),       // 29
    VT(impl__get_accHelp_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z),      // 30
    VT(impl__get_accHelpTopic_CMFCBaseAccessibleObject__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z), // 31
    VT(impl__get_accKeyboardShortcut_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z), // 32
    VT(impl__get_accFocus_CMFCBaseAccessibleObject__UEAAJPEAUtagVARIANT___Z),          // 33
    VT(impl__get_accSelection_CMFCBaseAccessibleObject__UEAAJPEAUtagVARIANT___Z),      // 34
    VT(impl__get_accDefaultAction_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z), // 35
    VT(impl__accSelect_CMFCBaseAccessibleObject__UEAAJJUtagVARIANT___Z),               // 36
    VT(impl__accLocation_CMFCBaseAccessibleObject__UEAAJPEAJ000UtagVARIANT___Z),       // 37
    VT(impl__accNavigate_CMFCBaseAccessibleObject__UEAAJJUtagVARIANT__PEAU2__Z),       // 38
    VT(impl__accHitTest_CMFCBaseAccessibleObject__UEAAJJJPEAUtagVARIANT___Z),          // 39
    VT(impl__accDoDefaultAction_CMFCBaseAccessibleObject__UEAAJUtagVARIANT___Z),       // 40
    VT(impl__put_accName_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEA_W_Z),         // 41
    VT(impl__put_accValue_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEA_W_Z),        // 42
    VT(Slot_GetParentWnd),                                                             // 43
    VT(impl__OnSetAccData_CMFCBaseAccessibleObject__UEAAHJ_Z),                         // 44
    VT(impl__SetACCData_CMFCBaseAccessibleObject__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z), // 45
};
// .rdata 0x2dcf80, 28 slots: IUnknown (0xe430/0xe390/0xe3d0), IDispatch, IAccessible.
void* const g_XAccessible_vtbl[kXAccVtblSlots] = {
    VT(XAcc_QueryInterface),                                                           // 0
    VT(XAcc_AddRef),                                                                   // 1
    VT(XAcc_Release),                                                                  // 2
    VT(impl__GetTypeInfoCount_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAI_Z),      // 3
    VT(impl__GetTypeInfo_XAccessible_CMFCBaseAccessibleObject__UEAAJIKPEAPEAUITypeInfo___Z), // 4
    VT(impl__GetIDsOfNames_XAccessible_CMFCBaseAccessibleObject__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z), // 5
    VT(impl__Invoke_XAccessible_CMFCBaseAccessibleObject__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z), // 6
    VT(impl__get_accParent_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAPEAUIDispatch___Z), // 7
    VT(impl__get_accChildCount_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAJ_Z),     // 8
    VT(impl__get_accChild_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z), // 9
    VT(impl__get_accName_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z), // 10
    VT(impl__get_accValue_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z), // 11
    VT(impl__get_accDescription_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z), // 12
    VT(impl__get_accRole_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAU3__Z), // 13
    VT(impl__get_accState_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAU3__Z), // 14
    VT(impl__get_accHelp_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z), // 15
    VT(impl__get_accHelpTopic_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z), // 16
    VT(impl__get_accKeyboardShortcut_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z), // 17
    VT(impl__get_accFocus_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAUtagVARIANT___Z), // 18
    VT(impl__get_accSelection_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAUtagVARIANT___Z), // 19
    VT(impl__get_accDefaultAction_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEAPEA_W_Z), // 20
    VT(impl__accSelect_XAccessible_CMFCBaseAccessibleObject__UEAAJJUtagVARIANT___Z),   // 21
    VT(impl__accLocation_XAccessible_CMFCBaseAccessibleObject__UEAAJPEAJ000UtagVARIANT___Z), // 22
    VT(impl__accNavigate_XAccessible_CMFCBaseAccessibleObject__UEAAJJUtagVARIANT__PEAU3__Z), // 23
    VT(impl__accHitTest_XAccessible_CMFCBaseAccessibleObject__UEAAJJJPEAUtagVARIANT___Z), // 24
    VT(impl__accDoDefaultAction_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT___Z), // 25
    VT(impl__put_accName_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEA_W_Z), // 26
    VT(impl__put_accValue_XAccessible_CMFCBaseAccessibleObject__UEAAJUtagVARIANT__PEA_W_Z), // 27
};
#undef VT
}  // namespace
