// CAnimationRect — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// AddTransition, GetAnimationVariableList, GetDefaultValue, GetValue and
// SetDefaultValue are transcribed from the retail exports (RVAs cited per
// function, mfc140u).  In mfc140.dll each sits 0x80 higher (e.g. GetValue
// 0x5cd0 (mfc140u) / 0x5d50 (mfc140)) with the same instructions; only
// RIP-relative displacements (IAT / CFG-dispatch slots) differ in encoding.
//
// RETAIL LAYOUT (afxanimationcontroller.h:1671, DECLARE_DYNAMIC, derives
// CAnimationBaseObject; m_nObjectSize 440 == 0x1b8 in
// core/animation/RuntimeClasses.cpp).  Offsets as the bodies below read them:
//   +0x000 CAnimationBaseObject (0x28 bytes; see CAnimationBaseObject.cpp)
//   +0x028 CAnimationVariable m_leftValue    (SetDefaultValue: `add $0x28,%rcx`)
//   +0x088 CAnimationVariable m_topValue     (`lea 0x88(%rdi),%rcx`)
//   +0x0e8 CAnimationVariable m_rightValue   (`lea 0xe8(%rdi),%rcx`)
//   +0x148 CAnimationVariable m_bottomValue  (`lea 0x148(%rdi),%rcx`)
//   +0x1a8 CSize m_szInitial                 (cx +0x1a8, cy +0x1ac)
//   +0x1b0 BOOL  m_bFixedSize                (GetValue: `cmpl $0x0,0x1b0(%rbx)`)
// so a retail CAnimationVariable is 0x60 bytes.  Within one, the rect bodies
// touch +0x08 m_variable (CComPtr<IUIAnimationVariable>) and +0x10
// m_dblDefaultValue, both only via code inlined from the variable's own
// accessors (see below).  include/openmfc does not declare CAnimationRect or
// CAnimationBaseObject, so the layout is pinned here on a file-local shadow.
//
// HOW THE EMBEDDED VARIABLES ARE REACHED.  Retail inlines two exported
// CAnimationVariable members into these bodies (register allocation differs,
// the logic is the export's), and calls a third out of line:
//   AddTransition(CBaseTransition*)  RVA 0x3e80 (mfc140u), inlined 4x into
//     AddTransition.  OpenMFC's thunk for it does NOT reproduce the export:
//     it neither tests nor sets CBaseTransition::m_pRelatedVariable, so that
//     part is transcribed inline below and only the list append is delegated.
//   GetValue(INT32&)                 RVA 0x3e50 (mfc140u), inlined into
//     GetValue; called here through its impl__ thunk.
//   SetDefaultValue(DOUBLE)          RVA 0x3df0 (mfc140u; not in
//     mfc140u_rva_symbols.json -- SetDefaultValue(CRect) below calls it out
//     of line, and its instructions match the mfc140 export at RVA 0x3e70
//     apart from the RIP-relative CFG-dispatch displacement); called here
//     through its impl__ thunk.
// Thunks live in core/animation/CAnimationVariable.cpp.
// The fourth accessor, CAnimationVariable::GetDefaultValue(), is inline in
// afxanimationcontroller.h (no export); retail reads m_dblDefaultValue at
// +0x10 with `cvttsd2si`.
//
// DEVIATION, file-wide: OpenMFC's CAnimationVariable does not keep its state
// at retail offsets -- the ctor / SetDefaultValue / GetValue thunks key a
// side table on the object's address (core/animation/CAnimationVariable.cpp),
// and +0x10 is never written.  Reading +0x10 as retail does would therefore
// return memory OpenMFC's SetDefaultValue never touched.  DefaultValueOf()
// below instead obtains the default through the GetValue(DOUBLE&) thunk.  That
// equals m_dblDefaultValue only because OpenMFC's variable never creates an
// IUIAnimationVariable (its Create returns TRUE without creating one), so
// its GetValue always takes the retail "m_variable == NULL -> return
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
extern "C" long MS_ABI impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(CAnimationVariable* pThis, int* pValue);
extern "C" long MS_ABI impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(CAnimationVariable* pThis, double* pValue);
extern "C" void MS_ABI impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(CAnimationVariable* pThis, double value);
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
    CPlex** ppHead, unsigned long long nMax, unsigned long long cbElement);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

namespace {

struct S_CAnimationRect {
    unsigned char  base[0x28];     // +0x000 CAnimationBaseObject
    unsigned char  m_leftValue[0x60];   // +0x028 CAnimationVariable
    unsigned char  m_topValue[0x60];    // +0x088
    unsigned char  m_rightValue[0x60];  // +0x0e8
    unsigned char  m_bottomValue[0x60]; // +0x148
    std::int32_t   m_szInitial_cx; // +0x1a8
    std::int32_t   m_szInitial_cy; // +0x1ac
    std::int32_t   m_bFixedSize;   // +0x1b0
    std::int32_t   pad_;           // +0x1b4 (alignment tail)
};
static_assert(offsetof(S_CAnimationRect, m_leftValue) == 0x028, "m_leftValue");
static_assert(offsetof(S_CAnimationRect, m_topValue) == 0x088, "m_topValue");
static_assert(offsetof(S_CAnimationRect, m_rightValue) == 0x0e8, "m_rightValue");
static_assert(offsetof(S_CAnimationRect, m_bottomValue) == 0x148, "m_bottomValue");
static_assert(offsetof(S_CAnimationRect, m_szInitial_cx) == 0x1a8, "m_szInitial.cx");
static_assert(offsetof(S_CAnimationRect, m_szInitial_cy) == 0x1ac, "m_szInitial.cy");
static_assert(offsetof(S_CAnimationRect, m_bFixedSize) == 0x1b0, "m_bFixedSize");
static_assert(sizeof(S_CAnimationRect) == 0x1b8, "sizeof(CAnimationRect) == 440");

inline S_CAnimationRect* Self(void* p) { return static_cast<S_CAnimationRect*>(p); }
inline CAnimationVariable* Var(unsigned char* p) { return reinterpret_cast<CAnimationVariable*>(p); }

// Retail CBaseTransition prefix (afxanimationcontroller.h: CObject vfptr,
// m_type, CComPtr m_transition, m_pStartKeyframe, m_pEndKeyframe,
// m_pRelatedVariable, m_bAdded).  Its ctors are inline in that header (none
// is exported), so every transition handed to AddTransition was built by
// client code at this layout, with m_pRelatedVariable initialised to NULL.
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

// Retail CRect / RECT: left +0, top +4, right +8, bottom +0xc.
struct S_Rect { std::int32_t left, top, right, bottom; };
static_assert(sizeof(S_Rect) == 16, "RECT");

// The default-value read retail inlines as `cvttsd2si m_dblDefaultValue`
// (+0x10 of the variable).  DEVIATION: goes through the GetValue(DOUBLE&)
// thunk -- see the file header for why and when that is equivalent.
inline int DefaultValueOf(unsigned char* pVar) {
    double d = 0.0;
    impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(Var(pVar), &d);
    return static_cast<int>(d);  // truncation, as cvttsd2si
}

// Retail-layout CList<CAnimationVariable*, CAnimationVariable*> and its CNode
// (same shape CAnimationBaseObject.cpp documents and pins).
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
// template instantiation at RVA 0x7908 (mfc140u) / 0x7988 (mfc140) that
// GetAnimationVariableList calls, with NewNode inlined:
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
// DEVIATION: retail's CPlex::Create (RVA 0x271300 mfc140u) throws
// AfxThrowInvalidArgException itself when nMax or cbElement is 0, allocates
// with a throwing operator new, and its result is used unchecked; OpenMFC's
// Create (core/collections/CPlex.cpp) can return NULL instead, so a NULL
// block skips the free-chain build and falls through to the ENSURE throw.
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

// Symbol: ??0CAnimationRect@@QEAA@AEBVCPoint@@AEBVCSize@@II_K@Z
extern "C" void* MS_ABI impl___0CAnimationRect__QEAA_AEBVCPoint__AEBVCSize__II_K_Z(
    void* pThis, const void* /*point*/, const void* /*size*/,
    unsigned int unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    return pThis;
}
// Symbol: ??0CAnimationRect@@QEAA@AEBVCRect@@II_K@Z
extern "C" void* MS_ABI impl___0CAnimationRect__QEAA_AEBVCRect__II_K_Z(
    void* pThis, const void* /*rect*/,
    unsigned int unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    return pThis;
}
// Symbol: ??0CAnimationRect@@QEAA@HHHHII_K@Z
extern "C" void* MS_ABI impl___0CAnimationRect__QEAA_HHHHII_K_Z(
    void* pThis, int unused0, int unused1, int unused2, int unused3,
    unsigned int unused4, unsigned long long unused5) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    (void)unused3;
    (void)unused4;
    (void)unused5;
    return pThis;
}
// Symbol: ??0CAnimationRect@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationRect__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ?AddTransition@CAnimationRect@@QEAAXPEAVCBaseTransition@@000@Z
extern "C" void MS_ABI impl__AddTransition_CAnimationRect__QEAAXPEAVCBaseTransition__000_Z(
    void* pThis, void* pLeftTransition, void* pTopTransition,
    void* pRightTransition, void* pBottomTransition) {
    // Retail RVA 0x5e50 (mfc140u): four inlined copies of
    // CAnimationVariable::AddTransition (RVA 0x3e80 mfc140u), in this order:
    //   m_leftValue.AddTransition(pLeftTransition);       // +0x028
    //   m_topValue.AddTransition(pTopTransition);         // +0x088
    //   m_rightValue.AddTransition(pRightTransition);     // +0x0e8
    //   m_bottomValue.AddTransition(pBottomTransition);   // +0x148 (4th arg, read from the stack)
    // Each inlined copy is the NULL / already-related test, the
    // m_pRelatedVariable store and a CObList::AddTail (RVA 0x231e70 mfc140u)
    // on the variable's m_lstTransitions -- see VarAddTransition above.
    S_CAnimationRect* self = Self(pThis);
    VarAddTransition(self->m_leftValue, pLeftTransition);
    VarAddTransition(self->m_topValue, pTopTransition);
    VarAddTransition(self->m_rightValue, pRightTransition);
    VarAddTransition(self->m_bottomValue, pBottomTransition);
}

// Symbol: ?GetAnimationVariableList@CAnimationRect@@MEAAXAEAV?$CList@PEAVCAnimationVariable@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetAnimationVariableList_CAnimationRect__MEAAXAEAV__CList_PEAVCAnimationVariable__PEAV1____Z(
    void* pThis, void* pList) {
    // Retail RVA 0x5f00 (mfc140u): four calls to the CList AddTail
    // instantiation (RVA 0x7908 mfc140u, the last one a tail-jump).  Note
    // the order is left, RIGHT, TOP, bottom -- not declaration order:
    //   lst.AddTail(&m_leftValue);    // +0x028
    //   lst.AddTail(&m_rightValue);   // +0x0e8
    //   lst.AddTail(&m_topValue);     // +0x088
    //   lst.AddTail(&m_bottomValue);  // +0x148
    // The list is not cleared first and no pointer is NULL-checked.
    S_CAnimationRect* self = Self(pThis);
    RetailVarList* lst = static_cast<RetailVarList*>(pList);
    ListAddTail(lst, self->m_leftValue);
    ListAddTail(lst, self->m_rightValue);
    ListAddTail(lst, self->m_topValue);
    ListAddTail(lst, self->m_bottomValue);
}

// Symbol: ?GetDefaultValue@CAnimationRect@@QEAA?AVCRect@@XZ
extern "C" void* MS_ABI impl__GetDefaultValue_CAnimationRect__QEAA_AVCRect__XZ(void* pThis, void* pRet) {
    // Retail RVA 0x5ca0 (mfc140u).  Returns CRect by value: this in RCX, the
    // hidden return buffer in RDX, which is also returned in RAX.
    //   pRet->left   = (int)m_leftValue.m_dblDefaultValue;    // cvttsd2si 0x38(%rcx)
    //   pRet->top    = (int)m_topValue.m_dblDefaultValue;     // 0x98
    //   pRet->right  = (int)m_rightValue.m_dblDefaultValue;   // 0xf8
    //   pRet->bottom = (int)m_bottomValue.m_dblDefaultValue;  // 0x158
    //   return pRet;
    // DEVIATION: the default is read through DefaultValueOf() (file header).
    S_CAnimationRect* self = Self(pThis);
    S_Rect* rc = static_cast<S_Rect*>(pRet);
    rc->left = DefaultValueOf(self->m_leftValue);
    rc->top = DefaultValueOf(self->m_topValue);
    rc->right = DefaultValueOf(self->m_rightValue);
    rc->bottom = DefaultValueOf(self->m_bottomValue);
    return pRet;
}

// Symbol: ?GetValue@CAnimationRect@@QEAAHAEAVCRect@@@Z
extern "C" int MS_ABI impl__GetValue_CAnimationRect__QEAAHAEAVCRect___Z(void* pThis, void* pRect) {
    // Retail RVA 0x5cd0 (mfc140u), with CAnimationVariable::GetValue(INT32&)
    // (RVA 0x3e50 mfc140u) inlined at each use:
    //   rect = GetDefaultValue();                  // all four written first, unconditionally
    //   INT32 nLeft = 0, nTop = 0, nRight = 0, nBottom = 0;
    //   if (FAILED(m_leftValue.GetValue(nLeft)))  return FALSE;   // `js` -> xor eax
    //   if (FAILED(m_topValue.GetValue(nTop)))    return FALSE;
    //   if (m_bFixedSize) {                                        // +0x1b0
    //       ::SetRect(&rect, nLeft, nTop,
    //                 nLeft + m_szInitial.cx, nTop + m_szInitial.cy);  // +0x1a8 / +0x1ac
    //       return TRUE;                                           // right/bottom not read
    //   }
    //   if (FAILED(m_rightValue.GetValue(nRight)))   return FALSE;
    //   if (FAILED(m_bottomValue.GetValue(nBottom))) return FALSE;
    //   ::SetRect(&rect, nLeft, nTop, nRight, nBottom);
    //   return TRUE;
    // A FALSE return leaves rect holding the defaults.  SetRect's IAT slot
    // resolved with iat.py (USER32!SetRect); its result is not consulted.
    // DEVIATION: the defaults come from DefaultValueOf() (file header).
    S_CAnimationRect* self = Self(pThis);
    RECT* rc = static_cast<RECT*>(pRect);
    impl__GetDefaultValue_CAnimationRect__QEAA_AVCRect__XZ(pThis, pRect);

    int nLeft = 0, nTop = 0, nRight = 0, nBottom = 0;
    if (FAILED(impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(Var(self->m_leftValue), &nLeft))) {
        return FALSE;
    }
    if (FAILED(impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(Var(self->m_topValue), &nTop))) {
        return FALSE;
    }
    if (self->m_bFixedSize != 0) {
        ::SetRect(rc, nLeft, nTop, nLeft + self->m_szInitial_cx, nTop + self->m_szInitial_cy);
        return TRUE;
    }
    if (FAILED(impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(Var(self->m_rightValue), &nRight))) {
        return FALSE;
    }
    if (FAILED(impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(Var(self->m_bottomValue), &nBottom))) {
        return FALSE;
    }
    ::SetRect(rc, nLeft, nTop, nRight, nBottom);
    return TRUE;
}

// Symbol: ?SetDefaultValue@CAnimationRect@@QEAAXAEBVCRect@@@Z
extern "C" void MS_ABI impl__SetDefaultValue_CAnimationRect__QEAAXAEBVCRect___Z(void* pThis, const void* pRect) {
    // Retail RVA 0x5c10 (mfc140u; not in mfc140u_rva_symbols.json --
    // identified by its instructions matching the mfc140 export at RVA 0x5c90):
    //   m_leftValue.SetDefaultValue((DOUBLE)rect.left);     // cvtdq2pd, out-of-line call
    //   m_topValue.SetDefaultValue((DOUBLE)rect.top);
    //   m_rightValue.SetDefaultValue((DOUBLE)rect.right);
    //   m_bottomValue.SetDefaultValue((DOUBLE)rect.bottom);
    //   m_szInitial.cx = rect.right - rect.left;             // +0x1a8
    //   m_szInitial.cy = rect.bottom - rect.top;             // +0x1ac
    // (retail computes cy first in registers but stores cx first.)
    S_CAnimationRect* self = Self(pThis);
    const S_Rect* rc = static_cast<const S_Rect*>(pRect);
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(Var(self->m_leftValue), static_cast<double>(rc->left));
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(Var(self->m_topValue), static_cast<double>(rc->top));
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(Var(self->m_rightValue), static_cast<double>(rc->right));
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(Var(self->m_bottomValue), static_cast<double>(rc->bottom));
    self->m_szInitial_cx = rc->right - rc->left;
    self->m_szInitial_cy = rc->bottom - rc->top;
}
