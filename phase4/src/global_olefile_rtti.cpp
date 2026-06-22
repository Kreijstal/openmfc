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

namespace {
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
} // namespace

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
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
