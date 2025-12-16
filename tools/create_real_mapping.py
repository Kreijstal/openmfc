#!/usr/bin/env python3
"""
Create real ordinal mapping by matching our symbols with real ordinals from dumpbin.
"""

import json
from pathlib import Path
from typing import Dict, List

def main():
    # Load our symbols
    symbols_path = Path("artifacts/mfc_symbols.txt")
    symbols = symbols_path.read_text().strip().split('\n')
    symbols = [s.strip() for s in symbols if s.strip()]
    print(f"Loaded {len(symbols)} symbols from {symbols_path}")
    
    # Load real ordinals
    ordinals_path = Path("/tmp/real_ordinals.json")
    ordinals_data = json.loads(ordinals_path.read_text())
    real_entries = ordinals_data.get("exports", {}).get("mfc140u", [])
    
    # Create lookup dictionary
    ordinal_lookup = {}
    for entry in real_entries:
        symbol = entry.get("symbol")
        ordinal = entry.get("ordinal")
        if symbol and ordinal is not None:
            ordinal_lookup[symbol] = ordinal
    
    print(f"Loaded {len(ordinal_lookup)} real symbol→ordinal mappings")
    
    # Match symbols with real ordinals
    matched = []
    not_found = []
    
    for symbol in symbols:
        if symbol in ordinal_lookup:
            matched.append({
                "symbol": symbol,
                "ordinal": ordinal_lookup[symbol],
                "source": "real_dumpbin"
            })
        else:
            not_found.append(symbol)
    
    print(f"\nMatched {len(matched)} symbols with real ordinals")
    print(f"Could not find {len(not_found)} symbols in real data")
    
    if not_found:
        print("\nFirst 10 unmatched symbols:")
        for sym in not_found[:10]:
            print(f"  {sym}")
    
    # Create database
    db = {
        "exports": {
            "mfc140u": matched
        },
        "metadata": {
            "total_exports": len(matched),
            "unmatched_symbols": len(not_found),
            "has_real_ordinals": True,
            "source": "dumpbin /ALL mfc140u.lib",
            "note": "Real ordinals extracted from MFC .lib file"
        }
    }
    
    # Write output
    output_path = Path("mfc_real_ordinal_mapping.json")
    output_path.write_text(json.dumps(db, indent=2), encoding="ascii")
    
    print(f"\n✅ Wrote {len(matched)} real ordinal mappings to {output_path}")
    
    # Show some examples
    print("\n📊 Example mappings:")
    for entry in matched[:10]:
        print(f"  {entry['ordinal']:6d} -> {entry['symbol'][:60]}...")
    
    # Check AfxThrowMemoryException
    for entry in matched:
        if "AfxThrowMemoryException" in entry["symbol"]:
            print(f"\n🔥 AfxThrowMemoryException: ordinal {entry['ordinal']} (was 256 with guessed mapping)")
            break
    
    return 0

if __name__ == "__main__":
    import sys
    sys.exit(main())