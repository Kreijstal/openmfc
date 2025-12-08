// Exception implementations for OpenMFC
// For now, these just log to maintain compatibility with existing tests
// In the future, we need to implement proper SEH-based exception throwing

#include <cstdio>
#include <cstdlib>
#include <windows.h>

// MS ABI calling convention for x64
#if defined(__GNUC__)
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {
    fprintf(stderr, "AfxThrowMemoryException: Memory allocation failed (stub)\n");
    // TODO: Implement proper CMemoryException throwing via SEH
}

extern "C" void MS_ABI stub__AfxThrowFileException__YAXHJPB_W_Z(int cause, LONG lOsError, const wchar_t* lpszFileName) {
    fprintf(stderr, "AfxThrowFileException: cause=%d, error=%ld (stub)\n", cause, lOsError);
    if (lpszFileName) {
        fprintf(stderr, "  file: %ls\n", lpszFileName);
    }
    // TODO: Implement proper CFileException throwing via SEH
}

extern "C" void MS_ABI stub__AfxThrowResourceException__YAXXZ() {
    fprintf(stderr, "AfxThrowResourceException (stub)\n");
    // TODO: Implement proper CResourceException throwing via SEH
}

extern "C" void MS_ABI stub__AfxThrowNotSupportedException__YAXXZ() {
    fprintf(stderr, "AfxThrowNotSupportedException (stub)\n");
    // TODO: Implement proper CNotSupportedException throwing via SEH
}

extern "C" void MS_ABI stub__AfxThrowUserException__YAXXZ() {
    fprintf(stderr, "AfxThrowUserException (stub)\n");
    // TODO: Implement proper CUserException throwing via SEH
}

extern "C" void MS_ABI stub__AfxThrowInvalidArgException__YAXXZ() {
    fprintf(stderr, "AfxThrowInvalidArgException (stub)\n");
    // TODO: Implement proper CInvalidArgException throwing via SEH
}

// AfxAbort - terminates the application
extern "C" void MS_ABI stub__AfxAbort__YAXXZ() {
    fprintf(stderr, "AfxAbort: Terminating application (stub)\n");
    abort();
}