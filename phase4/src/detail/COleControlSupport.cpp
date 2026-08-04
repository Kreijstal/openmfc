#include "COleControlSupport.h"

static_assert(offsetof(COleControl, m_piidEvents) == 240, "m_piidEvents @240");
static_assert(offsetof(COleControl, m_rcPos) == 288, "m_rcPos @288");
static_assert(offsetof(COleControl, m_pControlSite) == 488, "m_pControlSite @488");
static_assert(offsetof(COleControl, m_xPropConnPt) == 816, "m_xPropConnPt @816");
namespace openmfc { namespace detail { namespace colecontrol {
long PtX(unsigned long long pt) { return (long)(unsigned int)(pt & 0xFFFFFFFFull); }
long PtY(unsigned long long pt) { return (long)(unsigned int)(pt >> 32); }
} } }  // namespace openmfc::detail::colecontrol
