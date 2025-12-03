#!/usr/bin/env python3
"""
Simple regression test for open_undname.py against recorded MSVC undecorations.
"""
import re
import subprocess
import sys
from pathlib import Path
from typing import List, Tuple

ROOT = Path(__file__).resolve().parent
DATA_DIR = ROOT / "testdata"
DEMangler = ROOT / "open_undname.py"


def load_expectations() -> List[Tuple[str, str]]:
    pairs: List[Tuple[str, str]] = []
    pat = re.compile(r'^is :- "(?P<demangled>.*)"')
    for path in sorted(DATA_DIR.glob("compare_*.txt")):
        current = None
        with path.open("r", encoding="utf-8", errors="ignore") as f:
            for line in f:
                line = line.rstrip("\n")
                if line.startswith("??"):
                    current = line.strip()
                m = pat.search(line)
                if m and current:
                    pairs.append((current, m.group("demangled")))
                    current = None
    return pairs


def run_demangle(symbol: str) -> str:
    out = subprocess.check_output(
        [sys.executable, str(DEMangler), symbol],
        cwd=ROOT.parent,
        text=True,
        stderr=subprocess.STDOUT,
    )
    return out.strip()


def main() -> int:
    failures = []
    for mangled, expected in load_expectations():
        got = run_demangle(mangled)
        if got != expected:
            failures.append((mangled, expected, got))

    if failures:
        print(f"{len(failures)} mismatches")
        for mangled, expected, got in failures[:10]:
            print(mangled)
            print(f"  expected: {expected}")
            print(f"  got     : {got}")
            print()
        return 1

    print("All demangling expectations matched.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
