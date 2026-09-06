#include "OleOleexceptionRttiSupport.h"

static CRuntimeClass* AFXAPI _openmfc_gb_classCOleException() { return &CException::classCException; }
CRuntimeClass classCOleException = {
    "COleException",
    static_cast<int>(sizeof(COleException)),
    0xFFFF,
    nullptr,
    &_openmfc_gb_classCOleException,
    nullptr,
    nullptr
};
static CRuntimeClass* AFXAPI _openmfc_gb_classCOleDispatchException() { return &classCOleException; }
CRuntimeClass classCOleDispatchException = {
    "COleDispatchException",
    static_cast<int>(sizeof(COleDispatchException)),
    0xFFFF,
    nullptr,
    &_openmfc_gb_classCOleDispatchException,  // chains through COleException -- as retail does
    nullptr,
    nullptr
};
