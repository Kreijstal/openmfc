#!/usr/bin/env python3
"""
MFC ABI Compatibility Result Comparator

Compares JSON results from test_mfc_abi_compat.cpp run against
real MFC vs OpenMFC.

Usage:
    python compare_mfc_results.py real_mfc_results.json openmfc_results.json
"""

import json
import sys
from typing import Dict, List, Any

def load_results(filepath: str) -> Dict[str, Any]:
    """Load JSON results from file."""
    with open(filepath, 'r') as f:
        return json.load(f)

def compare_results(real_mfc: Dict, openmfc: Dict) -> List[Dict]:
    """Compare two result sets, return list of differences."""
    # Build lookup tables
    real_results = {(r['section'], r['name']): r for r in real_mfc['results']}
    open_results = {(r['section'], r['name']): r for r in openmfc['results']}
    
    differences = []
    all_keys = set(real_results.keys()) | set(open_results.keys())
    
    for key in sorted(all_keys):
        section, name = key
        real = real_results.get(key)
        opened = open_results.get(key)
        
        if real is None:
            differences.append({
                'section': section,
                'name': name,
                'issue': 'Missing in Real MFC',
                'openmfc': opened['actual']
            })
        elif opened is None:
            differences.append({
                'section': section,
                'name': name,
                'issue': 'Missing in OpenMFC',
                'real_mfc': real['actual']
            })
        elif real['actual'] != opened['actual']:
            differences.append({
                'section': section,
                'name': name,
                'issue': 'Value mismatch',
                'real_mfc': real['actual'],
                'openmfc': opened['actual']
            })
    
    return differences

def print_report(real_mfc: Dict, openmfc: Dict, differences: List[Dict]):
    """Print comparison report."""
    print("=" * 70)
    print("MFC ABI COMPATIBILITY COMPARISON REPORT")
    print("=" * 70)
    print()
    
    # Summary
    print(f"Real MFC Implementation:")
    print(f"  Total tests:  {real_mfc['total']}")
    print(f"  Passed:       {real_mfc['passed']}")
    print(f"  Failed:       {real_mfc['failed']}")
    print()
    
    print(f"OpenMFC Implementation:")
    print(f"  Total tests:  {openmfc['total']}")
    print(f"  Passed:       {openmfc['passed']}")
    print(f"  Failed:       {openmfc['failed']}")
    print()
    
    # Calculate matches
    total_tests = len(set((r['section'], r['name']) for r in real_mfc['results']) |
                      set((r['section'], r['name']) for r in openmfc['results']))
    matches = total_tests - len(differences)
    
    print("-" * 70)
    print(f"Comparison: {matches}/{total_tests} results match")
    print("-" * 70)
    print()
    
    if differences:
        print("DIFFERENCES FOUND:")
        print()
        
        # Group by section
        by_section = {}
        for d in differences:
            section = d['section']
            if section not in by_section:
                by_section[section] = []
            by_section[section].append(d)
        
        for section in sorted(by_section.keys()):
            print(f"[{section}]")
            for d in by_section[section]:
                print(f"  • {d['name']}")
                print(f"    Issue: {d['issue']}")
                if 'real_mfc' in d:
                    print(f"    Real MFC: {d['real_mfc']}")
                if 'openmfc' in d:
                    print(f"    OpenMFC:  {d['openmfc']}")
            print()
        
        print("=" * 70)
        print(f"RESULT: {len(differences)} DIFFERENCE(S) - OpenMFC NOT FULLY COMPATIBLE")
        print("=" * 70)
        return False
    else:
        print("=" * 70)
        print("RESULT: ALL TESTS MATCH!")
        print("OpenMFC is ABI-compatible with Real MFC for all tested features.")
        print("=" * 70)
        return True

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <real_mfc_results.json> <openmfc_results.json>")
        sys.exit(1)
    
    real_mfc_file = sys.argv[1]
    openmfc_file = sys.argv[2]
    
    try:
        real_mfc = load_results(real_mfc_file)
        openmfc = load_results(openmfc_file)
    except FileNotFoundError as e:
        print(f"Error: {e}")
        sys.exit(1)
    except json.JSONDecodeError as e:
        print(f"Error parsing JSON: {e}")
        sys.exit(1)
    
    differences = compare_results(real_mfc, openmfc)
    success = print_report(real_mfc, openmfc, differences)
    
    sys.exit(0 if success else 1)

if __name__ == '__main__':
    main()
