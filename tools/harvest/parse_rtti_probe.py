#!/usr/bin/env python3
"""Validate and normalize JSON-lines output from rtti_probe.exe."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    parser.add_argument("--dll-version", default="")
    parser.add_argument("--dll-sha256", default="")
    args = parser.parse_args()

    manifest = json.loads(args.manifest.read_text(encoding="utf-8"))
    expected = {target["class"] for target in manifest["targets"]}
    rows: dict[str, dict] = {}
    malformed: list[dict] = []

    for line_number, raw_line in enumerate(
        args.input.read_text(encoding="utf-8-sig").splitlines(), 1
    ):
        line = raw_line.strip()
        if not line:
            continue
        try:
            row = json.loads(line)
        except json.JSONDecodeError as exc:
            malformed.append({"line": line_number, "error": str(exc), "text": line})
            continue
        class_name = row.get("class")
        if not class_name:
            malformed.append(
                {"line": line_number, "error": "missing class", "text": line}
            )
            continue
        if class_name in rows:
            malformed.append(
                {"line": line_number, "error": f"duplicate class {class_name}"}
            )
            continue
        rows[class_name] = row

    missing = sorted(expected - rows.keys())
    unexpected = sorted(rows.keys() - expected)
    probe_errors = {
        name: row.get("error", "invalid descriptor")
        for name, row in rows.items()
        if row.get("error")
        or row.get("sizeof", 0) <= 0
        or row.get("base") in (None, "?")
    }

    normalized_classes = {
        name: {
            "base": rows[name]["base"],
            "schema": rows[name]["schema"],
            "sizeof": rows[name]["sizeof"],
            "get_this_class_ordinal": next(
                target["get_this_class_ordinal"]
                for target in manifest["targets"]
                if target["class"] == name
            ),
            "get_runtime_class_ordinal": next(
                target["get_runtime_class_ordinal"]
                for target in manifest["targets"]
                if target["class"] == name
            ),
        }
        for name in sorted(expected & rows.keys())
        if name not in probe_errors
    }
    output = {
        "metadata": {
            "dll_version": args.dll_version,
            "dll_sha256": args.dll_sha256.lower(),
            "mapping_sha256": manifest["mapping_sha256"],
        },
        "classes": normalized_classes,
    }
    summary = {
        "expected": len(expected),
        "harvested": len(normalized_classes),
        "missing": missing,
        "unexpected": unexpected,
        "probe_errors": probe_errors,
        "malformed": malformed,
    }
    args.output.write_text(json.dumps(output, indent=2, sort_keys=True) + "\n")
    args.summary.write_text(json.dumps(summary, indent=2, sort_keys=True) + "\n")
    print(
        f"RTTI descriptors: {len(normalized_classes)}/{len(expected)} harvested; "
        f"missing={len(missing)} errors={len(probe_errors)} malformed={len(malformed)}"
    )
    return 1 if missing or probe_errors or malformed else 0


if __name__ == "__main__":
    raise SystemExit(main())
