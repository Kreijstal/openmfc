// Generated Export ABI Test
// Tests that key MFC exports have correct ABI

#include <windows.h>
#include <cstdio>

// Test loading by ordinal
bool test_ordinal_loading(HMODULE hModule) {
    bool all_ok = true;
    
    // Test ordinal 256: ??$CopyElements@VCOleVariant@@@@YAXPAVCOleVariant@@PBV0@H@Z
    FARPROC func256 = GetProcAddress(hModule, MAKEINTRESOURCEA(256));
    if (!func256) {
        printf("FAIL: Could not load ordinal 256\n");
        all_ok = false;
    }
    
    // Test ordinal 257: ??$HashKey@ABUtagVARIANT@@@@YAIABUtagVARIANT@@@Z
    FARPROC func257 = GetProcAddress(hModule, MAKEINTRESOURCEA(257));
    if (!func257) {
        printf("FAIL: Could not load ordinal 257\n");
        all_ok = false;
    }
    
    // Test ordinal 258: ??$HashKey@PB_W@@YAIPB_W@Z
    FARPROC func258 = GetProcAddress(hModule, MAKEINTRESOURCEA(258));
    if (!func258) {
        printf("FAIL: Could not load ordinal 258\n");
        all_ok = false;
    }
    
    // Test ordinal 259: ??$HashKey@PBD@@YAIPBD@Z
    FARPROC func259 = GetProcAddress(hModule, MAKEINTRESOURCEA(259));
    if (!func259) {
        printf("FAIL: Could not load ordinal 259\n");
        all_ok = false;
    }
    
    // Test ordinal 260: ??$HashKey@VCComBSTR@ATL@@@@YAIVCComBSTR@ATL@@@Z
    FARPROC func260 = GetProcAddress(hModule, MAKEINTRESOURCEA(260));
    if (!func260) {
        printf("FAIL: Could not load ordinal 260\n");
        all_ok = false;
    }
    
    // Test ordinal 261: ??$SerializeElements@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@@YAXAAVCArchive@@PAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
    FARPROC func261 = GetProcAddress(hModule, MAKEINTRESOURCEA(261));
    if (!func261) {
        printf("FAIL: Could not load ordinal 261\n");
        all_ok = false;
    }
    
    // Test ordinal 262: ??$SerializeElements@V?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@@YAXAAVCArchive@@PAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@H@Z
    FARPROC func262 = GetProcAddress(hModule, MAKEINTRESOURCEA(262));
    if (!func262) {
        printf("FAIL: Could not load ordinal 262\n");
        all_ok = false;
    }
    
    // Test ordinal 263: ??$SerializeElements@VCComBSTR@ATL@@@@YAXAAVCArchive@@PAVCComBSTR@ATL@@H@Z
    FARPROC func263 = GetProcAddress(hModule, MAKEINTRESOURCEA(263));
    if (!func263) {
        printf("FAIL: Could not load ordinal 263\n");
        all_ok = false;
    }
    
    // Test ordinal 264: ??$SerializeElements@VCOleVariant@@@@YAXAAVCArchive@@PAVCOleVariant@@H@Z
    FARPROC func264 = GetProcAddress(hModule, MAKEINTRESOURCEA(264));
    if (!func264) {
        printf("FAIL: Could not load ordinal 264\n");
        all_ok = false;
    }
    
    return all_ok;
}

// Test loading by name (MSVC mangled)
bool test_name_loading(HMODULE hModule) {
    bool all_ok = true;
    
    // Test symbol: ??$CopyElements@VCOleVariant@@@@YAXPAVCOleVariant@@PBV0@H@Z
    FARPROC func_256_name = GetProcAddress(hModule, "??$CopyElements@VCOleVariant@@@@YAXPAVCOleVariant@@PBV0@H@Z");
    if (!func_256_name) {
        printf("FAIL: Could not load by name: ??$CopyElements@VCOleVariant@@@@YAXPAVCOleVariant@@PBV0@H@Z\n");
        all_ok = false;
    }
    
    // Test symbol: ??$HashKey@ABUtagVARIANT@@@@YAIABUtagVARIANT@@@Z
    FARPROC func_257_name = GetProcAddress(hModule, "??$HashKey@ABUtagVARIANT@@@@YAIABUtagVARIANT@@@Z");
    if (!func_257_name) {
        printf("FAIL: Could not load by name: ??$HashKey@ABUtagVARIANT@@@@YAIABUtagVARIANT@@@Z\n");
        all_ok = false;
    }
    
    // Test symbol: ??$HashKey@PB_W@@YAIPB_W@Z
    FARPROC func_258_name = GetProcAddress(hModule, "??$HashKey@PB_W@@YAIPB_W@Z");
    if (!func_258_name) {
        printf("FAIL: Could not load by name: ??$HashKey@PB_W@@YAIPB_W@Z\n");
        all_ok = false;
    }
    
    // Test symbol: ??$HashKey@PBD@@YAIPBD@Z
    FARPROC func_259_name = GetProcAddress(hModule, "??$HashKey@PBD@@YAIPBD@Z");
    if (!func_259_name) {
        printf("FAIL: Could not load by name: ??$HashKey@PBD@@YAIPBD@Z\n");
        all_ok = false;
    }
    
    // Test symbol: ??$HashKey@VCComBSTR@ATL@@@@YAIVCComBSTR@ATL@@@Z
    FARPROC func_260_name = GetProcAddress(hModule, "??$HashKey@VCComBSTR@ATL@@@@YAIVCComBSTR@ATL@@@Z");
    if (!func_260_name) {
        printf("FAIL: Could not load by name: ??$HashKey@VCComBSTR@ATL@@@@YAIVCComBSTR@ATL@@@Z\n");
        all_ok = false;
    }
    
    // Test symbol: ??$SerializeElements@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@@YAXAAVCArchive@@PAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
    FARPROC func_261_name = GetProcAddress(hModule, "??$SerializeElements@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@@YAXAAVCArchive@@PAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z");
    if (!func_261_name) {
        printf("FAIL: Could not load by name: ??$SerializeElements@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@@YAXAAVCArchive@@PAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z\n");
        all_ok = false;
    }
    
    // Test symbol: ??$SerializeElements@V?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@@YAXAAVCArchive@@PAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@H@Z
    FARPROC func_262_name = GetProcAddress(hModule, "??$SerializeElements@V?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@@YAXAAVCArchive@@PAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@H@Z");
    if (!func_262_name) {
        printf("FAIL: Could not load by name: ??$SerializeElements@V?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@@YAXAAVCArchive@@PAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@H@Z\n");
        all_ok = false;
    }
    
    // Test symbol: ??$SerializeElements@VCComBSTR@ATL@@@@YAXAAVCArchive@@PAVCComBSTR@ATL@@H@Z
    FARPROC func_263_name = GetProcAddress(hModule, "??$SerializeElements@VCComBSTR@ATL@@@@YAXAAVCArchive@@PAVCComBSTR@ATL@@H@Z");
    if (!func_263_name) {
        printf("FAIL: Could not load by name: ??$SerializeElements@VCComBSTR@ATL@@@@YAXAAVCArchive@@PAVCComBSTR@ATL@@H@Z\n");
        all_ok = false;
    }
    
    // Test symbol: ??$SerializeElements@VCOleVariant@@@@YAXAAVCArchive@@PAVCOleVariant@@H@Z
    FARPROC func_264_name = GetProcAddress(hModule, "??$SerializeElements@VCOleVariant@@@@YAXAAVCArchive@@PAVCOleVariant@@H@Z");
    if (!func_264_name) {
        printf("FAIL: Could not load by name: ??$SerializeElements@VCOleVariant@@@@YAXAAVCArchive@@PAVCOleVariant@@H@Z\n");
        all_ok = false;
    }
    
    return all_ok;
}

int main() {
    printf("=== Generated Export ABI Test ===\n");
    
    HMODULE hModule = LoadLibraryA("openmfc.dll");
    if (!hModule) {
        printf("Error: Could not load openmfc.dll\n");
        return 1;
    }
    
    bool ordinal_ok = test_ordinal_loading(hModule);
    bool name_ok = test_name_loading(hModule);
    
    FreeLibrary(hModule);
    
    if (ordinal_ok && name_ok) {
        printf("✅ Export ABI compatible\n");
        return 0;
    } else {
        printf("❌ Export ABI issues detected\n");
        return 1;
    }
}
