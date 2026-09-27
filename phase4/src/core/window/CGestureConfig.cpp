// CGestureConfig — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Every body in this file is transcribed from the retail mfc140u.dll exports
// (RVAs below are function entries in mfc140u, read with disas.py --u).
//
// Layout.  include/openmfc only forward-declares CGestureConfig, so the class
// is modelled here as void* pThis over an in-file shadow struct.  The retail
// declaration (atlmfc/include/afxwin.h, `class CGestureConfig : public CObject`)
// has exactly two data members, PGESTURECONFIG m_pConfigs and int m_nConfigs;
// the retail constructor (RVA 0x2926a0, mfc140u) stores the vftable at +0x00,
// m_nConfigs = 5 at +0x10 and the operator-new result at +0x08, and the scalar
// deleting destructor (RVA 0x292740, mfc140u) passes 0x18 as the object size,
// which pins the layout asserted below.  Array elements are indexed with a
// stride of 12 (`lea (i,i,2)` then `*4`), i.e. sizeof(GESTURECONFIG), with
// dwID / dwWant / dwBlock read at element +0 / +4 / +8.
//
// vftable.  Retail's CGestureConfig vftable (0x180336c00 in mfc140u) has five
// slots: [0] CObject::GetRuntimeClass (RVA 0x37a0; the class has no
// DECLARE_DYNAMIC), [1] the scalar deleting destructor (RVA 0x292740, not
// exported), [2..4] Serialize / AssertValid / Dump, all the folded `ret` at
// RVA 0x27d0.  It is reproduced below as g_CGestureConfig_vtbl, following the
// pattern of featurepack/docking/CRecentDockSiteInfo.cpp.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstdlib>

// ---- sibling thunks (definitions seen in the tree; signatures from them)
// core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const CObject* pThis);
// detail/MemcoreSupport.cpp -- OpenMFC's exported ::operator new (malloc-backed)
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);

// ---- this class's own thunks, referenced by the constructor / vftable below
extern "C" void MS_ABI impl___1CGestureConfig__UEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__Modify_CGestureConfig__QEAAHKKK_Z(void* pThis, unsigned long dwID,
                                                              unsigned long dwWant, unsigned long dwBlock);
extern "C" void MS_ABI impl__EnablePan_CGestureConfig__QEAAXHK_Z(void* pThis, int bEnable, unsigned long dwFlags);

namespace {

struct S_GestureConfig {
    void*           vfptr;      // 0x00
    GESTURECONFIG*  m_pConfigs; // 0x08
    int             m_nConfigs; // 0x10
};
static_assert(sizeof(GESTURECONFIG) == 12, "retail indexes m_pConfigs with a 12-byte stride");
static_assert(offsetof(GESTURECONFIG, dwID) == 0, "dwID at element +0");
static_assert(offsetof(GESTURECONFIG, dwWant) == 4, "dwWant at element +4");
static_assert(offsetof(GESTURECONFIG, dwBlock) == 8, "dwBlock at element +8");
static_assert(offsetof(S_GestureConfig, m_pConfigs) == 0x08, "ctor: mov %rax,0x8(%rbx)");
static_assert(offsetof(S_GestureConfig, m_nConfigs) == 0x10, "ctor: movl $0x5,0x10(%rcx)");
static_assert(sizeof(S_GestureConfig) == 0x18, "scalar deleting dtor passes size 0x18");

inline S_GestureConfig* Self(void* p) { return static_cast<S_GestureConfig*>(p); }
inline const S_GestureConfig* Self(const void* p) { return static_cast<const S_GestureConfig*>(p); }

// Scalar deleting destructor, retail RVA 0x292740 (mfc140u), vftable slot 1:
//   vfptr = vftable; free(m_pConfigs);            // the ~CGestureConfig body, inlined
//   if (flags & 1) { if (flags & 4) <folded ret 0x27d0>(this, 0x18); else free(this); }
//   return this;
// (Both frees are import slot 0x1802c74e8 of mfc140u, api-ms-win-crt-heap free.)
void* MS_ABI GestureConfig_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CGestureConfig__UEAA_XZ(pThis);
    if ((flags & 1) && !(flags & 4)) std::free(pThis);
    return pThis;
}
// Slots 2..4 (Serialize / AssertValid / Dump) are the folded `ret` at RVA 0x27d0.
void MS_ABI GestureConfig_NoOp(void* /*pThis*/, void* /*arg*/) {}

#define VT(fn) reinterpret_cast<const void*>(&fn)
const void* const g_CGestureConfig_vtbl[5] = {
    VT(impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ),
    VT(GestureConfig_ScalarDeletingDtor),
    VT(GestureConfig_NoOp),
    VT(GestureConfig_NoOp),
    VT(GestureConfig_NoOp),
};
#undef VT

} // namespace

// CGestureConfig::CGestureConfig -- retail RVA 0x2926a0 (mfc140u):
//   vfptr = vftable;
//   m_nConfigs = 5;
//   m_pConfigs = operator new(0x3c);              // call ??2@YAPEAX_K@Z, 5 * 12 bytes
//   for (int i = 0; i < m_nConfigs; i++) {        // m_nConfigs re-read each iteration
//       m_pConfigs[i].dwID    = i + 3;            // GID_ZOOM .. GID_PRESSANDTAP
//       m_pConfigs[i].dwWant  = (m_pConfigs[i].dwID != 4) ? 1 : 0;  // GC_ALLGESTURES except GID_PAN
//       m_pConfigs[i].dwBlock = 0;
//   }
//   Modify(5 /*GID_ROTATE*/, 0, 1 /*GC_ROTATE*/); // call to RVA 0x2927c0
//   EnablePan(TRUE, 0x1a);                        // call to RVA 0x2928f0; 0x1a = vertical|gutter|inertia
//   return this;
// Allocation goes through OpenMFC's exported operator new, which is
// malloc-backed, so the destructor's free() below pairs with it as retail's does.
// Retail does not null-test the allocation; neither does this body.  DEVIATION in
// the failure mode only: retail's operator new (RVA 0x27f0, mfc140u) retries
// malloc through the new-handler pointer it loads from +0x50 of the state object
// returned by the call to AfxGetModuleThreadState (RVA 0x133a20, mfc140u),
// whereas OpenMFC's is a single malloc.
// Symbol: ??0CGestureConfig@@QEAA@XZ
extern "C" void* MS_ABI impl___0CGestureConfig__QEAA_XZ(void* pThis) {
    S_GestureConfig* s = Self(pThis);
    s->vfptr = const_cast<void*>(static_cast<const void*>(g_CGestureConfig_vtbl));
    s->m_nConfigs = 5;
    s->m_pConfigs = static_cast<GESTURECONFIG*>(impl___2_YAPEAX_K_Z(0x3c));
    for (int i = 0; i < s->m_nConfigs; i++) {
        s->m_pConfigs[i].dwID = static_cast<DWORD>(i + 3);
        s->m_pConfigs[i].dwWant = (s->m_pConfigs[i].dwID != 4) ? 1u : 0u;
        s->m_pConfigs[i].dwBlock = 0;
    }
    impl__Modify_CGestureConfig__QEAAHKKK_Z(pThis, 5, 0, 1);
    impl__EnablePan_CGestureConfig__QEAAXHK_Z(pThis, TRUE, 0x1a);
    return pThis;
}

// CGestureConfig::~CGestureConfig -- retail RVA 0x2927a0 (mfc140u):
//   vfptr = vftable;
//   tail-jump free(m_pConfigs);                   // import slot 0x1802c74e8 (mfc140u), CRT free
// Symbol: ??1CGestureConfig@@UEAA@XZ
extern "C" void MS_ABI impl___1CGestureConfig__UEAA_XZ(void* pThis) {
    S_GestureConfig* s = Self(pThis);
    s->vfptr = const_cast<void*>(static_cast<const void*>(g_CGestureConfig_vtbl));
    std::free(s->m_pConfigs);
}

// CGestureConfig::EnablePan(BOOL bEnable, DWORD dwFlags) -- retail RVA 0x2928f0 (mfc140u):
//   if (!bEnable) {
//       Modify(4 /*GID_PAN*/, 0, 0x1e);           // block all four sub-flags
//   } else {
//       DWORD mask = (dwFlags & 1 /*GC_PAN*/) ? 0x1e : dwFlags;
//       // SSE over the constant {2,4,8,0x10}: OR of the flags present in mask,
//       // and OR of the flags absent from it.
//       Modify(4, mask & 0x1e, ~mask & 0x1e);
//   }                                             // tail-jump to Modify (RVA 0x2927c0)
// Note GC_PAN (bit 0) itself is never passed in dwWant or dwBlock.
// Signature corrected: the generated list had no `this`.
// Symbol: ?EnablePan@CGestureConfig@@QEAAXHK@Z
extern "C" void MS_ABI impl__EnablePan_CGestureConfig__QEAAXHK_Z(void* pThis, int bEnable, unsigned long dwFlags) {
    const unsigned long kAll = 0x1e; // GC_PAN_WITH_SINGLE_FINGER_VERTICALLY|HORIZONTALLY|GUTTER|INERTIA
    if (!bEnable) {
        impl__Modify_CGestureConfig__QEAAHKKK_Z(pThis, 4, 0, kAll);
        return;
    }
    const unsigned long mask = (dwFlags & 1) ? kAll : dwFlags;
    impl__Modify_CGestureConfig__QEAAHKKK_Z(pThis, 4, mask & kAll, ~mask & kAll);
}

// The four single-flag Enable* methods share one retail shape:
//   Modify(<GID>, bEnable != 0, bEnable == 0);    // tail-jump to Modify (RVA 0x2927c0)
// i.e. want GC_xxx (== 1) when enabling, block it when disabling.

// CGestureConfig::EnablePressAndTap -- retail RVA 0x2928d0 (mfc140u), GID 7 (GID_PRESSANDTAP).
// Signature corrected: the generated list had no `this`.
// Symbol: ?EnablePressAndTap@CGestureConfig@@QEAAXH@Z
extern "C" void MS_ABI impl__EnablePressAndTap_CGestureConfig__QEAAXH_Z(void* pThis, int bEnable) {
    impl__Modify_CGestureConfig__QEAAHKKK_Z(pThis, 7, bEnable != 0, bEnable == 0);
}

// CGestureConfig::EnableRotate -- retail RVA 0x292890 (mfc140u), GID 5 (GID_ROTATE).
// Signature corrected: the generated list had no `this`.
// Symbol: ?EnableRotate@CGestureConfig@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableRotate_CGestureConfig__QEAAXH_Z(void* pThis, int bEnable) {
    impl__Modify_CGestureConfig__QEAAHKKK_Z(pThis, 5, bEnable != 0, bEnable == 0);
}

// CGestureConfig::EnableTwoFingerTap -- retail RVA 0x2928b0 (mfc140u), GID 6 (GID_TWOFINGERTAP).
// Signature corrected: the generated list had no `this`.
// Symbol: ?EnableTwoFingerTap@CGestureConfig@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableTwoFingerTap_CGestureConfig__QEAAXH_Z(void* pThis, int bEnable) {
    impl__Modify_CGestureConfig__QEAAHKKK_Z(pThis, 6, bEnable != 0, bEnable == 0);
}

// CGestureConfig::EnableZoom -- retail RVA 0x292870 (mfc140u), GID 3 (GID_ZOOM).
// Signature corrected: the generated list had no `this`.
// Symbol: ?EnableZoom@CGestureConfig@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableZoom_CGestureConfig__QEAAXH_Z(void* pThis, int bEnable) {
    impl__Modify_CGestureConfig__QEAAHKKK_Z(pThis, 3, bEnable != 0, bEnable == 0);
}

// CGestureConfig::Get(DWORD dwID, BOOL bWant) const -- retail RVA 0x292820 (mfc140u):
//   int n = m_nConfigs;                           // read once
//   for (int i = 0; i < n; i++)
//       if (m_pConfigs[i].dwID == dwID)
//           return bWant ? m_pConfigs[i].dwWant : m_pConfigs[i].dwBlock;
//   return (DWORD)-1;                             // `or $0xffffffff,%eax`
// Signature corrected: the generated list had no `this`.
// Symbol: ?Get@CGestureConfig@@QEBAKKH@Z
extern "C" unsigned long MS_ABI impl__Get_CGestureConfig__QEBAKKH_Z(const void* pThis, unsigned long dwID, int bWant) {
    const S_GestureConfig* s = Self(pThis);
    const int n = s->m_nConfigs;
    for (int i = 0; i < n; i++) {
        const GESTURECONFIG& c = s->m_pConfigs[i];
        if (c.dwID == dwID) {
            return bWant ? c.dwWant : c.dwBlock;
        }
    }
    return 0xFFFFFFFFul;
}

// CGestureConfig::Modify(DWORD dwID, DWORD dwWant, DWORD dwBlock) -- retail RVA 0x2927c0 (mfc140u):
//   for (int i = 0; i < m_nConfigs; i++) {        // m_nConfigs re-read each iteration
//       if (m_pConfigs[i].dwID == dwID) {
//           m_pConfigs[i].dwWant  |= dwWant;
//           m_pConfigs[i].dwBlock |= dwBlock;
//           m_pConfigs[i].dwWant  &= ~dwBlock;
//           m_pConfigs[i].dwBlock &= ~dwWant;
//           return TRUE;
//       }
//   }
//   return FALSE;
// Signature corrected: the generated list had no `this`.
// Symbol: ?Modify@CGestureConfig@@QEAAHKKK@Z
extern "C" int MS_ABI impl__Modify_CGestureConfig__QEAAHKKK_Z(void* pThis, unsigned long dwID,
                                                              unsigned long dwWant, unsigned long dwBlock) {
    S_GestureConfig* s = Self(pThis);
    for (int i = 0; i < s->m_nConfigs; i++) {
        GESTURECONFIG& c = s->m_pConfigs[i];
        if (c.dwID == dwID) {
            c.dwWant |= dwWant;
            c.dwBlock |= dwBlock;
            c.dwWant &= ~dwBlock;
            c.dwBlock &= ~dwWant;
            return TRUE;
        }
    }
    return FALSE;
}
