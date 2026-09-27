// CAnimationColor — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// AddTransition, GetAnimationVariableList, GetDefaultValue, GetValue and
// SetDefaultValue are transcribed from the retail bodies (RVAs cited per
// function, mfc140u).  Each has the same instructions in mfc140.dll 0x80
// higher (e.g. GetValue 0x5280 (mfc140u) / 0x5300 (mfc140)); only
// RIP-relative and call displacements differ in encoding.
//
// RETAIL LAYOUT (afxanimationcontroller.h:1499, DECLARE_DYNAMIC, derives
// CAnimationBaseObject; m_nObjectSize 328 == 0x148 in
// core/animation/RuntimeClasses.cpp).  Offsets as the bodies below read them:
//   +0x000 CAnimationBaseObject (0x28 bytes; see CAnimationBaseObject.cpp)
//   +0x028 CAnimationVariable m_rValue   (AddTransition: `add $0x28,%rcx`)
//   +0x088 CAnimationVariable m_gValue   (`lea 0x88(%rsi),%rcx`)
//   +0x0e8 CAnimationVariable m_bValue   (`lea 0xe8(%rsi),%rcx`)
// three 0x60-byte variables ending exactly at 0x148, so there is no member
// after m_bValue.  Which variable is which channel is fixed by
// GetDefaultValue: +0x38 (= +0x28 variable's m_dblDefaultValue) lands in
// bits 0-7 (red), +0x98 in bits 8-15 (green), +0xf8 in bits 16-23 (blue).
// Within a variable the bodies touch +0x08 m_variable (CComPtr
// <IUIAnimationVariable>), +0x10 m_dblDefaultValue and +0x18
// m_lstTransitions, all via code inlined from the variable's own accessors.
// include/openmfc does not declare CAnimationColor or CAnimationBaseObject,
// so the layout is pinned here on a file-local shadow.
//
// HOW THE EMBEDDED VARIABLES ARE REACHED -- same scheme as
// core/animation/CAnimationRect.cpp:
//   CAnimationVariable::AddTransition(CBaseTransition*)  RVA 0x3e80 (mfc140u),
//     inlined 3x into AddTransition.  OpenMFC's thunk does not test or set
//     CBaseTransition::m_pRelatedVariable, so that part is transcribed inline
//     below and only the list append is delegated to the thunk.
//   CAnimationVariable::GetValue(INT32&)  RVA 0x3e50 (mfc140u), inlined 3x
//     into GetValue: `m_variable == NULL ? (v = (int)m_dblDefaultValue, S_OK)
//     : m_variable->vtbl[0x30/8 = 6](&v)` (IUIAnimationVariable::
//     GetIntegerValue, dispatched through the CFG pointer at mfc140u VA
//     0x1802c7b30).  Called here through its impl__ thunk.
//   CAnimationVariable::SetDefaultValue(DOUBLE)  RVA 0x3df0 (mfc140u; that
//     address is not in mfc140u_rva_symbols.json -- identified by matching
//     the mfc140 export at RVA 0x3e70), called out of line by
//     SetDefaultValue; called here through its impl__ thunk.
// Thunks live in core/animation/CAnimationVariable.cpp.
//
// DEVIATION, file-wide: OpenMFC's CAnimationVariable does not keep its state
// at retail offsets -- its ctor / SetDefaultValue / GetValue thunks key a side
// table on the object's address, and +0x10 is never written.  Retail's
// inlined `cvttsd2si m_dblDefaultValue` reads are therefore replaced by
// DefaultValueOf(), which goes through the GetValue(DOUBLE&) thunk.  That
// equals m_dblDefaultValue only because OpenMFC's variable never creates an
// IUIAnimationVariable (its Create returns TRUE without creating one), so its
// GetValue always takes the retail "m_variable == NULL -> return
// m_dblDefaultValue" path.  If CAnimationVariable ever moves to retail
// layout, replace DefaultValueOf() with a direct read of +0x10.
//
// The two constructors below are NOT on this pass's assignment and remain
// no-op stubs (they do not construct the base or the three variables).
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

// ---- sibling impl__ exports called by the bodies in this file ----
// Signatures derived from the mangled names; definitions live in
// core/animation/CAnimationVariable.cpp, core/collections/CPlex.cpp and
// detail/MfcExceptionsSupport.cpp.
extern "C" void MS_ABI impl__AddTransition_CAnimationVariable__QEAAXPEAVCBaseTransition___Z(
    CAnimationVariable* pThis, void* pTransition);
extern "C" long MS_ABI impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(CAnimationVariable* pThis, int* pValue);
extern "C" long MS_ABI impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(CAnimationVariable* pThis, double* pValue);
extern "C" void MS_ABI impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(CAnimationVariable* pThis, double value);
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
    CPlex** ppHead, unsigned long long nMax, unsigned long long cbElement);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

namespace {

struct S_CAnimationColor {
    unsigned char base[0x28];       // +0x000 CAnimationBaseObject
    unsigned char m_rValue[0x60];   // +0x028 CAnimationVariable
    unsigned char m_gValue[0x60];   // +0x088
    unsigned char m_bValue[0x60];   // +0x0e8
};
static_assert(offsetof(S_CAnimationColor, m_rValue) == 0x028, "m_rValue");
static_assert(offsetof(S_CAnimationColor, m_gValue) == 0x088, "m_gValue");
static_assert(offsetof(S_CAnimationColor, m_bValue) == 0x0e8, "m_bValue");
static_assert(sizeof(S_CAnimationColor) == 0x148, "sizeof(CAnimationColor) == 328");

inline S_CAnimationColor* Self(void* p) { return static_cast<S_CAnimationColor*>(p); }
inline CAnimationVariable* Var(unsigned char* p) { return reinterpret_cast<CAnimationVariable*>(p); }

// Retail CBaseTransition prefix (afxanimationcontroller.h: CObject vfptr,
// m_type, CComPtr m_transition, m_pStartKeyframe, m_pEndKeyframe,
// m_pRelatedVariable).  Same shadow as CAnimationRect.cpp.
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
//       m_lstTransitions /*+0x18*/.AddTail(pTransition);  // CObList::AddTail, RVA 0x231e70 (mfc140u)
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

// The default-value read retail inlines as `cvttsd2si m_dblDefaultValue`
// (+0x10 of the variable).  DEVIATION: goes through the GetValue(DOUBLE&)
// thunk -- see the file header for why and when that is equivalent.
inline int DefaultValueOf(unsigned char* pVar) {
    double d = 0.0;
    impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(Var(pVar), &d);
    return static_cast<int>(d);  // truncation, as cvttsd2si
}

// RGB(r, g, b) with each channel narrowed by BYTE cast, as retail's
// `movzbl` + `shl $0x8` / `shl $0x10` + `or` sequences do.
inline unsigned long MakeRgb(int r, int g, int b) {
    return static_cast<unsigned long>(static_cast<unsigned char>(r))
         | (static_cast<unsigned long>(static_cast<unsigned char>(g)) << 8)
         | (static_cast<unsigned long>(static_cast<unsigned char>(b)) << 16);
}

// Retail-layout CList<CAnimationVariable*, CAnimationVariable*> and its CNode
// (same shape CAnimationBaseObject.cpp / CAnimationRect.cpp pin).
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
// calls, with NewNode inlined.  Transcription copied from CAnimationRect.cpp,
// which decoded it:
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
// DEVIATION: retail's CPlex::Create throws on failure and its result is used
// unchecked; OpenMFC's Create (core/collections/CPlex.cpp) can return NULL,
// so a NULL block skips the free-chain build and falls through to the ENSURE
// throw.
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

// Symbol: ??0CAnimationColor@@QEAA@KII_K@Z
extern "C" void* MS_ABI impl___0CAnimationColor__QEAA_KII_K_Z(
    void* pThis, unsigned long long unused0, unsigned int unused1, unsigned int unused2, unsigned long long unused3) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    (void)unused3;
    return pThis;
}
// Symbol: ??0CAnimationColor@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationColor__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ?AddTransition@CAnimationColor@@QEAAXPEAVCBaseTransition@@00@Z
extern "C" void MS_ABI impl__AddTransition_CAnimationColor__QEAAXPEAVCBaseTransition__00_Z(
    void* pThis, void* pRTransition, void* pGTransition, void* pBTransition) {
    // Retail RVA 0x53a0 (mfc140u): three inlined copies of
    // CAnimationVariable::AddTransition (RVA 0x3e80 mfc140u), in this order:
    //   m_rValue.AddTransition(pRTransition);   // +0x028 (RDX)
    //   m_gValue.AddTransition(pGTransition);   // +0x088 (R8)
    //   m_bValue.AddTransition(pBTransition);   // +0x0e8 (R9)
    // Each is the NULL / already-related test, the m_pRelatedVariable store
    // and a call to CObList::AddTail (RVA 0x231e70 mfc140u) on the variable's
    // m_lstTransitions -- see VarAddTransition above.
    S_CAnimationColor* self = Self(pThis);
    VarAddTransition(self->m_rValue, pRTransition);
    VarAddTransition(self->m_gValue, pGTransition);
    VarAddTransition(self->m_bValue, pBTransition);
}

// Symbol: ?GetAnimationVariableList@CAnimationColor@@MEAAXAEAV?$CList@PEAVCAnimationVariable@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetAnimationVariableList_CAnimationColor__MEAAXAEAV__CList_PEAVCAnimationVariable__PEAV1____Z(
    void* pThis, void* pList) {
    // Retail RVA 0x5430 (mfc140u): three calls to the CList AddTail
    // instantiation (RVA 0x7908 mfc140u; the last is a tail-jump), in
    // declaration order:
    //   lst.AddTail(&m_rValue);   // +0x028
    //   lst.AddTail(&m_gValue);   // +0x088
    //   lst.AddTail(&m_bValue);   // +0x0e8
    // The list is not cleared first and no pointer is NULL-checked.
    S_CAnimationColor* self = Self(pThis);
    RetailVarList* lst = static_cast<RetailVarList*>(pList);
    ListAddTail(lst, self->m_rValue);
    ListAddTail(lst, self->m_gValue);
    ListAddTail(lst, self->m_bValue);
}

// Symbol: ?GetDefaultValue@CAnimationColor@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__GetDefaultValue_CAnimationColor__QEAAKXZ(void* pThis) {
    // Retail RVA 0x5250 (mfc140u), no calls:
    //   return RGB((BYTE)(int)m_rValue.m_dblDefaultValue,    // cvttsd2si 0x38(%rcx)
    //              (BYTE)(int)m_gValue.m_dblDefaultValue,    // 0x98
    //              (BYTE)(int)m_bValue.m_dblDefaultValue);   // 0xf8
    // DEVIATION: the defaults are read through DefaultValueOf() (file header).
    S_CAnimationColor* self = Self(pThis);
    return MakeRgb(DefaultValueOf(self->m_rValue),
                   DefaultValueOf(self->m_gValue),
                   DefaultValueOf(self->m_bValue));
}

// Symbol: ?GetValue@CAnimationColor@@QEAAHAEAK@Z
extern "C" int MS_ABI impl__GetValue_CAnimationColor__QEAAHAEAK_Z(void* pThis, unsigned long* pColor) {
    // Retail RVA 0x5280 (mfc140u), with CAnimationVariable::GetValue(INT32&)
    // (RVA 0x3e50 mfc140u) inlined at each use:
    //   INT32 nR = 0, nG = 0, nB = 0;
    //   color = GetDefaultValue();                 // written first, unconditionally
    //   if (FAILED(m_rValue.GetValue(nR))) return FALSE;   // `js` -> xor eax
    //   if (FAILED(m_gValue.GetValue(nG))) return FALSE;
    //   if (FAILED(m_bValue.GetValue(nB))) return FALSE;
    //   color = RGB((BYTE)nR, (BYTE)nG, (BYTE)nB);
    //   return TRUE;
    // A FALSE return leaves color holding the defaults.  pColor is not
    // NULL-checked.
    // DEVIATION: the defaults come from DefaultValueOf() (file header).
    S_CAnimationColor* self = Self(pThis);
    *pColor = impl__GetDefaultValue_CAnimationColor__QEAAKXZ(pThis);

    int nR = 0, nG = 0, nB = 0;
    if (FAILED(impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(Var(self->m_rValue), &nR))) {
        return FALSE;
    }
    if (FAILED(impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(Var(self->m_gValue), &nG))) {
        return FALSE;
    }
    if (FAILED(impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(Var(self->m_bValue), &nB))) {
        return FALSE;
    }
    *pColor = MakeRgb(nR, nG, nB);
    return TRUE;
}

// Symbol: ?SetDefaultValue@CAnimationColor@@QEAAXK@Z
extern "C" void MS_ABI impl__SetDefaultValue_CAnimationColor__QEAAXK_Z(void* pThis, unsigned long color) {
    // Retail RVA 0x51e0 (mfc140u; not in mfc140u_rva_symbols.json --
    // identified by its instructions matching the mfc140 export at RVA
    // 0x5260).  Three out-of-line calls to CAnimationVariable::
    // SetDefaultValue(DOUBLE) (RVA 0x3df0 mfc140u; the last a tail-jump):
    //   m_rValue.SetDefaultValue((DOUBLE)GetRValue(color));   // movzbl %dl
    //   m_gValue.SetDefaultValue((DOUBLE)GetGValue(color));   // (WORD)color >> 8
    //   m_bValue.SetDefaultValue((DOUBLE)GetBValue(color));   // (BYTE)(color >> 16)
    S_CAnimationColor* self = Self(pThis);
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(
        Var(self->m_rValue), static_cast<double>(color & 0xffu));
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(
        Var(self->m_gValue), static_cast<double>((color & 0xffffu) >> 8));
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(
        Var(self->m_bValue), static_cast<double>((color >> 16) & 0xffu));
}
