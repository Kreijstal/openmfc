#!/usr/bin/env python3
"""
Create ordinal mapping file from symbols list.

Since we don't have real ordinal mapping from MFC .lib file,
create a mapping file with placeholder ordinals that can be
updated later with real data.

Usage:
  python3 create_ordinal_mapping.py --symbols artifacts/mfc_symbols.txt --out mfc_ordinal_mapping.json
"""

import argparse
import json
from pathlib import Path
from typing import Dict, List

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--symbols", required=True, help="Path to symbols list file")
    ap.add_argument("--out", required=True, help="Output JSON path")
    ap.add_argument("--base", type=int, default=256, help="Base ordinal (default: 256)")
    args = ap.parse_args()
    
    symbols_path = Path(args.symbols)
    if not symbols_path.exists():
        print(f"Error: {symbols_path} not found")
        return 1
    
    # Read symbols
    symbols = symbols_path.read_text().strip().split('\n')
    symbols = [s.strip() for s in symbols if s.strip()]
    print(f"Loaded {len(symbols)} symbols from {symbols_path}")
    
    # Create mapping with placeholder ordinals
    # WARNING: These are NOT the real ordinals!
    # Real ordinals must be extracted from MFC .lib file using dumpbin /ALL
    mapping = []
    for idx, symbol in enumerate(symbols):
        mapping.append({
            "symbol": symbol,
            "ordinal": args.base + idx,
            "note": "PLACEHOLDER - NOT REAL ORDINAL",
            "source": "guessed_array_index"
        })
    
    # Create database structure
    db = {
        "exports": {
            "mfc140u": mapping
        },
        "metadata": {
            "total_exports": len(mapping),
            "base_ordinal": args.base,
            "has_real_ordinals": False,
            "warning": "ORDINALS ARE GUESSED AND WRONG FOR ABI COMPATIBILITY",
            "note": "Real ordinals must be extracted from MFC .lib file using dumpbin /ALL on Windows",
            "next_steps": [
                "1. On Windows: dumpbin /ALL mfc140u.lib > mfc_lib_analysis.txt",
                "2. Parse output for symbol→ordinal references",
                "3. Update this file with real ordinals"
            ]
        }
    }
    
    # Write output
    output_path = Path(args.out)
    output_path.write_text(json.dumps(db, indent=2), encoding="ascii")
    
    print(f"\nWrote {len(mapping)} entries to {output_path}")
    print("\n⚠️  WARNING: Using GUESSED ordinals (array index + {args.base})")
    print("   This is WRONG for ABI compatibility!")
    print("\nTo get REAL ordinals:")
    print("1. Run on Windows: dumpbin /ALL mfc140u.lib > mfc_lib_analysis.txt")
    print("2. Parse output for symbol→ordinal references")
    print("3. Update this file with real ordinals")
    
    return 0

if __name__ == "__main__":
    import sys
    sys.exit(main())