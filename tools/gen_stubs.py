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
        
        # For MSVC compatibility: try to handle name mangling
        # MSVC expects unmangled names in source, DLL exports mangled names
        # The import library should map unmangled -> mangled
        # We'll export the mangled name, and hope MSVC's lib tool handles it
        # If not, we might need to create import library differently
        lines.append(f"    {symbol}={stub_name} @{mfc_ordinal}")
    
    lines.append(f"    ; Total exports: {len(entries)}")
    lines.append(f"    ; All functions have unique stubs")
    lines.append(f"    ; Note: MSVC linking may require proper import library creation")
    lines.append(f"    ; The 'lib' tool might not handle C++ name mangling automatically")
    
    return "\n".join(lines) + "\n"


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
        
        # Choose appropriate generic stub based on signature
        if '@@YAPAX' in symbol or '@@YAPAV' in symbol or '@@YAPA' in symbol:
            # Returns pointer
            lines.append(f'// Stub for {symbol}')
            lines.append(f'extern "C" void* MS_ABI {stub_name}() {{')
            lines.append(f'    return generic_stub_ptr();')
            lines.append('}')
        elif '@@YAH' in symbol or '@@YAI' in symbol or '@@YAJ' in symbol or '@@YAK' in symbol:
            # Returns int/long
            lines.append(f'// Stub for {symbol}')
            lines.append(f'extern "C" int MS_ABI {stub_name}() {{')
            lines.append(f'    return generic_stub_int();')
            lines.append('}')
        else:
            # Default: void return
            lines.append(f'// Stub for {symbol}')
            lines.append(f'extern "C" void MS_ABI {stub_name}() {{')
            lines.append(f'    generic_stub_void();')
            lines.append('}')
        
        lines.append('')
    
    lines.append(f'// Generated {stub_count} stub functions')
    lines.append('// .def file maps MSVC-mangled names to these stub functions')
    
    return "\n".join(lines)




def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--mapping", required=True, help="Path to ordinal mapping JSON (use mfc_real_ordinal_mapping.json)")
    ap.add_argument("--out-def", required=True)
    ap.add_argument("--out-stubs", required=True)
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