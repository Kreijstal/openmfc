#include <openmfc/afxstr.h>
#include <cstdio>
#include <cwchar>

// Test CString header-only functionality
// These tests use the header-only CString implementation.
// No DLL loading needed - CString is fully implemented in the header.
//
// Build with MSVC: cl /nologo /EHsc /MD /I include test_string_funcs.cpp openmfc.lib
// Build with MinGW (header-only tests): x86_64-w64-mingw32-g++ -std=c++17 -I include test_string_funcs.cpp -o test.exe

// Forward declarations for DLL-exported functions (compile-time linking)
// These will generate MSVC mangled names when compiled with MSVC
#ifdef _MSC_VER
__declspec(dllimport) int AFXAPI AfxExtractSubString(CString& rString, const wchar_t* lpszFullString, int iSubString, wchar_t chSep);
#endif

int main() {
    int failures = 0;

    // Test 1: CString Concatenation
    printf("Test 1: CString Concatenation... ");
    CString s1(L"Hello");
    CString s2(L" World");
    CString s3 = s1 + s2;
    if (wcscmp(s3.GetString(), L"Hello World") == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL (Expected 'Hello World', got '%ls')\n", s3.GetString());
        failures++;
    }

    // Test 2: AfxExtractSubString (only with MSVC - uses compile-time linking)
#ifdef _MSC_VER
    printf("Test 2: AfxExtractSubString (compile-time linked)... ");
    {
        CString strFull = L"One;Two;Three";
        CString strSub;
        
        int b1 = AfxExtractSubString(strSub, strFull, 0, L';');
        bool b1_val = (strSub == L"One");
        if (!b1 || !b1_val) printf("  [0] ret=%d, val='%ls' (expected 'One')\n", b1, strSub.GetString());

        int b2 = AfxExtractSubString(strSub, strFull, 1, L';');
        bool b2_val = (strSub == L"Two");
        if (!b2 || !b2_val) printf("  [1] ret=%d, val='%ls' (expected 'Two')\n", b2, strSub.GetString());

        int b3 = AfxExtractSubString(strSub, strFull, 2, L';');
        bool b3_val = (strSub == L"Three");
        if (!b3 || !b3_val) printf("  [2] ret=%d, val='%ls' (expected 'Three')\n", b3, strSub.GetString());

        int b4 = AfxExtractSubString(strSub, strFull, 3, L';');
        if (b4) printf("  [3] ret=%d (expected 0)\n", b4);
        
        if (b1 && b1_val && b2 && b2_val && b3 && b3_val && !b4) 
        {
            printf("PASS\n");
        } else {
            printf("FAIL (Logic check failed)\n");
            failures++;
        }
    }
#else
    printf("Test 2: AfxExtractSubString... SKIPPED (requires MSVC)\n");
#endif

    // Test 3: CString Formatting
    printf("Test 3: CString Formatting... ");
    CString sFormat;
    sFormat.Format(L"Value: %d", 42);
    if (sFormat == L"Value: 42") {
        printf("PASS\n");
    } else {
        printf("FAIL (Expected 'Value: 42', got '%ls')\n", sFormat.GetString());
        failures++;
    }

    // Test 4: CString::Mid
    printf("Test 4: CString::Mid... ");
    CString sMid = L"0123456789";
    if (sMid.Mid(3, 4) == L"3456") {
        printf("PASS\n");
    } else {
        printf("FAIL (Expected '3456', got '%ls')\n", sMid.Mid(3, 4).GetString());
        failures++;
    }

    // Test 5: CString::Find
    printf("Test 5: CString::Find... ");
    CString sFind = L"Hello World";
    if (sFind.Find(L'W') == 6) {
        printf("PASS\n");
    } else {
        printf("FAIL (Expected 6, got %d)\n", sFind.Find(L'W'));
        failures++;
    }

    // Test 6: CString::Replace
    printf("Test 6: CString::Replace... ");
    CString sReplace = L"Hello World";
    sReplace.Replace(L"World", L"MFC");
    if (sReplace == L"Hello MFC") {
        printf("PASS\n");
    } else {
        printf("FAIL (Expected 'Hello MFC', got '%ls')\n", sReplace.GetString());
        failures++;
    }

    // Test 7: CString case conversion
    printf("Test 7: CString::MakeUpper... ");
    CString sUpper = L"Hello";
    sUpper.MakeUpper();
    if (sUpper == L"HELLO") {
        printf("PASS\n");
    } else {
        printf("FAIL (Expected 'HELLO', got '%ls')\n", sUpper.GetString());
        failures++;
    }

    if (failures == 0) {
        printf("\nAll tests passed\n");
        return 0;
    } else {
        printf("\n%d tests failed\n", failures);
        return 1;
    }
}
