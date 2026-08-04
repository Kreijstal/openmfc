// RuntimeClasses — OpenMFC implementation.
// Sources: global_oleresidual_rtti.cpp

#include "detail/COleControlModuleSupport.h"

#define OR_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CSettingsStore@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CSettingsStore@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CSettingsStore,
           impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CSettingsStore__UEBAPEAUCRuntimeClass__XZ)

#undef OR_GETTERS
