#!/usr/bin/env python3
"""Compute per-class implementation coverage from the current manifest.

A class's exports = every exported symbol whose immediately-enclosing class is
that class. A class is:
  - fully implemented : every export is a real (strong impl_) override
  - partial           : some but not all exports are real
  - not implemented   : zero exports are real
skip-listed exports are excluded from the denominator (compiler-generated /
impractical), matching the export-coverage metric.
"""
import re
import sys
from collections import defaultdict

import stub_coverage as s

# Signature boundary: the @@ that precedes calling-convention markers.
CC = re.compile(r'@@([YQUSIMACEBKG])([AEIG]?)')


def enclosing_class(sym):
    """Return the immediately-enclosing class name, or None for free functions."""
    if not sym.startswith('?'):
        return None
    m = CC.search(sym)
    if not m:
        # data symbol: ?name@Class@@<access><type>...
        m2 = re.search(r'@@[0-9]', sym)
        if not m2:
            return None
        boundary = m2.start()
    else:
        boundary = m.start()
    qual = sym[1:boundary]  # qualified-name portion, innermost-first, @-separated
    # Special names: ??<op>... — the op sits where the method name would be.
    if qual.startswith('?'):
        body = qual[1:]
        if body.startswith('$'):
            # template special name: ??$name@<targs...>@Class@@ — too ambiguous
            # to split reliably; fall through to generic tokenization below.
            qual = body[1:]
        else:
            # operator opcode: single char, or _<char> extended opcode.
            if body.startswith('_') and len(body) >= 2:
                op_len = 2
            else:
                op_len = 1
            rest = body[op_len:]
            # ctor/dtor/operator: the class is the FIRST token after the opcode.
            toks = rest.split('@')
            for t in toks:
                if t and t[0].isalpha() or (t and t[0] == '_'):
                    return t
            return toks[0] if toks and toks[0] else None
    # Normal method: ?name@Class@... -> class is the 2nd token.
    toks = qual.split('@')
    # templates: ??$ handled above via fall-through leaves name@targs@Class
    # For plain names the 2nd non-empty token is the class.
    if len(toks) >= 2:
        return toks[1] if toks[1] else None
    return None


def main():
    m, _ = s.load_current_manifest()
    real = {e['symbol'] for e in m['real']}
    stub = {e['symbol'] for e in m['stub']}
    # denominator = real + stub (skip excluded)
    per_class_total = defaultdict(int)
    per_class_real = defaultdict(int)
    for e in m['real']:
        c = enclosing_class(e['symbol'])
        if c:
            per_class_total[c] += 1
            per_class_real[c] += 1
    for e in m['stub']:
        c = enclosing_class(e['symbol'])
        if c:
            per_class_total[c] += 1

    classes = sorted(per_class_total)
    full = [c for c in classes if per_class_real[c] == per_class_total[c]]
    partial = [c for c in classes if 0 < per_class_real[c] < per_class_total[c]]
    none = [c for c in classes if per_class_real[c] == 0]
    total = len(classes)
    print(f"classes with exports: {total}")
    print(f"fully implemented:   {len(full)}  ({100*len(full)/total:.1f}%)")
    print(f"partial:             {len(partial)}  ({100*len(partial)/total:.1f}%)")
    print(f"not implemented:     {len(none)}  ({100*len(none)/total:.1f}%)")
    print(f"full+partial (>=1):  {len(full)+len(partial)}  ({100*(len(full)+len(partial))/total:.1f}%)")
    if '--list-partial' in sys.argv:
        print("\n# partial (real/total):")
        for c in sorted(partial, key=lambda c: per_class_total[c]-per_class_real[c], reverse=True):
            print(f"  {c}: {per_class_real[c]}/{per_class_total[c]}")
    if '--list-none' in sys.argv:
        print("\n# not implemented (total exports):")
        for c in sorted(none, key=lambda c: per_class_total[c], reverse=True):
            print(f"  {c}: {per_class_total[c]}")
    if '--near' in sys.argv:
        # classes closest to full (fewest missing) — best ROI to finish
        print("\n# partial sorted by fewest-missing (quick finishes):")
        for c in sorted(partial, key=lambda c: per_class_total[c]-per_class_real[c]):
            print(f"  {c}: missing {per_class_total[c]-per_class_real[c]} ({per_class_real[c]}/{per_class_total[c]})")


if __name__ == '__main__':
    main()
