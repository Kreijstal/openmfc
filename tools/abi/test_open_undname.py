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

# Some DLLs (e.g., vcamp/vccorlib) still have known mismatches.
# Skip them by default to keep regression runs green, but allow opt-in via env.
KNOWN_MISMATCH_FILES: set[str] = set()


def load_expectations() -> List[Tuple[str, str]]:
    pairs: List[Tuple[str, str]] = []
    pat_is = re.compile(r'^is :- "(?P<demangled>.*)"')
    pat_und = re.compile(r'^\s*undname:\s*(?P<demangled>.+)$', re.IGNORECASE)
    pat_header = re.compile(r'^Undecoration of :- "(?P<sym>\?\?.*)"')
    for path in sorted(DATA_DIR.glob("compare_*.txt")):
        if (path.name in KNOWN_MISMATCH_FILES) and not INCLUDE_KNOWN_MISMATCH:
            continue
        current = None
        with path.open("r", encoding="utf-8", errors="ignore") as f:
            for line in f:
                # Handle CRLF files from Windows runners
                line = line.rstrip("\r\n")
                if line.startswith("?"):
                    current = line.strip()
                m_hdr = pat_header.search(line)
                if m_hdr:
                    current = m_hdr.group("sym")
                m = pat_is.search(line)
                if m and current:
                    pairs.append((current, m.group("demangled")))
                    current = None
                m2 = pat_und.search(line)
                if m2 and current:
                    dem = m2.group("demangled").strip()
                    # Skip banner lines like "Microsoft (R) C++ Name Undecorator"
                    if "Name Undecorator" in dem:
                        continue
                    if "generated via winedump" in dem:
                        continue
                    pairs.append((current, dem))
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
