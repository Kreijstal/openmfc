#!/usr/bin/env python3
"""
Create complete ordinal mapping using ALL symbols from dumpbin /ALL output.
This creates the definitive mapping for OpenMFC DLL exports.
"""

import json
from pathlib import Path
from typing import Dict, List


def main():
    # Load real ordinals from dumpbin /ALL output
    ordinals_path = Path("/tmp/real_ordinals_new.json")
    ordinals_data = json.loads(ordinals_path.read_text())
    real_entries = ordinals_data.get("exports", {}).get("mfc140u", [])
    
    print(f"Loaded {len(real_entries)} real symbol→ordinal mappings")
    
    # Create complete mapping - use ALL symbols from real ordinals
    complete_mapping = []
    
    for entry in real_entries:
        symbol = entry.get("symbol")
        ordinal = entry.get("ordinal")
        if symbol and ordinal is not None:
            complete_mapping.append({
                "symbol": symbol,
                "ordinal": ordinal,
                "source": "dumpbin_all"
            })
    
    print(f"Created complete mapping with {len(complete_mapping)} entries")
    
    # Check for important symbols
    important_symbols = [
        ("?AfxThrowMemoryException@@YAXXZ", 2350),
        ("?AfxWinInit@@YAHPAUHINSTANCE__@@0PADH@Z", None),
        ("?AfxWinMain@@YAHPAUHINSTANCE__@@0PADH@Z", None),
        ("??0CObject@@QAE@XZ", None),
        ("??1CObject@@UAE@XZ", None),
    ]
    
    print("\n🔍 Checking important symbols:")
    symbol_to_ordinal = {entry["symbol"]: entry["ordinal"] for entry in complete_mapping}
    
    for sym, expected_ordinal in important_symbols:
        if sym in symbol_to_ordinal:
            ordinal = symbol_to_ordinal[sym]
            status = "✅"
            if expected_ordinal and ordinal != expected_ordinal:
                status = f"⚠️ (expected {expected_ordinal})"
            print(f"  {status} {sym}: ordinal {ordinal}")
        else:
            print(f"  ❌ {sym}: NOT FOUND")
    
    # Create database
    db = {
        "exports": {
            "mfc140u": complete_mapping
        },
        "metadata": {
            "total_exports": len(complete_mapping),
            "min_ordinal": min(entry["ordinal"] for entry in complete_mapping),
            "max_ordinal": max(entry["ordinal"] for entry in complete_mapping),
            "has_real_ordinals": True,
            "source": "dumpbin /ALL mfc140u.lib",
            "note": "Complete symbol→ordinal mapping extracted from MFC .lib file"
        }
    }
    
    # Write output
    output_path = Path("mfc_complete_ordinal_mapping.json")
    output_path.write_text(json.dumps(db, indent=2), encoding="ascii")
    
    print(f"\n✅ Wrote complete mapping to {output_path}")
    
    # Show statistics
    ordinals = [entry["ordinal"] for entry in complete_mapping]
    print(f"\n📊 Statistics:")
    print(f"  Total symbols: {len(complete_mapping)}")
    print(f"  Ordinal range: {min(ordinals)} - {max(ordinals)}")
    print(f"  Missing ordinals: {len([o for o in range(256, max(ordinals)+1) if o not in ordinals])}")
    
    # Show first 10 and last 10 entries
    print(f"\n📋 First 10 mappings:")
    for entry in complete_mapping[:10]:
        print(f"  {entry['ordinal']:6d} -> {entry['symbol'][:60]}...")
    
    print(f"\n📋 Last 10 mappings:")
    for entry in complete_mapping[-10:]:
        print(f"  {entry['ordinal']:6d} -> {entry['symbol'][:60]}...")
    
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main())