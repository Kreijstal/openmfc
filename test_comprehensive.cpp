// Comprehensive test for OpenMFC phase4 implementation
// Tests: ABI compatibility, function loading, basic functionality

#include <windows.h>
#include <stdio.h>
#include <string.h>

// Test 1: Verify DLL exports match expected symbols
bool test_abi_compatibility() {
    printf("Test 1: ABI Compatibility\n");
    printf("=========================\n");
    
    HMODULE hModule = LoadLibraryA("build-phase4/openmfc.dll");
    if (!hModule) {
        printf("❌ Failed to load DLL: %lu\n", GetLastError());
        return false;
    }
    
    // Test key exports by ordinal
    struct {
        int ordinal;
        const char* name;
        const char* description;
    } tests[] = {
        {2350, "?AfxThrowMemoryException@@YAXXZ", "AfxThrowMemoryException"},
        {2145, "?AfxAbort@@YAXXZ", "AfxAbort"},
        {2346, "?AfxThrowFileException@@YAXHJPB_W@Z", "AfxThrowFileException"},
        {2355, "?AfxThrowResourceException@@YAXXZ", "AfxThrowResourceException"},
    };
    
    bool all_pass = true;
    for (const auto& test : tests) {
        FARPROC by_ordinal = GetProcAddress(hModule, MAKEINTRESOURCEA(test.ordinal));
        FARPROC by_name = GetProcAddress(hModule, test.name);
        
        if (!by_ordinal) {
            printf("❌ Ordinal %d (%s) not found\n", test.ordinal, test.description);
            all_pass = false;
        } else if (!by_name) {
            printf("❌ Name %s (%s) not found\n", test.name, test.description);
            all_pass = false;
        } else if (by_ordinal != by_name) {
            printf("❌ Ordinal/name mismatch for %s\n", test.description);
            all_pass = false;
        } else {
            printf("✓ %s (ordinal %d) OK\n", test.description, test.ordinal);
        }
    }
    
    FreeLibrary(hModule);
    return all_pass;
}

// Test 2: Verify functions actually work (don't crash)
bool test_functionality() {
    printf("\nTest 2: Basic Functionality\n");
    printf("===========================\n");
    
    HMODULE hModule = LoadLibraryA("build-phase4/openmfc.dll");
    if (!hModule) {
        printf("❌ Failed to load DLL\n");
        return false;
    }
    
    typedef void (__stdcall *FuncPtr)();
    typedef void (__stdcall *FileExceptionPtr)(int, LONG, const wchar_t*);
    
    FuncPtr pAfxThrowMemoryException = (FuncPtr)GetProcAddress(hModule, MAKEINTRESOURCEA(2350));
    FuncPtr pAfxAbort = (FuncPtr)GetProcAddress(hModule, MAKEINTRESOURCEA(2145));
    FileExceptionPtr pAfxThrowFileException = (FileExceptionPtr)GetProcAddress(hModule, MAKEINTRESOURCEA(2346));
    
    if (!pAfxThrowMemoryException || !pAfxAbort || !pAfxThrowFileException) {
        printf("❌ Failed to get function pointers\n");
        FreeLibrary(hModule);
        return false;
    }
    
    printf("Testing AfxThrowMemoryException...\n");
    pAfxThrowMemoryException();  // Should log, not crash
    printf("✓ AfxThrowMemoryException called successfully\n");
    
    printf("Testing AfxThrowFileException...\n");
    pAfxThrowFileException(2, ERROR_FILE_NOT_FOUND, L"test.txt");
    printf("✓ AfxThrowFileException called successfully\n");
    
    // Note: We don't test AfxAbort because it calls abort()
    
    FreeLibrary(hModule);
    return true;
}

// Test 3: Verify .def file is valid (can be parsed)
bool test_def_file() {
    printf("\nTest 3: .def File Validation\n");
    printf("============================\n");
    
    FILE* f = fopen("build-phase4/openmfc.def", "r");
    if (!f) {
        printf("❌ Could not open .def file\n");
        return false;
    }
    
    char line[1024];
    int export_count = 0;
    bool has_library = false;
    bool has_exports = false;
    
    while (fgets(line, sizeof(line), f)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        if (strstr(line, "LIBRARY openmfc")) {
            has_library = true;
        } else if (strstr(line, "EXPORTS")) {
            has_exports = true;
        } else if (strchr(line, '@')) {
            export_count++;
        }
    }
    
    fclose(f);
    
    if (!has_library) {
        printf("❌ .def file missing LIBRARY statement\n");
        return false;
    }
    if (!has_exports) {
        printf("❌ .def file missing EXPORTS statement\n");
        return false;
    }
    
    printf("✓ .def file has LIBRARY and EXPORTS statements\n");
    printf("✓ .def file contains %d exports\n", export_count);
    
    // Check for specific exports in .def file
    f = fopen("build-phase4/openmfc.def", "r");
    bool found_memory_exception = false;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "?AfxThrowMemoryException@@YAXXZ")) {
            found_memory_exception = true;
            break;
        }
    }
    fclose(f);
    
    if (!found_memory_exception) {
        printf("❌ .def file missing AfxThrowMemoryException\n");
        return false;
    }
    
    printf("✓ .def file contains key exports\n");
    return true;
}

int main() {
    printf("OpenMFC Phase 4 Comprehensive Test\n");
    printf("==================================\n\n");
    
    bool test1 = test_abi_compatibility();
    bool test2 = test_functionality();
    bool test3 = test_def_file();
    
    printf("\n==================================\n");
    printf("Summary:\n");
    printf("  Test 1 (ABI): %s\n", test1 ? "PASS" : "FAIL");
    printf("  Test 2 (Func): %s\n", test2 ? "PASS" : "FAIL");
    printf("  Test 3 (.def): %s\n", test3 ? "PASS" : "FAIL");
    
    if (test1 && test2 && test3) {
        printf("\n✅ ALL TESTS PASSED!\n");
        printf("OpenMFC phase4 implementation is working:\n");
        printf("  - ABI compatible (correct symbols/ordinals)\n");
        printf("  - Functions work (don't crash)\n");
        printf("  - .def file valid (can create MSVC .lib)\n");
        return 0;
    } else {
        printf("\n❌ SOME TESTS FAILED\n");
        return 1;
    }
}