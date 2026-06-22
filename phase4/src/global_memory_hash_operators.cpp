#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdlib>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Global scalar/array operator new[] and operator delete[].
// MFC re-exports these; forward to the C++ runtime allocator.

// Symbol: ??_U@YAPEAX_K@Z
extern "C" void* MS_ABI impl____U_YAPEAX_K_Z(std::size_t size) {
    // operator new[](size_t): forward to the array allocation function.
    return ::operator new[](size);
}

// Symbol: ??_V@YAXPEAX@Z
extern "C" void MS_ABI impl____V_YAXPEAX_Z(void* p) {
    // operator delete[](void*): pair with operator new[].
    ::operator delete[](p);
}

// MFC HashKey<> template helpers for string pointer keys.
// Standard MFC hash: nHash = (nHash << 5) + nHash + ch  (i.e. *33).

// Symbol: ??$HashKey@PEB_W@@YAIPEB_W@Z
extern "C" unsigned int MS_ABI impl____HashKey_PEB_W__YAIPEB_W_Z(const wchar_t* key) {
    unsigned int nHash = 0;
    if (key) {
        while (*key) {
            nHash = (nHash << 5) + nHash + static_cast<unsigned int>(*key++);
        }
    }
    return nHash;
}

// Symbol: ??$HashKey@PEBD@@YAIPEBD@Z
extern "C" unsigned int MS_ABI impl____HashKey_PEBD__YAIPEBD_Z(const char* key) {
    unsigned int nHash = 0;
    if (key) {
        while (*key) {
            nHash = (nHash << 5) + nHash +
                    static_cast<unsigned int>(static_cast<unsigned char>(*key++));
        }
    }
    return nHash;
}
