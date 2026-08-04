// CPropExchange — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?ExchangeVersion@CPropExchange@@UEAAHAEAKKH@Z
extern "C" int MS_ABI impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z(
    void* /*pThis*/, unsigned long* pVersion, unsigned long preferredVersion, int) {
    if (pVersion == nullptr) {
        return 0;
    }

    const unsigned long loadedVersion = *pVersion;
    if (loadedVersion == 0) {
        *pVersion = preferredVersion;
        return 1;
    }

    *pVersion = loadedVersion;
    return (loadedVersion <= preferredVersion) ? 1 : 0;
}
