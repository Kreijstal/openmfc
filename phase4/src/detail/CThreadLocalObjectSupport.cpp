#include "CThreadLocalObjectSupport.h"

static_assert(sizeof(S_Cthreadlocalobject) == 4, "CThreadLocalObject must be 4 bytes");
static_assert(offsetof(S_Cthreadlocalobject, m_nSlot) == 0, "m_nSlot at offset 0");
namespace openmfc { namespace detail { namespace cthreadlocalobject {
Store& store() {
    Store s;
    return s;
}
DWORD allocSlot() {
    Store& s = store();
    EnterCriticalSection(&s.cs);
    DWORD slot = s.nextSlot++;
    LeaveCriticalSection(&s.cs);
    return slot;
}
void destroyValue(void* val) {
    if (val == nullptr) return;
    void** vtbl = *reinterpret_cast<void***>(val);
    if (vtbl == nullptr) return;
    typedef void* (MS_ABI *DelDtor)(void*, unsigned);
    reinterpret_cast<DelDtor>(vtbl[0])(val, 1u); // flag bit0 => operator delete
}
} } }  // namespace openmfc::detail::cthreadlocalobject
