// MFC Exception test - uses real MFC headers
//
// Build with MSVC:
//   cl /nologo /EHsc /MD /D_AFXDLL test_exception_mfc.cpp openmfc.lib
//
// This test uses the real MFC headers to ensure our exception throwing
// is compatible with actual MFC code.

#define _AFXDLL
#include <afx.h>
#include <cstdio>

int main() {
    printf("=== MFC Exception Test (Real Headers) ===\n");
    printf("Testing CMemoryException with actual MFC headers\n\n");
    fflush(stdout);

    printf("Test 1: Catch as CMemoryException*\n");
    fflush(stdout);

    try {
        printf("  Calling AfxThrowMemoryException()...\n");
        fflush(stdout);

        AfxThrowMemoryException();

        printf("  ERROR: Should have thrown!\n");
        fflush(stdout);
        return 1;
    }
    catch (CMemoryException* e) {
        printf("  SUCCESS: Caught CMemoryException*!\n");
        printf("    Exception pointer: %p\n", (void*)e);
        // Don't call e->Delete() since our static object shouldn't be deleted
        fflush(stdout);
    }
    catch (CException* e) {
        printf("  PARTIAL: Caught as CException* (base class)\n");
        printf("    Exception pointer: %p\n", (void*)e);
        fflush(stdout);
    }
    catch (...) {
        printf("  FAILED: Caught with ... (type matching failed)\n");
        fflush(stdout);
        return 1;
    }

    printf("\nTest 2: Catch as CException* (base class)\n");
    fflush(stdout);

    try {
        printf("  Calling AfxThrowMemoryException()...\n");
        fflush(stdout);

        AfxThrowMemoryException();

        printf("  ERROR: Should have thrown!\n");
        fflush(stdout);
        return 1;
    }
    catch (CException* e) {
        printf("  SUCCESS: Caught as CException*!\n");
        printf("    Exception pointer: %p\n", (void*)e);
        fflush(stdout);
    }
    catch (...) {
        printf("  FAILED: Caught with ... (type matching failed)\n");
        fflush(stdout);
        return 1;
    }

    printf("\nTest 3: Catch CFileException* (Heap Allocated)\n");
    fflush(stdout);

    try {
        printf("  Calling AfxThrowFileException(CFileException::fileNotFound, -1, L\"test.txt\")...\n");
        fflush(stdout);

        AfxThrowFileException(CFileException::fileNotFound, -1, L"test.txt");

        printf("  ERROR: Should have thrown!\n");
        fflush(stdout);
        return 1;
    }
    catch (CFileException* e) {
        printf("  SUCCESS: Caught CFileException*!\n");
        printf("    Exception pointer: %p\n", (void*)e);
        printf("    Cause: %d (Expected: %d)\n", e->m_cause, CFileException::fileNotFound);
        // In real MFC, we should delete the exception. 
        // Our implementation allocates with 'new', so this is safe and correct.
        e->Delete(); 
        fflush(stdout);
    }
    catch (CException* e) {
        printf("  PARTIAL: Caught as CException* (base class)\n");
        e->Delete();
        fflush(stdout);
    }
    catch (...) {
        printf("  FAILED: Caught with ... (type matching failed)\n");
        fflush(stdout);
        return 1;
    }

    printf("\n=== All Tests Passed ===\n");
    fflush(stdout);
    return 0;
}
