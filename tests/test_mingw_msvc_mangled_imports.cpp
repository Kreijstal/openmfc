// MinGW test for OpenMFC (MSVC-mangled exports)
//
// This test is intentionally *not* using MFC C++ classes, because MinGW uses
// the Itanium C++ ABI. Instead, it resolves selected MSVC-mangled exports by
// name using Win32 APIs and calls a few "safe" ones.
//
// Goal: verify the DLL export table is usable from MinGW when names are
// provided explicitly.

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static FARPROC must_get(HMODULE dll, const char* name) {
    FARPROC p = GetProcAddress(dll, name);
    if (!p) {
        std::printf("FAIL: GetProcAddress(%s) (err=%lu)\n", name, (unsigned long)GetLastError());
    }
    return p;
}

using AfxGetDllVersionFn = std::uint32_t(__cdecl*)();
using AfxGetThreadFn = void*(__cdecl*)();
using OperatorNewFn = void*(__cdecl*)(std::size_t);
using OperatorDeleteFn = void(__cdecl*)(void*);

static AfxGetDllVersionFn pAfxGetDllVersion = nullptr;
static AfxGetThreadFn pAfxGetThread = nullptr;
static OperatorNewFn pOperatorNew = nullptr;
static OperatorDeleteFn pOperatorDelete = nullptr;

static int test_operator_new_delete() {
    std::printf("Test: MFC exported operator new/delete... ");

    void* ptr = pOperatorNew(64);
    if (!ptr) {
        std::printf("FAIL (null)\n");
        return 1;
    }

    std::memset(ptr, 0xA5, 64);
    pOperatorDelete(ptr);

    std::printf("OK\n");
    return 0;
}

static int test_version() {
    std::printf("Test: AfxGetDllVersion... ");
    const std::uint32_t v = pAfxGetDllVersion();
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
    void* p = pAfxGetThread();
    // In this test there's no global CWinApp instance, so nullptr is fine.
    std::printf("OK (%p)\n", p);
    return 0;
}

int main() {
    std::printf("=== OpenMFC MinGW MSVC-mangled import test ===\n");

    HMODULE dll = LoadLibraryW(L"openmfc.dll");
    if (!dll) {
        std::printf("FAIL: LoadLibraryW(openmfc.dll) (err=%lu)\n", (unsigned long)GetLastError());
        return 1;
    }

    // Resolve by MSVC-mangled export name.
    pAfxGetDllVersion = reinterpret_cast<AfxGetDllVersionFn>(must_get(dll, "?AfxGetDllVersion@@YAKXZ"));
    pAfxGetThread = reinterpret_cast<AfxGetThreadFn>(must_get(dll, "?AfxGetThread@@YAPEAVCWinThread@@XZ"));
    pOperatorNew = reinterpret_cast<OperatorNewFn>(must_get(dll, "??2@YAPEAX_K@Z"));
    pOperatorDelete = reinterpret_cast<OperatorDeleteFn>(must_get(dll, "??3@YAXPEAX@Z"));

    if (!pAfxGetDllVersion || !pAfxGetThread || !pOperatorNew || !pOperatorDelete) {
        return 1;
    }

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
