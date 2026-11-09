// Native CObject metadata access has no initialization side effects.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
// Symbol: ?GetRuntimeClass@CObject@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const CObject*) {return CObject::GetThisClass();}
// Symbol: ?GetThisClass@CObject@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CObject__SAPEAUCRuntimeClass__XZ() {return CObject::GetThisClass();}
