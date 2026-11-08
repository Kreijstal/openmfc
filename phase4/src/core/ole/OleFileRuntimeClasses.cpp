// Runtime ancestry for OLE files with no separate exported class-data symbols.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#define MS_ABI
#define OFILE_DESC(Cls, Size, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }

// Parents before children so each can take the previous descriptor's address.
OFILE_DESC(CMonikerFile,            64,  &COleStreamFile::classCOleStreamFile);
OFILE_DESC(CAsyncMonikerFile,       96,  &classCMonikerFile);
OFILE_DESC(CDataPathProperty,       112, &classCAsyncMonikerFile);
OFILE_DESC(CCachedDataPathProperty, 200, &classCDataPathProperty);
#undef OFILE_DESC

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define OFILE_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }


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
