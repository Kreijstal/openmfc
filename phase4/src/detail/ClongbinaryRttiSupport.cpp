#include "ClongbinaryRttiSupport.h"

namespace openmfc { namespace detail { namespace clongbinaryrtti {
CRuntimeClass classCLongBinary = {
    "CLongBinary",
    24,                       // m_nObjectSize (sizeof(CLongBinary), runtime-verified)
    0xFFFF,                   // m_wSchema (DYNAMIC, not serializable)
    nullptr,                  // m_pfnCreateObject (no DYNCREATE factory)
    nullptr,                  // m_pfnGetBaseClass
    &CObject::classCObject,   // m_pBaseClass
    nullptr                   // m_pNextClass
};
} } }  // namespace openmfc::detail::clongbinaryrtti
