// Test CString functions across DLL boundary
#include <openmfc/afxstr.h>
#include <cstdio>

int main() {
    printf("Testing CString DLL exports...\n");
    
    // Test 1: CString::LoadString (member function)
    CString str1;
    int len = str1.LoadString(999);  // Should get "Format %1 and %2"
    printf("LoadString(999) returned length: %d\n", len);
    printf("String: %ls\n", (const wchar_t*)str1);
    
    // Test 2: AfxExtractSubString
    CString str2;
    const wchar_t* full = L"Line1\nLine2\nLine3";
    bool found = AfxExtractSubString(str2, full, 1, L'\n');  // Get Line2
    printf("AfxExtractSubString found: %s\n", found ? "true" : "false");
    printf("Extracted: %ls\n", (const wchar_t*)str2);
    
    // Test 3: AfxFormatString1
    CString str3;
    AfxFormatString1(str3, 999, L"First");  // Format "Format %1 and %2" with "First"
    printf("AfxFormatString1 result: %ls\n", (const wchar_t*)str3);
    
    // Test 4: AfxFormatString2  
    CString str4;
    AfxFormatString2(str4, 999, L"First", L"Second");
    printf("AfxFormatString2 result: %ls\n", (const wchar_t*)str4);
    
    // Test 5: AfxMessageBox (will print to console in our stub)
    printf("Calling AfxMessageBox...\n");
    int result = AfxMessageBox(L"Test message from DLL");
    printf("AfxMessageBox returned: %d\n", result);
    
    printf("\nAll CString DLL tests completed!\n");
    return 0;
}