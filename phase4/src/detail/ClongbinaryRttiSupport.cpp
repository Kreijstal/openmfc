#include "ClongbinaryRttiSupport.h"

namespace openmfc { namespace detail { namespace clongbinaryrtti {
static CRuntimeClass* AFXAPI _openmfc_gb_classCLongBinary() { return &CObject::classCObject; }
CRuntimeClass classCLongBinary = {
    "CLongBinary",
    24,                       // m_nObjectSize (sizeof(CLongBinary), runtime-verified)
    0xFFFF,                   // m_wSchema (DYNAMIC, not serializable)
    nullptr,                  // m_pfnCreateObject (no DYNCREATE factory)
    &_openmfc_gb_classCLongBinary,  // m_pfnGetBaseClass -> CObject
    nullptr,                  // m_pNextClass
    nullptr                   // m_pClassInit
};
} } }  // namespace openmfc::detail::clongbinaryrtti
