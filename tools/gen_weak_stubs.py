#!/usr/bin/env python3
"""
Generate weak stub implementations for OpenMFC.

Unlike gen_stubs.py which generates regular stubs, this generates
__attribute__((weak)) stubs that can be overridden by strong symbols.

This allows us to:
1. Generate stubs for ALL 14k+ exports (ABI completeness)
2. Implement real functions that override the weak stubs
3. Never worry about duplicate symbol errors

Usage:
    python3 tools/gen_weak_stubs.py \
        --mapping mfc_complete_ordinal_mapping.json \
        --out-def build/openmfc.def \
        --out-stubs build/weak_stubs.cpp
"""

import argparse
import json
import re
from pathlib import Path
from typing import Dict, List, Any


def generate_stub_name(symbol: str) -> str:
    """Generate a valid C identifier from MSVC mangled name."""
    stub = re.sub(r'[?@]', '_', symbol)
    stub = re.sub(r'[^a-zA-Z0-9_]', '_', stub)
    if not stub[0].isalpha() and stub[0] != '_':
        stub = '_' + stub
    if len(stub) > 200:
        stub = stub[:100] + '_' + stub[-100:]
    return 'impl_' + stub


def is_data_export(symbol: str) -> bool:
    """
    Determine if a symbol is a data export (variable) vs function.

    MSVC mangling:
    - Functions end with @Z (parameter list terminator)
    - Data exports have patterns like:
      - @@3IA = global unsigned int
      - @@2VClassName@@B = static const class member
      - @@3PA = global pointer
    """
    import re

    # Functions always end with Z (or XZ for void params)
    if symbol.endswith('Z'):
        # But check it's not a data pattern that happens to have Z in the type
        # Data: @@3... or @@2... followed by type, no @Z at end
        if re.search(r'@@[23][A-Z][A-Z0-9_]*$', symbol):
            return True
        return False

    # Doesn't end in Z - likely data
    # Check for common data patterns
    if re.search(r'@@[23][A-Z]', symbol):
        return True

    return True  # Default to data if not clearly a function


def guess_data_type(symbol: str) -> str:
    """
    Guess the C type for a data export based on MSVC mangling.

    Common type codes:
    - I = unsigned int
    - H = int
    - J = long
    - K = unsigned long
    - N = double
    - M = float
    - PA/PB = pointer
    - V = class
    """
    import re

    # Global/static data: @@3<type> or @@2<type>
    match = re.search(r'@@[23]([A-Z][A-Z0-9_]*)$', symbol)
    if match:
        type_code = match.group(1)
        if type_code.startswith('I'):
            return 'unsigned int'
        elif type_code.startswith('H'):
            return 'int'
        elif type_code.startswith('J'):
            return 'long'
        elif type_code.startswith('K'):
            return 'unsigned long'
        elif type_code.startswith('N'):
            return 'double'
        elif type_code.startswith('M'):
            return 'float'
        elif type_code.startswith('P'):
            return 'void*'
        elif type_code.startswith('V'):
            # Class type - use opaque struct
            return 'void*'

    return 'void*'  # Default to pointer


def guess_return_type(symbol: str) -> str:
    """
    Guess return type from MSVC mangling.

    MSVC x64 mangling patterns:
    @@YA = __cdecl function
    @@YG = __stdcall function
    @@YI = __fastcall function

    Return type follows:
    X = void
    H = int
    I = unsigned int
    J = long
    K = unsigned long
    N = double
    M = float
    PA = pointer
    PB = const pointer
    PEA = pointer (64-bit)
    AAV = reference to class
    ABV = const reference to class
    ?AV = class by value
    """
    # Look for return type indicator after calling convention
    patterns = [
        (r'@@Y[AGIQ][AE]?X', 'void'),      # void return
        (r'@@Y[AGIQ][AE]?_N', 'bool'),     # bool return
        (r'@@Y[AGIQ][AE]?H', 'int'),       # int return
        (r'@@Y[AGIQ][AE]?I', 'uint'),      # unsigned int
        (r'@@Y[AGIQ][AE]?J', 'long'),      # long
        (r'@@Y[AGIQ][AE]?K', 'ulong'),     # unsigned long
        (r'@@Y[AGIQ][AE]?N', 'double'),    # double
        (r'@@Y[AGIQ][AE]?M', 'float'),     # float
        (r'@@Y[AGIQ][AE]?P[AEB]', 'ptr'),  # pointer
        (r'@@Y[AGIQ][AE]?\?AV', 'class'),  # class by value
        (r'@@Y[AGIQ][AE]?A[AEB]V', 'ref'), # reference to class
    ]

    for pattern, ret_type in patterns:
        if re.search(pattern, symbol):
            return ret_type

    # Member functions (not starting with global function marker)
    if '@@QAE' in symbol or '@@QBE' in symbol or '@@UAE' in symbol or '@@UBE' in symbol:
        # These are member functions, check return after the access specifier
        if 'XZ' in symbol[-10:]:  # void at end
            return 'void'
        if re.search(r'@@[QU][AB]E[AE]?X', symbol):
            return 'void'
        if re.search(r'@@[QU][AB]E[AE]?H', symbol):
            return 'int'
        if re.search(r'@@[QU][AB]E[AE]?_N', symbol):
            return 'bool'
        if re.search(r'@@[QU][AB]E[AE]?P[AEB]', symbol):
            return 'ptr'

    # Default to void for safety
    return 'void'


def emit_def(entries: List[Dict[str, Any]]) -> str:
    """Generate .def file with ordinal exports."""
    lines = ["LIBRARY openmfc", "EXPORTS"]

    for entry in entries:
        symbol = entry["symbol"]
        ordinal = entry.get("ordinal", 0)
        stub_name = generate_stub_name(symbol)

        # Data exports need the DATA keyword
        if is_data_export(symbol):
            lines.append(f"    {symbol}={stub_name} @{ordinal} DATA")
        else:
            lines.append(f"    {symbol}={stub_name} @{ordinal}")

    lines.append(f"    ; Total exports: {len(entries)}")
    return "\n".join(lines) + "\n"


def emit_weak_stubs(entries: List[Dict[str, Any]]) -> str:
    """Generate weak stub implementations."""

    lines = [
        '// Auto-generated stubs for OpenMFC',
        '//',
        '// DO NOT EDIT - regenerate with gen_weak_stubs.py',
        '//',
        '// Note: We do NOT use __attribute__((weak)) because PE/COFF weak',
        '// symbols behave differently than ELF. Instead, to override a stub:',
        '// 1. Add the function name to an exclusion list in gen_weak_stubs.py',
        '// 2. Regenerate stubs (the excluded function will not be generated)',
        '// 3. Provide your own implementation with the same impl_xxx name',
        '',
        '#include <cstdio>',
        '#include <cstdlib>',
        '',
        '// Calling convention for MSVC compatibility',
        '#if defined(__GNUC__)',
        '  #define MS_ABI __attribute__((ms_abi))',
        '#else',
        '  #define MS_ABI',
        '#endif',
        '',
        '// Stub behavior: log and return safe default',
        '// In debug builds, you might want to make these more verbose',
        '#ifdef OPENMFC_VERBOSE_STUBS',
        '  #define STUB_LOG(name) std::fprintf(stderr, "STUB: %s\\n", name)',
        '#else',
        '  #define STUB_LOG(name) ((void)0)',
        '#endif',
        '',
        '// ============================================================',
        '// DATA EXPORTS (global variables, static members)',
        '// ============================================================',
        '',
    ]

    generated = set()
    func_count = 0
    data_count = 0

    # First pass: data exports
    for entry in entries:
        symbol = entry["symbol"]
        stub_name = generate_stub_name(symbol)

        if stub_name in generated:
            continue

        if is_data_export(symbol):
            generated.add(stub_name)
            data_count += 1
            data_type = guess_data_type(symbol)
            # Generate data as simple C-linkage variable
            lines.append(f'// Data: {symbol}')
            lines.append(f'{data_type} {stub_name} = 0;')
            lines.append('')

    lines.append('// ============================================================')
    lines.append('// FUNCTION EXPORTS')
    lines.append('// ============================================================')
    lines.append('')

    # Second pass: function exports
    for entry in entries:
        symbol = entry["symbol"]
        stub_name = generate_stub_name(symbol)

        if stub_name in generated:
            continue
        generated.add(stub_name)
        func_count += 1

        ret_type = guess_return_type(symbol)

        # Generate appropriate stub based on return type
        if ret_type == 'void':
            lines.append(f'extern "C" void MS_ABI {stub_name}() {{')
            lines.append(f'    STUB_LOG("{symbol}");')
            lines.append('}')
        elif ret_type in ('int', 'long', 'uint', 'ulong'):
            lines.append(f'extern "C" int MS_ABI {stub_name}() {{')
            lines.append(f'    STUB_LOG("{symbol}");')
            lines.append('    return 0;')
            lines.append('}')
        elif ret_type == 'bool':
            lines.append(f'extern "C" int MS_ABI {stub_name}() {{')
            lines.append(f'    STUB_LOG("{symbol}");')
            lines.append('    return 0;  // false')
            lines.append('}')
        elif ret_type in ('float', 'double'):
            lines.append(f'extern "C" double MS_ABI {stub_name}() {{')
            lines.append(f'    STUB_LOG("{symbol}");')
            lines.append('    return 0.0;')
            lines.append('}')
        elif ret_type in ('ptr', 'class', 'ref'):
            lines.append(f'extern "C" void* MS_ABI {stub_name}() {{')
            lines.append(f'    STUB_LOG("{symbol}");')
            lines.append('    return nullptr;')
            lines.append('}')
        else:
            # Default: void
            lines.append(f'extern "C" void MS_ABI {stub_name}() {{')
            lines.append(f'    STUB_LOG("{symbol}");')
            lines.append('}')

        lines.append('')

    lines.append(f'// Generated {func_count} function stubs + {data_count} data stubs')
    lines.append(f'// Total: {len(generated)} exports')
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(
        description="Generate weak stubs for OpenMFC"
    )
    parser.add_argument(
        "--mapping", "-m",
        required=True,
        help="Path to ordinal mapping JSON"
    )
    parser.add_argument(
        "--out-def",
        required=True,
        help="Output .def file path"
    )
    parser.add_argument(
        "--out-stubs",
        required=True,
        help="Output weak stubs .cpp file path"
    )
    parser.add_argument(
        "--exclude",
        help="Comma-separated list of symbols to exclude from stub generation"
    )
    args = parser.parse_args()

    mapping_path = Path(args.mapping)
    if not mapping_path.exists():
        print(f"Error: Mapping file not found: {mapping_path}")
        return 1

    data = json.loads(mapping_path.read_text())
    all_entries = data.get("exports", {}).get("mfc140u", [])
    
    # Filter excluded symbols for stub generation only
    stub_entries = all_entries
    if args.exclude:
        excluded_symbols = set(args.exclude.split(','))
        original_count = len(stub_entries)
        stub_entries = [e for e in stub_entries if e.get("symbol") not in excluded_symbols]
        print(f"Excluded {original_count - len(stub_entries)} symbols from stub generation")

    # Sort by ordinal
    all_entries.sort(key=lambda x: x.get("ordinal", 0))
    stub_entries.sort(key=lambda x: x.get("ordinal", 0))

    print(f"Loaded {len(all_entries)} entries from mapping")

    # Generate .def (ALL entries, even excluded ones)
    Path(args.out_def).parent.mkdir(parents=True, exist_ok=True)
    Path(args.out_def).write_text(emit_def(all_entries), encoding="ascii")
    print(f"Generated: {args.out_def}")

    # Generate weak stubs (only non-excluded entries)
    Path(args.out_stubs).parent.mkdir(parents=True, exist_ok=True)
    Path(args.out_stubs).write_text(emit_weak_stubs(stub_entries), encoding="ascii")
    print(f"Generated: {args.out_stubs}")

    # Stats
    ordinals = [e.get("ordinal", 0) for e in all_entries]
    print(f"Ordinal range: {min(ordinals)} to {max(ordinals)}")


if __name__ == "__main__":
    main()
