#include "MemcoreSupport.h"

namespace openmfc { namespace detail { namespace memcore {
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size) {
    if (size == 0) {
        size = 1;
    }
    return std::malloc(size);
}
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr) {
    std::free(ptr);
}
extern "C" void* MS_ABI impl___U_YAPEAX_K_Z(std::size_t size) {
    return impl___2_YAPEAX_K_Z(size);
}
extern "C" void MS_ABI impl___V_YAXPEAX_Z(void* ptr) {
    impl___3_YAXPEAX_Z(ptr);
}
extern "C" void* MS_ABI impl____U_YAPEAX_K_Z(std::size_t size) {
    return impl___2_YAPEAX_K_Z(size);
}
extern "C" void MS_ABI impl____V_YAXPEAX_Z(void* ptr) {
    impl___3_YAXPEAX_Z(ptr);
}
extern "C" void impl__AfxDebugBreak() {
    DebugBreak();
}
extern "C" int impl__AfxCheckMemory() {
    #ifdef _DEBUG
    return _CrtCheckMemory();
    #else
    return TRUE;
    #endif
}
void DumpToDebugger(const wchar_t* psz) {
    if (psz) {
        OutputDebugStringW(psz);
        fwprintf(stderr, L"%s", psz);
    }
}
extern "C" void impl__AfxDump__PB_W(const wchar_t* psz) {
    DumpToDebugger(psz);
}
extern "C" void impl__AfxDumpStack(unsigned long dwFlags) {
    (void)dwFlags;
    OutputDebugStringW(L"AfxDumpStack: (stack trace not available on this platform)\n");
}
} } }  // namespace openmfc::detail::memcore
