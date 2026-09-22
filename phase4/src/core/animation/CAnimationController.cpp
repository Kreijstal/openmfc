// CAnimationController — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, mfccore.cpp
//
// ---------------------------------------------------------------------------
// Retail object layout (mfc140u.dll).  CAnimationController is not declared
// in OpenMFC's public headers; client (MSVC) code allocates it with the retail
// layout from atlmfc/include/afxanimationcontroller.h (read on this host,
// member names quoted from it) and a client-derived class reads
// m_pAnimationManager & co. inline through that header.  The offsets below are
// the stores made by the retail constructor ??0CAnimationController@@QEAA@XZ,
// RVA 0x6620 (mfc140u), which writes exactly:
//
//   +0x00  vfptr                       = CAnimationController vftable
//   +0x08  BOOL   m_bIsValid           = 1
//   +0x10  IUIAnimationManager*           m_pAnimationManager   = NULL (CComPtr)
//   +0x18  IUIAnimationTransitionLibrary* m_pTransitionLibrary  = NULL (CComPtr)
//   +0x20  IUIAnimationTimer*             m_pAnimationTimer     = NULL (CComPtr)
//   +0x28  IUIAnimationTransitionFactory* m_pTransitionFactory  = NULL (CComPtr)
//   +0x30  CWnd*  m_pRelatedWnd        = NULL
//   +0x38  CList<CAnimationGroup*, CAnimationGroup*> m_lstAnimationGroups:
//          +0x38 vfptr = CList vftable, +0x40 m_pNodeHead = 0, +0x48 m_pNodeTail = 0,
//          +0x50 m_nCount = 0, +0x58 m_pNodeFree = 0, +0x60 m_pBlocks = 0,
//          +0x68 m_nBlockSize = 10 (qword store)
//   sizeof == 0x70 (112), which is the m_nObjectSize this repo already records
//   in core/animation/RuntimeClasses.cpp (ANIM_DESC(CAnimationController, 112)).
//
// Which of the four COM slots holds which interface was pinned from the four
// accessors, each of which CoCreateInstance()s into its own slot (all RVAs
// mfc140u, CLSID/IID constants read out of .rdata at the cited addresses and
// checked against uianimation.h):
//   GetUIAnimationManager    0x6760  +0x10  CLSID 0x2d97e8 / IID 0x33d340
//   GetUITransitionLibrary   0x6800  +0x18  CLSID 0x2d97f8 / IID 0x33d330
//   GetUIAnimationTimer      0x68a0  +0x20  CLSID 0x2d9818 / IID 0x33d320
//   GetUITransitionFactory   0x6940  +0x28  CLSID 0x2d9808 / IID 0x33d310
//
// The destructor ??1CAnimationController@@UEAA@XZ, RVA 0x66d0 (mfc140u),
// stores the vftable, calls RemoveAllAnimationGroups (0x6bc0), runs the
// CList destructor on +0x38 (stores the CList vftable and calls RemoveAll,
// 0x8350), then Release()s +0x28, +0x20, +0x18, +0x10 in that order, each
// guarded by a NULL test and dispatched through IUnknown slot 2.
//
// Retail vftable for CAnimationController (0x1802daa98 in mfc140u; every slot
// read with the vtable dumper and matched to its export):
//    0 GetRuntimeClass                 13 OnAnimationValueChanged (ret)
//    1 {scalar deleting dtor} 0x6680   14 OnAnimationIntegerValueChanged (ret)
//    2 Serialize (ret)                 15 OnStoryboardStatusChanged (ret)
//    3 AssertValid (ret)               16 OnStoryboardUpdated (ret)
//    4 Dump (ret)                      17 OnHasPriorityCancel   (xor eax,eax; ret)
//    5 EnableAnimationManagerEvent     18 OnHasPriorityConclude (same body, ICF)
//    6 EnablePriorityComparisonHandler 19 OnHasPriorityTrim     (same body, ICF)
//    7 EnableStoryboardEventHandler    20 OnHasPriorityCompress (same body, ICF)
//    8 EnableAnimationTimerEventHandler21 OnAnimationTimerPreUpdate (ret)
//    9 UpdateAnimationManager          22 OnAnimationTimerPostUpdate 0x71f0
//   10 IsAnimationInProgress           23 OnAnimationTimerRenderingTooSlow (ret)
//   11 OnBeforeAnimationStart (ret)    24 OnAfterSchedule 0x7130
//   12 OnAnimationManagerStatusChanged 0x7190
// "(ret)" slots all point at the shared one-byte `ret` at 0x27d0.  The MSVC
// declaration order in afxanimationcontroller.h agrees with this slot order.
//
// Group / keyframe bookkeeping in this file is a clean-room side table
// (file-local registries keyed by controller / group / object pointers) that
// predates the retail-layout work; retail keeps the groups in
// m_lstAnimationGroups.  The side table is kept as is.  The CList sub-object
// at +0x38 is therefore zero-filled by the constructor (with m_nBlockSize = 10
// as retail stores it) and left alone by the destructor: this DLL never puts a
// node in it and plants NO CList vptr there (there is no MSVC-layout
// CList<CAnimationGroup*> vtable in this DLL), so a client that dispatched
// through it would fault deterministically instead of hitting a wrong slot.
// ---------------------------------------------------------------------------

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"
#include "detail/CAnimationControllerSupport.h"

#include <uianimation.h>

// ---- sibling impl__ exports called by the bodies in this file ----
// Every declaration below matches a definition that exists today: the first
// two in core/animation/RuntimeClasses.cpp, the third in
// core/animation/CBaseKeyFrame.cpp, the rest further down in this file.
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAnimationController__UEBAPEAUCRuntimeClass__XZ(const void* pThis);
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CBaseKeyFrame__UEBAPEAUCRuntimeClass__XZ(const void* pThis);
extern "C" int MS_ABI impl__AddToStoryboard_CBaseKeyFrame__UEAAHPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int bDeepAdd);
extern "C" void MS_ABI impl___1CAnimationController__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__RemoveAllAnimationGroups_CAnimationController__QEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__I_Z(
    void* pThis, unsigned int groupId);
extern "C" void* MS_ABI impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__PEAUIUIAnimationStoryboard___Z(
    void* pThis, void* pStoryboard);
extern "C" void* MS_ABI impl__GetUIAnimationManager_CAnimationController__QEAAPEAUIUIAnimationManager__XZ(void* pThis);
extern "C" void* MS_ABI impl__GetUIAnimationTimer_CAnimationController__QEAAPEAUIUIAnimationTimer__XZ(void* pThis);
extern "C" int MS_ABI impl__EnableAnimationManagerEvent_CAnimationController__UEAAHH_Z(void* pThis, int bEnable);
extern "C" int MS_ABI impl__EnablePriorityComparisonHandler_CAnimationController__UEAAH_K_Z(
    void* pThis, unsigned long long dwHandlerType);
extern "C" int MS_ABI impl__EnableStoryboardEventHandler_CAnimationController__UEAAHIH_Z(
    void* pThis, unsigned int nGroupID, int bEnable);
extern "C" int MS_ABI impl__EnableAnimationTimerEventHandler_CAnimationController__UEAAHHW4__MIDL___MIDL_itf_UIAnimation_0000_0012_0001___Z(
    void* pThis, int bEnable, int idleBehavior);
extern "C" void MS_ABI impl__UpdateAnimationManager_CAnimationController__UEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__IsAnimationInProgress_CAnimationController__UEAAHXZ(void* pThis);
extern "C" void MS_ABI impl__OnBeforeAnimationStart_CAnimationController__UEAAXPEAVCAnimationGroup___Z(
    void* pThis, void* pGroup);
extern "C" void MS_ABI impl__OnAnimationManagerStatusChanged_CAnimationController__UEAAXW4__MIDL___MIDL_itf_UIAnimation_0000_0000_0002__0_Z(
    void* pThis, int newStatus, int previousStatus);
extern "C" void MS_ABI impl__OnAnimationValueChanged_CAnimationController__UEAAXPEAVCAnimationGroup__PEAVCAnimationBaseObject__PEAUIUIAnimationVariable__NN_Z(
    void* pThis, void* pGroup, void* pObject, void* pVariable, double newValue, double prevValue);
extern "C" void MS_ABI impl__OnAnimationIntegerValueChanged_CAnimationController__UEAAXPEAVCAnimationGroup__PEAVCAnimationBaseObject__PEAUIUIAnimationVariable__HH_Z(
    void* pThis, void* pGroup, void* pObject, void* pVariable, int newValue, int prevValue);
extern "C" void MS_ABI impl__OnStoryboardStatusChanged_CAnimationController__UEAAXPEAVCAnimationGroup__W4__MIDL___MIDL_itf_UIAnimation_0000_0002_0001__1_Z(
    void* pThis, void* pGroup, int newStatus, int previousStatus);
extern "C" void MS_ABI impl__OnStoryboardUpdated_CAnimationController__UEAAXPEAVCAnimationGroup___Z(
    void* pThis, void* pGroup);
extern "C" int MS_ABI impl__OnHasPriorityCancel_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* pThis, void* pGroupScheduled, void* pGroupNew, int priorityEffect);
extern "C" int MS_ABI impl__OnHasPriorityConclude_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* pThis, void* pGroupScheduled, void* pGroupNew, int priorityEffect);
extern "C" int MS_ABI impl__OnHasPriorityTrim_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* pThis, void* pGroupScheduled, void* pGroupNew, int priorityEffect);
extern "C" int MS_ABI impl__OnHasPriorityCompress_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* pThis, void* pGroupScheduled, void* pGroupNew, int priorityEffect);
extern "C" void MS_ABI impl__OnAnimationTimerPreUpdate_CAnimationController__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnAnimationTimerPostUpdate_CAnimationController__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnAnimationTimerRenderingTooSlow_CAnimationController__UEAAXI_Z(
    void* pThis, unsigned int fps);
extern "C" void MS_ABI impl__OnAfterSchedule_CAnimationController__MEAAXPEAVCAnimationGroup___Z(
    void* pThis, void* pGroup);

namespace {

// Retail CAnimationController layout view (see the file header for the
// derivation of every offset).  The CList sub-object is spelled out field by
// field because this DLL has no retail-layout CList<CAnimationGroup*>.
struct S_CAnimationController {
    void*     vfptr;                 // +0x00
    int       m_bIsValid;            // +0x08
    void*     m_pAnimationManager;   // +0x10  IUIAnimationManager*
    void*     m_pTransitionLibrary;  // +0x18  IUIAnimationTransitionLibrary*
    void*     m_pAnimationTimer;     // +0x20  IUIAnimationTimer*
    void*     m_pTransitionFactory;  // +0x28  IUIAnimationTransitionFactory*
    CWnd*     m_pRelatedWnd;         // +0x30
    void*     m_lst_vfptr;           // +0x38  m_lstAnimationGroups (CList) begins
    void*     m_lst_pNodeHead;       // +0x40
    void*     m_lst_pNodeTail;       // +0x48
    long long m_lst_nCount;          // +0x50  INT_PTR
    void*     m_lst_pNodeFree;       // +0x58
    void*     m_lst_pBlocks;         // +0x60
    long long m_lst_nBlockSize;      // +0x68  INT_PTR, retail stores 10
};
static_assert(offsetof(S_CAnimationController, m_bIsValid) == 0x08, "m_bIsValid");
static_assert(offsetof(S_CAnimationController, m_pAnimationManager) == 0x10, "m_pAnimationManager");
static_assert(offsetof(S_CAnimationController, m_pTransitionLibrary) == 0x18, "m_pTransitionLibrary");
static_assert(offsetof(S_CAnimationController, m_pAnimationTimer) == 0x20, "m_pAnimationTimer");
static_assert(offsetof(S_CAnimationController, m_pTransitionFactory) == 0x28, "m_pTransitionFactory");
static_assert(offsetof(S_CAnimationController, m_pRelatedWnd) == 0x30, "m_pRelatedWnd");
static_assert(offsetof(S_CAnimationController, m_lst_vfptr) == 0x38, "m_lstAnimationGroups");
static_assert(offsetof(S_CAnimationController, m_lst_nBlockSize) == 0x68, "m_lstAnimationGroups.m_nBlockSize");
static_assert(sizeof(S_CAnimationController) == 0x70, "sizeof(CAnimationController) == 112");

// OnAnimationManagerStatusChanged / OnAnimationTimerPostUpdate read the related
// window's HWND as m_pRelatedWnd->[0x40]; OpenMFC's CWnd keeps m_hWnd there too.
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd at +0x40");

// Retail CBaseKeyFrame layout (afxanimationcontroller.h:298, members quoted):
// CObject vfptr, UI_ANIMATION_KEYFRAME m_keyframe (+0x08), BOOL m_bAdded
// (+0x10), BOOL m_bIsKeyframeAtOffset (+0x14); sizeof 0x18 = the 24 that
// RuntimeClasses.cpp records for the class.  The process-wide sentinel
// CAnimationController::g_KeyframeStoryboardStart lives at 0x3b1ba8 (mfc140u)
// and reads, qword by qword: 0x1802dae80 (CBaseKeyFrame vftable),
// 0xffffffffffffffff (UI_ANIMATION_KEYFRAME_STORYBOARD_START), 0x0000000100000001
// (m_bAdded = 1, m_bIsKeyframeAtOffset = 1).
struct S_CBaseKeyFrame {
    void*                 vfptr;                  // +0x00
    UI_ANIMATION_KEYFRAME m_keyframe;             // +0x08
    int                   m_bAdded;               // +0x10
    int                   m_bIsKeyframeAtOffset;  // +0x14
};
static_assert(offsetof(S_CBaseKeyFrame, m_keyframe) == 0x08, "CBaseKeyFrame::m_keyframe");
static_assert(offsetof(S_CBaseKeyFrame, m_bAdded) == 0x10, "CBaseKeyFrame::m_bAdded");
static_assert(offsetof(S_CBaseKeyFrame, m_bIsKeyframeAtOffset) == 0x14, "CBaseKeyFrame::m_bIsKeyframeAtOffset");
static_assert(sizeof(S_CBaseKeyFrame) == 0x18, "sizeof(CBaseKeyFrame) == 24");

// CLSIDs / IIDs exactly as the four accessors pass them to CoCreateInstance
// (read out of mfc140u .rdata at the addresses cited in the file header; they
// are the uianimation.h constants, spelled out here so this TU does not need
// a uuid import library).
constexpr GUID kCLSID_UIAnimationManager           = {0x4c1fc63a, 0x695c, 0x47e8, {0xa3,0x39,0x1a,0x19,0x4b,0xe3,0xd0,0xb8}};
constexpr GUID kIID_IUIAnimationManager            = {0x9169896c, 0xac8d, 0x4e7d, {0x94,0xe5,0x67,0xfa,0x4d,0xc2,0xf2,0xe8}};
constexpr GUID kCLSID_UIAnimationTransitionLibrary = {0x1d6322ad, 0xaa85, 0x4ef5, {0xa8,0x28,0x86,0xd7,0x10,0x67,0xd1,0x45}};
constexpr GUID kIID_IUIAnimationTransitionLibrary  = {0xca5a14b1, 0xd24f, 0x48b8, {0x8f,0xe4,0xc7,0x81,0x69,0xba,0x95,0x4e}};
constexpr GUID kCLSID_UIAnimationTimer             = {0xbfcd4a0c, 0x06b6, 0x4384, {0xb7,0x68,0x0d,0xaa,0x79,0x2c,0x38,0x0e}};
constexpr GUID kIID_IUIAnimationTimer              = {0x6b0efad1, 0xa053, 0x41d6, {0x90,0x85,0x33,0xa6,0x89,0x14,0x46,0x65}};
constexpr GUID kCLSID_UIAnimationTransitionFactory = {0x8a9b1cdd, 0xfcd7, 0x419c, {0x8b,0x44,0x42,0xfd,0x17,0xdb,0x18,0x87}};
constexpr GUID kIID_IUIAnimationTransitionFactory  = {0xfcd91e03, 0x3e3b, 0x45ad, {0xbb,0xb1,0x6d,0xfc,0x81,0x53,0x74,0x3d}};
// EnableAnimationTimerEventHandler (0x7310) QueryInterface()s the manager for
// this IID (0x33d2a0 in mfc140u .rdata = IID_IUIAnimationTimerUpdateHandler).
constexpr GUID kIID_IUIAnimationTimerUpdateHandler = {0x195509b7, 0x5d5e, 0x4e3e, {0xb2,0x78,0xee,0x37,0x59,0xb3,0x67,0xad}};
// IIDs the handler objects below answer to in QueryInterface (uianimation.h).
constexpr GUID kIID_IUnknown                          = {0x00000000, 0x0000, 0x0000, {0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};
constexpr GUID kIID_IUIAnimationManagerEventHandler   = {0x783321ed, 0x78a3, 0x4366, {0xb5,0x74,0x6a,0xf6,0x07,0xa6,0x47,0x88}};
constexpr GUID kIID_IUIAnimationTimerEventHandler     = {0x274a7dea, 0xd771, 0x4095, {0xab,0xbd,0x8d,0xf7,0xab,0xd2,0x3c,0xe3}};
constexpr GUID kIID_IUIAnimationStoryboardEventHandler= {0x3d5c9008, 0xec7c, 0x4364, {0x9f,0x8a,0x9a,0xf3,0xc5,0x8c,0xba,0xe6}};
constexpr GUID kIID_IUIAnimationPriorityComparison    = {0x83fa9b74, 0x5f86, 0x4618, {0xbc,0x6a,0xa2,0xfa,0xc1,0x9b,0x3f,0x44}};

// Dispatch through the controller's OWN vtable (MSVC layout: either the one
// this file plants in the constructor, or the client-derived class's).  This
// is how retail's handler objects reach the virtuals -- `mov (%rcx),%rdx;
// mov 0xNN(%rdx),%rax; call *guard` -- so a client override is honoured.
// Slot numbers are the retail vftable slots listed in the file header.
enum : size_t {
    kSlot_OnAnimationManagerStatusChanged  = 12,
    kSlot_OnStoryboardStatusChanged        = 15,
    kSlot_OnStoryboardUpdated              = 16,
    kSlot_OnHasPriorityCancel              = 17,
    kSlot_OnHasPriorityConclude            = 18,
    kSlot_OnHasPriorityTrim                = 19,
    kSlot_OnHasPriorityCompress            = 20,
    kSlot_OnAnimationTimerPreUpdate        = 21,
    kSlot_OnAnimationTimerPostUpdate       = 22,
    kSlot_OnAnimationTimerRenderingTooSlow = 23,
};
template <typename Fn>
Fn ControllerSlot(void* pController, size_t slot) {
    return reinterpret_cast<Fn>((*static_cast<void***>(pController))[slot]);
}

// ---------------------------------------------------------------------------
// The UIAnimation callback objects retail creates in the Enable* methods.
// Each retail object is 0x18 bytes -- vfptr, CAnimationController*
// m_pAnimationController (+0x08), LONG refcount (+0x10) -- allocated with
// operator new, refcount starting at 0 and AddRef()ed once by the CComPtr the
// Enable* method holds.  Their AddRef/Release (0x7d80 / 0x7d60, shared by all
// seven vtables) are plain non-interlocked increments / decrements; Release
// deletes at zero and then returns 0.  QueryInterface (e.g. 0x8240 for the
// manager handler) answers IID_IUnknown and the handler's own IID, returns
// E_POINTER for a NULL out-pointer and E_NOINTERFACE (with *ppv = NULL)
// otherwise.  The layout of these objects is private to this DLL (a client
// only ever sees them through the interface), so they are ordinary mingw COM
// objects here; only the behaviour is transcribed.
// ---------------------------------------------------------------------------
template <class Itf>
struct AnimationHandlerBase : Itf {
    void* m_pAnimationController = nullptr;
    LONG  m_nRef = 0;
    const GUID& m_iid;

    explicit AnimationHandlerBase(const GUID& iid) : m_iid(iid) {}
    virtual ~AnimationHandlerBase() = default;

    STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override {
        if (ppv == nullptr) {
            return E_POINTER;
        }
        if (IsEqualGUID(riid, kIID_IUnknown) || IsEqualGUID(riid, m_iid)) {
            *ppv = static_cast<Itf*>(this);
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }
    STDMETHODIMP_(ULONG) AddRef() override {
        return static_cast<ULONG>(++m_nRef);
    }
    STDMETHODIMP_(ULONG) Release() override {
        const LONG n = --m_nRef;
        if (n == 0) {
            delete this;
            return 0;
        }
        return static_cast<ULONG>(n);
    }
};

// CAnimationManagerEventHandler (retail vftable 0x2daa40; OnManagerStatusChanged
// 0x82d0): forwards to controller slot 12 when a controller is attached and
// returns S_OK either way.
struct AnimationManagerEventHandler : AnimationHandlerBase<IUIAnimationManagerEventHandler> {
    AnimationManagerEventHandler() : AnimationHandlerBase(kIID_IUIAnimationManagerEventHandler) {}
    STDMETHODIMP OnManagerStatusChanged(UI_ANIMATION_MANAGER_STATUS newStatus,
                                        UI_ANIMATION_MANAGER_STATUS previousStatus) override {
        if (m_pAnimationController != nullptr) {
            ControllerSlot<void (MS_ABI*)(void*, int, int)>(
                m_pAnimationController, kSlot_OnAnimationManagerStatusChanged)(
                m_pAnimationController, newStatus, previousStatus);
        }
        return S_OK;
    }
};

// CAnimationTimerEventHandler (retail vftable 0x2daa08; OnPreUpdate 0x81b0,
// OnPostUpdate 0x81e0, OnRenderingTooSlow 0x8210): E_FAIL when no controller
// is attached, otherwise forward to slot 21 / 22 / 23 and return S_OK.
struct AnimationTimerEventHandler : AnimationHandlerBase<IUIAnimationTimerEventHandler> {
    AnimationTimerEventHandler() : AnimationHandlerBase(kIID_IUIAnimationTimerEventHandler) {}
    STDMETHODIMP OnPreUpdate() override {
        if (m_pAnimationController == nullptr) {
            return E_FAIL;
        }
        ControllerSlot<void (MS_ABI*)(void*)>(m_pAnimationController, kSlot_OnAnimationTimerPreUpdate)(
            m_pAnimationController);
        return S_OK;
    }
    STDMETHODIMP OnPostUpdate() override {
        if (m_pAnimationController == nullptr) {
            return E_FAIL;
        }
        ControllerSlot<void (MS_ABI*)(void*)>(m_pAnimationController, kSlot_OnAnimationTimerPostUpdate)(
            m_pAnimationController);
        return S_OK;
    }
    STDMETHODIMP OnRenderingTooSlow(UINT32 fps) override {
        if (m_pAnimationController == nullptr) {
            return E_FAIL;
        }
        ControllerSlot<void (MS_ABI*)(void*, unsigned int)>(
            m_pAnimationController, kSlot_OnAnimationTimerRenderingTooSlow)(m_pAnimationController, fps);
        return S_OK;
    }
};

// CAnimationStoryboardEventHandler (retail vftable 0x2da9d8;
// OnStoryboardStatusChanged 0x8060, OnStoryboardUpdated 0x80d0): E_FAIL when
// no controller is attached or FindAnimationGroup(storyboard) (a direct call
// to 0x6e60) finds no group; otherwise forward to slot 15 / 16, return S_OK.
struct AnimationStoryboardEventHandler : AnimationHandlerBase<IUIAnimationStoryboardEventHandler> {
    AnimationStoryboardEventHandler() : AnimationHandlerBase(kIID_IUIAnimationStoryboardEventHandler) {}
    STDMETHODIMP OnStoryboardStatusChanged(IUIAnimationStoryboard* storyboard,
                                           UI_ANIMATION_STORYBOARD_STATUS newStatus,
                                           UI_ANIMATION_STORYBOARD_STATUS previousStatus) override {
        if (m_pAnimationController == nullptr) {
            return E_FAIL;
        }
        void* pGroup = impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__PEAUIUIAnimationStoryboard___Z(
            m_pAnimationController, storyboard);
        if (pGroup == nullptr) {
            return E_FAIL;
        }
        ControllerSlot<void (MS_ABI*)(void*, void*, int, int)>(
            m_pAnimationController, kSlot_OnStoryboardStatusChanged)(
            m_pAnimationController, pGroup, newStatus, previousStatus);
        return S_OK;
    }
    STDMETHODIMP OnStoryboardUpdated(IUIAnimationStoryboard* storyboard) override {
        if (m_pAnimationController == nullptr) {
            return E_FAIL;
        }
        void* pGroup = impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__PEAUIUIAnimationStoryboard___Z(
            m_pAnimationController, storyboard);
        if (pGroup == nullptr) {
            return E_FAIL;
        }
        ControllerSlot<void (MS_ABI*)(void*, void*)>(m_pAnimationController, kSlot_OnStoryboardUpdated)(
            m_pAnimationController, pGroup);
        return S_OK;
    }
};

// CUIAnimationPriorityComparisonHandler<Method> (four retail vftables:
// Cancel 0x2da980 -> slot 17, Compress 0x2da958 -> slot 20, Conclude
// 0x2da930 -> slot 18, Trim 0x2da908 -> slot 19; HasPriority bodies 0x7c00 /
// 0x7b90 / 0x7b20 / 0x7ab0, which differ only in the slot they dispatch).
// E_FAIL when no controller is attached or either storyboard maps to no group;
// otherwise S_OK when the virtual returns non-zero, S_FALSE when it returns 0.
template <size_t kSlot>
struct AnimationPriorityComparisonHandler : AnimationHandlerBase<IUIAnimationPriorityComparison> {
    AnimationPriorityComparisonHandler() : AnimationHandlerBase(kIID_IUIAnimationPriorityComparison) {}
    STDMETHODIMP HasPriority(IUIAnimationStoryboard* pStoryboardScheduled,
                             IUIAnimationStoryboard* pStoryboardNew,
                             UI_ANIMATION_PRIORITY_EFFECT priorityEffect) override {
        if (m_pAnimationController == nullptr) {
            return E_FAIL;
        }
        void* pGroupScheduled = impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__PEAUIUIAnimationStoryboard___Z(
            m_pAnimationController, pStoryboardScheduled);
        if (pGroupScheduled == nullptr) {
            return E_FAIL;
        }
        void* pGroupNew = impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__PEAUIUIAnimationStoryboard___Z(
            m_pAnimationController, pStoryboardNew);
        if (pGroupNew == nullptr) {
            return E_FAIL;
        }
        const int bResult = ControllerSlot<int (MS_ABI*)(void*, void*, void*, int)>(
            m_pAnimationController, kSlot)(m_pAnimationController, pGroupScheduled, pGroupNew, priorityEffect);
        return bResult != 0 ? S_OK : S_FALSE;
    }
};

// Shared shape of the "create handler, hand it to a Set*Handler slot, drop
// our reference" sequence that every Enable* method performs (e.g. 0x7273..
// 0x72fa inside EnableAnimationManagerEvent 0x7230): operator new, refcount 0,
// AddRef (the CComPtr), m_pAnimationController = this, the Set* call with
// the handler, result = SUCCEEDED(hr), then Release.  Retail tests the result
// of operator new; what it does when that is NULL differs per caller:
// EnableAnimationManagerEvent (0x72de), EnableAnimationTimerEventHandler
// (0x7444) and EnableStoryboardEventHandler (0x7556) skip the Set* call and
// report FALSE, while EnablePriorityComparisonHandler (0x76df) still makes the
// Set* call with a NULL handler and then judges only SUCCEEDED(hr) (0x76ff:
// a successful NULL install falls through to the next flag, 0x770b) --
// kSetNullOnAllocFailure selects between the two.
template <class Handler, class Itf, bool kSetNullOnAllocFailure, typename SetFn>
int InstallHandler(void* pController, SetFn set) {
    Handler* pHandler = new (std::nothrow) Handler();
    Itf* pItf = nullptr;
    if (pHandler != nullptr) {
        pHandler->AddRef();
        pHandler->m_pAnimationController = pController;
        pItf = pHandler;
    } else if (!kSetNullOnAllocFailure) {
        return FALSE;
    }
    const HRESULT hr = set(pItf);
    const int bResult = SUCCEEDED(hr) ? TRUE : FALSE;
    if (pItf != nullptr) {
        pItf->Release();
    }
    return bResult;
}

// Lazy CoCreateInstance into one of the four CComPtr slots -- the body shared
// (modulo slot / CLSID / IID) by the four accessors listed in the file header:
//   if (!m_bIsValid) return NULL;
//   if (slot) return slot;
//   hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, iid, &slot);
//   if (FAILED(hr)) { if (slot) { p = slot; slot = NULL; p->Release(); }
//                     if (hr == REGDB_E_CLASSNOTREG || hr == E_NOINTERFACE) m_bIsValid = FALSE;
//                     return NULL; }
//   return slot;
// CoCreateInstance is the ole32 import at IAT slot 0x2c78a8 (mfc140u).
void* LazyCreateAnimationObject(S_CAnimationController* self, void** slot, const GUID& clsid, const GUID& iid) {
    if (self->m_bIsValid == 0) {
        return nullptr;
    }
    if (*slot != nullptr) {
        return *slot;
    }
    const HRESULT hr = ::CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, iid, slot);
    if (FAILED(hr)) {
        if (*slot != nullptr) {
            IUnknown* pUnk = static_cast<IUnknown*>(*slot);
            *slot = nullptr;
            pUnk->Release();
        }
        if (hr == REGDB_E_CLASSNOTREG || hr == E_NOINTERFACE) {
            self->m_bIsValid = 0;
        }
        return nullptr;
    }
    return *slot;
}

void ReleaseAnimationSlot(void** slot) {
    IUnknown* pUnk = static_cast<IUnknown*>(*slot);
    if (pUnk != nullptr) {
        pUnk->Release();
    }
}

// Side-table stand-in for retail's pGroup->m_pStoryboard (+0x80) read in
// EnableStoryboardEventHandler: the storyboard registry in this file maps
// storyboard -> group, so the group's storyboard is the reverse lookup.
// Caveat: RegisterStoryboard only records a pointer that is already a
// registered group token, and CAnimationGroup.cpp passes its own `this`
// (never a token), so in the current tree the registry stays EMPTY -- this
// lookup returns NULL and EnableStoryboardEventHandler returns FALSE until
// CAnimationGroup gets its retail layout (see headerRequests).
void* FindStoryboardForAnimationGroup(void* pGroup);

} // namespace

// MSVC-layout vtables planted by the constructor and the sentinel keyframe
// (defined at the end of this file, next to the bodies they point at).
namespace openmfc { namespace detail { namespace canimationcontroller {
extern void* const g_CAnimationController_vtbl[25];
extern void* const g_CBaseKeyFrameSentinel_vtbl[6];
} } }
namespace {
struct AnimationControllerGroupToken {
    unsigned int id = 0;
    void* owner = nullptr;   // the CAnimationController* that AddAnimationObject() ran on
};

struct AnimationControllerObjectToken {};

struct AnimationControllerKeyframeToken {
    void* creator = nullptr;
};

struct AnimationControllerObjectAssociation {
    void* object = nullptr;
    void* group = nullptr;
};

struct CAnimationControllerObjectImpl : CObject {};

std::mutex g_animationControllerStateMutex;
std::uint32_t g_animationControllerNextGroupId = 1;
std::unordered_set<unsigned int> g_animationControllerGroupIds;
std::unordered_map<void*, unsigned int> g_animationControllerGroupByHandle;
std::unordered_map<unsigned int, AnimationControllerGroupToken*> g_animationControllerGroupById;
std::unordered_map<void*, void*> g_animationControllerGroupByObject;
std::unordered_map<void*, void*> g_animationControllerGroupByStoryboard;
std::unordered_map<void*, AnimationControllerObjectAssociation> g_animationControllerObjectByVariable;
std::unordered_map<void*, AnimationControllerKeyframeToken> g_animationControllerKeyframes;
std::unordered_map<unsigned int, double> g_animationControllerGroupScheduleTimes;
std::unordered_set<unsigned int> g_animationControllerActiveGroups;

unsigned int AllocateAnimationControllerGroupId() {
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    const unsigned int id = g_animationControllerNextGroupId++;
    g_animationControllerGroupIds.insert(id);
    return id;
}

void RegisterAnimationControllerGroup(AnimationControllerGroupToken* token) {
    if (!token) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerGroupByHandle[token] = token->id;
    g_animationControllerGroupById[token->id] = token;
}

void UnregisterAnimationControllerGroup(AnimationControllerGroupToken* token) {
    if (!token) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    auto handleIt = g_animationControllerGroupByHandle.find(token);
    if (handleIt == g_animationControllerGroupByHandle.end()) {
        return;
    }
    const unsigned int groupId = handleIt->second;
    g_animationControllerGroupById.erase(groupId);
    g_animationControllerGroupIds.erase(groupId);
    g_animationControllerGroupByHandle.erase(handleIt);
    g_animationControllerGroupScheduleTimes.erase(groupId);
    g_animationControllerActiveGroups.erase(groupId);

    for (auto it = g_animationControllerGroupByObject.begin();
         it != g_animationControllerGroupByObject.end();) {
        if (it->second == token) {
            it = g_animationControllerGroupByObject.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = g_animationControllerGroupByStoryboard.begin();
         it != g_animationControllerGroupByStoryboard.end();) {
        if (it->second == token) {
            it = g_animationControllerGroupByStoryboard.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = g_animationControllerObjectByVariable.begin();
         it != g_animationControllerObjectByVariable.end();) {
        if (it->second.group == token) {
            it = g_animationControllerObjectByVariable.erase(it);
        } else {
            ++it;
        }
    }
}

bool IsValidAnimationControllerGroup(unsigned int groupId) {
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    return groupId != 0 && g_animationControllerGroupIds.count(groupId) != 0;
}

void* FindAnimationControllerGroupById(unsigned int groupId) {
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    auto it = g_animationControllerGroupById.find(groupId);
    return it == g_animationControllerGroupById.end() ? nullptr : it->second;
}

void RegisterAnimationControllerObject(void* pAnimationObject, void* pGroup) {
    if (!pAnimationObject || !pGroup) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerGroupByObject[pAnimationObject] = pGroup;
}

void UnregisterKeyframesForGroupId(unsigned int groupId) {
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    for (auto it = g_animationControllerKeyframes.begin(); it != g_animationControllerKeyframes.end();) {
        if (reinterpret_cast<uintptr_t>(it->second.creator) == groupId) {
            it = g_animationControllerKeyframes.erase(it);
        } else {
            ++it;
        }
    }
}

void RemoveAnimationControllerGroup(AnimationControllerGroupToken* token) {
    if (!token) {
        return;
    }
    UnregisterAnimationControllerGroup(token);
    UnregisterKeyframesForGroupId(token->id);
}

void RemoveAnimationControllerGroupById(unsigned int groupId) {
    if (groupId == 0) {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
        auto it = g_animationControllerGroupById.find(groupId);
        if (it == g_animationControllerGroupById.end()) {
            return;
        }
        auto* token = it->second;
        if (token != nullptr) {
            g_animationControllerGroupIds.erase(groupId);
            g_animationControllerGroupById.erase(it);
            g_animationControllerGroupByHandle.erase(token);
            g_animationControllerGroupScheduleTimes.erase(groupId);
            g_animationControllerActiveGroups.erase(groupId);
            for (auto objectIt = g_animationControllerGroupByObject.begin();
                 objectIt != g_animationControllerGroupByObject.end();) {
                if (objectIt->second == token) {
                    objectIt = g_animationControllerGroupByObject.erase(objectIt);
                } else {
                    ++objectIt;
                }
            }
            for (auto storyboardIt = g_animationControllerGroupByStoryboard.begin();
                 storyboardIt != g_animationControllerGroupByStoryboard.end();) {
                if (storyboardIt->second == token) {
                    storyboardIt = g_animationControllerGroupByStoryboard.erase(storyboardIt);
                } else {
                    ++storyboardIt;
                }
            }
            for (auto variableIt = g_animationControllerObjectByVariable.begin();
                 variableIt != g_animationControllerObjectByVariable.end();) {
                if (variableIt->second.group == token) {
                    variableIt = g_animationControllerObjectByVariable.erase(variableIt);
                } else {
                    ++variableIt;
                }
            }
        }
    }
    UnregisterKeyframesForGroupId(groupId);
}

void RegisterAnimationControllerKeyframe(void* pKeyframe, void* creator) {
    if (!pKeyframe) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerKeyframes[pKeyframe] = AnimationControllerKeyframeToken{creator};
}

void RegisterAnimationControllerVariable(
    void* pAnimationVariable, void* pAnimationObject, void* pAnimationGroup) {
    if (!pAnimationVariable || !pAnimationObject || !pAnimationGroup) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerObjectByVariable[pAnimationVariable] = {
        pAnimationObject, pAnimationGroup
    };
}
} // namespace

namespace openmfc { namespace detail { namespace animationcontroller {
void RegisterStoryboard(void* animationGroup, void* storyboard) {
    if (!animationGroup || !storyboard) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    if (g_animationControllerGroupByHandle.count(animationGroup) != 0) {
        g_animationControllerGroupByStoryboard[storyboard] = animationGroup;
    }
}
} } }

namespace {
void* FindStoryboardForAnimationGroup(void* pGroup) {
    if (pGroup == nullptr) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    for (const auto& entry : g_animationControllerGroupByStoryboard) {
        if (entry.second == pGroup) {
            return entry.first;
        }
    }
    return nullptr;
}
} // namespace

// Symbol: ??0CAnimationController@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationController__QEAA_XZ(void* pThis) {
    // Retail RVA 0x6620 (mfc140u), transcribed store for store (see the file
    // header): vfptr, m_bIsValid = 1, the four CComPtrs and m_pRelatedWnd
    // NULL, the CList sub-object's vfptr + five NULL/zero members and
    // m_nBlockSize = 10.  Deviation: the CList vfptr slot at +0x38 is left
    // NULL -- this DLL has no MSVC-layout CList<CAnimationGroup*> vtable to
    // plant, and never puts a node in the list (groups live in the side table).
    // The CObject base constructor is not run: retail does not call it either
    // (CObject has no data; the only vptr store is the one below).
    S_CAnimationController* self = static_cast<S_CAnimationController*>(pThis);
    self->vfptr = const_cast<void**>(&openmfc::detail::canimationcontroller::g_CAnimationController_vtbl[0]);
    self->m_bIsValid = TRUE;
    self->m_pAnimationManager = nullptr;
    self->m_pTransitionLibrary = nullptr;
    self->m_pAnimationTimer = nullptr;
    self->m_pTransitionFactory = nullptr;
    self->m_pRelatedWnd = nullptr;
    self->m_lst_vfptr = nullptr;
    self->m_lst_pNodeHead = nullptr;
    self->m_lst_pNodeTail = nullptr;
    self->m_lst_nCount = 0;
    self->m_lst_pNodeFree = nullptr;
    self->m_lst_pBlocks = nullptr;
    self->m_lst_nBlockSize = 10;
    return pThis;
}
// Symbol: ??1CAnimationController@@UEAA@XZ
extern "C" void MS_ABI impl___1CAnimationController__UEAA_XZ(void* pThis) {
    // Retail RVA 0x66d0 (mfc140u): store the CAnimationController vftable,
    // RemoveAllAnimationGroups() (direct call, 0x6bc0), destroy
    // m_lstAnimationGroups (CList vftable store + RemoveAll, 0x8350), then
    // release m_pTransitionFactory (+0x28), m_pAnimationTimer (+0x20),
    // m_pTransitionLibrary (+0x18), m_pAnimationManager (+0x10), in that
    // order, each only if non-NULL, through IUnknown slot 2 (Release).
    // Deviation: the CList teardown is skipped -- this DLL never populates
    // the list (see the file header).  RemoveAllAnimationGroups here is the
    // side-table version; it removes only the group tokens this controller
    // created (see its comment), as retail's walk of the controller's own
    // list does.
    S_CAnimationController* self = static_cast<S_CAnimationController*>(pThis);
    self->vfptr = const_cast<void**>(&openmfc::detail::canimationcontroller::g_CAnimationController_vtbl[0]);
    impl__RemoveAllAnimationGroups_CAnimationController__QEAAXXZ(pThis);
    ReleaseAnimationSlot(&self->m_pTransitionFactory);
    ReleaseAnimationSlot(&self->m_pAnimationTimer);
    ReleaseAnimationSlot(&self->m_pTransitionLibrary);
    ReleaseAnimationSlot(&self->m_pAnimationManager);
}
// Symbol: ?IsAnimationInProgress@CAnimationController@@UEAAHXZ
extern "C" int MS_ABI impl__IsAnimationInProgress_CAnimationController__UEAAHXZ(void* /*pThis*/) {
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    return (!g_animationControllerActiveGroups.empty() ||
            !g_animationControllerGroupScheduleTimes.empty()) ? TRUE : FALSE;
}

// Symbol: ?AddAnimationObject@CAnimationController@@QEAAPEAVCAnimationGroup@@PEAVCAnimationBaseObject@@@Z
extern "C" void* MS_ABI impl__AddAnimationObject_CAnimationController__QEAAPEAVCAnimationGroup__PEAVCAnimationBaseObject___Z(
    void* pThis, void* pAnimationObject) {
    if (pAnimationObject == nullptr) {
        return nullptr;
    }
    auto* pGroup = new (std::nothrow) AnimationControllerGroupToken();
    if (pGroup == nullptr) {
        return nullptr;
    }
    pGroup->id = AllocateAnimationControllerGroupId();
    pGroup->owner = pThis;
    RegisterAnimationControllerGroup(pGroup);
    RegisterAnimationControllerObject(pAnimationObject, pGroup);
    return pGroup;
}

// Symbol: ?AddKeyframeToGroup@CAnimationController@@QEAAHIPEAVCBaseKeyFrame@@@Z
extern "C" int MS_ABI impl__AddKeyframeToGroup_CAnimationController__QEAAHIPEAVCBaseKeyFrame___Z(
    void* /*pThis*/, unsigned int groupId, void* pKeyframe) {
    if (groupId == 0 || pKeyframe == nullptr) {
        return FALSE;
    }
    if (!IsValidAnimationControllerGroup(groupId)) {
        return FALSE;
    }
    RegisterAnimationControllerKeyframe(pKeyframe,
        reinterpret_cast<void*>(static_cast<uintptr_t>(groupId)));
    return TRUE;
}

// Symbol: ?AnimateGroup@CAnimationController@@QEAAHIH@Z
extern "C" int MS_ABI impl__AnimateGroup_CAnimationController__QEAAHIH_Z(
    void* /*pThis*/, unsigned int groupId, int /*bAnimateNow*/) {
    if (!IsValidAnimationControllerGroup(groupId)) {
        return FALSE;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerActiveGroups.insert(groupId);
    return TRUE;
}

// Symbol: ?CleanUpGroup@CAnimationController@@IEAAXPEAVCAnimationGroup@@@Z
extern "C" void MS_ABI impl__CleanUpGroup_CAnimationController__IEAAXPEAVCAnimationGroup___Z(
    void* /*pThis*/, void* pGroup) {
    // Retail RVA 0x7130 (mfc140u), the same bytes as OnAfterSchedule (ICF):
    //   if (pGroup) { pGroup->RemoveKeyframes();     // direct call, 0x6120
    //                 pGroup->RemoveTransitions(); } // direct call, 0x62c0
    // The group itself is NOT unregistered or freed here.  CleanUpGroup(UINT)
    // (0x7160) is only FindAnimationGroup(nGroupID) followed by the same two
    // calls and does not free the group either; the exports that unlink a
    // group from m_lstAnimationGroups and run ~CAnimationGroup (0x5fd0) +
    // delete are RemoveAnimationGroup(UINT) (0x6b10) and
    // RemoveAllAnimationGroups (0x6bc0).  The previous body here deleted the
    // token, leaving a caller's CAnimationGroup* dangling.  Side-table
    // analogue of the two CAnimationGroup calls: drop the keyframes
    // registered for the group (the same mapping RemoveTransitions(UINT)
    // below uses); the two CAnimationGroup thunks are argument-less no-ops in
    // this tree, see headerRequests.
    if (pGroup == nullptr) {
        return;
    }
    UnregisterKeyframesForGroupId(static_cast<AnimationControllerGroupToken*>(pGroup)->id);
}

// Symbol: ?CleanUpGroup@CAnimationController@@QEAAXI@Z
extern "C" void MS_ABI impl__CleanUpGroup_CAnimationController__QEAAXI_Z(void* pThis, unsigned int groupId) {
    // Retail RVA 0x7160 (mfc140u):
    //   pGroup = FindAnimationGroup(nGroupID);       // direct call, 0x6e30
    //   if (pGroup) { pGroup->RemoveKeyframes();     // direct call, 0x6120
    //                 pGroup->RemoveTransitions(); } // direct call, 0x62c0
    // i.e. the CAnimationGroup* overload above applied to the looked-up group;
    // the group stays registered.  The previous body here unregistered the
    // group id instead, which retail does not do (RemoveAnimationGroup(UINT),
    // 0x6b10, is the export that does).
    void* pGroup = impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__I_Z(pThis, groupId);
    if (pGroup != nullptr) {
        impl__CleanUpGroup_CAnimationController__IEAAXPEAVCAnimationGroup___Z(pThis, pGroup);
    }
}

// Symbol: ?CreateKeyframe@CAnimationController@@QEAAPEAVCKeyFrame@@IPEAVCBaseKeyFrame@@N@Z
extern "C" void* MS_ABI impl__CreateKeyframe_CAnimationController__QEAAPEAVCKeyFrame__IPEAVCBaseKeyFrame__N_Z(
    void* /*pThis*/, unsigned int groupId, void* pBaseKeyframe, double /*time*/) {
    if (!pBaseKeyframe || !IsValidAnimationControllerGroup(groupId)) {
        return nullptr;
    }
    auto* keyframe = new (std::nothrow) AnimationControllerKeyframeToken();
    if (keyframe == nullptr) {
        return nullptr;
    }
    RegisterAnimationControllerKeyframe(keyframe,
        reinterpret_cast<void*>(static_cast<uintptr_t>(groupId)));
    return keyframe;
}

// Symbol: ?CreateKeyframe@CAnimationController@@QEAAPEAVCKeyFrame@@IPEAVCBaseTransition@@@Z
extern "C" void* MS_ABI impl__CreateKeyframe_CAnimationController__QEAAPEAVCKeyFrame__IPEAVCBaseTransition___Z(
    void* /*pThis*/, unsigned int /*groupId*/, void* pTransition) {
    if (!pTransition) {
        return nullptr;
    }
    auto* keyframe = new (std::nothrow) AnimationControllerKeyframeToken();
    if (keyframe == nullptr) {
        return nullptr;
    }
    RegisterAnimationControllerKeyframe(keyframe, pTransition);
    return keyframe;
}

// Symbol: ?CreateObject@CAnimationController@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CAnimationController__SAPEAVCObject__XZ() {
    return new (std::nothrow) CAnimationControllerObjectImpl();
}

// Symbol: ?EnableAnimationManagerEvent@CAnimationController@@UEAAHH@Z
extern "C" int MS_ABI impl__EnableAnimationManagerEvent_CAnimationController__UEAAHH_Z(void* pThis, int bEnable) {
    // Retail RVA 0x7230 (mfc140u):
    //   pMgr = GetUIAnimationManager()  (direct call to 0x6760; non-virtual)
    //   if (!pMgr) return FALSE;
    //   if (!bEnable) { pMgr->SetManagerEventHandler(NULL); return TRUE; }
    //   create CAnimationManagerEventHandler, hr = pMgr->SetManagerEventHandler(h),
    //   release h, return SUCCEEDED(hr).
    // SetManagerEventHandler is IUIAnimationManager slot 15 (call *0x78(%rax)).
    IUIAnimationManager* pMgr = static_cast<IUIAnimationManager*>(
        impl__GetUIAnimationManager_CAnimationController__QEAAPEAUIUIAnimationManager__XZ(pThis));
    if (pMgr == nullptr) {
        return FALSE;
    }
    if (!bEnable) {
        pMgr->SetManagerEventHandler(nullptr);
        return TRUE;
    }
    return InstallHandler<AnimationManagerEventHandler, IUIAnimationManagerEventHandler, false>(
        pThis, [pMgr](IUIAnimationManagerEventHandler* h) { return pMgr->SetManagerEventHandler(h); });
}

// Symbol: ?EnableAnimationTimerEventHandler@CAnimationController@@UEAAHHW4__MIDL___MIDL_itf_UIAnimation_0000_0012_0001@@@Z
extern "C" int MS_ABI impl__EnableAnimationTimerEventHandler_CAnimationController__UEAAHHW4__MIDL___MIDL_itf_UIAnimation_0000_0012_0001___Z(
    void* pThis, int bEnable, int idleBehavior) {
    // Retail RVA 0x7310 (mfc140u):
    //   pTimer = GetUIAnimationTimer()    (direct call, 0x68a0)
    //   pMgr   = GetUIAnimationManager()  (direct call, 0x6760)
    //   pUpdate = NULL;
    //   if (pMgr && FAILED(pMgr->QueryInterface(IID_IUIAnimationTimerUpdateHandler, &pUpdate))) pUpdate = NULL;
    //   if (!pTimer || !pUpdate) result = FALSE;
    //   else if (!bEnable) { pTimer->SetTimerEventHandler(NULL);            // slot 4 (0x20)
    //                        pTimer->SetTimerUpdateHandler(NULL, idleBehavior);  // slot 3 (0x18)
    //                        result = TRUE; }
    //   else { create CAnimationTimerEventHandler h;
    //          hr = pTimer->SetTimerEventHandler(h);
    //          if (FAILED(hr)) result = FALSE;
    //          else { if (pUpdate) pTimer->SetTimerUpdateHandler(pUpdate, idleBehavior); result = TRUE; }
    //          release h; }
    //   if (pUpdate) pUpdate->Release();
    //   return result;
    IUIAnimationTimer* pTimer = static_cast<IUIAnimationTimer*>(
        impl__GetUIAnimationTimer_CAnimationController__QEAAPEAUIUIAnimationTimer__XZ(pThis));
    IUIAnimationManager* pMgr = static_cast<IUIAnimationManager*>(
        impl__GetUIAnimationManager_CAnimationController__QEAAPEAUIUIAnimationManager__XZ(pThis));
    IUIAnimationTimerUpdateHandler* pUpdate = nullptr;
    if (pMgr != nullptr) {
        if (FAILED(pMgr->QueryInterface(kIID_IUIAnimationTimerUpdateHandler, reinterpret_cast<void**>(&pUpdate)))) {
            pUpdate = nullptr;
        }
    }
    int bResult = FALSE;
    if (pTimer != nullptr && pUpdate != nullptr) {
        if (!bEnable) {
            pTimer->SetTimerEventHandler(nullptr);
            pTimer->SetTimerUpdateHandler(nullptr, static_cast<UI_ANIMATION_IDLE_BEHAVIOR>(idleBehavior));
            bResult = TRUE;
        } else {
            bResult = InstallHandler<AnimationTimerEventHandler, IUIAnimationTimerEventHandler, false>(
                pThis, [&](IUIAnimationTimerEventHandler* h) {
                    const HRESULT hr = pTimer->SetTimerEventHandler(h);
                    if (SUCCEEDED(hr) && pUpdate != nullptr) {
                        pTimer->SetTimerUpdateHandler(pUpdate, static_cast<UI_ANIMATION_IDLE_BEHAVIOR>(idleBehavior));
                    }
                    return hr;
                });
        }
    }
    if (pUpdate != nullptr) {
        pUpdate->Release();
    }
    return bResult;
}

// Symbol: ?EnablePriorityComparisonHandler@CAnimationController@@UEAAH_K@Z
extern "C" int MS_ABI impl__EnablePriorityComparisonHandler_CAnimationController__UEAAH_K_Z(
    void* pThis, unsigned long long dwHandlerType) {
    // Retail RVA 0x7590 (mfc140u).  IUIAnimationManager slots used: 16
    // SetCancelPriorityComparison (0x80), 17 SetTrimPriorityComparison (0x88),
    // 18 SetCompressPriorityComparison (0x90), 19 SetConcludePriorityComparison
    // (0x98).  Only the LOW BYTE of dwHandlerType is ever tested (`test %sil`).
    //   pMgr = GetUIAnimationManager(); if (!pMgr) return FALSE;
    //   if ((BYTE)dw == 0 /* UI_ANIMATION_PHT_NONE */) {
    //       SetCancel(NULL); SetCompress(NULL); SetConclude(NULL); SetTrim(NULL); return TRUE; }
    //   if (dw & 0x10) SetCancel(NULL);     if (dw & 0x20) SetConclude(NULL);
    //   if (dw & 0x40) SetCompress(NULL);   if (dw & 0x80) SetTrim(NULL);
    //   if (dw & 0x01) { h = new Cancel handler;   hr = SetCancel(h);   release; if FAILED return FALSE; }
    //   if (dw & 0x04) { h = new Compress handler; hr = SetCompress(h); release; if FAILED return FALSE; }
    //   if (dw & 0x02) { h = new Conclude handler; hr = SetConclude(h); release; if FAILED return FALSE; }
    //   if (dw & 0x08) { h = new Trim handler;     hr = SetTrim(h);     release; if FAILED return FALSE; }
    //   return TRUE;
    // (each "new" is NULL-tested; a NULL result still makes the Set* call,
    // with a NULL handler, and the FAILED test then decides -- see
    // InstallHandler.)
    // (the four vftables 0x2da980 / 0x2da958 / 0x2da930 / 0x2da908 are the
    // Cancel / Compress / Conclude / Trim instantiations; see the handler
    // class comment for how each was matched to its controller slot.)
    IUIAnimationManager* pMgr = static_cast<IUIAnimationManager*>(
        impl__GetUIAnimationManager_CAnimationController__QEAAPEAUIUIAnimationManager__XZ(pThis));
    if (pMgr == nullptr) {
        return FALSE;
    }
    const unsigned int dw = static_cast<unsigned int>(dwHandlerType & 0xFF);
    if (dw == 0) {
        pMgr->SetCancelPriorityComparison(nullptr);
        pMgr->SetCompressPriorityComparison(nullptr);
        pMgr->SetConcludePriorityComparison(nullptr);
        pMgr->SetTrimPriorityComparison(nullptr);
        return TRUE;
    }
    if (dw & 0x10) {
        pMgr->SetCancelPriorityComparison(nullptr);
    }
    if (dw & 0x20) {
        pMgr->SetConcludePriorityComparison(nullptr);
    }
    if (dw & 0x40) {
        pMgr->SetCompressPriorityComparison(nullptr);
    }
    if (dw & 0x80) {
        pMgr->SetTrimPriorityComparison(nullptr);
    }
    if (dw & 0x01) {
        if (!InstallHandler<AnimationPriorityComparisonHandler<kSlot_OnHasPriorityCancel>, IUIAnimationPriorityComparison, true>(
                pThis, [pMgr](IUIAnimationPriorityComparison* h) { return pMgr->SetCancelPriorityComparison(h); })) {
            return FALSE;
        }
    }
    if (dw & 0x04) {
        if (!InstallHandler<AnimationPriorityComparisonHandler<kSlot_OnHasPriorityCompress>, IUIAnimationPriorityComparison, true>(
                pThis, [pMgr](IUIAnimationPriorityComparison* h) { return pMgr->SetCompressPriorityComparison(h); })) {
            return FALSE;
        }
    }
    if (dw & 0x02) {
        if (!InstallHandler<AnimationPriorityComparisonHandler<kSlot_OnHasPriorityConclude>, IUIAnimationPriorityComparison, true>(
                pThis, [pMgr](IUIAnimationPriorityComparison* h) { return pMgr->SetConcludePriorityComparison(h); })) {
            return FALSE;
        }
    }
    if (dw & 0x08) {
        if (!InstallHandler<AnimationPriorityComparisonHandler<kSlot_OnHasPriorityTrim>, IUIAnimationPriorityComparison, true>(
                pThis, [pMgr](IUIAnimationPriorityComparison* h) { return pMgr->SetTrimPriorityComparison(h); })) {
            return FALSE;
        }
    }
    return TRUE;
}

// Symbol: ?EnableStoryboardEventHandler@CAnimationController@@UEAAHIH@Z
extern "C" int MS_ABI impl__EnableStoryboardEventHandler_CAnimationController__UEAAHIH_Z(
    void* pThis, unsigned int nGroupID, int bEnable) {
    // Retail RVA 0x7490 (mfc140u):
    //   pGroup = FindAnimationGroup(nGroupID)   (direct call, 0x6e30)
    //   if (!pGroup) return FALSE;
    //   pSB = pGroup->m_pStoryboard (+0x80); if (!pSB) return FALSE;
    //   if (!bEnable) { pSB->SetStoryboardEventHandler(NULL); return TRUE; }   // slot 19 (0x98)
    //   create CAnimationStoryboardEventHandler h; hr = pSB->SetStoryboardEventHandler(h);
    //   release h; return SUCCEEDED(hr).
    // Deviation: groups in this file are side-table tokens, not retail
    // CAnimationGroup objects, so the +0x80 read is replaced by the reverse
    // lookup in the storyboard registry (see FindStoryboardForAnimationGroup
    // -- which, in the current tree, is never populated, so the bEnable
    // branches below are reached only once CAnimationGroup registers real
    // storyboards).
    void* pGroup = impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__I_Z(pThis, nGroupID);
    if (pGroup == nullptr) {
        return FALSE;
    }
    IUIAnimationStoryboard* pStoryboard =
        static_cast<IUIAnimationStoryboard*>(FindStoryboardForAnimationGroup(pGroup));
    if (pStoryboard == nullptr) {
        return FALSE;
    }
    if (!bEnable) {
        pStoryboard->SetStoryboardEventHandler(nullptr);
        return TRUE;
    }
    return InstallHandler<AnimationStoryboardEventHandler, IUIAnimationStoryboardEventHandler, false>(
        pThis, [pStoryboard](IUIAnimationStoryboardEventHandler* h) { return pStoryboard->SetStoryboardEventHandler(h); });
}

// Symbol: ?FindAnimationGroup@CAnimationController@@QEAAPEAVCAnimationGroup@@I@Z
extern "C" void* MS_ABI impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__I_Z(
    void* /*pThis*/, unsigned int groupId) {
    return FindAnimationControllerGroupById(groupId);
}

// Symbol: ?FindAnimationGroup@CAnimationController@@QEAAPEAVCAnimationGroup@@PEAUIUIAnimationStoryboard@@@Z
extern "C" void* MS_ABI impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__PEAUIUIAnimationStoryboard___Z(
    void* /*pThis*/, void* pStoryboard) {
    if (pStoryboard == nullptr) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    auto it = g_animationControllerGroupByStoryboard.find(pStoryboard);
    return it == g_animationControllerGroupByStoryboard.end() ? nullptr : it->second;
}

// Symbol: ?FindAnimationObject@CAnimationController@@QEAAHPEAUIUIAnimationVariable@@PEAPEAVCAnimationBaseObject@@PEAPEAVCAnimationGroup@@@Z
extern "C" int MS_ABI impl__FindAnimationObject_CAnimationController__QEAAHPEAUIUIAnimationVariable__PEAPEAVCAnimationBaseObject__PEAPEAVCAnimationGroup___Z(
    void* /*pThis*/, void* pAnimationVariable,
    void** pAnimationObject, void** pAnimationGroup) {
    if (pAnimationObject == nullptr || pAnimationGroup == nullptr) {
        return E_POINTER;
    }
    *pAnimationObject = nullptr;
    *pAnimationGroup = nullptr;
    if (pAnimationVariable == nullptr) {
        return FALSE;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    auto it = g_animationControllerObjectByVariable.find(pAnimationVariable);
    if (it == g_animationControllerObjectByVariable.end()) {
        return FALSE;
    }
    *pAnimationObject = it->second.object;
    *pAnimationGroup = it->second.group;
    return TRUE;
}

// Symbol: ?GetKeyframeStoryboardStart@CAnimationController@@SAPEAVCBaseKeyFrame@@XZ
extern "C" void* MS_ABI impl__GetKeyframeStoryboardStart_CAnimationController__SAPEAVCBaseKeyFrame__XZ() {
    // Retail RVA 0x7900 (mfc140u): `lea g_KeyframeStoryboardStart, %rax; ret`
    // -- returns the address of the process-wide static CBaseKeyFrame at
    // 0x3b1ba8, whose contents are transcribed into the sentinel below (see
    // S_CBaseKeyFrame).  It is the value CAnimationGroup::RemoveKeyframes
    // (0x6120) compares keyframe pointers against so as never to delete it.
    static S_CBaseKeyFrame s_keyframeStoryboardStart = {
        const_cast<void**>(&openmfc::detail::canimationcontroller::g_CBaseKeyFrameSentinel_vtbl[0]),
        UI_ANIMATION_KEYFRAME_STORYBOARD_START,
        TRUE,   // m_bAdded
        TRUE,   // m_bIsKeyframeAtOffset
    };
    return &s_keyframeStoryboardStart;
}

// Symbol: ?GetUIAnimationManager@CAnimationController@@QEAAPEAUIUIAnimationManager@@XZ
extern "C" void* MS_ABI impl__GetUIAnimationManager_CAnimationController__QEAAPEAUIUIAnimationManager__XZ(
    void* pThis) {
    // Retail RVA 0x6760 (mfc140u): the lazy-CoCreateInstance body documented at
    // LazyCreateAnimationObject, on m_pAnimationManager (+0x10) with
    // CLSID_UIAnimationManager / IID_IUIAnimationManager.
    S_CAnimationController* self = static_cast<S_CAnimationController*>(pThis);
    return LazyCreateAnimationObject(self, &self->m_pAnimationManager,
                                     kCLSID_UIAnimationManager, kIID_IUIAnimationManager);
}

// Symbol: ?GetUIAnimationTimer@CAnimationController@@QEAAPEAUIUIAnimationTimer@@XZ
extern "C" void* MS_ABI impl__GetUIAnimationTimer_CAnimationController__QEAAPEAUIUIAnimationTimer__XZ(
    void* pThis) {
    // Retail RVA 0x68a0 (mfc140u): same body on m_pAnimationTimer (+0x20) with
    // CLSID_UIAnimationTimer / IID_IUIAnimationTimer.
    S_CAnimationController* self = static_cast<S_CAnimationController*>(pThis);
    return LazyCreateAnimationObject(self, &self->m_pAnimationTimer,
                                     kCLSID_UIAnimationTimer, kIID_IUIAnimationTimer);
}

// Symbol: ?GetUITransitionFactory@CAnimationController@@QEAAPEAUIUIAnimationTransitionFactory@@XZ
extern "C" void* MS_ABI impl__GetUITransitionFactory_CAnimationController__QEAAPEAUIUIAnimationTransitionFactory__XZ(
    void* pThis) {
    // Retail RVA 0x6940 (mfc140u): same body on m_pTransitionFactory (+0x28)
    // with CLSID_UIAnimationTransitionFactory / IID_IUIAnimationTransitionFactory.
    S_CAnimationController* self = static_cast<S_CAnimationController*>(pThis);
    return LazyCreateAnimationObject(self, &self->m_pTransitionFactory,
                                     kCLSID_UIAnimationTransitionFactory, kIID_IUIAnimationTransitionFactory);
}

// Symbol: ?GetUITransitionLibrary@CAnimationController@@QEAAPEAUIUIAnimationTransitionLibrary@@XZ
extern "C" void* MS_ABI impl__GetUITransitionLibrary_CAnimationController__QEAAPEAUIUIAnimationTransitionLibrary__XZ(
    void* pThis) {
    // Retail RVA 0x6800 (mfc140u): same body on m_pTransitionLibrary (+0x18)
    // with CLSID_UIAnimationTransitionLibrary / IID_IUIAnimationTransitionLibrary.
    S_CAnimationController* self = static_cast<S_CAnimationController*>(pThis);
    return LazyCreateAnimationObject(self, &self->m_pTransitionLibrary,
                                     kCLSID_UIAnimationTransitionLibrary, kIID_IUIAnimationTransitionLibrary);
}

// Symbol: ?OnAfterSchedule@CAnimationController@@MEAAXPEAVCAnimationGroup@@@Z
extern "C" void MS_ABI impl__OnAfterSchedule_CAnimationController__MEAAXPEAVCAnimationGroup___Z(
    void* pThis, void* pGroup) {
    // Retail RVA 0x7130 (mfc140u) -- the same bytes serve the protected
    // CleanUpGroup(CAnimationGroup*) export (identical-COMDAT folding):
    //   if (pGroup) { pGroup->RemoveKeyframes();     // direct call, 0x6120
    //                 pGroup->RemoveTransitions(); } // direct call, 0x62c0
    // Both CAnimationGroup thunks in core/animation/CAnimationGroup.cpp are
    // argument-less placeholders with empty bodies, and the groups this file
    // hands out are side-table tokens, so the retail calls cannot be made yet
    // (see headerRequests).  This is the side-table analogue that its ICF twin
    // above performs: drop the keyframes registered for the group.  Note the
    // keyframe registry in this tree is write-only (nothing reads it back),
    // so the observable effect is bookkeeping only.
    impl__CleanUpGroup_CAnimationController__IEAAXPEAVCAnimationGroup___Z(pThis, pGroup);
}

// Symbol: ?OnAnimationIntegerValueChanged@CAnimationController@@UEAAXPEAVCAnimationGroup@@PEAVCAnimationBaseObject@@PEAUIUIAnimationVariable@@HH@Z
extern "C" void MS_ABI impl__OnAnimationIntegerValueChanged_CAnimationController__UEAAXPEAVCAnimationGroup__PEAVCAnimationBaseObject__PEAUIUIAnimationVariable__HH_Z(
    void* /*pThis*/, void* pAnimationGroup, void* pAnimationObject,
    void* pAnimationVariable, int /*oldValue*/, int /*newValue*/) {
    RegisterAnimationControllerVariable(
        pAnimationVariable, pAnimationObject, pAnimationGroup);
}

// Symbol: ?OnAnimationManagerStatusChanged@CAnimationController@@UEAAXW4__MIDL___MIDL_itf_UIAnimation_0000_0000_0002@@0@Z
extern "C" void MS_ABI impl__OnAnimationManagerStatusChanged_CAnimationController__UEAAXW4__MIDL___MIDL_itf_UIAnimation_0000_0000_0002__0_Z(
    void* pThis, int newStatus, int /*previousStatus*/) {
    // Retail RVA 0x7190 (mfc140u):
    //   if (newStatus != UI_ANIMATION_MANAGER_BUSY /* 1 */) return;
    //   if (m_pRelatedWnd && m_pRelatedWnd->m_hWnd && ::IsWindow(m_hWnd))   // IAT 0x2c7138
    //       ::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE /* 1 */);      // IAT 0x2c7130
    if (newStatus != UI_ANIMATION_MANAGER_BUSY) {
        return;
    }
    S_CAnimationController* self = static_cast<S_CAnimationController*>(pThis);
    if (self->m_pRelatedWnd == nullptr) {
        return;
    }
    HWND hWnd = self->m_pRelatedWnd->m_hWnd;
    if (hWnd == nullptr || !::IsWindow(hWnd)) {
        return;
    }
    ::RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE);
}

// Symbol: ?OnAnimationTimerPostUpdate@CAnimationController@@UEAAXXZ
extern "C" void MS_ABI impl__OnAnimationTimerPostUpdate_CAnimationController__UEAAXXZ(void* pThis) {
    // Retail RVA 0x71f0 (mfc140u):
    //   if (m_pRelatedWnd && m_pRelatedWnd->m_hWnd && ::IsWindow(m_hWnd))   // IAT 0x2c7138
    //       ::InvalidateRect(m_hWnd, NULL, TRUE);                           // IAT 0x2c7128
    S_CAnimationController* self = static_cast<S_CAnimationController*>(pThis);
    if (self->m_pRelatedWnd == nullptr) {
        return;
    }
    HWND hWnd = self->m_pRelatedWnd->m_hWnd;
    if (hWnd == nullptr || !::IsWindow(hWnd)) {
        return;
    }
    ::InvalidateRect(hWnd, nullptr, TRUE);
}

// Symbol: ?OnAnimationTimerPreUpdate@CAnimationController@@UEAAXXZ
extern "C" void MS_ABI impl__OnAnimationTimerPreUpdate_CAnimationController__UEAAXXZ(void* /*pThis*/) {
    // Retail: the shared `ret` at 0x27d0 (mfc140u) -- no-op.
}

// Symbol: ?OnAnimationTimerRenderingTooSlow@CAnimationController@@UEAAXI@Z
extern "C" void MS_ABI impl__OnAnimationTimerRenderingTooSlow_CAnimationController__UEAAXI_Z(
    void* /*pThis*/, unsigned int /*fps*/) {
    // Retail: the shared `ret` at 0x27d0 (mfc140u) -- no-op.
}

// Symbol: ?OnAnimationValueChanged@CAnimationController@@UEAAXPEAVCAnimationGroup@@PEAVCAnimationBaseObject@@PEAUIUIAnimationVariable@@NN@Z
extern "C" void MS_ABI impl__OnAnimationValueChanged_CAnimationController__UEAAXPEAVCAnimationGroup__PEAVCAnimationBaseObject__PEAUIUIAnimationVariable__NN_Z(
    void* /*pThis*/, void* pAnimationGroup, void* pAnimationObject,
    void* pAnimationVariable, double /*oldValue*/, double /*newValue*/) {
    RegisterAnimationControllerVariable(
        pAnimationVariable, pAnimationObject, pAnimationGroup);
}

// Symbol: ?OnBeforeAnimationStart@CAnimationController@@UEAAXPEAVCAnimationGroup@@@Z
extern "C" void MS_ABI impl__OnBeforeAnimationStart_CAnimationController__UEAAXPEAVCAnimationGroup___Z(
    void* /*pThis*/, void* /*pGroup*/) {
    // Retail: the shared `ret` at 0x27d0 (mfc140u) -- no-op.
}

// The four OnHasPriority* virtuals share one retail body at 0x71e0 (mfc140u):
// `xor %eax,%eax; ret` -- they return FALSE ("the scheduled storyboard does
// not have priority").  The previous stubs here returned TRUE, which inverted
// what UIAnimation is told through the priority-comparison handlers.
// Symbol: ?OnHasPriorityCancel@CAnimationController@@UEAAHPEAVCAnimationGroup@@0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001@@@Z
extern "C" int MS_ABI impl__OnHasPriorityCancel_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* /*pThis*/, void* /*pGroupScheduled*/, void* /*pGroupNew*/, int /*priorityEffect*/) {
    return FALSE;
}

// Symbol: ?OnHasPriorityCompress@CAnimationController@@UEAAHPEAVCAnimationGroup@@0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001@@@Z
extern "C" int MS_ABI impl__OnHasPriorityCompress_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* /*pThis*/, void* /*pGroupScheduled*/, void* /*pGroupNew*/, int /*priorityEffect*/) {
    return FALSE;
}

// Symbol: ?OnHasPriorityConclude@CAnimationController@@UEAAHPEAVCAnimationGroup@@0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001@@@Z
extern "C" int MS_ABI impl__OnHasPriorityConclude_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* /*pThis*/, void* /*pGroupScheduled*/, void* /*pGroupNew*/, int /*priorityEffect*/) {
    return FALSE;
}

// Symbol: ?OnHasPriorityTrim@CAnimationController@@UEAAHPEAVCAnimationGroup@@0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001@@@Z
extern "C" int MS_ABI impl__OnHasPriorityTrim_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* /*pThis*/, void* /*pGroupScheduled*/, void* /*pGroupNew*/, int /*priorityEffect*/) {
    return FALSE;
}

// Symbol: ?OnStoryboardStatusChanged@CAnimationController@@UEAAXPEAVCAnimationGroup@@W4__MIDL___MIDL_itf_UIAnimation_0000_0002_0001@@1@Z
extern "C" void MS_ABI impl__OnStoryboardStatusChanged_CAnimationController__UEAAXPEAVCAnimationGroup__W4__MIDL___MIDL_itf_UIAnimation_0000_0002_0001__1_Z(
    void* /*pThis*/, void* /*pGroup*/, int /*newStatus*/, int /*previousStatus*/) {
    // Retail: the shared `ret` at 0x27d0 (mfc140u) -- no-op.
}

// Symbol: ?OnStoryboardUpdated@CAnimationController@@UEAAXPEAVCAnimationGroup@@@Z
extern "C" void MS_ABI impl__OnStoryboardUpdated_CAnimationController__UEAAXPEAVCAnimationGroup___Z(
    void* /*pThis*/, void* /*pGroup*/) {
    // Retail: the shared `ret` at 0x27d0 (mfc140u) -- no-op.
}

// Symbol: ?RemoveAllAnimationGroups@CAnimationController@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllAnimationGroups_CAnimationController__QEAAXXZ(
    void* pThis) {
    // Retail RVA 0x6bc0 (mfc140u) walks THIS controller's m_lstAnimationGroups
    // (+0x38), destroys and frees each group (0x5fd0 + sized delete of 0x98),
    // then tail-jumps to CList::RemoveAll (0x8350).  The side-table analogue
    // removes only the tokens AddAnimationObject() created on this controller;
    // it used to clear the whole process-wide registry, which meant destroying
    // one controller dropped every other controller's groups too.
    std::vector<AnimationControllerGroupToken*> groupsToDelete;
    {
        std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
        for (auto& entry : g_animationControllerGroupByHandle) {
            auto* pGroup = static_cast<AnimationControllerGroupToken*>(entry.first);
            if (pGroup != nullptr && pGroup->owner == pThis) {
                groupsToDelete.push_back(pGroup);
            }
        }
    }
    for (auto* pGroup : groupsToDelete) {
        RemoveAnimationControllerGroup(pGroup);   // takes the lock itself
        delete pGroup;
    }
}

// Symbol: ?RemoveAnimationGroup@CAnimationController@@QEAAXI@Z
extern "C" void MS_ABI impl__RemoveAnimationGroup_CAnimationController__QEAAXI_Z(void* /*pThis*/, unsigned int groupId) {
    if (groupId == 0) {
        return;
    }
    void* pGroupHandle = FindAnimationControllerGroupById(groupId);
    if (pGroupHandle == nullptr) {
        return;
    }
    RemoveAnimationControllerGroupById(groupId);
    delete static_cast<AnimationControllerGroupToken*>(pGroupHandle);
}

// Symbol: ?RemoveAnimationObject@CAnimationController@@QEAAXPEAVCAnimationBaseObject@@H@Z
extern "C" void MS_ABI impl__RemoveAnimationObject_CAnimationController__QEAAXPEAVCAnimationBaseObject__H_Z(
    void* /*pThis*/, void* pAnimationObject, int /*bNoDelete*/) {
    if (pAnimationObject == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerGroupByObject.erase(pAnimationObject);
    for (auto it = g_animationControllerObjectByVariable.begin();
         it != g_animationControllerObjectByVariable.end();) {
        if (it->second.object == pAnimationObject) {
            it = g_animationControllerObjectByVariable.erase(it);
        } else {
            ++it;
        }
    }
}

// Symbol: ?RemoveTransitions@CAnimationController@@QEAAXI@Z
extern "C" void MS_ABI impl__RemoveTransitions_CAnimationController__QEAAXI_Z(void* /*pThis*/, unsigned int groupId) {
    if (!IsValidAnimationControllerGroup(groupId)) {
        return;
    }
    UnregisterKeyframesForGroupId(groupId);
}

// Symbol: ?ScheduleGroup@CAnimationController@@QEAAHIN@Z
extern "C" int MS_ABI impl__ScheduleGroup_CAnimationController__QEAAHIN_Z(
    void* /*pThis*/, unsigned int groupId, double time) {
    if (!IsValidAnimationControllerGroup(groupId)) {
        return FALSE;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerGroupScheduleTimes[groupId] = time;
    return TRUE;
}

// Symbol: ?UpdateAnimationManager@CAnimationController@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateAnimationManager_CAnimationController__UEAAXXZ(
    void* /*pThis*/) {
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);

    // Retail RVA 0x6f80 (mfc140u): pMgr = GetUIAnimationManager(); pTimer =
    // GetUIAnimationTimer(); if both: pTimer->GetTime(&t) (IUIAnimationTimer
    // slot 8) and pMgr->Update(t, NULL) (IUIAnimationManager slot 8).  Retail
    // IsAnimationInProgress (0x6ff0) is pMgr->GetStatus(&s) (slot 11) and
    // returns SUCCEEDED(hr) && s == UI_ANIMATION_MANAGER_BUSY.  The accessors
    // above now create those COM objects, but AnimateGroup / ScheduleGroup in
    // this file are side-table simulations that never hand a storyboard to
    // the real manager, so driving it here would report IDLE for every group
    // this file "animates".  The side-table simulation is therefore kept:
    // treat one update as the completion boundary for groups that were
    // explicitly animated or scheduled, keeping IsAnimationInProgress
    // consistent with AnimateGroup without pretending asynchronous work
    // continues forever.
    for (const auto& scheduled : g_animationControllerGroupScheduleTimes) {
        if (g_animationControllerGroupIds.count(scheduled.first) != 0) {
            g_animationControllerActiveGroups.insert(scheduled.first);
        }
    }
    g_animationControllerGroupScheduleTimes.clear();
    g_animationControllerActiveGroups.clear();
}

// ---------------------------------------------------------------------------
// MSVC-layout vtables.  Slot order for CAnimationController is the retail
// vftable listed in the file header; every slot is a direct call to the
// impl__ thunk above (never a virtual re-dispatch, so no recursion), except
// slots 1-4, which need file-local wrappers.
// ---------------------------------------------------------------------------
namespace openmfc { namespace detail { namespace canimationcontroller {
namespace {
void* MS_ABI vt_vecDelDtor_CAnimationController(void* pThis, unsigned int flags) {
    // Retail scalar deleting destructor at 0x6680 (mfc140u; not an export,
    // and its body tests only flags & 1 and flags & 4 -- no array branch):
    // ~CAnimationController (0x66d0); then if (flags & 1): when !(flags & 4)
    // `free(this)` (ucrt free, IAT 0x2c74e8), when (flags & 4) `call 0x27d0`
    // with edx = 0x70 -- and 0x27d0 in this image is a bare `ret` (bytes
    // C2 00 00), so that variant frees nothing; returns this.  Here the ordinary (flags & 1) case goes through this
    // DLL's sized operator delete, as the other hand-authored vtables in this
    // repo go through its operator delete; the flags & 4 variant is not
    // distinguished.
    impl___1CAnimationController__UEAA_XZ(pThis);
    if (flags & 1) {
        ::operator delete(pThis, sizeof(S_CAnimationController));
    }
    return pThis;
}
void MS_ABI vt_Serialize_CAnimationController(void* pThis, void* pAr) {
    // Retail slot 2 is the shared `ret` (0x27d0): CObject::Serialize is empty.
    if (pThis == nullptr || pAr == nullptr) return;
    static_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}
void MS_ABI vt_AssertValid_CAnimationController(const void* pThis) {
    if (pThis == nullptr) return;
    static_cast<const CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI vt_Dump_CAnimationController(const void* pThis, void* /*dc*/) {
    if (pThis == nullptr) return;
    static_cast<const CObject*>(pThis)->CObject::Dump();
}
void* MS_ABI vt_vecDelDtor_CBaseKeyFrameSentinel(void* pThis, unsigned int /*flags*/) {
    // The only object that carries this vtable is the static sentinel returned
    // by GetKeyframeStoryboardStart; it owns nothing and is never on the heap,
    // so neither a destructor body nor a free is run here (retail's own
    // ??_GCBaseKeyFrame would `free` a static if a client ever deleted it --
    // that is a client bug in both DLLs; here it is a no-op instead of a fault).
    return pThis;
}
} // namespace

extern void* const g_CAnimationController_vtbl[25] = {
    (void*)&impl__GetRuntimeClass_CAnimationController__UEBAPEAUCRuntimeClass__XZ,  //  0 CObject::GetRuntimeClass
    (void*)&vt_vecDelDtor_CAnimationController,                                     //  1 {scalar deleting dtor}
    (void*)&vt_Serialize_CAnimationController,                                      //  2 CObject::Serialize
    (void*)&vt_AssertValid_CAnimationController,                                    //  3 CObject::AssertValid
    (void*)&vt_Dump_CAnimationController,                                           //  4 CObject::Dump
    (void*)&impl__EnableAnimationManagerEvent_CAnimationController__UEAAHH_Z,      //  5
    (void*)&impl__EnablePriorityComparisonHandler_CAnimationController__UEAAH_K_Z, //  6
    (void*)&impl__EnableStoryboardEventHandler_CAnimationController__UEAAHIH_Z,    //  7
    (void*)&impl__EnableAnimationTimerEventHandler_CAnimationController__UEAAHHW4__MIDL___MIDL_itf_UIAnimation_0000_0012_0001___Z, // 8
    (void*)&impl__UpdateAnimationManager_CAnimationController__UEAAXXZ,            //  9
    (void*)&impl__IsAnimationInProgress_CAnimationController__UEAAHXZ,             // 10
    (void*)&impl__OnBeforeAnimationStart_CAnimationController__UEAAXPEAVCAnimationGroup___Z, // 11
    (void*)&impl__OnAnimationManagerStatusChanged_CAnimationController__UEAAXW4__MIDL___MIDL_itf_UIAnimation_0000_0000_0002__0_Z, // 12
    (void*)&impl__OnAnimationValueChanged_CAnimationController__UEAAXPEAVCAnimationGroup__PEAVCAnimationBaseObject__PEAUIUIAnimationVariable__NN_Z, // 13
    (void*)&impl__OnAnimationIntegerValueChanged_CAnimationController__UEAAXPEAVCAnimationGroup__PEAVCAnimationBaseObject__PEAUIUIAnimationVariable__HH_Z, // 14
    (void*)&impl__OnStoryboardStatusChanged_CAnimationController__UEAAXPEAVCAnimationGroup__W4__MIDL___MIDL_itf_UIAnimation_0000_0002_0001__1_Z, // 15
    (void*)&impl__OnStoryboardUpdated_CAnimationController__UEAAXPEAVCAnimationGroup___Z, // 16
    (void*)&impl__OnHasPriorityCancel_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z,   // 17
    (void*)&impl__OnHasPriorityConclude_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z, // 18
    (void*)&impl__OnHasPriorityTrim_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z,     // 19
    (void*)&impl__OnHasPriorityCompress_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z, // 20
    (void*)&impl__OnAnimationTimerPreUpdate_CAnimationController__UEAAXXZ,         // 21
    (void*)&impl__OnAnimationTimerPostUpdate_CAnimationController__UEAAXXZ,        // 22
    (void*)&impl__OnAnimationTimerRenderingTooSlow_CAnimationController__UEAAXI_Z, // 23
    (void*)&impl__OnAfterSchedule_CAnimationController__MEAAXPEAVCAnimationGroup___Z, // 24
};

// CBaseKeyFrame's retail vftable (0x1802dae80 in mfc140u): 0 GetRuntimeClass,
// 1 {dtor} 0x86d0, 2-4 the shared `ret`, 5 AddToStoryboard (0x3a60) -- the
// order afxanimationcontroller.h declares.  Used only by the sentinel.
extern void* const g_CBaseKeyFrameSentinel_vtbl[6] = {
    (void*)&impl__GetRuntimeClass_CBaseKeyFrame__UEBAPEAUCRuntimeClass__XZ,  // 0 CObject::GetRuntimeClass
    (void*)&vt_vecDelDtor_CBaseKeyFrameSentinel,                             // 1 {dtor}
    (void*)&vt_Serialize_CAnimationController,                               // 2 CObject::Serialize
    (void*)&vt_AssertValid_CAnimationController,                             // 3 CObject::AssertValid
    (void*)&vt_Dump_CAnimationController,                                    // 4 CObject::Dump
    (void*)&impl__AddToStoryboard_CBaseKeyFrame__UEAAHPEAUIUIAnimationStoryboard__H_Z, // 5 AddToStoryboard
};
} } }
