#!/usr/bin/env python3
"""
Implementation Safety Validator

Scans implementation files for unsafe patterns that would break MSVC ABI.
This catches problems BEFORE they get into the DLL.

Unsafe patterns:
1. C++ class method definitions (produces GCC mangling)
2. Missing extern "C"
3. Missing MS_ABI attribute
4. Wrong stub naming convention

Safe patterns:
1. extern "C" void MS_ABI stub__Name__Mangled() { ... }
2. extern "C" int variable = 0;  // data exports
"""

import argparse
import re
import sys
from pathlib import Path
from typing import List, Tuple, Dict

# Unsafe patterns to reject
UNSAFE_PATTERNS = [
    # C++ class method definitions (will be GCC-mangled)
    (r'^\s*(?:virtual\s+)?(?:inline\s+)?(?:const\s+)?\w+\s+\w+::\w+\s*\([^)]*\)\s*(?:const\s*)?[{;]', 
     "C++ class method definition (produces GCC mangling)"),
    
    # Missing extern "C"
    (r'^\s*(?!(?:extern\s+"C"\s+))(?!(?:#))(?!(?:/\*))(?!(?://))\w+\s+\w+\s*\([^)]*\)\s*[{;]',
     "Function definition without extern \"C\" (will be GCC-mangled)"),
    
    # Wrong calling convention (missing MS_ABI)
    (r'^\s*extern\s+"C"\s+(?!.*MS_ABI).*\w+\s+\w+\s*\([^)]*\)\s*[{;]',
     "extern \"C\" function missing MS_ABI attribute"),
    
    # Direct MSVC-mangled name as function name (should use stub_ prefix)
    (r'^\s*extern\s+"C".*\?\w+@@',
     "Using MSVC-mangled name directly (should use stub_ prefix)"),
]

# Safe pattern (what we want to see)
SAFE_PATTERN = r'^\s*extern\s+"C".*MS_ABI\s+stub_'

def check_file(filepath: Path) -> Tuple[bool, List[str], int]:
    """Check a single file for unsafe patterns."""
    errors = []
    
    try:
        content = filepath.read_text(encoding='utf-8', errors='ignore')
    except Exception as e:
        return False, [f"Could not read file: {e}"], 0
    
    lines = content.split('\n')
    
    for i, line in enumerate(lines, 1):
        # Skip comments and preprocessor directives
        stripped = line.strip()
        if stripped.startswith('//') or stripped.startswith('/*') or stripped.startswith('#'):
            continue
            
        for pattern, message in UNSAFE_PATTERNS:
            if re.search(pattern, line):
                errors.append(f"Line {i}: {message}")
                errors.append(f"  {line.strip()}")
                break
    
    # Also check that we have at least SOME safe patterns
    safe_count = len(re.findall(SAFE_PATTERN, content, re.MULTILINE))
    
    return len(errors) == 0, errors, safe_count

def main():
    parser = argparse.ArgumentParser(description="Validate implementation files for MSVC ABI safety")
    parser.add_argument("files", nargs="+", help="Implementation files to check")
    parser.add_argument("--strict", action="store_true", help="Fail on warnings")
    args = parser.parse_args()
    
    all_errors = []
    total_safe = 0
    total_files = 0
    
    print("=== Implementation Safety Validation ===\n")
    
    for file_pattern in args.files:
        for filepath in Path('.').glob(file_pattern):
            if not filepath.is_file():
                continue
                
            if filepath.suffix not in ['.cpp', '.c', '.cc', '.cxx']:
                continue
                
            total_files += 1
            is_safe, errors, safe_count = check_file(filepath)
            
            if safe_count > 0:
                print(f"✅ {filepath}: {safe_count} safe stub(s)")
                total_safe += safe_count
            else:
                print(f"⚠️  {filepath}: No safe stubs found")
                
            if errors:
                print(f"  ❌ Found {len(errors)//2} unsafe pattern(s):")
                for error in errors:
                    print(f"    {error}")
                all_errors.extend([f"{filepath}: {e}" for e in errors if "Line" in e])
    
    print(f"\n=== Summary ===")
    print(f"Files checked: {total_files}")
    print(f"Total safe stubs: {total_safe}")
    
    if all_errors:
        print(f"\n❌ Found {len(all_errors)} unsafe pattern(s):")
        for error in all_errors[:10]:  # Show first 10 errors
            print(f"  {error}")
        if len(all_errors) > 10:
            print(f"  ... and {len(all_errors) - 10} more")
        
        print("\n💡 How to fix:")
        print("1. Always use 'extern \"C\"' wrapper")
        print("2. Always use MS_ABI attribute")
        print("3. Use stub_ prefix naming (from gen_weak_stubs.py)")
        print("4. NEVER write C++ class methods in implementation files")
        print("\nExample safe implementation:")
        print('''  extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {
      // implementation here
  }''')
        
        sys.exit(1)
    else:
        print("\n✅ All implementation files are MSVC ABI safe!")
        print("   (No accidental GCC mangling detected)")
        sys.exit(0)

if __name__ == "__main__":
    main()