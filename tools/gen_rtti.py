#!/usr/bin/env python3
"""
Generate RTTI/EH metadata C file and header from harvested exceptions/layouts.
Inputs:
  --exceptions exceptions.json
  --out-c src/generated_rtti.c
  --out-h include/openmfc/eh_rtti.h
"""
import argparse
import json
from pathlib import Path
from typing import Dict, List, Set

HEADER_TEMPLATE = """#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// MSVC RTTI/EH structures
typedef struct TypeDescriptor {
    const void* pVFTable;
    void* spare;
    char name[1]; // Variable length
} TypeDescriptor;

typedef struct PMD {
    int mdisp;
    int pdisp;
    int vdisp;
} PMD;

typedef struct CatchableType {
    unsigned int properties;
    const TypeDescriptor* pType;
    PMD thisDisplacement;
    int sizeOrOffset;
    void (*copyFunction)(void);
} CatchableType;

typedef struct CatchableTypeArray {
    int nCatchableTypes;
    const CatchableType* types[]; // Variable length
} CatchableTypeArray;

typedef struct ThrowInfo {
    unsigned int attributes;
    void (*pmfnUnwind)(void);
    int (*pForwardCompat)(void);
    const CatchableTypeArray* pCatchableTypeArray;
} ThrowInfo;

// Forward declarations
%s

#ifdef __cplusplus
}
#endif
"""

SOURCE_TEMPLATE = """#include "openmfc/eh_rtti.h"
#include <stddef.h>

// Helper macro for section placement
#ifdef _WIN32
#define RDATA __attribute__((section(".rdata")))
#else
#define RDATA __attribute__((section(".data.rel.ro")))
#endif

#ifndef __stdcall
#define __stdcall
#endif

// External copy constructors (thunks)
// These will be implemented in exceptions.c or assembly
extern void __stdcall CException_CopyCtor(void* dest, void* src);
// For now, we might use a generic one or specific ones. 
// The harvester didn't give us the copy function name, so we might default to null or a generic one.
// In a real scenario, we'd need to know which copy ctor to use.
// For this phase, we'll use NULL or a stub.

// TypeDescriptors
%s

// CatchableTypes
%s

// CatchableTypeArrays
%s

// ThrowInfos
%s
"""

def mangle_name_to_sym(name: str) -> str:
    # Simple sanitization for C symbol names
    # .?AVCMemoryException@@ -> TD_CMemoryException
    # This is a heuristic; might need robust demangling or a map.
    # For now, just strip non-alnum and use as ID.
    clean = "".join(c if c.isalnum() else "_" for c in name)
    return clean

def emit_header(exceptions: Dict) -> str:
    decls = []
    for name in exceptions:
        decls.append(f"extern const ThrowInfo TI_{name};")
    return HEADER_TEMPLATE % "\n".join(decls)

def emit_source(exceptions: Dict) -> str:
    # 1. Collect all unique TypeDescriptors
    type_descriptors: Set[str] = set()
    for info in exceptions.values():
        for ct in info.get("catchable_types", []):
            type_descriptors.add(ct["name"])
    
    # Emit TypeDescriptors
    td_lines = []
    td_map = {} # name -> symbol
    for name in sorted(type_descriptors):
        sym = "TD_" + mangle_name_to_sym(name)
        td_map[name] = sym
        # MSVC-compatible struct initialization
        td_lines.append(f"// {name}")
        td_lines.append(f"RDATA static const struct {{")
        td_lines.append(f"    const void* pVFTable;")
        td_lines.append(f"    void* spare;")
        td_lines.append(f"    char name[{len(name)+1}];")
        td_lines.append(f"}} {sym}_struct = {{")
        td_lines.append(f"    0,  /* pVFTable */")
        td_lines.append(f"    0,  /* spare */")
        td_lines.append(f"    \"{name}\"  /* name */")
        td_lines.append(f"}};")
        td_lines.append(f"RDATA const TypeDescriptor* const {sym} = (const TypeDescriptor*)&{sym}_struct;")
        td_lines.append("")

    # Emit CatchableTypes and Arrays
    ct_lines = []
    cta_lines = []
    ti_lines = []

    for name, info in exceptions.items():
        catchables = info.get("catchable_types", [])
        
        # Emit CatchableTypes for this exception
        ct_syms = []
        for idx, ct in enumerate(catchables):
            ct_name = ct["name"]
            ct_sym = f"CT_{name}_{idx}"
            td_sym = td_map[ct_name]
            offset = ct["offset"]
            
            # MSVC-compatible initialization
            ct_lines.append(f"RDATA static const CatchableType {ct_sym} = {{")
            ct_lines.append(f"    0,  /* properties */")
            ct_lines.append(f"    {td_sym},  /* pType */")
            ct_lines.append(f"    {{ {offset}, -1, 0 }},  /* thisDisplacement */")
            ct_lines.append(f"    64,  /* sizeOrOffset */")
            ct_lines.append(f"    0  /* copyFunction */")
            ct_lines.append(f"}};")
            ct_syms.append(ct_sym)
        
        # Emit CatchableTypeArray
        cta_sym = f"CTA_{name}"
        array_size = max(len(ct_syms), 1)  # MSVC doesn't like zero-sized arrays
        cta_lines.append(f"RDATA static const struct {{")
        cta_lines.append(f"    int n;")
        cta_lines.append(f"    const CatchableType* types[{array_size}];")
        cta_lines.append(f"}} {cta_sym}_struct = {{")
        cta_lines.append(f"    {len(ct_syms)},  /* n */")
        if ct_syms:
            cta_lines.append(f"    {{ {', '.join(['&' + s for s in ct_syms])} }}  /* types */")
        else:
            cta_lines.append(f"    {{ 0 }}  /* types (empty, placeholder) */")
        cta_lines.append(f"}};")
        cta_lines.append("")
        
        # Emit ThrowInfo
        ti_sym = f"TI_{name}"
        ti_lines.append(f"RDATA const ThrowInfo {ti_sym} = {{")
        ti_lines.append(f"    0,  /* attributes */")
        ti_lines.append(f"    0,  /* pmfnUnwind */")
        ti_lines.append(f"    0,  /* pForwardCompat */")
        ti_lines.append(f"    (const CatchableTypeArray*)&{cta_sym}_struct  /* pCatchableTypeArray */")
        ti_lines.append(f"}};")
        ti_lines.append("")
    return SOURCE_TEMPLATE % (
        "\n".join(td_lines),
        "\n".join(ct_lines),
        "\n".join(cta_lines),
        "\n".join(ti_lines)
    )

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--exceptions", required=True)
    ap.add_argument("--out-c", required=True)
    ap.add_argument("--out-h", required=True)
    args = ap.parse_args()

    exceptions = json.loads(Path(args.exceptions).read_text())
    Path(args.out_h).write_text(emit_header(exceptions), encoding="ascii")
    Path(args.out_c).write_text(emit_source(exceptions), encoding="ascii")

if __name__ == "__main__":
    main()
