// MinGW import test for OpenMFC (MSVC-mangled exports)
//
// This test is intentionally *not* using MFC C++ classes, because MinGW uses
// the Itanium C++ ABI. Instead, it imports selected MSVC-mangled exports via
// asm labels and calls a few "safe" ones.
//
// Goal: verify the DLL/export table + import library are usable from MinGW when
// names are provided explicitly.

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

#if defined(__GNUC__)
#  define DLLIMPORT __attribute__((dllimport))
#else
#  define DLLIMPORT __declspec(dllimport)
#endif

extern "C" {
// unsigned long __cdecl AfxGetDllVersion(void)
DLLIMPORT std::uint32_t AfxGetDllVersion() __asm__("?AfxGetDllVersion@@YAKXZ");

// void* __cdecl AfxGetThread(void)   (real signature returns CWinThread*)
DLLIMPORT void* AfxGetThread() __asm__("?AfxGetThread@@YAPEAVCWinThread@@XZ");

// void* __cdecl operator new(size_t)
DLLIMPORT void* mfc_operator_new(std::size_t size) __asm__("??2@YAPEAX_K@Z");

// void __cdecl operator delete(void*)
DLLIMPORT void mfc_operator_delete(void* ptr) __asm__("??3@YAXPEAX@Z");
}

static int test_operator_new_delete() {
    std::printf("Test: MFC exported operator new/delete... ");

    void* ptr = mfc_operator_new(64);
    if (!ptr) {
        std::printf("FAIL (null)\n");
        return 1;
    }

    std::memset(ptr, 0xA5, 64);
    mfc_operator_delete(ptr);

    std::printf("OK\n");
    return 0;
}

static int test_version() {
    std::printf("Test: AfxGetDllVersion... ");
    const std::uint32_t v = AfxGetDllVersion();
    std::printf("0x%08X ", v);
    if (v != 0x00000E00u) {
        std::printf("FAIL (expected 0x00000E00)\n");
        return 1;
    }
    std::printf("OK\n");
    return 0;
}

static int test_afx_get_thread() {
    std::printf("Test: AfxGetThread import... ");
    void* p = AfxGetThread();
    // In this test there's no global CWinApp instance, so nullptr is fine.
    std::printf("OK (%p)\n", p);
    return 0;
}

int main() {
    std::printf("=== OpenMFC MinGW MSVC-mangled import test ===\n");

    int fails = 0;
    fails += test_operator_new_delete();
    fails += test_version();
    fails += test_afx_get_thread();

    if (fails == 0) {
        std::printf("PASS\n");
        return 0;
    }
    std::printf("FAIL (%d)\n", fails);
    return 1;
}

