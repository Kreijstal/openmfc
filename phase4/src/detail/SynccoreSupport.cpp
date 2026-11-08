#include "SynccoreSupport.h"

IMPLEMENT_DYNAMIC(CSyncObject, CObject)
IMPLEMENT_DYNAMIC(CCriticalSection, CSyncObject)
IMPLEMENT_DYNAMIC(CMutex, CSyncObject)
IMPLEMENT_DYNAMIC(CSemaphore, CSyncObject)
IMPLEMENT_DYNAMIC(CEvent, CSyncObject)
namespace openmfc { namespace detail { namespace synccore {
extern "C" void* MS_ABI impl___0CSyncObject__QEAA_PEB_W_Z(void* pThis, const wchar_t* pstrName) {
    if (!pThis) {
        return nullptr;
    }
    return new(pThis) CSyncObject(pstrName);
}
} } }  // namespace openmfc::detail::synccore
