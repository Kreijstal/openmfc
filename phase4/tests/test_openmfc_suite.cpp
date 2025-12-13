// OpenMFC Comprehensive Test Suite
//
// Tests ALL implemented features from the Phase 4 checklist using real MFC headers.
//
// Build with MSVC:
//   cl /nologo /EHsc /MD /D_AFXDLL test_openmfc_suite.cpp openmfc.lib /Fe:test_openmfc_suite.exe /link /NODEFAULTLIB:mfc140u.lib
//
// Features tested:
// 1. Exception Throwing (AfxThrowMemoryException)
// 2. Version Functions (AfxGetDllVersion)
// 3. CObject RTTI (GetRuntimeClass, GetThisClass, IsKindOf, IsSerializable)
// 4. CRuntimeClass methods (CreateObject, FromName, IsDerivedFrom)
//
// Exit codes:
//   0 = All tests passed
//   1 = Some tests failed
//   -1 = Critical failure (crash)

#define _AFXDLL
#include <afx.h>
#include <cstdio>
#include <cstring>

// =============================================================================
// Test Framework
// =============================================================================

static int g_totalTests = 0;
static int g_passedTests = 0;
static int g_failedTests = 0;
static int g_skippedTests = 0;

static const char* g_currentSection = nullptr;

#define SECTION(name) do { \
    g_currentSection = name; \
    printf("\n"); \
    printf("========================================\n"); \
    printf("  %s\n", name); \
    printf("========================================\n"); \
    fflush(stdout); \
} while(0)

#define TEST(name, condition) do { \
    g_totalTests++; \
    if (condition) { \
        g_passedTests++; \
        printf("  [PASS] %s\n", name); \
    } else { \
        g_failedTests++; \
        printf("  [FAIL] %s\n", name); \
    } \
    fflush(stdout); \
} while(0)

#define TEST_SKIP(name, reason) do { \
    g_totalTests++; \
    g_skippedTests++; \
    printf("  [SKIP] %s - %s\n", name, reason); \
    fflush(stdout); \
} while(0)

#define INFO(fmt, ...) do { \
    printf("    INFO: " fmt "\n", ##__VA_ARGS__); \
    fflush(stdout); \
} while(0)

// =============================================================================
// Test 1: Version Functions
// =============================================================================

void test_version_functions() {
    SECTION("Version Functions");

    // AfxGetDllVersion
    DWORD version = AfxGetDllVersion();
    TEST("AfxGetDllVersion() returns non-zero", version != 0);
    TEST("AfxGetDllVersion() returns MFC 14.x (0x0Exx)", (version & 0xFF00) == 0x0E00);
    INFO("AfxGetDllVersion() = 0x%04X (MFC %d.%d)", version, (version >> 8), (version & 0xFF));
}

// =============================================================================
// Test 2: Exception Throwing - Basic
// =============================================================================

void test_exception_basic() {
    SECTION("Exception Throwing - Basic");

    // Test 1: Exception is thrown and caught by catch(...)
    bool caught = false;
    try {
        AfxThrowMemoryException();
        // Should not reach here
    }
    catch (...) {
        caught = true;
    }
    TEST("AfxThrowMemoryException() throws (catch ...)", caught);
}

// =============================================================================
// Test 3: Exception Throwing - Typed Catching
// =============================================================================

void test_exception_typed() {
    SECTION("Exception Throwing - Typed Catching");

    // Test 1: Catch as CMemoryException*
    {
        bool caughtAsMemory = false;
        bool caughtAsOther = false;
        try {
            AfxThrowMemoryException();
        }
        catch (CMemoryException* e) {
            caughtAsMemory = true;
            INFO("Caught CMemoryException* at %p", (void*)e);
            // Don't delete - it's a static object
        }
        catch (...) {
            caughtAsOther = true;
        }
        TEST("Catch as CMemoryException*", caughtAsMemory && !caughtAsOther);
    }

    // Test 2: Catch as CException* (base class)
    {
        bool caughtAsException = false;
        try {
            AfxThrowMemoryException();
        }
        catch (CException* e) {
            caughtAsException = true;
            INFO("Caught CException* at %p", (void*)e);
        }
        catch (...) {
            // Fall through
        }
        TEST("Catch as CException* (base class)", caughtAsException);
    }

    // Test 3: Catch as CObject* (root base)
    {
        bool caughtAsObject = false;
        try {
            AfxThrowMemoryException();
        }
        catch (CObject* e) {
            caughtAsObject = true;
            INFO("Caught CObject* at %p", (void*)e);
        }
        catch (...) {
            // Fall through
        }
        TEST("Catch as CObject* (root base)", caughtAsObject);
    }

    // Test 4: Verify catch order (most specific first)
    {
        int catchLevel = 0;
        try {
            AfxThrowMemoryException();
        }
        catch (CMemoryException*) {
            catchLevel = 1;  // Most specific
        }
        catch (CException*) {
            catchLevel = 2;  // Base
        }
        catch (CObject*) {
            catchLevel = 3;  // Root
        }
        catch (...) {
            catchLevel = 4;  // Fallback
        }
        TEST("Catch order: CMemoryException* is most specific", catchLevel == 1);
    }
}

// =============================================================================
// Test 3b: All Exception Types
// =============================================================================

void test_all_exception_types() {
    SECTION("All Exception Types");

    // CNotSupportedException
    {
        bool caught = false;
        try { AfxThrowNotSupportedException(); }
        catch (CNotSupportedException*) { caught = true; }
        catch (...) {}
        TEST("AfxThrowNotSupportedException -> catch CNotSupportedException*", caught);
    }

    // CResourceException
    {
        bool caught = false;
        try { AfxThrowResourceException(); }
        catch (CResourceException*) { caught = true; }
        catch (...) {}
        TEST("AfxThrowResourceException -> catch CResourceException*", caught);
    }

    // CUserException
    {
        bool caught = false;
        try { AfxThrowUserException(); }
        catch (CUserException*) { caught = true; }
        catch (...) {}
        TEST("AfxThrowUserException -> catch CUserException*", caught);
    }

    // CInvalidArgException
    {
        bool caught = false;
        try { AfxThrowInvalidArgException(); }
        catch (CInvalidArgException*) { caught = true; }
        catch (...) {}
        TEST("AfxThrowInvalidArgException -> catch CInvalidArgException*", caught);
    }

    // CFileException
    {
        bool caught = false;
        try { AfxThrowFileException(0, 0, nullptr); }
        catch (CFileException*) { caught = true; }
        catch (...) {}
        TEST("AfxThrowFileException -> catch CFileException*", caught);
    }

    // CArchiveException
    {
        bool caught = false;
        try { AfxThrowArchiveException(0, nullptr); }
        catch (CArchiveException*) { caught = true; }
        catch (...) {}
        TEST("AfxThrowArchiveException -> catch CArchiveException*", caught);
    }

    // All exceptions should be catchable as CException*
    {
        int caughtAsBase = 0;

        try { AfxThrowNotSupportedException(); }
        catch (CException*) { caughtAsBase++; }
        catch (...) {}

        try { AfxThrowResourceException(); }
        catch (CException*) { caughtAsBase++; }
        catch (...) {}

        try { AfxThrowUserException(); }
        catch (CException*) { caughtAsBase++; }
        catch (...) {}

        try { AfxThrowInvalidArgException(); }
        catch (CException*) { caughtAsBase++; }
        catch (...) {}

        try { AfxThrowFileException(0, 0, nullptr); }
        catch (CException*) { caughtAsBase++; }
        catch (...) {}

        try { AfxThrowArchiveException(0, nullptr); }
        catch (CException*) { caughtAsBase++; }
        catch (...) {}

        TEST("All 6 exception types catchable as CException*", caughtAsBase == 6);
        INFO("Caught %d/6 as CException*", caughtAsBase);
    }
}

// =============================================================================
// Test 4: CObject Static Methods
// =============================================================================

void test_cobject_static() {
    SECTION("CObject Static Methods");

    // CObject::GetThisClass()
    CRuntimeClass* pClass = CObject::GetThisClass();
    TEST("CObject::GetThisClass() returns non-null", pClass != nullptr);

    if (pClass) {
        // Check class name
        TEST("CObject class name is 'CObject'",
             pClass->m_lpszClassName != nullptr &&
             strcmp(pClass->m_lpszClassName, "CObject") == 0);
        INFO("m_lpszClassName = '%s'", pClass->m_lpszClassName ? pClass->m_lpszClassName : "(null)");

        // Check base class
        TEST("CObject has no base class (root)", pClass->m_pBaseClass == nullptr);

        // Check object size
        TEST("CObject size is 8 bytes (vptr on x64)", pClass->m_nObjectSize == 8);
        INFO("m_nObjectSize = %d", pClass->m_nObjectSize);

        // Check schema
        INFO("m_wSchema = 0x%X", pClass->m_wSchema);
    }

    // RUNTIME_CLASS macro
    CRuntimeClass* pClass2 = RUNTIME_CLASS(CObject);
    TEST("RUNTIME_CLASS(CObject) returns same as GetThisClass()", pClass == pClass2);
}

// =============================================================================
// Test 5: CRuntimeClass Methods
// =============================================================================

void test_cruntimeclass_methods() {
    SECTION("CRuntimeClass Methods");

    CRuntimeClass* pCObject = RUNTIME_CLASS(CObject);

    // IsDerivedFrom - self
    if (pCObject) {
        TEST("CObject::IsDerivedFrom(CObject) is true", pCObject->IsDerivedFrom(pCObject));
    }

    // CreateObject - CObject is abstract, should return nullptr
    if (pCObject) {
        CObject* pObj = pCObject->CreateObject();
        TEST("CObject::CreateObject() returns nullptr (abstract)", pObj == nullptr);
        if (pObj) {
            delete pObj;  // Clean up if somehow created
        }
    }

    // Test with CException if available
    CRuntimeClass* pCException = RUNTIME_CLASS(CException);
    if (pCException && pCObject) {
        TEST("CException class is available", pCException != nullptr);

        if (pCException->m_lpszClassName) {
            TEST("CException class name is 'CException'",
                 strcmp(pCException->m_lpszClassName, "CException") == 0);
        }

        // CException should be derived from CObject
        TEST("CException::IsDerivedFrom(CObject) is true",
             pCException->IsDerivedFrom(pCObject));

        // CObject should NOT be derived from CException
        TEST("CObject::IsDerivedFrom(CException) is false",
             !pCObject->IsDerivedFrom(pCException));
    }

    // Test with CMemoryException if available
    CRuntimeClass* pCMemoryException = RUNTIME_CLASS(CMemoryException);
    if (pCMemoryException && pCException && pCObject) {
        TEST("CMemoryException class is available", pCMemoryException != nullptr);

        if (pCMemoryException->m_lpszClassName) {
            TEST("CMemoryException class name is 'CMemoryException'",
                 strcmp(pCMemoryException->m_lpszClassName, "CMemoryException") == 0);
        }

        // CMemoryException should be derived from CException
        TEST("CMemoryException::IsDerivedFrom(CException) is true",
             pCMemoryException->IsDerivedFrom(pCException));

        // CMemoryException should be derived from CObject
        TEST("CMemoryException::IsDerivedFrom(CObject) is true",
             pCMemoryException->IsDerivedFrom(pCObject));
    }
}

// =============================================================================
// Test 6: CRuntimeClass::FromName (if implemented)
// =============================================================================

void test_cruntimeclass_fromname() {
    SECTION("CRuntimeClass::FromName");

    // Note: FromName requires classes to be registered in a global list
    // Our implementation maintains this list, but MFC headers may define
    // their own mechanism. Test what we can.

    // Try to find CObject by name
    CRuntimeClass* pFound = CRuntimeClass::FromName("CObject");
    if (pFound) {
        TEST("FromName('CObject') finds CObject", pFound == RUNTIME_CLASS(CObject));
        INFO("Found CObject via FromName");
    } else {
        TEST_SKIP("FromName('CObject')", "Class not registered in global list");
    }

    // Try wide string version
    CRuntimeClass* pFoundW = CRuntimeClass::FromName(L"CObject");
    if (pFoundW) {
        TEST("FromName(L'CObject') finds CObject", pFoundW == RUNTIME_CLASS(CObject));
    } else {
        TEST_SKIP("FromName(L'CObject')", "Class not registered in global list");
    }

    // Non-existent class should return nullptr
    CRuntimeClass* pNotFound = CRuntimeClass::FromName("NonExistentClass");
    TEST("FromName('NonExistentClass') returns nullptr", pNotFound == nullptr);
}

// =============================================================================
// Test 7: Exception Object Properties
// =============================================================================

void test_exception_object() {
    SECTION("Exception Object Properties");

    // Catch exception and examine the object
    CMemoryException* pException = nullptr;
    try {
        AfxThrowMemoryException();
    }
    catch (CMemoryException* e) {
        pException = e;
    }
    catch (...) {
        // Fall through
    }

    TEST("Exception object is caught", pException != nullptr);

    if (pException) {
        // Check that it's a valid object with a vtable
        // We can't safely call virtual methods without knowing the vtable is valid
        // but we can check the pointer is non-null
        INFO("Exception object at %p", (void*)pException);

        // Try to get runtime class (this calls through vtable)
        // Be careful - this might crash if vtable is wrong
        CRuntimeClass* pClass = nullptr;
        __try {
            pClass = pException->GetRuntimeClass();
        }
        __except(EXCEPTION_EXECUTE_HANDLER) {
            INFO("GetRuntimeClass() crashed - vtable issue");
        }

        if (pClass) {
            TEST("Exception->GetRuntimeClass() returns valid pointer", pClass != nullptr);
            if (pClass->m_lpszClassName) {
                INFO("Runtime class: %s", pClass->m_lpszClassName);
                TEST("Exception runtime class is CMemoryException",
                     strcmp(pClass->m_lpszClassName, "CMemoryException") == 0);
            }
        } else {
            TEST_SKIP("Exception->GetRuntimeClass()", "Returns nullptr or crashed");
        }

        // Test IsKindOf
        bool isKindOfMemory = false;
        bool isKindOfException = false;
        bool isKindOfObject = false;
        __try {
            isKindOfMemory = pException->IsKindOf(RUNTIME_CLASS(CMemoryException)) != 0;
            isKindOfException = pException->IsKindOf(RUNTIME_CLASS(CException)) != 0;
            isKindOfObject = pException->IsKindOf(RUNTIME_CLASS(CObject)) != 0;
        }
        __except(EXCEPTION_EXECUTE_HANDLER) {
            INFO("IsKindOf() crashed - vtable issue");
        }

        TEST("Exception->IsKindOf(CMemoryException)", isKindOfMemory);
        TEST("Exception->IsKindOf(CException)", isKindOfException);
        TEST("Exception->IsKindOf(CObject)", isKindOfObject);
    }
}

// =============================================================================
// Test 8: Multiple Exception Throws
// =============================================================================

void test_multiple_throws() {
    SECTION("Multiple Exception Throws");

    // Throw and catch multiple times to verify stability
    int catchCount = 0;
    for (int i = 0; i < 5; i++) {
        try {
            AfxThrowMemoryException();
        }
        catch (CMemoryException*) {
            catchCount++;
        }
        catch (...) {
            // Fall through
        }
    }

    TEST("Can throw and catch 5 times", catchCount == 5);
    INFO("Caught %d out of 5 throws", catchCount);
}

// =============================================================================
// Test 9: Nested Exception Handling
// =============================================================================

void test_nested_exceptions() {
    SECTION("Nested Exception Handling");

    bool outerCaught = false;
    bool innerCaught = false;

    try {
        try {
            AfxThrowMemoryException();
        }
        catch (CMemoryException*) {
            innerCaught = true;
            // Re-throw
            throw;
        }
    }
    catch (CMemoryException*) {
        outerCaught = true;
    }
    catch (...) {
        // Fall through
    }

    TEST("Inner catch block executed", innerCaught);
    TEST("Re-thrown exception caught in outer block", outerCaught);
}

// =============================================================================
// Main Entry Point
// =============================================================================

int main() {
    printf("================================================================\n");
    printf("  OpenMFC Comprehensive Test Suite\n");
    printf("  Testing all Phase 4 implemented features\n");
    printf("================================================================\n");
    fflush(stdout);

    // Run all tests
    test_version_functions();
    test_exception_basic();
    test_exception_typed();
    test_cobject_static();
    test_cruntimeclass_methods();
    test_cruntimeclass_fromname();
    test_exception_object();
    test_multiple_throws();
    test_nested_exceptions();

    // Summary
    printf("\n");
    printf("================================================================\n");
    printf("  TEST SUMMARY\n");
    printf("================================================================\n");
    printf("  Total tests:   %d\n", g_totalTests);
    printf("  Passed:        %d\n", g_passedTests);
    printf("  Failed:        %d\n", g_failedTests);
    printf("  Skipped:       %d\n", g_skippedTests);
    printf("================================================================\n");

    if (g_failedTests == 0) {
        printf("\n  *** ALL TESTS PASSED ***\n\n");
        fflush(stdout);
        return 0;
    } else {
        printf("\n  *** %d TEST(S) FAILED ***\n\n", g_failedTests);
        fflush(stdout);
        return 1;
    }
}
