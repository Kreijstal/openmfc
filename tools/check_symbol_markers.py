#!/usr/bin/env python3
"""
Validate `// Symbol:` markers in phase4/src against the real export list.

A marker names the MSVC export that the adjacent `impl__` function implements.
build_phase4.sh greps these markers into AUTO_EXCLUDES so the stub generator
skips them. A marker naming a symbol that is not an export therefore excludes
nothing -- which is harmless in itself, but hides a real failure mode:

  If a *correctly mangled* export exists that the marker missed (typically the
  marker uses 32-bit mangling -- PAV/QBA/UBA -- where retail exports the 64-bit
  PEAV/QEBA/UEBA form), then the .def still aliases that export to the
  correctly-mangled impl__ symbol. The stub generator defines it, so the export
  resolves to a stub returning 0 while the hand-written implementation beside
  the marker is orphaned and unreachable. Coverage metrics count it as done.

This script separates those cases:

  ORPHANED  a real export exists under different mangling, and no overload of
            that name is implemented -> implementation is dead code
  SHADOWED  every export of that name IS implemented elsewhere -> the stale
            marker is redundant clutter
  AMBIGUOUS the name has overloads and only some are implemented. symbol_key
            deliberately ignores parameter types (that is what lets it match a
            32-bit marker to its 64-bit twin), so it cannot tell which overload
            the marker meant. Reported separately and treated as a failure:
            folding these into SHADOWED would let a genuine orphan hide behind
            an implemented sibling overload, which is precisely the bug this
            script exists to catch.
  UNKNOWN   no export by that name at all -> dead code for something retail
            never exported; safe, but the marker should be dropped or demoted
            to a plain comment

Exit status is 1 if any ORPHANED or AMBIGUOUS marker is found (unless
--allow-orphaned), 0 otherwise, so this can gate CI.

Usage:
  python3 tools/gen_manifest.py --out /tmp/manifest.json
  python3 tools/check_symbol_markers.py --manifest /tmp/manifest.json
"""

import argparse
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

# Mangling fragments that are 32-bit-only. Their presence in a marker on an
# x64 target is almost always the root cause.
X86_MANGLING = re.compile(r'P[AB][VUD_]|@@[QUIM][AB][A-Z]')


def symbol_key(sym):
    """Identity of a symbol ignoring calling-convention/pointer-width mangling.

    ?Duplicate@COleStreamFile@@UBAPAVCFile@@XZ  and
    ?Duplicate@COleStreamFile@@UEBAPEAVCFile@@XZ
    share a key, so one can be recognised as the other's mis-mangled twin.
    """
    m = re.match(r'^(\?{1,2})([^@]*)@([^@]*)@', sym)
    if not m:
        return None
    return (m.group(2), m.group(3))


def main():
    ap = argparse.ArgumentParser(description='Validate // Symbol: markers against the export list')
    ap.add_argument('--manifest', required=True, help='manifest .json from gen_manifest.py')
    ap.add_argument('--allow-orphaned', action='store_true',
                    help='report orphaned/ambiguous markers but exit 0 anyway')
    args = ap.parse_args()

    with open(args.manifest, encoding='utf-8') as f:
        man = json.load(f)

    real_syms = {e['symbol'] for e in man['real']}
    all_exports = real_syms | {e['symbol'] for e in man['skip']} | {e['symbol'] for e in man['stub']}

    by_key = defaultdict(list)
    for s in all_exports:
        k = symbol_key(s)
        if k:
            by_key[k].append(s)

    orphaned, shadowed, unknown, ambiguous = [], [], [], []
    for e in man['stale']:
        sym = e['symbol']
        cands = by_key.get(symbol_key(sym), [])
        impl = [c for c in cands if c in real_syms]
        if not cands:
            unknown.append((e, sym))
        elif not impl:
            # No overload of this name is implemented, so whatever the marker
            # meant, the implementation beside it is unreachable.
            orphaned.append((e, sym, cands))
        elif len(impl) == len(cands):
            shadowed.append((e, sym, cands))
        else:
            # The key ignores parameter types, so a name with overloads can
            # match several exports. With only some implemented we cannot tell
            # which one the marker meant: calling it SHADOWED would let a real
            # orphan through the gate, and calling it ORPHANED would cry wolf.
            ambiguous.append((e, sym, cands, impl))

    def loc(e):
        return f"{e['file']}:{e.get('line', '?')}"

    if orphaned:
        print(f'ORPHANED ({len(orphaned)}) — export exists, implementation unreachable:')
        for e, sym, cands in sorted(orphaned, key=lambda x: loc(x[0])):
            hint = '  [32-bit mangling]' if X86_MANGLING.search(sym) else ''
            print(f'  {loc(e)}{hint}')
            print(f'    marker: {sym}')
            for c in cands:
                print(f'    retail: {c}')
        print()

    if shadowed:
        print(f'SHADOWED ({len(shadowed)}) — stale marker, correct form implemented elsewhere:')
        for e, sym, _ in sorted(shadowed, key=lambda x: loc(x[0])):
            print(f'  {loc(e)}  {sym}')
        print()

    if ambiguous:
        print(f'AMBIGUOUS ({len(ambiguous)}) — name has overloads, only some implemented;')
        print('                cannot tell which one the marker meant:')
        for e, sym, cands, impl in sorted(ambiguous, key=lambda x: loc(x[0])):
            print(f'  {loc(e)}')
            print(f'    marker: {sym}')
            for c in cands:
                print(f'    {"implemented" if c in impl else "STUB       "}: {c}')
        print()

    if unknown:
        print(f'UNKNOWN ({len(unknown)}) — not an export under any mangling; marker is inert:')
        for e, sym in sorted(unknown, key=lambda x: loc(x[0])):
            print(f'  {loc(e)}  {sym}')
        print()

    total = len(orphaned) + len(shadowed) + len(unknown) + len(ambiguous)
    print(f'{total} stale marker(s): {len(orphaned)} orphaned, {len(shadowed)} shadowed, '
          f'{len(ambiguous)} ambiguous, {len(unknown)} unknown.')

    if (orphaned or ambiguous) and not args.allow_orphaned:
        print('\nFAIL: orphaned markers mean an export resolves to a stub while a real '
              'implementation sits unreachable beside the marker; ambiguous ones may be '
              'the same thing hidden behind an overload.', file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
