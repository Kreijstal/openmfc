// MFC Smoke Test for OpenMFC
//
// This test uses real MFC headers (#include <afx.h>) and verifies that
// MSVC can link MFC code against our OpenMFC DLL.
//
// This is the strongest possible link-time verification because:
// 1. We use Microsoft's actual MFC headers
// 2. MSVC generates all the mangled names for CString, CObject, etc.
// 3. If linking succeeds, our exports match what real MFC apps need

// Must define this before including MFC headers
#define _AFXDLL  // Use MFC as a shared DLL

#include <afx.h>
#include <cstdio>

int main() {
    printf("=== OpenMFC MFC Smoke Test ===\n\n");

    // If we reach main(), MSVC successfully linked all MFC symbols.
    printf("Link test PASSED: MSVC resolved all MFC imports.\n\n");

    printf("Testing CString...\n");

    // CString constructor from const char* (ANSI)
    // This calls: ??0?$CStringT@_WV?$StrTraitMFC_DLL@...@@QAA@PBD@Z
    CString s1("Hello");
    printf("  CString s1(\"Hello\") - constructed\n");

    // CString constructor from wide string
    // This calls: ??0?$CStringT@_WV?$StrTraitMFC_DLL@...@@QAA@PB_W@Z
    CString s2(L"World");
    printf("  CString s2(L\"World\") - constructed\n");

    // CString copy constructor
    CString s3(s1);
    printf("  CString s3(s1) - copy constructed\n");

    // CString concatenation (operator+)
    CString s4 = s1 + L" " + s2;
    printf("  CString s4 = s1 + L\" \" + s2 - concatenated\n");

    // CString::GetLength()
    int len = s4.GetLength();
    printf("  s4.GetLength() = %d\n", len);

    // Destructors will be called when going out of scope
    printf("\n=== All tests passed ===\n");
    printf("OpenMFC successfully linked with real MFC headers.\n");

    return 0;
}
