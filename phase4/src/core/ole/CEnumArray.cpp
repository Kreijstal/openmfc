// CEnumArray — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// =============================================================================
// CEnumArray is MFC's generic COM enumerator over a flat array (private
// oleimpl2.h; not declared anywhere in include/openmfc).  It derives from
// CCmdTarget and exposes one nested interface part, XEnumVOID, whose seven
// IEnumXXXX-shaped methods forward to four protected virtuals (OnNext, OnSkip,
// OnReset, OnClone) so derived enumerators only override those.
//
// Everything below is transcribed from the retail mfc140u.dll disassembly; the
// addresses are RVAs in mfc140u.dll (function bodies are byte-identical in the
// ANSI twin mfc140.dll, the RVAs are not).
//
// Retail layout (sizeof 0x70: OnClone allocates `new(0x70)`, and the scalar
// deleting destructor's sized-delete path passes 0x70):
//   +0x00..+0x3f  the retail CCmdTarget part (vfptr, m_dwRef +0x08,
//                 m_pOuterUnknown +0x10, m_xInnerUnknown +0x18, m_xDispatch
//                 +0x20, m_bResultExpected +0x28, m_xConnPtContainer +0x30,
//                 m_pModuleState +0x38), as CCmdTarget::CCmdTarget (RVA
//                 0x1de3f0) and ~CCmdTarget (RVA 0x1de430) access it
//   +0x40  m_nSizeElem    qword; ctor (RVA 0x25b660): `mov %rbx,0x40(%r14)` (arg 2)
//   +0x48  m_pClonedFrom  qword pointer; ctor zeroes it; Clone (0x25ba50) sets it
//   +0x50  m_pvEnum       qword pointer; ctor: `mov %rsi,0x50(%r14)` (arg 3)
//   +0x58  m_nCurPos      dword; ctor zeroes it; OnNext/OnSkip compare it unsigned
//   +0x5c  m_nSize        dword; ctor: `mov %edi,0x5c(%r14)` (arg 4, r9d)
//   +0x60  m_bNeedFree    dword; ctor: stack arg 5 -> `mov %eax,0x60(%r14)`
//   +0x68  XEnumVOID m_xEnumVOID  ctor stores the XEnumVOID vftable here; every
//                                 XEnumVOID method rebases rcx by -0x68 to reach
//                                 the owner (Next/Skip/Reset/Clone with
//                                 `lea -0x68(%rcx),...`, QueryInterface/AddRef/
//                                 Release with `add $-0x68,%rcx`)
//
// Retail vftable (mfc140u .rdata RVA 0x32f8c8, 26 slots): slots 0..21 are
// CCmdTarget's (0 GetRuntimeClass 0xe290, 1 scalar deleting dtor 0x25b6d0,
// 2..4 the folded `ret` at 0x27d0, 5 OnCmdMsg 0x1de460, 6 OnFinalRelease
// 0x1de880, 7 and 18 the folded `mov $1,%eax; ret` at 0x3a60, 8..10 and 19..21
// the folded `xor %eax,%eax; ret` at 0x71e0, 11 GetTypeLib 0x1de7a0, 12..17 the
// CCmdTarget map getters 0x1de840/0x1de8f0/0x1de850/0x1de8e0/0x1de870/
// 0x1de860), then 22 OnNext 0x25b770, 23 OnSkip 0x25b7e0, 24 OnReset 0x25b800,
// 25 OnClone 0x25b810.  The XEnumVOID vftable (RVA 0x32f9a0) is QueryInterface
// 0x25b8a0, AddRef 0x25b860, Release 0x25b870, Next 0x25b8d0, Skip 0x25b980,
// Reset 0x25ba00, Clone 0x25ba50.  OnNext/OnSkip/OnReset/Skip/Reset have no
// entry in the RVA symbol map; they were identified through the export
// address table (the ordinals resolve to exactly those vftable entries).
//
// OpenMFC deviations (deliberate):
//   * The vtables.  This DLL has no MSVC-emitted vftable for the class, so the
//     constructor installs the two hand-authored tables at the end of this
//     file, in retail slot order, pointing at this file's MS_ABI functions and
//     at CCmdTarget's exported thunks -- the technique of
//     featurepack/controls/CMFCBaseAccessibleObject.cpp.  The folded
//     constant-return slots are reproduced by file-local functions returning
//     the same constants.  No RTTI Complete Object Locator precedes them.
//   * CCmdTarget's constructor and destructor bodies are reproduced inline
//     (cited where they occur) rather than through ??0/??1CCmdTarget's thunks:
//     those operate on OpenMFC's 32-byte CCmdTarget (placement new, and a
//     virtual `~CCmdTarget()` through the vptr), neither of which is valid on
//     this retail-layout object.
//   * Exceptions.  Next, Skip and Clone run their loop/body inside MFC TRY /
//     END_TRY: each has a C++ EH handler in its .pdata unwind info (Reset has
//     none), and each has a continuation that no branch inside the function
//     reaches and that reloads the saved result -- 0x25b94f, 0x25b9c8 and
//     0x25bab4, where the catch funclet resumes.
//     This DLL raises MFC exceptions through the MSVC runtime's
//     _CxxThrowException, which unwinds through gcc frames without a gcc catch
//     clause seeing it (see core/ole/CArchiveStream.cpp's header), so the
//     catch arms are not reproduced: an exception from OnNext/OnSkip/OnClone
//     propagates to the caller instead of being turned into a return code.
//   * NULL-allocation guards, noted at OnClone and Clone.
//
// Known gap outside this file: OpenMFC's CCmdTarget::InternalRelease
// (core/runtime/CCmdTarget.cpp) only decrements m_dwRef and never calls
// OnFinalRelease, so a released enumerator is not freed.
// =============================================================================

#include "detail/ManualSmallStubImplementationsSupport.h"

// Sibling thunks, declared with the signatures their mangled names describe.
// core/runtime/AFX_MAINTAIN_STATE2.cpp -- the METHOD_PROLOGUE_EX state push.
extern "C" void* MS_ABI impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(void* pThis, void* pState);
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(void* pThis);
// core/runtime/CCmdTarget.cpp
extern "C" unsigned long MS_ABI impl__InternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);
extern "C" unsigned long MS_ABI impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget* pThis, const void* iid, void** ppvObj);
extern "C" unsigned long MS_ABI impl__ExternalAddRef_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);
extern "C" unsigned long MS_ABI impl__ExternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CCmdTarget* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
extern "C" void MS_ABI impl__OnFinalRelease_CCmdTarget__UEAAXXZ(CCmdTarget* pThis);
extern "C" long MS_ABI impl__GetTypeLib_CCmdTarget__UEAAJKPEAPEAUITypeLib___Z(CCmdTarget* pThis, unsigned long lcid, void** ppTypeLib);
extern "C" const void* MS_ABI impl__GetCommandMap_CCmdTarget__MEBAPEBUAFX_OLECMDMAP__XZ(const CCmdTarget* pThis);
extern "C" const void* MS_ABI impl__GetDispatchMap_CCmdTarget__MEBAPEBUAFX_DISPMAP__XZ(const CCmdTarget* pThis);
extern "C" const void* MS_ABI impl__GetConnectionMap_CCmdTarget__MEBAPEBUAFX_CONNECTIONMAP__XZ(const CCmdTarget* pThis);
extern "C" const void* MS_ABI impl__GetEventSinkMap_CCmdTarget__MEBAPEBUAFX_EVENTSINKMAP__XZ(const CCmdTarget* pThis);
// core/runtime/MessageMaps.cpp, core/runtime/InterfaceMaps.cpp, core/runtime/RuntimeClasses.cpp
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CCmdTarget__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis);
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_CCmdTarget__MEBAPEBUAFX_INTERFACEMAP__XZ(const void* pThis);
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCmdTarget__UEBAPEAUCRuntimeClass__XZ(const CCmdTarget* pThis);
// detail/MemcoreSupport.cpp -- MFC's ::operator new / ::operator delete exports.
// Retail ??2 (0x27f0) loops malloc and, on failure, calls the new handler at
// AFX_MODULE_THREAD_STATE+0x50, returning NULL only if that handler returns 0;
// the default one (0x8a8b0, stored by the AFX_MODULE_THREAD_STATE ctor at
// 0x133580) calls AfxThrowMemoryException instead.  Retail ??3 (0x27c0) is a
// jmp to free.  OpenMFC's ??2 is plain malloc and returns NULL on failure.
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);
// detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

// This file's own thunks, referenced before their definitions.
extern "C" void* MS_ABI impl___0CEnumArray__QEAA__KPEBXIH_Z(
    void* pThis, unsigned long long nSizeElem, const void* pvEnum, unsigned int nSize, int bNeedFree);
extern "C" void MS_ABI impl___1CEnumArray__UEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__OnNext_CEnumArray__MEAAHPEAX_Z(void* pThis, void* pv);
extern "C" int MS_ABI impl__OnSkip_CEnumArray__MEAAHXZ(void* pThis);
extern "C" void MS_ABI impl__OnReset_CEnumArray__MEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__OnClone_CEnumArray__MEAAPEAV1_XZ(void* pThis);
extern "C" long MS_ABI impl__Next_XEnumVOID_CEnumArray__UEAAJKPEAXPEAK_Z(
    void* pThis, unsigned long celt, void* reelt, unsigned long* pceltFetched);
extern "C" long MS_ABI impl__Skip_XEnumVOID_CEnumArray__UEAAJK_Z(void* pThis, unsigned long celt);
extern "C" long MS_ABI impl__Reset_XEnumVOID_CEnumArray__UEAAJXZ(void* pThis);
extern "C" long MS_ABI impl__Clone_XEnumVOID_CEnumArray__UEAAJPEAPEAUIEnumVOID___Z(void* pThis, void** ppenum);

namespace {

//=============================================================================
// Retail layout (see the top note for the instruction behind each line).
//=============================================================================
constexpr size_t kOff_vfptr              = 0x00;
constexpr size_t kOff_m_dwRef            = 0x08;
constexpr size_t kOff_m_pOuterUnknown    = 0x10;
constexpr size_t kOff_m_xInnerUnknown    = 0x18;
constexpr size_t kOff_m_xDispatch        = 0x20;
constexpr size_t kOff_m_bResultExpected  = 0x28;
constexpr size_t kOff_m_xConnPtContainer = 0x30;
constexpr size_t kOff_m_pModuleState     = 0x38;
constexpr size_t kOff_m_nSizeElem        = 0x40;
constexpr size_t kOff_m_pClonedFrom      = 0x48;
constexpr size_t kOff_m_pvEnum           = 0x50;
constexpr size_t kOff_m_nCurPos          = 0x58;
constexpr size_t kOff_m_nSize            = 0x5c;
constexpr size_t kOff_m_bNeedFree        = 0x60;
constexpr size_t kOff_m_xEnumVOID        = 0x68;
constexpr size_t kSizeof                 = 0x70;

static_assert(kOff_m_pModuleState + sizeof(void*) == kOff_m_nSizeElem, "retail CCmdTarget part is 0x40 bytes");
static_assert(kOff_m_nCurPos + sizeof(unsigned int) == kOff_m_nSize, "m_nCurPos/m_nSize are adjacent UINTs");
static_assert(kOff_m_bNeedFree + sizeof(int) <= kOff_m_xEnumVOID, "m_bNeedFree precedes the interface part");
static_assert(kOff_m_xEnumVOID + sizeof(void*) == kSizeof, "sizeof(CEnumArray) == 0x70");

// ~CCmdTarget (0x1de430) calls slot 0x38/8 of m_xDispatch's own vtable
// (`mov 0x38(%rax),%rax; call`), with rcx = &m_xDispatch; the MFC source makes
// the COleDispatchImpl::Disconnect call there.
constexpr int kSlot_Dispatch_Disconnect = 0x38 / 8;
// CEnumArray virtuals, from the XEnumVOID call sites (`mov (owner),%rax;
// mov 0xNN(%rax),%rax; call`): the loads at 0x25b920 inside Next (0x25b8d0),
// 0x25b9ad inside Skip (0x25b980), 0x25ba1e inside Reset (0x25ba00) and
// 0x25ba85 inside Clone (0x25ba50).
constexpr int kSlot_OnNext  = 0xb0 / 8;   // 22
constexpr int kSlot_OnSkip  = 0xb8 / 8;   // 23
constexpr int kSlot_OnReset = 0xc0 / 8;   // 24
constexpr int kSlot_OnClone = 0xc8 / 8;   // 25
constexpr int kClassVtblSlots = 26;
constexpr int kXEnumVtblSlots = 7;

template <class T>
inline T& Member(void* pObj, size_t off) {
    return *reinterpret_cast<T*>(static_cast<char*>(pObj) + off);
}

// `mov (%rcx),%rax ; call *0xNN(%rax)`: slot NN/8 of pObj's own vtable.
template <class Fn>
inline Fn SlotFn(void* pObj, int slot) {
    return reinterpret_cast<Fn>((*static_cast<void***>(pObj))[slot]);
}

// METHOD_PROLOGUE_EX: Next, Skip, Reset and Clone recover the owner with
// `lea -0x68(%rcx)` and build AFX_MAINTAIN_STATE2 from owner->m_pModuleState
// (`mov 0x38(owner),%rdx ; call 0x133170`), restoring the previous module
// state on exit.  Reproduced through the exported AFX_MAINTAIN_STATE2 thunks;
// the local is only the key those thunks track the push under.
inline void* OwnerFromEnumVOID(void* pEnumVOID) {
    return static_cast<char*>(pEnumVOID) - kOff_m_xEnumVOID;
}

struct MaintainState {
    void* m_slot[2];
    explicit MaintainState(void* pOwner) {
        impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(
            m_slot, Member<void*>(pOwner, kOff_m_pModuleState));
    }
    ~MaintainState() { impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(m_slot); }
    MaintainState(const MaintainState&) = delete;
    MaintainState& operator=(const MaintainState&) = delete;
};

extern void* const g_CEnumArray_vtbl[kClassVtblSlots];
extern void* const g_XEnumVOID_vtbl[kXEnumVtblSlots];

}  // namespace

// CEnumArray::CEnumArray(size_t nSizeElem, const void* pvEnum, UINT nSize,
// BOOL bNeedFree) -- mfc140u RVA 0x25b660:
//     CCmdTarget::CCmdTarget(this)                  ; call 0x1de3f0
//     vfptr        = &CEnumArray::`vftable'          ; .rdata 0x32f8c8
//     m_xEnumVOID  = &XEnumVOID::`vftable'           ; .rdata 0x32f9a0, +0x68
//     m_bNeedFree  = bNeedFree                       ; +0x60 (stack arg)
//     m_nSizeElem  = nSizeElem                       ; +0x40
//     m_pClonedFrom = NULL                           ; +0x48
//     m_nCurPos    = 0                               ; +0x58
//     m_nSize      = nSize                           ; +0x5c
//     m_pvEnum     = pvEnum                          ; +0x50
//     return this
// CCmdTarget::CCmdTarget (0x1de3f0) is: m_pModuleState = AfxGetModuleState()
// (call 0x133930), m_dwRef = 1, m_pOuterUnknown = m_xInnerUnknown =
// m_xDispatch = m_xConnPtContainer = NULL, m_bResultExpected = 1 -- made inline
// here (see the top note).  Deviation: the pThis guard.
// Symbol: ??0CEnumArray@@QEAA@_KPEBXIH@Z
extern "C" void* MS_ABI impl___0CEnumArray__QEAA__KPEBXIH_Z(
    void* pThis, unsigned long long nSizeElem, const void* pvEnum, unsigned int nSize, int bNeedFree) {
    if (pThis == nullptr) return pThis;
    // CCmdTarget::CCmdTarget, 0x1de3f0
    Member<void*>(pThis, kOff_m_pModuleState)     = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    Member<long>(pThis, kOff_m_dwRef)             = 1;
    Member<void*>(pThis, kOff_m_pOuterUnknown)    = nullptr;
    Member<void*>(pThis, kOff_m_xInnerUnknown)    = nullptr;
    Member<void*>(pThis, kOff_m_xDispatch)        = nullptr;
    Member<void*>(pThis, kOff_m_xConnPtContainer) = nullptr;
    Member<int>(pThis, kOff_m_bResultExpected)    = 1;
    // this class
    Member<void*>(pThis, kOff_vfptr)          = const_cast<void**>(&g_CEnumArray_vtbl[0]);
    Member<void*>(pThis, kOff_m_xEnumVOID)    = const_cast<void**>(&g_XEnumVOID_vtbl[0]);
    Member<int>(pThis, kOff_m_bNeedFree)      = bNeedFree;
    Member<unsigned long long>(pThis, kOff_m_nSizeElem) = nSizeElem;
    Member<void*>(pThis, kOff_m_pClonedFrom)  = nullptr;
    Member<unsigned int>(pThis, kOff_m_nCurPos) = 0;
    Member<unsigned int>(pThis, kOff_m_nSize)   = nSize;
    Member<const void*>(pThis, kOff_m_pvEnum)   = pvEnum;
    return pThis;
}

// CEnumArray::~CEnumArray -- mfc140u RVA 0x25b720:
//     vfptr = &CEnumArray::`vftable'                 ; .rdata 0x32f8c8
//     if (m_pClonedFrom != NULL)                     ; +0x48
//         m_pClonedFrom->InternalRelease();          ; call 0x26cdb0
//     if (m_bNeedFree)                               ; cmpl $0,+0x60
//         operator delete(m_pvEnum, 1);              ; call 0x2b77b0 (edx = 1),
//                                                    ;   a jmp to ??3 (0x27c0) -> free
//     tail-jump CCmdTarget::~CCmdTarget              ; 0x1de430
// CCmdTarget::~CCmdTarget (0x1de430) is
//     if (m_xDispatch.vptr != NULL) m_xDispatch->vtbl[7]()   ; +0x20
//     m_pModuleState = NULL
// and is reproduced inline (see the top note).  The vptr store re-installs
// this file's vtable, as retail re-installs its own.  Deviation: the pThis
// guard.
// Symbol: ??1CEnumArray@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumArray__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    Member<void*>(pThis, kOff_vfptr) = const_cast<void**>(&g_CEnumArray_vtbl[0]);
    void* pClonedFrom = Member<void*>(pThis, kOff_m_pClonedFrom);
    if (pClonedFrom != nullptr) {
        impl__InternalRelease_CCmdTarget__QEAAKXZ(static_cast<CCmdTarget*>(pClonedFrom));
    }
    if (Member<int>(pThis, kOff_m_bNeedFree) != 0) {
        impl___3_YAXPEAX_Z(Member<void*>(pThis, kOff_m_pvEnum));
    }
    // CCmdTarget::~CCmdTarget, 0x1de430
    void* pDispatchPart = static_cast<char*>(pThis) + kOff_m_xDispatch;
    if (Member<void*>(pThis, kOff_m_xDispatch) != nullptr) {
        using DisconnectFn = void (MS_ABI*)(void* pDispatch);
        SlotFn<DisconnectFn>(pDispatchPart, kSlot_Dispatch_Disconnect)(pDispatchPart);
    }
    Member<void*>(pThis, kOff_m_pModuleState) = nullptr;
}

// CEnumArray::OnClone -- mfc140u RVA 0x25b810:
//     p = operator new(0x70);                        ; call 0x27f0 (??2)
//     if (p != NULL)
//         p = CEnumArray(p, m_nSizeElem, m_pvEnum, m_nSize, FALSE);   ; call 0x25b660
//     p->m_nCurPos = m_nCurPos;                      ; +0x58, NOT NULL-checked
//     return p;
// The clone shares the source array and never frees it (bNeedFree = FALSE);
// Clone then links it back to the owner through m_pClonedFrom.
// Deviation: the NULL check on the result.  In retail, ??2 does not return
// NULL under the default new handler -- it throws CMemoryException (see the
// ??2 note at the top), which Clone's TRY turns into E_UNEXPECTED.  Only when
// a handler installed with AfxSetNewHandler returns 0 does ??2 return NULL,
// and then retail writes m_nCurPos through the NULL pointer.  OpenMFC's ??2
// returns NULL on failure, so this returns NULL, and Clone maps that to
// E_UNEXPECTED, the result retail gives by default.
// Symbol: ?OnClone@CEnumArray@@MEAAPEAV1@XZ
extern "C" void* MS_ABI impl__OnClone_CEnumArray__MEAAPEAV1_XZ(void* pThis) {
    void* pClone = impl___2_YAPEAX_K_Z(kSizeof);
    if (pClone == nullptr) return nullptr;
    impl___0CEnumArray__QEAA__KPEBXIH_Z(pClone,
                                        Member<unsigned long long>(pThis, kOff_m_nSizeElem),
                                        Member<void*>(pThis, kOff_m_pvEnum),
                                        Member<unsigned int>(pThis, kOff_m_nSize),
                                        FALSE);
    Member<unsigned int>(pClone, kOff_m_nCurPos) = Member<unsigned int>(pThis, kOff_m_nCurPos);
    return pClone;
}

// CEnumArray::OnNext(void* pv) -- mfc140u RVA 0x25b770:
//     if (m_nCurPos >= m_nSize) return FALSE;        ; UINTs at +0x58 / +0x5c
//     Checked::memcpy_s(pv, m_nSizeElem,
//                       m_pvEnum + m_nCurPos * m_nSizeElem, m_nSizeElem);
//     ++m_nCurPos;                                   ; `add %eax,0x58(%rbx)`, eax = 1
//     return TRUE;
// memcpy_s is inlined: nothing happens when m_nSizeElem == 0; a NULL pv runs
// `*_errno() = EINVAL` (0x1802c7660), _invalid_parameter_noinfo (0x1802c7668)
// and AfxThrowInvalidArgException (0x227720); a NULL source first memsets pv
// (0x1802c7418) and then takes the same path; otherwise memcpy (0x1802c7420).
// Deviation, as in core/dialog/CDialogTemplate.cpp: the errno store and the
// invalid-parameter handler call are skipped and the exception thrown directly
// (under the UCRT's default handler retail terminates before the throw).
// Symbol: ?OnNext@CEnumArray@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumArray__MEAAHPEAX_Z(void* pThis, void* pv) {
    const unsigned int nCurPos = Member<unsigned int>(pThis, kOff_m_nCurPos);
    if (nCurPos >= Member<unsigned int>(pThis, kOff_m_nSize)) return FALSE;
    const size_t nSizeElem = Member<unsigned long long>(pThis, kOff_m_nSizeElem);
    if (nSizeElem != 0) {
        if (pv == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
        const BYTE* pSrc = Member<BYTE*>(pThis, kOff_m_pvEnum) + static_cast<size_t>(nCurPos) * nSizeElem;
        if (pSrc == nullptr) {
            memset(pv, 0, nSizeElem);
            impl__AfxThrowInvalidArgException__YAXXZ();
        }
        memcpy(pv, pSrc, nSizeElem);
    }
    Member<unsigned int>(pThis, kOff_m_nCurPos) += 1;
    return TRUE;
}

// CEnumArray::OnReset -- mfc140u RVA 0x25b800: `movl $0x0,0x58(%rcx); ret`.
// Symbol: ?OnReset@CEnumArray@@MEAAXXZ
extern "C" void MS_ABI impl__OnReset_CEnumArray__MEAAXXZ(void* pThis) {
    Member<unsigned int>(pThis, kOff_m_nCurPos) = 0;
}

// CEnumArray::OnSkip -- mfc140u RVA 0x25b7e0:
//     if (m_nCurPos >= m_nSize) return FALSE;
//     return ++m_nCurPos < m_nSize;
// Note the retail quirk this preserves: skipping onto the end (from the last
// element) advances the cursor but returns FALSE.
// Symbol: ?OnSkip@CEnumArray@@MEAAHXZ
extern "C" int MS_ABI impl__OnSkip_CEnumArray__MEAAHXZ(void* pThis) {
    const unsigned int nSize = Member<unsigned int>(pThis, kOff_m_nSize);
    unsigned int& nCurPos = Member<unsigned int>(pThis, kOff_m_nCurPos);
    if (nCurPos >= nSize) return FALSE;
    ++nCurPos;
    return nCurPos < nSize ? TRUE : FALSE;
}

// CEnumArray::XEnumVOID::Clone(IEnumVOID** ppenum) -- mfc140u RVA 0x25ba50
// (rcx = &m_xEnumVOID):
//     pThis = this - 0x68; AFX_MAINTAIN_STATE2(pThis->m_pModuleState)
//     *ppenum = NULL;                                ; no NULL check on ppenum
//     sc = E_UNEXPECTED;
//     TRY {
//         CEnumArray* pClone = pThis->OnClone();     ; vtbl +0xc8, slot 25
//         CEnumArray* pOwner = pThis->m_pClonedFrom ? pThis->m_pClonedFrom : pThis;
//         pClone->m_pClonedFrom = pOwner;            ; +0x48, cmovne
//         InterlockedIncrement(&pOwner->m_dwRef);    ; `lock incl 0x8(%rbx)`
//         *ppenum = &pClone->m_xEnumVOID;            ; +0x68
//         sc = S_OK;
//     } END_TRY                                      ; continuation 0x25bab4
//     return sc;
// So every clone (of a clone, too) holds one reference on the ORIGINAL
// enumerator, which owns the array; ~CEnumArray drops it.
// Deviations: the catch arm (top note); and retail dereferences a NULL clone
// (only possible from an OnClone override, or from a user new handler that
// returns 0), while this returns E_UNEXPECTED -- the sc retail holds at that
// point, and also the value retail returns when the base OnClone's allocation
// throws CMemoryException.
// Symbol: ?Clone@XEnumVOID@CEnumArray@@UEAAJPEAPEAUIEnumVOID@@@Z
extern "C" long MS_ABI impl__Clone_XEnumVOID_CEnumArray__UEAAJPEAPEAUIEnumVOID___Z(void* pThis, void** ppenum) {
    void* pOuter = OwnerFromEnumVOID(pThis);
    MaintainState state(pOuter);
    *ppenum = nullptr;
    using OnCloneFn = void* (MS_ABI*)(void* pThis);
    void* pClone = SlotFn<OnCloneFn>(pOuter, kSlot_OnClone)(pOuter);
    if (pClone == nullptr) return E_UNEXPECTED;
    void* pClonedFrom = Member<void*>(pOuter, kOff_m_pClonedFrom);
    void* pOwner = pClonedFrom != nullptr ? pClonedFrom : pOuter;
    Member<void*>(pClone, kOff_m_pClonedFrom) = pOwner;
    ::InterlockedIncrement(&Member<LONG>(pOwner, kOff_m_dwRef));
    *ppenum = static_cast<char*>(pClone) + kOff_m_xEnumVOID;
    return S_OK;
}

// CEnumArray::XEnumVOID::Next(ULONG celt, void* reelt, ULONG* pceltFetched) --
// mfc140u RVA 0x25b8d0 (rcx = &m_xEnumVOID):
//     pThis = this - 0x68; AFX_MAINTAIN_STATE2(pThis->m_pModuleState)
//     if (pceltFetched != NULL) *pceltFetched = 0;
//     BYTE* pchCur = (BYTE*)reelt; ULONG celtT = celt; sc = E_UNEXPECTED;
//     TRY {
//         while (celtT != 0 && pThis->OnNext(pchCur)) {   ; vtbl +0xb0, slot 22
//             pchCur += pThis->m_nSizeElem;              ; +0x40
//             --celtT;
//         }
//         if (pceltFetched != NULL) *pceltFetched = celt - celtT;
//         sc = celtT != 0 ? S_FALSE : S_OK;              ; test/setne
//     } END_TRY                                           ; continuation 0x25b94f
//     return sc;
// Deviation: the catch arm (top note).
// Symbol: ?Next@XEnumVOID@CEnumArray@@UEAAJKPEAXPEAK@Z
extern "C" long MS_ABI impl__Next_XEnumVOID_CEnumArray__UEAAJKPEAXPEAK_Z(
    void* pThis, unsigned long celt, void* reelt, unsigned long* pceltFetched) {
    void* pOuter = OwnerFromEnumVOID(pThis);
    MaintainState state(pOuter);
    if (pceltFetched != nullptr) *pceltFetched = 0;
    BYTE* pchCur = static_cast<BYTE*>(reelt);
    unsigned long celtT = celt;
    using OnNextFn = int (MS_ABI*)(void* pThis, void* pv);
    while (celtT != 0 && SlotFn<OnNextFn>(pOuter, kSlot_OnNext)(pOuter, pchCur) != 0) {
        pchCur += Member<unsigned long long>(pOuter, kOff_m_nSizeElem);
        --celtT;
    }
    if (pceltFetched != nullptr) *pceltFetched = celt - celtT;
    return celtT != 0 ? S_FALSE : S_OK;
}

// CEnumArray::XEnumVOID::Reset -- mfc140u RVA 0x25ba00 (rcx = &m_xEnumVOID):
//     pThis = this - 0x68; AFX_MAINTAIN_STATE2(pThis->m_pModuleState)
//     pThis->OnReset();                              ; vtbl +0xc0, slot 24
//     return S_OK;
// (no TRY here: 0x25ba00 has no exception handler in its .pdata unwind info,
// unlike Next, Skip and Clone.)
// Symbol: ?Reset@XEnumVOID@CEnumArray@@UEAAJXZ
extern "C" long MS_ABI impl__Reset_XEnumVOID_CEnumArray__UEAAJXZ(void* pThis) {
    void* pOuter = OwnerFromEnumVOID(pThis);
    MaintainState state(pOuter);
    using OnResetFn = void (MS_ABI*)(void* pThis);
    SlotFn<OnResetFn>(pOuter, kSlot_OnReset)(pOuter);
    return S_OK;
}

// CEnumArray::XEnumVOID::Skip(ULONG celt) -- mfc140u RVA 0x25b980
// (rcx = &m_xEnumVOID):
//     pThis = this - 0x68; AFX_MAINTAIN_STATE2(pThis->m_pModuleState)
//     ULONG celtT = celt;
//     TRY {
//         while (celtT != 0 && pThis->OnSkip())      ; vtbl +0xb8, slot 23
//             --celtT;
//     } END_TRY                                      ; continuation 0x25b9c8
//     return celtT != 0 ? S_FALSE : S_OK;            ; both paths
// Deviation: the catch arm (top note).
// Symbol: ?Skip@XEnumVOID@CEnumArray@@UEAAJK@Z
extern "C" long MS_ABI impl__Skip_XEnumVOID_CEnumArray__UEAAJK_Z(void* pThis, unsigned long celt) {
    void* pOuter = OwnerFromEnumVOID(pThis);
    MaintainState state(pOuter);
    unsigned long celtT = celt;
    using OnSkipFn = int (MS_ABI*)(void* pThis);
    while (celtT != 0 && SlotFn<OnSkipFn>(pOuter, kSlot_OnSkip)(pOuter) != 0) {
        --celtT;
    }
    return celtT != 0 ? S_FALSE : S_OK;
}

//=============================================================================
// Vtables (retail slot order; see the top note)
//=============================================================================
namespace {

// Retail's scalar deleting destructor, RVA 0x25b6d0 (vftable slot 1):
//     ~CEnumArray();                                 ; call 0x25b720
//     if (flags & 1) {
//         if (flags & 4) call 0x27d0 (this, 0x70)    ; a folded `ret`: frees nothing
//         else           free(this)                  ; IAT 0x1802c74e8
//     }
//     return this;
void* MS_ABI Slot_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CEnumArray__UEAA_XZ(pThis);
    if ((flags & 1) != 0 && (flags & 4) == 0) {
        impl___3_YAXPEAX_Z(pThis);
    }
    return pThis;
}

// The identical-COMDAT-folded bodies retail's slots point at.
void MS_ABI Slot_NoOp(void*) {}                   // 0x27d0: ret
int MS_ABI Slot_ReturnTrue(void*) { return 1; }   // 0x3a60: mov $1,%eax; ret
int MS_ABI Slot_ReturnZero(void*) { return 0; }   // 0x71e0: xor %eax,%eax; ret

// XEnumVOID IUnknown slots (not exported).
// QueryInterface, RVA 0x25b8a0: owner = this - 0x68; if owner->m_pOuterUnknown
// (+0x10) tail-call its QueryInterface (slot 0) with the same arguments, else
// tail-jump CCmdTarget::InternalQueryInterface (0x26cfe0) -- i.e.
// ExternalQueryInterface inlined.
long MS_ABI XEnum_QueryInterface(void* pThis, const IID* riid, void** ppvObj) {
    void* pOuter = OwnerFromEnumVOID(pThis);
    IUnknown* pOuterUnknown = Member<IUnknown*>(pOuter, kOff_m_pOuterUnknown);
    if (pOuterUnknown != nullptr) return pOuterUnknown->QueryInterface(*riid, ppvObj);
    return static_cast<long>(impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(
        static_cast<CCmdTarget*>(pOuter), riid, ppvObj));
}
// AddRef, RVA 0x25b860: `add $-0x68,%rcx; jmp ExternalAddRef (0x26cd80)`.
unsigned long MS_ABI XEnum_AddRef(void* pThis) {
    return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(static_cast<CCmdTarget*>(OwnerFromEnumVOID(pThis)));
}
// Release, RVA 0x25b870: ExternalRelease inlined (outer Release through slot 2
// when m_pOuterUnknown is set, else InternalRelease 0x26cdb0) -- the same body
// as the exported ExternalRelease thunk.
unsigned long MS_ABI XEnum_Release(void* pThis) {
    return impl__ExternalRelease_CCmdTarget__QEAAKXZ(static_cast<CCmdTarget*>(OwnerFromEnumVOID(pThis)));
}

#define VT(fn) reinterpret_cast<void*>(&fn)
void* const g_CEnumArray_vtbl[kClassVtblSlots] = {
    VT(impl__GetRuntimeClass_CCmdTarget__UEBAPEAUCRuntimeClass__XZ),                 // 0
    VT(Slot_ScalarDeletingDtor),                                                     // 1
    VT(Slot_NoOp),                                                                   // 2  Serialize
    VT(Slot_NoOp),                                                                   // 3  AssertValid
    VT(Slot_NoOp),                                                                   // 4  Dump
    VT(impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z),           // 5
    VT(impl__OnFinalRelease_CCmdTarget__UEAAXXZ),                                    // 6
    VT(Slot_ReturnTrue),                                                             // 7  IsInvokeAllowed
    VT(Slot_ReturnZero),                                                             // 8  GetDispatchIID
    VT(Slot_ReturnZero),                                                             // 9  GetTypeInfoCount
    VT(Slot_ReturnZero),                                                             // 10 GetTypeLibCache
    VT(impl__GetTypeLib_CCmdTarget__UEAAJKPEAPEAUITypeLib___Z),                      // 11
    VT(impl__GetMessageMap_CCmdTarget__MEBAPEBUAFX_MSGMAP__XZ),                      // 12
    VT(impl__GetCommandMap_CCmdTarget__MEBAPEBUAFX_OLECMDMAP__XZ),                   // 13
    VT(impl__GetDispatchMap_CCmdTarget__MEBAPEBUAFX_DISPMAP__XZ),                    // 14
    VT(impl__GetConnectionMap_CCmdTarget__MEBAPEBUAFX_CONNECTIONMAP__XZ),            // 15
    VT(impl__GetInterfaceMap_CCmdTarget__MEBAPEBUAFX_INTERFACEMAP__XZ),              // 16
    VT(impl__GetEventSinkMap_CCmdTarget__MEBAPEBUAFX_EVENTSINKMAP__XZ),              // 17
    VT(Slot_ReturnTrue),                                                             // 18 OnCreateAggregates
    VT(Slot_ReturnZero),                                                             // 19 GetInterfaceHook
    VT(Slot_ReturnZero),                                                             // 20 GetExtraConnectionPoints
    VT(Slot_ReturnZero),                                                             // 21 GetConnectionHook
    VT(impl__OnNext_CEnumArray__MEAAHPEAX_Z),                                        // 22
    VT(impl__OnSkip_CEnumArray__MEAAHXZ),                                            // 23
    VT(impl__OnReset_CEnumArray__MEAAXXZ),                                           // 24
    VT(impl__OnClone_CEnumArray__MEAAPEAV1_XZ),                                      // 25
};
void* const g_XEnumVOID_vtbl[kXEnumVtblSlots] = {
    VT(XEnum_QueryInterface),                                                        // 0
    VT(XEnum_AddRef),                                                                // 1
    VT(XEnum_Release),                                                               // 2
    VT(impl__Next_XEnumVOID_CEnumArray__UEAAJKPEAXPEAK_Z),                          // 3
    VT(impl__Skip_XEnumVOID_CEnumArray__UEAAJK_Z),                                  // 4
    VT(impl__Reset_XEnumVOID_CEnumArray__UEAAJXZ),                                  // 5
    VT(impl__Clone_XEnumVOID_CEnumArray__UEAAJPEAPEAUIEnumVOID___Z),                // 6
};
#undef VT
}  // namespace
