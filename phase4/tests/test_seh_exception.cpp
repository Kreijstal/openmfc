#include <windows.h>
#include <stdio.h>

// Test SEH exception structure for C++ exceptions
// Based on MSVC _CxxThrowException implementation

// Magic number for C++ exceptions in SEH
#define EXCEPTION_MICROSOFT_CPLUSPLUS 0xE06D7363  // 'msc' + 0xE0

// Simple test to understand SEH exception structure
int main() {
    printf("Testing SEH exception structure...\n");
    
    // Try to raise a C++-like SEH exception
    __try {
        printf("Before RaiseException\n");
        
        // Parameters for _CxxThrowException:
        // ExceptionCode = 0xE06D7363 (C++ exception)
        // ExceptionFlags = 0x19930520 (throw info version?)
        // NumberOfArguments = 0
        // Arguments = NULL
        
        RaiseException(
            EXCEPTION_MICROSOFT_CPLUSPLUS,  // C++ exception code
            0x19930520,                     // Exception flags (throw info)
            0,                              // Number of arguments
            NULL                            // Arguments
        );
        
        printf("After RaiseException (should not reach here)\n");
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        printf("Caught SEH exception: Code=0x%08X\n", 
               GetExceptionCode());
        
        PEXCEPTION_RECORD pRecord = GetExceptionInformation()->ExceptionRecord;
        printf("ExceptionRecord: Code=0x%08X, Flags=0x%08X\n",
               pRecord->ExceptionCode,
               pRecord->ExceptionFlags);
    }
    
    return 0;
}