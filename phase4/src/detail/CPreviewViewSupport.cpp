#include "CPreviewViewSupport.h"

namespace openmfc { namespace detail { namespace cpreviewview {
int& PV_INT(void* p, size_t off) {
    return *reinterpret_cast<int*>(reinterpret_cast<char*>(p) + off);
}
unsigned int& PV_UINT(void* p, size_t off) {
    return *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(p) + off);
}
void*& PV_PTR(void* p, size_t off) {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(p) + off);
}
HWND PV_HWND(void* p) {
    return *reinterpret_cast<HWND*>(reinterpret_cast<char*>(p) + 0x40);
}
} } }  // namespace openmfc::detail::cpreviewview
