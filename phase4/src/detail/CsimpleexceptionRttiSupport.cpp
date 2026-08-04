#include "CsimpleexceptionRttiSupport.h"

static_assert(sizeof(wchar_t) == 2, "Unicode build expects 2-byte wchar_t");
extern "C" {
CRuntimeClass classCSimpleException = {
    "CSimpleException",                 // m_lpszClassName
    (int)sizeof(CSimpleException),      // m_nObjectSize
    0xFFFF,                             // m_wSchema
    nullptr,                            // m_pfnCreateObject (abstract base, no DYNCREATE)
    nullptr,                            // m_pfnGetBaseClass
    &CException::classCException,        // m_pBaseClass -> CException
    nullptr                            // m_pNextClass
};
}
