#!/usr/bin/env python3
"""
Create a dummy DLL that imports MFC functions, then analyze its import table
to extract symbol→ordinal mapping.

Steps:
1. Generate C++ file that references MFC symbols
2. Compile with MSVC, linking with mfc140u.lib
3. Use dumpbin /IMPORTS to analyze imports
4. Extract ordinal mapping
"""

import argparse
import subprocess
import tempfile
import json
from pathlib import Path
from typing import List, Dict
import os


def generate_importer_cpp(symbols: List[str], output_path: Path):
    """Generate C++ file that references MFC symbols."""
    lines = [
        "#include <windows.h>",
        "",
        "// Declare MFC functions we want to import",
        "// These declarations force the linker to create import thunks",
        "",
    ]
    
    # Add declarations for each symbol
    for symbol in symbols[:100]:  # Limit to 100 for testing
        # Convert mangled name to function declaration
        # This is tricky because we don't know the signature
        # We'll use generic void function for now
        lines.append(f'extern "C" void {symbol}();')
    
    lines.extend([
        "",
        "BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {",
        "    return TRUE;",
        "}",
        "",
        "// Reference all symbols to force imports",
        "void reference_symbols() {",
    ])
    
    for symbol in symbols[:100]:
        lines.append(f"    {symbol}();")
    
    lines.extend([
        "}",
        ""
    ])
    
    output_path.write_text("\n".join(lines), encoding="ascii")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--symbols", required=True, help="Path to symbols list")
    ap.add_argument("--out", required=True, help="Output JSON path")
    args = ap.parse_args()
    
    # Load symbols
    symbols = []
    with open(args.symbols, 'r') as f:
        for line in f:
            sym = line.strip()
            if sym:
                symbols.append(sym)
    
    print(f"Loaded {len(symbols)} symbols")
    
    # Create temporary directory
    with tempfile.TemporaryDirectory() as tmpdir:
        tmpdir = Path(tmpdir)
        
        # Generate C++ file
        cpp_path = tmpdir / "importer.cpp"
        generate_importer_cpp(symbols[:50], cpp_path)  # Test with 50 symbols
        
        print(f"Generated {cpp_path}")
        
        # Note: This would need to run on Windows with MSVC
        # We can't compile it here on Linux
        print("")
        print("This script needs to run on Windows with MSVC installed.")
        print("Steps to extract real ordinals:")
        print("1. Compile importer.cpp with: cl /LD importer.cpp mfc140u.lib")
        print("2. Analyze with: dumpbin /IMPORTS importer.dll")
        print("3. Parse output to get symbol→ordinal mapping")
    
    # Create placeholder output
    db = {
        "exports": {
            "mfc140u": []
        },
        "metadata": {
            "note": "Need to run on Windows with MSVC to extract real ordinals",
            "symbol_count": len(symbols)
        }
    }
    
    Path(args.out).write_text(json.dumps(db, indent=2), encoding="ascii")
    print(f"\nWrote placeholder to {args.out}")
    print("ERROR: Need Windows/MSVC to extract real ordinals!")


if __name__ == "__main__":
    main()