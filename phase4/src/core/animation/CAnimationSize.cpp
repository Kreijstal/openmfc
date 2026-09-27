// CAnimationSize — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// AddTransition, GetAnimationVariableList, GetDefaultValue, GetValue and
// SetDefaultValue are transcribed from the retail exports.
//
// WHERE THE RETAIL BODIES ARE.  In BOTH retail images the linker folded these
// five bodies with the byte-identical CAnimationPoint ones; each RVA below
// carries two export ordinals (CAnimationPoint's and CAnimationSize's), read
// straight from each image's export address table.  The RVA maps are keyed
// by RVA and keep one name per RVA, which is why mfc140u_rva_symbols.json
// names these RVAs only as CAnimationPoint (or not at all) and
// mfc140_rva_symbols.json names them only as CAnimationSize.
//   mfc140u (ordinals 2023 / 4898 / 5205 / 7483 / 13120):
//     AddTransition 0x4cc0, GetAnimationVariableList 0x4d20,
//     GetDefaultValue 0x4be0, GetValue 0x4c00, SetDefaultValue 0x4ba0.
//   mfc140 (ANSI twin): 0x4d40, 0x4da0, 0x4c60, 0x4c80, 0x4c20 respectively
//     (e.g. 0x4d40 carries ordinals 2014 and 2016; 0x4c20 carries 13058 and
//     13060).
// (pefile's parsed symbol list stops at ordinal ~9566; the SetDefaultValue
// ordinals above were read from the EAT directly, which has 14109 entries.)
// This file does not rely on the folding and does not call CAnimationPoint.
//
// RETAIL LAYOUT (afxanimationcontroller.h, DECLARE_DYNAMIC, derives
// CAnimationBaseObject; m_nObjectSize 232 == 0xe8 in
// core/animation/RuntimeClasses.cpp).  Offsets as the bodies below read them:
//   +0x000 CAnimationBaseObject (0x28 bytes; see CAnimationBaseObject.cpp)
//   +0x028 CAnimationVariable m_cxValue   (SetDefaultValue: `add $0x28,%rcx`)
//   +0x088 CAnimationVariable m_cyValue   (`lea 0x88(%rdi),%rcx`)
// The default ctor (RVA 0x4d60 mfc140u) writes nothing past +0xe3 (its
// highest store is the dword `mov %r9d,0xe0(%rcx)`), consistent
// with two 0x60-byte CAnimationVariable members and sizeof 0xe8.  Within a
// variable the bodies touch +0x08 m_variable (CComPtr<IUIAnimationVariable>)
// and +0x10 m_dblDefaultValue.  include/openmfc does not declare
// CAnimationSize or CAnimationBaseObject, so the layout is pinned here on a
// file-local shadow.
//
// HOW THE EMBEDDED VARIABLES ARE REACHED.  Retail inlines two exported
// CAnimationVariable members and calls a third out of line:
//   AddTransition(CBaseTransition*)  RVA 0x3e80 (mfc140u), inlined twice into
//     AddTransition.  OpenMFC's thunk for it does NOT reproduce the export:
//     it neither tests nor sets CBaseTransition::m_pRelatedVariable, so that
//     part is transcribed inline below and only the list append is delegated.
//   GetValue(DOUBLE&)                RVA 0x3e20 (mfc140u), inlined twice into
//     GetValue (NOT GetValue(INT32&) -- the inlined COM call is vtable byte
//     offset 0x18, slot 3, IUIAnimationVariable::GetValue(DOUBLE*), and the
//     result is truncated with cvttsd2si; GetValue(INT32&) would use slot 6,
//     GetIntegerValue).  Called here through its impl__ thunk.
//   SetDefaultValue(DOUBLE)          RVA 0x3df0 (mfc140u; EAT ordinal 13121,
//     not in mfc140u_rva_symbols.json), called out of line
//     by SetDefaultValue(const CSize&); called here through its impl__ thunk.
// Thunks live in core/animation/CAnimationVariable.cpp.
//
// DEVIATION, file-wide (same as CAnimationRect.cpp): OpenMFC's
// CAnimationVariable does not keep its state at retail offsets -- the ctor /
// SetDefaultValue / GetValue thunks key a side table on the object's address
// (core/animation/CAnimationVariable.cpp), and +0x10 is never written.
// Reading +0x10 as retail does would return memory OpenMFC's SetDefaultValue
// never touched.  DefaultValueOf() below instead obtains the default through
// the GetValue(DOUBLE&) thunk.  That equals m_dblDefaultValue only because
// OpenMFC's variable never creates an IUIAnimationVariable (its Create
// returns TRUE without creating one), so its GetValue always takes the retail
// "m_variable == NULL -> return m_dblDefaultValue" path.  If
// CAnimationVariable ever moves to retail layout, replace DefaultValueOf()
// with a direct read of +0x10.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

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

struct S_CAnimationSize {
    unsigned char base[0x28];        // +0x000 CAnimationBaseObject
    unsigned char m_cxValue[0x60];   // +0x028 CAnimationVariable
    unsigned char m_cyValue[0x60];   // +0x088 CAnimationVariable
};
static_assert(offsetof(S_CAnimationSize, m_cxValue) == 0x028, "m_cxValue");
static_assert(offsetof(S_CAnimationSize, m_cyValue) == 0x088, "m_cyValue");
static_assert(sizeof(S_CAnimationSize) == 0xe8, "sizeof(CAnimationSize) == 232");

inline S_CAnimationSize* Self(void* p) { return static_cast<S_CAnimationSize*>(p); }
inline CAnimationVariable* Var(unsigned char* p) { return reinterpret_cast<CAnimationVariable*>(p); }

// Retail CSize / SIZE: cx +0, cy +4 (GetDefaultValue: `mov %eax,(%rdx)`,
// `mov %eax,0x4(%rdx)`).
struct S_Size { std::int32_t cx, cy; };
static_assert(sizeof(S_Size) == 8, "SIZE");

// Retail CBaseTransition prefix (same shape CAnimationRect.cpp pins).  Its
// ctors are inline in afxanimationcontroller.h (none is exported), so every
// transition handed to AddTransition was built by client code at this layout.
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
//       m_lstTransitions /*+0x18*/.AddTail(pTransition);
//   }
// The AddTail is an out-of-line call to RVA 0x231e70 (mfc140u), which carries
// both CObList::AddTail(CObject*) (ordinal 1983) and the folded
// CPtrList::AddTail(void*) (ordinal 1985); m_lstTransitions is declared
// CObList in afxanimationcontroller.h.
// DEVIATION: the AddTail goes through OpenMFC's AddTransition thunk, which
// appends to its side table rather than to m_lstTransitions at +0x18.
void VarAddTransition(unsigned char* pVar, void* pTransition) {
    if (pTransition == nullptr) return;
    S_CBaseTransition* t = static_cast<S_CBaseTransition*>(pTransition);
    if (t->m_pRelatedVariable != nullptr) return;
    t->m_pRelatedVariable = pVar;
    impl__AddTransition_CAnimationVariable__QEAAXPEAVCBaseTransition___Z(Var(pVar), pTransition);
}

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
// calls.  Transcription copied from CAnimationRect.cpp's ListAddTail (same
// callee); see the DEVIATION note there about OpenMFC's CPlex::Create being
// able to return NULL where retail's throws.
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

// Symbol: ??0CAnimationSize@@QEAA@AEBVCSize@@II_K@Z
extern "C" void* MS_ABI impl___0CAnimationSize__QEAA_AEBVCSize__II_K_Z(
    void* pThis, const void* /*size*/,
    unsigned int unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    return pThis;
}
// Symbol: ??0CAnimationSize@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationSize__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ?AddTransition@CAnimationSize@@QEAAXPEAVCBaseTransition@@0@Z
extern "C" void MS_ABI impl__AddTransition_CAnimationSize__QEAAXPEAVCBaseTransition__0_Z(
    void* pThis, void* pCXTransition, void* pCYTransition) {
    // Retail RVA 0x4cc0 (mfc140u; folded with CAnimationPoint, see header):
    // two inlined copies of CAnimationVariable::AddTransition, in this order:
    //   m_cxValue.AddTransition(pCXTransition);   // +0x028, RDX
    //   m_cyValue.AddTransition(pCYTransition);   // +0x088, R8
    // Each is the NULL / already-related test, the m_pRelatedVariable store
    // and the AddTail call -- see VarAddTransition above.
    S_CAnimationSize* self = Self(pThis);
    VarAddTransition(self->m_cxValue, pCXTransition);
    VarAddTransition(self->m_cyValue, pCYTransition);
}

// Symbol: ?GetAnimationVariableList@CAnimationSize@@MEAAXAEAV?$CList@PEAVCAnimationVariable@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetAnimationVariableList_CAnimationSize__MEAAXAEAV__CList_PEAVCAnimationVariable__PEAV1____Z(
    void* pThis, void* pList) {
    // Retail RVA 0x4d20 (mfc140u; folded with CAnimationPoint): two calls to
    // the CList AddTail instantiation (RVA 0x7908 mfc140u, the second a
    // tail-jump):
    //   lst.AddTail(&m_cxValue);   // +0x028
    //   lst.AddTail(&m_cyValue);   // +0x088
    // The list is not cleared first and no pointer is NULL-checked.
    S_CAnimationSize* self = Self(pThis);
    RetailVarList* lst = static_cast<RetailVarList*>(pList);
    ListAddTail(lst, self->m_cxValue);
    ListAddTail(lst, self->m_cyValue);
}

// Symbol: ?GetDefaultValue@CAnimationSize@@QEAA?AVCSize@@XZ
extern "C" void* MS_ABI impl__GetDefaultValue_CAnimationSize__QEAA_AVCSize__XZ(void* pThis, void* pRet) {
    // Retail RVA 0x4be0 (mfc140u; folded with CAnimationPoint).  Returns CSize
    // by value through a hidden buffer: this in RCX, the buffer in RDX, which
    // is also returned in RAX.
    //   pRet->cx = (int)m_cxValue.m_dblDefaultValue;   // cvttsd2si 0x38(%rcx)
    //   pRet->cy = (int)m_cyValue.m_dblDefaultValue;   // cvttsd2si 0x98(%rcx)
    //   return pRet;
    // DEVIATION: the default is read through DefaultValueOf() (file header).
    S_CAnimationSize* self = Self(pThis);
    S_Size* sz = static_cast<S_Size*>(pRet);
    sz->cx = DefaultValueOf(self->m_cxValue);
    sz->cy = DefaultValueOf(self->m_cyValue);
    return pRet;
}

// Symbol: ?GetValue@CAnimationSize@@QEAAHAEAVCSize@@@Z
extern "C" int MS_ABI impl__GetValue_CAnimationSize__QEAAHAEAVCSize___Z(void* pThis, void* pSize) {
    // Retail RVA 0x4c00 (mfc140u; folded with CAnimationPoint), with
    // CAnimationVariable::GetValue(DOUBLE&) (RVA 0x3e20 mfc140u) inlined at
    // each use.  Unlike CAnimationRect::GetValue, the defaults are NOT
    // written up front; they are written only on the failure path:
    //   DOUBLE dbl = 0.0;
    //   if (FAILED(m_cxValue.GetValue(dbl))) goto fail;   // `jns` skips fail
    //   size.cx = (int)dbl;                               // cvttsd2si, stored
    //   if (FAILED(m_cyValue.GetValue(dbl))) goto fail;   // `js` back to fail
    //   size.cy = (int)dbl;
    //   return TRUE;
    // fail:
    //   size.cx = (int)m_cxValue.m_dblDefaultValue;       // cvttsd2si (%rsi) = +0x38
    //   size.cy = (int)m_cyValue.m_dblDefaultValue;       // cvttsd2si 0x98(%rdi)
    //   return FALSE;
    // So a failure on cy also overwrites the cx already stored.  The COM call
    // (m_variable->GetValue, vtable byte offset 0x18) goes through the CFG
    // dispatch pointer, not an import.
    // DEVIATION: the variable reads go through the GetValue(DOUBLE&) thunk and
    // the defaults through DefaultValueOf() (file header).
    S_CAnimationSize* self = Self(pThis);
    S_Size* sz = static_cast<S_Size*>(pSize);
    double dbl = 0.0;
    if (SUCCEEDED(impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(Var(self->m_cxValue), &dbl))) {
        sz->cx = static_cast<int>(dbl);
        if (SUCCEEDED(impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(Var(self->m_cyValue), &dbl))) {
            sz->cy = static_cast<int>(dbl);
            return TRUE;
        }
    }
    sz->cx = DefaultValueOf(self->m_cxValue);
    sz->cy = DefaultValueOf(self->m_cyValue);
    return FALSE;
}

// Symbol: ?SetDefaultValue@CAnimationSize@@QEAAXAEBVCSize@@@Z
extern "C" void MS_ABI impl__SetDefaultValue_CAnimationSize__QEAAXAEBVCSize___Z(void* pThis, const void* pSize) {
    // Retail RVA 0x4ba0 (mfc140u; EAT ordinal 13120, folded with
    // CAnimationPoint's ordinal 13118; not in mfc140u_rva_symbols.json):
    //   m_cxValue.SetDefaultValue((DOUBLE)size.cx);   // cvtdq2pd, out-of-line call
    //   m_cyValue.SetDefaultValue((DOUBLE)size.cy);   // cvtdq2pd, tail-jump
    // Nothing else is stored (CAnimationSize has no m_szInitial analogue).
    S_CAnimationSize* self = Self(pThis);
    const S_Size* sz = static_cast<const S_Size*>(pSize);
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(Var(self->m_cxValue), static_cast<double>(sz->cx));
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(Var(self->m_cyValue), static_cast<double>(sz->cy));
}
