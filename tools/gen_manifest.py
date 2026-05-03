#!/usr/bin/env python3
"""
Generate an accurate export-by-export manifest classifying each symbol's
implementation status.

Categories:
  real       – Symbol: comment in source matching a valid export
  skip       – In thunks_skip.txt matching a valid export
  stub       – In export mapping, no real implementation, not skipped
  stale      – Symbol: comment for a symbol NOT in the export mapping
  bad_skip   – In thunks_skip.txt but NOT in the export mapping

Usage:
  python3 tools/gen_manifest.py --out manifest.json
  python3 tools/gen_manifest.py --out manifest.txt
"""

import argparse
import json
import os
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def load_exports(mapping_path):
    """Load all exported symbols from the JSON mapping file."""
    with open(mapping_path) as f:
        data = json.load(f)
    # Normalize to dict: symbol -> info
    exports = {}
    for item in data['exports']['mfc140u']:
        sym = item['symbol']
        exports[sym] = {
            'ordinal': item.get('ordinal', 0),
            'source': item.get('source', ''),
        }
    return exports


def load_skip_list(skip_path):
    """Load symbols from thunks_skip.txt."""
    skips = set()
    with open(skip_path) as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith('#'):
                skips.add(line)
    return skips


def collect_symbol_comments(src_dir):
    """Scan source files for // Symbol: comments.
    Returns dict: symbol -> {'file': str, 'line': int}
    """
    comments = {}
    for fname in sorted(os.listdir(src_dir)):
        if not fname.endswith('.cpp'):
            continue
        fpath = os.path.join(src_dir, fname)
        with open(fpath) as f:
            for i, line in enumerate(f, 1):
                if line.startswith('// Symbol:'):
                    sym = line.strip()[10:].strip()
                    # Don't overwrite if duplicate (unlikely but possible)
                    if sym not in comments:
                        comments[sym] = {'file': fname, 'line': i}
    return comments


def extract_class(symbol):
    """Extract class name from MSVC mangled symbol."""
    m = re.match(r'\?\w+@(\w+)@@', symbol)
    return m.group(1) if m else None


def generate_manifest(exports, symbol_comments, skip_set):
    """Classify each export by status."""
    manifest = {
        'real': [],
        'skip': [],
        'stub': [],
        'stale': [],
        'bad_skip': [],
    }

    # Process all exports
    for sym, info in sorted(exports.items()):
        cls = extract_class(sym)
        entry = {
            'symbol': sym,
            'class': cls,
            'ordinal': info['ordinal'],
        }

        if sym in skip_set:
            entry['category'] = 'skip'
            entry['file'] = 'tools/thunks_skip.txt'
            manifest['skip'].append(entry)
        elif sym in symbol_comments:
            entry['category'] = 'real'
            entry.update(symbol_comments[sym])
            manifest['real'].append(entry)
        else:
            entry['category'] = 'stub'
            manifest['stub'].append(entry)

    # Find stale comments (in source but not in exports)
    for sym, info in sorted(symbol_comments.items()):
        if sym not in exports:
            manifest['stale'].append({
                'symbol': sym,
                'class': extract_class(sym),
                'category': 'stale',
                'file': info['file'],
                'line': info['line'],
            })

    # Find bad skip entries (in skip list but not in exports)
    for sym in sorted(skip_set):
        if sym not in exports:
            manifest['bad_skip'].append({
                'symbol': sym,
                'class': extract_class(sym),
                'category': 'bad_skip',
                'file': 'tools/thunks_skip.txt',
            })

    return manifest


def print_summary(manifest, total_exports):
    """Print a human-readable summary."""
    real = len(manifest['real'])
    skip = len(manifest['skip'])
    stub = len(manifest['stub'])
    stale = len(manifest['stale'])
    bad_skip = len(manifest['bad_skip'])

    print(f'=== OpenMFC Export Manifest ===')
    print(f'')
    print(f'Total exports:          {total_exports:>6d}')
    print(f'')
    print(f'Real implementations:   {real:>6d}  ({100*real/total_exports:5.1f}%)')
    print(f'Skip-listed:            {skip:>6d}  ({100*skip/total_exports:5.1f}%)')
    print(f'Remaining stubs:        {stub:>6d}  ({100*stub/total_exports:5.1f}%)')
    print(f'')
    if stale:
        print(f'⚠ Stale Symbol: comments: {stale:>6d}  (symbols in source but not exported)')
    if bad_skip:
        print(f'⚠ Bad skip-list entries:   {bad_skip:>6d}  (in skip list but not exported)')
    verified = real + skip + stub
    if verified != total_exports:
        print(f'⚠ Unaccounted:              {total_exports - verified:>6d}')

    # Class breakdown
    print(f'')
    print(f'=== Top Classes by Real Implementations ===')
    class_real = Counter()
    for entry in manifest['real']:
        if entry['class']:
            class_real[entry['class']] += 1
    for cls, count in class_real.most_common(20):
        print(f'  {cls:40s} {count:>4d}')

    # Stale breakdown
    if stale:
        print(f'')
        print(f'=== Stale Symbols by File ===')
        stale_by_file = Counter(e['file'] for e in manifest['stale'])
        for fname, count in stale_by_file.most_common(15):
            print(f'  {fname:40s} {count:>4d}')

    # Bad skip
    if bad_skip:
        print(f'')
        print(f'=== Bad Skip-List Entries (not in exports) ===')
        for entry in manifest['bad_skip'][:20]:
            print(f'  {entry["symbol"]}')


def main():
    parser = argparse.ArgumentParser(description='Generate export manifest')
    parser.add_argument('--out', required=True, help='Output file (.json or .txt)')
    parser.add_argument('--mapping', default=str(ROOT / 'mfc_complete_ordinal_mapping.json'))
    parser.add_argument('--source-dir', default=str(ROOT / 'phase4/src'))
    parser.add_argument('--skip', default=str(ROOT / 'tools/thunks_skip.txt'))
    args = parser.parse_args()

    exports = load_exports(args.mapping)
    skip_set = load_skip_list(args.skip)
    symbol_comments = collect_symbol_comments(args.source_dir)
    manifest = generate_manifest(exports, symbol_comments, skip_set)

    if args.out.endswith('.json'):
        with open(args.out, 'w') as f:
            json.dump(manifest, f, indent=2)
        print(f'Wrote {args.out}')
    else:
        # Print summary to file
        old_stdout = sys.stdout
        with open(args.out, 'w') as f:
            sys.stdout = f
            print_summary(manifest, len(exports))
            sys.stdout = old_stdout
        print_summary(manifest, len(exports))
        print(f'Wrote {args.out}')


if __name__ == '__main__':
    main()
