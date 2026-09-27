// CAnimationPoint — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// AddTransition, GetAnimationVariableList, GetDefaultValue, GetValue and
// SetDefaultValue are transcribed from the retail exports (RVAs cited per
// function, mfc140u).  mfc140_rva_symbols.json (the ANSI map disas.py reads
// by default) lists only the two ctors and GetThisClass for CAnimationPoint,
// none of these five, so every RVA below was read with `disas.py --u`;
// SetDefaultValue is also missing from
// mfc140u_rva_symbols.json and was resolved through the mfc140u export
// table (ordinal 13118 -> RVA 0x4ba0).
//
// RETAIL LAYOUT (afxanimationcontroller.h:1188, DECLARE_DYNAMIC, derives
// CAnimationBaseObject; m_nObjectSize 232 == 0xe8 in
// core/animation/RuntimeClasses.cpp).  Offsets as the bodies below read them:
//   +0x000 CAnimationBaseObject (0x28 bytes; see CAnimationBaseObject.cpp)
//   +0x028 CAnimationVariable m_xValue   (SetDefaultValue: `add $0x28,%rcx`)
//   +0x088 CAnimationVariable m_yValue   (`lea 0x88(%rdi),%rcx`)
// The default ctor (RVA 0x49a0 mfc140u) stores the CAnimationVariable
// vfptr (0x1802dae28 mfc140u; its slot 1 is CAnimationVariable::Create) at
// +0x28 and +0x88, and its highest-offset store is the dword at +0xe0 (the
// last store in program order is the qword at +0xd8), which fits a
// 0x60-byte CAnimationVariable ending at 0xe8.  Within a variable these
// bodies touch +0x08 m_variable (CComPtr<IUIAnimationVariable>), +0x10
// m_dblDefaultValue and +0x18 m_lstTransitions (CObList; AddTransition),
// all only via code inlined from the variable's own accessors.
// include/openmfc does not declare CAnimationPoint or CAnimationBaseObject,
// so the layout is pinned here on a file-local shadow (same approach as
// CAnimationRect.cpp).
//
// HOW THE EMBEDDED VARIABLES ARE REACHED (thunks in
// core/animation/CAnimationVariable.cpp):
//   AddTransition(CBaseTransition*)  RVA 0x3e80 (mfc140u), inlined twice into
//     AddTransition.  OpenMFC's thunk neither tests nor sets
//     CBaseTransition::m_pRelatedVariable, so that part is transcribed inline
//     below and only the list append is delegated.
//   GetValue(DOUBLE&)                RVA 0x3e20 (mfc140u), inlined twice into
//     GetValue (the non-NULL path calls IUIAnimationVariable vtable +0x18,
//     slot 3 = GetValue(DOUBLE*), then truncates with cvttsd2si -- NOT the
//     GetValue(INT32&) accessor, whose non-NULL path uses slot 6 (+0x30,
//     GetIntegerValue)); called here through its impl__ thunk.
//   SetDefaultValue(DOUBLE)          RVA 0x3df0 (mfc140u, export ordinal
//     13121), called out of line by SetDefaultValue(POINT); called here
//     through its impl__ thunk.
// The default-value read, CAnimationVariable::GetDefaultValue(), is inline
// in afxanimationcontroller.h (no export); retail reads m_dblDefaultValue at
// +0x10 of the variable with `cvttsd2si`.
//
// DEVIATION, file-wide (identical to CAnimationRect.cpp): OpenMFC's
// CAnimationVariable does not keep its state at retail offsets -- its ctor /
// SetDefaultValue / GetValue thunks key a side table on the object's address,
// and +0x10 is never written.  DefaultValueOf() below therefore obtains the
// default through the GetValue(DOUBLE&) thunk.  That equals
// m_dblDefaultValue only because OpenMFC's variable never creates an
// IUIAnimationVariable (its Create returns TRUE without creating one), so its
// GetValue always takes the retail "m_variable == NULL -> return
// m_dblDefaultValue" path.  If CAnimationVariable ever moves to retail
// layout, replace DefaultValueOf() with a direct read of +0x10.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstring>

// ---- sibling impl__ exports called by the bodies in this file ----
// Signatures derived from the mangled names; definitions live in
// core/animation/CAnimationVariable.cpp, core/collections/CPlex.cpp and
// detail/MfcExceptionsSupport.cpp.
extern "C" void MS_ABI impl__AddTransition_CAnimationVariable__QEAAXPEAVCBaseTransition___Z(
    CAnimationVariable* pThis, void* pTransition);
extern "C" long MS_ABI impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(CAnimationVariable* pThis, double* pValue);
extern "C" void MS_ABI impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(CAnimationVariable* pThis, double value);
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
    CPlex** ppHead, unsigned long long nMax, unsigned long long cbElement);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

namespace {

struct S_CAnimationPoint {
    unsigned char  base[0x28];      // +0x000 CAnimationBaseObject
    unsigned char  m_xValue[0x60];  // +0x028 CAnimationVariable
    unsigned char  m_yValue[0x60];  // +0x088 CAnimationVariable
};
static_assert(offsetof(S_CAnimationPoint, m_xValue) == 0x028, "m_xValue");
static_assert(offsetof(S_CAnimationPoint, m_yValue) == 0x088, "m_yValue");
static_assert(sizeof(S_CAnimationPoint) == 0xe8, "sizeof(CAnimationPoint) == 232");

inline S_CAnimationPoint* Self(void* p) { return static_cast<S_CAnimationPoint*>(p); }
inline CAnimationVariable* Var(unsigned char* p) { return reinterpret_cast<CAnimationVariable*>(p); }

// Retail CBaseTransition prefix (afxanimationcontroller.h: CObject vfptr,
// m_type, CComPtr m_transition, m_pStartKeyframe, m_pEndKeyframe,
// m_pRelatedVariable, ...).  Same shape CAnimationRect.cpp pins.
struct S_CBaseTransition {
    const void* vfptr;              // +0x00
    std::int32_t m_type;            // +0x08
    std::int32_t pad0_;             // +0x0c
    void* m_transition;             // +0x10
    void* m_pStartKeyframe;         // +0x18
    void* m_pEndKeyframe;           // +0x20
    void* m_pRelatedVariable;       // +0x28 (retail: `cmpq $0x0,0x28(%rdx)`)
};
static_assert(offsetof(S_CBaseTransition, m_pRelatedVariable) == 0x28, "m_pRelatedVariable");

// CAnimationVariable::AddTransition as retail inlines it (export RVA 0x3e80
// mfc140u):
//   if (pTransition != NULL && pTransition->m_pRelatedVariable == NULL) {
//       pTransition->m_pRelatedVariable = this;
//       m_lstTransitions /*+0x18*/.AddTail(pTransition);   // CObList::AddTail
//   }
// DEVIATION: the AddTail goes through OpenMFC's AddTransition thunk, which
// appends to its side table rather than to m_lstTransitions at +0x18.
void VarAddTransition(unsigned char* pVar, void* pTransition) {
    if (pTransition == nullptr) return;
    S_CBaseTransition* t = static_cast<S_CBaseTransition*>(pTransition);
    if (t->m_pRelatedVariable != nullptr) return;
    t->m_pRelatedVariable = pVar;
    impl__AddTransition_CAnimationVariable__QEAAXPEAVCBaseTransition___Z(Var(pVar), pTransition);
}

// Retail CPoint / POINT: x +0, y +4.
struct S_Point { std::int32_t x, y; };
static_assert(sizeof(S_Point) == 8, "POINT");

// The default-value read retail inlines as `cvttsd2si m_dblDefaultValue`
// (+0x10 of the variable).  DEVIATION: goes through the GetValue(DOUBLE&)
// thunk -- see the file header for why and when that is equivalent.
inline int DefaultValueOf(unsigned char* pVar) {
    double d = 0.0;
    impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(Var(pVar), &d);
    return static_cast<int>(d);  // truncation, as cvttsd2si
}

// Retail-layout CList<CAnimationVariable*, CAnimationVariable*> and its CNode
// (same shape CAnimationBaseObject.cpp and CAnimationRect.cpp pin).
struct VarNode {
    VarNode* pNext;  // +0x00
    VarNode* pPrev;  // +0x08
    void*    data;   // +0x10  CAnimationVariable*
};
static_assert(sizeof(VarNode) == 0x18, "sizeof(CNode) == 24 (CPlex::Create cbElement)");
static_assert(offsetof(VarNode, data) == 0x10, "CNode::data");

struct RetailVarList {
    const void*   vfptr;        // +0x00
    VarNode*      m_pNodeHead;  // +0x08
    VarNode*      m_pNodeTail;  // +0x10
    std::intptr_t m_nCount;     // +0x18
    VarNode*      m_pNodeFree;  // +0x20
    CPlex*        m_pBlocks;    // +0x28
    std::intptr_t m_nBlockSize; // +0x30
};
static_assert(offsetof(RetailVarList, m_pNodeHead) == 0x08, "m_pNodeHead");
static_assert(offsetof(RetailVarList, m_pNodeTail) == 0x10, "m_pNodeTail");
static_assert(offsetof(RetailVarList, m_nCount) == 0x18, "m_nCount");
static_assert(offsetof(RetailVarList, m_pNodeFree) == 0x20, "m_pNodeFree");
static_assert(offsetof(RetailVarList, m_pBlocks) == 0x28, "m_pBlocks");
static_assert(offsetof(RetailVarList, m_nBlockSize) == 0x30, "m_nBlockSize");

// CList<CAnimationVariable*, CAnimationVariable*>::AddTail -- the unexported
// template instantiation at RVA 0x7908 (mfc140u) that GetAnimationVariableList
// calls, with NewNode inlined:
//   pOldTail = m_pNodeTail;
//   if (m_pNodeFree == NULL) {
//       pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CNode) /*0x18*/);
//       pNode = (CNode*)pNewBlock->data() + (m_nBlockSize - 1);
//       for (i = m_nBlockSize - 1; i >= 0; i--, pNode--) {
//           pNode->pNext = m_pNodeFree; m_pNodeFree = pNode; }
//   }
//   if (m_pNodeFree == NULL) AfxThrowInvalidArgException();   // ENSURE
//   pNode = m_pNodeFree; m_pNodeFree = pNode->pNext;
//   pNode->pPrev = pOldTail; pNode->pNext = NULL; m_nCount++; pNode->data = newElement;
//   if (m_pNodeTail != NULL) m_pNodeTail->pNext = pNode; else m_pNodeHead = pNode;
//   m_pNodeTail = pNode; return pNode;
// DEVIATION: retail's CPlex::Create (called at 0x7936 inside that AddTail;
// RVA 0x271300 mfc140u) uses its result unchecked; OpenMFC's Create
// (core/collections/CPlex.cpp) can return NULL instead, so a NULL block
// skips the free-chain build and falls through to the ENSURE throw.
void ListAddTail(RetailVarList* pList, void* newElement) {
    VarNode* pOldTail = pList->m_pNodeTail;
    if (pList->m_pNodeFree == nullptr) {
        const std::intptr_t n = pList->m_nBlockSize;
        CPlex* pNewBlock = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
            &pList->m_pBlocks, static_cast<unsigned long long>(n), sizeof(VarNode));
        if (pNewBlock != nullptr) {
            VarNode* pNode = reinterpret_cast<VarNode*>(
                reinterpret_cast<unsigned char*>(pNewBlock) + sizeof(void*)) + (n - 1);
            for (std::intptr_t i = n - 1; i >= 0; --i, --pNode) {
                pNode->pNext = pList->m_pNodeFree;
                pList->m_pNodeFree = pNode;
            }
        }
    }
    if (pList->m_pNodeFree == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    VarNode* pNode = pList->m_pNodeFree;
    pList->m_pNodeFree = pNode->pNext;
    pNode->pPrev = pOldTail;
    pNode->pNext = nullptr;
    pList->m_nCount++;
    pNode->data = newElement;
    if (pList->m_pNodeTail != nullptr) {
        pList->m_pNodeTail->pNext = pNode;
    } else {
        pList->m_pNodeHead = pNode;
    }
    pList->m_pNodeTail = pNode;
}

} // namespace

// Symbol: ??0CAnimationPoint@@QEAA@AEBVCPoint@@II_K@Z
extern "C" void* MS_ABI impl___0CAnimationPoint__QEAA_AEBVCPoint__II_K_Z(
    void* pThis, const void* /*point*/, unsigned int unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    return pThis;
}
// Symbol: ??0CAnimationPoint@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationPoint__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ?AddTransition@CAnimationPoint@@QEAAXPEAVCBaseTransition@@0@Z
extern "C" void MS_ABI impl__AddTransition_CAnimationPoint__QEAAXPEAVCBaseTransition__0_Z(
    void* pThis, void* pXTransition, void* pYTransition) {
    // Retail RVA 0x4cc0 (mfc140u): two inlined copies of
    // CAnimationVariable::AddTransition (RVA 0x3e80 mfc140u), in this order:
    //   m_xValue.AddTransition(pXTransition);   // +0x028
    //   m_yValue.AddTransition(pYTransition);   // +0x088
    // Each inlined copy is the NULL / already-related test, the
    // m_pRelatedVariable store and a CObList::AddTail (RVA 0x231e70 mfc140u)
    // on the variable's m_lstTransitions -- see VarAddTransition above.
    S_CAnimationPoint* self = Self(pThis);
    VarAddTransition(self->m_xValue, pXTransition);
    VarAddTransition(self->m_yValue, pYTransition);
}

// Symbol: ?GetAnimationVariableList@CAnimationPoint@@MEAAXAEAV?$CList@PEAVCAnimationVariable@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetAnimationVariableList_CAnimationPoint__MEAAXAEAV__CList_PEAVCAnimationVariable__PEAV1____Z(
    void* pThis, void* pList) {
    // Retail RVA 0x4d20 (mfc140u): two calls to the CList AddTail
    // instantiation (RVA 0x7908 mfc140u), the second a tail-jump:
    //   lst.AddTail(&m_xValue);   // +0x028
    //   lst.AddTail(&m_yValue);   // +0x088
    // The list is not cleared first and no pointer is NULL-checked.
    S_CAnimationPoint* self = Self(pThis);
    RetailVarList* lst = static_cast<RetailVarList*>(pList);
    ListAddTail(lst, self->m_xValue);
    ListAddTail(lst, self->m_yValue);
}

// Symbol: ?GetDefaultValue@CAnimationPoint@@QEAA?AVCPoint@@XZ
extern "C" void* MS_ABI impl__GetDefaultValue_CAnimationPoint__QEAA_AVCPoint__XZ(void* pThis, void* pRet) {
    // Retail RVA 0x4be0 (mfc140u).  Returns CPoint by value: this in RCX, the
    // hidden return buffer in RDX, which is also returned in RAX.
    //   pRet->x = (int)m_xValue.m_dblDefaultValue;   // cvttsd2si 0x38(%rcx)
    //   pRet->y = (int)m_yValue.m_dblDefaultValue;   // cvttsd2si 0x98(%rcx)
    //   return pRet;
    // DEVIATION: the default is read through DefaultValueOf() (file header).
    S_CAnimationPoint* self = Self(pThis);
    S_Point* pt = static_cast<S_Point*>(pRet);
    pt->x = DefaultValueOf(self->m_xValue);
    pt->y = DefaultValueOf(self->m_yValue);
    return pRet;
}

// Symbol: ?GetValue@CAnimationPoint@@QEAAHAEAVCPoint@@@Z
extern "C" int MS_ABI impl__GetValue_CAnimationPoint__QEAAHAEAVCPoint___Z(void* pThis, void* pPoint) {
    // Retail RVA 0x4c00 (mfc140u), with CAnimationVariable::GetValue(DOUBLE&)
    // (RVA 0x3e20 mfc140u) inlined at each use (one DOUBLE local in this
    // function's RCX home slot, zeroed on entry and not re-zeroed for y):
    //   DOUBLE dbl = 0.0;
    //   if (FAILED(m_xValue.GetValue(dbl))) goto fail;      // `jns` over the fail block
    //   point.x = (int)dbl;                                 // cvttsd2si, stored now
    //   if (FAILED(m_yValue.GetValue(dbl))) goto fail;      // `js 0x180004c4c`
    //   point.y = (int)dbl;
    //   return TRUE;
    // fail:                                                 // 0x180004c4c
    //   point.x = (int)m_xValue.m_dblDefaultValue;          // cvttsd2si (+0x38)
    //   point.y = (int)m_yValue.m_dblDefaultValue;          // cvttsd2si 0x98(%rdi)
    //   return FALSE;
    // So a y failure overwrites the x already stored.  Retail never writes
    // the defaults on the success path.  The IUIAnimationVariable call in the
    // inlined accessor is `call *0x1802c7b30`: that .rdata slot is the load
    // config's GuardCFDispatchFunctionPointer (mfc140u; iatu.py: not an
    // import), which points at the `jmp *%rax` stub at 0x1802b9d30 (mfc140u).
    // DEVIATION: the per-variable read goes through the GetValue(DOUBLE&)
    // thunk and the fail-path defaults through DefaultValueOf() (file header).
    S_CAnimationPoint* self = Self(pThis);
    S_Point* pt = static_cast<S_Point*>(pPoint);
    double dbl = 0.0;
    if (SUCCEEDED(impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(Var(self->m_xValue), &dbl))) {
        pt->x = static_cast<int>(dbl);
        if (SUCCEEDED(impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(Var(self->m_yValue), &dbl))) {
            pt->y = static_cast<int>(dbl);
            return TRUE;
        }
    }
    pt->x = DefaultValueOf(self->m_xValue);
    pt->y = DefaultValueOf(self->m_yValue);
    return FALSE;
}

// Symbol: ?SetDefaultValue@CAnimationPoint@@QEAAXAEBUtagPOINT@@@Z
extern "C" void MS_ABI impl__SetDefaultValue_CAnimationPoint__QEAAXAEBUtagPOINT___Z(void* pThis, const void* pPoint) {
    // Retail RVA 0x4ba0 (mfc140u; not in mfc140u_rva_symbols.json -- resolved
    // through the export table, ordinal 13118):
    //   m_xValue.SetDefaultValue((DOUBLE)pt.x);   // cvtdq2pd, call 0x180003df0
    //   m_yValue.SetDefaultValue((DOUBLE)pt.y);   // cvtdq2pd, tail-jump 0x180003df0
    // 0x3df0 is CAnimationVariable::SetDefaultValue(DOUBLE) (export ordinal 13121).
    S_CAnimationPoint* self = Self(pThis);
    const S_Point* pt = static_cast<const S_Point*>(pPoint);
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(Var(self->m_xValue), static_cast<double>(pt->x));
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(Var(self->m_yValue), static_cast<double>(pt->y));
}
