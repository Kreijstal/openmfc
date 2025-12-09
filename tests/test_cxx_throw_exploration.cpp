// MSVC Exploration: How _CxxThrowException works
//
// This test explores the internal structure of MSVC C++ exceptions
// to understand what OpenMFC needs to provide.
//
// Build with MSVC:
//   cl /nologo /EHsc /MD /FA test_cxx_throw_exploration.cpp
//   (the /FA flag generates assembly listing to see _CxxThrowException call)
//
// Key findings:
// - _CxxThrowException is in vcruntime140.dll
// - It takes: void* pExceptionObject, _ThrowInfo* pThrowInfo
// - ThrowInfo contains type descriptors that must match catch handlers

#include <cstdio>
#include <cstdint>
#include <windows.h>
#include <typeinfo>

// MSVC's C++ exception code
#define EH_EXCEPTION_NUMBER 0xE06D7363  // 'msc' | 0xE0000000

// Simple class to throw
class MyException {
public:
    int code;
    const char* msg;

    MyException(int c, const char* m) : code(c), msg(m) {
        printf("  MyException constructed: code=%d, msg=%s\n", code, msg);
    }

    ~MyException() {
        printf("  MyException destroyed\n");
    }
};

// Test 1: Normal C++ throw/catch
void test_normal_throw() {
    printf("\n--- Test 1: Normal C++ throw/catch ---\n");

    try {
        printf("Throwing MyException...\n");
        throw MyException(42, "test error");
    }
    catch (MyException& e) {
        printf("Caught MyException: code=%d, msg=%s\n", e.code, e.msg);
    }
}

// Test 2: Examine what the compiler generates for throw
void test_throw_structure() {
    printf("\n--- Test 2: Examining throw structure ---\n");

    // When MSVC compiles: throw MyException(42, "test");
    // It generates roughly:
    //   1. Construct exception object on stack
    //   2. Call _CxxThrowException(&obj, &_TI_MyException)
    //
    // _TI_MyException is a static structure containing:
    //   - Pointer to destructor
    //   - Pointer to catch type array
    //   - Each catch type has RTTI type_info*

    // We can see the type_info for MyException:
    const std::type_info& ti = typeid(MyException);
    printf("type_info for MyException:\n");
    printf("  name() = %s\n", ti.name());
    printf("  raw_name() = %s\n", ti.raw_name());
    printf("  &typeid = %p\n", (void*)&ti);
}

// Test 3: SEH filter to inspect C++ exception structure
LONG WINAPI exception_filter(EXCEPTION_POINTERS* ep) {
    printf("\n  In exception filter:\n");
    printf("  ExceptionCode = 0x%08lX\n", ep->ExceptionRecord->ExceptionCode);

    if (ep->ExceptionRecord->ExceptionCode == EH_EXCEPTION_NUMBER) {
        printf("  This is a C++ exception!\n");
        printf("  NumberParameters = %lu\n", ep->ExceptionRecord->NumberParameters);

        // For C++ exceptions, parameters are:
        // [0] = magic number (0x19930520 for x86, different for x64)
        // [1] = pointer to exception object
        // [2] = pointer to _ThrowInfo
        // [3] = (x64 only) module base for RVA calculations

        if (ep->ExceptionRecord->NumberParameters >= 3) {
            printf("  Param[0] (magic) = 0x%p\n",
                   (void*)ep->ExceptionRecord->ExceptionInformation[0]);
            printf("  Param[1] (object) = 0x%p\n",
                   (void*)ep->ExceptionRecord->ExceptionInformation[1]);
            printf("  Param[2] (throw info) = 0x%p\n",
                   (void*)ep->ExceptionRecord->ExceptionInformation[2]);
#ifdef _WIN64
            if (ep->ExceptionRecord->NumberParameters >= 4) {
                printf("  Param[3] (image base) = 0x%p\n",
                       (void*)ep->ExceptionRecord->ExceptionInformation[3]);
            }
#endif
        }
    }

    return EXCEPTION_EXECUTE_HANDLER;  // Handle it in SEH
}

// Helper function that throws - separated to avoid mixing C++ EH and SEH
__declspec(noinline) void throw_my_exception() {
    throw MyException(99, "seh test");
}

// Use SEH-only in this function (no C++ objects that need unwinding)
void test_seh_inspection() {
    printf("\n--- Test 3: SEH inspection of C++ exception ---\n");

    __try {
        throw_my_exception();
    }
    __except (exception_filter(GetExceptionInformation())) {
        printf("Caught in SEH handler after filter inspection\n");
    }
}

int main() {
    printf("=== MSVC C++ Exception Exploration ===\n");

    test_normal_throw();
    test_throw_structure();
    test_seh_inspection();

    printf("\n=== Exploration Complete ===\n");
    printf("\nKey insight: To throw exceptions that MSVC can catch,\n");
    printf("OpenMFC must call _CxxThrowException with proper ThrowInfo\n");
    printf("that contains matching type_info pointers.\n");

    return 0;
}
