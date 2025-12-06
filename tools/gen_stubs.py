#!/usr/bin/env python3
"""
Generate .def and stub C++ for OpenMFC exports.

Inputs:
  --db mfc_db.json (as produced by parse_exports.py)
Outputs:
  --out-def openmfc.def
  --out-stubs stubs.cpp

Generates ordinal-based exports matching MFC format.
"""

import argparse
import json
from pathlib import Path
from typing import Dict, List, Any


def emit_def(entries: List[Dict[str, Any]]) -> str:
    lines = ["LIBRARY openmfc", "EXPORTS"]
    for entry in entries:
        symbol = entry["symbol"]
        
        # Use ordinal from entry if available, otherwise fall back to array index
        if "ordinal" in entry and entry["ordinal"] is not None:
            mfc_ordinal = entry["ordinal"]
        else:
            # Fallback: use array index + 256 (WRONG but works for testing)
            idx = entries.index(entry) if entry in entries else 0
            mfc_ordinal = 256 + idx
        
        # Map to appropriate stub based on symbol
        if "AfxThrowMemoryException" in symbol:
            stub_name = "stub_AfxThrowMemoryException"
        elif "AfxThrowFileException" in symbol:
            stub_name = "stub_AfxThrowFileException"
        else:
            stub_name = "generic_stub"
        
        # Use INTERNALNAME in .def file: mangled_name=internal_name @ordinal
        lines.append(f"    {symbol}={stub_name} @{mfc_ordinal}")
    return "\n".join(lines) + "\n"


def emit_stubs(entries: List[Dict[str, Any]]) -> str:
    """Generate C++ stub implementations."""
    
    lines = [
        '#include <cstdio>',
        '#include <cstdlib>',
        '',
        '// MS_ABI: On MSVC, functions use MS ABI by default. On GCC/MinGW, we need the attribute.',
        '#if defined(_MSC_VER)',
        '  #define MS_ABI',
        '#elif defined(__GNUC__)',
        '  #define MS_ABI __attribute__((ms_abi))',
        '#else',
        '  #define MS_ABI',
        '#endif',
        '',
        '// Generic stub function',
        'extern "C" void MS_ABI generic_stub() {',
        '    std::fprintf(stderr, "Not Implemented\\n");',
        '}',
        '',
        '// Specialized stubs for exception functions',
        'extern "C" void MS_ABI stub_AfxThrowMemoryException() {',
        '    std::fprintf(stderr, "Not Implemented: AfxThrowMemoryException\\n");',
        '}',
        '',
        'extern "C" void MS_ABI stub_AfxThrowFileException(int cause, long lOsError, const wchar_t* lpszFileName) {',
        '    std::fprintf(stderr, "Not Implemented: AfxThrowFileException\\n");',
        '}',
        '',
        '// No #pragma directives needed - .def file handles export mapping',
    ]
    
    return "\n".join(lines)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--db", help="Path to mfc_db.json (legacy)")
    ap.add_argument("--mapping", help="Path to ordinal mapping JSON")
    ap.add_argument("--out-def", required=True)
    ap.add_argument("--out-stubs", required=True)
    args = ap.parse_args()

    entries: List[Dict[str, Any]] = []
    
    # Try to load from mapping file first
    if args.mapping:
        mapping_path = Path(args.mapping)
        if mapping_path.exists():
            mapping: Dict = json.loads(mapping_path.read_text())
            entries = mapping.get("exports", {}).get("mfc140u", [])
            print(f"Loaded {len(entries)} entries from mapping file")
    
    # Fall back to legacy database
    if not entries and args.db:
        db_path = Path(args.db)
        if db_path.exists():
            db: Dict = json.loads(db_path.read_text())
            for dll, dll_entries in db.get("exports", {}).items():
                if dll != "mfc140u":
                    continue
                entries.extend(dll_entries)
            print(f"Loaded {len(entries)} entries from legacy database")
    
    # If still no entries, create minimal test set
    if not entries:
        entries = [
            {"symbol": "?AfxThrowMemoryException@@YAXXZ", "ordinal": 256},
            {"symbol": "?AfxThrowFileException@@YAXHJPB_W@Z", "ordinal": 257},
            {"symbol": "?AfxGetApp@@YAPAVCWinApp@@XZ", "ordinal": 258},
            {"symbol": "?AfxGetMainWnd@@YAPAVCWnd@@XZ", "ordinal": 259},
            {"symbol": "?AfxWinInit@@YAHPAUHINSTANCE__@@0PA_WH@Z", "ordinal": 260},
        ]
        print("Created minimal test set (5 entries)")
    
    # Sort by ordinal if available
    if entries and "ordinal" in entries[0]:
        entries.sort(key=lambda x: x.get("ordinal", 0))
    
    Path(args.out_def).write_text(emit_def(entries), encoding="ascii")
    Path(args.out_stubs).write_text(emit_stubs(entries), encoding="ascii")
    
    # Show some statistics
    if entries:
        ordinals = [e.get("ordinal", 0) for e in entries]
        if ordinals:
            min_ord = min(ordinals)
            max_ord = max(ordinals)
            print(f"Generated {len(entries)} exports with ordinals {min_ord} to {max_ord}")
        else:
            print(f"Generated {len(entries)} exports (no ordinals in data)")
    
    # Warn if using guessed ordinals
    if entries and entries[0].get("source") == "guessed_array_index":
        print("\nWARNING: Using GUESSED ordinals!")
        print("   This is WRONG for ABI compatibility!")
        print("   Update mapping file with real ordinals from MFC .lib file")


if __name__ == "__main__":
    main()