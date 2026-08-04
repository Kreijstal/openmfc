// RuntimeClasses — OpenMFC implementation.
// Sources: global_docitem_edit_rtti.cpp, global_frames_docs_preview_rtti.cpp, global_views_rendertarget_rtti.cpp

#define OPENMFC_APPCORE_IMPL

// other_mfc shard 03/08 real exports.
//
// Implements the unambiguous RTTI accessors (GetThisClass / GetRuntimeClass)
// for classes whose CRuntimeClass is already defined via IMPLEMENT_DYNAMIC in
// the repo, plus a couple of trivial virtuals with well-defined defaults.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"

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

// Symbol: ?GetRuntimeClass@CDocItem@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDocItem__UEBAPEAUCRuntimeClass__XZ(const CDocItem*) {
    return CDocItem::GetThisClass();
}
// Symbol: ?GetThisClass@CDocItem@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDocItem__SAPEAUCRuntimeClass__XZ() {
    return CDocItem::GetThisClass();
}
#define FRAMES_DOCS_PREVIEW_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CRichEditDoc@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CRichEditDoc@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CRichEditDoc,
           impl__GetThisClass_CRichEditDoc__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CRichEditDoc__UEBAPEAUCRuntimeClass__XZ)
#undef FRAMES_DOCS_PREVIEW_GETTERS
#define VIEWS_RENDERTARGET_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CHtmlEditDoc@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CHtmlEditDoc@@UEBAPEAUCRuntimeClass@@XZ
VIEWS_RENDERTARGET_GETTERS(CHtmlEditDoc,
           impl__GetThisClass_CHtmlEditDoc__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CHtmlEditDoc__UEBAPEAUCRuntimeClass__XZ)
#undef VIEWS_RENDERTARGET_GETTERS
