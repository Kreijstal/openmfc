#!/usr/bin/env python3
"""
Extract symbol→ordinal mappings from dumpbin /ALL output on .lib files.

The dumpbin /ALL output contains import thunks with ordinal references.
We parse these to create a mapping of mangled symbol names to ordinals.

Usage:
    python extract_ordinals.py --dumpbin mfc_lib_all.txt --out-json ordinal_map.json
"""

import argparse
import json
import re
from pathlib import Path
from typing import Dict, List, Tuple


def parse_dumpbin_all(content: str) -> List[Tuple[str, int]]:
    """
    Parse dumpbin /ALL output to extract symbol→ordinal mappings.

    The output contains entries like:
        Symbol name  : ?AfxGetApp@@YAPAVCWinApp@@XZ
        Type         : code
        Ordinal      : 2344

    Or in import thunk format:
        ?AfxGetApp@@YAPEAVCWinApp@@XZ (ordinal 2344)
    """
    mappings = []

    # Pattern 1: Multi-line format from dumpbin /ALL
    # Symbol name  : ?Something@@XXX
    # ...
    # Ordinal      : 1234
    symbol_pattern = re.compile(r'Symbol name\s*:\s*(\?[^\s]+)', re.MULTILINE)
    ordinal_pattern = re.compile(r'Ordinal\s*:\s*(\d+)', re.MULTILINE)

    # Pattern 2: Import thunk format
    # ?Symbol@@XXX (ordinal 1234)
    thunk_pattern = re.compile(r'(\?[^\s]+)\s*\(ordinal\s+(\d+)\)', re.MULTILINE | re.IGNORECASE)

    # Pattern 3: Archive member with import descriptor
    # __IMPORT_DESCRIPTOR_mfc140u or similar
    # Usually followed by ordinal info

    # Pattern 4: Direct ordinal reference in archive member listing
    # Looking for lines like:
    #   Ordinal  1234
    # near symbol definitions

    # Try pattern 2 first (most reliable)
    for match in thunk_pattern.finditer(content):
        symbol = match.group(1)
        ordinal = int(match.group(2))
        mappings.append((symbol, ordinal))

    if mappings:
        print(f"Found {len(mappings)} mappings using thunk pattern")
        return mappings

    # Try parsing section by section
    # Look for Archive member sections
    sections = content.split('Archive member name at')

    for section in sections[1:]:  # Skip first (before any archive member)
        # Look for symbol + ordinal pairs in each section

        # Try to find lines with ordinal info
        lines = section.split('\n')
        current_symbol = None

        for line in lines:
            # Check for symbol definition
            sym_match = re.search(r'\|\s+(\?[^\s|]+)', line)
            if sym_match:
                current_symbol = sym_match.group(1)
                # Remove __imp_ prefix if present
                if current_symbol.startswith('__imp_'):
                    current_symbol = current_symbol[6:]

            # Check for ordinal
            ord_match = re.search(r'ordinal\s*[:\s]+(\d+)', line, re.IGNORECASE)
            if ord_match and current_symbol:
                ordinal = int(ord_match.group(1))
                mappings.append((current_symbol, ordinal))
                current_symbol = None

    if mappings:
        print(f"Found {len(mappings)} mappings using section parsing")
        return mappings

    # Last resort: look for any symbol + ordinal on the same line
    simple_pattern = re.compile(r'(\?[A-Za-z0-9_@$?]+)\s+.*?ordinal\s+(\d+)', re.IGNORECASE)
    for match in simple_pattern.finditer(content):
        symbol = match.group(1)
        ordinal = int(match.group(2))
        mappings.append((symbol, ordinal))

    print(f"Found {len(mappings)} total mappings")
    return mappings


def main():
    parser = argparse.ArgumentParser(description='Extract ordinal mappings from dumpbin output')
    parser.add_argument('--dumpbin', required=True, help='Path to dumpbin /ALL output file')
    parser.add_argument('--out-symbols', help='Output file for corrected symbols list')
    parser.add_argument('--out-json', required=True, help='Output JSON file for ordinal mappings')
    args = parser.parse_args()

    dumpbin_path = Path(args.dumpbin)
    if not dumpbin_path.exists():
        print(f"Error: {dumpbin_path} not found")
        return 1

    print(f"Reading dumpbin output from {dumpbin_path}...")
    content = dumpbin_path.read_text(encoding='utf-8', errors='replace')
    print(f"Read {len(content)} characters")

    # Parse the content
    mappings = parse_dumpbin_all(content)

    # Deduplicate (keep first occurrence)
    seen = set()
    unique_mappings = []
    for symbol, ordinal in mappings:
        if symbol not in seen:
            seen.add(symbol)
            unique_mappings.append((symbol, ordinal))

    print(f"After deduplication: {len(unique_mappings)} unique symbols")

    # Sort by ordinal
    unique_mappings.sort(key=lambda x: x[1])

    # Create output structure
    exports = []
    for symbol, ordinal in unique_mappings:
        exports.append({
            "symbol": symbol,
            "ordinal": ordinal,
            "source": "dumpbin_all"
        })

    output = {
        "exports": {
            "mfc140u": exports
        },
        "metadata": {
            "total_exports": len(exports),
            "source": str(dumpbin_path),
            "note": "Extracted from dumpbin /ALL output"
        }
    }

    if exports:
        output["metadata"]["min_ordinal"] = exports[0]["ordinal"]
        output["metadata"]["max_ordinal"] = exports[-1]["ordinal"]

    # Write JSON output
    output_path = Path(args.out_json)
    output_path.write_text(json.dumps(output, indent=2), encoding='ascii')
    print(f"Wrote ordinal mappings to {output_path}")

    # Write symbols list if requested
    if args.out_symbols:
        symbols_path = Path(args.out_symbols)
        symbols_path.write_text('\n'.join(symbol for symbol, _ in unique_mappings), encoding='ascii')
        print(f"Wrote symbols list to {symbols_path}")

    # Show some statistics
    if exports:
        print(f"\nStatistics:")
        print(f"  Total symbols: {len(exports)}")
        print(f"  Ordinal range: {exports[0]['ordinal']} - {exports[-1]['ordinal']}")

        # Show first few
        print(f"\nFirst 5 mappings:")
        for entry in exports[:5]:
            print(f"  {entry['ordinal']:6d} -> {entry['symbol'][:60]}")

        # Show last few
        print(f"\nLast 5 mappings:")
        for entry in exports[-5:]:
            print(f"  {entry['ordinal']:6d} -> {entry['symbol'][:60]}")

    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main())
