#include "CMFCControlContainerSupport.h"

static_assert(offsetof(CContainer, m_pWnd) == 0x8, "m_pWnd @0x08");
static_assert(offsetof(CContainer, m_pControls) == 0x18, "m_pControls @0x18");
static_assert(offsetof(CContainer, m_nControls) == 0x20, "m_nControls @0x20");
namespace openmfc { namespace detail { namespace cmfccontrolcontainer {
void ContainerArrayAppend(CContainer* s, void* pObj)
{
    if (s->m_nControls >= s->m_nMaxControls) {
        long long nNew = (s->m_nMaxControls > 0) ? s->m_nMaxControls * 2 : 4;
        if (nNew <= s->m_nControls) nNew = s->m_nControls + 1;
        void** pNew = reinterpret_cast<void**>(
            std::realloc(s->m_pControls, static_cast<size_t>(nNew) * sizeof(void*)));
        if (pNew == nullptr) return;   // allocation failure: no append
        s->m_pControls = pNew;
        s->m_nMaxControls = nNew;
    }
    s->m_pControls[s->m_nControls++] = pObj;
}
} } }  // namespace openmfc::detail::cmfccontrolcontainer
