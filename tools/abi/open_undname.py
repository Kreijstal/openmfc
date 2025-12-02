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

    def demangle(self):
        if not self.mangled.startswith("?"):
            return self.mangled

        self.consume()  # skip leading '?'

        # Special operators (ctor/dtor)
        is_special = False
        if self.peek() == "?":
            self.consume()
            func_name = self.parse_special_member()
            is_special = True
        else:
            func_name = self.parse_fully_qualified_name()

        # Access/convention blob (e.g., QEAA = public __cdecl)
        access_blob = ""
        while self.peek() and self.peek().isupper():
            access_blob += self.consume()
            if len(access_blob) >= 4:
                break  # heuristic stop

        # Return type (not present for ctors/dtors)
        ret_type = ""
        if not is_special:
            ret_type = self.parse_type()

        # Arguments until 'Z' or '@'
        args = []
        while self.peek() not in (None, "Z", "@"):
            args.append(self.parse_type())
        # consume terminator if present
        if self.peek() in ("Z", "@"):
            self.consume()

        arg_str = ", ".join(args)
        if ret_type:
            return f"{ret_type} {func_name}({arg_str})"
        return f"{func_name}({arg_str})"


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
