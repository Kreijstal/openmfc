// DynCreateFactories — OpenMFC implementation.
// Sources: global_dyncreate_factories.cpp

#include "detail/DyncreateFactoriesSupport.h"

// Symbol: ?CreateObject@CSettingsStore@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CSettingsStore__SAPEAVCObject__XZ()
{ return CreateSized<CSettingsStoreObject, 56>(); }
