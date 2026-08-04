// CAsyncMonikerFile — OpenMFC implementation.
// Sources: filecore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?OnProgress@CAsyncMonikerFile@@MEAAXKKKPEB_W@Z
extern "C" void MS_ABI impl__OnProgress_CAsyncMonikerFile__MEAAXKKKPEB_W_Z(
    void* /*CAsyncMonikerFile**/ pThis, unsigned long ulProgress, unsigned long ulProgressMax,
    unsigned long ulStatusCode, const wchar_t* wszStatusText) {
    (void)pThis; (void)ulProgress; (void)ulProgressMax; (void)ulStatusCode; (void)wszStatusText;
    // Default MFC implementation is a no-op callback for download progress.
}
// Symbol: ??0CAsyncMonikerFile@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAsyncMonikerFile__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CAsyncMonikerFile@@UEAA@XZ
extern "C" void MS_ABI impl___1CAsyncMonikerFile__UEAA_XZ(void* pThis) {
    (void)pThis;
}
