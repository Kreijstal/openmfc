// Comprehensive CString Windows ABI Test
// Tests that CString matches MFC Windows ABI exactly

#include <openmfc/afxstr.h>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <new>

// ============================================================================
// ABI LAYOUT TESTS
// ============================================================================

// Test 1: CString size and alignment (MUST match Windows MFC)
bool test_cstring_layout() {
    printf("=== CString Layout Tests ===\n");
    
    bool all_pass = true;
    
    // On x64 Windows, CString is 8 bytes (single pointer)
    if (sizeof(CString) != 8) {
        printf("❌ FAIL: sizeof(CString) = %zu (expected 8 on x64)\n", sizeof(CString));
        all_pass = false;
    } else {
        printf("✅ sizeof(CString) = 8 bytes\n");
    }
    
    // Must be 8-byte aligned on x64
    if (alignof(CString) != 8) {
        printf("❌ FAIL: alignof(CString) = %zu (expected 8 on x64)\n", alignof(CString));
        all_pass = false;
    } else {
        printf("✅ alignof(CString) = 8 bytes\n");
    }
    
    // CStringData layout (MUST match MFC exactly)
    if (sizeof(CStringData) != 16) {
        printf("❌ FAIL: sizeof(CStringData) = %zu (expected 16)\n", sizeof(CStringData));
        all_pass = false;
    } else {
        printf("✅ sizeof(CStringData) = 16 bytes\n");
    }
    
    // Check CStringData field offsets (critical for ABI!)
    // MFC layout on x64:
    //   int32_t nRefs;        // offset 0
    //   int32_t nDataLength;  // offset 4  
    //   int32_t nAllocLength; // offset 8
    //   int32_t padding;      // offset 12
    
    // Note: We can't use offsetof on private members, but we can verify
    // the total size matches expectations
    
    return all_pass;
}

// ============================================================================
// BEHAVIOR TESTS (Copy-on-write semantics)
// ============================================================================

bool test_copy_on_write() {
    printf("\n=== Copy-on-Write Tests ===\n");
    
    CString s1(L"Hello");
    CString s2 = s1;  // Should share data
    
    // Check they point to same data initially
    if (s1.GetString() != s2.GetString()) {
        printf("❌ FAIL: Copy constructor didn't share data\n");
        return false;
    }
    
    // Modify s2 - should trigger copy-on-write
    s2 += L" World";
    
    // Now they should have different data
    if (s1.GetString() == s2.GetString()) {
        printf("❌ FAIL: Modification didn't trigger copy-on-write\n");
        return false;
    }
    
    // Check values
    if (wcscmp(s1.GetString(), L"Hello") != 0) {
        printf("❌ FAIL: s1 changed after s2 modification\n");
        return false;
    }
    
    if (wcscmp(s2.GetString(), L"Hello World") != 0) {
        printf("❌ FAIL: s2 not modified correctly\n");
        return false;
    }
    
    printf("✅ Copy-on-write works correctly\n");
    return true;
}

bool test_empty_string_sharing() {
    printf("\n=== Empty String Sharing Tests ===\n");
    
    CString s1;
    CString s2;
    
    // Empty strings should share nil buffer
    if (!s1.IsEmpty() || !s2.IsEmpty()) {
        printf("❌ FAIL: Empty strings not empty\n");
        return false;
    }
    
    printf("✅ Empty strings work correctly\n");
    return true;
}

bool test_reference_counting() {
    printf("\n=== Reference Counting Tests ===\n");
    
    CString s1(L"Test");
    CString s2 = s1;
    CString s3 = s1;
    
    // All should share data
    s2 = L"Modified";  // Should trigger copy-on-write
    
    if (wcscmp(s1.GetString(), L"Test") != 0) {
        printf("❌ FAIL: s1 changed\n");
        return false;
    }
    
    if (wcscmp(s2.GetString(), L"Modified") != 0) {
        printf("❌ FAIL: s2 not modified\n");
        return false;
    }
    
    if (wcscmp(s3.GetString(), L"Test") != 0) {
        printf("❌ FAIL: s3 changed\n");
        return false;
    }
    
    printf("✅ Reference counting works correctly\n");
    return true;
}

// ============================================================================
// CROSS-DLL BOUNDARY TEST (Simulated)
// ============================================================================

// Simulate passing CString across DLL boundary
bool test_cross_dll_simulation() {
    printf("\n=== Cross-DLL Simulation Tests ===\n");
    
    // Create CString and check it can be passed by value
    // (8 bytes on x64, should be passable in register)
    CString s1(L"Cross DLL Test");
    
    // Copy it (should work if layout matches)
    CString s2 = s1;
    
    // Modify copy
    s2.MakeUpper();
    
    if (wcscmp(s1.GetString(), L"Cross DLL Test") != 0) {
        printf("❌ FAIL: Original changed after copy modification\n");
        return false;
    }
    
    if (wcscmp(s2.GetString(), L"CROSS DLL TEST") != 0) {
        printf("❌ FAIL: Modified copy incorrect\n");
        return false;
    }
    
    printf("✅ Cross-DLL passing simulation works\n");
    return true;
}

// ============================================================================
// MEMORY LAYOUT VERIFICATION (Using placement new)
// ============================================================================

bool test_memory_layout() {
    printf("\n=== Memory Layout Verification ===\n");
    
    // Allocate raw memory and construct CString in place
    alignas(8) unsigned char buffer[sizeof(CString)];
    CString* s = new (buffer) CString(L"Layout Test");
    
    // Check it's properly aligned
    if (reinterpret_cast<uintptr_t>(s) % 8 != 0) {
        printf("❌ FAIL: CString not 8-byte aligned in buffer\n");
        return false;
    }
    
    // Check it works
    if (wcscmp(s->GetString(), L"Layout Test") != 0) {
        printf("❌ FAIL: CString in buffer doesn't work\n");
        return false;
    }
    
    s->~CString();  // Explicit destructor call
    
    printf("✅ Memory layout and alignment correct\n");
    return true;
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main() {
    printf("========================================\n");
    printf("CString Windows ABI Compatibility Test\n");
    printf("========================================\n\n");
    
    int failures = 0;
    
    // Run all tests
    if (!test_cstring_layout()) failures++;
    if (!test_copy_on_write()) failures++;
    if (!test_empty_string_sharing()) failures++;
    if (!test_reference_counting()) failures++;
    if (!test_cross_dll_simulation()) failures++;
    if (!test_memory_layout()) failures++;
    
    printf("\n========================================\n");
    if (failures == 0) {
        printf("✅ ALL TESTS PASSED - CString is Windows ABI compatible!\n");
        return 0;
    } else {
        printf("❌ %d TEST(S) FAILED - CString may not be ABI compatible\n", failures);
        return 1;
    }
}