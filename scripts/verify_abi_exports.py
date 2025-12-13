#!/usr/bin/env python3
"""
ABI Export Verification Script

Verifies that a built DLL exports all required symbols from the ordinal mapping.
This is a regression test to ensure we don't break ABI compatibility.

Usage:
    python3 scripts/verify_abi_exports.py \
        --mapping mfc_complete_ordinal_mapping.json \
        --dll build-phase3/openmfc.dll

Exit codes:
    0 - All required symbols present (may have extras - that's OK)
    1 - Missing required symbols (ABI broken!)
    2 - Error (file not found, parse error, etc.)
"""

import argparse
import json
import subprocess
import sys
import re
from pathlib import Path
from typing import Dict, List, Set, Tuple


def extract_dll_exports(dll_path: Path) -> Dict[str, int]:
    """
    Extract exports from DLL using objdump.
    Returns dict of {symbol_name: ordinal}
    """
    exports = {}

    # Try MinGW objdump first
    objdump = "x86_64-w64-mingw32-objdump"
    try:
        result = subprocess.run(
            [objdump, "-x", str(dll_path)],
            capture_output=True,
            text=True,
            check=True
        )
    except FileNotFoundError:
        # Fall back to regular objdump
        objdump = "objdump"
        result = subprocess.run(
            [objdump, "-x", str(dll_path)],
            capture_output=True,
            text=True,
            check=True
        )

    # Parse the export table
    # We need to handle different objdump formats
    # Format 1 (some versions):
    #   [Ordinal/Name Pointer] Table
    #   [   0] +base[ 256]  0000 symbol_name
    # Format 2 (MinGW/German locale?):
    #   Export Address Table -- Ordinal Base 256
    #   ...
    #   [Name Pointer/Ordinal] Table
    #   [   0] symbol_name

    ordinal_base = 1  # Default if not found (though usually 1 or 256)
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
                # But be careful, some headers might appear
                if "Table" in line:
                    break
                # break # Don't break too eagerly, might be empty lines


    return exports


def load_required_exports(mapping_path: Path) -> Dict[str, int]:
    """
    Load required exports from ordinal mapping JSON.
    Returns dict of {symbol_name: ordinal}
    """
    with open(mapping_path, 'r') as f:
        data = json.load(f)

    required = {}
    entries = data.get("exports", {}).get("mfc140u", [])
    for entry in entries:
        symbol = entry.get("symbol")
        ordinal = entry.get("ordinal")
        if symbol and ordinal is not None:
            required[symbol] = ordinal

    return required


def verify_exports(
    required: Dict[str, int],
    actual: Dict[str, int]
) -> Tuple[List[str], List[Tuple[str, int, int]], List[str]]:
    """
    Compare required vs actual exports.

    Returns:
        - missing: symbols in required but not in actual
        - wrong_ordinal: symbols with mismatched ordinals [(symbol, expected, actual)]
        - extra: symbols in actual but not in required (informational only)
    """
    missing = []
    wrong_ordinal = []
    extra = []

    for symbol, expected_ord in required.items():
        if symbol not in actual:
            missing.append(symbol)
        elif actual[symbol] != expected_ord:
            wrong_ordinal.append((symbol, expected_ord, actual[symbol]))

    for symbol in actual:
        if symbol not in required:
            extra.append(symbol)

    return missing, wrong_ordinal, extra


def main():
    parser = argparse.ArgumentParser(
        description="Verify DLL exports match required ABI"
    )
    parser.add_argument(
        "--mapping", "-m",
        required=True,
        help="Path to ordinal mapping JSON"
    )
    parser.add_argument(
        "--dll", "-d",
        required=True,
        help="Path to DLL to verify"
    )
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Show detailed output"
    )
    parser.add_argument(
        "--show-extra",
        action="store_true",
        help="Show extra symbols (not in mapping)"
    )
    parser.add_argument(
        "--max-missing",
        type=int,
        default=0,
        help="Maximum allowed missing symbols (default: 0)"
    )
    args = parser.parse_args()

    mapping_path = Path(args.mapping)
    dll_path = Path(args.dll)

    # Validate inputs
    if not mapping_path.exists():
        print(f"ERROR: Mapping file not found: {mapping_path}", file=sys.stderr)
        return 2

    if not dll_path.exists():
        print(f"ERROR: DLL not found: {dll_path}", file=sys.stderr)
        return 2

    # Load data
    print(f"Loading required exports from: {mapping_path}")
    try:
        required = load_required_exports(mapping_path)
    except Exception as e:
        print(f"ERROR: Failed to load mapping: {e}", file=sys.stderr)
        return 2

    print(f"Extracting exports from: {dll_path}")
    try:
        actual = extract_dll_exports(dll_path)
    except Exception as e:
        print(f"ERROR: Failed to extract DLL exports: {e}", file=sys.stderr)
        return 2

    # Compare
    missing, wrong_ordinal, extra = verify_exports(required, actual)

    # Report
    print()
    print("=" * 60)
    print("ABI Export Verification Report")
    print("=" * 60)
    print(f"Required symbols:  {len(required)}")
    print(f"Actual exports:    {len(actual)}")
    print(f"Missing symbols:   {len(missing)}")
    print(f"Wrong ordinals:    {len(wrong_ordinal)}")
    print(f"Extra symbols:     {len(extra)}")
    print("=" * 60)

    # Show missing symbols
    if missing:
        print()
        print("MISSING SYMBOLS (ABI BREAK!):")
        for sym in sorted(missing)[:50]:  # Show first 50
            expected_ord = required[sym]
            print(f"  - {sym} @{expected_ord}")
        if len(missing) > 50:
            print(f"  ... and {len(missing) - 50} more")

    # Show wrong ordinals
    if wrong_ordinal:
        print()
        print("WRONG ORDINALS (ABI BREAK!):")
        for sym, expected, actual_ord in sorted(wrong_ordinal)[:20]:
            print(f"  - {sym}: expected @{expected}, got @{actual_ord}")
        if len(wrong_ordinal) > 20:
            print(f"  ... and {len(wrong_ordinal) - 20} more")

    # Show extra symbols (if requested)
    if args.show_extra and extra:
        print()
        print("EXTRA SYMBOLS (OK - informational only):")
        for sym in sorted(extra)[:20]:
            print(f"  + {sym}")
        if len(extra) > 20:
            print(f"  ... and {len(extra) - 20} more")

    # Determine result
    total_errors = len(missing) + len(wrong_ordinal)

    print()
    if total_errors == 0:
        print("✅ ABI VERIFICATION PASSED")
        print(f"   All {len(required)} required symbols present with correct ordinals")
        if extra:
            print(f"   ({len(extra)} extra symbols - allowed)")
        return 0
    elif total_errors <= args.max_missing:
        print(f"⚠️  ABI VERIFICATION PASSED (with {total_errors} allowed errors)")
        return 0
    else:
        print("❌ ABI VERIFICATION FAILED")
        print(f"   {len(missing)} missing symbols")
        print(f"   {len(wrong_ordinal)} wrong ordinals")
        return 1


if __name__ == "__main__":
    sys.exit(main())
