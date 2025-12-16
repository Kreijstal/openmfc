#!/usr/bin/env python3
"""
Generate implementation skeleton for OpenMFC classes.

This tool helps generate boilerplate for implementing MFC class methods.
It parses MSVC mangled names and generates readable C extern stubs.

Usage:
    python3 tools/gen_impl_skeleton.py --class CDC --mapping mfc_complete_ordinal_mapping.json
    python3 tools/gen_impl_skeleton.py --pattern "?Create@CWnd" --mapping mfc_complete_ordinal_mapping.json
"""

import argparse
import json
import re
from pathlib import Path
from typing import List, Dict, Tuple, Optional


# MSVC x64 mangling type codes
TYPE_CODES = {
    'X': 'void',
    '_N': 'bool',
    'C': 'signed char',
    'D': 'char',
    'E': 'unsigned char',
    'F': 'short',
    'G': 'unsigned short',
    'H': 'int',
    'I': 'unsigned int',
    'J': 'long',
    'K': 'unsigned long',
    '_J': '__int64',
    '_K': 'unsigned __int64',
    'M': 'float',
    'N': 'double',
    'O': 'long double',
    '_W': 'wchar_t',
}

# Member function modifiers (x64)
MEMBER_MODIFIERS = {
    'Q': 'public',          # QEAA = public __cdecl
    'R': 'public const',    # QEBA = public const __cdecl
    'S': 'public static',   # SA = public static
    'A': 'private',         # AEAA = private
    'B': 'private const',   # AEBA = private const
    'I': 'protected',
    'M': 'protected',       # MEAA = protected
    'U': 'public virtual',  # UEAA = public virtual
    'E': 'public virtual const',  # UEBA = public virtual const
}


def demangle_msvc_name(symbol: str) -> Dict:
    """
    Parse MSVC mangled name and extract components.

    Returns dict with:
    - class_name: The class name if method
    - method_name: The method/function name
    - is_constructor: True if constructor
    - is_destructor: True if destructor
    - return_type: Parsed return type
    - params: List of parameter types
    - is_const: True if const method
    - is_virtual: True if virtual method
    """
    result = {
        'symbol': symbol,
        'class_name': None,
        'method_name': None,
        'is_constructor': False,
        'is_destructor': False,
        'return_type': 'void',
        'params': [],
        'is_const': False,
        'is_virtual': False,
        'is_static': False,
        'ordinal': None,
    }

    # Check for constructor (??0) or destructor (??1)
    if symbol.startswith('??0'):
        result['is_constructor'] = True
        match = re.match(r'\?\?0([^@]+)@@', symbol)
        if match:
            result['class_name'] = match.group(1)
            result['method_name'] = result['class_name']  # ctor name = class name
            result['return_type'] = result['class_name'] + '*'
    elif symbol.startswith('??1'):
        result['is_destructor'] = True
        match = re.match(r'\?\?1([^@]+)@@', symbol)
        if match:
            result['class_name'] = match.group(1)
            result['method_name'] = '~' + result['class_name']
            result['return_type'] = 'void'
    elif symbol.startswith('??_'):
        # Operators and special functions
        match = re.match(r'\?\?([_A-Z])([^@]+)@@', symbol)
        if match:
            op_code = match.group(1)
            result['class_name'] = match.group(2)
            # Common operators
            ops = {'_U': 'operator new[]', '_V': 'operator delete[]',
                   '2': 'operator new', '3': 'operator delete'}
            result['method_name'] = ops.get(op_code, f'operator_{op_code}')
    else:
        # Regular method/function: ?MethodName@ClassName@@...
        match = re.match(r'\?([^@]+)@([^@]+)@@', symbol)
        if match:
            result['method_name'] = match.group(1)
            result['class_name'] = match.group(2)
        else:
            # Global function: ?FunctionName@@...
            match = re.match(r'\?([^@]+)@@', symbol)
            if match:
                result['method_name'] = match.group(1)

    # Parse member function modifiers
    mod_match = re.search(r'@@([QRSIAMUE])([AE])([AB]?)([AB]?)', symbol)
    if mod_match:
        mod = mod_match.group(1)
        if mod in ['U', 'E']:
            result['is_virtual'] = True
        if mod in ['S']:
            result['is_static'] = True
        # Second char: A = __cdecl (x86), E = __cdecl (x64)
        # Third char: A = mutable, B = const
        if mod_match.group(3) == 'B' or mod_match.group(4) == 'B':
            result['is_const'] = True

    # Parse return type (after @@ and calling convention indicator)
    ret_match = re.search(r'@@[A-Z][AE][AB]?[AB]?([A-Z_@?]+)', symbol)
    if ret_match:
        type_code = ret_match.group(1)
        result['return_type'] = parse_type_code(type_code)

    return result


def parse_type_code(code: str) -> str:
    """Parse a single MSVC type code to C type."""
    if not code:
        return 'void'

    # Check simple types first
    for tc, ctype in TYPE_CODES.items():
        if code.startswith(tc):
            return ctype

    # Pointer types
    if code.startswith('PEA') or code.startswith('PA'):
        # Pointer to something
        rest = code[3:] if code.startswith('PEA') else code[2:]
        if rest.startswith('V'):
            # Pointer to class
            class_match = re.match(r'V([^@]+)@@', rest)
            if class_match:
                return class_match.group(1) + '*'
        elif rest.startswith('_W'):
            return 'wchar_t*'
        elif rest.startswith('D'):
            return 'char*'
        return 'void*'

    if code.startswith('PEB') or code.startswith('PB'):
        # Const pointer
        rest = code[3:] if code.startswith('PEB') else code[2:]
        if rest.startswith('_W'):
            return 'const wchar_t*'
        elif rest.startswith('D'):
            return 'const char*'
        return 'const void*'

    # Reference types
    if code.startswith('AEA') or code.startswith('AA'):
        return 'ref'  # Reference
    if code.startswith('AEB') or code.startswith('AB'):
        return 'const_ref'  # Const reference

    # Class by value
    if code.startswith('?AV') or code.startswith('V'):
        class_match = re.match(r'\??A?V([^@]+)@@', code)
        if class_match:
            return class_match.group(1)

    # Struct
    if code.startswith('U'):
        struct_match = re.match(r'U([^@]+)@@', code)
        if struct_match:
            return struct_match.group(1)

    return 'void*'  # Default


def generate_stub_name(symbol: str) -> str:
    """Generate a valid C identifier from MSVC mangled name (for .def file linking)."""
    stub = re.sub(r'[?@]', '_', symbol)
    stub = re.sub(r'[^a-zA-Z0-9_]', '_', stub)
    if not stub[0].isalpha() and stub[0] != '_':
        stub = '_' + stub
    if len(stub) > 200:
        stub = stub[:100] + '_' + stub[-100:]
    return 'stub_' + stub


def generate_impl_name(info: Dict) -> str:
    """Generate a readable implementation function name."""
    parts = ['openmfc']
    if info['class_name']:
        parts.append(info['class_name'])
    if info['method_name']:
        method = info['method_name']
        # Clean up operator names
        method = method.replace('~', 'dtor_')
        method = method.replace('operator ', 'op_')
        parts.append(method)
    return '_'.join(parts)


def generate_skeleton(symbols: List[Dict], output_style: str = 'stub') -> str:
    """
    Generate implementation skeleton for given symbols.

    output_style:
    - 'stub': Generate stub_ names that override weak stubs
    - 'impl': Generate openmfc_ readable names (requires .def changes)
    """
    lines = []
    lines.append('// Auto-generated implementation skeleton')
    lines.append('// Generated by gen_impl_skeleton.py')
    lines.append('')
    lines.append('#define OPENMFC_APPCORE_IMPL')
    lines.append('#include "openmfc/afxwin.h"')
    lines.append('#include <windows.h>')
    lines.append('')
    lines.append('#ifdef __GNUC__')
    lines.append('  #define MS_ABI __attribute__((ms_abi))')
    lines.append('#else')
    lines.append('  #define MS_ABI')
    lines.append('#endif')
    lines.append('')

    for sym in symbols:
        info = demangle_msvc_name(sym['symbol'])
        info['ordinal'] = sym.get('ordinal')

        stub_name = generate_stub_name(sym['symbol'])
        impl_name = generate_impl_name(info)

        # Generate comment header
        lines.append(f'// {info["class_name"] or "Global"}::{info["method_name"]}')
        lines.append(f'// Symbol: {sym["symbol"]}')
        if info['ordinal']:
            lines.append(f'// Ordinal: {info["ordinal"]}')

        # Determine function name based on style
        func_name = stub_name if output_style == 'stub' else impl_name

        # Generate function signature
        ret_type = info['return_type'] or 'void'
        params = []

        if info['class_name'] and not info['is_static']:
            # Member function - add this pointer
            const_qual = 'const ' if info['is_const'] else ''
            params.append(f'{const_qual}{info["class_name"]}* pThis')

        # TODO: Parse actual parameters from mangled name
        # For now, use placeholder
        param_str = ', '.join(params) if params else 'void'

        lines.append(f'extern "C" {ret_type} MS_ABI {func_name}({param_str}) {{')

        if info['is_constructor']:
            lines.append('    if (!pThis) return nullptr;')
            lines.append('    // TODO: Initialize members')
            lines.append('    return pThis;')
        elif info['is_destructor']:
            lines.append('    if (!pThis) return;')
            lines.append('    // TODO: Clean up')
        elif ret_type != 'void':
            if ret_type.endswith('*'):
                lines.append('    return nullptr; // TODO: Implement')
            elif ret_type in ['int', 'BOOL', 'UINT', 'unsigned int']:
                lines.append('    return 0; // TODO: Implement')
            else:
                lines.append('    // TODO: Implement')
                lines.append(f'    {ret_type} result = {{}};')
                lines.append('    return result;')
        else:
            lines.append('    // TODO: Implement')

        lines.append('}')
        lines.append('')

    return '\n'.join(lines)


def main():
    parser = argparse.ArgumentParser(description='Generate implementation skeleton for OpenMFC')
    parser.add_argument('--mapping', required=True, help='Path to ordinal mapping JSON')
    parser.add_argument('--class', dest='class_name', help='Class name to generate (e.g., CDC, CWnd)')
    parser.add_argument('--pattern', help='Regex pattern to match symbols')
    parser.add_argument('--output', '-o', help='Output file (default: stdout)')
    parser.add_argument('--style', choices=['stub', 'impl'], default='stub',
                        help='Output style: stub (override weak stubs) or impl (readable names)')
    parser.add_argument('--limit', type=int, default=50, help='Max symbols to output')
    args = parser.parse_args()

    # Load mapping
    with open(args.mapping) as f:
        mapping = json.load(f)

    exports = mapping['exports']['mfc140u']

    # Filter symbols
    filtered = []
    for exp in exports:
        sym = exp['symbol']

        # Filter by class name
        if args.class_name:
            # Match class in mangled name: @ClassName@@
            if f'@{args.class_name}@@' not in sym:
                # Also check for ??0ClassName@@ (constructor) and ??1ClassName@@ (destructor)
                if not (sym.startswith(f'??0{args.class_name}@@') or
                        sym.startswith(f'??1{args.class_name}@@')):
                    continue

        # Filter by pattern
        if args.pattern:
            if not re.search(args.pattern, sym):
                continue

        # Skip data exports (not functions)
        if not sym.endswith('Z'):
            continue

        filtered.append(exp)
        if len(filtered) >= args.limit:
            break

    if not filtered:
        print(f"No symbols found matching criteria", file=__import__('sys').stderr)
        return 1

    # Generate output
    output = generate_skeleton(filtered, args.style)

    if args.output:
        with open(args.output, 'w') as f:
            f.write(output)
        print(f"Generated {len(filtered)} stubs to {args.output}")
    else:
        print(output)

    return 0


if __name__ == '__main__':
    exit(main())
