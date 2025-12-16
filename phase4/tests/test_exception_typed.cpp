// Typed exception test - catch with CMemoryException* without class imports
//
// Build with MSVC:
//   cl /nologo /EHsc /MD test_exception_typed.cpp openmfc.lib
//
// This test tries to catch CMemoryException* by declaring our own
// minimal class structure, avoiding __declspec(dllimport) vtable issues.

#include <cstdio>
#include <cstdint>

// Import only the throw function
__declspec(dllimport) void AfxThrowMemoryException();

// Forward declare CRuntimeClass to avoid vtable import
struct CRuntimeClass;

// Minimal CMemoryException layout - no dllimport to avoid vtable import issues
// We just need the class structure to match what OpenMFC throws
class CObject {
public:
    virtual ~CObject() {}
    virtual CRuntimeClass* GetRuntimeClass() const { return nullptr; }
};

class CException : public CObject {
public:
    int m_bAutoDelete;
    virtual ~CException() {}
};

class CMemoryException : public CException {
public:
    virtual ~CMemoryException() {}
};

int main() {
    printf("=== Typed Exception Test ===\n");
    printf("Testing if we can catch CMemoryException* specifically\n\n");
    fflush(stdout);

    printf("Test 1: Try to catch as CMemoryException*\n");
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
        fflush(stdout);
    }
    catch (...) {
        printf("  PARTIAL: Caught with ... (type matching failed)\n");
        fflush(stdout);
    }

    printf("\n=== Test Complete ===\n");
    fflush(stdout);
    return 0;
}
