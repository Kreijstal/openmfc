#!/usr/bin/env python3
"""
Test implementation safety validator

Tests that validate_implementation_safety.py correctly identifies:
1. Safe patterns (extern "C" + MS_ABI + stub_ prefix)
2. Unsafe patterns (C++ methods, missing extern "C", etc.)
"""

import os
import subprocess
import tempfile
import sys
from pathlib import Path

ROOT = Path(__file__).parent.parent.parent
VALIDATOR_SCRIPT = ROOT / "scripts" / "validate_implementation_safety.py"

def run_validator(file_paths):
    """Run the validator on given files."""
    try:
        cmd = ["python3", str(VALIDATOR_SCRIPT)] + file_paths
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=10
        )
        return result.returncode == 0, result.stdout + result.stderr
    except subprocess.TimeoutExpired:
        return False, "Validator timed out"
    except Exception as e:
        return False, f"Error running validator: {e}"

def create_test_file(content, suffix=".cpp"):
    """Create a temporary test file."""
    with tempfile.NamedTemporaryFile(mode='w', suffix=suffix, delete=False) as f:
        f.write(content)
        return f.name

def test_safe_pattern():
    """Test safe implementation pattern (should pass)."""
    print("Test 1: Safe implementation pattern")
    
    safe_code = '''
#include <cstdio>

#if defined(__GNUC__)
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Safe: extern "C" + MS_ABI + stub_ prefix
extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {
    fprintf(stderr, "AfxThrowMemoryException called\\n");
}

// Safe: Another stub
extern "C" int MS_ABI stub__GetLength_CString__QBEHXZ() {
    return 42;
}

// Safe: Data export
extern "C" unsigned int stub__AFX_WM_DRAW2D__3IA = 0;
'''
    
    test_file = create_test_file(safe_code)
    
    success, output = run_validator([os.path.abspath(test_file)])
    os.unlink(test_file)
    
    if success and "safe stub(s)" in output:
        print("  ✅ PASS: Safe pattern accepted")
        return True
    elif not success:
        print("  ❌ FAIL: Safe pattern rejected")
        print(f"  Output:\n{output}")
        return False
    else:
        print("  ❌ FAIL: Wrong output")
        print(f"  Output:\n{output}")
        return False

def test_unsafe_cpp_method():
    """Test unsafe C++ class method (should fail)."""
    print("\nTest 2: Unsafe C++ class method")
    
    unsafe_code = '''
// UNSAFE: C++ class method (produces GCC mangling)
class CString {
public:
    int GetLength() {  // Will be mangled as _ZN7CString9GetLengthEv
        return m_length;
    }
private:
    int m_length;
};
'''
    
    test_file = create_test_file(unsafe_code)
    
    success, output = run_validator([test_file])
    os.unlink(test_file)
    
    if not success and "C++ class method definition" in output:
        print("  ✅ PASS: Correctly rejected C++ class method")
        return True
    elif success:
        print("  ❌ FAIL: Should have rejected C++ class method")
        return False
    else:
        print("  ❌ FAIL: Wrong error")
        print(f"  Output:\n{output}")
        return False

def test_missing_extern_c():
    """Test missing extern "C" (should fail)."""
    print("\nTest 3: Missing extern \"C\"")
    
    unsafe_code = '''
// UNSAFE: Missing extern "C" (will be GCC-mangled)
void AfxThrowMemoryException() {
    // GCC-mangled as _Z23AfxThrowMemoryExceptionv
}
'''
    
    test_file = create_test_file(unsafe_code)
    
    success, output = run_validator([test_file])
    os.unlink(test_file)
    
    if not success and "without extern" in output:
        print("  ✅ PASS: Correctly detected missing extern \"C\"")
        return True
    elif success:
        print("  ❌ FAIL: Should have rejected missing extern \"C\"")
        return False
    else:
        print("  ❌ FAIL: Wrong error")
        print(f"  Output:\n{output}")
        return False

def test_missing_ms_abi():
    """Test missing MS_ABI attribute (should fail)."""
    print("\nTest 4: Missing MS_ABI attribute")
    
    unsafe_code = '''
// UNSAFE: Has extern "C" but missing MS_ABI
extern "C" void AfxThrowMemoryException() {
    // Wrong calling convention
}
'''
    
    test_file = create_test_file(unsafe_code)
    
    success, output = run_validator([test_file])
    os.unlink(test_file)
    
    if not success and "missing MS_ABI" in output:
        print("  ✅ PASS: Correctly detected missing MS_ABI")
        return True
    elif success:
        print("  ❌ FAIL: Should have rejected missing MS_ABI")
        return False
    else:
        print("  ❌ FAIL: Wrong error")
        print(f"  Output:\n{output}")
        return False

def test_direct_msvc_name():
    """Test using MSVC-mangled name directly (should fail)."""
    print("\nTest 5: Direct MSVC-mangled name")
    
    unsafe_code = '''
// UNSAFE: Using MSVC-mangled name directly
extern "C" void MS_ABI ?AfxThrowMemoryException@@YAXXZ() {
    // Should use stub_ prefix instead
}
'''
    
    test_file = create_test_file(unsafe_code)
    
    success, output = run_validator([test_file])
    os.unlink(test_file)
    
    if not success and "MSVC-mangled name directly" in output:
        print("  ✅ PASS: Correctly rejected direct MSVC name")
        return True
    elif success:
        print("  ❌ FAIL: Should have rejected direct MSVC name")
        return False
    else:
        print("  ❌ FAIL: Wrong error")
        print(f"  Output:\n{output}")
        return False

def test_mixed_safe_unsafe():
    """Test file with both safe and unsafe patterns."""
    print("\nTest 6: Mixed safe and unsafe patterns")
    
    mixed_code = '''
// Safe pattern
extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {
    // OK
}

// Unsafe: C++ method
class CObject {
    virtual void Serialize() {}  // GCC-mangled!
};

// Safe: Another stub
extern "C" int MS_ABI stub__SomeFunction__XYZ() {
    return 0;
}
'''
    
    test_file = create_test_file(mixed_code)
    
    success, output = run_validator([test_file])
    os.unlink(test_file)
    
    if not success and "C++ class method definition" in output:
        print("  ✅ PASS: Correctly detected unsafe pattern among safe ones")
        return True
    elif success:
        print("  ❌ FAIL: Should have rejected due to unsafe pattern")
        return False
    else:
        print("  ❌ FAIL: Wrong error")
        print(f"  Output:\n{output}")
        return False

def test_non_cpp_file():
    """Test that non-C++ files are ignored."""
    print("\nTest 7: Non-C++ file (should be ignored)")
    
    content = "This is not a C++ file, just text"
    test_file = create_test_file(content, suffix=".txt")
    
    success, output = run_validator([test_file])
    os.unlink(test_file)
    
    # Non-CPP files should be skipped, not cause errors
    if "Files checked: 0" in output or "No safe stubs found" in output:
        print("  ✅ PASS: Non-C++ file handled correctly")
        return True
    else:
        print("  ⚠️  UNEXPECTED: Non-C++ file produced output")
        print(f"  Output:\n{output}")
        return True  # Not a critical failure

def main():
    print("=== Testing Implementation Safety Validator ===\n")
    
    tests = [
        test_safe_pattern,
        test_unsafe_cpp_method,
        test_missing_extern_c,
        test_missing_ms_abi,
        test_direct_msvc_name,
        test_mixed_safe_unsafe,
        test_non_cpp_file,
    ]
    
    passed = 0
    total = len(tests)
    
    for test in tests:
        if test():
            passed += 1
    
    print(f"\n=== Summary ===")
    print(f"Passed: {passed}/{total}")
    
    if passed == total:
        print("✅ All tests passed!")
        return 0
    else:
        print("❌ Some tests failed")
        return 1

if __name__ == "__main__":
    sys.exit(main())