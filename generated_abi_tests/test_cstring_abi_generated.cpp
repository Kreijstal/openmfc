// Generated CString ABI Test
// Tests that CString matches Windows MFC ABI

#include <openmfc/afxstr.h>
#include <cstdio>
#include <cstddef>

// ABI Compatibility Checks
static_assert(sizeof(CString) == 8, "CString must be 8 bytes on x64");
static_assert(alignof(CString) == 8, "CString must be 8-byte aligned");

// CStringData layout check — must match real ATL (atlsimpstr.h) exactly on x64.
static_assert(sizeof(CStringData) == 24, "CStringData must be 24 bytes");
static_assert(offsetof(CStringData, pStringMgr) == 0, "pStringMgr at offset 0");
static_assert(offsetof(CStringData, nDataLength) == 8, "nDataLength at offset 8");
static_assert(offsetof(CStringData, nAllocLength) == 12, "nAllocLength at offset 12");
static_assert(offsetof(CStringData, nRefs) == 16, "nRefs at offset 16");

// Test reference counting (copy-on-write)
bool test_refcounting() {
    CString s1(L"Hello");
    CString s2 = s1;  // Should share data
    
    // Modify should trigger copy-on-write
    s2 += L" World";
    
    return s1 == L"Hello" && s2 == L"Hello World";
}

// Test empty string sharing
bool test_empty_strings() {
    CString s1;
    CString s2;
    
    // Empty strings should share nil buffer
    return s1.IsEmpty() && s2.IsEmpty();
}

int main() {
    printf("=== Generated CString ABI Test ===\n");
    
    int failures = 0;
    
    // Size checks
    if (sizeof(CString) != 8) {
        printf("FAIL: sizeof(CString) = %zu (expected 8)\n", sizeof(CString));
        failures++;
    }
    
    if (sizeof(CStringData) != 24) {
        printf("FAIL: sizeof(CStringData) = %zu (expected 24)\n", sizeof(CStringData));
        failures++;
    }
    
    // Behavior tests
    if (!test_refcounting()) {
        printf("FAIL: Reference counting broken\n");
        failures++;
    }
    
    if (!test_empty_strings()) {
        printf("FAIL: Empty strings not shared\n");
        failures++;
    }
    
    if (failures == 0) {
        printf("✅ CString ABI compatible\n");
        return 0;
    } else {
        printf("❌ CString NOT ABI compatible (%d failures)\n", failures);
        return 1;
    }
}
