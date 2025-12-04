#!/usr/bin/env python3
"""
Emit a header with static_asserts for sizes and offsets from layouts.json.
Input schema (produced by harvester):
{
  "CWnd": {
    "sizeof": 120,
    "members": {"m_hWnd": 16}
  }
}
"""
import argparse
import json
from pathlib import Path


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--input", required=True, help="layouts.json path")
    ap.add_argument("--out", required=True, help="output header")
    args = ap.parse_args()

    data = json.loads(Path(args.input).read_text())
    lines = ["#pragma once", "#include <cstddef>", "#include <type_traits>", "", "namespace openmfc_layout_asserts {"]

    for cls, info in sorted(data.items()):
        size = info.get("sizeof")
        if size is not None:
            lines.append(f"static_assert(sizeof({cls}) == {size}, \"sizeof({cls}) mismatch\);")
        members = info.get("members", {}) or {}
        for mem, off in sorted(members.items()):
            lines.append(f"static_assert(offsetof({cls}, {mem}) == {off}, \"offsetof({cls}::{mem}) mismatch\);")
        lines.append("")

    lines.append("} // namespace openmfc_layout_asserts")
    Path(args.out).write_text("\n".join(lines), encoding="ascii")


if __name__ == "__main__":
    main()
