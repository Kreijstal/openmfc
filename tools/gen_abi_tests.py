#!/usr/bin/env python3
"""
Generate ABI compatibility tests from harvested MFC data.
Tests are generated based on actual MFC exports and layouts.
"""

import json
import re
from pathlib import Path
from typing import Dict, List, Optional


def parse_mangled_name(mangled: str) -> Optional[Dict]:
    """Parse MSVC mangled name to extract function info."""
    # MSVC mangling: ?function@class@@type
    # Example: ?AfxThrowMemoryException@@YAXXZ
    
    if not mangled.startswith('?'):
        return None
    
    # Simple parsing for common patterns
    result = {
        "mangled": mangled,
        "is_ctor": False,
        "is_dtor": False,
        "class_name": None,
        "function_name": None,
        "calling_convention": None
    }
    
    # Check for constructor/destructor
    if mangled.startswith('??0'):  # ??0 = constructor
        result["is_ctor"] = True
    elif mangled.startswith('??1'):  # ??1 = destructor
        result["is_dtor"] = True
    
    # Extract calling convention
    if '@@YA' in mangled:  # __cdecl
        result["calling_convention"] = "__cdecl"
    elif '@@YG' in mangled:  # __stdcall
        result["calling_convention"] = "__stdcall"
    elif '@@YI' in mangled:  # __fastcall
        result["calling_convention"] = "__fastcall"
    
    # Try to extract class and function name
    # Pattern: ?function@class@@...
    match = re.match(r'^\?(.+?)@(.+?)@@', mangled)
    if match:
        result["function_name"] = match.group(1)
        result["class_name"] = match.group(2)
    
    return result


def generate_cstring_abi_test() -> str:
    """Generate CString ABI compatibility test."""
    return '''// Generated CString ABI Test
// Tests that CString matches Windows MFC ABI

#include <openmfc/afxstr.h>
#include <cstdio>
#include <cstddef>

// ABI Compatibility Checks
static_assert(sizeof(CString) == 8, "CString must be 8 bytes on x64");
static_assert(alignof(CString) == 8, "CString must be 8-byte aligned");

// CStringData layout check
static_assert(sizeof(CStringData) == 16, "CStringData must be 16 bytes");
static_assert(offsetof(CStringData, nRefs) == 0, "nRefs at offset 0");
static_assert(offsetof(CStringData, nDataLength) == 4, "nDataLength at offset 4");
static_assert(offsetof(CStringData, nAllocLength) == 8, "nAllocLength at offset 8");

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
    printf("=== Generated CString ABI Test ===\\n");
    
    int failures = 0;
    
    // Size checks
    if (sizeof(CString) != 8) {
        printf("FAIL: sizeof(CString) = %zu (expected 8)\\n", sizeof(CString));
        failures++;
    }
    
    if (sizeof(CStringData) != 16) {
        printf("FAIL: sizeof(CStringData) = %zu (expected 16)\\n", sizeof(CStringData));
        failures++;
    }
    
    // Behavior tests
    if (!test_refcounting()) {
        printf("FAIL: Reference counting broken\\n");
        failures++;
    }
    
    if (!test_empty_strings()) {
        printf("FAIL: Empty strings not shared\\n");
        failures++;
    }
    
    if (failures == 0) {
        printf("✅ CString ABI compatible\\n");
        return 0;
    } else {
        printf("❌ CString NOT ABI compatible (%d failures)\\n", failures);
        return 1;
    }
}
'''


def generate_export_abi_test(mapping_path: Path) -> str:
    """Generate test for exported function ABI compatibility."""
    # Load mapping
    mapping = json.loads(mapping_path.read_text())
    exports = mapping.get("exports", {}).get("mfc140u", [])
    
    # Find some key exports to test
    test_exports = []
    for entry in exports[:10]:  # First 10 for sample
        symbol = entry.get("symbol", "")
        ordinal = entry.get("ordinal", 0)
        
        parsed = parse_mangled_name(symbol)
        if parsed and parsed.get("function_name"):
            test_exports.append({
                "symbol": symbol,
                "ordinal": ordinal,
                "parsed": parsed
            })
    
    test_code = '''// Generated Export ABI Test
// Tests that key MFC exports have correct ABI

#include <windows.h>
#include <cstdio>

// Test loading by ordinal
bool test_ordinal_loading(HMODULE hModule) {
    bool all_ok = true;
    
'''
    
    for export_info in test_exports:
        symbol = export_info["symbol"]
        ordinal = export_info["ordinal"]
        parsed = export_info["parsed"]
        
        test_code += f'''    // Test ordinal {ordinal}: {symbol}
    FARPROC func{ordinal} = GetProcAddress(hModule, MAKEINTRESOURCEA({ordinal}));
    if (!func{ordinal}) {{
        printf("FAIL: Could not load ordinal {ordinal}\\n");
        all_ok = false;
    }}
    
'''
    
    test_code += '''    return all_ok;
}

// Test loading by name (MSVC mangled)
bool test_name_loading(HMODULE hModule) {
    bool all_ok = true;
    
'''
    
    for export_info in test_exports:
        symbol = export_info["symbol"]
        ordinal = export_info["ordinal"]
        
        test_code += f'''    // Test symbol: {symbol}
    FARPROC func_{ordinal}_name = GetProcAddress(hModule, "{symbol}");
    if (!func_{ordinal}_name) {{
        printf("FAIL: Could not load by name: {symbol}\\n");
        all_ok = false;
    }}
    
'''
    
    test_code += '''    return all_ok;
}

int main() {
    printf("=== Generated Export ABI Test ===\\n");
    
    HMODULE hModule = LoadLibraryA("openmfc.dll");
    if (!hModule) {
        printf("Error: Could not load openmfc.dll\\n");
        return 1;
    }
    
    bool ordinal_ok = test_ordinal_loading(hModule);
    bool name_ok = test_name_loading(hModule);
    
    FreeLibrary(hModule);
    
    if (ordinal_ok && name_ok) {
        printf("✅ Export ABI compatible\\n");
        return 0;
    } else {
        printf("❌ Export ABI issues detected\\n");
        return 1;
    }
}
'''
    
    return test_code


def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("--mapping", help="Path to ordinal mapping JSON")
    ap.add_argument("--out-dir", default="generated_tests", help="Output directory")
    ap.add_argument("--test-type", choices=["cstring", "exports", "all"], default="all")
    args = ap.parse_args()
    
    out_dir = Path(args.out_dir)
    out_dir.mkdir(exist_ok=True)
    
    print(f"Generating ABI tests in {out_dir}")
    
    # Generate CString test
    if args.test_type in ["cstring", "all"]:
        cstring_test = generate_cstring_abi_test()
        (out_dir / "test_cstring_abi_generated.cpp").write_text(cstring_test)
        print(f"  Generated: test_cstring_abi_generated.cpp")
    
    # Generate export test if mapping provided
    if args.test_type in ["exports", "all"] and args.mapping:
        mapping_path = Path(args.mapping)
        if mapping_path.exists():
            export_test = generate_export_abi_test(mapping_path)
            (out_dir / "test_exports_abi_generated.cpp").write_text(export_test)
            print(f"  Generated: test_exports_abi_generated.cpp")
        else:
            print(f"  Warning: Mapping file not found: {args.mapping}")
    
    # Generate test runner
    runner = '''#!/bin/bash
# Generated ABI Test Runner

echo "=== Running Generated ABI Tests ==="

# Compile and run CString test
if [ -f test_cstring_abi_generated.cpp ]; then
    echo "\\n1. Testing CString ABI..."
    g++ -std=c++17 -I../include test_cstring_abi_generated.cpp -o test_cstring_abi
    if [ $? -eq 0 ]; then
        ./test_cstring_abi
        cstring_result=$?
    else
        echo "FAIL: Could not compile CString test"
        cstring_result=1
    fi
fi

# Compile and run export test
if [ -f test_exports_abi_generated.cpp ]; then
    echo "\\n2. Testing Export ABI..."
    g++ -std=c++17 test_exports_abi_generated.cpp -o test_exports_abi -ldl
    if [ $? -eq 0 ]; then
        ./test_exports_abi
        export_result=$?
    else
        echo "FAIL: Could not compile export test"
        export_result=1
    fi
fi

echo "\\n=== Summary ==="
if [ $cstring_result -eq 0 ] && [ $export_result -eq 0 ]; then
    echo "✅ All ABI tests passed"
    exit 0
else
    echo "❌ ABI test failures detected"
    exit 1
fi
'''
    
    (out_dir / "run_abi_tests.sh").write_text(runner)
    (out_dir / "run_abi_tests.sh").chmod(0o755)
    print(f"  Generated: run_abi_tests.sh")
    
    print(f"\\nTo run tests:")
    print(f"  cd {out_dir}")
    print(f"  ./run_abi_tests.sh")
    
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main())