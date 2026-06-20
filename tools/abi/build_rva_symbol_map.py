#!/usr/bin/env python3
"""Build an RVA->mangled-symbol map for a NONAME-exporting DLL (e.g. mfc140.dll).

MFC DLLs export by ordinal only, so neither the DLL nor its export table carries
names. We join two sources:
  * `dumpbin /exports <dll>`  -> ordinal -> RVA   (the loaded image's addresses)
  * `dumpbin /ALL <lib>`      -> symbol  -> ordinal (mangled names, via the .lib)
producing RVA -> symbol, which lets us name vtable slots harvested as raw RVAs.

Usage:
  build_rva_symbol_map.py --exports mfc140.exports \
      --ordinal-map mfc140_ordinal_map.json --module mfc140.dll \
      --out mfc140_rva_symbols.json
"""
import argparse, json, re, sys
from pathlib import Path

# dumpbin /exports rows look like:  "  6360    0 002275C0 [NONAME]"  (ordinal hint RVA name)
# or for forwarders/named exports the 4th column is a symbol. We only need cols 1 & 3.
EXPORT_ROW = re.compile(r"^\s*(\d+)\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]+)\s+\S")

def parse_exports(path):
    """ordinal(int) -> rva(int) from `dumpbin /exports` output."""
    out, in_table = {}, False
    for line in Path(path).read_text(encoding="utf-8", errors="replace").splitlines():
        if "ordinal" in line and "RVA" in line:
            in_table = True
            continue
        if not in_table:
            continue
        m = EXPORT_ROW.match(line)
        if m:
            out[int(m.group(1))] = int(m.group(2), 16)
    return out

def load_ordinal_map(path):
    """ordinal(int) -> symbol from extract_ordinals.py output."""
    d = json.load(open(path))
    exp = d["exports"]
    mod = next(iter(exp.values())) if len(exp) == 1 else exp.get("mfc140u", [])
    return {int(e["ordinal"]): e["symbol"] for e in mod if e.get("ordinal") is not None}

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--exports", required=True)
    ap.add_argument("--ordinal-map", required=True)
    ap.add_argument("--module", required=True)
    ap.add_argument("--out", required=True)
    a = ap.parse_args()

    ord_to_rva = parse_exports(a.exports)
    ord_to_sym = load_ordinal_map(a.ordinal_map)
    rva_to_sym = {}
    for ordn, rva in ord_to_rva.items():
        sym = ord_to_sym.get(ordn)
        if sym:
            rva_to_sym[f"0x{rva:x}"] = sym

    out = {
        "module": a.module,
        "by_rva": rva_to_sym,
        "metadata": {
            "exports_with_rva": len(ord_to_rva),
            "named_ordinals": len(ord_to_sym),
            "resolved_rva_to_symbol": len(rva_to_sym),
            "source_exports": a.exports,
            "source_ordinal_map": a.ordinal_map,
        },
    }
    Path(a.out).write_text(json.dumps(out, indent=2), encoding="ascii")
    print(f"wrote {a.out}: {len(rva_to_sym)} RVA->symbol entries "
          f"({len(ord_to_rva)} exports, {len(ord_to_sym)} named ordinals)")
    return 0

if __name__ == "__main__":
    sys.exit(main())
