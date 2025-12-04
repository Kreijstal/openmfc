#!/usr/bin/env python3
"""
Generate C vtable arrays and extern declarations from harvested ABI metadata.

Expected input (JSON):
{
  "classes": [
    {
      "name": "CReferenceTest",
      "slots": [
        {"symbol": "??_GCReferenceTest@@QEAA@PEAXI@Z"},
        {"symbol": "?GetValue@CReferenceTest@@QEBAHXZ"},
        {"symbol": "?SetValue@CReferenceTest@@QEAAXH@Z"}
      ]
    }
  ]
}

Output: C source with vtable arrays and a header with extern declarations.
"""
import argparse
import json
from pathlib import Path
from typing import List, Dict


def gen_array(name: str, slots: List[Dict[str, str]]) -> str:
    lines = [f'// VTable for {name}', f'void* {name}_vtable[] = {{']
    for slot in slots:
        sym = slot["symbol"]
        lines.append(f'    (void*)&{sym},')
    lines.append('};')
    return "\n".join(lines)


def gen_header_entry(name: str) -> str:
    return f'extern void* {name}_vtable[];'


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--input", required=True, help="Path to abi_metadata.json")
    ap.add_argument("--out-c", required=True, help="Output C file for vtables")
    ap.add_argument("--out-h", required=True, help="Output header with externs")
    args = ap.parse_args()

    data = json.loads(Path(args.input).read_text())
    classes = data.get("classes", [])
    c_out = []
    h_out = ["#pragma once", ""]
    for cls in classes:
        name = cls["name"]
        slots = cls.get("slots", [])
        c_out.append(gen_array(name, slots))
        c_out.append("")  # spacer
        h_out.append(gen_header_entry(name))
    Path(args.out_c).write_text("\n".join(c_out), encoding="ascii")
    Path(args.out_h).write_text("\n".join(h_out), encoding="ascii")


if __name__ == "__main__":
    main()
