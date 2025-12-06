// Test for CString implementation
// Build: x86_64-w64-mingw32-g++ -I../include -o test_cstring.exe test_cstring.cpp

#include <openmfc/afxstr.h>
#include <cstdio>

#define TEST(name) void test_##name()
#define ASSERT(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL: %s at %d\n", #cond, __LINE__); failed++; } else { passed++; } } while(0)

static int passed = 0;
static int failed = 0;

TEST(construction) {
    // Default constructor
    CString s1;
    ASSERT(s1.IsEmpty());
    ASSERT(s1.GetLength() == 0);

    // From literal
    CString s2(L"Hello");
    ASSERT(!s2.IsEmpty());
    ASSERT(s2.GetLength() == 5);
    ASSERT(wcscmp(s2.GetString(), L"Hello") == 0);

    // Copy constructor
    CString s3(s2);
    ASSERT(s3.GetLength() == 5);
    ASSERT(s3 == s2);

    // Empty string literal
    CString s4(L"");
    ASSERT(s4.IsEmpty());
}

TEST(assignment) {
    CString s1(L"Hello");
    CString s2;
    
    s2 = s1;
    ASSERT(s2 == L"Hello");
    
    s2 = L"World";
    ASSERT(s2 == L"World");
    ASSERT(s1 == L"Hello"); // s1 unchanged
    
    s2 = L"";
    ASSERT(s2.IsEmpty());
}

TEST(concatenation) {
    CString s1(L"Hello");
    CString s2(L" World");
    
    CString s3 = s1 + s2;
    ASSERT(s3 == L"Hello World");
    
    s1 += L"!";
    ASSERT(s1 == L"Hello!");
    
    s1 += s2;
    ASSERT(s1 == L"Hello! World");
}

TEST(comparison) {
    CString s1(L"ABC");
    CString s2(L"ABC");
    CString s3(L"DEF");
    
    ASSERT(s1 == s2);
    ASSERT(s1 != s3);
    ASSERT(s1 == L"ABC");
    ASSERT(s1 != L"XYZ");
}

TEST(substring) {
    CString s(L"Hello World");
    
    CString left = s.Left(5);
    ASSERT(left == L"Hello");
    
    CString right = s.Right(5);
    ASSERT(right == L"World");
    
    CString mid = s.Mid(6, 5);
    ASSERT(mid == L"World");
}

TEST(buffer) {
    CString s(L"Hello");
    
    wchar_t* buf = s.GetBuffer(10);
    buf[5] = L'!';
    buf[6] = L'\0';
    s.ReleaseBuffer();
    
    ASSERT(s == L"Hello!");
    ASSERT(s.GetLength() == 6);
}

TEST(sizeof_check) {
    // CString should be one pointer
    ASSERT(sizeof(CString) == sizeof(void*));

    // CStringData should be 16 bytes
    ASSERT(sizeof(CStringData) == 16);
}

TEST(find) {
    CString s(L"Hello World");

    ASSERT(s.Find(L'o') == 4);
    ASSERT(s.Find(L'o', 5) == 7);
    ASSERT(s.Find(L'x') == -1);
    ASSERT(s.Find(L"World") == 6);
    ASSERT(s.Find(L"xyz") == -1);
    ASSERT(s.ReverseFind(L'o') == 7);
    ASSERT(s.FindOneOf(L"aeiou") == 1); // 'e' at position 1
}

TEST(case_conversion) {
    CString s1(L"Hello World");
    s1.MakeUpper();
    ASSERT(s1 == L"HELLO WORLD");

    CString s2(L"Hello World");
    s2.MakeLower();
    ASSERT(s2 == L"hello world");
}

TEST(trim) {
    CString s1(L"  Hello  ");
    s1.Trim();
    ASSERT(s1 == L"Hello");

    CString s2(L"  Hello");
    s2.TrimLeft();
    ASSERT(s2 == L"Hello");

    CString s3(L"Hello  ");
    s3.TrimRight();
    ASSERT(s3 == L"Hello");
}

TEST(replace) {
    CString s1(L"Hello World");
    int count = s1.Replace(L'o', L'0');
    ASSERT(count == 2);
    ASSERT(s1 == L"Hell0 W0rld");

    CString s2(L"foo bar foo");
    count = s2.Replace(L"foo", L"baz");
    ASSERT(count == 2);
    ASSERT(s2 == L"baz bar baz");
}

TEST(format) {
    CString s;
    s.Format(L"Hello %ls", L"World");
    ASSERT(s == L"Hello World");

    CString s2;
    s2.Format(L"Number: %d", 42);
    ASSERT(s2 == L"Number: 42");
}

TEST(compare_nocase) {
    CString s(L"Hello");
    ASSERT(s.CompareNoCase(L"HELLO") == 0);
    ASSERT(s.CompareNoCase(L"hello") == 0);
    ASSERT(s.CompareNoCase(L"World") != 0);
}

TEST(delete_insert) {
    CString s1(L"Hello World");
    s1.Delete(5, 6); // Delete " World"
    ASSERT(s1 == L"Hello");

    CString s2(L"Hello");
    s2.Insert(5, L'!');
    ASSERT(s2 == L"Hello!");

    CString s3(L"Hello!");
    s3.Insert(5, L" World");
    ASSERT(s3 == L"Hello World!");
}

TEST(reverse) {
    CString s(L"Hello");
    s.MakeReverse();
    ASSERT(s == L"olleH");
}

TEST(span) {
    CString s(L"123abc456");
    CString digits = s.SpanIncluding(L"0123456789");
    ASSERT(digits == L"123");

    CString letters = s.SpanExcluding(L"0123456789");
    ASSERT(letters == L"");  // starts with digit

    CString s2(L"abc123");
    CString alpha = s2.SpanExcluding(L"0123456789");
    ASSERT(alpha == L"abc");
}

int main() {
    std::printf("Running CString tests...\n");

    test_construction();
    test_assignment();
    test_concatenation();
    test_comparison();
    test_substring();
    test_buffer();
    test_sizeof_check();
    test_find();
    test_case_conversion();
    test_trim();
    test_replace();
    test_format();
    test_compare_nocase();
    test_delete_insert();
    test_reverse();
    test_span();

    std::printf("Results: %d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
