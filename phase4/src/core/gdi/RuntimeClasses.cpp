// RuntimeClasses — OpenMFC implementation.
// Sources: gdi_object_rtti.cpp, gdi_rgn_richeditview_rtti.cpp, global_frame_htmlview_rtti.cpp, global_frames_docs_preview_rtti.cpp, global_taskdialog_dc_rtti.cpp

#define OPENMFC_APPCORE_IMPL

// GDI drawing shard 01 - generated real implementations
//
// Implements RTTI helpers (GetThisClass/GetRuntimeClass) and CreateObject for
// GDI classes whose memory layout the repo headers define faithfully via
// DECLARE_DYNAMIC. These are unambiguous and safe to override.

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
// CGdiObject
// ---------------------------------------------------------------------------




// ---------------------------------------------------------------------------
// CPen
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// CBrush
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// CFont
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// CBitmap
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// CPalette
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// CDC
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// CClientDC
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// CPaintDC
// ---------------------------------------------------------------------------


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
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define FRAMES_DOCS_PREVIEW_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
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

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 2 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define TASKDIALOG_DC_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
TASKDIALOG_DC_DESC(CTaskDialog, 240, 0xFFFF, &CObject::classCObject);
TASKDIALOG_DC_DESC(CWindowlessDC, 40, 0xFFFF, &CDC::classCDC);
#undef TASKDIALOG_DC_DESC

#define TASKDIALOG_DC_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetRuntimeClass@CGdiObject@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CGdiObject__UEBAPEAUCRuntimeClass__XZ(
    const CGdiObject* pThis) {
    (void)pThis;
    return CGdiObject::GetThisClass();
}
// Symbol: ?GetThisClass@CGdiObject@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CGdiObject__SAPEAUCRuntimeClass__XZ() {
    return CGdiObject::GetThisClass();
}
// Symbol: ?CreateObject@CGdiObject@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CGdiObject__SAPEAVCObject__XZ() {
    return new CGdiObject();
}
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CPen@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CPen@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CBrush@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CBrush@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CFont@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CFont@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CBitmap@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CBitmap@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CPalette@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CPalette@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CDC@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CDC@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CClientDC@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CClientDC@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CPaintDC@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CPaintDC@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CRgn@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRgn__UEBAPEAUCRuntimeClass__XZ(
    const CRgn* pThis) {
    (void)pThis;
    return CRgn::GetThisClass();
}
// Symbol: ?GetThisClass@CRgn@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRgn__SAPEAUCRuntimeClass__XZ() {
    return CRgn::GetThisClass();
}
// Symbol: ?GetRuntimeClass@CWindowDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWindowDC__UEBAPEAUCRuntimeClass__XZ(
    const CWindowDC* pThis) {
    (void)pThis;
    return CWindowDC::GetThisClass();
}
// Symbol: ?GetThisClass@CWindowDC@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWindowDC__SAPEAUCRuntimeClass__XZ() {
    return CWindowDC::GetThisClass();
}
// Symbol: ?GetThisClass@CMetaFileDC@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMetaFileDC__SAPEAUCRuntimeClass__XZ() {
    return CMetaFileDC::GetThisClass();
}
// Symbol: ?GetRuntimeClass@CMetaFileDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMetaFileDC__UEBAPEAUCRuntimeClass__XZ(
    const CMetaFileDC* pThis) {
    return CMetaFileDC::GetThisClass();
}
#define FRAMES_DOCS_PREVIEW_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CPreviewDC@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CPreviewDC@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CPreviewDC,
           impl__GetThisClass_CPreviewDC__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CPreviewDC__UEBAPEAUCRuntimeClass__XZ)
#undef FRAMES_DOCS_PREVIEW_GETTERS
#define TASKDIALOG_DC_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CWindowlessDC@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CWindowlessDC@@UEBAPEAUCRuntimeClass@@XZ
TASKDIALOG_DC_GETTERS(CWindowlessDC,
           impl__GetThisClass_CWindowlessDC__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CWindowlessDC__UEBAPEAUCRuntimeClass__XZ)
#undef TASKDIALOG_DC_GETTERS
#undef TASKDIALOG_DC_GETTERS
