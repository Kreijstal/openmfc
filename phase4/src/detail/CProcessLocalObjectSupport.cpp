#include "CProcessLocalObjectSupport.h"

static_assert(sizeof(S_Cprocesslocalobject) == 8, "CProcessLocalObject size mismatch");
static_assert(offsetof(S_Cprocesslocalobject, m_pObject) == 0, "m_pObject offset mismatch");
namespace openmfc { namespace detail { namespace cprocesslocalobject {
SRWLOCK g_processLocalLock = SRWLOCK_INIT;
} } }  // namespace openmfc::detail::cprocesslocalobject
