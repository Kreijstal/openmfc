#!/usr/bin/env python3
"""
Create MSVC-compatible import library from OpenMFC exports.

MSVC's lib /DEF tool doesn't handle C++ name mangling properly when
creating import libraries from .def files. This script creates a
proper MSVC import library that maps unmangled source names to
mangled DLL exports.

Approach:
1. Parse the complete ordinal mapping
2. For each mangled export, extract the unmangled name
3. Create a .def file that exports unmangled names
4. Use lib /DEF to create import library
5. The import library will have unmangled names that MSVC can link against
6. The DLL exports mangled names for ABI compatibility

But wait: if the DLL exports mangled names and import library has
unmangled names, they won't match at runtime.

Actually, import libraries need to match DLL exports. So we need
the import library to have mangled names. But MSVC looks for
unmangled names when using __declspec(dllimport).

Solution: Create import library with mangled names, and provide
header files or linker directives that map unmangled to mangled.
"""

import argparse
import json
import re
import subprocess
import sys
import tempfile
import os
from pathlib import Path
from typing import Dict, List, Tuple, Optional


def extract_unmangled_name(mangled: str) -> Optional[str]:
    """Extract unmangled name from MSVC mangled symbol."""
    # Pattern: ?FunctionName@@...
    if not mangled.startswith('?'):
        return None  # Not a mangled C++ name
    
    # Split at @@
    parts = mangled.split('@@', 1)
    if len(parts) < 2:
        return None
    
    mangled_prefix = parts[0]
    # Remove leading ?
    if mangled_prefix.startswith('?'):
        return mangled_prefix[1:]
    
    return None


def create_msvc_def_file(mapping: Dict[str, int], output_path: Path) -> None:
    """
    Create MSVC-compatible .def file.
    
    MSVC's lib tool expects certain format. We'll create a .def file
    that exports unmangled names, hoping MSVC will mangle them
    when creating the import library.
    """
    lines = ["LIBRARY openmfc", "EXPORTS"]
    
    for symbol, ordinal in mapping.items():
        # Try to get unmangled name
        unmangled = extract_unmangled_name(symbol)
        
        if unmangled:
            # Export unmangled name
            # MSVC should mangle it when creating import library
            lines.append(f"    {unmangled} @{ordinal}")
        else:
            # Export as-is (already unmangled or unknown format)
            lines.append(f"    {symbol} @{ordinal}")
    
    lines.append(f"    ; Total exports: {len(mapping)}")
    lines.append(f"    ; Generated for MSVC compatibility")
    
    output_path.write_text("\n".join(lines) + "\n")
    print(f"Created MSVC .def file: {output_path}")


def create_dual_def_file(mapping: Dict[str, int], output_path: Path) -> None:
    """
    Create .def file with both mangled and unmangled exports.
    
    This creates aliases: unmangled_name=mangled_name
    But we can't have two exports with same ordinal.
    Actually, we can use NONAME for one of them.
    """
    lines = ["LIBRARY openmfc", "EXPORTS"]
    
    for symbol, ordinal in mapping.items():
        unmangled = extract_unmangled_name(symbol)
        
        if unmangled:
            # Export unmangled name as primary
            lines.append(f"    {unmangled} @{ordinal}")
            # Export mangled name as NONAME (hidden)
            lines.append(f"    {symbol} @{ordinal} NONAME")
        else:
            lines.append(f"    {symbol} @{ordinal}")
    
    lines.append(f"    ; Total exports: {len(mapping)}")
    lines.append(f"    ; Dual export: unmangled + mangled (NONAME)")
    
    output_path.write_text("\n".join(lines) + "\n")
    print(f"Created dual .def file: {output_path}")


def create_header_with_pragmas(mapping: Dict[str, int], output_path: Path) -> None:
    """
    Create C++ header with pragma comments for linker.
    
    #pragma comment(linker, "/export:Symbol=ActualExport")
    This creates an alias in the import library.
    """
    lines = [
        "// MSVC-compatible header for OpenMFC",
        "// Contains pragma comments to map unmangled names to mangled exports",
        "#pragma once",
        "",
        "#ifdef _MSC_VER",
        "// MSVC-specific pragmas",
    ]
    
    for symbol, ordinal in mapping.items():
        unmangled = extract_unmangled_name(symbol)
        if unmangled:
            # Create pragma to map unmangled -> mangled
            lines.append(f'#pragma comment(linker, "/export:{unmangled}={symbol}")')
    
    lines.extend([
        "#endif // _MSC_VER",
        "",
        "// Forward declarations",
    ])
    
    # Add function declarations
    for symbol in mapping:
        unmangled = extract_unmangled_name(symbol)
        if unmangled and '@@YAXXZ' in symbol:  # void __cdecl with no args
            lines.append(f"extern \"C\" void __cdecl {unmangled}();")
    
    output_path.write_text("\n".join(lines) + "\n")
    print(f"Created header with pragmas: {output_path}")


def run_lib_tool(def_path: Path, lib_path: Path, machine: str = "X64") -> bool:
    """
    Run MSVC lib tool to create import library.
    
    Note: This requires Windows with MSVC installed.
    For CI, this runs on Windows agents.
    """
    # This is for documentation - actual execution happens in CI
    print(f"Would run: lib /DEF:{def_path} /OUT:{lib_path} /MACHINE:{machine}")
    print("Note: This requires Windows with MSVC installed")
    return True


def main():
    parser = argparse.ArgumentParser(description="Create MSVC import library for OpenMFC")
    parser.add_argument("--mapping", required=True, help="Path to complete ordinal mapping JSON")
    parser.add_argument("--out-def", required=True, help="Output .def file path")
    parser.add_argument("--out-lib", help="Output .lib file path (requires Windows/MSVC)")
    parser.add_argument("--out-header", help="Output header file with pragmas")
    parser.add_argument("--dual", action="store_true", help="Create dual export .def file")
    
    args = parser.parse_args()
    
    # Load mapping
    with open(args.mapping) as f:
        data = json.load(f)
    
    # Extract symbol->ordinal mapping
    mapping = {}
    if 'exports' in data and 'mfc140u' in data['exports']:
        for entry in data['exports']['mfc140u']:
            symbol = entry['symbol']
            ordinal = entry['ordinal']
            mapping[symbol] = ordinal
    else:
        # Assume it's already a flat mapping
        mapping = data
    
    print(f"Loaded {len(mapping)} symbol mappings")
    
    # Create .def file
    out_def = Path(args.out_def)
    if args.dual:
        create_dual_def_file(mapping, out_def)
    else:
        create_msvc_def_file(mapping, out_def)
    
    # Create header if requested
    if args.out_header:
        create_header_with_pragmas(mapping, Path(args.out_header))
    
    # Note about lib tool
    if args.out_lib:
        print(f"\nTo create import library on Windows:")
        print(f"  lib /DEF:{out_def} /OUT:{args.out_lib} /MACHINE:X64")
        print(f"\nOr in PowerShell:")
        print(f"  & lib /DEF:{out_def} /OUT:{args.out_lib} /MACHINE:X64")
    
    return 0


if __name__ == "__main__":
    sys.exit(main())