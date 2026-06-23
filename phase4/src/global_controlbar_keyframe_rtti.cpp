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

namespace {
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
} // namespace

#define CONTROLBAR_KEYFRAME_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CControlBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CControlBar@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CControlBar,
           impl__GetThisClass_CControlBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CControlBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CDockBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDockBar@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CDockBar,
           impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CDockBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@COleResizeBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleResizeBar@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(COleResizeBar,
           impl__GetThisClass_COleResizeBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleResizeBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CReBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CReBar@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CReBar,
           impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CReBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CStatusBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CStatusBar@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CStatusBar,
           impl__GetThisClass_CStatusBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CStatusBar__UEBAPEAUCRuntimeClass__XZ)
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
// Symbol: ?GetThisClass@CPreviewView@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CPreviewView@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CPreviewView,
           impl__GetThisClass_CPreviewView__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CPreviewView__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CPreviewViewEx@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CPreviewViewEx@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CPreviewViewEx,
           impl__GetThisClass_CPreviewViewEx__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CPreviewViewEx__UEBAPEAUCRuntimeClass__XZ)
#undef CONTROLBAR_KEYFRAME_GETTERS
