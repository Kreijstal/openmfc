#include "StringAtlThunksSupport.h"

namespace openmfc { namespace detail { namespace stringatlthunks {
HINSTANCE GetResHandle() {
    HINSTANCE h = AfxGetResourceHandle();
    if (!h) h = AfxGetInstanceHandle();
    if (!h) h = (HINSTANCE)GetModuleHandleW(nullptr);
    return h;
}
} } }  // namespace openmfc::detail::stringatlthunks
