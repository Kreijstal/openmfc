#!/usr/bin/env python3
"""
Test core MSVC ABI safety logic

Tests the fundamental logic without external dependencies.
"""

import re
import sys

def check_exports_for_msvc_mangling(exports_text):
    """
    Core logic from test_msvc_abi_simple.sh
    Returns (is_valid, message)
    """
    lines = exports_text.strip().split('\n')
    exports = []
    
    # Parse exports (simplified)
    for line in lines:
        line = line.strip()
        if not line:
            continue
        # Assume each line is an export name
        exports.append(line)
    
    total = len(exports)
    msvc_count = sum(1 for e in exports if e.startswith('?'))
    gcc_count = sum(1 for e in exports if e.startswith('_Z'))
    
    # Check 1: Must have enough MSVC-mangled names
    if msvc_count < 100:
        return False, f"Not enough MSVC-mangled exports (need 100+, got {msvc_count})"
    
    # Check 2: Must NOT have GCC-mangled names
    if gcc_count > 0:
        return False, f"Found {gcc_count} GCC-mangled exports"
    
    # Check 3: Key MSVC symbols must be present
    key_symbols = [
        "?AfxThrowMemoryException@@YAXXZ",
        "?AfxThrowFileException@@YAXHJPB_W@Z",
        "?AfxThrowNotSupportedException@@YAXXZ",
        "?AfxThrowInvalidArgException@@YAXXZ",
        "?AfxAbort@@YAXXZ",
    ]
    
    missing = []
    for symbol in key_symbols:
        if not any(symbol in e for e in exports):
            missing.append(symbol)
    
    if missing:
        return False, f"Missing key symbols: {', '.join(missing)}"
    
    return True, f"OK: {msvc_count} MSVC, {gcc_count} GCC, all keys present"

def test_msvc_only():
    """Test with only MSVC-mangled exports."""
    print("Test 1: Only MSVC-mangled exports")
    
    exports = ["?AfxThrowMemoryException@@YAXXZ"]
    # Add more to reach threshold
    for i in range(200):
        exports.append(f"?SomeFunction{i}@@YAXXZ")
    
    exports.append("?AfxThrowFileException@@YAXHJPB_W@Z")
    exports.append("?AfxThrowNotSupportedException@@YAXXZ")
    exports.append("?AfxThrowInvalidArgException@@YAXXZ")
    exports.append("?AfxAbort@@YAXXZ")
    
    valid, msg = check_exports_for_msvc_mangling('\n'.join(exports))
    
    if valid:
        print("  ✅ PASS: MSVC-only exports accepted")
        return True
    else:
        print(f"  ❌ FAIL: {msg}")
        return False

def test_gcc_mangling():
    """Test with GCC-mangled exports."""
    print("\nTest 2: GCC-mangled exports (should fail)")
    
    exports = ["?AfxThrowMemoryException@@YAXXZ", "_ZN7CObject15GetRuntimeClassEv"]
    for i in range(200):
        exports.append(f"?SomeFunction{i}@@YAXXZ")
    
    valid, msg = check_exports_for_msvc_mangling('\n'.join(exports))
    
    if not valid and "GCC-mangled" in msg:
        print("  ✅ PASS: Correctly rejected GCC mangling")
        return True
    elif valid:
        print("  ❌ FAIL: Should have rejected GCC mangling")
        return False
    else:
        print(f"  ❌ FAIL: Wrong error: {msg}")
        return False

def test_missing_key_symbols():
    """Test with missing key symbols."""
    print("\nTest 3: Missing key symbols (should fail)")
    
    exports = []
    for i in range(200):
        exports.append(f"?SomeFunction{i}@@YAXXZ")
    # Don't add the required key symbols
    
    valid, msg = check_exports_for_msvc_mangling('\n'.join(exports))
    
    if not valid and "Missing key symbols" in msg:
        print("  ✅ PASS: Correctly detected missing key symbols")
        return True
    elif valid:
        print("  ❌ FAIL: Should have detected missing symbols")
        return False
    else:
        print(f"  ❌ FAIL: Wrong error: {msg}")
        return False

def test_insufficient_msvc():
    """Test with insufficient MSVC exports."""
    print("\nTest 4: Insufficient MSVC exports (should fail)")
    
    exports = ["?AfxThrowMemoryException@@YAXXZ", "?AfxThrowFileException@@YAXHJPB_W@Z"]
    # Only 2 MSVC exports
    
    valid, msg = check_exports_for_msvc_mangling('\n'.join(exports))
    
    if not valid and "Not enough MSVC-mangled exports" in msg:
        print("  ✅ PASS: Correctly detected insufficient exports")
        return True
    elif valid:
        print("  ❌ FAIL: Should have rejected insufficient exports")
        return False
    else:
        print(f"  ❌ FAIL: Wrong error: {msg}")
        return False

def test_implementation_safety_logic():
    """Test implementation safety detection logic."""
    print("\nTest 5: Implementation safety patterns")
    
    # Safe patterns
    safe_patterns = [
        'extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {',
        'extern "C" int MS_ABI stub__GetLength_CString__QBEHXZ() {',
    ]
    
    # Unsafe patterns
    unsafe_patterns = [
        'int CString::GetLength() {',
        'void AfxThrowMemoryException() {',
        'extern "C" void AfxThrowMemoryException() {',  # Missing MS_ABI
        'extern "C" void MS_ABI ?AfxThrowMemoryException@@YAXXZ() {',  # Direct MSVC name
    ]
    
    safe_re = re.compile(r'^\s*extern\s+"C".*MS_ABI\s+stub_')
    unsafe_re = re.compile(r'^\s*(?:virtual\s+)?\w+\s+\w+::\w+\s*\(')
    
    safe_count = 0
    for pattern in safe_patterns:
        if safe_re.search(pattern):
            safe_count += 1
    
    unsafe_detected = 0
    for pattern in unsafe_patterns:
        if unsafe_re.search(pattern):
            unsafe_detected += 1
    
    if safe_count == len(safe_patterns) and unsafe_detected > 0:
        print("  ✅ PASS: Pattern detection works")
        return True
    else:
        print(f"  ❌ FAIL: Safe: {safe_count}/{len(safe_patterns)}, Unsafe: {unsafe_detected}")
        return False

def main():
    print("=== Testing Core MSVC ABI Safety Logic ===\n")
    
    tests = [
        test_msvc_only,
        test_gcc_mangling,
        test_missing_key_symbols,
        test_insufficient_msvc,
        test_implementation_safety_logic,
    ]
    
    passed = 0
    total = len(tests)
    
    for test in tests:
        if test():
            passed += 1
    
    print(f"\n=== Summary ===")
    print(f"Passed: {passed}/{total}")
    
    if passed == total:
        print("\n✅ All core logic tests passed!")
        print("\n📋 Core safety logic validates:")
        print("   1. MSVC vs GCC mangling detection")
        print("   2. Key symbol presence checking")
        print("   3. Implementation pattern validation")
        return 0
    else:
        print("\n❌ Some core logic tests failed")
        return 1

if __name__ == "__main__":
    sys.exit(main())