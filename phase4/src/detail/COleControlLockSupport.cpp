#include "COleControlLockSupport.h"

static_assert(sizeof(S_Colecontrollock) == 40, "COleControlLock must be size 40");
static_assert(offsetof(S_Colecontrollock, m_clsid) == 8, "m_clsid @8");
static_assert(offsetof(S_Colecontrollock, m_pClassFactory) == 24, "m_pClassFactory @24");
static_assert(offsetof(S_Colecontrollock, m_pNextLock) == 32, "m_pNextLock @32");
namespace openmfc { namespace detail { namespace colecontrollock {
void* MS_ABI vdtor_Colecontrollock(void* p, unsigned flags) {
    impl___1COleControlLock__UEAA_XZ(p);
    if (flags & 1)
        ::operator delete(p);
    return p;
}
} } }  // namespace openmfc::detail::colecontrollock
