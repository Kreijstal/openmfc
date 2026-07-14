#!/usr/bin/env python3
"""Merge sharded MSVC layout-harvest summaries and enforce completeness."""

from __future__ import annotations

import argparse
import glob
import json
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--targets", required=True, type=Path)
    parser.add_argument("--runtime-only", type=Path)
    parser.add_argument("--summaries", required=True)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()

    targets = {
        line.strip()
        for line in args.targets.read_text(encoding="utf-8").splitlines()
        if line.strip() and not line.lstrip().startswith("#")
    }
    runtime_only = set()
    if args.runtime_only:
        runtime_only = {
            line.strip()
            for line in args.runtime_only.read_text(encoding="utf-8").splitlines()
            if line.strip() and not line.lstrip().startswith("#")
        }
    unknown_runtime_only = sorted(runtime_only - targets)
    if unknown_runtime_only:
        raise SystemExit(
            "runtime-only classes are absent from the target inventory: "
            + ", ".join(unknown_runtime_only)
        )
    summary_paths = sorted(Path(path) for path in glob.glob(args.summaries, recursive=True))
    if not summary_paths:
        raise SystemExit(f"no summaries matched {args.summaries}")

    successes: set[str] = set()
    failures: dict[str, dict] = {}
    duplicates: set[str] = set()
    shards: list[dict] = []
    seen: set[str] = set()

    for path in summary_paths:
        summary = json.loads(path.read_text(encoding="utf-8-sig"))
        shards.append(
            {
                "path": str(path),
                "shard_index": summary["shard_index"],
                "selected": summary["selected"],
                "succeeded_count": summary["succeeded_count"],
                "failed_count": summary["failed_count"],
            }
        )
        for class_name in summary["succeeded"]:
            if class_name in seen:
                duplicates.add(class_name)
            seen.add(class_name)
            successes.add(class_name)
        for failure in summary["failed"]:
            class_name = failure["class"]
            if class_name in seen:
                duplicates.add(class_name)
            seen.add(class_name)
            failures[class_name] = failure

    missing = sorted(targets - seen)
    unexpected = sorted(seen - targets)
    accepted_runtime_only = sorted(set(failures) & runtime_only)
    unapproved_failures = sorted(set(failures) - runtime_only)
    combined = {
        "expected": len(targets),
        "harvested": len(successes),
        "accounted": len(successes) + len(accepted_runtime_only),
        "failed_count": len(failures),
        "runtime_only_count": len(accepted_runtime_only),
        "runtime_only": accepted_runtime_only,
        "unapproved_failures": unapproved_failures,
        "missing": missing,
        "unexpected": unexpected,
        "duplicates": sorted(duplicates),
        "failures": dict(sorted(failures.items())),
        "shards": sorted(shards, key=lambda shard: shard["shard_index"]),
    }
    args.output.write_text(json.dumps(combined, indent=2, sort_keys=True) + "\n")
    print(
        f"MSVC layouts: {len(successes)}/{len(targets)} harvested; "
        f"runtime-only={len(accepted_runtime_only)} "
        f"unapproved-failures={len(unapproved_failures)} "
        f"missing={len(missing)} duplicates={len(duplicates)}"
    )
    return 1 if unapproved_failures or missing or unexpected or duplicates else 0


if __name__ == "__main__":
    raise SystemExit(main())
