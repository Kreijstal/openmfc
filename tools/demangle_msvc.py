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


@dataclass
class FuncInfo:
    """Demangled function info."""
    symbol: str
    ret_type: str = "void"
    params: List[ParamInfo] = field(default_factory=list)
    is_data: bool = False
    data_type: str = "unsigned int"


#=============================================================================
# Type parser state machine
#=============================================================================

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


def _parse_params(s: str, pos: int) -> Tuple[List[ParamInfo], int]:
    """Parse parameter list starting at pos. Returns (params, new_pos).
    Stops at @Z or Z (end of function) or end of string."""
    params = []
    
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
        
        # Parse one parameter type
        param_str, pos = _parse_one_type(s, pos)
        if param_str and param_str != 'void':
            params.append(ParamInfo(c_type=param_str))
        
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
        # If inner is a class type, simplify to void*
        if '/*' in inner:
            inner = 'void'
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
        if '/*' in inner:
            inner = 'void'
        return f"{prefix}{inner}*", new_pos
    
    # Class type (V)
    if c == 'V':
        new_pos = _consume_class_name(s, pos + 1)
        return f"void* /*class*/", new_pos
    
    # Struct type (U)
    if c == 'U':
        new_pos = _consume_class_name(s, pos + 1)
        return f"void* /*struct*/", new_pos
    
    # Enum type (W4)
    if c == 'W':
        if pos + 1 < len(s) and s[pos + 1] == '4':
            return 'int /*enum*/', pos + 2
        new_pos = _consume_class_name(s, pos + 1)
        return 'int /*enum*/', new_pos
    
    # Back-reference (0-9)
    if c.isdigit():
        return 'void*', pos + 1
    
    # Function pointer (P6...Z)
    # P6 = function pointer prefix, then return type, params, @Z, then the actual Z
    if c == '6' or c == '8':
        # Just skip to the next Z
        idx = s.find('Z', pos)
        if idx != -1:
            return 'void* /*fnptr*/', idx + 1
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
    
    cc_pattern = re.compile(r'@@([YQUS])([AEIG])')
    match = cc_pattern.search(symbol)
    if not match:
        # Try older style: @@Q..., @@U..., @@S... (single letter)
        cc_pattern2 = re.compile(r'@@([QUS])([^@A-Za-z]|$)')
        match = cc_pattern2.search(symbol)
    
    if not match:
        return result
    
    boundary_pos = match.start()  # Position of the @@ boundary
    tail = symbol[boundary_pos + 2:]  # Everything after the boundary @@
    
    # Parse calling convention from tail
    pos = 0
    cc_found = False
    
    # Two-letter markers
    two_letter = ['YA', 'YG', 'YI', 'QE', 'UE', 'SE', 'SA']
    for marker in two_letter:
        if tail.startswith(marker):
            pos = len(marker)
            cc_found = True
            break
    
    if not cc_found:
        # Single-letter markers: Q, U, S
        if tail and tail[0] in 'QUS':
            pos = 1
            cc_found = True
    
    if not cc_found:
        return result
    
    # After calling convention, skip x64 'A' markers and 'B' const qualifiers.
    # In x64 MSVC, after UE/QE/SA/etc., there are up to 3 marker chars (A, B)
    # before the return type begins. Reference types (AA, AEA, AEB) only appear
    # as actual types, never as markers.
    # Strategy: greedily skip all consecutive A and B characters.
    
    while pos < len(tail) and tail[pos] in 'AB':
        pos += 1
    
    # Parse return type
    ret_str, pos = _parse_one_type(tail, pos)
    if ret_str:
        result.ret_type = ret_str
    
    # Parse parameters
    result.params, _ = _parse_params(tail, pos)
    
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
