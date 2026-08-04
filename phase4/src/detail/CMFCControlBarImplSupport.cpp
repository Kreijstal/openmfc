#include "CMFCControlBarImplSupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl___1CMFCControlBarImpl__UEAA_XZ(void* pThis);

static_assert(sizeof(S_Cmfccontrolbarimpl) == 16, "CMFCControlBarImpl must be 16 bytes");
static_assert(offsetof(S_Cmfccontrolbarimpl, vfptr) == 0, "vfptr at 0");
static_assert(offsetof(S_Cmfccontrolbarimpl, m_pBar) == 8, "m_pBar at 8");
namespace openmfc { namespace detail { namespace cmfccontrolbarimpl {
HWND BarHwnd(void* pBar) {
    if (!pBar) return nullptr;
    return *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pBar) + 64);
}
void* MS_ABI vdtor_Cmfccontrolbarimpl(void* p, unsigned flags) {
    impl___1CMFCControlBarImpl__UEAA_XZ(p);
    if (flags & 1) ::operator delete(p);
    return p;
}
} } }  // namespace openmfc::detail::cmfccontrolbarimpl
