#!/usr/bin/env python3
"""
MSVC x64 Symbol Demangler v2

Demangles Microsoft Visual C++ x64 mangled names.
Focuses on extracting: return type, parameter count, and basic parameter types.
Sufficient for generating properly-typed extern "C" stubs.

Key MSVC x64 mangling rules:
- Everything starts with ?
- Template functions: ??$name@args@@...
- Class members: ?name@class@@<access><ret>params@Z
- Global functions: ?name@@YA<ret>params@Z
- Data: ?name@class@@2<type>A or ?name@@3<type>A
- @@ separates name from type information
- @Z terminates function signatures
- V<classname>@@ = class type
- U<structname>@@ = struct type
- 0-9 = back-references to previous types
- PEA = pointer (x64), PEB = const pointer (x64)
- AEA = reference (x64), AEB = const reference (x64)

Type codes:
  X=void, D=char, E=uchar, F=short, G=ushort,
  H=int, I=uint, J=long, K=ulong, M=float, N=double,
  _N=bool, _J=__int64, _K=unsigned __int64, _W=wchar_t
"""

import re
from dataclasses import dataclass, field
from typing import List, Tuple, Optional


@dataclass
class ParamInfo:
    """Simple parameter info — just enough to generate a stub."""
    c_type: str  # C type to use in stub declaration
    is_reference: bool = False  # True if originally a C++ reference (AEA/AEB)


@dataclass
class FuncInfo:
    """Demangled function info."""
    symbol: str
    ret_type: str = "void"
    params: List[ParamInfo] = field(default_factory=list)
    is_data: bool = False
    data_type: str = "unsigned int"
    is_static: bool = False
    is_virtual: bool = False
    is_const_method: bool = False
    success: bool = False   # True if the demangler successfully parsed the symbol


# Module-level flag: when True, V/U/W4 handlers try to resolve type names.
# Set by thunk generators that need proper C++ types.
# Default False (safe for typed stub generators that work with void*).
RESOLVE_TYPE_NAMES = False

# Type code to C type mapping
TYPE_MAP = {
    'X': 'void', 'D': 'char', 'E': 'unsigned char',
    'F': 'short', 'G': 'unsigned short', 'H': 'int',
    'I': 'unsigned int', 'J': 'long', 'K': 'unsigned long',
    'M': 'float', 'N': 'double',
}

EXT_TYPE_MAP = {
    'N': 'bool', 'J': '__int64', 'K': 'unsigned __int64', 'W': 'wchar_t',
}

# Types that are self-contained (don't need class name parsing)
PRIMITIVE_TYPES = set('XDEFGHIJKMNW')


def _consume_class_name(s: str, pos: int) -> int:
    """Skip a class/struct name in the mangled string.
    Class names end with @@ or at the next @ that isn't part of a nested name."""
    # A class name like "VCOleVariant" is followed by "@@"
    # But nested templates like "V?$CStringT@..." have internal @
    # We need to find the closing @@
    
    # Actually, in MSVC mangling, a class name appearing in the parameter list
    # after the main @@ is always followed by @@ (its own closing).
    # So we just find the next @@.
    
    idx = s.find('@@', pos)
    if idx != -1:
        return idx + 2
    return pos + 1


def _extract_type_name(s: str, pos: int) -> Tuple[str, int]:
    """Extract a C++ type name from the mangled string at pos.
    Returns (type_name, new_pos), or ('void', new_pos) if unresolvable.
    Uses 'void' (not 'void*') so pointer/reference wrappers add their own *."""
    # Back-reference digit (0-9) — unresolvable without context
    if pos < len(s) and s[pos].isdigit():
        return 'void', pos + 1
    # Template names start with ? — can't use as C++ identifiers
    if pos < len(s) and s[pos] in '?$':
        idx = s.find('@@', pos)
        if idx != -1:
            return 'void', idx + 2
        return 'void', pos + 1
    # Find the terminating @@
    idx = s.find('@@', pos)
    if idx != -1:
        name = s[pos:idx]
        # Names containing @ are namespace-qualified (e.g., CTime@ATL)
        # Names starting with __ are MSVC-internal (e.g., __POSITION)
        if '@' in name or name.startswith('__'):
            return 'void', idx + 2
        return name, idx + 2
    # No @@ found — single-@ termination (for simple names or W4 enums)
    idx = s.find('@', pos)
    if idx != -1:
        name = s[pos:idx]
        if '@' in name or name.startswith('__') or '?' in name:
            return 'void', idx + 1
        return name, idx + 1
    return 'void', pos + 1


def _parse_params(s: str, pos: int) -> Tuple[List[ParamInfo], int]:
    """Parse parameter list starting at pos. Returns (params, new_pos).
    Stops at @Z or Z (end of function) or end of string."""
    params = []
    type_table = []  # MSVC back-reference table (index 0 = first unique type)
    
    while pos < len(s):
        c = s[pos]
        
        # End markers
        if c == 'Z':
            break
        if c == '@':
            if pos + 1 < len(s) and s[pos + 1] == 'Z':
                break
            # Standalone @ is a parameter separator or part of a name
            pos += 1
            continue
        
        # Check if this parameter is a reference
        # Reference markers: AEA, AEB, AA, AB (A followed by A/E, then A/B)
        is_ref = False
        if c == 'A':
            nxt = s[pos + 1] if pos + 1 < len(s) else ''
            if nxt in ('A', 'E'):
                is_ref = True
        
        # Back-reference (0-9): reuse a type from the type table
        if c.isdigit():
            ref_idx = int(c)
            if ref_idx < len(type_table):
                ref_type, ref_is_ref = type_table[ref_idx]
                params.append(ParamInfo(c_type=ref_type, is_reference=ref_is_ref))
            else:
                # Back-reference to unknown type — fall back to void*
                params.append(ParamInfo(c_type='void*', is_reference=False))
            pos += 1
        else:
            # Parse one parameter type
            param_str, pos = _parse_one_type(s, pos)
            if param_str and param_str != 'void':
                params.append(ParamInfo(c_type=param_str, is_reference=is_ref))
                # Add to type table for back-reference resolution
                # Only add if not already in table (MSVC deduplicates)
                entry = (param_str, is_ref)
                if entry not in type_table:
                    type_table.append(entry)
        
        # Skip @ separators between params
        while pos < len(s) and s[pos] == '@':
            if pos + 1 < len(s) and s[pos + 1] == 'Z':
                pos += 2
                break
            pos += 1
    
    return params, pos


def _parse_one_type(s: str, pos: int) -> Tuple[str, int]:
    """Parse one type starting at pos. Returns (c_type_string, new_pos)."""
    if pos >= len(s):
        return 'void', pos
    
    c = s[pos]
    
    # Primitive types
    if c in TYPE_MAP:
        return TYPE_MAP[c], pos + 1
    
    # Extended types (_N, _J, _K, _W)
    if c == '_':
        if pos + 1 < len(s) and s[pos + 1] in EXT_TYPE_MAP:
            return EXT_TYPE_MAP[s[pos + 1]], pos + 2
        return 'int', pos + 1
    
    # Pointer types
    if c == 'P':
        if pos + 1 >= len(s):
            return 'void*', pos + 1
        
        c2 = s[pos + 1]
        const = False
        
        # Function pointer: P6...Z or P8...Z
        # These should NOT get another * appended — P6/P8 already denotes
        # a function pointer (itself a valid C type like `void(*)(void*)`).
        if c2 == '6' or c2 == '8':
            # Skip to the Z that closes the function type (P6 rtype params @Z)
            idx = s.find('Z', pos + 2)
            if idx != -1:
                return 'void* /*fnptr*/', idx + 1
            return 'void*', pos + 2
        
        if c2 == 'E' and pos + 2 < len(s) and s[pos + 2] == 'A':  # PEA
            inner, new_pos = _parse_one_type(s, pos + 3)
            const = False
        elif c2 == 'E' and pos + 2 < len(s) and s[pos + 2] == 'B':  # PEB
            inner, new_pos = _parse_one_type(s, pos + 3)
            const = True
        elif c2 == 'A':  # PA
            inner, new_pos = _parse_one_type(s, pos + 2)
            const = False
        elif c2 == 'B':  # PB
            inner, new_pos = _parse_one_type(s, pos + 2)
            const = True
        else:
            inner, new_pos = _parse_one_type(s, pos + 1)
            const = False
        
        prefix = "const " if const else ""
        return f"{prefix}{inner}*", new_pos
    
    # Reference types
    if c == 'A':
        if pos + 1 >= len(s):
            return 'void*', pos + 1
        
        c2 = s[pos + 1]
        const = False
        
        if c2 == 'E' and pos + 2 < len(s) and s[pos + 2] == 'A':  # AEA
            inner, new_pos = _parse_one_type(s, pos + 3)
        elif c2 == 'E' and pos + 2 < len(s) and s[pos + 2] == 'B':  # AEB
            inner, new_pos = _parse_one_type(s, pos + 3)
            const = True
        elif c2 == 'A':  # AA
            inner, new_pos = _parse_one_type(s, pos + 2)
        elif c2 == 'B':  # AB
            inner, new_pos = _parse_one_type(s, pos + 2)
            const = True
        else:
            inner, new_pos = _parse_one_type(s, pos + 1)
        
        # References are passed as pointers in extern "C" stubs
        prefix = "const " if const else ""
        return f"{prefix}{inner}*", new_pos
    
    # Class type (V)
    if c == 'V':
        if RESOLVE_TYPE_NAMES:
            type_name, new_pos = _extract_type_name(s, pos + 1)
            return type_name, new_pos
        new_pos = _consume_class_name(s, pos + 1)
        return f"void* /*class*/", new_pos
    
    # Struct type (U)
    if c == 'U':
        if RESOLVE_TYPE_NAMES:
            type_name, new_pos = _extract_type_name(s, pos + 1)
            if type_name.startswith('tag') and type_name != 'void':
                type_name = type_name[3:]
            return type_name, new_pos
        new_pos = _consume_class_name(s, pos + 1)
        return f"void* /*struct*/", new_pos
    
    # Enum type (W4) - enums are int-sized in MSVC
    if c == 'W':
        if pos + 1 < len(s) and s[pos + 1] == '4':
            if RESOLVE_TYPE_NAMES:
                type_name, new_pos = _extract_type_name(s, pos + 2)
                if type_name == 'void':
                    return 'int', new_pos
                return type_name, new_pos
            return 'int /*enum*/', pos + 2
        new_pos = _consume_class_name(s, pos + 1)
        return 'int /*enum*/', new_pos
    
    # Function pointer (P6...Z)
    # P6 = function pointer prefix, then return type, params, @Z, then the actual Z
    # NOTE: must be checked before digit (back-reference) because 6 and 8 are digits too
    if c == '6' or c == '8':
        # Just skip to the next Z
        idx = s.find('Z', pos)
        if idx != -1:
            return 'void* /*fnptr*/', idx + 1
        return 'void*', pos + 1
    
    # Back-reference (0-9) — must come after 6/8 check above
    if c.isdigit():
        return 'void*', pos + 1
    
    # Unknown — skip
    return 'void*', pos + 1


def demangle(symbol: str) -> FuncInfo:
    """Demangle an MSVC x64 symbol and return FuncInfo."""
    result = FuncInfo(symbol=symbol)
    
    if not symbol.startswith('?'):
        return result
    
    # Data symbols: ?name@Class@@<access><type>A or ?name@@<access><type>A
    # Access levels: 0=private, 1=protected, 2=public, 3=global
    # Data symbols don't end with Z and have @@[0-9] followed by type code
    if not symbol.endswith('Z'):
        data_match = re.search(r'@@([0-9])([A-Z][A-Z0-9_]*)', symbol)
        if data_match:
            result.is_data = True
            result.success = True
            type_code = data_match.group(2)
            if type_code.startswith('I'): result.data_type = 'unsigned int'
            elif type_code.startswith('H'): result.data_type = 'int'
            elif type_code.startswith('J'): result.data_type = 'long'
            elif type_code.startswith('K'): result.data_type = 'unsigned long'
            elif type_code.startswith('N'): result.data_type = 'double'
            elif type_code.startswith('M'): result.data_type = 'float'
            elif type_code.startswith('P'): result.data_type = 'void*'
            elif type_code.startswith('V'): result.data_type = 'void* /*CRuntimeClass*/'
            return result
    
    # Find the boundary @@ — the one just before the calling convention markers.
    # Pattern: @@YA..., @@QE..., @@UE..., @@SA..., @@SE..., @@YG..., @@YI...
    # The boundary @@ is followed by [YQUS] [AEIG] 
    
    # Access: Q=public, A=private, C=protected, I=internal, E=private virtual,
    #         M=protected virtual, S=static, U=public virtual, Y=global,
    #         K=static __based/__ptr64
    # Convention: A=x64, E=x64 virtual, G=vectorcall, I=clrcall
    # Single-letter legacy: Q=member, U=virtual, S=static
    cc_pattern = re.compile(r'@@([YQUSIMACEBK])([AEIG]?)')
    match = cc_pattern.search(symbol)
    if not match:
        # Try older style / operators without @@: ?name@YA..., ??N@YA...
        cc_pattern2 = re.compile(r'(?:@|@@)([YQUSIMACEBK])([AEIG]?)')
        match = cc_pattern2.search(symbol)
    
    if not match:
        return result
    
    boundary_pos = match.start()  # Position of the @@ boundary
    tail = symbol[boundary_pos + 2:]  # Everything after the boundary @@
    
    # Parse calling convention from tail
    pos = 0
    cc_found = False
    
    # Two-letter markers
    two_letter = ['YA', 'YG', 'YI', 'QE', 'UE', 'SE', 'SA', 'IE', 'ME',
                  'AE', 'AA', 'CE', 'CA', 'EE', 'EA',
                  'KE', 'KA', 'KI', 'KG']
    for marker in two_letter:
        if tail.startswith(marker):
            pos = len(marker)
            cc_found = True
            break
    
    if not cc_found:
        # Single-letter markers: Q, U, S, A, B, C, E, I, M, K
        if tail and tail[0] in 'QUSABCEIMK':
            pos = 1
            cc_found = True
    
    if not cc_found:
        return result
    
    # Set flags based on calling convention
    matched_cc = tail[:pos]
    if matched_cc.startswith('S') or matched_cc.startswith('K'):
        result.is_static = True
    if matched_cc.startswith('U'):
        result.is_virtual = True
    
    # After calling convention, skip x64 'A' markers and 'B' const qualifiers.
    # In x64 MSVC, after UE/QE/SA/etc., there are up to 3 marker chars (A, B)
    # before the return type begins. Reference types (AA, AEA, AEB) only appear
    # as actual types, never as markers.
    # Strategy: greedily skip all consecutive A and B characters.
    # Track if we saw 'B' (const method)
    
    while pos < len(tail) and tail[pos] in 'AB':
        if tail[pos] == 'B':
            result.is_const_method = True
        pos += 1
    
    # Parse return type
    ret_str, pos = _parse_one_type(tail, pos)
    if ret_str:
        result.ret_type = ret_str
    
    # Parse parameters
    result.params, _ = _parse_params(tail, pos)
    
    result.success = True
    return result


#=============================================================================
# Tests
#=============================================================================
if __name__ == "__main__":
    tests = [
        # Simple member functions
        "?GetRuntimeClass@CObject@@UEBAPEAUCRuntimeClass@@XZ",
        "?IsKindOf@CObject@@QEBAHPEBUCRuntimeClass@@@Z",
        "?Serialize@CObject@@UEAAXAEAVCArchive@@@Z",
        
        # Global functions
        "??$CopyElements@VCOleVariant@@@@YAXPEAVCOleVariant@@PEBV0@_J@Z",
        "??$HashKey@PEB_W@@YAIPEB_W@Z",
        "??$HashKey@AEBUtagVARIANT@@@@YAIAEBUtagVARIANT@@@Z",
        "?AfxThrowMemoryException@@YAXXZ",
        
        # Complex signatures
        "?AfxFormatString1@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEB_W@Z",
        "?OnDrawMenuBorder@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z",
        "?LoadFrame@CFrameWnd@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z",
        
        # Constructors/destructors
        "??0CWinApp@@QEAA@PEB_W@Z",
        "??1CWinApp@@UEAA@XZ",
        
        # Feature Pack
        "?GetInstance@CMFCVisualManager@@SAPEAV1@XZ",
        "?AddItem@CMFCDynamicLayout@@QEAAHIUMoveSettings@1@USizeSettings@1@@Z",
        
        # Data
        "?classCObject@CObject@@2UCRuntimeClass@@A",
        "?AFX_TAB_IMAGE_MARGIN@CMFCBaseTabCtrl@@2HA",
    ]
    
    for s in tests:
        info = demangle(s)
        if info.is_data:
            print(f"D  [{info.data_type}] {s[:90]}")
        else:
            p = ", ".join(p.c_type for p in info.params)
            print(f"F  {info.ret_type} ({p})  {s[:90]}")
