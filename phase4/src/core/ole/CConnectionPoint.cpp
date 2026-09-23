// CConnectionPoint — OpenMFC implementation.
// Sources: retail mfc140u.dll disassembly (method as in core/ole/COleControl.cpp).
//
// CConnectionPoint (afxdisp.h) is an AFX_NOVTABLE, abstract CCmdTarget
// (GetIID is pure).  Every live instance is a client-compiled subclass
// (BEGIN_CONNECTION_PART), so the object carries the RETAIL layout and an
// MSVC-layout vtable that the subclass constructor installed.  OpenMFC's
// own CCmdTarget is 32 bytes (afxwin.h) and has no member for anything
// from +0x20 up, so this file works on the object through the shadow
// struct below.  Nothing is added to a shared header.
//
// Layout, pinned from the retail bodies (all RVAs are mfc140u):
//   +0x00  vfptr                  never written by this class (AFX_NOVTABLE)
//   +0x08  m_dwRef                CCmdTarget ctor 0x1de3f0: `mov %ecx,0x8(%rbx)` (=1)
//   +0x10  m_pOuterUnknown        CCmdTarget ctor (XConnPt::Release 0x24b6d0 reads
//                                 +0x10 of the OWNER, this - m_nOffset, not this one)
//   +0x18  m_xInnerUnknown        CCmdTarget ctor
//   +0x20  m_xDispatch (vfptr)    CCmdTarget ctor; CCmdTarget dtor 0x1de430 tests it
//   +0x28  m_bResultExpected      CCmdTarget ctor: `mov %ecx,0x28(%rbx)` (=1)
//   +0x30  m_xConnPtContainer     CCmdTarget ctor
//   +0x38  m_pModuleState         CCmdTarget ctor: AfxGetModuleState() -> 0x38
//   +0x40  m_nOffset              GetContainer 0x24b540: `sub 0x40(%rcx),%rcx`
//   +0x48  m_pUnkFirstConnection  CConnectionPoint ctor 0x24b390: `movq $0,0x48(%rbx)`
//   +0x50  m_pConnections         CConnectionPoint ctor: `movq $0,0x50(%rbx)`
//   +0x58  m_xConnPt (vfptr)      CConnectionPoint ctor: `mov %rax,0x58(%rbx)`,
//                                 rax = XConnPt vftable 0x18032c408
//   sizeof 0x60: +0x58 is the last member the ctor writes and is 8 bytes.
// Every XConnPt method is entered with rcx = &m_xConnPt and recovers the
// outer object with `add $-0x58,%rcx` (METHOD_PROLOGUE); kOffXConnPt below.
//
// Outer virtuals.  The XConnPt bodies call these through the object's own
// vtable; the byte offsets are read from the call sites:
//   0xb0 GetContainer                 GetConnectionPointContainer 0x24b7c0
//   0xb8 GetIID                       GetConnectionInterface 0x24b790, QuerySinkInterface 0x24b670
//   0xc0 OnAdvise                     Advise 0x24b800, Unadvise 0x24b940
//   0xc8 GetMaxConnections            Advise 0x24b800
//   0xd0 QuerySinkInterface           Advise 0x24b800
// (slots 22..26: the five new virtuals in afxdisp.h declaration order, which
// puts them directly after 22 inherited CCmdTarget slots.)  Each call goes through the CFG dispatch pointer
// 0x1802c7b30, whose target 0x2b9d30 is `jmp *%rax` -- a plain indirect call.
//
// m_pConnections.  Retail creates it with `new CPtrArray` inlined
// (CreateConnectionArray 0x24b590: operator new(0x28), vftable 0x180321190)
// and afterwards reads CPtrArray's m_pData (+0x08) and m_nSize (+0x10)
// directly (GetNextConnection, Advise, Unadvise, the dtor), as do MSVC
// clients that inline GetConnections()->GetSize()/GetAt().  OpenMFC's
// CPtrArray (afx.h OPENMFC_DECLARE_ARRAY_WRAPPER) keeps its elements in a
// side table keyed by the object address and never fills those fields, so its
// impl__ thunks cannot be used here.  The array is therefore kept in the
// retail field layout (S_RetailPtrArray) by this file, the approach
// core/controlbar/CDockBar.cpp takes for m_arrBars.  DEVIATION: its vfptr is
// stored as nullptr, not the retail CPtrArray vftable (OpenMFC has no
// MSVC-layout CPtrArray vtable), and it is freed here directly rather than
// through the virtual deleting destructor retail calls.  Only this file
// allocates it, so allocation and free stay paired.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <ocidl.h>
#include <olectl.h>
#include <cstdlib>
#include <cstring>

extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();          // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();              // detail/MfcExceptionsSupport.cpp
extern "C" unsigned long MS_ABI impl__ExternalAddRef_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);   // core/runtime/CCmdTarget.cpp
extern "C" unsigned long MS_ABI impl__ExternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);  // core/runtime/CCmdTarget.cpp
extern "C" unsigned long MS_ABI impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(
    CCmdTarget* pThis, const void* iid, void** ppvObj);                                        // core/runtime/CCmdTarget.cpp

// This file's own thunks, referenced before their definitions (the XConnPt
// vtable, and sibling calls that retail makes directly rather than virtually).
extern "C" void MS_ABI impl__CreateConnectionArray_CConnectionPoint__QEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__GetConnectionCount_CConnectionPoint__QEAAHXZ(void* pThis);
extern "C" void* MS_ABI impl__GetNextConnection_CConnectionPoint__QEBAPEAUIUnknown__AEAPEAU__POSITION___Z(
    const void* pThis, void** pPos);
extern "C" long MS_ABI impl__GetConnectionInterface_XConnPt_CConnectionPoint__UEAAJPEAU_GUID___Z(
    void* pThis, GUID* pIID);
extern "C" long MS_ABI impl__GetConnectionPointContainer_XConnPt_CConnectionPoint__UEAAJPEAPEAUIConnectionPointContainer___Z(
    void* pThis, IConnectionPointContainer** ppCPC);
extern "C" long MS_ABI impl__Advise_XConnPt_CConnectionPoint__UEAAJPEAUIUnknown__PEAK_Z(
    void* pThis, IUnknown* pUnkSink, unsigned long* pdwCookie);
extern "C" long MS_ABI impl__Unadvise_XConnPt_CConnectionPoint__UEAAJK_Z(void* pThis, unsigned long dwCookie);
extern "C" long MS_ABI impl__EnumConnections_XConnPt_CConnectionPoint__UEAAJPEAPEAUIEnumConnections___Z(
    void* pThis, IEnumConnections** ppEnum);

namespace {

// --- retail-layout CPtrArray (afxcoll.h) ------------------------------------
struct S_RetailPtrArray {
    void*   vfptr;       // +0x00
    void**  m_pData;     // +0x08  GetNextConnection: `mov 0x8(%rax),%rax`
    INT_PTR m_nSize;     // +0x10  GetStartPosition: `cmp %rax,0x10(%rdx)`
    INT_PTR m_nMaxSize;  // +0x18  CPtrArray::SetSize 0x1d45d0: `mov 0x18(%rdi),%r8`
    INT_PTR m_nGrowBy;   // +0x20  CPtrArray::SetSize: `mov %r8,0x20(%rcx)`
};
static_assert(offsetof(S_RetailPtrArray, m_pData) == 0x08, "CPtrArray::m_pData at +0x08");
static_assert(offsetof(S_RetailPtrArray, m_nSize) == 0x10, "CPtrArray::m_nSize at +0x10");
static_assert(offsetof(S_RetailPtrArray, m_nMaxSize) == 0x18, "CPtrArray::m_nMaxSize at +0x18");
static_assert(offsetof(S_RetailPtrArray, m_nGrowBy) == 0x20, "CPtrArray::m_nGrowBy at +0x20");
static_assert(sizeof(S_RetailPtrArray) == 0x28, "CreateConnectionArray allocates 0x28 bytes");

// --- retail-layout CConnectionPoint ------------------------------------------
struct S_CConnectionPoint {
    void*             vfptr;                  // +0x00
    long              m_dwRef;                // +0x08
    long              _pad0c;
    IUnknown*         m_pOuterUnknown;        // +0x10
    void*             m_xInnerUnknown;        // +0x18
    void*             m_xDispatch;            // +0x20
    int               m_bResultExpected;      // +0x28
    int               _pad2c;
    void*             m_xConnPtContainer;     // +0x30
    void*             m_pModuleState;         // +0x38
    size_t            m_nOffset;              // +0x40
    IUnknown*         m_pUnkFirstConnection;  // +0x48
    S_RetailPtrArray* m_pConnections;         // +0x50
    const void*       m_xConnPt;              // +0x58  XConnPt vfptr
};
static_assert(offsetof(S_CConnectionPoint, m_dwRef) == 0x08, "CCmdTarget::m_dwRef at +0x08");
static_assert(offsetof(S_CConnectionPoint, m_pOuterUnknown) == 0x10, "CCmdTarget::m_pOuterUnknown at +0x10");
static_assert(offsetof(S_CConnectionPoint, m_xInnerUnknown) == 0x18, "CCmdTarget::m_xInnerUnknown at +0x18");
static_assert(offsetof(S_CConnectionPoint, m_xDispatch) == 0x20, "CCmdTarget::m_xDispatch at +0x20");
static_assert(offsetof(S_CConnectionPoint, m_bResultExpected) == 0x28, "CCmdTarget::m_bResultExpected at +0x28");
static_assert(offsetof(S_CConnectionPoint, m_xConnPtContainer) == 0x30, "CCmdTarget::m_xConnPtContainer at +0x30");
static_assert(offsetof(S_CConnectionPoint, m_pModuleState) == 0x38, "CCmdTarget::m_pModuleState at +0x38");
static_assert(offsetof(S_CConnectionPoint, m_nOffset) == 0x40, "CConnectionPoint::m_nOffset at +0x40");
static_assert(offsetof(S_CConnectionPoint, m_pUnkFirstConnection) == 0x48, "m_pUnkFirstConnection at +0x48");
static_assert(offsetof(S_CConnectionPoint, m_pConnections) == 0x50, "m_pConnections at +0x50");
static_assert(offsetof(S_CConnectionPoint, m_xConnPt) == 0x58, "m_xConnPt at +0x58");
static_assert(sizeof(S_CConnectionPoint) == 0x60, "sizeof(CConnectionPoint) == 0x60");

constexpr size_t kOffXConnPt = 0x58;  // METHOD_PROLOGUE: `add $0xffffffffffffffa8,%rcx`

inline S_CConnectionPoint* CP(void* pThis) { return static_cast<S_CConnectionPoint*>(pThis); }
inline const S_CConnectionPoint* CP(const void* pThis) { return static_cast<const S_CConnectionPoint*>(pThis); }
inline void* OuterFromConnPt(void* pConnPt) { return static_cast<char*>(pConnPt) - kOffXConnPt; }

// The owning CCmdTarget: (BYTE*)this - m_nOffset (GetContainer, XConnPt::AddRef/Release).
inline CCmdTarget* OwnerOf(void* pThis) {
    return reinterpret_cast<CCmdTarget*>(static_cast<char*>(pThis) - CP(pThis)->m_nOffset);
}

// --- outer virtuals (MSVC vtable at +0, byte offsets from the call sites) ----
constexpr size_t kVtGetContainer        = 0xb0;
constexpr size_t kVtGetIID              = 0xb8;
constexpr size_t kVtOnAdvise            = 0xc0;
constexpr size_t kVtGetMaxConnections   = 0xc8;
constexpr size_t kVtQuerySinkInterface  = 0xd0;

template <typename Fn>
inline Fn VSlot(void* pObj, size_t byteOffset) {
    void* const* vtbl = *static_cast<void* const* const*>(pObj);
    return reinterpret_cast<Fn>(vtbl[byteOffset / sizeof(void*)]);
}
inline IConnectionPointContainer* VGetContainer(void* p) {
    return VSlot<IConnectionPointContainer* (MS_ABI*)(void*)>(p, kVtGetContainer)(p);
}
inline const IID* VGetIID(void* p) {
    return VSlot<const IID* (MS_ABI*)(void*)>(p, kVtGetIID)(p);
}
inline void VOnAdvise(void* p, int bAdvise) {
    VSlot<void (MS_ABI*)(void*, int)>(p, kVtOnAdvise)(p, bAdvise);
}
inline int VGetMaxConnections(void* p) {
    return VSlot<int (MS_ABI*)(void*)>(p, kVtGetMaxConnections)(p);
}
inline long VQuerySinkInterface(void* p, IUnknown* pUnkSink, void** ppInterface) {
    return VSlot<long (MS_ABI*)(void*, IUnknown*, void**)>(p, kVtQuerySinkInterface)(p, pUnkSink, ppInterface);
}

// --- CPtrArray::SetAtGrow / SetSize on the retail fields ---------------------
// CPtrArray::SetSize(nNewSize, -1), mfc140u 0x1d45d0, growth branch only (the
// only callers here grow the array by appending):
//   if (nNewSize <= m_nMaxSize) { zero [m_nSize, nNewSize); m_nSize = nNewSize; }
//   else { grow = m_nGrowBy ? m_nGrowBy : clamp(m_nSize / 8, 4, 1024);
//          nNewMax = max(m_nMaxSize + grow, nNewSize);
//          new buffer; copy m_nSize elements; zero the rest up to nNewSize;
//          free(old) (import `free`, 0x1802c74e8); commit sizes }
//   first allocation (m_pData == NULL): exactly nNewSize elements, zeroed.
// Retail allocates with MFC operator new (0x27f0), which loops on malloc
// (import 0x1802c74c8) through the new-handler at +0x50 of
// AfxGetModuleThreadState() (0x133a20) and returns NULL when there is no
// handler or it gives up -- whereupon retail SetSize would memset NULL.  This
// uses malloc and the matching free, and on failure calls
// AfxThrowMemoryException instead (DEVIATION; the throw is what MFC's
// default new-handler is expected to do, not something read from 0x27f0).
void RetailSetSizeGrow(S_RetailPtrArray* a, INT_PTR nNewSize) {
    if (a->m_pData == nullptr) {
        void** p = static_cast<void**>(std::malloc(static_cast<size_t>(nNewSize) * sizeof(void*)));
        if (p == nullptr) { impl__AfxThrowMemoryException__YAXXZ(); return; }
        std::memset(p, 0, static_cast<size_t>(nNewSize) * sizeof(void*));
        a->m_pData = p;
        a->m_nMaxSize = nNewSize;
    } else if (nNewSize <= a->m_nMaxSize) {
        if (nNewSize > a->m_nSize) {
            std::memset(a->m_pData + a->m_nSize, 0,
                        static_cast<size_t>(nNewSize - a->m_nSize) * sizeof(void*));
        }
    } else {
        INT_PTR grow = a->m_nGrowBy;
        if (grow == 0) {
            grow = a->m_nSize / 8;
            grow = (grow < 4) ? 4 : ((grow > 1024) ? 1024 : grow);
        }
        INT_PTR nNewMax = (nNewSize < a->m_nMaxSize + grow) ? a->m_nMaxSize + grow : nNewSize;
        void** p = static_cast<void**>(std::malloc(static_cast<size_t>(nNewMax) * sizeof(void*)));
        if (p == nullptr) { impl__AfxThrowMemoryException__YAXXZ(); return; }
        std::memcpy(p, a->m_pData, static_cast<size_t>(a->m_nSize) * sizeof(void*));
        std::memset(p + a->m_nSize, 0, static_cast<size_t>(nNewSize - a->m_nSize) * sizeof(void*));
        std::free(a->m_pData);
        a->m_pData = p;
        a->m_nMaxSize = nNewMax;
    }
    a->m_nSize = nNewSize;
}
// CPtrArray::SetAtGrow, mfc140u 0x1d4910:
//   if (nIndex < 0) AfxThrowInvalidArgException();      ; js -> call 0x227720
//   if (nIndex >= m_nSize) SetSize(nIndex + 1, -1);     ; call 0x1d45d0
//   m_pData[nIndex] = newElement;
void RetailSetAtGrow(S_RetailPtrArray* a, INT_PTR nIndex, void* newElement) {
    if (nIndex < 0) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    if (nIndex >= a->m_nSize) RetailSetSizeGrow(a, nIndex + 1);
    a->m_pData[nIndex] = newElement;
}
// The bounds check CPtrArray::GetAt / ElementAt inline into every caller:
// `test idx; js` and `cmp 0x10(arr),idx; jge` -> AfxThrowInvalidArgException
// (mfc140u 0x227720).
inline bool RetailIndexOk(const S_RetailPtrArray* a, INT_PTR nIndex) {
    if (nIndex < 0 || nIndex >= a->m_nSize) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return false;
    }
    return true;
}
void RetailDeletePtrArray(S_RetailPtrArray* a) {
    std::free(a->m_pData);
    std::free(a);
}

// --- XConnPt IUnknown (not exported; entries of the XConnPt vftable) --------
// XConnPt::QueryInterface, mfc140u 0x24b710:
//   if (memcmp(riid, &IID_IUnknown, 16) == 0 ||          ; .rdata 0x1802d9a58
//       memcmp(riid, &IID_IConnectionPoint, 16) == 0) {  ; .rdata 0x1802d9c88
//       *ppv = this; this->vtbl[1]();  /* AddRef */ return S_OK;
//   }
//   *ppv = NULL; return E_NOINTERFACE;
// It answers from the part itself and does not delegate to the owner.  Retail
// checks neither riid nor ppv for NULL; neither does this.
long MS_ABI XConnPt_QueryInterface(void* pThis, const IID* riid, void** ppv);
// XConnPt::AddRef, mfc140u 0x24b700:
//   rcx -= 0x58; rcx -= m_nOffset; jmp CCmdTarget::ExternalAddRef (0x26cd80)
unsigned long MS_ABI XConnPt_AddRef(void* pThis) {
    return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(OwnerOf(OuterFromConnPt(pThis)));
}
// XConnPt::Release, mfc140u 0x24b6d0: the same owner adjustment, then
// ExternalRelease inlined (m_pOuterUnknown ? m_pOuterUnknown->Release()
// : InternalRelease 0x26cdb0) -- the ExternalRelease thunk makes the same
// split.  Note that OpenMFC's InternalRelease thunk (core/runtime/CCmdTarget.cpp)
// documents that it does not call OnFinalRelease when the count reaches 0.
unsigned long MS_ABI XConnPt_Release(void* pThis) {
    return impl__ExternalRelease_CCmdTarget__QEAAKXZ(OwnerOf(OuterFromConnPt(pThis)));
}
long MS_ABI XConnPt_QueryInterface(void* pThis, const IID* riid, void** ppv) {
    if (std::memcmp(riid, &IID_IUnknown, sizeof(IID)) == 0 ||
        std::memcmp(riid, &IID_IConnectionPoint, sizeof(IID)) == 0) {
        *ppv = pThis;
        XConnPt_AddRef(pThis);   // retail: this->vtbl[1], i.e. this table's AddRef
        return S_OK;
    }
    *ppv = nullptr;
    return E_NOINTERFACE;
}

// XConnPt vftable, mfc140u 0x18032c408 (slot order read from that table):
//   0 QueryInterface 0x24b710   1 AddRef 0x24b700   2 Release 0x24b6d0
//   3 GetConnectionInterface 0x24b790   4 GetConnectionPointContainer 0x24b7c0
//   5 Advise 0x24b800   6 Unadvise 0x24b940   7 EnumConnections 0x24bc60
void* const g_XConnPtVtbl[8] = {
    reinterpret_cast<void*>(&XConnPt_QueryInterface),
    reinterpret_cast<void*>(&XConnPt_AddRef),
    reinterpret_cast<void*>(&XConnPt_Release),
    reinterpret_cast<void*>(&impl__GetConnectionInterface_XConnPt_CConnectionPoint__UEAAJPEAU_GUID___Z),
    reinterpret_cast<void*>(&impl__GetConnectionPointContainer_XConnPt_CConnectionPoint__UEAAJPEAPEAUIConnectionPointContainer___Z),
    reinterpret_cast<void*>(&impl__Advise_XConnPt_CConnectionPoint__UEAAJPEAUIUnknown__PEAK_Z),
    reinterpret_cast<void*>(&impl__Unadvise_XConnPt_CConnectionPoint__UEAAJK_Z),
    reinterpret_cast<void*>(&impl__EnumConnections_XConnPt_CConnectionPoint__UEAAJPEAPEAUIEnumConnections___Z),
};

} // namespace

// CConnectionPoint::CConnectionPoint, mfc140u 0x24b390:
//     CCmdTarget::CCmdTarget(this);             ; call 0x1de3f0
//     m_pUnkFirstConnection = NULL;             ; movq $0,0x48(%rbx)
//     m_pConnections = NULL;                    ; movq $0,0x50(%rbx)
//     m_xConnPt.vfptr = &XConnPt::`vftable';    ; 0x18032c408 -> 0x58(%rbx)
// CCmdTarget::CCmdTarget (0x1de3f0) is reproduced inline, as
// featurepack/controls/CMFCBaseAccessibleObject.cpp does, rather than through
// ??0CCmdTarget's thunk: that thunk placement-news OpenMFC's 32-byte
// CCmdTarget (installing g++'s vtable and zeroing only +0x08..+0x1f), while
// retail stores no vptr (AFX_NOVTABLE) and initialises up to +0x38:
//     m_pModuleState = AfxGetModuleState();     ; call 0x133930, -> 0x38
//     m_dwRef = 1;                              ; -> 0x08
//     m_pOuterUnknown = m_xInnerUnknown = m_xDispatch = m_xConnPtContainer = 0;
//     m_bResultExpected = 1;                    ; -> 0x28
// m_nOffset (+0x40) is left for the BEGIN_CONNECTION_PART subclass ctor to
// set, as in retail.  Deviation: the pThis guard.
// Symbol: ??0CConnectionPoint@@QEAA@XZ
extern "C" void* MS_ABI impl___0CConnectionPoint__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    S_CConnectionPoint* p = CP(pThis);
    p->m_pModuleState = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    p->m_dwRef = 1;
    p->m_pOuterUnknown = nullptr;
    p->m_xInnerUnknown = nullptr;
    p->m_xDispatch = nullptr;
    p->m_xConnPtContainer = nullptr;
    p->m_bResultExpected = 1;
    p->m_pUnkFirstConnection = nullptr;
    p->m_pConnections = nullptr;
    p->m_xConnPt = &g_XConnPtVtbl[0];
    return pThis;
}

// CConnectionPoint::~CConnectionPoint, mfc140u 0x24b3d0:
//     POSITION pos = GetStartPosition();        ; inlined
//     while (pos != NULL) {
//         LPUNKNOWN pUnk = GetNextConnection(pos);   ; call 0x24b4c0
//         if (pUnk != NULL) pUnk->Release();         ; vtbl +0x10
//     }
//     if (m_pConnections != NULL)
//         m_pConnections->vtbl[1](1);           ; scalar deleting dtor = delete
//     CCmdTarget::~CCmdTarget(this);            ; call 0x1de430
// The body runs in a GS-cookie-protected frame of 0x460 bytes (0x260 in the
// ANSI mfc140.dll -- among this file's bodies the only ANSI/Unicode difference, which
// fits a TCHAR[512] local) and has a second entry, 0x24b46d, that reloads
// `this` from the frame before the CCmdTarget call: the shape of a catch
// continuation -- presumably MFC's AFX_BEGIN_DESTRUCTOR / AFX_END_DESTRUCTOR,
// which swallow a CException*.  The handler itself (in the unwind data) was
// not decoded.
// CCmdTarget::~CCmdTarget (0x1de430) is reproduced inline:
//     if (m_xDispatch.vfptr != NULL) m_xDispatch.vtbl[7](&m_xDispatch);  ; +0x38: Disconnect
//     m_pModuleState = NULL;
// not through ??1CCmdTarget's thunk, which runs `pThis->~CCmdTarget()` -- a
// virtual call through this object's MSVC-layout vptr (the reason
// CMFCBaseAccessibleObject.cpp inlines it too).
// DEVIATIONS: no exception handler is installed; the array is
// freed directly (see the header note); the pThis guard.
// Symbol: ??1CConnectionPoint@@UEAA@XZ
extern "C" void MS_ABI impl___1CConnectionPoint__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_CConnectionPoint* p = CP(pThis);
    void* pos = (p->m_pUnkFirstConnection != nullptr) ? reinterpret_cast<void*>(-1)
              : (p->m_pConnections != nullptr && p->m_pConnections->m_nSize != 0) ? reinterpret_cast<void*>(1)
              : nullptr;
    while (pos != nullptr) {
        IUnknown* pUnk = static_cast<IUnknown*>(
            impl__GetNextConnection_CConnectionPoint__QEBAPEAUIUnknown__AEAPEAU__POSITION___Z(pThis, &pos));
        if (pUnk != nullptr) pUnk->Release();
    }
    if (p->m_pConnections != nullptr) {
        RetailDeletePtrArray(p->m_pConnections);
    }
    // CCmdTarget::~CCmdTarget
    if (p->m_xDispatch != nullptr) {
        void* pDispatchPart = &p->m_xDispatch;
        VSlot<void (MS_ABI*)(void*)>(pDispatchPart, 0x38)(pDispatchPart);
    }
    p->m_pModuleState = nullptr;
}

// CConnectionPoint::CreateConnectionArray, mfc140u 0x24b590:
//     CPtrArray* pArray = new CPtrArray;        ; operator new(0x28), inlined ctor:
//                                               ;   vftable, m_pData = m_nSize =
//                                               ;   m_nMaxSize = m_nGrowBy = 0
//     m_pConnections = pArray;                  ; stored even when new gave NULL
//     if (m_pUnkFirstConnection != NULL) {
//         pArray->Add(m_pUnkFirstConnection);   ; SetAtGrow(m_nSize, p), 0x1d4910
//         m_pUnkFirstConnection = NULL;
//     }
// DEVIATIONS: vfptr stored as nullptr (header note); an allocation failure
// with a first connection pending returns instead of dereferencing NULL.
// Symbol: ?CreateConnectionArray@CConnectionPoint@@QEAAXXZ
extern "C" void MS_ABI impl__CreateConnectionArray_CConnectionPoint__QEAAXXZ(void* pThis) {
    S_CConnectionPoint* p = CP(pThis);
    S_RetailPtrArray* pArray = static_cast<S_RetailPtrArray*>(std::calloc(1, sizeof(S_RetailPtrArray)));
    p->m_pConnections = pArray;
    if (p->m_pUnkFirstConnection != nullptr) {
        if (pArray == nullptr) return;
        RetailSetAtGrow(pArray, pArray->m_nSize, p->m_pUnkFirstConnection);
        p->m_pUnkFirstConnection = nullptr;
    }
}

// CConnectionPoint::GetConnectionCount, mfc140u 0x24b600:
//     if (m_pUnkFirstConnection != NULL) return 1;
//     if (m_pConnections == NULL) return 0;
//     int n = 0;
//     if (m_pConnections->m_nSize != 0) {
//         POSITION pos = (POSITION)1;
//         do { if (GetNextConnection(pos) != NULL) ++n; } while (pos != NULL);  ; call 0x24b4c0
//     }
//     return n;
// Symbol: ?GetConnectionCount@CConnectionPoint@@QEAAHXZ
extern "C" int MS_ABI impl__GetConnectionCount_CConnectionPoint__QEAAHXZ(void* pThis) {
    S_CConnectionPoint* p = CP(pThis);
    if (p->m_pUnkFirstConnection != nullptr) return 1;
    if (p->m_pConnections == nullptr) return 0;
    int n = 0;
    if (p->m_pConnections->m_nSize != 0) {
        void* pos = reinterpret_cast<void*>(1);
        do {
            if (impl__GetNextConnection_CConnectionPoint__QEBAPEAUIUnknown__AEAPEAU__POSITION___Z(pThis, &pos) != nullptr)
                ++n;
        } while (pos != nullptr);
    }
    return n;
}

// CConnectionPoint::GetConnections, mfc140u 0x24b520:
//     if (m_pConnections == NULL) CreateConnectionArray();   ; call 0x24b590
//     return m_pConnections;
// The array returned has the retail field layout (header note), which is what
// an MSVC client's inlined GetSize()/GetAt() reads.
// Symbol: ?GetConnections@CConnectionPoint@@QEAAPEBVCPtrArray@@XZ
extern "C" const void* MS_ABI impl__GetConnections_CConnectionPoint__QEAAPEBVCPtrArray__XZ(void* pThis) {
    if (CP(pThis)->m_pConnections == nullptr)
        impl__CreateConnectionArray_CConnectionPoint__QEAAXXZ(pThis);
    return CP(pThis)->m_pConnections;
}

// CConnectionPoint::GetContainer, mfc140u 0x24b540:
//     CCmdTarget* pOwner = (CCmdTarget*)((BYTE*)this - m_nOffset);   ; sub 0x40(%rcx),%rcx
//     LPCONNECTIONPOINTCONTAINER pCPC = NULL;
//     pOwner->ExternalQueryInterface(&IID_IConnectionPointContainer, &pCPC);
//         ; inlined: m_pOuterUnknown (+0x10) ? its QueryInterface (vtbl +0)
//         ;          : InternalQueryInterface (0x26cfe0); IID at .rdata 0x1802d9ca8
//     return pCPC;                              ; HRESULT ignored
// The ExternalQueryInterface thunk performs the same two-way dispatch.
// CAVEAT: without an outer unknown it lands in OpenMFC's InternalQueryInterface,
// whose GetInterface walks only CCmdTarget's own (empty) interface map, not the
// client owner's map (core/runtime/CCmdTarget.cpp), so today this returns NULL
// unless the owner is aggregated.
// Symbol: ?GetContainer@CConnectionPoint@@UEAAPEAUIConnectionPointContainer@@XZ
extern "C" IConnectionPointContainer* MS_ABI impl__GetContainer_CConnectionPoint__UEAAPEAUIConnectionPointContainer__XZ(void* pThis) {
    IConnectionPointContainer* pCPC = nullptr;
    impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(
        OwnerOf(pThis), &IID_IConnectionPointContainer, reinterpret_cast<void**>(&pCPC));
    return pCPC;
}

// CConnectionPoint::GetMaxConnections: the export resolves (ordinal 5714) to
// mfc140u 0x88a0, a body shared by identical-COMDAT folding:
//     or $0xffffffff,%eax ; ret
// i.e. return -1 (no limit).
// Symbol: ?GetMaxConnections@CConnectionPoint@@UEAAHXZ
extern "C" int MS_ABI impl__GetMaxConnections_CConnectionPoint__UEAAHXZ(void* pThis) {
    (void)pThis;
    return -1;
}

// CConnectionPoint::GetNextConnection, mfc140u 0x24b4c0:
//     if (pos == (POSITION)-1) { pos = NULL; return m_pUnkFirstConnection; }
//     int nIndex = (int)pos - 1;                ; low dword of pos, dec
//     pos = (POSITION)((INT_PTR)pos + 1);
//     if ((INT_PTR)pos > m_pConnections->m_nSize) pos = NULL;
//     return m_pConnections->GetAt(nIndex);     ; bounds check -> 0x227720
// Symbol: ?GetNextConnection@CConnectionPoint@@QEBAPEAUIUnknown@@AEAPEAU__POSITION@@@Z
extern "C" void* MS_ABI impl__GetNextConnection_CConnectionPoint__QEBAPEAUIUnknown__AEAPEAU__POSITION___Z(
    const void* pThis, void** pPos) {
    const S_CConnectionPoint* p = CP(pThis);
    INT_PTR pos = reinterpret_cast<INT_PTR>(*pPos);
    if (pos == -1) {
        *pPos = nullptr;
        return p->m_pUnkFirstConnection;
    }
    int nIndex = static_cast<int>(pos) - 1;
    ++pos;
    *pPos = reinterpret_cast<void*>(pos);
    const S_RetailPtrArray* a = p->m_pConnections;
    if (pos > a->m_nSize) *pPos = nullptr;
    if (!RetailIndexOk(a, nIndex)) return nullptr;
    return a->m_pData[nIndex];
}

// CConnectionPoint::GetStartPosition, mfc140u 0x24b4a0:
//     if (m_pUnkFirstConnection != NULL) return (POSITION)-1;
//     if (m_pConnections == NULL) return NULL;
//     return (POSITION)(m_pConnections->m_nSize != 0);
// Symbol: ?GetStartPosition@CConnectionPoint@@QEBAPEAU__POSITION@@XZ
extern "C" void* MS_ABI impl__GetStartPosition_CConnectionPoint__QEBAPEAU__POSITION__XZ(const void* pThis) {
    const S_CConnectionPoint* p = CP(pThis);
    if (p->m_pUnkFirstConnection != nullptr) return reinterpret_cast<void*>(-1);
    if (p->m_pConnections == nullptr) return nullptr;
    return reinterpret_cast<void*>(static_cast<INT_PTR>(p->m_pConnections->m_nSize != 0));
}

// CConnectionPoint::OnAdvise: the export resolves (ordinal 8624) to mfc140u
// 0x27d0, which is `ret $0x0` -- an empty body shared by COMDAT folding.
// Symbol: ?OnAdvise@CConnectionPoint@@UEAAXH@Z
extern "C" void MS_ABI impl__OnAdvise_CConnectionPoint__UEAAXH_Z(void* pThis, int bAdvise) {
    (void)pThis;
    (void)bAdvise;
}

// CConnectionPoint::QuerySinkInterface, mfc140u 0x24b670:
//     if (ppInterface == NULL) return E_POINTER;
//     *ppInterface = NULL;
//     return pUnkSink->QueryInterface(*GetIID(), ppInterface);   ; GetIID = vtbl +0xb8
// Symbol: ?QuerySinkInterface@CConnectionPoint@@UEAAJPEAUIUnknown@@PEAPEAX@Z
extern "C" long MS_ABI impl__QuerySinkInterface_CConnectionPoint__UEAAJPEAUIUnknown__PEAPEAX_Z(
    void* pThis, IUnknown* pUnkSink, void** ppInterface) {
    if (ppInterface == nullptr) return E_POINTER;
    *ppInterface = nullptr;
    return pUnkSink->QueryInterface(*VGetIID(pThis), ppInterface);
}

// CConnectionPoint::XConnPt::Advise, mfc140u 0x24b800 (rcx = &m_xConnPt):
//     if (pUnkSink == NULL) return E_POINTER;
//     pThis = this - 0x58;
//     int nMax = pThis->GetMaxConnections();                        ; vtbl +0xc8
//     if (nMax >= 0 && pThis->GetConnectionCount() == nMax)         ; call 0x24b600
//         return CONNECT_E_ADVISELIMIT;                             ; 0x80040201
//     LPUNKNOWN pSink;
//     hr = pThis->QuerySinkInterface(pUnkSink, (void**)&pSink);     ; vtbl +0xd0
//     if (FAILED(hr)) return hr;
//     if (m_pUnkFirstConnection == NULL && m_pConnections == NULL) {
//         m_pUnkFirstConnection = pSink; dwCookie = 1;
//     } else {
//         if (m_pConnections == NULL) CreateConnectionArray();      ; call 0x24b590
//         dwCookie = 0;
//         for (i = 0; dwCookie == 0 && i < m_nSize; ++i)            ; first NULL hole
//             if (GetAt(i) == NULL) { SetAt(i, pSink); dwCookie = i + 1; }
//         if (dwCookie == 0) dwCookie = Add(pSink) + 1;              ; SetAtGrow(m_nSize, ...)
//     }
//     pThis->OnAdvise(TRUE);                                        ; vtbl +0xc0
//     if (pdwCookie != NULL) *pdwCookie = dwCookie;
//     return S_OK;
// The GetAt bounds check inside the hole search cannot fail (i < m_nSize); the
// `return E_UNEXPECTED` after it is a fallback for a non-returning throw.
// Symbol: ?Advise@XConnPt@CConnectionPoint@@UEAAJPEAUIUnknown@@PEAK@Z
extern "C" long MS_ABI impl__Advise_XConnPt_CConnectionPoint__UEAAJPEAUIUnknown__PEAK_Z(
    void* pThis, IUnknown* pUnkSink, unsigned long* pdwCookie) {
    if (pUnkSink == nullptr) return E_POINTER;
    void* pOuter = OuterFromConnPt(pThis);
    S_CConnectionPoint* p = CP(pOuter);

    int nMax = VGetMaxConnections(pOuter);
    if (nMax >= 0 && impl__GetConnectionCount_CConnectionPoint__QEAAHXZ(pOuter) == nMax)
        return CONNECT_E_ADVISELIMIT;

    void* pSink = nullptr;
    long hr = VQuerySinkInterface(pOuter, pUnkSink, &pSink);
    if (hr < 0) return hr;

    unsigned long dwCookie;
    if (p->m_pUnkFirstConnection == nullptr && p->m_pConnections == nullptr) {
        p->m_pUnkFirstConnection = static_cast<IUnknown*>(pSink);
        dwCookie = 1;
    } else {
        if (p->m_pConnections == nullptr)
            impl__CreateConnectionArray_CConnectionPoint__QEAAXXZ(pOuter);
        S_RetailPtrArray* a = p->m_pConnections;
        dwCookie = 0;
        for (INT_PTR i = 0; dwCookie == 0 && i < a->m_nSize; ++i) {
            if (!RetailIndexOk(a, i)) return E_UNEXPECTED;
            if (a->m_pData[i] == nullptr) {
                a->m_pData[i] = pSink;
                dwCookie = static_cast<unsigned long>(i + 1);
            }
        }
        if (dwCookie == 0) {
            INT_PTR nIndex = a->m_nSize;
            RetailSetAtGrow(a, nIndex, pSink);
            dwCookie = static_cast<unsigned long>(nIndex) + 1;
        }
    }
    VOnAdvise(pOuter, TRUE);
    if (pdwCookie != nullptr) *pdwCookie = dwCookie;
    return S_OK;
}

// CConnectionPoint::XConnPt::EnumConnections -- STUB.  Retail (mfc140u
// 0x24bc60) is:
//     AFX_MANAGE_STATE2(pThis->m_pModuleState);         ; call 0x133170, [rcx-0x20]
//     CEnumConnections* pEnum = new CEnumConnections(NULL, 0);   ; new(0x78), ctor 0x24ba10
//     if (m_pUnkFirstConnection) pEnum->AddConnection({first, 1});  ; 0x24bb50
//     n = (int)m_pConnections->m_nSize;                ; if m_pConnections; read once
//     for (i = 0; i < n; ) {
//         pUnk = GetAt(i); ++i;                        ; bounds check -> 0x227720
//         if (pUnk) pEnum->AddConnection({pUnk, i});   ; cookie = index + 1
//     }
//     *ppEnum = pEnum ? &pEnum->m_xEnumVOID : NULL;    ; lea 0x68(%rbx)
//     return pEnum ? S_OK : E_OUTOFMEMORY;
// plus an exception continuation at 0x24bd3e that reloads ppEnum/pEnum from
// the frame, stores *ppEnum = NULL and returns by the reloaded pEnum (the catch
// handler itself, which presumably clears that slot, was not decoded).
// The IEnumConnections it hands out is the CEnumArray::XEnumVOID interface
// at +0x68 of a 0x78-byte CEnumConnections.  OpenMFC's ??0CEnumConnections
// (core/ole/CEnumConnections.cpp) is a stub that returns pThis without
// building that object or its interface vtable, so the pointer retail returns
// would have no vtable here.  Rather than hand out a dangling interface, the
// stub clears *ppEnum and fails.
// Symbol: ?EnumConnections@XConnPt@CConnectionPoint@@UEAAJPEAPEAUIEnumConnections@@@Z
extern "C" long MS_ABI impl__EnumConnections_XConnPt_CConnectionPoint__UEAAJPEAPEAUIEnumConnections___Z(
    void* pThis, IEnumConnections** ppEnum) {
    (void)pThis;
    if (ppEnum != nullptr) *ppEnum = nullptr;
    return E_NOTIMPL;
}

// CConnectionPoint::XConnPt::GetConnectionInterface, mfc140u 0x24b790:
//     *pIID = *(this - 0x58)->GetIID();        ; vtbl +0xb8, 16-byte copy
//     return S_OK;
// No NULL check on pIID in retail; none here.
// Symbol: ?GetConnectionInterface@XConnPt@CConnectionPoint@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetConnectionInterface_XConnPt_CConnectionPoint__UEAAJPEAU_GUID___Z(
    void* pThis, GUID* pIID) {
    *pIID = *VGetIID(OuterFromConnPt(pThis));
    return S_OK;
}

// CConnectionPoint::XConnPt::GetConnectionPointContainer, mfc140u 0x24b7c0:
//     *ppCPC = (this - 0x58)->GetContainer();  ; vtbl +0xb0
//     return *ppCPC != NULL ? S_OK : E_FAIL;   ; neg/sbb/not/and 0x80004005
// Symbol: ?GetConnectionPointContainer@XConnPt@CConnectionPoint@@UEAAJPEAPEAUIConnectionPointContainer@@@Z
extern "C" long MS_ABI impl__GetConnectionPointContainer_XConnPt_CConnectionPoint__UEAAJPEAPEAUIConnectionPointContainer___Z(
    void* pThis, IConnectionPointContainer** ppCPC) {
    IConnectionPointContainer* pCPC = VGetContainer(OuterFromConnPt(pThis));
    *ppCPC = pCPC;
    return pCPC != nullptr ? S_OK : E_FAIL;
}

// CConnectionPoint::XConnPt::Unadvise, mfc140u 0x24b940 (rcx = &m_xConnPt):
//     pThis = this - 0x58;
//     if (m_pUnkFirstConnection != NULL) {
//         if (dwCookie != 1) return CONNECT_E_NOCONNECTION;   ; 0x80040200
//         m_pUnkFirstConnection->Release(); m_pUnkFirstConnection = NULL;
//     } else {
//         if (m_pConnections == NULL) return CONNECT_E_NOCONNECTION;
//         int i = (int)dwCookie - 1;
//         if (i < 0 || i >= (int)m_nSize) return CONNECT_E_NOCONNECTION;  ; 32-bit cmp
//         LPUNKNOWN pUnk = GetAt(i);                           ; bounds check -> throw
//         if (pUnk == NULL) return CONNECT_E_NOCONNECTION;
//         pUnk->Release();
//         SetAt(i, NULL);                                      ; bounds check -> throw
//     }
//     pThis->OnAdvise(FALSE);                                  ; vtbl +0xc0
//     return S_OK;
// The `return` after each RetailIndexOk failure is only a fallback: the
// exception thunk throws and does not return.
// Symbol: ?Unadvise@XConnPt@CConnectionPoint@@UEAAJK@Z
extern "C" long MS_ABI impl__Unadvise_XConnPt_CConnectionPoint__UEAAJK_Z(void* pThis, unsigned long dwCookie) {
    void* pOuter = OuterFromConnPt(pThis);
    S_CConnectionPoint* p = CP(pOuter);
    if (p->m_pUnkFirstConnection != nullptr) {
        if (dwCookie != 1) return CONNECT_E_NOCONNECTION;
        p->m_pUnkFirstConnection->Release();
        p->m_pUnkFirstConnection = nullptr;
    } else {
        S_RetailPtrArray* a = p->m_pConnections;
        if (a == nullptr) return CONNECT_E_NOCONNECTION;
        int i = static_cast<int>(dwCookie) - 1;
        if (i < 0 || i >= static_cast<int>(a->m_nSize)) return CONNECT_E_NOCONNECTION;
        if (!RetailIndexOk(a, i)) return CONNECT_E_NOCONNECTION;
        IUnknown* pUnk = static_cast<IUnknown*>(a->m_pData[i]);
        if (pUnk == nullptr) return CONNECT_E_NOCONNECTION;
        pUnk->Release();
        a = p->m_pConnections;   // retail re-reads +0x50 after the Release
        if (!RetailIndexOk(a, i)) return CONNECT_E_NOCONNECTION;
        a->m_pData[i] = nullptr;
    }
    VOnAdvise(pOuter, FALSE);
    return S_OK;
}
