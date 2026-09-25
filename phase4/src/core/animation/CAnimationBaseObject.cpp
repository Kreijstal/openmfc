// CAnimationBaseObject — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The bodies of the destructor, SetID, CreateTransitions, ApplyTransitions,
// ClearTransitions, EnableValueChangedEvent, EnableIntegerValueChangedEvent,
// ContainsVariable and SetParentAnimationObjects are transcribed from the
// retail exports (RVAs cited per function, mfc140u; the bodies are
// byte-identical in mfc140.dll, where each sits 0x80 higher).
// SetID and SetParentAnimationObjects have no entry in
// mfc140u_rva_symbols.json.  SetParentAnimationObjects' mfc140u entry 0x4720
// is slot 11 of the CAnimationBaseObject vftable at RVA 0x2dad70 (mfc140u);
// SetID's entry 0x42c0 (mfc140u) is identified only by its bytes matching the
// mfc140 export ?SetID@CAnimationBaseObject@@QEAAXII@Z (RVA 0x4340, mfc140).
//
// RETAIL LAYOUT (afxanimationcontroller.h:749, DECLARE_DYNAMIC, derives
// CObject; m_nObjectSize 40 in core/animation/RuntimeClasses.cpp).  The
// offsets below are the ones every body in this file reads:
//   +0x00 vfptr
//   +0x08 UINT32 m_nGroupID                 (SetID: `mov %r8d,0x8(%rcx)`)
//   +0x0c UINT32 m_nObjectID                (SetID: `mov %edx,0xc(%rcx)`)
//   +0x10 DWORD_PTR m_dwUserData            (declaration order; no body here touches it)
//   +0x18 BOOL m_bAutodestroyTransitions    (ClearTransitions: `cmp %esi,0x18(%rdi)`)
//   +0x20 CAnimationController* m_pParentController (dtor / SetID / Apply / Create)
// include/openmfc does not declare CAnimationBaseObject, so the layout is
// pinned here with static_asserts on a file-local shadow struct.
//
// Virtual slots (mfc140u vftable at RVA 0x2dad70): CObject's five (0..4), then
// 5 ApplyTransitions, 6 ClearTransitions, 7 EnableValueChangedEvent,
// 8 EnableIntegerValueChangedEvent, 9 ContainsVariable,
// 10 GetAnimationVariableList (pure virtual: entry 0x2b8170 (mfc140u) is a
// jmp through the VCRUNTIME140!_purecall import), 11 SetParentAnimationObjects.
// Every list-walking body below calls slot 10
// (`mov (%rcx),%rax; mov 0x50(%rax),%rax`, then the CFG dispatch call).
//
// The list those bodies build.  Retail constructs a stack
// CList<CAnimationVariable*, CAnimationVariable*> (afxtempl.h: derives CObject)
// and passes it by reference to slot 10.  Stores observed in every caller:
//   +0x00 vfptr = the CList<CAnimationVariable*,...> vftable
//   +0x08 m_pNodeHead = 0, +0x10 m_pNodeTail = 0, +0x18 m_nCount = 0,
//   +0x20 m_pNodeFree = 0, +0x28 m_pBlocks = 0, +0x30 m_nBlockSize = 10
//         (the 16-byte constant {0, 10} stored over +0x28..+0x37).
// It then walks node->pNext (+0) / node->data (+0x10) from m_pNodeHead, and
// destroys the list by calling an unexported ~CList helper (RVA 0x79b4
// mfc140u / 0x7a34 mfc140) that re-stores the vfptr, zeroes m_nCount /
// m_pNodeFree / m_pNodeTail / m_pNodeHead, frees each m_pBlocks block with an
// inlined FreeDataChain loop (CRT `free`, IAT slot resolved with iat.py), and
// zeroes m_pBlocks.
//
// Retail CAnimationVariable offsets read/written through the list
// (afxanimationcontroller.h:513; the constructor ??0CAnimationVariable@@QEAA@N@Z,
// RVA 0x3c30 mfc140u, stores +0x8, +0x10, +0x18..+0x48, +0x50, +0x58):
//   +0x08 ATL::CComPtr<IUIAnimationVariable> m_variable
//   +0x50 CAnimationBaseObject* m_pParentObject
//
// DEVIATIONS, file-wide:
//  * The stack list's vfptr is left NULL.  OpenMFC has no MSVC-layout
//    CList<CAnimationVariable*, CAnimationVariable*> vftable to store (OpenMFC's
//    own CList template in include/openmfc/afx.h has no vfptr and a different
//    field order, so it cannot stand in).  A retail-built override filling the
//    list with AddTail/AddHead runs the afxtempl.h inline code, whose only
//    virtual call is ASSERT_VALID(this) (compiled out of release builds);
//    NewNode touches only non-vfptr members (m_pNodeFree, m_pBlocks,
//    m_nBlockSize, m_nCount) and calls the exported CPlex::Create.
//  * The block chain is released with the exported CPlex::FreeDataChain thunk
//    instead of retail's inlined CRT `free` loop: blocks in this process come
//    from OpenMFC's CPlex::Create (core/collections/CPlex.cpp), which allocates
//    with new[] and whose FreeDataChain is the matching deallocator.
//  * The destructor does not store the CAnimationBaseObject vftable (retail's
//    first store, right after the `sub $0x28,%rsp` prologue); OpenMFC has no
//    MSVC-layout vftable for this class.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstring>

// ---- sibling impl__ exports called by the bodies in this file ----
// Signatures derived from the mangled names; definitions live in
// core/animation/CAnimationController.cpp, core/animation/CAnimationVariable.cpp
// and core/collections/CPlex.cpp.
extern "C" void MS_ABI impl__RemoveAnimationObject_CAnimationController__QEAAXPEAVCAnimationBaseObject__H_Z(
    void* pThis, void* pAnimationObject, int bNoDelete);
extern "C" void* MS_ABI impl__AddAnimationObject_CAnimationController__QEAAPEAVCAnimationGroup__PEAVCAnimationBaseObject___Z(
    void* pThis, void* pAnimationObject);
extern "C" void* MS_ABI impl__GetUITransitionFactory_CAnimationController__QEAAPEAUIUIAnimationTransitionFactory__XZ(
    void* pThis);
extern "C" void* MS_ABI impl__GetUITransitionLibrary_CAnimationController__QEAAPEAUIUIAnimationTransitionLibrary__XZ(
    void* pThis);
extern "C" int MS_ABI impl__CreateTransitions_CAnimationVariable__QEAAHPEAUIUIAnimationTransitionLibrary__PEAUIUIAnimationTransitionFactory___Z(
    void* pThis, void* pTransitionLibrary, void* pTransitionFactory);
extern "C" void MS_ABI impl__ClearTransitions_CAnimationVariable__QEAAXH_Z(void* pThis, int bAutodestroy);
extern "C" void MS_ABI impl__EnableValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(
    void* pThis, void* pController, int bEnable);
extern "C" void MS_ABI impl__EnableIntegerValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(
    void* pThis, void* pController, int bEnable);
extern "C" void MS_ABI impl__ApplyTransitions_CAnimationVariable__QEAAXPEAVCAnimationController__PEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pController, void* pStoryboard, int bDependOnKeyframes);
extern "C" void MS_ABI impl__FreeDataChain_CPlex__QEAAXXZ(void* pThis);

namespace {

struct S_CAnimationBaseObject {
    void*          vfptr;                      // +0x00
    std::uint32_t  m_nGroupID;                 // +0x08
    std::uint32_t  m_nObjectID;                // +0x0c
    std::uintptr_t m_dwUserData;               // +0x10
    int            m_bAutodestroyTransitions;  // +0x18
    void*          m_pParentController;        // +0x20  CAnimationController*
};
static_assert(offsetof(S_CAnimationBaseObject, m_nGroupID) == 0x08, "m_nGroupID");
static_assert(offsetof(S_CAnimationBaseObject, m_nObjectID) == 0x0c, "m_nObjectID");
static_assert(offsetof(S_CAnimationBaseObject, m_dwUserData) == 0x10, "m_dwUserData");
static_assert(offsetof(S_CAnimationBaseObject, m_bAutodestroyTransitions) == 0x18, "m_bAutodestroyTransitions");
static_assert(offsetof(S_CAnimationBaseObject, m_pParentController) == 0x20, "m_pParentController");
static_assert(sizeof(S_CAnimationBaseObject) == 0x28, "sizeof(CAnimationBaseObject) == 40");

inline S_CAnimationBaseObject* Self(void* p) { return static_cast<S_CAnimationBaseObject*>(p); }

// Retail CAnimationVariable members reached through the list (file header).
constexpr std::size_t kOffVar_m_variable      = 0x08;
constexpr std::size_t kOffVar_m_pParentObject = 0x50;

constexpr int kSlot_GetAnimationVariableList = 10;  // vtbl byte offset 0x50

// Retail-layout CList<CAnimationVariable*, CAnimationVariable*> and its CNode.
struct VarNode {
    VarNode* pNext;  // +0x00
    VarNode* pPrev;  // +0x08
    void*    data;   // +0x10  CAnimationVariable*
};
static_assert(offsetof(VarNode, data) == 0x10, "CNode::data");

struct RetailVarList {
    const void*   vfptr;        // +0x00
    VarNode*      m_pNodeHead;  // +0x08
    VarNode*      m_pNodeTail;  // +0x10
    std::intptr_t m_nCount;     // +0x18
    VarNode*      m_pNodeFree;  // +0x20
    void*         m_pBlocks;    // +0x28  CPlex*
    std::intptr_t m_nBlockSize; // +0x30
};
static_assert(offsetof(RetailVarList, m_pNodeHead) == 0x08, "m_pNodeHead");
static_assert(offsetof(RetailVarList, m_nCount) == 0x18, "m_nCount");
static_assert(offsetof(RetailVarList, m_pBlocks) == 0x28, "m_pBlocks");
static_assert(offsetof(RetailVarList, m_nBlockSize) == 0x30, "m_nBlockSize");
static_assert(sizeof(RetailVarList) == 0x38, "sizeof(CList) == 56");

// The construct / GetAnimationVariableList / destroy sequence that opens and
// closes every list-walking body in this file.
class VariableList {
public:
    explicit VariableList(void* pObject) {
        m_list.vfptr = nullptr;       // deviation: see file header
        m_list.m_pNodeHead = nullptr;
        m_list.m_pNodeTail = nullptr;
        m_list.m_nCount = 0;
        m_list.m_pNodeFree = nullptr;
        m_list.m_pBlocks = nullptr;
        m_list.m_nBlockSize = 10;
        using Fn = void (MS_ABI*)(void*, RetailVarList*);
        void* const* vtbl = *static_cast<void* const* const*>(pObject);
        reinterpret_cast<Fn>(vtbl[kSlot_GetAnimationVariableList])(pObject, &m_list);
    }
    ~VariableList() {
        m_list.m_nCount = 0;
        m_list.m_pNodeFree = nullptr;
        m_list.m_pNodeTail = nullptr;
        m_list.m_pNodeHead = nullptr;
        if (m_list.m_pBlocks != nullptr) {
            impl__FreeDataChain_CPlex__QEAAXXZ(m_list.m_pBlocks);  // deviation: see file header
        }
        m_list.m_pBlocks = nullptr;
    }
    VariableList(const VariableList&) = delete;
    VariableList& operator=(const VariableList&) = delete;

    VarNode* Head() const { return m_list.m_pNodeHead; }

private:
    RetailVarList m_list;
};

inline void* VarField(void* pVar, std::size_t off) {
    void* v;
    std::memcpy(&v, static_cast<unsigned char*>(pVar) + off, sizeof(v));
    return v;
}
inline void SetVarField(void* pVar, std::size_t off, void* v) {
    std::memcpy(static_cast<unsigned char*>(pVar) + off, &v, sizeof(v));
}

} // namespace

// Symbol: ??0CAnimationBaseObject@@QEAA@II_K@Z
extern "C" void* MS_ABI impl___0CAnimationBaseObject__QEAA_II_K_Z(
    void* pThis, unsigned int unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    return pThis;
}
// Symbol: ??0CAnimationBaseObject@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationBaseObject__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CAnimationBaseObject@@UEAA@XZ
extern "C" void MS_ABI impl___1CAnimationBaseObject__UEAA_XZ(void* pThis) {
    // Retail RVA 0x4290 (mfc140u):
    //   vfptr = CAnimationBaseObject vftable;         // not done here, see file header
    //   if (m_pParentController != NULL)
    //       m_pParentController->RemoveAnimationObject(this, TRUE);
    S_CAnimationBaseObject* self = Self(pThis);
    if (self->m_pParentController != nullptr) {
        impl__RemoveAnimationObject_CAnimationController__QEAAXPEAVCAnimationBaseObject__H_Z(
            self->m_pParentController, pThis, 1);
    }
}
// Symbol: ?ApplyTransitions@CAnimationBaseObject@@UEAAHPEAUIUIAnimationStoryboard@@H@Z
extern "C" int MS_ABI impl__ApplyTransitions_CAnimationBaseObject__UEAAHPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int bDependOnKeyframes) {
    // Retail RVA 0x43e0 (mfc140u):
    //   if (m_pParentController == NULL) return FALSE;     // before the list is built
    //   GetAnimationVariableList(lst);                      // vtbl slot 10
    //   for each node: pVar = node->data; advance; if (pVar == NULL) continue;
    //       pVar->m_pParentObject = this;                   // +0x50
    //       pVar->ApplyTransitions(m_pParentController, pStoryboard, bDependOnKeyframes);
    //   return TRUE;                                        // the variable's result is not consulted
    S_CAnimationBaseObject* self = Self(pThis);
    if (self->m_pParentController == nullptr) {
        return FALSE;
    }
    VariableList lst(pThis);
    for (VarNode* node = lst.Head(); node != nullptr;) {
        void* pVar = node->data;
        node = node->pNext;
        if (pVar == nullptr) {
            continue;
        }
        SetVarField(pVar, kOffVar_m_pParentObject, pThis);
        impl__ApplyTransitions_CAnimationVariable__QEAAXPEAVCAnimationController__PEAUIUIAnimationStoryboard__H_Z(
            pVar, self->m_pParentController, pStoryboard, bDependOnKeyframes);
    }
    return TRUE;
}

// Symbol: ?ClearTransitions@CAnimationBaseObject@@UEAAXH@Z
extern "C" void MS_ABI impl__ClearTransitions_CAnimationBaseObject__UEAAXH_Z(void* pThis, int bAutodestroy) {
    // Retail RVA 0x4490 (mfc140u): GetAnimationVariableList(lst) (slot 10);
    // for each non-NULL node->data:
    //   pVar->ClearTransitions(bAutodestroy != 0 || m_bAutodestroyTransitions != 0);
    // (retail materialises the argument as exactly 0 or 1).
    S_CAnimationBaseObject* self = Self(pThis);
    VariableList lst(pThis);
    for (VarNode* node = lst.Head(); node != nullptr;) {
        void* pVar = node->data;
        node = node->pNext;
        if (pVar == nullptr) {
            continue;
        }
        const int bArg = (bAutodestroy != 0 || self->m_bAutodestroyTransitions != 0) ? 1 : 0;
        impl__ClearTransitions_CAnimationVariable__QEAAXH_Z(pVar, bArg);
    }
}

// Symbol: ?ContainsVariable@CAnimationBaseObject@@UEAAHPEAUIUIAnimationVariable@@@Z
extern "C" int MS_ABI impl__ContainsVariable_CAnimationBaseObject__UEAAHPEAUIUIAnimationVariable___Z(
    void* pThis, void* pVariable) {
    // Retail RVA 0x4680 (mfc140u):
    //   if (pVariable == NULL) return FALSE;                // before the list is built
    //   GetAnimationVariableList(lst);                      // vtbl slot 10
    //   for each node: pVar = node->data;
    //       if (pVar && pVar->m_variable != NULL && pVar->m_variable == pVariable)  // +0x08
    //           return TRUE;
    //   return FALSE;
    if (pVariable == nullptr) {
        return FALSE;
    }
    VariableList lst(pThis);
    for (VarNode* node = lst.Head(); node != nullptr; node = node->pNext) {
        void* pVar = node->data;
        if (pVar == nullptr) {
            continue;
        }
        void* pIVar = VarField(pVar, kOffVar_m_variable);
        if (pIVar != nullptr && pIVar == pVariable) {
            return TRUE;
        }
    }
    return FALSE;
}

// Symbol: ?CreateTransitions@CAnimationBaseObject@@QEAAHXZ
extern "C" int MS_ABI impl__CreateTransitions_CAnimationBaseObject__QEAAHXZ(void* pThis) {
    // Retail RVA 0x4320 (mfc140u):
    //   if (m_pParentController == NULL) return FALSE;      // before the list is built
    //   GetAnimationVariableList(lst);                      // vtbl slot 10
    //   for each node: pVar = node->data;
    //       if (pVar == NULL) return FALSE;                 // NULL element aborts the walk
    //       advance;
    //       pFactory = m_pParentController->GetUITransitionFactory();   // called first
    //       pLibrary = m_pParentController->GetUITransitionLibrary();
    //       if (!pVar->CreateTransitions(pLibrary, pFactory)) return FALSE;
    //   return TRUE;
    S_CAnimationBaseObject* self = Self(pThis);
    if (self->m_pParentController == nullptr) {
        return FALSE;
    }
    VariableList lst(pThis);
    for (VarNode* node = lst.Head(); node != nullptr;) {
        void* pVar = node->data;
        if (pVar == nullptr) {
            return FALSE;
        }
        node = node->pNext;
        void* pFactory =
            impl__GetUITransitionFactory_CAnimationController__QEAAPEAUIUIAnimationTransitionFactory__XZ(
                self->m_pParentController);
        void* pLibrary =
            impl__GetUITransitionLibrary_CAnimationController__QEAAPEAUIUIAnimationTransitionLibrary__XZ(
                self->m_pParentController);
        if (!impl__CreateTransitions_CAnimationVariable__QEAAHPEAUIUIAnimationTransitionLibrary__PEAUIUIAnimationTransitionFactory___Z(
                pVar, pLibrary, pFactory)) {
            return FALSE;
        }
    }
    return TRUE;
}

// Symbol: ?EnableIntegerValueChangedEvent@CAnimationBaseObject@@UEAAXPEAVCAnimationController@@H@Z
extern "C" void MS_ABI impl__EnableIntegerValueChangedEvent_CAnimationBaseObject__UEAAXPEAVCAnimationController__H_Z(
    void* pThis, void* pController, int bEnable) {
    // Retail RVA 0x45e0 (mfc140u): GetAnimationVariableList(lst) (slot 10);
    // for each non-NULL node->data: pVar->EnableIntegerValueChangedEvent(pController, bEnable).
    // The argument controller is forwarded; m_pParentController is not read.
    VariableList lst(pThis);
    for (VarNode* node = lst.Head(); node != nullptr;) {
        void* pVar = node->data;
        node = node->pNext;
        if (pVar == nullptr) {
            continue;
        }
        impl__EnableIntegerValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(
            pVar, pController, bEnable);
    }
}

// Symbol: ?EnableValueChangedEvent@CAnimationBaseObject@@UEAAXPEAVCAnimationController@@H@Z
extern "C" void MS_ABI impl__EnableValueChangedEvent_CAnimationBaseObject__UEAAXPEAVCAnimationController__H_Z(
    void* pThis, void* pController, int bEnable) {
    // Retail RVA 0x4540 (mfc140u): GetAnimationVariableList(lst) (slot 10);
    // for each non-NULL node->data: pVar->EnableValueChangedEvent(pController, bEnable).
    VariableList lst(pThis);
    for (VarNode* node = lst.Head(); node != nullptr;) {
        void* pVar = node->data;
        node = node->pNext;
        if (pVar == nullptr) {
            continue;
        }
        impl__EnableValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(
            pVar, pController, bEnable);
    }
}

// Symbol: ?SetID@CAnimationBaseObject@@QEAAXII@Z
extern "C" void MS_ABI impl__SetID_CAnimationBaseObject__QEAAXII_Z(
    void* pThis, unsigned int nObjectID, unsigned int nGroupID) {
    // Retail RVA 0x42c0 (mfc140u; see file header for how it was located):
    //   m_nObjectID = nObjectID;                            // unconditional, first
    //   if (m_pParentController == NULL) { m_nGroupID = nGroupID; return; }
    //   if (m_nGroupID == nGroupID) return;
    //   m_pParentController->RemoveAnimationObject(this, TRUE);
    //   m_nGroupID = nGroupID;
    //   m_pParentController->AddAnimationObject(this);     // result discarded
    S_CAnimationBaseObject* self = Self(pThis);
    void* pController = self->m_pParentController;
    self->m_nObjectID = nObjectID;
    if (pController == nullptr) {
        self->m_nGroupID = nGroupID;
        return;
    }
    if (self->m_nGroupID == nGroupID) {
        return;
    }
    impl__RemoveAnimationObject_CAnimationController__QEAAXPEAVCAnimationBaseObject__H_Z(pController, pThis, 1);
    self->m_nGroupID = nGroupID;
    impl__AddAnimationObject_CAnimationController__QEAAPEAVCAnimationGroup__PEAVCAnimationBaseObject___Z(
        pController, pThis);
}

// Symbol: ?SetParentAnimationObjects@CAnimationBaseObject@@MEAAXXZ
extern "C" void MS_ABI impl__SetParentAnimationObjects_CAnimationBaseObject__MEAAXXZ(void* pThis) {
    // Retail RVA 0x4720 (mfc140u; vftable slot 11): GetAnimationVariableList(lst)
    // (slot 10); for each non-NULL node->data: pVar->m_pParentObject = this (+0x50).
    VariableList lst(pThis);
    for (VarNode* node = lst.Head(); node != nullptr;) {
        void* pVar = node->data;
        node = node->pNext;
        if (pVar == nullptr) {
            continue;
        }
        SetVarField(pVar, kOffVar_m_pParentObject, pThis);
    }
}
