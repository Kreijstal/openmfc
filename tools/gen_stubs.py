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
import re
from pathlib import Path
from typing import Dict, List, Any


def generate_stub_name(symbol: str) -> str:
    """Generate a valid C identifier from MSVC mangled name."""
    # Remove ? and @@ which are invalid in C identifiers
    # Replace with _
    stub = re.sub(r'[?@]', '_', symbol)
    
    # Remove other invalid characters
    stub = re.sub(r'[^a-zA-Z0-9_]', '_', stub)
    
    # Ensure it starts with letter or underscore
    if not stub[0].isalpha() and stub[0] != '_':
        stub = '_' + stub
    
    # Truncate if too long (Windows limit is 247 chars for extern "C")
    if len(stub) > 200:
        # Keep first 100 and last 100 chars
        stub = stub[:100] + '_' + stub[-100:]
    
    return 'stub_' + stub


def extract_unmangled_name(mangled: str) -> str:
    """Extract unmangled name from MSVC mangled symbol."""
    # Pattern: ?FunctionName@@...
    if not mangled.startswith('?'):
        return mangled  # Not a mangled C++ name
    
    # MSVC mangling patterns:
    # 1. Global functions: ?FunctionName@@...
    # 2. Member functions: ?FunctionName@Class@@...
    # 3. Templated functions: ??$FunctionName@...@@...
    
    # For simplicity, we'll use a different approach:
    # Instead of trying to extract unmangled names (which is complex),
    # we'll use the mangled name as-is for MSVC.
    # The real issue is that MSVC's lib tool needs the right format.
    
    # Actually, let's think about this differently.
    # The DLL exports mangled names like ?AfxThrowMemoryException@@YAXXZ
    # MSVC source code uses AfxThrowMemoryException()
    # The import library needs to map AfxThrowMemoryException -> ?AfxThrowMemoryException@@YAXXZ
    
    # For now, return the mangled name as-is.
    # We'll handle the mapping differently.
    return mangled


def emit_def(entries: List[Dict[str, Any]]) -> str:
    lines = ["LIBRARY openmfc", "EXPORTS"]
    
    for i, entry in enumerate(entries):
        symbol = entry["symbol"]
        
        # Use ordinal from entry if available, otherwise fall back to array index
        if "ordinal" in entry and entry["ordinal"] is not None:
            mfc_ordinal = entry["ordinal"]
        else:
            # Fallback: use array index + 256 (WRONG but works for testing)
            mfc_ordinal = 256 + i
        
        # All public exports get unique stubs
        stub_name = generate_stub_name(symbol)
        
        # Try to create unmangled alias name
        unmangled = extract_unmangled_name(symbol)
        
        if unmangled != symbol:
            # Export with alias: mangled_name points to stub
            # This creates mangled export in DLL
            lines.append(f"    {symbol}={stub_name} @{mfc_ordinal}")
        else:
            # Can't extract unmangled name, just export as-is
            lines.append(f"    {symbol}={stub_name} @{mfc_ordinal}")
    
    lines.append(f"    ; Total exports: {len(entries)}")
    lines.append(f"    ; DLL exports MSVC-mangled names")
    lines.append(f"    ; MSVC import library needs to map unmangled names to these")
    
    return "\n".join(lines) + "\n"


def emit_msvc_def(entries: List[Dict[str, Any]]) -> str:
    """Generate MSVC-compatible .def file with unmangled aliases."""
    lines = ["LIBRARY openmfc", "EXPORTS"]
    
    for i, entry in enumerate(entries):
        symbol = entry["symbol"]
        
        # Use ordinal from entry if available, otherwise fall back to array index
        if "ordinal" in entry and entry["ordinal"] is not None:
            mfc_ordinal = entry["ordinal"]
        else:
            # Fallback: use array index + 256 (WRONG but works for testing)
            mfc_ordinal = 256 + i
        
        # Extract unmangled name for MSVC compatibility
        unmangled = extract_unmangled_name(symbol)
        
        # For MSVC: export unmangled name that maps to mangled stub
        # Format: unmangled_name=mangled_stub_name @ordinal
        # This creates an alias in the import library
        stub_name = generate_stub_name(symbol)
        lines.append(f"    {unmangled}={stub_name} @{mfc_ordinal}")
    
    lines.append(f"    ; Total exports: {len(entries)}")
    lines.append(f"    ; MSVC-compatible format with unmangled names")
    lines.append(f"    ; Maps unmangled names to mangled stub functions")
    
    return "\n".join(lines) + "\n"


def emit_objcopy_mapping(entries: List[Dict[str, Any]]) -> str:
    """Generate symbol mapping file for objcopy --redefine-syms."""
    lines = []
    
    for entry in entries:
        symbol = entry["symbol"]
        stub_name = generate_stub_name(symbol)
        
        # Extract unmangled name for the C stub
        unmangled = extract_unmangled_name(symbol)
        if unmangled == symbol:
            # Can't extract unmangled name, use stub name directly
            unmangled = stub_name
        
        # objcopy mapping format: old_name new_name
        # We want to rename unmangled C name to MSVC-mangled name
        lines.append(f'{unmangled} {symbol}')
    
    return "\n".join(lines)


def parse_function_signature(symbol: str) -> Dict[str, Any]:
    """Parse MSVC mangled name to extract basic signature info."""
    # Very basic parsing - just enough to generate stubs
    result = {
        'is_void': False,
        'has_args': True,
        'name': 'unknown'
    }
    
    # Extract function name (between ? and @@)
    match = re.match(r'\?(?P<name>[^@]+)@@', symbol)
    if match:
        result['name'] = match.group('name')
    
    # Check if void with no args: @@YAXXZ
    if '@@YAXXZ' in symbol:
        result['is_void'] = True
        result['has_args'] = False
    
    # Check for common patterns to guess signature
    # @@YAX = void __cdecl
    # @@YAPAX = void* __cdecl  
    # @@YA?AV = class __cdecl returning class
    # etc.
    
    return result







def emit_stubs(entries: List[Dict[str, Any]]) -> str:
    """Generate C++ stub implementations for all functions."""
    
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
        '// Generic stub implementations',
        'extern "C" void MS_ABI generic_stub_void() {',
        '    std::fprintf(stderr, "Not Implemented (generic stub)\\n");',
        '}',
        '',
        'extern "C" int MS_ABI generic_stub_int() {',
        '    std::fprintf(stderr, "Not Implemented (generic stub returning int)\\n");',
        '    return 0;',
        '}',
        '',
        'extern "C" void* MS_ABI generic_stub_ptr() {',
        '    std::fprintf(stderr, "Not Implemented (generic stub returning pointer)\\n");',
        '    return nullptr;',
        '}',
        '',
    ]
    
    generated_stubs = set()
    stub_count = 0
    
    # Generate stubs for ALL entries (no filtering)
    for entry in entries:
        symbol = entry["symbol"]
        stub_name = generate_stub_name(symbol)
        
        if stub_name in generated_stubs:
            continue
        
        generated_stubs.add(stub_name)
        stub_count += 1
        
        # Extract unmangled name for C export
        unmangled = extract_unmangled_name(symbol)
        if unmangled == symbol:
            # Can't extract unmangled name, use stub name
            unmangled = stub_name
        
        # Choose appropriate generic stub based on signature
        if '@@YAPAX' in symbol or '@@YAPAV' in symbol or '@@YAPA' in symbol:
            # Returns pointer
            lines.append(f'// Stub for {symbol}')
            lines.append(f'extern "C" void* MS_ABI {unmangled}() {{')
            lines.append(f'    return generic_stub_ptr();')
            lines.append('}')
        elif '@@YAH' in symbol or '@@YAI' in symbol or '@@YAJ' in symbol or '@@YAK' in symbol:
            # Returns int/long
            lines.append(f'// Stub for {symbol}')
            lines.append(f'extern "C" int MS_ABI {unmangled}() {{')
            lines.append(f'    return generic_stub_int();')
            lines.append('}')
        else:
            # Default: void return
            lines.append(f'// Stub for {symbol}')
            lines.append(f'extern "C" void MS_ABI {unmangled}() {{')
            lines.append(f'    generic_stub_void();')
            lines.append('}')
        
        lines.append('')
    
    lines.append(f'// Generated {stub_count} stub functions')
    lines.append('// Functions have unmangled C names with dllexport')
    lines.append('// objcopy will rename them to MSVC-mangled names')
    
    return "\n".join(lines)




def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--mapping", required=True, help="Path to ordinal mapping JSON (use mfc_real_ordinal_mapping.json)")
    ap.add_argument("--out-def", required=True)
    ap.add_argument("--out-stubs", required=True)
    ap.add_argument("--msvc-def", help="Optional: Generate MSVC-compatible .def file")
    ap.add_argument("--out-asm", help="Optional: Generate assembly aliases file")
    args = ap.parse_args()

    mapping_path = Path(args.mapping)
    if not mapping_path.exists():
        print(f"Error: Mapping file not found: {mapping_path}")
        return 1
    
    mapping: Dict = json.loads(mapping_path.read_text())
    entries = mapping.get("exports", {}).get("mfc140u", [])
    print(f"Loaded {len(entries)} entries from mapping file")
    
    # Sort by ordinal if available
    if entries and "ordinal" in entries[0]:
        entries.sort(key=lambda x: x.get("ordinal", 0))
    
    Path(args.out_def).write_text(emit_def(entries), encoding="ascii")
    Path(args.out_stubs).write_text(emit_stubs(entries), encoding="ascii")
    
    # Generate MSVC .def file if requested
    if args.msvc_def:
        Path(args.msvc_def).write_text(emit_msvc_def(entries), encoding="ascii")
        print(f"Generated MSVC-compatible .def file: {args.msvc_def}")
    
    # Generate objcopy mapping if requested
    if args.out_asm:  # Reusing --out-asm for objcopy mapping
        Path(args.out_asm).write_text(emit_objcopy_mapping(entries), encoding="ascii")
        print(f"Generated objcopy mapping file: {args.out_asm}")
    
    # Show some statistics
    if entries:
        ordinals = [e.get("ordinal", 0) for e in entries if "ordinal" in e]
        if ordinals:
            min_ord = min(ordinals)
            max_ord = max(ordinals)
            print(f"Generated {len(entries)} exports with ordinals {min_ord} to {max_ord}")
            print(f"{len(ordinals)}/{len(entries)} entries have ordinals")
        else:
            print(f"Generated {len(entries)} exports (no ordinals in data)")


if __name__ == "__main__":
    main()