// OpenMFC class-specific generated fallback stubs for unresolved symbols

#include <cstddef>
#include <objbase.h>
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ?Attach@CMonikerFile@@IEAAHPEAUIMoniker@@PEAUIBindHost@@PEAUIBindStatusCallback@@PEAUIBindCtx@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Attach_CMonikerFile__IEAAHPEAUIMoniker__PEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(void* /*struct*/* p0, void* /*struct*/* p1, void* /*struct*/* p2, void* /*struct*/* p3, void* /*class*/* p4) {
    return 0;
}

// Symbol: ?Attach@CMonikerFile@@IEAAHPEB_WPEAUIBindHost@@PEAUIBindStatusCallback@@PEAUIBindCtx@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Attach_CMonikerFile__IEAAHPEB_WPEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(const wchar_t* p0, void* /*struct*/* p1, void* /*struct*/* p2, void* /*struct*/* p3, void* /*class*/* p4) {
    return 0;
}

// Symbol: ?Close@CMonikerFile@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CMonikerFile__UEAAXXZ() {}

// Symbol: ?CreateBindContext@CMonikerFile@@IEAAPEAUIBindCtx@@PEAVCFileException@@@Z
extern "C" IBindCtx* MS_ABI impl__CreateBindContext_CMonikerFile__IEAAPEAUIBindCtx__PEAVCFileException___Z(
    void* pThis, void* pException) {
    (void)pThis;
    (void)pException;
    IBindCtx* pBindCtx = nullptr;
    if (FAILED(::CreateBindCtx(0, &pBindCtx))) return nullptr;
    return pBindCtx;
}

// Symbol: ?CreateBindHost@CMonikerFile@@KAPEAUIBindHost@@XZ
extern "C" void* MS_ABI impl__CreateBindHost_CMonikerFile__KAPEAUIBindHost__XZ() {
    return nullptr;
}

// Symbol: ?Detach@CMonikerFile@@QEAAHPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Detach_CMonikerFile__QEAAHPEAVCFileException___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Flush@CMonikerFile@@UEAAXXZ
extern "C" void MS_ABI impl__Flush_CMonikerFile__UEAAXXZ() {}

// Symbol: ?Open@CMonikerFile@@MEAAHPEAUIMoniker@@PEAUIBindHost@@PEAUIBindStatusCallback@@PEAUIBindCtx@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CMonikerFile__MEAAHPEAUIMoniker__PEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(void* /*struct*/* p0, void* /*struct*/* p1, void* /*struct*/* p2, void* /*struct*/* p3, void* /*class*/* p4) {
    return 0;
}

// Symbol: ?Open@CMonikerFile@@MEAAHPEB_WPEAUIBindHost@@PEAUIBindStatusCallback@@PEAUIBindCtx@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CMonikerFile__MEAAHPEB_WPEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(const wchar_t* p0, void* /*struct*/* p1, void* /*struct*/* p2, void* /*struct*/* p3, void* /*class*/* p4) {
    return 0;
}

// Symbol: ?Open@CMonikerFile@@UEAAHPEAUIMoniker@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CMonikerFile__UEAAHPEAUIMoniker__PEAVCFileException___Z(void* /*struct*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?Open@CMonikerFile@@UEAAHPEB_WPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CMonikerFile__UEAAHPEB_WPEAVCFileException___Z(const wchar_t* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?PostBindToStream@CMonikerFile@@MEAAHPEAVCFileException@@@Z
extern "C" int MS_ABI impl__PostBindToStream_CMonikerFile__MEAAHPEAVCFileException___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ??1CMonikerFile@@UEAA@XZ
extern "C" void MS_ABI impl___1CMonikerFile__UEAA_XZ(void* pThis) {
    if (pThis) static_cast<COleStreamFile*>(pThis)->~COleStreamFile();
}
