// Native exception-to-SCODE classification used by OLE callback boundaries.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
extern "C" CRuntimeClass* impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ();
// Symbol: ?Process@COleException@@SAJPEBVCException@@@Z
extern "C" LONG impl__Process_COleException__SAJPEBVCException___Z(const CException* error) {
    if (error->IsKindOf(COleException::GetThisClass())) return static_cast<const COleException*>(error)->m_sc;
    if (error->IsKindOf(impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ())) return E_OUTOFMEMORY;
    if (error->IsKindOf(impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ())) return E_NOTIMPL;
    return E_UNEXPECTED;
}
