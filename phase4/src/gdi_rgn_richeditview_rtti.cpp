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

// ---------------------------------------------------------------------------
// CWindowDC  (DECLARE_DYNAMIC, IMPLEMENT_DYNAMIC(CWindowDC, CDC))
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
// CRichEditView  (DECLARE_DYNCREATE, IMPLEMENT_DYNCREATE(CRichEditView, CView))
// ---------------------------------------------------------------------------

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
