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
// Symbol: ?Close@CAsyncMonikerFile@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CAsyncMonikerFile__UEAAXXZ() {}

// Symbol: ?CreateBindStatusCallback@CAsyncMonikerFile@@MEAAPEAUIUnknown@@PEAU2@@Z
extern "C" void* MS_ABI impl__CreateBindStatusCallback_CAsyncMonikerFile__MEAAPEAUIUnknown__PEAU2__Z(void* /*struct*/* p0) {
    return nullptr;
}

// Symbol: ?EndCallbacks@CAsyncMonikerFile@@IEAAXXZ
extern "C" void MS_ABI impl__EndCallbacks_CAsyncMonikerFile__IEAAXXZ() {}

// Symbol: ?GetBindInfo@CAsyncMonikerFile@@MEBAKXZ
extern "C" unsigned long MS_ABI impl__GetBindInfo_CAsyncMonikerFile__MEBAKXZ() {
    return 0;
}

// Symbol: ?GetPriority@CAsyncMonikerFile@@MEBAJXZ
extern "C" long MS_ABI impl__GetPriority_CAsyncMonikerFile__MEBAJXZ() {
    return 0;
}

// Symbol: ?OnDataAvailable@CAsyncMonikerFile@@MEAAXKK@Z
extern "C" void MS_ABI impl__OnDataAvailable_CAsyncMonikerFile__MEAAXKK_Z(unsigned long p0, unsigned long p1) {}

// Symbol: ?OnLowResource@CAsyncMonikerFile@@MEAAXXZ
extern "C" void MS_ABI impl__OnLowResource_CAsyncMonikerFile__MEAAXXZ() {}

// Symbol: ?OnStartBinding@CAsyncMonikerFile@@MEAAXXZ
extern "C" void MS_ABI impl__OnStartBinding_CAsyncMonikerFile__MEAAXXZ() {}

// Symbol: ?OnStopBinding@CAsyncMonikerFile@@MEAAXJPEB_W@Z
extern "C" void MS_ABI impl__OnStopBinding_CAsyncMonikerFile__MEAAXJPEB_W_Z(long p0, const wchar_t* p1) {}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEAUIMoniker@@PEAUIBindHost@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEAUIMoniker__PEAUIBindHost__PEAVCFileException___Z(void* /*struct*/* p0, void* /*struct*/* p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEAUIMoniker@@PEAUIServiceProvider@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEAUIMoniker__PEAUIServiceProvider__PEAVCFileException___Z(void* /*struct*/* p0, void* /*struct*/* p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEAUIMoniker@@PEAUIUnknown@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEAUIMoniker__PEAUIUnknown__PEAVCFileException___Z(void* /*struct*/* p0, void* /*struct*/* p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEAUIMoniker@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEAUIMoniker__PEAVCFileException___Z(void* /*struct*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEB_WPEAUIBindHost@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAUIBindHost__PEAVCFileException___Z(const wchar_t* p0, void* /*struct*/* p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEB_WPEAUIServiceProvider@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAUIServiceProvider__PEAVCFileException___Z(const wchar_t* p0, void* /*struct*/* p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEB_WPEAUIUnknown@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAUIUnknown__PEAVCFileException___Z(const wchar_t* p0, void* /*struct*/* p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEB_WPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAVCFileException___Z(const wchar_t* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?PostBindToStream@CAsyncMonikerFile@@MEAAHPEAVCFileException@@@Z
extern "C" int MS_ABI impl__PostBindToStream_CAsyncMonikerFile__MEAAHPEAVCFileException___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@CAsyncMonikerFile@@UEAAIPEAXI@Z
extern "C" unsigned int MS_ABI impl__Read_CAsyncMonikerFile__UEAAIPEAXI_Z(void* p0, unsigned int p1) {
    return 0;
}
