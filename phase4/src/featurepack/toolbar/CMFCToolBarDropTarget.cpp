// CMFCToolBarDropTarget — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Retail (afxtoolbardroptarget.h): class CMFCToolBarDropTarget : public
// COleDropTarget { CMFCToolBar* m_pOwner; ... }.  Every body below is
// transcribed from the retail export; RVAs are mfc140u.dll export entries,
// resolved through the export table by ordinal.
//
// Layout (retail): COleDropTarget occupies 0x00..0x67 (retail's COleDropTarget
// ctor, RVA 0x25a810 mfc140u, writes up to its XDropTarget vptr at +0x60),
// m_pOwner is at +0x68 (the ctor, Register and the four handlers all access
// +0x68; the dtor does not touch it), sizeof == 0x70 (112).
// include/openmfc/afxmfc.h models the class as an opaque 112-byte struct, so
// m_pOwner is addressed through the accessor below and pinned here.
//
// Owner dispatch -- deliberate deviation.  Retail forwards every drag
// notification to the owning toolbar through its vftable: slot 0x7a8 (245)
// OnDrop, 0x7b0 (246) OnDragEnter, 0x7b8 (247) OnDragLeave, 0x7c0 (248)
// OnDragOver -- names read from the CMFCToolBar vftable 0x1803157c8
// (mfc140u).  A CMFCToolBar built by OpenMFC (??0CMFCToolBar@@QEAA@XZ is a
// placement-new in featurepack/toolbar/Thunks.cpp) carries a GCC-generated
// vtable in which those byte offsets name unrelated functions, so the owner
// is called through the CMFCToolBar impl__ thunks instead, non-virtually.
// Consequence: an override of these four in a class derived from CMFCToolBar
// is NOT reached from here; retail would reach it.  Among mfc140u's own
// exports that means ?OnDragOver@CMFCPopupMenuBar@@ and
// ?OnDragOver@CMFCOutlookBarPane@@ (the only CMFCToolBar-derived overrides of
// the four in the export table), plus any client subclass override.
//
// Embedded drop targets -- known hazard outside this file.  OpenMFC's C++
// CMFCToolBar::CMFCToolBar() (featurepack/toolbar/CMFCToolBar.cpp) memsets
// everything from m_bLocked (0x10b8) onward, which includes m_DropTarget
// (0x1230), and never runs ??0CMFCToolBarDropTarget on it.  A toolbar built
// by OpenMFC therefore holds an unconstructed drop target with a NULL vptr.
// Register below would hand that object to COleDropTarget::Register, whose
// OLE adapter later dispatches virtually through the NULL vptr.  Nothing in
// OpenMFC calls Register today: CMFCToolBar::OnCreate, where retail calls
// m_DropTarget.Register(this), is still a stub.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <type_traits>

namespace {

// Opaque 112-byte mirror from include/openmfc/afxmfc.h (retail sizeof 112).
static_assert(sizeof(CMFCToolBarDropTarget) == 0x70,
              "retail sizeof(CMFCToolBarDropTarget) == 112");
// The base-class ctor thunk placement-constructs OpenMFC's own C++
// COleDropTarget into pThis; it must not extend past +0x68, where retail's
// m_pOwner begins, or it would overwrite that member.
static_assert(sizeof(COleDropTarget) == 0x68,
              "OpenMFC COleDropTarget must fit below m_pOwner (+0x68)");
// CPoint arrives by value in one register under the MS x64 ABI (8-byte
// aggregate); the parameter type below is only ABI-correct if GCC agrees.
static_assert(sizeof(CPoint) == 8 && std::is_trivially_copyable<CPoint>::value,
              "CPoint must be an 8-byte trivially-copyable aggregate");

constexpr std::size_t kOwnerOffset = 0x68;   // CMFCToolBar* m_pOwner

inline CMFCToolBar*& Owner(void* pThis) {
    return *reinterpret_cast<CMFCToolBar**>(static_cast<char*>(pThis) + kOwnerOffset);
}

// The vptr ??0COleDropTarget@@QEAA@XZ installs, recorded by our ctor; the
// dtor re-installs it (see there).
void* g_oleDropTargetVptr = nullptr;

} // namespace

// Base class, core/ole/Thunks.cpp.
extern "C" void* MS_ABI impl___0COleDropTarget__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1COleDropTarget__UEAA_XZ(void* pThis);
extern "C" int   MS_ABI impl__Register_COleDropTarget__QEAAHPEAVCWnd___Z(COleDropTarget* pThis, CWnd* p0);
extern "C" int   MS_ABI impl__IsDataAvailable_COleDataObject__QEAAHGPEAUtagFORMATETC___Z(
    COleDataObject* pThis, unsigned short p0, FORMATETC* p1);
// detail/MfcExceptionsSupport.cpp (retail ENSURE failure path).
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// Owner handlers, featurepack/toolbar/CMFCToolBar.cpp.
extern "C" int MS_ABI impl__OnDrop_CMFCToolBar__MEAAHPEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBar* pThis, COleDataObject* pDataObject, unsigned long dropEffect, CPoint point);
extern "C" unsigned long MS_ABI impl__OnDragEnter_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBar* pThis, COleDataObject* pDataObject, unsigned long dwKeyState, CPoint point);
extern "C" void MS_ABI impl__OnDragLeave_CMFCToolBar__MEAAXXZ(CMFCToolBar* pThis);
extern "C" unsigned long MS_ABI impl__OnDragOver_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBar* pThis, COleDataObject* pDataObject, unsigned long dwKeyState, CPoint point);
// Statics, featurepack/toolbar/StaticData.cpp.
extern "C" std::int32_t  impl__m_bCustomizeMode_CMFCToolBar__1HA;      // 0x3be35c (mfc140u)
extern "C" std::uint16_t impl__m_cFormat_CMFCToolBarButton__2GA;       // 0x3be1d4 (mfc140u)

namespace {

// The gate shared by OnDragEnter / OnDragOver / OnDropEx, identical in all
// three retail bodies:
//     ENSURE(m_pOwner != NULL);        // cmpq $0,0x68(%rcx); je -> AfxThrowInvalidArgException
//     if (!CMFCToolBar::m_bCustomizeMode) return FALSE;
//     if (!pDataObject->IsDataAvailable(CMFCToolBarButton::m_cFormat, NULL)) return FALSE;
// The ENSURE comes first, before the customize-mode test.
inline bool AcceptsToolbarButton(void* pThis, COleDataObject* pDataObject) {
    if (Owner(pThis) == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0) return false;
    return impl__IsDataAvailable_COleDataObject__QEAAHGPEAUtagFORMATETC___Z(
               pDataObject, impl__m_cFormat_CMFCToolBarButton__2GA, nullptr) != 0;
}

} // namespace

// Retail (RVA 0x167380, mfc140u):
//     COleDropTarget::COleDropTarget();          // call 0x18025a810
//     vptr = CMFCToolBarDropTarget::vftable;     // 0x1803179b8 (mfc140u)
//     m_pOwner = NULL;                           // movq $0,0x68(%rbx)
//     return this;
// Deviation: OpenMFC has no CMFCToolBarDropTarget vtable, so the object keeps
// the COleDropTarget vptr the base ctor installed; virtual calls made through
// that vptr reach COleDropTarget's handlers, not the overrides in this file.
// Symbol: ??0CMFCToolBarDropTarget@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarDropTarget__QEAA_XZ(void* pThis) {
    impl___0COleDropTarget__QEAA_XZ(pThis);
    g_oleDropTargetVptr = *static_cast<void**>(pThis);
    Owner(pThis) = nullptr;
    return pThis;
}

// Retail (RVA 0x167400, mfc140u), complete:
//     vptr = CMFCToolBarDropTarget::vftable;     // 0x1803179b8 (mfc140u)
//     jmp COleDropTarget::~COleDropTarget        // 0x18025a8f0
// The ??1COleDropTarget thunk runs `p->~COleDropTarget()`, which dispatches
// through the object's vptr.  Retail's vptr reset is reproduced with the only
// vptr OpenMFC has for this object -- the COleDropTarget one recorded by the
// ctor -- so that dispatch lands on COleDropTarget's own dtor even if a
// derived class had overwritten the vptr.  If the ctor never ran in this
// process the store is skipped.
// Symbol: ??1CMFCToolBarDropTarget@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarDropTarget__UEAA_XZ(void* pThis) {
    if (g_oleDropTargetVptr != nullptr) *static_cast<void**>(pThis) = g_oleDropTargetVptr;
    impl___1COleDropTarget__UEAA_XZ(pThis);
}

// Retail (RVA 0x167420, mfc140u), complete:
//     ENSURE(m_pOwner); if (!m_bCustomizeMode) return 0;
//     if (!pDataObject->IsDataAvailable(CMFCToolBarButton::m_cFormat)) return 0;
//     return m_pOwner->OnDragEnter(pDataObject, dwKeyState, point);   // vslot 0x7b0
// pWnd is not read.  Owner call is non-virtual here -- see file header.
// Symbol: ?OnDragEnter@CMFCToolBarDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragEnter_CMFCToolBarDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KVCPoint___Z(
    void* pThis, CWnd* pWnd, COleDataObject* pDataObject, unsigned long dwKeyState, CPoint point) {
    (void)pWnd;
    if (!AcceptsToolbarButton(pThis, pDataObject)) return DROPEFFECT_NONE;
    return impl__OnDragEnter_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
        Owner(pThis), pDataObject, dwKeyState, point);
}

// Retail (RVA 0x1674a0, mfc140u), complete:
//     ENSURE(m_pOwner != NULL);                  // je -> AfxThrowInvalidArgException
//     m_pOwner->OnDragLeave();                   // tail jmp via vslot 0x7b8
// No customize-mode test here.  pWnd is not read.  Owner call is non-virtual
// here -- see file header.
// Symbol: ?OnDragLeave@CMFCToolBarDropTarget@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnDragLeave_CMFCToolBarDropTarget__UEAAXPEAVCWnd___Z(void* pThis, CWnd* pWnd) {
    (void)pWnd;
    CMFCToolBar* pOwner = Owner(pThis);
    if (pOwner == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    impl__OnDragLeave_CMFCToolBar__MEAAXXZ(pOwner);
}

// Retail (RVA 0x1674d0, mfc140u), complete -- same shape as OnDragEnter:
//     ENSURE(m_pOwner); if (!m_bCustomizeMode) return 0;
//     if (!pDataObject->IsDataAvailable(CMFCToolBarButton::m_cFormat)) return 0;
//     return m_pOwner->OnDragOver(pDataObject, dwKeyState, point);    // vslot 0x7c0
// pWnd is not read.  Owner call is non-virtual here -- see file header.
// Symbol: ?OnDragOver@CMFCToolBarDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragOver_CMFCToolBarDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KVCPoint___Z(
    void* pThis, CWnd* pWnd, COleDataObject* pDataObject, unsigned long dwKeyState, CPoint point) {
    (void)pWnd;
    if (!AcceptsToolbarButton(pThis, pDataObject)) return DROPEFFECT_NONE;
    return impl__OnDragOver_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
        Owner(pThis), pDataObject, dwKeyState, point);
}

// Retail (RVA 0x167550, mfc140u; export ordinal 9680 -- this entry is absent
// from mfc140u_rva_symbols.json but the export table resolves it), complete:
//     ENSURE(m_pOwner); if (!m_bCustomizeMode) return 0;
//     if (!pDataObject->IsDataAvailable(CMFCToolBarButton::m_cFormat)) return 0;
//     return m_pOwner->OnDrop(pDataObject, dropEffect, point)          // vslot 0x7a8
//            ? dropEffect : DROPEFFECT_NONE;                           // neg/sbb/and %esi
// dropEffect is the 4th argument (%r9d); pWnd and dropList (5th, stack) are
// not read.  Owner call is non-virtual here -- see file header.
// Symbol: ?OnDropEx@CMFCToolBarDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KKVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDropEx_CMFCToolBarDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KKVCPoint___Z(
    void* pThis, CWnd* pWnd, COleDataObject* pDataObject, unsigned long dropEffect,
    unsigned long dropList, CPoint point) {
    (void)pWnd; (void)dropList;
    if (!AcceptsToolbarButton(pThis, pDataObject)) return DROPEFFECT_NONE;
    const int bDropped = impl__OnDrop_CMFCToolBar__MEAAHPEAVCOleDataObject__KVCPoint___Z(
        Owner(pThis), pDataObject, dropEffect, point);
    return bDropped ? dropEffect : DROPEFFECT_NONE;
}

// Retail (RVA 0x154d0, mfc140u; export ordinal 12206 -- absent from
// mfc140u_rva_symbols.json, resolved through the export table), complete:
//     m_pOwner = pOwner;                         // mov %rdx,0x68(%rcx)
//     return COleDropTarget::Register(pOwner);   // jmp 0x18025a960, pOwner passed unchanged as CWnd*
// Only safe on a drop target built by the ctor above -- see the embedded
// drop-target hazard in the file header.
// Symbol: ?Register@CMFCToolBarDropTarget@@QEAAHPEAVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__Register_CMFCToolBarDropTarget__QEAAHPEAVCMFCToolBar___Z(void* pThis, CMFCToolBar* pOwner) {
    Owner(pThis) = pOwner;
    return impl__Register_COleDropTarget__QEAAHPEAVCWnd___Z(
        static_cast<COleDropTarget*>(pThis), reinterpret_cast<CWnd*>(pOwner));
}
