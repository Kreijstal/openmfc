#!/usr/bin/env python3
"""
Generate .def and stub C++ for OpenMFC exports.

Inputs:
  --db mfc_db.json (as produced by parse_exports.py)
Outputs:
  --out-def openmfc.def
  --out-stubs stubs.cpp

Simplifications:
- Only process Unicode exports (no 'A' suffixed ANSI).
- Stub bodies log to stderr and return default zero/null.
"""
import argparse
import json
from pathlib import Path
from typing import Dict, List


def is_ansi(symbol: str) -> bool:
    # crude filter: symbols with 'A@' or '@@YA?AW4' are not reliable; prefer 'W' suffix
    return symbol.endswith('A') or '@YA?A' in symbol


def emit_def(exports: List[str]) -> str:
    lines = ["LIBRARY openmfc", "EXPORTS"]
    for idx, sym in enumerate(exports):
        # Phase0b pattern: mangled_name = simple_c_name
        # This allows extern "C" implementations with simple names
        lines.append(f"{sym} = stub_{idx}")
    return "\n".join(lines) + "\n"


def emit_stubs(exports: List[str]) -> str:
    lines = [
        "#include <cstdio>",
        "#include <cstdlib>",
        "",
        "// MS_ABI: On MSVC, functions use MS ABI by default. On GCC/MinGW, we need the attribute.",
        "#if defined(_MSC_VER)",
        "  #define MS_ABI",
        "#elif defined(__GNUC__)",
        "  #define MS_ABI __attribute__((ms_abi))",
        "#else",
        "  #define MS_ABI",
        "#endif",
        "",
        "// Forward declarations for exception helpers",
        "extern \"C\" void MS_ABI AfxThrowMemoryException();",
        "extern \"C\" void MS_ABI AfxThrowFileException(int cause, int lOsError, const char* pFileName);",
        "",
    ]
    
    for idx, sym in enumerate(exports):
        # Special handling for known throwing functions
        # Use extern "C" to get simple unmangled names that match the .def aliases
        if "AfxThrowMemoryException" in sym:
             lines.append(f"extern \"C\" void MS_ABI stub_{idx}() {{ AfxThrowMemoryException(); }}")
        elif "AfxThrowFileException" in sym:
             lines.append(f"extern \"C\" void MS_ABI stub_{idx}(int cause, int lOsError, const char* pFileName) {{ AfxThrowFileException(cause, lOsError, pFileName); }}")
        else:
             lines.append(f"extern \"C\" void MS_ABI stub_{idx}() {{ std::fprintf(stderr, \"Not Implemented: {sym}\\n\"); }}")

    lines.append("")
    return "\n".join(lines)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--db", required=True)
    ap.add_argument("--out-def", required=True)
    ap.add_argument("--out-stubs", required=True)
    args = ap.parse_args()

    db: Dict = json.loads(Path(args.db).read_text())
    exports: List[str] = []
    for dll, entries in db.get("exports", {}).items():
        if dll != "mfc140u":
            continue
        for ent in entries:
            sym = ent["symbol"] if isinstance(ent, dict) else ent
            if is_ansi(sym):
                continue
            exports.append(sym)

    exports = sorted({sym for sym in exports if not sym.startswith("[")})
    if not exports:
        exports = ["?AfxThrowMemoryException@@YAXXZ", "?AfxThrowFileException@@YAXW4__unnamed@0@Z"]
    Path(args.out_def).write_text(emit_def(exports), encoding="ascii")
    Path(args.out_stubs).write_text(emit_stubs(exports), encoding="ascii")


if __name__ == "__main__":
    main()
