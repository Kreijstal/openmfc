#include "MemcoreSupport.h"
#include "ModuleThreadStateSupport.h"

namespace openmfc { namespace detail { namespace memcore {
// Symbol: ??2@YAPEAX_K@Z
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size) {
    using Handler = AFX_MODULE_THREAD_STATE::NewHandler;
    const auto uninitialized = reinterpret_cast<Handler>(UINT_PTR(-1));
    Handler handler = uninitialized;
    for (;;) {
        void* memory = std::malloc(size);
        if (memory)
            return memory;
        if (handler == uninitialized)
            handler = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pfnNewHandler;
        if (!handler || !handler(size))
            return nullptr;
    }
}
// Symbol: ??3@YAXPEAX@Z
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
