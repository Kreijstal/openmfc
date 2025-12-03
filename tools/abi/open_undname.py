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
        try:
            idx = int(digits)
            if idx < len(self.name_scopes):
                return self.name_scopes[idx]
        except ValueError:
            pass
        refs = []
        for ch in reversed(digits):
            if ch.isdigit():
                idx = int(ch)
                if idx < len(self.name_backrefs):
                    refs.append(self.name_backrefs[idx])
        return "::".join(refs) if refs else digits

    def parse_fully_qualified_name(self):
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
            self.name_backrefs.append(frag)
            self.name_scopes.append("::".join(reversed(parts)))
            if self.peek() == "@" and self.mangled[self.pos:self.pos + 2] != "@@":
                self.consume()
        return "::".join(reversed(parts))

    def parse_function_pointer(self, cc_code: str):
        cc_map = {"A": "__cdecl", "G": "__stdcall", "I": "__fastcall"}
        cc = cc_map.get(cc_code, "__cdecl")
        ret = self.parse_type()
        args = []
        if self.peek() == "X":
            self.consume()
            args.append("void")
        else:
            while self.peek() not in (None, "Z"):
                args.append(self.parse_type())
        if self.peek() == "Z":
            self.consume()
        return f"{ret} ({cc} *)({', '.join(args)})"

    def parse_type(self):
        c = self.consume()
        if c is None:
            return "UNKNOWN"
        if c.isdigit():
            idx = int(c)
            if idx < len(self.type_backrefs):
                return self.type_backrefs[idx]
        if c in ("B", "C", "D"):
            cv_map = {"B": "const", "C": "volatile", "D": "const volatile"}
            base = self.parse_type()
            t = f"{base} {cv_map[c]}".strip()
            self.type_backrefs.append(t)
            return t
        if c == "W":
            t = "wchar_t"
            self.type_backrefs.append(t)
            return t

        # Primitives
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
        }
        if c in prim:
            t = prim[c]
            self.type_backrefs.append(t)
            return t
        if c == "_":
            c2 = self.consume()
            ext = {
                "N": "bool",
                "J": "__int64",
                "K": "unsigned __int64",
                "W": "wchar_t",
                "S": "short",
                "U": "unsigned short",
                "L": "long",
                "M": "unsigned long",
                "Q": "__int128",
                "R": "unsigned __int128",
            }
            t = ext.get(c2, f"UNKNOWN__{c2}")
            self.type_backrefs.append(t)
            return t

        # Pointers/references (AEAVFoo@@ -> Foo&)
        if c == "A" and self.mangled[self.pos:self.pos + 3] == "EAV":
            self.pos += 3  # skip EAV
            name = self.parse_fully_qualified_name()
            t = f"class {name} & __ptr64"
            self.type_backrefs.append(t)
            return t
        if c == "A" and self.mangled[self.pos:self.pos + 4] == "EBV_":
            self.pos += 4  # skip EBV_
            name = self.parse_fully_qualified_name()
            t = f"class {name} const & __ptr64"
            self.type_backrefs.append(t)
            return t
        if c == "A" and self.mangled[self.pos:self.pos + 3] == "EBV":
            self.pos += 3
            name = self.parse_fully_qualified_name()
            t = f"class {name} const & __ptr64"
            self.type_backrefs.append(t)
            return t

        # Pointers/references
        if c in ("P", "Q", "R", "S", "A", "B", "C", "D"):
            is_ref = c in ("A", "B", "C", "D")
            cv_map = {
                "P": "",
                "Q": "const",
                "R": "volatile",
                "S": "const volatile",
                "A": "",
                "B": "const",
                "C": "volatile",
                "D": "const volatile",
            }
            cv = cv_map.get(c, "")

            while self.peek() == "E":
                self.consume()  # skip pointer marker often present before class types

            if self.peek() == "6":
                self.consume()
                t = self.parse_function_pointer(self.consume() or "A")
            else:
                if self.peek() == "A" and self.mangled[self.pos:self.pos + 3] in ("AV", "AU"):
                    self.consume()  # A
                    kind_char = self.consume()
                    name = self.parse_fully_qualified_name()
                    kind = "class" if kind_char == "V" else "struct"
                    base = f"{kind} {name}"
                elif self.mangled[self.pos:self.pos + 3] in ("EAV", "EBV"):
                    is_const_cls = self.mangled[self.pos:self.pos + 3] == "EBV"
                    self.pos += 3  # skip EAV/EBV
                    name = self.parse_fully_qualified_name()
                    base = f"class {name}"
                    if is_const_cls:
                        base += " const"
                else:
                    base = self.parse_type()
                if cv:
                    base = f"{base} {cv}"
                t = f"{base} {'&' if is_ref else '*'} __ptr64"
            self.type_backrefs.append(t)
            return t

        # Class/struct types
        if c in ("V", "U"):
            name = self.parse_fully_qualified_name()
            kind = "class" if c == "V" else "struct"
            t = f"{kind} {name}"
            self.type_backrefs.append(t)
            return t

        # Template class types (e.g., V?$Foo@@)
        if c == "?":
            if self.peek() == "$":
                name = self.parse_template_name()
                t = f"class {name}"
                self.type_backrefs.append(t)
                return t
            # Unknown ?-prefixed type
            t = "UNKNOWN"
            self.type_backrefs.append(t)
            return t

        t = f"UNK({c})"
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
            args.append(self.parse_type())
            if self.peek() == "@":
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
        return templ

    def parse_special_member(self):
        op = self.consume()  # 0 ctor, 1 dtor, etc.
        fq = self.parse_fully_qualified_name()
        cls = fq if fq else ""
        leaf = fq.split("::")[-1] if fq else ""
        if op == "0":
            return f"{cls}::{leaf}"
        if op == "1":
            return f"{cls}::~{leaf}"
        return f"{cls}::operator?"

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
        elif scope_char in ("Q", "R", "U", "V"):
            scope = "public"
            if scope_char in ("U", "V"):
                is_virtual = True
        elif scope_char in ("S", "T"):
            scope = "public"
            is_static = True

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

        prefix = scope
        if is_virtual:
            prefix += " virtual"
        if is_static:
            prefix += " static"

        return prefix, is_const, cc, is_static

    def demangle(self):
        if not self.mangled.startswith("?"):
            return self.mangled

        self.consume()  # skip ?

        # Check for special operators (??0, ??1) vs Regular Names
        is_special = False
        func_name = ""

        if self.peek() == "?" and not (self.mangled[self.pos:self.pos + 2] == "?$"):
            self.consume()  # skip 2nd ?
            func_name = self.parse_special_member()
            is_special = True
        else:
            func_name = self.parse_fully_qualified_name()

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
        else:
            prefix, is_const, cc, is_static = self.parse_access_convention()

        # Return Type
        # Constructors/Destructors do NOT have a return type in mangling.
        ret_type = ""
        if not is_special:
            # Special case: Type-cast operators (??B) have return type logic differently?
            # For standard methods, return type is next.
            # Note: 'X' = void, 'H' = int, etc.
            # If we hit '@', it might mean something else, but usually return type is mandatory.
            if self.peek() != "@":
                ret_type = self.parse_type()

        # Arguments
        args = []
        varargs = False
        # Argument list is terminated by 'Z' (End) or '@' (Varargs?)
        # 'X' is void (void param list = no params)
        if self.peek() == "@":
            self.consume()  # separator before args for some encodings
        if self.peek() == "X":
            self.consume()  # void param
            args.append("void")
            if self.peek() == "Z":
                self.consume()
        else:
            while self.peek() != "Z" and self.peek() is not None and self.peek() != "@":
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

        res = f"{prefix}: " if prefix else ""
        if ret_type:
            res += f"{ret_type} "
        if cc:
            res += f"{cc} "

        res += f"{func_name}({arg_str})"

        if is_const:
            res += " const"

        # For ctors/dtors the mangling implies __ptr64 on the implicit this.
        if is_special:
            res += " __ptr64"

        # Mark implicit this-pointer width on non-static member functions only.
        if prefix and not is_static and not res.endswith("__ptr64"):
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
