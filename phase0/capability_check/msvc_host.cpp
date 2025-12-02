#include <windows.h>
#include <iostream>
#include <cstdlib>

typedef void* (*P_MALLOC)(size_t);
typedef void* (*P_TESTFUNC)(int);
typedef int (*P_EXCEPT)();
typedef void (*P_FREE)(void*);

static bool env_enabled(const char* name) {
    return GetEnvironmentVariableA(name, nullptr, 0) > 0;
}

int main() {
    std::cout << "[Host] Loading mingw_dll.dll..." << std::endl;
    HMODULE hDll = LoadLibraryA("mingw_dll.dll");
    if (!hDll) {
        std::cerr << "[Fail] Could not load DLL. Error: " << GetLastError() << std::endl;
        return 1;
    }

    // Test 1: Symbol Resolution
    std::cout << "[Host] Resolving symbols..." << std::endl;

    // Resolve by the MANGLED name defined in the .def file
    auto func = (P_TESTFUNC)GetProcAddress(hDll, "?GetValue@CTest@@UEAAPEAXH@Z");
    if (!func) {
        std::cerr << "[Fail] Could not find mangled symbol." << std::endl;
        return 1;
    }

    if (func(10) != (void*)20) {
        std::cerr << "[Fail] Function returned wrong value." << std::endl;
        return 1;
    }
    std::cout << "[Pass] Symbol resolution and call." << std::endl;

    // Test 2: Exception Sanity
    auto safeEx = (P_EXCEPT)GetProcAddress(hDll, "Shim_SafeException");
    if (safeEx && safeEx() == 42) {
        std::cout << "[Pass] Exception caught internally." << std::endl;
    } else {
        std::cerr << "[Fail] Exception test failed." << std::endl;
        return 1;
    }

    // Test 3: Heap Interop
    // Rule: allocation/free must remain in the same CRT. Use DLL-side free.
    auto shimMalloc = (P_MALLOC)GetProcAddress(hDll, "Shim_Malloc");
    auto shimFree = (P_FREE)GetProcAddress(hDll, "Shim_Free");
    if (!shimMalloc || !shimFree) {
        std::cerr << "[Fail] Heap test missing required exports (Shim_Malloc/ Shim_Free)." << std::endl;
        return 1;
    }

    void* ptr = shimMalloc(128);
    std::cout << "[Host] Allocated " << ptr << " in DLL (same CRT expected)." << std::endl;
    shimFree(ptr);
    std::cout << "[Pass] Freed via DLL Shim_Free (same CRT)." << std::endl;

    // Optional forbidden cross-CRT test (disabled by default to avoid intentional crash).
    if (env_enabled("ENABLE_FORBIDDEN_CROSS_FREE")) {
        void* bad = shimMalloc(64);
        std::cout << "[Host] FORBIDDEN test: freeing DLL allocation with host CRT free (may crash)." << std::endl;
        free(bad); // Intentional misuse; only run when explicitly enabled.
        std::cout << "[Warn] Forbidden cross-free did not crash; still disallowed by policy." << std::endl;
    }

    std::cout << "[Done] All tests executed." << std::endl;
    return 0;
}
