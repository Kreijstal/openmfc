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

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define TASKDIALOG_DC_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
TASKDIALOG_DC_DESC(CTaskDialog, 240, 0xFFFF, &CObject::classCObject);
TASKDIALOG_DC_DESC(CWindowlessDC, 40, 0xFFFF, &CDC::classCDC);
#undef TASKDIALOG_DC_DESC
} // namespace

#define TASKDIALOG_DC_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CTaskDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CTaskDialog@@UEBAPEAUCRuntimeClass@@XZ
TASKDIALOG_DC_GETTERS(CTaskDialog,
           impl__GetThisClass_CTaskDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CTaskDialog__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CWindowlessDC@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CWindowlessDC@@UEBAPEAUCRuntimeClass@@XZ
TASKDIALOG_DC_GETTERS(CWindowlessDC,
           impl__GetThisClass_CWindowlessDC__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CWindowlessDC__UEBAPEAUCRuntimeClass__XZ)
#undef TASKDIALOG_DC_GETTERS
