// RuntimeClasses — OpenMFC implementation.
// Sources: global_csimpleexception_rtti.cpp, global_frame_htmlview_rtti.cpp, global_simple_exceptions_rtti.cpp, ole_oleexception_rtti.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CsimpleexceptionRttiSupport.h"
#include "detail/SimpleExceptionsRttiSupport.h"
#include "detail/OleOleexceptionRttiSupport.h"

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif








// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CMemoryException@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CInvalidArgException@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CInvalidArgException@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CNotSupportedException@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CNotSupportedException@@UEBAPEAUCRuntimeClass@@XZ
// ---- CResourceException_SimpleExceptionsRtti ------------------------------------------------------
// Symbol: ?GetThisClass@CResourceException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CResourceException__SAPEAUCRuntimeClass__XZ() {
    return &class_CResourceException;
}
// Symbol: ?GetRuntimeClass@CResourceException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_CResourceException__UEBAPEAUCRuntimeClass__XZ(const void* /*pThis*/) {
    return &class_CResourceException;
}
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CUserException@@SAPEAUCRuntimeClass@@XZ
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CUserException@@UEBAPEAUCRuntimeClass@@XZ
