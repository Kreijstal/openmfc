// Memory core (global new/delete exported by MFC)
//
// MSVC clients may resolve global operator new/delete from mfc140u.dll.
// If we leave these as auto-generated "void" stubs, MSVC code will treat the
// uninitialized RAX value as a pointer and crash immediately.

#define OPENMFC_APPCORE_IMPL
#include <cstddef>
#include <cstdlib>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

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

// Array new/delete use a slightly different mangling and stub naming in our generator.
extern "C" void* MS_ABI impl____U_YAPEAX_K_Z(std::size_t size) {
    return impl___2_YAPEAX_K_Z(size);
}

extern "C" void MS_ABI impl____V_YAXPEAX_Z(void* ptr) {
    impl___3_YAXPEAX_Z(ptr);
}
