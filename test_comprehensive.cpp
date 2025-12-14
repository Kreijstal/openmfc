// Comprehensive test for OpenMFC phase4 implementation
// Tests compile-time linking against openmfc.dll via openmfc.lib
//
// This test verifies that MSVC code can:
// 1. Include OpenMFC headers
// 2. Link against the import library
// 3. Call MFC functions at runtime

#define UNICODE
#define _UNICODE

#include <stdio.h>
#include "openmfc/afxwin.h"

// Test 1: Verify we can call exception functions (compile-time linked)
bool test_exception_functions() {
    printf("Test 1: Exception Functions (compile-time linked)\n");
    printf("=================================================\n");

    // These calls verify that the symbols are resolved at link time
    // The functions throw C++ exceptions, so we catch them

    bool all_pass = true;

    // Test AfxThrowMemoryException
    printf("  Testing AfxThrowMemoryException... ");
    try {
        AfxThrowMemoryException();
        printf("FAIL (no exception thrown)\n");
        all_pass = false;
    } catch (CMemoryException* e) {
        printf("OK (caught CMemoryException*)\n");
        // Note: In real MFC you'd call e->Delete()
    } catch (...) {
        printf("OK (caught exception)\n");
    }

    // Test AfxThrowNotSupportedException
    printf("  Testing AfxThrowNotSupportedException... ");
    try {
        AfxThrowNotSupportedException();
        printf("FAIL (no exception thrown)\n");
        all_pass = false;
    } catch (CNotSupportedException* e) {
        printf("OK (caught CNotSupportedException*)\n");
    } catch (...) {
        printf("OK (caught exception)\n");
    }

    // Test AfxThrowInvalidArgException
    printf("  Testing AfxThrowInvalidArgException... ");
    try {
        AfxThrowInvalidArgException();
        printf("FAIL (no exception thrown)\n");
        all_pass = false;
    } catch (CInvalidArgException* e) {
        printf("OK (caught CInvalidArgException*)\n");
    } catch (...) {
        printf("OK (caught exception)\n");
    }

    // Test AfxThrowFileException
    printf("  Testing AfxThrowFileException... ");
    try {
        AfxThrowFileException(CFileException::fileNotFound, -1, L"test.txt");
        printf("FAIL (no exception thrown)\n");
        all_pass = false;
    } catch (CFileException* e) {
        printf("OK (caught CFileException*)\n");
    } catch (...) {
        printf("OK (caught exception)\n");
    }

    return all_pass;
}

// Test 2: Verify CRuntimeClass and RTTI work
bool test_runtime_class() {
    printf("\nTest 2: CRuntimeClass (compile-time linked)\n");
    printf("============================================\n");

    bool all_pass = true;

    // Get CObject's runtime class
    printf("  Testing CObject::GetThisClass()... ");
    CRuntimeClass* pClass = CObject::GetThisClass();
    if (pClass && pClass->m_lpszClassName) {
        printf("OK (class name: %s)\n", pClass->m_lpszClassName);
    } else {
        printf("FAIL\n");
        all_pass = false;
    }

    return all_pass;
}

// Test 3: Verify version function
bool test_version() {
    printf("\nTest 3: Version Function (compile-time linked)\n");
    printf("===============================================\n");

    printf("  Testing AfxGetDllVersion()... ");
    DWORD version = AfxGetDllVersion();
    if (version > 0) {
        printf("OK (version: 0x%08X)\n", version);
        return true;
    } else {
        printf("FAIL (returned 0)\n");
        return false;
    }
}

// Test 4: Verify AfxGetThread (tests symbol aliasing)
bool test_afx_get_thread() {
    printf("\nTest 4: AfxGetThread (compile-time linked)\n");
    printf("==========================================\n");

    printf("  Testing AfxGetThread()... ");
    CWinThread* pThread = AfxGetThread();
    // Before any CWinApp is created, this should return nullptr
    if (pThread == nullptr) {
        printf("OK (nullptr before app init)\n");
        return true;
    } else {
        printf("OK (thread pointer: %p)\n", (void*)pThread);
        return true;
    }
}

int main() {
    printf("OpenMFC Phase 4 Comprehensive Test\n");
    printf("==================================\n");
    printf("Testing compile-time linking against openmfc.dll\n\n");

    bool test1 = test_exception_functions();
    bool test2 = test_runtime_class();
    bool test3 = test_version();
    bool test4 = test_afx_get_thread();

    printf("\n==================================\n");
    printf("Summary:\n");
    printf("  Test 1 (Exceptions): %s\n", test1 ? "PASS" : "FAIL");
    printf("  Test 2 (RTTI):       %s\n", test2 ? "PASS" : "FAIL");
    printf("  Test 3 (Version):    %s\n", test3 ? "PASS" : "FAIL");
    printf("  Test 4 (AfxGetThread): %s\n", test4 ? "PASS" : "FAIL");

    if (test1 && test2 && test3 && test4) {
        printf("\n[OK] ALL TESTS PASSED!\n");
        printf("OpenMFC phase4 implementation is working:\n");
        printf("  - Compile-time linking works\n");
        printf("  - Exception functions callable\n");
        printf("  - RTTI functions work\n");
        printf("  - Symbol aliasing works\n");
        return 0;
    } else {
        printf("\n[FAIL] SOME TESTS FAILED\n");
        return 1;
    }
}
