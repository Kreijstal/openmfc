#include "CThreadLocalObjectSupport.h"
#include <new>
static_assert(sizeof(S_Cthreadlocalobject) == 4, "native thread-local slot");
namespace openmfc {
namespace detail {
namespace cthreadlocalobject {
CThreadSlotData* threadSlots = nullptr;
DWORD tlsReferences = 0;
alignas(CThreadSlotData) static BYTE slotStorage[sizeof(CThreadSlotData)]{};
CThreadSlotData* InitializeThreadSlots() {
    // Native initialization publishes a pointer to module-lifetime storage
    // after construction. Slot storage itself outlives all module slot owners.
    if (!threadSlots)
        threadSlots = ::new (slotStorage) CThreadSlotData;
    return threadSlots;
}
} // namespace cthreadlocalobject
} // namespace detail
} // namespace openmfc
