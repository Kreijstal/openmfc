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
    Returns dict: className -> {methodName: return_type or None}."""
    
    implemented = defaultdict(dict)
    
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
            #   ReturnType OuterClass::InnerClass::InnerClass(  (nested class ctor)
            #   ClassName::~ClassName(
            for m in re.finditer(r'(?:^|\n)\s*((?:[\w:<>*&\s]+?)\s+)?([\w:]+)::(\w+|~\w+)\s*\(', content):
                full_cls = m.group(2)  # may contain :: for nested classes
                method = m.group(3)
                ret_type = (m.group(1) or 'void').strip()
                # Strip common qualifiers
                ret_type = ret_type.split()[-1] if ret_type else 'void'
                # For nested classes, use the simple (last) class name for lookup
                simple_cls = full_cls.split('::')[-1]
                # Also store qualified version
                if '::' in full_cls:
                    implemented[full_cls][method] = ret_type
                implemented[simple_cls][method] = ret_type
                # Skip macros and common false positives
                if simple_cls in ('if', 'while', 'for', 'switch', 'return', 'sizeof',
                           'decltype', 'static_cast', 'reinterpret_cast',
                           'dynamic_cast', 'const_cast', 'IMPLEMENT', 'DECLARE',
                           'BEGIN', 'END', 'AFX', 'ON_COMMAND', 'ON_WM',
                           'std', 'CString', '__if_exists', '__if_not_exists'):
                    continue
                if method in ('cpp', 'h', 'c', 'NULL', 'nullptr'):
                    continue
    
    return implemented


def find_header_for_class(include_dir: str, class_name: str) -> str:
    """Find which header declares a given class.
    For nested classes (Outer::Inner), searches for the Outer class."""
    search_name = class_name.split('::')[-1]  # Use simple name for lookup
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
            if re.search(rf'\bclass\s+{search_name}\s*[:{{\s]', content):
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
        if not info.success:
            error_count += 1
            continue
        
        # Extract class name (supports nested classes: Outer::Inner)
        match = re.search(r'@(\w+)@(\w+)@@', symbol)  # nested: Inner@Outer@@
        if match:
            class_name = match.group(2) + '::' + match.group(1)  # Outer::Inner
        else:
            match = re.search(r'@(\w+)@@', symbol)
            if not match:
                match = re.search(r'\?\?0(\w+)@@', symbol)
            if not match:
                match = re.search(r'\?\?1(\w+)@@', symbol)
            if match:
                class_name = match.group(1)
        
        if not match:
            continue
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
        
        # CString return-by-value detection: @@<convention><ret>...?AV?$CStringT...
        # CString ref params: @@<convention><ret>...AEAV?$CStringT... or ...AEBV?$CStringT...
        is_cstring_retval = False
        has_cstring_ref = False
        is_retval = False  # General return-by-value (enum, struct, class by value)
        retval_ctype = ''  # C++ type name for the return value
        tail_match = re.search(r'@@([A-Z].*)$', symbol)
        if tail_match:
            tail = tail_match.group(1)
            # CString in return type: appears RIGHT after calling convention markers,
            # as ?AV?$CStringT... before any parameter separators (@)
            if '?AV?$CStringT' in tail:
                is_cstring_retval = True
            # CString reference param: AEAV?$CStringT or AEBV?$CStringT in params area
            if 'AEAV?$CStringT' in tail or 'AEBV?$CStringT' in tail:
                has_cstring_ref = True
            # General return-by-value: ?AV (class), ?AU (struct), ?AW4 (enum) in return position
            # Pattern: @@<access/digits>?A<V|U|W4><type>...
            if not is_cstring_retval:
                retval_pattern = re.match(r'[A-Z0-9]+\?A([VUW]\d?)', tail)
                if retval_pattern:
                    is_retval = True
                    # Extract the C++ type name from the mangled symbol
                    # Look for the class/struct/enum name after ?AV/?AU/?AW4
                    type_match = re.search(r'\?A[VUW]\d?(.+?)@', tail)
                    if type_match:
                        raw_type = type_match.group(1)
                        # Clean up: $ for templates, simple names pass through
                        retval_ctype = raw_type.replace('$', '')
        # Also check full symbol for ref params (in case tail match fails)
        if 'AEAV?$CStringT' in symbol or 'AEBV?$CStringT' in symbol:
            has_cstring_ref = True

        # Figure out the C++ method name from the MSVC name
        # Extract method name (between initial ? and @)
        method_match = re.match(r'\?(\??\$?\w+)@', symbol)
        if not method_match:
            continue
        msvc_method = method_match.group(1)
        
        # Map MSVC mangled method names to C++ names
        method_name = msvc_method.lstrip('?$')
        
        # Check if we've implemented this method
        method_is_void = False
        if method_name not in implemented[class_name]:
            # Try constructor (??0 = ctor)
            if symbol.startswith('??0') and class_name in implemented[class_name]:
                method_name = class_name
            # Try destructor (??1 = dtor)
            elif symbol.startswith('??1') and ('~' + class_name) in implemented[class_name]:
                method_name = '~' + class_name
            else:
                continue
        
        # Check if the actual C++ method returns void (vs what MSVC demangling says)
        method_ret = implemented[class_name].get(method_name, '')
        method_is_void = (method_ret == 'void')
        
        # Build parameter list for the thunk
        param_decls = []
        param_names = []
        for i, p in enumerate(info.params):
            ct = clean_type(p.c_type)
            pname = f'p{i}'
            param_decls.append(f'{ct} {pname}')
            param_names.append(pname)
        
        # Collapse CString reference artifact triplets (void*, void**, void*)
        # into a single CString& parameter.
        cstring_call_names = None  # if set, use these for call_params
        if has_cstring_ref and not is_cstring_retval:
            new_decls = []
            new_names = []
            call_names = []
            i = 0
            param_idx = 0  # index into original info.params
            while i < len(param_decls):
                # Check for (void*, void**, void*) triplet
                if (i + 2 < len(param_decls) and
                    'void*' in param_decls[i] and
                    'void**' in param_decls[i+1] and
                    'void*' in param_decls[i+2]):
                    is_const_cstring = 'AEBV?$CStringT' in symbol
                    ct_str = 'const CString*' if is_const_cstring else 'CString*'
                    new_decls.append(f'{ct_str} p{i}')
                    new_names.append(f'p{i}')
                    call_names.append(f'(*p{i})')
                    i += 3
                    param_idx += 3
                else:
                    new_decls.append(param_decls[i])
                    new_names.append(param_names[i])
                    # For non-CString params, dereference if original was a reference
                    if param_idx < len(info.params) and info.params[param_idx].is_reference:
                        call_names.append(f'(*{param_names[i]})')
                    else:
                        call_names.append(param_names[i])
                    i += 1
                    param_idx += 1
            
            param_decls = new_decls
            param_names = new_names
            cstring_call_names = call_names
        
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
        if cstring_call_names is not None:
            call_params = list(cstring_call_names)
        else:
            call_params = []
            for i, p in enumerate(info.params):
                pname = f'p{i}'
                ct = clean_type(p.c_type)
                if p.is_reference and ct not in ('void*', 'const void*', 'void'):
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
            if is_retval:
                # Static with return-by-value: hidden return pointer is first param
                lines.append(f'// Symbol: {symbol}')
                lines.append(f'// Static: {class_name}::{method_name}  [retval]')
                
                # Determine the C++ return type name
                cpp_ret_type = retval_ctype
                if not cpp_ret_type and info.params:
                    ptype = info.params[0].c_type
                    cpp_ret_type = ptype.replace('*', '').replace(' /*class*/', '').replace(' /*struct*/', '').replace(' /*enum*/', '').replace(' ', '').strip()
                
                # Skip hidden return pointer (index 0), rest are real params
                real_params = info.params[1:] if len(info.params) >= 1 else []
                retval_param_parts = [f'{cpp_ret_type}* __ret']
                retval_call_args = []
                for i, p in enumerate(real_params):
                    ct = clean_type(p.c_type)
                    retval_param_parts.append(f'{ct} p{i}')
                    if p.is_reference and ct not in ('void*', 'const void*', 'void'):
                        retval_call_args.append(f'(*p{i})')
                    else:
                        retval_call_args.append(f'p{i}')
                retval_params_str = ', '.join(retval_param_parts)
                retval_call_str = ', '.join(retval_call_args)
                
                lines.append(f'extern "C" void MS_ABI {stub_name}({retval_params_str}) {{')
                if retval_call_str:
                    lines.append(f'    *__ret = {class_name}::{method_name}({retval_call_str});')
                else:
                    lines.append(f'    *__ret = {class_name}::{method_name}();')
                lines.append(f'}}')
            else:
                lines.append(f'// Symbol: {symbol}')
                lines.append(f'// Static: {class_name}::{method_name}')
                
                thunk_params = params_str
                lines.append(f'extern "C" {c_ret} MS_ABI {stub_name}({thunk_params}) {{')
                if ret == 'void' or method_is_void:
                    lines.append(f'    {class_name}::{method_name}({call_str});')
                    if ret != 'void':
                        lines.append(f'    return {{}};')
                else:
                    lines.append(f'    return ({c_ret}){class_name}::{method_name}({call_str});')
                lines.append(f'}}')
        
        elif is_global:
            # Global function
            lines.append(f'// Symbol: {symbol}')
            thunk_params = params_str
            lines.append(f'extern "C" {c_ret} MS_ABI {stub_name}({thunk_params}) {{')
            if ret == 'void' or method_is_void:
                lines.append(f'    {method_name}({call_str});')
                if ret != 'void':
                    lines.append(f'    return {{}};')
            else:
                lines.append(f'    return ({c_ret}){method_name}({call_str});')
            lines.append(f'}}')
        
        elif class_name:
            # Regular member function
            
            # CString return-by-value: hidden return pointer in RDX.
            # The demangler produces artifact params (void*, void**, void*) from the
            # CString template name.  Real explicit params come after those.
            if is_cstring_retval:
                lines.append(f'// Symbol: {symbol}')
                lines.append(f'// {class_name}::{method_name}  [CString retval]')
                this_type = f'const {class_name}*' if info.is_const_method else f'{class_name}*'
                
                # Skip CString template artifact params (first 3)
                real_params = info.params[3:] if len(info.params) >= 3 else []
                
                # Build explicit param declarations and call args
                cstring_param_parts = [f'{this_type} pThis', 'CString* __ret']
                cstring_call_args = []
                for i, p in enumerate(real_params):
                    ct = clean_type(p.c_type)
                    cstring_param_parts.append(f'{ct} p{i}')
                    if p.is_reference and ct not in ('void*', 'const void*', 'void'):
                        cstring_call_args.append(f'(*p{i})')
                    else:
                        cstring_call_args.append(f'p{i}')
                cstring_params_str = ', '.join(cstring_param_parts)
                cstring_call_str = ', '.join(cstring_call_args)
                
                lines.append(f'extern "C" void MS_ABI {stub_name}({cstring_params_str}) {{')
                if cstring_call_str:
                    lines.append(f'    *__ret = pThis->{method_name}({cstring_call_str});')
                else:
                    lines.append(f'    *__ret = pThis->{method_name}();')
                lines.append(f'}}')
            
            elif is_retval:
                # General return-by-value (enum, struct, class): hidden return pointer
                # is the first parameter. Real explicit params start at index 1.
                lines.append(f'// Symbol: {symbol}')
                lines.append(f'// {class_name}::{method_name}  [retval]')
                this_type = f'const {class_name}*' if info.is_const_method else f'{class_name}*'
                
                # Determine the actual C++ return type name
                cpp_ret_type = retval_ctype
                if not cpp_ret_type and info.params:
                    # Fallback: extract from demangler param comment
                    ptype = info.params[0].c_type
                    cpp_ret_type = ptype.replace('*', '').replace(' /*class*/', '').replace(' /*struct*/', '').replace(' /*enum*/', '').replace(' ', '').strip()
                    if cpp_ret_type == 'void':
                        cpp_ret_type = 'void'
                
                # Skip the hidden return pointer param (index 0)
                real_params = info.params[1:] if len(info.params) >= 1 else []
                
                # Build explicit param declarations and call args
                retval_param_parts = [f'{this_type} pThis', f'{cpp_ret_type}* __ret']
                retval_call_args = []
                for i, p in enumerate(real_params):
                    ct = clean_type(p.c_type)
                    retval_param_parts.append(f'{ct} p{i}')
                    if p.is_reference and ct not in ('void*', 'const void*', 'void'):
                        retval_call_args.append(f'(*p{i})')
                    else:
                        retval_call_args.append(f'p{i}')
                retval_params_str = ', '.join(retval_param_parts)
                retval_call_str = ', '.join(retval_call_args)
                
                lines.append(f'extern "C" void MS_ABI {stub_name}({retval_params_str}) {{')
                if retval_call_str:
                    lines.append(f'    *__ret = pThis->{method_name}({retval_call_str});')
                else:
                    lines.append(f'    *__ret = pThis->{method_name}();')
                lines.append(f'}}')
            
            else:
                lines.append(f'// Symbol: {symbol}')
                lines.append(f'// {class_name}::{method_name}')
            
                this_type = f'const {class_name}*' if info.is_const_method else f'{class_name}*'
                
                if params_str:
                    thunk_params = f'{this_type} pThis, {params_str}'
                else:
                    thunk_params = f'{this_type} pThis'
                
                lines.append(f'extern "C" {c_ret} MS_ABI {stub_name}({thunk_params}) {{')
                if ret == 'void' or method_is_void:
                    lines.append(f'    pThis->{method_name}({call_str});')
                    if ret != 'void':
                        lines.append(f'    return {{}};')
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
