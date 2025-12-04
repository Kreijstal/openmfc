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
    lines.extend(exports)
    return "\n".join(lines) + "\n"


def stub_signature(sym: str) -> str:
    # placeholder C linkage wrapper; real signatures need demangle parsing
    return f"extern \"C\" void {sym}()"  # ms_abi will be added per-target


def emit_stubs(exports: List[str]) -> str:
    lines = ["#include <cstdio>", "#ifdef _WIN32", "#define MS_ABI __attribute__((ms_abi))", "#else", "#define MS_ABI", "#endif", ""]
    for sym in exports:
        lines.append(f"extern \"C\" void MS_ABI {sym}() {{ std::fprintf(stderr, \"Not Implemented: {sym}\\n\"); }}")
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

    exports = sorted(set(exports))
    Path(args.out_def).write_text(emit_def(exports), encoding="ascii")
    Path(args.out_stubs).write_text(emit_stubs(exports), encoding="ascii")


if __name__ == "__main__":
    main()
