// Test for CObject and CRuntimeClass implementation
// Build: x86_64-w64-mingw32-g++ -std=c++17 -I../include -o test_cobject.exe test_cobject.cpp

#include <openmfc/afx.h>
#include <cstdio>
#include <cstring>

#define TEST(name) void test_##name()
#define ASSERT(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL: %s at %d\n", #cond, __LINE__); failed++; } else { passed++; } } while(0)

static int passed = 0;
static int failed = 0;

// Test class derived from CObject
class CTestObject : public CObject {
    DECLARE_DYNAMIC(CTestObject)
public:
    CTestObject() : m_value(42) {}
    int GetValue() const { return m_value; }
private:
    int m_value;
};

IMPLEMENT_DYNAMIC(CTestObject, CObject)

// Test class with DYNCREATE
class CCreatableObject : public CObject {
    DECLARE_DYNCREATE(CCreatableObject)
public:
    CCreatableObject() : m_id(123) {}
    int GetId() const { return m_id; }
private:
    int m_id;
};

IMPLEMENT_DYNCREATE(CCreatableObject, CObject)

// Two-level inheritance
class CDerivedTest : public CTestObject {
    DECLARE_DYNAMIC(CDerivedTest)
public:
    CDerivedTest() : m_extra(999) {}
    int GetExtra() const { return m_extra; }
private:
    int m_extra;
};

IMPLEMENT_DYNAMIC(CDerivedTest, CTestObject)

TEST(sizeof_check) {
    // CObject should be 8 bytes (vptr only)
    ASSERT(sizeof(CObject) == 8);
    
    // Derived classes have additional data
    ASSERT(sizeof(CTestObject) > sizeof(CObject));
}

TEST(runtime_class) {
    CTestObject obj;
    
    // GetRuntimeClass should return the correct class
    CRuntimeClass* pClass = obj.GetRuntimeClass();
    ASSERT(pClass != nullptr);
    ASSERT(pClass == RUNTIME_CLASS(CTestObject));
    
    // Class name should be correct
    ASSERT(std::strcmp(pClass->m_lpszClassName, "CTestObject") == 0);
    
    // Base class should be CObject
    ASSERT(pClass->m_pBaseClass == RUNTIME_CLASS(CObject));
}

TEST(is_kind_of) {
    CTestObject obj;
    CDerivedTest derived;
    
    // CTestObject is a CTestObject
    ASSERT(obj.IsKindOf(RUNTIME_CLASS(CTestObject)));
    
    // CTestObject is a CObject
    ASSERT(obj.IsKindOf(RUNTIME_CLASS(CObject)));
    
    // CDerivedTest is a CDerivedTest, CTestObject, and CObject
    ASSERT(derived.IsKindOf(RUNTIME_CLASS(CDerivedTest)));
    ASSERT(derived.IsKindOf(RUNTIME_CLASS(CTestObject)));
    ASSERT(derived.IsKindOf(RUNTIME_CLASS(CObject)));
    
    // CTestObject is NOT a CDerivedTest
    ASSERT(!obj.IsKindOf(RUNTIME_CLASS(CDerivedTest)));
}

TEST(dynamic_create) {
    // CCreatableObject has CreateObject factory
    CRuntimeClass* pClass = RUNTIME_CLASS(CCreatableObject);
    
    CObject* pObj = pClass->CreateObject();
    ASSERT(pObj != nullptr);
    
    // Should be the right type
    ASSERT(pObj->IsKindOf(RUNTIME_CLASS(CCreatableObject)));
    
    // Should be functional
    CCreatableObject* pCreatable = static_cast<CCreatableObject*>(pObj);
    ASSERT(pCreatable->GetId() == 123);
    
    delete pObj;
}

TEST(non_creatable) {
    // CTestObject does NOT have CreateObject factory (uses DECLARE_DYNAMIC not DECLARE_DYNCREATE)
    CRuntimeClass* pClass = RUNTIME_CLASS(CTestObject);
    
    CObject* pObj = pClass->CreateObject();
    ASSERT(pObj == nullptr);
}

TEST(cobject_base) {
    // CObject's base class should be nullptr
    CRuntimeClass* pClass = RUNTIME_CLASS(CObject);
    ASSERT(pClass->m_pBaseClass == nullptr);
    ASSERT(std::strcmp(pClass->m_lpszClassName, "CObject") == 0);
}

TEST(virtual_methods) {
    CTestObject* pTest = new CTestObject();
    CObject* pObj = pTest;
    
    // Virtual GetRuntimeClass should resolve to CTestObject
    ASSERT(pObj->GetRuntimeClass() == RUNTIME_CLASS(CTestObject));
    
    delete pTest;
}

int main() {
    std::printf("Running CObject tests...\n");
    
    test_sizeof_check();
    test_runtime_class();
    test_is_kind_of();
    test_dynamic_create();
    test_non_creatable();
    test_cobject_base();
    test_virtual_methods();
    
    std::printf("Results: %d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
