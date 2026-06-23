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

namespace {
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
} // namespace

#define FRAMES_DOCS_PREVIEW_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CMultiPaneFrameWnd@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMultiPaneFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CMultiPaneFrameWnd,
           impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMultiPaneFrameWnd__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@COleCntrFrameWndEx@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleCntrFrameWndEx@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(COleCntrFrameWndEx,
           impl__GetThisClass_COleCntrFrameWndEx__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleCntrFrameWndEx__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@COleDBRecordView@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleDBRecordView@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(COleDBRecordView,
           impl__GetThisClass_COleDBRecordView__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleDBRecordView__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@COlePropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COlePropertyPage@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(COlePropertyPage,
           impl__GetThisClass_COlePropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COlePropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CPreviewDC@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CPreviewDC@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CPreviewDC,
           impl__GetThisClass_CPreviewDC__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CPreviewDC__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CRichEditCntrItem@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CRichEditCntrItem@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CRichEditCntrItem,
           impl__GetThisClass_CRichEditCntrItem__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CRichEditCntrItem__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CRichEditDoc@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CRichEditDoc@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CRichEditDoc,
           impl__GetThisClass_CRichEditDoc__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CRichEditDoc__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CSmartDockingGroupGuidesManager@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CSmartDockingGroupGuidesManager@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CSmartDockingGroupGuidesManager,
           impl__GetThisClass_CSmartDockingGroupGuidesManager__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CSmartDockingGroupGuidesManager__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CSmartDockingStandaloneGuide@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CSmartDockingStandaloneGuide@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CSmartDockingStandaloneGuide,
           impl__GetThisClass_CSmartDockingStandaloneGuide__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CSmartDockingStandaloneGuide__UEBAPEAUCRuntimeClass__XZ)
#undef FRAMES_DOCS_PREVIEW_GETTERS
