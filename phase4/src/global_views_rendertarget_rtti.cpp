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

namespace {
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
} // namespace

#define VIEWS_RENDERTARGET_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CSplitterWnd@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CSplitterWnd@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CSplitterWnd,
           impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CSplitterWnd__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CSplitterWndEx@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CSplitterWndEx@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CSplitterWndEx,
           impl__GetThisClass_CSplitterWndEx__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CSplitterWndEx__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CBaseTransition@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CBaseTransition@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CBaseTransition,
           impl__GetThisClass_CBaseTransition__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CBaseTransition__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CBitmapRenderTarget@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CBitmapRenderTarget@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CBitmapRenderTarget,
           impl__GetThisClass_CBitmapRenderTarget__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CBitmapRenderTarget__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CGopherFileFind@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CGopherFileFind@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CGopherFileFind,
           impl__GetThisClass_CGopherFileFind__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CGopherFileFind__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CHtmlEditDoc@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CHtmlEditDoc@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CHtmlEditDoc,
           impl__GetThisClass_CHtmlEditDoc__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CHtmlEditDoc__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CHtmlEditView@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CHtmlEditView@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CHtmlEditView,
           impl__GetThisClass_CHtmlEditView__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CHtmlEditView__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CHwndRenderTarget@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CHwndRenderTarget@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CHwndRenderTarget,
           impl__GetThisClass_CHwndRenderTarget__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CHwndRenderTarget__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMDITabProxyWnd@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMDITabProxyWnd@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CMDITabProxyWnd,
           impl__GetThisClass_CMDITabProxyWnd__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMDITabProxyWnd__UEBAPEAUCRuntimeClass__XZ)
#undef VIEWS_RENDERTARGET_GETTERS
