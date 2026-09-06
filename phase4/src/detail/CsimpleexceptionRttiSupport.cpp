#include "CsimpleexceptionRttiSupport.h"

static_assert(sizeof(wchar_t) == 2, "Unicode build expects 2-byte wchar_t");
extern "C" {
static CRuntimeClass* AFXAPI _openmfc_gb_classCSimpleException() { return &CException::classCException; }
CRuntimeClass classCSimpleException = {
    "CSimpleException",                 // m_lpszClassName
    (int)sizeof(CSimpleException),      // m_nObjectSize
    0xFFFF,                             // m_wSchema
    nullptr,                            // m_pfnCreateObject (abstract base, no DYNCREATE)
    &_openmfc_gb_classCSimpleException,  // m_pfnGetBaseClass -> CException
    nullptr,                            // m_pNextClass
    nullptr                             // m_pClassInit
};
}
