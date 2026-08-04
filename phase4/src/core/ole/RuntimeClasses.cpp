// RuntimeClasses — OpenMFC implementation.
// Sources: global_common_control_rtti.cpp, global_frames_docs_preview_rtti.cpp, global_oledoc_rtti.cpp, global_olefile_rtti.cpp, global_oleframe_rtti.cpp, global_oleresidual_rtti.cpp, ole_clientitem_dialogs_rtti.cpp, ole_control_rtti.cpp, ole_dialogs_rtti.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/COleControlModuleSupport.h"

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxdao.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Destructors for classes whose repo header definitions faithfully model the
// real MFC ABI (thin CWnd/CDialog control wrappers with no extra owning state,
// plus a few value/handle-owning classes with explicit cleanup logic).
//
// Each impl simply invokes the C++ destructor, which performs exactly the work
// the real MFC destructor does (chaining to the base destructor and freeing any
// owned members).  All are null-guarded.
//
// Placeholder stub classes that are only modelled as `char _pad[N]` (uncertain
// real layout) are intentionally left with their weak stubs.

// ---- Thin Win32 control wrappers (CWnd-derived, defaulted destructors) ----
















// ---- Thin CDialog-derived wrappers (defaulted destructors) ----





// ---- Handle/value-owning classes with explicit cleanup ----



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

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for the OLE document /
// server-item family (16 exports, 8 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory), following the repo's IMPLEMENT_DYNAMIC convention
// (m_pfnGetBaseClass null, m_pBaseClass set). The document chain
// (COleDocument -> COleLinkingDoc -> COleServerDoc -> COleServerDocEx) and the
// server-item chain (COleServerItem -> CDocObjectServerItem) chain within this
// file; the chain roots point at the existing real base descriptors that the DLL
// already defines (CDocument::classCDocument in docview.cpp,
// CDocItem::classCDocItem / COleClientItem::classCOleClientItem in olecore.cpp,
// CCmdTarget::classCCmdTarget in appcore.cpp). This reproduces the retail
// RUNTIME_CLASS graph IsKindOf/IsDerivedFrom walk. m_nObjectSize is the real
// MSVC sizeof harvested with cl.exe /d1reportSingleClassLayout. GetRuntimeClass
// returns the static descriptor directly (never pThis->GetRuntimeClass(), which
// would recurse through this very export in a drop-in client).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxole.h"   // CDocument/CDocItem/COleClientItem/CCmdTarget + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define OLED_DESC(Cls, Size, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, (BaseDesc), nullptr }

// Parents-first so each derived descriptor can take its base's address.
OLED_DESC(COleDocument,         488, &CDocument::classCDocument);
OLED_DESC(COleLinkingDoc,       560, &classCOleDocument);
OLED_DESC(COleServerDoc,        664, &classCOleLinkingDoc);
OLED_DESC(COleServerDocEx,      664, &classCOleServerDoc);
OLED_DESC(COleServerItem,       224, &CDocItem::classCDocItem);
OLED_DESC(CDocObjectServerItem, 224, &classCOleServerItem);
OLED_DESC(COleObjectFactory,    136, &CCmdTarget::classCCmdTarget);
OLED_DESC(COleDocObjectItem,    280, &COleClientItem::classCOleClientItem);
#undef OLED_DESC

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define OLED_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for the OLE file/stream
// and data-path family (10 exports, 5 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory), following the repo's IMPLEMENT_DYNAMIC convention
// (m_pfnGetBaseClass null, m_pBaseClass set; the exported IsDerivedFrom falls back
// to m_pBaseClass). m_nObjectSize and m_wSchema are the real values read from
// mfc140u.dll (compile against mfc140u.lib, call the exported getter under Wine,
// read the raw descriptor) — all five are DECLARE_DYNAMIC, schema 0xFFFF.
//
// The chain roots at CFile::classCFile (filecore.cpp), which a companion change
// gave a real descriptor (previously CFile::GetThisClass returned null); the rest
// chain in-file: COleStreamFile -> CMonikerFile -> CAsyncMonikerFile ->
// CDataPathProperty -> CCachedDataPathProperty. So IsKindOf/IsDerivedFrom walk an
// unbroken graph in a drop-in client. GetRuntimeClass returns the static descriptor
// directly (never pThis->GetRuntimeClass(), which would recurse through this very
// export).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxole.h"   // CFile (afx.h) + CObject + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define OFILE_DESC(Cls, Size, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, (BaseDesc), nullptr }

// Parents before children so each can take the previous descriptor's address.
OFILE_DESC(COleStreamFile,          56,  &CFile::classCFile);
OFILE_DESC(CMonikerFile,            64,  &classCOleStreamFile);
OFILE_DESC(CAsyncMonikerFile,       96,  &classCMonikerFile);
OFILE_DESC(CDataPathProperty,       112, &classCAsyncMonikerFile);
OFILE_DESC(CCachedDataPathProperty, 200, &classCDataPathProperty);
#undef OFILE_DESC

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define OFILE_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for the OLE in-place
// frame-window and OLE common-dialog families (16 exports, 8 classes).
//
// These are MFC classes with no separately-exported CRuntimeClass data symbol —
// only the two getters are exported. The repo has no class body for them, so each
// gets a file-internal CRuntimeClass descriptor following the repo's IMPLEMENT_*
// convention (m_pfnGetBaseClass null, m_pBaseClass set; the exported IsDerivedFrom
// falls back to m_pBaseClass). m_nObjectSize and m_wSchema are the real values read
// from mfc140u.dll (compile against mfc140u.lib, call the exported getter under
// Wine, read the raw descriptor): the four COle*Dialog classes are DECLARE_DYNAMIC
// and the four frame windows are DECLARE_DYNCREATE, so all carry schema 0xFFFF.
// m_pfnCreateObject is left null even for the DYNCREATE frames: OpenMFC has no class
// body so it cannot manufacture instances, and null honestly signals
// "not constructible" (real mfc140u points it at a real factory we cannot replicate).
//
// Each m_pBaseClass chains to a descriptor consistent with what the base class's
// *exported* GetThisClass returns: COleDialog::classCOleDialog (olecore.cpp) and
// CFrameWnd::classCFrameWnd (wincore.cpp), whose exported getters return those same
// members, plus the in-file COleIPFrameWnd / COleDocIPFrameWnd descriptors. So
// IsKindOf/IsDerivedFrom walk an unbroken graph in a drop-in client.
// GetRuntimeClass returns the static descriptor directly (never
// pThis->GetRuntimeClass(), which would recurse through this very export).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxole.h"     // COleDialog + CFrameWnd/CDialog + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define OF_DESC(Cls, Size, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, (BaseDesc), nullptr }

// OLE common dialogs (DECLARE_DYNAMIC) -> COleDialog.
OF_DESC(COleInsertDialog,       1008, &COleDialog::classCOleDialog);
OF_DESC(COleLinksDialog,        432,  &COleDialog::classCOleDialog);
OF_DESC(COlePasteSpecialDialog, 480,  &COleDialog::classCOleDialog);
OF_DESC(COlePropertiesDialog,   720,  &COleDialog::classCOleDialog);

// OLE in-place frame windows (DECLARE_DYNCREATE). COleIPFrameWnd before its
// derivatives so they can take its address; likewise COleDocIPFrameWnd.
OF_DESC(COleIPFrameWnd,         640,  &CFrameWnd::classCFrameWnd);
OF_DESC(COleIPFrameWndEx,       1936, &classCOleIPFrameWnd);
OF_DESC(COleDocIPFrameWnd,      640,  &classCOleIPFrameWnd);
OF_DESC(COleDocIPFrameWndEx,    1936, &classCOleDocIPFrameWnd);
#undef OF_DESC

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define OF_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// This shard implements MFC RTTI accessors (GetThisClass / GetRuntimeClass)
// for OLE/COM/ActiveX classes that are fully defined in the OpenMFC headers
// with DECLARE_DYNAMIC + IMPLEMENT_DYNAMIC. These are unambiguous: GetThisClass
// returns the static CRuntimeClass for the class, and GetRuntimeClass returns
// the same (virtual) value. We null-guard the virtual variants.

//-----------------------------------------------------------------------------
// CCmdTarget
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// COleBusyDialog
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// COleChangeIconDialog
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// COleChangeSourceDialog
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// COleClientItem
//-----------------------------------------------------------------------------

// OLE/COM ActiveX shard 02 - real exports overriding weak stubs.
// Implements RTTI helpers (GetThisClass/GetRuntimeClass) and message-map
// accessors for COleControl. COleControl is declared with DECLARE_DYNAMIC in
// afxole.h and registered via IMPLEMENT_DYNAMIC(COleControl, CWnd) in
// olecore.cpp, so its static runtime class and the CWnd message map are
// available here.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// OpenMFC export shard: ole_com_activex 03
//
// This shard implements only the MFC RTTI accessors (GetThisClass /
// GetRuntimeClass) for classes whose full definition with DECLARE_DYNAMIC is
// available in the repo headers. These are unambiguous:
//   - GetThisClass() is a static accessor returning the class's CRuntimeClass.
//   - GetRuntimeClass() is the virtual override returning the same.
// Everything else in this shard (control-site/container/in-place-frame methods,
// data-source/object internals, currency parsing, etc.) depends on internal
// MFC state or memory layout the headers do not model faithfully, so those
// weak stubs are intentionally left in place.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// COleDialog
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// COleConvertDialog
// ---------------------------------------------------------------------------


// Symbol: ??1COleControlSiteOrWnd@@QEAA@XZ
extern "C" void MS_ABI impl___1COleControlSiteOrWnd__QEAA_XZ(COleControlSiteOrWnd* pThis) {
    if (pThis) pThis->~COleControlSiteOrWnd();
}
#define FRAMES_DOCS_PREVIEW_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@COleCntrFrameWndEx@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleCntrFrameWndEx@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(COleCntrFrameWndEx,
           impl__GetThisClass_COleCntrFrameWndEx__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleCntrFrameWndEx__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@COlePropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COlePropertyPage@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(COlePropertyPage,
           impl__GetThisClass_COlePropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COlePropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CRichEditCntrItem@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CRichEditCntrItem@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CRichEditCntrItem,
           impl__GetThisClass_CRichEditCntrItem__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CRichEditCntrItem__UEBAPEAUCRuntimeClass__XZ)
#undef FRAMES_DOCS_PREVIEW_GETTERS
#define OLED_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@COleDocument@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleDocument@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleDocument,
             impl__GetThisClass_COleDocument__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleDocument__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleLinkingDoc@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleLinkingDoc@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleLinkingDoc,
             impl__GetThisClass_COleLinkingDoc__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleLinkingDoc__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleServerDoc@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleServerDoc@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleServerDoc,
             impl__GetThisClass_COleServerDoc__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleServerDoc__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleServerDocEx@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleServerDocEx@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleServerDocEx,
             impl__GetThisClass_COleServerDocEx__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleServerDocEx__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleServerItem@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleServerItem@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleServerItem,
             impl__GetThisClass_COleServerItem__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleServerItem__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CDocObjectServerItem@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDocObjectServerItem@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(CDocObjectServerItem,
             impl__GetThisClass_CDocObjectServerItem__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CDocObjectServerItem__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleObjectFactory@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleObjectFactory@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleObjectFactory,
             impl__GetThisClass_COleObjectFactory__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleObjectFactory__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleDocObjectItem@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleDocObjectItem@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleDocObjectItem,
             impl__GetThisClass_COleDocObjectItem__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleDocObjectItem__UEBAPEAUCRuntimeClass__XZ)

#undef OLED_GETTERS
#undef OLED_GETTERS
#define OFILE_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@COleStreamFile@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleStreamFile@@UEBAPEAUCRuntimeClass@@XZ
OFILE_GETTERS(COleStreamFile,
              impl__GetThisClass_COleStreamFile__SAPEAUCRuntimeClass__XZ,
              impl__GetRuntimeClass_COleStreamFile__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CMonikerFile@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMonikerFile@@UEBAPEAUCRuntimeClass@@XZ
OFILE_GETTERS(CMonikerFile,
              impl__GetThisClass_CMonikerFile__SAPEAUCRuntimeClass__XZ,
              impl__GetRuntimeClass_CMonikerFile__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CAsyncMonikerFile@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CAsyncMonikerFile@@UEBAPEAUCRuntimeClass@@XZ
OFILE_GETTERS(CAsyncMonikerFile,
              impl__GetThisClass_CAsyncMonikerFile__SAPEAUCRuntimeClass__XZ,
              impl__GetRuntimeClass_CAsyncMonikerFile__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CDataPathProperty@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDataPathProperty@@UEBAPEAUCRuntimeClass@@XZ
OFILE_GETTERS(CDataPathProperty,
              impl__GetThisClass_CDataPathProperty__SAPEAUCRuntimeClass__XZ,
              impl__GetRuntimeClass_CDataPathProperty__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CCachedDataPathProperty@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CCachedDataPathProperty@@UEBAPEAUCRuntimeClass@@XZ
OFILE_GETTERS(CCachedDataPathProperty,
              impl__GetThisClass_CCachedDataPathProperty__SAPEAUCRuntimeClass__XZ,
              impl__GetRuntimeClass_CCachedDataPathProperty__UEBAPEAUCRuntimeClass__XZ)

#undef OFILE_GETTERS
#undef OFILE_GETTERS
#define OF_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@COleInsertDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleInsertDialog@@UEBAPEAUCRuntimeClass@@XZ
OF_GETTERS(COleInsertDialog,
           impl__GetThisClass_COleInsertDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleInsertDialog__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleLinksDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleLinksDialog@@UEBAPEAUCRuntimeClass@@XZ
OF_GETTERS(COleLinksDialog,
           impl__GetThisClass_COleLinksDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleLinksDialog__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COlePasteSpecialDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COlePasteSpecialDialog@@UEBAPEAUCRuntimeClass@@XZ
OF_GETTERS(COlePasteSpecialDialog,
           impl__GetThisClass_COlePasteSpecialDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COlePasteSpecialDialog__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COlePropertiesDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COlePropertiesDialog@@UEBAPEAUCRuntimeClass@@XZ
OF_GETTERS(COlePropertiesDialog,
           impl__GetThisClass_COlePropertiesDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COlePropertiesDialog__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleIPFrameWnd@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleIPFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
OF_GETTERS(COleIPFrameWnd,
           impl__GetThisClass_COleIPFrameWnd__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleIPFrameWnd__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleIPFrameWndEx@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleIPFrameWndEx@@UEBAPEAUCRuntimeClass@@XZ
OF_GETTERS(COleIPFrameWndEx,
           impl__GetThisClass_COleIPFrameWndEx__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleIPFrameWndEx__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleDocIPFrameWnd@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleDocIPFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
OF_GETTERS(COleDocIPFrameWnd,
           impl__GetThisClass_COleDocIPFrameWnd__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleDocIPFrameWnd__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleDocIPFrameWndEx@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleDocIPFrameWndEx@@UEBAPEAUCRuntimeClass@@XZ
OF_GETTERS(COleDocIPFrameWndEx,
           impl__GetThisClass_COleDocIPFrameWndEx__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleDocIPFrameWndEx__UEBAPEAUCRuntimeClass__XZ)

#undef OF_GETTERS
#undef OF_GETTERS
#define OR_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CDocObjectServer@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDocObjectServer@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CDocObjectServer,
           impl__GetThisClass_CDocObjectServer__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CDocObjectServer__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleControlModule@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleControlModule@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(COleControlModule,
           impl__GetThisClass_COleControlModule__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleControlModule__UEBAPEAUCRuntimeClass__XZ)

#undef OR_GETTERS
// Symbol: ?InitInstance@COleControlModule@@UEAAHXZ
extern "C" int MS_ABI impl__InitInstance_COleControlModule__UEAAHXZ(COleControlModule* pThis) {
    return pThis ? pThis->CWinApp::InitInstance() : 0;
}
// Symbol: ?ExitInstance@COleControlModule@@UEAAHXZ
extern "C" int MS_ABI impl__ExitInstance_COleControlModule__UEAAHXZ(COleControlModule* pThis) {
    return pThis ? pThis->CWinApp::ExitInstance() : 0;
}
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@COleBusyDialog@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@COleBusyDialog@@UEBAPEAUCRuntimeClass@@XZ
// Symbol: ?GetThisClass@COleChangeIconDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleChangeIconDialog__SAPEAUCRuntimeClass__XZ() {
    return COleChangeIconDialog::GetThisClass();
}
// Symbol: ?GetRuntimeClass@COleChangeIconDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleChangeIconDialog__UEBAPEAUCRuntimeClass__XZ(
        const COleChangeIconDialog* pThis) {
    if (!pThis) return COleChangeIconDialog::GetThisClass();
    return COleChangeIconDialog::GetThisClass();
}
// Symbol: ?GetThisClass@COleChangeSourceDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleChangeSourceDialog__SAPEAUCRuntimeClass__XZ() {
    return COleChangeSourceDialog::GetThisClass();
}
// Symbol: ?GetRuntimeClass@COleChangeSourceDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleChangeSourceDialog__UEBAPEAUCRuntimeClass__XZ(
        const COleChangeSourceDialog* pThis) {
    if (!pThis) return COleChangeSourceDialog::GetThisClass();
    return COleChangeSourceDialog::GetThisClass();
}
// Symbol: ?GetThisClass@COleClientItem@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleClientItem__SAPEAUCRuntimeClass__XZ() {
    return COleClientItem::GetThisClass();
}
// Symbol: ?GetRuntimeClass@COleClientItem@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleClientItem__UEBAPEAUCRuntimeClass__XZ(
        const COleClientItem* pThis) {
    if (!pThis) return COleClientItem::GetThisClass();
    return COleClientItem::GetThisClass();
}
// Symbol: ?GetThisClass@COleControl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_COleControl__SAPEAUCRuntimeClass__XZ() {
    return COleControl::GetThisClass();
}
// Symbol: ?GetRuntimeClass@COleControl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_COleControl__UEBAPEAUCRuntimeClass__XZ(const COleControl* pThis) {
    (void)pThis;
    return COleControl::GetThisClass();
}
// Symbol: ?GetThisClass@COleDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDialog__SAPEAUCRuntimeClass__XZ() {
    return COleDialog::GetThisClass();
}
// Symbol: ?GetRuntimeClass@COleDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleDialog__UEBAPEAUCRuntimeClass__XZ(
    const COleDialog* pThis) {
    if (!pThis) return COleDialog::GetThisClass();
    return COleDialog::GetThisClass();
}
// Symbol: ?GetThisClass@COleConvertDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleConvertDialog__SAPEAUCRuntimeClass__XZ() {
    return COleConvertDialog::GetThisClass();
}
// Symbol: ?GetRuntimeClass@COleConvertDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleConvertDialog__UEBAPEAUCRuntimeClass__XZ(
    const COleConvertDialog* pThis) {
    if (!pThis) return COleConvertDialog::GetThisClass();
    return COleConvertDialog::GetThisClass();
}
