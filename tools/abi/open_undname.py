#!/usr/bin/env python3
"""
Minimal clean-room MSVC C++ demangler.
Supports common Phase 0A/0B cases: classes, basic types, pointers, ctors/dtors.
"""
import sys
import argparse
import subprocess


class Undecorator:
    def __init__(self, mangled: str):
        self.mangled = mangled
        self.pos = 0

    def peek(self):
        return self.mangled[self.pos] if self.pos < len(self.mangled) else None

    def consume(self):
        ch = self.peek()
        if ch is not None:
            self.pos += 1
        return ch

    def parse_name_fragment(self):
        name = ""
        while self.peek() not in (None, "@"):
            name += self.consume()
        if self.peek() == "@":
            self.consume()  # skip '@'
        return name

    def parse_fully_qualified_name(self):
        # Names are encoded in reverse with trailing @@
        parts = []
        while self.peek() != "@":
            frag = self.parse_name_fragment()
            if frag:
                parts.append(frag)
            else:
                break
        if self.peek() == "@":
            self.consume()  # skip final '@'
        return "::".join(reversed(parts))

    def parse_type(self):
        c = self.consume()
        if c is None:
            return "UNKNOWN"

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
            return prim[c]
        if c == "_":
            c2 = self.consume()
            ext = {
                "N": "bool",
                "J": "__int64",
                "K": "unsigned __int64",
            }
            return ext.get(c2, f"UNKNOWN__{c2}")

        # Pointers/references (AEAVFoo@@ -> Foo&)
        if c == "A" and self.mangled[self.pos:self.pos + 3] == "EAV":
            self.pos += 3  # skip EAV
            name = self.parse_fully_qualified_name()
            return f"class {name} &"

        # Pointers/references
        if c == "P":
            qual = self.consume()  # A=ref, B=const, E=ptr, etc.
            base = self.parse_type()
            if qual == "A":
                return f"{base} &"
            if qual == "B":
                return f"{base} const *"
            # Default to pointer
            return f"{base} *"

        # Class/struct types
        if c in ("V", "U"):
            name = self.parse_fully_qualified_name()
            return f"class {name}"

        return f"UNK({c})"

    def parse_special_member(self):
        op = self.consume()  # 0 ctor, 1 dtor, etc.
        fq = self.parse_fully_qualified_name()
        cls = fq.split("::")[0] if fq else ""
        if op == "0":
            return f"{cls}::{cls}"
        if op == "1":
            return f"{cls}::~{cls}"
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
        extra_prop_char = self.consume()
        is_const = False

        def apply_prop(ch):
            nonlocal is_virtual, is_const
            if ch is None or is_static:
                return
            if ch == "E":
                is_virtual = True
            elif ch == "B":
                is_const = True
            elif ch == "F":
                is_virtual = True
                is_const = True

        apply_prop(prop_char)
        apply_prop(extra_prop_char)

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

        return prefix, is_const, cc

    def demangle(self):
        if not self.mangled.startswith("?"):
            return self.mangled

        self.consume()  # skip ?

        # Check for special operators (??0, ??1) vs Regular Names
        is_special = False
        func_name = ""

        if self.peek() == "?":
            self.consume()  # skip 2nd ?
            func_name = self.parse_special_member()
            is_special = True
        else:
            func_name = self.parse_fully_qualified_name()

        # Parse Access, Virtual, Static, etc.
        prefix, is_const, cc = self.parse_access_convention()

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
        # Argument list is terminated by 'Z' (End) or '@' (Varargs?)
        # 'X' is void (void param list = no params)
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

        arg_str = ", ".join(args)

        # Assemble Result
        # e.g. "public virtual void Class::Func(int) const"

        res = f"{prefix} "
        if ret_type:
            res += f"{ret_type} "
        if cc and cc != "__cdecl":
            res += f"{cc} "  # hide default cdecl to be cleaner

        res += f"{func_name}({arg_str})"

        if is_const:
            res += " const"

        return res


def main():
    ap = argparse.ArgumentParser(description="Clean-room MSVC demangler (minimal)")
    ap.add_argument("symbol", help="Mangled symbol (e.g., ?GetValue@CClass@@QEBAHXZ)")
    args = ap.parse_args()

    # If the platform provides Microsoft's undname, prefer that for exact fidelity.
    def try_system_undname(sym: str):
        try:
            out = subprocess.check_output(["undname", sym], stderr=subprocess.STDOUT, text=True)
            return out.strip()
        except (FileNotFoundError, subprocess.CalledProcessError):
            return None

    prefer = try_system_undname(args.symbol)
    if prefer is not None:
        print(prefer)
        return

    u = Undecorator(args.symbol)
    print(u.demangle())


if __name__ == "__main__":
    main()
