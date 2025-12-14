// MinGW link-time test for OpenMFC (MSVC-mangled exports)
//
// This test intentionally avoids MFC C++ classes (MinGW uses the Itanium C++
// ABI). Instead, CI generates a tiny *ordinal-only import library* that maps
// clean C names to specific ordinals in openmfc.dll.
//
// Goal: verify we can link and run without GetProcAddress.

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" {
__declspec(dllimport) std::uint32_t __cdecl AfxGetDllVersion();
__declspec(dllimport) void* __cdecl AfxGetThread();

// These are mapped to MSVC's exported global operator new/delete by the
// ordinal-only import library generated in CI.
__declspec(dllimport) void* __cdecl mfc_operator_new(std::size_t size);
__declspec(dllimport) void __cdecl mfc_operator_delete(void* ptr);
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
