#include <windows.h>
#include <iostream>

typedef void* (*P_MALLOC)(size_t);
typedef void* (*P_TESTFUNC)(int);
typedef int (*P_EXCEPT)();

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
    // Note: This often FAILS if CRTs don't match (UCRT vs MSVCRT).
    // We expect this to be the risky part.
    auto shimMalloc = (P_MALLOC)GetProcAddress(hDll, "Shim_Malloc");
    if (shimMalloc) {
        void* ptr = shimMalloc(128);
        std::cout << "[Host] Allocated " << ptr << " in DLL." << std::endl;

        // DANGER ZONE: Freeing memory allocated in MinGW from MSVC
        try {
            free(ptr);
            std::cout << "[Pass?] Freed memory (No crash detected yet)." << std::endl;
        } catch (...) {
            std::cerr << "[Fail] Crash during free()!" << std::endl;
            return 1;
        }
    }

    std::cout << "[Done] All tests executed." << std::endl;
    return 0;
}
