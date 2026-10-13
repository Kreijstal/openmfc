#define OPENMFC_APPCORE_IMPL
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
static CRuntimeClass* AFXAPI _openmfc_gb_classCOleDispatchException() { return &CException::classCException; }
CRuntimeClass classCOleDispatchException = {
    "COleDispatchException",
    static_cast<int>(sizeof(COleDispatchException)),
    0xFFFF,
    nullptr,
    &_openmfc_gb_classCOleDispatchException,  // COleDispatchException derives directly from CException
    nullptr,
    nullptr
};

// Symbol: ?GetThisClass@COleException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ() {
    return &classCOleException;
}
// Symbol: ?GetRuntimeClass@COleException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleException__UEBAPEAUCRuntimeClass__XZ(
    const void* /*pThis*/) {
    return &classCOleException;
}
// Symbol: ?GetThisClass@COleDispatchException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDispatchException__SAPEAUCRuntimeClass__XZ() {
    return &classCOleDispatchException;
}
// Symbol: ?GetRuntimeClass@COleDispatchException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleDispatchException__UEBAPEAUCRuntimeClass__XZ(
    const void* /*pThis*/) {
    return &classCOleDispatchException;
}
