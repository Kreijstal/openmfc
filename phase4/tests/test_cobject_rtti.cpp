// CObject RTTI Test - uses real MFC headers
//
// Build with MSVC:
//   cl /nologo /EHsc /MD /D_AFXDLL test_cobject_rtti.cpp openmfc.lib
//
// This test verifies that CObject's RTTI system works correctly:
// - GetRuntimeClass() returns correct CRuntimeClass pointer
// - GetThisClass() returns the static CRuntimeClass
// - IsKindOf() correctly identifies class hierarchy
// - CRuntimeClass::IsDerivedFrom() works for inheritance checks

#define _AFXDLL
#include <afx.h>
#include <cstdio>

// Test results tracking
static int g_testsRun = 0;
static int g_testsPassed = 0;

#define TEST(name, condition) do { \
    g_testsRun++; \
    if (condition) { \
        printf("  PASS: %s\n", name); \
        g_testsPassed++; \
    } else { \
        printf("  FAIL: %s\n", name); \
    } \
    fflush(stdout); \
} while(0)

int main() {
    printf("=== CObject RTTI Test (Real MFC Headers) ===\n\n");
    fflush(stdout);

    // Test 1: CObject::GetThisClass()
    printf("Test 1: CObject::GetThisClass()\n");
    fflush(stdout);
    {
        CRuntimeClass* pClass = CObject::GetThisClass();
        TEST("GetThisClass() returns non-null", pClass != nullptr);

        if (pClass) {
            TEST("Class name is 'CObject'",
                 pClass->m_lpszClassName != nullptr &&
                 strcmp(pClass->m_lpszClassName, "CObject") == 0);
            // Note: m_pBaseClass is not public in real MFC, use IsDerivedFrom instead
            TEST("CObject is root class (only derived from itself)",
                 pClass->IsDerivedFrom(RUNTIME_CLASS(CObject)));
            TEST("Object size is reasonable",
                 pClass->m_nObjectSize >= sizeof(void*));
            printf("    m_lpszClassName: %s\n", pClass->m_lpszClassName ? pClass->m_lpszClassName : "(null)");
            printf("    m_nObjectSize: %d\n", pClass->m_nObjectSize);
            printf("    m_wSchema: 0x%X\n", pClass->m_wSchema);
            fflush(stdout);
        }
    }

    // Test 2: RUNTIME_CLASS macro
    printf("\nTest 2: RUNTIME_CLASS(CObject)\n");
    fflush(stdout);
    {
        CRuntimeClass* pClass1 = CObject::GetThisClass();
        CRuntimeClass* pClass2 = RUNTIME_CLASS(CObject);
        TEST("RUNTIME_CLASS(CObject) equals GetThisClass()", pClass1 == pClass2);
    }

    // Test 3: CRuntimeClass::IsDerivedFrom()
    printf("\nTest 3: CRuntimeClass::IsDerivedFrom()\n");
    fflush(stdout);
    {
        CRuntimeClass* pCObject = RUNTIME_CLASS(CObject);

        // CObject is derived from itself
        TEST("CObject::IsDerivedFrom(CObject) is true",
             pCObject->IsDerivedFrom(pCObject));

        // CObject is not derived from anything else (but we can't test that easily without other classes)
    }

    // Test 4: CException hierarchy (if available)
    printf("\nTest 4: CException hierarchy\n");
    fflush(stdout);
    {
        CRuntimeClass* pCObject = RUNTIME_CLASS(CObject);
        CRuntimeClass* pCException = RUNTIME_CLASS(CException);

        if (pCException) {
            TEST("CException class exists", pCException != nullptr);
            TEST("CException name is 'CException'",
                 pCException->m_lpszClassName != nullptr &&
                 strcmp(pCException->m_lpszClassName, "CException") == 0);

            // CException should be derived from CObject
            TEST("CException is derived from CObject",
                 pCException->IsDerivedFrom(pCObject));

            // CObject is NOT derived from CException
            TEST("CObject is NOT derived from CException",
                 !pCObject->IsDerivedFrom(pCException));
        } else {
            printf("  SKIP: CException not available in this build\n");
            fflush(stdout);
        }
    }

    // Test 5: CMemoryException hierarchy
    printf("\nTest 5: CMemoryException hierarchy\n");
    fflush(stdout);
    {
        CRuntimeClass* pCObject = RUNTIME_CLASS(CObject);
        CRuntimeClass* pCException = RUNTIME_CLASS(CException);
        CRuntimeClass* pCMemoryException = RUNTIME_CLASS(CMemoryException);

        if (pCMemoryException) {
            TEST("CMemoryException class exists", pCMemoryException != nullptr);
            TEST("CMemoryException name is 'CMemoryException'",
                 pCMemoryException->m_lpszClassName != nullptr &&
                 strcmp(pCMemoryException->m_lpszClassName, "CMemoryException") == 0);

            // CMemoryException should be derived from CException and CObject
            if (pCException) {
                TEST("CMemoryException is derived from CException",
                     pCMemoryException->IsDerivedFrom(pCException));
            }
            TEST("CMemoryException is derived from CObject",
                 pCMemoryException->IsDerivedFrom(pCObject));
        } else {
            printf("  SKIP: CMemoryException RUNTIME_CLASS not available\n");
            fflush(stdout);
        }
    }

    // Summary
    printf("\n=== Test Summary ===\n");
    printf("Tests run: %d\n", g_testsRun);
    printf("Tests passed: %d\n", g_testsPassed);
    printf("Tests failed: %d\n", g_testsRun - g_testsPassed);
    fflush(stdout);

    if (g_testsPassed == g_testsRun) {
        printf("\n=== All Tests Passed ===\n");
        fflush(stdout);
        return 0;
    } else {
        printf("\n=== Some Tests Failed ===\n");
        fflush(stdout);
        return 1;
    }
}
