#!/usr/bin/env python3
"""
Generate extern "C" MS_ABI thunks that route DLL exports through
our real C++ method implementations.

Scans our source files for implemented ClassName::MethodName pairs,
demangles all export symbols from the mapping file, and generates
thunk wrappers for each export that has a real C++ implementation.

Usage:
    python3 tools/gen_thunks.py \
        --mapping mfc_complete_ordinal_mapping.json \
        --source-dir phase4/src \
        --include-dir include/openmfc \
        --out phase4/src/thunks.cpp
"""

import argparse
import json
import os
import re
import sys
from pathlib import Path
from collections import defaultdict

sys.path.insert(0, str(Path(__file__).parent))
import demangle_msvc
demangle_msvc.RESOLVE_TYPE_NAMES = True
from demangle_msvc import demangle


def scan_implemented_methods(source_dir: str) -> dict:
    """Scan C++ source files for class method implementations.
    Returns dict: className -> set of methodNames."""
    
    implemented = defaultdict(set)
    
    for root, dirs, files in os.walk(source_dir):
        for fn in files:
            if not fn.endswith('.cpp'):
                continue
            filepath = os.path.join(root, fn)
            try:
                with open(filepath) as f:
                    content = f.read()
            except:
                continue
            
            # Skip auto-generated files
            if 'Auto-generated' in content[:200]:
                continue
            
            # Find patterns like:
            #   ReturnType ClassName::MethodName(
            #   ClassName::MethodName(
            #   ClassName::~ClassName(
            #   ReturnType NS::ClassName::MethodName(
            for m in re.finditer(r'(?:^|\n)\s*(?:[\w:<>*&\s]+?\s+)?(\w+)::(\w+)\s*\(', content):
                cls, method = m.group(1), m.group(2)
                # Skip macros and common false positives
                if cls in ('if', 'while', 'for', 'switch', 'return', 'sizeof',
                           'decltype', 'static_cast', 'reinterpret_cast',
                           'dynamic_cast', 'const_cast', 'IMPLEMENT', 'DECLARE',
                           'BEGIN', 'END', 'AFX', 'ON_COMMAND', 'ON_WM',
                           'std', 'CString', '__if_exists', '__if_not_exists'):
                    continue
                if method in ('cpp', 'h', 'c', 'NULL', 'nullptr'):
                    continue
                implemented[cls].add(method)
    
    return implemented


def find_header_for_class(include_dir: str, class_name: str) -> str:
    """Find which header declares a given class."""
    for root, dirs, files in os.walk(include_dir):
        for fn in files:
            if not fn.endswith('.h'):
                continue
            filepath = os.path.join(root, fn)
            try:
                with open(filepath) as f:
                    content = f.read()
            except:
                continue
            if re.search(rf'\bclass\s+{class_name}\s*[:{{\s]', content):
                return os.path.relpath(filepath, include_dir)
    return None


# Type code -> C++ default return value and C type
RETURN_DEFAULTS = {
    'void': ('', 'void'),
    'int': ('0', 'int'),
    'long': ('0L', 'long'),
    'unsigned int': ('0u', 'unsigned int'),
    'unsigned long': ('0UL', 'unsigned long'),
    'short': ('0', 'short'),
    'unsigned short': ('0', 'unsigned short'),
    'char': ('0', 'char'),
    'unsigned char': ('0', 'unsigned char'),
    'bool': ('false', 'bool'),
    '__int64': ('0LL', '__int64'),
    'unsigned __int64': ('0ULL', 'unsigned __int64'),
    'float': ('0.0f', 'float'),
    'double': ('0.0', 'double'),
    'wchar_t': ('0', 'wchar_t'),
}

# Parameters that can be forwarded directly
DIRECT_PARAM_TYPES = {
    'int', 'long', 'unsigned int', 'unsigned long', 'short', 'unsigned short',
    'char', 'unsigned char', 'bool', '__int64', 'unsigned __int64',
    'float', 'double', 'wchar_t',
}


def clean_type(t: str) -> str:
    """Clean up type string for C++ usage."""
    return t.replace(' /*class*/', '').replace(' /*struct*/', '').replace(' /*enum*/', '')


def generate_thunks(all_exports, implemented, include_dir) -> str:
    """Generate the thunks .cpp file."""
    
    # Load skip list from file (symbols that fail to compile as auto-generated thunks)
    _skip_file = Path(__file__).parent / 'thunks_skip.txt'
    SKIP_SYMBOLS = set()
    if _skip_file.exists():
        SKIP_SYMBOLS = set(line.strip() for line in _skip_file.read_text().splitlines() if line.strip() and not line.startswith('#'))
    
    # Also skip symbols that are already manually implemented in other source files
    # (they have // Symbol: comments in other phase4/src/*.cpp files)
    SOURCE_DIR = Path(__file__).parent.parent / 'phase4' / 'src'
    for src_file in SOURCE_DIR.glob('*.cpp'):
        if src_file.name == 'thunks.cpp':
            continue
        try:
            content = src_file.read_text(encoding='utf-8', errors='replace')
            for m in re.finditer(r'^// Symbol: (.+)$', content, re.MULTILINE):
                SKIP_SYMBOLS.add(m.group(1).strip())
            # Also extract stub names from extern "C" MS_ABI declarations
            for m in re.finditer(r'extern\s+"C".*?\b(impl_\w+)\s*\(', content):
                # We'll check against generated stub names later
                pass
        except:
            pass
    
    # Pre-compute set of existing stub names from manual source files
    EXISTING_STUBS = set()
    for src_file in SOURCE_DIR.glob('*.cpp'):
        if src_file.name == 'thunks.cpp':
            continue
        try:
            content = src_file.read_text(encoding='utf-8', errors='replace')
            for m in re.finditer(r'(impl_[A-Za-z0-9_]+)\s*\(', content):
                EXISTING_STUBS.add(m.group(1))
        except:
            pass
    
    # Collect which headers we need
    needed_headers = set()
    
    lines = []
    lines.append('// Auto-generated thunks — routes DLL exports through real C++ implementations')
    lines.append('// Generated by tools/gen_thunks.py')
    lines.append('// DO NOT EDIT')
    lines.append('')
    lines.append('#define OPENMFC_APPCORE_IMPL')
    lines.append('')
    lines.append('#ifdef __GNUC__')
    lines.append('  #define MS_ABI __attribute__((ms_abi))')
    lines.append('#else')
    lines.append('  #define MS_ABI')
    lines.append('#endif')
    lines.append('')
    
    # First pass: figure out which headers we need
    for entry in all_exports:
        symbol = entry.get('symbol', '')
        if not symbol.endswith('Z'):
            continue
        
        # Skip symbols that need manual thunks
        if symbol in SKIP_SYMBOLS:
            continue
        
        info = demangle(symbol)
        if not info.params and info.ret_type == 'void':
            continue
        
        # Extract class name from symbol (everything between @ and @@)
        # Pattern: ?method@ClassName@@...
        match = re.search(r'@(\w+)@@', symbol)
        if not match:
            # Constructor: ??0ClassName@@...
            match = re.search(r'\?\?0(\w+)@@', symbol)
        if not match:
            # Destructor: ??1ClassName@@...
            match = re.search(r'\?\?1(\w+)@@', symbol)
        if not match:
            # Template: ??$name@ClassName@@...
            match = re.search(r'@(\w+)@@[YQUS]', symbol)
        
        if match:
            class_name = match.group(1)
            if class_name in implemented:
                hdr = find_header_for_class(include_dir, class_name)
                if hdr:
                    needed_headers.add(hdr)
    
    # Write includes with proper ordering:
    # afxwin.h must come before afxdb.h, afxinet.h, afxsock.h
    # (they need the full CException and CFormView definitions from afxwin.h)
    priority_order = ['afxwin.h', 'afxole.h', 'afxmfc.h', 'afxdb.h', 'afxinet.h', 'afxsock.h', 'afx.h', 'afxstr.h']
    ordered = []
    for h in priority_order:
        if h in needed_headers:
            ordered.append(h)
    for h in sorted(needed_headers):
        if h not in ordered:
            ordered.append(h)
    for hdr in ordered:
        lines.append(f'#include "openmfc/{hdr}"')
    lines.append('')
    lines.append('// For memcpy')
    lines.append('#include <cstring>')
    lines.append('')
    
    thunk_count = 0
    error_count = 0
    
    for entry in all_exports:
        symbol = entry.get('symbol', '')
        if not symbol.endswith('Z'):
            continue
        
        # Skip symbols that need manual thunks
        if symbol in SKIP_SYMBOLS:
            continue
        
        info = demangle(symbol)
        if not info.params and info.ret_type == 'void' and '?' in info.symbol:
            error_count += 1
            continue
        
        # Extract class name
        match = re.search(r'@(\w+)@@', symbol)
        if not match:
            match = re.search(r'\?\?0(\w+)@@', symbol)
        if not match:
            match = re.search(r'\?\?1(\w+)@@', symbol)
        
        if not match:
            continue
        
        class_name = match.group(1)
        if class_name not in implemented:
            continue
        
        # Generate stub name (matching the typed stub convention)
        stub = re.sub(r'[?@]', '_', symbol)
        stub = re.sub(r'[^a-zA-Z0-9_]', '_', stub)
        if not stub[0].isalpha() and stub[0] != '_':
            stub = '_' + stub
        if len(stub) > 200:
            stub = stub[:100] + '_' + stub[-100:]
        stub_name = 'impl_' + stub
        
        # Skip if this stub name already exists in manual source files
        if stub_name in EXISTING_STUBS:
            continue
        
        # Figure out the C++ method name from the MSVC name
        # Extract method name (between initial ? and @)
        method_match = re.match(r'\?(\??\$?\w+)@', symbol)
        if not method_match:
            continue
        msvc_method = method_match.group(1)
        
        # Map MSVC mangled method names to C++ names
        method_name = msvc_method.lstrip('?$')
        
        # Check if we've implemented this method
        if method_name not in implemented[class_name]:
            # Try constructor (??0 = ctor)
            if symbol.startswith('??0') and class_name in implemented[class_name]:
                method_name = class_name
            # Try destructor (??1 = dtor)
            elif symbol.startswith('??1') and ('~' + class_name) in [m for m in implemented[class_name]]:
                method_name = '~' + class_name
            else:
                continue
        
        # Build parameter list for the thunk
        param_decls = []
        param_names = []
        for i, p in enumerate(info.params):
            ct = clean_type(p.c_type)
            pname = f'p{i}'
            param_decls.append(f'{ct} {pname}')
            param_names.append(pname)
        
        params_str = ', '.join(param_decls)
        
        # Clean return type
        ret = clean_type(info.ret_type)
        
        # Determine how to call the C++ method
        # For member functions, 'this' is passed as the first hidden parameter in x64
        # In the MS_ABI stub, we receive it as the first explicit parameter... 
        # Actually no. In our current typed stubs, parameters are already correct.
        # We just need to figure out the 'this' pointer.
        
        # In MSVC x64 ABI, 'this' is passed as the first integer register (RCX).
        # In our extern "C" functions, we don't have a 'this' pointer.
        # We need to receive all parameters as explicit args and then call the C++ method.
        
        # For simple forwarding: cast first void* param to ClassName* and call method
        # But this doesn't work for static methods or global functions.
        
        # Actually, let me take a simpler approach:
        # For each demangled symbol, generate a thunk that:
        # 1. Takes all parameters explicitly (as per MSVC ABI)
        # 2. Calls the C++ method with those parameters
        # 3. For member functions, the first parameter IS 'this' in the correct position
        
        # The key insight: in MSVC x64, member functions receive 'this' in RCX.
        # In our extern "C" MS_ABI function, the first parameter goes to RCX too.
        # So we just need to declare the first param as ClassName* instead of void*.
        
        # But wait — our demangler already parses parameters from the mangled name.
        # For member functions, the mangled name does NOT include 'this'.
        # So our param list from the demangler is the EXPLICIT params without 'this'.
        # The 'this' pointer is implicit in the MSVC calling convention.
        
        # In our extern "C" thunk, we need to ADD 'this' as the first parameter.
        
        # Determine function type
        is_static = info.is_static
        is_constructor = symbol.startswith('??0')
        is_destructor = symbol.startswith('??1')
        is_global = symbol.startswith('??$') and '@@YA' in symbol
        
        # Build the parameter names for calling
        call_params = []
        for i, p in enumerate(info.params):
            pname = f'p{i}'
            ct = clean_type(p.c_type)
            if p.is_reference and ct not in ('void*', 'const void*', 'void'):
                # Reference params arrive as pointers in extern "C", dereference for C++ call
                call_params.append(f'(*{pname})')
            else:
                call_params.append(pname)
        
        call_str = ', '.join(call_params)
        
        # Default return
        if ret in RETURN_DEFAULTS:
            default_val, c_ret = RETURN_DEFAULTS[ret]
        elif '*' in ret:
            default_val = 'nullptr'
            c_ret = ret
        else:
            default_val = '0'
            c_ret = 'void*'
        
        # Generate the thunk body based on function type
        if is_constructor and class_name:
            # Constructor: use placement new, return this
            lines.append(f'// Symbol: {symbol}')
            lines.append(f'// Constructor: {class_name}::{class_name}')
            
            if params_str:
                thunk_params = f'void* pThis, {params_str}'
            else:
                thunk_params = 'void* pThis'
            
            lines.append(f'extern "C" void* MS_ABI {stub_name}({thunk_params}) {{')
            if call_str:
                lines.append(f'    return new(pThis) {class_name}({call_str});')
            else:
                lines.append(f'    return new(pThis) {class_name}();')
            lines.append(f'}}')
        
        elif is_destructor and class_name:
            # Destructor: call destructor, return nothing
            lines.append(f'// Symbol: {symbol}')
            lines.append(f'// Destructor: {class_name}::~{class_name}')
            
            lines.append(f'extern "C" void MS_ABI {stub_name}(void* pThis) {{')
            lines.append(f'    (({class_name}*)pThis)->~{class_name}();')
            lines.append(f'}}')
        
        elif is_static and class_name:
            # Static method
            lines.append(f'// Symbol: {symbol}')
            lines.append(f'// Static: {class_name}::{method_name}')
            
            thunk_params = params_str
            lines.append(f'extern "C" {c_ret} MS_ABI {stub_name}({thunk_params}) {{')
            if ret == 'void':
                lines.append(f'    {class_name}::{method_name}({call_str});')
            else:
                lines.append(f'    return ({c_ret}){class_name}::{method_name}({call_str});')
            lines.append(f'}}')
        
        elif is_global:
            # Global function
            lines.append(f'// Symbol: {symbol}')
            thunk_params = params_str
            lines.append(f'extern "C" {c_ret} MS_ABI {stub_name}({thunk_params}) {{')
            if ret == 'void':
                lines.append(f'    {method_name}({call_str});')
            else:
                lines.append(f'    return ({c_ret}){method_name}({call_str});')
            lines.append(f'}}')
        
        elif class_name:
            # Regular member function
            lines.append(f'// Symbol: {symbol}')
            lines.append(f'// {class_name}::{method_name}')
            
            this_type = f'const {class_name}*' if info.is_const_method else f'{class_name}*'
            
            if params_str:
                thunk_params = f'{this_type} pThis, {params_str}'
            else:
                thunk_params = f'{this_type} pThis'
            
            lines.append(f'extern "C" {c_ret} MS_ABI {stub_name}({thunk_params}) {{')
            if ret == 'void':
                lines.append(f'    pThis->{method_name}({call_str});')
            elif '*' in ret:
                lines.append(f'    return ({c_ret})pThis->{method_name}({call_str});')
            else:
                lines.append(f'    return ({c_ret})pThis->{method_name}({call_str});')
            lines.append(f'}}')
        
        lines.append('')
        thunk_count += 1
    
    lines.append(f'// Generated {thunk_count} thunks')
    lines.append(f'// Errors (could not parse): {error_count}')
    
    return '\n'.join(lines)


def main():
    parser = argparse.ArgumentParser(description="Generate thunks for OpenMFC exports")
    parser.add_argument("--mapping", required=True, help="Path to ordinal mapping JSON")
    parser.add_argument("--source-dir", default="phase4/src", help="Source directory to scan")
    parser.add_argument("--include-dir", default="include/openmfc", help="Include directory")
    parser.add_argument("--out", required=True, help="Output .cpp file")
    args = parser.parse_args()
    
    # Scan implemented methods
    implemented = scan_implemented_methods(args.source_dir)
    total = sum(len(v) for v in implemented.values())
    print(f"Found {total} implemented methods across {len(implemented)} classes")
    
    # Load mapping
    mapping_path = Path(args.mapping)
    if not mapping_path.exists():
        print(f"Error: Mapping file not found: {mapping_path}", file=sys.stderr)
        return 1
    
    data = json.loads(mapping_path.read_text())
    all_exports = data.get('exports', {}).get('mfc140u', [])
    print(f"Loaded {len(all_exports)} exports")
    
    # Generate thunks
    content = generate_thunks(all_exports, implemented, args.include_dir)
    
    Path(args.out).parent.mkdir(parents=True, exist_ok=True)
    Path(args.out).write_text(content, encoding="ascii", errors="replace")
    
    lines = content.count('\n')
    print(f"Generated: {args.out} ({lines} lines, {len(content)} bytes)")


if __name__ == "__main__":
    main()
