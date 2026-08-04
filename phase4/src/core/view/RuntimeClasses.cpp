// RuntimeClasses — OpenMFC implementation.
// Sources: gdi_rgn_richeditview_rtti.cpp, global_commonctrl2_rtti.cpp, global_controlbar_keyframe_rtti.cpp, global_frame_htmlview_rtti.cpp, global_frames_docs_preview_rtti.cpp, global_oleresidual_rtti.cpp, global_view_dialog_ctrl_rtti.cpp, global_views_rendertarget_rtti.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/COleControlModuleSupport.h"

// GDI drawing shard 02 - generated real implementations
//
// Implements RTTI helpers (GetThisClass/GetRuntimeClass), the DYNCREATE
// factory (CreateObject), and a well-known static data member for classes
// whose runtime-class machinery the repo headers define faithfully via
// DECLARE_DYNAMIC / DECLARE_DYNCREATE (backed by IMPLEMENT_* in the repo).
// These are unambiguous and safe to override.  Everything else in this shard
// (drawing / paint / view virtuals, Direct2D render-target methods, etc.)
// needs internal state or layout the headers do not model, so it is left as
// the existing weak stub.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// CRgn  (DECLARE_DYNAMIC, IMPLEMENT_DYNAMIC(CRgn, CGdiObject))
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// CWindowDC  (DECLARE_DYNAMIC, IMPLEMENT_DYNAMIC(CWindowDC, CDC))
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// CRichEditView  (DECLARE_DYNCREATE, IMPLEMENT_DYNCREATE(CRichEditView, CView))
// ---------------------------------------------------------------------------




// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a second batch of
// common-control / view wrappers (14 exports, 7 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory) whose m_pBaseClass chains to the real base descriptor
// (classCWnd / classCButton / classCView), following the repo's own
// IMPLEMENT_DYNAMIC convention (m_pfnGetBaseClass null, m_pBaseClass set) so
// IsKindOf/IsDerivedFrom walk the correct RUNTIME_CLASS graph. m_nObjectSize is
// the real MSVC sizeof harvested with cl.exe /d1reportSingleClassLayout:
//   CMonthCalCtrl 232, CReBarCtrl 232, CStatusBarCtrl 232, CToolBarCtrl 232,
//   CRichEditCtrl 232 (all CWnd), CSplitButton 248 (CButton),
//   CTabView 15880 (CView).
// GetRuntimeClass returns the static descriptor directly (never
// pThis->GetRuntimeClass(), which would recurse through this very export in a
// drop-in client).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxwin.h"   // CWnd / CButton / CView + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define CC2_DESC(Cls, Base, Size) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, &Base::class##Base, nullptr }

CC2_DESC(CMonthCalCtrl,  CWnd,    232);
CC2_DESC(CReBarCtrl,     CWnd,    232);
CC2_DESC(CStatusBarCtrl, CWnd,    232);
CC2_DESC(CToolBarCtrl,   CWnd,    232);
CC2_DESC(CRichEditCtrl,  CWnd,    232);
CC2_DESC(CSplitButton,   CButton, 248);
CC2_DESC(CTabView,       CView,   15880);
#undef CC2_DESC







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

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif








// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 9 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define FRAMES_DOCS_PREVIEW_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
FRAMES_DOCS_PREVIEW_DESC(CMultiPaneFrameWnd, 768, 0x80000002, &CPaneFrameWnd::classCPaneFrameWnd);
FRAMES_DOCS_PREVIEW_DESC(COleCntrFrameWndEx, 1352, 0xFFFF, &CFrameWnd::classCFrameWnd);
FRAMES_DOCS_PREVIEW_DESC(COleDBRecordView, 360, 0xFFFF, &CFormView::classCFormView);
FRAMES_DOCS_PREVIEW_DESC(COlePropertyPage, 512, 0xFFFF, &CDialog::classCDialog);
FRAMES_DOCS_PREVIEW_DESC(CPreviewDC, 88, 0xFFFF, &CDC::classCDC);
FRAMES_DOCS_PREVIEW_DESC(CRichEditCntrItem, 256, 0x00000000, &COleClientItem::classCOleClientItem);
FRAMES_DOCS_PREVIEW_DESC(CRichEditDoc, 672, 0xFFFF, &COleServerDoc::classCOleServerDoc);
FRAMES_DOCS_PREVIEW_DESC(CSmartDockingGroupGuidesManager, 6680, 0xFFFF, &CObject::classCObject);
FRAMES_DOCS_PREVIEW_DESC(CSmartDockingStandaloneGuide, 1128, 0xFFFF, &CObject::classCObject);
#undef FRAMES_DOCS_PREVIEW_DESC

#define FRAMES_DOCS_PREVIEW_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a batch of control,
// view and dialog wrappers (10 exports, 5 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory) whose m_pBaseClass chains to the real base descriptor
// (classCWnd / classCButton / classCView / classCDialog / classCFileDialog),
// matching the retail RUNTIME_CLASS graph that IsKindOf walks. m_nObjectSize is
// the real MSVC sizeof harvested with cl.exe /d1reportSingleClassLayout:
//   CDateTimeCtrl 232 (CWnd), CBitmapButton 296 (CButton), CCtrlView 264 (CView),
//   CCommonDialog 304 (CDialog), CFolderPickerDialog 1424 (CFileDialog).
// GetRuntimeClass returns the static descriptor directly (never
// pThis->GetRuntimeClass(), which would recurse through this very export in a
// drop-in client).

#include "openmfc/afxwin.h"   // CWnd/CButton/CView/CDialog/CFileDialog + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define VDC_DESC(Cls, Base, Size) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, &Base::class##Base, nullptr }

VDC_DESC(CDateTimeCtrl,       CWnd,        232);
VDC_DESC(CBitmapButton,       CButton,     296);
VDC_DESC(CCtrlView,           CView,       264);
VDC_DESC(CCommonDialog,       CDialog,     304);
VDC_DESC(CFolderPickerDialog, CFileDialog, 1424);
#undef VDC_DESC





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

// Symbol: ?GetRuntimeClass@CRichEditView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRichEditView__UEBAPEAUCRuntimeClass__XZ(
    const CRichEditView* pThis) {
    (void)pThis;
    return CRichEditView::GetThisClass();
}
// Symbol: ?GetThisClass@CRichEditView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRichEditView__SAPEAUCRuntimeClass__XZ() {
    return CRichEditView::GetThisClass();
}
// Symbol: ?CreateObject@CRichEditView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CRichEditView__SAPEAVCObject__XZ() {
    return CRichEditView::CreateObject();
}
// Static data member: long CRichEditView::lMaxSize.  In MFC this is the default
// rich-edit text limit, 64 KB (afxrich.cpp: long CRichEditView::lMaxSize = 64*1024).
// Symbol: ?lMaxSize@CRichEditView@@2KA
extern "C" unsigned long impl__lMaxSize_CRichEditView__2KA = 64UL * 1024UL;
// Symbol: ?GetThisClass@CTabView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTabView__SAPEAUCRuntimeClass__XZ() {
    return &classCTabView;
}
// Symbol: ?GetRuntimeClass@CTabView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTabView__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCTabView;
}
#define CONTROLBAR_KEYFRAME_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
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
#undef CONTROLBAR_KEYFRAME_GETTERS
// Symbol: ?GetThisClass@CHtmlView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHtmlView__SAPEAUCRuntimeClass__XZ() {
    return CHtmlView::GetThisClass();
}
// Symbol: ?GetRuntimeClass@CHtmlView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHtmlView__UEBAPEAUCRuntimeClass__XZ(
    const CHtmlView* pThis) {
    return CHtmlView::GetThisClass();
}
// Symbol: ?CreateObject@CHtmlView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CHtmlView__SAPEAVCObject__XZ() {
    return CHtmlView::CreateObject();
}
#define FRAMES_DOCS_PREVIEW_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@COleDBRecordView@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleDBRecordView@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(COleDBRecordView,
           impl__GetThisClass_COleDBRecordView__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleDBRecordView__UEBAPEAUCRuntimeClass__XZ)
#undef FRAMES_DOCS_PREVIEW_GETTERS
#define OR_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CDHtmlDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDHtmlDialog@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CDHtmlDialog,
           impl__GetThisClass_CDHtmlDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CDHtmlDialog__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CMultiPageDHtmlDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMultiPageDHtmlDialog@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CMultiPageDHtmlDialog,
           impl__GetThisClass_CMultiPageDHtmlDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMultiPageDHtmlDialog__UEBAPEAUCRuntimeClass__XZ)

#undef OR_GETTERS
// Symbol: ?GetThisClass@CCtrlView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CCtrlView__SAPEAUCRuntimeClass__XZ() {
    return &classCCtrlView;
}
// Symbol: ?GetRuntimeClass@CCtrlView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCtrlView__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCCtrlView;
}
#define VIEWS_RENDERTARGET_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CHtmlEditView@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CHtmlEditView@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CHtmlEditView,
           impl__GetThisClass_CHtmlEditView__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CHtmlEditView__UEBAPEAUCRuntimeClass__XZ)
#undef VIEWS_RENDERTARGET_GETTERS
