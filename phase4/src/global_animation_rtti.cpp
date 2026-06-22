// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for the CAnimation*
// animation wrapper family (14 exports, 7 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory), following the repo's own IMPLEMENT_DYNAMIC convention
// (m_pfnGetBaseClass null, m_pBaseClass set). CAnimationBaseObject and
// CAnimationController derive straight from CObject; the five animation-value
// wrappers derive from CAnimationBaseObject, so m_pBaseClass points at the
// in-file descriptor that the base class's own GetThisClass returns. This
// reproduces the retail RUNTIME_CLASS graph IsKindOf/IsDerivedFrom walk.
// m_nObjectSize is the real MSVC sizeof harvested with cl.exe
// /d1reportSingleClassLayout. GetRuntimeClass returns the static descriptor
// directly (never pThis->GetRuntimeClass(), which would recurse through this
// very export in a drop-in client).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxwin.h"   // CObject + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define ANIM_DESC(Cls, Size, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, (BaseDesc), nullptr }

// Parents-first so each derived descriptor can take its base's address.
ANIM_DESC(CAnimationBaseObject, 40,  &CObject::classCObject);
ANIM_DESC(CAnimationController,  112, &CObject::classCObject);
ANIM_DESC(CAnimationValue,       136, &classCAnimationBaseObject);
ANIM_DESC(CAnimationColor,       328, &classCAnimationBaseObject);
ANIM_DESC(CAnimationPoint,       232, &classCAnimationBaseObject);
ANIM_DESC(CAnimationRect,        440, &classCAnimationBaseObject);
ANIM_DESC(CAnimationSize,        232, &classCAnimationBaseObject);
#undef ANIM_DESC
} // namespace

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define ANIM_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CAnimationBaseObject@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CAnimationBaseObject@@UEBAPEAUCRuntimeClass@@XZ
ANIM_GETTERS(CAnimationBaseObject,
             impl__GetThisClass_CAnimationBaseObject__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CAnimationBaseObject__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CAnimationController@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CAnimationController@@UEBAPEAUCRuntimeClass@@XZ
ANIM_GETTERS(CAnimationController,
             impl__GetThisClass_CAnimationController__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CAnimationController__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CAnimationValue@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CAnimationValue@@UEBAPEAUCRuntimeClass@@XZ
ANIM_GETTERS(CAnimationValue,
             impl__GetThisClass_CAnimationValue__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CAnimationValue__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CAnimationColor@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CAnimationColor@@UEBAPEAUCRuntimeClass@@XZ
ANIM_GETTERS(CAnimationColor,
             impl__GetThisClass_CAnimationColor__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CAnimationColor__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CAnimationPoint@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CAnimationPoint@@UEBAPEAUCRuntimeClass@@XZ
ANIM_GETTERS(CAnimationPoint,
             impl__GetThisClass_CAnimationPoint__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CAnimationPoint__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CAnimationRect@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CAnimationRect@@UEBAPEAUCRuntimeClass@@XZ
ANIM_GETTERS(CAnimationRect,
             impl__GetThisClass_CAnimationRect__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CAnimationRect__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CAnimationSize@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CAnimationSize@@UEBAPEAUCRuntimeClass@@XZ
ANIM_GETTERS(CAnimationSize,
             impl__GetThisClass_CAnimationSize__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CAnimationSize__UEBAPEAUCRuntimeClass__XZ)

#undef ANIM_GETTERS
