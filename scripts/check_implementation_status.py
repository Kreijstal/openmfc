#!/usr/bin/env python3
"""
Phase 4 Implementation Status Checker

Analyzes an OpenMFC DLL to determine which symbols are:
- Implemented (strong symbols overriding weak stubs)
- Stubbed (weak symbols - not yet implemented)

Usage:
    python3 scripts/check_implementation_status.py \
        --mapping mfc_complete_ordinal_mapping.json \
        --dll build-phase4/openmfc.dll \
        --obj build-phase4/weak_stubs.o

Or with just the mapping to see all symbols:
    python3 scripts/check_implementation_status.py \
        --mapping mfc_complete_ordinal_mapping.json \
        --list-all
"""

import argparse
import json
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Set, Tuple

# Add the tools directory to path for the demangler
sys.path.insert(0, str(Path(__file__).parent.parent / "tools" / "abi"))
from open_undname import Undecorator


def demangle(symbol: str) -> str:
    """Demangle an MSVC-mangled symbol using open_undname."""
    try:
        u = Undecorator(symbol)
        return u.demangle()
    except Exception:
        return symbol


def get_weak_symbols(obj_path: Path) -> Set[str]:
    """Extract weak symbol names from object file."""
    weak = set()
    try:
        result = subprocess.run(
            ["x86_64-w64-mingw32-nm", str(obj_path)],
            capture_output=True,
            text=True,
            check=True
        )
        for line in result.stdout.split('\n'):
            parts = line.split()
            if len(parts) >= 2:
                # 'w' = weak symbol, 'W' = weak defined symbol
                if parts[-2] in ('w', 'W', 'v', 'V'):
                    weak.add(parts[-1])
    except Exception as e:
        print(f"Warning: Could not read object file: {e}", file=sys.stderr)
    return weak


def get_strong_symbols(obj_paths: List[Path]) -> Set[str]:
    """Extract strong (non-weak) defined symbols from object files."""
    strong = set()
    for obj_path in obj_paths:
        try:
            result = subprocess.run(
                ["x86_64-w64-mingw32-nm", str(obj_path)],
                capture_output=True,
                text=True,
                check=True
            )
            for line in result.stdout.split('\n'):
                parts = line.split()
                if len(parts) >= 3:
                    # Uppercase letters (except w/W) are defined symbols
                    sym_type = parts[1]
                    if sym_type.isupper() and sym_type not in ('W', 'V'):
                        strong.add(parts[2])
        except Exception:
            pass
    return strong


def load_mapping(mapping_path: Path) -> List[Dict]:
    """Load the ordinal mapping."""
    with open(mapping_path) as f:
        data = json.load(f)
    return data.get("exports", {}).get("mfc140u", [])


def categorize_symbols(entries: List[Dict]) -> Dict[str, List[Dict]]:
    """Categorize symbols by type (class, function, data, etc.)."""
    categories = {
        "constructors": [],
        "destructors": [],
        "operators": [],
        "virtual_methods": [],
        "static_methods": [],
        "member_functions": [],
        "global_functions": [],
        "data": [],
        "other": [],
    }

    for entry in entries:
        sym = entry["symbol"]
        demangled = demangle(sym)

        if sym.startswith("??0"):
            categories["constructors"].append(entry)
        elif sym.startswith("??1"):
            categories["destructors"].append(entry)
        elif sym.startswith("??") and not sym.startswith("??$"):
            categories["operators"].append(entry)
        elif "@@U" in sym or "@@V" in sym:  # Virtual methods
            categories["virtual_methods"].append(entry)
        elif "@@S" in sym:  # Static methods
            categories["static_methods"].append(entry)
        elif "@@Q" in sym or "@@A" in sym:  # Member functions
            categories["member_functions"].append(entry)
        elif "@@Y" in sym:  # Global functions
            categories["global_functions"].append(entry)
        elif "@@3" in sym or "@@2" in sym:  # Data
            categories["data"].append(entry)
        else:
            categories["other"].append(entry)

    return categories


def extract_class_name(symbol: str) -> str:
    """Extract class name from a mangled symbol."""
    # Simple heuristic: find @ClassName@@ pattern
    import re
    match = re.search(r'@([A-Z][a-zA-Z0-9_]+)@@', symbol)
    if match:
        return match.group(1)
    return "Unknown"


def main():
    parser = argparse.ArgumentParser(
        description="Check OpenMFC implementation status"
    )
    parser.add_argument(
        "--mapping", "-m",
        required=True,
        help="Path to ordinal mapping JSON"
    )
    parser.add_argument(
        "--dll", "-d",
        help="Path to built DLL (optional)"
    )
    parser.add_argument(
        "--obj",
        help="Path to weak stubs object file"
    )
    parser.add_argument(
        "--impl-objs",
        nargs="*",
        help="Paths to implementation object files"
    )
    parser.add_argument(
        "--list-all",
        action="store_true",
        help="List all symbols with demangled names"
    )
    parser.add_argument(
        "--by-class",
        action="store_true",
        help="Group output by class name"
    )
    parser.add_argument(
        "--summary",
        action="store_true",
        help="Show summary statistics only"
    )
    parser.add_argument(
        "--filter-class",
        help="Filter to specific class name"
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=50,
        help="Limit number of symbols shown per category"
    )
    args = parser.parse_args()

    mapping_path = Path(args.mapping)
    if not mapping_path.exists():
        print(f"Error: Mapping file not found: {mapping_path}", file=sys.stderr)
        return 1

    entries = load_mapping(mapping_path)
    print(f"Loaded {len(entries)} symbols from mapping")
    print()

    # Get weak/strong symbol info if object files provided
    weak_symbols = set()
    strong_symbols = set()

    if args.obj:
        weak_symbols = get_weak_symbols(Path(args.obj))
        print(f"Found {len(weak_symbols)} weak symbols in stubs object")

    if args.impl_objs:
        strong_symbols = get_strong_symbols([Path(p) for p in args.impl_objs])
        print(f"Found {len(strong_symbols)} strong symbols in implementation objects")

    # Categorize symbols
    categories = categorize_symbols(entries)

    print()
    print("=" * 70)
    print("SYMBOL CATEGORY SUMMARY")
    print("=" * 70)
    for cat, items in categories.items():
        print(f"  {cat:20s}: {len(items):6d}")
    print("=" * 70)
    print(f"  {'TOTAL':20s}: {len(entries):6d}")
    print()

    if args.summary:
        return 0

    # Group by class if requested
    if args.by_class:
        by_class: Dict[str, List[Dict]] = {}
        for entry in entries:
            cls = extract_class_name(entry["symbol"])
            if cls not in by_class:
                by_class[cls] = []
            by_class[cls].append(entry)

        # Sort by count
        sorted_classes = sorted(by_class.items(), key=lambda x: -len(x[1]))

        print("=" * 70)
        print("SYMBOLS BY CLASS (Top 30)")
        print("=" * 70)
        for cls, items in sorted_classes[:30]:
            print(f"  {cls:30s}: {len(items):6d}")

        if args.filter_class:
            print()
            print(f"Symbols for class: {args.filter_class}")
            print("-" * 70)
            if args.filter_class in by_class:
                for entry in by_class[args.filter_class][:args.limit]:
                    sym = entry["symbol"]
                    demangled = demangle(sym)
                    print(f"  @{entry['ordinal']:5d}: {demangled[:60]}")
            else:
                print(f"  Class not found: {args.filter_class}")
        return 0

    # List all symbols
    if args.list_all:
        print("=" * 70)
        print("ALL SYMBOLS (first 100)")
        print("=" * 70)
        for entry in entries[:100]:
            sym = entry["symbol"]
            ordinal = entry.get("ordinal", 0)
            demangled = demangle(sym)
            print(f"@{ordinal:5d}: {demangled[:65]}")
        if len(entries) > 100:
            print(f"  ... and {len(entries) - 100} more")
        return 0

    # Default: show samples from each category
    for cat, items in categories.items():
        if not items:
            continue
        print()
        print(f"=== {cat.upper()} ({len(items)} total) ===")
        for entry in items[:5]:
            sym = entry["symbol"]
            ordinal = entry.get("ordinal", 0)
            demangled = demangle(sym)
            print(f"  @{ordinal:5d}: {demangled[:60]}")
        if len(items) > 5:
            print(f"  ... and {len(items) - 5} more")

    return 0


if __name__ == "__main__":
    sys.exit(main())
