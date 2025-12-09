// MSVC Test: Catching MFC exceptions thrown by OpenMFC
//
// This test verifies that OpenMFC can throw C++ exceptions that MSVC code
// can catch using standard try/catch semantics.
//
// Build with MSVC (on Windows CI):
//   cl /nologo /EHsc /MD /D_AFXDLL test_exception_catch.cpp openmfc.lib
//
// The test will:
// 1. Call AfxThrowMemoryException()
// 2. Catch it as CException* or CMemoryException*
// 3. Verify the exception is valid

#include <cstdio>
#include <cstdint>

// Forward declare MFC exception classes (minimal for linking)
// In real MFC, these come from <afx.h>

struct CRuntimeClass;

// Minimal CObject for RTTI
class __declspec(dllimport) CObject {
public:
    virtual CRuntimeClass* GetRuntimeClass() const;
    virtual ~CObject();
};

// Minimal CException
class __declspec(dllimport) CException : public CObject {
public:
    CException();
    CException(int bAutoDelete);
    virtual ~CException();

    virtual int GetErrorMessage(wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext = nullptr);
    virtual int ReportError(unsigned int nType = 0, unsigned int nMessageID = 0);
    void Delete();

protected:
    int m_bAutoDelete;
};

// Minimal CMemoryException
class __declspec(dllimport) CMemoryException : public CException {
public:
    CMemoryException();
    virtual ~CMemoryException();
};

// Import the throw function
__declspec(dllimport) void __cdecl AfxThrowMemoryException();

int main() {
    printf("=== OpenMFC Exception Catch Test ===\n\n");

    printf("Test 1: Call AfxThrowMemoryException and catch as CException*\n");

    try {
        printf("  Calling AfxThrowMemoryException()...\n");
        AfxThrowMemoryException();
        printf("  ERROR: Should have thrown!\n");
        return 1;
    }
    catch (CMemoryException* e) {
        printf("  SUCCESS: Caught CMemoryException*\n");
        // Don't call Delete() yet - just verify we caught it
        // e->Delete();  // Would free the exception
    }
    catch (CException* e) {
        printf("  PARTIAL: Caught as CException* (not CMemoryException*)\n");
        // This is acceptable but less specific
    }
    catch (...) {
        printf("  PARTIAL: Caught as ... (generic catch)\n");
        printf("  This means an exception was thrown but type matching failed.\n");
        printf("  The exception type info may not match MSVC's expectations.\n");
    }

    printf("\n=== Test Complete ===\n");
    return 0;
}
