// RuntimeClasses — OpenMFC implementation.
// Sources: global_animation_rtti.cpp, global_controlbar_keyframe_rtti.cpp, global_views_rendertarget_rtti.cpp

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

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define ANIM_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 9 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxwin.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define CONTROLBAR_KEYFRAME_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
CONTROLBAR_KEYFRAME_DESC(CControlBar, 328, 0xFFFF, &CWnd::classCWnd);
CONTROLBAR_KEYFRAME_DESC(CDockBar, 400, 0xFFFF, &classCControlBar);
CONTROLBAR_KEYFRAME_DESC(COleResizeBar, 408, 0xFFFF, &classCControlBar);
CONTROLBAR_KEYFRAME_DESC(CReBar, 336, 0xFFFF, &classCControlBar);
CONTROLBAR_KEYFRAME_DESC(CStatusBar, 336, 0xFFFF, &classCControlBar);
CONTROLBAR_KEYFRAME_DESC(CBaseKeyFrame, 24, 0xFFFF, &CObject::classCObject);
CONTROLBAR_KEYFRAME_DESC(CKeyFrame, 48, 0xFFFF, &classCBaseKeyFrame);
CONTROLBAR_KEYFRAME_DESC(CPreviewView, 536, 0xFFFF, &CScrollView::classCScrollView);
CONTROLBAR_KEYFRAME_DESC(CPreviewViewEx, 5536, 0xFFFF, &classCPreviewView);
#undef CONTROLBAR_KEYFRAME_DESC

#define CONTROLBAR_KEYFRAME_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 9 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxinet.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define VIEWS_RENDERTARGET_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
VIEWS_RENDERTARGET_DESC(CSplitterWnd, 384, 0xFFFF, &CWnd::classCWnd);
VIEWS_RENDERTARGET_DESC(CSplitterWndEx, 384, 0xFFFF, &classCSplitterWnd);
VIEWS_RENDERTARGET_DESC(CBaseTransition, 56, 0xFFFF, &CObject::classCObject);
VIEWS_RENDERTARGET_DESC(CBitmapRenderTarget, 88, 0xFFFF, &CRenderTarget::classCRenderTarget);
VIEWS_RENDERTARGET_DESC(CGopherFileFind, 72, 0xFFFF, &CFileFind::classCFileFind);
VIEWS_RENDERTARGET_DESC(CHtmlEditDoc, 384, 0xFFFF, &CDocument::classCDocument);
VIEWS_RENDERTARGET_DESC(CHtmlEditView, 592, 0xFFFF, &CHtmlView::classCHtmlView);
VIEWS_RENDERTARGET_DESC(CHwndRenderTarget, 88, 0xFFFF, &CRenderTarget::classCRenderTarget);
VIEWS_RENDERTARGET_DESC(CMDITabProxyWnd, 240, 0xFFFF, &CWnd::classCWnd);
#undef VIEWS_RENDERTARGET_DESC

#define VIEWS_RENDERTARGET_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

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
#undef ANIM_GETTERS
#define CONTROLBAR_KEYFRAME_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CBaseKeyFrame@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CBaseKeyFrame@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CBaseKeyFrame,
           impl__GetThisClass_CBaseKeyFrame__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CBaseKeyFrame__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CKeyFrame@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CKeyFrame@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CKeyFrame,
           impl__GetThisClass_CKeyFrame__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CKeyFrame__UEBAPEAUCRuntimeClass__XZ)
#undef CONTROLBAR_KEYFRAME_GETTERS
#define VIEWS_RENDERTARGET_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CBaseTransition@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CBaseTransition@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CBaseTransition,
           impl__GetThisClass_CBaseTransition__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CBaseTransition__UEBAPEAUCRuntimeClass__XZ)
#undef VIEWS_RENDERTARGET_GETTERS
