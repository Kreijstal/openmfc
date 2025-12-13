#include <openmfc/afxstr.h>
#include <cstdio>
#include <cwchar>
#include <windows.h>

// Mock AfxMessageBox for testing (since we are in console)
// In a real app, this would be linked from openmfc.dll
// But for this test, we want to verify logic without popping up windows if possible,
// or just verify it links.
// Actually, we are testing the DLL, so we should call the DLL function.
// But running it in CI might block if it shows a modal dialog.
// For now, we will test non-GUI functions primarily.

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

    // Test 2: AfxExtractSubString (via GetProcAddress to handle mangling)
    printf("Test 2: AfxExtractSubString... ");
    
    HMODULE hDll = LoadLibraryA("openmfc.dll");
    if (!hDll) {
        printf("FAIL (LoadLibrary failed)\n");
        failures++;
    } else {
        // MSVC mangled name for AfxExtractSubString
        const char* symName = "?AfxExtractSubString@@YAHAAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PB_WH_W@Z";
        typedef bool (AFXAPI *PFN_AfxExtractSubString)(CString&, const wchar_t*, int, wchar_t);
        PFN_AfxExtractSubString pAfxExtractSubString = (PFN_AfxExtractSubString)GetProcAddress(hDll, symName);
        
        if (!pAfxExtractSubString) {
            printf("FAIL (GetProcAddress failed for %s)\n", symName);
            failures++;
        } else {
            CString strFull = L"One;Two;Three";
            CString strSub;
            
            bool b1 = pAfxExtractSubString(strSub, strFull, 0, L';');
            bool b1_val = (strSub == L"One");
            if (!b1 || !b1_val) printf("  [0] ret=%d, val='%ls' (expected 'One')\n", b1, strSub.GetString());

            bool b2 = pAfxExtractSubString(strSub, strFull, 1, L';');
            bool b2_val = (strSub == L"Two");
            if (!b2 || !b2_val) printf("  [1] ret=%d, val='%ls' (expected 'Two')\n", b2, strSub.GetString());

            bool b3 = pAfxExtractSubString(strSub, strFull, 2, L';');
            bool b3_val = (strSub == L"Three");
            if (!b3 || !b3_val) printf("  [2] ret=%d, val='%ls' (expected 'Three')\n", b3, strSub.GetString());

            bool b4 = pAfxExtractSubString(strSub, strFull, 3, L';');
            if (b4) printf("  [3] ret=%d (expected 0)\n", b4);
            
            if (b1 && b1_val && b2 && b2_val && b3 && b3_val && !b4) 
            {
                printf("PASS\n");
            } else {
                printf("FAIL (Logic check failed)\n");
                failures++;
            }
        }
        FreeLibrary(hDll);
    }

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

    if (failures == 0) {
        printf("All tests passed\n");
        return 0;
    } else {
        printf("%d tests failed\n", failures);
        return 1;
    }
}
