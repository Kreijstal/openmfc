// Native runtime-class construction consumes framework exceptions from a
// client factory while preserving other C++ exceptions.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
extern "C" void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ();
// Symbol: ?CreateObject@CRuntimeClass@@QEAAPEAVCObject@@XZ
extern "C" CObject* AFXAPI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(CRuntimeClass* type) {
    if(!type)impl__AfxThrowInvalidArgException__YAXXZ();
    if(!type->m_pfnCreateObject)return nullptr;
    CObject* object=nullptr;
    try {object=type->m_pfnCreateObject();}
    catch(CException* error) {error->Delete();}
    return object;
}
CObject* CRuntimeClass::CreateObject() {
    return impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(this);
}
