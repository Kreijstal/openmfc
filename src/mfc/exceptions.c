#include "openmfc/eh_rtti.h"
#include <stdlib.h>

// Import _CxxThrowException from vcruntime140.dll
// Note: On MinGW, this might be named differently or require specific declaration.
// For x64, it's usually available.
#ifdef _WIN32
#define STDCALL __attribute__((stdcall))
#else
#define STDCALL
#endif

__attribute__((dllimport)) void STDCALL _CxxThrowException(void* pExceptionObject, const ThrowInfo* pThrowInfo);

// External helpers implemented in C++ (afxmem.cpp)
void* OpenMFC_New_CMemoryException(void);
void* OpenMFC_New_CFileException(int cause, int lOsError, const char* pFileName);

// Public API implementation (called by stubs)

void AfxThrowMemoryException(void) {
    void* pEx = OpenMFC_New_CMemoryException();
    _CxxThrowException(pEx, &TI_CMemoryException);
}

void AfxThrowFileException(int cause, int lOsError, const char* pFileName) {
    void* pEx = OpenMFC_New_CFileException(cause, lOsError, pFileName);
    _CxxThrowException(pEx, &TI_CFileException);
}

// Thunks for copy constructors (referenced by RTTI)
// For now, these are just stubs. In a real implementation, they would call the C++ copy ctor.
void STDCALL CException_CopyCtor(void* dest, void* src) {
    // TODO: Implement copy logic if needed.
    // For simple exceptions, shallow copy might be enough, or we might need to call the actual copy ctor.
    // Since we don't have the C++ class definition here, we can't easily do it.
    // But for the smoke test, we might not need it if we catch by reference or pointer.
    // Catching by value requires this.
}
