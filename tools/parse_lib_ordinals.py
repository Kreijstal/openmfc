#!/usr/bin/env python3
"""
Attempt to parse MFC .lib file to extract symbol→ordinal mapping.

MFC DLL exports by ordinal only. The import library (.lib) contains
import thunks that map symbols to ordinals.

This is a complex task because:
1. .lib files are COFF archives
2. Import thunks contain ordinal references
3. Need to parse COFF/PE binary format

Simpler approach: Create dummy DLL that imports MFC, then analyze its import table.
"""

import argparse
import struct
import json
from pathlib import Path
from typing import Dict, List, Optional


def parse_coff_archive(lib_path: Path) -> List[Dict]:
    """Attempt to parse COFF archive (.lib) file."""
    print(f"Attempting to parse {lib_path}...")
    
    if not lib_path.exists():
        print(f"Error: {lib_path} not found")
        return []
    
    data = lib_path.read_bytes()
    
    # COFF archive starts with "!<arch>\n"
    if not data.startswith(b"!<arch>\n"):
        print("Error: Not a valid COFF archive")
        return []
    
    members = []
    offset = 8  # Skip "!<arch>\n"
    
    try:
        while offset < len(data):
            # Parse archive member header (60 bytes)
            if offset + 60 > len(data):
                break
                
            header = data[offset:offset+60]
            name = header[0:16].decode('ascii', errors='ignore').strip()
            timestamp = header[16:28].decode('ascii', errors='ignore').strip()
            owner_id = header[28:34].decode('ascii', errors='ignore').strip()
            group_id = header[34:40].decode('ascii', errors='ignore').strip()
            mode = header[40:48].decode('ascii', errors='ignore').strip()
            size_str = header[48:58].decode('ascii', errors='ignore').strip()
            
            try:
                size = int(size_str)
            except ValueError:
                break
            
            # Skip header
            offset += 60
            
            # Check for special members
            if name == '/':
                # Symbol table
                print(f"Found symbol table at offset {offset}, size {size}")
            elif name == '//':
                # Long name table
                print(f"Found long name table at offset {offset}, size {size}")
            elif name.startswith('/'):
                # Regular archive member with numeric name
                print(f"Found archive member {name} at offset {offset}, size {size}")
            else:
                # Regular archive member
                print(f"Found archive member '{name}' at offset {offset}, size {size}")
            
            # Skip to next member (align to even boundary)
            offset += size
            if offset % 2 != 0:
                offset += 1
    
    except Exception as e:
        print(f"Error parsing COFF archive: {e}")
    
    return members


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--lib", required=True, help="Path to mfc140u.lib file")
    ap.add_argument("--out", required=True, help="Output JSON path")
    args = ap.parse_args()
    
    lib_path = Path(args.lib)
    
    # Try to parse the .lib file
    members = parse_coff_archive(lib_path)
    
    # For now, create placeholder output
    db = {
        "exports": {
            "mfc140u": []
        },
        "metadata": {
            "note": "Failed to extract real ordinals. Need proper COFF parser.",
            "lib_file": str(lib_path),
            "file_size": lib_path.stat().st_size if lib_path.exists() else 0
        }
    }
    
    Path(args.out).write_text(json.dumps(db, indent=2), encoding="ascii")
    print(f"Wrote placeholder to {args.out}")
    print("ERROR: Need proper COFF parser to extract real ordinals!")


if __name__ == "__main__":
    main()