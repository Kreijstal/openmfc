#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdlib>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ??2CNoTrackObject@@SAPEAX_K@Z
extern "C" void* MS_ABI impl___2CNoTrackObject__SAPEAX_K_Z(std::size_t size) {
    if (size == 0) {
        size = 1;
    }
    return std::malloc(size);
}

// Symbol: ??3CNoTrackObject@@SAXPEAX@Z
extern "C" void MS_ABI impl___3CNoTrackObject__SAXPEAX_Z(void* ptr) {
    std::free(ptr);
}
