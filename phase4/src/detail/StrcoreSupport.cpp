#include "StrcoreSupport.h"

namespace openmfc { namespace detail { namespace strcore {
HINSTANCE GetResourceHandle() {
    HINSTANCE hInst = AfxGetResourceHandle();
    if (hInst == nullptr) {
        hInst = AfxGetInstanceHandle();
    }
    return hInst;
}
wchar_t NormalizePathChar(wchar_t ch)
{
    if (ch == L'/') return L'\\';
    return static_cast<wchar_t>(std::towlower(static_cast<wint_t>(ch)));
}
} } }  // namespace openmfc::detail::strcore
