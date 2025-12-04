#!/usr/bin/env python3
"""
Parse dumpbin /exports output and optional undname output into a structured DB.

Usage:
  python tools/parse_exports.py \
    --dll mfc140u --exports artifacts/mfc140u.exports \
    --dll atl140 --exports artifacts/atl140.exports \
    --metadata artifacts/metadata.json \
    --out artifacts/mfc_db.json

The undname output can be provided via --demangled path; lines formatted as
"symbol => demangled" are used to enrich the result if present.
"""
import argparse
import json
import re
from pathlib import Path
from typing import Dict, List, Optional, Tuple

EXPORT_LINE = re.compile(
    r"^\s*(?P<ordinal>\d+)\s+(?:[0-9A-Fa-f]+\s+)?(?P<rva>[0-9A-Fa-f]+)\s+(?P<symbol>.+?)\s*$"
)
DEMANGLE_LINE = re.compile(r"^(?P<raw>\S+)\s*=>\s*(?P<pretty>.+)$")


def load_demangle_map(path: Optional[Path]) -> Dict[str, str]:
    if not path or not path.exists():
        return {}
    mapping: Dict[str, str] = {}
    for line in path.read_text().splitlines():
        m = DEMANGLE_LINE.match(line.strip())
        if m:
            mapping[m.group("raw")] = m.group("pretty")
    return mapping


def parse_exports(path: Path) -> List[Tuple[int, str]]:
    out: List[Tuple[int, str]] = []
    for line in path.read_text().splitlines():
        m = EXPORT_LINE.match(line)
        if not m:
            continue
        out.append((int(m.group("ordinal")), m.group("symbol")))
    return out


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--dll", action="append", required=True, help="DLL name (mfc140u, atl140, etc.)")
    ap.add_argument("--exports", action="append", required=True, help="Path to dumpbin exports for each DLL (same order as --dll)")
    ap.add_argument("--demangled", help="Optional demangle map file (lines: symbol => demangled)")
    ap.add_argument("--metadata", required=True, help="Path to metadata.json")
    ap.add_argument("--out", required=True, help="Path to write mfc_db.json")
    args = ap.parse_args()

    if len(args.dll) != len(args.exports):
        raise SystemExit("--dll and --exports counts must match")

    demap = load_demangle_map(Path(args.demangled) if args.demangled else None)
    metadata = json.loads(Path(args.metadata).read_text())

    db = {
        "metadata": metadata,
        "exports": {}
    }

    for dll, exp_path in zip(args.dll, args.exports):
        entries = []
        for ordinal, symbol in parse_exports(Path(exp_path)):
            entry = {
                "ordinal": ordinal,
                "symbol": symbol,
            }
            if symbol in demap:
                entry["demangled"] = demap[symbol]
            entries.append(entry)
        db["exports"][dll] = entries

    Path(args.out).write_text(json.dumps(db, indent=2, sort_keys=True), encoding="ascii")


if __name__ == "__main__":
    main()
