#!/usr/bin/env python3
"""
Parse `dumpbin /EXPORTS` output into CSV for easier mangling lookup.
"""

import argparse
import csv
import re
import sys
from typing import List, Dict


def parse_dumpbin_exports(filepath: str) -> List[Dict[str, str]]:
    """
    Parses 'dumpbin /EXPORTS' output.
    Looks for lines like:
       1    0 00001010 ??0CReferenceTest@@QEAA@XZ
    """
    exports: List[Dict[str, str]] = []
    # Capture ordinal, hint, RVA, and the symbol name; ignore any trailing ILT info.
    row_pattern = re.compile(r"^\s*(\d+)\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]+)\s+(\S+)")
    start_parsing = False

    with open(filepath, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            line = line.strip()

            # Skip headers until we see the column definition
            if "ordinal" in line and "name" in line:
                start_parsing = True
                continue
            if not start_parsing:
                continue
            if not line:
                continue
            if "Summary" in line:
                break

            match = row_pattern.match(line)
            if match:
                ordinal, hint, rva, name = match.groups()
                exports.append({"ordinal": ordinal, "rva": rva, "name": name})

    return exports


def main() -> int:
    parser = argparse.ArgumentParser(description="Parse MSVC dumpbin exports to CSV")
    parser.add_argument("input_file", help="Path to exports.txt from dumpbin")
    args = parser.parse_args()

    exports = parse_dumpbin_exports(args.input_file)

    writer = csv.writer(sys.stdout)
    writer.writerow(["Ordinal", "RVA", "MangledName", "Notes"])

    for exp in exports:
        note = ""
        name = exp["name"]
        if "??0" in name:
            note = "Constructor"
        elif "??1" in name:
            note = "Destructor"
        elif "??_G" in name:
            note = "Scalar Deleting Dtor"
        writer.writerow([exp["ordinal"], exp["rva"], name, note])

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
