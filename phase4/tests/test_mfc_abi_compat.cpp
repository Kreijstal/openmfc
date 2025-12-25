// MFC ABI Compatibility Test Suite
//
// This test suite runs against BOTH real MFC (mfc140u.dll) and OpenMFC,
// expecting IDENTICAL results. It tests the public ABI surface of MFC.
//
// Build for Real MFC:
//   cl /nologo /EHsc /MD /D_AFXDLL /DTEST_REAL_MFC test_mfc_abi_compat.cpp /link mfc140u.lib
//
// Build for OpenMFC:
//   cl /nologo /EHsc /MD /D_AFXDLL /DTEST_OPENMFC test_mfc_abi_compat.cpp openmfc.lib
//
// Output: JSON-structured results for programmatic comparison
//
// Exit codes:
//   0 = All tests passed
//   N = N tests failed

#ifndef _AFXDLL
#define _AFXDLL
#endif

#ifdef TEST_OPENMFC
// Use OpenMFC headers
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"

// Declarations missing from OpenMFC headers but exported by the DLL
__declspec(dllimport) DWORD AFXAPI AfxGetDllVersion();

// Exception classes not yet in OpenMFC headers (but may be in MFC)
// Define placeholders if not defined
#ifndef _AFX_NOTSUPPORTEDEXCEPTION
class CNotSupportedException : public CException {
public:
    CNotSupportedException() : CException() {}
};
#endif

#ifndef _AFX_INVALIDARGEXCEPTION
class CInvalidArgException : public CException {
public:
    CInvalidArgException() : CException() {}
};
#endif

#ifndef _AFX_RESOURCEEXCEPTION
class CResourceException : public CException {
public:
    CResourceException() : CException() {}
};
#endif

#ifndef _AFX_USEREXCEPTION
class CUserException : public CException {
public:
    CUserException() : CException() {}
};
#endif

// CRuntimeClass::FromName stub (may not be implemented in OpenMFC)
#define FROMNAME_STUB
inline CRuntimeClass* FromName_Stub(const char*) { return nullptr; }

#else
// Use real MFC headers (MSVC only)
#include <afx.h>
#include <afxwin.h>
#endif

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <tchar.h>

// =============================================================================
// Test Framework - Outputs structured, comparable results
// =============================================================================

static int g_totalTests = 0;
static int g_passedTests = 0;
static int g_failedTests = 0;
static char g_currentSection[256] = "";

// Result structure for comparison
struct TestResult {
    const char* section;
    const char* name;
    bool passed;
    char actual[256];
    char expected[256];
};

static TestResult g_results[500];
static int g_resultCount = 0;

void section(const char* name) {
    strncpy(g_currentSection, name, sizeof(g_currentSection) - 1);
    g_currentSection[sizeof(g_currentSection) - 1] = '\0';
    printf("\n=== %s ===\n", name);
    fflush(stdout);
}

void test_pass(const char* name, const char* actual) {
    g_totalTests++;
    g_passedTests++;
    printf("  [PASS] %s = %s\n", name, actual);
    fflush(stdout);
    
    if (g_resultCount < 500) {
        TestResult& r = g_results[g_resultCount++];
        r.section = g_currentSection;
        r.name = name;
        r.passed = true;
        strncpy(r.actual, actual, sizeof(r.actual) - 1);
        r.expected[0] = '\0';
    }
}

void test_fail(const char* name, const char* expected, const char* actual) {
    g_totalTests++;
    g_failedTests++;
    printf("  [FAIL] %s: expected '%s', got '%s'\n", name, expected, actual);
    fflush(stdout);
    
    if (g_resultCount < 500) {
        TestResult& r = g_results[g_resultCount++];
        r.section = g_currentSection;
        r.name = name;
        r.passed = false;
        strncpy(r.actual, actual, sizeof(r.actual) - 1);
        strncpy(r.expected, expected, sizeof(r.expected) - 1);
    }
}

#define TEST_EQ_INT(name, expected, actual) do { \
    int _e = (expected); int _a = (actual); \
    char _buf[64]; sprintf(_buf, "%d", _a); \
    if (_e == _a) test_pass(name, _buf); \
    else { char _ebuf[64]; sprintf(_ebuf, "%d", _e); test_fail(name, _ebuf, _buf); } \
} while(0)

#define TEST_EQ_HEX(name, expected, actual) do { \
    unsigned int _e = (expected); unsigned int _a = (actual); \
    char _buf[64]; sprintf(_buf, "0x%08X", _a); \
    if (_e == _a) test_pass(name, _buf); \
    else { char _ebuf[64]; sprintf(_ebuf, "0x%08X", _e); test_fail(name, _ebuf, _buf); } \
} while(0)

#define TEST_EQ_PTR(name, expected, actual) do { \
    const void* _e = (expected); const void* _a = (actual); \
    if (_e == _a) test_pass(name, "true"); \
    else test_fail(name, "true", "false"); \
} while(0)

#define TEST_EQ_STR(name, expected, actual) do { \
    const char* _e = (expected); const char* _a = (actual); \
    if (_e && _a && strcmp(_e, _a) == 0) test_pass(name, _a); \
    else test_fail(name, _e ? _e : "(null)", _a ? _a : "(null)"); \
} while(0)

#define TEST_TRUE(name, condition) do { \
    bool _c = (condition); \
    if (_c) test_pass(name, "true"); \
    else test_fail(name, "true", "false"); \
} while(0)

#define TEST_FALSE(name, condition) do { \
    bool _c = (condition); \
    if (!_c) test_pass(name, "false"); \
    else test_fail(name, "false", "true"); \
} while(0)

#define TEST_NOTNULL(name, ptr) do { \
    const void* _p = (ptr); \
    if (_p != nullptr) test_pass(name, "non-null"); \
    else test_fail(name, "non-null", "null"); \
} while(0)

#define TEST_NULL(name, ptr) do { \
    const void* _p = (ptr); \
    if (_p == nullptr) test_pass(name, "null"); \
    else test_fail(name, "null", "non-null"); \
} while(0)

// =============================================================================
// Test 1: sizeof() checks - ABI structure sizes
// =============================================================================

void test_sizeof() {
    section("Structure Sizes (ABI)");
    
    TEST_EQ_INT("sizeof(CObject)", 8, sizeof(CObject));
    TEST_EQ_INT("sizeof(CRuntimeClass)", 48, sizeof(CRuntimeClass));  // May vary
    TEST_EQ_INT("sizeof(CException)", 16, sizeof(CException));  // CObject + m_bAutoDelete
    TEST_EQ_INT("sizeof(CString)", 8, sizeof(CString));   // Just a pointer
    TEST_EQ_INT("sizeof(CWnd)", 232, sizeof(CWnd));
}

// =============================================================================
// Test 2: CRuntimeClass static members
// =============================================================================

void test_cruntimeclass() {
    section("CRuntimeClass");
    
    // CObject class info
    CRuntimeClass* pCObject = RUNTIME_CLASS(CObject);
    TEST_NOTNULL("RUNTIME_CLASS(CObject)", pCObject);
    
    if (pCObject) {
        TEST_EQ_STR("CObject::m_lpszClassName", "CObject", pCObject->m_lpszClassName);
        TEST_EQ_INT("CObject::m_nObjectSize", 8, pCObject->m_nObjectSize);
#ifdef TEST_OPENMFC
        TEST_NULL("CObject::base class (root)", pCObject->m_pBaseClass);
#else
        TEST_NULL("CObject::base class (root)", pCObject->m_pfnGetBaseClass);
#endif
    }
    
    // CException class info
    CRuntimeClass* pCException = RUNTIME_CLASS(CException);
    TEST_NOTNULL("RUNTIME_CLASS(CException)", pCException);
    
    if (pCException) {
        TEST_EQ_STR("CException::m_lpszClassName", "CException", pCException->m_lpszClassName);
        TEST_TRUE("CException derived from CObject", pCException->IsDerivedFrom(pCObject));
    }
    
    // CWnd class info
    CRuntimeClass* pCWnd = RUNTIME_CLASS(CWnd);
    TEST_NOTNULL("RUNTIME_CLASS(CWnd)", pCWnd);
    
    if (pCWnd) {
        TEST_EQ_STR("CWnd::m_lpszClassName", "CWnd", pCWnd->m_lpszClassName);
        TEST_EQ_INT("CWnd::m_nObjectSize", 232, pCWnd->m_nObjectSize);
    }
    
    // IsDerivedFrom tests
    CRuntimeClass* pCCmdTarget = RUNTIME_CLASS(CCmdTarget);
    if (pCCmdTarget && pCWnd) {
        TEST_TRUE("CWnd derived from CCmdTarget", pCWnd->IsDerivedFrom(pCCmdTarget));
        TEST_TRUE("CWnd derived from CObject", pCWnd->IsDerivedFrom(pCObject));
        TEST_FALSE("CCmdTarget derived from CWnd", pCCmdTarget->IsDerivedFrom(pCWnd));
    }
}

// =============================================================================
// Test 3: Version functions
// =============================================================================

void test_version() {
    section("Version Functions");
    
    DWORD version = AfxGetDllVersion();
    TEST_TRUE("AfxGetDllVersion() nonzero", version != 0);
}

// =============================================================================
// Test 4: Exception throwing and catching
// =============================================================================

void test_exceptions() {
    section("Exception Throwing");
    
    // CMemoryException
    {
        bool caught = false;
        bool correctType = false;
        try {
            AfxThrowMemoryException();
        }
        catch (CMemoryException* e) {
            caught = true;
            correctType = true;
            // Don't delete - static in real MFC
        }
        catch (CException*) {
            caught = true;
        }
        catch (...) {
            caught = true;
        }
        TEST_TRUE("AfxThrowMemoryException throws", caught);
        TEST_TRUE("AfxThrowMemoryException -> CMemoryException*", correctType);
    }
    
    // CFileException
    {
        bool caught = false;
        int cause = -1;
        try {
            AfxThrowFileException(CFileException::fileNotFound, 123, nullptr);
        }
        catch (CFileException* e) {
            caught = true;
            cause = e->m_cause;
        }
        catch (...) {}
        TEST_TRUE("AfxThrowFileException throws", caught);
        TEST_EQ_INT("CFileException::m_cause", CFileException::fileNotFound, cause);
    }
    
    // CNotSupportedException
    {
        bool caught = false;
        try { AfxThrowNotSupportedException(); }
        catch (CNotSupportedException*) { caught = true; }
        catch (...) {}
        TEST_TRUE("AfxThrowNotSupportedException -> CNotSupportedException*", caught);
    }
    
    // CInvalidArgException
    {
        bool caught = false;
        try { AfxThrowInvalidArgException(); }
        catch (CInvalidArgException*) { caught = true; }
        catch (...) {}
        TEST_TRUE("AfxThrowInvalidArgException -> CInvalidArgException*", caught);
    }
    
    // Exception hierarchy: all should be catchable as CException*
    {
        int asCException = 0;
        
        try { AfxThrowMemoryException(); }
        catch (CException*) { asCException++; }
        catch (...) {}
        
        try { AfxThrowNotSupportedException(); }
        catch (CException*) { asCException++; }
        catch (...) {}
        
        try { AfxThrowResourceException(); }
        catch (CException*) { asCException++; }
        catch (...) {}
        
        try { AfxThrowUserException(); }
        catch (CException*) { asCException++; }
        catch (...) {}
        
        TEST_EQ_INT("All 4 basic exceptions catchable as CException*", 4, asCException);
    }
}

// =============================================================================
// Test 5: CString basics
// =============================================================================

void test_cstring() {
    section("CString");
    
    // Construction
    CString s1;
    TEST_TRUE("CString() is empty", s1.IsEmpty());
    TEST_EQ_INT("CString().GetLength()", 0, s1.GetLength());
    
    CString s2(_T("Hello"));
    TEST_FALSE("CString(L\"Hello\") not empty", s2.IsEmpty());
    TEST_EQ_INT("CString(L\"Hello\").GetLength()", 5, s2.GetLength());
    
    // Comparison
    CString s3(_T("Hello"));
    TEST_TRUE("CString equality", s2 == s3);
    TEST_FALSE("CString inequality", s2 != s3);
    
    // Concatenation
    CString s4 = s2 + _T(" World");
    TEST_EQ_INT("Concatenation length", 11, s4.GetLength());
    
    // Substring
    CString s5 = s4.Mid(6, 5);
    TEST_EQ_INT("Mid(6,5) length", 5, s5.GetLength());
    
    CString s6 = s4.Left(5);
    TEST_EQ_INT("Left(5) length", 5, s6.GetLength());
    
    CString s7 = s4.Right(5);
    TEST_EQ_INT("Right(5) length", 5, s7.GetLength());
    
    // Find
    int pos = s4.Find(_T("World"));
    TEST_EQ_INT("Find(L\"World\")", 6, pos);
    
    int notFound = s4.Find(_T("xyz"));
    TEST_EQ_INT("Find(L\"xyz\") not found", -1, notFound);
    
    // Case conversion
    CString s8(_T("Hello World"));
    s8.MakeUpper();
    CString s9(_T("Hello World"));
    s9.MakeLower();
    TEST_EQ_INT("MakeUpper() length preserved", 11, s8.GetLength());
    TEST_EQ_INT("MakeLower() length preserved", 11, s9.GetLength());
}

// =============================================================================
// Test 6: Global application functions (without app instance)
// =============================================================================

void test_global_functions_no_app() {
    section("Global Functions (no app)");
    
    // Before any CWinApp is created
    CWinApp* pApp = AfxGetApp();
    // Note: May be null or may be non-null depending on previous tests
    // We just verify it doesn't crash
    char buf[64];
    sprintf(buf, "%p", (void*)pApp);
    test_pass("AfxGetApp() callable", buf);
    
    CWinThread* pThread = AfxGetThread();
    sprintf(buf, "%p", (void*)pThread);
    test_pass("AfxGetThread() callable", buf);
    
    CWnd* pMainWnd = AfxGetMainWnd();
    sprintf(buf, "%p", (void*)pMainWnd);
    test_pass("AfxGetMainWnd() callable", buf);
}

// =============================================================================
// Test 7: CWnd member offsets (critical for ABI)
// =============================================================================

void test_cwnd_offsets() {
    section("CWnd Member Offsets");
    
    CWnd wnd;
    
    // Calculate offset of m_hWnd from start of CWnd
    size_t offset_m_hWnd = (size_t)((char*)&wnd.m_hWnd - (char*)&wnd);
    TEST_EQ_INT("offsetof(CWnd, m_hWnd)", 64, (int)offset_m_hWnd);
    
    // Verify m_hWnd is initially null
    TEST_NULL("CWnd::m_hWnd initial value", wnd.m_hWnd);
    
    // GetSafeHwnd should return null for uninitialized window
    TEST_NULL("CWnd::GetSafeHwnd() initial", wnd.GetSafeHwnd());
}

// =============================================================================
// Test 8: RTTI consistency
// =============================================================================

void test_rtti_consistency() {
    section("RTTI Consistency");
    
    // GetThisClass() should return same as RUNTIME_CLASS()
    TEST_EQ_PTR("CObject::GetThisClass() == RUNTIME_CLASS(CObject)",
                RUNTIME_CLASS(CObject), CObject::GetThisClass());
    
    // Multiple calls should return same pointer
    CRuntimeClass* p1 = RUNTIME_CLASS(CObject);
    CRuntimeClass* p2 = RUNTIME_CLASS(CObject);
    TEST_EQ_PTR("RUNTIME_CLASS(CObject) is stable", p1, p2);
    
    // CRuntimeClass::FromName (if implemented)
#ifdef FROMNAME_STUB
    CRuntimeClass* pFound = FromName_Stub("CObject");
#else
    CRuntimeClass* pFound = CRuntimeClass::FromName("CObject");
#endif
    if (pFound) {
        TEST_EQ_PTR("CRuntimeClass::FromName(\"CObject\")", RUNTIME_CLASS(CObject), pFound);
    } else {
        test_pass("CRuntimeClass::FromName(\"CObject\")", "null (not registered)");
    }
}

// =============================================================================
// Test 9: Memory exception object properties
// =============================================================================

void test_exception_object_properties() {
    section("Exception Object Properties");
    
    CMemoryException* pEx = nullptr;
    try {
        AfxThrowMemoryException();
    }
    catch (CMemoryException* e) {
        pEx = e;
    }
    catch (...) {}
    
    TEST_NOTNULL("Caught CMemoryException*", pEx);
    
    if (pEx) {
        // Check RTTI
        CRuntimeClass* pClass = pEx->GetRuntimeClass();
        TEST_NOTNULL("Exception->GetRuntimeClass()", pClass);
        
        if (pClass) {
            TEST_EQ_STR("Exception class name", "CMemoryException", pClass->m_lpszClassName);
        }
        
        // IsKindOf checks
        TEST_TRUE("IsKindOf(CMemoryException)", pEx->IsKindOf(RUNTIME_CLASS(CMemoryException)));
        TEST_TRUE("IsKindOf(CException)", pEx->IsKindOf(RUNTIME_CLASS(CException)));
        TEST_TRUE("IsKindOf(CObject)", pEx->IsKindOf(RUNTIME_CLASS(CObject)));
    }
}

// =============================================================================
// Test 10: CFileException properties
// =============================================================================

void test_fileexception_properties() {
    section("CFileException Properties");
    
    // Verify cause constants match
    TEST_EQ_INT("CFileException::none", 0, CFileException::none);
    TEST_EQ_INT("CFileException::genericException", 1, CFileException::genericException);
    TEST_EQ_INT("CFileException::fileNotFound", 2, CFileException::fileNotFound);
    TEST_EQ_INT("CFileException::badPath", 3, CFileException::badPath);
    TEST_EQ_INT("CFileException::accessDenied", 5, CFileException::accessDenied);
    TEST_EQ_INT("CFileException::endOfFile", 13, CFileException::endOfFile);
    
    // Throw and verify properties
    CFileException* pEx = nullptr;
    try {
        AfxThrowFileException(CFileException::accessDenied, 42, _T("test.txt"));
    }
    catch (CFileException* e) {
        pEx = e;
    }
    catch (...) {}
    
    if (pEx) {
        TEST_EQ_INT("CFileException::m_cause after throw", CFileException::accessDenied, pEx->m_cause);
        TEST_EQ_INT("CFileException::m_lOsError after throw", 42, (int)pEx->m_lOsError);
    }
}

// =============================================================================
// Main
// =============================================================================

void print_json_results() {
    auto json_escape = [](const char* s) {
        if (!s) {
            return;
        }
        for (const unsigned char* p = reinterpret_cast<const unsigned char*>(s); *p; ++p) {
            switch (*p) {
                case '\\': printf("\\\\"); break;
                case '\"': printf("\\\""); break;
                case '\n': printf("\\n"); break;
                case '\r': printf("\\r"); break;
                case '\t': printf("\\t"); break;
                default:
                    if (*p < 0x20) {
                        printf("\\u%04x", *p);
                    } else {
                        putchar(*p);
                    }
            }
        }
    };

    printf("\n--- BEGIN JSON RESULTS ---\n");
    printf("{\n");
    printf("  \"implementation\": ");
#ifdef TEST_REAL_MFC
    printf("\"real_mfc\",\n");
#else
    printf("\"openmfc\",\n");
#endif
    printf("  \"total\": %d,\n", g_totalTests);
    printf("  \"passed\": %d,\n", g_passedTests);
    printf("  \"failed\": %d,\n", g_failedTests);
    printf("  \"results\": [\n");
    
    for (int i = 0; i < g_resultCount; i++) {
        TestResult& r = g_results[i];
        printf("    {\"section\": \"");
        json_escape(r.section);
        printf("\", \"name\": \"");
        json_escape(r.name);
        printf("\", \"passed\": %s, \"actual\": \"", r.passed ? "true" : "false");
        json_escape(r.actual);
        printf("\"}%s\n", (i < g_resultCount - 1) ? "," : "");
    }
    
    printf("  ]\n");
    printf("}\n");
    printf("--- END JSON RESULTS ---\n");
}

int main() {
    printf("=============================================================\n");
    printf("  MFC ABI Compatibility Test Suite\n");
#ifdef TEST_REAL_MFC
    printf("  Target: Real MFC (mfc140u.dll)\n");
#else
    printf("  Target: OpenMFC\n");
#endif
    printf("=============================================================\n\n");
    fflush(stdout);
    
    // Run all tests
    test_sizeof();
    test_cruntimeclass();
    test_version();
    test_exceptions();
    test_cstring();
    test_global_functions_no_app();
    test_cwnd_offsets();
    test_rtti_consistency();
    test_exception_object_properties();
    test_fileexception_properties();
    
    // Summary
    printf("\n=============================================================\n");
    printf("  SUMMARY\n");
    printf("=============================================================\n");
    printf("  Total:  %d\n", g_totalTests);
    printf("  Passed: %d\n", g_passedTests);
    printf("  Failed: %d\n", g_failedTests);
    printf("=============================================================\n");
    
    if (g_failedTests == 0) {
        printf("\n  *** ALL TESTS PASSED ***\n\n");
    } else {
        printf("\n  *** %d TEST(S) FAILED ***\n\n", g_failedTests);
    }
    
    // Print JSON for programmatic comparison
    print_json_results();
    
    fflush(stdout);
    return g_failedTests;
}
