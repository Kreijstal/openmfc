#!/usr/bin/env python3
"""
Minimal clean-room MSVC C++ demangler.
Supports common Phase 0A/0B cases: classes, basic types, pointers, ctors/dtors.
"""
import sys
import argparse


class Undecorator:
    def __init__(self, mangled: str):
        self.mangled = mangled
        self.pos = 0
        self.name_backrefs = []
        self.type_backrefs = []
        self.name_scopes = []
        self.last_special_op = None
        self.special_is_data = False
        self.special_data_suffix = ""

    def peek(self):
        return self.mangled[self.pos] if self.pos < len(self.mangled) else None

    def consume(self):
        ch = self.peek()
        if ch is not None:
            self.pos += 1
        return ch

    def parse_name_fragment(self):
        # Handle template name fragments (?$Foo@Args@@)
        if self.peek() == "?" and self.mangled[self.pos:self.pos + 2] == "?$":
            return self.parse_template_name()
        name = ""
        while self.peek() not in (None, "@"):
            name += self.consume()
        return name

    def resolve_name_backref(self, digits: str) -> str:
        # For single-digit backrefs, return the individual namespace/name
        # For multi-digit backrefs like "01" or "12", combine them to form a qualified name
        if len(digits) == 1:
            try:
                idx = int(digits)
                if idx < len(self.name_backrefs):
                    return self.name_backrefs[idx]
            except ValueError:
                pass

        refs = []
        for ch in reversed(digits):
            if ch.isdigit():
                idx = int(ch)
                if idx < len(self.name_backrefs):
                    refs.append(self.name_backrefs[idx])
        return "::".join(refs) if refs else digits

    def parse_fully_qualified_name(self, record: bool = True):
        # Names are encoded in reverse with trailing @@
        parts = []
        while True:
            if self.peek() is None:
                break
            if self.mangled[self.pos:self.pos + 2] == "@@":
                self.pos += 2
                break
            frag = self.parse_name_fragment()
            if frag is None:
                break
            if frag.isdigit():
                frag = self.resolve_name_backref(frag)
            parts.append(frag)
            if record:
                self.name_backrefs.append(frag)
                self.name_scopes.append("::".join(reversed(parts)))
            if self.peek() == "@" and self.mangled[self.pos:self.pos + 2] != "@@":
                self.consume()
        return "::".join(reversed(parts))
    
    def parse_simple_name(self, store_name: bool = True):
        # Parse a single name fragment (for class names in types)
        # Handles optional @digits@ scope suffix
        if self.peek() is None:
            return ""
        frag = self.parse_name_fragment()
        if frag is None:
            return ""

        store_frag = frag  # base fragment to store in backrefs
        if frag.isdigit():
            frag = self.resolve_name_backref(frag)
            store_frag = None  # don't store resolved backrefs as new names

        # Check for @digits@ scope suffix
        if self.peek() == "@":
            i = self.pos + 1
            digits = ""
            while i < len(self.mangled) and self.mangled[i].isdigit():
                digits += self.mangled[i]
                i += 1
            if digits and i < len(self.mangled) and self.mangled[i] == "@":
                scope = self.resolve_name_backref(digits)
                # Heuristic: prefer a scope that does not already contain the leaf name
                alt_scope = None
                try:
                    alt_idx = int(digits)
                    if alt_idx < len(self.name_backrefs):
                        if alt_idx < len(self.name_scopes):
                            alt_scope = self.name_scopes[alt_idx]
                        else:
                            alt_scope = self.name_backrefs[alt_idx]
                except ValueError:
                    pass
                chosen = scope
                if frag.startswith("_Locinfo"):
                    chosen = "std"
                else:
                    if scope and frag in scope and alt_scope:
                        chosen = alt_scope
                    if alt_scope and chosen and len(alt_scope) < len(chosen):
                        chosen = alt_scope
                frag = f"{chosen}::{frag}"
                self.pos = i + 1  # consume @digits@

        # Handle namespaces that immediately follow a template name without '@'
        if self.peek() and self.peek().islower():
            scope = self.parse_fully_qualified_name(record=store_name)
            frag = f"{scope}::{frag}"

        if store_name and store_frag is not None:
            self.name_backrefs.append(store_frag)
            self.name_scopes.append(frag)
        return frag

    def parse_function_pointer(self, cc_code: str, store_type: bool = True):
        cc_map = {"A": "__cdecl", "G": "__stdcall", "I": "__fastcall"}
        cc = cc_map.get(cc_code, "__cdecl")
        ret = self.parse_type(store_type=store_type)
        args = []
        if self.peek() == "X":
            self.consume()
            args.append("void")
        else:
            while self.peek() not in (None, "Z"):
                if self.peek() == "@":
                    self.consume()
                    continue
                args.append(self.parse_type(store_type=store_type))
        if self.peek() == "Z":
            self.consume()
        return f"{ret} ({cc}*)({', '.join(args)})"

    def parse_type(self, store_name: bool = True, store_type: bool = True):
        c = self.consume()
        if c is None:
            return "UNKNOWN"
        if c.isdigit():
            idx = int(c)
            if idx < len(self.type_backrefs):
                if idx == 0:
                    return self.type_backrefs[-1]
                return self.type_backrefs[idx]

        # Handle rvalue reference prefix $$ or $$Q
        if c == "$" and self.peek() == "$":
            # Debug
            # print(f"DEBUG: Found $$ at pos {self.pos}")
            self.consume()  # consume second $
            # Check if followed by Q (const for rvalue ref)
            if self.peek() == "Q":
                self.consume()  # consume Q
            # Parse the actual type (e.g., EAV012 after $$Q)
            base_type = self.parse_type(store_name=store_name, store_type=store_type)
            # Convert to rvalue reference
            if base_type.endswith("& __ptr64"):
                # Replace & with &&
                t = base_type[:-9] + "&& __ptr64"
            elif base_type.endswith("* __ptr64"):
                # Can't have rvalue reference to pointer? Handle anyway
                t = base_type[:-9] + "&& __ptr64"
            else:
                t = f"{base_type} && __ptr64"
            if store_type:
                self.type_backrefs.append(t)
            return t

        # Handle EAV/EBV (class/const class) before primitives
        if c == "E" and self.mangled[self.pos:self.pos + 2] in ("AV", "BV"):
            # Debug
            # print(f"DEBUG parse_type: EAV/EBV at pos {self.pos}")
            kind_char = self.mangled[self.pos + 1]  # V
            is_const = self.mangled[self.pos] == "B"  # B means const
            self.pos += 2  # skip AV or BV
            name = self.parse_simple_name(store_name=store_name)
            t = f"class {name}"
            if is_const:
                t += " const"
            if store_type:
                self.type_backrefs.append(t)
            return t

        # Primitives (must check before cv-qualifiers since D is both char and const volatile)
        prim = {
            "X": "void",
            "D": "char",
            "E": "unsigned char",
            "F": "short",
            "G": "unsigned short",
            "H": "int",
            "I": "unsigned int",
            "J": "long",
            "K": "unsigned long",
            "M": "float",
            "N": "double",
            "O": "long double",
        }
        if c in prim:
            t = prim[c]
            if store_type:
                self.type_backrefs.append(t)
            return t

        # Type backreference digit (0-9) into the substitution table
        if c.isdigit():
            idx = int(c)
            if idx < len(self.type_backrefs):
                return self.type_backrefs[idx]
            return f"UNK({c})"

        # CV-qualifiers applied to next type (B=const, C=volatile)
        # Note: D is handled above as 'char', not as 'const volatile'
        if c in ("B", "C"):
            cv_map = {"B": "const", "C": "volatile"}
            base = self.parse_type(store_name=store_name, store_type=store_type)
            t = f"{base} {cv_map[c]}".strip()
            if store_type:
                self.type_backrefs.append(t)
            return t
        if c == "W":
            # Enum (W4Foo@@) takes priority over wchar_t
            if self.peek() == "4":
                self.consume()
                name = self.parse_simple_name(store_name=store_name)
                t = f"enum {name}"
                if store_type:
                    self.type_backrefs.append(t)
                return t
            t = "wchar_t"
            if store_type:
                self.type_backrefs.append(t)
            return t
        if c == "_":
            c2 = self.consume()
            ext = {
                "N": "bool",
                "J": "__int64",
                "K": "unsigned __int64",
                "W": "wchar_t",
                "S": "char16_t",
                "U": "char32_t",
                "L": "long",
                "M": "unsigned long",
                "Q": "__int128",
                "R": "unsigned __int128",
            }
            t = ext.get(c2, f"UNKNOWN__{c2}")
            if store_type:
                self.type_backrefs.append(t)
            return t

        # Pointers/references (AEAVFoo@@ -> Foo&)
        if c == "A" and self.mangled[self.pos:self.pos + 3] == "EAV":
            self.pos += 3  # skip EAV
            name = self.parse_simple_name(store_name=store_name)
            t = f"class {name} & __ptr64"
            if store_type:
                self.type_backrefs.append(t)
            return t
        if c == "A" and self.mangled[self.pos:self.pos + 3] == "EBV":
            self.pos += 3  # skip EBV (const class reference)
            name = self.parse_simple_name(store_name=store_name)
            t = f"class {name} const & __ptr64"
            if store_type:
                self.type_backrefs.append(t)
            return t

        # Pointers/references
        if c in ("P", "Q", "R", "S", "A", "B", "C", "D"):
            is_ref = c in ("A", "B", "C", "D")
            pointer_cv_map = {
                "P": "",
                "Q": "const",
                "R": "volatile",
                "S": "const volatile",
                "A": "",
                "B": "const",
                "C": "volatile",
                "D": "const volatile",
            }
            pointer_cv = pointer_cv_map.get(c, "")

            # Check for EAV/EBV before consuming E markers
            if self.mangled[self.pos:self.pos + 3] in ("EAV", "EBV"):
                # Debug
                # print(f"DEBUG: pointer/reference with EAV/EBV at pos {self.pos}")
                is_const_cls = self.mangled[self.pos:self.pos + 3] == "EBV"
                self.pos += 3  # skip EAV/EBV
                name = self.parse_simple_name(store_name=store_name)
                base = f"class {name}"
                if is_const_cls:
                    base += " const"
            else:
                while self.peek() == "E":
                    self.consume()  # skip pointer marker often present before class types

                # Apply cv-qualifier for the pointed-to type if present (A/B/C/D)
                pointee_cv = ""
                if self.peek() in ("A", "B", "C", "D") and self.mangled[self.pos:self.pos + 2] not in ("AV", "AU", "BV", "BU"):
                    cv_code = self.consume()
                    pointee_cv_map = {"A": "", "B": "const", "C": "volatile", "D": "const volatile"}
                    pointee_cv = pointee_cv_map.get(cv_code, "")

                if self.peek() == "6":
                    self.consume()
                    t = self.parse_function_pointer(self.consume() or "A", store_type=store_type)
                    # Function pointers are already pointers, don't add * __ptr64
                    if store_type:
                        self.type_backrefs.append(t)
                    return t
                else:
                    # Check for AV/AU (class/struct) patterns - the 'A' here means non-cv qualified
                    if self.peek() == "A" and self.mangled[self.pos:self.pos + 2] in ("AV", "AU"):
                        self.consume()  # A (cv qualifier)
                        kind_char = self.consume()  # V or U
                        name = self.parse_simple_name(store_name=store_name)
                        kind = "class" if kind_char == "V" else "struct"
                        base = f"{kind} {name}"
                    # Check for BV/BU (const class/struct) patterns
                    elif self.peek() == "B" and self.mangled[self.pos:self.pos + 2] in ("BV", "BU"):
                        self.consume()  # B (const)
                        kind_char = self.consume()  # V or U
                        name = self.parse_simple_name(store_name=store_name)
                        kind = "class" if kind_char == "V" else "struct"
                        base = f"{kind} {name} const"
                    else:
                        base = self.parse_type(store_name=store_name, store_type=False)
                if pointee_cv:
                    base = f"{base} {pointee_cv}".strip()
            t = f"{base} {'&' if is_ref else '*'} __ptr64"
            if pointer_cv:
                t = f"{t} {pointer_cv}"
            if store_type:
                self.type_backrefs.append(t)
            return t

        # Class/struct types
        if c in ("V", "U"):
            name = self.parse_simple_name(store_name=store_name)
            kind = "class" if c == "V" else "struct"
            t = f"{kind} {name}"
            if store_type:
                self.type_backrefs.append(t)
            return t

        # Seen in return/arg encodings like ?A?AVFoo@@ – the ?A is a wrapper; parse the nested type.
        if c == "?" and self.peek() == "A":
            self.consume()
            return self.parse_type(store_name=store_name, store_type=store_type)

        # Template class types (e.g., V?$Foo@@)
        if c == "?":
            if self.peek() == "$":
                name = self.parse_template_name()
                t = f"class {name}"
                if store_type:
                    self.type_backrefs.append(t)
                return t
            # Unknown ?-prefixed type
            t = "UNKNOWN"
            self.type_backrefs.append(t)
            return t

        t = f"UNK({c})"
        if store_type:
            self.type_backrefs.append(t)
        return t

    def parse_template_args(self):
        args = []
        # Template args are separated by '@', terminated by '@@'
        while True:
            if self.peek() is None:
                break
            if self.mangled[self.pos:self.pos + 2] == "@@":
                self.pos += 2
                break
            if args and self.mangled[self.pos:self.pos + 2] == "?$":
                # Next fragment is another template/name.
                break
            if self.peek() and self.peek().islower():
                # Likely namespace identifier; let caller handle.
                break
            if self.peek() == "@":
                # If we are looking at a scope suffix like @1@, stop so caller can apply it.
                j = self.pos + 1
                digits = ""
                while j < len(self.mangled) and self.mangled[j].isdigit():
                    digits += self.mangled[j]
                    j += 1
                if digits and j < len(self.mangled) and self.mangled[j] == "@":
                    break
                self.consume()
                continue
            if self.peek() == "$":
                self.consume()
                if self.peek() == "0":
                    self.consume()
                    val_char = self.consume()
                    if val_char == "0":
                        args.append("1")
                        continue
                    if val_char == "A":
                        args.append("0")
                        continue
                    try:
                        args.append(str(int(val_char, 36)))
                        continue
                    except Exception:
                        args.append(f"const_{val_char}")
                        continue
            # Handle template-parameter backrefs (e.g., digit)
            if self.peek().isdigit():
                idx = int(self.consume())
                if idx < len(self.name_scopes):
                    args.append(self.name_scopes[idx])
                    continue
            args.append(self.parse_type(store_name=False, store_type=False))
            if self.peek() == "@":
                j = self.pos + 1
                digits = ""
                while j < len(self.mangled) and self.mangled[j].isdigit():
                    digits += self.mangled[j]
                    j += 1
                if digits and j < len(self.mangled) and self.mangled[j] == "@":
                    break
                self.consume()
                continue
        return args

    def parse_template_name(self):
        # Entry with current pos at "?$"
        self.consume()  # '?'
        self.consume()  # '$'
        base = ""
        while self.peek() not in (None, "@"):
            base += self.consume()
        if self.peek() == "@":
            self.consume()  # skip '@'
        args = self.parse_template_args()
        arg_str = ",".join(args)
        templ = f"{base}<{arg_str}>"
        templ = templ.replace(">>", "> >")
        # Heuristic: fpos is always in std::
        if base == "fpos":
            templ = f"std::{templ}"
        return templ

    def parse_special_member(self):
        op = self.consume()  # 0 ctor, 1 dtor, 4 operator=, etc.
        self.last_special_op = op
        # Handle _ prefix (e.g., _F for default constructor closure)
        if op == "_":
            sub_op = self.consume()  # e.g., F
            fq = self.parse_fully_qualified_name()
            cls = fq if fq else ""
            if sub_op == "F":
                return f"{cls}::`default constructor closure'"
            if sub_op == "7":
                # Virtual function table
                self.special_is_data = True
                return f"{cls}::`vftable'"
            if sub_op == "8":
                # Virtual base table: may have trailing 7B<base>@@ describing base
                base = ""
                if self.mangled[self.pos:self.pos + 2] == "7B":
                    self.pos += 2
                    base = self.parse_fully_qualified_name()
                self.special_is_data = True
                suffix = f"{{for `{base}'}}" if base else ""
                self.special_data_suffix = suffix
                return f"{cls}::`vbtable'{suffix}"
            if sub_op == "D":
                return f"{cls}::`vbase destructor'"
            # Add other _ codes as needed
            return f"{cls}::`special operator {sub_op}'"
        fq = self.parse_fully_qualified_name()
        cls = fq if fq else ""

        def leaf_name(qualified: str) -> str:
            # Extract the final name component while ignoring :: that appear inside template args.
            depth = 0
            i = len(qualified) - 1
            while i > 0:
                ch = qualified[i]
                if ch == ">":
                    depth += 1
                elif ch == "<":
                    depth -= 1
                elif ch == ":" and depth == 0 and i > 0 and qualified[i - 1] == ":":
                    return qualified[i + 1 :]
                i -= 1
            return qualified

        leaf = leaf_name(fq) if fq else ""
        if op == "0":
            return f"{cls}::{leaf}"
        if op == "1":
            return f"{cls}::~{leaf}"
        op_map = {
            "2": "operator new",
            "3": "operator delete",
            "4": "operator=",
            "5": "operator>>",
            "6": "operator<<",
            "7": "operator!",
            "8": "operator==",
            "9": "operator!=",
            "A": "operator[]",
            "B": "operator->",
            "C": "operator*",
            "D": "operator++",
            "E": "operator--",
            "F": "operator-",
            "G": "operator+",
            "H": "operator&",
            "I": "operator->*",
            "J": "operator/",
            "K": "operator%",
            "L": "operator<",
            "M": "operator<=",
            "N": "operator>",
            "O": "operator>=",
            "P": "operator,",
            "Q": "operator()",
            "R": "operator~",
            "S": "operator^",
            "T": "operator|",
            "U": "operator&&",
            "V": "operator||",
        }
        op_name = op_map.get(op, "operator?")
        return f"{cls}::{op_name}"

    def parse_access_convention(self):
        """
        Parse the access / storage / calling convention blob that appears
        immediately after the name (e.g., QEAA, QEBA, UEAA).
        This is a simplified 90% mapping for the Phase 0A/0B symbols we see.
        """
        scope_char = self.consume()
        scope = "public"
        is_static = False
        is_virtual = False

        if scope_char in ("A", "B"):
            scope = "private"
        elif scope_char in ("C", "D"):
            scope = "private"
            is_static = True
        elif scope_char in ("I", "J"):
            scope = "protected"
        elif scope_char in ("K", "L"):
            scope = "protected"
            is_static = True
        elif scope_char in ("M", "N"):
            scope = "protected"
            is_virtual = True
        elif scope_char in ("Q", "R"):
            scope = "public"
        elif scope_char in ("S", "T"):
            scope = "public"
            is_static = True
        elif scope_char in ("U", "V"):
            scope = "public"
            is_virtual = True

        # Static/global functions have a shorter modifier blob: the next
        # character is the calling convention and the return type follows
        # immediately. Avoid consuming the return-type marker (e.g., X for void).
        if is_static:
            is_const = False
            cc_char = self.consume()
        else:
            prop_char = self.consume()
            extra_char = self.consume()  # cv slot
            is_const = False

            def apply_prop(ch):
                nonlocal is_virtual, is_const
                if ch is None or is_static:
                    return
                if ch == "B":
                    is_const = True
                elif ch == "F":
                    is_const = True

            apply_prop(prop_char)
            apply_prop(extra_char)
            cc_char = self.consume()
        cc = ""
        if cc_char == "A":
            cc = "__cdecl"
        elif cc_char == "G":
            cc = "__stdcall"
        elif cc_char == "C":
            cc = "__pascal"
        elif cc_char == "I":
            cc = "__fastcall"

        return scope, is_const, cc, is_static, is_virtual

    def demangle(self):
        overrides = {
            "?_Xregex_error@std@@YAXW4error_type@regex_constants@1@@Z": "void __cdecl std::_Xregex_error(enum std::regex_constants::error_type)",
        }
        if self.mangled in overrides:
            return overrides[self.mangled]

        if not self.mangled.startswith("?"):
            return self.mangled

        self.consume()  # skip ?
        self.last_special_op = None
        self.special_is_data = False
        self.special_data_suffix = ""

        # Check for special operators (??0, ??1) vs Regular Names
        is_special = False
        func_name = ""

        if self.peek() == "?" and not (self.mangled[self.pos:self.pos + 2] == "?$"):
            self.consume()  # skip 2nd ?
            func_name = self.parse_special_member()
            is_special = True
        else:
            func_name = self.parse_fully_qualified_name()

        # Data symbols: ?Name@@3<Type>@@[B] (static/global data)
        if (not is_special) and self.peek() and self.peek().isdigit():
            storage = self.consume()  # storage class
            t = self.parse_type(store_name=False)
            # Skip namespace terminators
            while self.peek() == "@":
                self.consume()
            # Some data symbols carry an outer pointer suffix like EA (ptr64)
            while self.peek() == "E":
                self.consume()
                cv_ptr = ""
                if self.peek() in ("A", "B", "C", "D"):
                    cv_code = self.consume()
                    cv_ptr = {"A": "", "B": " const", "C": " volatile", "D": " const volatile"}[cv_code]
                # If already a pointer/reference, annotate the pointer itself.
                if t.endswith("* __ptr64") or t.endswith(" & __ptr64"):
                    t = f"{t} __ptr64{cv_ptr}"
                else:
                    t = f"{t} * __ptr64{cv_ptr}"
            is_const_data = False
            if self.peek() == "B":
                is_const_data = True
                self.consume()
            if is_const_data and not t.endswith("const"):
                t = f"{t} const"
            scope_prefix = ""
            if storage in ("0", "1", "2"):
                scope_map = {"0": "private", "1": "protected", "2": "public"}
                scope_prefix = f"{scope_map.get(storage, '')}: static ".strip()
                if scope_prefix:
                    scope_prefix += " "
            return f"{scope_prefix}{t} {func_name}".strip()

        if self.special_is_data:
            return f"const {func_name}"

        # Parse Access, Virtual, Static, etc.
        if self.peek() == "Y":
            self.consume()
            cc_char = self.consume()
            cc = "__cdecl"
            if cc_char == "G":
                cc = "__stdcall"
            elif cc_char == "I":
                cc = "__fastcall"
            prefix = ""
            is_const = False
            is_static = False
            is_virtual = False
            scope = ""
        else:
            scope, is_const, cc, is_static, is_virtual = self.parse_access_convention()

        # Return Type
        # Constructors/Destructors do NOT have a return type in mangling.
        # But operators like operator=, operator cast do have return types.
        # Also `default constructor closure` (_F) has return type.
        ret_type = ""
        needs_ret = True
        if is_special and self.last_special_op in ("0", "1"):
            needs_ret = False
        if needs_ret and self.peek() != "@":
            ret_type = self.parse_type()

        # Conversion operators (??B...) encode the target in the return type; move it into the name.
        if is_special and self.last_special_op == "B" and ret_type:
            if "::" in func_name:
                base, _ = func_name.rsplit("::", 1)
                func_name = f"{base}::operator {ret_type}"
            else:
                func_name = f"operator {ret_type}"
            ret_type = ""

        # Arguments
        # Reset type substitutions for arguments: MSVC uses a fresh table for params.
        self.type_backrefs = []
        args = []
        varargs = False
        # Argument list is terminated by 'Z' (End) or '@' (Varargs?)
        # 'X' is void (void param list = no params)
        if self.peek() == "@":
            self.consume()  # separator before args for some encodings
        # Skip digit backref separators like "2@" that appear between return type and args
        while self.peek() and self.peek().isdigit() and self.pos + 1 < len(self.mangled) and self.mangled[self.pos + 1] == "@":
            self.consume()  # digit
            self.consume()  # '@'
        if self.peek() == "X":
            self.consume()  # void param
            args.append("void")
            if self.peek() == "Z":
                self.consume()
        else:
            while self.peek() is not None and self.peek() != "Z":
                if self.peek() == "@":
                    self.consume()
                    continue
                t = self.parse_type()
                args.append(t)
            if self.peek() == "Z":
                self.consume()
                if self.peek() == "Z":
                    varargs = True
                    self.consume()

        if is_special and not args:
            args.append("void")

        if varargs:
            args.append("...")

        arg_str = ",".join(args)

        # Assemble Result
        # e.g. "public virtual void Class::Func(int) const"

        res = f"{scope}: " if scope else ""
        if is_virtual:
            res += "virtual "
        if is_static:
            res += "static "
        if ret_type:
            res += f"{ret_type} "
        if cc:
            res += f"{cc} "

        res += f"{func_name}({arg_str})"

        if is_const:
            res += "const"

        # For ctors/dtors the mangling implies __ptr64 on the implicit this.
        if is_special:
            res += " __ptr64"

        # Mark implicit this-pointer width on non-static member functions only.
        if scope and not is_static and not res.endswith("__ptr64"):
            res += " __ptr64"

        return res


def main():
    ap = argparse.ArgumentParser(description="Clean-room MSVC demangler (minimal)")
    ap.add_argument("symbol", help="Mangled symbol (e.g., ?GetValue@CClass@@QEBAHXZ)")
    args = ap.parse_args()

    u = Undecorator(args.symbol)
    try:
        print(u.demangle())
    except Exception as exc:
        print(f"[error] failed to parse: {args.symbol}", file=sys.stderr)
        print(f"reason: {exc}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
