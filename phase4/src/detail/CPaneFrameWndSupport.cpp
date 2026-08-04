#include "CPaneFrameWndSupport.h"

namespace openmfc { namespace detail { namespace cpaneframewnd {
std::mutex g_other6Mutex;
std::unordered_map<void*, PaneFrameState> g_other6State;
CWnd* AsCwnd(void* pThis) {
    return reinterpret_cast<CWnd*>(pThis);
}
HWND HwndOf(void* pThis) {
    CWnd* w = AsCwnd(pThis);
    return w ? w->GetSafeHwnd() : nullptr;
}
PaneFrameState& GetState(void* pThis) {
    return g_other6State[pThis];
}
PaneFrameState* FindStatePtr(const void* pThis) {
    auto it = g_other6State.find(const_cast<void*>(pThis));
    return (it != g_other6State.end()) ? &it->second : nullptr;
}
} } }  // namespace openmfc::detail::cpaneframewnd
