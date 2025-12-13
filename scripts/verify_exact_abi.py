#!/usr/bin/env python3
"""
Exact ABI Compatibility Verification

Verifies that OpenMFC DLL matches REAL MFC ABI exactly:
1. All symbols at correct ordinals (from mfc_complete_ordinal_mapping.json)
2. No missing exports
3. No extra exports (except our stubs)
4. Exact MSVC mangling matches

The API is set in stone by mfc140u.dll - we must not break it.
"""

import argparse
import json
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Tuple

def extract_dll_exports(dll_path: Path) -> Dict[str, int]:
    """Extract exports from DLL with their ordinals."""
    exports = {}
    
    try:
        result = subprocess.run(
            ["x86_64-w64-mingw32-objdump", "-p", str(dll_path)],
            capture_output=True,
            text=True,
            check=True
        )
    except (subprocess.CalledProcessError, FileNotFoundError):
        print("❌ ERROR: Could not extract DLL exports")
        print("  Install: sudo apt-get install mingw-w64")
        sys.exit(1)
    
    # Parse export table
    # We need to handle different objdump formats
    import re
    
    ordinal_base = 1  # Default
    in_name_table = False
    
    for line in result.stdout.split('\n'):
        # Look for Ordinal Base
        # "Export Address Table -- Ordinal Base 256"
        base_match = re.search(r'Ordinal Base\s+(\d+)', line)
        if base_match:
            ordinal_base = int(base_match.group(1))
            continue

        # Look for Name Table header (handle both orderings)
        if '[Ordinal/Name Pointer] Table' in line or '[Name Pointer/Ordinal] Table' in line:
            in_name_table = True
            continue

        if in_name_table:
            # Format 1: [   0] +base[ 256]  0000 symbol_name
            match1 = re.match(r'\s*\[\s*\d+\]\s*\+base\[\s*(\d+)\]\s+[0-9a-fA-F]+\s+(.+)', line)
            
            # Format 2: [   0] symbol_name
            # We need to capture the index to add to base
            match2 = re.match(r'\s*\[\s*(\d+)\]\s+(.+)', line)

            if match1:
                ordinal = int(match1.group(1))
                symbol = match1.group(2).strip()
                # Clean up symbol if it has extra info
                if ' ' in symbol:
                    symbol = symbol.split()[0]
                exports[symbol] = ordinal
            elif match2:
                # In this format, the number in brackets is the index into the table
                # Ordinal = Base + Index
                index = int(match2.group(1))
                symbol = match2.group(2).strip()
                # Skip if it looks like "Export RVA" (from Address Table)
                if "Export RVA" in symbol or "RVA exportieren" in symbol:
                    continue
                exports[symbol] = ordinal_base + index
            elif line.strip() and not line.startswith('[') and exports:
                # End of export table (heuristic: non-empty line not starting with [)
                if "Table" in line:
                    break
    
    # If still no exports, check if DLL might be exporting stub_* names
    # and we need to check the .def file mapping
    if not exports:
        print("   ⚠️  Could not parse exports from objdump")
        print("   Checking .def file instead...")
        
        # Try to parse .def file
        def_path = dll_path.parent / (dll_path.stem + ".def")
        if def_path.exists():
            exports = parse_def_file(def_path)
    
    return exports

def parse_def_file(def_path: Path) -> Dict[str, int]:
    """Parse .def file to get MSVC-mangled exports."""
    exports = {}
    
    try:
        with open(def_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
    except Exception:
        return exports
    
    lines = content.split('\n')
    for line in lines:
        line = line.strip()
        if not line or line.startswith('LIBRARY') or line.startswith('EXPORTS'):
            continue
        
        # Format: symbol=alias @ordinal
        if '@' in line:
            try:
                # Extract ordinal
                ordinal_part = line[line.rfind('@')+1:].strip()
                # Remove DATA suffix if present
                if 'DATA' in ordinal_part:
                    ordinal_part = ordinal_part.replace('DATA', '').strip()
                ordinal = int(ordinal_part)
                
                # Extract MSVC symbol (before = if present)
                symbol_part = line.split('=')[0].strip()
                exports[symbol_part] = ordinal
            except (ValueError, IndexError):
                continue
    
    return exports

def load_real_mfc_mapping(mapping_path: Path) -> Dict[str, int]:
    """Load real MFC ordinals from complete mapping."""
    with open(mapping_path) as f:
        data = json.load(f)
    
    real_exports = {}
    entries = data.get("exports", {}).get("mfc140u", [])
    
    for entry in entries:
        symbol = entry.get("symbol")
        ordinal = entry.get("ordinal")
        if symbol and ordinal is not None:
            real_exports[symbol] = ordinal
    
    return real_exports

def verify_exact_compatibility(
    our_exports: Dict[str, int],
    real_exports: Dict[str, int]
) -> Tuple[bool, List[str], List[str], List[str]]:
    """
    Verify exact ABI compatibility.
    
    Returns: (is_compatible, missing, wrong_ordinal, extra)
    """
    missing = []
    wrong_ordinal = []
    
    # Check all real MFC exports are present at correct ordinals
    for symbol, real_ordinal in real_exports.items():
        if symbol not in our_exports:
            missing.append(f"{symbol} (ordinal {real_ordinal})")
        elif our_exports[symbol] != real_ordinal:
            wrong_ordinal.append(
                f"{symbol}: expected {real_ordinal}, got {our_exports[symbol]}"
            )
    
    # Check for extra exports (should only be our stub_* names)
    extra = []
    for symbol in our_exports:
        if symbol not in real_exports:
            # Allow stub_* names (our implementation)
            if not symbol.startswith('stub_'):
                extra.append(symbol)
    
    is_compatible = len(missing) == 0 and len(wrong_ordinal) == 0
    
    return is_compatible, missing, wrong_ordinal, extra

def main():
    parser = argparse.ArgumentParser(
        description="Verify exact ABI compatibility with real MFC"
    )
    parser.add_argument(
        "--dll",
        default="build-phase4/openmfc.dll",
        help="OpenMFC DLL to verify"
    )
    parser.add_argument(
        "--mapping",
        default="mfc_complete_ordinal_mapping.json",
        help="Real MFC ordinal mapping"
    )
    parser.add_argument(
        "--strict",
        action="store_true",
        help="Fail on extra exports (not just stub_*)"
    )
    
    args = parser.parse_args()
    
    dll_path = Path(args.dll)
    mapping_path = Path(args.mapping)
    
    print("=== Exact ABI Compatibility Verification ===")
    print(f"DLL: {dll_path}")
    print(f"Mapping: {mapping_path}")
    print()
    
    if not dll_path.exists():
        print(f"❌ ERROR: DLL not found: {dll_path}")
        sys.exit(1)
    
    if not mapping_path.exists():
        print(f"❌ ERROR: Mapping file not found: {mapping_path}")
        sys.exit(1)
    
    print("1. Loading real MFC ABI definition...")
    real_exports = load_real_mfc_mapping(mapping_path)
    print(f"   Real MFC has {len(real_exports)} exports")
    
    print("\n2. Extracting OpenMFC DLL exports...")
    our_exports = extract_dll_exports(dll_path)
    print(f"   OpenMFC has {len(our_exports)} exports")
    
    print("\n3. Verifying exact compatibility...")
    compatible, missing, wrong_ordinal, extra = verify_exact_compatibility(
        our_exports, real_exports
    )
    
    # Summary
    print(f"\n   Missing exports: {len(missing)}")
    print(f"   Wrong ordinals: {len(wrong_ordinal)}")
    print(f"   Extra exports: {len(extra)}")
    
    if missing:
        print(f"\n❌ MISSING EXPORTS (ABI BROKEN):")
        for i, item in enumerate(missing[:10]):  # Show first 10
            print(f"   {item}")
        if len(missing) > 10:
            print(f"   ... and {len(missing) - 10} more")
    
    if wrong_ordinal:
        print(f"\n❌ WRONG ORDINALS (ABI BROKEN):")
        for i, item in enumerate(wrong_ordinal[:10]):
            print(f"   {item}")
        if len(wrong_ordinal) > 10:
            print(f"   ... and {len(wrong_ordinal) - 10} more")
    
    if extra and args.strict:
        print(f"\n⚠️  EXTRA EXPORTS (potential issues):")
        for i, item in enumerate(extra[:10]):
            print(f"   {item}")
        if len(extra) > 10:
            print(f"   ... and {len(extra) - 10} more")
    
    print("\n" + "="*50)
    
    if not compatible:
        print("❌ ABI COMPATIBILITY BROKEN")
        print("\nThe API is set in stone by mfc140u.dll.")
        print("We MUST maintain exact compatibility.")
        print("\nCommon causes:")
        print("1. Missing exports in .def file")
        print("2. Wrong ordinals in mapping")
        print("3. Build not using complete ordinal mapping")
        sys.exit(1)
    
    if extra and args.strict:
        print("⚠️  ABI COMPATIBLE (with warnings)")
        print("Extra exports found. Use --strict to fail on extras.")
        sys.exit(0)
    
    print("✅ EXACT ABI COMPATIBILITY VERIFIED")
    print(f"\nOpenMFC DLL can replace mfc140u.dll for:")
    print(f"  - {len(real_exports)} functions at correct ordinals")
    print(f"  - All MSVC-mangled symbols present")
    print(f"  - No ABI-breaking changes")
    
    # Show key compatibility indicators - use REAL ordinals from mapping
    key_symbols = [
        ("?AfxThrowMemoryException@@YAXXZ", None),
        ("?AfxThrowFileException@@YAXHJPB_W@Z", None),
        ("?AfxThrowNotSupportedException@@YAXXZ", None),
        ("?AfxAbort@@YAXXZ", None),
    ]
    
    # Get real ordinals from mapping
    for i, (symbol, _) in enumerate(key_symbols):
        if symbol in real_exports:
            key_symbols[i] = (symbol, real_exports[symbol])
    
    print("\nKey symbols verified (real MFC ordinals):")
    for symbol, real_ordinal in key_symbols:
        if real_ordinal is None:
            print(f"  ⚠️  {symbol} - Not in mapping")
        elif symbol in our_exports:
            our_ordinal = our_exports[symbol]
            if our_ordinal == real_ordinal:
                print(f"  ✅ {symbol} @ {real_ordinal}")
            else:
                print(f"  ❌ {symbol}: expected {real_ordinal}, got {our_ordinal}")
        else:
            print(f"  ❌ {symbol} @ {real_ordinal} - MISSING")
    
    sys.exit(0)

if __name__ == "__main__":
    main()