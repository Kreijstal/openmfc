// Test for MFC Global String Functions
// Build: g++ -std=c++17 -I../include -o test_string_funcs test_string_funcs.cpp

#include <openmfc/afxwin.h>
#include <cstdio>
#include <cstring>

#define TEST(name) void test_##name()
#define ASSERT(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL: %s at %d\n", #cond, __LINE__); failed++; } else { passed++; } } while(0)

static int passed = 0;
static int failed = 0;

TEST(extract_substring) {
    CString strFull = L"One\nTwo\nThree";
    CString strSub;
    
    // Test extraction
    ASSERT(AfxExtractSubString(strSub, strFull, 0));
    ASSERT(strSub == L"One");
    
    ASSERT(AfxExtractSubString(strSub, strFull, 1));
    ASSERT(strSub == L"Two");
    
    ASSERT(AfxExtractSubString(strSub, strFull, 2));
    ASSERT(strSub == L"Three");
    
    // Out of bounds
    ASSERT(!AfxExtractSubString(strSub, strFull, 3));
    
    // Custom separator
    strFull = L"A,B,C";
    ASSERT(AfxExtractSubString(strSub, strFull, 1, L','));
    ASSERT(strSub == L"B");
}

TEST(load_string) {
    // Mock LoadString implementation returns "Resource String %u"
    CString str;
    
    ASSERT(str.LoadString(123));
    ASSERT(str == L"Resource String 123");
    
    ASSERT(str.LoadString(456));
    ASSERT(str == L"Resource String 456");
}

TEST(format_string) {
    CString str;
    
    // Test AfxFormatString1
    // ID 999 returns "Format %1 and %2"
    AfxFormatString1(str, 999, L"One");
    ASSERT(str == L"Format One and %2");
    
    // Test AfxFormatString2
    AfxFormatString2(str, 999, L"One", L"Two");
    ASSERT(str == L"Format One and Two");
    
    // Test with nulls
    AfxFormatString2(str, 999, nullptr, nullptr);
    ASSERT(str == L"Format %1 and %2");
}

TEST(message_box) {
    // Just verify it links and runs (prints to stdout in mock)
    // AfxMessageBox(L"Test Message");
    // AfxMessageBox(123); // Should load string 123
    std::printf("Skipping AfxMessageBox test (headless environment)\n");
}

int main() {
    std::printf("Running String Function tests...\n");
    
    test_extract_substring();
    test_load_string();
    test_format_string();
    test_message_box();
    
    std::printf("Results: %d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
