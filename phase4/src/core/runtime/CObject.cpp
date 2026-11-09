// CObject — OpenMFC implementation.
// Sources: cobject_impl.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CRuntimeClassSupport.h"

// Native runtime metadata access lives in CObjectMetadata.cpp.
// Symbol: ?IsSerializable@CObject@@QEBAHXZ
extern "C" int MS_ABI impl__IsSerializable_CObject__QEBAHXZ(
    const CObject* pThis  // RCX = this pointer
) {
    const CRuntimeClass* pClass = GetObjectRuntimeClass(pThis);
    return pClass && pClass->m_wSchema != 0xFFFF ? TRUE : FALSE;
}
