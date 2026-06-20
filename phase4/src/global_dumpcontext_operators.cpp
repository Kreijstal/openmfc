#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdlib>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CNoTrackObject::operator new / delete are static allocators used by MFC for
// allocations that should not participate in debug memory tracking. A thin
// forward to the CRT heap is behaviorally correct.

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

// CDumpContext::operator<< overloads. CDumpContext is a debug-only diagnostic
// output stream; each overload returns a reference to the stream (its `this`).
// Returning `this` unchanged is the correct chaining behavior and produces no
// observable side effects in a release build (where dump output is suppressed).

// Symbol: ??6CDumpContext@@QEAAAEAV0@_J@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0__J_Z(void* self, long long) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@_K@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0__K_Z(void* self, unsigned long long) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@AEBVCObject@@@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_AEBVCObject___Z(void* self, const void*) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@E@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_E_Z(void* self, unsigned char) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@G@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_G_Z(void* self, unsigned short) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@H@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_H_Z(void* self, int) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@I@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_I_Z(void* self, unsigned int) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@J@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_J_Z(void* self, long) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@K@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_K_Z(void* self, unsigned long) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHACCEL__@@@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHACCEL_____Z(void* self, void*) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHDC__@@@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHDC_____Z(void* self, void*) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHFONT__@@@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHFONT_____Z(void* self, void*) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHMENU__@@@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHMENU_____Z(void* self, void*) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHWND__@@@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHWND_____Z(void* self, void*) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEB_W@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEB_W_Z(void* self, const wchar_t*) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEBD@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEBD_Z(void* self, const char*) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEBVCObject@@@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEBVCObject___Z(void* self, const void*) {
    return self;
}

// Symbol: ??6CDumpContext@@QEAAAEAV0@PEBX@Z
extern "C" void* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEBX_Z(void* self, const void*) {
    return self;
}
