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
        ordinal = entry.get("ordinal", "0")
        
        # Handle both string and integer ordinals
        try:
            if isinstance(ordinal, str):
                mfc_ordinal = int(ordinal)
            else:
                mfc_ordinal = int(ordinal)
        except (ValueError, TypeError):
            # Fallback: use array index + 256
            idx = entries.index(entry)
            mfc_ordinal = 256 + idx
            print(f"WARNING: Invalid ordinal for {symbol}, using {mfc_ordinal}")
        
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
    lines = [
        "#include <cstdio>",
        "#include <cstdlib>",
        "",
        "// MS_ABI: On MSVC, functions use MS ABI by default. On GCC/MinGW, we need the attribute.",
        "#if defined(_MSC_VER)",
        "  #define MS_ABI",
        "#elif defined(__GNUC__)",
        "  #define MS_ABI __attribute__((ms_abi))",
        "#else",
        "  #define MS_ABI",
        "#endif",
        "",
        "// Forward declarations for exception helpers (C++ linkage)",
        "void AfxThrowMemoryException();",
        "void AfxThrowFileException(int cause, long lOsError, const wchar_t* lpszFileName);",
        "",
        "// Generic stub function",
        "extern \"C\" void MS_ABI generic_stub() {",
        "    std::fprintf(stderr, \"Not Implemented\\n\");",
        "}",
        "",
        "// Specialized stubs for known functions",
        "extern \"C\" void MS_ABI stub_AfxThrowMemoryException() {",
        "    AfxThrowMemoryException();",
        "}",
        "",
        "extern \"C\" void MS_ABI stub_AfxThrowFileException(int cause, long lOsError, const wchar_t* lpszFileName) {",
        "    AfxThrowFileException(cause, lOsError, lpszFileName);",
        "}",
        "",
        "// No #pragma directives needed - .def file handles export mapping",
        "",
    ]
    
    return "\n".join(lines)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--db", required=True)
    ap.add_argument("--out-def", required=True)
    ap.add_argument("--out-stubs", required=True)
    args = ap.parse_args()

    # Load database
    db: Dict = json.loads(Path(args.db).read_text())
    
    # Get MFC entries
    entries: List[Dict[str, Any]] = []
    for dll, dll_entries in db.get("exports", {}).items():
        if dll != "mfc140u":
            continue
        entries.extend(dll_entries)
    
    # If no entries, create minimal test set
    if not entries:
        entries = [
            {"symbol": "?AfxThrowMemoryException@@YAXXZ", "ordinal": 0},
            {"symbol": "?AfxThrowFileException@@YAXHJPB_W@Z", "ordinal": 1},
            {"symbol": "?AfxGetApp@@YAPAVCWinApp@@XZ", "ordinal": 2},
            {"symbol": "?AfxGetMainWnd@@YAPAVCWnd@@XZ", "ordinal": 3},
            {"symbol": "?AfxWinInit@@YAHPAUHINSTANCE__@@0PA_WH@Z", "ordinal": 4},
        ]
    
    Path(args.out_def).write_text(emit_def(entries), encoding="ascii")
    Path(args.out_stubs).write_text(emit_stubs(entries), encoding="ascii")
    
    print(f"Generated {len(entries)} exports starting at ordinal 256")


if __name__ == "__main__":
    main()