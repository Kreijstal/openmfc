#!/usr/bin/env python3
"""
Generate RTTI/EH metadata C file and header from harvested exceptions/layouts.
Inputs:
  --exceptions exceptions.json
  --out-c src/generated_rtti.c
  --out-h include/openmfc/eh_rtti.h
Assumes TypeDescriptor bytes are embedded as hex strings in throw_info_bytes.
"""
import argparse
import json
from pathlib import Path

HEADER = """#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
struct TypeDescriptor;
struct PMD { int mdisp; int pdisp; int vdisp; };
struct CatchableType {
    unsigned int properties;
    const struct TypeDescriptor* pType;
    struct PMD thisDisplacement;
    int sizeOrOffset;
    void (*copyFunction)(void);
};
struct CatchableTypeArray {
    int nCatchableTypes;
    const struct CatchableType* types[1];
};
struct ThrowInfo {
    unsigned int attributes;
    void (*pmfnUnwind)(void);
    int (*pForwardCompat)(void);
    const struct CatchableTypeArray* pCatchableTypeArray;
};
#ifdef __cplusplus
}
#endif
"""


def emit_header(exceptions):
    lines = [HEADER]
    for name in exceptions:
        lines.append(f"extern const struct ThrowInfo TI_{name};")
    lines.append("")
    return "\n".join(lines)


def emit_source(exceptions):
    lines = ["#include \"openmfc/eh_rtti.h\"", "#include <stddef.h>", ""]
    for name, info in exceptions.items():
        ti_bytes = info.get("throw_info_bytes", "")
        # Store raw ThrowInfo bytes; in a real implementation we would rebuild fields.
        lines.append(f"__attribute__((section(\".rdata\"))) static const unsigned char TI_BYTES_{name}[] = {{")
        chunks = [ti_bytes[i:i+2] for i in range(0, len(ti_bytes), 2)]
        chunk_lines = []
        for i, b in enumerate(chunks):
            sep = "," if i + 1 < len(chunks) else ""
            chunk_lines.append(f"  0x{b}{sep}")
        lines.extend(chunk_lines)
        lines.append("};")
        lines.append(f"const struct ThrowInfo* TI_PTR_{name} = (const struct ThrowInfo*)TI_BYTES_{name};")
        lines.append("")
    return "\n".join(lines)


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
