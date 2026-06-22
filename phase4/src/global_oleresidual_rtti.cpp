// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a batch of OLE/HTML
// residual classes (16 exports, 8 classes).
//
// These are MFC classes with no separately-exported CRuntimeClass data symbol —
// only the two getters are exported. The repo has no class body for them, so each
// gets a file-internal CRuntimeClass descriptor following the repo's IMPLEMENT_*
// convention (m_pfnGetBaseClass null, m_pBaseClass set; the exported IsDerivedFrom
// falls back to m_pBaseClass). m_nObjectSize and m_wSchema are the real values read
// from mfc140u.dll (compile against mfc140u.lib, call the exported getter under
// Wine, read the raw descriptor): most are DECLARE_DYNAMIC/DYNCREATE (schema
// 0xFFFF), but CMouseManager and CUserTool are DECLARE_SERIAL and carry real schema
// versions (0x00000001 and 0x80000001 = VERSIONABLE_SCHEMA|1). m_pfnCreateObject is
// left null even for the DYNCREATE/SERIAL classes — OpenMFC has no class body to
// manufacture instances, and null honestly signals "not constructible".
//
// Each m_pBaseClass chains to a descriptor consistent with the base's exported
// GetThisClass: CCmdTarget::classCCmdTarget / CWinApp::classCWinApp (appcore.cpp),
// CObject::classCObject (afx.h), CDialog::classCDialog (dlgcore.cpp), and the
// in-file CDHtmlDialog descriptor. GetRuntimeClass returns the static descriptor
// directly (never pThis->GetRuntimeClass(), which would recurse through this export).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxwin.h"   // CCmdTarget/CWinApp/CDialog/CObject + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define OR_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }

// CDHtmlDialog before CMultiPageDHtmlDialog so the latter can take its address.
OR_DESC(CDHtmlDialog,          664, 0xFFFF,     &CDialog::classCDialog);          // DECLARE_DYNAMIC
OR_DESC(CMultiPageDHtmlDialog, 672, 0xFFFF,     &classCDHtmlDialog);              // DECLARE_DYNAMIC
OR_DESC(CDocObjectServer,      144, 0xFFFF,     &CCmdTarget::classCCmdTarget);    // DECLARE_DYNAMIC
OR_DESC(CDynLinkLibrary,       128, 0xFFFF,     &CCmdTarget::classCCmdTarget);    // DECLARE_DYNAMIC
OR_DESC(COleControlModule,     376, 0xFFFF,     &CWinApp::classCWinApp);          // DECLARE_DYNAMIC
OR_DESC(CSettingsStore,        56,  0xFFFF,     &CObject::classCObject);          // DECLARE_DYNCREATE
OR_DESC(CMouseManager,         176, 0x00000001, &CObject::classCObject);          // DECLARE_SERIAL, schema 1
OR_DESC(CUserTool,             56,  0x80000001, &CObject::classCObject);          // DECLARE_SERIAL, VERSIONABLE|1
#undef OR_DESC
} // namespace

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
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

// Symbol: ?GetThisClass@CDocObjectServer@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDocObjectServer@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CDocObjectServer,
           impl__GetThisClass_CDocObjectServer__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CDocObjectServer__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CDynLinkLibrary@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDynLinkLibrary@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CDynLinkLibrary,
           impl__GetThisClass_CDynLinkLibrary__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CDynLinkLibrary__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleControlModule@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleControlModule@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(COleControlModule,
           impl__GetThisClass_COleControlModule__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleControlModule__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CSettingsStore@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CSettingsStore@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CSettingsStore,
           impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CSettingsStore__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CMouseManager@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMouseManager@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CMouseManager,
           impl__GetThisClass_CMouseManager__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMouseManager__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CUserTool@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CUserTool@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CUserTool,
           impl__GetThisClass_CUserTool__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CUserTool__UEBAPEAUCRuntimeClass__XZ)

#undef OR_GETTERS
