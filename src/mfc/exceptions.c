#include "openmfc/eh_rtti.h"
#include <stdlib.h>

// Import _CxxThrowException from vcruntime140.dll
#ifdef _MSC_VER
    // MSVC syntax
    __declspec(dllimport) void __stdcall _CxxThrowException(void* pExceptionObject, const ThrowInfo* pThrowInfo);
#else
    // GCC/MinGW syntax
    __attribute__((dllimport)) void __attribute__((stdcall)) _CxxThrowException(void* pExceptionObject, const ThrowInfo* pThrowInfo);
#endif

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
#ifdef _MSC_VER
void __stdcall CException_CopyCtor(void* dest, void* src) {
#else
void __attribute__((stdcall)) CException_CopyCtor(void* dest, void* src) {
#endif
    // TODO: Implement copy logic if needed.
    // For simple exceptions, shallow copy might be enough, or we might need to call the actual copy ctor.
    // Since we don't have the C++ class definition here, we can't easily do it.
    // But for the smoke test, we might not need it if we catch by reference or pointer.
    // Catching by value requires this.
}
