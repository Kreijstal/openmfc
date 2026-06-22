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

namespace {
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
} // namespace

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
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
