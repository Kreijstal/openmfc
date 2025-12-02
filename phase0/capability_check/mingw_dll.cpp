#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Build with: x86_64-w64-mingw32-g++ -shared ...

extern "C" {

    // 1. Heap Interop Test
    // We allocate memory here. The Host will try to free it.
    // If CRTs are mismatched, this crashes.
    __declspec(dllexport) void* Shim_Malloc(size_t size) {
        return malloc(size);
    }
    // Pairing free to keep allocation/deallocation inside the DLL's CRT.
    __declspec(dllexport) void Shim_Free(void* ptr) {
        free(ptr);
    }

    // 2. Symbol Export Test
    // We will export this via .def file with a specific mangled name.
    // The implementation is plain C, but the export will look like C++.
    void* Shim_TestFunc(int a) {
        // Return a pointer value based on input to prove we ran
        return (void*)(uintptr_t)(a * 2);
    }

    // 3. Exception Sanity
    // Ensure standard C++ exceptions don't leak into MSVC frames.
    __declspec(dllexport) int Shim_SafeException() {
        try {
            throw 1;
        } catch (...) {
            return 42; // Success
        }
        return 0; // Fail
    }
}
