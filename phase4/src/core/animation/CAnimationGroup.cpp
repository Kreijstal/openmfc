// CAnimationGroup — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Every body below is transcribed from the retail export in mfc140u.dll (RVAs
// cited per function; the function bodies are byte-identical in mfc140.dll).
// Four of the exports -- RemoveKeyframes, RemoveTransitions, Schedule and
// SetAutodestroyTransitions -- have no entry in mfc140u_rva_symbols.json; their
// RVAs were read from mfc140u's export address table by ordinal
// (mfc_complete_ordinal_mapping.json: 12379, 12432, 12702, 12940).  Cross-checks
// (all mfc140u): RemoveKeyframes / RemoveTransitions are the two direct-call
// targets at 0x5fdc / 0x5fe4 inside ~CAnimationGroup (0x5fd0); Schedule is the
// target of the call at 0x70f3 inside CAnimationController::ScheduleGroup
// (0x70b0), and its body is the sequence Animate inlines at 0x64f9..0x656d.
// SetAutodestroyTransitions has no direct caller anywhere in the image; its RVA
// rests on the export table alone (its body is the walk transcribed below).
//
// RETAIL LAYOUT (afxanimationcontroller.h:1912; CAnimationGroup has no base and
// no vtable).  Every offset is a store made by the retail constructor
// ??0CAnimationGroup@@QEAA@PEAVCAnimationController@@I@Z, RVA 0x5f60 (mfc140u):
//   +0x00  UINT32  m_nGroupID                    (r8d)
//   +0x08  CAnimationController* m_pParentController (rdx)
//   +0x10  CObList m_lstAnimationObjects: vfptr +0x10, m_pNodeHead +0x18,
//          m_pNodeTail +0x20, m_nCount +0x28, m_pNodeFree +0x30,
//          m_pBlocks +0x38, m_nBlockSize +0x40 (= 10)
//   +0x48  CObList m_lstKeyFrames:        vfptr +0x48, m_pNodeHead +0x50, ...,
//          m_nCount +0x60, ..., m_nBlockSize +0x78 (= 10)
//   +0x80  CComPtr<IUIAnimationStoryboard> m_pStoryboard (= NULL)
//   +0x88  BOOL m_bAutodestroyKeyframes        (= 1)
//   +0x8c  BOOL m_bAutoclearTransitions        (= 1, low half of a qword store of 1)
//   +0x90  BOOL m_bAutodestroyAnimationObjects (= 0, high half of that store)
//   sizeof == 0x98: CAnimationController::AddAnimationObject (RVA 0x69e0,
//   mfc140u) allocates `mov $0x98,%ecx` before calling the constructor.
//
// Objects reached through the lists (retail layouts, used below):
//   CAnimationBaseObject (DECLARE_DYNAMIC; runtime class at 0x2dafd0 in mfc140u,
//     m_nObjectSize 0x28): vfptr +0, m_nGroupID +8, m_nObjectID +0xc,
//     m_dwUserData +0x10, m_bAutodestroyTransitions +0x18,
//     m_pParentController +0x20 (AddAnimationObject stores the controller there).
//     Virtual slots (CObject's five, then declaration order): 5 ApplyTransitions,
//     6 ClearTransitions, 9 ContainsVariable.
//   CBaseKeyFrame (runtime class at 0x2db030 in mfc140u, m_nObjectSize 0x18):
//     m_bIsKeyframeAtOffset +0x14 (S_CBaseKeyFrame in CAnimationController.cpp);
//     virtual slot 5 AddToStoryboard.
//   Slot 1 of both is the MSVC scalar deleting destructor; retail deletes with
//   `mov $1,%edx; call *8(vtbl)`.
//
// DEVIATIONS, file-wide:
//  * List storage.  OpenMFC's CObList keeps its contents in a side table keyed
//    by the list's address (OPENMFC_DECLARE_LIST_WRAPPER, include/openmfc/afx.h)
//    and has an OpenMFC vtable, so the constructor runs the exported CObList
//    constructor thunk on both sub-objects instead of storing retail's CObList
//    vftable, and every walk goes through the FindIndex thunk + node view (the
//    pattern of featurepack/taskspane/CMFCTasksPane.cpp) instead of reading the
//    raw m_pNodeHead / m_nCount words.  Walk order and per-node work are retail's.
//  * Group objects handed out by THIS DLL's CAnimationController are not
//    CAnimationGroups: AddAnimationObject returns a 16-byte side-table token
//    ({id @0, owner @8}, CAnimationController.cpp).  The bodies here are only
//    valid on a real 0x98-byte group (one built by the constructor below); on a
//    token they would read past the allocation.  Nothing inside this DLL calls
//    them today; see the report's headerRequests.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CAnimationControllerSupport.h"

#include <cstring>
#include <uianimation.h>

// ---- sibling impl__ exports called by the bodies in this file ----
// Each signature is derived from the mangled name; the definitions live in:
//   core/collections/CObList.cpp        ctor / dtor / RemoveAll / RemoveTail / FindIndex
//   core/runtime/CObject.cpp            IsKindOf
//   core/animation/RuntimeClasses.cpp   GetThisClass@CAnimationBaseObject / @CBaseKeyFrame
//   core/animation/CAnimationController.cpp  GetKeyframeStoryboardStart
//   core/animation/CAnimationBaseObject.cpp  CreateTransitions (void* pThis),
//       transcribed from retail RVA 0x4320 (mfc140u).
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(CObList* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CObList__UEAA_XZ(CObList* pThis);
extern "C" void MS_ABI impl__RemoveAll_CObList__QEAAXXZ(CObList* pThis);
extern "C" CObject* MS_ABI impl__RemoveTail_CObList__QEAAPEAVCObject__XZ(CObList* pThis);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(
    const CObList* pThis, long long nIndex);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAnimationBaseObject__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseKeyFrame__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__GetKeyframeStoryboardStart_CAnimationController__SAPEAVCBaseKeyFrame__XZ();
extern "C" int MS_ABI impl__CreateTransitions_CAnimationBaseObject__QEAAHXZ(void* pThis);

// Exports defined later in this file that earlier bodies call.
extern "C" void MS_ABI impl__RemoveKeyframes_CAnimationGroup__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RemoveTransitions_CAnimationGroup__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__ApplyTransitions_CAnimationGroup__QEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__CreateTransitions_CAnimationGroup__IEAAHXZ(void* pThis);
extern "C" int MS_ABI impl__Schedule_CAnimationGroup__QEAAHPEAUIUIAnimationTimer__N_Z(
    void* pThis, IUIAnimationTimer* pTimer, double time);

namespace {

struct S_CAnimationGroup {
    std::uint32_t           m_nGroupID;                      // +0x00
    void*                   m_pParentController;             // +0x08  CAnimationController*
    alignas(8) unsigned char m_lstAnimationObjects[0x38];    // +0x10  CObList
    alignas(8) unsigned char m_lstKeyFrames[0x38];           // +0x48  CObList
    IUIAnimationStoryboard* m_pStoryboard;                   // +0x80  CComPtr
    int                     m_bAutodestroyKeyframes;         // +0x88
    int                     m_bAutoclearTransitions;         // +0x8c
    int                     m_bAutodestroyAnimationObjects;  // +0x90
};
static_assert(offsetof(S_CAnimationGroup, m_pParentController) == 0x08, "m_pParentController");
static_assert(offsetof(S_CAnimationGroup, m_lstAnimationObjects) == 0x10, "m_lstAnimationObjects");
static_assert(offsetof(S_CAnimationGroup, m_lstKeyFrames) == 0x48, "m_lstKeyFrames");
static_assert(offsetof(S_CAnimationGroup, m_pStoryboard) == 0x80, "m_pStoryboard");
static_assert(offsetof(S_CAnimationGroup, m_bAutodestroyKeyframes) == 0x88, "m_bAutodestroyKeyframes");
static_assert(offsetof(S_CAnimationGroup, m_bAutoclearTransitions) == 0x8c, "m_bAutoclearTransitions");
static_assert(offsetof(S_CAnimationGroup, m_bAutodestroyAnimationObjects) == 0x90, "m_bAutodestroyAnimationObjects");
static_assert(sizeof(S_CAnimationGroup) == 0x98, "sizeof(CAnimationGroup) == 0x98");
// The CObList constructor thunk placement-constructs OpenMFC's CObList into the
// 0x38-byte slots above, so OpenMFC's CObList must be exactly retail-sized.
static_assert(sizeof(CObList) == 0x38, "sizeof(CObList) == 56");
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");

// Retail member offsets of the objects stored in the lists (see file header).
constexpr std::size_t kOffBaseObjAutodestroyTransitions = 0x18;  // CAnimationBaseObject
constexpr std::size_t kOffBaseObjParentController       = 0x20;  // CAnimationBaseObject
constexpr std::size_t kOffKeyFrameIsAtOffset            = 0x14;  // CBaseKeyFrame
// MSVC vtable slots.
constexpr int kSlot_ScalarDeletingDtor        = 1;   // CObject
constexpr int kSlot_BaseObj_ApplyTransitions  = 5;   // CAnimationBaseObject
constexpr int kSlot_BaseObj_ClearTransitions  = 6;
constexpr int kSlot_BaseObj_ContainsVariable  = 9;
constexpr int kSlot_KeyFrame_AddToStoryboard  = 5;   // CBaseKeyFrame
constexpr int kSlot_Ctrl_OnBeforeAnimationStart = 11; // CAnimationController (vftable
                                                      // listing in CAnimationController.cpp)

inline S_CAnimationGroup* Self(void* p) { return static_cast<S_CAnimationGroup*>(p); }
inline CObList* AnimationObjects(void* p) { return reinterpret_cast<CObList*>(Self(p)->m_lstAnimationObjects); }
inline CObList* KeyFrames(void* p) { return reinterpret_cast<CObList*>(Self(p)->m_lstKeyFrames); }

template <typename Fn>
inline Fn VSlot(const void* pObj, int slot) {
    void* const* vtbl = *static_cast<void* const* const*>(pObj);
    return reinterpret_cast<Fn>(vtbl[slot]);
}

// Mirror of CList<CObject*,CObject*>::CNode -- what a CObList::POSITION points at
// (pNext / pPrev / data), identical to retail's node layout (retail reads
// node->pNext at +0 and node->data at +0x10 in every walk below).
struct GrpNode {
    GrpNode* pNext;
    GrpNode* pPrev;
    CObject* data;
};
static_assert(offsetof(GrpNode, data) == 0x10, "CNode::data");

// Replaces retail's direct read of CObList::m_pNodeHead (see the file header).
inline GrpNode* HeadNode(const CObList* list) {
    CObList::POSITION pos = impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(list, 0);
    GrpNode* node = nullptr;
    std::memcpy(&node, &pos, sizeof(node));
    return node;
}
// Replaces retail's `m_nCount != 0` test: FindIndex(0) is non-NULL exactly
// when the list has an element.
inline bool ListNotEmpty(const CObList* list) { return HeadNode(list) != nullptr; }

// Retail's inlined DYNAMIC_DOWNCAST test `p && p->IsKindOf(RUNTIME_CLASS(X))`.
inline bool IsAnimationBaseObject(CObject* p) {
    return p != nullptr &&
           impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
               p, impl__GetThisClass_CAnimationBaseObject__SAPEAUCRuntimeClass__XZ()) != 0;
}
inline bool IsBaseKeyFrame(CObject* p) {
    return p != nullptr &&
           impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
               p, impl__GetThisClass_CBaseKeyFrame__SAPEAUCRuntimeClass__XZ()) != 0;
}

// `delete p` on a CObject: MSVC scalar deleting destructor, flag 1.
inline void DeleteCObject(CObject* p) {
    VSlot<void* (MS_ABI*)(void*, unsigned int)>(p, kSlot_ScalarDeletingDtor)(p, 1);
}

// Retail's CObList constructor stores for one embedded list after the OpenMFC
// CObList constructor has run: m_pNodeHead..m_pBlocks = 0, m_nBlockSize = 10.
// OpenMFC's list never reads these words (its contents live in the side
// table), but a client that walks the public list inline reads them, and the
// OpenMFC constructor leaves them untouched.
inline void ZeroRawListWords(unsigned char* list) {
    std::memset(list + 0x08, 0, 0x28);                 // +0x08 .. +0x2f
    const long long blockSize = 10;
    std::memcpy(list + 0x30, &blockSize, sizeof(blockSize));  // m_nBlockSize
}

} // namespace

// Symbol: ??0CAnimationGroup@@QEAA@PEAVCAnimationController@@I@Z
extern "C" void* MS_ABI impl___0CAnimationGroup__QEAA_PEAVCAnimationController__I_Z(
    void* pThis, void* pParentController, unsigned int nGroupID) {
    // Retail RVA 0x5f60 (mfc140u), transcribed store for store (see the file
    // header): m_nGroupID, m_pParentController, both CObLists (vfptr, five
    // zero words, m_nBlockSize 10), m_pStoryboard NULL, m_bAutodestroyKeyframes
    // = 1, m_bAutoclearTransitions = 1, m_bAutodestroyAnimationObjects = 0.
    // Deviation: each CObList is built by the exported CObList constructor
    // thunk (OpenMFC vtable + side-table registration) instead of storing the
    // retail CObList vftable; the raw list words are then set as retail sets them.
    S_CAnimationGroup* self = Self(pThis);
    self->m_nGroupID = nGroupID;
    self->m_pParentController = pParentController;
    impl___0CObList__QEAA__J_Z(AnimationObjects(pThis), 10);
    ZeroRawListWords(self->m_lstAnimationObjects);
    impl___0CObList__QEAA__J_Z(KeyFrames(pThis), 10);
    ZeroRawListWords(self->m_lstKeyFrames);
    self->m_pStoryboard = nullptr;
    self->m_bAutodestroyKeyframes = TRUE;
    self->m_bAutoclearTransitions = TRUE;
    self->m_bAutodestroyAnimationObjects = FALSE;
    return pThis;
}
// Symbol: ??1CAnimationGroup@@QEAA@XZ
extern "C" void MS_ABI impl___1CAnimationGroup__QEAA_XZ(void* pThis) {
    // Retail RVA 0x5fd0 (mfc140u):
    //   RemoveKeyframes();                       // direct call, 0x6120
    //   RemoveTransitions();                     // direct call, 0x62c0
    //   if (m_bAutodestroyAnimationObjects) {    // +0x90
    //       while (m_lstAnimationObjects.m_nCount != 0) {       // +0x28
    //           CObject* p = m_lstAnimationObjects.RemoveTail(); // 0x231fb0
    //           if (p) delete p;                 // vtbl slot 1, flag 1
    //       }
    //   } else {
    //       for each p in m_lstAnimationObjects (head +0x18, pNext read first):
    //           DYNAMIC_DOWNCAST(CAnimationBaseObject, p)->m_pParentController = NULL;  // +0x20
    //       m_lstAnimationObjects.RemoveAll();   // 0x8350
    //   }
    //   m_pStoryboard.~CComPtr();               // Release() if non-NULL, slot 2
    //   m_lstKeyFrames.~CObList();              // vftable store + RemoveAll (0x8350)
    //   m_lstAnimationObjects.~CObList();       // vftable store + tail-jmp RemoveAll
    // Deviation: retail stores through the DYNAMIC_DOWNCAST result without a
    // NULL test (a NULL element, or one that is not a CAnimationBaseObject,
    // makes it write to address 0x20); this body skips such elements instead
    // of faulting.
    // The two ~CObList steps run OpenMFC's CObList destructor thunk (drops the
    // side-table storage) -- see the file header.
    S_CAnimationGroup* self = Self(pThis);
    impl__RemoveKeyframes_CAnimationGroup__QEAAXXZ(pThis);
    impl__RemoveTransitions_CAnimationGroup__QEAAXXZ(pThis);

    CObList* objects = AnimationObjects(pThis);
    if (self->m_bAutodestroyAnimationObjects != 0) {
        while (ListNotEmpty(objects)) {
            CObject* p = impl__RemoveTail_CObList__QEAAPEAVCObject__XZ(objects);
            if (p != nullptr) {
                DeleteCObject(p);
            }
        }
    } else {
        for (GrpNode* node = HeadNode(objects); node != nullptr;) {
            CObject* p = node->data;
            node = node->pNext;
            if (IsAnimationBaseObject(p)) {
                void* nullController = nullptr;
                std::memcpy(reinterpret_cast<unsigned char*>(p) + kOffBaseObjParentController,
                            &nullController, sizeof(nullController));
            }
        }
        impl__RemoveAll_CObList__QEAAXXZ(objects);
    }

    if (self->m_pStoryboard != nullptr) {
        self->m_pStoryboard->Release();
    }
    impl___1CObList__UEAA_XZ(KeyFrames(pThis));
    impl___1CObList__UEAA_XZ(objects);
}

// Symbol: ?AddKeyframes@CAnimationGroup@@IEAAXPEAUIUIAnimationStoryboard@@H@Z
extern "C" void MS_ABI impl__AddKeyframes_CAnimationGroup__IEAAXPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int bAddDeep) {
    // Retail RVA 0x60b0 (mfc140u): walk m_lstKeyFrames (head +0x50, pNext read
    // before the element is processed); for each element that IsKindOf
    // CBaseKeyFrame (runtime class 0x2db030):
    //   if (pKeyframe->m_bIsKeyframeAtOffset)       // +0x14
    //       pKeyframe->AddToStoryboard(pStoryboard, bAddDeep);   // vtbl slot 5
    //   else if (bAddDeep)
    //       pKeyframe->AddToStoryboard(pStoryboard, TRUE);
    // The HRESULT/BOOL result is ignored.  Non-keyframe and NULL elements are
    // skipped.
    // OpenMFC addition, not in retail: RegisterStoryboard() records
    // storyboard -> group in CAnimationController.cpp's side table, which its
    // FindAnimationGroup(IUIAnimationStoryboard*) consults (retail instead
    // compares each group's m_pStoryboard).  It only records groups that are
    // side-table tokens, so for a real CAnimationGroup it is a no-op.
    openmfc::detail::animationcontroller::RegisterStoryboard(pThis, pStoryboard);
    for (GrpNode* node = HeadNode(KeyFrames(pThis)); node != nullptr;) {
        CObject* p = node->data;
        node = node->pNext;
        if (!IsBaseKeyFrame(p)) {
            continue;
        }
        int bAtOffset = 0;
        std::memcpy(&bAtOffset, reinterpret_cast<unsigned char*>(p) + kOffKeyFrameIsAtOffset, sizeof(bAtOffset));
        int bDeep;
        if (bAtOffset != 0) {
            bDeep = bAddDeep;
        } else if (bAddDeep != 0) {
            bDeep = TRUE;
        } else {
            continue;
        }
        VSlot<int (MS_ABI*)(void*, void*, int)>(p, kSlot_KeyFrame_AddToStoryboard)(p, pStoryboard, bDeep);
    }
}

// Symbol: ?AddTransitions@CAnimationGroup@@IEAAXPEAUIUIAnimationStoryboard@@H@Z
extern "C" void MS_ABI impl__AddTransitions_CAnimationGroup__IEAAXPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int bDependOnKeyframes) {
    // Retail RVA 0x6190 (mfc140u): walk m_lstAnimationObjects (head +0x18); for
    // each element that IsKindOf CAnimationBaseObject (runtime class 0x2dafd0):
    //   pObj->ApplyTransitions(pStoryboard, bDependOnKeyframes);  // vtbl slot 5
    // The BOOL result is ignored.
    // OpenMFC addition, not in retail: RegisterStoryboard() -- see AddKeyframes.
    openmfc::detail::animationcontroller::RegisterStoryboard(pThis, pStoryboard);
    for (GrpNode* node = HeadNode(AnimationObjects(pThis)); node != nullptr;) {
        CObject* p = node->data;
        node = node->pNext;
        if (IsAnimationBaseObject(p)) {
            VSlot<int (MS_ABI*)(void*, void*, int)>(p, kSlot_BaseObj_ApplyTransitions)(
                p, pStoryboard, bDependOnKeyframes);
        }
    }
}
// Symbol: ?Animate@CAnimationGroup@@QEAAHPEAUIUIAnimationManager@@PEAUIUIAnimationTimer@@H@Z
extern "C" int MS_ABI impl__Animate_CAnimationGroup__QEAAHPEAUIUIAnimationManager__PEAUIUIAnimationTimer__H_Z(
    void* pThis, IUIAnimationManager* pManager, IUIAnimationTimer* pTimer, int bScheduleNow) {
    // Retail RVA 0x63e0 (mfc140u):
    //   m_pStoryboard.Release();      // +0x80: if non-NULL, NULL it then Release()
    //                                 // (the pattern is emitted twice; the second
    //                                 // test sees the NULL the first one stored)
    //   if (!pManager || !pTimer) return FALSE;
    //   if (FAILED(pManager->CreateStoryboard(&m_pStoryboard))) return FALSE;  // slot 5
    //   if (FAILED(m_pStoryboard->SetTag(NULL, m_nGroupID))) return FALSE;    // slot 15
    //   ApplyTransitions();           // inlined: 0x648e..0x64d3 == RVA 0x6250
    //   if (!bScheduleNow) return TRUE;
    //   if (FAILED(pTimer->GetTime(&t))) return FALSE;                        // slot 8
    //   return Schedule(pTimer, t);   // inlined: 0x64f9..0x656d == RVA 0x6590
    //                                 // minus its pTimer test (already non-NULL)
    // ApplyTransitions and Schedule are called as the exports here; their
    // bodies are the inlined sequences.
    S_CAnimationGroup* self = Self(pThis);
    for (int pass = 0; pass < 2; ++pass) {
        IUIAnimationStoryboard* old = self->m_pStoryboard;
        if (old != nullptr) {
            self->m_pStoryboard = nullptr;
            old->Release();
        }
    }
    if (pManager == nullptr || pTimer == nullptr) {
        return FALSE;
    }
    if (FAILED(pManager->CreateStoryboard(&self->m_pStoryboard))) {
        return FALSE;
    }
    if (FAILED(self->m_pStoryboard->SetTag(nullptr, self->m_nGroupID))) {
        return FALSE;
    }
    impl__ApplyTransitions_CAnimationGroup__QEAAXXZ(pThis);
    if (bScheduleNow == 0) {
        return TRUE;
    }
    UI_ANIMATION_SECONDS time = 0.0;
    if (FAILED(pTimer->GetTime(&time))) {
        return FALSE;
    }
    return impl__Schedule_CAnimationGroup__QEAAHPEAUIUIAnimationTimer__N_Z(pThis, pTimer, time);
}

// Symbol: ?ApplyTransitions@CAnimationGroup@@QEAAXXZ
extern "C" void MS_ABI impl__ApplyTransitions_CAnimationGroup__QEAAXXZ(void* pThis) {
    // Retail RVA 0x6250 (mfc140u):
    //   if (m_pStoryboard == NULL) return;          // +0x80
    //   if (!CreateTransitions()) return;           // direct call, 0x61f0
    //   AddKeyframes(m_pStoryboard, FALSE);         // 0x60b0
    //   AddTransitions(m_pStoryboard, FALSE);       // 0x6190
    //   AddKeyframes(m_pStoryboard, TRUE);
    //   AddTransitions(m_pStoryboard, TRUE);
    // m_pStoryboard is reloaded from +0x80 before each call.
    S_CAnimationGroup* self = Self(pThis);
    if (self->m_pStoryboard == nullptr) {
        return;
    }
    if (!impl__CreateTransitions_CAnimationGroup__IEAAHXZ(pThis)) {
        return;
    }
    impl__AddKeyframes_CAnimationGroup__IEAAXPEAUIUIAnimationStoryboard__H_Z(pThis, self->m_pStoryboard, FALSE);
    impl__AddTransitions_CAnimationGroup__IEAAXPEAUIUIAnimationStoryboard__H_Z(pThis, self->m_pStoryboard, FALSE);
    impl__AddKeyframes_CAnimationGroup__IEAAXPEAUIUIAnimationStoryboard__H_Z(pThis, self->m_pStoryboard, TRUE);
    impl__AddTransitions_CAnimationGroup__IEAAXPEAUIUIAnimationStoryboard__H_Z(pThis, self->m_pStoryboard, TRUE);
}

// Symbol: ?CreateTransitions@CAnimationGroup@@IEAAHXZ
extern "C" int MS_ABI impl__CreateTransitions_CAnimationGroup__IEAAHXZ(void* pThis) {
    // Retail RVA 0x61f0 (mfc140u): walk m_lstAnimationObjects (head +0x18); for
    // each element that IsKindOf CAnimationBaseObject:
    //   if (!pObj->CreateTransitions()) return FALSE;   // direct call, 0x4320
    // return TRUE (also for an empty list).
    for (GrpNode* node = HeadNode(AnimationObjects(pThis)); node != nullptr;) {
        CObject* p = node->data;
        node = node->pNext;
        if (IsAnimationBaseObject(p) &&
            !impl__CreateTransitions_CAnimationBaseObject__QEAAHXZ(p)) {
            return FALSE;
        }
    }
    return TRUE;
}

// Symbol: ?FindAnimationObject@CAnimationGroup@@QEAAPEAVCAnimationBaseObject@@PEAUIUIAnimationVariable@@@Z
extern "C" void* MS_ABI impl__FindAnimationObject_CAnimationGroup__QEAAPEAVCAnimationBaseObject__PEAUIUIAnimationVariable___Z(
    void* pThis, void* pVariable) {
    // Retail RVA 0x6370 (mfc140u): walk m_lstAnimationObjects (head +0x18); the
    // first element that IsKindOf CAnimationBaseObject and whose
    // ContainsVariable(pVariable) (vtbl slot 9) is non-zero is returned;
    // otherwise NULL.
    for (GrpNode* node = HeadNode(AnimationObjects(pThis)); node != nullptr;) {
        CObject* p = node->data;
        node = node->pNext;
        if (IsAnimationBaseObject(p) &&
            VSlot<int (MS_ABI*)(void*, void*)>(p, kSlot_BaseObj_ContainsVariable)(p, pVariable) != 0) {
            return p;
        }
    }
    return nullptr;
}

// Symbol: ?RemoveKeyframes@CAnimationGroup@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveKeyframes_CAnimationGroup__QEAAXXZ(void* pThis) {
    // Retail RVA 0x6120 (mfc140u; export ordinal 12379):
    //   if (m_bAutodestroyKeyframes) {                         // +0x88
    //       while (m_lstKeyFrames.m_nCount != 0) {             // +0x60
    //           CObject* p = m_lstKeyFrames.RemoveTail();      // 0x231fb0
    //           if (p != &CAnimationController::g_KeyframeStoryboardStart  // 0x3b1ba8
    //               && p != NULL)
    //               delete p;                                  // vtbl slot 1, flag 1
    //       }
    //   } else {
    //       m_lstKeyFrames.RemoveAll();                        // 0x8350
    //   }
    // The sentinel's address is taken from GetKeyframeStoryboardStart: in
    // retail that returns &g_KeyframeStoryboardStart (RVA 0x7900, mfc140u);
    // in OpenMFC it returns the function-local stand-in sentinel defined in
    // CAnimationController.cpp, the only sentinel this DLL hands out.
    S_CAnimationGroup* self = Self(pThis);
    CObList* keyframes = KeyFrames(pThis);
    if (self->m_bAutodestroyKeyframes == 0) {
        impl__RemoveAll_CObList__QEAAXXZ(keyframes);
        return;
    }
    void* const sentinel = impl__GetKeyframeStoryboardStart_CAnimationController__SAPEAVCBaseKeyFrame__XZ();
    while (ListNotEmpty(keyframes)) {
        CObject* p = impl__RemoveTail_CObList__QEAAPEAVCObject__XZ(keyframes);
        if (p != sentinel && p != nullptr) {
            DeleteCObject(p);
        }
    }
}

// Symbol: ?RemoveTransitions@CAnimationGroup@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveTransitions_CAnimationGroup__QEAAXXZ(void* pThis) {
    // Retail RVA 0x62c0 (mfc140u; export ordinal 12432):
    //   if (!m_bAutoclearTransitions) return;                  // +0x8c
    //   for each element of m_lstAnimationObjects (head +0x18) that IsKindOf
    //   CAnimationBaseObject:  pObj->ClearTransitions(FALSE);  // vtbl slot 6
    if (Self(pThis)->m_bAutoclearTransitions == 0) {
        return;
    }
    for (GrpNode* node = HeadNode(AnimationObjects(pThis)); node != nullptr;) {
        CObject* p = node->data;
        node = node->pNext;
        if (IsAnimationBaseObject(p)) {
            VSlot<void (MS_ABI*)(void*, int)>(p, kSlot_BaseObj_ClearTransitions)(p, FALSE);
        }
    }
}

// Symbol: ?Schedule@CAnimationGroup@@QEAAHPEAUIUIAnimationTimer@@N@Z
extern "C" int MS_ABI impl__Schedule_CAnimationGroup__QEAAHPEAUIUIAnimationTimer__N_Z(
    void* pThis, IUIAnimationTimer* pTimer, double time) {
    // Retail RVA 0x6590 (mfc140u; export ordinal 12702):
    //   if (!pTimer || !m_pParentController || !m_pStoryboard) return FALSE;
    //   if (time == 0.0) pTimer->GetTime(&time);   // slot 8; HRESULT NOT checked
    //   m_pParentController->OnBeforeAnimationStart(this);           // vtbl slot 11
    //   return SUCCEEDED(m_pStoryboard->Schedule(time, NULL));       // slot 11
    // (the 0.0 is the .rdata double at RVA 0x34fe98, mfc140u.)  Deviation: when GetTime
    // fails retail schedules with whatever its uninitialised local holds; here
    // the local starts at the caller's 0.0.
    S_CAnimationGroup* self = Self(pThis);
    if (pTimer == nullptr || self->m_pParentController == nullptr || self->m_pStoryboard == nullptr) {
        return FALSE;
    }
    UI_ANIMATION_SECONDS when = time;
    if (time == 0.0) {
        (void)pTimer->GetTime(&when);
    }
    void* controller = self->m_pParentController;
    VSlot<void (MS_ABI*)(void*, void*)>(controller, kSlot_Ctrl_OnBeforeAnimationStart)(controller, pThis);
    return SUCCEEDED(self->m_pStoryboard->Schedule(when, nullptr)) ? TRUE : FALSE;
}

// Symbol: ?SetAutodestroyTransitions@CAnimationGroup@@QEAAXH@Z
extern "C" void MS_ABI impl__SetAutodestroyTransitions_CAnimationGroup__QEAAXH_Z(void* pThis, int bAutoDestroy) {
    // Retail RVA 0x6320 (mfc140u; export ordinal 12940): for each element of
    // m_lstAnimationObjects (head +0x18) that IsKindOf CAnimationBaseObject,
    // the inline CAnimationBaseObject::SetAutodestroyTransitions:
    //   pObj->m_bAutodestroyTransitions = bAutoDestroy;       // +0x18
    for (GrpNode* node = HeadNode(AnimationObjects(pThis)); node != nullptr;) {
        CObject* p = node->data;
        node = node->pNext;
        if (IsAnimationBaseObject(p)) {
            std::memcpy(reinterpret_cast<unsigned char*>(p) + kOffBaseObjAutodestroyTransitions,
                        &bAutoDestroy, sizeof(bAutoDestroy));
        }
    }
}
